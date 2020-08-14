//#include "stdafx.h"
#include "i2c_sys.h"

#include "drv_i2c_ext.h"
#include "hi_debug.h"

#include "drv_demod.h"

HI_U32 g_r820c_tuner_i2c_channel = 0;
HI_U32 g_r820c_tuner_port = 0;
HI_U8 data[51];
#if 0
BOOL flag;
LONG *IoRead;
LONG ulIoPortConfig;
HANDLE hDevice[127];
I2C_TRANS TransI2C;
HINSTANCE hDLL;
GETDLLVERSION GetDllVersion;
OPENDEVICEINSTANCE OpenDeviceInstance;
CLOSEDEVICEINSTANCE CloseDeviceInstance;
DETECTDEVICE DetectDevice;
GETDEVICECOUNT GetDeviceCount;
GETDEVICEINFO GetDeviceInfo;
OPENDEVICEBYSERIALID OpenDeviceBySerialId;
GETSERIALID GetSerialId;
CONFIGIOPORTS ConfigIoPorts;
GETIOCONFIG GetIoConfig;
READIOPORTS ReadIoPorts;
WRITEIOPORTS WriteIoPorts;
READI2C ReadI2c;
WRITEI2C WriteI2c;

bool I2C_Init()
{
	hDLL = LoadLibrary("UsbI2cIo.dll");          // attempt load dll
	if( hDLL != NULL) {
		// got handle to dll, now initialize API function pointers
		GetDllVersion=(GETDLLVERSION)GetProcAddress(hDLL,"DAPI_GetDllVersion");
		OpenDeviceInstance=(OPENDEVICEINSTANCE)GetProcAddress(hDLL,"DAPI_OpenDeviceInstance");
		CloseDeviceInstance=(CLOSEDEVICEINSTANCE)GetProcAddress(hDLL,"DAPI_CloseDeviceInstance");
		DetectDevice=(DETECTDEVICE)GetProcAddress(hDLL,"DAPI_DetectDevice");
		GetDeviceCount=(GETDEVICECOUNT)GetProcAddress(hDLL,"DAPI_GetDeviceCount");
		GetDeviceInfo=(GETDEVICEINFO)GetProcAddress(hDLL,"DAPI_GetDeviceInfo");
		OpenDeviceBySerialId=(OPENDEVICEBYSERIALID)GetProcAddress(hDLL,"DAPI_OpenDeviceBySerialId");
		GetSerialId=(GETSERIALID)GetProcAddress(hDLL,"DAPI_GetSerialId");
		ConfigIoPorts=(CONFIGIOPORTS)GetProcAddress(hDLL,"DAPI_ConfigIoPorts"); 
		GetIoConfig=(GETIOCONFIG)GetProcAddress(hDLL,"DAPI_GetIoConfig"); 
		ReadIoPorts=(READIOPORTS)GetProcAddress(hDLL,"DAPI_ReadIoPorts");
		WriteIoPorts=(WRITEIOPORTS)GetProcAddress(hDLL,"DAPI_WriteIoPorts");
		ReadI2c=(READI2C)GetProcAddress(hDLL,"DAPI_ReadI2c");
		WriteI2c=(WRITEI2C)GetProcAddress(hDLL,"DAPI_WriteI2c");
	}
	else
		return false;
	
	
	hDevice[0]=OpenDeviceInstance("UsbI2cIo", 0);
	if(hDevice == INVALID_HANDLE_VALUE) 
	return false;

  TransI2C.byTransType = I2C_TRANS_8ADR;

	return true;
}

bool Close_I2C()
{
	CloseDeviceInstance(hDevice[0]);
	return true;
}
#endif
bool I2C_Write_Len(I2C_LEN_TYPE *I2C_Info)
{
#if 0
	int DataCunt = 0;
	LONG lWrite   = 0;

	TransI2C.bySlvDevAddr = R828_ADDRESS;
	TransI2C.wMemoryAddr  = I2C_Info->RegAddr;
	TransI2C.wCount       = I2C_Info->Len;

	for(DataCunt = 0;DataCunt < TransI2C.wCount;DataCunt ++)
	{
		TransI2C.Data[DataCunt] = I2C_Info->Data[DataCunt];
	}

	lWrite = WriteI2c(hDevice[0], &TransI2C);

	if(lWrite != TransI2C.wCount)
		return false;
	
	return true;
#endif
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
    
    qam_config_i2c_out(g_r820c_tuner_port,1);
    
    //status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port].enI2cChannel, g_stTunerOps[u32TunerPort].u32TunerAddress, &I2cDataStr);
    status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port].enI2cChannel, R828_ADDRESS, &I2cDataStr );

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



bool I2C_Read_Len(I2C_LEN_TYPE *I2C_Info)
{
#if 0
	int DataCunt = 0;
	LONG lRead   = 0;

	TransI2C.bySlvDevAddr = R828_ADDRESS;
	TransI2C.wMemoryAddr  = 0x00;
	TransI2C.wCount       = I2C_Info->Len;

	lRead = ReadI2c(hDevice[0], &TransI2C);

	if(lRead != TransI2C.wCount)
		return false;

	for(DataCunt = 0;DataCunt < TransI2C.wCount;DataCunt ++)
	{
		 I2C_Info->Data[DataCunt] = R828_Convert(TransI2C.Data[DataCunt]);
	}
	
	return true;
#endif
    TUNER_I2C_DATA_S stDataStruct = {0};
    HI_S32 s32status = HI_SUCCESS;
    HI_U8 subaddress = 0;
    HI_S32 DataCunt = 0;
    
    qam_config_i2c_out(g_r820c_tuner_port, 1); 

    stDataStruct.pu8ReceiveBuf = I2C_Info->Data;
    stDataStruct.u32ReceiveLength = I2C_Info->Len;
    stDataStruct.pu8SendBuf = &subaddress;
    stDataStruct.u32SendLength = 1;

    s32status = tuner_i2c_receive_data(g_stTunerOps[g_r820c_tuner_port].enI2cChannel, R828_ADDRESS, &stDataStruct);

    if(HI_SUCCESS != s32status)
    {
        HI_ERR_TUNER( "tuner_i2c_receive_data error s32status = 0x%x\n", s32status );
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



bool I2C_Write(I2C_TYPE *I2C_Info)
{
#if 0
	int DataCunt = 0;
	LONG lWrite   = 0;

	TransI2C.bySlvDevAddr = R828_ADDRESS;
	TransI2C.wMemoryAddr  = I2C_Info->RegAddr;
	TransI2C.wCount       = 1;
	TransI2C.Data[0]      = I2C_Info->Data;

	lWrite = WriteI2c(hDevice[0], &TransI2C);

	if(lWrite != TransI2C.wCount)
		return false;
	
	return true;
#endif
    HI_S32 status = RT_Success;
    TUNER_I2C_DATA_S I2cDataStr;

    data[0] = I2C_Info->RegAddr;
    data[1] = I2C_Info->Data;
    I2cDataStr.pu8ReceiveBuf = 0;
    I2cDataStr.u32ReceiveLength = 0;
    I2cDataStr.pu8SendBuf = data;
    I2cDataStr.u32SendLength = 2;
    qam_config_i2c_out(g_r820c_tuner_port,1);

    status = tuner_i2c_send_data( g_stTunerOps[g_r820c_tuner_port].enI2cChannel, R828_ADDRESS, &I2cDataStr );
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
        HI_ERR_TUNER("<0>write_I2C error, 0x%x\n", status);
        return RT_Fail;
    }
}


int R828_Convert(int InvertNum)
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