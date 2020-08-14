/*
 *           Copyright 2010 Availink, Inc.
 *
 *  This software contains Availink proprietary information and
 *  its use and disclosure are restricted solely to the terms in
 *  the corresponding written license agreement. It shall not be 
 *  disclosed to anyone other than valid licensees without
 *  written permission of Availink, Inc.
 *
 */


///
/// @file
/// @brief Implements the functions declared in IBSP.h. 
/// 
//$Revision: 563 $ 
//$Date: 2008-09-24 17:24:29 -0400 (Wed, 24 Sep 2008) $
// 
#include "IBSP.h"
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/init.h>
#include "hi_kernel_adapt.h"

#include "drv_tuner_ext.h"
#include "hi_drv_i2c.h"

/// The following table illustrates a set of PLL configuration values to operate AVL6211 in two modes:
// Standard performance mode.
// High performance mode

/// Please refer to the AVL6211 channel receiver datasheet for detailed information on highest symbol rate 
/// supported by the demod in both these modes.

///For more information on other supported clock frequencies and PLL settings for higher symbol rates, please 
///contact Availink.

/// Users can remove unused elements from the following array to reduce the SDK footprint size.
const struct AVL_DVBSx_PllConf pll_conf[] =
{
	// The following set of PLL configuration at different reference clock frequencies refer to demod operation
	// in standard performance mode.
	 {503,  1, 7, 4, 2,  4000, 11200, 16800, 25200} ///< Reference clock 4 MHz,   Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{447,  1, 7, 4, 2,  4500, 11200, 16800, 25200} ///< Reference clock 4.5 MHz, Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  4, 7, 4, 2, 10000, 11200, 16800, 25200} ///< Reference clock 10 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{503,  7, 7, 4, 2, 16000, 11200, 16800, 25200} ///< Reference clock 16 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	,{111,  2, 7, 4, 2, 27000, 11200, 16800, 25200} ///< Reference clock 27 MHz,  Demod clock 112 MHz, FEC clock 168 MHz, MPEG clock 252 MHz
	
	// The following set of PLL configuration at different reference clock frequencies refer to demod operation
	// in high performance mode. 
	,{566,  1, 7, 4, 2,  4000, 12600, 18900, 28350} /// < Reference clock 4 MHz,   Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{503,  1, 7, 4, 2,  4500, 12600, 18900, 28350} ///< Reference clock 4.5 MHz, Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  4, 7, 4, 2, 10000, 12600, 18900, 28350} ///< Reference clock 10 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{566,  7, 7, 4, 2, 16000, 12600, 18900, 28350} ///< Reference clock 16 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz
	,{377,  8, 7, 4, 2, 27000, 12600, 18900, 28350} ///< Reference clock 27 MHz,  Demod clock 126 MHz, FEC clock 189 MHz, MPEG clock 283.5 MHz

};

const AVL_uint16 pll_array_size = sizeof(pll_conf)/sizeof(struct AVL_DVBSx_PllConf);


AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Initialize(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Dispose(void)
{
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_Delay( AVL_uint32 uiMS )
{
	mdelay(2*uiMS); //dongfuhai
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CRead(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff, AVL_puint16 puiSize )
{
	HI_S32  Ret = 0;

	/* Support GPIO I2C */
    if (/*HI_UNF_I2C_CHANNEL_QAM*/4 >= pAVLChip->m_I2CChn)
    {
	    Ret = HI_DRV_I2C_ReadDirectly(pAVLChip->m_I2CChn, (pAVLChip -> m_SlaveAddr) << 1 | 1, 0, 0, pucBuff, *puiSize);
    }
    /*else
    {
	    Ret = gpio_i2c_read_ext_directly(pAVLChip->m_I2CChn, (pAVLChip -> m_SlaveAddr) << 1 | 1, 0, 0, pucBuff, *puiSize);
    }*/

	return Ret;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_I2CWrite(  const struct AVL_DVBSx_Chip * pAVLChip,  AVL_puchar pucBuff,  AVL_puint16  puiSize )
{
	HI_S32  Ret = 0;

	/* Support GPIO I2C */
    if (/*HI_UNF_I2C_CHANNEL_QAM*/4 >= pAVLChip->m_I2CChn)
    {
    	Ret = HI_DRV_I2C_Write(pAVLChip->m_I2CChn, (pAVLChip -> m_SlaveAddr)<<1 | 0, 0, 0, pucBuff, *puiSize);
    }
    /*else
    {
    	Ret = gpio_i2c_write_ext(pAVLChip->m_I2CChn, (pAVLChip -> m_SlaveAddr)<<1 | 0, 0, 0, pucBuff, *puiSize);
    }*/
    
	return Ret;
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_InitSemaphore( AVL_psemaphore pSemaphore )
{
	HI_INIT_MUTEX(pSemaphore);
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_WaitSemaphore( AVL_psemaphore pSemaphore )
{
	if (0 != down_interruptible(pSemaphore))
	{
        return (AVL_DVBSx_EC_GeneralFail);
    }
	return(AVL_DVBSx_EC_OK);
}

AVL_DVBSx_ErrorCode AVL_DVBSx_IBSP_ReleaseSemaphore( AVL_psemaphore pSemaphore )
{
	up(pSemaphore);
	return(AVL_DVBSx_EC_OK);
}
