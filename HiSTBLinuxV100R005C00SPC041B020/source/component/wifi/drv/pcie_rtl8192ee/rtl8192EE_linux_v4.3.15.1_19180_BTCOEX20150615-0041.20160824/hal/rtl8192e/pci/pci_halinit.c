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
#define _HCI_HAL_INIT_C_

//#include <drv_types.h>
#include <rtl8192e_hal.h>


// For Two MAC FPGA verify we must disable all MAC/BB/RF setting
#define FPGA_UNKNOWN		0
#define FPGA_2MAC			1
#define FPGA_PHY			2
#define ASIC					3
#define BOARD_TYPE			ASIC

#if BOARD_TYPE == FPGA_2MAC
#else // FPGA_PHY and ASIC
#define FPGA_RF_UNKOWN	0
#define FPGA_RF_8225		1
#define FPGA_RF_0222D		2
#define FPGA_RF				FPGA_RF_0222D
#endif


u8 NORMAL_PAGE_NUM_8192EE[] = {0xe9, 0x08, 0x08, 0x08};
u8 WMM_PAGE_NUM_8192EE[]    = {0x30, 0x20, 0x20, 0x20};

//-------------------------------------------------------------------
//
//	EEPROM Content Parsing
//
//-------------------------------------------------------------------

static VOID
Hal_ReadPROMContent_BT_8821A(
	IN PADAPTER 	Adapter
	)
{

#if MP_DRIVER == 1
	if (Adapter->registrypriv.mp_mode == 1)
		EFUSE_ShadowMapUpdate(Adapter, EFUSE_BT, _FALSE);
#endif		
	
}

static VOID
hal_ReadIDs_8192EE(
	IN	PADAPTER	Adapter,
	IN	u8			*PROMContent,
	IN	BOOLEAN		AutoloadFail
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);

	if( !AutoloadFail )
	{
		// VID, DID 
		pHalData->EEPROMVID = EF2Byte( *(u16 *)&PROMContent[EEPROM_VID_8192EE] );
		pHalData->EEPROMDID = EF2Byte( *(u16 *)&PROMContent[EEPROM_DID_8192EE] );
		pHalData->EEPROMSVID = EF2Byte( *(u16 *)&PROMContent[EEPROM_SVID_8192EE] );
		pHalData->EEPROMSMID = EF2Byte( *(u16 *)&PROMContent[EEPROM_SMID_8192EE] );

		// Customer ID, 0x00 and 0xff are reserved for Realtek. 		
		pHalData->EEPROMCustomerID = *(u8 *)&PROMContent[EEPROM_CustomID_8192E];
		if(pHalData->EEPROMCustomerID == 0xFF)
			pHalData->EEPROMCustomerID = EEPROM_Default_CustomerID_8188E;
		pHalData->EEPROMSubCustomerID = EEPROM_Default_SubCustomerID;
	}
	else
	{
		pHalData->EEPROMVID 		= 0;
		pHalData->EEPROMDID 		= 0;
		pHalData->EEPROMSVID 		= 0;
		pHalData->EEPROMSMID 		= 0;

		// Customer ID, 0x00 and 0xff are reserved for Realtek.
		pHalData->EEPROMCustomerID	= EEPROM_Default_CustomerID;
		pHalData->EEPROMSubCustomerID	= EEPROM_Default_SubCustomerID;
	}

	DBG_871X("VID = 0x%04X, DID = 0x%04X\n", pHalData->EEPROMVID, pHalData->EEPROMDID);
	DBG_871X("SVID = 0x%04X, SMID = 0x%04X\n", pHalData->EEPROMSVID, pHalData->EEPROMSMID);
}

static VOID
hal_ReadMACAddress_8192EE(
	IN	PADAPTER		pAdapter,
	IN	u8				*PROMContent,
	IN	BOOLEAN			AutoLoadFail
	)
{
	u8	sMacAddr[6] = {0x00, 0xE0, 0x4C, 0x81, 0x92, 0x00};
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(pAdapter);

	if(AutoLoadFail)
	{
		//sMacAddr[5] = (u8)GetRandomNumber(1, 254);
		_rtw_memcpy(pEEPROM->mac_addr, sMacAddr, ETH_ALEN);
	}
	else
	{
		_rtw_memcpy(pEEPROM->mac_addr, &PROMContent[EEPROM_MAC_ADDR_8192EE], ETH_ALEN);
	}

	DBG_871X("%s MAC Address from EFUSE = "MAC_FMT"\n",__FUNCTION__, MAC_ARG(pEEPROM->mac_addr));
}

static VOID
Hal_ReadEfusePCIeCap8192EE(
	IN	PADAPTER	Adapter,	
	IN	u8			*PROMContent,
	IN	BOOLEAN 	AutoloadFail
	)
{
	u8	AspmOscSupport = RT_PCI_ASPM_OSC_IGNORE;
	u16	PCIeCap = 0;	

	if(!AutoloadFail)
	{
		PCIeCap = PROMContent[EEPROM_PCIE_DEV_CAP_01] |
			(PROMContent[EEPROM_PCIE_DEV_CAP_02]<<8);

		DBG_871X("Hal_ReadEfusePCIeCap8192EE(): PCIeCap = %#x\n", PCIeCap);

		//
		// <Roger_Notes> We shall take L0S/L1 accept latency into consideration for ASPM Configuration policy, 2013.03.27.
		// L1 accept Latency: 0x8d from PCI CFG space offset 0x75
		// L0S accept Latency: 0x80 from PCI CFG space offset 0x74
		//
		if(PCIeCap == 0x8d80)
			AspmOscSupport |= RT_PCI_ASPM_OSC_ENABLE;
		else	
			AspmOscSupport |= RT_PCI_ASPM_OSC_DISABLE;
	}

	//rtw_hal_set_def_var(Adapter, HAL_DEF_PCI_ASPM_OSC, (u8 *)&AspmOscSupport);
	DBG_871X("Hal_ReadEfusePCIeCap92EE(): AspmOscSupport = %d\n", AspmOscSupport);
}

static VOID
hal_CustomizedBehavior_8192EE(
	PADAPTER			Adapter
)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	EEPROM_EFUSE_PRIV	*pEEPROM = GET_EEPROM_EFUSE_PRIV(Adapter);
	struct led_priv	*pledpriv = &(Adapter->ledpriv);
	
	pledpriv->LedStrategy = SW_LED_MODE7; //Default LED strategy.
	pHalData->bLedOpenDrain = _TRUE;// Support Open-drain arrangement for controlling the LED. Added by Roger, 2009.10.16.
	
	switch(pEEPROM->CustomerID)
	{
		case RT_CID_DEFAULT:
			break;

		default:
			//DBG_871X("Unkown hardware Type \n");
			break;
	}
	DBG_871X("hal_CustomizedBehavior_8192EE(): RT Customized ID: 0x%02X\n", pEEPROM->CustomerID);

#if 0
	if(Adapter->bInHctTest)
	{
		pMgntInfo->PowerSaveControl.bInactivePs = FALSE;
		pMgntInfo->PowerSaveControl.bIPSModeBackup = FALSE;
		pMgntInfo->PowerSaveControl.bLeisurePs = FALSE;
		pMgntInfo->PowerSaveControl.bLeisurePsModeBackup = FALSE;
		pMgntInfo->keepAliveLevel = 0;
	}
#endif
}

static VOID
hal_CustomizeByCustomerID_8192EE(
	IN	PADAPTER		pAdapter
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(pAdapter);
	EEPROM_EFUSE_PRIV	*pEEPROM = GET_EEPROM_EFUSE_PRIV(pAdapter);

	// If the customer ID had been changed by registry, do not cover up by EEPROM.
	if(pEEPROM->CustomerID == RT_CID_DEFAULT)
	{
		switch(pHalData->EEPROMCustomerID)
		{
			default:
				pEEPROM->CustomerID = RT_CID_DEFAULT;
				break;
					
		}
	}
	DBG_871X("MGNT Customer ID: 0x%2x\n", pEEPROM->CustomerID);
	
	hal_CustomizedBehavior_8192EE(pAdapter);
}


static VOID
InitAdapterVariablesByPROM_8192EE(
	IN	PADAPTER	Adapter
	)
{
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(Adapter);

	//DBG_871X("InitAdapterVariablesByPROM_8821E()!!\n"); 

#ifdef CONFIG_EFUSE_CONFIG_FILE
	Rtw_Hal_readPGDataFromConfigFile(Adapter);
#else //CONFIG_EFUSE_CONFIG_FILE
	hal_InitPGData_8192E(Adapter, pEEPROM->efuse_eeprom_data);
#endif
	Hal_EfuseParseIDCode8192E(Adapter, pEEPROM->efuse_eeprom_data);	
	
	Hal_ReadPROMVersion8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	hal_ReadIDs_8192EE(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
#ifdef CONFIG_EFUSE_CONFIG_FILE
	Rtw_Hal_ReadMACAddrFromFile(Adapter);
#else //CONFIG_EFUSE_CONFIG_FILE
	hal_ReadMACAddress_8192EE(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
#endif
	Hal_ReadPowerSavingMode8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadTxPowerInfo8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadBoardType8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadEfusePCIeCap8192EE(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);

	//
	// Read Bluetooth co-exist and initialize
	//
	Hal_EfuseParseBTCoexistInfo8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	
	Hal_ReadChannelPlan8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_EfuseParseXtal_8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadThermalMeter_8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadPAType_8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadAmplifierType_8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadAntennaDiversity8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	Hal_ReadAntennaDiversity8192E(Adapter, pEEPROM->efuse_eeprom_data, pEEPROM->bautoload_fail_flag);
	hal_CustomizeByCustomerID_8192EE(Adapter);
}


static void Hal_ReadPROMContent_8192E(
	IN PADAPTER 		Adapter
	)
{	
	EEPROM_EFUSE_PRIV *pEEPROM = GET_EEPROM_EFUSE_PRIV(Adapter);
	u8			eeValue;

	/* check system boot selection */
	eeValue = rtw_read8(Adapter, REG_SYS_EEPROM_CTRL);
	pEEPROM->EepromOrEfuse		= (eeValue & EEPROMSEL) ? _TRUE : _FALSE;
	pEEPROM->bautoload_fail_flag	= (eeValue & EEPROM_EN) ? _FALSE : _TRUE;

	DBG_8192C("Boot from %s, Autoload %s !\n", (pEEPROM->EepromOrEfuse ? "EEPROM" : "EFUSE"),
				(pEEPROM->bautoload_fail_flag ? "Fail" : "OK") );

	//pHalData->EEType = IS_BOOT_FROM_EEPROM(Adapter) ? EEPROM_93C46 : EEPROM_BOOT_EFUSE;

	InitAdapterVariablesByPROM_8192EE(Adapter);
}

static void ReadAdapterInfo8192EE(PADAPTER Adapter)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);	

	DBG_871X("====> ReadAdapterInfo8192EE\n");
	
	// Read all content in Efuse/EEPROM.	
	Hal_ReadPROMContent_8192E(Adapter);

	// We need to define the RF type after all PROM value is recognized.
	hal_ReadRFType_8192E(Adapter);

	DBG_871X("<==== ReadAdapterInfo8192EE\n");
}


void rtl8192ee_interface_configure(PADAPTER Adapter)
{
	HAL_DATA_TYPE	*pHalData=GET_HAL_DATA(Adapter);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct pwrctrl_priv	*pwrpriv = dvobj_to_pwrctl(pdvobjpriv);

_func_enter_;

	////close ASPM for AMD defaultly
	pdvobjpriv->const_amdpci_aspm = 0;

	//// ASPM PS mode.
	//// 0 - Disable ASPM, 1 - Enable ASPM without Clock Req, 
	//// 2 - Enable ASPM with Clock Req, 3- Alwyas Enable ASPM with Clock Req,
	//// 4-  Always Enable ASPM without Clock Req.
	//// set defult to rtl8188ee:3 RTL8192E:2
	pdvobjpriv->const_pci_aspm = 0;

	//// Setting for PCI-E device */
	pdvobjpriv->const_devicepci_aspm_setting = 0x03;

	//// Setting for PCI-E bridge */
	pdvobjpriv->const_hostpci_aspm_setting = 0x03;

	//// In Hw/Sw Radio Off situation.
	//// 0 - Default, 1 - From ASPM setting without low Mac Pwr, 
	//// 2 - From ASPM setting with low Mac Pwr, 3 - Bus D3
	//// set default to RTL8192CE:0 RTL8192SE:2
	pdvobjpriv->const_hwsw_rfoff_d3 = 0;

	//// This setting works for those device with backdoor ASPM setting such as EPHY setting.
	//// 0: Not support ASPM, 1: Support ASPM, 2: According to chipset.
	pdvobjpriv->const_support_pciaspm = 1;

	pwrpriv->reg_rfoff = 0;
	pwrpriv->rfoff_reason = 0;

	pHalData->bL1OffSupport = _FALSE;
_func_exit_;
}

VOID
DisableInterrupt8192EE (
	IN PADAPTER			Adapter
	)
{
	struct dvobj_priv	*pdvobjpriv= adapter_to_dvobj(Adapter);

	rtw_write32(Adapter, REG_HIMR0_8192E, IMR_DISABLED_8192E);	

	rtw_write32(Adapter, REG_HIMR1_8192E, IMR_DISABLED_8192E);	// by tynli

	pdvobjpriv->irq_enabled = 0;
}

VOID
ClearInterrupt8192EE(
	IN PADAPTER			Adapter
	)
{
	HAL_DATA_TYPE	*pHalData=GET_HAL_DATA(Adapter);
	u32	tmp = 0;

	tmp = rtw_read32(Adapter, REG_HISR0_8192E);
	rtw_write32(Adapter, REG_HISR0_8192E, tmp);
	pHalData->IntArray[0] = 0;

	tmp = rtw_read32(Adapter, REG_HISR1_8192E);
	rtw_write32(Adapter, REG_HISR1_8192E, tmp);
	pHalData->IntArray[1] = 0;

}


VOID
EnableInterrupt8192EE(
	IN PADAPTER			Adapter
	)
{
	HAL_DATA_TYPE	*pHalData=GET_HAL_DATA(Adapter);
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);

#ifdef CONFIG_CONCURRENT_MODE
	if ((Adapter->isprimary == _FALSE) && Adapter->pbuddy_adapter){
		Adapter = Adapter->pbuddy_adapter;
		pHalData=GET_HAL_DATA(Adapter);
		pdvobjpriv = adapter_to_dvobj(Adapter);
	}
#endif
	pdvobjpriv->irq_enabled = 1;

	rtw_write32(Adapter, REG_HIMR0_8192E, pHalData->IntrMask[0]&0xFFFFFFFF);

	rtw_write32(Adapter, REG_HIMR1_8192E, pHalData->IntrMask[1]&0xFFFFFFFF);

}

BOOLEAN
InterruptRecognized8192EE(
	IN	PADAPTER			Adapter
	)
{
	HAL_DATA_TYPE	*pHalData=GET_HAL_DATA(Adapter);
	BOOLEAN			bRecognized = _FALSE;

#ifdef CONFIG_CONCURRENT_MODE
        if ((Adapter->isprimary == _FALSE) && Adapter->pbuddy_adapter){
		Adapter = Adapter->pbuddy_adapter;
		pHalData=GET_HAL_DATA(Adapter);
	}
#endif
	// 2013.11.18 Glayrainx suggests that turn off IMR and
	// restore after cleaning ISR.

	rtw_write32(Adapter, REG_HIMR0_8192E, 0 );
	rtw_write32(Adapter, REG_HIMR1_8192E, 0 );

	pHalData->IntArray[0] = rtw_read32(Adapter, REG_HISR0_8192E);	
	pHalData->IntArray[0] &= pHalData->IntrMask[0];
	rtw_write32(Adapter, REG_HISR0_8192E, pHalData->IntArray[0]);

	//For HISR extension. Added by tynli. 2009.10.07.
	pHalData->IntArray[1] = rtw_read32(Adapter, REG_HISR1_8192E);	
	pHalData->IntArray[1] &= pHalData->IntrMask[1];
	rtw_write32(Adapter, REG_HISR1_8192E, pHalData->IntArray[1]);

	if (((pHalData->IntArray[0])&pHalData->IntrMask[0])!=0 || 
		((pHalData->IntArray[1])&pHalData->IntrMask[1])!=0)
		bRecognized = _TRUE;

	// restore IMR
	rtw_write32(Adapter, REG_HIMR0_8192E, pHalData->IntrMask[0]&0xFFFFFFFF);
	rtw_write32(Adapter, REG_HIMR1_8192E, pHalData->IntrMask[1]&0xFFFFFFFF);

	return bRecognized;
}

VOID
UpdateInterruptMask8192EE(
	IN	PADAPTER		Adapter,
	IN	u32		AddMSR, 	u32		AddMSR1,
	IN	u32		RemoveMSR, u32		RemoveMSR1
	)
{
	HAL_DATA_TYPE	*pHalData=GET_HAL_DATA(Adapter);
#ifdef CONFIG_CONCURRENT_MODE
        if ((Adapter->isprimary == _FALSE) && Adapter->pbuddy_adapter){
		Adapter = Adapter->pbuddy_adapter;
		pHalData=GET_HAL_DATA(Adapter);
	}
#endif

	DisableInterrupt8192EE( Adapter );

	if( AddMSR )
	{
		pHalData->IntrMask[0] |= AddMSR;
	}
	if( AddMSR1 )
	{
		pHalData->IntrMask[1] |= AddMSR1;
	}

	if( RemoveMSR )
	{
		pHalData->IntrMask[0] &= (~RemoveMSR);
	}

	if( RemoveMSR1 )
	{
		pHalData->IntrMask[1] &= (~RemoveMSR1);
	}

	EnableInterrupt8192EE( Adapter );
}

static VOID
HwConfigureRTL8192EE(
		IN	PADAPTER			Adapter
		)
{

	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	u32	regRRSR = 0;


	//1 This part need to modified according to the rate set we filtered!!
	//
	// Set RRSR, RATR, and BW_OPMODE registers
	//
	switch(pHalData->CurrentWirelessMode)
	{
	case WIRELESS_MODE_B:
		regRRSR = RATE_ALL_CCK;
		break;
	case WIRELESS_MODE_A:
		regRRSR = RATE_ALL_OFDM_AG;
		break;
	case WIRELESS_MODE_G:
		regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
		break;
	case WIRELESS_MODE_UNKNOWN:
	case WIRELESS_MODE_AUTO:
	case WIRELESS_MODE_N_24G:
	case WIRELESS_MODE_AC_24G:
		regRRSR = RATE_ALL_CCK | RATE_ALL_OFDM_AG;
		break;
	case WIRELESS_MODE_N_5G:
	case WIRELESS_MODE_AC_5G:
		regRRSR = RATE_ALL_OFDM_AG;
		break;
	default:
		break;
	}

	// Init value for RRSR.
	rtw_write32(Adapter, REG_RRSR, regRRSR);

//TODO_8192EE
#if 0
	// ARFB table 9 for 11ac 5G 2SS
#if (HAL_MAC_ENABLE == 0)	
	rtw_write32(Adapter, REG_ARFR0, 0x00000010);
#endif
#endif
//TODO_8192EE


//TODO_8192EE
#if 0
	// ARFB table 10 for 11ac 5G 1SS
#if (HAL_MAC_ENABLE == 0)	
	rtw_write32(Adapter, REG_ARFR2, 0x00000010);
#endif
#endif
//TODO_8192EE
	// Configure rate id
	// 	bgn 40M 2ss RATEID_IDX_BGN_40M_2SS
	rtw_write32(Adapter, REG_ARFR0_8192E, 0x0ffff015);

	//	bgn 20M 2ss BN 
	rtw_write32(Adapter, REG_ARFR2_8192E, 0x0f8ff0ff);
	
	// Set SLOT time Reg518 0x9
	PlatformEFIOWrite1Byte(Adapter,REG_SLOT, 0x09);

	// 0x420[7] = 0 , enable retry AMPDU in new AMPD not singal MPDU. 
	rtw_write16(Adapter,REG_FWHW_TXQ_CTRL, 0x1F80);


	// Set retry limit
	//3vivi, 20100928, especially for DTM, performance_ext, To avoid asoc too long to another AP more than 4.1 seconds.
	//3we find retry 7 times maybe not enough, so we retry more than 7 times to pass DTM.
	//if(pMgntInfo->bPSPXlinkMode)
	//{
	//	pHalData->ShortRetryLimit = 3;
	//	pHalData->LongRetryLimit = 3;	
		// Set retry limit
	//	rtw_write16(Adapter,REG_RL, 0x0303);
	//}
	//else
		rtw_write16(Adapter,REG_RL, 0x0707);

	// BAR settings
	rtw_write32(Adapter, REG_BAR_MODE_CTRL, 0x0201ffff);


	// Set Data / Response auto rate fallack retry count
	rtw_write32(Adapter, REG_DARFRC, 0x01000000);
	rtw_write32(Adapter, REG_DARFRC+ 4, 0x07060504);
	rtw_write32(Adapter, REG_RARFRC, 0x01000000);
	rtw_write32(Adapter, REG_RARFRC + 4, 0x07060504);

	// Beacon related, for rate adaptive
	rtw_write8(Adapter, REG_ATIMWND, 0x2);
	
	rtw_write8(Adapter, REG_BCN_MAX_ERR, 0xff);


	// 20100211 Joseph: Change original setting of BCN_CTRL(0x550) from 
	// 0x1e(0x2c for test chip) ro 0x1f(0x2d for test chip). Set BIT0 of this register disable ATIM
	// function. Since we do not use HIGH_QUEUE anymore, ATIM function is no longer used.
	// Also, enable ATIM function may invoke HW Tx stop operation. This may cause ping failed
	// sometimes in long run test. So just disable it now.
	//PlatformAtomicExchange((pu4Byte)(&pHalData->RegBcnCtrlVal), 0x1d);
	pHalData->RegBcnCtrlVal = 0x1d;
#ifdef CONFIG_CONCURRENT_MODE
	rtw_write16(Adapter, REG_BCN_CTRL, 0x1010);	// For 2 PORT TSF SYNC
#else
	rtw_write8(Adapter, REG_BCN_CTRL, (u8)(pHalData->RegBcnCtrlVal));
#endif

	// Marked out by Bruce, 2010-09-09.
	// This register is configured for the 2nd Beacon (multiple BSSID). 
	// We shall disable this register if we only support 1 BSSID.
	//vivi guess 92d also need this, also 92d now doesnot set this reg
	PlatformEFIOWrite1Byte(Adapter, REG_BCN_CTRL_1, 0);

	// BCN_CTRL1 Reg551 0x10

	// TBTT prohibit hold time. Suggested by designer TimChen.
	rtw_write8(Adapter, REG_TBTT_PROHIBIT + 1,0xff); // 8 ms

	rtw_write8(Adapter, REG_PIFS, 0);
	rtw_write8(Adapter, REG_AGGR_BREAK_TIME, 0x16);

	// AGGR_BK_TIME Reg51A 0x16
	
	rtw_write16(Adapter, REG_NAV_PROT_LEN, 0x0040);
	rtw_write16(Adapter, REG_PROT_MODE_CTRL, 0x08ff);

	if(!Adapter->registrypriv.wifi_spec)
	{
		//For Rx TP. Suggested by SD1 Richard. Added by tynli. 2010.04.12.
		rtw_write32(Adapter, REG_FAST_EDCA_CTRL, 0x03086666);
	}
	else
	{
		//For WiFi WMM. suggested by timchen. Added by tynli.	
		rtw_write32(Adapter, REG_FAST_EDCA_CTRL, 0x0);
	}

	// ACKTO for IOT issue.
	PlatformEFIOWrite1Byte(Adapter, REG_ACKTO, 0x40);

#if DISABLE_BB_RF
	
	// 0x50 for 80MHz clock
	rtw_write8(Adapter, REG_USTIME_TSF, 0x50);
	rtw_write8(Adapter, REG_USTIME_EDCA, 0x50);

	// Set Spec SIFS (used in NAV)
	rtw_write16(Adapter,REG_SPEC_SIFS, 0x1010);
	rtw_write16(Adapter,REG_MAC_SPEC_SIFS, 0x1010);

	// Set SIFS for CCK
	rtw_write16(Adapter,REG_SIFS_CTX, 0x1010);	

	// Set SIFS for OFDM
	rtw_write16(Adapter,REG_SIFS_TRX, 0x1010);

	// PIFS
	rtw_write8(Adapter, REG_PIFS, 0);

	// Protection Ctrl
	rtw_write16(Adapter, REG_PROT_MODE_CTRL, 0x08ff);

	// BAR settings
	rtw_write32(Adapter, REG_BAR_MODE_CTRL, 0x0001ffff);
	
	// ACKTO for IOT issue.
	rtw_write8(Adapter, REG_ACKTO, 0x40);
#else
	// Set Spec SIFS Reg428 Reg63A 0x100a
	rtw_write16(Adapter,REG_SPEC_SIFS, 0x100a);
	rtw_write16(Adapter,REG_MAC_SPEC_SIFS, 0x100a);

	// Set SIFS for CCK Reg514 0x100a
	rtw_write16(Adapter,REG_SIFS_CTX, 0x100a);	

	// Set SIFS for OFDM Reg516 0x100a
	rtw_write16(Adapter,REG_SIFS_TRX, 0x100a);

	// Protection Ctrl Reg4C8 0x08ff

	// BAR settings Reg4CC 0x01ffff

	// PIFS Reg512 0x1C

	// ACKTO for IOT issue Reg640 0x80
#endif

	rtw_write8(Adapter, REG_SINGLE_AMPDU_CTRL, 0x80);

	rtw_write8(Adapter, REG_RX_PKT_LIMIT, 0x20);

	rtw_write16(Adapter, REG_MAX_AGGR_NUM, 0x1717);

//#if (OMNIPEEK_SNIFFER_ENABLED == 1)
	// For sniffer parsing legacy rate bandwidth information
//	PHY_SetBBReg(Adapter, 0x834, BIT26, 0x0);
//#endif

	rtw_write32(Adapter, REG_MAR, 0xffffffff);
	rtw_write32(Adapter, REG_MAR+4, 0xffffffff);

	//Reject all control frame - default value is 0
	//rtw_write16(Adapter,REG_RXFLTMAP1,0x0);

	pHalData->RegTxPause = rtw_read8(Adapter, REG_TXPAUSE); 
	pHalData->RegFwHwTxQCtrl = rtw_read8(Adapter, REG_FWHW_TXQ_CTRL+2);
	pHalData->RegReg542 = rtw_read8(Adapter, REG_TBTT_PROHIBIT+2);
	pHalData->RegCR_1 = rtw_read8(Adapter, REG_CR+1);
}

static u32 _InitPowerOn_8192EE(PADAPTER padapter)
{
	u32	status = RT_STATUS_SUCCESS;
	u8	value8, bMacPwrCtrlOn=_FALSE;
	u32	value32;

	DBG_871X("==>%s \n",__FUNCTION__);

	
//TODO_8192EE
	rtw_write8(padapter, REG_RSV_CTRL, 0x0);

	//Auto Power Down to CHIP-off State
	value8 = (rtw_read8(padapter, REG_APS_FSMCO+1) & (~BIT7));
	rtw_write8(padapter, REG_APS_FSMCO+1, value8);
	
//TODO_8192EE


	rtw_hal_get_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	if (bMacPwrCtrlOn == _TRUE)
		return status;
	
	value32 = rtw_read32(padapter, REG_SYS_CFG1_8192E);
	if(value32 & BIT_SPSLDO_SEL){
		//LDO
		rtw_write8(padapter, REG_LDO_SWR_CTRL, 0xC3); 
	}
	else	{
		//SPS
		//0x7C [6] = 1¡¦b0 (IC default, 0x83)
		//0x14[23:20]=b¡¦0101 (raise 1.2V voltage)
		value8 = rtw_read8(padapter,0x16);
		rtw_write8(padapter,0x16,value8|BIT4|BIT6);
		
		//u32 voltage = (rtw_read32(padapter,0x14)& 0xFF0FFFFF )|(0x05<<20);
		//rtw_write32(padapter,0x14,voltage);
		
		rtw_write8(padapter, REG_LDO_SWR_CTRL, 0x83);
	}
	
	//adjust xtal/afe before enable PLL, suggest by SD1-Scott
	Hal_CrystalAFEAdjust(padapter);

	// HW Power on sequence
	if(!HalPwrSeqCmdParsing(padapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_PCI_MSK, Rtl8192E_NIC_ENABLE_FLOW))
		return _FAIL;

//TODO_8192EE
/*
	//Enable Power Down Interrupt 
	tmpU1b = (rtw_read8(padapter, REG_APS_FSMCO) | BIT4);
	rtw_write8(padapter, REG_APS_FSMCO, tmpU1b);
*/	
//TODO_8192EE

	// Release MAC IO register reset
	value8 = rtw_read8(padapter, REG_CR);
	value8 = 0xff;
	rtw_write8(padapter, REG_CR, value8);
	rtw_mdelay_os(2);	

	value8 = 0x7f;
	rtw_write8(padapter, REG_HWSEQ_CTRL, value8);
	rtw_mdelay_os(2);

	// Add for wakeup online
	value8 = PlatformEFIORead1Byte(padapter, REG_SYS_CLKR);
	PlatformEFIOWrite1Byte(padapter, REG_SYS_CLKR, (value8|BIT3));
	value8 = rtw_read8(padapter, REG_GPIO_MUXCFG+1);
	rtw_write8(padapter, REG_GPIO_MUXCFG+1, (value8 & ~BIT4));

	// Release MAC IO register reset
	// 9.	CR 0x100[7:0]	= 0xFF;
	// 10.	CR 0x101[1]	= 0x01; // Enable SEC block
	rtw_write16(padapter,REG_CR, 0x2ff);

	bMacPwrCtrlOn = _TRUE;
	rtw_hal_set_hwreg(padapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	
	return status;
}

static u8
_RQPN_Init_8192EE(
	IN  PADAPTER	Adapter,
	OUT u8		*pNPQRQPNVaule,
	OUT u32		*pRQPNValue
	)
{
	u8			i, numNQ, numPubQ, numHQ, numLQ;
	u8			llt_page_num;
	u8*			numPageArray;

	if(Adapter->registrypriv.wifi_spec)
		numPageArray = WMM_PAGE_NUM_8192EE;
	else
		numPageArray = NORMAL_PAGE_NUM_8192EE;

    numHQ   = numPageArray[1];
    numLQ   = numPageArray[2];
    numNQ   = numPageArray[3];

	llt_page_num = TX_TOTAL_PAGE_NUMBER_8192E - 1; //Reserve 1 page at the boundary for safety
    numPubQ = llt_page_num - (numHQ+numLQ+numNQ);

	*pNPQRQPNVaule = _NPQ(numNQ);
	*pRQPNValue = _HPQ(numHQ) | _LPQ(numLQ) | _PUBQ(numPubQ) | LD_RQPN; 

	// Set whether Tx page size excludes the public queue
	if(Adapter->registrypriv.wifi_spec)
		*pRQPNValue |=0x0b000000; // BIT_EPQ_PUBLIC_DIS|BIT_LPQ_PUBLIC_DIS|BIT_HPQ_PUBLIC_DIS	
		
	return TX_PAGE_BOUNDARY_8192E;
}

static s32
LLT_table_init_8192EE(
	IN	PADAPTER	Adapter,
	IN	u8			txpktbuf_bndy,
	IN	u32			RQPN,
	IN	u8			RQPN_NPQ
	)
{
	u16	i, maxPage = 255;
	s32	status = _SUCCESS;
	u8	tmp1byte=0, testcnt=0;

	/* NPQ MUST be set before RQPN */
	rtw_write8(Adapter, REG_RQPN_NPQ, RQPN_NPQ);

	// Set reserved page for each queue
	// 11.	RQPN 
	rtw_write32(Adapter, REG_RQPN, RQPN);

	_InitTxBufferBoundary_8192E(Adapter, txpktbuf_bndy);

	// 15.	WMAC_LBK_BF_HD 0x45D[7:0] =  0xF6			//WMAC_LBK_BF_HD
	PlatformEFIOWrite1Byte(Adapter,REG_WMAC_LBK_BUF_HD_8192E, txpktbuf_bndy);
	
	// Set Tx/Rx page size (Tx must be 128 Bytes, Rx can be 64,128,256,512,1024 bytes)
	// 16.	PBP [7:0] = 0x11								// TRX page size
	//  		This register is obseleted. Page size is configured in tx buffer descriptor
	rtw_write8(Adapter,REG_PBP, 0x31);

	// 17.	DRV_INFO_SZ = 0x04
	rtw_write8(Adapter,REG_RX_DRVINFO_SZ, 0x4);	
	tmp1byte = rtw_read8(Adapter,REG_AUTO_LLT+2);
	PlatformEFIOWrite1Byte(Adapter,REG_AUTO_LLT+2,tmp1byte|BIT0);

	while(tmp1byte & BIT0)
	{
		tmp1byte = rtw_read8(Adapter,REG_AUTO_LLT+2);
		rtw_udelay_os(10);
		testcnt++;
		if(testcnt>10)
			break;
	
	}

	return _SUCCESS;	
}

static  u32 
InitMAC_8192EE(
	IN	PADAPTER	Adapter
)
{
	u8	tmpU1b;
	u16	tmpU2b;
	struct recv_priv	*precvpriv = &Adapter->recvpriv;
	struct xmit_priv	*pxmitpriv = &Adapter->xmitpriv;
	HAL_DATA_TYPE	*pHalData	= GET_HAL_DATA(Adapter);

	DBG_871X("=======>InitMAC_8192EE()\n");

	_InitPowerOn_8192EE(Adapter);

	//if(!pHalData->bMACFuncEnable)	
	{
		//System init
		// 18.	LLT_table_init(Adapter); 
		u32	RQPN = 0x80EB0808;
		u8	RQPN_NPQ = 0;
		u8	TX_PAGE_BOUNDARY = 
				_RQPN_Init_8192EE(Adapter, &RQPN_NPQ, &RQPN);
		
		if(LLT_table_init_8192EE(Adapter, TX_PAGE_BOUNDARY, RQPN, RQPN_NPQ) == _FAIL)
			return _FAIL;
	}
	//_InitQueuePriority_8192E(Adapter);		
	_InitPageBoundary_8192E(Adapter);	

	// Enable Host Interrupt
	rtw_write32(Adapter,REG_HISR0_8192E, 0xffffffff);
	rtw_write32(Adapter,REG_HISR1_8192E, 0xffffffff);

	tmpU2b = rtw_read16(Adapter,REG_TRXDMA_CTRL);
	tmpU2b &= 0xf;
	if(Adapter->registrypriv.wifi_spec)
		tmpU2b |= 0xF9B1;
	else
		tmpU2b |= 0xF5B1;
	rtw_write16(Adapter,REG_TRXDMA_CTRL, tmpU2b);

	
	// Reported Tx status from HW for rate adaptive.
	// 2009/12/03 MH This should be realtive to power on step 14. But in document V11  
	// still not contain the description.!!!
	rtw_write8(Adapter,REG_FWHW_TXQ_CTRL+1, 0x1F);

	// Set RCR register
	rtw_write32(Adapter,REG_RCR, pHalData->ReceiveConfig);
	rtw_write16(Adapter, REG_RXFLTMAP2, 0xFFFF);

	// Set TCR register
	rtw_write32(Adapter,REG_TCR, pHalData->TransmitConfig);
	
	//
	// Set TX/RX descriptor physical address(from OS API).
	//
	rtw_write32(Adapter, REG_BCNQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[BCN_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_MGQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[MGT_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_VOQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[VO_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_VIQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[VI_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_BEQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[BE_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_BKQ_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[BK_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_HI0Q_TXBD_DESA_8192E, (u64)pxmitpriv->tx_ring[HIGH_QUEUE_INX].dma & DMA_BIT_MASK(32));
	rtw_write32(Adapter, REG_RXQ_RXBD_DESA_8192E, (u64)precvpriv->rx_ring[RX_MPDU_QUEUE].dma & DMA_BIT_MASK(32));

#ifdef CONFIG_64BIT_DMA
	// 2009/10/28 MH For DMA 64 bits. We need to assign the high 32 bit address
	// for NIC HW to transmit data to correct path.
	rtw_write32(Adapter, REG_BCNQ_TXBD_DESA_8192E+4, 
		((u64)pxmitpriv->tx_ring[BCN_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter, REG_MGQ_TXBD_DESA_8192E+4, 
		((u64)pxmitpriv->tx_ring[MGT_QUEUE_INX].dma)>>32);  
	rtw_write32(Adapter, REG_VOQ_TXBD_DESA_8192E+4, 
		((u64)pxmitpriv->tx_ring[VO_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter, REG_VIQ_TXBD_DESA_8192E+4, 
		((u64)pxmitpriv->tx_ring[VI_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter, REG_BEQ_TXBD_DESA_8192E+4, 
		((u64)pxmitpriv->tx_ring[BE_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter, REG_BKQ_TXBD_DESA_8192E+4,
		((u64)pxmitpriv->tx_ring[BK_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter,REG_HI0Q_TXBD_DESA_8192E+4,
		((u64)pxmitpriv->tx_ring[HIGH_QUEUE_INX].dma)>>32);
	rtw_write32(Adapter, REG_RXQ_RXBD_DESA_8192E+4, 
		((u64)precvpriv->rx_ring[RX_MPDU_QUEUE].dma)>>32);

	// 2009/10/28 MH If RX descriptor address is not equal to zero. We will enable
	// DMA 64 bit functuion.
	// Note: We never saw thd consition which the descripto address are divided into
	// 4G down and 4G upper seperate area.
	if (((u64)precvpriv->rx_ring[RX_MPDU_QUEUE].dma)>>32 != 0)
	{
		//DBG_871X("RX_DESC_HA=%08lx\n", ((u64)priv->rx_ring_dma[RX_MPDU_QUEUE])>>32);
		DBG_871X("Enable DMA64 bit\n");

		// Check if other descriptor address is zero and abnormally be in 4G lower area.
		if (((u64)pxmitpriv->tx_ring[MGT_QUEUE_INX].dma)>>32)
		{
			DBG_871X("MGNT_QUEUE HA=0\n");
		}
		
		PlatformEnableDMA64(Adapter);
	}
	else
	{
		DBG_871X("Enable DMA32 bit\n");
	}
#endif

	// pci buffer descriptor mode: Reset the Read/Write point to 0
	PlatformEFIOWrite4Byte(Adapter, REG_TSFTIMER_HCI_8192E, 0x3fffffff);

	tmpU1b = rtw_read8(Adapter,REG_PCIE_CTRL_REG_8192E+3);
	rtw_write8(Adapter,REG_PCIE_CTRL_REG_8192E+3, (tmpU1b|0xF7));

	// 20100318 Joseph: Reset interrupt migration setting when initialization. Suggested by SD1.
	rtw_write32(Adapter, REG_INT_MIG, 0);	
	pHalData->bInterruptMigration = _FALSE;

	//2009.10.19. Reset H2C protection register. by tynli.
	rtw_write32(Adapter, REG_MCUTST_1, 0x0);

#if MP_DRIVER == 1
	if (Adapter->registrypriv.mp_mode == 1) {
		rtw_write32(Adapter, REG_MACID, 0x87654321);
		rtw_write32(Adapter, 0x0700, 0x87654321);
	}
#endif

	// pic buffer descriptor mode: 
	// ---- tx
	rtw_write16(Adapter, REG_MGQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_VOQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_VIQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_BEQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_VOQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_BKQ_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI0Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI1Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI2Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI3Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI4Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI5Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI6Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));
	rtw_write16(Adapter, REG_HI7Q_TXBD_NUM_8192E, TX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<12 ) & 0x3000));		
	// ---- rx. support 32 bits in linux
//8192EE_TODO	
	//rtw_write16(Adapter, REG_RX_RXBD_NUM_8192E, RX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<13 ) & 0x6000) |0x8000); //using 64bit 
	rtw_write16(Adapter, REG_RX_RXBD_NUM_8192E, RX_DESC_NUM_92E |((RTL8192EE_SEG_NUM<<13 ) & 0x6000) ); //using 32bit 
//8192EE_TODO	
	// ---- reset read/write point
	rtw_write32(Adapter, REG_TSFTIMER_HCI_8192E, 0XFFFFFFFF); //Reset read/write point


	rtw_write8(Adapter,0x577,0x03);

	//for Crystal 40 Mhz setting
	rtw_write8(Adapter,REG_AFE_CTRL4_8192E,0x2A);
	rtw_write8(Adapter,REG_AFE_CTRL4_8192E+1,0x00);
	rtw_write8(Adapter,REG_AFE_CTRL2_8192E,0x83);

	//Forced the antenna b to wifi.
	if(pHalData->EEPROMBluetoothCoexist ==FALSE)
	{
		u1Byte tmp1Byte=0;
	
		tmp1Byte = rtw_read8(Adapter,0x64);
		rtw_write8(Adapter, 0x64, (tmp1Byte & 0xFB) & ~BIT2);
		
		tmp1Byte = rtw_read8(Adapter,0x65);
		rtw_write8(Adapter,0x65,( tmp1Byte&0xFE)|BIT0 );

		tmp1Byte = rtw_read8(Adapter,0x4F);
		rtw_write8(Adapter,0x4F,( tmp1Byte&0xF6)|BIT3|BIT0 );	
	}

	pHalData->RxTag=0;
	// Release Pcie Interface Tx DMA.
#if defined(USING_RX_TAG)
	rtw_write16(Adapter, REG_PCIE_CTRL_REG_8192E, 0x8000);
#else
	rtw_write16(Adapter, REG_PCIE_CTRL_REG_8192E, 0x0000);
#endif


	DBG_871X("InitMAC_8192EE() <====\n");

	return _SUCCESS;
}
//TODO_8192EE
#if 0
u16
hal_MDIORead_8192EE(	
	IN	PADAPTER 	Adapter,
	IN 	u8			Addr
)
{
	u16	ret = 0;

	u8	tmpU1b = 0, count = 0;

	rtw_write8(Adapter, REG_MDIO_CTL_8812, Addr|BIT6);
	tmpU1b = rtw_read8(Adapter, REG_MDIO_CTL_8812) & BIT6 ;
	count = 0;
	while(tmpU1b && count < 20)
	{
		rtw_udelay_os(10);
		tmpU1b = rtw_read8(Adapter, REG_MDIO_CTL_8812) & BIT6;
		count++;
	}
	if(0 == tmpU1b)
	{
		ret = rtw_read16(Adapter, REG_MDIO_RDATA_8812);
	}

	return ret;
}

VOID
hal_MDIOWrite_8812E(	
	IN	PADAPTER 	Adapter,
	IN 	u8			Addr,
	IN	u16			Data
)
{
	u8	tmpU1b = 0, count = 0;

	rtw_write16(Adapter, REG_MDIO_WDATA_8812, Data);
	rtw_write8(Adapter, REG_MDIO_CTL_8812, Addr|BIT5);
	tmpU1b = rtw_read8(Adapter, REG_MDIO_CTL_8812) & BIT5 ;
	count = 0;
	while(tmpU1b && count < 20)
	{
		rtw_udelay_os(10);
		tmpU1b = rtw_read8(Adapter, REG_MDIO_CTL_8812) & BIT5; 
		count++;
	}
}

#endif	
//TODO_8192EE

static VOID
EnableAspmBackDoor_8192E(IN	PADAPTER Adapter)
{
	u4Byte tmp4Byte = 0, count = 0;
	u1Byte tmp1byte = 0;


	// 0x70f BIT7 is used to control L0S
	// 20100212 Tynli: Set register offset 0x70f in PCI configuration space to the value 0x23 
	// for all bridge suggested by SD1. Origianally this is only for INTEL.
	// 20100422 Joseph: Set PCI configuration space offset 0x70F to 0x93 to Enable L0s for all platform.
	// This is suggested by SD1 Glayrainx and for Lenovo's request.
	// 20120316 YJ: Use BIT31|value(read from 0x70C) intead of 0x93.
	rtw_write16(Adapter, REG_DBI_FLAG_V1_8192E, 0x70c);
	rtw_write8(Adapter, REG_DBI_FLAG_V1_8192E+2, 0x2);
	tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
	count = 0;
	while(tmp1byte && count < 20)
	{
		rtw_udelay_os(10);
		tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
		count++;
	}	
	if(0 == tmp1byte)
	{
		tmp4Byte=rtw_read32(Adapter, REG_DBI_RDATA_V1_8192E);
		rtw_write32(Adapter, REG_DBI_WDATA_V1_8192E, tmp4Byte|BIT31);
		rtw_write16(Adapter, REG_DBI_FLAG_V1_8192E, 0xf70c);
		rtw_write8(Adapter, REG_DBI_FLAG_V1_8192E+2, 0x1);
	}
	
	tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
	count = 0;
	while(tmp1byte && count < 20)
	{
		rtw_udelay_os(10);
		tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
		count++;
	}

	// 0x719 Bit3 is for L1 BIT4 is for clock request 
	// 20100427 Joseph: Disable L1 for Toshiba AMD platform. If AMD platform do not contain
	// L1 patch, driver shall disable L1 backdoor.
	// 20120316 YJ: Use BIT11|BIT12|value(read from 0x718) intead of 0x1b.
	rtw_write16(Adapter, REG_DBI_FLAG_V1_8192E, 0x718);
	rtw_write8(Adapter, REG_DBI_FLAG_V1_8192E+2, 0x2);
	tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
	count = 0;
	while(tmp1byte && count < 20)
	{
		rtw_udelay_os(10);
		tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
		count++;
	}
	if(GET_HAL_DATA(Adapter)->bSupportBackDoor || (0 == tmp1byte))
	{
		tmp4Byte = rtw_read32(Adapter, REG_DBI_RDATA_V1_8192E);
		rtw_write32(Adapter, REG_DBI_WDATA_V1_8192E, tmp4Byte|BIT11|BIT12);
		rtw_write16(Adapter, REG_DBI_FLAG_V1_8192E, 0xf718);
		rtw_write8(Adapter, REG_DBI_FLAG_V1_8192E+2, 0x1);
	}
	tmp1byte = PlatformEFIORead1Byte(Adapter, REG_DBI_FLAG_V1_8192E+2);
	count = 0;
	while(tmp1byte && count < 20)
	{
		rtw_udelay_os(10);
		tmp1byte = rtw_read8(Adapter, REG_DBI_FLAG_V1_8192E+2);
		count++;
	}
}

static u32 rtl8192ee_hal_init(PADAPTER Adapter)
{
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(Adapter);
	EEPROM_EFUSE_PRIV	*pEEPROM = GET_EEPROM_EFUSE_PRIV(Adapter);
	struct pwrctrl_priv		*pwrpriv = adapter_to_pwrctl(Adapter);
	u32	rtStatus = _SUCCESS;
	u8	tmpU1b, u1bRegCR;
	u32	i;
	BOOLEAN bSupportRemoteWakeUp, is2T2R = TRUE;
	u32	NavUpper = WiFiNavUpperUs;

_func_enter_;

	//
	// No I/O if device has been surprise removed
	//
	if (Adapter->bSurpriseRemoved)
	{
		DBG_8192C("rtl8812ae_hal_init(): bSurpriseRemoved!\n");
		return _SUCCESS;
	}

	Adapter->init_adpt_in_progress = _TRUE;
	DBG_8192C("=======>rtl8192ee_hal_init()\n");

	tmpU1b = rtw_read8(Adapter, REG_SYS_CLKR+1);	
	u1bRegCR = rtw_read8(Adapter, REG_CR);
	DBG_871X(" power-on :REG_CR 0x100=0x%02x.\n", u1bRegCR);
	if((tmpU1b&BIT3) &&  (u1bRegCR != 0 && u1bRegCR != 0xEA))
	{
		//pHalData->bMACFuncEnable = TRUE;
		DBG_871X(" MAC has already power on.\n");
	}
	else
	{
		//pHalData->bMACFuncEnable = FALSE;		
		// Set FwPSState to ALL_ON mode to prevent from the I/O be return because of 32k
		// state which is set before sleep under wowlan mode. 2012.01.04. by tynli.
		//pHalData->FwPSState = FW_PS_STATE_ALL_ON_88E;
	}

	//
	// 1. MAC Initialize
	//
	rtStatus = InitMAC_8192EE(Adapter);
	if(rtStatus != _SUCCESS)
	{
		DBG_8192C("Init MAC failed\n");
		return rtStatus;
	}

#if HAL_FW_ENABLE
	if ((Adapter->registrypriv.mp_mode != 1) || IS_HARDWARE_TYPE_8821(Adapter)) {
		tmpU1b = rtw_read8(Adapter, REG_SYS_CFG);
		rtw_write8(Adapter, REG_SYS_CFG, tmpU1b & 0x7F);

		rtStatus = FirmwareDownload8192E(Adapter, _FALSE);

		if(rtStatus != _SUCCESS)
		{
			DBG_8192C("FwLoad failed\n");
			rtStatus = _SUCCESS;
			Adapter->bFWReady = _FALSE;
			pHalData->fw_ractrl = _FALSE;
		}
		else
		{
			DBG_8192C("FwLoad SUCCESSFULLY!!!\n");
			Adapter->bFWReady = _TRUE;
			pHalData->fw_ractrl = _TRUE;
		}
		InitializeFirmwareVars8192E(Adapter);
	}
#endif

	pHalData->CurrentChannel = 0;//set 0 to trigger switch correct channel	

	//
	// 2. Initialize MAC/PHY Config by MACPHY_reg.txt
	//
#if (HAL_MAC_ENABLE == 1)
	DBG_871X("8192ee MAC Config Start!\n");
	rtStatus = PHY_MACConfig8192E(Adapter);
	if (rtStatus != _SUCCESS)
	{
		DBG_871X("8192ee MAC Config failed\n");
		return rtStatus;
	}
	DBG_871X("8192ee MAC Config Finished!\n");


//TODO_8192EE
	//remove to sync with windows driver
	//rtw_write32(Adapter,REG_RCR, rtw_read32(Adapter, REG_RCR)&~(RCR_ADF) );
//TODO_8192EE	
#endif	// #if (HAL_MAC_ENABLE == 1)

	//
	// 3. Initialize BB After MAC Config PHY_reg.txt, AGC_Tab.txt
	//
#if (HAL_BB_ENABLE == 1)
	DBG_871X("BB Config Start!\n");
	rtStatus = PHY_BBConfig8192E(Adapter);
	if (rtStatus!= _SUCCESS)
	{
		DBG_871X("BB Config failed\n");
		return rtStatus;
	}
	DBG_871X("BB Config Finished!\n");
#endif	// #if (HAL_BB_ENABLE == 1)

	//
	// 4. Initiailze RF RAIO_A.txt RF RAIO_B.txt
	//
#if (HAL_RF_ENABLE == 1)		
	DBG_871X("RF Config started!\n");
	rtStatus = PHY_RFConfig8192E(Adapter);
	if(rtStatus != _SUCCESS)
	{
		DBG_871X("RF Config failed\n");
		return rtStatus;
	}
	DBG_871X("RF Config Finished!\n");


#endif	// #if (HAL_RF_ENABLE == 1)	


//TODO_8192EE
	//Port from Windows Driver
	pHalData->RfRegChnlVal[0] = PHY_QueryRFReg(Adapter, 0, RF_CHNLBW, bRFRegOffsetMask);
	pHalData->RfRegChnlVal[1] = PHY_QueryRFReg(Adapter, 1, RF_CHNLBW, bRFRegOffsetMask);
	//pHalData->backupRF0x1A = (u4Byte)PHY_QueryRFReg(Adapter, ODM_RF_PATH_A, RF_RX_G1, bRFRegOffsetMask);		
	pHalData->RfRegChnlVal[0] = ((pHalData->RfRegChnlVal[0] & 0xfffff3ff) | BIT10 |BIT11  );
	PHY_SetRFReg(Adapter, ODM_RF_PATH_A, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);
	PHY_SetRFReg(Adapter, ODM_RF_PATH_B, RF_CHNLBW, bRFRegOffsetMask, pHalData->RfRegChnlVal[0]);

	/*---- Set CCK and OFDM Block "ON"----*/

	/*---- Set CCK and OFDM Block "ON"----*/
	PHY_SetBBReg(Adapter, rFPGA0_RFMOD, bCCKEn, 0x1);			    
	PHY_SetBBReg(Adapter, rFPGA0_RFMOD, bOFDMEn, 0x1);

	//Must set this, otherwise the rx sensitivity will be very pool. Maddest
	PHY_SetRFReg(Adapter, ODM_RF_PATH_A, 0xB1, bRFRegOffsetMask, 0x54418);
	

	//3 Set Hardware(MAC default setting.)
	HwConfigureRTL8192EE(Adapter);

//TODO_8192EE
//	if(Adapter->registrypriv.channel <= 14)
//		PHY_SwitchWirelessBand8812(Adapter, BAND_ON_2_4G);
//	else
//		PHY_SwitchWirelessBand8812(Adapter, BAND_ON_5G);
//TODO_8192EE
	rtw_hal_set_chnl_bw(Adapter, Adapter->registrypriv.channel, 
		CHANNEL_WIDTH_20, HAL_PRIME_CHNL_OFFSET_DONT_CARE, HAL_PRIME_CHNL_OFFSET_DONT_CARE);

	// We should set pHalData->bMACFuncEnable to TURE after LLT initialize and Fw download
	// and before GPIO detect. by tynli. 2011.05.27.
	//pHalData->bMACFuncEnable = _TRUE;

	//3Security related
	//-----------------------------------------------------------------------------
	// Set up security related. 070106, by rcnjko:
	// 1. Clear all H/W keys.
	//-----------------------------------------------------------------------------
	invalidate_cam_all(Adapter);

	// Joseph debug: MAC_SEC_EN need to be set
	rtw_write8(Adapter, REG_CR+1, (rtw_read8(Adapter, REG_CR+1)|BIT1));

	//2=======================================================
	// RF Power Save
	//2=======================================================
	// Fix the bug that Hw/Sw radio off before S3/S4, the RF off action will not be executed 
	// in MgntActSet_RF_State() after wake up, because the value of pHalData->eRFPowerState 
	// is the same as eRfOff, we should change it to eRfOn after we config RF parameters.
	// Added by tynli. 2010.03.30.
	pwrpriv->rf_pwrstate = rf_on;

	pwrpriv->rfoff_reason |= (pwrpriv->reg_rfoff) ? RF_CHANGE_BY_SW : 0;

	if(pwrpriv->rfoff_reason&RF_CHANGE_BY_HW)
		pwrpriv->b_hw_radio_off = _TRUE;

	if(pwrpriv->rfoff_reason > RF_CHANGE_BY_PS)
	{ // H/W or S/W RF OFF before sleep.
		DBG_8192C("InitializeAdapter8188EE(): Turn off RF for RfOffReason(%d) ----------\n", pwrpriv->rfoff_reason);
		//MgntActSet_RF_State(Adapter, rf_off, pwrpriv->rfoff_reason, _TRUE);
	}
	else
	{
		pwrpriv->rf_pwrstate = rf_on;
		pwrpriv->rfoff_reason = 0;

		DBG_8192C("InitializeAdapter8812E(): Turn on  ----------\n");

		// LED control
		rtw_led_control(Adapter, LED_CTL_POWER_ON);

	}

	// Fix the bug that when the system enters S3/S4 then tirgger HW radio off, after system
	// wakes up, the scan OID will be set from upper layer, but we still in RF OFF state and scan
	// list is empty, such that the system might consider the NIC is in RF off state and will wait 
	// for several seconds (during this time the scan OID will not be set from upper layer anymore)
	// even though we have already HW RF ON, so we tell the upper layer our RF state here.
	// Added by tynli. 2010.04.01.
	//DrvIFIndicateCurrentPhyStatus(Adapter);

	if(Adapter->registrypriv.hw_wps_pbc)
	{
		tmpU1b = rtw_read8(Adapter, GPIO_IO_SEL);
		tmpU1b &= ~(HAL_8192C_HW_GPIO_WPS_BIT);
		rtw_write8(Adapter, GPIO_IO_SEL, tmpU1b);	//enable GPIO[2] as input mode
	}

	//
	// Execute TX power tracking later
	//

	// We must set MAC address after firmware download. HW do not support MAC addr
	// autoload now.
	hal_init_macaddr(Adapter);

	EnableAspmBackDoor_8192E(Adapter);


#ifdef CONFIG_BT_COEXIST
	//_InitBTCoexist(Adapter);
#endif
#ifdef CONFIG_BT_COEXIST
	// YiWei 20140624 , Fix 8192eu mailbox BT info no response issue 
    // reduce I2C clock rate to 156KHz (default 1.25Mhz)
    if ( pHalData->EEPROMBluetoothCoexist == 1)
    {
    	rtw_write8(Adapter, rPMAC_TxPacketNum, rtw_read8(Adapter, rPMAC_TxPacketNum)|0x7);
    }
	// Init BT hw config.
	rtw_btcoex_HAL_Initialize(Adapter, _FALSE);
#endif

	rtl8192e_InitHalDm(Adapter);

	Adapter->init_adpt_in_progress = _FALSE;

#ifdef CONFIG_CHECK_AC_LIFETIME
	// Enable lifetime check for the four ACs
	rtw_write8(Adapter, REG_LIFETIME_CTRL, 0x0F);
#endif	// CONFIG_CHECK_AC_LIFETIME

#if defined(CONFIG_CONCURRENT_MODE) || defined(CONFIG_TX_MCAST2UNI)
#ifdef CONFIG_TX_MCAST2UNI
	rtw_write16(Adapter, REG_PKT_VO_VI_LIFE_TIME, 0x0400);	// unit: 256us. 256ms
	rtw_write16(Adapter, REG_PKT_BE_BK_LIFE_TIME, 0x0400);	// unit: 256us. 256ms
#else	// CONFIG_TX_MCAST2UNI
	rtw_write16(Adapter, REG_PKT_VO_VI_LIFE_TIME, 0x3000);	// unit: 256us. 3s
	rtw_write16(Adapter, REG_PKT_BE_BK_LIFE_TIME, 0x3000);	// unit: 256us. 3s
#endif	// CONFIG_TX_MCAST2UNI
#endif	// CONFIG_CONCURRENT_MODE || CONFIG_TX_MCAST2UNI


	//enable tx DMA to drop the redundate data of packet
	rtw_write16(Adapter,REG_TXDMA_OFFSET_CHK, (rtw_read16(Adapter,REG_TXDMA_OFFSET_CHK) | DROP_DATA_EN));

	pHalData->RegBcnCtrlVal = rtw_read8(Adapter, REG_BCN_CTRL);
	pHalData->RegTxPause = rtw_read8(Adapter, REG_TXPAUSE); 
	pHalData->RegFwHwTxQCtrl = rtw_read8(Adapter, REG_FWHW_TXQ_CTRL+2);
	pHalData->RegReg542 = rtw_read8(Adapter, REG_TBTT_PROHIBIT+2);

	//EnableInterrupt8188EE(Adapter);

#if (MP_DRIVER == 1)
	if (Adapter->registrypriv.mp_mode == 1)
	{
		Adapter->mppriv.channel = pHalData->CurrentChannel;
		MPT_InitializeAdapter(Adapter, Adapter->mppriv.channel);
	}
	else
#endif
/*	if(pHalData->eRFPowerState == eRfOn)
	{
		if(pHalData->LedStrategy != SW_LED_MODE10)
			PHY_SetRFPathSwitch_8812(Adapter, pMgntInfo->bDefaultAntenna);	//Wifi default use Main

		if(pHalData->bIQKInitialized )
			PHY_IQCalibrate_8812(Adapter, TRUE);
		else
		{
			PHY_IQCalibrate_8812(Adapter,FALSE);
			pHalData->bIQKInitialized = TRUE;
		}		
		ODM_TXPowerTrackingCheck(&pHalData->DM_OutSrc);
		PHY_LCCalibrate_8812(Adapter);
	}
*/

	is2T2R = IS_2T2R(pHalData->VersionID);

	tmpU1b = EFUSE_Read1Byte(Adapter, 0x1FA); 

	if(!(tmpU1b & BIT0))
	{
		PHY_SetRFReg(Adapter, ODM_RF_PATH_A, 0x15, 0x0F, 0x05);
		DBG_871X("PA BIAS path A\n");
	}	

	if(!(tmpU1b & BIT1) && is2T2R)
	{
		PHY_SetRFReg(Adapter, ODM_RF_PATH_B, 0x15, 0x0F, 0x05);
		DBG_871X("PA BIAS path B\n");
	}
	
	//Fixed LDPC rx hang issue.
	{
		u4Byte	tmp4Byte = rtw_read32(Adapter, REG_SYS_SWR_CTRL1_8192E);
		rtw_write8(Adapter,REG_SYS_SWR_CTRL2_8192E,0x75);
		tmp4Byte=  (tmp4Byte & 0xfff00fff)|(0x7E<<12);
		rtw_write32(Adapter,REG_SYS_SWR_CTRL1_8192E,tmp4Byte );
	}
	PHY_SetRFEReg_8192E(Adapter);

	/* For 11n logo 5.2.3 AP & STA association and Throughput Honoring NAV */
	if( Adapter->registrypriv.wifi_spec==1 )
		NavUpper = 0;

	rtw_hal_set_hwreg(Adapter, HW_VAR_NAV_UPPER, ((u8 *)&NavUpper));

#ifdef CONFIG_XMIT_ACK
	//ack for xmit mgmt frames.
	rtw_write32(Adapter, REG_FWHW_TXQ_CTRL, rtw_read32(Adapter, REG_FWHW_TXQ_CTRL)|BIT(12));
#endif //CONFIG_XMIT_ACK

#ifdef CONFIG_HIGH_CHAN_SUPER_CALIBRATION
	rtw_hal_set_chnl_bw(Adapter, 13, 
		CHANNEL_WIDTH_20, HAL_PRIME_CHNL_OFFSET_DONT_CARE, HAL_PRIME_CHNL_OFFSET_DONT_CARE); 

	PHY_SpurCalibration_8192E(Adapter);
	
	rtw_hal_set_chnl_bw(Adapter, Adapter->registrypriv.channel, 
		CHANNEL_WIDTH_20, HAL_PRIME_CHNL_OFFSET_DONT_CARE, HAL_PRIME_CHNL_OFFSET_DONT_CARE);
#endif

	if( Adapter->registrypriv.wifi_spec==1 ) {
		/* For 11n logo 5.2.29 Traffic Differentition in Single BSS with WMM STA */
		PHY_SetBBReg(Adapter, rOFDM0_ECCAThreshold, bMaskDWord, 0x00fe0301);
	}

/*{
	DBG_8192C("===== Start Dump Reg =====");
	for(i = 0 ; i <= 0xeff ; i+=4)
	{
		if(i%16==0)
			DBG_8192C("\n%04x: ",i);
		DBG_8192C("0x%08x ",rtw_read32(Adapter, i));
	}
	DBG_8192C("\n ===== End Dump Reg =====\n");
}*/

_func_exit_;

	DBG_871X("read[REG_MGQ_TXBD_DESA_8192E] = %x\n", rtw_read32(Adapter, REG_MGQ_TXBD_DESA_8192E));

	return rtStatus;
}

//
// 2009/10/13 MH Acoording to documetn form Scott/Alfred....
// This is based on version 8.1.
//




VOID
hal_poweroff_8192ee(
	IN	PADAPTER			Adapter 
)
{
	u8 u1bTmp;
	u8 bMacPwrCtrlOn =_FALSE;
	HAL_DATA_TYPE		*pHalData	= GET_HAL_DATA(Adapter);

	rtw_hal_get_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);
	if(bMacPwrCtrlOn == _FALSE)	
		return ;
	//pHalData->bMACFuncEnable = _FALSE;

	// Run LPS WL RFOFF flow
	HalPwrSeqCmdParsing(Adapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_PCI_MSK, Rtl8192E_NIC_LPS_ENTER_FLOW);

	// 0x1F[7:0] = 0		// turn off RF
	//rtw_write8(Adapter, REG_RF_CTRL_8812, 0x00);

	//	==== Reset digital sequence   ======	
	if((rtw_read8(Adapter, REG_MCUFWDL)&BIT7) && Adapter->bFWReady) //8051 RAM code
	{	
		_8051Reset8192E(Adapter);
	}

	// Reset MCU. Suggested by Filen. 2011.01.26. by tynli.
	u1bTmp = rtw_read8(Adapter, REG_SYS_FUNC_EN+1);
	rtw_write8(Adapter, REG_SYS_FUNC_EN+1, (u1bTmp&(~BIT2)));

	// MCUFWDL 0x80[1:0]=0				// reset MCU ready status
	rtw_write8(Adapter, REG_MCUFWDL, 0x00);

	// HW card disable configuration.
	HalPwrSeqCmdParsing(Adapter, PWR_CUT_ALL_MSK, PWR_FAB_ALL_MSK, PWR_INTF_PCI_MSK, Rtl8192E_NIC_DISABLE_FLOW);
	
	// Reset MCU IO Wrapper
	u1bTmp = rtw_read8(Adapter, REG_RSV_CTRL+1);
	rtw_write8(Adapter, REG_RSV_CTRL+1, (u1bTmp&(~BIT0)));	
	u1bTmp = rtw_read8(Adapter, REG_RSV_CTRL+1);
	rtw_write8(Adapter, REG_RSV_CTRL+1, u1bTmp|BIT0);

	// RSV_CTRL 0x1C[7:0] = 0x0E			// lock ISO/CLK/Power control register
	rtw_write8(Adapter, REG_RSV_CTRL, 0x0e);

	bMacPwrCtrlOn = _FALSE;	
	rtw_hal_set_hwreg(Adapter, HW_VAR_APFM_ON_MAC, &bMacPwrCtrlOn);

	Adapter->bFWReady = _FALSE;

}

VOID
PowerOffAdapter8192EE(
	IN	PADAPTER			Adapter	
	)
{
	hal_poweroff_8192ee(Adapter);
}

static u32 rtl8192ee_hal_deinit(PADAPTER Adapter)
{
	u8	u1bTmp = 0;
	u32 	count = 0;
	u8	bSupportRemoteWakeUp = _FALSE;
	HAL_DATA_TYPE		*pHalData = GET_HAL_DATA(Adapter);
	struct pwrctrl_priv		*pwrpriv = adapter_to_pwrctl(Adapter);
	
_func_enter_;

	DBG_871X("====> %s() \n", __FUNCTION__);

	if (Adapter->bHaltInProgress == _TRUE)
	{
		DBG_8192C("====> Abort rtl8812ae_hal_deinit()\n");
		return _FAIL;
	}

	Adapter->bHaltInProgress = _TRUE;

	//
	// No I/O if device has been surprise removed
	//
	if (Adapter->bSurpriseRemoved)
	{
		Adapter->bHaltInProgress = _FALSE;
		return _SUCCESS;
	}

	RT_SET_PS_LEVEL(pwrpriv, RT_RF_OFF_LEVL_HALT_NIC);

	// Without supporting WoWLAN or the driver is in awake (D0) state, we should 
	// call PowerOffAdapter8188EE() to run the power sequence. 2009.04.23. by tynli.
	if(!bSupportRemoteWakeUp )//||!pMgntInfo->bPwrSaveState) 
	{
		// 2009/10/13 MH For power off test.
		PowerOffAdapter8192EE(Adapter);	
	}
#if 0
	else
	{
		s32	rtStatus = _SUCCESS;

		//--------------------------------------------------------
		//3 <1> Prepare for configuring wowlan related infomations
		//--------------------------------------------------------

		// Clear Fw WoWLAN event.
		rtw_write8(Adapter, REG_MCUTST_WOWLAN, 0x0);

#if (USE_SPECIFIC_FW_TO_SUPPORT_WOWLAN == 1)
		SetFwRelatedForWoWLAN8812A(Adapter, _TRUE);
#endif

		SetWoWLANCAMEntry8812(Adapter);
		PlatformEFIOWrite1Byte(Adapter, REG_WKFMCAM_NUM_8812, pPSC->WoLPatternNum);
		RT_TRACE(COMP_INIT, DBG_LOUD, ("Number of WoL pattern filled in CAM = %d\n", pPSC->WoLPatternNum));

		// Dynamically adjust Tx packet boundary for download reserved page packet.
		rtStatus = DynamicRQPN8812AE(Adapter, 0xE0, 0x3, 0x80c20d0d); // reserve 30 pages for rsvd page
		if(rtStatus == RT_STATUS_SUCCESS)
		{
			pHalData->bReInitLLTTable = _TRUE;
		}

		//--------------------------------------------------------
		//3 <2> Set Fw releted H2C cmd.
		//--------------------------------------------------------

		// Set WoWLAN related security information.
		SetFwGlobalInfoCmd_8812(Adapter);

		HalDownloadRSVDPage8812E(Adapter, _TRUE); 	


		// Just enable AOAC related functions when we connect to AP.
		if(pMgntInfo->mAssoc && pMgntInfo->OpMode == RT_OP_MODE_INFRASTRUCTURE)
		{
			// Set Join Bss Rpt H2C cmd and download RSVD page.
			// Fw will check media status to send null packet and perform WoWLAN LPS.
			Adapter->HalFunc.SetHwRegHandler(Adapter, HW_VAR_AID, 0);			
			MacIdIndicateSpecificMediaStatus(Adapter, MAC_ID_STATIC_FOR_DEFAULT_PORT, RT_MEDIA_CONNECT);

			HalSetFWWoWlanMode8812(Adapter, _TRUE);
			// Enable Fw Keep alive mechanism.
			HalSetFwKeepAliveCmd8812(Adapter, _TRUE);
			
			//Enable disconnect decision control.
			SetFwDisconnectDecisionCtrlCmd_8812(Adapter, _TRUE);
		}

		//--------------------------------------------------------
		//3 <3> Hw Configutations
		//--------------------------------------------------------

		//Wait untill Rx DMA Finished before host sleep. FW Pause Rx DMA may happens when received packet doing dma.  //YJ,add,111101
		 rtw_write8(Adapter, REG_RXDMA_CONTROL_8812, BIT2);
		 
		u1bTmp = rtw_read8(Adapter, REG_RXDMA_CONTROL_8812);
		count=0;
		while(!(u1bTmp & BIT(1)) &&(count++ < 100))
		{
			rtw_udelay_os(10); // 10 us
			u1bTmp = rtw_read8(Adapter, REG_RXDMA_CONTROL_8812);
		}
		DBG_871X("HaltAdapter8812E(): 222 Wait untill Rx DMA Finished before host sleep. count=%d\n", count);

		rtw_write8(Adapter, REG_APS_FSMCO+1, 0x0);

		PlatformClearPciPMEStatus(Adapter);

		// tynli_test for normal chip wowlan. 2010.01.26. Suggested by Sd1 Isaac and designer Alfred.
		rtw_write8(Adapter, REG_SYS_CLKR, (rtw_read8(Adapter, REG_SYS_CLKR)|BIT3));

		//prevent 8051 to be reset by PERST#
		rtw_write8(Adapter, REG_RSV_CTRL, 0x20);
		rtw_write8(Adapter, REG_RSV_CTRL, 0x60);
	}

	// For wowlan+LPS+32k.
	if(bSupportRemoteWakeUp && Adapter->bEnterPnpSleep) 
	{
		BOOLEAN		bEnterFwLPS = TRUE;
		u1Byte		QueueID;
		PRT_TCB 	pTcb;

		
		// Set the WoWLAN related function control enable. 
		// It should be the last H2C cmd in the WoWLAN flow. 2012.02.10. by tynli.
		SetFwRemoteWakeCtrlCmd_8723B(Adapter, 1);

		// Stop Pcie Interface Tx DMA.
		PlatformEFIOWrite1Byte(Adapter, REG_PCIE_CTRL_REG_8723B+1, 0xff);
		RT_TRACE(COMP_POWER, DBG_LOUD, ("Stop PCIE Tx DMA.\n"));
		
		// Wait for TxDMA idle.
		count = 0;
		do
		{
			u1bTmp = PlatformEFIORead1Byte(Adapter, REG_PCIE_CTRL_REG_8723B);
			PlatformSleepUs(10); // 10 us
			count++;
		}while((u1bTmp != 0) && (count < 100));
		RT_TRACE(COMP_INIT, DBG_LOUD, ("HaltAdapter8188EE(): Wait untill Tx DMA Finished before host sleep. count=%d\n", count));

		// Set Fw to enter LPS mode.		
		if(	pMgntInfo->mAssoc && 
			pMgntInfo->OpMode == RT_OP_MODE_INFRASTRUCTURE &&
			(pPSC->WoWLANLPSLevel > 0))
		{
			RT_TRACE(COMP_POWER, DBG_LOUD, ("FwLPS: Sleep!!\n"));	
			Adapter->HalFunc.SetHwRegHandler(Adapter, HW_VAR_FW_LPS_ACTION, (pu1Byte)(&bEnterFwLPS));
			pPSC->bEnterLPSDuringSuspend = TRUE;
		}
		
		PlatformAcquireSpinLock(Adapter, RT_TX_SPINLOCK);
		// guangan, 2009/08/28, return TCB in busy queue to idle queue when resume from S3/S4.
		for(QueueID = 0; QueueID < MAX_TX_QUEUE; QueueID++)
		{
			// 2004.08.11, revised by rcnjko.
			while(!RTIsListEmpty(&Adapter->TcbBusyQueue[QueueID]))
			{
				pTcb=(PRT_TCB)RTRemoveHeadList(&Adapter->TcbBusyQueue[QueueID]);
				ReturnTCB(Adapter, pTcb, RT_STATUS_SUCCESS);
			}
		}

		PlatformReleaseSpinLock(Adapter, RT_TX_SPINLOCK);

		if(pHalData->HwROFEnable)
		{
			u1bTmp = PlatformEFIORead1Byte(Adapter, REG_HSISR_8812+3);
			PlatformEFIOWrite1Byte(Adapter, REG_HSISR_8812+3, u1bTmp|BIT1);
		}	
	}
#endif

	DBG_871X("%s() <====\n",__FUNCTION__);

	Adapter->bHaltInProgress = _FALSE;

_func_exit_;

	return _SUCCESS;
}

void SetHwReg8192EE(PADAPTER Adapter, u8 variable, u8* val)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	//struct dm_priv	*pdmpriv = &pHalData->dmpriv;
	//DM_ODM_T 		*podmpriv = &pHalData->odmpriv;

#if defined(CONFIG_WOWLAN) || defined(CONFIG_AP_WOWLAN)
	struct wowlan_ioctl_param *poidparam;
	struct recv_buf *precvbuf;
	struct pwrctrl_priv *pwrctl = adapter_to_pwrctl(Adapter);
	int res, i;
	u32 tmp;
	u16 len = 0;
	u8 mstatus = (*(u8 *)val);
	u8 trycnt = 100;
	u8 data[4];
#endif

_func_enter_;

	switch(variable)
	{
		case HW_VAR_SET_RPWM:
#ifdef CONFIG_LPS_LCLK
			{
				u8	ps_state = *((u8 *)val);
				//rpwm value only use BIT0(clock bit) ,BIT6(Ack bit), and BIT7(Toggle bit) for 88e.
				//BIT0 value - 1: 32k, 0:40MHz.
				//BIT6 value - 1: report cpwm value after success set, 0:do not report.
				//BIT7 value - Toggle bit change.
				//modify by Thomas. 2012/4/2.
				ps_state = ps_state & 0xC1;
				//DBG_871X("##### Change RPWM value to = %x for switch clk #####\n",ps_state);
				rtw_write8(Adapter, REG_PCIE_HRPWM, ps_state);
			}
#endif
#ifdef CONFIG_AP_WOWLAN
			if (pwrctl->wowlan_ap_mode == _TRUE)
			{
				u8	ps_state = *((u8 *)val);
				DBG_871X("%s, RPWM\n", __func__);
				//rpwm value only use BIT0(clock bit) ,BIT6(Ack bit), and BIT7(Toggle bit) for 88e.
				//BIT0 value - 1: 32k, 0:40MHz.
				//BIT6 value - 1: report cpwm value after success set, 0:do not report.
				//BIT7 value - Toggle bit change.
				//modify by Thomas. 2012/4/2.
				ps_state = ps_state & 0xC1;
				//DBG_871X("##### Change RPWM value to = %x for switch clk #####\n",ps_state);
				rtw_write8(Adapter, REG_USB_HRPWM, ps_state);
			}
#endif
			break;
		case HW_VAR_PCIE_STOP_TX_DMA:
			rtw_write16(Adapter, REG_PCIE_CTRL_REG, 0xff00);
			break;
			
		case HW_VAR_APFM_ON_MAC:
			pHalData->bMacPwrCtrlOn = *val;
			DBG_871X("%s: bMacPwrCtrlOn=%d\n", __func__, pHalData->bMacPwrCtrlOn);
			break;

		case HW_VAR_NAV_UPPER:
			{
				u32 usNavUpper = *((u32*)val);

				if (usNavUpper > HAL_NAV_UPPER_UNIT_8192E * 0xFF)
				{
					RT_TRACE(_module_hal_init_c_, _drv_notice_, ("The setting value (0x%08X us) of NAV_UPPER is larger than (%d * 0xFF)!!!\n", usNavUpper, HAL_NAV_UPPER_UNIT_8192E));
					break;
				}

				// The value of ((usNavUpper + HAL_NAV_UPPER_UNIT_8192E - 1) / HAL_NAV_UPPER_UNIT_8192E)
				// is getting the upper integer.
				usNavUpper = (usNavUpper + HAL_NAV_UPPER_UNIT_8192E - 1) / HAL_NAV_UPPER_UNIT_8192E;
				rtw_write8(Adapter, REG_NAV_UPPER, (u8)usNavUpper);
			}
			break;
			
#ifdef CONFIG_WOWLAN_OLD
		case HW_VAR_WOWLAN:
			{
			poidparam = (struct wowlan_ioctl_param *)val;
			switch (poidparam->subcode){
				case WOWLAN_ENABLE:
					DBG_871X_LEVEL(_drv_always_, "WOWLAN_ENABLE\n");

					SetFwRelatedForWoWLAN8192E(Adapter, _TRUE);

					//Set Pattern
					//if(pwrctl->wowlan_pattern==_TRUE)
					//	rtw_wowlan_reload_pattern(Adapter);

					//RX DMA stop
					DBG_871X_LEVEL(_drv_always_, "Pause DMA\n");
					rtw_write32(Adapter,REG_RXPKT_NUM,(rtw_read32(Adapter,REG_RXPKT_NUM)|RW_RELEASE_EN));
					do{
						if((rtw_read32(Adapter, REG_RXPKT_NUM)&RXDMA_IDLE)) {
							DBG_871X_LEVEL(_drv_always_, "RX_DMA_IDLE is true\n");
							if (padapter->intf_stop)
								padapter->intf_stop(Adapter);
							break;
						} else {
							// If RX_DMA is not idle, receive one pkt from DMA
							DBG_871X_LEVEL(_drv_always_, "RX_DMA_IDLE is not true\n");
						}
					}while(trycnt--);
					if(trycnt ==0)
						DBG_871X_LEVEL(_drv_always_, "Stop RX DMA failed...... \n");

					//Set WOWLAN H2C command.
					DBG_871X_LEVEL(_drv_always_, "Set WOWLan cmd\n");
					rtl8192e_set_wowlan_cmd(Adapter, 1);

					mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
					trycnt = 10;

					while(!(mstatus&BIT1) && trycnt>1) {
						mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
						DBG_871X_LEVEL(_drv_always_, "Loop index: %d :0x%02x\n", trycnt, mstatus);
						trycnt --;
						rtw_msleep_os(2);
					}

					pwrctl->wowlan_wake_reason = rtw_read8(Adapter, REG_WOWLAN_WAKE_REASON);
					DBG_871X_LEVEL(_drv_always_, "wowlan_wake_reason: 0x%02x\n",
										pwrctl->wowlan_wake_reason);

					// Invoid SE0 reset signal during suspending
					rtw_write8(Adapter, REG_RSV_CTRL, 0x20);
					rtw_write8(Adapter, REG_RSV_CTRL, 0x60);

					//rtw_msleep_os(10);
					break;
				case WOWLAN_DISABLE:
					DBG_871X_LEVEL(_drv_always_, "WOWLAN_DISABLE\n");
					trycnt = 10;
					// 1. Read wakeup reason
					pwrctl->wowlan_wake_reason = rtw_read8(Adapter, REG_WOWLAN_WAKE_REASON);
					DBG_871X_LEVEL(_drv_always_, "wakeup_reason: 0x%02x, mac_630=0x%08x, mac_634=0x%08x, mac_1c0=0x%08x, mac_1c4=0x%08x"
					", mac_494=0x%08x, , mac_498=0x%08x, mac_49c=0x%08x, mac_608=0x%08x, mac_4a0=0x%08x, mac_4a4=0x%08x\n"
					", mac_1cc=0x%08x, mac_2f0=0x%08x, mac_2f4=0x%08x, mac_2f8=0x%08x, mac_2fc=0x%08x, mac_8c=0x%08x"
					, pwrctl->wowlan_wake_reason, rtw_read32(Adapter, 0x630), rtw_read32(Adapter, 0x634)
					, rtw_read32(Adapter, 0x1c0), rtw_read32(Adapter, 0x1c4)
					, rtw_read32(Adapter, 0x494), rtw_read32(Adapter, 0x498), rtw_read32(Adapter, 0x49c), rtw_read32(Adapter, 0x608)
					, rtw_read32(Adapter, 0x4a0), rtw_read32(Adapter, 0x4a4)
					, rtw_read32(Adapter, 0x1cc), rtw_read32(Adapter, 0x2f0), rtw_read32(Adapter, 0x2f4), rtw_read32(Adapter, 0x2f8)
					, rtw_read32(Adapter, 0x2fc), rtw_read32(Adapter, 0x8c));

					rtl8192e_set_wowlan_cmd(Adapter, 0);
					mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
					DBG_871X_LEVEL(_drv_info_, "%s mstatus:0x%02x\n", __func__, mstatus);

					while(mstatus&BIT1 && trycnt>1) {
						mstatus = rtw_read8(Adapter, REG_WOW_CTRL);
						DBG_871X_LEVEL(_drv_always_, "Loop index: %d :0x%02x\n", trycnt, mstatus);
						trycnt --;
						rtw_msleep_os(2);
					}

					if (mstatus & BIT1)
						printk("System did not release RX_DMA\n");
					else
						SetFwRelatedForWoWLAN8192E(Adapter, _FALSE);

					rtw_msleep_os(2);
					if(!(pwrctl->wowlan_wake_reason & FWDecisionDisconnect))
						rtl8192e_set_FwJoinBssReport_cmd(Adapter, 1);
					//rtw_msleep_os(10);
					break;
				default:
					break;
			}
		}
			break;
#endif				

#ifdef CONFIG_AP_WOWLAN
		case HW_VAR_AP_WOWLAN:
		{
			poidparam = (struct wowlan_ioctl_param *)val;
			switch (poidparam->subcode) {
			case WOWLAN_AP_ENABLE:
				DBG_871X("%s, WOWLAN_AP_ENABLE\n", __func__);
				// 1. Download WOWLAN FW
				DBG_871X_LEVEL(_drv_always_, "Re-download WoWlan FW!\n");
#ifdef DBG_CHECK_FW_PS_STATE
				if(rtw_fw_ps_state(Adapter) == _FAIL) {
					pdbgpriv->dbg_enwow_dload_fw_fail_cnt++;
					DBG_871X_LEVEL(_drv_always_, "wowlan enable no leave 32k\n");
				}
#endif //DBG_CHECK_FW_PS_STATE
				do {
					if (rtw_read8(Adapter, REG_HMETFR) == 0x00) {
						DBG_871X_LEVEL(_drv_always_, "Ready to change FW.\n");
						break;
					}
					rtw_msleep_os(10);
					DBG_871X_LEVEL(_drv_always_, "trycnt: %d\n", (100-trycnt));
				} while (trycnt--);

				SetFwRelatedForWoWLAN8192E(Adapter, _TRUE);

				// 2. RX DMA stop
				DBG_871X_LEVEL(_drv_always_, "Pause DMA\n");
				trycnt = 100;
				rtw_write32(Adapter,REG_RXPKT_NUM,
					(rtw_read32(Adapter,REG_RXPKT_NUM)|RW_RELEASE_EN));
				do {
					if ((rtw_read32(Adapter, REG_RXPKT_NUM)&RXDMA_IDLE)) {
						DBG_871X_LEVEL(_drv_always_, "RX_DMA_IDLE is true\n");
						if (Adapter->intf_stop)
							Adapter->intf_stop(Adapter);
						break;
					} else {
						// If RX_DMA is not idle, receive one pkt from DMA
						DBG_871X_LEVEL(_drv_always_, "RX_DMA_IDLE is not true\n");
					}
				} while (trycnt--);

				if (trycnt == 0)
					DBG_871X_LEVEL(_drv_always_, "Stop RX DMA failed...... \n");

				// 5. Set Enable WOWLAN H2C command.
				DBG_871X_LEVEL(_drv_always_, "Set Enable AP WOWLan cmd\n");
				rtl8192e_set_ap_wowlan_cmd(Adapter, 1);
				// 6. add some delay for H2C cmd ready
				rtw_msleep_os(10);
				// 7. enable AP power save
				rtl8192e_set_ap_ps_wowlan_cmd(Adapter, 1);

				rtw_write8(Adapter, REG_WOWLAN_WAKE_REASON, 0);

				// Invoid SE0 reset signal during suspending
				rtw_write8(Adapter, REG_RSV_CTRL, 0x20);
				rtw_write8(Adapter, REG_RSV_CTRL, 0x60);
				break;
			case WOWLAN_AP_DISABLE:
				DBG_871X("%s, WOWLAN_AP_DISABLE\n", __func__);
				// 1. Read wakeup reason
				pwrctl->wowlan_wake_reason =
					rtw_read8(Adapter, REG_WOWLAN_WAKE_REASON);

				DBG_871X_LEVEL(_drv_always_, "wakeup_reason: 0x%02x\n",
						pwrctl->wowlan_wake_reason);

				// 2. diable AP power save
				rtl8192e_set_ap_ps_wowlan_cmd(Adapter, 0);
				// 3.  Set Disable WOWLAN H2C command.
				DBG_871X_LEVEL(_drv_always_, "Set Disable WOWLan cmd\n");
				rtl8192e_set_ap_wowlan_cmd(Adapter, 0);
				// 6. add some delay for H2C cmd ready
				rtw_msleep_os(2);
#ifdef DBG_CHECK_FW_PS_STATE
				if (rtw_fw_ps_state(Adapter) == _FAIL) {
					pdbgpriv->dbg_diswow_dload_fw_fail_cnt++;
					DBG_871X_LEVEL(_drv_always_, "wowlan enable no leave 32k\n");
				}
#endif //DBG_CHECK_FW_PS_STATE

				DBG_871X_LEVEL(_drv_always_, "Release RXDMA\n");

				rtw_write32(Adapter, REG_RXPKT_NUM,
					(rtw_read32(Adapter,REG_RXPKT_NUM) & (~RW_RELEASE_EN)));

				do {
					if (rtw_read8(Adapter, REG_HMETFR) == 0x00) {
						DBG_871X_LEVEL(_drv_always_, "Ready to change FW.\n");
						break;
					}
					rtw_msleep_os(10);
					DBG_871X_LEVEL(_drv_always_, "trycnt: %d\n", (100-trycnt));
				} while (trycnt--);

				SetFwRelatedForWoWLAN8192E(Adapter, _FALSE);
#ifdef CONFIG_GPIO_WAKEUP
				DBG_871X_LEVEL(_drv_always_, "Set Wake GPIO to high for default.\n");
				HalSetOutPutGPIO(Adapter, WAKEUP_GPIO_IDX, 1);
#endif

#ifdef CONFIG_CONCURRENT_MODE
				if (rtw_buddy_adapter_up(Adapter) == _TRUE &&
					check_buddy_fwstate(Adapter, WIFI_AP_STATE) == _TRUE) {
					rtl8192e_set_FwJoinBssReport_cmd(Adapter->pbuddy_adapter, RT_MEDIA_CONNECT);
					issue_beacon(Adapter->pbuddy_adapter, 0);
				} else {
					rtl8192e_set_FwJoinBssReport_cmd(Adapter, RT_MEDIA_CONNECT);
					issue_beacon(Adapter, 0);
				}
#else
				rtl8192e_set_FwJoinBssReport_cmd(Adapter, RT_MEDIA_CONNECT);
				issue_beacon(Adapter, 0);
#endif

				break;
			default:
				break;
			}
		}
			break;
#endif //CONFIG_AP_WOWLAN

		default:
			SetHwReg8192E(Adapter, variable, val);
			break;
	}

_func_exit_;
}

void GetHwReg8192EE(PADAPTER Adapter, u8 variable, u8* val)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	//DM_ODM_T 		*podmpriv = &pHalData->odmpriv;
_func_enter_;

	switch(variable)
	{
		case HW_VAR_APFM_ON_MAC:
			*val = pHalData->bMacPwrCtrlOn;
			break;
			
		default:
			GetHwReg8192E(Adapter,variable,val);
			break;
	}

_func_exit_;
}

//
//	Description:
//		Change default setting of specified variable.
//
u8
SetHalDefVar8192EE(
	IN	PADAPTER				Adapter,
	IN	HAL_DEF_VARIABLE		eVariable,
	IN	PVOID					pValue
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	u8			bResult = _SUCCESS;

	switch(eVariable)
	{
		case HAL_DEF_PCI_SUUPORT_L1_BACKDOOR:
			pHalData->bSupportBackDoor = *((PBOOLEAN)pValue);
			break;
		default:
			SetHalDefVar8192E(Adapter,eVariable,pValue);
			break;
	}

	return bResult;
}

//
//	Description: 
//		Query setting of specified variable.
//
u8
GetHalDefVar8192EE(
	IN	PADAPTER				Adapter,
	IN	HAL_DEF_VARIABLE		eVariable,
	IN	PVOID					pValue
	)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(Adapter);
	u8			bResult = _SUCCESS;

	switch(eVariable)
	{
		case HW_VAR_MAX_RX_AMPDU_FACTOR:
			*(( u32*)pValue) = MAX_AMPDU_FACTOR_64K; //MAX_AMPDU_FACTOR_64K;
			break;

		case HAL_DEF_PCI_SUUPORT_L1_BACKDOOR:		
			*((PBOOLEAN)pValue) = pHalData->bSupportBackDoor;		
			break;

		case HAL_DEF_PCI_AMD_L1_SUPPORT:
			*((PBOOLEAN)pValue) = _TRUE;// Support L1 patch on AMD platform in default, added by Roger, 2012.04.30.
			break;

		default:
			GetHalDefVar8192E(Adapter,eVariable,pValue);
			break;
	}

	return bResult;
}

static void rtl8192ee_init_default_value(_adapter * padapter)
{
	HAL_DATA_TYPE	*pHalData = GET_HAL_DATA(padapter);


	rtl8192e_init_default_value(padapter);

	pHalData->CurrentWirelessMode = WIRELESS_MODE_AUTO;
	pHalData->bDefaultAntenna = 1;

	//
	// Set TCR-Transmit Control Register. The value is set in InitializeAdapter8190Pci()
	//
//TODO_8192ee
#if 0//original
	pHalData->TransmitConfig = CFENDFORM | BIT12 | BIT13;
#else
	pHalData->TransmitConfig = CFENDFORM | BIT15;
#endif	
//TODO_8192ee	

	//
	// Set RCR-Receive Control Register . The value is set in InitializeAdapter8190Pci().
	//
//TODO_8192ee
/*
	pHalData->ReceiveConfig = (\
								RCR_APPFCS			|
								RCR_APP_MIC			|
								RCR_APP_ICV			|
								RCR_APP_PHYST_RXFF	|   
								//RCR_NONQOS_VHT	|
								RCR_HTC_LOC_CTRL	|
								RCR_AMF				|
								RCR_ACF				|	
								RCR_ADF				|	// Note: This bit controls the PS-Poll packet filter.
								RCR_AICV			|
								RCR_ACRC32			|
								RCR_AB				|
								RCR_AM				|
								RCR_APM				|
								0);
*/								
//TODO_8192ee. original

	pHalData->ReceiveConfig = (\
								/* RCR_APPFCS			| */
								RCR_APP_MIC			|
								RCR_APP_ICV			|
								RCR_APP_PHYST_RXFF	|
								RCR_NONQOS_VHT	|
								RCR_HTC_LOC_CTRL	|
								RCR_AMF				|
								RCR_CBSSID_DATA			|
								RCR_CBSSID_BCN			|
								/* RCR_ACF				| */
								/* RCR_ADF				|	// Note: This bit controls the PS-Poll packet filter. */
								RCR_AB				|
								RCR_AM				|
								RCR_APM				|
								0);

#if (1 == RTL8192E_RX_PACKET_INCLUDE_CRC)
	pHalData->ReceiveConfig |= ACRC32;
#endif

/* TODO_8192ee */

	//
	// Set Interrupt Mask Register
	//
	pHalData->IntrMaskDefault[0]	= (u32)(			\
								IMR_PSTIMEOUT_8192E|
								//IMR_GTINT4_8812			|
								IMR_GTINT3_8192E			|
								IMR_TXBCN0ERR_8192E		|
								IMR_TXBCN0OK_8192E		|
								IMR_BCNDMAINT0_8192E	|
								IMR_HSISR_IND_ON_INT_8192E	|
								IMR_C2HCMD_8192E		|
								//IMR_CPWM_8192E			|
								IMR_HIGHDOK_8192E		|
								IMR_MGNTDOK_8192E		|
								IMR_BKDOK_8192E			|
								IMR_BEDOK_8192E			|
								IMR_VIDOK_8192E			|
								IMR_VODOK_8192E			|
								IMR_RDU_8192E			|
								IMR_ROK_8192E			|
								0);
	pHalData->IntrMaskDefault[1] 	= (u32)(\
								IMR_RXFOVW_8192E		|
								IMR_TXFOVW_8192E		|
								0);

	// 2012/03/27 hpfan Add for win8 DTM DPC ISR test
	pHalData->IntrMaskReg[0]	=	(u32)(	\
								IMR_RDU_8192E		|
								IMR_PSTIMEOUT_8192E	|
								0);
	pHalData->IntrMaskReg[1]	=	(u32)(	\
								IMR_C2HCMD_8192E		|
								0);

	//if (padapter->bUseThreadHandleInterrupt)
	//{
	//	pHalData->IntrMask[0] =pHalData->IntrMaskReg[0];
	//	pHalData->IntrMask[1] =pHalData->IntrMaskReg[1];
	//}
	//else
	{
		pHalData->IntrMask[0]= pHalData->IntrMaskDefault[0];
		pHalData->IntrMask[1]= pHalData->IntrMaskDefault[1];
	}

	if(IS_HARDWARE_TYPE_8812E(padapter))
	{
		pHalData->SysIntrMask[0] = (u32)(			\
								HSIMR_PDN_INT_EN		|
								HSIMR_RON_INT_EN		|
								0);
	}
	else
	{
		pHalData->SysIntrMask[0] = (u32)(			\
								HSIMR_GPIO12_0_INT_EN			|
								HSIMR_GPIO9_INT_EN				|
								0);
	}
}

void rtl8192ee_set_hal_ops(_adapter * padapter)
{
	struct hal_ops	*pHalFunc = &padapter->HalFunc;

_func_enter_;

	pHalFunc->hal_power_on = _InitPowerOn_8192EE;
	pHalFunc->hal_power_off = hal_poweroff_8192ee;
	pHalFunc->hal_init = &rtl8192ee_hal_init;
	pHalFunc->hal_deinit = &rtl8192ee_hal_deinit;

	pHalFunc->inirp_init = &rtl8192ee_init_desc_ring;
	pHalFunc->inirp_deinit = &rtl8192ee_free_desc_ring;
	pHalFunc->irp_reset = &rtl8192ee_reset_desc_ring;

	pHalFunc->init_xmit_priv = &rtl8192ee_init_xmit_priv;
	pHalFunc->free_xmit_priv = &rtl8192ee_free_xmit_priv;

	pHalFunc->init_recv_priv = &rtl8192ee_init_recv_priv;
	pHalFunc->free_recv_priv = &rtl8192ee_free_recv_priv;
#ifdef CONFIG_SW_LED
	pHalFunc->InitSwLeds = &rtl8192ee_InitSwLeds;
	pHalFunc->DeInitSwLeds = &rtl8192ee_DeInitSwLeds;
#else //case of hw led or no led
	pHalFunc->InitSwLeds = NULL;
	pHalFunc->DeInitSwLeds = NULL;	
#endif//CONFIG_SW_LED
	
	pHalFunc->init_default_value = &rtl8192ee_init_default_value;
	pHalFunc->intf_chip_configure = &rtl8192ee_interface_configure;
	pHalFunc->read_adapter_info = &ReadAdapterInfo8192EE;

	pHalFunc->enable_interrupt = &EnableInterrupt8192EE;
	pHalFunc->disable_interrupt = &DisableInterrupt8192EE;
	pHalFunc->interrupt_handler = &rtl8192ee_interrupt;

	pHalFunc->SetHwRegHandler = &SetHwReg8192EE;
	pHalFunc->GetHwRegHandler = &GetHwReg8192EE;
  	pHalFunc->GetHalDefVarHandler = &GetHalDefVar8192EE;
 	pHalFunc->SetHalDefVarHandler = &SetHalDefVar8192EE;

	pHalFunc->hal_xmit = &rtl8192ee_hal_xmit;
	pHalFunc->mgnt_xmit = &rtl8192ee_mgnt_xmit;
	pHalFunc->hal_xmitframe_enqueue = &rtl8192ee_hal_xmitframe_enqueue;

#ifdef CONFIG_HOSTAPD_MLME
	pHalFunc->hostap_mgnt_xmit_entry = &rtl8192ee_hostap_mgnt_xmit_entry;
#endif

#ifdef CONFIG_XMIT_THREAD_MODE
	pHalFunc->xmit_thread_handler = &rtl8192ee_xmit_buf_handler;
#endif
	rtl8192e_set_hal_ops(pHalFunc);
_func_exit_;

}


