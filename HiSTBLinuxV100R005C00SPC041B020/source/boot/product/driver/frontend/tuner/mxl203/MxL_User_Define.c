/*
 
 Driver APIs for MxL203RF Tuner
 
 Copyright, Maxlinear, Inc.
 All Rights Reserved
 
 File Name:      MxL_User_Define.c

 */
//#include <linux/delay.h>
//#include <linux/seq_file.h>

//#include "hi_i2c.h"
#include "MxL_User_Define.h"
#include "MxL203RF_Common.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
//#include "hi_timer.h"


static HI_U32 g_MxlTunerPort;
static HI_U32 g_MxlI2cChnNum;


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//																		   //
//					I2C Functions (implement by customer)				   //
//																		   //
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

/******************************************************************************
**
**  Name: MxL_I2C_Write
**
**  Description:    I2C write operations
**
**  Parameters:    	
**					DeviceAddr	- MxL203RF Device address
**					pArray		- Write data array pointer
**					count		- total number of array
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
UINT32 MxL_I2C_Write(UINT8 DeviceAddr, UINT8* pArray, UINT32 count)
{
    TUNER_I2C_DATA_S stI2cData;

    qam_config_i2c_out(g_MxlTunerPort,1);
    stI2cData.pu8ReceiveBuf = NULL;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = pArray;
    stI2cData.u32SendLength = count;
    tuner_i2c_send_data(g_MxlI2cChnNum,DeviceAddr,&stI2cData);

	return MxL_OK;	
}

/******************************************************************************
**
**  Name: MxL_I2C_Read
**
**  Description:    I2C read operations
**
**  Parameters:    	
**					DeviceAddr	- MxL203RF Device address
**					Addr		- register address for read
**					*Data		- data return
**
**  Returns:        0 if success
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
UINT32 MxL_I2C_Read(UINT8 DeviceAddr, UINT8 Addr, UINT8* mData)
{
    /* first step */
    TUNER_I2C_DATA_S stI2cData;
    UINT8 Array[2];

#if 0
    config_i2c_out(g_MxlTunerPort,1);

    Array[0] = 0xFB;
    Array[1] = Addr;

    stI2cData.pu8ReceiveBuf = NULL;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = Array;
    stI2cData.u32SendLength = 2;
    tuner_i2c_send_data(g_MxlI2cChnNum,DeviceAddr,&stI2cData);

    /* second step */
    config_i2c_out(g_MxlTunerPort,1);
    stI2cData.pu8ReceiveBuf = mData;
    stI2cData.u32ReceiveLength = 1;
    stI2cData.pu8SendBuf = NULL;
    stI2cData.u32SendLength = 0;
    tuner_i2c_receive_data(g_MxlI2cChnNum,DeviceAddr,&stI2cData);
#else
    Array[0] = Addr;
    Array[1] = 0xFB;

    qam_config_i2c_out(g_MxlTunerPort,1);
    stI2cData.pu8ReceiveBuf = mData;
    stI2cData.u32ReceiveLength = 1;
    stI2cData.pu8SendBuf = Array;
    stI2cData.u32SendLength = 2;

    tuner_i2c_receive_data(g_MxlI2cChnNum,DeviceAddr,&stI2cData);
#endif    
	return MxL_OK;    
}

/******************************************************************************
**
**  Name: MxL_Delay
**
**  Description:    Delay function in milli-second
**
**  Parameters:    	
**					mSec		- milli-second to delay
**
**  Returns:        0
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   12-16-2007   khuang initial release.
**
******************************************************************************/
void MxL_Delay(UINT32 mSec)
{
    //msleep_interruptible(mSec);
    HI_TIMER_mdelay(mSec);
}


void MxL_I2C_SetChn(UINT32 I2CChn)
{
    g_MxlI2cChnNum = I2CChn;
}

void MxL_I2C_SetPort(UINT32 TunerPort)
{
    g_MxlTunerPort = TunerPort;
}

