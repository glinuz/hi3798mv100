#include "i2c_sys.h"
#include "R820C.h"

//#include "hi_i2c.h"
#include "hi_debug.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"

HI_U32 g_r820c_tuner_i2c_channel = 0;
HI_U32 g_r820c_tuner_port[3] = {0, 0, 0};
HI_U8 data[51];

R828_ErrCode I2C_Write_Len(R828_TUNER_NUM_TYPE R828_Tuner_Num, I2C_LEN_TYPE *I2C_Info)
{
    HI_S32 status = HI_SUCCESS;
    TUNER_I2C_DATA_S I2cDataStr;
    HI_U32 i=0;

    data[0] = I2C_Info->RegAddr;
    for( i=1;i<=I2C_Info->Len;i++ )
        data[i] = I2C_Info->Data[i-1];
    I2cDataStr.pu8ReceiveBuf = 0;
    I2cDataStr.u32ReceiveLength = 0;
    I2cDataStr.pu8SendBuf = data;
    
    I2cDataStr.u32SendLength = I2C_Info->Len+1;
    
    qam_config_i2c_out(g_r820c_tuner_port[R828_Tuner_Num],1);
    
    //status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port].enI2cChannel, g_stTunerOps[u32TunerPort].u32TunerAddress, &I2cDataStr);
    status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port[R828_Tuner_Num]].enI2cChannel, R828_ADDRESS, &I2cDataStr );

    if (HI_SUCCESS == status)
    {
        return RT_Success;
    }
    else
    {
        return RT_Fail;
    }
#if 0
    for( i=0;i<=I2C_Info->Len;i++ )
    {
        HI_INFO_TUNER("The data write in is reg 0x%x:0x%x",0x5+i,I2C_Info->Data[i]);
    }

    R828_ErrCode ret = 0;
    I2C_Info -> Len = 32;
    I2C_Info -> RegAddr = 0;
    ret = I2C_Read_Len(I2C_Info);

    for( i=0;i<=I2C_Info->Len;i++ )
    {
        HI_INFO_TUNER("The data write in is reg 0x%x:0x%x",i,I2C_Info->Data[i]);
    }
#endif
}



R828_ErrCode I2C_Read_Len(R828_TUNER_NUM_TYPE R828_Tuner_Num, I2C_LEN_TYPE *I2C_Info)
{
    TUNER_I2C_DATA_S stDataStruct = {0};
    HI_S32 s32status = HI_SUCCESS;
    HI_U8 subaddress = 0;
    HI_S32 DataCunt = 0;
    
    qam_config_i2c_out(g_r820c_tuner_port[R828_Tuner_Num], 1); 

    stDataStruct.pu8ReceiveBuf = I2C_Info->Data;
    stDataStruct.u32ReceiveLength = I2C_Info->Len;
    stDataStruct.pu8SendBuf = &subaddress;
    stDataStruct.u32SendLength = 1;

    s32status = tuner_i2c_receive_data(g_stTunerOps[g_r820c_tuner_port[R828_Tuner_Num]].enI2cChannel, R828_ADDRESS, &stDataStruct);

    if(HI_SUCCESS != s32status)
    {
        TUNER_DEBUG( "tuner_i2c_receive_data error s32status = 0x%x\n", s32status );
        return RT_Fail;
    }
    else
    {
        for(DataCunt = 0;DataCunt < I2C_Info->Len;DataCunt ++)
        {
            I2C_Info->Data[DataCunt] = R828_Convert(I2C_Info->Data[DataCunt]);
        }

        return RT_Success;
    }
}


//#define I2C_Write R820C_I2C_Write

R828_ErrCode I2C_Write(R828_TUNER_NUM_TYPE R828_Tuner_Num, I2C_TYPE *I2C_Info)
{
    //int DataCunt = 0;
    HI_S32 status = RT_Success;
    TUNER_I2C_DATA_S I2cDataStr;

    data[0] = I2C_Info->RegAddr;
    data[1] = I2C_Info->Data;
    I2cDataStr.pu8ReceiveBuf = 0;
    I2cDataStr.u32ReceiveLength = 0;
    I2cDataStr.pu8SendBuf = data;
    I2cDataStr.u32SendLength = 2;
    qam_config_i2c_out(g_r820c_tuner_port[R828_Tuner_Num],1);

    status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port[R828_Tuner_Num]].enI2cChannel, R828_ADDRESS, &I2cDataStr );
#if 0
    HI_INFO_TUNER("write in reg 0x%x:0x%x\n",I2C_Info->RegAddr,I2C_Info->Data);

    R828_ErrCode ret = 0;
    I2C_LEN_TYPE Info;
    Info . Len = 32;
    Info . RegAddr = 0;
    ret = I2C_Read_Len(&Info);

    HI_S32 i = I2C_Info->RegAddr;
    //for( i=0;i<32;i++ )
    {
        HI_INFO_TUNER("read from reg 0x%x:0x%x\n",i,Info.Data[i]);
    }
#endif
    if (HI_SUCCESS == status)
    {
        return RT_Success;
    }
    else
    {
        TUNER_DEBUG("<0>write_I2C error, 0x%x\n", status);
        return RT_Fail;
    }
}


HI_S32 R828_Convert(int InvertNum)
{
	int ReturnNum = 0;
	int AddNum    = 0x80;
	int BitNum    = 0x01;
	int CuntNum   = 0;

	for(CuntNum = 0;CuntNum < 8;CuntNum ++)
	{
		if(BitNum & InvertNum)
			ReturnNum += AddNum;

		AddNum /= 2;
		BitNum *= 2;
	}

	return ReturnNum;
}
