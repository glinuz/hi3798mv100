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

//============================================================
// include files
//============================================================

#include "mp_precomp.h"

#include "../phydm_precomp.h"

#if (RTL8821A_SUPPORT == 1)

VOID
phydm_set_ext_band_switch_8821A(
	IN		PVOID		pDM_VOID,
	IN		u4Byte		band
	)
{
	PDM_ODM_T		pDM_Odm = (PDM_ODM_T)pDM_VOID;
	
	/*Output Pin Settings*/
	ODM_SetMACReg(pDM_Odm, 0x4C, BIT23, 0); /*select DPDT_P and DPDT_N as output pin*/
	ODM_SetMACReg(pDM_Odm, 0x4C, BIT24, 1); /*by WLAN control*/
	
	ODM_SetBBReg(pDM_Odm, 0xCB4, 0xF, 7); /*DPDT_P = 1b'0*/
	ODM_SetBBReg(pDM_Odm, 0xCB4, 0xF0, 7); /*DPDT_N = 1b'0*/

	if (band == ODM_BAND_2_4G) {
		ODM_SetBBReg(pDM_Odm, 0xCB4, (BIT29|BIT28), 1);
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("***8821A set band switch = 2b'01\n"));
	} else {
		ODM_SetBBReg(pDM_Odm, 0xCB4, BIT29|BIT28, 2);
		ODM_RT_TRACE(pDM_Odm, ODM_COMP_ANT_DIV, ODM_DBG_LOUD, ("***8821A set band switch = 2b'10\n"));
	}
}

VOID
odm_DynamicTryStateAgg_8821A(
	IN		PDM_ODM_T		pDM_Odm
	)
{
	if((pDM_Odm->SupportICType == ODM_RTL8821) && (pDM_Odm->SupportInterface == ODM_ITRF_USB))
	{
		if(pDM_Odm->RSSI_Min > 25)
			ODM_Write1Byte(pDM_Odm, 0x4CF, 0x02);
		else if(pDM_Odm->RSSI_Min < 20)
			ODM_Write1Byte(pDM_Odm, 0x4CF, 0x00);
	}
}

VOID
odm_HWSetting_8821A(
	IN		PDM_ODM_T		pDM_Odm
	)
{
	odm_DynamicTryStateAgg_8821A(pDM_Odm);
}

#endif //#if (RTL8821A_SUPPORT == 1)

