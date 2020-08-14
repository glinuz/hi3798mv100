#include "hi_type.h"
#include "R836.h"

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

extern HI_U32 g_r836_tuner_i2c_channel;
extern HI_U32 g_r836_tuner_port;
extern UINT8 R840_ADDRESS;
bool R836_I2C_Write_Len(I2C_LEN_TYPE *I2C_Info);
bool R836_I2C_Read_Len(I2C_LEN_TYPE *I2C_Info);
bool R836_I2C_Write(I2C_TYPE *I2C_Info);
int R836_Convert(int InvertNum);
void R836_I2C_SetPort(unsigned int u32TunerPort);
