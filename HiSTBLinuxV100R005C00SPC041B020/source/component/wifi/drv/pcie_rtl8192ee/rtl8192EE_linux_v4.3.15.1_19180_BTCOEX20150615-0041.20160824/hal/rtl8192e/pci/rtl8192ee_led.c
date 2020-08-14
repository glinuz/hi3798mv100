/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#include "drv_types.h"
#include <rtl8192e_hal.h>

//================================================================================
//	Prototype of protected function.
//================================================================================

//================================================================================
// LED_819xUsb routines. 
//================================================================================

//
//	Description:
//		Turn on LED according to LedPin specified.
//
static void
SwLedOn_8192EE(
	_adapter			*padapter, 
	PLED_PCIE		pLed
)
{
	u16	LedReg = REG_LEDCFG0;
	u8	LedCfg = 0;
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);
	struct led_priv	*ledpriv = &(padapter->ledpriv);

	if( (padapter->bSurpriseRemoved == _TRUE) || ( padapter->bDriverStopped == _TRUE))
	{
		return;
	}

	switch(pLed->LedPin)
	{	
		case LED_PIN_LED0:	
			if(ledpriv->LedStrategy == SW_LED_MODE10)
				LedReg = REG_LEDCFG0;
			else
				LedReg = REG_LEDCFG1;
			break;

		case LED_PIN_LED1:
			LedReg = REG_LEDCFG2;
			break;

		case LED_PIN_GPIO0:
		default:
			break;
	}

	//DBG_871X("In SwLedOn, LedAddr:%X LEDPIN=%d \n", LedReg, pLed->LedPin);

	LedCfg = rtw_read8(padapter, LedReg);
	LedCfg |= BIT5; //Set 0x4c[21]
	LedCfg &= ~(BIT7|BIT6|BIT3|BIT2|BIT1|BIT0); //Clear 0x4c[23:22] and 0x4c[19:16]
	rtw_write8(padapter, LedReg, LedCfg); // SW control led0 on.
	pLed->bLedOn = _TRUE;
}


//
//	Description:
//		Turn off LED according to LedPin specified.
//
static void
SwLedOff_8192EE(
	_adapter			*padapter, 
	PLED_PCIE		pLed
)
{
	u16	LedReg = REG_LEDCFG0;
	HAL_DATA_TYPE *pHalData = GET_HAL_DATA(padapter);
	struct led_priv	*ledpriv = &(padapter->ledpriv);

	if(padapter->bSurpriseRemoved == _TRUE)
	{
             return;
	}

	switch(pLed->LedPin)
	{
		case LED_PIN_LED0:
			if(ledpriv->LedStrategy == SW_LED_MODE10)
				LedReg = REG_LEDCFG0;
			else
				LedReg = REG_LEDCFG1;
			break;

		case LED_PIN_LED1:
			LedReg = REG_LEDCFG2;
			break;

		case LED_PIN_GPIO0:
		default:
			break;
	}

	//DBG_871X("In SwLedOff,LedAddr:%X LEDPIN=%d\n", LedReg, pLed->LedPin);

	if(pHalData->bLedOpenDrain == _TRUE) // Open-drain arrangement for controlling the LED
	{
		u8	LedCfg = rtw_read8(padapter, LedReg);
	
		LedCfg &= 0xd0; // Set to software control.
		rtw_write8(padapter, LedReg, (LedCfg|BIT3));

		// Open-drain arrangement
		LedCfg = rtw_read8(padapter, REG_MAC_PINMUX_CFG);
		LedCfg &= 0xFE;// Set GPIO[8] to input mode
		rtw_write8(padapter, REG_MAC_PINMUX_CFG, LedCfg);

	}
	else
	{
		rtw_write8(padapter, LedReg, 0x28);
	}

	pLed->bLedOn = _FALSE;
}

//
//	Description:
//		Initialize all LED_871x objects.
//
void
rtl8192ee_InitSwLeds(
	_adapter	*padapter
	)
{
	struct led_priv *pledpriv = &(padapter->ledpriv);

	pledpriv->LedControlHandler = LedControlPCIE;

	pledpriv->SwLedOn = SwLedOn_8192EE;
	pledpriv->SwLedOff = SwLedOff_8192EE;


	InitLed(padapter, &(pledpriv->SwLed0), LED_PIN_LED0);

	InitLed(padapter, &(pledpriv->SwLed1), LED_PIN_LED1);
}


//
//	Description:
//		DeInitialize all LED_819xUsb objects.
//
void
rtl8192ee_DeInitSwLeds(
	_adapter	*padapter
	)
{
	struct led_priv	*ledpriv = &(padapter->ledpriv);

	DeInitLed( &(ledpriv->SwLed0) );
	DeInitLed( &(ledpriv->SwLed1) );
}

