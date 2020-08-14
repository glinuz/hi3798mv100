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
#define _HCI_INTF_C_

#include <drv_types.h>
#include <linux/pci_regs.h>

#ifndef CONFIG_PCI_HCI

#error "CONFIG_PCI_HCI shall be on!\n"

#endif


#if defined (PLATFORM_LINUX) && defined (PLATFORM_WINDOWS)

#error "Shall be Linux or Windows, but not both!\n"

#endif

#ifdef CONFIG_80211N_HT
extern int rtw_ht_enable;
extern int rtw_bw_mode;
extern int rtw_ampdu_enable;//for enable tx_ampdu
#endif

#ifdef CONFIG_GLOBAL_UI_PID
int ui_pid[3] = {0, 0, 0};
#endif

extern int pm_netdev_open(struct net_device *pnetdev,u8 bnormal);
int rtw_resume_process(_adapter *padapter);

#ifdef CONFIG_PM
static int rtw_pci_suspend(struct pci_dev *pdev, pm_message_t state);
static int rtw_pci_resume(struct pci_dev *pdev);
#endif

static int rtw_drv_init(struct pci_dev *pdev, const struct pci_device_id *pdid);
static void rtw_dev_remove(struct pci_dev *pdev);

static struct specific_device_id specific_device_id_tbl[] = {
	{.idVendor=0x0b05, .idProduct=0x1791, .flags=SPEC_DEV_ID_DISABLE_HT},
	{.idVendor=0x13D3, .idProduct=0x3311, .flags=SPEC_DEV_ID_DISABLE_HT},
	{}
};

struct pci_device_id rtw_pci_id_tbl[] = {
#ifdef CONFIG_RTL8188E
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8179),.driver_data = RTL8188E},
#endif
#ifdef CONFIG_RTL8192C
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8178),.driver_data = RTL8188C_8192C},
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8177),.driver_data = RTL8188C_8192C},
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8176),.driver_data = RTL8188C_8192C},
#endif
#ifdef CONFIG_RTL8192D
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8193),.driver_data = RTL8192D},
#endif
#ifdef CONFIG_RTL8812A
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8812),.driver_data = RTL8812},
#endif
#ifdef CONFIG_RTL8821A
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x8821),.driver_data = RTL8821},
#endif
#ifdef CONFIG_RTL8192E
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0x818B),.driver_data = RTL8192E},
#endif
#ifdef CONFIG_RTL8723B
	{PCI_DEVICE(PCI_VENDER_ID_REALTEK, 0xb723),.driver_data = RTL8723B},
#endif
	{},
};

struct pci_drv_priv {
	struct pci_driver rtw_pci_drv;
	int drv_registered;
};


static struct pci_drv_priv pci_drvpriv = {
	.rtw_pci_drv.name = (char*)DRV_NAME,
	.rtw_pci_drv.probe = rtw_drv_init,
	.rtw_pci_drv.remove = rtw_dev_remove,
	.rtw_pci_drv.id_table = rtw_pci_id_tbl,
#ifdef CONFIG_PM	
	.rtw_pci_drv.suspend = rtw_pci_suspend,
	.rtw_pci_drv.resume = rtw_pci_resume,
#endif
};


MODULE_DEVICE_TABLE(pci, rtw_pci_id_tbl);


static u16 pcibridge_vendors[PCI_BRIDGE_VENDOR_MAX] = {
	INTEL_VENDOR_ID,
	ATI_VENDOR_ID,
	AMD_VENDOR_ID,
	SIS_VENDOR_ID
};

#define PCI_PM_CAP_ID		0x01	// The Capability ID for PME function
void	PlatformClearPciPMEStatus(PADAPTER Adapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct pci_dev 	*pdev = pdvobjpriv->ppcidev;
	BOOLEAN		PCIClkReq = _FALSE;
	u8	CapId=0xff;
	u8	CapPointer=0;
	//u16	CapHdr;
	RT_PCI_CAPABILITIES_HEADER CapHdr;
	u8	PMCSReg;
	int	result;

	// Get the Capability pointer first, 
	// the Capability Pointer is located at offset 0x34 from the Function Header

	result = pci_read_config_byte(pdev, 0x34, &CapPointer);
	if (result != 0) {
		DBG_871X("%s() pci_read_config_byte 0x34 Failed!\n",__FUNCTION__);
	} else {
		DBG_871X("PlatformClearPciPMEStatus(): PCI configration 0x34 = 0x%2x\n", CapPointer);
		do {
			//end of pci capability
			if (CapPointer == 0x00) {
				CapId = 0xff;
				break;
			}

			//result = pci_read_config_word(pdev, CapPointer, &CapHdr);
			result = pci_read_config_byte(pdev, CapPointer, &CapHdr.CapabilityID);
			if (result != 0) {
				DBG_871X("%s() pci_read_config_byte %x Failed!\n",__FUNCTION__,CapPointer);
				CapId = 0xff;
				break;
			}

			result = pci_read_config_byte(pdev, CapPointer + 1, &CapHdr.Next);
			if (result != 0) {
				DBG_871X("%s() pci_read_config_byte %x Failed!\n",__FUNCTION__,CapPointer);
				CapId = 0xff;
				break;
			}

			//CapId = CapHdr & 0xFF;
			CapId = CapHdr.CapabilityID;

			DBG_871X("PlatformClearPciPMEStatus(): in pci configration1, CapPointer%x = %x\n", CapPointer, CapId);

			if(CapId == PCI_PM_CAP_ID) {
				break;
			} else {
				// point to next Capability
				//CapPointer = (CapHdr >> 8) & 0xFF;
				CapPointer = CapHdr.Next;
			}
		}while(_TRUE);
		
		if (CapId == PCI_PM_CAP_ID) {
			// Get the PM CSR (Control/Status Register), 
			// The PME_Status is located at PM Capatibility offset 5, bit 7
			result = pci_read_config_byte(pdev, CapPointer+5, &PMCSReg);
			if (PMCSReg & BIT7) {
				// PME event occured, clear the PM_Status by write 1
				PMCSReg = PMCSReg | BIT7;

				pci_write_config_byte(pdev, CapPointer+5, PMCSReg);
				PCIClkReq = _TRUE;
				// Read it back to check
				pci_read_config_byte(pdev, CapPointer+5, &PMCSReg);
				DBG_871X("PlatformClearPciPMEStatus(): Clear PME status 0x%2x to 0x%2x\n", CapPointer+5, PMCSReg);
			} else {
				DBG_871X("PlatformClearPciPMEStatus(): PME status(0x%2x) = 0x%2x\n", CapPointer+5, PMCSReg);
			}
		} else {
			DBG_871X("PlatformClearPciPMEStatus(): Cannot find PME Capability\n");
		}
	}

	DBG_871X("PME, value_offset = %x, PME EN = %x\n", CapPointer+5, PCIClkReq);
}

static u8 rtw_pci_platform_switch_device_pci_aspm(_adapter *padapter, u8 value)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);
	BOOLEAN		bResult = _FALSE;
	int	Result = 0;
	int	error;

	Result = pci_write_config_byte(pdvobjpriv->ppcidev, pcipriv->pciehdr_offset+0x10, value);	// enable I/O space
	DBG_871X("PlatformSwitchDevicePciASPM(0x%x) = 0x%x\n", pcipriv->pciehdr_offset+0x10, value);
	if(Result != 0)
	{
		DBG_871X("PlatformSwitchDevicePciASPM() Failed!\n");
		bResult = _FALSE;
	}
	else 
	{
		bResult = _TRUE;
	}

	return bResult;
}

// 
// When we set 0x01 to enable clk request. Set 0x0 to disable clk req.  
// 
static u8 rtw_pci_switch_clk_req(_adapter *padapter, u8 value)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	u8	buffer, bResult = _FALSE;
	int	error;

	buffer = value;

	if(!padapter->hw_init_completed)
		return bResult;

	// the clock request is located at offset 0x81, suppose the PCIE Capability register is located at offset 0x70
	// the correct code should be: search the PCIE capability register first and then the clock request is located offset 0x11
	error = pci_write_config_byte(pdvobjpriv->ppcidev, 0x81, buffer);
	if(error != 0)
	{
		DBG_871X("rtw_pci_switch_clk_req error (%d)\n",error);
	}
	else
	{
		bResult = _TRUE;
	}

	return bResult;
}

/*Disable RTL8192SE ASPM & Disable Pci Bridge ASPM*/
void rtw_pci_disable_aspm(_adapter *padapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pwrctrl_priv	*pwrpriv = dvobj_to_pwrctl(pdvobjpriv);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	struct pci_dev	*bridge_pdev = pdev->bus->self;
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);
	u8	linkctrl_reg;
	u8	pcibridge_linkctrlreg, aspmlevel = 0;


	// We shall check RF Off Level for ASPM function instead of registry settings, revised by Roger, 2013.03.29.
	if(!(pwrpriv->reg_rfps_level & (RT_RF_LPS_LEVEL_ASPM|RT_RF_PS_LEVEL_ALWAYS_ASPM)) )
		return;

	if(!padapter->hw_init_completed)
		return;

	if (pcipriv->pcibridge_vendor == PCI_BRIDGE_VENDOR_UNKNOWN) {
		RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("%s(): PCI(Bridge) UNKNOWN.\n", __FUNCTION__));
		return;
	}

	linkctrl_reg = pcipriv->linkctrl_reg;
	pcibridge_linkctrlreg = pcipriv->pcibridge_linkctrlreg;

	// Set corresponding value.
	aspmlevel |= BIT(0) | BIT(1);
	linkctrl_reg &=~aspmlevel;
	pcibridge_linkctrlreg &=~aspmlevel;

	//
	// 09/08/21 MH From Sd1 suggestion. we need to adjust ASPM enable sequence
	// CLK_REQ ==> delay 50us ==> Device ==> Host ==> delay 50us 
	//

	if (pwrpriv->reg_rfps_level & RT_RF_OFF_LEVL_CLK_REQ) {
		RT_CLEAR_PS_LEVEL(pwrpriv, RT_RF_OFF_LEVL_CLK_REQ);
		rtw_pci_switch_clk_req(padapter, 0x0);
	}

	{
		/*for promising device will in L0 state after an I/O.*/ 
		u8 tmp_u1b;
		pci_read_config_byte(pdev, (pcipriv->pciehdr_offset+0x10), &tmp_u1b);
	}

	rtw_pci_platform_switch_device_pci_aspm(padapter, linkctrl_reg);

	rtw_udelay_os(50);

	//When there exists anyone's BusNum, DevNum, and FuncNum that are set to 0xff,
	// we do not execute any action and return. Added by tynli.
	if( (pcipriv->busnumber == 0xff && pcipriv->devnumber == 0xff && pcipriv->funcnumber == 0xff) ||
		(pcipriv->pcibridge_busnum == 0xff && pcipriv->pcibridge_devnum == 0xff && pcipriv->pcibridge_funcnum == 0xff) )
	{
		// Do Nothing!!
	}
	else
	{
		//4 //Disable Pci Bridge ASPM 
		pci_write_config_byte(bridge_pdev, (pcipriv->pcibridge_pciehdr_offset+0x10), pcibridge_linkctrlreg);
		DBG_871X("PlatformDisableASPM():PciBridge Write reg[%x] = %x\n",
			(pcipriv->pcibridge_pciehdr_offset+0x10), pcibridge_linkctrlreg);
		rtw_udelay_os(50);
	}

}

/*Enable RTL8192SE ASPM & Enable Pci Bridge ASPM for 
power saving We should follow the sequence to enable 
RTL8192SE first then enable Pci Bridge ASPM
or the system will show bluescreen.*/
void rtw_pci_enable_aspm(_adapter *padapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pwrctrl_priv	*pwrpriv = dvobj_to_pwrctl(pdvobjpriv);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	struct pci_dev	*bridge_pdev = pdev->bus->self;
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);
	u16	aspmlevel = 0;		
	u8	u_pcibridge_aspmsetting = 0;
	u8	u_device_aspmsetting = 0;
	u32	u_device_aspmsupportsetting = 0;


	// We shall check RF Off Level for ASPM function instead of registry settings, revised by Roger, 2013.03.29.
	if(!(pwrpriv->reg_rfps_level & (RT_RF_LPS_LEVEL_ASPM|RT_RF_PS_LEVEL_ALWAYS_ASPM)) )
		return;

	//When there exists anyone's BusNum, DevNum, and FuncNum that are set to 0xff,
	// we do not execute any action and return. Added by tynli. 
	if( (pcipriv->busnumber == 0xff && pcipriv->devnumber == 0xff && pcipriv->funcnumber == 0xff) ||
		(pcipriv->pcibridge_busnum == 0xff && pcipriv->pcibridge_devnum == 0xff && pcipriv->pcibridge_funcnum == 0xff) )
	{
		DBG_871X("rtw_pci_enable_aspm(): Fail to enable ASPM. Cannot find the Bus of PCI(Bridge).\n");
		return;
	}

//Get Bridge ASPM Support
//not to enable bridge aspm if bridge does not support
//Added by sherry 20100803
	//if (IS_HARDWARE_TYPE_8192DE(padapter))
	{
		// Get the Link Capability, it ls located at offset 0x0c from the PCIE Capability
		pci_read_config_dword(bridge_pdev, (pcipriv->pcibridge_pciehdr_offset+0x0C), &u_device_aspmsupportsetting);

		DBG_871X("rtw_pci_enable_aspm(): Bridge ASPM support %x \n",u_device_aspmsupportsetting);
		if(((u_device_aspmsupportsetting & BIT(11)) != BIT(11)) || ((u_device_aspmsupportsetting & BIT(10)) != BIT(10)))
		{
			if(pdvobjpriv->const_devicepci_aspm_setting == 3)
			{
				DBG_871X("rtw_pci_enable_aspm(): Bridge not support L0S or L1\n");
				return;
			}
			else if(pdvobjpriv->const_devicepci_aspm_setting == 2)
			{
				if((u_device_aspmsupportsetting & BIT(11)) != BIT(11))
				{
					DBG_871X("rtw_pci_enable_aspm(): Bridge not support L1 \n");
					return;
				}
			}
			else if(pdvobjpriv->const_devicepci_aspm_setting == 1)
			{
				if((u_device_aspmsupportsetting & BIT(10)) != BIT(10))
				{
					DBG_871X("rtw_pci_enable_aspm(): Bridge not support L0s \n");
					return;
				}

			}
		}
		else
		{
			DBG_871X("rtw_pci_enable_aspm(): Bridge support L0s and L1 \n");
		}
	}

	//
       // Skip following settings if ASPM has already enabled, added by Roger, 2013.03.15.
       //
	if((pcipriv->pcibridge_linkctrlreg & (BIT0|BIT1)) &&
		(pcipriv->linkctrl_reg & (BIT0|BIT1)))
	{// BIT0: L0S, BIT1:L1

		DBG_871X("PlatformEnableASPM(): ASPM is already enabled, skip incoming settings!!\n");
		return;
	}

	//4 Enable Pci Bridge ASPM 
	// Write PCI bridge PCIE-capability Link Control Register
	// Justin: Can we change the ASPM Control register ? 
	// The system BIOS should set this register with a correct value
	// If we change the force enable the ASPM L1/L0s, this may cause the system hang
	u_pcibridge_aspmsetting = pcipriv->pcibridge_linkctrlreg;
	u_pcibridge_aspmsetting |= pdvobjpriv->const_hostpci_aspm_setting;

	if (pcipriv->pcibridge_vendor == PCI_BRIDGE_VENDOR_INTEL ||
		pcipriv->pcibridge_vendor == PCI_BRIDGE_VENDOR_SIS )
		u_pcibridge_aspmsetting &= ~BIT(0); // for intel host 42 device 43

	pci_write_config_byte(bridge_pdev, (pcipriv->pcibridge_pciehdr_offset+0x10), u_pcibridge_aspmsetting);
	DBG_871X("PlatformEnableASPM():PciBridge Write reg[%x] = %x\n",
		(pcipriv->pcibridge_pciehdr_offset+0x10), 
		u_pcibridge_aspmsetting);

	rtw_udelay_os(50);

	/*Get ASPM level (with/without Clock Req)*/ 
	aspmlevel |= pdvobjpriv->const_devicepci_aspm_setting;
	u_device_aspmsetting = pcipriv->linkctrl_reg;
	u_device_aspmsetting |= aspmlevel; // device 43

	rtw_pci_platform_switch_device_pci_aspm(padapter, u_device_aspmsetting);

	if (pwrpriv->reg_rfps_level & RT_RF_OFF_LEVL_CLK_REQ) {
		rtw_pci_switch_clk_req(padapter, (pwrpriv->reg_rfps_level & RT_RF_OFF_LEVL_CLK_REQ) ? 1 : 0);
		RT_SET_PS_LEVEL(pwrpriv, RT_RF_OFF_LEVL_CLK_REQ);
	}

	rtw_udelay_os(50);
}

static u8 rtw_pci_get_amd_l1_patch(struct dvobj_priv *pdvobjpriv, struct pci_dev *pdev)
{
	u8	status = _FALSE;
	u8	offset_e0;
	u32	offset_e4;

	pci_write_config_byte(pdev, 0xE0, 0xA0);
	pci_read_config_byte(pdev, 0xE0, &offset_e0);

	if (offset_e0 == 0xA0) {
		pci_read_config_dword(pdev, 0xE4, &offset_e4);
		if (offset_e4 & BIT(23))
			status = _TRUE;
	}

	return status;
}

static s32	rtw_pci_get_linkcontrol_reg (struct pci_dev *pdev, u8 *LinkCtrlReg, u8 *HdrOffset)
{
	u8 CapabilityPointer;
	RT_PCI_CAPABILITIES_HEADER	CapabilityHdr;
	s32 status = _FAIL;

	//get CapabilityOffset
	pci_read_config_byte(pdev, 0x34, &CapabilityPointer);	// the capability pointer is located offset 0x34
	
	// Loop through the capabilities in search of the power management capability. 
	// The list is NULL-terminated, so the last offset will always be zero.

	while (CapabilityPointer != 0) 
	{
    		// Read the header of the capability at  this offset. If the retrieved capability is not
	  	// the power management capability that we are looking for, follow the link to the 
	  	// next capability and continue looping.

		//4 get CapabilityHdr
		//pci_read_config_word(pdev, CapabilityPointer, (u16 *)&CapabilityHdr);
		pci_read_config_byte(pdev, CapabilityPointer,  (u8 *)&CapabilityHdr.CapabilityID);
		pci_read_config_byte(pdev, CapabilityPointer+1,(u8 *)&CapabilityHdr.Next);

		// Found the PCI express capability
 		if (CapabilityHdr.CapabilityID == PCI_CAPABILITY_ID_PCI_EXPRESS) 
		{
			break;
		}
		else 
		{
       		// This is some other capability. Keep looking for the PCI express capability.
       		CapabilityPointer = CapabilityHdr.Next;
   		}
	}

	/* Get the Link Control Register, it located at offset 0x10 from the Capability Header */
	if (CapabilityHdr.CapabilityID == PCI_CAPABILITY_ID_PCI_EXPRESS) //
	{
		*HdrOffset = CapabilityPointer;
		pci_read_config_byte(pdev, CapabilityPointer+0x10, LinkCtrlReg);

		status = _SUCCESS;
	}
	else
	{
		// We didn't find a PCIe capability.
		DBG_871X("GetPciLinkCtrlReg(): Cannot Find PCIe Capability\n");
	}

	return (status);
}

static s32	rtw_set_pci_cache_line_size(struct pci_dev *pdev, u8 CacheLineSizeToSet)
{
	u8	ucPciCacheLineSize;
	s32	Result;

	//ucPciCacheLineSize  = pPciConfig->CacheLineSize;
	pci_read_config_byte(pdev, PCI_CACHE_LINE_SIZE, &ucPciCacheLineSize);

	if(ucPciCacheLineSize<8 || ucPciCacheLineSize>16)
	{
		DBG_871X("Driver Sets default Cache Line Size...\n");

		ucPciCacheLineSize = CacheLineSizeToSet;

		Result = pci_write_config_byte(pdev, PCI_CACHE_LINE_SIZE, ucPciCacheLineSize);
		
		if(Result != 0)
		{
			DBG_871X("pci_write_config_byte (CacheLineSize) Result=%d\n", Result);
			goto _SET_CACHELINE_SIZE_FAIL;
		}

		Result = pci_read_config_byte(pdev, PCI_CACHE_LINE_SIZE, &ucPciCacheLineSize);
		if(Result != 0)
		{
			DBG_871X("pci_read_config_byte (PciCacheLineSize) Result=%d\n", Result);
			goto _SET_CACHELINE_SIZE_FAIL;
		}

		if((ucPciCacheLineSize != CacheLineSizeToSet))
		{
			DBG_871X("Failed to set Cache Line Size to 0x%x! ucPciCacheLineSize=%x\n", CacheLineSizeToSet, ucPciCacheLineSize);
			goto _SET_CACHELINE_SIZE_FAIL;
		}		
	}

	return _SUCCESS;

_SET_CACHELINE_SIZE_FAIL:

	return _FAIL;
}


#define PCI_CMD_ENABLE_BUS_MASTER		BIT(2)
#define PCI_CMD_DISABLE_INTERRUPT		BIT(10)
#define CMD_BUS_MASTER				BIT(2)

static s32 rtw_pci_parse_configuration(struct pci_dev *pdev, struct dvobj_priv *pdvobjpriv)
{
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);
	//PPCI_COMMON_CONFIG      	pPciConfig = (PPCI_COMMON_CONFIG) pucBuffer;
	//u16	usPciCommand = pPciConfig->Command;
	u16	usPciCommand = 0;
	int	Result, ret;
	u8	CapabilityOffset;
	RT_PCI_CAPABILITIES_HEADER	CapabilityHdr;
	u8	PCIeCap;
	u8	LinkCtrlReg;
	u8	ClkReqReg;

	//DBG_871X("%s==>\n", __FUNCTION__);

	pci_read_config_word(pdev, PCI_COMMAND, &usPciCommand);

	do{
		//3 Enable bus matering if it isn't enabled by the BIOS
		if(!(usPciCommand & PCI_CMD_ENABLE_BUS_MASTER))
		{
			DBG_871X("Bus master is not enabled by BIOS! usPciCommand=%x\n", usPciCommand);

			usPciCommand |= CMD_BUS_MASTER;

			Result = pci_write_config_word(pdev, PCI_COMMAND, usPciCommand);
			if(Result != 0)
			{
				DBG_871X("pci_write_config_word (Command) Result=%d\n", Result);
				ret = _FAIL;
				break;
			}

			Result = pci_read_config_word(pdev, PCI_COMMAND, &usPciCommand);
			if(Result != 0)
			{
				DBG_871X("pci_read_config_word (Command) Result=%d\n", Result);
				ret = _FAIL;
				break;
			}

			if(!(usPciCommand & PCI_CMD_ENABLE_BUS_MASTER))
			{
				DBG_871X("Failed to enable bus master! usPciCommand=%x\n", usPciCommand);
				ret = _FAIL;
				break;
			}
		}
		DBG_871X("Bus master is enabled. usPciCommand=%x\n", usPciCommand);

		//3 Enable interrupt
		if((usPciCommand & PCI_CMD_DISABLE_INTERRUPT))
		{
			DBG_871X("INTDIS==1 usPciCommand=%x\n", usPciCommand);

			usPciCommand &= (~PCI_CMD_DISABLE_INTERRUPT);

			Result = pci_write_config_word(pdev, PCI_COMMAND, usPciCommand);
			if(Result != 0)
			{
				DBG_871X("pci_write_config_word (Command) Result=%d\n", Result);
				ret = _FAIL;
				break;
			}

			Result = pci_read_config_word(pdev, PCI_COMMAND, &usPciCommand);
			if(Result != 0)
			{
				DBG_871X("pci_read_config_word (Command) Result=%d\n", Result);
				ret = _FAIL;
				break;
			}

			if((usPciCommand & PCI_CMD_DISABLE_INTERRUPT))
			{
				DBG_871X("Failed to set INTDIS to 0! usPciCommand=%x\n", usPciCommand);
				ret = _FAIL;
				break;
			}
		}

		//
		//Description: Find PCI express capability offset. Porting from 818xB by tynli 2008.12.19
		//
		//-------------------------------------------------------------

		//3 PCIeCap
		// The device supports capability lists. Find the capabilities.

		//CapabilityOffset = pPciConfig->u.type0.CapabilitiesPtr;
		pci_read_config_byte(pdev, PCI_CAPABILITY_LIST, &CapabilityOffset);
		
		// Loop through the capabilities in search of the power management capability. 
		// The list is NULL-terminated, so the last offset will always be zero.

		while (CapabilityOffset != 0)
		{
    			// Read the header of the capability at  this offset. If the retrieved capability is not
	  		// the power management capability that we are looking for, follow the link to the 
	  		// next capability and continue looping.

			//Result = pci_read_config_word(pdev, CapabilityOffset, (u16 *)&CapabilityHdr);
			Result = pci_read_config_byte(pdev, CapabilityOffset,  (u8 *)&CapabilityHdr.CapabilityID);
			if(Result != 0)
				break;

			Result = pci_read_config_byte(pdev, CapabilityOffset+1,(u8 *)&CapabilityHdr.Next);
			if(Result != 0)
				break;

			// Found the PCI express capability
			if (CapabilityHdr.CapabilityID == PCI_CAPABILITY_ID_PCI_EXPRESS) 
			{
				break;
			} 
			else 
			{
    				// This is some other capability. Keep looking for the PCI express capability.
    				CapabilityOffset = CapabilityHdr.Next;
			}
		}

		if(Result != 0)
		{
			DBG_871X("pci_read_config_word (RT_PCI_CAPABILITIES_HEADER) Result=%d\n", Result);
			break;
		}

		if (CapabilityHdr.CapabilityID == PCI_CAPABILITY_ID_PCI_EXPRESS) 
		{
			pcipriv->pciehdr_offset = CapabilityOffset;
			DBG_871X("PCIe Header Offset =%x\n", CapabilityOffset);

			// Skip past the capabilities header and read the PCI express capability
			// Justin: The PCI-e capability size should be 2 bytes, why we just get 1 byte
			// Beside, this PCIeCap seems no one reference it in the driver code
			Result = pci_read_config_byte(pdev, CapabilityOffset+ 2, &PCIeCap);

			if(Result != 0)
			{
				DBG_871X("pci_read_config_byte (PCIE Capability) Result=%d\n", Result);
				break;
			}

			pcipriv->pcie_cap = PCIeCap;
			DBG_871X("PCIe Capability =%x\n", PCIeCap);

			//3 Link Control Register
			// Read "Link Control Register" Field (80h ~81h)
			Result = pci_read_config_byte(pdev, CapabilityOffset+ 0x10, &LinkCtrlReg);
			if(Result != 0)
			{
				DBG_871X("pci_read_config_byte (Link Control Register) Result=%d\n", Result);
				break;
			}

			pcipriv->linkctrl_reg = LinkCtrlReg;
			DBG_871X("Link Control Register =%x\n", LinkCtrlReg);

			//3 Get Capability of PCI Clock Request
			// The clock request setting is located at 0x81[0]
			Result = pci_read_config_byte(pdev, CapabilityOffset+ 0x11, &ClkReqReg);
			if(Result != 0)
			{
				pcipriv->pci_clk_req = _FALSE;
				DBG_871X("pci_read_config_byte (Clock Request Register) Result=%d\n", Result);
				break;
			}
			if(ClkReqReg & BIT(0))
				pcipriv->pci_clk_req = _TRUE;
			else
				pcipriv->pci_clk_req = _FALSE;
			DBG_871X("Clock Request =%x\n", pcipriv->pci_clk_req);
		}
		else
		{
			// We didn't find a PCIe capability. 
			DBG_871X("Didn't Find PCIe Capability\n");
			break;
		}
		
		//3 Fill Cacheline 
		ret = rtw_set_pci_cache_line_size(pdev, 8);
		if(ret != _SUCCESS)
		{
			DBG_871X("rtw_set_pci_cache_line_size fail\n");
			break;
		}
	
//Include 92C suggested by SD1. Added by tynli. 2009.11.25.
// Enable the Backdoor
		{
			u8	tmp;

			Result = pci_read_config_byte(pdev, 0x98, &tmp);
			
			tmp |= BIT4;

			Result = pci_write_config_byte(pdev, 0x98, tmp);

		}
		ret = _SUCCESS;
	}while(_FALSE);

	return ret;
}

//
// Update PCI dependent default settings.
//
static void rtw_pci_update_default_setting(_adapter *padapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(padapter);
	struct pci_priv	*pcipriv = &(pdvobjpriv->pcipriv);
	struct pwrctrl_priv	*pwrpriv = dvobj_to_pwrctl(pdvobjpriv);

	//reset pPSC->reg_rfps_level & priv->b_support_aspm
	pwrpriv->reg_rfps_level = 0;

	// Update PCI ASPM setting
	//pwrpriv->const_amdpci_aspm = pdvobjpriv->const_amdpci_aspm;
	switch (pdvobjpriv->const_pci_aspm) {
		case 0:		// No ASPM
			break;

		case 1:		// ASPM dynamically enabled/disable.
			pwrpriv->reg_rfps_level |= RT_RF_LPS_LEVEL_ASPM;
			break;

		case 2:		// ASPM with Clock Req dynamically enabled/disable.
			pwrpriv->reg_rfps_level |= (RT_RF_LPS_LEVEL_ASPM | RT_RF_OFF_LEVL_CLK_REQ);
			break;

		case 3:		// Always enable ASPM and Clock Req from initialization to halt.
			pwrpriv->reg_rfps_level &= ~(RT_RF_LPS_LEVEL_ASPM);
			pwrpriv->reg_rfps_level |= (RT_RF_PS_LEVEL_ALWAYS_ASPM | RT_RF_OFF_LEVL_CLK_REQ);
			break;

		case 4:		// Always enable ASPM without Clock Req from initialization to halt.
			pwrpriv->reg_rfps_level &= ~(RT_RF_LPS_LEVEL_ASPM | RT_RF_OFF_LEVL_CLK_REQ);
			pwrpriv->reg_rfps_level |= RT_RF_PS_LEVEL_ALWAYS_ASPM;
			break;

		case 5: // Linux do not support ASPM OSC, added by Roger, 2013.03.27.	
			break;
	}

	pwrpriv->reg_rfps_level |= RT_RF_OFF_LEVL_HALT_NIC;

	// Update Radio OFF setting
	switch (pdvobjpriv->const_hwsw_rfoff_d3) {
		case 1:
			if (pwrpriv->reg_rfps_level & RT_RF_LPS_LEVEL_ASPM)
				pwrpriv->reg_rfps_level |= RT_RF_OFF_LEVL_ASPM;
			break;

		case 2:
			if (pwrpriv->reg_rfps_level & RT_RF_LPS_LEVEL_ASPM)
				pwrpriv->reg_rfps_level |= RT_RF_OFF_LEVL_ASPM;
			pwrpriv->reg_rfps_level |= RT_RF_OFF_LEVL_HALT_NIC;
			break;

		case 3:
			pwrpriv->reg_rfps_level |= RT_RF_OFF_LEVL_PCI_D3;
			break;
	}

	// Update Rx 2R setting
	//pPSC->reg_rfps_level |= ((pDevice->RegLPS2RDisable) ? RT_RF_LPS_DISALBE_2R : 0);

	//
	// Set HW definition to determine if it supports ASPM.
	//
	switch (pdvobjpriv->const_support_pciaspm) {
		case 1:	// Support ASPM.
			{
				u8	b_support_backdoor = _TRUE;
				u8	b_support_l1_on_amd = _FALSE;

				rtw_hal_get_def_var(padapter, HAL_DEF_PCI_AMD_L1_SUPPORT, &b_support_l1_on_amd);

				if(padapter->eeprompriv.CustomerID == RT_CID_TOSHIBA &&
					pcipriv->pcibridge_vendor == PCI_BRIDGE_VENDOR_AMD && 
					!pcipriv->amd_l1_patch && !b_support_l1_on_amd)
				{
					DBG_871X("%s(): Disable L1 Backdoor!!\n",__FUNCTION__);
					b_support_backdoor = _FALSE;
				}
				rtw_hal_set_def_var(padapter, HAL_DEF_PCI_SUUPORT_L1_BACKDOOR, &b_support_backdoor);
			}
			break;

		default:
			// Do nothing. Set when finding the chipset.
			break;
	}
}

static void rtw_pci_initialize_adapter_common(_adapter *padapter)
{
	struct pwrctrl_priv	*pwrpriv = adapter_to_pwrctl(padapter);

	rtw_pci_update_default_setting(padapter);

	if (pwrpriv->reg_rfps_level & RT_RF_PS_LEVEL_ALWAYS_ASPM) {
		// Always enable ASPM & Clock Req.
		rtw_pci_enable_aspm(padapter);
		RT_SET_PS_LEVEL(pwrpriv, RT_RF_PS_LEVEL_ALWAYS_ASPM);
	}

}

//
// 2009/10/28 MH Enable rtl8192ce DMA64 function. We need to enable 0x719 BIT5
//
#ifdef CONFIG_64BIT_DMA
u8 PlatformEnableDMA64(PADAPTER Adapter)
{
	struct dvobj_priv	*pdvobjpriv = adapter_to_dvobj(Adapter);
	struct pci_dev	*pdev = pdvobjpriv->ppcidev;
	u8	bResult = _TRUE;
	u8	value;

	pci_read_config_byte(pdev,0x719, &value);

	// 0x719 Bit5 is DMA64 bit fetch. 
	value |= (BIT5);

	pci_write_config_byte(pdev,0x719, value);
	
	return bResult;
}
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)) || (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,18))
#define rtw_pci_interrupt(x,y,z) rtw_pci_interrupt(x,y)
#endif

static irqreturn_t rtw_pci_interrupt(int irq, void *priv, struct pt_regs *regs)
{
	struct dvobj_priv *dvobj = (struct dvobj_priv *)priv;
	_adapter *adapter = dvobj->if1;

	if (dvobj->irq_enabled == 0) {
		return IRQ_HANDLED;
	}

	if(rtw_hal_interrupt_handler(adapter) == _FAIL)
		return IRQ_HANDLED;
		//return IRQ_NONE;

	return IRQ_HANDLED;
}

#ifdef RTK_DMP_PLATFORM
#define pci_iounmap(x,y) iounmap(y)
#endif

int pci_alloc_irq(struct dvobj_priv *dvobj)
{
	int err;
	struct pci_dev *pdev = dvobj->ppcidev;
	int ret;

	ret = pci_enable_msi( pdev );

	DBG_871X( "pci_enable_msi ret=%d\n", ret );
	
#if defined(IRQF_SHARED)
	err = request_irq(pdev->irq, &rtw_pci_interrupt, IRQF_SHARED, DRV_NAME, dvobj);
#else
	err = request_irq(pdev->irq, &rtw_pci_interrupt, SA_SHIRQ, DRV_NAME, dvobj);
#endif
	if (err) {
		DBG_871X("Error allocating IRQ %d",pdev->irq);
	} else {
		dvobj->irq_alloc = 1;
		DBG_871X("Request_irq OK, IRQ %d\n",pdev->irq);
	}

	return err?_FAIL:_SUCCESS;
}

static struct dvobj_priv	*pci_dvobj_init(struct pci_dev *pdev)
{
	int err;
	u32	status = _FAIL;
	struct dvobj_priv	*dvobj = NULL;
	struct pci_priv	*pcipriv = NULL;
	struct pci_dev	*bridge_pdev = pdev->bus->self;
	//u32	pci_cfg_space[16];
	unsigned long pmem_start, pmem_len, pmem_flags;
	u8	tmp;
	u8	PciBgVIdIdx;
	int	i;

_func_enter_;

	if((dvobj = devobj_init()) == NULL) {
		goto exit;
	}


	dvobj->ppcidev = pdev;
	pcipriv = &(dvobj->pcipriv);
	pci_set_drvdata(pdev, dvobj);


	if ( (err = pci_enable_device(pdev)) != 0) {
		DBG_871X(KERN_ERR "%s : Cannot enable new PCI device\n", pci_name(pdev));
		goto free_dvobj;
	}

#ifdef CONFIG_64BIT_DMA
	if (!pci_set_dma_mask(pdev, DMA_BIT_MASK(64))) {
		DBG_871X("RTL819xCE: Using 64bit DMA\n");
		if ((err = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(64))) != 0) {
			DBG_871X(KERN_ERR "Unable to obtain 64bit DMA for consistent allocations\n");
			goto disable_picdev;
		}
		dvobj->bdma64 = _TRUE;
	} else
#endif
	{
		if (!pci_set_dma_mask(pdev, DMA_BIT_MASK(32))) {
			if ((err = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32))) != 0) {
				DBG_871X(KERN_ERR "Unable to obtain 32bit DMA for consistent allocations\n");
				goto disable_picdev;
			}
		}
	}

	pci_set_master(pdev);

	if ((err = pci_request_regions(pdev, DRV_NAME)) != 0) {
		DBG_871X(KERN_ERR "Can't obtain PCI resources\n");
		goto disable_picdev;
	}

	// Search for memory map resource (index 0~5)
	for (i=0 ; i < 6 ; i++){
		pmem_start = pci_resource_start(pdev, i);
		pmem_len = pci_resource_len(pdev, i);
		pmem_flags = pci_resource_flags (pdev, i);

		if (pmem_flags & IORESOURCE_MEM)
			break;
	}

	if(i ==6)
	{
		DBG_871X(KERN_ERR "%s: No MMIO resource found, abort!\n", __func__);
		goto release_regions;
	}

#ifdef RTK_DMP_PLATFORM
	dvobj->pci_mem_start = (unsigned long)ioremap_nocache(pmem_start, pmem_len);
#else
	dvobj->pci_mem_start = (unsigned long)pci_iomap(pdev, i, pmem_len); /* shared mem start */
#endif
	if (dvobj->pci_mem_start == 0) {
		DBG_871X(KERN_ERR "Can't map PCI mem\n");
		goto release_regions;
	}

	DBG_871X("Memory mapped space start: 0x%08lx len:%08lx flags:%08lx, after map:0x%08lx\n",
		pmem_start, pmem_len, pmem_flags, dvobj->pci_mem_start);

	/*find bus info*/
	pcipriv->busnumber = pdev->bus->number;
	pcipriv->devnumber = PCI_SLOT(pdev->devfn);
	pcipriv->funcnumber = PCI_FUNC(pdev->devfn);

	/*find bridge info*/
	if(bridge_pdev){
		pcipriv->pcibridge_busnum = bridge_pdev->bus->number;
		pcipriv->pcibridge_devnum = PCI_SLOT(bridge_pdev->devfn);
		pcipriv->pcibridge_funcnum = PCI_FUNC(bridge_pdev->devfn);
		pcipriv->pcibridge_vendor = PCI_BRIDGE_VENDOR_UNKNOWN;
		pcipriv->pcibridge_vendorid = bridge_pdev->vendor;
		pcipriv->pcibridge_deviceid = bridge_pdev->device;
	}

#if 0
	// Read PCI configuration Space Header
	for (i=0;i<16;i++)
		pci_read_config_dword(pdev, (i<<2), &pci_cfg_space[i]);
#endif

	//rtw_pci_parse_configuration(pdev, dvobj, (u8 *)&pci_cfg_space);
	rtw_pci_parse_configuration(pdev, dvobj);

	for (PciBgVIdIdx = 0; PciBgVIdIdx < PCI_BRIDGE_VENDOR_MAX; PciBgVIdIdx++) {
		if (pcipriv->pcibridge_vendorid == pcibridge_vendors[PciBgVIdIdx]) {
			pcipriv->pcibridge_vendor = PciBgVIdIdx;
			DBG_871X("Pci Bridge Vendor is found: VID=0x%x, VendorIdx=%d\n", pcipriv->pcibridge_vendorid, PciBgVIdIdx);
			break;
		}
	}

	if(pcipriv->pcibridge_vendor != PCI_BRIDGE_VENDOR_UNKNOWN)
	{
		rtw_pci_get_linkcontrol_reg(bridge_pdev, &pcipriv->pcibridge_linkctrlreg, &pcipriv->pcibridge_pciehdr_offset);

		if(pcipriv->pcibridge_vendor == PCI_BRIDGE_VENDOR_AMD)
		{
			pcipriv->amd_l1_patch = rtw_pci_get_amd_l1_patch(dvobj, bridge_pdev);
		}
	}

	status = _SUCCESS;

iounmap:
	if (status != _SUCCESS && dvobj->pci_mem_start != 0) {
		#if 1//def RTK_DMP_PLATFORM
		pci_iounmap(pdev, (void *)dvobj->pci_mem_start);
		#endif
		dvobj->pci_mem_start = 0;
	}
release_regions:
	if (status != _SUCCESS)
		pci_release_regions(pdev);
disable_picdev:
	if (status != _SUCCESS)
		pci_disable_device(pdev);
free_dvobj:
	if (status != _SUCCESS && dvobj) {
		pci_set_drvdata(pdev, NULL);
		devobj_deinit(dvobj);
		dvobj = NULL;
	}
exit:
_func_exit_;
	return dvobj;
}


static void pci_dvobj_deinit(struct pci_dev *pdev)
{
	struct dvobj_priv *dvobj = pci_get_drvdata(pdev);
_func_enter_;

	pci_set_drvdata(pdev, NULL);
	if (dvobj) {
		if (dvobj->irq_alloc) {
			free_irq(pdev->irq, dvobj);
			pci_disable_msi( pdev );
			dvobj->irq_alloc = 0;
		}

		if (dvobj->pci_mem_start != 0) {
			#if 1//def RTK_DMP_PLATFORM
			pci_iounmap(pdev, (void *)dvobj->pci_mem_start);
			#endif
			dvobj->pci_mem_start = 0;
		}

		devobj_deinit(dvobj);
	}

	pci_release_regions(pdev);
	pci_disable_device(pdev);

_func_exit_;
}


static void decide_chip_type_by_pci_device_id(_adapter *padapter, struct pci_dev *pdev)
{
	u16	venderid, deviceid, irqline;
	u8	revisionid;
	struct dvobj_priv	*pdvobjpriv=adapter_to_dvobj(padapter);


	venderid = pdev->vendor;
	deviceid = pdev->device;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23))
	pci_read_config_byte(pdev, PCI_REVISION_ID, &revisionid); // PCI_REVISION_ID 0x08
#else
	revisionid = pdev->revision;
#endif
	pci_read_config_word(pdev, PCI_INTERRUPT_LINE, &irqline); // PCI_INTERRUPT_LINE 0x3c
	pdvobjpriv->irqline = irqline;


	//
	// Decide hardware type here. 
	//
	if( deviceid == HAL_HW_PCI_8185_DEVICE_ID ||
	    deviceid == HAL_HW_PCI_8188_DEVICE_ID ||
	    deviceid == HAL_HW_PCI_8198_DEVICE_ID)
	{
		DBG_871X("Adapter (8185/8185B) is found- VendorID/DeviceID=%x/%x\n", venderid, deviceid);
		padapter->HardwareType=HARDWARE_TYPE_RTL8185;
	}
	else if (deviceid == HAL_HW_PCI_8190_DEVICE_ID ||
		deviceid == HAL_HW_PCI_0045_DEVICE_ID ||
		deviceid == HAL_HW_PCI_0046_DEVICE_ID ||
		deviceid == HAL_HW_PCI_DLINK_DEVICE_ID)
	{
		DBG_871X("Adapter(8190 PCI) is found - vendorid/deviceid=%x/%x\n", venderid, deviceid);
		padapter->HardwareType = HARDWARE_TYPE_RTL8190P;
	}
	else if (deviceid == HAL_HW_PCI_8192_DEVICE_ID ||
		deviceid == HAL_HW_PCI_0044_DEVICE_ID ||
		deviceid == HAL_HW_PCI_0047_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8192SE_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8174_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8173_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8172_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8171_DEVICE_ID)
	{
		// 8192e and and 8192se may have the same device ID 8192. However, their Revision
		// ID is different
		// Added for 92DE. We deferentiate it from SVID,SDID.
		if( pdev->subsystem_vendor == 0x10EC && pdev->subsystem_device == 0xE020){
			padapter->HardwareType = HARDWARE_TYPE_RTL8192DE;
			DBG_871X("Adapter(8192DE) is found - VendorID/DeviceID/RID=%X/%X/%X\n", venderid, deviceid, revisionid);
		}else{
			switch (revisionid) {
				case HAL_HW_PCI_REVISION_ID_8192PCIE:
					DBG_871X("Adapter(8192 PCI-E) is found - vendorid/deviceid=%x/%x\n", venderid, deviceid);
					padapter->HardwareType = HARDWARE_TYPE_RTL8192E;
					break;
				case HAL_HW_PCI_REVISION_ID_8192SE:
					DBG_871X("Adapter(8192SE) is found - vendorid/deviceid=%x/%x\n", venderid, deviceid);
					padapter->HardwareType = HARDWARE_TYPE_RTL8192SE;
					break;
				default:
					DBG_871X("Err: Unknown device - vendorid/deviceid=%x/%x\n", venderid, deviceid);
					padapter->HardwareType = HARDWARE_TYPE_RTL8192SE;
					break;
			}
		}
	}
	else if(deviceid==HAL_HW_PCI_8723E_DEVICE_ID )
	{//RTL8723E may have the same device ID with RTL8192CET
		padapter->HardwareType = HARDWARE_TYPE_RTL8723AE;
		DBG_871X("Adapter(8723 PCI-E) is found - VendorID/DeviceID=%x/%x\n", venderid, deviceid);
	}
	else if (deviceid == HAL_HW_PCI_8192CET_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8192CE_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8191CE_DEVICE_ID ||
		deviceid == HAL_HW_PCI_8188CE_DEVICE_ID) 
	{
		DBG_871X("Adapter(8192C PCI-E) is found - vendorid/deviceid=%x/%x\n", venderid, deviceid);
		padapter->HardwareType = HARDWARE_TYPE_RTL8192CE;
	}
	else if (deviceid == HAL_HW_PCI_8192DE_DEVICE_ID ||
		deviceid == HAL_HW_PCI_002B_DEVICE_ID ){
		padapter->HardwareType = HARDWARE_TYPE_RTL8192DE;
		DBG_871X("Adapter(8192DE) is found - VendorID/DeviceID/RID=%X/%X/%X\n", venderid, deviceid, revisionid);
	}
	else if (deviceid == HAL_HW_PCI_8188EE_DEVICE_ID){
		padapter->HardwareType = HARDWARE_TYPE_RTL8188EE;
		padapter->chip_type = RTL8188E;
		DBG_871X("Adapter(8188EE) is found - VendorID/DeviceID/RID=%X/%X/%X\n", venderid, deviceid, revisionid);
	}
	
	else
	{
		DBG_871X("Err: Unknown device - vendorid/deviceid=%x/%x\n", venderid, deviceid);
		//padapter->HardwareType = HAL_DEFAULT_HARDWARE_TYPE;
	}


	padapter->chip_type = NULL_CHIP_TYPE;

	//TODO:
#ifdef CONFIG_RTL8192C
	padapter->chip_type = RTL8188C_8192C;
	padapter->HardwareType = HARDWARE_TYPE_RTL8192CE;
#endif
#ifdef CONFIG_RTL8192D
	pdvobjpriv->InterfaceNumber = revisionid;

	padapter->chip_type = RTL8192D;
	padapter->HardwareType = HARDWARE_TYPE_RTL8192DE;
#endif

}

static void rtw_decide_chip_type_by_pci_driver_data(_adapter *padapter, struct pci_dev *pdev, const struct pci_device_id *pdid)
{
	padapter->chip_type = pdid->driver_data;

	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C){	
		padapter->HardwareType = HARDWARE_TYPE_RTL8192CE;
		DBG_871X("CHIP TYPE: RTL8188C_8192C\n");
	}
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D){
		u8	revisionid;
		struct dvobj_priv	*pdvobjpriv=adapter_to_dvobj(padapter);

	#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23))
		pci_read_config_byte(pdev, PCI_REVISION_ID, &revisionid); // PCI_REVISION_ID 0x08
	#else
		revisionid = pdev->revision;
	#endif
		pdvobjpriv->InterfaceNumber = revisionid;

		padapter->HardwareType = HARDWARE_TYPE_RTL8192DE;
		DBG_871X("CHIP TYPE: RTL8192D\n");
	}
	#endif	
	
	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E){	
		padapter->HardwareType = HARDWARE_TYPE_RTL8188EE;
		DBG_871X("CHIP TYPE: RTL8188E\n");
	}
	#endif	

	#ifdef CONFIG_RTL8812A
	if(padapter->chip_type == RTL8812){
		padapter->HardwareType = HARDWARE_TYPE_RTL8812E;
		DBG_871X("CHIP TYPE: RTL8812E\n");
	}
	#endif

	#ifdef CONFIG_RTL8821A
	if(padapter->chip_type == RTL8821){
		padapter->HardwareType = HARDWARE_TYPE_RTL8821E;
		DBG_871X("CHIP TYPE: RTL8821AE\n");
	}
	#endif

	#ifdef CONFIG_RTL8723B
	if(padapter->chip_type == RTL8723B){
		padapter->HardwareType = HARDWARE_TYPE_RTL8723BE;
		DBG_871X("CHIP TYPE: RTL8723BE\n");
	}
	#endif

/*
	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A){	
		padapter->HardwareType = HARDWARE_TYPE_RTL8723AE;
	}
	#endif
*/	
	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E){	
		padapter->HardwareType = HARDWARE_TYPE_RTL8192EE;
		DBG_871X("CHIP TYPE: RTL8192EE\n");
	}
	#endif

}

void rtw_set_hal_ops(_adapter *padapter)
{
	//alloc memory for HAL DATA
	rtw_hal_data_init(padapter);
	
	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C)	
		rtl8192ce_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D)	
		rtl8192de_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E)	
		rtl8188ee_set_hal_ops(padapter);
	#endif

	#if defined(CONFIG_RTL8812A) || defined(CONFIG_RTL8821A)
	if((padapter->chip_type == RTL8812)||(padapter->chip_type == RTL8821))
		rtl8812ae_set_hal_ops(padapter);
	#endif

	#ifdef CONFIG_RTL8723B
	if(padapter->chip_type == RTL8723B)
		rtl8723be_set_hal_ops(padapter);
	#endif
	
/*	
	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A)	
		rtl8723ae_set_hal_ops(padapter);
	#endif
*/	
	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E)	
		rtl8192ee_set_hal_ops(padapter);
	#endif

}

void pci_set_intf_ops(_adapter *padapter,struct _io_ops *pops)
{
	#ifdef CONFIG_RTL8192C
	if(padapter->chip_type == RTL8188C_8192C)	
		rtl8192ce_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8192D
	if(padapter->chip_type == RTL8192D)	
		rtl8192de_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8188E
	if(padapter->chip_type == RTL8188E)	
		rtl8188ee_set_intf_ops(pops);
	#endif

	#if defined(CONFIG_RTL8812A) || defined(CONFIG_RTL8821A)
	if((padapter->chip_type == RTL8812)||(padapter->chip_type == RTL8821))
		rtl8812ae_set_intf_ops(pops);
	#endif

	#ifdef CONFIG_RTL8723B
	if(padapter->chip_type == RTL8723B)
		rtl8723be_set_intf_ops(pops);
	#endif
	
/*
	#ifdef CONFIG_RTL8723A
	if(padapter->chip_type == RTL8723A)	
		
	#endif
*/
	#ifdef CONFIG_RTL8192E
	if(padapter->chip_type == RTL8192E)	
		rtl8192ee_set_intf_ops(pops);
	#endif
		
}
static void pci_intf_start(_adapter *padapter)
{

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+pci_intf_start\n"));
	DBG_871X("+pci_intf_start\n");

	//Enable hw interrupt
	rtw_hal_enable_interrupt(padapter);

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-pci_intf_start\n"));
	DBG_871X("-pci_intf_start\n");
}

static void pci_intf_stop(_adapter *padapter)
{

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("+pci_intf_stop\n"));

	//Disable hw interrupt
	if(padapter->bSurpriseRemoved == _FALSE)
	{
		//device still exists, so driver can do i/o operation
		rtw_hal_disable_interrupt(padapter);
		tasklet_kill(&(padapter->recvpriv.recv_tasklet));
		tasklet_kill(&(padapter->recvpriv.irq_prepare_beacon_tasklet));
		tasklet_kill(&(padapter->xmitpriv.xmit_tasklet));
		
#ifdef CONFIG_CONCURRENT_MODE
		/*	This function only be called at driver removing. disable buddy_adapter too
			don't disable interrupt of buddy_adapter because it is same as primary.
		*/
		if (padapter->pbuddy_adapter){
			tasklet_kill(&(padapter->pbuddy_adapter->recvpriv.recv_tasklet));
			tasklet_kill(&(padapter->pbuddy_adapter->recvpriv.irq_prepare_beacon_tasklet));
			tasklet_kill(&(padapter->pbuddy_adapter->xmitpriv.xmit_tasklet));
		}
#endif

		rtw_hal_set_hwreg(padapter, HW_VAR_PCIE_STOP_TX_DMA, 0);

		rtw_hal_irp_reset(padapter);

		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("pci_intf_stop: SurpriseRemoved==_FALSE\n"));
	}
	else
	{
		// Clear irq_enabled to prevent handle interrupt function.
		adapter_to_dvobj(padapter)->irq_enabled = 0;
	}

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-pci_intf_stop\n"));

}

static void disable_ht_for_spec_devid(const struct pci_device_id *pdid)
{
#ifdef CONFIG_80211N_HT
	u16 vid, pid;
	u32 flags;
	int i;
	int num = sizeof(specific_device_id_tbl)/sizeof(struct specific_device_id);

	for(i=0; i<num; i++)
	{
		vid = specific_device_id_tbl[i].idVendor;
		pid = specific_device_id_tbl[i].idProduct;
		flags = specific_device_id_tbl[i].flags;

		if((pdid->vendor==vid) && (pdid->device==pid) && (flags&SPEC_DEV_ID_DISABLE_HT))
		{
			 rtw_ht_enable = 0;
			 rtw_bw_mode = 0;
			 rtw_ampdu_enable = 0;
		}

	}
#endif
}

#ifdef CONFIG_PM
static int rtw_pci_suspend(struct pci_dev *pdev, pm_message_t state)
{
	int ret = 0;
	struct dvobj_priv *dvobj = pci_get_drvdata(pdev);
	_adapter *padapter = dvobj->if1;

	ret = rtw_suspend_common(padapter);
	if ( (ret = pci_save_state(pdev)) != 0) {
		DBG_871X("%s Failed on pci_save_state (%d)\n",__FUNCTION__,ret);
		goto exit;
	}

	pci_disable_device(pdev);

	if ( (ret = pci_set_power_state(pdev, pci_choose_state(pdev, state))) != 0) {
		DBG_871X("%s Failed on pci_set_power_state (%d)\n",__FUNCTION__,ret);
	}
	
exit:	
	return ret;	
	
}

int rtw_resume_process(_adapter *padapter)
{
	return rtw_resume_common(padapter);
}

static int rtw_pci_resume(struct pci_dev *pdev)
{
	struct dvobj_priv *dvobj = pci_get_drvdata(pdev);
	_adapter *padapter = dvobj->if1;
	struct net_device *pnetdev = padapter->pnetdev;
	struct pwrctrl_priv *pwrpriv = dvobj_to_pwrctl(dvobj);
	int	err = 0;

	if ( (err = pci_set_power_state(pdev, PCI_D0)) != 0) {
		DBG_871X("%s Failed on pci_set_power_state (%d)\n",__FUNCTION__,err);
		goto exit;
	}

	if ( (err = pci_enable_device(pdev)) != 0) {
		DBG_871X("%s Failed on pci_enable_device (%d)\n",__FUNCTION__,err);
		goto exit;
	}

#if (LINUX_VERSION_CODE > KERNEL_VERSION(2,6,37))
	pci_restore_state(pdev);
#else
	if ( (err = pci_restore_state(pdev)) != 0) {
		DBG_871X("%s Failed on pci_restore_state (%d)\n",__FUNCTION__,err);
		goto exit;
	}
#endif

	if(pwrpriv->bInternalAutoSuspend)
	{
 		err = rtw_resume_process(padapter);
	}
	else
	{
		if(pwrpriv->wowlan_mode || pwrpriv->wowlan_ap_mode)
		{
			rtw_resume_lock_suspend();			
			err = rtw_resume_process(padapter);
			rtw_resume_unlock_suspend();
		}
		else
		{
#ifdef CONFIG_RESUME_IN_WORKQUEUE
			rtw_resume_in_workqueue(pwrpriv);
#else			
			if (rtw_is_earlysuspend_registered(pwrpriv))
			{
				/* jeff: bypass resume here, do in late_resume */
				rtw_set_do_late_resume(pwrpriv, _TRUE);
			}	
			else
			{
				rtw_resume_lock_suspend();			
				err = rtw_resume_process(padapter);
				rtw_resume_unlock_suspend();
			}
#endif
		}
	}

exit:

	return err;
}
#endif//CONFIG_PM

_adapter *rtw_pci_if1_init(struct dvobj_priv * dvobj, struct pci_dev *pdev, const struct pci_device_id *pdid)
{
	_adapter *padapter = NULL;
	struct net_device *pnetdev = NULL;
	int status = _FAIL;
	
	if ((padapter = (_adapter *)rtw_zvmalloc(sizeof(*padapter))) == NULL) {
		goto exit;
	}
	padapter->dvobj = dvobj;
	dvobj->if1 = padapter;
	
	padapter->bDriverStopped=_TRUE;

	dvobj->padapters[dvobj->iface_nums++] = padapter;
	padapter->iface_id = IFACE_ID0;

#if defined(CONFIG_CONCURRENT_MODE) || defined(CONFIG_DUALMAC_CONCURRENT)
	//set adapter_type/iface type for primary padapter
	padapter->isprimary = _TRUE;
	padapter->adapter_type = PRIMARY_ADAPTER;
	#ifndef CONFIG_HWPORT_SWAP
	padapter->iface_type = IFACE_PORT0;
	#else
	padapter->iface_type = IFACE_PORT1;
	#endif
#endif

	//step 1-1., decide the chip_type via device info	
	padapter->interface_type = RTW_PCIE;
	rtw_decide_chip_type_by_pci_driver_data(padapter, pdev, pdid);
	
	if((pnetdev = rtw_init_netdev(padapter)) == NULL) {
		goto free_adapter;
	}

	#if LINUX_VERSION_CODE > KERNEL_VERSION(2,5,0)
	SET_NETDEV_DEV(pnetdev, dvobj_to_dev(dvobj));
	#endif
	if (dvobj->bdma64)
		pnetdev->features |= NETIF_F_HIGHDMA;
	pnetdev->irq = pdev->irq;
	
	padapter = rtw_netdev_priv(pnetdev);

#ifdef CONFIG_IOCTL_CFG80211
	if(rtw_wdev_alloc(padapter, dvobj_to_dev(dvobj)) != 0) {
		goto free_adapter;
	}
#endif //CONFIG_IOCTL_CFG80211


	//step 2.	hook HalFunc, allocate HalData
	//hal_set_hal_ops(padapter);
	rtw_set_hal_ops(padapter);

	//step 3.
	padapter->intf_start=&pci_intf_start;
	padapter->intf_stop=&pci_intf_stop;


	//.2
	rtw_init_io_priv(padapter, pci_set_intf_ops);

	//.3
	rtw_hal_read_chip_version(padapter);
	
	//.4
	rtw_hal_chip_configure(padapter);

#ifdef CONFIG_BT_COEXIST
	rtw_btcoex_Initialize(padapter);
#endif // CONFIG_BT_COEXIST

	//step 4. read efuse/eeprom data and get mac_addr
	rtw_hal_read_chip_info(padapter);	

	//step 5. 
	if(rtw_init_drv_sw(padapter) ==_FAIL) {
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize driver software resource Failed!\n"));
		goto free_hal_data;
	}

	if(rtw_hal_inirp_init(padapter) ==_FAIL) {
		RT_TRACE(_module_hci_intfs_c_,_drv_err_,("Initialize PCI desc ring Failed!\n"));
		goto free_hal_data;
	}
	rtw_macaddr_cfg(adapter_mac_addr(padapter), padapter->eeprompriv.mac_addr);
	rtw_init_wifidirect_addrs(padapter, adapter_mac_addr(padapter), adapter_mac_addr(padapter));

	rtw_hal_disable_interrupt(padapter);

	//step 6. Init pci related configuration
	rtw_pci_initialize_adapter_common(padapter);

	DBG_871X("bDriverStopped:%d, bSurpriseRemoved:%d, bup:%d, hw_init_completed:%d\n"
		,padapter->bDriverStopped
		,padapter->bSurpriseRemoved
		,padapter->bup
		,padapter->hw_init_completed
	);

	status = _SUCCESS;

free_hal_data:
	if (status != _SUCCESS && padapter->HalData)
		rtw_hal_free_data(padapter);

free_wdev:
	if(status != _SUCCESS) {
		#ifdef CONFIG_IOCTL_CFG80211
		rtw_wdev_unregister(padapter->rtw_wdev);
		rtw_wdev_free(padapter->rtw_wdev);
		#endif
	}

free_adapter:
	if (status != _SUCCESS) {
		if (pnetdev)
			rtw_free_netdev(pnetdev);
		else
			rtw_vmfree((u8*)padapter, sizeof(*padapter));
		padapter = NULL;
	}
exit:
	return padapter;
}

static void rtw_pci_if1_deinit(_adapter *if1)
{
	struct net_device *pnetdev = if1->pnetdev;
	struct mlme_priv *pmlmepriv= &if1->mlmepriv;

	//	padapter->intf_stop(padapter);

	if(check_fwstate(pmlmepriv, _FW_LINKED))
		rtw_disassoc_cmd(if1, 0, _FALSE);

#ifdef CONFIG_AP_MODE
	free_mlme_ap_info(if1);
	#ifdef CONFIG_HOSTAPD_MLME
	hostapd_mode_unload(if1);
	#endif
#endif

	rtw_cancel_all_timer(if1);
#ifdef CONFIG_WOWLAN
	adapter_to_pwrctl(if1)->wowlan_mode=_FALSE;
#endif //CONFIG_WOWLAN
	rtw_dev_unload(if1);

	DBG_871X("%s, hw_init_completed=%d\n", __func__, if1->hw_init_completed);

	//s6.
	rtw_handle_dualmac(if1, 0);

#ifdef CONFIG_IOCTL_CFG80211
	if(if1->rtw_wdev) {
		rtw_wdev_free(if1->rtw_wdev);
	}
#endif //CONFIG_IOCTL_CFG80211

	rtw_hal_inirp_deinit(if1);
	rtw_free_drv_sw(if1);	

	if(pnetdev)
		rtw_free_netdev(pnetdev);
	
#ifdef CONFIG_PLATFORM_RTD2880B
	DBG_871X("wlan link down\n");
	rtd2885_wlan_netlink_sendMsg("linkdown", "8712");
#endif
}

/*
 * drv_init() - a device potentially for us
 *
 * notes: drv_init() is called when the bus driver has located a card for us to support.
 *        We accept the new device by returning 0.
*/
static int rtw_drv_init(struct pci_dev *pdev, const struct pci_device_id *pdid)
{
	int i, err = -ENODEV;

	int status = _FAIL;
	_adapter *if1 = NULL, *if2 = NULL;
	struct dvobj_priv *dvobj;
	struct net_device *pnetdev;
	
	RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("+rtw_drv_init\n"));
	//DBG_871X("+rtw_drv_init\n");

	//step 0.
	disable_ht_for_spec_devid(pdid);

	/* Initialize dvobj_priv */
	if ((dvobj = pci_dvobj_init(pdev)) == NULL) {
		RT_TRACE(_module_hci_intfs_c_, _drv_err_, ("initialize device object priv Failed!\n"));
		goto exit;
	}

	/* Initialize if1 */
	if ((if1 = rtw_pci_if1_init(dvobj, pdev, pdid)) == NULL) {
		DBG_871X("rtw_pci_if1_init Failed!\n");
		goto free_dvobj;
	}

	/* Initialize if2 */
#ifdef CONFIG_CONCURRENT_MODE
	if((if2 = rtw_drv_if2_init(if1, pci_set_intf_ops)) == NULL) {
		goto free_if1;
	}
#endif

#ifdef CONFIG_GLOBAL_UI_PID
	if(ui_pid[1]!=0) {
		DBG_871X("ui_pid[1]:%d\n",ui_pid[1]);
		rtw_signal_process(ui_pid[1], SIGUSR2);
	}
#endif

	//dev_alloc_name && register_netdev
	if (rtw_drv_register_netdev(if1) != _SUCCESS)
		goto free_if2;

#ifdef CONFIG_HOSTAPD_MLME
	hostapd_mode_init(if1);
#endif

#ifdef CONFIG_PLATFORM_RTD2880B
	DBG_871X("wlan link up\n");
	rtd2885_wlan_netlink_sendMsg("linkup", "8712");
#endif

	/* alloc irq */
	if (pci_alloc_irq(dvobj) != _SUCCESS)
		goto unregister_ndevs;

	RT_TRACE(_module_hci_intfs_c_,_drv_err_,("-871x_drv - drv_init, success!\n"));
	//DBG_871X("-871x_drv - drv_init, success!\n");

	status = _SUCCESS;

unregister_ndevs:
	if (status != _SUCCESS)
		rtw_unregister_netdevs(dvobj);
free_if2:
	if(status != _SUCCESS && if2) {
		#ifdef CONFIG_CONCURRENT_MODE
		rtw_drv_if2_stop(if2);
		rtw_drv_if2_free(if2);
		#endif
	}
free_if1:
	if (status != _SUCCESS && if1) {
		rtw_pci_if1_deinit(if1);
	}
free_dvobj:
	if (status != _SUCCESS)
		pci_dvobj_deinit(pdev);
exit:
	return status == _SUCCESS?0:-ENODEV;
}

/*
 * dev_remove() - our device is being removed
*/
//rmmod module & unplug(SurpriseRemoved) will call r871xu_dev_remove() => how to recognize both
static void rtw_dev_remove(struct pci_dev *pdev)
{
	struct dvobj_priv *pdvobjpriv = pci_get_drvdata(pdev);
	_adapter *padapter = pdvobjpriv->if1;
	struct net_device *pnetdev = padapter->pnetdev;

_func_exit_;

	DBG_871X("+rtw_dev_remove\n");

	pdvobjpriv->processing_dev_remove = _TRUE;

	if (unlikely(!padapter)) {
		return;
	}

	rtw_unregister_netdevs(pdvobjpriv);

	#if 0
#ifdef RTK_DMP_PLATFORM
	padapter->bSurpriseRemoved = _FALSE;	// always trate as device exists
											// this will let the driver to disable it's interrupt
#else
	if(pci_drvpriv.drv_registered == _TRUE)
	{
		//DBG_871X("r871xu_dev_remove():padapter->bSurpriseRemoved == _TRUE\n");
		padapter->bSurpriseRemoved = _TRUE;
	}
	/*else
	{
		//DBG_871X("r871xu_dev_remove():module removed\n");
		padapter->hw_init_completed = _FALSE;
	}*/
#endif
	#endif

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_ANDROID_POWER)
	rtw_unregister_early_suspend(dvobj_to_pwrctl(pdvobjpriv));
#endif

	rtw_pm_set_ips(padapter, IPS_NONE);
	rtw_pm_set_lps(padapter, PS_MODE_ACTIVE);

	LeaveAllPowerSaveMode(padapter);

#ifdef CONFIG_CONCURRENT_MODE
	rtw_drv_if2_stop(pdvobjpriv->if2);
#endif

	#ifdef CONFIG_BT_COEXIST
	rtw_btcoex_HaltNotify(padapter);
	#endif

	rtw_pci_if1_deinit(padapter);

#ifdef CONFIG_CONCURRENT_MODE
	rtw_drv_if2_free(pdvobjpriv->if2);
#endif

	pci_dvobj_deinit(pdev);

	DBG_871X("-r871xu_dev_remove, done\n");

_func_exit_;
	return;
}


static int __init rtw_drv_entry(void)
{
	int ret = 0;

	DBG_871X_LEVEL(_drv_always_, "module init start\n");
	dump_drv_version(RTW_DBGDUMP);
#ifdef BTCOEXVERSION
	DBG_871X_LEVEL(_drv_always_, DRV_NAME" BT-Coex version = %s\n", BTCOEXVERSION);
#endif // BTCOEXVERSION

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)) 
	//console_suspend_enabled=0;
#endif

	pci_drvpriv.drv_registered = _TRUE;
	rtw_suspend_lock_init();
	rtw_drv_proc_init();
	rtw_ndev_notifier_register();

	ret = pci_register_driver(&pci_drvpriv.rtw_pci_drv);

	if (ret != 0) {
		pci_drvpriv.drv_registered = _FALSE;
		rtw_suspend_lock_uninit();
		rtw_drv_proc_deinit();
		rtw_ndev_notifier_unregister();
		goto exit;
	}

exit:
	DBG_871X_LEVEL(_drv_always_, "module init ret=%d\n", ret);
	return ret;
}

static void __exit rtw_drv_halt(void)
{
	DBG_871X_LEVEL(_drv_always_, "module exit start\n");

	pci_drvpriv.drv_registered = _FALSE;
	
	pci_unregister_driver(&pci_drvpriv.rtw_pci_drv);

	rtw_suspend_lock_uninit();
	rtw_drv_proc_deinit();
	rtw_ndev_notifier_unregister();

	DBG_871X_LEVEL(_drv_always_, "module exit success\n");

	rtw_mstat_dump(RTW_DBGDUMP);
}


module_init(rtw_drv_entry);
module_exit(rtw_drv_halt);

