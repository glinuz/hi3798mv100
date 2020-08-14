/*
 MxL203RF Source Code : V6.2.5.0
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL203RF.c

 Description: The source code is for MxL203RF user to quickly integrate MxL203RF into their software.
	There are two functions the user can call to generate a array of I2C command that's require to
	program the MxL203RF tuner. The user should pass an array pointer and an integer pointer in to the 
	function. The funciton will fill up the array with format like follow:
	
		addr1
		data1
		addr2
		data2
		...
	
	The user can then pass this array to their I2C function to perform progromming the tuner. 
*/
/* #include "StdAfx.h" */

#include "MxL203RF_Common.h"
#include "MxL203RF.h"




UINT32 MxL203RF_Init(UINT8* pArray,				/* a array pointer that store the addr and data pairs for I2C write */
					UINT32* Array_Size,			/* a integer pointer that store the number of element in above array */
					UINT8 Mode,				
					UINT32 Xtal_Freq_Hz,			
					UINT32 IF_Freq_Hz,				
					UINT8 Invert_IF,											
					UINT8 Clk_Out_Enable,    
					UINT8 Clk_Out_Amp,
					UINT8 Xtal_Cap
					)
{
	
	UINT32 Reg_Index=0;
	UINT32 Array_Index=0;

	IRVType IRV_Init_Cable[]=
	{
		/*{ Addr, Data}	 */
		{ 0x02, 0x06}, 
		{ 0x03, 0x1A}, 
		{ 0x04, 0x14}, 
		{ 0x05, 0x0E},
		{ 0x0C, 0x00},
		{ 0x07, 0x14}, 
		{ 0x29, 0x03}, 
		{ 0x45, 0x01}, 
		{ 0x7A, 0xCF}, 
		{ 0x7C, 0x7C}, 
		{ 0x7E, 0x27}, 
		{ 0x93, 0xD7},
		{ 0x99, 0x40}, 
		{ 0x13, 0x01},
		{ 0x2F, 0x00}, 
		{ 0x60, 0x60}, 
		{ 0x70, 0x00}, 
		{ 0xB9, 0x10},
		{ 0x8E, 0x57},
		{ 0x58, 0x08}, 
		{ 0x5C, 0x00},
		{ 0x48, 0x37},
		{ 0x55, 0x06},
		{ 0x01, 0x01}, /*TOP_MASTER_ENABLE=1 */
		{ 0, 0}
	};
	/*edit Init setting here */

	PIRVType myIRV = 0;
	
	switch(Mode)
	{
	case MxL_MODE_CAB_STD: /*Cable Standard Mode */
		myIRV = IRV_Init_Cable;
		SetIRVBit(myIRV, 0x45, 0xFF, 0x01);
		SetIRVBit(myIRV, 0x7A, 0xFF, 0x6F);
		SetIRVBit(myIRV, 0x7C, 0xFF, 0x1C);
		SetIRVBit(myIRV, 0x7E, 0xFF, 0x7C);
		SetIRVBit(myIRV, 0x93, 0xFF, 0xE7);
		break;
	default:
		return MxL_ERR_INIT;
	}

	switch(IF_Freq_Hz)
	{
	case MxL_IF_4_57_MHZ: 
		SetIRVBit(myIRV, 0x02, 0x0F, 0x03);
		break;
	case MxL_IF_5_MHZ:
		SetIRVBit(myIRV, 0x02, 0x0F, 0x04);
		break;
	case MxL_IF_6_MHZ: 
		SetIRVBit(myIRV, 0x02, 0x0F, 0x06); 
		break;
	case MxL_IF_7_2_MHZ: 
		SetIRVBit(myIRV, 0x02, 0x0F, 0x08);
		break;
	default:
		return MxL_ERR_IF_FREQ;
	}


	if(Invert_IF) 
		SetIRVBit(myIRV, 0x02, 0x10, 0x10);   /*Invert IF*/
	else
		SetIRVBit(myIRV, 0x02, 0x10, 0x00);   /*Normal IF*/


	switch(Xtal_Freq_Hz)
	{
	case MxL_XTAL_16_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x00);
		SetIRVBit(myIRV, 0x58, 0x03, 0x03);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x36);
		break;
	case MxL_XTAL_20_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x01);
		SetIRVBit(myIRV, 0x58, 0x03, 0x03);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x2B);
		break;
	case MxL_XTAL_20_25_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x02);
		SetIRVBit(myIRV, 0x58, 0x03, 0x03);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x2A);
		break;
	case MxL_XTAL_20_48_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x03);
		SetIRVBit(myIRV, 0x58, 0x03, 0x03);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x2A);
		break;
	case MxL_XTAL_24_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x04);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x48);
		break;
	case MxL_XTAL_25_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x05);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x45);
		break;
	case MxL_XTAL_25_14_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x06);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x44);
		break;
	case MxL_XTAL_28_8_MHZ: 
		SetIRVBit(myIRV, 0x04, 0x0F, 0x08);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x3C);
		break;
	case MxL_XTAL_32_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x09);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x36);
		break;
	case MxL_XTAL_40_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x0A);
		SetIRVBit(myIRV, 0x58, 0x03, 0x00);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x2B);
		break;
	case MxL_XTAL_44_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x0B);
		SetIRVBit(myIRV, 0x58, 0x03, 0x02);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x4E);
		break;
	case MxL_XTAL_48_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x0C);
		SetIRVBit(myIRV, 0x58, 0x03, 0x02);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x48);
		break;
	case MxL_XTAL_49_3811_MHZ:
		SetIRVBit(myIRV, 0x04, 0x0F, 0x0D);
		SetIRVBit(myIRV, 0x58, 0x03, 0x02);
		SetIRVBit(myIRV, 0x5C, 0xFF, 0x45);
		break;
	default:
		return MxL_ERR_XTAL_FREQ;
	}

	if(!Clk_Out_Enable) /*default is enable  */
		SetIRVBit(myIRV, 0x03, 0x10, 0x00);   

	/* Clk_Out_Amp */
	SetIRVBit(myIRV, 0x03, 0x0F, Clk_Out_Amp);

	/* Xtal Capacitor */
	if (Xtal_Cap >0 && Xtal_Cap <=25)
		SetIRVBit(myIRV, 0x05, 0xFF, Xtal_Cap);
	else if (Xtal_Cap == 0)
		SetIRVBit(myIRV, 0x05, 0xFF, 0x3F);
	else
		return MxL_ERR_INIT;


	/* Generate one Array that Contain Data, Address */
	while (myIRV[Reg_Index].Num || myIRV[Reg_Index].Val)
	{
		pArray[Array_Index++] = myIRV[Reg_Index].Num;
		pArray[Array_Index++] = myIRV[Reg_Index].Val;
		Reg_Index++;
	}
	    
	*Array_Size=Array_Index;
	return MxL_OK;
}


UINT32 MxL203RF_RFTune(UINT8* pArray, 
					   UINT32* Array_Size, 
					   UINT32 RF_Freq, 
					   UINT8 BWMHz, 
					   UINT8 Mode
					   )				   
					
{
	IRVType IRV_RFTune[]=
	{
	/*{ Addr, Data} */
		{ 0x10, 0x00},  /*abort tune*/
		{ 0x0D, 0x15},	
		{ 0x0E, 0x40},	
		{ 0x0F, 0x0E},
		{ 0xAB, 0x10},  
		{ 0x91, 0x00},   
		{ 0, 0}
	};

	UINT32 dig_rf_freq=0;
	UINT32 rf_freq_MHz = 0;
	UINT32 temp = 0 ;
	UINT32 Reg_Index=0;
	UINT32 Array_Index=0;
	UINT32 i = 0;
	UINT32 frac_divider = 1000000;

	rf_freq_MHz = RF_Freq/MHz;

	switch(Mode)
	{
		case MxL_MODE_CAB_STD: /*CABLE */
			switch(BWMHz)
			{
				case MxL_BW_6MHz:
					SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x49);
				break;
				
				case MxL_BW_8MHz:
					SetIRVBit(IRV_RFTune, 0x0D, 0xFF, 0x6F);
				break;
				default:
					return MxL_ERR_RFTUNE;
			}
		break;

		default:
			return MxL_ERR_RFTUNE;
	}

	/*Convert RF frequency into 16 bits => 10 bit integer (MHz) + 6 bit fraction */
	dig_rf_freq = RF_Freq / MHz; /*Whole number portion of RF freq (in MHz) */
	temp = RF_Freq % MHz; /*Decimal portion of RF freq (in MHz) */
	for(i=0; i<6; i++)
	{
		dig_rf_freq <<= 1;
		frac_divider /=2;
		if(temp > frac_divider) /* Carryover from decimal */
		{
			temp -= frac_divider;
			dig_rf_freq++;
		}
	}

	/*add to have shift center point by 7.8124 kHz */
	if(temp > 7812)
		dig_rf_freq ++;

	SetIRVBit(IRV_RFTune, 0x0E, 0xFF, (UINT8)dig_rf_freq);
	SetIRVBit(IRV_RFTune, 0x0F, 0xFF, (UINT8)(dig_rf_freq>>8));

	/* Updated on 2010-0508 for V6.2.5*/
	if(Mode == MxL_MODE_CAB_STD /*|| Mode == MxL_MODE_CAB_OPT1*/)
	{
		if(RF_Freq < 333000000)
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x70);
		else if(RF_Freq < 667000000)
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x20);
		else
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x10);
	}
	else	/* If Terrestrial modes ... */
	{
		if(RF_Freq < 444000000)
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x70);
		else if(RF_Freq < 667000000)
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x20);
		else
			SetIRVBit(IRV_RFTune, 0xAB, 0xFF, 0x10);
	}


	if (RF_Freq <= 334000000)
		SetIRVBit(IRV_RFTune, 0x91, 0x40, 0x40);
	else
		SetIRVBit(IRV_RFTune, 0x91, 0x40, 0x00);


	/*Generate one Array that Contain Data, Address  */
	while (IRV_RFTune[Reg_Index].Num || IRV_RFTune[Reg_Index].Val)
	{
		pArray[Array_Index++] = IRV_RFTune[Reg_Index].Num;
		pArray[Array_Index++] = IRV_RFTune[Reg_Index].Val;
		Reg_Index++;
	}
    
	*Array_Size=Array_Index;
	
	return MxL_OK;
}

/* ========================== Local functions called by Init and RFTune ============================ */
UINT32 SetIRVBit(PIRVType pIRV, UINT8 Num, UINT8 Mask, UINT8 Val)
{
	while (pIRV->Num || pIRV->Val)
	{
		if (pIRV->Num==Num)
		{
			pIRV->Val&=~Mask;
			pIRV->Val|=Val;
		}
		pIRV++;

	}	
	return MxL_OK;
}

