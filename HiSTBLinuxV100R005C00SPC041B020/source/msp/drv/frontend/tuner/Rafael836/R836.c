//-----------------------------------------------------//
//  
// Filename: R836.c   
//
// This file is R836 tuner driver
// Copyright 2013 by Rafaelmicro., Inc.
// Author: Jason Wang
//-----------------------------------------------------//

//#include "stdafx.h"
#include "R836.h"
#include "Rafael_I2C.h"

#include "hi_drv_i2c.h"
#include "hi_debug.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"

#define BOOL HI_BOOL

#define  ADC_MULTI_READ  1      
UINT32 ADC_READ_DELAY = 2;  
UINT8  ADC_READ_COUNT = 1;   
UINT8  VGA_DELAY = 5;              
UINT8  FILTER_DELAY = 3;         
             
UINT32 RF_KHz = 435;

UINT8 R840_iniArray[R840_REG_NUM] = {

                           0x02, 0x44, 0x40, 0x40, 0x05,   //R0[7]=0, Q off, SD IM=0(off)
					  //   0x00, 0x01, 0x02, 0x03, 0x04 

	                        0x69, 0x17, 0x75, 0x40, 0x40, 0x06, 0x08, 0x89, 0xA4,   //R7[7]=0, clk out 
					//     0x05  0x06  0x07  0x08  0x09  0x0A  0x0B  0x0C  0x0D                                                    	   
													  
#if(R840_INIT_SMALL_XTAL_CAP==TRUE)
						   0x95, 0x9C, 0x73, 0x02, 0x72, 0x20, 0x0B, 0xAA, 0xAA, 
#else
						   0x95, 0x9C, 0x73, 0x02, 0x72, 0x00, 0x0B, 0xAA, 0xAA,  //LO=115.33M
					//     0x0E  0x0F  0x10  0x11  0x12  0x13  0x14  0x15  0x16                                                    	   
#endif
#if(R840_INIT_LT_ON==TRUE)
							0x90, 0x30, 0x40, 0x87, 0xC2, 0x25, 0x30, 0x26, 0x47};
#else
							0xB0, 0x30, 0x40, 0x87, 0xC2, 0x25, 0x30, 0x26, 0x47};
					//     0x17  0x18  0x19  0x1A  0x1B  0x1C  0x1D  0x1E  0x1F 
#endif

UINT8 R840_ADDRESS;
UINT8 R840_Chip = R836_R;			          
UINT8 R840_XtalDiv = XTAL_DIV2;
UINT8 R840_SetTfType = R840_TF_BEAD;   //Depend on HW
UINT8 R840_DetectTfType = R840_UL_USING_270NH;
UINT8 R840_Fil_Cal_Gap = 8;
UINT32 R840_IF_HIGH = 8500;  
UINT8 R840_Xtal_Pwr = XTAL_GM3V_STRONG;
UINT8 R840_Xtal_Pwr_tmp = XTAL_GM3V_STRONG;

//----------------------------------------------------------//
//                   Internal Parameters                    //
//----------------------------------------------------------//

UINT8 R840_Array[32];
R840_SectType R840_IMR_Data[5] = {
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0},
                                                  {0, 0, 0, 0}
                                                };//Please keep this array data for standby mode.
I2C_TYPE  R840_I2C;
I2C_LEN_TYPE R840_I2C_Len;

UINT8  R840_IMR_point_num;
UINT8  R840_Initial_done_flag = FALSE;
UINT8  R840_IMR_done_flag = FALSE;
UINT8  R840_Bandwidth = 0x00;
UINT8  R840_Lpf_Lsb = 0;   
UINT8  R840_Fil_Cal_flag[R840_STD_SIZE];
static UINT8 R840_Fil_Cal_BW[R840_STD_SIZE];
static UINT8 R840_Fil_Cal_code[R840_STD_SIZE];
static UINT8 R840_Fil_Cal_LpfLsb[R840_STD_SIZE]; 
static R840_Standard_Type R840_pre_standard = R840_STD_SIZE;
static UINT8 R840_IMR_Cal_Type = R840_IMR_CAL;
static UINT8 R840_SBY[R840_REG_NUM];
static UINT8 PulseCount = 0;
static UINT8 Pulse_LnaDisch = 10;
//0: L270n/68n(ISDB-T, DVB-T/T2)
//1: Bead/68n(DTMB)
//2: Bead/68n(ISDB-T, DVB-T/T2)
//3: L270n/68n(ATV)
//4: Bead/68n(DTMB+ATV)
//5: L270n/68n(ATSC, DVB-C, J83B)
//6: Bead/68n(ATSC, DVB-C, J83B)
UINT32 R840_LNA_HIGH_MID[R840_TF_SIZE] = { 612000, 612000, 612000, 612000, 612000, 500000, 500000}; 
UINT32 R840_LNA_MID_LOW[R840_TF_SIZE] = { 388000, 388000, 348000, 348000, 348000, 300000, 300000};
UINT32 R840_LNA_LOW_LOWEST[R840_TF_SIZE] = {164000, 164000, 148000, 124000, 124000, 156000, 156000};

UINT32 R840_TF_Freq_High[R840_TF_SIZE][R840_TF_HIGH_NUM] = 
{  	 { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
	 { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
	 { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
	 { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
	 { 784000, 776000, 744000, 712000, 680000, 648000, 616000},
     { 784000, 776000, 680000, 608000, 584000, 536000, 504000},
	 { 784000, 776000, 680000, 608000, 584000, 536000, 504000}
};
UINT32 R840_TF_Freq_Mid[R840_TF_SIZE][R840_TF_MID_NUM] = 
{	  {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
	  {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
	  {608000, 584000, 560000, 488000, 464000, 440000, 416000, 392000},
	  {608000, 560000, 488000, 464000, 440000, 416000, 392000, 352000},
	  {608000, 560000, 488000, 464000, 440000, 416000, 392000, 352000},
      {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000},
	  {488000, 464000, 440000, 416000, 392000, 352000, 320000, 304000}
};
UINT32 R840_TF_Freq_Low[R840_TF_SIZE][R840_TF_LOW_NUM] = 
{    {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
      {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
	  {320000, 304000, 272000, 240000, 232000, 216000, 192000, 168000},  //164~388
	  {256000, 240000, 232000, 192000, 160000, 152000, 144000, 128000},  //124~348
	  {264000, 240000, 192000, 184000, 176000, 152000, 144000, 128000},  //124~348
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000},  //156~300
      {280000, 248000, 232000, 216000, 192000, 176000, 168000, 160000}   //156~300
};
UINT32 R840_TF_Freq_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] = 
{    {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
      {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
	  {160000, 120000, 104000, 88000, 80000, 72000, 56000, 48000},
	  {120000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {104000, 96000,   88000,   80000, 72000, 64000, 56000, 48000},
	  {136000, 120000, 104000, 88000, 72000, 64000, 56000, 48000},
	  {128000, 120000, 104000, 96000, 80000, 72000, 64000, 56000}
};


UINT8 R840_TF_Result_High[R840_TF_SIZE][R840_TF_HIGH_NUM] = 
{    {0x00, 0x01, 0x03, 0x04, 0x08, 0x0A, 0x0A},
      {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
	  {0x00, 0x01, 0x03, 0x04, 0x08, 0x0A, 0x0A},
	  {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
	  {0x00, 0x01, 0x03, 0x04, 0x05, 0x07, 0x09},
      {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19},
	  {0x00, 0x01, 0x05, 0x0A, 0x0C, 0x13, 0x19}
};
UINT8 R840_TF_Result_Mid[R840_TF_SIZE][R840_TF_MID_NUM] = 
{    {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},
      {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},
	  {0x02, 0x03, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15},  
	  {0x02, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19},  
	  {0x02, 0x06, 0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19},
	  {0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19, 0x1E, 0x1F},
      {0x09, 0x0D, 0x0F, 0x12, 0x15, 0x19, 0x1E, 0x1F}
};
UINT8 R840_TF_Result_Low[R840_TF_SIZE][R840_TF_LOW_NUM] = 
{    {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0D, 0x13, 0x16},
      {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0B, 0x0F, 0x16},
	  {0x00, 0x02, 0x04, 0x07, 0x0A, 0x0D, 0x13, 0x16},
	  {0x05, 0x07, 0x0A, 0x0F, 0x1A, 0x1A, 0x23, 0x2A},
	  {0x05, 0x08, 0x10, 0x13, 0x1A, 0x1A, 0x23, 0x2A},
	  {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
	  {0x05, 0x08, 0x0C, 0x0E, 0x10, 0x14, 0x18, 0x1A},
};
UINT8 R840_TF_Result_Lowest[R840_TF_SIZE][R840_TF_LOWEST_NUM] = 
{    {0x00, 0x06, 0x0C, 0x15, 0x1C, 0x1F, 0x3C, 0x3F},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08},
	  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x08},
	  {0x06, 0x11, 0x15, 0x1C, 0x1F, 0x2F, 0x3C, 0x3F},
      {0x04, 0x08, 0x08, 0x08, 0x10, 0x12, 0x13, 0x13},
	  {0x06, 0x09, 0x0E, 0x18, 0x25, 0x2F, 0x3C, 0x3F},
	  {0x00, 0x04, 0x04, 0x08, 0x08, 0x10, 0x12, 0x13}
};

UINT8  R840_TF = 0;

static UINT16 R840_Lna_Plain_Acc_Gain[5][25] = 
{
 {0, 13, 25, 46, 65,  84, 102, 120, 137, 156, 176, 189, 204, 222, 237, 247, 255, 266, 280, 294, 307, 321, 326, 338, 338},
 {0, 15, 27, 57, 90, 116, 139, 161, 181, 204, 228, 237, 246, 263, 276, 286, 293, 303, 310, 317, 322, 327, 326, 331, 335},
 {0, 14, 31, 57, 80, 103, 124, 145, 164, 184, 206, 214, 224, 242, 256, 266, 274, 284, 291, 299, 305, 311, 307, 312, 317},
 {0,   0,  9,  31, 53,  74,  92, 110, 127, 147, 166, 176, 187, 205, 219, 229, 236, 247, 255, 264, 271, 277, 280, 285, 285},
 {0,   0, 15, 30, 45,  57,  71,  85,   97, 110, 122, 133, 147, 167, 182, 195, 204, 216, 228, 240, 251, 262, 264, 274, 275}
};

static UINT16 R840_Lna_Sharp_Acc_Gain[5][25] = 
{
 {105, 105, 105, 125, 145, 164, 182, 200, 218, 237, 258, 257, 258, 277, 291, 301, 309, 320, 320, 320, 320, 320, 326, 338, 338},
 { 82,   82,   84, 105, 135, 159, 180, 201, 223, 244, 268, 268, 270, 287, 300, 310, 318, 327, 327, 327, 327, 327, 326, 331, 335},
 {104, 105, 104, 120, 137, 157, 174, 193, 210, 229, 248, 248, 251, 269, 282, 293, 300, 310, 310, 310, 310, 311, 307, 312, 317},
 { 58,   60,   58,   80, 101, 122, 140, 159, 176, 195, 215, 215, 217, 235, 249, 259, 266, 277, 277, 277, 277, 277, 280, 285, 285},
 { 89,   88,   89, 102, 115, 128, 140, 153, 165, 177, 189, 190, 192, 212, 228, 240, 249, 262, 262, 262, 262, 262, 264, 274, 275}
};                                          

static UINT16 R840_Lna_Pulse_Comp[5][4] = 
{
	{  9, 25, 24, 24},
	{14, 40, 40, 39},
	{16, 35, 33, 31},
	{14, 21, 20, 20},
	{12, 18, 17, 17}
};
//----------------------------------------------------------//
//                   Internal static struct                         //
//----------------------------------------------------------//
static SysFreq_Info_Type  SysFreq_Info1;
static Sys_Info_Type         Sys_Info1;
static Freq_Info_Type       Freq_Info1;
//----------------------------------------------------------//
//                   Internal Functions                            //
//----------------------------------------------------------//
R840_ErrCode R840_TF_Check(void);
R840_ErrCode R840_Xtal_Check(void);
R840_ErrCode R840_InitReg(void);
R840_ErrCode R840_Cal_Prepare(UINT8 u1CalFlag);
R840_ErrCode R840_IMR(UINT8 IMR_MEM, UINT8 IM_Flag);
R840_ErrCode R840_PLL(UINT32 LO_Freq, R840_Standard_Type R840_Standard);
R840_ErrCode R840_MUX(UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard);
R840_ErrCode R840_IQ(R840_SectType* IQ_Pont);
R840_ErrCode R840_IQ_Tree(UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree);
R840_ErrCode R840_CompreCor(R840_SectType* CorArry);
R840_ErrCode R840_CompreStep(R840_SectType* StepArry, UINT8 Pace);
R840_ErrCode R840_Muti_Read(UINT8* IMR_Result_Data);
R840_ErrCode R840_Section(R840_SectType* SectionArry);
R840_ErrCode R840_F_IMR(R840_SectType* IQ_Pont);
R840_ErrCode R840_IMR_Cross(R840_SectType* IQ_Pont, UINT8* X_Direct);
R840_ErrCode R840_IMR_Iqcap(R840_SectType* IQ_Point);   
R840_ErrCode R840_SetTF(UINT32 u4FreqKHz, UINT8 u1TfType);
R840_ErrCode R840_SetStandard(R840_Standard_Type RT_Standard);
R840_ErrCode R840_SetFrequency(R840_Set_Info R840_INFO);

Sys_Info_Type R840_Sys_Sel(R840_Standard_Type R840_Standard);
Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard);
SysFreq_Info_Type R840_SysFreq_Sel(R840_Standard_Type R840_Standard,UINT32 RF_freq);

UINT8 R840_Filt_Cal_ADC(UINT32 IF_Freq, UINT8 R840_BW, UINT8 FilCal_Gap);


Sys_Info_Type R840_Sys_Sel(R840_Standard_Type R840_Standard)
{
	Sys_Info_Type R840_Sys_Info;

	switch (R840_Standard)
	{

	case R840_DVB_T_6M: 
	case R840_DVB_T2_6M: 
		R840_Sys_Info.IF_KHz=4570;                    //IF
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7150;          //CAL IF
		R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5 (2.0M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T_7M:  
	case R840_DVB_T2_7M:  
		R840_Sys_Info.IF_KHz=4570;                     //IF
		R840_Sys_Info.BW=0x40;                           //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7750;           //CAL IF
		R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8 (1.45M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T_8M: 
	case R840_DVB_T2_8M: 
		R840_Sys_Info.IF_KHz=4570;                     //IF
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8130;           //CAL IF
		R840_Sys_Info.HPF_COR=0x09;	             //R11[3:0]=9 (1.15M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T2_1_7M: 
		R840_Sys_Info.IF_KHz=1900;
		R840_Sys_Info.BW=0x40;                           //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7800;           //CAL IF
		R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
		break;

	case R840_DVB_T2_10M: 
		R840_Sys_Info.IF_KHz=5600;
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=10800;         //CAL IF
		R840_Sys_Info.HPF_COR=0x0C;	             //R11[3:0]=12
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
		break;

	case R840_DVB_C_8M:   
		R840_Sys_Info.IF_KHz=5070;
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF
		R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
		break;

	case R840_DVB_C_6M:  
		R840_Sys_Info.IF_KHz=5070;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=8025;          //CAL IF   
		R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0,  lna=max-1 & Buf 4, hpf+3
		break;

	case R840_J83B:  
		R840_Sys_Info.IF_KHz=5070;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7850;          //CAL IF  
		R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_ISDB_T_4063: 
		R840_Sys_Info.IF_KHz=4063;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7000;          //CAL IF  //7200
		R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_ISDB_T_4570:
		R840_Sys_Info.IF_KHz=4570;                    //IF
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7250;          //CAL IF
		R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5 (2.0M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DTMB_8M_4570: 
		R840_Sys_Info.IF_KHz=4570;
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF 
		R840_Sys_Info.HPF_COR=0x0B;	             //R11[3:0]=11
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_DTMB_6M_4500:
		R840_Sys_Info.IF_KHz=4500;
		R840_Sys_Info.BW=0x40;                           //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7000;           //CAL IF  
		R840_Sys_Info.HPF_COR=0x05;	             //R11[3:0]=5
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
		break;

	case R840_ATSC:  
		R840_Sys_Info.IF_KHz=5070;
		R840_Sys_Info.BW=0x40;                          //BW=7M  
		R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

    case R840_DVB_T_6M_IF_5M: 
	case R840_DVB_T2_6M_IF_5M: 
		R840_Sys_Info.IF_KHz=5000;                    //IF
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7400;          //CAL IF
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T_7M_IF_5M:  
	case R840_DVB_T2_7M_IF_5M:  
		R840_Sys_Info.IF_KHz=5000;                     //IF
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8300;           //CAL IF
		R840_Sys_Info.HPF_COR=0x07;	             //R11[3:0]=7 (1.67M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T_8M_IF_5M: 
	case R840_DVB_T2_8M_IF_5M: 
		R840_Sys_Info.IF_KHz=5000;                     //IF
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8500;           //CAL IF
		R840_Sys_Info.HPF_COR=0x08;	             //R11[3:0]=8 (1.45M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DVB_T2_1_7M_IF_5M: 
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x60;                           //BW=6M
		R840_Sys_Info.FILT_CAL_IF=5900;           //CAL IF
		R840_Sys_Info.HPF_COR=0x01;	             //R11[3:0]=1
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_DVB_C_8M_IF_5M:  
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=9000;           //CAL IF 
		R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x02;   //R30[1:0]=10, lna=max-1 & Buf 4, hpf+1
		break;

	case R840_DVB_C_6M_IF_5M:  
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=8100;          //CAL IF   
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_J83B_IF_5M:  
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7800;          //CAL IF  
		R840_Sys_Info.HPF_COR=0x03;	             //R11[3:0]=3 
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=1, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_ISDB_T_IF_5M: 
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x40;                          //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7700;          //CAL IF  
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4 (2.25M)
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x03;   //R30[1:0]=11, buf 8, hpf+1
		break;

	case R840_DTMB_8M_IF_5M: 
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8650;           //CAL IF  
		R840_Sys_Info.HPF_COR=0x09;	             //R11[3:0]=9
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_DTMB_6M_IF_5M:
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x40;                           //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7700;           //CAL IF  
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;
	
	case R840_ATSC_IF_5M:  
		R840_Sys_Info.IF_KHz=5000;
		R840_Sys_Info.BW=0x40;                          //BW=7M  
		R840_Sys_Info.FILT_CAL_IF=7900;          //CAL IF   
		R840_Sys_Info.HPF_COR=0x04;	             //R11[3:0]=4
		R840_Sys_Info.FILT_EXT_ENA=0x00;      //R11[4]=0, ext disable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	case R840_FM:  
		R840_Sys_Info.IF_KHz=2400;
		R840_Sys_Info.BW=0x40;                           //BW=7M
		R840_Sys_Info.FILT_CAL_IF=7200;           //CAL IF
		R840_Sys_Info.HPF_COR=0x02;	             //R11[3:0]=2
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	default:  //R840_DVB_T_8M
		R840_Sys_Info.IF_KHz=4570;                     //IF
		R840_Sys_Info.BW=0x00;                           //BW=8M
		R840_Sys_Info.FILT_CAL_IF=8330;           //CAL IF
		R840_Sys_Info.HPF_COR=0x0A;	             //R11[3:0]=10
		R840_Sys_Info.FILT_EXT_ENA=0x10;      //R11[4]=1, ext enable
		R840_Sys_Info.FILT_EXT_WIDEST=0x00;//R30[2]=0, ext normal
		R840_Sys_Info.FILT_EXT_POINT=0x00;   //R30[1:0]=0, lna=max-1 & Buf 4, hpf+3
		break;

	}


	//Set By DTV/ATV
	if (R840_Standard < R840_ATV_SIZE)  //ATV
	{
		R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2)
		R840_Sys_Info.NA_PWR_DET = 0x80;    //off         (R30[7]=1)
		R840_Sys_Info.SWBUF_CUR = 0x00;     //high  (R4[2]=0)
		R840_Sys_Info.SD_IM = 0x08;               //2       (R3[3:2]=2'b10)
	}
	else  //other DTV
	{
		R840_Sys_Info.SWCAP_CLK = 0x02;      //8k       (R18[1:0]=2) //AGC 500Hz map to 8k
		R840_Sys_Info.NA_PWR_DET = 0x00;   //on         (R30[7]=0)
		R840_Sys_Info.SWBUF_CUR = 0x04;     //low  (R4[2]=1)
		R840_Sys_Info.SD_IM = 0x00;              //0     (R3[3:2]=2'b00)
	}
	    
	R840_Sys_Info.FILT_CUR = 0x00;         //highest  (R10[6:5]=2'b00)
	R840_Sys_Info.RF_BUF_CUR = 0x00;    //high  (R4[7]=0)
	R840_Sys_Info.TF_CUR = 0x40;                  //low       (R3[6]=1)
	R840_Sys_Info.LNA_DET_MODE=0x01;    //discharge (R31[0]=1)            
	R840_Sys_Info.NA_DISCHARGE = 0x04;   //1          (R31[4:2]=3'b001)
	R840_Sys_Info.AGC_CLK = 0x00;              //1k        (R27[5:3]=3'b000)
	R840_Sys_Info.FILT_COMP = 0x20;      //1          (R30[6:5]=2'b01)

	//Filter 3dB
	switch(R840_Standard)
	{
		case R840_DVB_C_8M_IF_5M:
			R840_Sys_Info.FILT_3DB = 0x08;         // ON       (R30[3]=1)
		break;
	
		default: 
		    R840_Sys_Info.FILT_3DB = 0x00;         // OFF      (R30[3]=0)
		break;
	}

	//HPF_notch 
	if(R840_Standard==R840_DVB_T2_1_7M_IF_5M) 
		R840_Sys_Info.HPF_NOTCH = 0x10;          //low        (R6[4]=1)  
	else 
	    R840_Sys_Info.HPF_NOTCH = 0x00;          //high        (R6[4]=0)    

	//BW 1.7M
	if((R840_Standard==R840_DVB_T2_1_7M) || (R840_Standard==R840_FM))
		R840_Sys_Info.V17M = 0x80;       //enable, R11[7]=1
	else
	    R840_Sys_Info.V17M = 0x00;       //disable, R11[7]=0, (include DVBT2 1.7M IF=5MHz)

	//TF Type select
	switch(R840_Standard)
	{

		case R840_DVB_C_8M:
		case R840_DVB_C_6M:
		case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
		case R840_DVB_C_6M_IF_5M:
		case R840_J83B_IF_5M:
			 if(R840_DetectTfType==R840_UL_USING_BEAD)
			    R840_SetTfType = R840_TF_BEAD_LIN_ATSC;   //using bead
			 else
				R840_SetTfType = R840_TF_NARROW_ATSC;   //using 270n
			 break;	

		case R840_ATSC:  
		case R840_ATSC_IF_5M:  
			 if(R840_DetectTfType==R840_UL_USING_BEAD)
			    R840_SetTfType = R840_TF_BEAD_LIN_ATSC;  //using bead
			 else
			    R840_SetTfType = R840_TF_NARROW_ATSC;  //using 270n
			 break;

		case R840_DTMB_8M_4570:
		case R840_DTMB_6M_4500:
		case R840_DTMB_8M_IF_5M:
		case R840_DTMB_6M_IF_5M:
			if(R840_DetectTfType==R840_UL_USING_BEAD)
				R840_SetTfType = R840_TF_BEAD;		//Force using Bead
			else
				R840_SetTfType = R840_TF_NARROW;
			 break;

		default:		//DVB-T/T2, ISDB-T
			if(R840_DetectTfType==R840_UL_USING_BEAD)
				R840_SetTfType = R840_TF_BEAD_TERR;				
			else
				R840_SetTfType = R840_TF_NARROW;
			break;
	}


	return R840_Sys_Info;
}



Freq_Info_Type R840_Freq_Sel(UINT32 LO_freq, UINT32 RF_freq, R840_Standard_Type R840_Standard)
{
	Freq_Info_Type R840_Freq_Info;

	//----- RF dependent parameter --------
	//LNA band 
	if((RF_freq>0) && (RF_freq<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //<164
		 R840_Freq_Info.LNA_BAND = 0x60;   //R5[6:5]=2'b11; ultra low
	else if((RF_freq>=R840_LNA_LOW_LOWEST[R840_SetTfType]) && (RF_freq<R840_LNA_MID_LOW[R840_SetTfType]))  //164~388
		 R840_Freq_Info.LNA_BAND = 0x40;   //R5[6:5]=2'b10; low
	else if((RF_freq>=R840_LNA_MID_LOW[R840_SetTfType]) && (RF_freq<R840_LNA_HIGH_MID[R840_SetTfType]))  //388~612
		 R840_Freq_Info.LNA_BAND = 0x20;   //R5[6:5]=2'b01; mid
	else     // >612
		 R840_Freq_Info.LNA_BAND = 0x00;   //R5[6:5]=2'b00; high 
	
	//----- LO dependent parameter --------
	//IMR point 
	if((LO_freq>0) && (LO_freq<133000))  
         R840_Freq_Info.IMR_MEM = 0;   
	else if((LO_freq>=133000) && (LO_freq<221000))  
         R840_Freq_Info.IMR_MEM = 1;   
	else if((LO_freq>=221000) && (LO_freq<450000))  
		 R840_Freq_Info.IMR_MEM = 2;  
	else if((LO_freq>=450000) && (LO_freq<789000))  
		 R840_Freq_Info.IMR_MEM = 3;   
	else 
		 R840_Freq_Info.IMR_MEM = 4;

	//RF polyfilter band
	if((LO_freq>0) && (LO_freq<133000))  
         R840_Freq_Info.RF_POLY = 0x80;   //R25[7:6]=2'b10; low
	else if((LO_freq>=133000) && (LO_freq<221000))  
         R840_Freq_Info.RF_POLY = 0x40;   //R25[7:6]=2'b01; mid
	else if((LO_freq>=221000) && (LO_freq<789000))  
		 R840_Freq_Info.RF_POLY = 0x00;   //R25[7:6]=2'b00; highest    
	else
		 R840_Freq_Info.RF_POLY = 0xC0;   //R25[7:6]=2'b11; ultra high

	
	//LPF Cap, Notch
	switch(R840_Standard)
	{
		case R840_DVB_C_8M:
		case R840_DVB_C_6M:
		case R840_J83B:
        case R840_DVB_C_8M_IF_5M:
		case R840_DVB_C_6M_IF_5M:
		case R840_J83B_IF_5M:
			if((LO_freq>0) && (LO_freq<77000))  
			{
				R840_Freq_Info.LPF_CAP = 16;
				R840_Freq_Info.LPF_NOTCH = 10;
			}
			else if((LO_freq>=77000) && (LO_freq<85000))
			{
				R840_Freq_Info.LPF_CAP = 16;
				R840_Freq_Info.LPF_NOTCH = 4;
			}
			else if((LO_freq>=85000) && (LO_freq<115000))
			{
				R840_Freq_Info.LPF_CAP = 13;
				R840_Freq_Info.LPF_NOTCH = 3;
			}
			else if((LO_freq>=115000) && (LO_freq<125000))
			{
				R840_Freq_Info.LPF_CAP = 11;
				R840_Freq_Info.LPF_NOTCH = 1;
			}
			else if((LO_freq>=125000) && (LO_freq<141000))
			{
				R840_Freq_Info.LPF_CAP = 9;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=141000) && (LO_freq<157000))
			{
				R840_Freq_Info.LPF_CAP = 8;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=157000) && (LO_freq<181000))
			{
				R840_Freq_Info.LPF_CAP = 6;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=181000) && (LO_freq<205000))
			{
				R840_Freq_Info.LPF_CAP = 3;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else //LO>=201M
			{
				R840_Freq_Info.LPF_CAP = 0;
				R840_Freq_Info.LPF_NOTCH = 0;
			}

			break;

		default:  //Air, DTMB
			if((LO_freq>0) && (LO_freq<73000))  
			{
				R840_Freq_Info.LPF_CAP = 8;
				R840_Freq_Info.LPF_NOTCH = 10;
			}
			else if((LO_freq>=73000) && (LO_freq<81000))
			{
				R840_Freq_Info.LPF_CAP = 8;
				R840_Freq_Info.LPF_NOTCH = 4;
			}
			else if((LO_freq>=81000) && (LO_freq<89000))
			{
				R840_Freq_Info.LPF_CAP = 8;
				R840_Freq_Info.LPF_NOTCH = 3;
			}
			else if((LO_freq>=89000) && (LO_freq<121000))
			{
				R840_Freq_Info.LPF_CAP = 6;
				R840_Freq_Info.LPF_NOTCH = 1;
			}
			else if((LO_freq>=121000) && (LO_freq<145000))
			{
				R840_Freq_Info.LPF_CAP = 4;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=145000) && (LO_freq<153000))
			{
				R840_Freq_Info.LPF_CAP = 3;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=153000) && (LO_freq<177000))
			{
				R840_Freq_Info.LPF_CAP = 2;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else if((LO_freq>=177000) && (LO_freq<201000))
			{
				R840_Freq_Info.LPF_CAP = 1;
				R840_Freq_Info.LPF_NOTCH = 0;
			}
			else //LO>=201M
			{
				R840_Freq_Info.LPF_CAP = 0;
				R840_Freq_Info.LPF_NOTCH = 0;
			}

			break;

	}//end switch(standard)

	return R840_Freq_Info;

}



SysFreq_Info_Type R840_SysFreq_Sel(R840_Standard_Type R840_Standard,UINT32 RF_freq)
{
	SysFreq_Info_Type R840_SysFreq_Info;
	
	switch(R840_Standard)
	{

	case R840_DVB_T_6M:
	case R840_DVB_T_8M:
	case R840_DVB_T_6M_IF_5M:
	case R840_DVB_T_8M_IF_5M:

	if(R840_Chip==R836_R)
	{
		    R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)			
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
	}
	else
	{
			if((RF_freq>=782000) && (RF_freq<=790000)) 
			{
				R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
				R840_SysFreq_Info.LNA_VTH_L=0x52;		   // LNA VTH/L=0.84/0.54     (R13=0x52)
				R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
				R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
				R840_SysFreq_Info.RF_TOP=0x00;               // RF TOP=7                        (R26[7:5]=3'b000)
				R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
				R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
			}
			else
			{
				R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
				R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
				R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
				R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
				R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
				R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
				R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)			
			}
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
	
	}
		if((RF_freq>=350000) && (RF_freq<=540000))
		{
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
		}   
		break;

	case R840_DVB_T2_6M:
	case R840_DVB_T2_8M:
	case R840_DVB_T2_1_7M:
	case R840_DVB_T2_10M:
    case R840_DVB_T2_6M_IF_5M:
	case R840_DVB_T2_8M_IF_5M:
	case R840_DVB_T2_1_7M_IF_5M:

	if(R840_Chip==R836_R)
	{
#if(DTV_LTE_ONE_EQ==TRUE)
		if((RF_freq>=782000) && (RF_freq<=790000)) 
		{
			R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)    
			R840_SysFreq_Info.NRB_TOP=0xB0;            // Nrb TOP=4                       (R28[7:4]=4'b1011)
			R840_SysFreq_Info.NRB_TOP_ADDER=0;    //Nrb Adder: normal
		}
		else
#endif
		{
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)   
			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
		}
		    R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
//			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
//			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)			
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
//			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
	}
	else
	{
			if((RF_freq>=782000) && (RF_freq<=790000)) 
			{
				R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
				R840_SysFreq_Info.LNA_VTH_L=0x52;		   // LNA VTH/L=0.84/0.54     (R13=0x52)
				R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
				R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
				R840_SysFreq_Info.RF_TOP=0x00;               // RF TOP=7                        (R26[7:5]=3'b000)
				R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
				R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01)
			}
			else
			{
				R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
				R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
				R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0101)
				R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
				R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
				R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
				R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)			
			}
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
	
	}

		if((RF_freq>=350000) && (RF_freq<=540000))
		{
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
		}   
		break;

	case R840_DVB_T_7M:
	case R840_DVB_T_7M_IF_5M:
	case R840_DVB_T2_7M: 
	case R840_DVB_T2_7M_IF_5M:
		if(R840_Chip==R836_R)
		{
#if(DTV_VHF_ACI2_ENHANCE==TRUE)
			R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
			R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)                 
			R840_SysFreq_Info.NRB_TOP=0x30;            // Nrb TOP=12                       (R28[7:4]=4'b0011)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#else
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
			R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                     
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#endif
		}
		else if(R840_Chip==R836)
		{
#if(DTV_VHF_ACI2_ENHANCE==TRUE)
			R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
			R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)                 
			R840_SysFreq_Info.NRB_TOP=0x00;            // Nrb TOP=15                       (R28[7:4]=4'b0000)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
#else
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
			R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)			
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
					
			if((RF_freq>=164000) && (RF_freq<=216000)) 
			{
			   R840_SysFreq_Info.LNA_VTH_L=0xB6;	   // LNA VTH/L=1.44/0.94     (R13=0xB6)
			   R840_SysFreq_Info.NRB_TOP=0x20;         // Nrb TOP=13                     (R28[7:4]=4'b0010)
			   R840_SysFreq_Info.MIXER_TOP=0x04;	   // MIXER TOP=11               (R28[3:0]=4'b0100)
			}
#endif
		}
		break;

    case R840_DVB_C_8M:
	case R840_DVB_C_6M:	
	case R840_J83B:
	case R840_DVB_C_8M_IF_5M:
	case R840_DVB_C_6M_IF_5M:	
	case R840_J83B_IF_5M:
			if(RF_freq<165000)
			{
				R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
				R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74      (R13=0x94)
				R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
				R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
			}
			else if((RF_freq>=165000) && (RF_freq<350000))
			{
				R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
				R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
				R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
				R840_SysFreq_Info.NRB_TOP=0x90;            // Nrb TOP=6                       (R28[7:4]=4'b1001)
			}
			else if((RF_freq>=350000) && (RF_freq<540000))
			{
				R840_SysFreq_Info.LNA_TOP=0x04;		       // LNA TOP=3                    (R27[2:0]=3'b100)
				R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)
				R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
				R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			}
			else
			{
				R840_SysFreq_Info.LNA_TOP=0x04;		       // LNA TOP=3                    (R27[2:0]=3'b100)
				R840_SysFreq_Info.LNA_VTH_L=0x83;		   // LNA VTH/L=1.14/0.64     (R13=0x83)
				R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
				R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			}			
				R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
				R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
				R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)  		
				R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
				R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
				//R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
				R840_SysFreq_Info.RF_DISCHARGE=0xE0;    //RF disch=7 
		break;

	case R840_ISDB_T_4063:	
	case R840_ISDB_T_4570:	
	case R840_ISDB_T_IF_5M:	

	if(R840_Chip==R836_R)
	{
		if((RF_freq>=350000) && (RF_freq<=540000))
		{
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
		}
		else
		{
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
		}   
		R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
		R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
		R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
		R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
		R840_SysFreq_Info.NRB_TOP=0x80;            // Nrb TOP=7                       (R28[7:4]=4'b1000)
		R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11) 
		R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
		R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
		R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
	}
	else
	{
		if((RF_freq>=690000) && (RF_freq<=700000))
		{
			R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			R840_SysFreq_Info.LNA_VTH_L=0x62;		   // LNA VTH/L=0.94/0.54     (R13=0x62)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                     (R27[7:6]=2'b01) 
		}
		else
		{
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11) 
		}  
		R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
		R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
		R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
		
	}
		break;

	case R840_DTMB_8M_4570:
	case R840_DTMB_6M_4500:
	case R840_DTMB_8M_IF_5M:
	case R840_DTMB_6M_IF_5M:
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA5;	   // LNA VTH/L=1.34/0.84     (R13=0xA5)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)         
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		break;

	case R840_ATSC:
	case R840_ATSC_IF_5M:
       if(R840_DetectTfType==R840_UL_USING_BEAD)
	   {
		   if(RF_freq<88000)
		   {
		       R840_SysFreq_Info.LNA_TOP=0x03;	 	       // LNA TOP=4                    (R27[2:0]=3'b011)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)
			   R840_SysFreq_Info.RF_TOP=0xC0;               // RF TOP=1                        (R26[7:5]=3'b110)
			   R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=88000) && (RF_freq<104000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)		
			   R840_SysFreq_Info.RF_TOP=0xA0;               // RF TOP=2                        (R26[7:5]=3'b101)
			   R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=104000) && (RF_freq<156000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x30;             // Nrb TOP=12                    (R28[7:4]=4'b0011)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=156000) && (RF_freq<464000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=464000) && (RF_freq<500000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xB6;	       // LNA VTH/L=1.44/0.94     (R13=0xB6)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=685000) && (RF_freq<700000))
		   {
			   if(R840_Chip==R836_R)
			   {
   				   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
				   R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
			   }
			   else
			   {
	   		       R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
			   }			   
		   }
		   else if((RF_freq>=795000) && (RF_freq<805000))
		   {
			   	if(R840_Chip==R836)			     				 
			   {
	   		       R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
			   }		
			   else
			  {
				   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
			   }
		   }
		   else			  
		   {
				   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
	   }
	   else  //270n
	   {
		    if(RF_freq<88000)
		   {
		       R840_SysFreq_Info.LNA_TOP=0x02;	 	       // LNA TOP=5                    (R27[2:0]=3'b010)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
			   R840_SysFreq_Info.RF_TOP=0x80;               // RF TOP=3                        (R26[7:5]=3'b100)
			   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                    (R28[7:4]=4'b1100)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=88000) && (RF_freq<104000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=104000) && (RF_freq<248000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)	
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=248000) && (RF_freq<464000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xA5;	       // LNA VTH/L=1.34/0.84     (R13=0xA5)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
		   else if((RF_freq>=464000) && (RF_freq<500000))
		   {
			   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
			   R840_SysFreq_Info.LNA_VTH_L=0xB6;	       // LNA VTH/L=1.44/0.94     (R13=0xB6)		
			   R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
			   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
			   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }	
		   else if((RF_freq>=685000) && (RF_freq<700000))
		   {
			   if(R840_Chip==R836_R)
			   {
   				   R840_SysFreq_Info.LNA_TOP=0x02;		       // LNA TOP=5                    (R27[2:0]=3'b010)
				   R840_SysFreq_Info.LNA_VTH_L=0x93;	       // LNA VTH/L=1.24/0.64     (R13=0x93)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x60;    //RF disch=3                         (R31[7:5]=3'b011)
			   }
			   else
			   {
	   		       R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
			   }			   
		   }
		   else if((RF_freq>=795000) && (RF_freq<805000))
		   {
			   	if(R840_Chip==R836)			     				 
			   {
	   		       R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x62;	       // LNA VTH/L=0.94/0.54     (R13=0x62)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0xC0;             // Nrb TOP=3                      (R28[7:4]=4'b1100)
				   R840_SysFreq_Info.NRB_BW=0x40;             // Nrb BW=wide                   (R27[7:6]=2'b01)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=3                         (R31[7:5]=3'b001)
			   }		
			   else
			  {
				   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
			   }
		   }
		   else			  
		   {
				   R840_SysFreq_Info.LNA_TOP=0x01;		       // LNA TOP=6                    (R27[2:0]=3'b001)
				   R840_SysFreq_Info.LNA_VTH_L=0x94;	       // LNA VTH/L=1.24/0.74     (R13=0x94)		
				   R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)
				   R840_SysFreq_Info.NRB_TOP=0x90;             // Nrb TOP=6                      (R28[7:4]=4'b1001)
				   R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)
				   R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		   }
	   
	   }

	        R840_SysFreq_Info.MIXER_TOP=0x04;	       // MIXER TOP=11               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0xB7;   // MIXER VTH/L=1.44/1.04  (R14=0xB7)
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
//			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		break;

	case R840_FM:
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x60;               // RF TOP=4                        (R26[7:5]=3'b011)
			R840_SysFreq_Info.NRB_TOP=0x20;            // Nrb TOP=13                       (R28[7:4]=4'b0010)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)                                
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		break;

	default: //DVB-T 8M
			R840_SysFreq_Info.LNA_TOP=0x03;		       // LNA TOP=4                    (R27[2:0]=3'b011)
			R840_SysFreq_Info.LNA_VTH_L=0xA4;	   // LNA VTH/L=1.34/0.74     (R13=0xA4)
			R840_SysFreq_Info.MIXER_TOP=0x05;	       // MIXER TOP=10               (R28[3:0]=4'b0100)
			R840_SysFreq_Info.MIXER_VTH_L=0x95;   // MIXER VTH/L=1.24/0.84  (R14=0x95)
			R840_SysFreq_Info.RF_TOP=0x40;               // RF TOP=5                        (R26[7:5]=3'b010)                 
			R840_SysFreq_Info.NRB_TOP=0xC0;            // Nrb TOP=3                       (R28[7:4]=4'b1100)
			R840_SysFreq_Info.NRB_BW=0xC0;             // Nrb BW=lowest                  (R27[7:6]=2'b11)	                              
			R840_SysFreq_Info.LNA_DISCHARGE=0x0A; //LNA disch=10                   (R5[4:0]=5'b01010)
			R840_SysFreq_Info.NRB_TOP_ADDER=2;      //Nrb Adder: +6
			R840_SysFreq_Info.RF_DISCHARGE=0x20;    //RF disch=1                         (R31[7:5]=3'b001)
		break;
	
	} //end switch


	//TF LPF setting
	switch(R840_Standard)
	{
		case R840_DTMB_8M_4570:
		case R840_DTMB_6M_4500:
		case R840_DTMB_8M_IF_5M:
		case R840_DTMB_6M_IF_5M:
			 if(RF_freq<=196000)
 	             R840_SysFreq_Info.BYP_LPF = 0x04;      //low pass  (R1[2]=1)
			 else
				 R840_SysFreq_Info.BYP_LPF = 0x00;      //bypass  (R1[2]=0)
			      
		break;

		default:  //other standard
			 if(RF_freq<=236000)
 	             R840_SysFreq_Info.BYP_LPF = 0x04;      //low pass  (R1[2]=1)
			 else
				 R840_SysFreq_Info.BYP_LPF = 0x00;      //bypass  (R1[2]=0)

        break;
	}//end switch

	return R840_SysFreq_Info;
	
	}




R840_ErrCode R840_Init(HI_U32 u32TunerPort)
{
    UINT8 i;

    R836_I2C_SetPort(u32TunerPort);

	if(R840_Initial_done_flag==FALSE)
	{
		  //reset filter cal. data
		  for (i=0; i<R840_STD_SIZE; i++)
		  {	  
			  R840_Fil_Cal_flag[i] = FALSE;
			  R840_Fil_Cal_code[i] = 0;
			  R840_Fil_Cal_BW[i] = 0x00;
			  R840_Fil_Cal_LpfLsb[i] = 0;  
		  }

          if(R840_IMR_done_flag==FALSE)
		  {
	  		  //do UL TF check
			  //if(R840_InitReg() != RT_Success)        
				// return RT_Fail;

			  if(R840_TF_Check() != RT_Success)        
				 return RT_Fail;

			  //start IMR calibration
			  if(R840_InitReg() != RT_Success)        //write initial reg before doing IMR Cal
				 return RT_Fail;
    
			  if(R840_Cal_Prepare(R840_IMR_CAL) != RT_Success)     
				  return RT_Fail;

			  if(R840_IMR(3, TRUE) != RT_Success)       //Full K node 3
				return RT_Fail;

			  if(R840_IMR(0, FALSE) != RT_Success)
				return RT_Fail;

			  if(R840_IMR(1, FALSE) != RT_Success)
				return RT_Fail;

			  //if(R840_IMR(2, FALSE) != RT_Success)
				//return RT_Fail;
			  R840_IMR_Data[2].Gain_X  = R840_IMR_Data[1].Gain_X;
			  R840_IMR_Data[2].Phase_Y = R840_IMR_Data[1].Phase_Y;
			  R840_IMR_Data[2].Value = R840_IMR_Data[1].Value;
			  R840_IMR_Data[2].Iqcap = R840_IMR_Data[1].Iqcap;

			  if(R840_IMR(4, FALSE) != RT_Success)   
				return RT_Fail;

			  R840_IMR_done_flag = TRUE;
		  }

		  //do Xtal check
#if(R840_SHARE_XTAL_OUT==TRUE)
		  R840_Xtal_Pwr = XTAL_GM3V_STRONG;
#else
		  if(R840_InitReg() != RT_Success)        
			 return RT_Fail;

		  if(R840_Xtal_Check() != RT_Success)        
				   return RT_Fail;

		  if(R840_Xtal_Pwr_tmp==XTAL_GM3V_STRONG)
			  R840_Xtal_Pwr = XTAL_GM3V_STRONG;
		  else
			  R840_Xtal_Pwr = R840_Xtal_Pwr_tmp+1;
#endif

		  R840_Initial_done_flag = TRUE;

	} //end if(check init flag)

	//write initial reg
	if(R840_InitReg() != RT_Success)        
		return RT_Fail;

	R840_pre_standard = R840_STD_SIZE;

	return RT_Success;
}



R840_ErrCode R840_InitReg(void)
{
	UINT8 InitArrayCunt = 0;
	
	//Write Full Table, Set Xtal Power = highest at initial
	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = R840_REG_NUM;
	for(InitArrayCunt = 0; InitArrayCunt<R840_REG_NUM; InitArrayCunt ++)
	{
		R840_I2C_Len.Data[InitArrayCunt] = R840_iniArray[InitArrayCunt];
		R840_Array[InitArrayCunt] = R840_iniArray[InitArrayCunt];
	}
	if(R836_I2C_Write_Len(&R840_I2C_Len) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R840_ErrCode R840_TF_Check(void)
{
/*
	UINT32   RingVCO = 0;
	UINT32   RingFreq = 72000;
	UINT32   RingRef = R840_Xtal;
	UINT8     divnum_ring = 0;
	UINT8   VGA_Count = 0;
	UINT8   VGA_Read = 0;
*/
	if(R840_Initial_done_flag==FALSE)
	{
		R840_Init_Delay_MS(10);
	}
	else
	{
		R840_Delay_MS(10);
	}	

	//check status 
	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = 4;
	if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
	{
	    if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
            return RT_Fail;
	}	

	if((R840_I2C_Len.Data[3] & 0xE0) == 0x60)
		R840_Chip = R836;
	else if((R840_I2C_Len.Data[3] & 0xE0) == 0x40)
		R840_Chip = R836_R;
	else
        R840_Chip = R836;

	R840_DetectTfType = R840_UL_USING_270NH;
/*
	if(R840_Xtal==16000)  //16M	
         divnum_ring = 11;
	else if(R840_Xtal==24000)//24M
		 divnum_ring = 2;
	else if(R840_Xtal==27000)//27M
		 divnum_ring = 0;
	else
         divnum_ring = 11;
	
	 RingVCO = (16+divnum_ring)* 8 * RingRef;
	 RingFreq = RingVCO/48;

	 if(R840_Cal_Prepare(R840_TF_LNA_CAL) != RT_Success)      
	      return RT_Fail;

	 //Set Ring PLL, (R24, R25)
	 R840_I2C.RegAddr = 0x18;
	 R840_Array[24] = (R840_Array[24] & 0x00) | 0x80 | divnum_ring;    //pre div=6 & div_num
	 R840_I2C.Data = R840_Array[24];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	 R840_Array[25] = (R840_Array[25] & 0x00) | 0x8B;  //out div=8, RF poly=low band, power=min_lp
	 if(RingVCO>=3200000)
	    R840_Array[25] = (R840_Array[25] & 0xDF);   //vco_band=high, R25[5]=0
	 else
        R840_Array[25] = (R840_Array[25] | 0x20);      //vco_band=low, R25[5]=1

	 R840_I2C.RegAddr = 0x19;
	 R840_I2C.Data = R840_Array[25];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;	

     //Must do before PLL() 
	 if(R840_MUX(RingFreq + 5000, RingFreq, R840_STD_SIZE) != RT_Success)     //FilCal MUX (LO_Freq, RF_Freq)
	     return RT_Fail;

	 //Set PLL
	 if(R840_PLL((RingFreq + 5000), R840_STD_SIZE) != RT_Success)   //FilCal PLL
	       return RT_Fail;

	 //-----below must set after R840_MUX()-------//
	 //Set LNA TF=(1,15), Qctrl=off for VGA training
	 R840_I2C.RegAddr = 0x00;
     R840_Array[0] = 0x1F;  	
     R840_I2C.Data = R840_Array[0];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 // FilterComp OFF
	 R840_I2C.RegAddr = 0x0F;
     R840_Array[15] = R840_Array[15] & 0xFB;  	
     R840_I2C.Data = R840_Array[15];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 // Byp-LPF: Bypass
	 R840_I2C.RegAddr = 0x01;
     R840_Array[1] = R840_Array[1] & 0xFB;  	
     R840_I2C.Data = R840_Array[1];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

     //Adc=on; Vga code mode, Gain = -12dB  
     R840_I2C.RegAddr = 0x0C;
     R840_Array[12] = (R840_Array[12] & 0x00) | 0x10;
     R840_I2C.Data = R840_Array[12];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

	 // vcomp = 0
	 R840_I2C.RegAddr = 0x1E;
	 R840_Array[30] = (R840_Array[30] & 0x9F);	
	 R840_I2C.Data = R840_Array[30];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	 //------- increase VGA power to let ADC read value significant ---------//
	 for(VGA_Count=0; VGA_Count < 16; VGA_Count ++)
	 {
		R840_I2C.RegAddr = 0x0C;
		R840_I2C.Data = (R840_Array[12] & 0xF0) + VGA_Count;  
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
		
		if(R840_Initial_done_flag==FALSE)
		{
			R840_Init_Delay_MS(VGA_DELAY);
		}
		else
		{
			R840_Delay_MS(VGA_DELAY);
		}	
		
		if(R840_Muti_Read(&VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*ADC_READ_COUNT)
			break;
	 }

	 //Set LNA TF=(0,0)
	 R840_I2C.RegAddr = 0x00;
     R840_Array[0] = 0x00;  	
     R840_I2C.Data = R840_Array[0];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 if(R840_Initial_done_flag==FALSE)
	 {
		 R840_Init_Delay_MS(10);
	 }
	 else
	 {
		 R840_Delay_MS(10);
	 }	 

	 if(R840_Muti_Read(&VGA_Read) != RT_Success)
		  return RT_Fail;

	 if(VGA_Read > (36*ADC_READ_COUNT))
        R840_DetectTfType = R840_UL_USING_BEAD;
	 else
	    R840_DetectTfType = R840_UL_USING_270NH;
*/
	return RT_Success;
}

R840_ErrCode R840_Xtal_Check(void)
{
	UINT8 i = 0;

	// TF force sharp mode (for stable read)
	R840_I2C.RegAddr = 0x01;
	R840_Array[1] = R840_Array[1] | 0x01;    
	R840_I2C.Data = R840_Array[1];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	// NA det off (for stable read)
	R840_I2C.RegAddr = 0x1E;
	R840_Array[30] = R840_Array[30] | 0x80;  
	R840_I2C.Data = R840_Array[30];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//Xtal_pow1=low
	R840_I2C.RegAddr = 0x10;
	R840_Array[16] = (R840_Array[16] | 0x10);
	R840_I2C.Data = R840_Array[16];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//Xtal_pow0=highest  
	R840_I2C.RegAddr = 0x0F;
	R840_Array[15] = (R840_Array[15] & 0x9F);
	R840_I2C.Data = R840_Array[15];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//set pll autotune = 128kHz
	R840_I2C.RegAddr = 0x1D;
	R840_Array[29] = R840_Array[29] & 0x3F;
	R840_I2C.Data = R840_Array[29];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//set manual initial reg = 1 000000; b5=0 => cap 30p;
	R840_I2C.RegAddr = 0x13;
	R840_Array[19] = (R840_Array[19] & 0x80) | 0x40;
	R840_I2C.Data = R840_Array[19];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//set auto
	R840_I2C.RegAddr = 0x13;
	R840_Array[19] = (R840_Array[19] & 0xBF);
	R840_I2C.Data = R840_Array[19];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	for(i=0; i<XTAL_CHECK_SIZE; i++)  //from weak to strong
	{
		if(i==XTAL_GM3V_STRONG)  //
		{
		    R840_Array[15] = (R840_Array[15] & 0x9F) | 0x00;  //highest
			R840_Array[16] =  R840_Array[16] | 0x10;  //low
		}
		else
		{
			R840_Array[15] = (R840_Array[15] & 0x9F) | ((UINT8)(XTAL_NOR_HIGHEST-i)<<5); 
			R840_Array[16] =  R840_Array[16] & 0xEF;  //high
		}

		R840_I2C.RegAddr = 0x0F;
		R840_I2C.Data = R840_Array[15];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_I2C.RegAddr = 0x10;
		R840_I2C.Data = R840_Array[16];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
		
		if(R840_Initial_done_flag==FALSE)
		{
			R840_Init_Delay_MS(20);
		}
		else
		{
			R840_Delay_MS(20);
		}	

		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 3;
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
			if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
			   return RT_Fail;
		}

		//depend on init Nint & Div value (N=59.6667, Div=16)
		//lock to VCO band 8 if VCO=1845M for 16M Xtal
		//lock to VCO band 46 if VCO=2768M for 24M Xtal
		//lock to VCO band 53 if VCO=2919M for 27M Xtal
#if(R840_Xtal==16000)
		if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 14) && ((R840_I2C_Len.Data[2] & 0x3F) >= 2))  
#elif (R840_Xtal==24000)
		if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 51) && ((R840_I2C_Len.Data[2] & 0x3F) >= 39)) 
#elif (R840_Xtal==27000)
		if(((R840_I2C_Len.Data[2] & 0x40) == 0x40) && ((R840_I2C_Len.Data[2] & 0x3F) <= 59) && ((R840_I2C_Len.Data[2] & 0x3F) >= 47)) 
#endif
		{
		    R840_Xtal_Pwr_tmp =  i;  //lock
			break;
		}

		if(i==(XTAL_CHECK_SIZE-1))
		{
	        R840_Xtal_Pwr_tmp = i;
		}
	}


    return RT_Success;
}	

R840_ErrCode R840_Cal_Prepare(UINT8 u1CalFlag)
{
	 R840_Cal_Info_Type  Cal_Info;
 
	 switch(u1CalFlag)
	 {
	    case R840_IMR_CAL:
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
				Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB	OFF
				Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
				R840_IMR_Cal_Type = R840_IMR_CAL;
			break;
		case R840_IMR_LNA_CAL:						    
				Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x00;              //from RF_Buf ON, RF_Buf pwr on
				Cal_Info.LNA_POWER = 0xE0;             // LNA power OFF, TF cal ON, -6dB ON
				Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
				R840_IMR_Cal_Type = R840_IMR_LNA_CAL;
			break;
        case R840_TF_CAL: //TBD
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
				Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF		
				Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x9F;                  //manual: max
			break;
        case R840_TF_LNA_CAL:
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x00;              //from RF_Buf ON, RF_Buf pwr on
				Cal_Info.LNA_POWER = 0xE0;              //LNA power OFF, TF cal ON, -6dB ON		
				Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x80;                  //manual: min(0)
			break;
		case R840_LPF_CAL: 
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
				Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF		
				Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
				R840_IMR_Cal_Type = R840_LPF_CAL;
			break;
		case R840_LPF_LNA_CAL:
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x00;               //from RF_Buf ON, RF_Buf pwr on
				Cal_Info.LNA_POWER = 0xA0;              //LNA power OFF, TF cal ON, -6dB OFF		
				Cal_Info.MIXER_AMP_GAIN = 0x00;    //manual min(0)
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x80;                  //manual: min
			break;
		default:
			    Cal_Info.FILTER_6DB = 0x08;              //+6dB
				Cal_Info.RFBUF_OUT = 0x60;               //from RF_Buf ON, RF_Buf pwr off
				Cal_Info.LNA_POWER = 0x80;              //LNA power OFF, TF cal OFF, -6dB OFF		
				Cal_Info.MIXER_AMP_GAIN = 0x08;    //manual +8
				Cal_Info.MIXER_BUFFER_GAIN = 0x10; //manual min(0)
				Cal_Info.LNA_GAIN = 0x9F;                 //manual: max
	 }

	  //Ring From RF_Buf Output & RF_Buf Power
	  R840_I2C.RegAddr = 0x04;
      R840_Array[4] = (R840_Array[4] & 0xDF) | Cal_Info.RFBUF_OUT;  
      R840_I2C.Data = R840_Array[4];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail; 
  
	  //TF cal (LNA power ON/OFF , TF cal ON/OFF, TF_-6dB ON/OFF)
	  R840_I2C.RegAddr = 0x06;
      R840_Array[6] = (R840_Array[6] & 0x1F) | Cal_Info.LNA_POWER;
      R840_I2C.Data = R840_Array[6];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail; 

	  //LNA gain 
	  R840_I2C.RegAddr = 0x05;
	  R840_Array[5] = (R840_Array[5] & 0x60) | Cal_Info.LNA_GAIN;
      R840_I2C.Data = R840_Array[5];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail; 

	  //Mixer Amp Gain
	  R840_I2C.RegAddr = 0x07;
	  R840_Array[7] = (R840_Array[7] & 0xE0) | Cal_Info.MIXER_AMP_GAIN; 
      R840_I2C.Data = R840_Array[7];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	  //Mixer Buffer Gain
	  R840_I2C.RegAddr = 0x1A;
	  R840_Array[26] = (R840_Array[26] & 0xE0) | Cal_Info.MIXER_BUFFER_GAIN;  
      R840_I2C.Data = R840_Array[26];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;	 

	  // Set filter +0/6dB; NA det=OFF 
      R840_I2C.RegAddr  = 0x1E;
	  R840_Array[30] = (R840_Array[30] & 0xF7) | Cal_Info.FILTER_6DB | 0x80;
      R840_I2C.Data = R840_Array[30];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
	     return RT_Fail;

	 //---- General calibration setting ----//	 
	 //IMR IQ cap=0
	 R840_I2C.RegAddr = 0x03;
     R840_Array[3] = (R840_Array[3] & 0xFC);  
     R840_I2C.Data = R840_Array[3];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 // Set RF_Flag ON(%)
	 R840_I2C.RegAddr = 0x01;
     R840_Array[1] = R840_Array[1] | 0x01;  //force far-end mode
     R840_I2C.Data = R840_Array[1];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 //RingPLL power ON
     R840_I2C.RegAddr = 0x18;
     R840_Array[24] = (R840_Array[24] & 0xEF);
     R840_I2C.Data = R840_Array[24];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

	 //LPF filter code = 8
     R840_I2C.RegAddr = 0x0A;
     R840_Array[10] = (R840_Array[10] & 0xF0) | 0x08;
     R840_I2C.Data = R840_Array[10];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;
	 
     //HPF corner=1; LPF coarse=6M; 1.7M disable
     R840_I2C.RegAddr = 0x0B;
     R840_Array[11] = (R840_Array[11] & 0x00) | 0x61;
     R840_I2C.Data = R840_Array[11];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

     //ADC/VGA PWR on; Vga code mode(b4=1), Gain = 26.5dB; Large Code mode Gain(b5=1)
     R840_I2C.RegAddr = 0x0C;
     R840_Array[12] = (R840_Array[12] & 0x00) | 0x3B;  
     R840_I2C.Data = R840_Array[12];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

	 //LNA, RF, Nrb dector pw on; det2 cap=IF_det 
     R840_I2C.RegAddr = 0x1D;
     R840_Array[29] = (R840_Array[29] & 0xF0) | 0x02;
     R840_I2C.Data = R840_Array[29];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

      return RT_Success;
}




R840_ErrCode R840_IMR(UINT8 IMR_MEM, UINT8 IM_Flag)
{
	UINT32 RingVCO = 0;
	UINT32 RingFreq = 0;
	UINT8  u1MixerGain = 8;
	UINT32 RingRef = R840_Xtal;
	UINT8   divnum_ring = 0;

	R840_SectType IMR_POINT;

	R840_Array[24] &= 0x3F;    //clear ring_div1, R24[7:6] 
	R840_Array[25] &= 0xFC;   //clear ring_div2, R25[1:0] 

	if(R840_Xtal==16000)  //16M	
         divnum_ring = 9;
	else if(R840_Xtal==24000)//24M
		 divnum_ring = 1;
	else if(R840_Xtal==27000)//27M
		 divnum_ring = 0;
	else
         divnum_ring = 9;
	
	RingVCO = (16+divnum_ring)* 8 * RingRef;
	
	if(RingVCO>=3200000)
	    R840_Array[25] = (R840_Array[25] & 0xDF);   //vco_band=high, R25[5]=0
	else
        R840_Array[25] = (R840_Array[25] | 0x20);      //vco_band=low, R25[5]=1

	switch(IMR_MEM)
	{
	case 0: // RingFreq = 66.66M
		RingFreq = RingVCO/48;
		R840_Array[24] |= 0x80;  // ring_div1 /6 (2)
		R840_Array[25] |= 0x03;  // ring_div2 /8 (3)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  // ring_div_num = 9 for 16MHz RingRef
		u1MixerGain = 8;
		break;
	case 1: // RingFreq = 200M
		RingFreq = RingVCO/16;
		R840_Array[24] |= 0x00;  // ring_div1 /4 (0)
		R840_Array[25] |= 0x02;  // ring_div2 /4 (2)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  
		u1MixerGain = 6;
		break;
	case 2: // RingFreq = 400M
		RingFreq = RingVCO/8;
		R840_Array[24] |= 0x00;  // ring_div1 /4 (0)
		R840_Array[25] |= 0x01;  // ring_div2 /2 (1)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  
		u1MixerGain = 6;
		break;
	case 3: // RingFreq = 533.33M
		RingFreq = RingVCO/6;
		R840_Array[24] |= 0x80;  // ring_div1 /6 (2)
		R840_Array[25] |= 0x00;  // ring_div2 /1 (0)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  
		u1MixerGain = 8;
		break;
	case 4: // RingFreq = 800M
		RingFreq = RingVCO/4;
		R840_Array[24] |= 0x00;  // ring_div1 /4 (0)
		R840_Array[25] |= 0x00;  // ring_div2 /1 (0)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  
		u1MixerGain = 8;
		break;
	default:
		RingFreq = RingVCO/4;
		R840_Array[24] |= 0x00;  // ring_div1 /4 (0)
		R840_Array[25] |= 0x00;  // ring_div2 /1 (0)		
		//R840_Array[25] |= 0x00;  // vco_band = 0 (high)
        //R840_Array[24] |= divnum_ring;  
		u1MixerGain = 8;
		break;
	}

	  //Mixer Amp Gain
	  R840_I2C.RegAddr = 0x07;
	  R840_Array[7] = (R840_Array[7] & 0xE0) | u1MixerGain; 
      R840_I2C.Data = R840_Array[7];
      if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	//write I2C to set RingPLL
	R840_I2C.RegAddr = 0x18;
	R840_Array[24] = (R840_Array[24] & 0xF0) | divnum_ring;   //ring_div_num, R24[3:0]
	R840_I2C.Data = R840_Array[24];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	R840_I2C.RegAddr = 0x19;
	R840_I2C.Data = R840_Array[25];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//Ring PLL power
	if((RingFreq>0) && (RingFreq<R840_RING_POWER_FREQ))  
         R840_Array[25] = (R840_Array[25] & 0xF3) | 0x08;   //R25[3:2]=2'b10; min_lp
	else
        R840_Array[25] = (R840_Array[25] & 0xF3) | 0x00;   //R25[3:2]=2'b00; min

	R840_I2C.RegAddr = 0x19;
	R840_I2C.Data = R840_Array[25];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	//Must do MUX before PLL() 
	if(R840_MUX(RingFreq - R840_IMR_IF, RingFreq, R840_STD_SIZE) != RT_Success)      //IMR MUX (LO, RF)
		return RT_Fail;

	if(R840_PLL((RingFreq - R840_IMR_IF), R840_STD_SIZE) != RT_Success)  //IMR PLL
	    return RT_Fail;

	//Set TF, place after R840_MUX( )
	//TF is dependent to LNA/Mixer Gain setting
	if(R840_SetTF(RingFreq, (UINT8)R840_SetTfType) != RT_Success)
		return RT_Fail;

	//clear IQ_cap
	IMR_POINT.Iqcap = R840_Array[3] & 0xFC;

	if(IM_Flag == TRUE)
	{
	     if(R840_IQ(&IMR_POINT) != RT_Success)
		    return RT_Fail;
	}
	else
	{
		IMR_POINT.Gain_X = R840_IMR_Data[3].Gain_X;
		IMR_POINT.Phase_Y = R840_IMR_Data[3].Phase_Y;
		IMR_POINT.Value = R840_IMR_Data[3].Value;
		//IMR_POINT.Iqcap = R840_IMR_Data[3].Iqcap;
		if(R840_F_IMR(&IMR_POINT) != RT_Success)
			return RT_Fail;
	}

	//Save IMR Value
	switch(IMR_MEM)
	{
	case 0:
		R840_IMR_Data[0].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[0].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[0].Value = IMR_POINT.Value;
		R840_IMR_Data[0].Iqcap = IMR_POINT.Iqcap;		
		break;
	case 1:
		R840_IMR_Data[1].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[1].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[1].Value = IMR_POINT.Value;
		R840_IMR_Data[1].Iqcap = IMR_POINT.Iqcap;
		break;
	case 2:
		R840_IMR_Data[2].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[2].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[2].Value = IMR_POINT.Value;
		R840_IMR_Data[2].Iqcap = IMR_POINT.Iqcap;
		break;
	case 3:
		R840_IMR_Data[3].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[3].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[3].Value = IMR_POINT.Value;
		R840_IMR_Data[3].Iqcap = IMR_POINT.Iqcap;
		break;
	case 4:
		R840_IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[4].Value = IMR_POINT.Value;
		R840_IMR_Data[4].Iqcap = IMR_POINT.Iqcap;
		break;
    default:
		R840_IMR_Data[4].Gain_X  = IMR_POINT.Gain_X;
		R840_IMR_Data[4].Phase_Y = IMR_POINT.Phase_Y;
		R840_IMR_Data[4].Value = IMR_POINT.Value;
		R840_IMR_Data[4].Iqcap = IMR_POINT.Iqcap;
		break;
	}
	return RT_Success;
}


R840_ErrCode R840_PLL(UINT32 LO_Freq, R840_Standard_Type R840_Standard)
{
	UINT8  MixDiv = 2;
	UINT8  DivBuf = 0;
	UINT8  Ni = 0;
	UINT8  Si = 0;
	UINT8  DivNum = 0;
	UINT8  Nint = 0;
	UINT32 VCO_Min = 1730000; 
	UINT32 VCO_Max = VCO_Min*2;
	UINT32 VCO_Freq = 0;
	UINT32 PLL_Ref	= R840_Xtal;		
	UINT32 VCO_Fra	= 0;		
	UINT16 Nsdm = 2;
	UINT16 SDM = 0;
	UINT16 SDM16to9 = 0;
	UINT16 SDM8to1 = 0;
	UINT8   CP_CUR = 0x00;
	UINT8   CP_OFFSET = 0x00;
	UINT8   SDM_RES = 0x00;
	UINT8   XTAL_POW1 = 0x00;
	UINT8   XTAL_POW0 = 0x00;
	//UINT8   XTAL_GM = 0x00;
	UINT16 u2XalDivJudge;
	UINT8   u1XtalDivRemain;
	UINT8   VCO_current_trial = 0;
	UINT8   u1RfFlag = 0;
	UINT8   u1PulseFlag = 0;
	UINT8   CP_I2 = 0x00;
	UINT8   NS_RES = 0x00;

	//TF, NA fix
	u1RfFlag = (R840_Array[1] & 0x01);         //R1[0]
	u1PulseFlag = (R840_Array[30] & 0x80);   //R30[7]

	R840_I2C.RegAddr = 0x01;
	R840_Array[1] = R840_Array[1] | 0x01;     // TF force sharp mode
	R840_I2C.Data = R840_Array[1];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;
/*
	R840_I2C.RegAddr = 0x1E;
	R840_Array[30] = R840_Array[30] | 0x80;  // NA det off
	R840_I2C.Data = R840_Array[30];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;
*/
	//cp cur & offset setting
	if(R840_Standard < R840_ATV_SIZE) //ATV
	{
		//current
		if(LO_Freq < (48000+R840_IF_HIGH))
			CP_CUR = 0x10;        //0.5m, [5:3]=010     
		else if(LO_Freq < (64000+R840_IF_HIGH))
			CP_CUR = 0x10;        //0.5m, [5:3]=010        
		else if(LO_Freq < (80000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (96000+R840_IF_HIGH))
			CP_CUR = 0x28;        //0.2m, [5:3]=101       
		else if(LO_Freq < (112000+R840_IF_HIGH))
			CP_CUR = 0x28;        //0.2m, [5:3]=101      
		else if(LO_Freq < (128000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (176000+R840_IF_HIGH))
			CP_CUR = 0x18;        //0.4m, [5:3]=011
		else if(LO_Freq < (432000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (592000+R840_IF_HIGH))
			CP_CUR = 0x28;        //0.2m, [5:3]=101
		else if(LO_Freq < (744000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else if(LO_Freq < (752000+R840_IF_HIGH))
			CP_CUR = 0x20;        //0.3m, [5:3]=100
		else
			CP_CUR = 0x20;        //0.3m, [5:3]=100

		//CP_i2
		if(LO_Freq < (48000+R840_IF_HIGH))
			CP_I2 = 0x04;            //x2
		else if(LO_Freq < (64000+R840_IF_HIGH))
			CP_I2 = 0x00;               
		else if(LO_Freq < (96000+R840_IF_HIGH))
			CP_I2 = 0x04;           //x2    
		else
			CP_I2 = 0x00;    

        //offset
		if(LO_Freq < (88000+R840_IF_HIGH))
			CP_OFFSET = 0x04;  //30u,   [2]=1
		else if(LO_Freq < (96000+R840_IF_HIGH))
			CP_OFFSET = 0x00;  //0u,     [2]=0
		else if(LO_Freq < (160000+R840_IF_HIGH))
			CP_OFFSET = 0x04;  //30u,   [2]=1
		else if((LO_Freq < (240000+R840_IF_HIGH)) && (LO_Freq > (232000+R840_IF_HIGH)))
			CP_OFFSET = 0x04;  //30u,   [2]=1
		else if((LO_Freq < (272000+R840_IF_HIGH)) && (LO_Freq > (264000+R840_IF_HIGH)))
			CP_OFFSET = 0x04;  //30u,   [2]=1
        else 
			CP_OFFSET = 0x00;  //0u,     [2]=0

		if((LO_Freq < (64000+R840_IF_HIGH)) && (LO_Freq > (48000+R840_IF_HIGH)))
			NS_RES = 0x02;        //100R
		else
			NS_RES = 0x00;        //short
	}
	else  //DTV
	{
		if(LO_Freq < (96000+R840_IF_HIGH))	        
			CP_CUR = 0x28;        //0.2m, [5:3]=101
		else
			CP_CUR = 0x00;        //0.7m, [5:3]=000

		if((LO_Freq < (80000+R840_IF_HIGH)) && (LO_Freq > (72000+R840_IF_HIGH)))
            CP_OFFSET = 0x04;  //30u,   [2]=1
		else
			CP_OFFSET = 0x00;  //0u,     [2]=0

			CP_I2 = 0x00;
			NS_RES = 0x00;        //short
	}

	//CP ix2 & Ns_RES
	R840_I2C.RegAddr = 0x13;
	R840_Array[19] = (R840_Array[19] & 0xF9) | CP_I2 | NS_RES;  // CP_i2:b2, Ns_RES: b1
	R840_I2C.Data = R840_Array[19];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//CP current & Div Cuurent:100uA(10)
	R840_I2C.RegAddr = 0x11;
	R840_Array[17] = (R840_Array[17] & 0xC4) | 0x02 | CP_CUR;  // [5:3]=CP_CUR, [1:0]=2'b10
	R840_I2C.Data = R840_Array[17];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	// VCO current = 3(011) & CP Offset
	R840_I2C.RegAddr = 0x12;
	R840_Array[18] = (R840_Array[18] & 0x1B) | 0x60 | CP_OFFSET; // [7:5]=3'b011, [2]=offset
	R840_I2C.Data = R840_Array[18];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//set XTAL Power
#if(R840_SHARE_XTAL_OUT==TRUE)
			XTAL_POW1 = 0x10;        //low,       R16[4]=1
			XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00

			//R840_SetXtalIntCap(XTAL_CAP_SMALL);
#else
	if(R840_Initial_done_flag==TRUE)
	{

		    if(R840_Xtal_Pwr < XTAL_NOR_HIGH)
			{
				XTAL_POW1 = 0x00;        //high,       R16[4]=0
				XTAL_POW0 = 0x20;        //high,       R15[6:5]=01	     
			}
			else if(R840_Xtal_Pwr <= XTAL_NOR_HIGHEST)
			{
				XTAL_POW1 = 0x00;        //high,       R16[4]=0
				XTAL_POW0 = ((UINT8)(XTAL_NOR_HIGHEST-R840_Xtal_Pwr)<<5);      
			}
			else
			{
				XTAL_POW1 = 0x10;        //low,       R16[4]=1
				XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
			}

	}
	else
	{
		    XTAL_POW1 = 0x10;        //low,       R16[4]=1
			XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00
	}
#endif
	//set POW1 first to prevent dramatical swing drop when Inti_Reg(low/highest) then SetPllData
	R840_I2C.RegAddr = 0x10;
	R840_Array[16] = (R840_Array[16] & 0xEF) | XTAL_POW1; 
	R840_I2C.Data = R840_Array[16];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x0F;
	R840_Array[15] = (R840_Array[15] & 0x9F) | XTAL_POW0; 
	R840_I2C.Data = R840_Array[15];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	//IQ gen ON & current:Dmin, Bmin(00)
	R840_I2C.RegAddr = 0x17;
	R840_Array[23]    = (R840_Array[23] & 0xF8) | 0x00; // [2:1]=2'b00, [0]=1'b0
	R840_I2C.Data    = R840_Array[23];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//set pll autotune = 62.5kHz (fast)
	R840_I2C.RegAddr = 0x1D;
	R840_Array[29]    = R840_Array[29] & 0x3F;
	R840_I2C.Data    = R840_Array[29];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
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

	//SDM_Res
	if(MixDiv <= 4)  //Div=2,4
	{
		SDM_RES = 0x00;    //short, R19[4:3]=00
	}
	else
	{
		//SDM_RES = 0x18;   //400R, R19[4:3]=11
		SDM_RES = 0x10;   //200R, R19[4:3]=10
	}
	R840_I2C.RegAddr = 0x13;
	R840_Array[19]    = (R840_Array[19] & 0xE7) | SDM_RES; 
	R840_I2C.Data    = R840_Array[19];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;

	//Xtal Div setting
	if(R840_Standard == R840_STD_SIZE) //for cal, RingPLL not support xtal div2
	{
		    R840_XtalDiv = XTAL_DIV1;
	        R840_Array[16] = R840_Array[16] & 0xFB; //b2=0
	        PLL_Ref = R840_Xtal;	
	}
	else if(R840_Standard < R840_ATV_SIZE)    //for A normal operation
	{
		
	#if(R840_Xtal==16000)
		u2XalDivJudge = (UINT16) (LO_Freq/1000/8);
	#elif(R840_Xtal==24000)
		u2XalDivJudge = (UINT16) (LO_Freq/1000/12);
	#elif(R840_Xtal==27000)
		u2XalDivJudge = (UINT16) (LO_Freq/13500);
	#endif
		u1XtalDivRemain = (UINT8) (u2XalDivJudge % 2);
		if(u1XtalDivRemain==1) //odd
	   {
		    R840_XtalDiv = XTAL_DIV1;
	        R840_Array[16] = R840_Array[16] & 0xFB; //b2=0
	        PLL_Ref = R840_Xtal;	
		}
		else  // div2, note that agc clk also div2
		{
		    R840_XtalDiv = XTAL_DIV2;
	        R840_Array[16] |= 0x04;   //b2=1
	        PLL_Ref = R840_Xtal / 2;		
		}
	}
	else
	{
		if(LO_Freq < (432000+R840_IF_HIGH))
	   {
		   	R840_XtalDiv = XTAL_DIV2;
	        R840_Array[16] |= 0x04;   //b2=1
	        PLL_Ref = R840_Xtal / 2;		
		}
		else
		{
			R840_XtalDiv = XTAL_DIV1;
	        R840_Array[16] = R840_Array[16] & 0xFB; //b2=0
	        PLL_Ref = R840_Xtal;
		}
	}

	R840_I2C.RegAddr = 0x10;
	R840_I2C.Data = R840_Array[16];
    if(R836_I2C_Write(&R840_I2C) != RT_Success)
	   return RT_Fail;

	//Divider num
	R840_I2C.RegAddr = 0x10;
	R840_Array[16] &= 0x1F;
	R840_Array[16] |= (DivNum << 5);
	R840_I2C.Data = R840_Array[16];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	VCO_Freq = LO_Freq * MixDiv;
	Nint     = (UINT8) (VCO_Freq / 2 / PLL_Ref);
	VCO_Fra  = (UINT16) (VCO_Freq - 2 * PLL_Ref * Nint);

	//Boundary spur prevention
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

	//Ni & Si	
	Ni = (Nint - 13) / 4;
	Si = Nint - 4 *Ni - 13;
	R840_I2C.RegAddr = 0x14;
	R840_Array[20] = (Ni + (Si << 6));
	R840_I2C.Data = R840_Array[20];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	  return RT_Fail;
         	
	//pw_sdm	
	R840_I2C.RegAddr = 0x12;
	R840_Array[18] &= 0xF7;
	if(VCO_Fra == 0)
		R840_Array[18] |= 0x08;
	R840_I2C.Data = R840_Array[18];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
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

	R840_I2C.RegAddr = 0x16;
	R840_Array[22]    = (UINT8) SDM16to9;
	R840_I2C.Data    = R840_Array[22];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x15;
	R840_Array[21]    = (UINT8) SDM8to1;
	R840_I2C.Data    = R840_Array[21];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;


	if(R840_XtalDiv == XTAL_DIV2)
	{
		if(R840_Initial_done_flag==FALSE)
		{
			R840_Init_Delay_MS(20);
		}
		else
		{
	    	R840_Delay_MS(20);
		}
	}
	else
	{
		if(R840_Initial_done_flag==FALSE)
		{
			R840_Init_Delay_MS(10);
		}
		else
		{
	    	R840_Delay_MS(10);
		}
	}

	for(VCO_current_trial=0; VCO_current_trial<3; VCO_current_trial++)
	{
		//check PLL lock status 
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 3;
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
		    R836_I2C_Read_Len(&R840_I2C_Len);
		}		

		if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 ) 
		{
			R840_I2C.RegAddr = 0x12;
			R840_Array[18] = (R840_Array[18] & 0x1F) | ((2-VCO_current_trial) << 5);  //increase VCO current
			R840_I2C.Data = R840_Array[18];
			if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;
		}
		else
		{
		    break;
		}
	}

	if(VCO_current_trial>=2)
	{
		//check PLL lock status 
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 3;
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
		    R836_I2C_Read_Len(&R840_I2C_Len);
		}		

		if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 ) 
		{
			XTAL_POW1 = 0x10;        //low,       R16[4]=1
			XTAL_POW0 = 0x00;        //highest,   R15[6:5]=00

			R840_I2C.RegAddr = 0x0F;
			R840_Array[15] = (R840_Array[15] & 0x9F) | XTAL_POW0; 
			R840_I2C.Data = R840_Array[15];
			if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;

			R840_I2C.RegAddr = 0x10;
			R840_Array[16] = (R840_Array[16] & 0xEF) | XTAL_POW1; 
			R840_I2C.Data = R840_Array[16];
			if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;
		}
	}

	//set pll autotune = slow (2'b11)
	R840_I2C.RegAddr = 0x1D;
	R840_Array[29] = (R840_Array[29] & 0x3F) | 0xC0;
	R840_I2C.Data = R840_Array[29];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//restore TF, NA det setting
	R840_I2C.RegAddr = 0x01;
	R840_Array[1] = (R840_Array[1] & 0xFE) | u1RfFlag;     
	R840_I2C.Data = R840_Array[1];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;
/*
	R840_I2C.RegAddr = 0x1E;
	R840_Array[30] = (R840_Array[30] & 0x7F) | u1PulseFlag;  
	R840_I2C.Data = R840_Array[30];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;
*/
	return RT_Success;

}


R840_ErrCode R840_MUX(UINT32 LO_KHz, UINT32 RF_KHz, R840_Standard_Type R840_Standard)
{	
	UINT8 Reg08_IMR_Gain   = 0;
	UINT8 Reg09_IMR_Phase  = 0;
	UINT8 Reg03_IMR_Iqcap  = 0;

	Freq_Info1 = R840_Freq_Sel(LO_KHz, RF_KHz, R840_Standard);

	// LNA band (depend on RF_KHz)
	R840_I2C.RegAddr = 0x05;
	R840_Array[5] = (R840_Array[5] & 0x9F) | Freq_Info1.LNA_BAND;
	R840_I2C.Data = R840_Array[5];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	// RF Polyfilter
	R840_I2C.RegAddr = 0x19;
	R840_Array[25] = (R840_Array[25] & 0x3F) | Freq_Info1.RF_POLY;
	R840_I2C.Data = R840_Array[25];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	// LNA Cap
	R840_I2C.RegAddr = 0x01;
	R840_Array[1] = (R840_Array[1] & 0x07) | (Freq_Info1.LPF_CAP<<3);	
	R840_I2C.Data = R840_Array[1];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

if(R840_Chip==R836_R)
{
	// LNA Notch
	R840_I2C.RegAddr = 0x02;
	R840_Array[2] = (R840_Array[2] & 0xF0) | (Freq_Info1.LPF_NOTCH);
	R840_I2C.Data = R840_Array[2];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//Set_IMR
	if(R840_IMR_done_flag == TRUE)
	{
		Reg08_IMR_Gain = R840_IMR_Data[Freq_Info1.IMR_MEM].Gain_X & 0x2F;
		Reg09_IMR_Phase = R840_IMR_Data[Freq_Info1.IMR_MEM].Phase_Y & 0x2F;
		Reg03_IMR_Iqcap = R840_IMR_Data[Freq_Info1.IMR_MEM].Iqcap & 0x03;
	}
	else
	{
		Reg08_IMR_Gain = 0;
	    Reg09_IMR_Phase = 0;
		Reg03_IMR_Iqcap = 0;
	}

	R840_I2C.RegAddr = 0x08;                  
	R840_Array[8] = (R840_Array[8] & 0xD0) | Reg08_IMR_Gain;
	R840_I2C.Data = R840_Array[8];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
	R840_Array[9] = (R840_Array[9] & 0xD0) | Reg09_IMR_Phase;
	R840_I2C.Data =R840_Array[9]  ;
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
}
else
{
	// LNA Notch
	R840_I2C.RegAddr = 0x02;
	R840_Array[2] = (R840_Array[2] & 0xE0) | (Freq_Info1.LPF_NOTCH);
	R840_I2C.Data = R840_Array[2];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//Set_IMR
	if(R840_IMR_done_flag == TRUE)
	{
		Reg08_IMR_Gain = R840_IMR_Data[Freq_Info1.IMR_MEM].Gain_X & 0x3F;
		Reg09_IMR_Phase = R840_IMR_Data[Freq_Info1.IMR_MEM].Phase_Y & 0x3F;
		Reg03_IMR_Iqcap = R840_IMR_Data[Freq_Info1.IMR_MEM].Iqcap & 0x03;
	}
	else
	{
		Reg08_IMR_Gain = 0;
	    Reg09_IMR_Phase = 0;
		Reg03_IMR_Iqcap = 0;
	}

	R840_I2C.RegAddr = 0x08;                  
	R840_Array[8] = (R840_Array[8] & 0xC0) | Reg08_IMR_Gain;
	R840_I2C.Data = R840_Array[8];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
	R840_Array[9] = (R840_Array[9] & 0xC0) | Reg09_IMR_Phase;
	R840_I2C.Data =R840_Array[9]  ;
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
}
	R840_I2C.RegAddr = 0x03;
	R840_Array[3] = (R840_Array[3] & 0xFC) | Reg03_IMR_Iqcap;
	R840_I2C.Data =R840_Array[3]  ;
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R840_ErrCode R840_SetTF(UINT32 u4FreqKHz, UINT8 u1TfType)
{
    UINT8    u1FreqCount = 0;
	UINT32  u4Freq1 = 0;
	UINT32  u4Freq2 = 0;
	UINT32  u4Ratio;
	UINT8    u1TF_Set_Result1 = 0;
	UINT8    u1TF_Set_Result2 = 0;
	UINT8    u1TF_tmp1, u1TF_tmp2;
	//UINT8    u1TFCalNum = R840_TF_HIGH_NUM;

	if((u4FreqKHz>0) && (u4FreqKHz<R840_LNA_LOW_LOWEST[R840_SetTfType]))  //Ultra Low
	{
		 //u1TFCalNum = R840_TF_LOWEST_NUM;
         while((u4FreqKHz < R840_TF_Freq_Lowest[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_LOWEST_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Lowest[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_LOWEST_NUM)
        {
			 R840_TF = R840_TF_Result_Lowest[u1TfType][R840_TF_LOWEST_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Lowest[u1TfType][u1FreqCount-1]; 
		     u1TF_Set_Result2 = R840_TF_Result_Lowest[u1TfType][u1FreqCount]; 
		     u4Freq1 = R840_TF_Freq_Lowest[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Lowest[u1TfType][u1FreqCount]; 

			 //u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             //R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);

			 u1TF_tmp1 = ((u1TF_Set_Result1 & 0x40)>>2)*3 + (u1TF_Set_Result1 & 0x3F);  //b6 is 3xb4
			 u1TF_tmp2 = ((u1TF_Set_Result2 & 0x40)>>2)*3 + (u1TF_Set_Result2 & 0x3F);			 
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
			 R840_TF = u1TF_tmp1 + (UINT8)((u1TF_tmp2 - u1TF_tmp1)*u4Ratio/100);
			 if(R840_TF>=0x40)
				 R840_TF = (R840_TF + 0x10);

		 }
	}
	else if((u4FreqKHz>=R840_LNA_LOW_LOWEST[R840_SetTfType]) && (u4FreqKHz<R840_LNA_MID_LOW[R840_SetTfType]))  //Low
	{
		 //u1TFCalNum = R840_TF_LOW_NUM;
         while((u4FreqKHz < R840_TF_Freq_Low[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_LOW_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Low[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_LOW_NUM)
        {
			 R840_TF = R840_TF_Result_Low[u1TfType][R840_TF_LOW_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Low[u1TfType][u1FreqCount-1]; 
		     u1TF_Set_Result2 = R840_TF_Result_Low[u1TfType][u1FreqCount]; 
		     u4Freq1 = R840_TF_Freq_Low[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Low[u1TfType][u1FreqCount]; 

			 //u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             //R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);

			 u1TF_tmp1 = ((u1TF_Set_Result1 & 0x40)>>2) + (u1TF_Set_Result1 & 0x3F);  //b6 is 1xb4
			 u1TF_tmp2 = ((u1TF_Set_Result2 & 0x40)>>2) + (u1TF_Set_Result2 & 0x3F);			 
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
			 R840_TF = u1TF_tmp1 + (UINT8)((u1TF_tmp2 - u1TF_tmp1)*u4Ratio/100);
			 if(R840_TF>=0x40)
				 R840_TF = (R840_TF + 0x30);
		 }
	}
	else if((u4FreqKHz>=R840_LNA_MID_LOW[R840_SetTfType]) && (u4FreqKHz<R840_LNA_HIGH_MID[R840_SetTfType]))  //Mid
    {
		 //u1TFCalNum = R840_TF_MID_NUM;
         while((u4FreqKHz < R840_TF_Freq_Mid[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_MID_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_Mid[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_MID_NUM)
        {
			 R840_TF = R840_TF_Result_Mid[u1TfType][R840_TF_MID_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_Mid[u1TfType][u1FreqCount-1]; 
		     u1TF_Set_Result2 = R840_TF_Result_Mid[u1TfType][u1FreqCount]; 
		     u4Freq1 = R840_TF_Freq_Mid[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_Mid[u1TfType][u1FreqCount]; 
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);
		 }
	}
	else  //HIGH
	{
		 //u1TFCalNum = R840_TF_HIGH_NUM;
         while((u4FreqKHz < R840_TF_Freq_High[u1TfType][u1FreqCount]) && (u1FreqCount<R840_TF_HIGH_NUM))
		 {
            u1FreqCount++;
		 }

		 if(u1FreqCount==0)
		 {
			 R840_TF = R840_TF_Result_High[u1TfType][0];
		 }
		 else if(u1FreqCount==R840_TF_HIGH_NUM)
        {
			 R840_TF = R840_TF_Result_High[u1TfType][R840_TF_HIGH_NUM-1];
		 }
		 else
		 {
			 u1TF_Set_Result1 = R840_TF_Result_High[u1TfType][u1FreqCount-1]; 
		     u1TF_Set_Result2 = R840_TF_Result_High[u1TfType][u1FreqCount]; 
		     u4Freq1 = R840_TF_Freq_High[u1TfType][u1FreqCount-1];
		     u4Freq2 = R840_TF_Freq_High[u1TfType][u1FreqCount]; 
			 u4Ratio = (u4Freq1- u4FreqKHz)*100/(u4Freq1 - u4Freq2);
             R840_TF = u1TF_Set_Result1 + (UINT8)((u1TF_Set_Result2 - u1TF_Set_Result1)*u4Ratio/100);
		 }
	}
  

	R840_I2C.RegAddr = 0x00;
	R840_Array[0] = (R840_Array[0] & 0x80) | R840_TF;
	R840_I2C.Data = R840_Array[0]  ;
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

    return RT_Success;
}


R840_ErrCode R840_IQ(R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	//UINT8   VGA_Count = 0;
	//UINT8   VGA_Read = 0;
	UINT8   X_Direction;  // 1:X, 0:Y
		
	R840_I2C.RegAddr = 0x0C;
	R840_Array[12] = (R840_Array[12] & 0xF0) | 0x0F;  
	R840_I2C.Data = R840_Array[12];  
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;


	Compare_IQ[0].Gain_X  = R840_Array[8] & 0xC0; 
	Compare_IQ[0].Phase_Y = R840_Array[9] & 0xC0;
	//Compare_IQ[0].Iqcap = R840_iniArray[3] & 0xFC;

	    // Determine X or Y
	    if(R840_IMR_Cross(&Compare_IQ[0], &X_Direction) != RT_Success)
			return RT_Fail;

		if(X_Direction==1)
		{
			//compare and find min of 3 points. determine I/Q direction
		    if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
			  return RT_Fail;

		    //increase step to find min value of this direction
		    if(R840_CompreStep(&Compare_IQ[0], 0x08) != RT_Success)  //X
			  return RT_Fail;
		}
		else
		{
		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R840_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}

		//Another direction
		if(X_Direction==1)
		{	    
           if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
		     return RT_Fail;

		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		   	 return RT_Fail;

		   //increase step to find min value of this direction
		   if(R840_CompreStep(&Compare_IQ[0], 0x09) != RT_Success)  //Y
			 return RT_Fail;
		}
		else
		{
		   if(R840_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
		     return RT_Fail;
        
		   //compare and find min of 3 points. determine I/Q direction
		   if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		     return RT_Fail;

	       //increase step to find min value of this direction
		   if(R840_CompreStep(&Compare_IQ[0], 0x08) != RT_Success) //X
		     return RT_Fail;
		}
		

		//--- Check 3 points again---//
		if(X_Direction==1)
		{
		    if(R840_IQ_Tree(Compare_IQ[0].Phase_Y, Compare_IQ[0].Gain_X, 0x09, &Compare_IQ[0]) != RT_Success) //X
			  return RT_Fail;
		}
		else
		{
		   if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success) //Y
			return RT_Fail;
		}

		if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

    //Section-9 check
    //if(R840_F_IMR(&Compare_IQ[0]) != RT_Success)
	if(R840_Section(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

	//clear IQ_Cap = 0
	Compare_IQ[0].Iqcap = R840_Array[3] & 0xFC;

	if(R840_IMR_Iqcap(&Compare_IQ[0]) != RT_Success)
			return RT_Fail;

	*IQ_Pont = Compare_IQ[0];

	//reset gain/phase/iqcap control setting
	R840_I2C.RegAddr = 0x08;
	R840_Array[8] = R840_Array[8] & 0xC0;
	R840_I2C.Data = R840_Array[8];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
	R840_Array[9] = R840_Array[9] & 0xC0;
	R840_I2C.Data = R840_Array[9];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x03;
	R840_Array[3] = R840_Array[3] & 0xFC;
	R840_I2C.Data = R840_Array[3];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
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
R840_ErrCode R840_IQ_Tree(UINT8 FixPot, UINT8 FlucPot, UINT8 PotReg, R840_SectType* CompareTree)
{
	UINT8 TreeCunt  = 0;
	UINT8 TreeTimes = 3;
	UINT8 PntReg    = 0;
		
	if(PotReg == 0x08)
		PntReg = 0x09; //phase control
	else
		PntReg = 0x08; //gain control

	for(TreeCunt = 0;TreeCunt < TreeTimes;TreeCunt ++)
	{
		R840_I2C.RegAddr = PotReg;
		R840_I2C.Data    = FixPot;
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_I2C.RegAddr = PntReg;
		R840_I2C.Data    = FlucPot;
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		if(R840_Muti_Read(&CompareTree[TreeCunt].Value) != RT_Success)
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
			if((FlucPot & 0x1F) == 1) //if absolute location is 1, change I/Q direction
			{
				if(FlucPot & 0x20) //b[5]:I/Q selection. 0:Q-path, 1:I-path
				{
					FlucPot = (FlucPot & 0xC0) | 0x01;			
				}
				else
				{
					FlucPot = (FlucPot & 0xC0) | 0x21;
				}
			}
			else
				FlucPot = FlucPot - 2;  
				
		}
	}

	return RT_Success;
}




//-----------------------------------------------------------------------------------/ 
// Purpose: compare IMC result aray [0][1][2], find min value and store to CorArry[0]
// input: CorArry: three IMR data array
// output: TRUE or FALSE
//-----------------------------------------------------------------------------------/
R840_ErrCode R840_CompreCor(R840_SectType* CorArry)
{
	UINT8 CompCunt = 0;
	R840_SectType CorTemp;

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
R840_ErrCode R840_CompreStep(R840_SectType* StepArry, UINT8 Pace)
{
	R840_SectType StepTemp;
	
	//min value already saved in StepArry[0]
	StepTemp.Phase_Y = StepArry[0].Phase_Y;
	StepTemp.Gain_X  = StepArry[0].Gain_X;
	//StepTemp.Iqcap  = StepArry[0].Iqcap;

	while(((StepTemp.Gain_X & 0x1F) < R840_IMR_TRIAL) && ((StepTemp.Phase_Y & 0x1F) < R840_IMR_TRIAL))  
	{
		if(Pace == 0x08)
			StepTemp.Gain_X ++;
		else
			StepTemp.Phase_Y ++;
	
		R840_I2C.RegAddr = 0x08;
		R840_I2C.Data    = StepTemp.Gain_X ;
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		R840_I2C.RegAddr = 0x09;
		R840_I2C.Data    = StepTemp.Phase_Y;
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		if(R840_Muti_Read(&StepTemp.Value) != RT_Success)
			return RT_Fail;

		if(StepTemp.Value <= StepArry[0].Value)
		{
			StepArry[0].Gain_X  = StepTemp.Gain_X;
			StepArry[0].Phase_Y = StepTemp.Phase_Y;
			//StepArry[0].Iqcap = StepTemp.Iqcap;
			StepArry[0].Value   = StepTemp.Value;
		}
		else if((StepTemp.Value - 2*ADC_READ_COUNT) > StepArry[0].Value)
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
R840_ErrCode R840_Muti_Read(UINT8* IMR_Result_Data)
{
#if (ADC_MULTI_READ==3)
	UINT8 ReadCunt     = 0;
	UINT16 ReadAmount  = 0;
	UINT8 ReadMax = 0;
	UINT8 ReadMin = 255;
	UINT8 ReadData = 0;
	

	if(R840_Initial_done_flag==FALSE)
	{
		R840_Init_Delay_MS(ADC_READ_DELAY);//3
	}
	else
	{
		R840_Delay_MS(ADC_READ_DELAY);//3
	}

	for(ReadCunt = 0; ReadCunt < (ADC_READ_COUNT+2); ReadCunt ++)
	{
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 2;              // read 0x01
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
			R836_I2C_Read_Len(&R840_I2C_Len);
		}

		ReadData = (R840_I2C_Len.Data[1] & 0x3F);
		
		ReadAmount = ReadAmount + (UINT16)ReadData;
		
		if(ReadData < ReadMin)
			ReadMin = ReadData;
		
        if(ReadData > ReadMax)
			ReadMax = ReadData;
	}
	*IMR_Result_Data = (UINT8) (ReadAmount - (UINT16)ReadMax - (UINT16)ReadMin);
#else
	if(R840_Initial_done_flag==FALSE)
	{
		R840_Init_Delay_MS(ADC_READ_DELAY);//3
	}
	else
	{
		R840_Delay_MS(ADC_READ_DELAY);//3
	}

	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = 2;              // read 0x01
	if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
	{
		R836_I2C_Read_Len(&R840_I2C_Len);
	}
	*IMR_Result_Data = (R840_I2C_Len.Data[1] & 0x3F);
#endif

	return RT_Success;
}


R840_ErrCode R840_Section(R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	R840_SectType Compare_Bet[3];

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
	{
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	}
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
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

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R840_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}


R840_ErrCode R840_IMR_Cross(R840_SectType* IQ_Pont, UINT8* X_Direct)
{

	R840_SectType Compare_Cross[9]; //(0,0)(0,Q-1)(0,I-1)(Q-1,0)(I-1,0)+(0,Q-2)(0,I-2)(Q-2,0)(I-2,0)
	R840_SectType Compare_Temp;
	UINT8 CrossCount = 0;
	UINT8 Reg08 = R840_Array[8] & 0xC0;
	UINT8 Reg09 = R840_Array[9] & 0xC0;

	//memset(&Compare_Temp,0, sizeof(R840_SectType));
	Compare_Temp.Gain_X=0;
	Compare_Temp.Phase_Y=0;
	Compare_Temp.Iqcap=0;
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

    	R840_I2C.RegAddr = 0x08;
	    R840_I2C.Data    = Compare_Cross[CrossCount].Gain_X;
	    if(R836_I2C_Write(&R840_I2C) != RT_Success)
		   return RT_Fail;

	    R840_I2C.RegAddr = 0x09;
	    R840_I2C.Data    = Compare_Cross[CrossCount].Phase_Y;
	    if(R836_I2C_Write(&R840_I2C) != RT_Success)
		  return RT_Fail;
	
        if(R840_Muti_Read(&Compare_Cross[CrossCount].Value) != RT_Success)
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
R840_ErrCode R840_F_IMR(R840_SectType* IQ_Pont)
{
	R840_SectType Compare_IQ[3];
	R840_SectType Compare_Bet[3];
	//UINT8 VGA_Count = 0;
	//UINT8 VGA_Read = 0;

	R840_I2C.RegAddr = 0x0C;
	R840_Array[12] = (R840_Array[12] & 0xF0) | 0x0F;  
	R840_I2C.Data = R840_Array[12];  
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//Try X-1 column and save min result to Compare_Bet[0]
	if((IQ_Pont->Gain_X & 0x1F) == 0x00)
	{
		Compare_IQ[0].Gain_X = ((IQ_Pont->Gain_X) & 0xDF) + 1;  //Q-path, Gain=1
	}
	else
	{
		Compare_IQ[0].Gain_X  = IQ_Pont->Gain_X - 1;  //left point
	}
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)  // y-direction
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[0].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[0].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[0].Value = Compare_IQ[0].Value;

	//Try X column and save min result to Compare_Bet[1]
	Compare_IQ[0].Gain_X = IQ_Pont->Gain_X;
	Compare_IQ[0].Phase_Y = IQ_Pont->Phase_Y;

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
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

	if(R840_IQ_Tree(Compare_IQ[0].Gain_X, Compare_IQ[0].Phase_Y, 0x08, &Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	if(R840_CompreCor(&Compare_IQ[0]) != RT_Success)
		return RT_Fail;

	Compare_Bet[2].Gain_X = Compare_IQ[0].Gain_X;
	Compare_Bet[2].Phase_Y = Compare_IQ[0].Phase_Y;
	Compare_Bet[2].Value = Compare_IQ[0].Value;

	if(R840_CompreCor(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	//clear IQ_Cap = 0
	Compare_Bet[0].Iqcap = R840_Array[3] & 0xFC;

	if(R840_IMR_Iqcap(&Compare_Bet[0]) != RT_Success)
		return RT_Fail;

	*IQ_Pont = Compare_Bet[0];
	
	return RT_Success;
}


R840_ErrCode R840_IMR_Iqcap(R840_SectType* IQ_Point)   
{
    R840_SectType Compare_Temp;
	int i = 0;

	//Set Gain/Phase to right setting
	R840_I2C.RegAddr = 0x08;
	R840_I2C.Data = IQ_Point->Gain_X; 
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	R840_I2C.RegAddr = 0x09;
	R840_I2C.Data = IQ_Point->Phase_Y;
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	//try iqcap
	for(i=0; i<3; i++)
	{
		Compare_Temp.Iqcap = (UINT8) i;  
		R840_I2C.RegAddr = 0x03;
		R840_Array[3] = (R840_Array[3] & 0xFC) | Compare_Temp.Iqcap;  //**
		R840_I2C.Data = R840_Array[3];  
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			   return RT_Fail;

		if(R840_Muti_Read(&(Compare_Temp.Value)) != RT_Success)
			   return RT_Fail;

		if(Compare_Temp.Value < IQ_Point->Value)
		{
			IQ_Point->Value = Compare_Temp.Value; 
			IQ_Point->Iqcap = Compare_Temp.Iqcap;
		}
	}

    return RT_Success;
}


R840_ErrCode R840_GPIO(R840_GPIO_Type R840_GPIO_Conrl)
{
	//R19[0]=0 
	R840_I2C.RegAddr = 0x13;
	R840_Array[19] = R840_Array[19] & 0xFE;
	R840_I2C.Data = R840_Array[19];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

    //R15[0]
	if(R840_GPIO_Conrl == HI_SIG)
		R840_Array[15] |= 0x01;   //high
	else
		R840_Array[15] &= 0xFE;  //low

	R840_I2C.RegAddr = 0x0F;
	R840_I2C.Data = R840_Array[15];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}


R840_ErrCode R840_SetStandard(R840_Standard_Type RT_Standard)
{	 
	UINT8 u1FilCalGap = 8;

	Sys_Info1 = R840_Sys_Sel(RT_Standard);

	// Filter Calibration
	if(RT_Standard<R840_ATV_SIZE)    //ATV
	    u1FilCalGap = R840_Fil_Cal_Gap;
	else
	    u1FilCalGap = 8;   //? 13

    if(R840_Fil_Cal_flag[RT_Standard] == FALSE)
	{
		R840_Fil_Cal_code[RT_Standard] = R840_Filt_Cal_ADC(Sys_Info1.FILT_CAL_IF, Sys_Info1.BW, u1FilCalGap);
		R840_Fil_Cal_BW[RT_Standard] = R840_Bandwidth;
        R840_Fil_Cal_flag[RT_Standard] = TRUE;
        R840_Fil_Cal_LpfLsb[RT_Standard] = R840_Lpf_Lsb;  //respin

	    //Reset register and Array 
	    if(R840_InitReg() != RT_Success)        
		   return RT_Fail;
	}

	if(RT_Standard != R840_pre_standard)
	{
		 if(R840_InitReg() != RT_Success)      
		     return RT_Fail;

		 // Set TF current 
		R840_I2C.RegAddr = 0x03;
		R840_Array[3] = (R840_Array[3] & 0xBF) | Sys_Info1.TF_CUR;  
		R840_I2C.Data = R840_Array[3];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set Filter current 
		R840_I2C.RegAddr = 0x0A;
		R840_Array[10] = (R840_Array[10] & 0x9F) | Sys_Info1.FILT_CUR;  
		R840_I2C.Data = R840_Array[10];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set Switch Buffer current 
		R840_I2C.RegAddr = 0x04;
		R840_Array[4] = (R840_Array[4] & 0xFB) | Sys_Info1.SWBUF_CUR;   
		R840_I2C.Data = R840_Array[4];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set Filter Comp 
		R840_I2C.RegAddr = 0x1E;
		R840_Array[30] = (R840_Array[30] & 0x9F) | Sys_Info1.FILT_COMP;  
		R840_I2C.Data = R840_Array[30];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set sw cap clk
		R840_I2C.RegAddr = 0x12;
		R840_Array[18] = (R840_Array[18] & 0xFC) | Sys_Info1.SWCAP_CLK; 
		R840_I2C.Data = R840_Array[18];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set NA det power
		R840_I2C.RegAddr = 0x1E;
		R840_Array[30] = (R840_Array[30] & 0x7F) | Sys_Info1.NA_PWR_DET; 
		R840_I2C.Data = R840_Array[30];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	   // Set LNA det mode & NA_Discharge
		R840_I2C.RegAddr = 0x1F;	
		R840_Array[31] = (R840_Array[31] & 0xE2) | Sys_Info1.LNA_DET_MODE | Sys_Info1.NA_DISCHARGE; 
		R840_I2C.Data = R840_Array[31];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set AGC clk 
		R840_I2C.RegAddr = 0x1B;
		R840_Array[27] = (R840_Array[27] & 0xC7) | Sys_Info1.AGC_CLK;  
		R840_I2C.Data = R840_Array[27];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		// Set RF Buf Current
		R840_I2C.RegAddr = 0x04;
		R840_Array[4] = (R840_Array[4] & 0x7F) | Sys_Info1.RF_BUF_CUR;  
		R840_I2C.Data = R840_Array[4];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

 		// Set SD IM
		R840_I2C.RegAddr = 0x03;
		R840_Array[3] = (R840_Array[3] & 0xF3) | Sys_Info1.SD_IM;  
		R840_I2C.Data = R840_Array[3];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
	}
    R840_pre_standard = RT_Standard;

	// Set Filter Auto Ext  
	R840_I2C.RegAddr = 0x0B;
	R840_Array[11] = (R840_Array[11] & 0xEF) | Sys_Info1.FILT_EXT_ENA;  
	R840_I2C.Data = R840_Array[11];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	if(Sys_Info1.FILT_EXT_ENA==0x10)  //(%)
	{
			 if(R840_Fil_Cal_code[RT_Standard]< 2)  
			   R840_Fil_Cal_code[RT_Standard] = 2;
		
			 if((Sys_Info1.FILT_EXT_POINT & 0x02)==0x00)  //HPF+3
			 {
				  if(Sys_Info1.HPF_COR>12)  
				  {    Sys_Info1.HPF_COR = 12; }
			 }
			 else  //HPF+1
			 {
				  if(Sys_Info1.HPF_COR>14)  
				  {    Sys_Info1.HPF_COR = 15; }		 
			 }		  			
	}

if(R840_Chip==R836_R)
{
	// Set LPF Lsb bit
	R840_I2C.RegAddr = 0x06;
	R840_Array[6] = (R840_Array[6] & 0xF7) | (R840_Fil_Cal_LpfLsb[RT_Standard]<<3);  
	R840_I2C.Data = R840_Array[6];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
	    return RT_Fail;
}

	// Set LPF fine code
	R840_I2C.RegAddr = 0x0A;
	R840_Array[10] = (R840_Array[10] & 0xF0) | R840_Fil_Cal_code[RT_Standard];  
	R840_I2C.Data = R840_Array[10];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set LPF coarse BW
	R840_I2C.RegAddr = 0x0B;
	R840_Array[11] = (R840_Array[11] & 0x9F) | R840_Fil_Cal_BW[RT_Standard];
	R840_I2C.Data = R840_Array[11];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	// Set HPF corner & 1.7M mode
	R840_I2C.RegAddr = 0x0B;
	R840_Array[11] = (R840_Array[11] & 0x70) | Sys_Info1.HPF_COR | Sys_Info1.V17M;
	R840_I2C.Data = R840_Array[11];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

   // Set Filter 3dB
	R840_I2C.RegAddr = 0x1E;
	R840_Array[30] = (R840_Array[30] & 0xF7) | Sys_Info1.FILT_3DB;  
	R840_I2C.Data = R840_Array[30];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set Filter Ext Condition (%)
	R840_I2C.RegAddr = 0x1E;
    R840_Array[30] = (R840_Array[30] & 0xF8) | 0x04 | Sys_Info1.FILT_EXT_POINT;   //R30[1]:hp_sel; R30[0]:ext_cond
	R840_I2C.Data = R840_Array[30];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

if(R840_Chip==R836_R)
{
	//Set HPF notch
	R840_I2C.RegAddr = 0x06;
	R840_Array[6] = (R840_Array[6] & 0xEF) | Sys_Info1.HPF_NOTCH;  
	R840_I2C.Data = R840_Array[6];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
}

	// Record SW Version 
	R840_I2C.RegAddr = 0x0C;
	R840_Array[12] = (R840_Array[12] & 0xF0) | VER_NUM;  
    R840_I2C.Data = R840_Array[12];
    if(R836_I2C_Write(&R840_I2C) != RT_Success)
       return RT_Fail;

	// Record init flag
	R840_I2C.RegAddr = 0x1A;
	R840_Array[26] = ((R840_Array[26] & 0xF0) | (R840_Initial_done_flag<<1) | (R840_Fil_Cal_flag[RT_Standard]<<2) 
		| R840_IMR_done_flag | (R840_DetectTfType<<3));  
    R840_I2C.Data = R840_Array[26];
    if(R836_I2C_Write(&R840_I2C) != RT_Success)
       return RT_Fail;
    
	if(R840_Chip==R836_R)  
	{
		//disable IF_AGC read, R8[7]=0
		R840_I2C.RegAddr = 0x08;
		R840_Array[8] = R840_Array[8] & 0x7F;
		R840_I2C.Data = R840_Array[8];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

		//ADC power off, R12[7]=1
		R840_I2C.RegAddr = 0x0C;
		R840_Array[12] = R840_Array[12] | 0x80;
		R840_I2C.Data = R840_Array[12];
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
	}

	return RT_Success;
}


UINT8  R840_Filt_Cal_ADC(UINT32 IF_Freq, UINT8 R840_BW, UINT8 FilCal_Gap)
{
	 UINT8     u1FilterCodeResult = 0;
	 UINT8     u1FilterCode = 0;
	 UINT8     u1FilterCalValue = 0;
	 UINT8     u1FilterCalValuePre = 0;
	 UINT8     initial_cnt = 0;
	 UINT8     i = 0;
	 UINT32   RingVCO = 0;
	 UINT32   RingFreq = 72000;
	 UINT32   RingRef = R840_Xtal;
	 UINT8     divnum_ring = 0;
	 UINT8   VGA_Count = 0;
	 UINT8   VGA_Read = 0;

	if(R840_Xtal==16000)  //16M	
         divnum_ring = 11;
	else if(R840_Xtal==24000)//24M
		 divnum_ring = 2;
	else if(R840_Xtal==27000)//27M
		 divnum_ring = 0;
	else
         divnum_ring = 11;

	 RingVCO = (16+divnum_ring)* 8 * RingRef;
	 RingFreq = RingVCO/48;

	 //Write initial reg before doing calibration 
	 if(R840_InitReg() != RT_Success)        
		return RT_Fail;

	 if(R840_Cal_Prepare(R840_LPF_CAL) != RT_Success)      
	      return RT_Fail;

	 //Set Ring PLL, (R24, R25)
	 R840_I2C.RegAddr = 0x18;
	 R840_Array[24] = (R840_Array[24] & 0x00) | 0x80 | divnum_ring;    //pre div=6 & div_num
	 R840_I2C.Data = R840_Array[24];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	 R840_Array[25] = (R840_Array[25] & 0x00) | 0x8B;  //out div=8, RF poly=low band, power=min_lp
	 if(RingVCO>=3200000)
	    R840_Array[25] = (R840_Array[25] & 0xDF);   //vco_band=high, R25[5]=0
	 else
        R840_Array[25] = (R840_Array[25] | 0x20);      //vco_band=low, R25[5]=1

	 R840_I2C.RegAddr = 0x19;
	 R840_I2C.Data = R840_Array[25];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;	


     //Must do before PLL() 
	 if(R840_MUX(RingFreq + IF_Freq, RingFreq, R840_STD_SIZE) != RT_Success)     //FilCal MUX (LO_Freq, RF_Freq)
	     return RT_Fail;

	 //Set PLL
	 if(R840_PLL((RingFreq + IF_Freq), R840_STD_SIZE) != RT_Success)   //FilCal PLL
	       return RT_Fail;

	 //-----below must set after R840_MUX()-------//
	 //Set LNA TF for RF=72MHz. no use
	 if((R840_SetTfType==R840_TF_NARROW) || (R840_SetTfType==R840_TF_NARROW_ATSC))   //UL use 270n setting
	 {
	    R840_I2C.RegAddr = 0x00;
        R840_Array[0] = (R840_Array[0] & 0x80) | 0x1F;  
	 }
	 else
	 {
	    R840_I2C.RegAddr = 0x00;
        R840_Array[0] = (R840_Array[0] & 0x80) | 0x00;  
	 }
     R840_I2C.Data = R840_Array[0];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

     //Adc=on; Vga code mode, Gain = -12dB  
     R840_I2C.RegAddr = 0x0C;
     R840_Array[12] = (R840_Array[12] & 0x00) | 0x10;
     R840_I2C.Data = R840_Array[12];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
           return RT_Fail;

	 // vcomp = 0
	 R840_I2C.RegAddr = 0x1E;
	 R840_Array[30] = (R840_Array[30] & 0x9F);	
	 R840_I2C.Data = R840_Array[30];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;
	
	 //Set BW=8M, HPF corner narrowest; 1.7M disable
     R840_I2C.RegAddr = 0x0B;	
	 R840_Array[11] = (R840_Array[11] & 0x00);	  
     R840_I2C.Data = R840_Array[11];		
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;	

	 //------- increase VGA power to let ADC read value significant ---------//
	 R840_I2C.RegAddr = 0x0A;
     R840_Array[10] = (R840_Array[10] & 0xF0) | 0;  //filter code=0
     R840_I2C.Data = R840_Array[10];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
          return RT_Fail;

	 for(VGA_Count=0; VGA_Count < 16; VGA_Count ++)
	 {
		R840_I2C.RegAddr = 0x0C;
		R840_I2C.Data    = (R840_Array[12] & 0xF0) + VGA_Count;  
		if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
		
		if(R840_Initial_done_flag==FALSE)
		{
			R840_Init_Delay_MS(VGA_DELAY);
		}
		else
		{
	    		R840_Delay_MS(VGA_DELAY);
		}
			
		if(R840_Muti_Read(&VGA_Read) != RT_Success)
			return RT_Fail;

		if(VGA_Read > 40*ADC_READ_COUNT)
			break;
	 }

	 //------- Try suitable BW --------//

	 if(R840_BW==0x60) //6M
         initial_cnt = 1;  //try 7M first
	 else
		 initial_cnt = 0;  //try 8M first

	 for(i=initial_cnt; i<3; i++)
	 {
         if(i==0)
             R840_Bandwidth = 0x00; //8M
		 else if(i==1)
			 R840_Bandwidth = 0x40; //7M
		 else
			 R840_Bandwidth = 0x60; //6M

		 R840_I2C.RegAddr = 0x0B;	
	     R840_Array[11] = (R840_Array[11] & 0x00) | R840_Bandwidth;	  
         R840_I2C.Data = R840_Array[11];		
         if(R836_I2C_Write(&R840_I2C) != RT_Success)
		      return RT_Fail;	

		 // read code 0
		 R840_I2C.RegAddr = 0x0A;
		 R840_Array[10] = (R840_Array[10] & 0xF0) | 0;  //code 0
		 R840_I2C.Data = R840_Array[10];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			  return RT_Fail;
	 
		 if(R840_Initial_done_flag==FALSE)
		 {
			 R840_Init_Delay_MS(FILTER_DELAY);
		 }
		 else
		 {
			 R840_Delay_MS(FILTER_DELAY);
		 }
	     
		 if(R840_Muti_Read(&u1FilterCalValuePre) != RT_Success)
			  return RT_Fail;

		 //read code 13
		 R840_I2C.RegAddr = 0x0A;
		 R840_Array[10] = (R840_Array[10] & 0xF0) | 13;  //code 13
		 R840_I2C.Data = R840_Array[10];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			  return RT_Fail;

		 if(R840_Initial_done_flag==FALSE)
		 {
			 R840_Init_Delay_MS(FILTER_DELAY);
		 }
		 else
		 {
			 R840_Delay_MS(FILTER_DELAY);
		 }
	     
		 if(R840_Muti_Read(&u1FilterCalValue) != RT_Success)
			  return RT_Fail;

		 if(u1FilterCalValuePre > (u1FilterCalValue+8))  //suitable BW found
			 break;
	 }

     //-------- Try LPF filter code ---------//
	 u1FilterCalValuePre = 0;
	 for(u1FilterCode=0; u1FilterCode<16; u1FilterCode++)
	 {
         R840_I2C.RegAddr = 0x0A;
         R840_Array[10] = (R840_Array[10] & 0xF0) | u1FilterCode;
         R840_I2C.Data = R840_Array[10];
         if(R836_I2C_Write(&R840_I2C) != RT_Success)
              return RT_Fail;
		 
		 if(R840_Initial_done_flag==FALSE)
		 {
			 R840_Init_Delay_MS(FILTER_DELAY);
		 }
		 else
		 {
			 R840_Delay_MS(FILTER_DELAY);
		 }

		 if(R840_Muti_Read(&u1FilterCalValue) != RT_Success)
		      return RT_Fail;

		 if(u1FilterCode==0)
              u1FilterCalValuePre = u1FilterCalValue;

		 if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
		 {
			 u1FilterCodeResult = u1FilterCode;
			  break;
		 }

	 }

	 if(R840_Chip==R836_R)  
	 {
		 if(u1FilterCodeResult>0)   //try code-1 & lsb=1
		 {
 			 //extra LSB bit, R6[3]=1
			 R840_I2C.RegAddr = 0x06;
			 R840_Array[6] = (R840_Array[6] | 0x08);  
			 R840_I2C.Data = R840_Array[6];
			 if(R836_I2C_Write(&R840_I2C) != RT_Success)
				  return RT_Fail;

			 R840_I2C.RegAddr = 0x0A;
			 R840_Array[10] = (R840_Array[10] & 0xF0) | (u1FilterCodeResult-1);
			 R840_I2C.Data = R840_Array[10];
			 if(R836_I2C_Write(&R840_I2C) != RT_Success)
				  return RT_Fail;

		 	if(R840_Initial_done_flag==FALSE)
			{
				R840_Init_Delay_MS(10);//3
			}
			else
			{
				R840_Delay_MS(10);//3
			}


			 if(R840_Muti_Read(&u1FilterCalValue) != RT_Success)
				  return RT_Fail;

			 if((u1FilterCalValue+FilCal_Gap*ADC_READ_COUNT) < u1FilterCalValuePre)
			 {
				 u1FilterCodeResult = u1FilterCodeResult - 1;
				 R840_Lpf_Lsb = 1;
			 }
			 else
			 {
			 	 //u1FilterCodeResult = u1FilterCodeResult;
				 R840_Lpf_Lsb = 0;
			 }
		 }
     }

	 if(u1FilterCode==16)
          u1FilterCodeResult = 15;

	  return u1FilterCodeResult;

}

R840_ErrCode R840_SetFrequency(R840_Set_Info R840_INFO)
{

	 UINT32	LO_KHz;

     // Check Input Frequency Range
	 if((R840_INFO.RF_KHz<40000) || (R840_INFO.RF_KHz>1002000))
	 {
	          return RT_Fail;
	 }

	 if(R840_INFO.R840_Standard==R840_SECAM_L1_INV)
		  LO_KHz = R840_INFO.RF_KHz - Sys_Info1.IF_KHz;
	 else
	      LO_KHz = R840_INFO.RF_KHz + Sys_Info1.IF_KHz;

	 //Set MUX dependent var. Must do before PLL( ) 
     if(R840_MUX(LO_KHz, R840_INFO.RF_KHz, R840_INFO.R840_Standard) != RT_Success)   //normal MUX
        return RT_Fail;

     //Set PLL
     if(R840_PLL(LO_KHz, R840_INFO.R840_Standard) != RT_Success) //noraml PLL
        return RT_Fail;

	 //Set TF
	 if(R840_SetTF(R840_INFO.RF_KHz, R840_SetTfType) != RT_Success)
		return RT_Fail;

     R840_IMR_point_num = Freq_Info1.IMR_MEM;

	 //Q1.5K
	 if(R840_INFO.RF_KHz<R840_LNA_MID_LOW[R840_TF_NARROW])
         R840_Array[0] = R840_Array[0] | 0x80;   //on
	 else
		 R840_Array[0] = R840_Array[0] & 0x7F; //off
	 R840_I2C.RegAddr = 0x00;
	 R840_I2C.Data = R840_Array[0];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	 //set Xtal AAC OFF			 
	 R840_I2C.RegAddr = 0x04;
	 R840_Array[4] = R840_Array[4] & 0xFD;   //OFF
	 R840_I2C.Data = R840_Array[4];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;


     //Get Sys-Freq parameter
     SysFreq_Info1 = R840_SysFreq_Sel(R840_INFO.R840_Standard, R840_INFO.RF_KHz);

	
     // Set LNA_TOP
	 R840_I2C.RegAddr = 0x1B;
     R840_Array[27] = (R840_Array[27] & 0xF8) | (SysFreq_Info1.LNA_TOP);
     R840_I2C.Data = R840_Array[27];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
         return RT_Fail;

	  // Set LNA VTHL
	 R840_I2C.RegAddr = 0x0D;
     R840_Array[13] = (R840_Array[13] & 0x00) | SysFreq_Info1.LNA_VTH_L;
     R840_I2C.Data = R840_Array[13];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

     // Set MIXER TOP
	 R840_I2C.RegAddr = 0x1C;
     R840_Array[28] = (R840_Array[28] & 0xF0) | (SysFreq_Info1.MIXER_TOP); 
     R840_I2C.Data = R840_Array[28];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

     // Set MIXER VTHL
	 R840_I2C.RegAddr = 0x0E;
     R840_Array[14] = (R840_Array[14] & 0x00) | SysFreq_Info1.MIXER_VTH_L;
     R840_I2C.Data = R840_Array[14];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 // Set RF TOP
	 R840_I2C.RegAddr = 0x1A;
	 R840_Array[26] = (R840_Array[26] & 0x1F) | SysFreq_Info1.RF_TOP;
     R840_I2C.Data = R840_Array[26];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 // Set Nrb TOP
	 R840_I2C.RegAddr = 0x1C;
	 R840_Array[28] = (R840_Array[28] & 0x0F) | SysFreq_Info1.NRB_TOP;
     R840_I2C.Data = R840_Array[28];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 // Set Nrb BW
	 R840_I2C.RegAddr = 0x1B;
	 R840_Array[27] = (R840_Array[27] & 0x3F) | SysFreq_Info1.NRB_BW;
     R840_I2C.Data = R840_Array[27];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 // Set TF LPF
	 R840_I2C.RegAddr = 0x01;
	 R840_Array[1] = (R840_Array[1] & 0xFB) | SysFreq_Info1.BYP_LPF;
     R840_I2C.Data = R840_Array[1];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;
		
		
if(R840_Chip==R836_R)
{
	 //RF discharge 
	 R840_I2C.RegAddr = 0x1F;
	 R840_Array[31] = (R840_Array[31] & 0x1F) | SysFreq_Info1.RF_DISCHARGE;  
     R840_I2C.Data = R840_Array[31];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 if((R840_Array[5] & 0x80)==0x00) //LNA auto 
	 {
		 //LNA mode     /*must override Xtal GM! */
		 R840_I2C.RegAddr = 0x0F;
		 R840_Array[15] = (R840_Array[15] & 0xF7) | 0x08;  //disch
		 R840_I2C.Data = R840_Array[15];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
	 }

	 //LNA pulse flag    /*must override Xtal GM!, Fixed at Disable */
	 R840_I2C.RegAddr = 0x0F;
	 R840_Array[15] = (R840_Array[15] & 0xEF);  //disable
     R840_I2C.Data = R840_Array[15];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 //LNA img select /*fixed at  disch+img, R15[3]=1*/
	 R840_I2C.RegAddr = 0x10;
	 R840_Array[16] = (R840_Array[16] & 0xF7) | 0x08;   //disch+img
     R840_I2C.Data = R840_Array[16];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 Pulse_LnaDisch = SysFreq_Info1.LNA_DISCHARGE;
	 if((R840_Array[5] & 0x80)==0x00) //LNA auto 
	 {
		 //LNA discharge current
		 R840_I2C.RegAddr = 0x05;
		 R840_Array[5] = (R840_Array[5] & 0xE0) | SysFreq_Info1.LNA_DISCHARGE;  
		 R840_I2C.Data = R840_Array[5];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;
	 }

	 //Img NRB TOP adder //*must set after R840_MUX() *//
	 if(SysFreq_Info1.NRB_TOP_ADDER==0)
	 {
		 R840_Array[8] = (R840_Array[8] & 0xEF) | 0x00;  //nor, 0
		 R840_Array[15] = (R840_Array[15] & 0x7F) | 0x00;  //nor, 0
	 }
	 else if(SysFreq_Info1.NRB_TOP_ADDER==1)
	 {
		 R840_Array[8] = (R840_Array[8] & 0xEF) | 0x10;  //+3, 1
		 R840_Array[15] = (R840_Array[15] & 0x7F) | 0x00;  //+3, 0
	 }
	 else if(SysFreq_Info1.NRB_TOP_ADDER==2)
	 {
		 R840_Array[8] = (R840_Array[8] & 0xEF) | 0x00;  //+6, 0
		 R840_Array[15] = (R840_Array[15] & 0x7F) | 0x80;  //+6, 1
	 }
	 else
	 {
		 R840_Array[8] = (R840_Array[8] & 0xEF) | 0x10;  //+9, 1
		 R840_Array[15] = (R840_Array[15] & 0x7F) | 0x80;  //+9, 1
	 }

	 R840_I2C.RegAddr = 0x08;
     R840_I2C.Data = R840_Array[8];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;

	 R840_I2C.RegAddr = 0x0F;
     R840_I2C.Data = R840_Array[15];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
        return RT_Fail;
}
else  //R836
{
	     switch(R840_INFO.R840_Standard)
		 {
		 case R840_DVB_C_8M:
		 case R840_DVB_C_6M:
		 case R840_J83B:
		 case R840_DVB_C_8M_IF_5M:
		 case R840_DVB_C_6M_IF_5M:
		 case R840_J83B_IF_5M:
			 R840_Array[31] = (R840_Array[31] & 0x1E) | 0xE1; 
			 break;

		 default:
			 R840_Array[31] = (R840_Array[31] & 0x1E) | 0x21; 
			 break;
		 }

		 //Det Mode = Discharge; Current = 1
		 R840_I2C.RegAddr = 0x1F;
		 //R840_Array[31] = (R840_Array[31] & 0x1E) | 0x21;   			
		 R840_I2C.Data = R840_Array[31];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			 return RT_Fail;
}

 
     //Set LT
     if(R840_INFO.R840_LT==LT_ON)
	 {
		 R840_Array[23] = (R840_Array[23] & 0xDF);   //LT power on
	 }
	 else
	 {
	     R840_Array[23] = (R840_Array[23] | 0x20);      //LT power off
	 }
	 R840_I2C.RegAddr = 0x17;
	 R840_I2C.Data = R840_Array[23];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

     //Set Clk Out
	 if(R840_INFO.R840_ClkOutMode==CLK_OUT_OFF)
	 {
		 R840_Array[7] = (R840_Array[7] | 0x80);   //no clk out 
	 }
	 else
	 {
		 R840_Array[7] = (R840_Array[7] & 0x7F);   //clk out
	 }
	 R840_I2C.RegAddr = 0x07;
	 R840_I2C.Data = R840_Array[7];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

     //IF AGC1
	 R840_I2C.RegAddr = 0x1E;
	 R840_Array[30] = R840_Array[30] & 0xEF;
     R840_I2C.Data = R840_Array[30];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;   

	 if(R840_INFO.R840_Standard<R840_ATV_SIZE)   //ATV
	 {

		 if(R840_XtalDiv == XTAL_DIV1)
		 {
			 //AGC CLK to 500hz
			 R840_Array[27] = (R840_Array[27] & 0xC7) | 0x28;  //[5:3]=101
		 }
		 else
		 {
			 //AGC CLK to 1khz
			 R840_Array[27] = (R840_Array[27] & 0xC7) | 0x00;  //[5:3]=000
		 }
		 R840_I2C.RegAddr = 0x1B;			
		 R840_I2C.Data = R840_Array[27];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			 return RT_Fail;
	 }
	 else //DTV
	 {
		 //Hys = 1.0+/-0.x
		 R840_I2C.RegAddr = 0x06;			
		 R840_Array[6] = (R840_Array[6] & 0xFB) | 0x04;  //[2]=1
		 R840_I2C.Data = R840_Array[6];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			 return RT_Fail;

		 //NAT_det_sour = mixer buff out		 
		 R840_I2C.RegAddr = 0x18;			
		 R840_Array[24] = (R840_Array[24] & 0xDF) | 0x00;  //[5]=0
		 R840_I2C.Data = R840_Array[24];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			 return RT_Fail;

		 //AGC CLK to 1khz		 
		 R840_I2C.RegAddr = 0x1B;			
		 R840_Array[27] = (R840_Array[27] & 0xC7) | 0x00;  //[5:3]=000
		 R840_I2C.Data = R840_Array[27];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			 return RT_Fail;

		 //for DVB-T2
         switch(R840_INFO.R840_Standard)
		 {
		    case R840_DVB_T2_6M:
			case R840_DVB_T2_7M:
			case R840_DVB_T2_8M:
			case R840_DVB_T2_1_7M:
			case R840_DVB_T2_10M:
			case R840_DVB_T2_6M_IF_5M:
			case R840_DVB_T2_7M_IF_5M:
			case R840_DVB_T2_8M_IF_5M:
			case R840_DVB_T2_1_7M_IF_5M:

			if(R840_Initial_done_flag==FALSE)
			{
				R840_Init_Delay_MS(100);//3
			}
			else
			{
				R840_Delay_MS(100);//3
			}


				 //AGC clk 
				 R840_I2C.RegAddr = 0x1B;		
				 if(R840_Chip==R836)
					R840_Array[27] = (R840_Array[27] & 0xC7) | 0x20;  //[5:3]=100, 125Hz
				 else
					R840_Array[27] = (R840_Array[27] & 0xC7) | 0x38;  //[5:3]=111, 250Hz
				 R840_I2C.Data = R840_Array[27];
				 if(R836_I2C_Write(&R840_I2C) != RT_Success)
					 return RT_Fail;
			
				 //VHF N+2 ACI
				if(DTV_VHF_ACI2_ENHANCE==TRUE)
				{
					 if(R840_INFO.RF_KHz<=300000)   //VHF band
					{
						//Mixer Amp current: highest
						R840_I2C.RegAddr = 0x08;
						R840_Array[8] = R840_Array[8] & 0xBF;
						R840_I2C.Data = R840_Array[8];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;

						//force sharp mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] | 0x20);  //[5]=1
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //TF
						 if(R840_TF!=0)
						 {
				 				R840_I2C.RegAddr = 0x00;
								R840_Array[0] = (R840_Array[0] & 0x80) | (R840_TF-1);
								R840_I2C.Data = R840_Array[0]  ;
								if(R836_I2C_Write(&R840_I2C) != RT_Success)
									return RT_Fail;
						 }
					 }
					 else //UHF band
					 {
						//Mixer Amp current: lowest
						R840_I2C.RegAddr = 0x08;
						R840_Array[8] = R840_Array[8] | 0x40;
						R840_I2C.Data = R840_Array[8];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;	

						 //force plain mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;
					 }
				}
				else //DTV_VHF_ACI2_ENHANCE==FALSE
				{
						 //force plain mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;				
				}

				  //Q1.5K	
				 if((R840_INFO.RF_KHz>=170000) && (R840_INFO.RF_KHz<=210000)) 
				 {												 
					 R840_I2C.RegAddr = 0x00;
					 R840_Array[0] = R840_Array[0] & 0x7F; //off
					 R840_I2C.Data = R840_Array[0];
					 if(R836_I2C_Write(&R840_I2C) != RT_Success)
						return RT_Fail;
				 }

				//filter +3dB off
				R840_I2C.RegAddr = 0x1E;
				R840_Array[30] = (R840_Array[30] & 0xF7) & 0xF7;   //R30[3]=0
				R840_I2C.Data = R840_Array[30];
				if(R836_I2C_Write(&R840_I2C) != RT_Success)
					return RT_Fail;

				 //Lo buffer dc=highest
				R840_I2C.RegAddr = 0x07;
				R840_Array[7] = (R840_Array[7] & 0x9F) | 0x60;   //R7[6:5]=11
				R840_I2C.Data = R840_Array[7];
				if(R836_I2C_Write(&R840_I2C) != RT_Success)
					return RT_Fail;

				 if(R840_Chip==R836)
			    {
					 if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000)) 
					 {
						 //force sharp mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] | 0x20);  //[5]=1
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //filter +3dB							
						R840_I2C.RegAddr = 0x1E;
						R840_Array[30] = (R840_Array[30] & 0xF7) | 0x08;   //R30[3]=1
						R840_I2C.Data = R840_Array[30];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;

						 //Lo buffer dc=lowest
						R840_I2C.RegAddr = 0x07;
						R840_Array[7] = (R840_Array[7] & 0x9F) | 0x00;   //R7[6:5]=00
						R840_I2C.Data = R840_Array[7];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;
					 }				
				  }  //end of if(R840 || R836)

#if(DTV_LTE_ONE_EQ==TRUE)

				 if(R840_Chip==R836_R) 
			    {
					 if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000)) 
					 {
						 //force sharp mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //R3[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] | 0x20);  //R2[5]=1
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //filter +3dB							
						R840_I2C.RegAddr = 0x1E;
						R840_Array[30] = (R840_Array[30] & 0xF7) | 0x08;   //R30[3]=1
						R840_I2C.Data = R840_Array[30];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;

						 //AGC 1KHz
						 R840_I2C.RegAddr = 0x1B;											 	
						 R840_Array[27] = (R840_Array[27] & 0xC7) | 0x00;  //R27[5:3]=000
						 R840_I2C.Data = R840_Array[27];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //Filter comp: OFF
						 R840_I2C.RegAddr = 0x0F;			
						 R840_Array[15] = (R840_Array[15] & 0xFB);  //R15[2]=0
						 R840_I2C.Data = R840_Array[15];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;
					 }		
					 else
					 {
						 //Filter comp: ON
						 R840_I2C.RegAddr = 0x0F;			
						 R840_Array[15] = (R840_Array[15] | 0x04);  //R15[2]=1
						 R840_I2C.Data = R840_Array[15];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;					 
					 }
				  }  //end of if(R836R)

#endif
				 break;

			case R840_DVB_T_6M:
			case R840_DVB_T_7M:
			case R840_DVB_T_8M:
			case R840_DVB_T_6M_IF_5M:
			case R840_DVB_T_7M_IF_5M:
			case R840_DVB_T_8M_IF_5M:
		
				 //VHF N+2 ACI
				if(DTV_VHF_ACI2_ENHANCE==TRUE)
				{
					 if(R840_INFO.RF_KHz<=300000) //VHF band
					{
						//Mixer Amp current: highest
						R840_I2C.RegAddr = 0x08;
						R840_Array[8] = R840_Array[8] & 0xBF;
						R840_I2C.Data = R840_Array[8];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;

						//force sharp mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] | 0x20);  //[5]=1
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //TF
						 if(R840_TF!=0)
						 {
				 				R840_I2C.RegAddr = 0x00;
								R840_Array[0] = (R840_Array[0] & 0x80) | (R840_TF-1);
								R840_I2C.Data = R840_Array[0]  ;
								if(R836_I2C_Write(&R840_I2C) != RT_Success)
									return RT_Fail;
						 }
					 }
					 else
					 {
						//Mixer Amp current: lowest
						R840_I2C.RegAddr = 0x08;
						R840_Array[8] = R840_Array[8] | 0x40;
						R840_I2C.Data = R840_Array[8];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;		

						 //TF Auto
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] & 0x7F);  //[7]=0
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;		

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;
					 }
				}
				else //DTV_VHF_ACI2_ENHANCE==FALSE
				{
						//TF Auto
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] & 0x7F);  //[7]=0
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;		

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;
				}

				  //Q1.5K	
				 if((R840_INFO.RF_KHz>=170000) && (R840_INFO.RF_KHz<=210000)) 
				 {												 
					 R840_I2C.RegAddr = 0x00;
					 R840_Array[0] = R840_Array[0] & 0x7F; //off
					 R840_I2C.Data = R840_Array[0];
					 if(R836_I2C_Write(&R840_I2C) != RT_Success)
						return RT_Fail;
				 }
				 
		 		//filter +3dB off
				R840_I2C.RegAddr = 0x1E;
				R840_Array[30] = (R840_Array[30] & 0xF7) & 0xF7;   //R30[3]=0
				R840_I2C.Data = R840_Array[30];
				if(R836_I2C_Write(&R840_I2C) != RT_Success)
					return RT_Fail;

				 //Lo buffer dc=highest
				R840_I2C.RegAddr = 0x07;
				R840_Array[7] = (R840_Array[7] & 0x9F) | 0x60;   //R7[6:5]=11
				R840_I2C.Data = R840_Array[7];
				if(R836_I2C_Write(&R840_I2C) != RT_Success)
					return RT_Fail;

				 //must put after if(RF<300), or TF_mode overridded!!
				 if(R840_Chip==R836) 
			    {
					 if((R840_INFO.RF_KHz>=782000) && (R840_INFO.RF_KHz<=790000)) 
					 {
						 //force sharp mode
						 R840_I2C.RegAddr = 0x03;			
						 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
						 R840_I2C.Data = R840_Array[3];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 R840_I2C.RegAddr = 0x02;			
						 R840_Array[2] = (R840_Array[2] | 0x20);  //[5]=1
						 R840_I2C.Data = R840_Array[2];
						 if(R836_I2C_Write(&R840_I2C) != RT_Success)
							 return RT_Fail;

						 //filter +3dB							
						R840_I2C.RegAddr = 0x1E;
						R840_Array[30] = (R840_Array[30] & 0xF7) | 0x08;   //R30[3]=1
						R840_I2C.Data = R840_Array[30];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;

						 //Lo buffer dc=lowest
						R840_I2C.RegAddr = 0x07;
						R840_Array[7] = (R840_Array[7] & 0x9F) | 0x00;   //R7[6:5]=00
						R840_I2C.Data = R840_Array[7];
						if(R836_I2C_Write(&R840_I2C) != RT_Success)
							return RT_Fail;
					 }
		
				 }
				 break;

				case R840_ATSC:
				case R840_ATSC_IF_5M:
					if(R840_Chip==R836_R) 
					{
						if((R840_INFO.RF_KHz>=685000) && (R840_INFO.RF_KHz<700000))
						{
							 //force plain mode
							 R840_I2C.RegAddr = 0x03;			
							 R840_Array[3] = (R840_Array[3] | 0x80);  //[7]=1
							 R840_I2C.Data = R840_Array[3];
							 if(R836_I2C_Write(&R840_I2C) != RT_Success)
								 return RT_Fail;

							 R840_I2C.RegAddr = 0x02;			
							 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
							 R840_I2C.Data = R840_Array[2];
							 if(R836_I2C_Write(&R840_I2C) != RT_Success)
								 return RT_Fail;			
						}
						else 
						{
							 //TF Auto
							 R840_I2C.RegAddr = 0x03;			
							 R840_Array[3] = (R840_Array[3] & 0x7F);  //[7]=0
							 R840_I2C.Data = R840_Array[3];
							 if(R836_I2C_Write(&R840_I2C) != RT_Success)
								 return RT_Fail;		

							 R840_I2C.RegAddr = 0x02;			
							 R840_Array[2] = (R840_Array[2] & 0xDF);  //[5]=0
							 R840_I2C.Data = R840_Array[2];
							 if(R836_I2C_Write(&R840_I2C) != RT_Success)
								 return RT_Fail;				
						}
					}

				break;
		     default:		 
                 break;
		 }//end switch
	 }
	 return RT_Success;
}




R840_ErrCode R840_SetPllData(HI_U32 u32TunerPort, R840_Set_Info R840_INFO)
{	  
	  if(R840_Initial_done_flag==FALSE)
	  {
	      R840_Init(u32TunerPort);
	  }

      if(R840_SetStandard(R840_INFO.R840_Standard) != RT_Success)
		  return RT_Fail;

      if(R840_SetFrequency(R840_INFO) != RT_Success)
          return RT_Fail;

      return RT_Success;
}

R840_ErrCode R840_SetPllData_AutoSearch(HI_U32 u32TunerPort, R840_Set_Info R840_INFO)
{	  
	  UINT8 u1LpfCode, u1HpfCode;

	  if(R840_Initial_done_flag==FALSE)
	  {
	      R840_Init(u32TunerPort);
	  }

      if(R840_SetStandard(R840_INFO.R840_Standard) != RT_Success)
		  return RT_Fail;

      if(R840_SetFrequency(R840_INFO) != RT_Success)
          return RT_Fail;

	  // Set Filter Comp 0
	  R840_I2C.RegAddr = 0x1E;
	  R840_Array[30] = (R840_Array[30] & 0x9F) | 0x00;  //R30[6:5]
 	  R840_I2C.Data = R840_Array[30];
	  if(R836_I2C_Write(&R840_I2C) != RT_Success)
		  return RT_Fail;

	  //LPF/HPF wider
	  if(Sys_Info1.FILT_EXT_ENA==0x10)  //(%)
	  {		
		  if(R840_Fil_Cal_code[R840_INFO.R840_Standard]< 4)  
			 u1LpfCode = 2;	
		  else
             u1LpfCode = R840_Fil_Cal_code[R840_INFO.R840_Standard] - 2; 

		  u1HpfCode = Sys_Info1.HPF_COR;		  
	  }
	  else
	  {
		  if(R840_Fil_Cal_code[R840_INFO.R840_Standard]< 2) 
			 u1LpfCode = 0;	
		  else
		     u1LpfCode = R840_Fil_Cal_code[R840_INFO.R840_Standard] - 2;

		  if(Sys_Info1.HPF_COR>14)  
		     u1HpfCode = 15;
		  else
			 u1HpfCode = Sys_Info1.HPF_COR+1;		  
	  }
	 //Set LPF
	 R840_I2C.RegAddr = 0x0A;
	 R840_Array[10] = (R840_Array[10] & 0xF0) | u1LpfCode;  
	 R840_I2C.Data = R840_Array[10];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
		 return RT_Fail;

	 //Set HPF
     R840_I2C.RegAddr = 0x0B;
     R840_Array[11] = (R840_Array[11] & 0xF0) | u1HpfCode;
	 R840_I2C.Data = R840_Array[11];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
	     return RT_Fail;

      return RT_Success;
}


R840_ErrCode R840_Standby(R840_LoopThrough_Type R840_LoopSwitch)
{
	UINT8 i;

    for(i=0; i<R840_REG_NUM; i++)
	{
		R840_SBY[i]=R840_Array[i];
	}

	 //set LT switch
	 if(R840_LoopSwitch==LT_ON)
	 {
		 R840_Array[23] = (R840_Array[23] & 0xDF);   //LT power on;
		 R840_Array[6] = (R840_Array[6] & 0x7F);       //LNA on
		 R840_Array[29] = R840_Array[29] & 0xF7;      //LNA det on
	 }
	 else
	 {
	     R840_Array[23] = (R840_Array[23] | 0x20);      //LT power off; 
		 R840_Array[6] = (R840_Array[6] | 0x80);         //LNA off
		 R840_Array[29] = R840_Array[29] | 0x08;        //LNA det off
	 }

	 R840_I2C.RegAddr = 0x17;
	 R840_I2C.Data = R840_Array[23];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 R840_I2C.RegAddr = 0x06;
	 R840_I2C.Data = R840_Array[6];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //RF, Nrb Det PW off
	 R840_I2C.RegAddr = 0x1D;
	 R840_Array[29] = R840_Array[29] | 0x05;
	 R840_I2C.Data = R840_Array[29];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //LNA current = lowest, R6[1:0]=11   
	 R840_I2C.RegAddr = 0x06;
	 R840_Array[6] = R840_Array[6] | 0x03;
	 R840_I2C.Data = R840_Array[6];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;


	 //NAT PW off
	 R840_I2C.RegAddr = 0x1E;
	 R840_Array[30] = R840_Array[30] | 0x80;
	 R840_I2C.Data = R840_Array[30];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //Mixer & Mixer Buffer PW off
	 R840_I2C.RegAddr = 0x04;
	 R840_Array[4] = R840_Array[4] | 0x48;
	 R840_I2C.Data = R840_Array[4];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	if(R840_Chip==R836_R)
	{
		 //Xtal PW off
		 R840_I2C.RegAddr = 0x02;
		 R840_Array[2] = R840_Array[2] | 0x10;
		 R840_I2C.Data = R840_Array[2];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;
	}
	else
	{
		 //Xtal PW off
		 R840_I2C.RegAddr = 0x02;
		 R840_Array[2] = R840_Array[2] | 0x10;
		 R840_I2C.Data = R840_Array[2];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;

		 //Mixer Amp PW off
		 R840_I2C.RegAddr = 0x08;
		 R840_Array[8] = R840_Array[8] | 0x80;
		 R840_I2C.Data = R840_Array[8];
		 if(R836_I2C_Write(&R840_I2C) != RT_Success)
				return RT_Fail;
	}

	 //Polyphase/Amp PW off
	 R840_I2C.RegAddr = 0x09;
	 R840_Array[9] = R840_Array[9] | 0x80;
	 R840_I2C.Data = R840_Array[9];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //Filter/VGA PW off
	 R840_I2C.RegAddr = 0x0A;
	 R840_Array[10] = R840_Array[10] | 0x80;
	 R840_I2C.Data = R840_Array[10];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //All/AGC, ADC PW off
	 R840_I2C.RegAddr = 0x0C;
	 R840_Array[12] = R840_Array[12] | 0xC0;
	 R840_I2C.Data = R840_Array[12];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

     //PLL LDO A off
	 R840_I2C.RegAddr = 0x11;
	 R840_Array[17] = R840_Array[17] | 0xC0;
	 R840_I2C.Data = R840_Array[17];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //PLL DLDO 1& 2 off
	 R840_I2C.RegAddr = 0x17;
	 R840_Array[23] = R840_Array[23] | 0xD8;
	 R840_I2C.Data = R840_Array[23];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //Ping PLL off
	 R840_I2C.RegAddr = 0x18;
	 R840_Array[24] = R840_Array[24] | 0x10;
	 R840_I2C.Data = R840_Array[24];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	 //LPF bypass
	 R840_I2C.RegAddr = 0x01;
	 R840_Array[1] = R840_Array[1] & 0xFB;  //R1[2]=0
	 R840_I2C.Data = R840_Array[1];
	 if(R836_I2C_Write(&R840_I2C) != RT_Success)
			return RT_Fail;

	
	return RT_Success;
}

//-----------------------------------------------------------------------//
//  This function is used to wake up tuner from Standby mode   //
//-----------------------------------------------------------------------//
R840_ErrCode R840_WakeUp(void)
{
	UINT8 i;

	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = R840_REG_NUM;
	for(i = 0; i<R840_REG_NUM; i ++)
	{
		R840_I2C_Len.Data[i] = R840_SBY[i];
		R840_Array[i] = R840_SBY[i];
	}
	if(R836_I2C_Write_Len(&R840_I2C_Len) != RT_Success)
		return RT_Fail;

	return RT_Success;
}
R840_ErrCode R840_GetRfGain(R840_RF_Gain_Info *pR840_rf_gain)
{
	UINT16 acc_lna_gain[25] = {0, 12, 23, 46, 66, 86, 103, 120, 136, 154,              //0~9
		                                       173, 184, 196, 217, 233, 245, 254, 267, 278, 289,  //10~19
											   300, 311, 321, 332, 332         //20~23
	                                          };  

	UINT16 acc_rfbuf_gain;

	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = 5;
	if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
	{
		R836_I2C_Read_Len(&R840_I2C_Len);
	}

	pR840_rf_gain->RF_gain1 = (R840_I2C_Len.Data[3] & 0x1F);            //lna
	pR840_rf_gain->RF_gain2 = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  //rf
	pR840_rf_gain->RF_gain3 = (R840_I2C_Len.Data[4] & 0x0F);             //mixer

 if(R840_Chip==R836_R)  
 {
	if(pR840_rf_gain->RF_gain1 > 24) 
        pR840_rf_gain->RF_gain1 = 24;  //LNA gain max is mid band:24, others 23
 }
 else
 {
 	if(pR840_rf_gain->RF_gain1 > 22) 
       pR840_rf_gain->RF_gain1 = 22;  //LNA gain max is 22
 }

	if(pR840_rf_gain->RF_gain3 > 10)
        pR840_rf_gain->RF_gain3 = 10;  //MixerAmp gain max is 10	
	
	if(pR840_rf_gain->RF_gain2 < 14)
		acc_rfbuf_gain = pR840_rf_gain->RF_gain2*12;
	else if(pR840_rf_gain->RF_gain2 == 14)
		acc_rfbuf_gain = 163; //13*12+7
	else  //15
		acc_rfbuf_gain = 170; //13*12+14
	
	pR840_rf_gain->RF_gain_comb = (acc_lna_gain[pR840_rf_gain->RF_gain1] + acc_rfbuf_gain + pR840_rf_gain->RF_gain3*12);

    return RT_Success;
}





R840_ErrCode R840_RfGainMode(R840_RF_Gain_TYPE R840_RfGainType)
{
    UINT8 MixerGain = 0;
	UINT8 RfGain = 0;
	UINT8 LnaGain = 0;

	if(R840_RfGainType==RF_MANUAL)
	{
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 5; 
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
		    R836_I2C_Read_Len(&R840_I2C_Len);
		}

		MixerGain = (R840_I2C_Len.Data[4] & 0x0F);
		RfGain = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  
		LnaGain = (R840_I2C_Len.Data[3] & 0x1F);

		//LNA auto off
		if(R840_Chip==R836_R)
		{
	     R840_I2C.RegAddr = 0x0F;
	     R840_Array[15] = R840_Array[15] & 0xF7;
		 R840_I2C.Data = R840_Array[15];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
		}
	     R840_I2C.RegAddr = 0x05;
	     R840_Array[5] = R840_Array[5] | 0x80;
		 R840_I2C.Data = R840_Array[5];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer buffer off
	     R840_I2C.RegAddr = 0x1A;
	     R840_Array[26] = R840_Array[26] | 0x10;
		 R840_I2C.Data = R840_Array[26];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer auto off
	     R840_I2C.RegAddr = 0x07;
	     R840_Array[7] = R840_Array[7] & 0xEF;
		 R840_I2C.Data = R840_Array[7];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;



		//set LNA gain
	     R840_I2C.RegAddr = 0x05;
	     R840_Array[5] = (R840_Array[5] & 0xE0) | LnaGain;
		 R840_I2C.Data = R840_Array[5];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //set Mixer Buffer gain
	     R840_I2C.RegAddr = 0x1A;
	     R840_Array[26] = (R840_Array[26] & 0xF0) | RfGain;
		 R840_I2C.Data = R840_Array[26];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //set Mixer gain
	     R840_I2C.RegAddr = 0x07;
	     R840_Array[7] = (R840_Array[7] & 0xF0) | MixerGain;
		 R840_I2C.Data = R840_Array[7];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	}
	else
	{
	    //LNA auto on
		if(R840_Chip==R836_R)
		{
	     R840_I2C.RegAddr = 0x0F;
	     R840_Array[15] = R840_Array[15] | 0x08;
		 R840_I2C.Data = R840_Array[15];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
		}
	     R840_I2C.RegAddr = 0x05;
	     R840_Array[5] = R840_Array[5] & 0x7F;
		 R840_I2C.Data = R840_Array[5];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer buffer on
	     R840_I2C.RegAddr = 0x1A;
	     R840_Array[26] = R840_Array[26] & 0xEF;
		 R840_I2C.Data = R840_Array[26];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;

		 //Mixer auto on
	     R840_I2C.RegAddr = 0x07;
	     R840_Array[7] = R840_Array[7] | 0x10;
		 R840_I2C.Data = R840_Array[7];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	}

    return RT_Success;
}

//--------- Set IC_Internal_cap------------------------------------//
//   Xtal loading match formula: 
//   Xtal_cap_load*2 = IC_Internal_cap + External_cap
//   (default is for Xtal 16pF load)
//--------------------------------------------------------------------//
R840_ErrCode R840_SetXtalIntCap(R840_Xtal_Cap_TYPE R840_XtalCapType)
{
	 //Set internal Xtal cap 
	if(R840_XtalCapType==XTAL_CAP_LARGE)  //for Xtal CL >16pF
		R840_Array[19] = R840_Array[19] & 0xDF;
	else       //for Xtal CL < 16pF
		R840_Array[19] = R840_Array[19] | 0x20;

     R840_I2C.RegAddr = 0x13;
	 R840_I2C.Data = R840_Array[19];
     if(R836_I2C_Write(&R840_I2C) != RT_Success)
	       return RT_Fail;

	return RT_Success;
}

//--------- IF VGA mode control---------------------------------------------------------------//
//   1st input parameter (a) R840_VgaModeType
//   - VGA_AUTO: VGA controlled by chip self 
//   - VGA_MANUAL: Manual mode
//
//   2nd input parameter (b) u1ManualCode
//   - 0(smallest gain)~15(largest gain), only used in VGA_MANUAL mode
//--------------------------------------------------------------------------------------------------//
R840_ErrCode R840_VgaCtrl(R840_Vga_Mode_TYPE R840_VgaModeType, UINT8 u1ManualCode)
{

	if(R840_VgaModeType==VGA_MANUAL)
	{
		//VGA auto off
	     R840_I2C.RegAddr = 0x0C;
	     R840_Array[12] = R840_Array[12] | 0x10;
		 R840_Array[12] =(R840_Array[12] & 0xF0) | u1ManualCode;
		 R840_I2C.Data = R840_Array[12];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	}
	else
	{
		//VGA auto on
	     R840_I2C.RegAddr = 0x0C;
	     R840_Array[12] = R840_Array[12] & 0xEF;
		 R840_I2C.Data = R840_Array[12];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	
	}

    return RT_Success;
}
//------------------------------------------------------------------//
//  R840_PLL_Lock( ): Read PLL lock status (R3[6])        //
//  Return: 1: PLL lock                                                    //
//          0: PLL unlock                                                //
//------------------------------------------------------------------//
UINT8 R840_PLL_Lock(void)
{
	UINT8 fg_lock = 0;

	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = 3;
	if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
	{
	    R836_I2C_Read_Len(&R840_I2C_Len);
	}

	if( (R840_I2C_Len.Data[2] & 0x40) == 0x00 ) 		
		fg_lock = 0;		
	else
        fg_lock = 1;

	return fg_lock;
}

//------------------------------------------------------------------//
//  R840_SetIfFreq( ): Update tuner IF parameter               //
//  Input para1: desired IF output freq                               //
//  Input para2: RF freq                                                   //
//  Input para3: TV system number                                   //
//------------------------------------------------------------------//
R840_ErrCode R840_SetIfFreq(UINT16 IF_KHz, UINT32 RF_KHz, R840_Standard_Type u1SystemStd)
{
	Sys_Info_Type     Sys_Info_temp;
	R840_Set_Info     R840_Info;

	//Sys_Info1.IF_KHz = IF_KHz;

	R840_Info.R840_ClkOutMode = CLK_OUT_OFF;
	R840_Info.R840_IfAgc_Select = IF_AGC1;
	R840_Info.R840_LT = LT_OFF;
	R840_Info.R840_Standard = u1SystemStd;

	Sys_Info_temp = R840_Sys_Sel(u1SystemStd);
	R840_Info.RF_KHz = RF_KHz + IF_KHz - Sys_Info_temp.IF_KHz;

	if(RT_Success != R840_SetFrequency(R840_Info))
        return RT_Fail;

	return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLpfBw( ): Set LPF coarse tune parameter: LPF_BW         //
//  cover range of LPF_BW is:                                                          //
//		0: 7.60M~11.30MHz                                                              //
//		1: 6.45M~ 9.00MHz                                                               //
//		2: 5.60M~ 7.45MHz                                                                // 
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLpfBw(UINT8 LPF_BW)  
{
	UINT8 u1LpfBw;

	if(LPF_BW==0)
         u1LpfBw = 0x00;     //8M
    else if(LPF_BW==1)
         u1LpfBw = 0x40;     //7M
	else
         u1LpfBw = 0x60;     //6M

	// Set LPF coarse BW, R11[6:5]
	R840_I2C.RegAddr = 0x0B;
	R840_Array[11] = (R840_Array[11] & 0x9F) | u1LpfBw;
	R840_I2C.Data = R840_Array[11];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLpfOffset( ): Set LPF fine tune parameter: LPF_Offset     //
//  range: 0x00~0x0F    (R10[3:0])                                                    //
//  15 is narrowest; 0 is widest                                                           //
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLpfOffset(UINT8 LPF_Offset)  
{
	//Set LPF fine tune parameter
	R840_I2C.RegAddr = 0x0A;
	R840_Array[10] = (R840_Array[10] & 0xF0) | LPF_Offset;  
	R840_I2C.Data = R840_Array[10];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}

//---------------------------------------------------------------------//
//  R840_SetHpfOffset( ): Set HPF parameter: HPF_Offset     //
//  range is: 0x00~0x0F    (R11[3:0])                                    //
//  0 is narrowest; 15 is widest                                              //
//	0: 4.80M		4: 2.25M		8: 1.45M		12: 0.67M			//
//	1: 2.97M		5: 2.00M		9: 1.15M		13: 0.54M			//
//	2: 2.65M		6: 1.78M		10: 0.90M		14: 0.41M			//
//	3: 2.50M		7: 1.67M		11: 0.79M		15: 0.33M			//
//---------------------------------------------------------------------//
R840_ErrCode R840_SetHpfOffset(UINT8 HPF_Offset)  
{

	//Set HPF
    R840_I2C.RegAddr = 0x0B;
    R840_Array[11] = (R840_Array[11] & 0xF0) | HPF_Offset;
	R840_I2C.Data = R840_Array[11];
    if(R836_I2C_Write(&R840_I2C) != RT_Success)
	      return RT_Fail;

	return RT_Success;
}

//------------------------------------------------------------------//
//  R840_SetIfLpf( ): Set LPF                                          //
//  Input parameter: LPF_Cor_Freq    (unit: KHz)              //
//------------------------------------------------------------------//
R840_ErrCode R840_SetIfLpf(UINT32 LPF_Cor_Freq)
{
	UINT8     u1FilterCode;

	u1FilterCode = R840_Filt_Cal_ADC(LPF_Cor_Freq, 0x40, 8);

	// Set LPF fine code
	R840_I2C.RegAddr = 0x0A;
	R840_Array[10] = (R840_Array[10] & 0xF0) | u1FilterCode;  
	R840_I2C.Data = R840_Array[10];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	// Set LPF coarse BW
	R840_I2C.RegAddr = 0x0B;
	R840_Array[11] = (R840_Array[11] & 0x9F) | R840_Bandwidth;
	R840_I2C.Data = R840_Array[11];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		return RT_Fail;

	return RT_Success;
}

//------------------------------------------------------------------------------- //
//  R840_SetLnaTop( ): Set LNA target                                             //
//  range of LNA_Top is 0~7 (R27[2:0])                                            //
//	0 is max; 7 is min                                                                        //
//--------------------------------------------------------------------------------//
R840_ErrCode R840_SetLnaTop(UINT8 LNA_Top)
{
	R840_I2C.RegAddr = 0x1B;
	R840_Array[27] = (R840_Array[27] & 0xF8) | LNA_Top;
	R840_I2C.Data = R840_Array[27];
	if(R836_I2C_Write(&R840_I2C) != RT_Success)
		 return RT_Fail;

	return RT_Success;
}
//----------------------------------------------------------------------//
//  R840_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: output signal level (dBm*1000)                    //
//  4th parameter: output RF max gain indicator (1:max gain)    //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetRfRssi(UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, INT32 *RfLevelDbm, UINT8 *fgRfMaxGain)
{ 	
	UINT8  bRfFlag, bPulseFlag;
	UINT8  u1TF_Mode1, u1TF_Mode2;
	UINT8  u1Gain1, u1Gain2, u1Gain3;
	UINT16  acc_lna_gain;
	UINT16  acc_rfbuf_gain;
	UINT16  rf_total_gain;
	UINT16  u2PulseComp;
	UINT8   u1FreqIndex;
	UINT16  u2FreqFactor;
	INT32     rf_rssi;
	INT32    fine_tune = 0;    //for find tune 

	R840_RF_Gain_Info rf_gain_info;
	UINT8    rf_lna_gain;
	UINT32    rem;
	
	if(R840_Chip==R836_R) 
	{
		R840_I2C_Len.RegAddr = 0x00;
		R840_I2C_Len.Len = 5;
		if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
		{
			R836_I2C_Read_Len(&R840_I2C_Len);
		}
		u1Gain1 = (R840_I2C_Len.Data[3] & 0x1F);            //lna
		u1Gain2 = ((R840_I2C_Len.Data[4] & 0xF0) >> 4);  //rf
		u1Gain3 = (R840_I2C_Len.Data[4] & 0x0F);             //mixer
		bRfFlag = ((R840_I2C_Len.Data[1] & 0x80) >> 7);
		bPulseFlag = ((R840_I2C_Len.Data[1] & 0x40) >> 6);
		u1TF_Mode1 = (R840_Array[3] & 0x80) >> 7;          //R3[7], 0:auto    1:manuel
		u1TF_Mode2 = (R840_Array[2] & 0x20) >> 5;          //R2[5], 0:plain   1:sharp

		//Limit LNA, MixerAmp max gain
		if(u1Gain3 > 10)
	        u1Gain3 = 10; 

		 if(R840_Chip==R836_R)  
		 {
			if(u1Gain1 > 24) 
				u1Gain1 = 24;  

	       //max gain indicator
			if((u1Gain1==24) && (u1Gain2==15) && (u1Gain3==10))  
				*fgRfMaxGain = 1;
			else
				*fgRfMaxGain = 0;
		 }
		 else
		 {
	 		if(u1Gain1 > 22) 
				u1Gain1 = 22;  

			//max gain indicator
			if((u1Gain1==22) && (u1Gain2==15) && (u1Gain3==10))  
				*fgRfMaxGain = 1;
			else
				*fgRfMaxGain = 0;
		 }

		//Select LNA freq table
		if((RF_Freq_Khz<108000)||((RF_Freq_Khz>=147000)&&(RF_Freq_Khz<203000))||((RF_Freq_Khz>=243000)&&(RF_Freq_Khz<291000)))
		{
			u1FreqIndex = 0;
			u2FreqFactor = 110; 
		}
		else if((RF_Freq_Khz>=108000)&&(RF_Freq_Khz<147000))   //108~147M
		{
			u1FreqIndex = 0;
			u2FreqFactor = 80; 
		}
		else if((RF_Freq_Khz>=203000)&&(RF_Freq_Khz<243000))   //203~243M
		{
			u1FreqIndex = 0;
			u2FreqFactor = 50; 
		}
		else if((RF_Freq_Khz>=291000)&&(RF_Freq_Khz<307000))   //203~243M
		{
			u1FreqIndex = 0;
			u2FreqFactor = 140; 
		}
		else if((RF_Freq_Khz>=307000)&&(RF_Freq_Khz<350000))   //350~515M
		{
			u1FreqIndex = 0;
			u2FreqFactor = 170; 
		}
		else if((RF_Freq_Khz>=350000)&&(RF_Freq_Khz<395000))   //350~515M
		{
			u1FreqIndex = 1;
			u2FreqFactor = 160;//Æ«ÒÆ+2
		}
		else if((RF_Freq_Khz>=395000)&&(RF_Freq_Khz<506000))   //350~515M
		{
			u1FreqIndex = 1;
			u2FreqFactor = 140;//Æ«ÒÆ+2
		}
		else if((RF_Freq_Khz>=506000)&&(RF_Freq_Khz<515000))   //350~515M
		{
			u1FreqIndex = 1;
			u2FreqFactor = 180;//Æ«ÒÆ+2
		}
		else if((RF_Freq_Khz>=515000)&&(RF_Freq_Khz<562000))  //515~615M
		{
			u1FreqIndex = 2;
			u2FreqFactor = 160;//Æ«ÒÆ+3
		}
		else if((RF_Freq_Khz>=562000)&&(RF_Freq_Khz<618000))   //515~615M
		{
			u1FreqIndex = 2;
			u2FreqFactor = 140;//Æ«ÒÆ+3
		}
		else if((RF_Freq_Khz>=618000)&&(RF_Freq_Khz<705000))   //615~705M
		{
			u1FreqIndex = 3;
			u2FreqFactor = 110;//Æ«ÒÆ+3
		}
		else if((RF_Freq_Khz>=705000)&&(RF_Freq_Khz<786000))   //705~786M
		{
			u1FreqIndex = 4;
			u2FreqFactor = 110;//Æ«ÒÆ+4
		}
		else    // >=786
		{
			u1FreqIndex = 4;
			u2FreqFactor = 130;//Æ«ÒÆ+4
		}

		//Rf Buf Gain Table
		if(u1Gain2 < 14)
			acc_rfbuf_gain = u1Gain2*12;
		else if(u1Gain2 == 14)
			acc_rfbuf_gain = 163; //13*12+7
		else  //15
			acc_rfbuf_gain = 170; //13*12+14

		//LNA Gain
		if((bRfFlag==1 && u1TF_Mode1==0) || (u1TF_Mode1 & u1TF_Mode2))   //Sharp
			acc_lna_gain = R840_Lna_Sharp_Acc_Gain[u1FreqIndex][u1Gain1];
		else
			acc_lna_gain = R840_Lna_Plain_Acc_Gain[u1FreqIndex][u1Gain1];

	    //Pulse Flag
		if(bPulseFlag==1)
		{
			if((u1Gain1>=17) && (u1Gain2<=21))
				u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][0];
			else if(u1Gain1==22)
				u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][1];
			else if(u1Gain1==23)
				u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][2];
			else if(u1Gain1==24)
				u2PulseComp = R840_Lna_Pulse_Comp[u1FreqIndex][3];
			else
				u2PulseComp = 0;
		}
		else
			u2PulseComp = 0;

		acc_lna_gain = acc_lna_gain + u2PulseComp;

		//Add Rf Buf and Mixer Gain
		rf_total_gain = acc_lna_gain +acc_rfbuf_gain + u1Gain3*12;

		rf_rssi = fine_tune - (INT32) (rf_total_gain - u2FreqFactor);

		*RfLevelDbm = rf_rssi*100;
	}
	else
	{
		R840_GetRfGain(&rf_gain_info);
		rf_total_gain = rf_gain_info.RF_gain_comb;

		//max gain indicator
		if(rf_total_gain >= 592)
		{
	        	*fgRfMaxGain = 1;
		}
		else
		{
			*fgRfMaxGain = 0;		
		}
	 
		//calculate RF_RSSI
		if(RF_Freq_Khz<200000 )   //<200M
		{
		    rf_rssi = rf_total_gain*(-105) + 2000;  //177.5M
	 	}
		else if(RF_Freq_Khz<350000 )   //200~350M
		{
		    //rf_rssi = rf_total_gain*(-107) + 0;
			rf_rssi = rf_total_gain*(-110) + 500;  //226.5M
	 	}
		else if(RF_Freq_Khz<500000 )  //350~500M
		{
			 rf_rssi = rf_total_gain*(-106) + 7500;  //474M
			 
	 	}
		else if(RF_Freq_Khz<650000)  //500~650M
		{
	         rf_rssi = rf_total_gain*(-101) + 5000;  //554M
	 	}
		else if(RF_Freq_Khz<800000)  //650~800M
		{
		     rf_rssi = rf_total_gain*(-97) + 6300;  //666M
	 	}
		else  //>800M
		{
			 rf_rssi = rf_total_gain*(-92) + 6200;  //858M
	 	}

		switch(RT_Standard)
		{
			case R840_DVB_T2_6M:
			case R840_DVB_T2_7M:
			case R840_DVB_T2_8M:
			case R840_DVB_T2_1_7M:
			case R840_DVB_T2_10M:
			case R840_DVB_T2_6M_IF_5M:
			case R840_DVB_T2_7M_IF_5M:
			case R840_DVB_T2_8M_IF_5M:
			case R840_DVB_T2_1_7M_IF_5M:
			case R840_DVB_T_6M:
			case R840_DVB_T_7M:
			case R840_DVB_T_8M:
			case R840_DVB_T_6M_IF_5M:
			case R840_DVB_T_7M_IF_5M:
			case R840_DVB_T_8M_IF_5M:
	                  if((RF_Freq_Khz>=782000) && (RF_Freq_Khz<=790000)) 
			         {
						 if(rf_gain_info.RF_gain1 < 4)			
							 rf_lna_gain = 0;
	                     else if(rf_gain_info.RF_gain1 < 11)	
							 rf_lna_gain = rf_gain_info.RF_gain1-3;
						 else if(rf_gain_info.RF_gain1 < 13)	
							 rf_lna_gain = 7;
						 else if(rf_gain_info.RF_gain1 < 18)	
							 rf_lna_gain = rf_gain_info.RF_gain1-5;
						 else if(rf_gain_info.RF_gain1 < 22)	
							 rf_lna_gain = 12;
						 else
	                         rf_lna_gain = 13;

						 rf_total_gain = rf_lna_gain*15 +rf_gain_info.RF_gain2*12 + rf_gain_info.RF_gain3*12;
						 rf_rssi = rf_total_gain*(-110) - 1000; 
					 }
					 break;
			default:
				
	 			break;
		}
		 
		rem =(rf_rssi + 107000)%1000;
	 	 
		if(rem<500)
	    	*RfLevelDbm = (UINT32)((rf_rssi + 107000)/1000);
		else
			*RfLevelDbm = (UINT32)((rf_rssi + 107000)/1000)+1;
	}
	
	R840_PulseGain();
	
	return RT_Success;
}

//----------------------------------------------------------------------//
//  R840_GetIfRssi( ): Get IF VGA GAIN                               //
//  1st parameter: return IF VGA Gain     (dB*100)                   //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetIfRssi(INT32 *VgaGain)
{
	UINT8   adc_read;
    I2C_TYPE          Dlg_I2C;
	I2C_LEN_TYPE Dlg_I2C_Len;
	INT32   vga_table[64] = {                        //*100
		0, 0, 0, 0, 0, 0, 10, 10, 10, 10,   //0~9
		30, 50, 60, 100, 150, 300, 410, 520, 650, 780,   //10~19
		1030, 1260, 1350, 1440, 1520, 1600, 1670, 1750, 1900, 2090,   //20~29
		2180, 2350, 2450, 2640, 2790, 2940, 3100, 3270, 3430, 3550,   //30~39
		3670, 3790, 3870, 3950, 4030, 4110, 4150, 4190, 4220, 4250,   //40~49
		4300, 4350, 4400, 4440, 4470, 4500, 4520, 4540, 4560, 4580,   //50~59
		4600, 4610, 4610, 4620              //60~63
	};

	//enable IF_AGC read, R8[7]=1
	Dlg_I2C.RegAddr = 0x08;
	R840_Array[8] = R840_Array[8] | 0x80;
	Dlg_I2C.Data = R840_Array[8];
	if(R836_I2C_Write(&Dlg_I2C) != RT_Success)
		return RT_Fail;

	//ADC power on, R12[7]=0
	Dlg_I2C.RegAddr = 0x0C;
	R840_Array[12] = R840_Array[12] & 0x7F;
	Dlg_I2C.Data = R840_Array[12];
	if(R836_I2C_Write(&Dlg_I2C) != RT_Success)
		return RT_Fail;

	//read adc value
	Dlg_I2C_Len.RegAddr = 0x00;
	Dlg_I2C_Len.Len = 2;
	if(R836_I2C_Read_Len(&Dlg_I2C_Len) != RT_Success) // read data length
	{
		R836_I2C_Read_Len(&Dlg_I2C_Len);
	}
	adc_read = (Dlg_I2C_Len.Data[1] & 0x3F);

	*VgaGain = vga_table[adc_read];

	return RT_Success;
}

//----------------------------------------------------------------------//
//  R840_GetRfRssi( ): Get RF RSSI                                      //
//  1st parameter: input RF Freq    (KHz)                                //
//  2nd parameter: input Standard                                           //
//  3rd parameter: return signal level indicator (dBm)               //
//-----------------------------------------------------------------------//
R840_ErrCode R840_GetTotalRssi(UINT32 RF_Freq_Khz, R840_Standard_Type RT_Standard, INT32 *RssiDbm)
{
	INT32   rf_rssi;
	INT32   if_rssi;
	INT32   rem, total_rssi;
	INT32   ssi_offset = 0;   //need to fine tune by platform
	INT32   total_rssi_dbm;
	UINT8  rf_max_gain_flag;

	R840_GetRfRssi(RF_Freq_Khz, RT_Standard, &rf_rssi, &rf_max_gain_flag);

	if(R840_Chip==R836_R) 
	    R840_GetIfRssi(&if_rssi);  //vga gain        
	else
		if_rssi = 0;

    total_rssi = rf_rssi - (if_rssi*10);
	rem = total_rssi - (total_rssi/1000)*1000; //for rounding
	if((rem>-500) && (rem<500))
		total_rssi_dbm = (total_rssi/1000);
	else if(rem<=-500)
		total_rssi_dbm = (total_rssi/1000)-1;
	else
		total_rssi_dbm = (total_rssi/1000)+1;

	//for different platform, need to fine tune offset value
	*RssiDbm = total_rssi_dbm + ssi_offset;   

	return RT_Success;
}

R840_ErrCode R840_PulseGain(void)
{
#if(DTV_EXT_LNA==FALSE)

    UINT8 fg_Pulse = 0;
	UINT8 LnaGain;

	R840_I2C_Len.RegAddr = 0x00;
	R840_I2C_Len.Len = 5;
	if(R836_I2C_Read_Len(&R840_I2C_Len) != RT_Success)
	{
	    R836_I2C_Read_Len(&R840_I2C_Len);
	}

	if( (R840_I2C_Len.Data[1] & 0x40) == 0x00 ) 		
	{	
		PulseCount = 0;
	}
	else
	{
		if(PulseCount<2)
		    PulseCount ++;
	}

	if(PulseCount==2)
		fg_Pulse = 1;
	else
		fg_Pulse = 0;


	LnaGain = R840_I2C_Len.Data[3] & 0x1F;

	if((fg_Pulse==1) && ((R840_Array[5] & 0x80)==0)) //previous state is auto mode
	{
		if(LnaGain<=8)
			R840_Array[5] = (R840_Array[5] & 0xE0) | 0x08;
		else if(LnaGain<=11 && LnaGain>=9)
			R840_Array[5] = (R840_Array[5] & 0xE0) | 0x09;
		else if(LnaGain<=15 && LnaGain>=12)
			R840_Array[5] = (R840_Array[5] & 0xE0) | 0x0F;
		else
			R840_Array[5] = (R840_Array[5] & 0xE0) | LnaGain;

		 //LNA auto off
		if(R840_Chip==R836_R)
		{
	     R840_I2C.RegAddr = 0x0F;
	     R840_Array[15] = R840_Array[15] & 0xF7;
		 R840_I2C.Data = R840_Array[15];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
		}
				 
	     R840_I2C.RegAddr = 0x05;
	     R840_Array[5] = R840_Array[5] | 0x80;
		 R840_I2C.Data = R840_Array[5];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	}
	else if(fg_Pulse==0)
	{
	    //LNA auto on
		if(R840_Chip==R836_R)
		{
	     R840_I2C.RegAddr = 0x0F;
	     R840_Array[15] = R840_Array[15] | 0x08;
		 R840_I2C.Data = R840_Array[15];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
		}
				
	     R840_I2C.RegAddr = 0x05;
		 //R840_Array[5] = R840_Array[5] & 0x7F;
	     R840_Array[5] = (R840_Array[5] & 0x60) | Pulse_LnaDisch;
		 R840_I2C.Data = R840_Array[5];
	     if(R836_I2C_Write(&R840_I2C) != RT_Success)
		       return RT_Fail;
	}

#endif

	return RT_Success;
}


R840_ErrCode R840_set_tuner(HI_U32 u32TunerPort,HI_U32 u32PuRF,HI_U32 u32BandWidth,HI_U32 u32DVBMode)
{
	R840_Set_Info R840_Info;

    R836_I2C_SetPort(u32TunerPort);

	if(u32DVBMode)     //dvb-t
	{
		switch(u32BandWidth)
		{
			case 6000:		R840_Info.R840_Standard = R840_DVB_T_6M_IF_5M;		break;
			case 7000:		R840_Info.R840_Standard = R840_DVB_T_7M_IF_5M;		break;
			case 8000:		R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;		break;
			default:		R840_Info.R840_Standard = R840_DVB_T_8M_IF_5M;		break;
		}
		
	}
	else     //dvb-t2
	{
		switch(u32BandWidth)
		{
			case 1700:		R840_Info.R840_Standard = R840_DVB_T2_1_7M_IF_5M;		break;
			case 6000:		R840_Info.R840_Standard = R840_DVB_T2_6M_IF_5M;		break;
			case 7000:		R840_Info.R840_Standard = R840_DVB_T2_7M_IF_5M;		break;
			case 8000:		R840_Info.R840_Standard = R840_DVB_T2_8M_IF_5M;		break;
			default:		R840_Info.R840_Standard = R840_DVB_T2_8M_IF_5M;		break;
		}
	}

	//R840 Frequcncy Setting
	R840_Info.RF_KHz = u32PuRF;   //unit:KHz

	//Loop Through
	R840_Info.R840_LT = LT_OFF;    //Loop through OFF

	//Clk output
	R840_Info.R840_ClkOutMode = CLK_OUT_OFF;   //No tuner clock output for other IC

	//IF AGC select
	R840_Info.R840_IfAgc_Select = IF_AGC1;     //Select IF AGC1

	//Configure Tuner
	if(R840_SetPllData(u32TunerPort, R840_Info) != RT_Success)
		return RT_Fail;

	return RT_Success;
}

HI_S32 r836_init_tuner(HI_U32 u32TunerPort)
{
    g_r836_tuner_i2c_channel = g_stTunerOps[u32TunerPort].enI2cChannel;
    g_r836_tuner_port = u32TunerPort;
	R840_ADDRESS = g_stTunerOps[u32TunerPort].u32TunerAddress;

    //Initial R828
    if(R840_Init(u32TunerPort) != RT_Success)
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 r836_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF)
{
   // R840_Standard_Type R840_Standard_Set;
    R840_Set_Info R840_Info_Msg;
    RF_KHz = puRF;
  //RF > 115MHz
   
    R836_I2C_SetPort(u32TunerPort);
    //R828 Standard Setting
    if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[u32TunerPort].enDemodDevType)
    {   
        R840_Info_Msg.R840_Standard=R840_J83B;
    }
    else
    {    
         R840_Info_Msg.R840_Standard= R840_DVB_C_8M;
        
    } 
    R840_Info_Msg.R840_LT= LT_ON;
    R840_Info_Msg.R840_ClkOutMode = CLK_OUT_OFF;
    R840_Info_Msg.R840_IfAgc_Select = IF_AGC1;
    R840_Info_Msg.RF_KHz = puRF; // unit: kHz	
    if(R840_SetPllData(u32TunerPort, R840_Info_Msg) != RT_Success)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID r836_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    R840_ErrCode err = RT_Success;
    INT32 RssiDbm  ;
    R840_Set_Info R840_Info_Msg;
    UINT32 RfLevel  ;
    UINT8 fgRfMaxGain ;

	if (HI_UNF_DEMOD_DEV_TYPE_J83B == g_stTunerOps[u32TunerPort].enDemodDevType)
    {   
        R840_Info_Msg.R840_Standard=R840_J83B;
    }
    else
    {    
         R840_Info_Msg.R840_Standard= R840_DVB_C_8M;
        
    } 

	R840_Info_Msg.RF_KHz = RF_KHz;
    if(R840_Chip==R836_R)  
    {
	    err = R840_GetTotalRssi(R840_Info_Msg.RF_KHz, R840_Info_Msg.R840_Standard, &RssiDbm);

	     if(RT_Success != err)
	    {
	        HI_ERR_TUNER("R840_GetTotalRssi\n");
	        return;
	    }

		pu32SignalStrength[1] = (HI_U32)(RssiDbm + 106);	 
    }
    else
    {
    	     err = R840_GetRfRssi(R840_Info_Msg.RF_KHz, R840_Info_Msg.R840_Standard, &RfLevel, &fgRfMaxGain );    
	     if(RT_Success != err)
	    {
	        HI_ERR_TUNER("R840_GetRfRssi\n");
	        return;
	    }
		 
		 if( fgRfMaxGain == 1 )
		 {
			 pu32SignalStrength[2] = (HI_U32)( RfLevel + 2000 );	 
		 }
		 else
		 {
			 pu32SignalStrength[2] = (HI_U32)( RfLevel + 1000 );
		 }
    }
    return;
}

HI_VOID r836_tuner_resume (HI_U32 u32TunerPort)
{
    R840_Initial_done_flag=FALSE;
    r836_init_tuner(g_r836_tuner_port);
    return;
}
