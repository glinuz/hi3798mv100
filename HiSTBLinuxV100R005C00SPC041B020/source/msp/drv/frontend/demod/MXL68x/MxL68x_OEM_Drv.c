/*******************************************************************************
 *
 * FILE NAME          : MxL68x_OEM_Drv.c
 * 
 * AUTHOR             : Brenndon Lee
 *                      Dong Liu 
 *
 * DATE CREATED       : 07/30/2009
 *                      05/10/2012  
 *
 * DESCRIPTION        : This file contains I2C driver and Sleep functins that 
 *                      OEM should implement for MxL68x APIs
 *                             
 *******************************************************************************
 *                Copyright (c) 2010, MaxLinear, Inc.
 ******************************************************************************/
#include <linux/time.h>
#include <linux/string.h>
#include <asm/unistd.h>

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <asm/delay.h>

//hisilicon
#include "hi_type.h"

#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_drv_proc.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"


#include "MxL68x_OEM_Drv.h"

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_WriteRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C write operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_OEM_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 RegData)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // OEM should implement I2C write protocol that complies with MxL68x I2C
  // format.
  // 16 bit Register Write Protocol:
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |RegAddr(H)| |RegAddr(L)| |RegData(H)| |RegData(L)| |P|
  // +------+-+-----+-+-+----------+-+----------+-+----------+-+----------+-+-+
  // |SLAVE |         |A|          |A|          |A|          |A|          |A| |
  // +------+---------+-+----------+-+----------+-+----------+-+----------+-+-+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Ack), N(NACK), 
  // P(Stop condition)
    UINT8 sout[2];
    HI_S32 s32Ret = HI_SUCCESS;
	    	
    oem_data_t * user_data = (oem_data_t *) MxL68x_OEM_DataPtr[devId];
	   	   
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
		
		sout[0] = (UINT8)((RegData >> 8) & 0x00ff);
        sout[1] = (UINT8)(RegData & 0x00ff);	

        if(HI_STD_I2C_NUM >i2cIndex)
     	{
            s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, regAddr,2, sout, 2);
		   
     	    if(HI_SUCCESS != s32Ret)
     	    {
                HI_ERR_TUNER("---->[Failed] i2c:%d,addr:0x%x,reg:0x%x\n",i2cIndex,i2cAddress,regAddr);
     	        return MXL_FAILURE;
     	    }
            else
            {
                //HI_ERR_TUNER("---->[OK] i2c:%d,addr:0x%x,reg:0x%x,data:%x, and send[0:0x%x, 1:0x%x]\n",i2cIndex,i2cAddress,RegAddr,RegData,sout[0],sout[1]);
            }
     	}
     	else
     	{
     	    //s32Ret = HI_DRV_GPIOI2C_WriteExt(i2cIndex,i2cAddress, regAddr, 2, sout, 2);
	        s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(i2cIndex,i2cAddress, regAddr, 2, sout, 2);
     	    if(HI_SUCCESS != s32Ret)
     	    {
     	        return MXL_FAILURE;
     	    }
     	}	
	}
  
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxL68x_Ctrl_ReadRegister
--| 
--| AUTHOR        : Brenndon Lee
--|
--| DATE CREATED  : 7/30/2009
--|
--| DESCRIPTION   : This function does I2C read operation.
--|
--| RETURN VALUE  : True or False
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare68x_OEM_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *DataPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // OEM should implement I2C read protocol that complies with MxL68x I2C
  // format.

  // 16 Register Read Protocol:
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFB| |RegAddr(H)| |RegAddr(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // |SLAVE |         |A|    |A|    |A|          |A|          |A| |
  // +------+-+-----+-+-+----+-+----+-+----------+-+----------+-+-+
  // +------+-+-----+-+-+-----+-----+--+-+
  // |MASTER|S|SADDR|R| |     |     |MN|P|
  // +------+-+-----+-+-+-----+-----+--+-+
  // |SLAVE |         |A|Data |Data |  | |
  // +------+---------+-+-----+-----+--+-+
  // Legends: SADDR(I2c slave address), S(Start condition), MA(Master Ack), MN(Master NACK), 
  // P(Stop condition)
	HI_U16  readAddr;
	UINT8 sout[2];
	HI_S32 s32Ret = HI_SUCCESS;
	
  	oem_data_t * user_data = (oem_data_t *) MxL68x_OEM_DataPtr[devId];
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
        readAddr = ((regAddr<<8) & 0xff00)|((regAddr >> 8) & 0x00ff);

        if(HI_STD_I2C_NUM > i2cIndex)
        {
            s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, (0xff << 8) | (0xfb),2, (HI_U8*)&readAddr, 2);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("---->[Failed] pfnI2cWrite 0xff 0xfd regAddr[0x%x] \n",regAddr);
                return MXL_FAILURE;
            }
            MxLWare68x_OEM_SleepInUs(100);
            s32Ret = s_tuner_pI2cFunc->pfnI2cReadDirectly(i2cIndex, i2cAddress, readAddr, 0, sout, 2);
            if(HI_SUCCESS != s32Ret)
            {
                *DataPtr = 0;
                HI_ERR_TUNER("---->[Failed] i2c:%d,addr:0x%x,reg:0x%x\n",i2cIndex,i2cAddress,regAddr);
                return MXL_FAILURE;
            }
            else
            {
                //HI_ERR_TUNER("----->read regester reg:0x%04x,size:0x%x, date[0:0x%02x, 1:0x%02x]\n",(UINT16)readAddr,2,sout[0],sout[1]);
            }
	    }

	    else
	    {
    	    s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(i2cIndex, i2cAddress,readAddr, 4, sout, 2);
	        if(HI_SUCCESS != s32Ret)
	        {
	            *DataPtr = 0;
	            return MXL_FAILURE;
	        }
	    }
	   *DataPtr = (sout[0] << 8) | sout[1] ;
	}
  
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_WriteBlock
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete block write function
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8* bufPtr)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    // OEM should implement sleep operation 


    HI_S32 s32Ret = HI_SUCCESS;

    oem_data_t * user_data = (oem_data_t *) MxL68x_OEM_DataPtr[devId];  
    if (user_data)
    {
      UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
      UINT8 i2cAddress = user_data->i2cAddress;
      //ret = bufSize % 10;

      if(HI_STD_I2C_NUM >i2cIndex)
      {
         s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, regAddr,2, bufPtr, bufSize);
         
          if(HI_SUCCESS != s32Ret)
          {
              HI_ERR_TUNER("---->[Failed] i2c:%d,addr:0x%x,reg:0x%x\n",i2cIndex,i2cAddress,regAddr);
              return MXL_FAILURE;
          }
		  else
		  {
		     /* HI_ERR_TUNER("---->[OK] i2c:%d,addr:0x%x,reg:0x%x,size:%d,begin value:0x%x. list 4:\n",i2cIndex,i2cAddress,regAddr,bufSize,*bufPtr);
              UINT8 i;
              for(i=0;i<(bufSize>4 ? 4:bufSize);i++)
              {
                   printk(" 0x%x",*(bufPtr+i));
              }
              printk("\n");*/
		  }
      }
      else
      {
          s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(i2cIndex,i2cAddress, regAddr, 2, bufPtr, bufSize);
          if(HI_SUCCESS != s32Ret)
          {
              return MXL_FAILURE;
          }
      }
      
    }

  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_ReadBlock
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete block read function
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, UINT8* bufPtr)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    // OEM should implement sleep operation 
    HI_U16 readAddr;
    HI_S32 s32Ret = 0;

    oem_data_t * user_data = (oem_data_t *) MxL68x_OEM_DataPtr[devId];
    if (user_data)
    {
        UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
        UINT8 i2cAddress = user_data->i2cAddress;

		readAddr = ((regAddr<<8) & 0xff00)|((regAddr >> 8) & 0x00ff);
		//readAddr = regAddr;
		//HI_ERR_TUNER("---->readaddr:0x%04x,regaddr:0x%04x\n",readAddr,regAddr);

        if(HI_STD_I2C_NUM > i2cIndex)
        {
          s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, (0xff << 8) | (0xfd ),2, (HI_U8*)&readAddr, 2);
          if(HI_SUCCESS != s32Ret)
          {
              HI_ERR_TUNER("---->[Failed] pfnI2cWrite 0xff 0xfd regAddr[0x%x] \n",readAddr);
              return MXL_FAILURE;
          }
          MxLWare68x_OEM_SleepInUs(100);
          s32Ret = s_tuner_pI2cFunc->pfnI2cReadDirectly(i2cIndex, i2cAddress, readAddr, 0, bufPtr, readSize);
          if(HI_SUCCESS != s32Ret)
          {
              HI_ERR_TUNER("---->[Failed] i2c:%d,addr:0x%x,reg:0x%04x,block size:0x%x\n",i2cIndex,i2cAddress,regAddr,readSize);
              return MXL_FAILURE;
          }
          else
          {
              //HI_ERR_TUNER("----->read block reg:0x%04x,size:0x%x, date[0:0x%02x, 1:0x%02x]\n",(UINT16)readAddr,readSize,bufPtr[0],bufPtr[1]);
          }
      }
      else
      {
         // s32Ret = HI_DRV_GPIOI2C_ReadExt(i2cIndex, i2cAddress,readAddr, 4, bufPtr, readSize);
          s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(i2cIndex, i2cAddress,readAddr, 4, bufPtr, readSize);
          if(HI_SUCCESS != s32Ret)
          {
              return MXL_FAILURE;
          }
      }  

  
  }

  
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_RegisterCallBack
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function registers call back function which used to 
--|                 read AC data 
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_RegisterCallBack(INTERRUPT_TRIGGERED_CB_FN FuncPtr, UINT8 IntSrcId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  HI_ERR_TUNER("---->just for test.\n");
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare68x_OEM_UnRegisterCallBack
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete unregisters call back function which  
--|                 used to read AC data 
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
MXL_STATUS_E MxLWare68x_OEM_UnRegisterCallBack(UINT8 IntSrcId)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  // OEM should implement sleep operation 
  HI_ERR_TUNER("---->just for test.\n");
  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MXL_Sleep
--| 
--| AUTHOR        : Dong Liu
--|
--| DATE CREATED  : 01/10/2010
--|
--| DESCRIPTION   : This function complete sleep operation. WaitTime is in us unit
--|
--| RETURN VALUE  : None
--|
--|---------------------------------------------------------------------------*/
void MxLWare68x_OEM_SleepInUs( UINT32 delayTimeInUs)
{
  // OEM should implement sleep operation 
  if(delayTimeInUs < 1000)
  {
    udelay(delayTimeInUs);
  }
  else
  {
     mdelay(delayTimeInUs/1000);
  }
}
