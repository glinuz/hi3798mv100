/*****************************************************************************************
 *
 * FILE NAME          : MxL101SF_OEM_Drv.cpp
 *
 * AUTHOR             : Brenndon Lee, Mahendra Kondur
 *
 * DATE CREATED       : 1/24/2008
 *                      6/3/2010 - Description of I2C protocol compatible to MxL
 *
 * DESCRIPTION        : This file contains I2C emulation routine
 *
 *****************************************************************************************
 *                Copyright (c) 2006, MaxLinear, Inc.
 ****************************************************************************************/
#include <linux/time.h>
#include <linux/delay.h>

#include "hi_type.h"
#include "drv_i2c_ext.h"
#include "hi_drv_sys.h"
#include "drv_tuner_ioctl.h"

#include "MxL101SF_OEM_Drv.h"

extern HI_U32 u32CurTunerPort;
/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_ReadRegister
--|
--| AUTHOR        : Brenndon Lee, Mahendra Kondur
--|
--| DATE CREATED  : 1/24/2008
--|                 6/3/2010 - Description MxL101SF I2C read operation.
--|
--| DESCRIPTION   : This function does I2C read operation with MxL101SF.
--|                 The function accepts i2c slave address of MxL101SF along with
--|                 register address, pointer for register data and a pointer to a
--|                 private data structure which can be used for custom specific
--|                 operations. If private data structure is not required, user can
--|                 pass NULL.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_ReadRegister(UINT8 regAddr, UINT8 *dataPtr)
{
MXL_STATUS status = MXL_TRUE;

  // User should implememnt his own I2C read register routine corresponding to
  // his hardaware.

  //printf("MxL Reg Read : Addr - 0x%x, data - 0x%x\n", regAddr, *dataPtr);

    HI_U32  u32RegAddr = 0;
    HI_U32  u32RegNum = 0;
    HI_U32 u32TunerPort = 0;

    u32RegAddr = (0xFB<<8) | regAddr;
    u32RegNum = 2;
    u32TunerPort = (HI_U32)u32CurTunerPort;

    //if (HI_UNF_I2C_CHANNEL_QAM < g_stTunerOps[u32TunerPort].enI2cChannel)
        //status = gpio_i2c_read_ext(g_stTunerOps[u32TunerPort].enI2cChannel, I2CSlaveAddr << 1, u32RegAddr, u32RegNum, DataPtr, 1);
    //else
    if (/*HI_UNF_I2C_CHANNEL_QAM*/4 >= g_stTunerOps[u32TunerPort].enI2cChannel)
    {
        status = HI_DRV_I2C_Read(g_stTunerOps[u32TunerPort].enI2cChannel, (g_stTunerOps[u32TunerPort].u32DemodAddress) << 1, u32RegAddr, u32RegNum, dataPtr, 1);
    }
    if(MXL_TRUE != status)
    {
        return status;
    }

    return MXL_TRUE;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_WriteRegister
--|
--| AUTHOR        : Brenndon Lee, Mahendra Kondur
--|
--| DATE CREATED  : 1/24/2008
--|                 6/3/2010 - Description MxL101SF I2C write operation.
--|
--| DESCRIPTION   : This function does I2C write operation with MxL101SF.
--|                 The function accepts i2c slave address of MxL101SF along with
--|                 register address, register data value and a pointer to a
--|                 private data structure which can be used for custom specific
--|                 operations. If private data structure is not required, user can
--|                 pass NULL.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_WriteRegister(UINT8 regAddr, UINT8 regData)
{
    MXL_STATUS status = MXL_TRUE;

  // User should implememnt his own I2C write register routine corresponding to
  // his hardaware.

  //printf("MxL Reg Write : Addr - 0x%x, data - 0x%x\n", regAddr, regData);

    HI_U32 u32RegAddr = 0;
    HI_U32 u32RegNum  = 0;
    HI_U32 u32TunerPort = 0;

    u32RegAddr = regAddr;
    u32RegNum = 1;
    u32TunerPort = (HI_U32)u32CurTunerPort;

    if (/*HI_UNF_I2C_CHANNEL_QAM*/4 >= g_stTunerOps[u32TunerPort].enI2cChannel)
    {
        status = HI_DRV_I2C_Write(g_stTunerOps[u32TunerPort].enI2cChannel, (g_stTunerOps[u32TunerPort].u32DemodAddress) << 1, u32RegAddr, u32RegNum, &regData, 1);
    }
    if (MXL_TRUE != status)
    {
        return status;
    }

    return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_Sleep
--|
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 8/31/2009
--|
--| DESCRIPTION   : This function will cause the calling thread to be suspended
--|                 from execution until the number of milliseconds specified by
--|                 the argument time has elapsed
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_Sleep(UINT16 TimeinMilliseconds)
{
    MXL_STATUS status = MXL_TRUE;

    /* User should implememnt his own sleep routine corresponding to
    his Operating System platform. */

    /* printf("Ctrl_Sleep : %d msec's\n", Time);  */
    msleep_interruptible(TimeinMilliseconds);
    return status;
}

/*----------------------------------------------------------------------------------------
--| FUNCTION NAME : Ctrl_GetTime
--|
--| AUTHOR        : Mahendra Kondur
--|
--| DATE CREATED  : 10/05/2009
--|
--| DESCRIPTION   : This function will return current system's timestamp in
--|                 milliseconds resolution.
--|
--| RETURN VALUE  : True or False
--|
--|-------------------------------------------------------------------------------------*/

MXL_STATUS Ctrl_GetTime(UINT32 *TimeinMilliseconds)
{
  //*TimeinMilliseconds = current systems timestamp in milliseconds.
  // User should implement his own routine to get current system's timestamp in milliseconds.

    (HI_VOID)HI_DRV_SYS_GetTimeStampMs(TimeinMilliseconds);
    return MXL_TRUE;
}
