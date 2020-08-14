/* **************************************************** */
/*!
   @file	MN_DMD_I2C.c
   @brief	I2C communication wrapper
   @author	dongfuhai 66707
   @date	2012/03/30
   @param
		(c)	Hisilicon
   */
/* **************************************************** */
#include <linux/delay.h>
#include <linux/time.h>

#include "MN_DMD_driver.h"
#include "MN_I2C.h"
#include "drv_i2c_ext.h"
#include "hi_drv_gpioi2c.h"
#include "drv_tuner_ext.h"
#include "hi_drv_sys.h"
#include "drv_demod.h"

/* **************************************************** */
/* **************************************************** */
#define DMD_I2C_MAXSIZE	127
/*! I2C Initialize Function*/
DMD_ERROR_t DMD_API DMD_I2C_open(void)
{
	//TODO:	Please call I2C initialize API here
	//this function is called by DMD_API_open

	return DMD_E_OK;
}

/*! Write 1byte */
DMD_ERROR_t DMD_I2C_Write(DMD_u32_t i2cnum, DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
    //TODO:	Please call I2C 1byte Write API	
    //unsigned char err = 0;
    
    //err = I2cWrite(slvadr<<1,buf,2);
    
    int ret = 0;
    //DMD_u8_t rdreg;
    //if (HI_UNF_I2C_CHANNEL_QAM < i2cnum)
    //    ret = gpio_i2c_write_ext(i2cnum, slvadr<<1, adr, 1, &data, 1);
    //else
    if (/*HI_UNF_I2C_CHANNEL_QAM*/HI_STD_I2C_NUM > i2cnum)
    {
        ret = HI_DRV_I2C_Write(i2cnum, slvadr<<1, adr, 1, &data, 1);
    }
    else
    {
        ret = HI_DRV_GPIOI2C_WriteExt(i2cnum, slvadr<<1, adr, 1, &data, 1);
    }
    
    #if 0
    DMD_I2C_Read(i2cnum, slvadr, adr, &rdreg);
    HI_ERR_TUNER("read reg[0x%x] =0x%x,write value is 0x%x\n",adr, rdreg, data);
    #endif
    return ((ret == 0) ? DMD_E_OK:DMD_E_ERROR);
}

/*! Read 1byte */
DMD_ERROR_t DMD_I2C_Read(DMD_u32_t i2cnum, DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *pdata )
{
    //TODO:	call I2C 1byte Write API
    
    int ret = 0;
    if (/*HI_UNF_I2C_CHANNEL_QAM*/HI_STD_I2C_NUM > i2cnum)
    {
        ret = HI_DRV_I2C_Read(i2cnum, slvadr<<1, adr, 1, pdata, 1);   
    }
    else
    {
        ret = HI_DRV_GPIOI2C_ReadExt(i2cnum, slvadr<<1, adr, 1, pdata, 1);
    }
        
    return ((ret == 0) ? DMD_E_OK:DMD_E_ERROR);
}

/*! Write/Read any Length*/
DMD_ERROR_t DMD_I2C_WriteRead(DMD_u32_t i2cnum, DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* pwdata , DMD_u32_t wlen , DMD_u8_t* prdata , DMD_u32_t rlen)
{
    //TODO:	Please call I2C Read/Write API here
    unsigned char err = 0;
    //unsigned char bBuf[255];

    if(wlen)
    {
        if (/*HI_UNF_I2C_CHANNEL_QAM*/HI_STD_I2C_NUM > i2cnum)
        {
            err = HI_DRV_I2C_Write(i2cnum, slvadr<<1, adr, 1, pwdata, wlen);
        }
        else
        {
            err = HI_DRV_GPIOI2C_WriteExt(i2cnum, slvadr<<1, adr, 1, pwdata, wlen);
        }

        if (err)
        {      
            return DMD_E_ERROR;
        }
    }

    if(rlen)
    {
        if (/*HI_UNF_I2C_CHANNEL_QAM*/HI_STD_I2C_NUM > i2cnum)
        {
            err = HI_DRV_I2C_Read(i2cnum, slvadr<<1, adr, 1, prdata, rlen);
        }
        else
        {
            err = HI_DRV_GPIOI2C_ReadExt(i2cnum, slvadr<<1, adr, 1, prdata, rlen);
        }

        if(err)
        {
            return DMD_E_ERROR;
        }				
    }

    return DMD_E_OK;
}

/*! Timer wait */
DMD_ERROR_t DMD_wait( DMD_u32_t msecond ){

	//TODO: call timer wait function
	//msleep_interruptible(msecond);
	tuner_mdelay(msecond );
	return DMD_E_OK;
}

/*! Get System Time (ms) */
DMD_ERROR_t DMD_timer( DMD_u32_t* tim ){

    (HI_VOID)HI_DRV_SYS_GetTimeStampMs(tim);
    return DMD_E_OK;
}

