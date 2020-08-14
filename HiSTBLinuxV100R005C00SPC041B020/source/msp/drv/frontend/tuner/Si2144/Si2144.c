/***************************************************************************************
                  Silicon Laboratories Broadcast Si2144 application

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   FILE: Si2144.c
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_1_build_1
   Revision: 0.1
   Date: January 20 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/

#include <linux/delay.h>
#include <linux/semaphore.h>

#include "linux/kernel.h"

#include <hi_debug.h>
#include "hi_type.h"
#include "drv_i2c_ext.h"

#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "drv_tuner_ioctl.h"

#include "Si2144_typedefs.h"
#include "Silabs_L0_API.h"
#include "Si2144_L1_API.h"
#include "Si2144_L2_API.h"
#include "Si2144_Commands.h"

#define			Si2144_BASE_ADDRESS			0xc0

static L1_Si2144_Context          FrontEnd_Table;
L1_Si2144_Context         *Si2144;

/* Also init a simple i2c context to allow i2c communication checking */
//L0_Context* i2c;
//L0_Context  i2c_context;
//CONNECTION_TYPE mode;

HI_S32 si2144_init_tuner(HI_U32 u32TunerPort,HI_U32 u32I2cChannel)
{
	Si2144 = &(FrontEnd_Table);

    Si2144_L1_API_Init(u32TunerPort,Si2144, Si2144_BASE_ADDRESS,u32I2cChannel);

	Si2144->cmd->power_up.clock_mode =  Si2144_POWER_UP_CMD_CLOCK_MODE_XTAL;
	Si2144->cmd->power_up.en_xout    =  Si2144_POWER_UP_CMD_EN_XOUT_EN_XOUT;
	Si2144->cmd->config_clocks.clock_mode = Si2144_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL;

	Si2144->i2c->trackRead = Si2144->i2c->trackWrite = 1;
	if (Si2144_Init(Si2144) == 0 )
	{
		Si2144_Configure(Si2144);
	}
	else
	{
		HI_ERR_TUNER("si2144 init err.\n");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}


HI_S32 si2144_set_tuner (HI_U32 u32TunerPort, HI_U32 u32Frequency,HI_U32 u32BandWidth)
{
	HI_U32 u32Ret = NO_Si2144_ERROR;
	HI_U8 u8BW = 0;

	switch(u32BandWidth)
	{
		case 1700:
			u8BW = Si2144_DTV_MODE_PROP_BW_BW_1P7MHZ;
			break;
		case 6000:
			u8BW = Si2144_DTV_MODE_PROP_BW_BW_6MHZ;
			break;
		case 7000:
			u8BW = Si2144_DTV_MODE_PROP_BW_BW_7MHZ;
			break;
		case 8000:
		default:
			u8BW = Si2144_DTV_MODE_PROP_BW_BW_8MHZ;
			break;
	}
	HI_INFO_TUNER("u32Frequency:%d,u32BandWidth:%d\n",u32Frequency,u32BandWidth);
	
	u32Ret = Si2144_DTVTune (Si2144, u32Frequency, u8BW, Si2144_DTV_MODE_PROP_MODULATION_DVBT, Si2144_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL);
    if (u32Ret != NO_Si2144_ERROR)
    {
		HI_ERR_TUNER("si2144 set tuner err.\n");
		return HI_FAILURE;
	}

	return HI_SUCCESS;
}




