

/* **************************************************** */
/*!
   @file	MN_DMD_I2C_WIN.c
   @brief	I2C communication wrapper
   @author	STB
   @date	2015/4/15
   @param
		(c)	Hisilicon
   */
/* **************************************************** */
#include <linux/delay.h>
#include <linux/time.h>

#include "MN_DMD_driver.h"
#include "MN_I2C.h"
#include "MN_DMD_common.h"

#include "drv_i2c_ext.h"
#include "hi_drv_gpioi2c.h"
#include "drv_tuner_ext.h"
#include "hi_drv_sys.h"
#include "drv_demod.h"

/* **************************************************** */
/* **************************************************** */

#ifndef PC_CONTROL_FE

extern DMD_u32_t g_I2cNum;

#define DMD_I2C_MAXSIZE	127
/*! I2C Initialize Function*/
DMD_ERROR_t DMD_API MN8847X_DMD_I2C_open(void)
{
	//TODO:	Please call I2C initialize API here
	//this function is called by DMD_API_open

	return DMD_E_OK;
}


/* **************************************************** */
/* Suggest to use I2C function as below */
/* **************************************************** */
/*
----------------------  customer_i2c_write(slvadr, apData, length) --------------------------

  // +------+-+-----+-+-+----+-+----------+-+-+-----+--+-+-----+--+-+
  // |MASTER|S|SADDR|W| |RegAddr| |data1 | |data2 | | ... | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+-----+--+-+-----+--+-+
  // |SLAVE |         |A|       |A|      |A|      |A|     |A|
  // +------+-+-----+-+-+----+-+----------+-+-+-----+--+-+-----+--+-+


-----------------------  customer_i2c_read(slvadr, data, length) ----------------------------

  // +------+-+-----+-+-+-----+--+-+-----+--+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |A|     |A|     |MN|P|
  // +------+-+-----+-+-+-----+--+-+-----+--+-+-----+--+-+
  // |SLAVE |         |A|Data1| |Data2| | ... |
  // +------+---------+-+-----+--+-+-----+--+-+-----+--+-+
*/

/*! Write 1byte */
DMD_ERROR_t MN8847X_DMD_I2C_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
	int ret = 0;
    DMD_u32_t i2cnum = g_I2cNum;
    DMD_u8_t wdata = data;
	//TODO:	Please call I2C 1byte Write API
    
    //printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,slvadr<<1,adr);
    if (HI_STD_I2C_NUM > i2cnum)
    {
        ret = HI_DRV_I2C_Write(i2cnum, (slvadr<<1)&0xfe, adr, 1, &wdata, 1);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_WriteExt(i2cnum, (slvadr<<1)&0xfe, adr, 1, &wdata, 1);
    }

    if(ret != 0)
    {
        printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)&0xfe,adr);
        return DMD_E_ERROR;
    }
    else
    {
        //printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x,data:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)&0xfe,adr,wdata);
        return DMD_E_OK;
    }
}

/*! Read 1byte */
DMD_ERROR_t MN8847X_DMD_I2C_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *data )
{
	int ret = 0;
	DMD_u32_t i2cnum = g_I2cNum;
    DMD_u8_t rdata = 0;
	//TODO:	call I2C 1byte Write API
	//printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,slvadr<<1,adr);
    if (HI_STD_I2C_NUM > i2cnum)
    {
        ret = HI_DRV_I2C_Read(i2cnum, (slvadr<<1)|0x01, adr, 1, &rdata, 1);   
    }
    else
    {
        ret = HI_DRV_GPIOI2C_ReadExt(i2cnum, (slvadr<<1)|0x01, adr, 1, &rdata, 1);
    }

    *data = rdata;
    
    if(ret != 0)
    {
        printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)|0x01,adr);
        return DMD_E_ERROR;
    }
    else
    {
        //printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x,data:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)|0x01,adr,rdata);
        return DMD_E_OK;
    }
}

/* 
Write/Read any Length to DMD and Tuner;
DMD_u8_t slvadr : DMD's slave address.   
DMD_u8_t adr    : DMD's REG address; 

For writing CMD to tuner, 
DMD_u8_t slvadr is DMD's slave address;
DMD_u8_t adr is DMD's register TCBCOM(0xF7)
!!!Tuner's slave addr. and REG addr. are as continous data< upper layer : data[x]> 
*/
DMD_ERROR_t MN8847X_DMD_I2C_WriteRead(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	//TODO:	Please call I2C Read/Write API here
/*
【Warning】
CHN : 请务必了解后端所支持的 I2C 一次最大读写字节，如果无法一次性写入 2K 字节，请分批写入。最坏的情况就是一个字节一个字节写
ENG ： Pls. well understand the most bytes SoC supports to wirte/read I2C at one time. If cannot write 2000 bytes in one time, pls. do writting several times. Eventually, you can try writting one byte by one byte.
*/
    unsigned char err = 0;
    DMD_u32_t i2cnum = g_I2cNum;
    DMD_u8_t wwdata = 0,rrdata = 0;
    
    //printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,slvadr<<1,adr);
    wwdata = *wdata;
    
    if(wlen)
    {
        if (HI_STD_I2C_NUM > i2cnum)
        {
            err = HI_DRV_I2C_Write(i2cnum, (slvadr<<1)&0xfe, adr, 1, &wwdata, wlen);
        }  
        else
        {
            err = HI_DRV_GPIOI2C_WriteExt(i2cnum, (slvadr<<1)&0xfe, adr, 1, &wwdata, wlen);
        }

        if (err)
        {   
            printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)&0xfe,adr);
            return DMD_E_ERROR;
        }
    }

    if(rlen)
    {
        if (/*HI_UNF_I2C_CHANNEL_QAM*/HI_STD_I2C_NUM > i2cnum)
        {
            err = HI_DRV_I2C_Read(i2cnum, (slvadr<<1)|0x01, adr, 1, &rrdata, rlen);
        }
        else
        {
            err = HI_DRV_GPIOI2C_ReadExt(i2cnum, (slvadr<<1)|0x01, adr, 1, &rrdata, rlen);
        }

        *rdata = rrdata;
        if(err)
        {
            printk("-->[%s,%d] i2cnum:%d, dev:0x%x,reg:0x%x\n",__func__,__LINE__,i2cnum,(slvadr<<1)|0x01,adr);
            return DMD_E_ERROR;
        }               
    }

    
	return DMD_E_OK;
}

/*! Timer wait */
DMD_ERROR_t MN8847X_DMD_wait( DMD_u32_t msecond ){

	//TODO: call timer wait function 
    msleep_interruptible(msecond);

	return DMD_E_OK;
}

/*! Get System Time (ms) */
DMD_ERROR_t MN8847X_DMD_timer( DMD_u32_t* tim ){

    //TODO: call timer function 
    (HI_VOID)HI_DRV_SYS_GetTimeStampMs(tim);
	return DMD_E_OK;
}

#endif //#ifndef PC_CONTROL_FE
