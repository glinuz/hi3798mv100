//-----------------------------------------------------
//  
// Filename: R820C.c   
//
// This file is R820C tuner driver
// Copyright 2011 by Rafaelmicro., Inc.
//
//-----------------------------------------------------


//#include "stdafx.h"
#include "R820C.h"
#include "i2c_sys.h"


#include "drv_i2c_ext.h"
#include "hi_debug.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

#define BOOL HI_BOOL

#if(TUNER_CLK_OUT==TRUE)  //enable tuner clk output for share Xtal application
UINT8 R828_iniArry[27] = {0x83, 0x32, 0x75, 0xC0, 0x40, 0xE6, 0x6C, 0xF5, 0x53,
					/*     0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D                                                    */	   
													  
						   0x75, 0x68, 0x6C, 0x40, 0x80, 0x00, 0x0F, 0x00, 0xC0,//xtal_check
					/*     0x0E  0x0F  0x10  0x11  0x12  0x13  0x14  0x15  0x16                                                    */	   

						   0xB0, 0x48, 0xCC, 0x60, 0x00, 0x54, 0xA6, 0x4A, 0xC0};
					/*     0x17  0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F                                                    */
#else
UINT8 R828_iniArry[27] = {0x83, 0x32, 0x75, 0xC0, 0x40, 0xE6, 0x6C, 0xF5, 0x53,
					/*     0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D                                                    */	   
													  
						   0x75, 0x78, 0x6C, 0x40, 0x80, 0x00, 0x0F, 0x00, 0xC0,//xtal_check
					/*     0x0E  0x0F  0x10  0x11  0x12  0x13  0x14  0x15  0x16                                                    */	   

						   0xB0, 0x48, 0xCC, 0x60, 0x00, 0x54, 0xA6, 0x4A, 0xC0};
					/*     0x17  0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F                                                    */
#endif
UINT8 R828_ExtraArry[5] = {0x00, 0x31, 0xB8, 0x00, 0x00};  //R0~R4
UINT8 R828_ADDRESS=0x34;
UINT8 Rafael_Chip = R820C;
UINT8 USE_DIPLEXER=FALSE;
UINT8 FILTER_ATT=9;
static UINT32 R828_FiltIF_Khz = 8460;
//----------------------------------------------------------//
//                   Internal Structs                       //
//----------------------------------------------------------//
typedef struct _R828_SectType
{
	UINT8 Phase_Y;
	UINT8 Gain_X;
	UINT16 Value;
}R828_SectType;

typedef enum _BW_Type
{
	BW_6M = 0,
	BW_7M,
	BW_8M,
	BW_1_7M,
	BW_10M,
	BW_200K
}BW_Type;

typedef struct _Sys_Info_Type
{
	UINT16		IF_KHz;
	BW_Type		BW;
	UINT32		FILT_CAL_LO;
	UINT8		FILT_GAIN;
	UINT8		IMG_R;
	UINT8		FILT_Q;
	UINT8		HP_COR;
	UINT8       EXT_ENABLE;
	UINT8       LOOP_THROUGH;
	UINT8       LT_ATT;
	UINT8       FLT_EXT_WIDEST;
	UINT8       POLYFIL_CUR;
}Sys_Info_Type;

typedef struct _Freq_Info_Type
{
	UINT8		OPEN_D;
	UINT8		RF_MUX_PLOY;
	UINT8		TF_C;
	UINT8		XTAL_CAP20P;	
	UINT8		XTAL_CAP10P;	
	UINT8		XTAL_CAP0P;	
	UINT8		IMR_MEM;
}Freq_Info_Type;


typedef struct _SysFreq_Info_Type
{
	UINT8		LNA_TOP;
	UINT8		LNA_VTH_L;
	UINT8		MIXER_TOP;
	UINT8		MIXER_VTH_L;
	UINT8      AIR_CABLE1_IN;
	UINT8      CABLE2_IN;
	UINT8		PRE_DECT;
	UINT8      LNA_DISCHARGE;
	UINT8      CP_CUR;
	UINT8      DIV_BUF_CUR;
    UINT8      FILTER_CUR;
}SysFreq_Info_Type;

//----------------------------------------------------------//
//                   Internal Parameters                    //
//----------------------------------------------------------//
enum XTAL_CAP_VALUE
{
	XTAL_LOW_CAP_30P = 0,
    XTAL_LOW_CAP_20P,
    XTAL_LOW_CAP_10P,
    XTAL_LOW_CAP_0P,
	XTAL_HIGH_CAP_0P
};
UINT8 R828_Arry[27];
R828_SectType IMR_Data[5] = {
                                                  {0, 0, 0},
                                                  {0, 0, 0},
                                                  {0, 0, 0},
                                                  {0, 0, 0},
                                                  {0, 0, 0}
                                                };//Please keep this array data for standby mode.
I2C_TYPE  R828_I2C;
I2C_LEN_TYPE R828_I2C_Len;

UINT32 R828_IF_khz;
UINT32 R828_CAL_LO_khz;
UINT8  R828_IMR_point_num;
UINT8  R828_IMR_done_flag = FALSE;
UINT8  R828_Bandwidth = 0x00;
UINT8  R828_Fil_Cal_flag[STD_SIZE];
static UINT8 R828_Fil_Cal_BW[STD_SIZE];
static UINT8 R828_Fil_Cal_code[STD_SIZE];

static UINT8 Xtal_cap_sel = XTAL_LOW_CAP_0P;
static UINT8 Xtal_cap_sel_tmp = XTAL_LOW_CAP_0P;
static R828_Standard_Type R828_pre_standard = STD_SIZE;
//----------------------------------------------------------//
//                   Internal static struct                 //
//----------------------------------------------------------//
static SysFreq_Info_Type SysFreq_Info1;
static Sys_Info_Type Sys_Info1;
//static Freq_Info_Type R828_Freq_Info;
static Freq_Info_Type Freq_Info1;
//----------------------------------------------------------//
//                   Internal Functions                     //
//----------------------------------------------------------//
R828_ErrCode R828_Xtal_Check(void);
R828_ErrCode R828_InitReg(void);
R828_ErrCode R828_IMR_Prepare(void);
R828_ErrCode R828_IMR(UINT8 IMR_MEM, BOOL IM_Flag);
R828_ErrCode R828_PLL(UINT32 LO_Freq, R828_Standard_Type R828_Standard);
R828_ErrCode R828_MUX(UINT32 RF_KHz);
R828_ErrCode R828_IQ(R828_SectType* IQ_Pont);
R828_ErrCode R828_IQ_Tree(UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R828_SectType* CompareTree);
R828_ErrCode R828_CompreCor(R828_SectType* CorArry);
R828_ErrCode R828_CompreStep(R828_SectType* StepArry, UINT8 Pace);
R828_ErrCode R828_Muti_Read(UINT8 IMR_Reg, UINT16* IMR_Result_Data);
R828_ErrCode R828_Section(R828_SectType* SectionArry);
R828_ErrCode R828_F_IMR(R828_SectType* IQ_Pont);
R828_ErrCode R828_IMR_Cross(R828_SectType* IQ_Pont, UINT8* X_Direct);

Sys_Info_Type R828_Sys_Sel(R828_Standard_Type R828_Standard);
Freq_Info_Type R828_Freq_Sel(UINT32 RF_freq);
SysFreq_Info_Type R828_SysFreq_Sel(R828_Standard_Type R828_Standard,UINT32 RF_freq);

R828_ErrCode R828_Filt_Cal(UINT32 Cal_Freq,BW_Type R828_BW);
UINT8  R828_Filt_Cal_ADC(UINT32 IF_Freq, UINT8 R828_BW);
//R828_ErrCode R828_SetFrequency(R828_Set_Info R828_INFO, R828_SetFreq_Type R828_SetFreqMode);



Sys_Info_Type R828_Sys_Sel(R828_Standard_Type R828_Standard)
{
	Sys_Info_Type R828_Sys_Info;

	switch (R828_Standard)
	{

	case DVB_C_8M:  
		R828_Sys_Info.IF_KHz=5070;
		R828_Sys_Info.BW=BW_8M;
		R828_Sys_Info.FILT_CAL_LO=73500;
		R828_Sys_Info.FILT_GAIN=0x30;  //+3dB on, 6MHz on
		R828_Sys_Info.IMG_R=0x00;		//image negative
		R828_Sys_Info.FILT_Q=0x00;		//R10[4]:low Q(1'b0) //
		R828_Sys_Info.HP_COR=0x0B;		// 1.7M disable, +0cap, 1MHz		
		R828_Sys_Info.EXT_ENABLE=0x40;  //R30[6], ext enable; R30[5]:0 ext at LNA max
		R828_Sys_Info.LOOP_THROUGH=0x00; //R5[7], LT ON          //always ON
		R828_Sys_Info.LT_ATT=0x80;       //R31[7], LT ATT disable 
		R828_Sys_Info.FLT_EXT_WIDEST=0x00;//R15[7]: FLT_EXT_WIDE OFF
		R828_Sys_Info.POLYFIL_CUR=0x60;  //R25[6:5]:Min
		R828_FiltIF_Khz = 8750;  //8880-
		break;

	case DVB_C_6M_4063:
        R828_Sys_Info.IF_KHz=4063;
		R828_Sys_Info.BW=BW_6M;
		R828_Sys_Info.FILT_CAL_LO=55000;
		R828_Sys_Info.FILT_GAIN=0x30;  //+3dB on, 6MHz on
		R828_Sys_Info.IMG_R=0x00;		//image negative
		R828_Sys_Info.FILT_Q=0x00;		//R10[4]:low Q(1'b0) //
		R828_Sys_Info.HP_COR=0x6A;		// 1.7M disable, +2cap, 1.25MHz		
		R828_Sys_Info.EXT_ENABLE=0x40;  //R30[6], ext enable; R30[5]:0 ext at LNA max 
		R828_Sys_Info.LOOP_THROUGH=0x00; //R5[7], LT ON           //always ON
		R828_Sys_Info.LT_ATT=0x80;       //R31[7], LT ATT disable 
		R828_Sys_Info.FLT_EXT_WIDEST=0x00;//R15[7]: FLT_EXT_WIDE OFF
		R828_Sys_Info.POLYFIL_CUR=0x60;  //R25[6:5]:Min
		R828_FiltIF_Khz = 6475; //6540-
		break;

	case DVB_C_6M_5070:  
		R828_Sys_Info.IF_KHz=5070;
		R828_Sys_Info.BW=BW_6M;
		R828_Sys_Info.FILT_CAL_LO=73000;
		R828_Sys_Info.FILT_GAIN=0x30;  //+3dB on, 6MHz on
		R828_Sys_Info.IMG_R=0x00;		//image negative
		R828_Sys_Info.FILT_Q=0x00;		//R10[4]:low Q(1'b0) //
		R828_Sys_Info.HP_COR=0x06;		// 1.7M disable, +0cap, -8@1.75MHz		
		R828_Sys_Info.EXT_ENABLE=0x00;  //R30[6], ext disable; R30[5]:0 ext at LNA max 
		R828_Sys_Info.LOOP_THROUGH=0x00; //R5[7], LT ON           //always ON
		R828_Sys_Info.LT_ATT=0x80;       //R31[7], LT ATT disable 
		R828_Sys_Info.FLT_EXT_WIDEST=0x00;//R15[7]: FLT_EXT_WIDE OFF
		R828_Sys_Info.POLYFIL_CUR=0x60;  //R25[6:5]:Min
		R828_FiltIF_Khz = 8660; //8700+
		break;

	case J83B:  
		R828_Sys_Info.IF_KHz=5070;
		R828_Sys_Info.BW=BW_6M;
		R828_Sys_Info.FILT_CAL_LO=70000;
		R828_Sys_Info.FILT_GAIN=0x30;  //+3dB on, 6MHz on
		R828_Sys_Info.IMG_R=0x00;		//image negative
		R828_Sys_Info.FILT_Q=0x00;		//R10[4]:low Q(1'b0) //
		R828_Sys_Info.HP_COR=0x24;		// 1.7M disable, +1cap, -4@2.25MHz		
		R828_Sys_Info.EXT_ENABLE=0x00;  //R30[6], ext disable; R30[5]:0 ext at LNA max 
		R828_Sys_Info.LOOP_THROUGH=0x00; //R5[7], LT ON           //always ON
		R828_Sys_Info.LT_ATT=0x80;       //R31[7], LT ATT disable 
		R828_Sys_Info.FLT_EXT_WIDEST=0x00;//R15[7]: FLT_EXT_WIDE OFF
		R828_Sys_Info.POLYFIL_CUR=0x60;  //R25[6:5]:Min
		R828_FiltIF_Khz = 7900;
		break;



	default:  //DVB_C_8M
		R828_Sys_Info.IF_KHz=5070;
		R828_Sys_Info.BW=BW_8M;
		R828_Sys_Info.FILT_CAL_LO=73500;
		R828_Sys_Info.FILT_GAIN=0x30;  //+3dB on, 6MHz on
		R828_Sys_Info.IMG_R=0x00;		//image negative
		R828_Sys_Info.FILT_Q=0x00;		//R10[4]:low Q(1'b0) //
		R828_Sys_Info.HP_COR=0x0B;		// 1.7M disable, +0cap, 1MHz		
		R828_Sys_Info.EXT_ENABLE=0x40;  //R30[6], ext enable; R30[5]:0 ext at LNA max
		R828_Sys_Info.LOOP_THROUGH=0x00; //R5[7], LT ON          //always ON
		R828_Sys_Info.LT_ATT=0x80;       //R31[7], LT ATT disable 
		R828_Sys_Info.FLT_EXT_WIDEST=0x00;//R15[7]: FLT_EXT_WIDE OFF
		R828_Sys_Info.POLYFIL_CUR=0x60;  //R25[6:5]:Min
		R828_FiltIF_Khz = 8750;  //8880-
		break;

	}

	return R828_Sys_Info;
}



Freq_Info_Type R828_Freq_Sel(UINT32 LO_freq)
{
	Freq_Info_Type R828_Freq_Info;

	if((LO_freq>=0) && (LO_freq<50000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0xDF;     //R27[7:0]  band2,band0
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}

	else if((LO_freq>=50000) && (LO_freq<55000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0xBE;     //R27[7:0]  band4,band1 
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=55000) && (LO_freq<60000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x8B;     //R27[7:0]  band7,band4
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}	
	else if(( LO_freq>=60000) && (LO_freq<65000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x7B;     //R27[7:0]  band8,band4
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=65000) && (LO_freq<70000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x69;     //R27[7:0]  band9,band6
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}	
	else if(( LO_freq>=70000) && (LO_freq<75000))
	{
		R828_Freq_Info.OPEN_D=0x08; // low
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x58;     //R27[7:0]  band10,band7
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=75000) && (LO_freq<80000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x44;     //R27[7:0]  band11,band11
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=80000) && (LO_freq<90000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x44;     //R27[7:0]  band11,band11
		R828_Freq_Info.XTAL_CAP20P=0x02;  //R16[1:0]  20pF (10)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=90000) && (LO_freq<100000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x34;     //R27[7:0]  band12,band11
		R828_Freq_Info.XTAL_CAP20P=0x01;  //R16[1:0]  10pF (01)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=100000) && (LO_freq<110000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x34;     //R27[7:0]  band12,band11
		R828_Freq_Info.XTAL_CAP20P=0x01;  //R16[1:0]  10pF (01)   
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 0;
	}
	else if(( LO_freq>=110000) && (LO_freq<120000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x24;     //R27[7:0]  band13,band11
		R828_Freq_Info.XTAL_CAP20P=0x01;  //R16[1:0]  10pF (01)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 1;
	}
	else if(( LO_freq>=120000) && (LO_freq<140000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x24;     //R27[7:0]  band13,band11
		R828_Freq_Info.XTAL_CAP20P=0x01;  //R16[1:0]  10pF (01)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 1;
	}
	else if(( LO_freq>=140000) && (LO_freq<180000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x14;     //R27[7:0]  band14,band11
		R828_Freq_Info.XTAL_CAP20P=0x01;  //R16[1:0]  10pF (01)  
		R828_Freq_Info.XTAL_CAP10P=0x01; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 1;
	}
	else if(( LO_freq>=180000) && (LO_freq<220000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x13;     //R27[7:0]  band14,band12
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 1;
	}
	else if(( LO_freq>=220000) && (LO_freq<250000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x13;     //R27[7:0]  band14,band12
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 2;
	}
	else if(( LO_freq>=250000) && (LO_freq<280000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x11;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 2;
	}
	else if(( LO_freq>=280000) && (LO_freq<310000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x02;  //R26[7:6]=0 (LPF)  R26[1:0]=2 (low)
		R828_Freq_Info.TF_C=0x00;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 2;
	}
	else if(( LO_freq>=310000) && (LO_freq<450000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x41;  //R26[7:6]=1 (bypass)  R26[1:0]=1 (middle)
		R828_Freq_Info.TF_C=0x00;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 2;
	}
	else if(( LO_freq>=450000) && (LO_freq<588000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x41;  //R26[7:6]=1 (bypass)  R26[1:0]=1 (middle)
		R828_Freq_Info.TF_C=0x00;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 3;
	}
	else if(( LO_freq>=588000) && (LO_freq<650000))
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x40;  //R26[7:6]=1 (bypass)  R26[1:0]=0 (highest)
		R828_Freq_Info.TF_C=0x00;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 3;
	}
	else
	{
		R828_Freq_Info.OPEN_D=0x00; // high
		R828_Freq_Info.RF_MUX_PLOY = 0x40;  //R26[7:6]=1 (bypass)  R26[1:0]=0 (highest)
		R828_Freq_Info.TF_C=0x00;     //R27[7:0]  highest,highest
		R828_Freq_Info.XTAL_CAP20P=0x00;  //R16[1:0]  0pF (00)  
		R828_Freq_Info.XTAL_CAP10P=0x00; 
		R828_Freq_Info.XTAL_CAP0P=0x00;
		R828_Freq_Info.IMR_MEM = 4;
	}

	return R828_Freq_Info;
}



SysFreq_Info_Type R828_SysFreq_Sel(R828_Standard_Type R828_Standard,UINT32 RF_freq)
{
	SysFreq_Info_Type R828_SysFreq_Info;
	
	switch(R828_Standard)
	{

	case DVB_C_8M:
	case DVB_C_6M_4063:	
	case DVB_C_6M_5070:	
	case J83B:
			R828_SysFreq_Info.MIXER_TOP=0x44;	// MIXER TOP:11 , TOP-1, low-discharge
			R828_SysFreq_Info.LNA_TOP=0xCD;		// Detect BW 3, LNA TOP:1, PreDet Top:2
			R828_SysFreq_Info.LNA_VTH_L=0x62;		// LNA VTH 0.94	,  VTL 0.54
			R828_SysFreq_Info.MIXER_VTH_L=0x75;	// MIXER VTH 1.04, VTL 0.84
            R828_SysFreq_Info.AIR_CABLE1_IN=0x00;
			R828_SysFreq_Info.CABLE2_IN=0x00;
			R828_SysFreq_Info.PRE_DECT=0x40;
			R828_SysFreq_Info.LNA_DISCHARGE=14;
			R828_SysFreq_Info.CP_CUR=0x38;            // 111, auto
			R828_SysFreq_Info.DIV_BUF_CUR=0x30; // 11, 150u
			R828_SysFreq_Info.FILTER_CUR=0x40;    // 10, low
		break;


	default: //DVB-C
			R828_SysFreq_Info.MIXER_TOP=0x44;	// MIXER TOP:11 , TOP-1, low-discharge
			R828_SysFreq_Info.LNA_TOP=0xCD;		// Detect BW 3, LNA TOP:1, PreDet Top:2
			R828_SysFreq_Info.LNA_VTH_L=0x62;		// LNA VTH 0.94	,  VTL 0.54
			R828_SysFreq_Info.MIXER_VTH_L=0x75;	// MIXER VTH 1.04, VTL 0.84
            R828_SysFreq_Info.AIR_CABLE1_IN=0x00;
			R828_SysFreq_Info.CABLE2_IN=0x00;
			R828_SysFreq_Info.PRE_DECT=0x40;
			R828_SysFreq_Info.LNA_DISCHARGE=14;
			R828_SysFreq_Info.CP_CUR=0x38;            // 111, auto
			R828_SysFreq_Info.DIV_BUF_CUR=0x30; // 11, 150u
			R828_SysFreq_Info.FILTER_CUR=0x40;    // 10, low
		break;
	
	} //end switch

//DTV use Diplexer
if(USE_DIPLEXER==TRUE)
{
		if ((Rafael_Chip==R820C) || (Rafael_Chip==R820T) || (Rafael_Chip==R828S))
		{
			// Air-in (>=DIP_FREQ) & cable-1(<DIP_FREQ) 
				if(RF_freq >= DIP_FREQ)
				{
					R828_SysFreq_Info.AIR_CABLE1_IN = 0x00; //air in, cable-1 off
					R828_SysFreq_Info.CABLE2_IN = 0x00;     //cable-2 off
				}
				else
				{
					R828_SysFreq_Info.AIR_CABLE1_IN = 0x60; //cable-1 in, air off
					R828_SysFreq_Info.CABLE2_IN = 0x00;     //cable-2 off
				}
		}
}

	return R828_SysFreq_Info;
	
	}




R828_ErrCode R828_Xtal_Check(void)
{

	UINT8 ArrayNum = 27;
	for(ArrayNum=0;ArrayNum<27;ArrayNum++)
    {
          R828_Arry[ArrayNum] = R828_iniArry[ArrayNum];
    }

	//cap 30pF & Drive Low
	R828_I2C.RegAddr = 0x10;
	R828_Arry[11]    = (R828_Arry[11] & 0xF4) | 0x0B ;
	R828_I2C.Data    = R828_Arry[11];
	if(I2C_Write(&R828_I2C) != RT_Success)
	    return RT_Fail;

	//set pll autotune = 128kHz
	R828_I2C.RegAddr = 0x1A;
	R828_Arry[21]    = R828_Arry[21] & 0xF3;
	R828_I2C.Data    = R828_Arry[21];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//set manual initial reg = 111111; 
	R828_I2C.RegAddr = 0x13;
	R828_Arry[14]    = (R828_Arry[14] & 0x80) | 0x7F;
	R828_I2C.Data    = R828_Arry[14];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//set auto
	R828_I2C.RegAddr = 0x13;
	R828_Arry[14]    = (R828_Arry[14] & 0xBF);
	R828_I2C.Data    = R828_Arry[14];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	
	R828_Delay_MS(5);

	R828_I2C_Len.RegAddr = 0x00;
	R828_I2C_Len.Len     = 3;
	if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		return RT_Fail;

	// if 30pF unlock, set to cap 20pF
#if (USE_16M_XTAL==TRUE)
	//VCO=2360MHz for 16M Xtal. VCO band 26
    if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) > 29) || ((R828_I2C_Len.Data[2] & 0x3F) < 23)) 
#else
	if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) == 0x3F)) 
#endif
	{
		//cap 20pF 
	    R828_I2C.RegAddr = 0x10;
	    R828_Arry[11]    = (R828_Arry[11] & 0xFC) | 0x02;
	    R828_I2C.Data    = R828_Arry[11];
	    if(I2C_Write(&R828_I2C) != RT_Success)
		    return RT_Fail;

		R828_Delay_MS(5);
	
		R828_I2C_Len.RegAddr = 0x00;
		R828_I2C_Len.Len     = 3;
		if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		    return RT_Fail;

		// if 20pF unlock, set to cap 10pF
#if (USE_16M_XTAL==TRUE)
        if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) > 29) || ((R828_I2C_Len.Data[2] & 0x3F) < 23)) 
#else
	    if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) == 0x3F)) 
#endif
	   {
		   //cap 10pF 
	       R828_I2C.RegAddr = 0x10;
	       R828_Arry[11]    = (R828_Arry[11] & 0xFC) | 0x01;
	       R828_I2C.Data    = R828_Arry[11];
	       if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;

		   R828_Delay_MS(5);
	
		   R828_I2C_Len.RegAddr = 0x00;
		   R828_I2C_Len.Len     = 3;
		   if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		       return RT_Fail;

		   // if 10pF unlock, set to cap 0pF
#if (USE_16M_XTAL==TRUE)
           if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) > 29) || ((R828_I2C_Len.Data[2] & 0x3F) < 23)) 
#else
	       if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) == 0x3F)) 
#endif 
	      {
		      //cap 0pF 
	          R828_I2C.RegAddr = 0x10;
	          R828_Arry[11]    = (R828_Arry[11] & 0xFC) | 0x00;
	          R828_I2C.Data    = R828_Arry[11];
	          if(I2C_Write(&R828_I2C) != RT_Success)
		          return RT_Fail;
		
		      R828_Delay_MS(5);
	
		      R828_I2C_Len.RegAddr = 0x00;
		      R828_I2C_Len.Len     = 3;
		      if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		           return RT_Fail;
	
		      // if unlock, set to high drive
#if (USE_16M_XTAL==TRUE)
               if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) > 29) || ((R828_I2C_Len.Data[2] & 0x3F) < 23)) 
#else
	           if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) == 0x3F)) 
#endif 
			  {
				   //X'tal drive high
	               R828_I2C.RegAddr = 0x10;
	               R828_Arry[11]    = (R828_Arry[11] & 0xF7) ;
	               R828_I2C.Data    = R828_Arry[11];
	               if(I2C_Write(&R828_I2C) != RT_Success)
                          return RT_Fail;

				   //R828_Delay_MS(15);
				   R828_Delay_MS(20);
	
		           R828_I2C_Len.RegAddr = 0x00;
		           R828_I2C_Len.Len     = 3;
		           if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		                  return RT_Fail;

#if (USE_16M_XTAL==TRUE)
                   if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) > 29) || ((R828_I2C_Len.Data[2] & 0x3F) < 23)) 
#else
	               if(((R828_I2C_Len.Data[2] & 0x40) == 0x00) || ((R828_I2C_Len.Data[2] & 0x3F) == 0x3F)) 
#endif
				   {
			             return RT_Fail;
				   }
				   else //0p+high drive lock
				   {
						Xtal_cap_sel_tmp = XTAL_HIGH_CAP_0P;
				   }
			  }
		      else //0p lock
			  {
				   Xtal_cap_sel_tmp = XTAL_LOW_CAP_0P;
			  }
		   }
		   else //10p lock
		   {   
			   Xtal_cap_sel_tmp = XTAL_LOW_CAP_10P;
		   }
		}
		else //20p lock
		{
		   Xtal_cap_sel_tmp = XTAL_LOW_CAP_20P;
		}
	}
	else // 30p lock
	{
		Xtal_cap_sel_tmp = XTAL_LOW_CAP_30P;
	}

    return RT_Success;
}	
R828_ErrCode R828_Init(void)
{
    UINT8 i;

    //write initial reg
	//if(R828_InitReg() != RT_Success)         
	//	return RT_Fail;

	if(R828_IMR_done_flag==FALSE)
	{

	  //write initial reg
	  if(R828_InitReg() != RT_Success)        
		  return RT_Fail;

	  //Do Xtal check
	  if((Rafael_Chip==R820T) || (Rafael_Chip==R828S) || (Rafael_Chip==R820C))
	  {	 
		  Xtal_cap_sel = XTAL_HIGH_CAP_0P;
	  }
	  else
	  {
          if(R828_Xtal_Check() != RT_Success)        //1st
	          return RT_Fail;

		  Xtal_cap_sel = Xtal_cap_sel_tmp;
		  
		  if(R828_Xtal_Check() != RT_Success)        //2nd
	          return RT_Fail;
		  
		  if(Xtal_cap_sel_tmp > Xtal_cap_sel)
		  {
			  Xtal_cap_sel = Xtal_cap_sel_tmp;
		  }

		  if(R828_Xtal_Check() != RT_Success)        //3rd
	          return RT_Fail;
		  
		  if(Xtal_cap_sel_tmp > Xtal_cap_sel)
		  {
		      Xtal_cap_sel = Xtal_cap_sel_tmp;
		  }

	  }

	  //reset filter cal.
      for (i=0; i<STD_SIZE; i++)
	  {	  
		  R828_Fil_Cal_flag[i] = FALSE;
		  R828_Fil_Cal_code[i] = 0;
		  R828_Fil_Cal_BW[i] = 0x00;
	  }

	  //start imr cal.
	  if(R828_InitReg() != RT_Success)        //write initial reg before doing cal
		return RT_Fail;

	  if(R828_IMR_Prepare() != RT_Success)      
	      return RT_Fail;

	  if(R828_IMR(3, TRUE) != RT_Success)       //Full K node 3
		return RT_Fail;

	  if(R828_IMR(1, FALSE) != RT_Success)
		return RT_Fail;

	  if(R828_IMR(0, FALSE) != RT_Success)
		return RT_Fail;

	  if(R828_IMR(2, FALSE) != RT_Success)
		return RT_Fail;

	  if(R828_IMR(4, TRUE) != RT_Success)   //Full K node 4
		return RT_Fail;

	  R828_IMR_done_flag = TRUE;
	}

	//write initial reg
	if(R828_InitReg() != RT_Success)        
		return RT_Fail;

	R828_pre_standard = STD_SIZE;

	return RT_Success;
}



R828_ErrCode R828_InitReg(void)
{
	UINT8 InitArryCunt = 0;
	UINT8 InitArryNum  = 27;
	
	//Write Full Table
	R828_I2C_Len.RegAddr = 0x05;
	R828_I2C_Len.Len     = InitArryNum;
	for(InitArryCunt = 0;InitArryCunt < InitArryNum;InitArryCunt ++)
	{
		R828_I2C_Len.Data[InitArryCunt] = R828_iniArry[InitArryCunt];
	}
	if(I2C_Write_Len(&R828_I2C_Len) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R828_ErrCode R828_IMR_Prepare(void)

{
     UINT8 ArrayNum=27;
     for(ArrayNum=0;ArrayNum<27;ArrayNum++)
     {
           R828_Arry[ArrayNum] = R828_iniArry[ArrayNum];
     }
     //IMR Preparation    
     //lna off (air-in off)
     R828_I2C.RegAddr = 0x05;
     R828_Arry[0]     = R828_Arry[0]  | 0x20;
     R828_I2C.Data    = R828_Arry[0];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail; 
     //mixer gain mode = manual
     R828_I2C.RegAddr = 0x07;
     R828_Arry[2]     = (R828_Arry[2] & 0xEF);
     R828_I2C.Data    = R828_Arry[2];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //filter corner = lowest
     R828_I2C.RegAddr = 0x0A;
     R828_Arry[5]     = R828_Arry[5] | 0x0F;
     R828_I2C.Data    = R828_Arry[5];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //filter bw=+2cap, hp=5M
     R828_I2C.RegAddr = 0x0B;
     R828_Arry[6]    = (R828_Arry[6] & 0x90) | 0x60;
     R828_I2C.Data    = R828_Arry[6];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //adc=on, vga code mode, gain = 26.5dB  
     R828_I2C.RegAddr = 0x0C;
     R828_Arry[7]    = (R828_Arry[7] & 0x60) | 0x0B;
     R828_I2C.Data    = R828_Arry[7];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //ring clk = on
     R828_I2C.RegAddr = 0x0F;
     R828_Arry[10]   &= 0xF7;
     R828_I2C.Data    = R828_Arry[10];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //ring power = on
     R828_I2C.RegAddr = 0x18;
     R828_Arry[19]    = R828_Arry[19] | 0x10;
     R828_I2C.Data    = R828_Arry[19];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //from ring = ring pll in
     R828_I2C.RegAddr = 0x1C;
     R828_Arry[23]    = R828_Arry[23] | 0x02;
     R828_I2C.Data    = R828_Arry[23];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
     //sw_pdect = det3
     R828_I2C.RegAddr = 0x1E;
     R828_Arry[25]    = R828_Arry[25] | 0x80;
     R828_I2C.Data    = R828_Arry[25];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
	// Set filt_3dB
	//R828_Arry[1]  = R828_Arry[1] | 0x20;  
	R828_Arry[1]  = R828_Arry[1] & 0xDF;  
	R828_I2C.RegAddr  = 0x06;
	R828_I2C.Data     = R828_Arry[1];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

      return RT_Success;
}









R828_ErrCode R828_IMR(UINT8 IMR_MEM, BOOL IM_Flag)
{

	UINT32 RingVCO = 0;
	UINT32 RingFreq = 0;
	UINT32 RingRef = 0;
	UINT8 n_ring = 0;
	UINT8 n;

	R828_SectType IMR_POINT;


	if (R828_Xtal>24000)
		RingRef = R828_Xtal /2;
	else
		RingRef = R828_Xtal;

	for(n=0;n<16;n++)
	{
		if((16+n)* 8 * RingRef >= 3100000) 
		{
		n_ring=n;
		break;
		}

		if(n==15)   //n_ring not found
		{
            //return RT_Fail;
			n_ring=n;
		}

	}

	R828_Arry[19] &= 0xF0;      //set ring[3:0]
	R828_Arry[19] |= n_ring;
	RingVCO = (16+n_ring)* 8 * RingRef;
	R828_Arry[19]&=0xDF;   //clear ring_se23	
	R828_Arry[20]&=0xFC;   //clear ring_seldiv	
	R828_Arry[26]&=0xFC;   //clear ring_att

	switch(IMR_MEM)
	{
	case 0:
		RingFreq = RingVCO/48;
		R828_Arry[19]|=0x20;  // ring_se23 = 1
		R828_Arry[20]|=0x03;  // ring_seldiv = 3		
		R828_Arry[26]|=0x02;  // ring_att 10
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	case 1:
		RingFreq = RingVCO/16;
		R828_Arry[19]|=0x00;  // ring_se23 = 0
		R828_Arry[20]|=0x02;  // ring_seldiv = 2		
		R828_Arry[26]|=0x00;  // pw_ring 00
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	case 2:
		RingFreq = RingVCO/8;
		R828_Arry[19]|=0x00;  // ring_se23 = 0
		R828_Arry[20]|=0x01;  // ring_seldiv = 1		
		R828_Arry[26]|=0x03;  // pw_ring 11
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	case 3:
		RingFreq = RingVCO/6;
		R828_Arry[19]|=0x20;  // ring_se23 = 1
		R828_Arry[20]|=0x00;  // ring_seldiv = 0		
		R828_Arry[26]|=0x03;  // pw_ring 11
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	case 4:
		RingFreq = RingVCO/4;
		R828_Arry[19]|=0x00;  // ring_se23 = 0
		R828_Arry[20]|=0x00;  // ring_seldiv = 0		
		R828_Arry[26]|=0x00;  // pw_ring: min
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	default:
		RingFreq = RingVCO/4;
		R828_Arry[19]|=0x00;  // ring_se23 = 0
		R828_Arry[20]|=0x00;  // ring_seldiv = 0		
		R828_Arry[26]|=0x00;  // pw_ring: min
		R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x0A;
		break;
	}

    // Set Mixer Gain
     R828_I2C.RegAddr = 0x07;
     R828_I2C.Data    = R828_Arry[2];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;

	//write pw_ring,n_ring,ringdiv2 to I2C

	//------------n_ring,ring_se23----------//
	R828_I2C.RegAddr = 0x18;
	R828_I2C.Data    = R828_Arry[19];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	//------------ring_sediv----------------//
	R828_I2C.RegAddr = 0x19;
	R828_I2C.Data    = R828_Arry[20];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	//------------pw_ring-------------------//
	R828_I2C.RegAddr = 0x1f;
	R828_I2C.Data    = R828_Arry[26];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	
	//Must do before PLL() 
	if(R828_MUX(RingFreq - 5300) != RT_Success)				//MUX input freq ~ RF_in Freq
		return RT_Fail;

	if(R828_PLL((RingFreq - 5300), STD_SIZE) != RT_Success)                //set pll freq = ring freq - 6M
	    return RT_Fail;

	if(IM_Flag == TRUE)
	{
	if(R828_IQ(&IMR_POINT) != RT_Success)
		return RT_Fail;
	}
	else
	{
		IMR_POINT.Gain_X = IMR_Data[3].Gain_X;
		IMR_POINT.Phase_Y = IMR_Data[3].Phase_Y;
		IMR_POINT.Value = IMR_Data[3].Value;
		if(R828_F_IMR(&IMR_POINT) != RT_Success)
			return RT_Fail;
	}

	//Save IMR Value
	switch(IMR_MEM)
	{
	case 0:
		IMR_Data[0].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[0].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[0].Value   = IMR_POINT.Value;
		break;
	case 1:
		IMR_Data[1].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[1].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[1].Value   = IMR_POINT.Value;
		break;
	case 2:
		IMR_Data[2].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[2].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[2].Value   = IMR_POINT.Value;
		break;
	case 3:
		IMR_Data[3].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[3].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[3].Value   = IMR_POINT.Value;
		break;
	case 4:
		IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[4].Value   = IMR_POINT.Value;
		break;
    default:
		IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
		IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
		IMR_Data[4].Value   = IMR_POINT.Value;
		break;
	}
	return RT_Success;
}






R828_ErrCode R828_PLL(UINT32 LO_Freq, R828_Standard_Type R828_Standard)
{
	UINT8  MixDiv   = 2;
	UINT8  DivBuf   = 0;
	UINT8  Ni       = 0;
	UINT8  Si       = 0;
	UINT8  DivNum   = 0;
	UINT8  Nint     = 0;
	UINT32 VCO_Min  = 1770000;
	UINT32 VCO_Max  = VCO_Min*2;
	UINT32 VCO_Freq = 0;
	UINT32 PLL_Ref	= 0;		//Max 24000 (kHz)
	UINT32 VCO_Fra	= 0;		//VCO contribution by SDM (kHz)
	UINT16 Nsdm		= 2;
	UINT16 SDM		= 0;
	UINT16 SDM16to9	= 0;
	UINT16 SDM8to1  = 0;
	//UINT8  Judge    = 0;
	UINT8  VCO_fine_tune = 0;
	UINT8  VCO_current_trial = 0;

if ((Rafael_Chip==R620D) || (Rafael_Chip==R828D) || (Rafael_Chip==R828))  //X'tal can't not exceed 20MHz for ATV
{
	if(R828_Standard <= SECAM_L1)	  //ref set refdiv2, reffreq = Xtal/2 on ATV application
	{
       R828_Arry[11] |= 0x10; //b4=1
	   PLL_Ref = R828_Xtal /2;
	}
	else //DTV, FilCal, IMR
	{
       R828_Arry[11] &= 0xEF;
	   PLL_Ref = R828_Xtal;
	}
}
else
{
	if(R828_Xtal > 24000)	 
	{
       R828_Arry[11] |= 0x10; //b4=1
	   PLL_Ref = R828_Xtal /2;
	}
	else
	{
       R828_Arry[11] &= 0xEF;
	   PLL_Ref = R828_Xtal; 
	}
}

	R828_I2C.RegAddr = 0x10;
	R828_I2C.Data = R828_Arry[11];
    if(I2C_Write(&R828_I2C) != RT_Success)
	   return RT_Fail;

	//set pll autotune = 128kHz
	R828_I2C.RegAddr = 0x1A;
	R828_Arry[21]    = R828_Arry[21] & 0xF3;
	R828_I2C.Data    = R828_Arry[21];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
/*
	//Set VCO current = 100
	R828_I2C.RegAddr = 0x12;
	R828_Arry[13]    = (R828_Arry[13] & 0x1F) | 0x80; 
	R828_I2C.Data    = R828_Arry[13];
	if(I2C_Write(&R828_I2C) != RT_Success)
	    return RT_Fail;
*/
	//Set VCO current = 011
	R828_I2C.RegAddr = 0x12;
	R828_Arry[13]    = (R828_Arry[13] & 0x1F) | 0x60; 
	R828_I2C.Data    = R828_Arry[13];
	if(I2C_Write(&R828_I2C) != RT_Success)
	    return RT_Fail;

	//Divider
	while(MixDiv <= 64)
	{
		if(((LO_Freq * MixDiv) >= VCO_Min) && ((LO_Freq * MixDiv) < VCO_Max))
		{
			DivBuf = MixDiv;
			while(DivBuf > 2)
			{
				DivBuf = DivBuf >> 1;
				DivNum ++;
			}			
			break;
		}
		MixDiv = MixDiv << 1;
	}

	R828_I2C_Len.RegAddr = 0x00;
	R828_I2C_Len.Len     = 5;
	if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		return RT_Fail;	

   VCO_fine_tune = (R828_I2C_Len.Data[4] & 0x30)>>4;

	if(VCO_fine_tune > VCO_pwr_ref)
		DivNum = DivNum - 1;
	else if(VCO_fine_tune < VCO_pwr_ref)
	    DivNum = DivNum + 1; 
	
	R828_I2C.RegAddr = 0x10;
	R828_Arry[11] &= 0x1F;
	R828_Arry[11] |= (DivNum << 5);
	R828_I2C.Data = R828_Arry[11];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	VCO_Freq = LO_Freq * MixDiv;
	Nint     = (UINT8) (VCO_Freq / 2 / PLL_Ref);
	VCO_Fra  = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);

	//boundary spur prevention
	if (VCO_Fra < PLL_Ref/64)           //2*PLL_Ref/128
		VCO_Fra = 0;
	else if (VCO_Fra > PLL_Ref*127/64)  //2*PLL_Ref*127/128
	{
		VCO_Fra = 0;
		Nint ++;
	}
	else if((VCO_Fra > PLL_Ref*127/128) && (VCO_Fra < PLL_Ref)) //> 2*PLL_Ref*127/256,  < 2*PLL_Ref*128/256
		VCO_Fra = PLL_Ref*127/128;      // VCO_Fra = 2*PLL_Ref*127/256
	else if((VCO_Fra > PLL_Ref) && (VCO_Fra < PLL_Ref*129/128)) //> 2*PLL_Ref*128/256,  < 2*PLL_Ref*129/256
		VCO_Fra = PLL_Ref*129/128;      // VCO_Fra = 2*PLL_Ref*129/256
	else
		VCO_Fra = VCO_Fra;

	//N & S	
	Ni       = (Nint - 13) / 4;
	Si       = Nint - 4 *Ni - 13;
	R828_I2C.RegAddr = 0x14;
	R828_Arry[15]  = 0x00;
	R828_Arry[15] |= (Ni + (Si << 6));
	R828_I2C.Data = R828_Arry[15];
	
	if(I2C_Write(&R828_I2C) != RT_Success)
	  return RT_Fail;
         	
	//pw_sdm	
	R828_I2C.RegAddr = 0x12;
	R828_Arry[13] &= 0xF7;
	if(VCO_Fra == 0)
		R828_Arry[13] |= 0x08;
	R828_I2C.Data = R828_Arry[13];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//SDM calculator
	while(VCO_Fra > 1)
	{			
		if (VCO_Fra > (2*PLL_Ref / Nsdm))
		{		
			SDM = SDM + 32768 / (Nsdm/2);
			VCO_Fra = VCO_Fra - 2*PLL_Ref / Nsdm;
			if (Nsdm >= 0x8000)
				break;
		}
		Nsdm = Nsdm << 1;
	}

	SDM16to9 = SDM >> 8;
	SDM8to1 =  SDM - (SDM16to9 << 8);

	R828_I2C.RegAddr = 0x16;
	R828_Arry[17]    = (UINT8) SDM16to9;
	R828_I2C.Data    = R828_Arry[17];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	R828_I2C.RegAddr = 0x15;
	R828_Arry[16]    = (UINT8) SDM8to1;
	R828_I2C.Data    = R828_Arry[16];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


//	R828_Delay_MS(10);
	
if ((Rafael_Chip==R620D) || (Rafael_Chip==R828D) || (Rafael_Chip==R828))
{
	if(R828_Standard <= SECAM_L1)
	    R828_Delay_MS(20);
	else
	    R828_Delay_MS(10);
}
else
	 R828_Delay_MS(10);


    for(VCO_current_trial=0; VCO_current_trial<3; VCO_current_trial++)
	{
		//check PLL lock status
		R828_I2C_Len.RegAddr = 0x00;
		R828_I2C_Len.Len     = 3;
		if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
			return RT_Fail;

		if( (R828_I2C_Len.Data[2] & 0x40) == 0x00 ) 
		{
			R828_I2C.RegAddr = 0x12;
			R828_Arry[13]    = (R828_Arry[13] & 0x1F) | ((2-VCO_current_trial) << 5);  //increase VCO current
			R828_I2C.Data    = R828_Arry[13];
			if(I2C_Write(&R828_I2C) != RT_Success)
				 return RT_Fail;
		}
	}
	//set pll autotune = 8kHz
	R828_I2C.RegAddr = 0x1A;
	R828_Arry[21]    = R828_Arry[21] | 0x08;
	R828_I2C.Data    = R828_Arry[21];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;

}





R828_ErrCode R828_MUX(UINT32 RF_KHz)
{	
	//Freq_Info_Type Freq_Info1;
	UINT8 RT_Reg08   = 0;
	UINT8 RT_Reg09   = 0;
	Freq_Info1 = R828_Freq_Sel(RF_KHz);

	// Open Drain
	R828_I2C.RegAddr = 0x17;
	R828_Arry[18] = (R828_Arry[18] & 0xF7) | Freq_Info1.OPEN_D;
	R828_I2C.Data = R828_Arry[18];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	

	// RF_MUX,Polymux 
	R828_I2C.RegAddr = 0x1A;
	R828_Arry[21] = (R828_Arry[21] & 0x3C) | Freq_Info1.RF_MUX_PLOY;
	R828_I2C.Data = R828_Arry[21];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	// TF BAND
	R828_I2C.RegAddr = 0x1B;
	R828_Arry[22] &= 0x00;
	R828_Arry[22] |= Freq_Info1.TF_C;	
	R828_I2C.Data = R828_Arry[22];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	// XTAL CAP & Drive
	R828_I2C.RegAddr = 0x10;
	R828_Arry[11] &= 0xF4;
	switch(Xtal_cap_sel)
	{
	   case XTAL_LOW_CAP_30P:
	   case XTAL_LOW_CAP_20P:
		   R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP20P | 0x08;
		   break;

	   case XTAL_LOW_CAP_10P:
           R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP10P | 0x08;
		   break;

	   case XTAL_LOW_CAP_0P:
		   R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP0P | 0x08;
		   break;

	   case XTAL_HIGH_CAP_0P:
		   R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP0P | 0x00;
		   break;

	   default:
	       R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP0P | 0x08;
		   break;
	}
	R828_I2C.Data = R828_Arry[11];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	//Set_IMR
	if(R828_IMR_done_flag == TRUE)
	{
		RT_Reg08 = IMR_Data[Freq_Info1.IMR_MEM].Gain_X & 0x3F;
		RT_Reg09 = IMR_Data[Freq_Info1.IMR_MEM].Phase_Y & 0x3F;
	}
	else
	{
		RT_Reg08 = 0;
	    RT_Reg09 = 0;
	}

	R828_I2C.RegAddr = 0x08;
	R828_Arry[3] = R828_iniArry[3] & 0xC0;                   
	R828_Arry[3] = R828_Arry[3] | RT_Reg08;
	R828_I2C.Data = R828_Arry[3];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x09;
	R828_Arry[4] = R828_iniArry[4] & 0xC0;
	R828_Arry[4] = R828_Arry[4] | RT_Reg09;
	R828_I2C.Data =R828_Arry[4]  ;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}




R828_ErrCode R828_IQ(R828_SectType* IQ_Pont)
{
	R828_SectType Compare_IQ[3];
//	R828_SectType CompareTemp;
//	UINT8 IQ_Cunt  = 0;
	UINT8 VGA_Cunt = 0;
	UINT16 VGA_Read = 0;
	UINT8  X_Direction;  // 1:X, 0:Y
		
	// increase VGA power to let image significant
	for(VGA_Cunt = 12;VGA_Cunt < 16;VGA_Cunt ++)
	{
		R828_I2C.RegAddr = 0x0C;
		R828_I2C.Data    = (R828_Arry[7] & 0xF0) + VGA_Cunt;  
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_Delay_MS(10); //
		
		if(R828_Muti_Read(0x01, &VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*4)
			break;
	}

	//initial 0x08, 0x09
	//Compare_IQ[0].Gain_X  = 0x40; //should be 0xC0 in R828, Jason
	//Compare_IQ[0].Phase_Y = 0x40; //should be 0x40 in R828
	Compare_IQ[0].Gain_X  = R828_iniArry[3] & 0xC0; // Jason modified, clear b[5], b[4:0]
	Compare_IQ[0].Phase_Y = R828_iniArry[4] & 0xC0; //

	//while(IQ_Cunt < 3)
	//{
	    // Determine X or Y
	    if(R828_IMR_Cross(&Compare_IQ[0], &X_Direction) != RT_Success)
			return RT_Fail;

		//if(X_Direction==1)
		//{
		//    if(R828_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
		//	  return RT_Fail;
		//}
		//else
		//{
		//   if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
		//	return RT_Fail;
		//}

		/*
		//--- X direction ---//
	    //X: 3 points
		if(R828_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //
			return RT_Fail;
        
		//compare and find min of 3 points. determine I/Q direction
		if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		//increase step to find min value of this direction
		if(R828_CompreStep(&Compare_IQ[0], 0x08) != RT_Success)
			return RT_Fail;
		*/

		if(X_Direction==1)
		{
			//compare and find min of 3 points. determine I/Q direction
		    if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
			  return RT_Fail;

		    //increase step to find min value of this direction
		    if(R828_CompreStep(&Compare_IQ[0], 0x08) != RT_Success)  //X
			  return RT_Fail;
		}
		else
		{
		   //compare and find min of 3 points. determine I/Q direction
		   if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R828_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}
		/*
		//--- Y direction ---//
		//Y: 3 points
		if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //
			return RT_Fail;

		//compare and find min of 3 points. determine I/Q direction
		if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		//increase step to find min value of this direction
		if(R828_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)
			return RT_Fail;
        */

		//Another direction
		if(X_Direction==1)
		{	    
           if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
		     return RT_Fail;

		   //compare and find min of 3 points. determine I/Q direction
		   if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R828_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}
		else
		{
		   if(R828_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
		     return RT_Fail;
        
		   //compare and find min of 3 points. determine I/Q direction
		   if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		     return RT_Fail;

	       //increase step to find min value of this direction
		   if(R828_CompreStep(&Compare_IQ[0], 0x08) != RT_Success) //X
		     return RT_Fail;
		}
		//CompareTemp = Compare_IQ[0];

		//--- Check 3 points again---//
		if(X_Direction==1)
		{
		    if(R828_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
			  return RT_Fail;
		}
		else
		{
		   if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
			return RT_Fail;
		}

		//if(R828_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //
		//	return RT_Fail;

		if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		//if((CompareTemp.Gain_X == Compare_IQ[0].Gain_X) && (CompareTemp.Phase_Y == Compare_IQ[0].Phase_Y))//Ben Check
		//	break;
		
		//IQ_Cunt ++;
	//}
	//if(IQ_Cunt ==  3)
	//	return RT_Fail;

	//Section-4 Check 
    /*
	CompareTemp = Compare_IQ[0];
	for(IQ_Cunt = 0;IQ_Cunt < 5;IQ_Cunt ++)
	{
		if(R828_Section(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

		if((CompareTemp.Gain_X == Compare_IQ[0].Gain_X) && (CompareTemp.Phase_Y == Compare_IQ[0].Phase_Y))
			break;
	}
	*/

    //Section-9 check
    //if(R828_F_IMR(&Compare_IQ[0]) != RT_Success)
	if(R828_Section(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

	*IQ_Pont = Compare_IQ[0];

	//reset gain/phase control setting
	R828_I2C.RegAddr = 0x08;
	R828_I2C.Data    = R828_iniArry[3] & 0xC0; //Jason
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x09;
	R828_I2C.Data    = R828_iniArry[4] & 0xC0;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}



//--------------------------------------------------------------------------------------------
// Purpose: record IMC results by input gain/phase location
//          then adjust gain or phase positive 1 step and negtive 1 step, both record results
// input: FixPot: phase or gain
//        FlucPot phase or gain
//        PotReg: 0x08 or 0x09
//        CompareTree: 3 IMR trace and results
// output: TREU or FALSE
//--------------------------------------------------------------------------------------------
R828_ErrCode R828_IQ_Tree(UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R828_SectType* CompareTree)
{
	UINT8 TreeCunt  = 0;
	UINT8 TreeTimes = 3;
	UINT8 TempPot   = 0;
	UINT8 PntReg    = 0;
		
	if(PotReg == 0x08)
		PntReg = 0x09; //phase control
	else
		PntReg = 0x08; //gain control

	for(TreeCunt = 0;TreeCunt < TreeTimes;TreeCunt ++)
	{
		R828_I2C.RegAddr = PotReg;
		R828_I2C.Data    = FixPot;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_I2C.RegAddr = PntReg;
		R828_I2C.Data    = FlucPot;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		if(R828_Muti_Read(0x01, &CompareTree[TreeCunt].Value) != RT_Success)
			return RT_Fail;
	
		if(PotReg == 0x08)
		{
			CompareTree[TreeCunt].Gain_X  = FixPot;
			CompareTree[TreeCunt].Phase_Y = FlucPot;
		}
		else
		{
			CompareTree[TreeCunt].Phase_Y  = FixPot;
			CompareTree[TreeCunt].Gain_X = FlucPot;
		}
		
		if(TreeCunt == 0)   //try right-side point
			FlucPot ++; 
		else if(TreeCunt == 1) //try left-side point
		{
			if((FlucPot & 0x1F) < 0x02) //if absolute location is 1, change I/Q direction
			{
				TempPot = 2 - (FlucPot & 0x1F);
				if(FlucPot & 0x20) //b[5]:I/Q selection. 0:Q-path, 1:I-path
				{
					FlucPot &= 0xC0;
					FlucPot |= TempPot;
				}
				else
				{
					FlucPot |= (0x20 | TempPot);
				}
			}
			else
				FlucPot -= 2;  
		}
	}

	return RT_Success;
}




//-----------------------------------------------------------------------------------/ 
// Purpose: compare IMC result aray [0][1][2], find min value and store to CorArry[0]
// input: CorArry: three IMR data array
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R828_ErrCode R828_CompreCor(R828_SectType* CorArry)
{
	UINT8 CompCunt = 0;
	R828_SectType CorTemp;

	for(CompCunt = 3;CompCunt > 0;CompCunt --)
	{
		if(CorArry[0].Value > CorArry[CompCunt - 1].Value) //compare IMC result [0][1][2], find min value
		{
			CorTemp = CorArry[0];
			CorArry[0] = CorArry[CompCunt - 1];
			CorArry[CompCunt - 1] = CorTemp;
		}
	}

	return RT_Success;
}


//-------------------------------------------------------------------------------------//
// Purpose: if (Gain<9 or Phase<9), Gain+1 or Phase+1 and compare with min value
//          new < min => update to min and continue
//          new > min => Exit
// input: StepArry: three IMR data array
//        Pace: gain or phase register
// output: TRUE or FALSE 
//-------------------------------------------------------------------------------------//
R828_ErrCode R828_CompreStep(R828_SectType* StepArry, UINT8 Pace)
{
	//UINT8 StepCunt = 0;
	R828_SectType StepTemp;
	
	//min value already saved in StepArry[0]
	StepTemp.Phase_Y = StepArry[0].Phase_Y;
	StepTemp.Gain_X  = StepArry[0].Gain_X;

	while(((StepTemp.Gain_X & 0x1F) < IMR_TRIAL) && ((StepTemp.Phase_Y & 0x1F) < IMR_TRIAL))  //5->10
	{
		if(Pace == 0x08)
			StepTemp.Gain_X ++;
		else
			StepTemp.Phase_Y ++;
	
		R828_I2C.RegAddr = 0x08;
		R828_I2C.Data    = StepTemp.Gain_X ;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_I2C.RegAddr = 0x09;
		R828_I2C.Data    = StepTemp.Phase_Y;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		if(R828_Muti_Read(0x01, &StepTemp.Value) != RT_Success)
			return RT_Fail;

		if(StepTemp.Value <= StepArry[0].Value)
		{
			StepArry[0].Gain_X  = StepTemp.Gain_X;
			StepArry[0].Phase_Y = StepTemp.Phase_Y;
			StepArry[0].Value   = StepTemp.Value;
		}
		else if((StepTemp.Value - 8) > StepArry[0].Value)
		{
			break;		
		}
		
	} //end of while()
	
	return RT_Success;
}


//-----------------------------------------------------------------------------------/ 
// Purpose: read multiple IMC results for stability
// input: IMR_Reg: IMC result address
//        IMR_Result_Data: result 
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R828_ErrCode R828_Muti_Read(UINT8 IMR_Reg, UINT16* IMR_Result_Data)  //jason modified
{
	UINT8 ReadCunt     = 0;
	UINT16 ReadAmount  = 0;
	UINT8 ReadMax = 0;
	UINT8 ReadMin = 255;
	UINT8 ReadData = 0;
	
    R828_Delay_MS(5);

	for(ReadCunt = 0;ReadCunt < 6;ReadCunt ++)
	{
		R828_I2C_Len.RegAddr = 0x00;
		R828_I2C_Len.Len     = IMR_Reg + 1;  //IMR_Reg = 0x01
		if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
			return RT_Fail;

		ReadData = (R828_I2C_Len.Data[1] & 0x3F);
		
		ReadAmount = ReadAmount + (UINT16)ReadData;
		
		if(ReadData < ReadMin)
			ReadMin = ReadData;
		
        if(ReadData > ReadMax)
			ReadMax = ReadData;
	}
	*IMR_Result_Data = ReadAmount - (UINT16)ReadMax - (UINT16)ReadMin;

	return RT_Success;
}


R828_ErrCode R828_Section(R828_SectType* IQ_Pont)
{
	R828_SectType Compare_IQ[3];
	R828_SectType Compare_Bet[3];

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		/*
		if((IQ_Pont->Gain_X & 0xE0) == 0x40) //bug => only compare b[5],     
			Compare_IQ[0].Gain_X = 0x61; // Gain=1, I-path //Jason
		else
			Compare_IQ[0].Gain_X = 0x41; // Gain=1, Q-path
        */
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[1].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[1].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[1].Value = Compare_IQ[0].Value;

	//Try X+1 column and save min result to Compare_Bet[2]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)		
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) | 0x20) + 1;  //I-path, Gain=1
	else
	    Compare_IQ[0].Gain_X = IQ_Pont->Gain_X + 1;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R828_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}


R828_ErrCode R828_IMR_Cross(R828_SectType* IQ_Pont, UINT8* X_Direct)
{

	R828_SectType Compare_Cross[9]; //(0,0)(0,Q-1)(0,I-1)(Q-1,0)(I-1,0)+(0,Q-2)(0,I-2)(Q-2,0)(I-2,0)
	R828_SectType Compare_Temp;
	UINT8 CrossCount = 0;
    UINT8 Reg08 = R828_iniArry[3] & 0xC0;
	UINT8 Reg09 = R828_iniArry[4] & 0xC0;

	memset(&Compare_Temp,0, sizeof(R828_SectType));
	Compare_Temp.Value = 255;

	for(CrossCount=0; CrossCount<9; CrossCount++)
	{

		if(CrossCount==0)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==1)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;       //0
		  Compare_Cross[CrossCount].Phase_Y = Reg09 + 1;  //Q-1
		}
		else if(CrossCount==2)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;               //0
		  Compare_Cross[CrossCount].Phase_Y = (Reg09 | 0x20) + 1; //I-1
		}
		else if(CrossCount==3)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08 + 1; //Q-1
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==4)
		{
		  Compare_Cross[CrossCount].Gain_X = (Reg08 | 0x20) + 1; //I-1
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==5)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;       //0
		  Compare_Cross[CrossCount].Phase_Y = Reg09 + 2;  //Q-2
		}
		else if(CrossCount==6)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08;               //0
		  Compare_Cross[CrossCount].Phase_Y = (Reg09 | 0x20) + 2; //I-2
		}
		else if(CrossCount==7)
		{
		  Compare_Cross[CrossCount].Gain_X = Reg08 + 2; //Q-2
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}
		else if(CrossCount==8)
		{
		  Compare_Cross[CrossCount].Gain_X = (Reg08 | 0x20) + 2; //I-2
		  Compare_Cross[CrossCount].Phase_Y = Reg09;
		}

    	R828_I2C.RegAddr = 0x08;
	    R828_I2C.Data    = Compare_Cross[CrossCount].Gain_X;
	    if(I2C_Write(&R828_I2C) != RT_Success)
		   return RT_Fail;

	    R828_I2C.RegAddr = 0x09;
	    R828_I2C.Data    = Compare_Cross[CrossCount].Phase_Y;
	    if(I2C_Write(&R828_I2C) != RT_Success)
		  return RT_Fail;
	
        if(R828_Muti_Read(0x01, &Compare_Cross[CrossCount].Value) != RT_Success)
		  return RT_Fail;

		if( Compare_Cross[CrossCount].Value < Compare_Temp.Value)
		{
		  Compare_Temp.Value = Compare_Cross[CrossCount].Value;
		  Compare_Temp.Gain_X = Compare_Cross[CrossCount].Gain_X;
		  Compare_Temp.Phase_Y = Compare_Cross[CrossCount].Phase_Y;		
		}
	} //end for loop


    if(((Compare_Temp.Phase_Y & 0x3F)==0x01) || (Compare_Temp.Phase_Y & 0x3F)==0x02)  //phase Q1 or Q2
	{
      *X_Direct = (UINT8) 0;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[1].Gain_X;    //0
	  IQ_Pont[1].Phase_Y = Compare_Cross[1].Phase_Y; //Q1
	  IQ_Pont[1].Value = Compare_Cross[1].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[5].Gain_X;   //0
	  IQ_Pont[2].Phase_Y = Compare_Cross[5].Phase_Y;//Q2
	  IQ_Pont[2].Value = Compare_Cross[5].Value;
	}
	else if(((Compare_Temp.Phase_Y & 0x3F)==0x21) || (Compare_Temp.Phase_Y & 0x3F)==0x22)  //phase I1 or I2
	{
      *X_Direct = (UINT8) 0;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[2].Gain_X;    //0
	  IQ_Pont[1].Phase_Y = Compare_Cross[2].Phase_Y; //Q1
	  IQ_Pont[1].Value = Compare_Cross[2].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[6].Gain_X;   //0
	  IQ_Pont[2].Phase_Y = Compare_Cross[6].Phase_Y;//Q2
	  IQ_Pont[2].Value = Compare_Cross[6].Value;
	}
	else if(((Compare_Temp.Gain_X & 0x3F)==0x01) || (Compare_Temp.Gain_X & 0x3F)==0x02)  //gain Q1 or Q2
	{
      *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[3].Gain_X;    //Q1
	  IQ_Pont[1].Phase_Y = Compare_Cross[3].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[3].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[7].Gain_X;   //Q2
	  IQ_Pont[2].Phase_Y = Compare_Cross[7].Phase_Y;//0
	  IQ_Pont[2].Value = Compare_Cross[7].Value;
	}
	else if(((Compare_Temp.Gain_X & 0x3F)==0x21) || (Compare_Temp.Gain_X & 0x3F)==0x22)  //gain I1 or I2
	{
      *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;    //0
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y; //0
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[4].Gain_X;    //I1
	  IQ_Pont[1].Phase_Y = Compare_Cross[4].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[4].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[8].Gain_X;   //I2
	  IQ_Pont[2].Phase_Y = Compare_Cross[8].Phase_Y;//0
	  IQ_Pont[2].Value = Compare_Cross[8].Value;
	}
	else //(0,0) 
	{	
	  *X_Direct = (UINT8) 1;
	  IQ_Pont[0].Gain_X = Compare_Cross[0].Gain_X;
	  IQ_Pont[0].Phase_Y = Compare_Cross[0].Phase_Y;
	  IQ_Pont[0].Value = Compare_Cross[0].Value;

	  IQ_Pont[1].Gain_X = Compare_Cross[3].Gain_X;    //Q1
	  IQ_Pont[1].Phase_Y = Compare_Cross[3].Phase_Y; //0
	  IQ_Pont[1].Value = Compare_Cross[3].Value;

	  IQ_Pont[2].Gain_X = Compare_Cross[4].Gain_X;   //I1
	  IQ_Pont[2].Phase_Y = Compare_Cross[4].Phase_Y; //0
	  IQ_Pont[2].Value = Compare_Cross[4].Value;
	}
	return RT_Success;
}


//----------------------------------------------------------------------------------------//
// purpose: search surrounding points from previous point 
//          try (x-1), (x), (x+1) columns, and find min IMR result point
// input: IQ_Pont: previous point data(IMR Gain, Phase, ADC Result, RefRreq)
//                 will be updated to final best point                 
// output: TRUE or FALSE
//----------------------------------------------------------------------------------------//
R828_ErrCode R828_F_IMR(R828_SectType* IQ_Pont)
{
	R828_SectType Compare_IQ[3];
	R828_SectType Compare_Bet[3];
	UINT8 VGA_Cunt = 0;
	UINT16 VGA_Read = 0;

	//VGA
	for(VGA_Cunt = 12;VGA_Cunt < 16;VGA_Cunt ++)
	{
		R828_I2C.RegAddr = 0x0C;
        R828_I2C.Data    = (R828_Arry[7] & 0xF0) + VGA_Cunt;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_Delay_MS(10);
		
		if(R828_Muti_Read(0x01, &VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*4)
		break;
	}

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[1].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[1].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[1].Value = Compare_IQ[0].Value;

	//Try X+1 column and save min result to Compare_Bet[2]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)		
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) | 0x20) + 1;  //I-path, Gain=1
	else
	    Compare_IQ[0].Gain_X = IQ_Pont->Gain_X + 1;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R828_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R828_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R828_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}



R828_ErrCode R828_GPIO(R828_GPIO_Type R828_GPIO_Conrl)
{
	if(R828_GPIO_Conrl == HI_SIG)
		R828_Arry[10] |= 0x01;
	else
		R828_Arry[10] &= 0xFE;

	R828_I2C.RegAddr = 0x0F;
	R828_I2C.Data    = R828_Arry[10];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}





R828_ErrCode R828_SetStandard(R828_Standard_Type RT_Standard)
{	 

	// Used Normal Arry to Modify
	UINT8 ArrayNum=27;

	if(RT_Standard != R828_pre_standard)
	{
         for(ArrayNum=0;ArrayNum<27;ArrayNum++)
	    {
		    R828_Arry[ArrayNum] = R828_iniArry[ArrayNum];
	     }
	}
    R828_pre_standard = RT_Standard;


	// Look Up System Dependent Table
	Sys_Info1 = R828_Sys_Sel(RT_Standard);
	R828_IF_khz = Sys_Info1.IF_KHz;
	R828_CAL_LO_khz = Sys_Info1.FILT_CAL_LO;

	// Filter Calibration
    if(R828_Fil_Cal_flag[RT_Standard] == FALSE)
	{
		R828_Fil_Cal_code[RT_Standard] = R828_Filt_Cal_ADC(R828_FiltIF_Khz, (Sys_Info1.HP_COR & 0x60));
		R828_Fil_Cal_BW[RT_Standard] = R828_Bandwidth;
        R828_Fil_Cal_flag[RT_Standard] = TRUE;

	    //Reset register and Array 
	    if(R828_InitReg() != RT_Success)        
		   return RT_Fail;

		for(ArrayNum=0;ArrayNum<27;ArrayNum++)	       
		          R828_Arry[ArrayNum] = R828_iniArry[ArrayNum];

        //R828_Fil_Cal_flag[RT_Standard] = TRUE;
	}

    // Set LPF coarse BW  -----
	R828_Arry[6] = (R828_Arry[6] & 0x9F) | R828_Fil_Cal_BW[RT_Standard];
	R828_I2C.RegAddr = 0x0B;
	R828_I2C.Data = R828_Arry[6];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter Q, Fine code
	R828_Arry[5]  = (R828_Arry[5] & 0xE0) | Sys_Info1.FILT_Q | R828_Fil_Cal_code[RT_Standard];  
	R828_I2C.RegAddr = 0x0A;
	R828_I2C.Data = R828_Arry[5];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	// Set BW, Filter_gain, & HP corner
	R828_Arry[6] = (R828_Arry[6] & 0x70) | (Sys_Info1.HP_COR & 0x8F);
	R828_I2C.RegAddr = 0x0B;
	R828_I2C.Data = R828_Arry[6];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	// Set Img_R
	R828_Arry[2]  = (R828_Arry[2] & 0x7F) | Sys_Info1.IMG_R;  
	R828_I2C.RegAddr  = 0x07;
	R828_I2C.Data     = R828_Arry[2];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	// Set filt_3dB, V6MHz
	R828_Arry[1]  = (R828_Arry[1] & 0xCF) | Sys_Info1.FILT_GAIN;  
	R828_I2C.RegAddr  = 0x06;
	R828_I2C.Data     = R828_Arry[1];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

    //channel filter extension
	R828_Arry[25]  = (R828_Arry[25] & 0x9F) | Sys_Info1.EXT_ENABLE;  
	R828_I2C.RegAddr  = 0x1E;
	R828_I2C.Data     = R828_Arry[25];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	//Loop through
	R828_Arry[0]  = (R828_Arry[0] & 0x7F) | Sys_Info1.LOOP_THROUGH;  
	R828_I2C.RegAddr  = 0x05;
	R828_I2C.Data     = R828_Arry[0];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//Loop through attenuation
	R828_Arry[26]  = (R828_Arry[26] & 0x7F) | Sys_Info1.LT_ATT;  
	R828_I2C.RegAddr  = 0x1F;
	R828_I2C.Data     = R828_Arry[26];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

    //filter extention widest
	R828_Arry[10]  = (R828_Arry[10] & 0x7F) | Sys_Info1.FLT_EXT_WIDEST;  
	R828_I2C.RegAddr  = 0x0F;
	R828_I2C.Data     = R828_Arry[10];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//RF poly filter current
	R828_Arry[20]  = (R828_Arry[20] & 0x9F) | Sys_Info1.POLYFIL_CUR;  
	R828_I2C.RegAddr  = 0x19;
	R828_I2C.Data     = R828_Arry[20];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	// Record Init Flag & Xtal_check Result
	if(R828_IMR_done_flag == TRUE)
        R828_Arry[7]    = (R828_Arry[7] & 0xF0) | 0x01 | (Xtal_cap_sel<<1);
	else
	    R828_Arry[7]    = (R828_Arry[7] & 0xF0) | 0x00;

	R828_I2C.RegAddr = 0x0C;
    R828_I2C.Data    = R828_Arry[7];
    if(I2C_Write(&R828_I2C) != RT_Success)
       return RT_Fail;

	// Record version
	R828_I2C.RegAddr = 0x13;
	R828_Arry[14]    = (R828_Arry[14] & 0xC0) | VER_NUM;
	R828_I2C.Data    = R828_Arry[14];
	if(I2C_Write(&R828_I2C) != RT_Success)
	    return RT_Fail;

	return RT_Success;
}



R828_ErrCode R828_Filt_Cal(UINT32 Cal_Freq,BW_Type R828_BW)
{
	  //set in Sys_sel()
	/*
	if(R828_BW == BW_8M)
	{
		//set filt_cap = no cap
		R828_I2C.RegAddr = 0x0B;  //reg11
		R828_Arry[6]   &= 0x9F;  //filt_cap = no cap
		R828_I2C.Data    = R828_Arry[6];		
	}
	else if(R828_BW == BW_7M)
	{
		//set filt_cap = +1 cap
		R828_I2C.RegAddr = 0x0B;  //reg11
		R828_Arry[6]   &= 0x9F;  //filt_cap = no cap
		R828_Arry[6]   |= 0x20;  //filt_cap = +1 cap
		R828_I2C.Data    = R828_Arry[6];		
	}
	else if(R828_BW == BW_6M)
	{
		//set filt_cap = +2 cap
		R828_I2C.RegAddr = 0x0B;  //reg11
		R828_Arry[6]   &= 0x9F;  //filt_cap = no cap
		R828_Arry[6]   |= 0x60;  //filt_cap = +2 cap
		R828_I2C.Data    = R828_Arry[6];		
	}


    if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;	
*/

    // Set filt_cap
	R828_I2C.RegAddr  = 0x0B;
	R828_Arry[6]= (R828_Arry[6] & 0x9F) | (Sys_Info1.HP_COR & 0x60);
	R828_I2C.Data     = R828_Arry[6];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	//set cali clk =on
	R828_I2C.RegAddr = 0x0F;  //reg15
	R828_Arry[10]   |= 0x04;  //calibration clk=on
	R828_I2C.Data    = R828_Arry[10];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//X'tal cap 0pF for PLL	
	R828_I2C.RegAddr = 0x10;
	R828_Arry[11]    = (R828_Arry[11] & 0xFC) | 0x00;
	R828_I2C.Data    = R828_Arry[11];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//Set PLL Freq = Filter Cali Freq
	if(R828_PLL(Cal_Freq, STD_SIZE) != RT_Success)
		return RT_Fail;

	//Start Trigger
	R828_I2C.RegAddr = 0x0B;	//reg11
	R828_Arry[6]   |= 0x10;	    //vstart=1	
	R828_I2C.Data    = R828_Arry[6];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	//delay 0.5ms
	R828_Delay_MS(1);  

	//Stop Trigger
	R828_I2C.RegAddr = 0x0B;
	R828_Arry[6]   &= 0xEF;     //vstart=0
	R828_I2C.Data    = R828_Arry[6];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;


	//set cali clk =off
	R828_I2C.RegAddr  = 0x0F;	//reg15
	R828_Arry[10]    &= 0xFB;	//calibration clk=off
	R828_I2C.Data     = R828_Arry[10];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;

}




UINT8  R828_Filt_Cal_ADC(UINT32 IF_Freq, UINT8 R828_BW)
{
	 UINT8     u1FilterCodeResult = 0;
	 UINT8     u1FilterCode = 0;
	 UINT32   u4RingFreq = 533333;
	 //UINT32   u4RingFreq = 66666;
	 UINT16    u1FilterCalValue = 0;
	 UINT16    u1FilterCalValuePre = 0;
	 UINT8     initial_cnt = 0;
	 UINT8     i = 0;
	 // increase VGA power to let ADC read value significant
	 UINT8    VGA_Cunt = 0;
	 UINT16   VGA_Read = 0;

	 //Write initial reg before doing calibration 
	 if(R828_InitReg() != RT_Success)        
		return RT_Fail;

	 if(R828_IMR_Prepare() != RT_Success)      
	      return RT_Fail;

	 //Set Ring PLL(533.33MHz)	 	
	 R828_Arry[19] = (R828_Arry[19] & 0xF0) | 0x09;     // ring[3:0]=9
	 R828_Arry[19] = (R828_Arry[19] & 0xDF) | 0x20;    // ring_se23=1
	 R828_Arry[20] = (R828_Arry[20] & 0xFC) | 0x00;    // ring_seldiv = 0
     R828_Arry[26] = (R828_Arry[26] & 0xFC) | 0x03;    // ring_att 11
	 R828_Arry[2] = (R828_Arry[2] & 0xE0) | 0x00;        //mixer gain=0

     //---------- Set Mixer Gain------------//
     R828_I2C.RegAddr = 0x07;
     R828_I2C.Data    = R828_Arry[2];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;
	//------------n_ring,ring_se23----------//
	R828_I2C.RegAddr = 0x18;
	R828_I2C.Data    = R828_Arry[19];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	//------------ring_sediv----------------//
	R828_I2C.RegAddr = 0x19;
	R828_I2C.Data    = R828_Arry[20];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
	//------------pw_ring-------------------//
	R828_I2C.RegAddr = 0x1f;
	R828_I2C.Data    = R828_Arry[26];
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

     //Must do before PLL() 
	 if(R828_MUX(u4RingFreq + IF_Freq) != RT_Success)     //FilCal MUX ()
	     return RT_Fail;

	 //Set PLL
	 if(R828_PLL((u4RingFreq + IF_Freq), STD_SIZE) != RT_Success)   //FilCal PLL
	       return RT_Fail;

	 //-----below must set after R828_MUX()-------//
	 //Set filter cap, HPF corner widest; 1.7M disable
     R828_I2C.RegAddr = 0x0B;	
	 R828_Arry[6] = (0x00) | R828_BW | 0x0F;	
     R828_I2C.Data = R828_Arry[6];		
     if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;	

     //adc=on, vga code mode, gain = -12dB  
     R828_I2C.RegAddr = 0x0C;
     R828_Arry[7]    = (R828_Arry[7] & 0x60) | 0x00;
     R828_I2C.Data    = R828_Arry[7];
     if(I2C_Write(&R828_I2C) != RT_Success)
           return RT_Fail;

	 // Set Filter lowQ & channel filter ON
	 R828_Arry[5]  = (R828_Arry[5] | 0x90);  
	 R828_I2C.RegAddr  = 0x0A;
	 R828_I2C.Data     = R828_Arry[5];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	 // Disable Filter extension
	 R828_Arry[25]  = (R828_Arry[25] & 0xBF);  
	 R828_I2C.RegAddr  = 0x1E;
	 R828_I2C.Data     = R828_Arry[25];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	 R828_I2C.RegAddr = 0x0A;
     R828_Arry[5] = (R828_Arry[5] & 0xF0) | 0;  //filter code=0
     R828_I2C.Data = R828_Arry[5];
     if(I2C_Write(&R828_I2C) != RT_Success)
          return RT_Fail;

	 for(VGA_Cunt = 0;VGA_Cunt < 16;VGA_Cunt ++)
	{
		R828_I2C.RegAddr = 0x0C;
		R828_I2C.Data    = (R828_Arry[7] & 0xF0) + VGA_Cunt;  
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_Delay_MS(10); //
		
		if(R828_Muti_Read(0x01, &VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*4)
			break;
	}

	 //------- Try suitable BW --------//
	 if(R828_BW==0x60) //6M
         initial_cnt = 1;  //try 7M first
	 else
		 initial_cnt = 0;  //try 8M first

	 for(i=initial_cnt; i<3; i++)
	 {
         if(i==0)
             R828_Bandwidth = 0x00; //8M
		 else if(i==1)
			 R828_Bandwidth = 0x40; //7M
		 else
			 R828_Bandwidth = 0x60; //6M

		 R828_I2C.RegAddr = 0x0B;	
	     R828_Arry[6] = ( 0x00) | R828_Bandwidth;	  
         R828_I2C.Data = R828_Arry[6];		
         if(I2C_Write(&R828_I2C) != RT_Success)
		      return RT_Fail;	

		 // read code 0
		 R828_I2C.RegAddr = 0x0A;
		 R828_Arry[5] = (R828_Arry[5] & 0xF0) | 0;  //code 0
		 R828_I2C.Data = R828_Arry[5];
		 if(I2C_Write(&R828_I2C) != RT_Success)
			  return RT_Fail;

		 R828_Delay_MS(10); //delay ms
	     
		 if(R828_Muti_Read(0x01, &u1FilterCalValuePre) != RT_Success)
			  return RT_Fail;

		 //read code 13
		 R828_I2C.RegAddr = 0x0A;
		 R828_Arry[5] = (R828_Arry[5] & 0xF0) | 13;  //code 13
		 R828_I2C.Data = R828_Arry[5];
		 if(I2C_Write(&R828_I2C) != RT_Success)
			  return RT_Fail;

		 R828_Delay_MS(10); //delay ms
	     
		 if(R828_Muti_Read(0x01, &u1FilterCalValue) != RT_Success)
			  return RT_Fail;

		 if(u1FilterCalValuePre > (u1FilterCalValue+8*4))  //suitable BW found
			 break;
	 }

     //Try LPF filter code 
	 u1FilterCalValuePre = 0;
	 for(u1FilterCode=0; u1FilterCode<16; u1FilterCode++)
	 {
         R828_I2C.RegAddr = 0x0A;
         R828_Arry[5] = (R828_Arry[5] & 0xF0) | u1FilterCode;
         R828_I2C.Data = R828_Arry[5];
         if(I2C_Write(&R828_I2C) != RT_Success)
              return RT_Fail;

		 R828_Delay_MS(10); //delay(?) ms

		 if(R828_Muti_Read(0x01, &u1FilterCalValue) != RT_Success)
		      return RT_Fail;

		 if(u1FilterCode==0)
              u1FilterCalValuePre = u1FilterCalValue;

		 if((u1FilterCalValue+FILTER_ATT*4) < u1FilterCalValuePre) //att >2dB@f+1.4M
		 {
			 u1FilterCodeResult = u1FilterCode-1;   //set previous code
			  break;
		 }
	 }

	 if(u1FilterCode==16)
          u1FilterCodeResult = 15;

	  return u1FilterCodeResult;

}
R828_ErrCode R828_SetFrequency(R828_Set_Info R828_INFO, R828_SetFreq_Type R828_SetFreqMode)
{

	 UINT32	LO_KHz;
	 UINT8    i;

     // Check Input Frequency Range
	 if((R828_INFO.RF_KHz<40000) || (R828_INFO.RF_KHz>1002000))
	 {
	          return RT_Fail;
	 }

	 if(R828_INFO.R828_Standard==SECAM_L1)
		LO_KHz = R828_INFO.RF_KHz - Sys_Info1.IF_KHz;
	 else
	    LO_KHz = R828_INFO.RF_KHz + Sys_Info1.IF_KHz;

	 //Set MUX dependent var. Must do before PLL( ) 
     if(R828_MUX(LO_KHz) != RT_Success)
        return RT_Fail;

	 //* Caution: R620D, R828D, R828 Not support Clock Out
	 if((R828_INFO.R828_Standard>SECAM_L1) ) 
	 {
		 // set DTV Xtal CAP=0pF, HIGH, when RF > 90M
	     if(R828_INFO.RF_KHz>120000)  
	     {
	          R828_Arry[11] &= 0xF4;
			  R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP0P | 0x00;		  
	     }
		 else
		 {
			  R828_Arry[11] &= 0xF4;
			  R828_Arry[11] = R828_Arry[11] | Freq_Info1.XTAL_CAP0P | 0x08;		  		 
		 }
	 
	    R828_I2C.RegAddr = 0x10;
	    R828_I2C.Data = R828_Arry[11];
	    if(I2C_Write(&R828_I2C) != RT_Success)
	       return RT_Fail;
	 }

     //Set PLL
     if(R828_PLL(LO_KHz, R828_INFO.R828_Standard) != RT_Success)
        return RT_Fail;

     R828_IMR_point_num = Freq_Info1.IMR_MEM;


     //Get Sys-Freq parameter
     SysFreq_Info1 = R828_SysFreq_Sel(R828_INFO.R828_Standard, R828_INFO.RF_KHz);

    //Set Extra Register
	if((R828_INFO.R828_Standard==ATSC_4063) || (R828_INFO.R828_Standard==ATSC_5070) || (R828_INFO.R828_Standard==ATSC_KOREA))
	{
        R828_ExtraArry[1]=0x31;  //ATSC: LNA 1.5dB on
		R828_ExtraArry[2]=0xB8;  //ATSC: VGA comp on
	}
	else
	{
	    R828_ExtraArry[1]=0x30; //others: off
		R828_ExtraArry[2]=0xA8;  //others: VGA comp off
	}

	if(R828_INFO.RF_KHz<100000)
        R828_ExtraArry[0]=0x80; //Q enhance on
	else
        R828_ExtraArry[0]=0x00; //Q enhance off

		
	R828_I2C_Len.RegAddr = 0x00;
	R828_I2C_Len.Len     = 5;
	for(i = 0;  i<5;  i++)
	{
		R828_I2C_Len.Data[i] = R828_ExtraArry[i];
	}
	if(I2C_Write_Len(&R828_I2C_Len) != RT_Success)
		return RT_Fail;

     // write DectBW, pre_dect_TOP
     R828_Arry[24] = (R828_Arry[24] & 0x38) | (SysFreq_Info1.LNA_TOP & 0xC7);
     R828_I2C.RegAddr = 0x1D;
     R828_I2C.Data = R828_Arry[24];
     if(I2C_Write(&R828_I2C) != RT_Success)
         return RT_Fail;

     // write MIXER TOP, TOP+-1
     R828_Arry[23] = (R828_Arry[23] & 0x07) | (SysFreq_Info1.MIXER_TOP & 0xF8); 
     R828_I2C.RegAddr = 0x1C;
     R828_I2C.Data = R828_Arry[23];
     if(I2C_Write(&R828_I2C) != RT_Success)
        return RT_Fail;


     // write LNA VTHL
     R828_Arry[8] = (R828_Arry[8] & 0x00) | SysFreq_Info1.LNA_VTH_L;
     R828_I2C.RegAddr = 0x0D;
     R828_I2C.Data = R828_Arry[8];
     if(I2C_Write(&R828_I2C) != RT_Success)
        return RT_Fail;

     // write MIXER VTHL
     R828_Arry[9] = (R828_Arry[9] & 0x00) | SysFreq_Info1.MIXER_VTH_L;
     R828_I2C.RegAddr = 0x0E;
     R828_I2C.Data = R828_Arry[9];
     if(I2C_Write(&R828_I2C) != RT_Success)
        return RT_Fail;

	 // Cable-1/Air in 
	 R828_I2C.RegAddr = 0x05;
	 R828_Arry[0] &= 0x9F;
	 R828_Arry[0] |= SysFreq_Info1.AIR_CABLE1_IN;	
	 R828_I2C.Data = R828_Arry[0];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	 // Cable-2 in 
	 R828_I2C.RegAddr = 0x06;
	 R828_Arry[1] &= 0xF7;
	 R828_Arry[1] |= SysFreq_Info1.CABLE2_IN;	
	 R828_I2C.Data = R828_Arry[1];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

     //CP current
	 R828_I2C.RegAddr = 0x11;
	 R828_Arry[12] &= 0xC7;
	 R828_Arry[12] |= SysFreq_Info1.CP_CUR;	
	 R828_I2C.Data = R828_Arry[12];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		 return RT_Fail;	

	 //div buffer current and PLL current
	 R828_I2C.RegAddr = 0x17;
	 R828_Arry[18] &= 0xC9;
	 R828_Arry[18] |= SysFreq_Info1.DIV_BUF_CUR;
	 R828_I2C.Data = R828_Arry[18];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		 return RT_Fail;	

	 // Set channel filter current 
	 R828_I2C.RegAddr  = 0x0A;
	 R828_Arry[5]  = (R828_Arry[5] & 0x9F) | SysFreq_Info1.FILTER_CUR;  
	 R828_I2C.Data     = R828_Arry[5];
     if(I2C_Write(&R828_I2C) != RT_Success)
         return RT_Fail;


     //Set LNA
	 if(R828_INFO.R828_Standard > SECAM_L1)
	 {

		 if(R828_SetFreqMode==FAST_MODE)       //FAST mode 
		 {
			 // write LNA TOP
			 R828_Arry[24] = (R828_Arry[24] & 0xC7) | (SysFreq_Info1.LNA_TOP & 0x38);
			 R828_I2C.RegAddr = 0x1D;
			 R828_I2C.Data = R828_Arry[24];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				 return RT_Fail;
             
			 R828_Arry[23] = (R828_Arry[23] & 0xFB);  // 0: normal mode
			 R828_I2C.RegAddr = 0x1C;
			 R828_I2C.Data = R828_Arry[23];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
			
			 // PRE_DECT on
			 R828_Arry[1]  = (R828_Arry[1] & 0xBF) | SysFreq_Info1.PRE_DECT;
			 R828_I2C.RegAddr  = 0x06;
			 R828_I2C.Data     = R828_Arry[1];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
             
             // agc clk 1Khz, external det1 cap 1u
			 R828_Arry[21]  = (R828_Arry[21] & 0xCF) | 0x00;   			
			 R828_I2C.RegAddr  = 0x1A;
			 R828_I2C.Data     = R828_Arry[21];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 R828_Arry[11]  = (R828_Arry[11] & 0xFB) | 0x00;   			
			 R828_I2C.RegAddr  = 0x10;
			 R828_I2C.Data     = R828_Arry[11];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 // write LNA VTHL=0.64~1.04V
             R828_Arry[8] = (R828_Arry[8] & 0x00) | 0x73;
             R828_I2C.RegAddr = 0x0D;
             R828_I2C.Data = R828_Arry[8];
             if(I2C_Write(&R828_I2C) != RT_Success)
                 return RT_Fail;
		 }
		 else  //NORMAL mode
		 {

			 // write LNA TOP
			 R828_Arry[24] = (R828_Arry[24] & 0xC7) | (SysFreq_Info1.LNA_TOP & 0x38);
			 R828_I2C.RegAddr = 0x1D;
			 R828_I2C.Data = R828_Arry[24];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				 return RT_Fail;
             
			 R828_Arry[23] = (R828_Arry[23] & 0xFB);  // 0: normal mode
			 R828_I2C.RegAddr = 0x1C;
			 R828_I2C.Data = R828_Arry[23];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
			
			 // PRE_DECT on
			 R828_Arry[1]  = (R828_Arry[1] & 0xBF) | SysFreq_Info1.PRE_DECT;
			 R828_I2C.RegAddr  = 0x06;
			 R828_I2C.Data     = R828_Arry[1];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
             
             // agc clk 1Khz, external det1 cap 1u
			 R828_Arry[21]  = (R828_Arry[21] & 0xCF) | 0x00;   			
			 R828_I2C.RegAddr  = 0x1A;
			 R828_I2C.Data     = R828_Arry[21];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 R828_Arry[11]  = (R828_Arry[11] & 0xFB) | 0x00;   			
			 R828_I2C.RegAddr  = 0x10;
			 R828_I2C.Data     = R828_Arry[11];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 R828_Delay_MS(250);

			 // write discharge mode
			 R828_Arry[23] = (R828_Arry[23] & 0xFB) | (SysFreq_Info1.MIXER_TOP & 0x04); 
			 R828_I2C.RegAddr = 0x1C;
			 R828_I2C.Data = R828_Arry[23];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 // LNA discharge current
			 R828_Arry[25]  = (R828_Arry[25] & 0xE0) | SysFreq_Info1.LNA_DISCHARGE;  
			 R828_I2C.RegAddr  = 0x1E;
			 R828_I2C.Data     = R828_Arry[25];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 //agc clk 60hz 
			 R828_Arry[21]  = (R828_Arry[21] & 0xCF) | 0x20;   	
			 R828_I2C.RegAddr  = 0x1A;
			 R828_I2C.Data     = R828_Arry[21];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 R828_Arry[11]  = (R828_Arry[11] & 0xFB) | 0x04;   			
			 R828_I2C.RegAddr  = 0x10;
			 R828_I2C.Data     = R828_Arry[11];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
		 }
	 }
	 else 
	 {
		 if((R828_SetFreqMode==NORMAL_MODE) || (R828_SetFreqMode==FAST_MODE))
		 {
             // PRE_DECT on
			 R828_Arry[1]  = (R828_Arry[1] & 0xBF) | SysFreq_Info1.PRE_DECT;
			 R828_I2C.RegAddr  = 0x06;
			 R828_I2C.Data     = R828_Arry[1];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;			 

			 // write LNA TOP
			 R828_Arry[24] = (R828_Arry[24] & 0xC7) | (SysFreq_Info1.LNA_TOP & 0x38);
			 R828_I2C.RegAddr = 0x1D;
			 R828_I2C.Data = R828_Arry[24];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				 return RT_Fail;

			 // write discharge mode: Normal for ATV
			 R828_Arry[23] = (R828_Arry[23] & 0xFB) | (SysFreq_Info1.MIXER_TOP & 0x04); 
			 R828_I2C.RegAddr = 0x1C;
			 R828_I2C.Data = R828_Arry[23];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 // LNA discharge current (no use in Normal Mode)
			 R828_Arry[25]  = (R828_Arry[25] & 0xE0) | SysFreq_Info1.LNA_DISCHARGE;  
			 R828_I2C.RegAddr  = 0x1E;
			 R828_I2C.Data     = R828_Arry[25];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 // agc clk 1Khz, external det1 cap 1u
			 R828_Arry[21]  = (R828_Arry[21] & 0xCF) | 0x00;   			
			 R828_I2C.RegAddr  = 0x1A;
			 R828_I2C.Data     = R828_Arry[21];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;

			 R828_Arry[11]  = (R828_Arry[11] & 0xFB) | 0x00;   			
			 R828_I2C.RegAddr  = 0x10;
			 R828_I2C.Data     = R828_Arry[11];
			 if(I2C_Write(&R828_I2C) != RT_Success)
				return RT_Fail;
		 }
	 }

     return RT_Success;

}




R828_ErrCode R828_Standby(R828_LoopThrough_Type R828_LoopSwitch)
{
	if(R828_LoopSwitch == LOOP_THROUGH)
	{
		R828_I2C.RegAddr = 0x05;
		R828_I2C.Data = 0x03;
		if(I2C_Write(&R828_I2C) != RT_Success)
		    return RT_Fail;

		R828_I2C.RegAddr = 0x06;
		R828_I2C.Data    = 0x31;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;
	}
	else
	{
		R828_I2C.RegAddr = 0x05;
		R828_I2C.Data    = 0xA3;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;

		R828_I2C.RegAddr = 0x06;
		R828_I2C.Data    = 0xB1;
		if(I2C_Write(&R828_I2C) != RT_Success)
			return RT_Fail;
	}

	R828_I2C.RegAddr = 0x07;
	R828_I2C.Data    = 0x3A;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x08;
	R828_I2C.Data    = 0x40;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x09;
	R828_I2C.Data    = 0xC0;   //polyfilter off
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x0A;
	R828_I2C.Data    = 0x36;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x0C;
	R828_I2C.Data    = 0x35;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x0F;
	R828_I2C.Data    = 0x78;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x11;
	R828_I2C.Data    = 0x03;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x17;
	R828_I2C.Data    = 0xF4;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	R828_I2C.RegAddr = 0x19;
	R828_I2C.Data    = 0x0C;
	if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	
	return RT_Success;
}



R828_ErrCode R828_GetRfGain(R828_RF_Gain_Info *pR828_rf_gain)
{

	R828_I2C_Len.RegAddr = 0x00;
	R828_I2C_Len.Len     = 4;
	if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
		return RT_Fail;

	pR828_rf_gain->RF_gain1 = (R828_I2C_Len.Data[3] & 0x0F);
	pR828_rf_gain->RF_gain2 = ((R828_I2C_Len.Data[3] & 0xF0) >> 4);
	pR828_rf_gain->RF_gain_comb = pR828_rf_gain->RF_gain1*2 + pR828_rf_gain->RF_gain2;

    return RT_Success;
}


R828_ErrCode R828_RfGainMode(R828_RF_Gain_TYPE R828_RfGainType)
{
    UINT8 MixerGain = 0;
	UINT8 LnaGain = 0;

	if(R828_RfGainType==RF_MANUAL)
	{
		//LNA auto off
	     R828_I2C.RegAddr = 0x05;
	     R828_Arry[0] = R828_Arry[0] | 0x10;
		 R828_I2C.Data = R828_Arry[0];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer auto off
	     R828_I2C.RegAddr = 0x07;
	     R828_Arry[2] = R828_Arry[2] & 0xEF;
		 R828_I2C.Data = R828_Arry[2];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;

		R828_I2C_Len.RegAddr = 0x00;
		R828_I2C_Len.Len     = 4; 
		if(I2C_Read_Len(&R828_I2C_Len) != RT_Success)
			return RT_Fail;

		MixerGain = (R828_I2C_Len.Data[3] & 0xF0) >> 4;
		LnaGain = R828_I2C_Len.Data[3] & 0x0F;

		//set LNA gain
	     R828_I2C.RegAddr = 0x05;
	     R828_Arry[0] = (R828_Arry[0] & 0xF0) | LnaGain;
		 R828_I2C.Data = R828_Arry[0];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;

		 //set Mixer gain
	     R828_I2C.RegAddr = 0x07;
	     R828_Arry[2] = (R828_Arry[2] & 0xF0) | MixerGain;
		 R828_I2C.Data = R828_Arry[2];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;
	}
	else
	{
	    //LNA
	     R828_I2C.RegAddr = 0x05;
	     R828_Arry[0] = R828_Arry[0] & 0xEF;
		 R828_I2C.Data = R828_Arry[0];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer
	     R828_I2C.RegAddr = 0x07;
	     R828_Arry[2] = R828_Arry[2] | 0x10;
		 R828_I2C.Data = R828_Arry[2];
	     if(I2C_Write(&R828_I2C) != RT_Success)
		       return RT_Fail;
	}

    return RT_Success;
}


//-------------------------------------------------------//
// User Self-control Loop Through ON/OFF
// It must be placed after R828_SetFrequency( )
//------------------------------------------------------//
R828_ErrCode R828_SetLoopThrough(R828_LoopThrough_Type R828_LT_Type)
{
    if(R828_LT_Type == LOOP_THROUGH)
	{
         R828_Arry[0]  = (R828_Arry[0] & 0x7F) | 0x00;  
	}
	else
	{
		 R828_Arry[0]  = (R828_Arry[0] & 0x7F) | 0x80;  
	}
	 R828_I2C.RegAddr  = 0x05;
	 R828_I2C.Data     = R828_Arry[0];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	 return RT_Success;
}

R828_ErrCode R828_SetLoopThroughAttenu(R828_LoopThroughAtt_Type R828_LT_Att_Type, UINT8 R828_LT_Att_Gain)
{
    if(R828_LT_Att_Type == LT_ATT_ON)
	{
		 R828_Arry[26]  = (R828_Arry[26] & 0x7F) | 0x00;   //ON
	}
	else
	{
		 R828_Arry[26]  = (R828_Arry[26] & 0x7F) | 0x80;  //OFF
	}

	 //LT Att
	 R828_I2C.RegAddr  = 0x1F;
	 R828_I2C.Data     = R828_Arry[26];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;

	 //LT Att Gain
	 R828_ExtraArry[2] =  ((R828_ExtraArry[2] & 0xF8) | R828_LT_Att_Gain);
	 R828_I2C.RegAddr  = 0x02;
	 R828_I2C.Data     = R828_ExtraArry[2];
	 if(I2C_Write(&R828_I2C) != RT_Success)
		return RT_Fail;
		
	 return RT_Success;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//                      Smart GUI                               //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
UINT8 R828_IMR_XT[6];
UINT8 R828_IMR_YT[6];
R828_ErrCode R828_ReadIMR(void)
{
	UINT8 IMR_C = 0;
//	UINT8 IMR_X[6] = {0, 0, 0, 0, 0, 0};
//	UINT8 IMR_Y[6] = {0, 0, 0, 0, 0, 0};


	for(IMR_C = 0;IMR_C < 6;IMR_C ++)
	{
		R828_IMR_XT[IMR_C] = IMR_Data[IMR_C].Gain_X;
		R828_IMR_YT[IMR_C] = IMR_Data[IMR_C].Phase_Y;
	}
	
	return RT_Success;
}

HI_S32 r820c_init_tuner(HI_U32 u32TunerPort)
{
    g_r820c_tuner_i2c_channel = g_stTunerOps[u32TunerPort].enI2cChannel;
    g_r820c_tuner_port = u32TunerPort;

    //Initial R828
    if(R828_Init() != RT_Success)
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 r820c_set_tuner(HI_U32 u32TunerPort, HI_U8 i2c_channel, HI_U32 puRF)
{
    R828_Standard_Type R828_Standard_Set;
    R828_Set_Info R828_Info_Msg;
    R828_SetFreq_Type R828_SetFreq_Mode;
    
    //R828 Standard Setting
   
    if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[u32TunerPort].enDemodDevType)
    {   
        R828_Standard_Set=J83B;
        R828_Info_Msg.R828_Standard=J83B;
    }
    else
    {
        R828_Standard_Set=DVB_C_8M;
        R828_Info_Msg.R828_Standard=DVB_C_8M;
        
    } 
    if(R828_SetStandard( R828_Standard_Set) != RT_Success)
    {
        return HI_FAILURE;
    }
    
    //R828 Frequency Setting
    // R828_SetFreq_Mode = FAST_MODE; // auto scan
    R828_SetFreq_Mode = NORMAL_MODE; // manual scan or switch channel
    R828_Info_Msg.RF_KHz = puRF; // unit: kHz
    if(R828_SetFrequency(R828_Info_Msg, R828_SetFreq_Mode) != RT_Success)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID r820c_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    R828_ErrCode err = RT_Success;
    R828_RF_Gain_Info R828_rf_gain;

    err = R828_GetRfGain( &R828_rf_gain);
    if(RT_Success != err)
    {
        HI_ERR_TUNER("R828_GetRfGain\n");
        return;
    }
    pu32SignalStrength[2] = (HI_U32)(R828_rf_gain.RF_gain_comb);
    return;
}

HI_VOID r820c_tuner_resume (HI_U32 u32TunerPort)
{
    R828_IMR_done_flag = FALSE;
    r820c_init_tuner(g_r820c_tuner_port);
    return;
}

