/*

 Driver APIs for MxL203RF Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL203RF_API.c
 
 Version:    6.2.4
*/


//#include "StdAfx.h"
#include "MxL203RF_API.h"
#include "MxL_User_Define.h"
#include "MxL203RF.h"


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//							Tuner Functions								   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

MxL_ERR_MSG MxL_Set_Register(MxL203RF_TunerConfigS* myTuner, UINT8 RegAddr, UINT8 RegData)
{
	UINT32 Status=0;
	UINT8 pArray[2];
	pArray[0] = RegAddr;
	pArray[1] = RegData;
	Status = MxL_I2C_Write((UINT8)myTuner->I2C_Addr, pArray, 2);
	if(Status) return MxL_ERR_SET_REG;

	return MxL_OK;
}


MxL_ERR_MSG MxL_Get_Register(MxL203RF_TunerConfigS* myTuner, UINT8 RegAddr, UINT8 *RegData)
{
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, RegAddr, RegData))
		return MxL_ERR_GET_REG;
	return MxL_OK;

}

MxL_ERR_MSG MxL_Soft_Reset(MxL203RF_TunerConfigS* myTuner)
{
	UINT8 reg_reset;
	reg_reset = 0xFF;
	if(MxL_I2C_Write((UINT8)myTuner->I2C_Addr, &reg_reset, 1))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Stand_By(MxL203RF_TunerConfigS* myTuner)
{
	UINT8 pArray[4];	/* a array pointer that store the addr and data pairs for I2C write	*/
	
	pArray[0] = 0x01;
	pArray[1] = 0x0;
	pArray[2] = 0x10;
	pArray[3] = 0x0;

	if(MxL_I2C_Write((UINT8)myTuner->I2C_Addr, pArray, 4))
		return MxL_ERR_OTHERS;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Wake_Up(MxL203RF_TunerConfigS* myTuner)
{
	UINT8 pArray[2];	/* a array pointer that store the addr and data pairs for I2C write	*/

	pArray[0] = 0x01;
	pArray[1] = 0x01;

	if(MxL_I2C_Write((UINT8)myTuner->I2C_Addr, pArray, 2))
		return MxL_ERR_OTHERS;

	if(MxL_Tuner_RFTune(myTuner, myTuner->RF_Freq_Hz, myTuner->BW_MHz))
		return MxL_ERR_RFTUNE;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Tuner_Init(MxL203RF_TunerConfigS* myTuner)
{	
	UINT8 pArray[MAX_ARRAY_SIZE];	/* a array pointer that store the addr and data pairs for I2C write */
	UINT32 Array_Size;				/* a integer pointer that store the number of element in above array */

	/* Soft reset tuner */
	if(MxL_Soft_Reset(myTuner))
		return MxL_ERR_INIT;

	/* perform initialization calculation */
	MxL203RF_Init(pArray, &Array_Size, (UINT8)myTuner->Mode, (UINT32)myTuner->Xtal_Freq,
				(UINT32)myTuner->IF_Freq, (UINT8)myTuner->IF_Spectrum, (UINT8)myTuner->ClkOut_Setting, (UINT8)myTuner->ClkOut_Amp,
				(UINT8)myTuner->Xtal_Cap);

	/* perform I2C write here */
	if(MxL_I2C_Write((UINT8)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_INIT;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Tuner_RFTune(MxL203RF_TunerConfigS* myTuner, UINT32 RF_Freq_Hz, MxL203RF_BW_MHz BWMHz)
{
	UINT8 pArray[MAX_ARRAY_SIZE];	/* a array pointer that store the addr and data pairs for I2C write */
	UINT32 Array_Size;				/* a integer pointer that store the number of element in above array */	

	/* Register Address, Mask, and Value for CHANGE_CHAN_SQ */
	UINT8 Addr = 0x3E;
	UINT8 Mask = 0x20;
	UINT8 Val = 0;

	UINT32 counter = 0;

	/* Store information into struc	 */
	myTuner->RF_Freq_Hz = RF_Freq_Hz;
	myTuner->BW_MHz = BWMHz;

	/* Set CHANGE_CHAN_SQ = 0*/
	MxL_Get_Register(myTuner, Addr, &Val);
	Val = Val & ~Mask;
	MxL_Set_Register(myTuner, Addr, Val);
	
	/* perform Channel Change calculation */
	MxL203RF_RFTune(pArray,&Array_Size,RF_Freq_Hz,BWMHz, myTuner->Mode);

	/* perform I2C write here */
	if(MxL_I2C_Write((UINT8)myTuner->I2C_Addr, pArray, Array_Size))
		return MxL_ERR_RFTUNE;

	/* wait for 1ms */
	MxL_Delay(1);

	/* Set CHANGE_CHAN_SQ = 1*/
	Val = Val | Mask;
	MxL_Set_Register(myTuner, Addr, Val);

	MxL_Delay(2);

	/* SHFLUT */
	MxL_Set_Register(myTuner,0x5F,0x00);
	if(myTuner->Xtal_Freq == MxL_XTAL_24_MHZ)
	{
		counter = 0;
		if(myTuner->BW_MHz == MxL_BW_8MHz)
		{
			while(SHIFT_LUT8[counter].Ch_Freq_Hz != -1)
			{
				if(SHIFT_LUT8[counter].Ch_Freq_Hz == RF_Freq_Hz)
				{
					MxL_Set_Register(myTuner,0x5E,SHIFT_LUT8[counter].Reg1_Val);
					MxL_Set_Register(myTuner,0x5F,SHIFT_LUT8[counter].Reg2_Val);
					break;
				}
				counter++;
			}
		}
		else if(myTuner->BW_MHz == MxL_BW_6MHz)
		{
			while(SHIFT_LUT6[counter].Ch_Freq_Hz != -1)
			{
				if(SHIFT_LUT6[counter].Ch_Freq_Hz == RF_Freq_Hz)
				{
					MxL_Set_Register(myTuner,0x5E,SHIFT_LUT6[counter].Reg1_Val);
					MxL_Set_Register(myTuner,0x5F,SHIFT_LUT6[counter].Reg2_Val);
					break;
				}
				counter++;
			}
		}
	}

	/* Start Tune */
	MxL_Set_Register(myTuner, 0x10, 0x01);

	MxL_Delay(30);

	return MxL_OK;
}

MxL_ERR_MSG MxL_Enable_LT(MxL203RF_TunerConfigS* myTuner, UINT8 En_LT)
{
	UINT8 Status;
	if(En_LT)
		Status = MxL_Set_Register(myTuner, 0x0C, 0x00);
	else
		Status = MxL_Set_Register(myTuner, 0x0C, 0x01);

	if(Status)
		return MxL_ERR_SET_REG;

	return MxL_OK;
}

MxL_ERR_MSG MxL_Check_ChipVersion(MxL203RF_TunerConfigS* myTuner, MxL203RF_ChipVersion* myChipVersion)
{	
	UINT8 Data;
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, 0x15, &Data))
		return MxL_GET_ID_FAIL;
		
	switch(Data & 0x0F)
	{
	case 0x06: *myChipVersion=MxL_203RF_ES5; break;
	default: 
		*myChipVersion=MxL_UNKNOWN_ID;
	}	

	return MxL_OK;
}

MxL_ERR_MSG MxL_RFSynth_Lock_Status(MxL203RF_TunerConfigS* myTuner, BOOL* isLock)
{	
	UINT8 Data;
	*isLock = FALSE; 
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, 0x14, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x0C;
	if (Data == 0x0C)
		*isLock = TRUE;  /* RF Synthesizer is Lock */
	return MxL_OK;
}

MxL_ERR_MSG MxL_REFSynth_Lock_Status(MxL203RF_TunerConfigS* myTuner, BOOL* isLock)
{
	UINT8 Data;
	*isLock = FALSE; 
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, 0x14, &Data))
		return MxL_ERR_OTHERS;
	Data &= 0x03;
	if (Data == 0x03)
		*isLock = TRUE;   /*REF Synthesizer is Lock */
	return MxL_OK;
}

/*
MxL_ERR_MSG MxL_Check_RF_Input_Power(MxL203RF_TunerConfigS* myTuner, REAL32* RF_Input_Level)
{	
	REAL32 temp;

	UINT8 Data1, Data2, whole, dec;
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, 0x16, &Data1))  // LSBs 
		return MxL_ERR_OTHERS;
	if(MxL_I2C_Read((UINT8)myTuner->I2C_Addr, 0x17, &Data2))  //MSBs
		return MxL_ERR_OTHERS;	

	// Determine whole and fractional portions of the power 
	whole = (Data1 >> 3) | (Data2 << 5);
	dec = Data1 & 0x07;

	temp = (REAL32)(whole) + (REAL32)(dec*0.125);

	*RF_Input_Level= temp - 120;

	return MxL_OK;
}
*/

MxL_ERR_MSG MxL_Input_Power_LUT_Reg_Write(MxL203RF_TunerConfigS* myTuner, UINT8 LUT_Index, UINT8 LUT_Val)
{
	if(LUT_Index > 15)
		return MxL_ERR_OTHERS;
	
	MxL_Set_Register(myTuner, 0x6A, LUT_Index);
	MxL_Set_Register(myTuner, 0x6B, LUT_Val);

	return MxL_OK;
}
