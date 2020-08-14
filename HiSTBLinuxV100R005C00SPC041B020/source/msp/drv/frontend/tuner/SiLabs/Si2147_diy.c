/*****************************************************************************************************************************
Si2147
*****************************************************************************************************************************/
#include "Silabs_L0_API.h"
#include "silabs.h"
#include "drv_demod.h"
#include "drv_tuner_ioctl.h"
#include "hi_type.h"
#include "hi_drv_tuner.h"
#include "hi_drv_i2c.h"
#include "hi3137.h"

int     L0_ReadCommandBytes  (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer);
int     L0_WriteCommandBytes (L0_Context* i2c, int iNbBytes, unsigned char *pucDataBuffer);
//extern SILABS_FE_Context    *front_end;
#define SiLabs_Repeat_Times		3

void si2147_system_wait(int time_ms)
{
	tuner_mdelay(time_ms);
}

unsigned char OperationSi47XX_2w(HI_U32 u32TunerPort, T_OPERA_MODE operation, unsigned char *data, unsigned char numBytes)
{
	//TUNER_I2C_DATA_S stI2CData;
	HI_S32 u32Status = 1;
	HI_U32 i,j;
	HI_U8  u8ReadAddr[2] = {0x00,0x00};

	if(data == HI_NULL)
	{
		return HI_FAILURE;
	}
	
	if(operation==SiLabs_WRITE)
	{
		for(i = 0;i < SiLabs_Repeat_Times;i++)
		{
			hi3137_config_i2c_out(u32TunerPort);
			u32Status = HI_DRV_I2C_Write(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32TunerAddress,u8ReadAddr,0,data,numBytes);
			if(HI_SUCCESS == u32Status)
			{
				return 0;
			}
		}
	}
	else
	{
		for(i = 0;i < SiLabs_Repeat_Times;i++)
		{
			hi3137_config_i2c_out(u32TunerPort);
			u32Status = HI_DRV_I2C_Read_SiLabs(g_stTunerOps[u32TunerPort].enI2cChannel,g_stTunerOps[u32TunerPort].u32TunerAddress,u8ReadAddr,0,data,numBytes);
			if(HI_SUCCESS == u32Status)
			{
				return 0;
			}
		}
	}

	return u32Status;
}

void ResetSi2147_2w(void)
{
	//SendRSTb();	
}

/**************************************

Si2147_Power_Up()

***************************************/
T_ERROR_OP Si2147_Power_Up(HI_U32 u32TunerPort)
{

	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_power_up[] = {0xc0,0x00,0x0c,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x01,0x01};

	ResetSi2147_2w();
	//send CMD
	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_power_up[0]), 15);
	if(error_ind)
		return I2C_ERROR;

	//mdelay(300);	
	si2147_system_wait(500);
	
	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		HI_ERR_TUNER("Si47XX_reg_data[0]:0x%x\n",Si47XX_reg_data[0]);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us*/
	si2147_system_wait(10);

	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;

	return OK;
}

/**************************************

Si2147_Get_Rev(): read rev info: you should read out: 

//read rev info: you should read out: {0x80,0x49,0x31,0x30,0x00,0x00,0x31,0x30,0x01,0x01} for A10 firmware

note! only can read it correctly after power up!

***************************************/
T_ERROR_OP Si2147_Get_Rev(HI_U32 u32TunerPort,unsigned char *pREV_data)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_get_rev[] = {0x11};	

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_get_rev[0]), 1);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);
	
	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, pREV_data, 10);	
	if(error_ind)
		return I2C_ERROR;
		
	return OK;	

}

/**************************************

Si2147_Set_Property_DTV_CONFIG_IF_PORT()


***************************************/

T_ERROR_OP Si2147_Set_Property_DTV_CONFIG_IF_PORT(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	//unsigned char Si47XX_set_property[] = {0x14,0x00,0x02,0x07,0x01,0x00};	//0x0702,LIF_IF1,AGC source:INTERNAL
	unsigned char Si47XX_set_property[] = {0x14,0x00,0x02,0x07,0x01,0x02};	//0x0702,LIF_IF1,AGC source:EXTERNAL
										//00,00: pin12/13
										//01,00: pin9/11

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 6);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}

T_ERROR_OP Si2147_Set_Property_DTV_CONFIG_XO_CAP(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[] = {0x14,0x00,0x04,0x02,0x0d,0x00};	//xo cap = 8.2pf

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 6);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}


/**************************************

Si2147_Set_Property_DTV_MODE()


***************************************/

T_ERROR_OP Si2147_Set_Property_DTV_MODE(HI_U32 u32TunerPort,HI_U32 u32BandWidth)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[6] = {0x14,0x00,0x03,0x07,0x28,0x00};	//0x0703, 0x0028:8MHz,DVB-T; 0x0038:8MHz,DVB-C; 0x0068:8MHz,DTMB
										//xx00:normal, xx01: inverted

	switch(u32BandWidth)
	{
		case 1700:
			Si47XX_set_property[0] = 0x14;	Si47XX_set_property[1] = 0x00;	Si47XX_set_property[2] = 0x03;
			Si47XX_set_property[3] = 0x07;	Si47XX_set_property[4] = 0x29;	Si47XX_set_property[5] = 0x00;
			break;
		case 6000:
			Si47XX_set_property[0] = 0x14;	Si47XX_set_property[1] = 0x00;	Si47XX_set_property[2] = 0x03;
			Si47XX_set_property[3] = 0x07;	Si47XX_set_property[4] = 0x26;	Si47XX_set_property[5] = 0x00;
			break;
		case 7000:
			Si47XX_set_property[0] = 0x14;	Si47XX_set_property[1] = 0x00;	Si47XX_set_property[2] = 0x03;
			Si47XX_set_property[3] = 0x07;	Si47XX_set_property[4] = 0x27;	Si47XX_set_property[5] = 0x00;
			break;
		case 8000:
		default:
			Si47XX_set_property[0] = 0x14;	Si47XX_set_property[1] = 0x00;	Si47XX_set_property[2] = 0x03;
			Si47XX_set_property[3] = 0x07;	Si47XX_set_property[4] = 0x28;	Si47XX_set_property[5] = 0x00;
			break;
	}
	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 6);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}

/**************************************

Si2147_Set_Property_TUNER_IEN()

***************************************/

T_ERROR_OP Si2147_Set_Property_TUNER_IEN(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[] = {0x14,0x00,0x01,0x05,0x01,0x00};	//0x0501, 0x0001:TCIEN=ENABLE

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 6);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}

/**************************************

Si2147_Set_Property_DTV_IEN()

***************************************/

T_ERROR_OP Si2147_Set_Property_DTV_IEN(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[] = {0x14,0x00,0x01,0x07,0x01,0x00};	//0x0701, 0x0001:CHLIEN=ENABLE

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 6);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}

/**************************************

Si2147_TUNER_STATUS_Clear_TCINT()

***************************************/

static T_ERROR_OP Si2147_TUNER_STATUS_Clear_TCINT(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[] = {0x42,0x01};	

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 2);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}
/**************************************

Si2147_TUNER_STATUS_Clear_DTVINT()

***************************************/

static T_ERROR_OP Si2147_TUNER_STATUS_Clear_DTVINT(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_set_property[] = {0x62,0x01};	

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_set_property[0]), 2);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}

/**************************************

Si2147_Tune_Freq_DTV()

***************************************/

T_ERROR_OP Si2147_Tune_Freq_DTV(HI_U32 u32TunerPort, unsigned long tune_freq,HI_U32 u32BandWidth)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_tune_freq[] = {0x41,0x00,0x00,0x00,0x80,0x5A,0xB2,0x27};	//0x27B25A80=666000000Hz	
	unsigned char i;

	Si2147_Set_Property_DTV_MODE(u32TunerPort,u32BandWidth);
	       
	Si47XX_tune_freq[4] = (tune_freq&0xff) >> 0; 
	Si47XX_tune_freq[5] = (tune_freq&0xff00) >> 8;
	Si47XX_tune_freq[6] = (tune_freq&0xff0000) >> 16; 
	Si47XX_tune_freq[7] = (tune_freq&0xff000000) >> 24;

	Si2147_TUNER_STATUS_Clear_TCINT(u32TunerPort);
	Si2147_TUNER_STATUS_Clear_DTVINT(u32TunerPort);

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_tune_freq[0]), 8);
	if(error_ind)
		return I2C_ERROR;
		
	//DELAY(100ms);
	si2147_system_wait(100);

	//wait CTS&TUNINT = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		HI_ERR_TUNER("Si47XX_reg_data[0]:0x%x\n",Si47XX_reg_data[0]);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
		si2147_system_wait(5);
	}while(((Si47XX_reg_data[0]&0x81) != 0x81) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us*/

	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;
	si2147_system_wait(50);

	return OK;

}

/**************************************

Si2147_Power_Down()

***************************************/

T_ERROR_OP Si2147_Power_Down(HI_U32 u32TunerPort)
{
	unsigned short loop_counter = 0;
	unsigned char Si47XX_reg_data[32];	
	unsigned char error_ind = 0;
	unsigned char Si47XX_power_down[] = {0x13};	

	//send CMD
 	error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_WRITE, &(Si47XX_power_down[0]), 1);
	if(error_ind)
		return I2C_ERROR;

	//wait CTS = 1
	/*do
	{	
		error_ind = OperationSi47XX_2w(u32TunerPort, SiLabs_READ, &(Si47XX_reg_data[0]), 1);
		if(error_ind)
			return I2C_ERROR;	
		loop_counter++;
	}while(((Si47XX_reg_data[0]) != 0x80) && (loop_counter < 0xff));  //loop_counter limit should guarantee at least 300us
	
	if(loop_counter >= 0xff)
		return LOOP_EXP_ERROR;*/
	si2147_system_wait(10);

	return OK;

}



/**************************************

Si2147_Tune_DTV_Initial()

***************************************/

T_ERROR_OP Si2147_Tune_DTV_Initial(HI_U32 u32TunerPort)
{
	//unsigned char Si47XX_REV_data[10];
	//int i;

	if(Si2147_Power_Up(u32TunerPort) != OK) 
	{
		HI_ERR_TUNER("Si2147_Power_Up fail.\n");
		return ERROR;
	}
	/*if(Si2147_Get_Rev(&Si47XX_REV_data[0]) != OK) return ERROR;	
	HI_INFO_TUNER("get_rev\n");
	for(i=0;i<10;i++)
		HI_INFO_TUNER("%x", Si47XX_REV_data[i]);
        */
#if 1
	if(Si2147_Set_Property_DTV_CONFIG_IF_PORT(u32TunerPort) != OK) 
	{
		HI_ERR_TUNER("Si2147_Set_Property_DTV_CONFIG_IF_PORT fail.\n");
		return ERROR;
	}
	si2147_system_wait(10);
	if(Si2147_Set_Property_DTV_CONFIG_XO_CAP(u32TunerPort) != OK)
	{
		HI_ERR_TUNER("Si2147_Set_Property_DTV_CONFIG_IF_PORT fail.\n");
		return ERROR;
	}
	si2147_system_wait(10);
	if(Si2147_Set_Property_DTV_MODE(u32TunerPort,8000) != OK)
	{
		HI_ERR_TUNER("Si2147_Set_Property_DTV_MODE fail.\n");
		return ERROR;
	}
	si2147_system_wait(10);
	if(Si2147_Set_Property_TUNER_IEN(u32TunerPort) != OK)
	{
		HI_ERR_TUNER("Si2147_Set_Property_TUNER_IEN fail.\n");
		return ERROR;
	}
	si2147_system_wait(10);
    if(Si2147_Set_Property_DTV_IEN(u32TunerPort) != OK)
	{
		HI_ERR_TUNER("Si2147_Set_Property_DTV_IEN fail.\n");
		return ERROR;
	}

#endif
	
	return OK;
}

/**************************************

Si2147_Tune_DTV_Demo()

***************************************/

T_ERROR_OP Si2147_Tune_DTV_Demo(HI_U32 u32TunerPort, HI_U32 u32Frequency,HI_U32 u32BandWidth)
{
	if(Si2147_Tune_Freq_DTV(u32TunerPort,u32Frequency,u32BandWidth) != OK) 		//set freq 
	{
		HI_ERR_TUNER("Si2147_Tune_Freq_DTV fail.\n");
		return ERROR;
	}
	
	return OK;
}













