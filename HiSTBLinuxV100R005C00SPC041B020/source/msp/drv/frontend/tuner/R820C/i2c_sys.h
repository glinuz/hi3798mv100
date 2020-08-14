//#include "stdafx.h"
//#include "usbi2cio.h"
#include "R820C.h"
#include "hi_type.h"
#if 0

typedef WORD(CALLBACK* GETDLLVERSION)(void);
typedef HANDLE(CALLBACK* OPENDEVICEINSTANCE)(LPSTR,BYTE);
typedef BOOL(CALLBACK* CLOSEDEVICEINSTANCE)(HANDLE);
typedef BOOL(CALLBACK* DETECTDEVICE)(HANDLE);
typedef BYTE(CALLBACK* GETDEVICECOUNT)(LPSTR);
typedef BYTE(CALLBACK* GETDEVICEINFO)(LPSTR,LPDEVINFO);
typedef HANDLE(CALLBACK* OPENDEVICEBYSERIALID)(LPSTR,LPSTR);
typedef BOOL(CALLBACK* GETSERIALID)(HANDLE,LPSTR);
typedef BOOL(CALLBACK* CONFIGIOPORTS)(HANDLE,ULONG);
typedef BOOL(CALLBACK* GETIOCONFIG)(HANDLE,LPLONG);
typedef BOOL(CALLBACK* READIOPORTS)(HANDLE,LPLONG);
typedef BOOL(CALLBACK* WRITEIOPORTS)(HANDLE,ULONG,ULONG);
typedef BOOL(CALLBACK* READI2C)(HANDLE,PI2C_TRANS);
typedef BOOL(CALLBACK* WRITEI2C)(HANDLE,PI2C_TRANS);
#endif
typedef struct _I2C_LEN_TYPE
{
	UINT8 RegAddr;
	UINT8 Data[50];
	UINT8 Len;
}I2C_LEN_TYPE;

typedef struct _I2C_TYPE
{
	UINT8 RegAddr;
	UINT8 Data;
}I2C_TYPE;

#if 0
bool I2C_Init();
bool Close_I2C();
#endif
extern HI_U32 g_r820c_tuner_i2c_channel ;
extern HI_U32 g_r820c_tuner_port;
bool I2C_Write_Len( I2C_LEN_TYPE *I2C_Info);
bool I2C_Read_Len( I2C_LEN_TYPE *I2C_Info);
bool I2C_Write( I2C_TYPE *I2C_Info);
int R828_Convert(int InvertNum);
