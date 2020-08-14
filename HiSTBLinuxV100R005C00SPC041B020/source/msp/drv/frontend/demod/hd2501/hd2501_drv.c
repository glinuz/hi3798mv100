/*************************************************************
** file name:		hd2501_drv.c
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/

/* include -------------------------------------------------*/


#include <linux/delay.h>
#include "linux/kernel.h"

#include "hi_type.h"
#include "hi_drv_gpioi2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "hd2501_type.h"
#include "hd2501_drv.h"
#include "RDA5815.h"
#if 0 /* 2016/07/20-5812 not test yet */
#include "RDA5812.h"
#endif
#include "AV2026.h"

/* macro & structure ---------------------------------------*/

#ifdef _USE_HD2501_CHIP_

/* global & static variables -------------------------------*/

extern HI_U32 			genI2cChannelHD2501;

HDIC_Abss_TunerType_t   HD_Abss_eTunerType = HDIC_TUNER_UNKNOWN;
HDIC_DEMOD_Type_t 		demodType_st;
static HI_U32 s_u32TunerPort = 0;

typedef struct HDIC_SNR_DB_LUT_s
{
	U32		dB_integer;
	U32		dB_Demical;
	U32		Regval ;
}HDIC_SNR_DB_LUT_t;

U32 SNR_DB_LUT[] =
{
// 0 - 30dB, step is 1dB;
1024   ,    // 0 dB
1289   ,
1623   ,
2043   ,
2572   ,
3238   ,
4077   ,
5132   ,
6461   ,
8134   ,
10240  ,
12891  ,
16229  ,
20431  ,
25722  ,
32382  ,
40766  ,
51322  ,
64610  ,
81339  ,
102400 ,
128914 ,
162293 ,
204315 ,
257217 ,
323817 ,
407662 ,
513216 ,
646100 ,
813392 ,
1024000,
1289140,
1622931,
2043149,
2572172,
3238172,
4076617,
5132157,
6461003,
8133921     // 39dB
};

/* extern functions declaration ----------------------------*/
/* static functions declaration ----------------------------*/
/* functions define ----------------------------------------*/
/* static functions definition -----------------------------*/


/**********************************
* 延时函数,单位毫秒
***********************************/
void HDIC2501_Wait(U32 millisecond)
{
	mdelay(2*millisecond);
	return;
}

U8 HDIC_I2C_Write_AddrLen1Byte(U8 device_address, U16 register_address, U8 * value_buffer, U32 length)
{
	HI_S32 ret;
    if (HI_STD_I2C_NUM > genI2cChannelHD2501)
    {
    	ret = HI_DRV_I2C_Write(genI2cChannelHD2501, device_address, register_address, 1, value_buffer, length);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_WriteExt(genI2cChannelHD2501, device_address, register_address, 1, value_buffer, length);
    }

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}

U8 HDIC_I2C_Read_AddrLen1Byte(U8 device_address, U8 register_address, U8 * value_buffer, U32 length)
{
	HI_S32 ret;

    if (HI_STD_I2C_NUM > genI2cChannelHD2501)
    {
    	ret = HI_DRV_I2C_Read(genI2cChannelHD2501, device_address, register_address, 1, value_buffer, length);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_ReadExt(genI2cChannelHD2501, device_address, register_address, 1, value_buffer, length);
    }

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}

U8 HDIC_I2C_Write(U8 device_address, U8 register_address, U8 * value_buffer, U32 length)
{
	HI_S32 ret;

    if (HI_STD_I2C_NUM > genI2cChannelHD2501)
    {
    	ret = HI_DRV_I2C_Write(genI2cChannelHD2501, device_address, register_address, 2, value_buffer, length);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_WriteExt(genI2cChannelHD2501, device_address, register_address, 2, value_buffer, length);
    }

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}

U8 HDIC_I2C_Read(U8 device_address, U16 register_address, U8 * value_buffer, U32 length)
{
	HI_S32 ret;

    if (HI_STD_I2C_NUM > genI2cChannelHD2501)
    {
    	ret = HI_DRV_I2C_Read(genI2cChannelHD2501, device_address, register_address, 2, value_buffer, length);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_ReadExt(genI2cChannelHD2501, device_address, register_address, 2, value_buffer, length);
    }

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}



/********************************************
* 写2501的寄存器
* 2501是双字节的寄存器,加上寄存器的值,需要向2501
* 写三个字节的数据
********************************************/
U8 HDIC2501_WriteRegister(U16 Register, U8 Data)
{
	HI_S32 ret;
    U8 bValue;
    bValue = Data;

    ret = HDIC_I2C_Write( HDIC2501_I2C_ADDRESS, Register, &bValue, 1) ;

	if(HDIC_NO_ERROR == ret)
	{
		return HDIC_NO_ERROR;
	}
	else
	{
		HDIC_Demod_Print("-->>error @ %s:%d\n",__FUNCTION__,__LINE__);
		return HDIC_ERROR_IIC_ERROR;
	}
}

/********************************************
* 读2501的寄存器
* 2501是双字节的寄存器,所以先写入2个字的寄存器地址,
* 然后可以不需要产生停止位,再从2501读出1个字节.
********************************************/
U8 HDIC2501_ReadRegister(U16 Register, U8 *Data)
{
	HI_S32 ret;

    ret = HDIC_I2C_Read( HDIC2501_I2C_ADDRESS, Register, Data, 1) ;

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}

/*******************************************************************
*
* 打开2501 Tuner I2C转发功能
* 参数:Tuner的I2C设备地址.
*
*******************************************************************/
U8 HDIC2501_OpenTunerI2C(U8 Address)
{
    U8 err = HDIC_NO_ERROR;

	//20151126 规避控制tuner期间异常的中频信号，关闭AGC控制，并设置AGC放大倍数最小
	err |= HDIC2501_WriteRegister(0x22,0xA9);
	//20151126

    Address &= 0xFE;
    err |= HDIC2501_WriteRegister(0x0A,Address);
    err |= HDIC2501_WriteRegister(0x09,0x01);

    return (err);
}

/*******************************************************************
*
* 关闭2501 Tuner I2C转发功能
*
*******************************************************************/
U8 HDIC2501_CloseTunerI2C(void)
{
    U8 err = HDIC_NO_ERROR;

    HDIC2501_WriteRegister(0x09,0x00);
    HDIC2501_WriteRegister(0x0A,0x00);

	//20151126 规避控制tuner期间异常的中频信号，关闭AGC控制，并设置AGC放大倍数最小，此处打开AGC控制
	HDIC2501_WriteRegister(0x22,0xA8);
	//20151126

    return (err);
}

U8  tuner_register_write(U8 I2CSlaveAddr, U8 *data, U32 length)
{
#if 0
    return HDIC_I2C_Write( I2CSlaveAddr, data[0], &(data[1]), (length - 1) ) ;
#else
	HI_S32 ret;

	ret = HI_DRV_I2C_Write(genI2cChannelHD2501, I2CSlaveAddr, data[0], 1, &(data[1]), (length - 1));

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;

#endif
}


U8 tuner_register_read(U8 I2CSlaveAddr, U8 sub_addr, U8 *data, U32 length)
{
#if 0
	return HDIC_I2C_Read_AddrLen1Byte( I2CSlaveAddr, sub_addr, data, length) ;
	//return HDIC_I2C_Read( I2CSlaveAddr, sub_addr, data, length) ;
#else
	HI_S32 ret;

	ret = HI_DRV_I2C_Read(genI2cChannelHD2501, I2CSlaveAddr, sub_addr, 1, data, length);

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;

#endif
}

/*******************************************************************
*
* 2501初始化特定寄存器,需要在上电硬复位后调用此函数.
*
*******************************************************************/
U8 HDIC2501_InitReg(void)
{
    U8 err = HDIC_NO_ERROR;

    HDIC2501_WriteRegister(0x22,0xA8);
    HDIC2501_WriteRegister(0x6F,0x20);

    return err;
}

/*******************************************************************
*
* 设置2501 TS输出格式
* 参数1:TSEDGE_POSEDGE、TSEDGE_NEGEDGE。上升沿、下降沿。
* 参数2:TS_SERIAL、TS_PARALLEL。串行TS、并行TS。
* 参数3:NULLPACKET_ENABLED、NULLPACKET_DELETED。允许空包、去空包。
*
*******************************************************************/
HDIC_ErrorCode_t HDIC2501_SetTsFormat(U8 Edge, U8 Serial, U8 NullPacket)
{
    U8 err = HDIC_NO_ERROR;
    U8 readData = 0;
    HDIC2501_ReadRegister(0x05,&readData);
    readData &= 0xF0;
    readData |= 0x04;
    readData = readData | Edge | Serial | NullPacket;
    HDIC2501_WriteRegister(0x05,readData);

    return err;
}

/*******************************************************************
*
* 需要在上电硬复位HDIC2501芯片后,调用一次此函数,此后不再需要调用.
*
*
* 设置2501 TS输出格式
* 参数1:TSEDGE_POSEDGE、TSEDGE_NEGEDGE。上升沿、下降沿。
* 参数2:TS_SERIAL、TS_PARALLEL。串行TS、并行TS。
* 参数3:NULLPACKET_ENABLED、NULLPACKET_DELETED。允许空包、去空包。
*
*******************************************************************/
void HDIC2501_Init(void)
{
    //初始化特定寄存器
    HDIC2501_InitReg();

    //设置TS输出格式
    HDIC2501_SetTsFormat(TSEDGE_POSEDGE, TS_SERIAL, NULLPACKET_ENABLED);
}

/********************************************
* 配置Tuner
* 以SHARP Tuner为例子,并使用了I2C转发功能,
* 即Tuner的I2C是通过2501转发
********************************************/
U8 HDIC2501_SetTuner(U32 Frequency,U32 sysm)
{
    U8 err = HDIC_NO_ERROR;
    //U8 Tuner_Addr=0xC0;//Set Tuner address here,0xC0 is just an example
    //UINT16 Lpf;
    HDIC_Demod_Print("%s:%d,Tunertype=%d\n",__FUNCTION__,__LINE__,HD_Abss_eTunerType);
	
    if( HDIC_TUNER_RDA5812 == HD_Abss_eTunerType )	//if(HD_Abss_eTunerType == RDA5812_TUNER_HD)
    {
        HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_RDA5812);
#if 0  /* 2016/07/20-5812 not test yet */
        RDA5812_ITuner_Lock(Frequency,sysm);
#endif
        HDIC2501_Wait(50);
    }
    else if ( HDIC_TUNER_RDA5815 == HD_Abss_eTunerType )		//else if(HD_Abss_eTunerType == RDA5815_TUNER_HD)
    {
        HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_RDA5812);
#if 0 /* 2016/07/20 use hisi 5815M driver */
        RDA5815_ITuner_Lock(Frequency,sysm/1000);
#else
#if defined(TUNER_DEV_TYPE_RDA5815)
        RDA5815_set_tuner(s_u32TunerPort, genI2cChannelHD2501, Frequency*1000); //KHz
#endif
#endif
        HDIC2501_Wait(50);
    }
    else if ( HDIC_TUNER_RDA5815M == HD_Abss_eTunerType )	//else if(HD_Abss_eTunerType == RDA5815M_TUNER_HD)
    {
        HDIC_Demod_Print("HDIC2501_SetTuner RDA5815M  Frequency = %d MHz, sysm = %d KHz.\r\n", Frequency, sysm/1000);
        HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_RDA5812);
#if 0  /* 2016/07/20 5815M not test yet */
		RDA5815Set(Frequency,sysm/1000);
#endif
        HDIC2501_Wait(50);
    }
    else if ( HDIC_TUNER_AV2026 == HD_Abss_eTunerType )		//else if(HD_Abss_eTunerType == AV2026_TUNER_HD)
    {
        err = HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_AV2026);
		HDIC_Demod_Print("%s:%d,HDIC2501_OpenTunerI2C=%d\n",__FUNCTION__,__LINE__,err);
#if defined(TUNER_DEV_TYPE_AV2026)
        err = AV2026_Tuner_control(Frequency,sysm/1000);
		HDIC_Demod_Print("%s:%d,AV2026_Tuner_control=%d\n",__FUNCTION__,__LINE__,err);
#endif
        HDIC2501_Wait(50);
		HDIC_Demod_Print("%s:%d,after 50ms=%d\n",__FUNCTION__,__LINE__);
    }

    HDIC_Demod_Print("%s:%d,return\n",__FUNCTION__,__LINE__);

#if 0

#if tuner_SHARP6903
    else if(HD_Abss_eTunerType == SHARP6903_TUNER_HD)
    {
        HDIC2501_OpenTunerI2C(0xc0);
        HDIC_SetTuner_SHARP6903(Frequency,sysm);
        HDIC2501_Wait(50);
    }
#endif
#if tuner_SCM5008L
    else if(HD_Abss_eTunerType == SCM5008L_TUNER_HD)
    {
        HDIC2501_OpenTunerI2C(SCM5008L_DEV_ADDR);
        HDIC_SetTuner_SCM5008L(Frequency,sysm);
        HDIC2501_Wait(50);
    }
#endif
#if tuner_SCM5108L
    else if(HD_Abss_eTunerType == SCM5108L_TUNER_HD)
    {
        HDIC2501_OpenTunerI2C(SCM5108L_DEV_ADDR);
        HDIC_SetTuner_SCM5108L(Frequency,sysm);
        HDIC2501_Wait(50);
    }
#endif

#endif // #if 0

    HDIC2501_CloseTunerI2C();

    return err;
}

/*******************************************************************
*
* 设置2501的符号率
* 例如符号率是28.8M, 4个寄存器的值应为0x01,0xB7,0x74,0x00
*
*******************************************************************/
U8 HDIC2501_SetSymbolRate(U32 SymbolRate)
{
    U8 err = HDIC_NO_ERROR;
    U8 D3B,D3A,D39,D38;
    U32 RecCodeRate = SymbolRate;

    D3B = (U8)((RecCodeRate&0xff000000)>>24);
    HDIC2501_WriteRegister(0x3B,D3B);

    D3A = (U8)((RecCodeRate&0x00ff0000)>>16);
    HDIC2501_WriteRegister(0x3A,D3A);

    D39 = (U8)((RecCodeRate&0x0000ff00)>>8);
    HDIC2501_WriteRegister(0x39,D39);

    D38 = (U8)((RecCodeRate&0x000000ff));
    HDIC2501_WriteRegister(0x38,D38);

    return (err);
}

/*******************************************************************
*
* 设置2501的工作
* 参数1:SYMBOLRATEAUTODETECT_AUTO、SYMBOLRATEAUTODETECT_MANUAL.自动符号率、手动符号率。
* 参数2:符号率。参数1设置为手动符号率时起效。
*
*******************************************************************/
U8 HDIC2501_SetParameters(U8 SymbolRateAutoDetect,U32 SymbolRate_Hz)
{
    U8 err = HDIC_NO_ERROR;
    U8 D04;

    D04=0x0F+SymbolRateAutoDetect;
    HDIC2501_WriteRegister(0x04,D04);

    if(SymbolRateAutoDetect == SYMBOLRATEAUTODETECT_MANUAL)
    {
        //Write RecCodeRate
        HDIC2501_SetSymbolRate(SymbolRate_Hz);
    }

    return err;
}

/*******************************************************************
*
* 2501软复位,在配置完参数后,调用此函数让2501重新工作
*
*******************************************************************/
U8 HDIC2501_SoftReset(void)
{
    U8 err = HDIC_NO_ERROR;

    HDIC2501_WriteRegister(0x06,0xF0);
    HDIC2501_Wait(10);
    HDIC2501_WriteRegister(0x06,0xE0);

    return (err);
}

/*******************************************************************
*
* 判断2501是否锁住信号
*
*******************************************************************/
U8 HDIC2501_IsDemodLocked(U8 *locked, U8 *Status)
{
    U8 err = HDIC_NO_ERROR;
    U8 ReadData;
    HDIC2501_ReadRegister(0x10,&ReadData);
    *Status=ReadData;
    if ((ReadData&0x10)==0x10)
    {
        *locked=0x01;
    }
    else
    {
        *locked=0x00;
    }

    return err;
}

/*******************************************************************
*
* 获取信号的误码率
*
*******************************************************************/
U8 HDIC2501_GetSignalBER(U32 *pSignalBER)
{
	U8 err = HDIC_NO_ERROR;
	U8 ReadData1 = 0,ReadData2 = 0 ;
	U32 Temp;

	HDIC2501_ReadRegister(0xA1,&ReadData1);
	HDIC2501_ReadRegister(0xA2,&ReadData2);
	
	Temp = (((U16)ReadData2&0x0f)<<8) + ReadData1;

	pSignalBER[0] = 4096;	// Total 4096 LDPC blocks
	pSignalBER[1] = Temp;	// Error blocks in previous 4096 LDPC blocks.

	return (err);
}

/*******************************************************************
*
* 获取信号的信噪比
*
*******************************************************************/
U32 HDIC2501_GetSignalSNR_dBPlus10(U32 SNR_RegVal, U32 *MaxdBVal, U32 *dBPlus10 )
{
	U32 TopIndex = 0 ;
	U16 i = 0 ;

	TopIndex = sizeof ( SNR_DB_LUT ) / sizeof( U32 ) ;
	*MaxdBVal = TopIndex - 1;

	for (i = 0; i <= TopIndex; i++ )
	{
        if ( SNR_RegVal <= SNR_DB_LUT[i] )
        {
            *dBPlus10 = i ;
            return HDIC_NO_ERROR;
        }
	}
    *dBPlus10 = TopIndex - 1 ;

	return HDIC_NO_ERROR ;
}

U8 HDIC2501_GetSignalSNR(U32 *SignalSNR)
{
    U8 err = HDIC_NO_ERROR;
    U8 SNR65 = 0;
	U8 SNR66 = 0 ;
    U32 SNR_Temp = 0 ;
    U32 SNR_dB = 0 ;
    U32 SNR_dB_MaxInLUT = 0 ;

    HDIC2501_ReadRegister(0x65,&SNR65);
    HDIC2501_ReadRegister(0x66,&SNR66);
    SNR_Temp = SNR66*256 + SNR65;
    HDIC_Demod_Print("%s[%d]: enter, SNR66=0x%x, SNR65=0x%x\n\n", __FUNCTION__, __LINE__, SNR66, SNR65);

    if(SNR_Temp!=0)
    {
        //*SignalSNR = 10*log10((double)8192/SNR_Temp);
        HDIC2501_GetSignalSNR_dBPlus10( SNR_Temp, &SNR_dB_MaxInLUT, &SNR_dB ) ;
        SNR_dB = SNR_dB_MaxInLUT - SNR_dB ;

        HDIC_Demod_Print("%s[%d]: SNR_Temp=0x%x, SNR_dB_MaxInLUT=0x%x, SNR_dB=0x%x\n\n", __FUNCTION__, __LINE__, SNR_Temp, SNR_dB_MaxInLUT, SNR_dB);

        SNR_dB = ( SNR_dB * 100 ) / SNR_dB_MaxInLUT ;
        if ( SNR_dB > 100 )
            SNR_dB = 100 ;
        *SignalSNR = SNR_dB ;
    }
    else
    {
        *SignalSNR = 0;
    }

    HDIC_Demod_Print("%s[%d]: SignalSNR=0x%x\n\n", __FUNCTION__, __LINE__, *SignalSNR);

    //FEPrintf_("SignalSNR = %f \r\n", *SignalSNR);

    return (err);
}

/*******************************************************************
*
* 获取信号强度
*
*******************************************************************/
U8 HDIC2501_GetFieldStrength(U16 *FieldStrength)
{
	U8 err = HDIC_NO_ERROR;
	U8 ReadData1 = 0, ReadData2 = 0;

	HDIC2501_ReadRegister(0x48,&ReadData1);
	HDIC2501_ReadRegister(0x49,&ReadData2);

	*FieldStrength = (((U16)ReadData1&0x0f)<<8) + ReadData2;

    // TODO: need convert to 0 - 100
    if (*FieldStrength > 100)
    {
        *FieldStrength = 100;
    }

	return (err);
}

U8 HD2501_DemodHardwareReset(void)
{
    //TODO:reset the demodulator chip by hardware, by pio
//    HDIC_ErrorCode_t errCode = HDIC_NO_ERROR;
//    U32 Value;

//    U32 BitMask = 0x08;  //bit5

#if 1
    HDIC_Demod_Print("\n-->Nothing done in HD2501_DemodHardwareReset\n");

#else
    //set
    errCode = HDICGPIO_Read(PIOHandle_2501, &Value);
    if (errCode != HDIC_NO_ERROR)
    {
        FEPrintfErr("HDICGPIO_Open errCode = %d--------%d \n\r", errCode,__LINE__);
    }
    Value |= BitMask;
    errCode = HDICGPIO_Write(PIOHandle_2501, Value);
    HDIC2501_Wait(10 * 1000000);
    //clear
    Value &= ~BitMask;
    errCode = HDICGPIO_Write(PIOHandle_2501, Value);
    HDIC2501_Wait(50 * 1000000);

    //set
    Value |= BitMask;
    errCode = HDICGPIO_Write(PIOHandle_2501, Value);
    HDIC2501_Wait(10 * 1000000);

#endif // #if 1

    return HDIC_NO_ERROR;
}

U8 HDIC2501_Demod_AutoCheck( void )
{
    U8 result = 1;
    U8 jx;
    U8 ReadData = 0;
    HDIC_Demod_Print("%s[%d]: enter\n", __FUNCTION__, __LINE__);

    HD2501_DemodHardwareReset();
    HDIC2501_Wait(30);
    
    for (jx=0; jx<3; jx++)
    {
        result = HDIC2501_WriteRegister(0x0005,0x18);
        HDIC2501_ReadRegister(0x0005,&ReadData);
        
        HDIC_Demod_Print("====[%d]=====[ReadData=%x]======\n",__LINE__,ReadData);
        if(result == 0)
        {
            demodType_st = DEMOD_TYPE_HD2501;
            HDIC_Demod_Print("\n\rThe demod  type is HD2501 !!! \n\r");
            return HDIC_NO_ERROR;
        }
    }

    HDIC_Demod_Print("\n\rThe demod  type is NULL !!! \n\r");

    return HDIC_ERROR_FAIL;
}

U8 HDIC2501_TunerAutoCheck(HI_U32 u32TunerPort)
{
	U8 ret;
	U8 ix;
	U8 reg_data[10] = {0};
    U8 REG_VAL = 0x00;
    U8 REG_VAL_00 = 0x00;

    HDIC_Demod_Print("%s[%d]: enter\n", __FUNCTION__, __LINE__);

    s_u32TunerPort = u32TunerPort;
    
    for(ix=0; ix<5; ix++)
    {
        HDIC2501_Init();

        reg_data[0] = 0x01;
        reg_data[1] = 0xc4;

        /*---------------------------------AV2026 Auto----------------------------------*/

        HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_AV2026);
        tuner_register_write(HDIC_TUNER_DEV_ADDR_AV2026, reg_data, 2);
        
        ret = tuner_register_read(HDIC_TUNER_DEV_ADDR_AV2026,  reg_data[0], &REG_VAL, 1);
        HDIC_Demod_Print("\n\r=====[%s]===[%d]==[ret=%d]====[REG_VAL=%x]= \n\r",__FUNCTION__,__LINE__,ret,REG_VAL);
		
        HDIC2501_CloseTunerI2C();
        if(0 == ret)
        {
            HD_Abss_eTunerType = HDIC_TUNER_AV2026;
            HDIC_Demod_Print("\n\r AV2026_TUNER_HD detected! \n\r");
            return HDIC_NO_ERROR;
        }
        /*---------------------------------AV2026  END---------------------------------*/

        /*-------------------------RDA5812、RDA5815S、RAD5815M Auto--------------------*/

        HDIC2501_OpenTunerI2C(HDIC_TUNER_DEV_ADDR_RDA581x);

		//for(ix=0;ix<3;ix++)
        {

            ret = tuner_register_read(HDIC_TUNER_DEV_ADDR_RDA581x,  0x00, &REG_VAL_00, 1);
            ret = tuner_register_read(HDIC_TUNER_DEV_ADDR_RDA581x,  0x01, &REG_VAL, 1);
        }
        HDIC2501_CloseTunerI2C();

        HDIC_Demod_Print(" [0x00] = 0x%x \n\r",REG_VAL_00);
        HDIC_Demod_Print(" [0x01] = 0x%x \n\r",REG_VAL);
		
        if(0 == ret)
        {
#if 1
//            if((0x58 == REG_VAL_00)&&((REG_VAL & 0xf0) == 0xf0) )
            if((0x58 == REG_VAL_00)&&(REG_VAL == 0xf8) )
            {
                HDIC_Demod_Print("\n\r ==RDA5815M_TUNER_HD== \n\r");
                HD_Abss_eTunerType = HDIC_TUNER_RDA5815M ; //RDA5815M_TUNER_HD;
                HDIC2501_OpenTunerI2C( HDIC_TUNER_DEV_ADDR_RDA581x );
#if 0  /* 2016/07/20 5815M not test yet */
                ret = RDA5815Initial();  // for RDA5815M tuner
#endif
                HDIC2501_CloseTunerI2C();

            }
            else
#endif
                if(((REG_VAL & 0xf0) == 0xf0))
                {
                    HDIC_Demod_Print("\n\r ==RDA5815_TUNER_HD== \n\r");
                    HD_Abss_eTunerType = HDIC_TUNER_RDA5815; //RDA5815_TUNER_HD;
                    //HD_Abss_eTunerType = RDA5812_TUNER_HD;
                    HDIC2501_OpenTunerI2C( HDIC_TUNER_DEV_ADDR_RDA581x );
#if 0 /* 2016/07/20 use hisi 5815M driver */
                    ret = RDA5815_ITuner_Init();
#else
#if defined(TUNER_DEV_TYPE_RDA5815)
                    ret = RDA5815_init_tuner(u32TunerPort);
#endif
#endif
                    HDIC2501_CloseTunerI2C();
                }
                else if((REG_VAL & 0xc0) == 0xc0)
                {
                    HDIC_Demod_Print("\n\r ==RDA5812_TUNER_HD== \n\r");
                    HD_Abss_eTunerType =     HDIC_TUNER_RDA5812; //RDA5812_TUNER_HD;
                    HDIC2501_OpenTunerI2C( HDIC_TUNER_DEV_ADDR_RDA581x );
#if 0  /* 2016/07/20 5812 not test yet */
                    ret = RDA5812_ITuner_Init();
#endif
                    HDIC2501_CloseTunerI2C();
                }
                else
                {
                    return HDIC_ERROR_FAIL;
                }

            if(0 == ret)
                return HDIC_NO_ERROR;
        }
        /*-------------------------RDA5812、RDA5815S、RAD5815M END-----------------------*/

#if 0 // YanLei_Add:20160310

#if (tuner_SCM5108L||	tuner_SHARP6903)
        /*-------------------------SCM5108L AUTO-----------------------*/
        HDIC2501_OpenTunerI2C(0xc0);
//	for(ix=0;ix<3;ix++)
        {
            tuner_register_write(0xc0, reg_data, 2);
            FEPrintf_("\n\r zhaofan==%s===%d==[ret=%d]== \n\r",__FUNCTION__,__LINE__,ret);
            ret = tuner_register_read(0xc0,  0x01, &REG_VAL, 1);
            FEPrintf_("\n\r zhaofan==%s===%d==[ret=%d]== \n\r",__FUNCTION__,__LINE__,ret);
            if(ret == 0)
                break;
        }
        if(ret == 0)
        {
            HDIC2501_OpenTunerI2C(0xc0);
            //ret = tuner_register_read(0x18,  0x00, &REG_VAL_00, 1);
            QM1D1C0045_register_real_read(0x00, &REG_VAL);
            HDIC2501_CloseTunerI2C();

            if(REG_VAL == 0x68)
            {
                HD_Abss_eTunerType = SHARP6903_TUNER_HD;
                printf("\n\r ==SHARP6903_TUNER_HD== \n\r");
            }
            else
            {
                HD_Abss_eTunerType = SCM5008L_TUNER_HD;
                printf("\n\r ==SCM5008L_TUNER_HD== \n\r");
            }
            return DD_OK;
        }
        /*-------------------------SCM5008L END-----------------------*/

#endif
#if tuner_SCM5108L

        /*-------------------------SCM5108L AUTO-----------------------*/
        HDIC2501_OpenTunerI2C(0xc2);
//	for(ix=0;ix<3;ix++)
        {
            tuner_register_write(0xc2, reg_data, 2);
            ret = tuner_register_read(0xc2,  0x01, &REG_VAL, 1);
            if(ret == 0)
                break;
        }
        HDIC2501_CloseTunerI2C();
        if(ret == 0)
        {
            HD_Abss_eTunerType = SCM5108L_TUNER_HD;
            FEPrintf_("\n\r ==SCM5108L_TUNER_HD== \n\r");
            return DD_OK;
        }
        /*-------------------------SCM5108L END-----------------------*/
#endif

#endif //#if 0 // YanLei_Add:20160310
    }

    HDIC_Demod_Print("Demod:NONE tuner detected!\n");

    return HDIC_ERROR_FAIL;
}

U8 HDIC2501_SetPolar( HI_UNF_TUNER_FE_POLARIZATION_E Polar )
{
	U8 readData = 0 ;
	
    HDIC2501_ReadRegister(0x12,&readData);//默认值为0x10，右旋

    if (HI_UNF_TUNER_FE_POLARIZATION_R == Polar)
    {
        //  printf("the ant lnb is 13 V !!! \n");

        readData |= 0x10;
        readData &= 0xEF;
    }
    else
    {
        //readData |= 0x10;
        readData &= 0xEF;
        readData |= 0x10;
    }

    HDIC2501_WriteRegister(0x12,readData);

	return HDIC_NO_ERROR ;
}

U8 HDIC2501_Standby()
{
 #if 0  
    U32 err = HDIC_NO_ERROR ;

    err =  HI_UNF_GPIO_Init();
    err |= HI_UNF_GPIO_SetDirBit( HDIC2501_RESET_GPIO, HDIC_GPIO_OUTPUT);
    err |= HI_UNF_GPIO_WriteBit ( HDIC2501_RESET_GPIO, HDIC_GPIO_LOW);
    HDIC2501_Wait(30) ;
    err |= HI_UNF_GPIO_DeInit();

    if ( HI_SUCCESS == err )
        return HDIC_NO_ERROR ;
    else
        return HDIC_ERROR_FAIL ;
#else
    return HDIC_NO_ERROR ;
#endif
}

U8 HDIC2501_WakeUp()
{
#if 0
    U32 err = HDIC_NO_ERROR ;
    
    err =  HI_UNF_GPIO_Init();
    err |= HI_UNF_GPIO_SetDirBit( HDIC2501_RESET_GPIO, HDIC_GPIO_OUTPUT);
    err |= HI_UNF_GPIO_WriteBit ( HDIC2501_RESET_GPIO, HDIC_GPIO_HIGH);
    HDIC2501_Wait(5) ;
    err |= HI_UNF_GPIO_DeInit();

    if ( HI_SUCCESS == err )
        return HDIC_NO_ERROR ;
    else
        return HDIC_ERROR_FAIL ;
#else
    return HDIC_NO_ERROR ;
#endif
}

#endif //_USE_HD2501_CHIP_
