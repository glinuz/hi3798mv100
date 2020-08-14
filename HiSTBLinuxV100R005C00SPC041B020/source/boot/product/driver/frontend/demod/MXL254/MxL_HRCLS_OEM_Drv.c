/*******************************************************************************
 * Example MxL_HRCLS_OEM_Drv.c
 ******************************************************************************/

#ifndef S_SPLINT_S

/*#include <linux/time.h>
#include <linux/string.h>
#include <asm/unistd.h>

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>*/

#include "MxL_HRCLS_Common.h"

#include "hi_type.h"

#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"

#include "hi_debug.h"

#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"

/*#include "drv_i2c_ext.h"
#include "drv_gpioi2c_ext.h"
#include "drv_gpio_ext.h"
#include "hi_drv_gpioi2c.h"
#include "hi_debug.h"


#include "hi_drv_stat.h"
#include "hi_drv_dev.h"
#include "hi_drv_reg.h"
#include "hi_kernel_adapt.h"
#include "hi_drv_proc.h"

#include "drv_tuner_ext.h"
#include "drv_tuner_ioctl.h"
#include "hi_drv_diseqc.h"
#include "drv_demod.h"*/


//extern HI_U32 g_u32ResetGpioNo;

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_Reset
--| 
--| DESCRIPTION   : This function resets MxL_HRCLS through Reset Pin
--| PARAMETERS    : devId - MxL_HRCLS device id
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

/*MXL_STATUS_E MxLWare_HRCLS_OEM_Reset(UINT8 devId)
{
 
  // !!! FIXME !!!
  // OEM should toggle reset pin of MxL_HRCLS specified by I2C Slave Addr
    HI_S32 u32Ret = HI_SUCCESS; 
    //u32Ret  = HI_DRV_GPIO_SetDirBit(g_u32ResetCruGpioNo, HI_FALSE);
    u32Ret  = s_tuner_pGpioFunc->pfnGpioDirSetBit(g_u32ResetCruGpioNo, HI_FALSE);
	
    //u32Ret |= HI_DRV_GPIO_WriteBit(g_u32ResetCruGpioNo, 0);
    u32Ret |= s_tuner_pGpioFunc->pfnGpioWriteBit(g_u32ResetCruGpioNo, 0);
    MxLWare_HRCLS_OEM_DelayUsec(100);
    u32Ret |= s_tuner_pGpioFunc->pfnGpioWriteBit(g_u32ResetCruGpioNo, 1);
    if (HI_SUCCESS != u32Ret)
    {
        HI_ERR_TUNER("Demod reset failed !\n");
        return MXL_FAILURE;
    }
    MxLWare_HRCLS_OEM_DelayUsec(100);
    return MXL_SUCCESS;

}*/

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_WriteRegister
--|
--| DESCRIPTION   : This function does I2C write operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to write.
--|                 regData - Data to write to the specified address.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_WriteRegister(UINT8 devId, UINT16 regAddr, UINT16 regData)
{
    MXL_STATUS_E status = MXL_SUCCESS;
    UINT8 sout[2];
    HI_S32 s32Ret = HI_SUCCESS;
	    	
    oem_data_t * user_data = (oem_data_t *) MxL_HRCLS_OEM_DataPtr[devId];
	   	   
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
		
		sout[0] = (UINT8)((regData >> 8) & 0x00ff);
        	sout[1] = (UINT8)(regData & 0x00ff);	

              if(HI_STD_I2C_NUM >i2cIndex)
         	{
         	   s32Ret = HI_DRV_I2C_Write(i2cIndex, i2cAddress, regAddr,2, sout, 2);
               // s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, regAddr,2, sout, 2);
			   
         	    if(HI_SUCCESS != s32Ret)
         	    {
         	        return MXL_FAILURE;
         	    }
         	}
         	else
         	{
         	    s32Ret = HI_DRV_GPIOI2C_WriteExt(i2cIndex,i2cAddress, regAddr, 2, sout, 2);
		        //s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(i2cIndex,i2cAddress, regAddr, 2, sout, 2);
         	    if(HI_SUCCESS != s32Ret)
         	    {
         	        return MXL_FAILURE;
         	    }
         	}	
	}

  	return MXL_SUCCESS;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_ReadRegister
--|
--| DESCRIPTION   : This function does I2C read operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to read.
--|                 dataPtr - Data container to return 16 data.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadRegister(UINT8 devId, UINT16 regAddr, UINT16 *dataPtr)
{
      MXL_STATUS_E status = MXL_SUCCESS;
	HI_U32  readAddr;
	UINT8 sout[2];
	HI_S32 s32Ret = HI_SUCCESS;
	
  	oem_data_t * user_data = (oem_data_t *) MxL_HRCLS_OEM_DataPtr[devId];
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;

		readAddr = (0xff << 24) | (0xfb << 16) | regAddr;
                if(HI_STD_I2C_NUM > i2cIndex)
 		   {
 		          s32Ret = HI_DRV_I2C_Read(i2cIndex, i2cAddress, readAddr, 4, sout, 2);
			    //s32Ret = s_tuner_pI2cFunc->pfnI2cRead(i2cIndex, i2cAddress, readAddr, 4, sout, 2);
 		        if(HI_SUCCESS != s32Ret)
 		        {
 		            *dataPtr = 0;
 		            return MXL_FAILURE;
 		        }
 		    }
 		    else
 		    {
 		          s32Ret = HI_DRV_GPIOI2C_ReadExt(i2cIndex, i2cAddress,readAddr, 4, sout, 2);
			   // s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(i2cIndex, i2cAddress,readAddr, 4, sout, 2);
 		        if(HI_SUCCESS != s32Ret)
 		        {
 		            *dataPtr = 0;
 		            return MXL_FAILURE;
 		        }
 		    }
		   *dataPtr = (sout[0] << 8) | sout[1] ;
		//printk("===========i2c Index:%d, i2cAddress:%x, readAddr:%x\n", i2cIndex, i2cAddress, readAddr);
	//	printk("sout:0x%x5x\n",sout[1],sout[0]);
	}
              return status;
}
  
/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_WriteBlock
--|
--| DESCRIPTION   : This function does I2C block write operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register address of MxL_HRCLS to start a block write.
--|                 bufSize - The number of bytes to write
--|                 bufPtr - Data bytes to write to the specified address.
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_WriteBlock(UINT8 devId, UINT16 regAddr, UINT16 bufSize, UINT8 *bufPtr)
{
  	MXL_STATUS_E status = MXL_SUCCESS;
	HI_S32 s32Ret = HI_SUCCESS;
	
	oem_data_t * user_data = (oem_data_t *) MxL_HRCLS_OEM_DataPtr[devId];

	
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
		//ret = bufSize % 10;
	

		if(HI_STD_I2C_NUM >i2cIndex)
         	{
         	    s32Ret = HI_DRV_I2C_Write(i2cIndex, i2cAddress, regAddr,2, bufPtr, bufSize);
         	   //s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, regAddr,2, bufPtr, bufSize);
         	   
         	    if(HI_SUCCESS != s32Ret)
         	    {
         	        return MXL_FAILURE;
         	    }
         	}
         	else
         	{
         	      s32Ret = HI_DRV_GPIOI2C_WriteExt(i2cIndex,i2cAddress, regAddr, 2, bufPtr, bufSize);
		       // s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(i2cIndex,i2cAddress, regAddr, 2, bufPtr, bufSize);
         	    if(HI_SUCCESS != s32Ret)
         	    {
         	        return MXL_FAILURE;
         	    }
         	}
		
	}


  return status;
}

/*------------------------------------------------------------------------------
--| FUNCTION NAME : MxLWare_HRCLS_OEM_ReadBlock
--|
--| DESCRIPTION   : This function does I2C block read operation.
--| PARAMETERS    : devId - MxL_HRCLS device id
--|                 regAddr - Register Address to start a block read
--|                 readSize - The number of bytes to read
--|                 bufPtr - Container to hold readback data
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlock(UINT8 devId, UINT16 regAddr, UINT16 readSize, UINT8 *bufPtr)
{
  	MXL_STATUS_E status = MXL_SUCCESS;
	HI_U32 readAddr;
	HI_S32 s32Ret = 0;
	
  	oem_data_t * user_data = (oem_data_t *) MxL_HRCLS_OEM_DataPtr[devId];
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
		
		readAddr = (0xff << 24) | (0xfd << 16) | regAddr;
			
		if(HI_STD_I2C_NUM > i2cIndex)
		{
		    s32Ret = HI_DRV_I2C_Read(i2cIndex, i2cAddress, readAddr, 4, bufPtr, readSize);
		   // s32Ret = s_tuner_pI2cFunc->pfnI2cRead(i2cIndex, i2cAddress, readAddr, 4, bufPtr, readSize);
		    if(HI_SUCCESS != s32Ret)
		    {
		        return MXL_FAILURE;
		    }
		}
		else
		{
		    s32Ret = HI_DRV_GPIOI2C_ReadExt(i2cIndex, i2cAddress,readAddr, 4, bufPtr, readSize);
		    //s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExt(i2cIndex, i2cAddress,readAddr, 4, bufPtr, readSize);
		    if(HI_SUCCESS != s32Ret)
		    {
		        return MXL_FAILURE;
		    }
		}	
	
	}
  return status;
}

MXL_STATUS_E MxLWare_HRCLS_OEM_ReadBlockExt(UINT8 devId, UINT16 cmdId, UINT16 offset, UINT16 readSize, UINT8 *bufPtr)
{
  MXL_STATUS_E status = MXL_SUCCESS;

  // !!! FIXME !!!
  // OEM should implement I2C block read protocol that complies with MxL_HRCLS I2C
  // format.
  
  // Block Read Protocol (n bytes of data):
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // |MASTER|S|SADDR|W| |0xFF| |0xFD| |cmdId(H)| |cmdId(L)| |offset(H) | |offset(L)| |P|
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // |SLAVE |         |A|    |A|    |A|        |A|        |A|          |A|         |A| |
  // +------+-+-----+-+-+----+-+----+-+--------+-+--------+-+----------+-+---------+-+-+
  // +------+-+-----+-+-------+-+-----+-+-----+-+-----+-+-+
  // |MASTER|S|SADDR|R|       |A|     |A|       |     |N|P|
  // +------+-+-----+-+-+-----+-+-----+-+  ...  +-----+-+-+
  // |SLAVE |         |A|DATA1| |DATA2| |       |DATAn|   |
  // +------+---------+-+-----+-+-----+-+-----+-+-----+---+
  // Legends: SADDR (I2c slave address), S (Start condition), A (Acknowledgement), N(NACK), P(Stop condition)
  //MXL_STATUS_E status = MXL_SUCCESS;
	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 readAddr = 0;
	HI_U32 regAddr = 0;
	HI_U8  u8data[2] = {0};
	oem_data_t * user_data = (oem_data_t *) MxL_HRCLS_OEM_DataPtr[devId];
	
	if (user_data)
	{
		UINT8 i2cIndex = user_data->i2cIndex; // get device i2c address
		UINT8 i2cAddress = user_data->i2cAddress;
		//ret = bufSize % 10;
		
		regAddr = (0xff << 24) | (0xfd << 16) | cmdId;
		u8data[0]= ( offset >> 8 )& 0xff;
		u8data[1]= ( offset ) & 0xff;
		if(HI_STD_I2C_NUM >i2cIndex)
		{
		   s32Ret = HI_DRV_I2C_Write(i2cIndex, i2cAddress, regAddr,4, u8data, 2);
		   //s32Ret = s_tuner_pI2cFunc->pfnI2cWrite(i2cIndex, i2cAddress, regAddr,4, u8data, 2);
		   
			if(HI_SUCCESS != s32Ret)
			{
				return MXL_FAILURE;
			}
		}
		else
		{
			s32Ret = HI_DRV_GPIOI2C_WriteExt(i2cIndex, i2cAddress, regAddr,4, u8data, 2);
			//s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cWriteExt(i2cIndex, i2cAddress, regAddr,4, u8data, 2);
			if(HI_SUCCESS != s32Ret)
			{
				return MXL_FAILURE;
			}
		}
		
		//read
		  readAddr = (0xff << 24) | (0xfd << 16) | regAddr;
			  
		  if(HI_STD_I2C_NUM > i2cIndex)
		  {
		  
			  //s32Ret = s_tuner_pI2cFunc->pfnI2cReadDirectly(i2cIndex, i2cAddress, readAddr, 0, bufPtr, readSize);
			  if(HI_SUCCESS != s32Ret)
			  {
				  return MXL_FAILURE;
			  }
		  }
		  else
		  {
		   
			  //s32Ret = s_tuner_pGpioI2cFunc->pfnGpioI2cReadExtDiRectly(i2cIndex, i2cAddress,readAddr, 0, bufPtr, readSize);
			  if(HI_SUCCESS != s32Ret)
			  {
				  return MXL_FAILURE;
			  }
		  }   
		
	}

  return status;
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_LoadNVRAMFile
--|
--| DESCRIPTION   : Load MxLNVRAMFile
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_LoadNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen)
{
  MXL_STATUS_E status = MXL_SUCCESS;
  
  // !!! FIXME !!! 
  // To be implemented for customer OEM platform
 

 /************************************************************************************
   Open firmware file in binary mode
  ************************************************************************************/
    struct file *file_filp = NULL;
   /* mm_segment_t old_fs;
    struct inode *inode = NULL;
     int iSize = 0;
     ssize_t ret;

    if(file_filp== NULL)
    {
        file_filp = filp_open("/kmod/nvram50.bin", O_RDONLY, 0644);
    }
    if((file_filp== NULL) ||(IS_ERR(file_filp)))
    {
        HI_ERR_TUNER("error occured while opening file nvram50.bin, exiting...\n");
        return MXL_FAILURE ;
    }
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    inode = file_filp->f_dentry->d_inode;
    iSize = inode->i_size;
     if (iSize < bufLen)
    {
    	status = MXL_FAILURE;	
    	return status;
    }
    file_filp->f_op->llseek(file_filp,0,0);
    ret = file_filp->f_op->read(file_filp, bufPtr, bufLen, &file_filp->f_pos);

    if(ret < 0)
     { 
            return -1;
      }
       set_fs(old_fs);
	filp_close(file_filp, 0);*/
  
  return status;
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_SaveNVRAMFile
--|
--| DESCRIPTION   : Save MxLNVRAMFile
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

MXL_STATUS_E MxLWare_HRCLS_OEM_SaveNVRAMFile(UINT8 devId, UINT8 *bufPtr, UINT32 bufLen)
{
  MXL_STATUS_E status = MXL_FAILURE;
  // !!! FIXME !!! 
  // To be implemented for customer OEM platform
  
  return status;  
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_DelayUsec
--|
--| DESCRIPTION   : Delay in micro-seconds
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

void MxLWare_HRCLS_OEM_DelayUsec(UINT32 usec)
{
  // !!! FIXME !!! 
  // To be implemented for customer OEM platform
 
     udelay(usec);
  
}

/*------------------------------------------------------------------------------
--|
--| FUNCTION NAME : MxLWare_HRCLS_OEM_GetCurrTimeInUsec
--|
--| DESCRIPTION   : Get current time in micro-seconds
--|
--| RETURN VALUE  : MXL_SUCCESS or MXL_FAILURE
--|
--|---------------------------------------------------------------------------*/

void MxLWare_HRCLS_OEM_GetCurrTimeInUsec(UINT64* usecPtr)
{
#if 0
  struct timeval currTime;
#if __WORDSIZE == 32  
  static unsigned long long sec_count = 0;
  unsigned long long mask, incrementValue;
 
#endif  
  unsigned long long timeInUsec;

  MxL_HRCLS_DEBUG(" MxL_HRCLS_Ctrl_GetTimeTickInUsec ");
  gettimeofday(&currTime, NULL);

#if __WORDSIZE == 32  
  incrementValue = ((unsigned long long) 1 << (sizeof(currTime.tv_sec) * 8));
  mask = incrementValue-1;  

  if ((unsigned long long) currTime.tv_sec < (sec_count & mask))
  {
    sec_count += incrementValue;
  }
  sec_count &= ~(mask);
  sec_count |= (currTime.tv_sec & mask);
  timeInUsec = (unsigned long long)(sec_count * 1000 * 1000) + (unsigned long long) currTime.tv_usec;
#else
  timeInUsec = (unsigned long long)((unsigned long long) currTime.tv_sec * 1000 * 1000) + (unsigned long long)currTime.tv_usec;
//printf("sec:%x, usec:%lu, timeInUsec:%Lu\n", (unsigned int) currTime.tv_sec, (unsigned long) currTime.tv_usec, timeInUsec);  
#endif
  *usecPtr= (UINT64) timeInUsec;
#endif
 unsigned long long timeInUsec =0;
 *usecPtr= (UINT64) timeInUsec;
  // return MXL_SUCCESS;
}

#endif

