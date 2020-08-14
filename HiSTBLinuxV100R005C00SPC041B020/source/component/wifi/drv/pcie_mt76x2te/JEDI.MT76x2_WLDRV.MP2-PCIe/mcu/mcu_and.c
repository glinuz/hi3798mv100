/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */


#include	"rt_config.h"

#define WAIT_EMPTY_RETRY_COUNT		(200)
#define WAIT_EMPTY_DELAY_TIME_US        (1000)  /* in micro-second */
#define WAIT_RESPONSE_TIME		(100)   /* ideal time is about 45 ms. Considering that
						* other cases, such as BT and WIFI coexist,
						* set this value as 100 ms */

#ifdef RTMP_PCI_SUPPORT
int andes_pci_load_rom_patch(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;
	UINT32 start_offset, end_offset;
	UINT32 loop = 0, idx = 0, val = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	UINT32 mac_value;
	POS_COOKIE obj = (POS_COOKIE) ad->OS_Cookie;

	if (cap->rom_code_protect) {
loadfw_protect:
		RTMP_IO_READ32(ad, SEMAPHORE_03, &mac_value);
		loop++;

		if (((mac_value & 0x01) == 0x00) && (loop < GET_SEMAPHORE_RETRY_MAX)) {
			RtmpOsMsDelay(1);
			goto loadfw_protect;
		}

		if (loop >= GET_SEMAPHORE_RETRY_MAX) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: can not get the hw semaphore\n", __func__));
			return NDIS_STATUS_FAILURE;
		}
	}

	/* check rom patch if ready */
	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		RTMP_IO_READ32(ad, CLOCK_CTL, &mac_value);
	} else {
		RTMP_IO_READ32(ad, COM_REG0, &mac_value);
	}

	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		if (((mac_value & 0x01) == 0x01) && (cap->rom_code_protect)) {
			goto done;
		}
	} else {
		if (((mac_value & 0x02) == 0x02) && (cap->rom_code_protect)) {
			goto done;
		}
	}

#ifndef BB_SOC
	if (cap->load_code_method == BIN_FILE_METHOD) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("Load rom patch from BinFile $FIRMWARE_PATH/%s\n",
			  cap->rom_patch_bin_file_name));
		OS_LOAD_CODE_FROM_BIN(&cap->rom_patch, cap->rom_patch_bin_file_name, obj->pci_dev,
				      &cap->rom_patch_len);
	} else
#endif /* !BB_SOC */
	{
		cap->rom_patch = cap->rom_patch_header_image;
	}

	if (!cap->rom_patch) {
		if (cap->load_code_method == BIN_FILE_METHOD) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Load rom_patch from BinFile failed!!!\n", __func__));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("Please check if fw(%s) exist in firmware path such as "
				  "\"/etc/firmware\" or \"/lib/modules\"\n",
				  cap->rom_patch_bin_file_name));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
		} else {
			DBGPRINT(RT_DEBUG_ERROR, ("%s:Please assign a rom patch, load_method(%d)\n",
						  __func__, cap->load_code_method));
		}
		ret = NDIS_STATUS_FAILURE;
		goto done;
	}

	/* get rom patch information */
	DBGPRINT(RT_DEBUG_OFF, ("build time =\n"));

	for (loop = 0; loop < 16; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + loop)));

	if (IS_MT76x2(ad)) {
		if (((strncmp(cap->rom_patch, "20130809", 8) >= 0))
		    && (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))) {
			DBGPRINT(RT_DEBUG_OFF, ("rom patch for E3 IC\n"));

		} else if (((strncmp(cap->rom_patch, "20130809", 8) < 0))
			   && (MT_REV_LT(ad, MT76x2, REV_MT76x2E3))) {

			DBGPRINT(RT_DEBUG_OFF, ("rom patch for E2 IC\n"));
		} else {
			DBGPRINT(RT_DEBUG_OFF, ("rom patch do not match IC version\n"));
			RTMP_IO_READ32(ad, 0x0, &mac_value);
			DBGPRINT(RT_DEBUG_OFF, ("IC version(%x)\n", mac_value));
			ret = NDIS_STATUS_FAILURE;
			goto done;
		}
	}

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("platform =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 16 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("hw/sw version =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 20 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("patch version =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 24 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->rom_patch_offset - 10000);

	start_offset = PATCH_INFO_SIZE;
	end_offset = cap->rom_patch_len;

	/* Load rom patch code */
	for (idx = start_offset; idx < end_offset; idx += 4) {
		val = (*(cap->rom_patch + idx)) +
		    (*(cap->rom_patch + idx + 3) << 24) +
		    (*(cap->rom_patch + idx + 2) << 16) + (*(cap->rom_patch + idx + 1) << 8);

		RTMP_IO_WRITE32(ad, cap->rom_patch_offset + (idx - PATCH_INFO_SIZE), val);
	}

	RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, 0x0);

	/* Trigger rom */
	RTMP_IO_WRITE32(ad, INT_LEVEL, 0x04);

	/* check rom if ready */
	loop = 0;
	do {
		if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
			RTMP_IO_READ32(ad, CLOCK_CTL, &mac_value);
			if ((mac_value & 0x01) == 0x01)
				break;
		} else {
			RTMP_IO_READ32(ad, COM_REG0, &mac_value);
			if ((mac_value & 0x02) == 0x02)
				break;
		}

		RtmpOsMsDelay(10);
		loop++;
	} while (loop <= 200);

	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: CLOCK_CTL(0x%x) = 0x%x\n", __func__, CLOCK_CTL, mac_value));
		if ((mac_value & 0x01) != 0x01)
			ret = NDIS_STATUS_FAILURE;
	} else {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: COM_REG0(0x%x) = 0x%x\n", __func__, COM_REG0, mac_value));
		if ((mac_value & 0x02) != 0x02)
			ret = NDIS_STATUS_FAILURE;
	}

done:
	if (cap->rom_code_protect)
		RTMP_IO_WRITE32(ad, SEMAPHORE_03, 0x1);

	return ret;
}

int andes_pci_erase_rom_patch(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	if (cap->load_code_method == BIN_FILE_METHOD) {
		if (cap->rom_patch)
			RtmpOsVfree(cap->rom_patch);
		cap->rom_patch = NULL;
	}

	return 0;
}

int andes_pci_erasefw(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;
	u32 ilm_len, dlm_len;
	u16 fw_ver, build_ver;
	u32 loop = 0, idx = 0, val = 0;
	u32 mac_value;
	u32 start_offset, end_offset;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	if (!ad->chipCap.ram_code_protect) {

		ilm_len = (*(cap->FWImageName + 3) << 24) | (*(cap->FWImageName + 2) << 16) |
		    (*(cap->FWImageName + 1) << 8) | (*cap->FWImageName);

		dlm_len = (*(cap->FWImageName + 7) << 24) | (*(cap->FWImageName + 6) << 16) |
		    (*(cap->FWImageName + 5) << 8) | (*(cap->FWImageName + 4));

		fw_ver = (*(cap->FWImageName + 14) << 8) | (*(cap->FWImageName + 15));

		build_ver = (*(cap->FWImageName + 9) << 8) | (*(cap->FWImageName + 8));

		DBGPRINT(RT_DEBUG_OFF,
			 ("fw version:%d.%d ", (fw_ver & 0xff00) >> 8, fw_ver & 0x00ff));
		DBGPRINT(RT_DEBUG_TRACE, ("Build:%x\n", build_ver));
		DBGPRINT(RT_DEBUG_TRACE, ("Build Time:"));

		for (loop = 0; loop < 16; loop++)
			DBGPRINT(RT_DEBUG_TRACE, ("%c", *(cap->FWImageName + 16 + loop)));

		DBGPRINT(RT_DEBUG_TRACE, ("\n"));

		DBGPRINT(RT_DEBUG_TRACE, ("ILM Length = %d(bytes)\n", ilm_len));
		DBGPRINT(RT_DEBUG_TRACE, ("DLM Length = %d(bytes)\n", dlm_len));

		RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->ilm_offset);

		if (IS_MT76x2(ad)) {
			start_offset = FW_INFO_SIZE;
			RTMP_IO_WRITE32(ad, INT_LEVEL, 0x1);
			RtmpOsMsDelay(20);
		} else {
			if (cap->ram_code_protect) {
				RTMP_IO_WRITE32(ad, CPU_CTL, 0x0);
				RTMP_IO_WRITE32(ad, CPU_CTL, (0x1 << 20));
				RTMP_IO_WRITE32(ad, RESET_CTL, 0x10);
				RTMP_IO_WRITE32(ad, RESET_CTL, 0x300);
				RTMP_IO_WRITE32(ad, COM_REG0, 0x0);

				start_offset = FW_INFO_SIZE + IV_SIZE;
			} else {
				RTMP_IO_WRITE32(ad, RESET_CTL, 0x10);	/* reset fce */
				RTMP_IO_WRITE32(ad, RESET_CTL, 0x200);	/* reset cpu */
				RTMP_IO_WRITE32(ad, COM_REG0, 0x0);	/* clear mcu ready bit */

				start_offset = FW_INFO_SIZE;
			}
		}

		end_offset = FW_INFO_SIZE + ilm_len;

		if (!IS_MT76x2(ad)) {
			/* erase ilm */
			for (idx = start_offset; idx < end_offset; idx += 4) {
				val = (*(cap->FWImageName + idx)) +
				    (*(cap->FWImageName + idx + 3) << 24) +
				    (*(cap->FWImageName + idx + 2) << 16) +
				    (*(cap->FWImageName + idx + 1) << 8);

				RTMP_IO_WRITE32(ad, 0x80000 + (idx - FW_INFO_SIZE), 0);
			}

			if (cap->ram_code_protect) {
				/* Loading IV part into last 64 bytes of ILM */
				start_offset = FW_INFO_SIZE;
				end_offset = FW_INFO_SIZE + IV_SIZE;

				for (idx = start_offset; idx < end_offset; idx += 4) {
					val = (*(cap->FWImageName + idx)) +
					    (*(cap->FWImageName + idx + 3) << 24) +
					    (*(cap->FWImageName + idx + 2) << 16) +
					    (*(cap->FWImageName + idx + 1) << 8);

					RTMP_IO_WRITE32(ad,
							0x80000 + (0x54000 - IV_SIZE) + (idx -
											 FW_INFO_SIZE),
							0);
				}
			}

			RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->dlm_offset);

			start_offset = 32 + ilm_len;
			end_offset = 32 + ilm_len + dlm_len;

			/* erase dlm */
			for (idx = start_offset; idx < end_offset; idx += 4) {
				val = (*(cap->FWImageName + idx)) +
				    (*(cap->FWImageName + idx + 3) << 24) +
				    (*(cap->FWImageName + idx + 2) << 16) +
				    (*(cap->FWImageName + idx + 1) << 8);

				RTMP_IO_WRITE32(ad,
						0x80000 + (0x54000 - IV_SIZE) + (idx -
										 FW_INFO_SIZE), 0);
			}
		}

		RTMP_IO_READ32(ad, COM_REG0, &mac_value);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: COM_REG0(0x%x) = 0x%x\n", __func__, COM_REG0, mac_value));
		RTMP_IO_READ32(ad, RESET_CTL, &mac_value);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: RESET_CTL(0x%x) = 0x%x\n", __func__, RESET_CTL, mac_value));
	}

	if (cap->load_code_method == BIN_FILE_METHOD) {
		if (cap->FWImageName)
			RtmpOsVfree(cap->FWImageName);
		cap->FWImageName = NULL;
	}

	return ret;
}

NDIS_STATUS andes_pci_loadfw(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;
	UINT32 ilm_len, dlm_len;
	USHORT fw_ver, build_ver;
	UINT32 loop = 0, idx = 0, val = 0;
	UINT32 mac_value;
	UINT32 start_offset, end_offset;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	POS_COOKIE obj = (POS_COOKIE) ad->OS_Cookie;

	if (cap->ram_code_protect) {
loadfw_protect:
		RTMP_IO_READ32(ad, SEMAPHORE_00, &mac_value);
		loop++;

		if (((mac_value & 0x01) == 0) && (loop < GET_SEMAPHORE_RETRY_MAX)) {
			RtmpOsMsDelay(1);
			goto loadfw_protect;
		}

		if (loop >= GET_SEMAPHORE_RETRY_MAX) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: can not get the hw semaphore\n", __func__));
			return NDIS_STATUS_FAILURE;
		}
	}

	/* check MCU if ready */
	RTMP_IO_READ32(ad, COM_REG0, &mac_value);

	if (((mac_value & 0x01) == 0x01) && (cap->ram_code_protect)) {
		goto done;
	}
#ifndef BB_SOC
	if (cap->load_code_method == BIN_FILE_METHOD) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("Load WiFi firmware from BinFile $FIRMWARE_PATH/%s\n",
			  cap->fw_bin_file_name));
		OS_LOAD_CODE_FROM_BIN(&cap->FWImageName, cap->fw_bin_file_name, obj->pci_dev,
				      &cap->fw_len);
	} else
#endif /* !BB_SOC */
	{
		cap->FWImageName = cap->fw_header_image;
	}

	if (!cap->FWImageName) {
		if (cap->load_code_method == BIN_FILE_METHOD) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Load firmware from BinFile failed!!!\n", __func__));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("Please check if fw(%s) exist in firmware path such as "
				  "\"/etc/firmware\" or \"/lib/modules\"\n",
				  cap->fw_bin_file_name));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
		} else {
			DBGPRINT(RT_DEBUG_ERROR, ("%s:Please assign a fw image, load_method(%d)\n",
						  __func__, cap->load_code_method));
		}
		ret = NDIS_STATUS_FAILURE;
		goto done;
	}

	ilm_len = (*(cap->FWImageName + 3) << 24) | (*(cap->FWImageName + 2) << 16) |
	    (*(cap->FWImageName + 1) << 8) | (*cap->FWImageName);

	dlm_len = (*(cap->FWImageName + 7) << 24) | (*(cap->FWImageName + 6) << 16) |
	    (*(cap->FWImageName + 5) << 8) | (*(cap->FWImageName + 4));

	fw_ver = (*(cap->FWImageName + 11) << 8) | (*(cap->FWImageName + 10));

	build_ver = (*(cap->FWImageName + 9) << 8) | (*(cap->FWImageName + 8));

	DBGPRINT(RT_DEBUG_OFF, ("FW Version:%d.%d.%02d ", (fw_ver & 0xf000) >> 8,
				(fw_ver & 0x0f00) >> 8, fw_ver & 0x00ff));
	DBGPRINT(RT_DEBUG_OFF, ("Build:%x\n", build_ver));
	DBGPRINT(RT_DEBUG_OFF, ("Build Time:"));

	for (loop = 0; loop < 16; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->FWImageName + 16 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	if (IS_MT76x2(ad)) {
		if (((strncmp(cap->FWImageName + 16, "20130811", 8) >= 0))
		    && (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))) {
			DBGPRINT(RT_DEBUG_OFF, ("fw for E3 IC\n"));

		} else if (((strncmp(cap->FWImageName + 16, "20130811", 8) < 0))
			   && (MT_REV_LT(ad, MT76x2, REV_MT76x2E3))) {

			DBGPRINT(RT_DEBUG_OFF, ("fw for E2 IC\n"));
		} else {
			DBGPRINT(RT_DEBUG_OFF, ("fw do not match IC version\n"));
			RTMP_IO_READ32(ad, 0x0, &mac_value);
			DBGPRINT(RT_DEBUG_OFF, ("IC version(%x)\n", mac_value));
			ret = NDIS_STATUS_FAILURE;
			goto done;
		}
	}

	DBGPRINT(RT_DEBUG_TRACE, ("ILM Length = %d(bytes)\n", ilm_len));
	DBGPRINT(RT_DEBUG_TRACE, ("DLM Length = %d(bytes)\n", dlm_len));

	RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->ilm_offset);

	if (cap->ram_code_protect)
		start_offset = FW_INFO_SIZE + IV_SIZE;
	else
		start_offset = FW_INFO_SIZE;

	end_offset = FW_INFO_SIZE + ilm_len;

	/* Load ILM code */
	for (idx = start_offset; idx < end_offset; idx += 4) {
		val = (*(cap->FWImageName + idx)) +
		    (*(cap->FWImageName + idx + 3) << 24) +
		    (*(cap->FWImageName + idx + 2) << 16) + (*(cap->FWImageName + idx + 1) << 8);

		RTMP_IO_WRITE32(ad, 0x80000 + (idx - FW_INFO_SIZE), val);
	}

	if (cap->ram_code_protect) {
		/* Loading IV part into last 64 bytes of ILM */
		start_offset = FW_INFO_SIZE;
		end_offset = FW_INFO_SIZE + IV_SIZE;

		for (idx = start_offset; idx < end_offset; idx += 4) {
			val = (*(cap->FWImageName + idx)) +
			    (*(cap->FWImageName + idx + 3) << 24) +
			    (*(cap->FWImageName + idx + 2) << 16) +
			    (*(cap->FWImageName + idx + 1) << 8);

			RTMP_IO_WRITE32(ad, 0x80000 + (0x54000 - IV_SIZE) + (idx - FW_INFO_SIZE),
					val);
		}
	}

	if (IS_MT76x2(ad)) {
		if ((cap->dlm_offset - 0x10000) > 0x100000)
			RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, 0x110000);
		else
		RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->dlm_offset - 0x10000);
	} else
		RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, cap->dlm_offset);

	start_offset = FW_INFO_SIZE + ilm_len;
	end_offset = FW_INFO_SIZE + ilm_len + dlm_len;

	/* Load DLM code */
	for (idx = start_offset; idx < end_offset; idx += 4) {
		val = (*(cap->FWImageName + idx)) +
		    (*(cap->FWImageName + idx + 3) << 24) +
		    (*(cap->FWImageName + idx + 2) << 16) + (*(cap->FWImageName + idx + 1) << 8);

		if (IS_MT76x2(ad)) {
			UINT32 dlm_revise = 0;

			if (IS_MT76x2T(ad))
				dlm_revise = 0x5000;
			
			if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))
				RTMP_IO_WRITE32(ad, 0x90800 + (idx - FW_INFO_SIZE - ilm_len + dlm_revise), val);
			else
				RTMP_IO_WRITE32(ad, 0x90000 + (idx - FW_INFO_SIZE - ilm_len + dlm_revise), val);
		} else
			RTMP_IO_WRITE32(ad, 0x80000 + (idx - FW_INFO_SIZE - ilm_len), val);
	}

	RTMP_IO_WRITE32(ad, PCIE_REMAP_BASE4, 0x0);

#ifdef RTMP_FLASH_SUPPORT
#ifdef MT76x2
	if (IS_MT76x2(ad))
	{
		UCHAR e2p_type;
		e2p_type = ad->E2pAccessMode;
		e2p_type = ((e2p_type != 0) && (e2p_type < NUM_OF_E2P_MODE)) ? e2p_type : RtmpEepromGetDefault(ad);


		if (e2p_type == E2P_FLASH_MODE)
		{
			USHORT ee_val = 0;
			
			rtmp_nv_init(ad);
			rtmp_ee_flash_read(ad, EEPROM_NIC3_OFFSET, &ee_val);
			ad->NicConfig3.word = ee_val;
			if (ad->NicConfig3.field.XtalOption == 0x1)
			{
				/*
					MAC 730 bit [30] = 1: Co-Clock Enable
				*/
				RTMP_IO_READ32(ad, COM_REG0, &mac_value);
				mac_value |= (1 << 30);
				RTMP_IO_WRITE32(ad, COM_REG0, mac_value);
			}
		}
	}
#endif /* MT76x2 */
#endif /* RTMP_FLASH_SUPPORT */

	if (cap->ram_code_protect) {
		/* Trigger Firmware */
		RTMP_IO_WRITE32(ad, INT_LEVEL, 0x03);
	} else {
		if (IS_MT76x2(ad))
			RTMP_IO_WRITE32(ad, INT_LEVEL, 0x2);
		else
			RTMP_IO_WRITE32(ad, RESET_CTL, 0x300);
	}

	/* check MCU if ready */
	loop = 0;
	do {
		RTMP_IO_READ32(ad, COM_REG0, &mac_value);
		if ((mac_value & 0x01) == 0x1)
			break;
		RtmpOsMsDelay(10);
		loop++;
	} while (loop <= 200);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: COM_REG0(0x%x) = 0x%x\n", __func__, COM_REG0, mac_value));

	RTMP_IO_READ32(ad, COM_REG0, &mac_value);
	mac_value |= (1 << 1);
	RTMP_IO_WRITE32(ad, COM_REG0, mac_value);

	if ((mac_value & 0x01) != 0x1)
		ret = NDIS_STATUS_FAILURE;

done:
	if (cap->ram_code_protect)
		RTMP_IO_WRITE32(ad, SEMAPHORE_00, 0x1);

	return ret;
}
#endif /* endif */


#ifdef RTMP_MAC_USB
#ifdef INTERRUPT_EP_SWITCH_SUPPORT
static int andes_usb_switch_ep_transfer_type(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;

	EP_TRANSFER_TYPE_CHANGE_STRUC reqType = { };

	DBGPRINT(RT_DEBUG_OFF, ("%s for chip: %x\n", __func__, ad->ChipID));

	/*  Switch EP2 and/or EP5 to interrtpt transfer type based on combo chip or not */
	if (IS_MT7662U(ad) || IS_MT7632U(ad)) {
		reqType.field.ep2 = 1;
		reqType.field.ep5 = 1;
		reqType.field.transferType = 0x3; /* Interrupt */
	} else if (IS_MT7612U(ad)) {
		reqType.field.ep5 = 1;
		reqType.field.transferType = 0x3; /* Interrupt */
	} else
		DBGPRINT(RT_DEBUG_OFF, ("%s Unsupported chip id: %x\n", __func__, ad->ChipID));

	if (reqType.word)
		ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
					  0x91, 0x04, reqType.word, NULL, 0);

	return ret;
}
#endif /* INTERRUPT_EP_SWITCH_SUPPORT */

static int andes_usb_enable_patch(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	/* enable patch command */
	u8 cmd[11];

	cmd[0] = 0x6F;
	cmd[1] = 0xFC;
	cmd[2] = 0x08;
	cmd[3] = 0x01;
	cmd[4] = 0x20;
	cmd[5] = 0x04;
	cmd[6] = 0x00;
	cmd[7] = (cap->rom_patch_offset & 0xFF);
	cmd[8] = (cap->rom_patch_offset & 0xFF00) >> 8;
	cmd[9] = (cap->rom_patch_offset & 0xFF0000) >> 16;
	cmd[10] = (cap->rom_patch_offset & 0xFF000000) >> 24;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_CLASS_REQUEST_OUT, 0x01, 0x12, 0x00, cmd, 11);

	return ret;
}

static int andes_usb_reset_wmt(RTMP_ADAPTER *ad)
{
	int ret = NDIS_STATUS_SUCCESS;

	/* reset command */
	u8 cmd[8] = { 0x6F, 0xFC, 0x05, 0x01, 0x07, 0x01, 0x00, 0x04 };

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	RTUSB_VendorRequest(ad,
			    USBD_TRANSFER_DIRECTION_OUT,
			    DEVICE_CLASS_REQUEST_OUT, 0x01, 0x12, 0x00, cmd, 8);

	return ret;
}

static u16 checksume16(u8 *pData, int len)
{
	int sum = 0;

	while (len > 1) {
		sum += *((u16 *) pData);

		pData = pData + 2;

		if (sum & 0x80000000)
			sum = (sum & 0xFFFF) + (sum >> 16);

		len -= 2;
	}

	if (len)
		sum += *((u8 *) pData);

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}

static int andes_usb_chk_crc(RTMP_ADAPTER *ad, u32 checksum_len)
{
	int ret = 0;
	u8 cmd[8];
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	NdisMoveMemory(cmd, &cap->rom_patch_offset, 4);
	NdisMoveMemory(&cmd[4], &checksum_len, 4);

	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x01, 0x20, 0x00, cmd, 8);

	return ret;

}

static u16 andes_usb_get_crc(RTMP_ADAPTER *ad)
{
	int ret = 0;
	u16 crc, count = 0;

	while (1) {

		ret = RTUSB_VendorRequest(ad,
					  (USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK),
					  DEVICE_VENDOR_REQUEST_IN, 0x01, 0x21, 0x00, &crc, 2);

		if (crc != 0xFFFF)
			break;

		RtmpOsMsDelay(100);

		if (count++ > 100) {
			DBGPRINT(RT_DEBUG_ERROR, ("Query CRC over %d times\n", count));
			break;
		}
	}

	return crc;
}

static VOID usb_upload_rom_patch_complete(purbb_t urb)
{
	RTMP_OS_COMPLETION *load_rom_patch_done =
	    (RTMP_OS_COMPLETION *) RTMP_OS_USB_CONTEXT_GET(urb);

	RTMP_OS_COMPLETE(load_rom_patch_done);
}

int andes_usb_load_rom_patch(RTMP_ADAPTER *ad)
{
	PURB urb;
	POS_COOKIE obj = (POS_COOKIE) ad->OS_Cookie;
	ra_dma_addr_t rom_patch_dma;
	PUCHAR rom_patch_data;
	TXINFO_NMAC_CMD *tx_info;
	s32 sent_len;
	u32 cur_len = 0;
	u32 mac_value, loop = 0;
	u16 value;
	int ret = 0, total_checksum = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	USB_DMA_CFG_STRUC cfg;
	u32 patch_len = 0;
	RTMP_OS_COMPLETION load_rom_patch_done;

	if (cap->rom_code_protect) {
load_patch_protect:
		RTUSBReadMACRegister(ad, SEMAPHORE_03, &mac_value);
		loop++;

		if (((mac_value & 0x01) == 0x00) && (loop < GET_SEMAPHORE_RETRY_MAX)) {
			RtmpOsMsDelay(1);
			goto load_patch_protect;
		}

		if (loop >= GET_SEMAPHORE_RETRY_MAX) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: can not get the hw semaphore\n", __func__));
			return NDIS_STATUS_FAILURE;
		}
	}
	DBGPRINT(RT_DEBUG_OFF, ("%s --> ChipID : 0x%4x\n", __func__, ad->ChipID));

	/* Check rom patch if ready */
	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		RTUSBReadMACRegister(ad, CLOCK_CTL, &mac_value);
		DBGPRINT(RT_DEBUG_OFF,
			 ("%s --> mac_value : 0x%4x , cap->rom_code_protect = %u\n", __func__,
			  mac_value, cap->rom_code_protect));

		if (((mac_value & 0x01) == 0x01) && (cap->rom_code_protect)) {
			DBGPRINT(RT_DEBUG_OFF, ("Rom patch already loaded, skip load again\n"));
			goto error0;
		}
	} else {
		RTUSBReadMACRegister(ad, COM_REG0, &mac_value);

		if (((mac_value & 0x02) == 0x02) && (cap->rom_code_protect))
			goto error0;
	}

	/* Enable USB_DMA_CFG */
	USB_CFG_READ(ad, &cfg.word);
	cfg.word |= 0x00c00020;
	USB_CFG_WRITE(ad, cfg.word);

	if (cap->load_code_method == BIN_FILE_METHOD) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("Load rom_patch from BinFile $FIRMWARE_PATH/%s\n",
			  cap->rom_patch_bin_file_name));
		OS_LOAD_CODE_FROM_BIN(&cap->rom_patch, cap->rom_patch_bin_file_name, obj->pUsb_Dev,
				      &cap->rom_patch_len);
	} else
		cap->rom_patch = cap->rom_patch_header_image;

	if (!cap->rom_patch) {
		if (cap->load_code_method == BIN_FILE_METHOD) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Load rom_patch from BinFile failed!!!\n", __func__));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("Please check if fw(%s) exist in firmware path such as "
				  "\"/etc/firmware\" or \"/lib/modules\"\n",
				  cap->rom_patch_bin_file_name));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
		} else {
			DBGPRINT(RT_DEBUG_ERROR, ("%s:Please assign a rom patch, load_method(%d)\n",
						  __func__, cap->load_code_method));
		}
		ret = NDIS_STATUS_FAILURE;
		goto error0;
	}

	RTUSBVenderReset(ad);
	RtmpOsMsDelay(5);

	/* get rom patch information */
	DBGPRINT(RT_DEBUG_OFF, ("build time =\n"));

	for (loop = 0; loop < 16; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + loop)));

	if (IS_MT76x2(ad)) {
		if (((strncmp(cap->rom_patch, "20130809", 8) >= 0))
		    && (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))) {
			DBGPRINT(RT_DEBUG_OFF, ("rom patch for E3 IC\n"));

		} else if (((strncmp(cap->rom_patch, "20130809", 8) < 0))
			   && (MT_REV_LT(ad, MT76x2, REV_MT76x2E3))) {

			DBGPRINT(RT_DEBUG_OFF, ("rom patch for E2 IC\n"));
		} else {
			DBGPRINT(RT_DEBUG_OFF, ("rom patch do not match IC version\n"));
			RTMP_IO_READ32(ad, 0x0, &mac_value);
			DBGPRINT(RT_DEBUG_OFF, ("IC version(%x)\n", mac_value));
			ret = NDIS_STATUS_FAILURE;
			goto error1;
		}
	}

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("platform =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 16 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("hw/sw version =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 20 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	DBGPRINT(RT_DEBUG_OFF, ("patch version =\n"));

	for (loop = 0; loop < 4; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->rom_patch + 24 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	/* Enable FCE */
	RTUSBWriteMACRegister(ad, FCE_PSE_CTRL, 0x01, FALSE);

	/* FCE tx_fs_base_ptr */
	RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_BASE_PTR, 0x400230, FALSE);

	/* FCE tx_fs_max_cnt */
	RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_MAX_COUNT, 0x01, FALSE);

	/* FCE pdma enable */
	RTUSBWriteMACRegister(ad, FCE_PDMA_GLOBAL_CONF, 0x44, FALSE);

	/* FCE skip_fs_en */
	RTUSBWriteMACRegister(ad, FCE_SKIP_FS, 0x03, FALSE);

	/* Allocate URB */
	urb = RTUSB_ALLOC_URB(0);

	if (!urb) {
		DBGPRINT(RT_DEBUG_ERROR, ("can not allocate URB\n"));
		ret = NDIS_STATUS_RESOURCES;
		goto error1;
	}

	/* Allocate TransferBuffer */
	rom_patch_data =
		RTUSB_URB_ALLOC_BUFFER_SUSPEND(obj->pUsb_Dev, UPLOAD_PATCH_UNIT, &rom_patch_dma);

	if (!rom_patch_data) {
		ret = NDIS_STATUS_RESOURCES;
		goto error2;
	}

	DBGPRINT(RT_DEBUG_OFF, ("loading rom patch"));

	RTMP_OS_INIT_COMPLETION(&load_rom_patch_done);

	cur_len = 0x00;
	patch_len = cap->rom_patch_len - PATCH_INFO_SIZE;

	/* loading rom patch */
	while (1) {
		s32 sent_len_max = UPLOAD_PATCH_UNIT - sizeof(*tx_info) - USB_END_PADDING;

		sent_len =
		    (patch_len - cur_len) >= sent_len_max ? sent_len_max : (patch_len - cur_len);

		DBGPRINT(RT_DEBUG_TRACE, ("patch_len = %d\n", patch_len));
		DBGPRINT(RT_DEBUG_TRACE, ("cur_len = %d\n", cur_len));
		DBGPRINT(RT_DEBUG_TRACE, ("sent_len = %d\n", sent_len));

		if (sent_len > 0) {
			tx_info = (TXINFO_NMAC_CMD *) rom_patch_data;
			tx_info->info_type = CMD_PACKET;
			tx_info->pkt_len = sent_len;
			tx_info->d_port = CPU_TX_PORT;

#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR) tx_info, TYPE_TXINFO);
#endif /* endif */
			NdisMoveMemory(rom_patch_data + sizeof(*tx_info),
				       cap->rom_patch + PATCH_INFO_SIZE + cur_len, sent_len);

			/* four zero bytes for end padding */
			NdisZeroMemory(rom_patch_data + sizeof(*tx_info) + sent_len, 4);

			value = (cur_len + cap->rom_patch_offset) & 0xFFFF;

			DBGPRINT(RT_DEBUG_TRACE,
					("rom_patch_offset = %x\n", cap->rom_patch_offset));

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x230, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				ret = NDIS_STATUS_FAILURE;
				goto error3;
			}

			value = (((cur_len + cap->rom_patch_offset) & 0xFFFF0000) >> 16);

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x232, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				ret = NDIS_STATUS_FAILURE;
				goto error3;
			}

			cur_len += sent_len;

			while ((sent_len % 4) != 0)
				sent_len++;

			value = (sent_len & 0xFFFF0000) >> 16;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x234, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				ret = NDIS_STATUS_FAILURE;
				goto error3;
			}

			value = sent_len & 0x0000FFFF;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x236, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				ret = NDIS_STATUS_FAILURE;
				goto error3;
			}

			/* Initialize URB descriptor */
			RTUSB_FILL_HTTX_BULK_URB(urb,
						 obj->pUsb_Dev,
						 cap->CommandBulkOutAddr,
						 rom_patch_data,
						 sent_len + sizeof(*tx_info) + 4,
						 usb_upload_rom_patch_complete,
						 &load_rom_patch_done, rom_patch_dma);

			ret = RTUSB_SUBMIT_URB(urb);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("submit urb fail\n"));
				ret = NDIS_STATUS_FAILURE;
				goto error3;
			}

			DBGPRINT(RT_DEBUG_INFO,
				 ("%s: submit urb, sent_len = %d, patch_ilm = %d, cur_len = %d\n",
				  __func__, sent_len, patch_len, cur_len));

			if (!RTMP_OS_WAIT_FOR_COMPLETION_TIMEOUT
			    (&load_rom_patch_done, RTMPMsecsToJiffies(1000))) {
				RTUSB_UNLINK_URB(urb);
				ret = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR, ("upload fw timeout\n"));
				goto error3;
			}
			DBGPRINT(RT_DEBUG_OFF, ("."));

			RTUSBReadMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, &mac_value);
			mac_value++;
			RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, mac_value,
					      FALSE);

			RtmpOsMsDelay(5);
		} else {
			break;
		}

	}

	RTMP_OS_EXIT_COMPLETION(&load_rom_patch_done);

	total_checksum = checksume16(cap->rom_patch + PATCH_INFO_SIZE, patch_len);

	RtmpOsMsDelay(5);
	DBGPRINT(RT_DEBUG_OFF, ("Send checksum req..\n"));
	andes_usb_chk_crc(ad, patch_len);
	RtmpOsMsDelay(20);

	if (total_checksum != andes_usb_get_crc(ad)) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("checksum fail!, local(0x%x) <> fw(0x%x)\n", total_checksum,
			  andes_usb_get_crc(ad)));

		ret = NDIS_STATUS_FAILURE;
		goto error3;
	}

	ret = andes_usb_enable_patch(ad);

	if (ret) {
		ret = NDIS_STATUS_FAILURE;
		goto error3;
	}

	ret = andes_usb_reset_wmt(ad);

	RtmpOsMsDelay(20);

	/* Check ROM_PATCH if ready */
	loop = 0;

	do {
		if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
			RTUSBReadMACRegister(ad, CLOCK_CTL, &mac_value);
			if ((mac_value & 0x01) == 0x1)
				break;
		} else {
			RTUSBReadMACRegister(ad, COM_REG0, &mac_value);
			if ((mac_value & 0x02) == 0x2)
				break;
		}

		RtmpOsMsDelay(10);
		loop++;
	} while (loop <= 100);

	if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: CLOCK_CTL(0x%x) = 0x%x\n", __func__, CLOCK_CTL, mac_value));

		if ((mac_value & 0x01) != 0x1)
			ret = NDIS_STATUS_FAILURE;
	} else {
		DBGPRINT(RT_DEBUG_TRACE,
			 ("%s: CLOCK_CTL(0x%x) = 0x%x\n", __func__, COM_REG0, mac_value));

		if ((mac_value & 0x02) != 0x2)
			ret = NDIS_STATUS_FAILURE;
	}

error3:
	/* Free TransferBuffer */
	RTUSB_URB_FREE_BUFFER(obj->pUsb_Dev, UPLOAD_PATCH_UNIT, rom_patch_data, rom_patch_dma);

error2:
	/* Free URB */
	RTUSB_FREE_URB(urb);

#ifdef USB3_HALF_SPEED_SUPPORT
	if (ret == NDIS_STATUS_SUCCESS && IS_MT7662T(ad)) {
		if (cap->rom_code_protect)
			RTUSBWriteMACRegister(ad, SEMAPHORE_03, 0x1, FALSE);

		DBGPRINT(RT_DEBUG_OFF, ("==========================================\n"));
		DBGPRINT(RT_DEBUG_OFF, ("===============SWITCH TO 2.5G=============\n"));
		DBGPRINT(RT_DEBUG_OFF, ("==========================================\n"));
		RTUSB_VendorRequest(ad, USBD_TRANSFER_DIRECTION_OUT, DEVICE_VENDOR_REQUEST_OUT,
			0x91, 0x03, 0x1, NULL, 0);
		RTMP_SET_FLAG(ad, (fRTMP_ADAPTER_RESET_IN_PROGRESS | fRTMP_ADAPTER_NIC_NOT_EXIST));
		return NDIS_STATUS_FAILURE;
	}
#endif /* USB3_HALF_SPEED_SUPPORT */

#ifdef INTERRUPT_EP_SWITCH_SUPPORT
	if (ret == NDIS_STATUS_SUCCESS) {
		if (cap->rom_code_protect)
			RTUSBWriteMACRegister(ad, SEMAPHORE_03, 0x1, FALSE);
		andes_usb_switch_ep_transfer_type(ad);
	}
#endif /* INTERRUPT_EP_SWITCH_SUPPORT */

error1:
	andes_usb_erase_rom_patch(ad);

error0:
	if (cap->rom_code_protect)
		RTUSBWriteMACRegister(ad, SEMAPHORE_03, 0x1, FALSE);

	return ret;
}

int andes_usb_erase_rom_patch(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	if (cap->load_code_method == BIN_FILE_METHOD) {
		if (cap->rom_patch)
			RtmpOsVfree(cap->rom_patch);
		cap->rom_patch = NULL;
	}

	return 0;
}

static VOID usb_uploadfw_complete(purbb_t urb)
{
	RTMP_OS_COMPLETION *load_fw_done = NULL;

	if (urb) {
		load_fw_done = (RTMP_OS_COMPLETION *) RTMP_OS_USB_CONTEXT_GET(urb);
		if (load_fw_done)
			RTMP_OS_COMPLETE(load_fw_done);
	}
}

static NDIS_STATUS usb_load_ivb(RTMP_ADAPTER *ad)
{
	NDIS_STATUS Status = NDIS_STATUS_SUCCESS;
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	if (cap->load_iv) {
		Status = RTUSB_VendorRequest(ad,
					     USBD_TRANSFER_DIRECTION_OUT,
					     DEVICE_VENDOR_REQUEST_OUT,
					     0x01, 0x12, 0x00, cap->FWImageName + 32, 64);
	} else {
		Status = RTUSB_VendorRequest(ad,
					     USBD_TRANSFER_DIRECTION_OUT,
					     DEVICE_VENDOR_REQUEST_OUT,
					     0x01, 0x12, 0x00, NULL, 0x00);

	}

	if (Status) {
		DBGPRINT(RT_DEBUG_ERROR, ("Upload IVB Fail\n"));
		return Status;
	}

	return Status;
}

NDIS_STATUS andes_usb_loadfw(RTMP_ADAPTER *ad)
{
	PURB urb;
	POS_COOKIE obj = (POS_COOKIE) ad->OS_Cookie;
	ra_dma_addr_t fw_dma;
	PUCHAR fw_data;
	TXINFO_NMAC_CMD *tx_info;
	s32 sent_len;
	u32 cur_len = 0;
	u32 mac_value, loop = 0;
	u16 value;
	int ret = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	USB_DMA_CFG_STRUC cfg;
	u32 ilm_len = 0, dlm_len = 0, fw_ver = 0;
	u16 build_ver;
	RTMP_OS_COMPLETION load_fw_done;

	if (cap->ram_code_protect) {
loadfw_protect:
		RTUSBReadMACRegister(ad, SEMAPHORE_00, &mac_value);
		loop++;

		if (((mac_value & 0x01) == 0x00) && (loop < GET_SEMAPHORE_RETRY_MAX)) {
			RtmpOsMsDelay(1);
			goto loadfw_protect;
		}

		if (loop >= GET_SEMAPHORE_RETRY_MAX) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: can not get the hw semaphore\n", __func__));
			return NDIS_STATUS_FAILURE;
		}
	}
#ifdef NEW_WOW_REBOOT_USB_POWERON_SUPPORT
	RTUSBWriteMACRegister(ad, COM_REG0, 0, FALSE);
#endif /* NEW_WOW_REBOOT_USB_POWERON_SUPPORT */

	/* Check MCU if ready */
	RTUSBReadMACRegister(ad, COM_REG0, &mac_value);

#ifdef NEW_WOW_SUPPORT
	if (RTMP_TEST_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SW_RESET)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: Doing SW Reset, force reload FW...\n", __func__));
	} else
#endif /* NEW_WOW_SUPPORT */
	if (((mac_value & 0x01) == 0x01) && (cap->ram_code_protect))
		goto error0;

	RTUSBVenderReset(ad);
	RtmpOsMsDelay(5);

	/* Enable USB_DMA_CFG */
	USB_CFG_READ(ad, &cfg.word);
	cfg.word |= 0x00c00020;
	USB_CFG_WRITE(ad, cfg.word);

	if (cap->load_code_method == BIN_FILE_METHOD) {
		DBGPRINT(RT_DEBUG_OFF,
			 ("Load WiFi firmware from BinFile $FIRMWARE_PATH/%s\n",
			  cap->fw_bin_file_name));
		OS_LOAD_CODE_FROM_BIN(&cap->FWImageName, cap->fw_bin_file_name, obj->pUsb_Dev,
				      &cap->fw_len);
	} else
		cap->FWImageName = cap->fw_header_image;

	if (!cap->FWImageName) {
		if (cap->load_code_method == BIN_FILE_METHOD) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: Load firmware from BinFile failed!!!\n", __func__));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("Please check if fw(%s) exist in firmware path such as "
				  "\"/etc/firmware\" or \"/lib/modules\"\n",
				  cap->fw_bin_file_name));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"));
		} else {
			DBGPRINT(RT_DEBUG_ERROR, ("%s:Please assign a fw image, load_method(%d)\n",
						  __func__, cap->load_code_method));
		}
		ret = NDIS_STATUS_FAILURE;
		goto error0;
	}

	/* Get FW information */
	ilm_len = (*(cap->FWImageName + 3) << 24) | (*(cap->FWImageName + 2) << 16) |
	    (*(cap->FWImageName + 1) << 8) | (*cap->FWImageName);

	dlm_len = (*(cap->FWImageName + 7) << 24) | (*(cap->FWImageName + 6) << 16) |
	    (*(cap->FWImageName + 5) << 8) | (*(cap->FWImageName + 4));

	fw_ver = (*(cap->FWImageName + 12) << 24) | (*(cap->FWImageName + 13) << 16)
	    | (*(cap->FWImageName + 14) << 8) | (*(cap->FWImageName + 15));
	ad->FirmwareVersion = fw_ver;

	build_ver = (*(cap->FWImageName + 9) << 8) | (*(cap->FWImageName + 8));

	DBGPRINT(RT_DEBUG_OFF, ("fw version:%x.%x.%X ", (fw_ver & 0xff000000) >> 24,
				(fw_ver & 0x00ff0000) >> 16, fw_ver & 0x0000ffff));
	DBGPRINT(RT_DEBUG_OFF, ("build:%x\n", build_ver));
	DBGPRINT(RT_DEBUG_OFF, ("build time:"));

	for (loop = 0; loop < 16; loop++)
		DBGPRINT(RT_DEBUG_OFF, ("%c", *(cap->FWImageName + 16 + loop)));

	DBGPRINT(RT_DEBUG_OFF, ("\n"));

	if (IS_MT76x2(ad)) {
		if (((strncmp(cap->FWImageName + 16, "20130811", 8) >= 0))
		    && (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))) {
			DBGPRINT(RT_DEBUG_OFF, ("fw for E3 IC\n"));

		} else if (((strncmp(cap->FWImageName + 16, "20130811", 8) < 0))
			   && (MT_REV_LT(ad, MT76x2, REV_MT76x2E3))) {

			DBGPRINT(RT_DEBUG_OFF, ("fw for E2 IC\n"));
		} else {
			DBGPRINT(RT_DEBUG_OFF, ("fw do not match IC version\n"));
			RTMP_IO_READ32(ad, 0x0, &mac_value);
			DBGPRINT(RT_DEBUG_OFF, ("IC version(%x)\n", mac_value));
			ret = NDIS_STATUS_FAILURE;
			goto error1;
		}
	}

	DBGPRINT(RT_DEBUG_OFF, ("ilm length = %d(bytes)\n", ilm_len));
	DBGPRINT(RT_DEBUG_OFF, ("dlm length = %d(bytes)\n", dlm_len));

	/* Enable FCE to send in-band cmd */
	RTUSBWriteMACRegister(ad, FCE_PSE_CTRL, 0x01, FALSE);

	/* FCE tx_fs_base_ptr */
	RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_BASE_PTR, 0x400230, FALSE);

	/* FCE tx_fs_max_cnt */
	RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_MAX_COUNT, 0x01, FALSE);

	/* FCE pdma enable */
	RTUSBWriteMACRegister(ad, FCE_PDMA_GLOBAL_CONF, 0x44, FALSE);

	/* FCE skip_fs_en */
	RTUSBWriteMACRegister(ad, FCE_SKIP_FS, 0x03, FALSE);


	/* Allocate URB */
	urb = RTUSB_ALLOC_URB(0);

	if (!urb) {
		DBGPRINT(RT_DEBUG_ERROR, ("can not allocate URB\n"));
		ret = NDIS_STATUS_RESOURCES;
		goto error1;
	}

	/* Allocate TransferBuffer */
	fw_data = RTUSB_URB_ALLOC_BUFFER_SUSPEND(obj->pUsb_Dev, UPLOAD_FW_UNIT, &fw_dma);

	if (!fw_data) {
		ret = NDIS_STATUS_RESOURCES;
		goto error2;
	}

	DBGPRINT(RT_DEBUG_OFF, ("loading fw"));

	RTMP_OS_INIT_COMPLETION(&load_fw_done);

	if (cap->load_iv)
		cur_len = 0x40;
	else
		cur_len = 0x00;

	/* Loading ILM */
	while (1) {
		s32 sent_len_max = UPLOAD_FW_UNIT - sizeof(*tx_info) - USB_END_PADDING;

		sent_len = (ilm_len - cur_len) >= sent_len_max ? sent_len_max : (ilm_len - cur_len);

		if (sent_len > 0) {
			tx_info = (TXINFO_NMAC_CMD *) fw_data;
			tx_info->info_type = CMD_PACKET;
			tx_info->pkt_len = sent_len;
			tx_info->d_port = CPU_TX_PORT;

#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR) tx_info, TYPE_TXINFO);
#endif /* endif */
			NdisMoveMemory(fw_data + sizeof(*tx_info),
				       cap->FWImageName + FW_INFO_SIZE + cur_len, sent_len);

			/* four zero bytes for end padding */
			NdisZeroMemory(fw_data + sizeof(*tx_info) + sent_len, USB_END_PADDING);

			value = (cur_len + cap->ilm_offset) & 0xFFFF;

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x230, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				goto error3;
			}

			value = (((cur_len + cap->ilm_offset) & 0xFFFF0000) >> 16);

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x232, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				goto error3;
			}

			cur_len += sent_len;

			while ((sent_len % 4) != 0)
				sent_len++;

			value = (sent_len & 0xFFFF0000) >> 16;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x234, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				goto error3;
			}

			value = sent_len & 0x0000FFFF;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x236, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				goto error3;
			}

			/* Initialize URB descriptor */
			RTUSB_FILL_HTTX_BULK_URB(urb,
						 obj->pUsb_Dev,
						 cap->CommandBulkOutAddr,
						 fw_data,
						 sent_len + sizeof(*tx_info) + USB_END_PADDING,
						 usb_uploadfw_complete, &load_fw_done, fw_dma);

			ret = RTUSB_SUBMIT_URB(urb);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("submit urb fail\n"));
				goto error3;
			}

			DBGPRINT(RT_DEBUG_INFO,
				 ("%s: submit urb, sent_len = %d, ilm_ilm = %d, cur_len = %d\n",
				  __func__, sent_len, ilm_len, cur_len));

			if (!RTMP_OS_WAIT_FOR_COMPLETION_TIMEOUT
			    (&load_fw_done, RTMPMsecsToJiffies(UPLOAD_FW_TIMEOUT))) {
				RTUSB_UNLINK_URB(urb);
				ret = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR,
					 ("upload fw timeout(%dms)\n", UPLOAD_FW_TIMEOUT));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: submit urb, sent_len = %d, ilm_ilm = %d, cur_len = %d\n",
					  __func__, sent_len, ilm_len, cur_len));

				goto error3;
			}
			DBGPRINT(RT_DEBUG_OFF, ("."));

			RTUSBReadMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, &mac_value);
			mac_value++;
			RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, mac_value,
					      FALSE);

			RtmpOsMsDelay(5);
		} else {
			break;
		}

	}

	RTMP_OS_EXIT_COMPLETION(&load_fw_done);

	/* Re-Initialize completion */
	RTMP_OS_INIT_COMPLETION(&load_fw_done);

	cur_len = 0x00;

	/* Loading DLM */
	while (1) {
		s32 sent_len_max = UPLOAD_FW_UNIT - sizeof(*tx_info) - USB_END_PADDING;

		sent_len = (dlm_len - cur_len) >= sent_len_max ? sent_len_max : (dlm_len - cur_len);

		if (sent_len > 0) {
			tx_info = (TXINFO_NMAC_CMD *) fw_data;
			tx_info->info_type = CMD_PACKET;
			tx_info->pkt_len = sent_len;
			tx_info->d_port = CPU_TX_PORT;

#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR) tx_info, TYPE_TXINFO);
#endif /* endif */
			NdisMoveMemory(fw_data + sizeof(*tx_info),
				       cap->FWImageName + FW_INFO_SIZE + ilm_len + cur_len,
				       sent_len);

			NdisZeroMemory(fw_data + sizeof(*tx_info) + sent_len, USB_END_PADDING);

			if ((MT_REV_GTE(ad, MT76x2, REV_MT76x2E3)) && (!IS_MT76x2T(ad))) {
				value = ((cur_len + (cap->dlm_offset + 0x800)) & 0xFFFF);
				DBGPRINT(RT_DEBUG_OFF, ("%s: fw dlm shift 0x800\n",  __func__));
			} else {
				value = ((cur_len + (cap->dlm_offset)) & 0xFFFF);
				DBGPRINT(RT_DEBUG_OFF, ("%s: fw dlm no shift\n",  __func__));
			}

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x230, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				goto error3;
			}

			if (MT_REV_GTE(ad, MT76x2, REV_MT76x2E3))
				value =
				    (((cur_len + (cap->dlm_offset + 0x800)) & 0xFFFF0000) >> 16);
			else
				value = (((cur_len + (cap->dlm_offset)) & 0xFFFF0000) >> 16);

			/* Set FCE DMA descriptor */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x232, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma descriptor fail\n"));
				goto error3;
			}

			cur_len += sent_len;

			while ((sent_len % 4) != 0)
				sent_len++;

			value = (sent_len & 0xFFFF0000) >> 16;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x234, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				goto error3;
			}

			value = sent_len & 0x0000FFFF;

			/* Set FCE DMA length */
			ret = RTUSB_VendorRequest(ad,
						  USBD_TRANSFER_DIRECTION_OUT,
						  DEVICE_VENDOR_REQUEST_OUT,
						  0x42, value, 0x236, NULL, 0);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("set fce dma length fail\n"));
				goto error3;
			}

			/* Initialize URB descriptor */
			RTUSB_FILL_HTTX_BULK_URB(urb,
						 obj->pUsb_Dev,
						 cap->CommandBulkOutAddr,
						 fw_data,
						 sent_len + sizeof(*tx_info) + USB_END_PADDING,
						 usb_uploadfw_complete, &load_fw_done, fw_dma);

			ret = RTUSB_SUBMIT_URB(urb);

			if (ret) {
				DBGPRINT(RT_DEBUG_ERROR, ("submit urb fail\n"));
				goto error3;
			}

			DBGPRINT(RT_DEBUG_INFO,
				 ("%s: submit urb, sent_len = %d, dlm_len = %d, cur_len = %d\n",
				  __func__, sent_len, dlm_len, cur_len));

			if (!RTMP_OS_WAIT_FOR_COMPLETION_TIMEOUT
			    (&load_fw_done, RTMPMsecsToJiffies(UPLOAD_FW_TIMEOUT))) {
				RTUSB_UNLINK_URB(urb);
				ret = NDIS_STATUS_FAILURE;
				DBGPRINT(RT_DEBUG_ERROR,
					 ("upload fw timeout(%dms)\n", UPLOAD_FW_TIMEOUT));
				DBGPRINT(RT_DEBUG_INFO,
					 ("%s: submit urb, sent_len = %d, dlm_len = %d, cur_len = %d\n",
					  __func__, sent_len, dlm_len, cur_len));

				goto error3;
			}
			DBGPRINT(RT_DEBUG_OFF, ("."));

			RTUSBReadMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, &mac_value);
			mac_value++;
			RTUSBWriteMACRegister(ad, TX_CPU_PORT_FROM_FCE_CPU_DESC_INDEX, mac_value,
					      FALSE);
			RtmpOsMsDelay(5);
		} else {
			break;
		}

	}

	RTMP_OS_EXIT_COMPLETION(&load_fw_done);

	/* As suggested by FW team, we need to set 0x230~23c to zero
	   before upload 64 bytes interrupt vector
	 */
	value = 0;
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x230, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x230 fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x232, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x232 fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x234, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x234 fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x236, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x236 fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x238, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x238 fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x23a, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x23a fail\n"));
		goto error3;
	}
	ret = RTUSB_VendorRequest(ad,
				  USBD_TRANSFER_DIRECTION_OUT,
				  DEVICE_VENDOR_REQUEST_OUT, 0x42, value, 0x23c, NULL, 0);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("Reset fce 0x23c fail\n"));
		goto error3;
	}

	/* Upload new 64 bytes interrupt vector or reset andes */
	DBGPRINT(RT_DEBUG_OFF, ("\n"));
	usb_load_ivb(ad);

	/* Check MCU if ready */
	loop = 0;
	do {
		RTUSBReadMACRegister(ad, COM_REG0, &mac_value);
		if ((mac_value & 0x01) == 0x01)
			break;
		RtmpOsMsDelay(10);
		loop++;
	} while (loop <= 100);

	DBGPRINT(RT_DEBUG_TRACE, ("%s: COM_REG0(0x%x) = 0x%x\n", __func__, COM_REG0, mac_value));

	RTMP_IO_READ32(ad, COM_REG0, &mac_value);
	mac_value |= (1 << 1);
	RTMP_IO_WRITE32(ad, COM_REG0, mac_value);

	if ((mac_value & 0x01) != 0x01)
		ret = NDIS_STATUS_FAILURE;

error3:
	/* Free TransferBuffer */
	RTUSB_URB_FREE_BUFFER(obj->pUsb_Dev, UPLOAD_FW_UNIT, fw_data, fw_dma);

error2:
	/* Free URB */
	RTUSB_FREE_URB(urb);

error1:
	andes_usb_erasefw(ad);

error0:
	if (cap->ram_code_protect)
		RTUSBWriteMACRegister(ad, SEMAPHORE_00, 0x1, FALSE);

	/* Enable FCE to send in-band cmd */
	RTUSBWriteMACRegister(ad, FCE_PSE_CTRL, 0x01, FALSE);

	return ret;
}

int andes_usb_read_feature(RTMP_ADAPTER *ad, UINT16 index, UINT32 *value)
{
	int ret;
	UINT32 io_value;

	ret = RTUSB_VendorRequest(ad,
			(USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK),
			DEVICE_VENDOR_REQUEST_IN,
			0x91,
			VENDOR_CMD_FEATURE_STATE_READ,
			index,
			&io_value,
			4);

	*value = le2cpu32(io_value);

	return ret;
}

#endif /* endif */

int andes_usb_erasefw(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *cap = &ad->chipCap;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

	if (cap->load_code_method == BIN_FILE_METHOD) {
		if (cap->FWImageName)
			RtmpOsVfree(cap->FWImageName);
		cap->FWImageName = NULL;
	}

	return 0;
}

struct cmd_msg *andes_alloc_cmd_msg(RTMP_ADAPTER *ad, unsigned int length)
{
	struct cmd_msg *msg = NULL;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	PNDIS_PACKET net_pkt = NULL;
#ifdef RTMP_USB_SUPPORT
	PURB urb = NULL;
#endif /* endif */

	net_pkt =
	    RTMP_AllocateFragPacketBuffer(ad, cap->cmd_header_len + length + cap->cmd_padding_len);

	if (!net_pkt) {
		DBGPRINT(RT_DEBUG_ERROR, ("can not allocate net_pkt\n"));
		goto error0;
	}

	OS_PKT_RESERVE(net_pkt, cap->cmd_header_len);

	os_alloc_mem(NULL, (PUCHAR *) &msg, sizeof(*msg));

	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("can not allocate cmd msg\n"));
		goto error1;
	}

	CMD_MSG_CB(net_pkt)->msg = msg;

	memset(msg, 0x00, sizeof(*msg));

#ifdef RTMP_USB_SUPPORT
	urb = RTUSB_ALLOC_URB(0);

	if (!urb) {
		DBGPRINT(RT_DEBUG_ERROR, ("can not allocate urb\n"));
		goto error2;
	}

	msg->urb = urb;
#endif /* endif */

	msg->priv = (void *)ad;
	msg->net_pkt = net_pkt;

	ctl->alloc_cmd_msg++;

	return msg;

#ifdef RTMP_USB_SUPPORT
error2:
#endif /* endif */
	os_free_mem(NULL, msg);
error1:
	RTMPFreeNdisPacket(ad, net_pkt);
error0:
	return NULL;
}

void andes_init_cmd_msg(struct cmd_msg *msg, u8 type, BOOLEAN need_wait, u16 timeout,
			BOOLEAN need_retransmit, BOOLEAN need_rsp, u16 rsp_payload_len,
			char *rsp_payload, MSG_RSP_HANDLER rsp_handler)
{
	msg->type = type;
#ifdef RTMP_USB_SUPPORT
	msg->need_wait = need_wait;
#else
	msg->need_wait = FALSE;
#endif /* endif */
	msg->timeout = timeout;

	if (need_wait) {
#ifdef RTMP_PCI_SUPPORT
		msg->ack_done = FALSE;
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
		RTMP_OS_INIT_COMPLETION(&msg->ack_done);
#endif /* endif */
	}
#ifdef RTMP_USB_SUPPORT
	msg->need_retransmit = need_retransmit;
#else
	msg->need_retransmit = 0;
#endif /* RTMP_USB_SUPPORT */

#ifdef RTMP_USB_SUPPORT
	if (need_retransmit)
		msg->retransmit_times = CMD_MSG_RETRANSMIT_TIMES;
#else
	msg->retransmit_times = 0;
#endif /* RTMP_USB_SUPPORT */

#ifdef RTMP_USB_SUPPORT
	msg->need_rsp = need_rsp;
#else
	msg->need_rsp = FALSE;
#endif /* RTMP_USB_SUPPORT */
	msg->rsp_payload_len = rsp_payload_len;
	msg->rsp_payload = rsp_payload;
	msg->rsp_handler = rsp_handler;
}

void andes_append_cmd_msg(struct cmd_msg *msg, char *data, unsigned int len)
{
	PNDIS_PACKET net_pkt = msg->net_pkt;

	if (data)
		memcpy(OS_PKT_TAIL_BUF_EXTEND(net_pkt, len), data, len);
}

void andes_free_cmd_msg(struct cmd_msg *msg)
{
	PNDIS_PACKET net_pkt = msg->net_pkt;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) (msg->priv);
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (msg->need_wait) {
#ifdef RTMP_PCI_SUPPORT
		msg->ack_done = FALSE;
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
		RTMP_OS_EXIT_COMPLETION(&msg->ack_done);
#endif /* endif */
	}
#ifdef RTMP_USB_SUPPORT
	usb_unlink_urb(msg->urb);
	RTUSB_FREE_URB(msg->urb);
#endif /* endif */

	os_free_mem(NULL, msg);

	RTMPFreeNdisPacket(ad, net_pkt);
	ctl->free_cmd_msg++;
}

BOOLEAN is_inband_cmd_processing(RTMP_ADAPTER *ad)
{
	BOOLEAN ret = 0;

	return ret;
}

static UCHAR get_cmd_rsp_num(RTMP_ADAPTER *ad)
{
	UCHAR Num = 0;

	return Num;
}

static inline void andes_inc_error_count(struct MCU_CTRL *ctl, enum cmd_msg_error_type type)
{
	if (OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
		switch (type) {
		case error_tx_kickout_fail:
			ctl->tx_kickout_fail_count++;
			break;
		case error_tx_timeout_fail:
			ctl->tx_timeout_fail_count++;
			break;
		case error_rx_receive_fail:
			ctl->rx_receive_fail_count++;
			break;
		default:
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s:unknown cmd_msg_error_type(%d)\n", __func__, type));
		}
	}
}

static NDIS_SPIN_LOCK *andes_get_spin_lock(struct MCU_CTRL *ctl, DL_LIST *list)
{
	NDIS_SPIN_LOCK *lock = NULL;

	if (list == &ctl->txq)
		lock = &ctl->txq_lock;
	else if (list == &ctl->rxq)
		lock = &ctl->rxq_lock;
	else if (list == &ctl->ackq)
		lock = &ctl->ackq_lock;
	else if (list == &ctl->kickq)
		lock = &ctl->kickq_lock;
	else if (list == &ctl->tx_doneq)
		lock = &ctl->tx_doneq_lock;
	else if (list == &ctl->rx_doneq)
		lock = &ctl->rx_doneq_lock;
	else
		DBGPRINT(RT_DEBUG_ERROR, ("%s:illegal list\n", __func__));

	return lock;
}

static inline UCHAR andes_get_cmd_msg_seq(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	struct cmd_msg *msg;
	unsigned long flags;

	RTMP_SPIN_LOCK_IRQSAVE(&ctl->ackq_lock, &flags);
get_seq:
	ctl->cmd_seq >= 0xf ? ctl->cmd_seq = 1 : ctl->cmd_seq++;
	DlListForEach(msg, &ctl->ackq, struct cmd_msg, list) {
		if (msg->seq == ctl->cmd_seq) {
			DBGPRINT(RT_DEBUG_ERROR,
				 ("command(seq: %d) is still running\n", ctl->cmd_seq));
			DBGPRINT(RT_DEBUG_ERROR,
				 ("command response nums = %d\n", get_cmd_rsp_num(ad)));
			goto get_seq;
		}
	}
	RTMP_SPIN_UNLOCK_IRQRESTORE(&ctl->ackq_lock, &flags);

	return ctl->cmd_seq;
}

static void _andes_queue_tail_cmd_msg(DL_LIST *list, struct cmd_msg *msg, enum cmd_msg_state state)
{
	msg->state = state;
	DlListAddTail(list, &msg->list);
}

static void andes_queue_tail_cmd_msg(DL_LIST *list, struct cmd_msg *msg, enum cmd_msg_state state)
{
	unsigned long flags;
	NDIS_SPIN_LOCK *lock;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	_andes_queue_tail_cmd_msg(list, msg, state);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

static void _andes_queue_head_cmd_msg(DL_LIST *list, struct cmd_msg *msg, enum cmd_msg_state state)
{
	msg->state = state;
	DlListAdd(list, &msg->list);
}

static void andes_queue_head_cmd_msg(DL_LIST *list, struct cmd_msg *msg, enum cmd_msg_state state)
{
	unsigned long flags;
	NDIS_SPIN_LOCK *lock;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	_andes_queue_head_cmd_msg(list, msg, state);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

static u32 andes_queue_len(struct MCU_CTRL *ctl, DL_LIST *list)
{
	u32 qlen;
	unsigned long flags;
	NDIS_SPIN_LOCK *lock;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	qlen = DlListLen(list);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);

	return qlen;
}


static void andes_queue_init(struct MCU_CTRL *ctl, DL_LIST *list)
{

	unsigned long flags;
	NDIS_SPIN_LOCK *lock;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	DlListInit(list);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

static void _andes_unlink_cmd_msg(struct cmd_msg *msg, DL_LIST *list)
{
	if (!msg)
		return;

	DlListDel(&msg->list);
}

static void andes_unlink_cmd_msg(struct cmd_msg *msg, DL_LIST *list)
{
	unsigned long flags;
	NDIS_SPIN_LOCK *lock;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	_andes_unlink_cmd_msg(msg, list);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

static struct cmd_msg *_andes_dequeue_cmd_msg(DL_LIST *list)
{
	struct cmd_msg *msg;

	msg = DlListFirst(list, struct cmd_msg, list);

	_andes_unlink_cmd_msg(msg, list);

	return msg;
}

static struct cmd_msg *andes_dequeue_cmd_msg(struct MCU_CTRL *ctl, DL_LIST *list)
{
	unsigned long flags;
	struct cmd_msg *msg;
	NDIS_SPIN_LOCK *lock;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	msg = _andes_dequeue_cmd_msg(list);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);

	return msg;
}

static void andes_rx_process_cmd_msg(RTMP_ADAPTER *ad, struct cmd_msg *rx_msg)
{
	PNDIS_PACKET net_pkt = rx_msg->net_pkt;
	struct cmd_msg *msg, *msg_tmp;
	RXFCE_INFO_CMD *rx_info = (RXFCE_INFO_CMD *) GET_OS_PKT_DATAPTR(net_pkt);
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	unsigned long flags;
	PUCHAR pRxRspEvtPayload;
	UINT16 rsp_len;
#ifdef NEW_WOW_SUPPORT
	UINT32	Req_Type;
	MAC_TABLE_ENTRY *pEntry = &ad->MacTab.Content[BSSID_WCID];
#endif /* NEW_WOW_SUPPORT */
#ifdef MT76XX_BTCOEX_SUPPORT
	PUINT32 pDW;
#endif /*MT76XX_BTCOEX_SUPPORT */
#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR) rx_info, TYPE_RXINFO);
#endif /* endif */

	DBGPRINT(RT_DEBUG_INFO, ("(andex_rx_cmd)info_type=%d,evt_type=%d,d_port=%d,"
				 "qsel=%d,pcie_intr=%d,cmd_seq=%d,"
				 "self_gen=%d,pkt_len=%d\n",
				 rx_info->info_type, rx_info->evt_type, rx_info->d_port,
				 rx_info->qsel, rx_info->pcie_intr, rx_info->cmd_seq,
				 rx_info->self_gen, rx_info->pkt_len));

	if (rx_info->info_type != CMD_PACKET) {
		DBGPRINT(RT_DEBUG_ERROR, ("packet is not command response/self event\n"));
		return;
	}
#ifdef CONFIG_FW_DEBUG
	if (rx_info->evt_type == RSP_EVT_TYPE_FW_LOG) {
		pRxRspEvtPayload = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("[%s]\n==========HOST_LOG_EVENT, RspEvtType = %d (len=%d)==================\n",
			  __func__, rx_info->evt_type, rx_info->pkt_len));

		DBGPRINT(RT_DEBUG_OFF, ("%s\n", pRxRspEvtPayload));
	}
#endif /* CONFIG_FW_DEBUG */

#ifdef CONFIG_FW_CORE_DUMP
	if (rx_info->evt_type == RSP_EVT_TYPE_FORCE_DUMP) {
		pRxRspEvtPayload = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		DBGPRINT(RT_DEBUG_TRACE,
			 ("[%s]\n==========FW CORE DUMP EVENT, RspEvtType = %d (len=%d)==============\n",
			  __func__, rx_info->evt_type, rx_info->pkt_len));

		DBGPRINT(RT_DEBUG_OFF, ("%s\n", pRxRspEvtPayload));
	}
#endif /* CONFIG_FW_CORE_DUMP */

#ifdef CFG_TDLS_SUPPORT
	if (rx_info->evt_type == RSP_EVT_TYPE_TDLS_BACK_TO_BASE_CHANNEL) {
		ad->StaCfg.wpa_supplicant_info.CFG_Tdls_info.IamInOffChannel = FALSE;
		ad->StaCfg.wpa_supplicant_info.CFG_Tdls_info.bDoingChannelSwitch = FALSE;
	}
	if (rx_info->evt_type == RSP_EVT_TYPE_TDLS_STAY_TIME_OUT) {
		PCFG_TDLS_STRUCT pCfgTdls = &ad->StaCfg.wpa_supplicant_info.CFG_Tdls_info;

		cfg_tdls_chsw_resp(ad, pCfgTdls->CHSWPeerMacAddr, pCfgTdls->ChSwitchTime,
				   pCfgTdls->ChSwitchTimeout, 0);
	}
#endif /* CFG_TDLS_SUPPORT */
	if (rx_info->evt_type == RSP_EVT_TYPE_TX_STATUS_RSP_EVENT) {
#ifdef RT_CFG80211_SUPPORT
		UCHAR *TxSEvent = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		UINT32 Length = rx_info->pkt_len;

		CFG80211_TxStatusEvent(ad, TxSEvent, Length);
#endif /* endif */
	}

#ifdef WOW_BUF_PKT_SUPPORT
	if (rx_info->evt_type == RSP_EVT_TYPE_WOW_RSP_PACKET) {
		pRxRspEvtPayload = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW_RSP_PACKET_EVENT, RspEvtType = %d (len=%d)\n",
			__func__, rx_info->evt_type, rx_info->pkt_len));
		RtmpOsBufPktRcvHandle(ad->net_dev, pRxRspEvtPayload, rx_info->pkt_len);
	}
#endif /* WOW_BUF_PKT_SUPPORT */
#ifdef NEW_WOW_SUPPORT
	if (rx_info->evt_type == RSP_EVT_TYPE_WOW_RSP_EVENT) {
		pRxRspEvtPayload = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW_RSP_EVENT, RspEvtType = %d (len=%d)\n",
			__func__, rx_info->evt_type, rx_info->pkt_len));
		Req_Type = *(UINT32 *)(pRxRspEvtPayload);
		if (Req_Type == REQ_TYPE_WAKEUP_REASON) {
			ad->WOW_Cfg.WakeupReason = *(UINT32 *)(pRxRspEvtPayload + 8);
			/*
				Name				Bit field
				Wow magic packet	0
				Wow bitmap pattern	1
				Wow IPv4 TCP		2
				Wow IPv6 TCP		3
				Wow EAPOL identity	4
				Wow WFD invite		5
				Wow disassociation	8
				Wow deauthentication9
				Wow IPv4 UDP		10
				Wow IPv6 UDP		11
				Wow IPv4 TCP Sync	12
				Wow IPv6 TCP Sync	13
				Event AP_LOST		28
				Event GTK_ERROR		29
				Event 4WAY_REQ		30
				Event NLO_DISCOVERY	31
			*/
			DBGPRINT(RT_DEBUG_ERROR, ("WOW - WAKEUP REASON: %02X:%02X:%02X:%02X\n",
				pRxRspEvtPayload[8],
				pRxRspEvtPayload[9],
				pRxRspEvtPayload[10],
				pRxRspEvtPayload[11]));
		} else if (Req_Type == REQ_TYPE_SEQ_NUM) {
			pEntry->TxSeq[QID_AC_BE] =  *(UINT16 *)(pRxRspEvtPayload+8);
			DBGPRINT(RT_DEBUG_ERROR, ("QUERY SEQ NUM: %d\n", pEntry->TxSeq[QID_AC_BE]));
		}
	}
#endif /* NEW_WOW_SUPPORT */
#ifdef MT76XX_BTCOEX_SUPPORT
	if (rx_info->evt_type == RSP_EVT_TYPE_COEX_RSP_EVENT) {

		pRxRspEvtPayload = GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info);
		pDW = (PUINT32) (&pRxRspEvtPayload[0]);

		switch (*pDW) {
		case RSP_UpdateCOEXMode:
			DBGPRINT(RT_DEBUG_INFO, ("Receive COEX OPERATION RSP_UpdateCOEXMode\n"));
			pDW = (PUINT32) (&pRxRspEvtPayload[4]);
			ad->CoexMode.UpdateMode = (*pDW);
			DBGPRINT(RT_DEBUG_INFO, ("Receive COEX update mode %d\n", *pDW));
			break;

		case RSP_BT_DEVICE_LINK_STATUS:
			pDW = (PUINT32) (&pRxRspEvtPayload[4]);
			ad->BTStatusFlags = (*pDW);
			CoexParseBTStatus(ad);
			DBGPRINT(RT_DEBUG_INFO,
				 ("Receive COEX BT_DEVICE_LINK_STATUS = %d\n", *pDW));
			break;
		default:
			DBGPRINT(RT_DEBUG_ERROR,
				 ("Receive COEX update packets (%d)  unkonw COEX Operation\n",
				  *pDW));
			break;

		}

		return;
	}
#endif /* endif */

	if (rx_info->self_gen) {
		/* if have callback function */
		RTEnqueueInternalCmd(ad, CMDTHREAD_RESPONSE_EVENT_CALLBACK,
				     GET_OS_PKT_DATAPTR(net_pkt) + sizeof(*rx_info),
				     rx_info->pkt_len);
	} else {
#ifdef RTMP_USB_SUPPORT
		/*RTMP_SPIN_LOCK_IRQ(&ctl->ackq_lock); *//*WCNCR13183 */
		RTMP_SPIN_LOCK_IRQSAVE(&ctl->ackq_lock, &flags);
#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
		RTMP_SPIN_LOCK_IRQSAVE(&ctl->ackq_lock, &flags);
#endif /* endif */
		DlListForEachSafe(msg, msg_tmp, &ctl->ackq, struct cmd_msg, list) {
			if (msg->seq == rx_info->cmd_seq) {
				_andes_unlink_cmd_msg(msg, &ctl->ackq);
#ifdef RTMP_USB_SUPPORT
				/*RTMP_SPIN_UNLOCK_IRQ(&ctl->ackq_lock); *//*WCNCR13183 */
				RTMP_SPIN_UNLOCK_IRQRESTORE(&ctl->ackq_lock, &flags);
#endif /* endif */

				rsp_len = msg->rsp_payload_len;
				if ((rsp_len == rx_info->pkt_len) && (rsp_len != 0)) {
					if (msg->rsp_handler == NULL) {
						DBGPRINT(RT_DEBUG_ERROR,
							("no response handler function\n"));
					} else {
						msg->rsp_handler(msg,
							GET_OS_PKT_DATAPTR(net_pkt) +
								sizeof(*rx_info), rx_info->pkt_len);
					}
				} else if ((rsp_len == 0) && (rx_info->pkt_len == 8)) {
					DBGPRINT(RT_DEBUG_INFO,
						 ("command response(ack) success\n"));
				} else {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("expect response len(%d), command response len(%d) invalid\n",
						  msg->rsp_payload_len, rx_info->pkt_len));
					msg->rsp_payload_len = rx_info->pkt_len;
				}

				if (msg->need_wait) {
#ifdef RTMP_PCI_SUPPORT
					msg->ack_done = TRUE;
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
					RTMP_OS_COMPLETE(&msg->ack_done);
#endif /* endif */
				} else {
					andes_free_cmd_msg(msg);
				}
#ifdef RTMP_USB_SUPPORT
				/*RTMP_SPIN_LOCK_IRQ(&ctl->ackq_lock); *//*WCNCR13183 */
				RTMP_SPIN_LOCK_IRQSAVE(&ctl->ackq_lock, &flags);
#endif /* endif */

				break;
			}
		}

#ifdef RTMP_USB_SUPPORT
		/*RTMP_SPIN_UNLOCK_IRQ(&ctl->ackq_lock); *//*WCNCR13183 */
		RTMP_SPIN_UNLOCK_IRQRESTORE(&ctl->ackq_lock, &flags);
#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
		RTMP_SPIN_UNLOCK_IRQRESTORE(&ctl->ackq_lock, &flags);
#endif /* endif */
	}
}

#ifdef RTMP_PCI_SUPPORT
void pci_rx_cmd_msg_complete(RTMP_ADAPTER *ad, RXFCE_INFO *fce_info, PUCHAR payload)
{
	struct cmd_msg *msg;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
		return;
	}

	DBGPRINT(RT_DEBUG_TRACE, ("%s\n", __func__));

	msg = andes_alloc_cmd_msg(ad, sizeof(*fce_info) + fce_info->pkt_len);

	if (!msg)
		return;
#ifdef MT76x2
	andes_append_cmd_msg(msg, (char *)fce_info, sizeof(*fce_info));
	andes_append_cmd_msg(msg, (char *)payload, fce_info->pkt_len);
#else
	andes_append_cmd_msg(msg, (char *)fce_info, sizeof(*fce_info) + fce_info->pkt_len);
#endif
	andes_rx_process_cmd_msg(ad, msg);

	andes_free_cmd_msg(msg);
}
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
static void usb_rx_cmd_msg_complete(PURB urb)
{
	PNDIS_PACKET net_pkt = (PNDIS_PACKET) RTMP_OS_USB_CONTEXT_GET(urb);
	struct cmd_msg *msg = CMD_MSG_CB(net_pkt)->msg;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	POS_COOKIE pObj = (POS_COOKIE) ad->OS_Cookie;
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	enum cmd_msg_state state;
	unsigned long flags;
	int ret = 0;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return;

	if (msg->state != rx_start) {
		DBGPRINT(RT_DEBUG_OFF, ("%s(): Invalid RX state = %d. Ignore this msg (%lX) (%lX)\n"
			, __func__, msg->state
			, (unsigned long int)virt_to_phys(msg)
			, (unsigned long int)virt_to_phys(urb)));
		return;
	}

	andes_unlink_cmd_msg(msg, &ctl->rxq);

	OS_PKT_TAIL_BUF_EXTEND(net_pkt, RTMP_USB_URB_LEN_GET(urb));

	if (RTMP_USB_URB_STATUS_GET(urb) == 0) {
		state = rx_done;
	} else {
		state = rx_receive_fail;
		andes_inc_error_count(ctl, error_rx_receive_fail);
		DBGPRINT(RT_DEBUG_ERROR,
			 ("receive cmd msg fail(%d)\n", RTMP_USB_URB_STATUS_GET(urb)));
	}

	RTMP_SPIN_LOCK_IRQSAVE(&ctl->rx_doneq_lock, &flags);
	_andes_queue_tail_cmd_msg(&ctl->rx_doneq, msg, state);
	RTMP_SPIN_UNLOCK_IRQRESTORE(&ctl->rx_doneq_lock, &flags);

	/* STOP submit rx EP5, just return */
	if (INFRA_ON(ad) && ad->bSuppressEP5)
		return;

	msg = andes_alloc_cmd_msg(ad, 512);

	if (!msg)
		return;

	net_pkt = msg->net_pkt;

	if (pChipCap->CommandRspBulkInAddr == ad->IntrInEpAddr)
		RTUSB_FILL_INT_URB(msg->urb, pObj->pUsb_Dev,
				usb_rcvintpipe(pObj->pUsb_Dev, pChipCap->CommandRspBulkInAddr),
				   GET_OS_PKT_DATAPTR(net_pkt), 512, usb_rx_cmd_msg_complete,
				   net_pkt);
	else
	RTUSB_FILL_BULK_URB(msg->urb, pObj->pUsb_Dev,
			    usb_rcvbulkpipe(pObj->pUsb_Dev, pChipCap->CommandRspBulkInAddr),
				    GET_OS_PKT_DATAPTR(net_pkt), 512, usb_rx_cmd_msg_complete,
				    net_pkt);

	andes_queue_tail_cmd_msg(&ctl->rxq, msg, rx_start);

	ret = RTUSB_SUBMIT_URB(msg->urb);

	if (ret) {
		andes_unlink_cmd_msg(msg, &ctl->rxq);
		andes_inc_error_count(ctl, error_rx_receive_fail);
		DBGPRINT(RT_DEBUG_ERROR, ("%s:submit urb fail(%d)\n", __func__, ret));
		andes_queue_tail_cmd_msg(&ctl->rx_doneq, msg, rx_receive_fail);
	}

	andes_bh_schedule(ad);
}

int usb_rx_cmd_msg_submit(RTMP_ADAPTER *ad)
{
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;
	POS_COOKIE pObj = (POS_COOKIE) ad->OS_Cookie;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	struct cmd_msg *msg = NULL;
	PNDIS_PACKET net_pkt = NULL;
	int ret = 0;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return ret;

	msg = andes_alloc_cmd_msg(ad, 512);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		return ret;
	}

	net_pkt = msg->net_pkt;

	if (pChipCap->CommandRspBulkInAddr == ad->IntrInEpAddr)
		RTUSB_FILL_INT_URB(msg->urb, pObj->pUsb_Dev,
				usb_rcvintpipe(pObj->pUsb_Dev, pChipCap->CommandRspBulkInAddr),
				   GET_OS_PKT_DATAPTR(net_pkt), 512, usb_rx_cmd_msg_complete,
				   net_pkt);
	else
	RTUSB_FILL_BULK_URB(msg->urb, pObj->pUsb_Dev,
			    usb_rcvbulkpipe(pObj->pUsb_Dev, pChipCap->CommandRspBulkInAddr),
				    GET_OS_PKT_DATAPTR(net_pkt), 512, usb_rx_cmd_msg_complete,
				    net_pkt);

	andes_queue_tail_cmd_msg(&ctl->rxq, msg, rx_start);

	ret = RTUSB_SUBMIT_URB(msg->urb);

	if (ret) {
		andes_unlink_cmd_msg(msg, &ctl->rxq);
		andes_inc_error_count(ctl, error_rx_receive_fail);
		DBGPRINT(RT_DEBUG_ERROR, ("%s:submit urb fail(%d)\n", __func__, ret));
		andes_queue_tail_cmd_msg(&ctl->rx_doneq, msg, rx_receive_fail);
	}

	return ret;
}

int usb_rx_cmd_msgs_receive(RTMP_ADAPTER *ad)
{
	int ret = 0;
	int i;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	for (i = 0; (i < 1) && (andes_queue_len(ctl, &ctl->rxq) < 1); i++) {
		ret = usb_rx_cmd_msg_submit(ad);
		if (ret)
			break;
	}

	return ret;
}
#endif /* endif */

void andes_cmd_msg_bh(unsigned long param)
{
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) param;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	struct cmd_msg *msg = NULL;

	while ((msg = andes_dequeue_cmd_msg(ctl, &ctl->rx_doneq))) {
		switch (msg->state) {
		case rx_done:
			andes_rx_process_cmd_msg(ad, msg);
			andes_free_cmd_msg(msg);
			continue;
		case rx_receive_fail:
			andes_free_cmd_msg(msg);
			continue;
		default:
			DBGPRINT(RT_DEBUG_ERROR, ("unknow msg state(%d)\n", msg->state));
		}
	}

	while ((msg = andes_dequeue_cmd_msg(ctl, &ctl->tx_doneq))) {
		switch (msg->state) {
		case tx_done:
		case tx_kickout_fail:
		case tx_timeout_fail:
			andes_free_cmd_msg(msg);
			continue;
		default:
			DBGPRINT(RT_DEBUG_ERROR, ("unknow msg state(%d)\n", msg->state));
		}
	}

	if (OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
		andes_bh_schedule(ad);
#ifdef RTMP_USB_SUPPORT
		usb_rx_cmd_msgs_receive(ad);
#endif /* endif */
	}
}

void andes_bh_schedule(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return;

	if (((andes_queue_len(ctl, &ctl->rx_doneq) > 0)
	     || (andes_queue_len(ctl, &ctl->tx_doneq) > 0))
	    && OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
#ifndef WORKQUEUE_BH
		RTMP_NET_TASK_DATA_ASSIGN(&ctl->cmd_msg_task, (unsigned long)(ad));
		RTMP_OS_TASKLET_SCHE(&ctl->cmd_msg_task);
#else
		tasklet_hi_schedule(&ctl->cmd_msg_task);
#endif /* endif */
	}
}

#ifdef RTMP_PCI_SUPPORT
static UCHAR *txinfo_type_str[] = { "PKT", "CMD", "RSV" };
static UCHAR *txinfo_d_port_str[] =
    { "WLAN", "CPU_RX", "CPU_TX", "HOST", "VIRT_RX", "VIRT_TX", "DROP" };

VOID dump_cmd_txinfo(RTMP_ADAPTER *ad, TXINFO_STRUC *pTxInfo)
{
	DBGPRINT(RT_DEBUG_OFF, ("TxInfo:\n"));
	{
		struct _TXINFO_NMAC_CMD *cmd_txinfo = (struct _TXINFO_NMAC_CMD *)pTxInfo;

		hex_dump("Raw Data: ", (UCHAR *) pTxInfo, sizeof(TXINFO_STRUC));
		DBGPRINT(RT_DEBUG_OFF,
			 ("\t Info_Type=%d(%s)\n", cmd_txinfo->info_type,
			  txinfo_type_str[cmd_txinfo->info_type]));
		DBGPRINT(RT_DEBUG_OFF,
			 ("\t d_port=%d(%s)\n", cmd_txinfo->d_port,
			  txinfo_d_port_str[cmd_txinfo->d_port]));
		DBGPRINT(RT_DEBUG_OFF, ("\t cmd_type=%d\n", cmd_txinfo->cmd_type));
		DBGPRINT(RT_DEBUG_OFF, ("\t cmd_seq=%d\n", cmd_txinfo->cmd_seq));
		DBGPRINT(RT_DEBUG_OFF, ("\t pkt_len=0x%x\n", cmd_txinfo->pkt_len));
	}

	DBGPRINT(RT_DEBUG_OFF, ("\t"));
}

int pci_kick_out_cmd_msg(PRTMP_ADAPTER ad, struct cmd_msg *msg)
{
	int ret = NDIS_STATUS_SUCCESS;
	unsigned long flags = 0;
	ULONG FreeNum;
	PNDIS_PACKET net_pkt = msg->net_pkt;
	u32 SwIdx = 0, SrcBufPA;
	UCHAR *pSrcBufVA;
	UINT SrcBufLen = 0;
	PACKET_INFO PacketInfo;
	TXD_STRUC *pTxD;
	TXINFO_STRUC *pTxInfo;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
#ifdef RT_BIG_ENDIAN
	TXD_STRUC *pDestTxD;
	UCHAR tx_hw_info[TXD_SIZE];
#endif /* endif */

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return -1;

	FreeNum = GET_CTRLRING_FREENO(ad);

	if (FreeNum == 0) {
		DBGPRINT(RT_DEBUG_WARN,
			 ("%s FreeNum == 0 (TxCpuIdx = %d, TxDmaIdx = %d, TxSwFreeIdx = %d)\n",
			  __func__, ad->CtrlRing.TxCpuIdx, ad->CtrlRing.TxDmaIdx,
			  ad->CtrlRing.TxSwFreeIdx));
		return NDIS_STATUS_FAILURE;
	}

	RTMP_SPIN_LOCK_IRQSAVE(&ad->CtrlRingLock, &flags);

	RTMP_QueryPacketInfo(net_pkt, &PacketInfo, &pSrcBufVA, &SrcBufLen);

	if (pSrcBufVA == NULL) {
		RTMP_SPIN_UNLOCK_IRQRESTORE(&ad->CtrlRingLock, &flags);
		return NDIS_STATUS_FAILURE;
	}

	SwIdx = ad->CtrlRing.TxCpuIdx;
#ifdef RT_BIG_ENDIAN
	pDestTxD = (PTXD_STRUC) ad->CtrlRing.Cell[SwIdx].AllocVa;
	NdisMoveMemory(&tx_hw_info[0], (UCHAR *) pDestTxD, TXD_SIZE);
	pTxD = (PTXD_STRUC) & tx_hw_info[0];
#else
	pTxD = (PTXD_STRUC) ad->CtrlRing.Cell[SwIdx].AllocVa;
#endif /* endif */

	pTxInfo = (TXINFO_STRUC *) ((UCHAR *) pTxD + sizeof(TXD_STRUC));
	NdisMoveMemory(pTxInfo, pSrcBufVA, TXINFO_SIZE);

#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR) pTxD, TYPE_TXD);
#endif /* RT_BIG_ENDIAN */

	ad->CtrlRing.Cell[SwIdx].pNdisPacket = net_pkt;
	ad->CtrlRing.Cell[SwIdx].pNextNdisPacket = NULL;

	SrcBufPA = PCI_MAP_SINGLE(ad, (pSrcBufVA) + 4, (SrcBufLen) - 4, 0, RTMP_PCI_DMA_TODEVICE);

	pTxD->LastSec0 = 1;
	pTxD->LastSec1 = 0;
	pTxD->SDLen0 = (SrcBufLen - TXINFO_SIZE);
	pTxD->SDLen1 = 0;
	pTxD->SDPtr0 = SrcBufPA;
	pTxD->DMADONE = 0;

#ifdef RT_BIG_ENDIAN
	RTMPDescriptorEndianChange((PUCHAR) pTxD, TYPE_TXD);
	WriteBackToDescriptor((PUCHAR) pDestTxD, (PUCHAR) pTxD, FALSE, TYPE_TXD);
#endif /* endif */

	/* flush dcache if no consistent memory is supported */
	RTMP_DCACHE_FLUSH(SrcBufPA, SrcBufLen);
	RTMP_DCACHE_FLUSH(ad->CtrlRing.Cell[SwIdx].AllocPa, TXD_SIZE);

	/* Increase TX_CTX_IDX, but write to register later. */
	INC_RING_INDEX(ad->CtrlRing.TxCpuIdx, MGMT_RING_SIZE);

	if (msg->need_rsp)
		andes_queue_tail_cmd_msg(&ctl->ackq, msg, wait_cmd_out_and_ack);
	else
		andes_queue_tail_cmd_msg(&ctl->kickq, msg, wait_cmd_out);

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
		RTMP_SPIN_UNLOCK_IRQRESTORE(&ad->CtrlRingLock, &flags);
		return -1;
	}

	RTMP_IO_WRITE32(ad, ad->CtrlRing.hw_cidx_addr, ad->CtrlRing.TxCpuIdx);

	RTMP_SPIN_UNLOCK_IRQRESTORE(&ad->CtrlRingLock, &flags);

	return ret;
}

void pci_kick_out_cmd_msg_complete(PNDIS_PACKET net_pkt)
{
	struct cmd_msg *msg = CMD_MSG_CB(net_pkt)->msg;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return;

	if (!msg->need_rsp) {
		andes_unlink_cmd_msg(msg, &ctl->kickq);
		andes_queue_tail_cmd_msg(&ctl->tx_doneq, msg, tx_done);
	} else {
		msg->state = wait_ack;
	}

	andes_bh_schedule(ad);
}
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
static void usb_kick_out_cmd_msg_complete(PURB urb)
{
	PNDIS_PACKET net_pkt = (PNDIS_PACKET) RTMP_OS_USB_CONTEXT_GET(urb);
	struct cmd_msg *msg = CMD_MSG_CB(net_pkt)->msg;
	RTMP_ADAPTER *ad = (RTMP_ADAPTER *) msg->priv;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (RTMP_USB_URB_STATUS_GET(urb) == -ENOENT
		|| RTMP_USB_URB_STATUS_GET(urb) == -ECONNRESET) {
		/* URB status is invalid and we should not access this URB anymore */
		DBGPRINT(RT_DEBUG_ERROR,
			("%s(): Invalid Urb status %d\n",
			__func__, RTMP_USB_URB_STATUS_GET(urb)));
		return;
	}

	OS_CLEAR_BIT(MCU_TX_IN_PROGRESS, &ctl->flags);
	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return;

	if (msg->state != wait_cmd_out_and_ack && msg->state != wait_cmd_out) {
		/* If the msg does not come from source state,
		 * that means this msg was running into other states already.
		 * Do nothing here!
		 */
		DBGPRINT(RT_DEBUG_ERROR,
			("%s(): Invalid TX status %d %lX %d\n",
			__func__, msg->state,
			(unsigned long int)virt_to_phys(msg), RTMP_USB_URB_STATUS_GET(msg->urb)));
		andes_bh_schedule(ad);
		return;
	}


	if (RTMP_USB_URB_STATUS_GET(urb) == 0) {
		if (!msg->need_rsp) {
			andes_unlink_cmd_msg(msg, &ctl->kickq);
			andes_queue_tail_cmd_msg(&ctl->tx_doneq, msg, tx_done);
		} else {
			msg->state = wait_ack;
		}
	} else {
		if (!msg->need_rsp) {
			andes_unlink_cmd_msg(msg, &ctl->kickq);
			andes_queue_tail_cmd_msg(&ctl->tx_doneq, msg, tx_kickout_fail);
			andes_inc_error_count(ctl, error_tx_kickout_fail);
		} else {
			andes_unlink_cmd_msg(msg, &ctl->ackq);
			msg->state = tx_kickout_fail;
			andes_inc_error_count(ctl, error_tx_kickout_fail);
			RTMP_OS_COMPLETE(&msg->ack_done);
		}

		DBGPRINT(RT_DEBUG_ERROR,
			 ("kick out cmd msg fail(%d)\n", RTMP_USB_URB_STATUS_GET(urb)));
	}

	andes_bh_schedule(ad);
}

static int usb_kick_out_cmd_msg(PRTMP_ADAPTER ad, struct cmd_msg *msg)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	POS_COOKIE pObj = (POS_COOKIE) ad->OS_Cookie;
	int ret = 0;
	PNDIS_PACKET net_pkt = msg->net_pkt;
	RTMP_CHIP_CAP *pChipCap = &ad->chipCap;

	if (msg->state != tx_retransmit) {
		/* append four zero bytes padding when usb aggregate enable */
		memset(OS_PKT_TAIL_BUF_EXTEND(net_pkt, USB_END_PADDING), 0x00, USB_END_PADDING);
	}

	RTUSB_FILL_BULK_URB(msg->urb, pObj->pUsb_Dev,
			    usb_sndbulkpipe(pObj->pUsb_Dev, pChipCap->CommandBulkOutAddr),
			    GET_OS_PKT_DATAPTR(net_pkt), GET_OS_PKT_LEN(net_pkt),
			    usb_kick_out_cmd_msg_complete, net_pkt);

	if (msg->need_rsp)
		andes_queue_tail_cmd_msg(&ctl->ackq, msg, wait_cmd_out_and_ack);
	else
		andes_queue_tail_cmd_msg(&ctl->kickq, msg, wait_cmd_out);

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags))
		return -1;
	OS_SET_BIT(MCU_TX_IN_PROGRESS, &ctl->flags);
	ret = RTUSB_SUBMIT_URB(msg->urb);

	if (ret) {
		OS_CLEAR_BIT(MCU_TX_IN_PROGRESS, &ctl->flags);
		if (!msg->need_rsp) {
			andes_unlink_cmd_msg(msg, &ctl->kickq);
			andes_queue_tail_cmd_msg(&ctl->tx_doneq, msg, tx_kickout_fail);
			andes_inc_error_count(ctl, error_tx_kickout_fail);
		} else {
			andes_unlink_cmd_msg(msg, &ctl->ackq);
			msg->state = tx_kickout_fail;
			andes_inc_error_count(ctl, error_tx_kickout_fail);
			RTMP_OS_COMPLETE(&msg->ack_done);
		}

		DBGPRINT(RT_DEBUG_ERROR, ("%s:submit urb fail(%d)\n", __func__, ret));
	}

	return ret;
}

static void andes_usb_unlink_urb(RTMP_ADAPTER *ad, DL_LIST *list)
{
	unsigned long flags;
	struct cmd_msg *msg, *msg_tmp;
	NDIS_SPIN_LOCK *lock;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	DlListForEachSafe(msg, msg_tmp, list, struct cmd_msg, list) {
		RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
		if ((msg->state == wait_cmd_out_and_ack) || (msg->state == wait_cmd_out) ||
		    (msg->state == tx_start) || (msg->state == rx_start) ||
		    (msg->state == tx_retransmit))
			RTUSB_UNLINK_URB(msg->urb);
		RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	}
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

#endif /* endif */

static void andes_cleanup_cmd_msg(RTMP_ADAPTER *ad, DL_LIST *list)
{
	unsigned long flags;
	struct cmd_msg *msg, *msg_tmp;
	NDIS_SPIN_LOCK *lock;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	lock = andes_get_spin_lock(ctl, list);

	RTMP_SPIN_LOCK_IRQSAVE(lock, &flags);
	DlListForEachSafe(msg, msg_tmp, list, struct cmd_msg, list) {
		_andes_unlink_cmd_msg(msg, list);
		andes_free_cmd_msg(msg);
	}
	DlListInit(list);
	RTMP_SPIN_UNLOCK_IRQRESTORE(lock, &flags);
}

#ifdef RTMP_PCI_SUPPORT
static void andes_ctrl_pci_init(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	RTMP_SPIN_LOCK_IRQ(&ad->mcu_atomic);
	RTMP_CLEAR_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	ctl->cmd_seq = 0;
#ifndef WORKQUEUE_BH
	RTMP_OS_TASKLET_INIT(ad, &ctl->cmd_msg_task, andes_cmd_msg_bh, (unsigned long)ad);
#else
	tasklet_init(&ctl->cmd_msg_task, andes_cmd_msg_bh, (unsigned long)ad);
#endif /* endif */
	NdisAllocateSpinLock(ad, &ctl->txq_lock);
	andes_queue_init(ctl, &ctl->txq);
	NdisAllocateSpinLock(ad, &ctl->rxq_lock);
	andes_queue_init(ctl, &ctl->rxq);
	NdisAllocateSpinLock(ad, &ctl->ackq_lock);
	andes_queue_init(ctl, &ctl->ackq);
	NdisAllocateSpinLock(ad, &ctl->kickq_lock);
	andes_queue_init(ctl, &ctl->kickq);
	NdisAllocateSpinLock(ad, &ctl->tx_doneq_lock);
	andes_queue_init(ctl, &ctl->tx_doneq);
	NdisAllocateSpinLock(ad, &ctl->rx_doneq_lock);
	andes_queue_init(ctl, &ctl->rx_doneq);
	ctl->tx_kickout_fail_count = 0;
	ctl->tx_timeout_fail_count = 0;
	ctl->rx_receive_fail_count = 0;
	ctl->alloc_cmd_msg = 0;
	ctl->free_cmd_msg = 0;
	OS_SET_BIT(MCU_INIT, &ctl->flags);
	ctl->ad = ad;
	RTMP_SPIN_UNLOCK_IRQ(&ad->mcu_atomic);
}
#endif /* RTMP_PCI_SUPPORT */

#ifdef RTMP_USB_SUPPORT
static void andes_ctrl_usb_init(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	RTMP_CLEAR_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	ctl->cmd_seq = 0;
	RTMP_OS_TASKLET_INIT(ad, &ctl->cmd_msg_task, andes_cmd_msg_bh, (unsigned long)ad);
	NdisAllocateSpinLock(ad, &ctl->txq_lock);
	andes_queue_init(ctl, &ctl->txq);
	NdisAllocateSpinLock(ad, &ctl->rxq_lock);
	andes_queue_init(ctl, &ctl->rxq);
	NdisAllocateSpinLock(ad, &ctl->ackq_lock);
	andes_queue_init(ctl, &ctl->ackq);
	NdisAllocateSpinLock(ad, &ctl->kickq_lock);
	andes_queue_init(ctl, &ctl->kickq);
	NdisAllocateSpinLock(ad, &ctl->tx_doneq_lock);
	andes_queue_init(ctl, &ctl->tx_doneq);
	NdisAllocateSpinLock(ad, &ctl->rx_doneq_lock);
	andes_queue_init(ctl, &ctl->rx_doneq);
	ctl->tx_kickout_fail_count = 0;
	ctl->tx_timeout_fail_count = 0;
	ctl->rx_receive_fail_count = 0;
	ctl->alloc_cmd_msg = 0;
	ctl->free_cmd_msg = 0;
	ctl->ad = ad;
	OS_SET_BIT(MCU_INIT, &ctl->flags);
	usb_rx_cmd_msgs_receive(ad);
}
#endif /* endif */

void andes_ctrl_init(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (!OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
		/*avoid to tx/rx not be disable and load firmware timeout
		if (!andes_check_tx_rx_disable(ad))
			andes_suspend_CR_setting(ad);
		*/
#ifdef RTMP_PCI_SUPPORT
		andes_ctrl_pci_init(ad);
#endif

#ifdef RTMP_USB_SUPPORT
		andes_ctrl_usb_init(ad);
#endif /* endif */
	}

	ctl->power_on = FALSE;
	ctl->dpd_on = FALSE;
}

#ifdef RTMP_USB_SUPPORT
static void andes_ctrl_usb_exit(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	int count = 0;

	RTMP_CLEAR_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	OS_CLEAR_BIT(MCU_INIT, &ctl->flags);
	while (OS_TEST_BIT(MCU_TX_IN_PROGRESS, &ctl->flags)) {
		DBGPRINT(RT_DEBUG_TRACE, ("cmd_tx count = %d\n", count));
		count++;
		if (count > 1000)
			break;
		RtmpOsMsDelay(2);
	}
	andes_usb_unlink_urb(ad, &ctl->txq);
	andes_usb_unlink_urb(ad, &ctl->kickq);
	andes_usb_unlink_urb(ad, &ctl->ackq);
	andes_usb_unlink_urb(ad, &ctl->rxq);
	RTMP_OS_TASKLET_KILL(&ctl->cmd_msg_task);
	andes_cleanup_cmd_msg(ad, &ctl->txq);
	NdisFreeSpinLock(&ctl->txq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->ackq);
	NdisFreeSpinLock(&ctl->ackq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->rxq);
	NdisFreeSpinLock(&ctl->rxq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->kickq);
	NdisFreeSpinLock(&ctl->kickq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->tx_doneq);
	NdisFreeSpinLock(&ctl->tx_doneq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->rx_doneq);
	NdisFreeSpinLock(&ctl->rx_doneq_lock);
	DBGPRINT(RT_DEBUG_OFF, ("tx_kickout_fail_count = %ld\n", ctl->tx_kickout_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("tx_timeout_fail_count = %ld\n", ctl->tx_timeout_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("rx_receive_fail_count = %ld\n", ctl->rx_receive_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("alloc_cmd_msg = %ld\n", ctl->alloc_cmd_msg));
	DBGPRINT(RT_DEBUG_OFF, ("free_cmd_msg = %ld\n", ctl->free_cmd_msg));
}
#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
static void andes_ctrl_pci_exit(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	unsigned long flags;

	RTMP_SPIN_LOCK_IRQSAVE(&ad->mcu_atomic, &flags);
	RTMP_CLEAR_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	OS_CLEAR_BIT(MCU_INIT, &ctl->flags);
	RTMP_OS_TASKLET_KILL(&ctl->cmd_msg_task);
	andes_cleanup_cmd_msg(ad, &ctl->txq);
	NdisFreeSpinLock(&ctl->txq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->ackq);
	NdisFreeSpinLock(&ctl->ackq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->rxq);
	NdisFreeSpinLock(&ctl->rxq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->kickq);
	NdisFreeSpinLock(&ctl->kickq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->tx_doneq);
	NdisFreeSpinLock(&ctl->tx_doneq_lock);
	andes_cleanup_cmd_msg(ad, &ctl->rx_doneq);
	NdisFreeSpinLock(&ctl->rx_doneq_lock);
	DBGPRINT(RT_DEBUG_OFF, ("tx_kickout_fail_count = %ld\n", ctl->tx_kickout_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("tx_timeout_fail_count = %ld\n", ctl->tx_timeout_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("rx_receive_fail_count = %ld\n", ctl->rx_receive_fail_count));
	DBGPRINT(RT_DEBUG_OFF, ("alloc_cmd_msg = %ld\n", ctl->alloc_cmd_msg));
	DBGPRINT(RT_DEBUG_OFF, ("free_cmd_msg = %ld\n", ctl->free_cmd_msg));
	RTMP_SPIN_UNLOCK_IRQRESTORE(&ad->mcu_atomic, &flags);
}
#endif /* endif */

void andes_ctrl_exit(RTMP_ADAPTER *ad)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;

	if (OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
#ifdef RTMP_PCI_SUPPORT
		andes_ctrl_pci_exit(ad);
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
		andes_ctrl_usb_exit(ad);
#endif /* endif */
	}

	ctl->power_on = FALSE;
	ctl->dpd_on = FALSE;
}

static int andes_dequeue_and_kick_out_cmd_msgs(RTMP_ADAPTER *ad)
{
	struct cmd_msg *msg = NULL;
	PNDIS_PACKET net_pkt = NULL;
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	int ret = NDIS_STATUS_SUCCESS;
	TXINFO_NMAC_CMD *tx_info;

	while ((msg = andes_dequeue_cmd_msg(ctl, &ctl->txq))) {
		if (!RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD)
		    || RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_NIC_NOT_EXIST)
		    || RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_SUSPEND)) {
			if (!msg->need_rsp)
				andes_free_cmd_msg(msg);
			continue;
		}

		if (andes_queue_len(ctl, &ctl->ackq) > 0) {
			andes_queue_head_cmd_msg(&ctl->txq, msg, msg->state);
			ret = NDIS_STATUS_FAILURE;
			continue;
		}

		net_pkt = msg->net_pkt;

		if (msg->state != tx_retransmit) {
			if (msg->need_rsp)
				msg->seq = andes_get_cmd_msg_seq(ad);
			else
				msg->seq = 0;

			tx_info =
			    (TXINFO_NMAC_CMD *) OS_PKT_HEAD_BUF_EXTEND(net_pkt, sizeof(*tx_info));
			tx_info->info_type = CMD_PACKET;
			tx_info->d_port = CPU_TX_PORT;
			tx_info->cmd_type = msg->type;
			tx_info->cmd_seq = msg->seq;
			tx_info->pkt_len = GET_OS_PKT_LEN(net_pkt) - sizeof(*tx_info);

#ifdef RT_BIG_ENDIAN
			*(UINT32 *) tx_info = le2cpu32(*(UINT32 *) tx_info);
			/* RTMPDescriptorEndianChange((PUCHAR)tx_info, TYPE_TXINFO); */
#endif /* endif */
		}

#ifdef RTMP_USB_SUPPORT
		ret = usb_kick_out_cmd_msg(ad, msg);
#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
		ret = pci_kick_out_cmd_msg(ad, msg);
#endif /* endif */

		if (ret) {
			DBGPRINT(RT_DEBUG_ERROR, ("kick out msg fail\n"));
			break;
		}
	}

	andes_bh_schedule(ad);

	return ret;
}

static int andes_wait_for_complete_timeout(struct cmd_msg *msg, long timeout)
{
	int ret = 0;
#ifdef RTMP_USB_SUPPORT
	long expire;

	expire = timeout ? RTMPMsecsToJiffies(timeout) : RTMPMsecsToJiffies(CMD_MSG_TIMEOUT);
#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
	timeout = CMD_MSG_TIMEOUT;

	do {
		if (msg->ack_done == TRUE)
			return -1;

		OS_WAIT(1);
		timeout--;
	} while (timeout != 0);
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
	ret = RTMP_OS_WAIT_FOR_COMPLETION_TIMEOUT(&msg->ack_done, expire);
#endif /* endif */

	return ret;
}

int andes_send_cmd_msg(PRTMP_ADAPTER ad, struct cmd_msg *msg)
{
	struct MCU_CTRL *ctl = &ad->MCUCtrl;
	int ret = 0;
	BOOLEAN need_wait = msg->need_wait;

	if (!RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD)
	    || RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_NIC_NOT_EXIST)
	    || RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_SUSPEND)
#ifdef NEW_WOW_SUPPORT
	    /* block inband command after enter suspend mode to avoid other unexpected inband command affecting FW */
	    || ad->WOW_Cfg.bInSuspendMode & WOW_SUSPEND_COMPLETE
	    /* block inband command during sw reset */
	    || RTMP_TEST_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SW_RESET)
#endif /* NEW_WOW_SUPPORT */
	    ) {

		andes_free_cmd_msg(msg);
		return NDIS_STATUS_FAILURE;
	}

	andes_queue_tail_cmd_msg(&ctl->txq, msg, tx_start);

retransmit:
	ret = 0;
	andes_dequeue_and_kick_out_cmd_msgs(ad);

	/* Wait for response */
	if (need_wait) {
		enum cmd_msg_state state;

		if (!andes_wait_for_complete_timeout(msg, msg->timeout)) {
			ret = NDIS_STATUS_FAILURE;
#ifdef NEW_WOW_SUPPORT
			if (RTMP_TEST_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDED)
			    && (msg->type == CMD_BURST_READ)) {
				/* During suspend state, this should be a dummy command to work-around
				   Toggle bit error issue. So this cmd could be lost. Therefore, we did
				   not check its response.
				 */
				DBGPRINT(RT_DEBUG_ERROR,
					 ("Dummy cmd(%d) timeout during suspend, Ignore\n",
					  msg->type));
			} else
#endif /* NEW_WOW_SUPPORT */
			{
				DBGPRINT(RT_DEBUG_ERROR,
					 ("command %d timeout %d ms\n", msg->type, msg->timeout));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("txq qlen = %d\n", andes_queue_len(ctl, &ctl->txq)));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("rxq qlen = %d\n", andes_queue_len(ctl, &ctl->rxq)));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("kickq qlen = %d\n", andes_queue_len(ctl, &ctl->kickq)));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("ackq qlen = %d\n", andes_queue_len(ctl, &ctl->ackq)));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("tx_doneq.qlen = %d\n",
					  andes_queue_len(ctl, &ctl->tx_doneq)));
				DBGPRINT(RT_DEBUG_ERROR,
					 ("rx_done qlen = %d\n",
					  andes_queue_len(ctl, &ctl->rx_doneq)));
			}
			if (OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
				if (msg->state == wait_cmd_out_and_ack) {
#ifdef RTMP_USB_SUPPORT
					RTUSB_UNLINK_URB(msg->urb);
#endif /* endif */
				} else if (msg->state == wait_ack) {
					andes_unlink_cmd_msg(msg, &ctl->ackq);
				}
			}

			andes_inc_error_count(ctl, error_tx_timeout_fail);
			state = tx_timeout_fail;
			if (msg->retransmit_times > 0)
				msg->retransmit_times--;
			if (!ad->bSuppressEP5)
				DBGPRINT(RT_DEBUG_ERROR,
				 ("msg->retransmit_times = %d\n", msg->retransmit_times));

			/* Set to 0 First */
			RTMP_IO_WRITE32(ad, TSO_CTRL, 0x0);

			/* Debug CR */
			if (!ad->bSuppressEP5)
				Set_Register_Dump(ad, NULL);

			if (msg->retransmit_times == 0 &&
				RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_START_UP)) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: Continuous cmd timeout for %d times, Reset!\n",
						__func__,
						CMD_MSG_RETRANSMIT_TIMES));
#ifdef RTMP_MAC_USB
				Set_Chip_Reset(ad, NULL);
#endif
			}
		} else {
			if (msg->state == tx_kickout_fail) {
				state = tx_kickout_fail;
				msg->retransmit_times--;
			} else {
				state = tx_done;
				msg->retransmit_times = 0;
			}
		}

		if (OS_TEST_BIT(MCU_INIT, &ctl->flags)) {
			if (msg->need_retransmit && (msg->retransmit_times > 0)) {
#ifdef RTMP_PCI_SUPPORT
				msg->ack_done = FALSE;
#endif /* endif */

#ifdef RTMP_USB_SUPPORT
				RTMP_OS_EXIT_COMPLETION(&msg->ack_done);
				RTMP_OS_INIT_COMPLETION(&msg->ack_done);
#endif /* endif */
				state = tx_retransmit;
				andes_queue_head_cmd_msg(&ctl->txq, msg, state);
				goto retransmit;
			} else {
				andes_queue_tail_cmd_msg(&ctl->tx_doneq, msg, state);
			}
		} else {
			andes_free_cmd_msg(msg);
		}
	}

	return ret;
}

static void andes_pwr_event_handler(RTMP_ADAPTER *ad, char *payload, u16 payload_len)
{

}

static void andes_wow_event_handler(RTMP_ADAPTER *ad, char *payload, u16 payload_len)
{

}

static void andes_carrier_detect_event_handler(RTMP_ADAPTER *ad, char *payload, u16 payload_len)
{

}

static void andes_dfs_detect_event_handler(PRTMP_ADAPTER ad, char *payload, u16 payload_len)
{

}

MSG_EVENT_HANDLER msg_event_handler_tb[] = {
	andes_pwr_event_handler,
	andes_wow_event_handler,
	andes_carrier_detect_event_handler,
	andes_dfs_detect_event_handler,
};

int andes_burst_write(RTMP_ADAPTER *ad, u32 offset, u32 *data, u32 cnt)
{
	struct cmd_msg *msg;
	unsigned int var_len, offset_num, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!data)
		return -1;

	offset_num = cnt / ((cap->InbandPacketMaxLen - sizeof(offset)) / 4);

	if (cnt % ((cap->InbandPacketMaxLen - sizeof(offset)) / 4))
		var_len = sizeof(offset) * (offset_num + 1) + 4 * cnt;
	else
		var_len = sizeof(offset) * offset_num + 4 * cnt;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if (((sent_len < cap->InbandPacketMaxLen)
		     || ((cur_len + cap->InbandPacketMaxLen) == var_len)))
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet) {
			andes_init_cmd_msg(msg, CMD_BURST_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		} else {
			andes_init_cmd_msg(msg, CMD_BURST_WRITE, FALSE, 0, FALSE, FALSE, 0, NULL,
					   NULL);
		}

		value = cpu2le32(offset + cap->WlanMemmapOffset + cur_index * 4);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		for (i = 0; i < ((sent_len - 4) / 4); i++) {
			value = cpu2le32(data[i + cur_index]);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);

		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}
		cur_index += ((sent_len - 4) / 4);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

static void andes_burst_read_callback(struct cmd_msg *msg, char *rsp_payload, u16 rsp_payload_len)
{
	u32 i;
	u32 *data;

	NdisMoveMemory(msg->rsp_payload, rsp_payload + 4, rsp_payload_len - 4);

	for (i = 0; i < (msg->rsp_payload_len - 4) / 4; i++) {
		data = (u32 *) (msg->rsp_payload + i * 4);
		*data = le2cpu32(*data);
	}
}

int andes_burst_read(RTMP_ADAPTER *ad, u32 offset, u32 cnt, u32 *data)
{
	struct cmd_msg *msg;
	unsigned int cur_len = 0, rsp_len, offset_num, receive_len;
	u32 value, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;

	if (!data)
		return -1;

	offset_num = cnt / ((cap->InbandPacketMaxLen - sizeof(offset)) / 4);

	if (cnt % ((cap->InbandPacketMaxLen - sizeof(offset)) / 4))
		rsp_len = sizeof(offset) * (offset_num + 1) + 4 * cnt;
	else
		rsp_len = sizeof(offset) * offset_num + 4 * cnt;

	while (cur_len < rsp_len) {
		receive_len = (rsp_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (rsp_len - cur_len);

		msg = andes_alloc_cmd_msg(ad, 8);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		andes_init_cmd_msg(msg, CMD_BURST_READ, TRUE, 0, TRUE, TRUE, receive_len,
				   (char *)(&data[cur_index]), andes_burst_read_callback);

		value = cpu2le32(offset + cap->WlanMemmapOffset + cur_index * 4);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = cpu2le32((receive_len - 4) / 4);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		ret = andes_send_cmd_msg(ad, msg);

		if (ret) {
			if (cnt == 1)
				*data = 0xffffffff;
		}

		cur_index += ((receive_len - 4) / 4);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

static void andes_random_read_callback(struct cmd_msg *msg, char *rsp_payload, u16 rsp_payload_len)
{
	u32 i;
	RTMP_REG_PAIR *reg_pair = (RTMP_REG_PAIR *) msg->rsp_payload;

	for (i = 0; i < msg->rsp_payload_len / 8; i++) {
		NdisMoveMemory(&reg_pair[i].Value, rsp_payload + 8 * i + 4, 4);
		reg_pair[i].Value = le2cpu32(reg_pair[i].Value);
	}
}

int andes_random_read(RTMP_ADAPTER *ad, RTMP_REG_PAIR *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 8, cur_len = 0, receive_len;
	u32 i, value, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		receive_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		msg = andes_alloc_cmd_msg(ad, receive_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		andes_init_cmd_msg(msg, CMD_RANDOM_READ, TRUE, 0, TRUE, TRUE, receive_len,
				   (char *)&reg_pair[cur_index], andes_random_read_callback);

		for (i = 0; i < receive_len / 8; i++) {
			value = cpu2le32(reg_pair[i + cur_index].Register + cap->WlanMemmapOffset);
			andes_append_cmd_msg(msg, (char *)&value, 4);
			value = 0;
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += receive_len / 8;
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

static void andes_rf_random_read_callback(struct cmd_msg *msg, char *rsp_payload,
					  u16 rsp_payload_len)
{
	u32 i;
	BANK_RF_REG_PAIR *reg_pair = NULL;

	if ((!msg) || (!msg->rsp_payload) || (msg->rsp_payload_len == 0)) {
		DBGPRINT(RT_DEBUG_ERROR, ("andes_rf_random_read_callback fail\n"));
		return;
	}

	reg_pair = (BANK_RF_REG_PAIR *)msg->rsp_payload;

	for (i = 0; i < msg->rsp_payload_len / 8; i++)
		NdisMoveMemory(&reg_pair[i].Value, rsp_payload + 8 * i + 4, 1);
}

int andes_rf_random_read(RTMP_ADAPTER *ad, BANK_RF_REG_PAIR *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 8, cur_len = 0, receive_len;
	u32 i, value, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		receive_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		msg = andes_alloc_cmd_msg(ad, receive_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		andes_init_cmd_msg(msg, CMD_RANDOM_READ, TRUE, 0, TRUE, TRUE, receive_len,
				   (char *)&reg_pair[cur_index], andes_rf_random_read_callback);

		for (i = 0; i < (receive_len) / 8; i++) {
			value = 0;

			/* RF selection */
			value = (value & ~0x80000000) | 0x80000000;

			/* RF bank */
			value = (value & ~0x00ff0000) | (reg_pair[i + cur_index].Bank << 16);

			/* RF Index */
			value = (value & ~0x0000ffff) | reg_pair[i + cur_index].Register;

			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
			value = 0;
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += receive_len / 8;
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

int andes_read_modify_write(RTMP_ADAPTER *ad, R_M_W_REG *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 12, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if ((sent_len < cap->InbandPacketMaxLen)
		    || (cur_len + cap->InbandPacketMaxLen) == var_len)
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet)
			andes_init_cmd_msg(msg, CMD_READ_MODIFY_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		else
			andes_init_cmd_msg(msg, CMD_READ_MODIFY_WRITE, FALSE, 0, FALSE, FALSE, 0,
					   NULL, NULL);

		for (i = 0; i < (sent_len / 12); i++) {
			/* Address */
			value = cpu2le32(reg_pair[i + cur_index].Register + cap->WlanMemmapOffset);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			/* ClearBitMask */
			value = cpu2le32(reg_pair[i + cur_index].ClearBitMask);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			/* UpdateData */
			value = cpu2le32(reg_pair[i + cur_index].Value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += (sent_len / 12);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

int andes_rf_read_modify_write(RTMP_ADAPTER *ad, RF_R_M_W_REG *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 12, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if ((sent_len < cap->InbandPacketMaxLen)
		    || (cur_len + cap->InbandPacketMaxLen) == var_len)
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet)
			andes_init_cmd_msg(msg, CMD_READ_MODIFY_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		else
			andes_init_cmd_msg(msg, CMD_READ_MODIFY_WRITE, FALSE, 0, FALSE, FALSE, 0,
					   NULL, NULL);

		for (i = 0; i < sent_len / 12; i++) {
			value = 0;
			/* RF selection */
			value = (value & ~0x80000000) | 0x80000000;

			/* RF bank */
			value = (value & ~0x00ff0000) | (reg_pair[i + cur_index].Bank << 16);

			/* RF Index */
			value = (value & ~0x000000ff) | reg_pair[i + cur_index].Register;
			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			value = 0;
			/* ClearBitMask */
			value = (value & ~0x000000ff) | reg_pair[i + cur_index].ClearBitMask;
			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			value = 0;
			/* UpdateData */
			value = (value & ~0x000000ff) | reg_pair[i + cur_index].Value;
			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}
		cur_index += (sent_len / 12);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

int andes_random_write(RTMP_ADAPTER *ad, RTMP_REG_PAIR *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 8, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if ((sent_len < cap->InbandPacketMaxLen)
		    || (cur_len + cap->InbandPacketMaxLen) == var_len)
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet)
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		else
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, FALSE, 0, FALSE, FALSE, 0, NULL,
					   NULL);

		for (i = 0; i < (sent_len / 8); i++) {
			/* Address */
			value = cpu2le32(reg_pair[i + cur_index].Register + cap->WlanMemmapOffset);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			/* UpdateData */
			value = cpu2le32(reg_pair[i + cur_index].Value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		};

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += (sent_len / 8);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

int andes_rf_random_write(RTMP_ADAPTER *ad, BANK_RF_REG_PAIR *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 8, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if ((sent_len < cap->InbandPacketMaxLen)
		    || (cur_len + cap->InbandPacketMaxLen) == var_len)
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet)
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		else
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, FALSE, 0, FALSE, FALSE, 0, NULL,
					   NULL);

		for (i = 0; i < (sent_len / 8); i++) {
			value = 0;
			/* RF selection */
			value = (value & ~0x80000000) | 0x80000000;

			/* RF bank */
			value = (value & ~0x00ff0000) | (reg_pair[i + cur_index].Bank << 16);

			/* RF Index */
			value = (value & ~0x000000ff) | reg_pair[i + cur_index].Register;

			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);

			value = 0;
			/* UpdateData */
			value = (value & ~0x000000ff) | reg_pair[i + cur_index].Value;
			value = cpu2le32(value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		}

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += (sent_len / 8);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}

#ifdef CONFIG_ANDES_BBP_RANDOM_WRITE_SUPPORT
int andes_bbp_random_write(RTMP_ADAPTER *ad, RTMP_REG_PAIR *reg_pair, u32 num)
{
	struct cmd_msg *msg;
	unsigned int var_len = num * 8, cur_len = 0, sent_len;
	u32 value, i, cur_index = 0;
	RTMP_CHIP_CAP *cap = &ad->chipCap;
	int ret = 0;
	BOOLEAN last_packet = FALSE;

	if (!reg_pair)
		return -1;

	while (cur_len < var_len) {
		sent_len = (var_len - cur_len) > cap->InbandPacketMaxLen
		    ? cap->InbandPacketMaxLen : (var_len - cur_len);

		if ((sent_len < cap->InbandPacketMaxLen)
		    || (cur_len + cap->InbandPacketMaxLen) == var_len)
			last_packet = TRUE;

		msg = andes_alloc_cmd_msg(ad, sent_len);

		if (!msg) {
			ret = NDIS_STATUS_RESOURCES;
			goto error;
		}

		if (last_packet)
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, TRUE, 0, TRUE, TRUE, 0, NULL,
					   NULL);
		else
			andes_init_cmd_msg(msg, CMD_RANDOM_WRITE, FALSE, 0, FALSE, FALSE, 0, NULL,
					   NULL);

		for (i = 0; i < (sent_len / 8); i++) {
			/* BBP selection */
			value = 0x40000000;

			/* Address */
			value |= reg_pair[i + cur_index].Register;
			value = cpu2le32(value);

			andes_append_cmd_msg(msg, (char *)&value, 4);

			/* UpdateData */
			value = cpu2le32(reg_pair[i + cur_index].Value);
			andes_append_cmd_msg(msg, (char *)&value, 4);
		};

		ret = andes_send_cmd_msg(ad, msg);
		if (ret != 0) {
			DBGPRINT(RT_DEBUG_ERROR, ("%s: !!ERROR, andes cmd send fail\n", __func__));
			goto error;
		}

		cur_index += (sent_len / 8);
		cur_len += cap->InbandPacketMaxLen;
	}

error:
	return ret;
}
#endif /* CONFIG_ANDES_BBP_RANDOM_WRITE_SUPPORT */

int andes_sc_random_write(RTMP_ADAPTER *ad, CR_REG *table, u32 nums, u32 flags)
{
	u32 varlen = 0, i, j;
	RTMP_REG_PAIR *sw_ch_table = NULL, temp;

	if (!table)
		return -1;

	for (i = 0; i < nums; i++) {
		if ((table[i].flags & (_BAND | _BW | _TX_RX_SETTING)) == flags)
			varlen += sizeof(RTMP_REG_PAIR);
	}

	os_alloc_mem(NULL, (UCHAR **) &sw_ch_table, varlen);

	if (!sw_ch_table) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s: memory is not available for allocating switch channel table\n",
			  __func__));
		return -1;
	}

	for (i = 0, j = 0; i < nums; i++) {
		if ((table[i].flags & (_BAND | _BW | _TX_RX_SETTING)) == flags) {
			temp.Register = table[i].offset;
			temp.Value = table[i].value;
			NdisMoveMemory(&sw_ch_table[j], &temp, sizeof(temp));
			j++;
		}
	}

	andes_random_write(ad, sw_ch_table, varlen / sizeof(RTMP_REG_PAIR));

	os_free_mem(NULL, sw_ch_table);

	return 0;
}

int andes_sc_rf_random_write(RTMP_ADAPTER *ad, BANK_RF_CR_REG *table, u32 nums, u32 flags)
{
	u32 varlen = 0, i, j;
	BANK_RF_REG_PAIR *sw_ch_table = NULL, temp;

	if (!table)
		return -1;

	for (i = 0; i < nums; i++) {
		if ((table[i].flags & (_BAND | _BW | _TX_RX_SETTING)) == flags)
			varlen += sizeof(BANK_RF_REG_PAIR);
	}

	os_alloc_mem(NULL, (UCHAR **) &sw_ch_table, varlen);

	if (!sw_ch_table) {
		DBGPRINT(RT_DEBUG_ERROR,
			 ("%s: memory is not available for allocating switch channel table\n",
			  __func__));
		return -1;
	}

	for (i = 0, j = 0; i < nums; i++) {
		if ((table[i].flags & (_BAND | _BW | _TX_RX_SETTING)) == flags) {
			temp.Bank = table[i].bank;
			temp.Register = table[i].offset;
			temp.Value = table[i].value;
			NdisMoveMemory(&sw_ch_table[j], &temp, sizeof(temp));
			j++;
		}
	}

	andes_rf_random_write(ad, sw_ch_table, varlen / sizeof(BANK_RF_REG_PAIR));

	os_free_mem(NULL, sw_ch_table);

	return 0;
}

int andes_pwr_saving(RTMP_ADAPTER *ad, u32 op, u32 level,
		     u32 listen_interval, u32 pre_tbtt_lead_time,
		     u8 tim_byte_offset, u8 tim_byte_pattern)
{
	struct cmd_msg *msg;
	unsigned int var_len;
	u32 value;
	int ret = 0;

	/* Power operation and Power Level */
	var_len = 8;

	if (op == RADIO_OFF_ADVANCE) {
		/* Listen interval, Pre-TBTT, TIM info */
		var_len += 12;
	}

	if (op == RADIO_ON_ADVANCE) {
		/* Listen interval, Pre-TBTT, TIM info, PS Mode, Adv WOW enable */
		var_len += CMD_RADIO_ON_ADVANCE_LEN;
	}

	msg = andes_alloc_cmd_msg(ad, var_len);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	if ((op == RADIO_ON) || (op == RADIO_OFF))
		andes_init_cmd_msg(msg, CMD_POWER_SAVING_OP, TRUE, WAIT_RESPONSE_TIME, TRUE, TRUE,
			0, 0, NULL);
	else
		andes_init_cmd_msg(msg, CMD_POWER_SAVING_OP, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);

	/* Power operation */
	value = cpu2le32(op);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* Power Level */
	value = cpu2le32(level);

	andes_append_cmd_msg(msg, (char *)&value, 4);

	if (op == RADIO_OFF_ADVANCE) {
		/* Listen interval */
		value = cpu2le32(listen_interval);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		/* Pre TBTT lead time */
		value = cpu2le32(pre_tbtt_lead_time);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		/* TIM Info */
		value = (value & ~0x000000ff) | tim_byte_pattern;
		value = (value & ~0x0000ff00) | (tim_byte_offset << 8);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}

	if (op == RADIO_ON_ADVANCE) {
		/* Listen interval */
		value = cpu2le32(listen_interval);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		/* Pre TBTT lead time */
		value = cpu2le32(pre_tbtt_lead_time);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		/* TIM Info */
		value = (value & ~0x000000ff) | tim_byte_pattern;
		value = (value & ~0x0000ff00) | (tim_byte_offset << 8);
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		/* PS mode Info */
		value = 0;	/* STA mode */
		/* value = 1; *//* P2P GO mode */
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = 0;
#ifdef NEW_WOW_SUPPORT
		if (ADV_WOW_ENTRY(ad))
			value = ((ad->WOW_Cfg.nAdvSleepTime << 0x01) | ad->WOW_Cfg.bAdvEnable);
#endif /* NEW_WOW_SUPPORT */
		value = cpu2le32(value);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}

	ret = andes_send_cmd_msg(ad, msg);
	if (ret)
		DBGPRINT(RT_DEBUG_ERROR, ("[%s] op:%d ret:%d\n", __func__, op, ret));

error:
	return ret;
}

#ifdef MT76XX_BTCOEX_SUPPORT
int andes_coex_op_param(RTMP_ADAPTER *ad, u32 op, u32 mode)
{
	struct cmd_msg *msg;
	u32 value;
	int ret = 0;

	/* Function ID and Parameter */
	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_COEX_OP_PARAM, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/* Coex Operation */
	value = cpu2le32(op);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* Coex Force Mode */
	value = cpu2le32(mode);
	andes_append_cmd_msg(msg, (char *)&value, 4);
	ret = andes_send_cmd_msg(ad, msg);
error:
	return ret;
}
#endif

int andes_fun_set(RTMP_ADAPTER *ad, u32 fun_id, u32 param)
{
	struct cmd_msg *msg;
	u32 value;
	int ret = 0;

	/* Function ID and Parameter */
	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	if (fun_id != Q_SELECT)
		andes_init_cmd_msg(msg, CMD_FUN_SET_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);
	else
		andes_init_cmd_msg(msg, CMD_FUN_SET_OP, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);

	/* Function ID */
	value = cpu2le32(fun_id);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* Parameter */
	if (fun_id != USB2_SW_DISCONNECT && fun_id != USB3_SW_DISCONNECT) {
		value = cpu2le32(param);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}
	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

int andes_calibration(RTMP_ADAPTER *ad, u32 cal_id, ANDES_CALIBRATION_PARAM *param)
{
	struct cmd_msg *msg;
	u32 value;
	int ret = 0;

	DBGPRINT(RT_DEBUG_INFO, ("%s:cal_id(%d)\n ", __func__, cal_id));

#ifdef MT76x2
#ifdef RTMP_PCI_SUPPORT
	if (IS_MT76x2(ad) && (cal_id != TSSI_COMPENSATION_7662)) {
		ANDES_CALIBRATION_START(ad);
	}
#endif /* RTMP_PCI_SUPPORT */
#endif /* MT76x2 */

#ifdef MT76x2
	/* Calibration ID and Parameter */
	if (cal_id == TSSI_COMPENSATION_7662 && IS_MT76x2(ad))
		msg = andes_alloc_cmd_msg(ad, 12);
	else
#endif /* MT76x2 */
		msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_CALIBRATION_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/* Calibration ID */
	value = cpu2le32(cal_id);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* Parameter */
#ifdef MT76x2
	if (cal_id == TSSI_COMPENSATION_7662 && IS_MT76x2(ad)) {
		value = cpu2le32(param->mt76x2_tssi_comp_param.pa_mode);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = cpu2le32(param->mt76x2_tssi_comp_param.tssi_slope_offset);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	} else
#endif /* MT76x2 */
	{
		value = cpu2le32(param->generic);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}

	ret = andes_send_cmd_msg(ad, msg);

#ifdef MT76x2
#ifdef RTMP_PCI_SUPPORT
	if (IS_MT76x2(ad) && ((cal_id != TSSI_COMPENSATION_7662))) {
		ANDES_WAIT_CALIBRATION_DONE(ad);
	}
#endif /* RTMP_PCI_SUPPORT */
#endif /* MT76x2 */

error:
	return ret;
}

#ifdef NEW_WOW_SUPPORT
void reset_TxRx_Info(struct _RTMP_ADAPTER *ad)
{
	int i = 0;

	PCMD_RSP_CONTEXT pCmdRspEventContext = &ad->CmdRspEventContext;

	ad->PendingRx = 0;
	ad->NextRxBulkInReadIndex = 0;	/* Next Rx Read index */
	ad->NextRxBulkInIndex = 0;	/*RX_RING_SIZE -1;  Rx Bulk pointer */
	ad->NextRxBulkInPosition = 0;
	ad->TransferBufferLength = 0;
	ad->ReadPosition = 0;

	/* Reset RX DS */
	for (i = 0; i < (RX_RING_SIZE); i++) {
		PRX_CONTEXT pRxContext = &(ad->RxContext[i]);

		ASSERT((pRxContext->TransferBuffer != NULL));
		ASSERT((pRxContext->pUrb != NULL));

		NdisZeroMemory(pRxContext->TransferBuffer, MAX_RXBULK_SIZE);

		pRxContext->pAd = ad;
		pRxContext->pIrp = NULL;
		pRxContext->InUse = FALSE;
		pRxContext->IRPPending = FALSE;
		pRxContext->Readable = FALSE;
		pRxContext->bRxHandling = FALSE;
		pRxContext->BulkInOffset = 0;
	}

	pCmdRspEventContext->pAd = ad;
	pCmdRspEventContext->InUse = FALSE;
	pCmdRspEventContext->Readable = FALSE;
	NdisZeroMemory(pCmdRspEventContext->CmdRspBuffer, CMD_RSP_BULK_SIZE);

	/* Reset Tx DS */
	for (i = 0; i < NUM_OF_TX_RING; i++) {
		PHT_TX_CONTEXT pHTTXContext = &(ad->TxContext[i]);

		pHTTXContext->pAd = ad;
		pHTTXContext->pIrp = NULL;
		pHTTXContext->IRPPending = FALSE;
		pHTTXContext->NextBulkOutPosition = 0;
		pHTTXContext->ENextBulkOutPosition = 0;
		pHTTXContext->CurWritePosition = 0;
		pHTTXContext->CurWriteRealPos = 0;
		pHTTXContext->BulkOutSize = 0;
		pHTTXContext->BulkOutPipeId = i;
		pHTTXContext->bRingEmpty = TRUE;
		pHTTXContext->bCopySavePad = FALSE;
#ifdef USB_BULK_BUF_ALIGMENT
		pHTTXContext->CurtBulkIdx = 0;
		pHTTXContext->CurWriteIdx = 0;
		pHTTXContext->NextBulkIdx = 0;
#endif
		ad->BulkOutPending[i] = FALSE;
		ad->NextBulkOutIndex[i] = i;
	}

	/* Reset MGMT DS */
	{
		PTX_CONTEXT pMLMEContext = NULL;
		RTMP_MGMT_RING *pMgmtRing = &ad->MgmtRing;

		for (i = 0; i < MGMT_RING_SIZE; i++) {
			pMgmtRing->Cell[i].pNdisPacket = NULL;
			pMgmtRing->Cell[i].pNextNdisPacket = NULL;

			pMLMEContext = (PTX_CONTEXT) ad->MgmtRing.Cell[i].AllocVa;

			pMLMEContext->pAd = ad;
			pMLMEContext->SelfIdx = i;

		}
		DBGPRINT(RT_DEBUG_TRACE, ("MGMT Ring: total %d entry allocated\n", i));
	}

	ad->MgmtRing.TxSwFreeIdx = MGMT_RING_SIZE;
	ad->MgmtRing.TxCpuIdx = 0;
	ad->MgmtRing.TxDmaIdx = 0;
}

static int andes_wow_send_config_cmd(RTMP_ADAPTER *pAd, u8 type,
	BOOLEAN need_wait, u16 timeout, BOOLEAN need_rsp,
		PUINT32 dataPtr, unsigned int var_len)
{
	struct cmd_msg *msg = NULL;
	u32 value, i;
	int ret = NDIS_STATUS_SUCCESS;

	msg = andes_alloc_cmd_msg(pAd, var_len);
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW andes_alloc_cmd_msg fail\n", __func__));
		return NDIS_STATUS_RESOURCES;
	}

	andes_init_cmd_msg(msg, type, need_wait, timeout, TRUE, need_rsp, 0, NULL, NULL);

	for (i = 0; i < (var_len/4); i++) {
		value = cpu2le32(dataPtr[i]);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}

	ret = andes_send_cmd_msg(pAd, msg);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW send command %d fail\n", __func__, type));
		return ret;
	}

	RtmpOsMsDelay(SEND_CMD_DELAY_TIME);

	return NDIS_STATUS_SUCCESS;
}

void andes_send_dummy_bulkout(struct _RTMP_ADAPTER *ad)
{

#define DUMMY_DATABULKOUT_MAX_RETRY			10

	UINT32 var_len = 0, Value = 0, pipeIdx = 0, bulkRetry = 0, ret = 0;
	NEW_WOW_PARAM_STRUCT wow_param = { };
	u32 *data = NULL;
	PURB pUrb = NULL;
	unsigned long IrqFlags = 0;
	PTX_CONTEXT pNullContext = &(ad->NullContext);

	var_len = sizeof(wow_param);
	NdisZeroMemory(&wow_param, var_len);

	/* Dummy In-Band Command bulkout for EP8 to clean up potential toggle error */
	data = (PUINT32) &wow_param;
	read_reg(ad, 0x40, 0x2820, &Value);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("Before sending 1st Dummy cmd to EP8, ToggleErrBits=0x%x\n",
		  Value));
	if (Value & 0xFFFF)
		write_reg(ad, 0x40, 0x2820, (Value & 0xFFFF));

	andes_burst_read(ad, 0x0254, 0x4, data);
	read_reg(ad, 0x40, 0x2820, &Value);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("Sent 1st Dummy to EP8(cmd), ToggleErrBits=0x%x\n", Value));
	if (Value & 0xFFFF)
		write_reg(ad, 0x40, 0x2820, (Value & 0xFFFF));

	andes_burst_read(ad, 0x0254, 0x4, data);
	read_reg(ad, 0x40, 0x2820, &Value);
	DBGPRINT(RT_DEBUG_TRACE,
		 ("Sent 2nd Dummy to EP8(cmd), ToggleErrBits=0x%x\n", Value));
	if (Value & 0xFFFF)
		write_reg(ad, 0x40, 0x2820, (Value & 0xFFFF));

	/* Dummy Data/Mgmt Bulkout for EP 4~7 to clean up potential toggle error */
	/* For EP 4~7 */
	/*      We need to compse Null frame first if it does not exist,
	   so that we could actually send out dummy
	 */
	if (pNullContext->BulkOutSize == 0)
		ComposeNullFrame(ad);
	for (pipeIdx = 0; pipeIdx < 4; pipeIdx++) {
		for (bulkRetry = 0; bulkRetry < DUMMY_DATABULKOUT_MAX_RETRY;
		     bulkRetry++) {
			/*    Note: Althought we send to EP4~7,
						but we are using Null frames contents.
						use Nullframe's pipe to lock and wait (MGMTPIPEIDX)
			 */
			RTMP_IRQ_LOCK(&ad->BulkOutLock[MGMTPIPEIDX], IrqFlags);
			if ((ad->BulkOutPending[MGMTPIPEIDX] == TRUE)
			    || RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_NEED_STOP_TX)) {
				RTMP_IRQ_UNLOCK(&ad->BulkOutLock[MGMTPIPEIDX],
						IrqFlags);
				if (bulkRetry ==
				    (DUMMY_DATABULKOUT_MAX_RETRY - 1)) {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("Fail to send Dummy EP-%d due to URB pending\n",
						  (pipeIdx + 4)));
				} else
					RtmpOsMsDelay(1);
				continue;
			}
			ad->BulkOutPending[MGMTPIPEIDX] = TRUE;
			ad->watchDogTxPendingCnt[MGMTPIPEIDX] = 1;
			pNullContext->IRPPending = TRUE;
			RTMP_IRQ_UNLOCK(&ad->BulkOutLock[MGMTPIPEIDX],
					IrqFlags);
			/*      Check Toggle Error bits.
			   Toogle Error bits are CR 0x2820 bits 0~15 corresponding to Tx EP0~15.
			 */
			read_reg(ad, 0x40, 0x2820, &Value);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("Before sending dummy EP-%d ToggleErrBits=0x%x\n",
				  (pipeIdx + 4), (Value & 0xFFFF)));
			if (Value & 0xFFFF) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("Clearing ToggleErrBits\n"));
				/* Clear Toggle Bits (Clear-on-write) */
				write_reg(ad, 0x40, 0x2820, (Value & 0xFFFF));
			}
#ifdef RT_BIG_ENDIAN
			RTMPDescriptorEndianChange((PUCHAR)
						   pNullContext->TransferBuffer,
						   TYPE_TXINFO);
#endif /* RT_BIG_ENDIAN */

			/* Init Tx context descriptor */
			RTUSBInitTxDesc(ad, pNullContext, pipeIdx,
					(usb_complete_t)
					RtmpUsbBulkOutNullFrameComplete);

			pUrb = pNullContext->pUrb;
			if (RTUSB_SUBMIT_URB(pUrb) != 0) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("%s: Submit Tx (EP-%d)URB failed (err=%d)\n",
					  __func__, pipeIdx + 4, ret));
				RTMP_IRQ_LOCK(&ad->BulkOutLock[MGMTPIPEIDX],
					      IrqFlags);
				ad->BulkOutPending[MGMTPIPEIDX] = FALSE;
				pNullContext->IRPPending = FALSE;
				ad->watchDogTxPendingCnt[MGMTPIPEIDX] = 0;
				RTMP_IRQ_UNLOCK(&ad->BulkOutLock[MGMTPIPEIDX],
						IrqFlags);
				if (bulkRetry ==
				    (DUMMY_DATABULKOUT_MAX_RETRY - 1)) {
					DBGPRINT(RT_DEBUG_ERROR,
						 ("Fail to send Dummy EP-%d due to Submit fail\n",
						  (pipeIdx + 4)));
				} else
					RtmpOsMsDelay(1);
				continue;
			}

			ad->BulkOutReq++;
			/* Check Toggle Error bits again after sending dummy and clear it */
			read_reg(ad, 0x40, 0x2820, &Value);
			DBGPRINT(RT_DEBUG_TRACE,
				 ("After sending dummy EP-%d: ToggleErrBits=0x%x\n",
				  (pipeIdx + 4), (Value & 0xFFFF)));
			if (Value & 0xFFFF) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("Clearing ToggleErrBits\n"));
				/* Clear Toggle Bits (Clear-on-write) */
				write_reg(ad, 0x40, 0x2820, (Value & 0xFFFF));
			}

			/* Try next bulk idx */
			break;
		}

		/* Monitor the EP-7 complete */
		if (pipeIdx == 3) {
			for (bulkRetry = 0;
			     bulkRetry < DUMMY_DATABULKOUT_MAX_RETRY;
			     bulkRetry++) {
				RTMP_IRQ_LOCK(&ad->BulkOutLock[MGMTPIPEIDX],
					      IrqFlags);
				if (ad->BulkOutPending[MGMTPIPEIDX] != TRUE) {
					/* complete also at EP-7 */
					RTMP_IRQ_UNLOCK(&ad->BulkOutLock
							[MGMTPIPEIDX],
							IrqFlags);
					break;
				}
				RTMP_IRQ_UNLOCK(&ad->BulkOutLock[MGMTPIPEIDX],
						IrqFlags);
				RtmpOsMsDelay(1);
			}
			if (bulkRetry == DUMMY_DATABULKOUT_MAX_RETRY) {
				DBGPRINT(RT_DEBUG_ERROR,
					 ("WARN! EP-%d not complete!\n",
					  pipeIdx + 4));
			}
		}

	}

}

static int andes_wow_feature_enable(RTMP_ADAPTER *pAd, BOOLEAN bEnable)
{
	NEW_WOW_PARAM_STRUCT wow_param;

	NdisZeroMemory(&wow_param, sizeof(wow_param));
	wow_param.Parameter = WOW_ENABLE;
	wow_param.Value = bEnable;
	DBGPRINT(RT_DEBUG_TRACE,
			("%s: Set WOW feature %s\n",
				 __func__, bEnable == TRUE?"enable":"disable"));

	if (bEnable)
		return andes_wow_send_config_cmd(pAd, CMD_WOW_FEATURE,
		FALSE, 0, FALSE, (PUINT32)&wow_param, sizeof(wow_param));

	return andes_wow_send_config_cmd(pAd, CMD_WOW_FEATURE,
		TRUE, TRAFFIC_SWITCH_COMMAND_TIMEOUT,
			TRUE, (PUINT32)&wow_param, sizeof(wow_param));
}

static int andes_wow_traffic_switch(RTMP_ADAPTER *pAd, UINT32 trafficPath)
{
	NEW_WOW_PARAM_STRUCT wow_param;

	DBGPRINT(RT_DEBUG_TRACE, ("%s: Request traffic switch to Host\n", __func__));
	NdisZeroMemory(&wow_param, sizeof(wow_param));
	wow_param.Parameter = WOW_TRAFFIC;
	wow_param.Value = trafficPath;

	return andes_wow_send_config_cmd(pAd, CMD_WOW_FEATURE,
		TRUE, TRAFFIC_SWITCH_COMMAND_TIMEOUT,
			TRUE, (PUINT32)&wow_param, sizeof(wow_param));
}

static int andes_wow_set_port_config(RTMP_ADAPTER *pAd)
{
	NEW_WOW_TCP_CFG_STRUCT tcp_cfg;
	NEW_WOW_UDP_CFG_STRUCT udp_cfg;
	struct cmd_msg *msg;
	u32 i, value;
	unsigned int var_len;
	int ret = 0;

	if (pAd->WOW_Cfg.TcpPortCntV4 != 0 || pAd->WOW_Cfg.TcpPortCntV6 != 0) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set TCP packet wake up\n"));
		NdisZeroMemory(&tcp_cfg, sizeof(tcp_cfg));

		/* TCP Config Type + TCP IP + TCP Port Cnt v4 + TCP Port Cnt v6 */
		var_len = sizeof(tcp_cfg) + WOW_TCP_PORT_CNT_LEN;

		var_len += ((pAd->WOW_Cfg.TcpPortCntV4 + pAd->WOW_Cfg.TcpPortCntV6)*4);

		msg = andes_alloc_cmd_msg(pAd, var_len);
		if (!msg) {
			DBGPRINT(RT_DEBUG_ERROR,
				("%s: andes_alloc_cmd_msg for WOW_TCP_CFG return NULL\n",
				__func__));
			return NDIS_STATUS_FAILURE;
		}
		andes_init_cmd_msg(msg, CMD_WOW_CONFIG, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);

		tcp_cfg.Config_Type = WOW_TCP_CFG;
		value = cpu2le32(tcp_cfg.Config_Type);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = cpu2le32(*(UINT32 *)tcp_cfg.IP);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = cpu2le32(pAd->WOW_Cfg.TcpPortCntV4);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		if (pAd->WOW_Cfg.TcpPortCntV4 != 0) {
			DBGPRINT(RT_DEBUG_ERROR,
				("WOW TCPv4 port count=:%d\n", pAd->WOW_Cfg.TcpPortCntV4));
			for (i = 0; i < pAd->WOW_Cfg.TcpPortCntV4; i++) {
				if (pAd->WOW_Cfg.TcpPortV4[i] == 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						("Error TCPv4 port[%d] == 0, ignore\n", i));
					break;
				}
				DBGPRINT(RT_DEBUG_ERROR,
					("Set TCP port v4[%d]:%d\n", i, pAd->WOW_Cfg.TcpPortV4[i]));
				value = cpu2le32(pAd->WOW_Cfg.TcpPortV4[i]);
				andes_append_cmd_msg(msg, (char *)&value, 4);
			}
		}

		value = cpu2le32(pAd->WOW_Cfg.TcpPortCntV6);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		if (pAd->WOW_Cfg.TcpPortCntV6 != 0) {
			DBGPRINT(RT_DEBUG_ERROR,
				("WOW TCPv6 port count=:%d\n", pAd->WOW_Cfg.TcpPortCntV6));
			for (i = 0; i < pAd->WOW_Cfg.TcpPortCntV6; i++) {
				if (pAd->WOW_Cfg.TcpPortV6[i] == 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						("Error TCPv6 port[%d] == 0, ignore\n", i));
					break;
				}
				value = cpu2le32(pAd->WOW_Cfg.TcpPortV6[i]);
				andes_append_cmd_msg(msg, (char *)&value, 4);
				DBGPRINT(RT_DEBUG_TRACE,
					("Set TCP port v6[%d]:%d\n", i, pAd->WOW_Cfg.TcpPortV6[i]));
			}
		}
		ret = andes_send_cmd_msg(pAd, msg);
	}

	/* for wake up by UDP packet */
	if (pAd->WOW_Cfg.UdpPortCntV4 != 0 || pAd->WOW_Cfg.UdpPortCntV6 != 0) {
		DBGPRINT(RT_DEBUG_ERROR, ("Set UDP packet wake up\n"));
		NdisZeroMemory(&udp_cfg,  sizeof(udp_cfg));

		/* UDP Config Type + UDP Port Cnt v4 + UDP Port Cnt v6 */
		var_len = sizeof(udp_cfg) + WOW_UDP_PORT_CNT_LEN;

		var_len += ((pAd->WOW_Cfg.UdpPortCntV4 + pAd->WOW_Cfg.UdpPortCntV6)*4);

		msg = andes_alloc_cmd_msg(pAd, var_len);
		if (!msg) {
			DBGPRINT(RT_DEBUG_ERROR,
				("%s: andes_alloc_cmd_msg for WOW_UDP_CFG return NULL\n",
				__func__));
			return NDIS_STATUS_FAILURE;
		}
		andes_init_cmd_msg(msg, CMD_WOW_CONFIG, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);

		udp_cfg.Config_Type = WOW_UDP_CFG;
		value = cpu2le32(udp_cfg.Config_Type);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		value = cpu2le32(pAd->WOW_Cfg.UdpPortCntV4);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		if (pAd->WOW_Cfg.UdpPortCntV4 != 0) {
			DBGPRINT(RT_DEBUG_ERROR,
				("WOW UDPv4 port count=:%d\n", pAd->WOW_Cfg.UdpPortCntV4));
			for (i = 0; i < pAd->WOW_Cfg.UdpPortCntV4; i++) {
				if (pAd->WOW_Cfg.UdpPortV4[i] == 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						("Error UDPv4 port[%d] == 0, ignore\n", i));
					break;
				}
				DBGPRINT(RT_DEBUG_ERROR,
					("Set UDP port v4[%d]:%d\n", i, pAd->WOW_Cfg.UdpPortV4[i]));
				value = cpu2le32(pAd->WOW_Cfg.UdpPortV4[i]);
				andes_append_cmd_msg(msg, (char *)&value, 4);
			}
		}

		value = cpu2le32(pAd->WOW_Cfg.UdpPortCntV6);
		andes_append_cmd_msg(msg, (char *)&value, 4);

		if (pAd->WOW_Cfg.UdpPortCntV6 != 0) {
			DBGPRINT(RT_DEBUG_ERROR,
				("WOW UDPv6 port count=:%d\n", pAd->WOW_Cfg.UdpPortCntV6));
			for (i = 0; i < pAd->WOW_Cfg.UdpPortCntV6; i++) {
				if (pAd->WOW_Cfg.UdpPortV6[i] == 0) {
					DBGPRINT(RT_DEBUG_ERROR,
						("Error UDPv6 port[%d] == 0, ignore\n", i));
					break;
				}
				value = cpu2le32(pAd->WOW_Cfg.UdpPortV6[i]);
				andes_append_cmd_msg(msg, (char *)&value, 4);
				DBGPRINT(RT_DEBUG_TRACE,
					("Set UDP port v6[%d]:%d\n", i, pAd->WOW_Cfg.UdpPortV6[i]));
			}
		}

		DBGPRINT(RT_DEBUG_TRACE,
			("UDP port count v4:%d\nUDP port count v6:%d\n",
			pAd->WOW_Cfg.UdpPortCntV4, pAd->WOW_Cfg.UdpPortCntV4));
		ret = andes_send_cmd_msg(pAd, msg);
	}

	return ret;
}

static int andes_wow_set_configuration(RTMP_ADAPTER *ad)
{
	NEW_WOW_PARAM_STRUCT wow_param;
	NEW_WOW_MASK_CFG_STRUCT mask_cfg;
	NEW_WOW_SEC_CFG_STRUCT sec_cfg;
	NEW_WOW_INFRA_CFG_STRUCT infra_cfg;
	NEW_WOW_ARP_OFFLOAD_CFG_STRUCT offload_arp_cfg;
	struct NEW_WOW_ALWAYS_TRIGGER_STRUCT wow_trig_cfg;
	struct NEW_WMT_WAKEUP_GPIO_STRUCT wow_wmt_gpio;
	NEW_WOW_AID_CHANNEL_STRUCT wow_aid_channel;
	struct NEW_WOW_QOS_SEQ_STRUCT	wow_qos_seq_cfg;
	MAC_TABLE_ENTRY *pEntry = &ad->MacTab.Content[BSSID_WCID];

	UINT32 Value = 0;
	UINT32 zeroIP[4] = {0x00};
	u32 i;

	/* Step 2 : Set WOW mask configuration  */
	NdisZeroMemory(&mask_cfg, sizeof(mask_cfg));
	mask_cfg.Config_Type = WOW_MASK_CFG; /* detect mask config */
	/* Enabel GTK re-key enable for bit0 and ARP/NS offload for bit 1 */
	mask_cfg.Function_Enable = 3;

	if (ADV_WOW_ENTRY(ad) == FALSE) {
		mask_cfg.Detect_Mask = 1UL << WOW_MAGIC_PKT; /* magic packet */

		if (ad->WOW_Cfg.bWakeonportEnable == TRUE) {
			if (ad->WOW_Cfg.TcpPortCntV4 != 0 || ad->WOW_Cfg.TcpPortCntV6 != 0)
				mask_cfg.Detect_Mask |= 1UL << WOW_PKT_TCP_PORT_ARRAY;

			if (ad->WOW_Cfg.UdpPortCntV4 != 0 || ad->WOW_Cfg.UdpPortCntV6 != 0)
				mask_cfg.Detect_Mask |= 1UL << WOW_PKT_UDP_PORT_ARRAY;
		}
	} else
		mask_cfg.Detect_Mask = 0;

	mask_cfg.Event_Mask = 0;
	DBGPRINT(RT_DEBUG_ERROR, ("WOW detect mask = 0x%x\n", mask_cfg.Detect_Mask));
	andes_wow_send_config_cmd(ad, CMD_WOW_CONFIG,
		FALSE, 0, FALSE, (PUINT32)&mask_cfg, sizeof(mask_cfg));

	if (ADV_WOW_ENTRY(ad) == FALSE) {
		/* Set wake on port configuration */
		if (ad->WOW_Cfg.bWakeonportEnable == TRUE)
			andes_wow_set_port_config(ad);

		/* Step 3.1: Set Always Trigger Wakeup */
		if (ad->WOW_Cfg.bAlwaysTrigger) {
			if ((ad->WOW_Cfg.bInBand == 0) && (ad->WOW_Cfg.nHoldTime != 0)) {
				DBGPRINT(RT_DEBUG_ERROR,
					("%s: Always trigger wakeup\n", __func__));
				NdisZeroMemory(&wow_trig_cfg, sizeof(wow_trig_cfg));
				wow_trig_cfg.Parameter = WOW_ALWAYS_TRIG;
				wow_trig_cfg.Always_Trigger_Wakeup = ad->WOW_Cfg.bAlwaysTrigger;
				andes_wow_send_config_cmd(ad, CMD_WOW_FEATURE,
					FALSE, 0, FALSE, (PUINT32)&wow_trig_cfg,
						sizeof(wow_trig_cfg));
			}
		}

		if (ad->WOW_Cfg.bWmtGPIOCmd) {
			/* Use WMT command to configure GPIO for wakeup */
			NdisZeroMemory(&wow_wmt_gpio, sizeof(wow_wmt_gpio));
			DBGPRINT(RT_DEBUG_ERROR,
				("WOW Wmt command, len=%zu\n", sizeof(wow_wmt_gpio)));

			wow_wmt_gpio.HCI_OPCode = HCI_GPIO_CONFIG_OPCODE;
			wow_wmt_gpio.HCI_Length = HCI_GPIO_CONFIG_LEN;

			wow_wmt_gpio.WMT_Direct_Type = WMT_GPIO_CONFIG_DirTYPE;
			wow_wmt_gpio.WMT_OPCode = WMT_GPIO_CONFIG_OPCODE;
			wow_wmt_gpio.WMT_Length = WMT_GPIO_CONFIG_LEN;

			wow_wmt_gpio.GPIO_Pin_Mode = ad->WOW_Cfg.nWmtGPIOMode;
			wow_wmt_gpio.Trigger = WMT_GPIO_CONFIG_WIFI;
			wow_wmt_gpio.WoW_Pin_Num = WMT_WOW_GPIO;

			if (ad->WOW_Cfg.bWmtWiFiHighActive)
				wow_wmt_gpio.WoW_Initial_Level = 0;
			else
				wow_wmt_gpio.WoW_Initial_Level = 1;

			if (ad->WOW_Cfg.bWmtWiFiWake1Toggle)
				Value = ad->WOW_Cfg.bWmtWiFiHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15;

			wow_wmt_gpio.WiFi_Level_Wake1_Time1 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.WiFi_Level_Wake1_Time2 =
				(Value | ad->WOW_Cfg.nWmtWiFiWake1HoldTime);
			wow_wmt_gpio.WiFi_Level_Wake1_Time3 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);

			if (ad->WOW_Cfg.bWmtWiFiWake2Toggle)
				Value = ad->WOW_Cfg.bWmtWiFiHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15;

			wow_wmt_gpio.WiFi_Level_Wake2_Time1 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.WiFi_Level_Wake2_Time2 =
				(Value | ad->WOW_Cfg.nWmtWiFiWake2HoldTime);
			wow_wmt_gpio.WiFi_Level_Wake2_Time3 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);

			if (ad->WOW_Cfg.bWmtWiFiWake3Toggle)
				Value = ad->WOW_Cfg.bWmtWiFiHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15;

			wow_wmt_gpio.WiFi_Level_Wake3_Time1 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.WiFi_Level_Wake3_Time2 =
				(Value | ad->WOW_Cfg.nWmtWiFiWake3HoldTime);
			wow_wmt_gpio.WiFi_Level_Wake3_Time3 =
				(((ad->WOW_Cfg.bWmtWiFiHighActive ^ 1) << 15) | 1);

			wow_wmt_gpio.WoBT_Pin_Num = WMT_WOBLE_GPIO;

			if (ad->WOW_Cfg.bWmtBTHighActive)
				wow_wmt_gpio.WoBT_Initial_Level = 0;
			else
				wow_wmt_gpio.WoBT_Initial_Level = 1;

			if (ad->WOW_Cfg.bWmtBTWake1Toggle)
				Value = ad->WOW_Cfg.bWmtBTHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15;

			wow_wmt_gpio.BT_Level_Wake1_Time1 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.BT_Level_Wake1_Time2 =
				(Value | ad->WOW_Cfg.nWmtBTWake1HoldTime);
			wow_wmt_gpio.BT_Level_Wake1_Time3 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);

			if (ad->WOW_Cfg.bWmtBTWake2Toggle)
				Value = ad->WOW_Cfg.bWmtBTHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15;

			wow_wmt_gpio.BT_Level_Wake2_Time1 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.BT_Level_Wake2_Time2 =
				(Value | ad->WOW_Cfg.nWmtBTWake2HoldTime);
			wow_wmt_gpio.BT_Level_Wake2_Time3 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);

			if (ad->WOW_Cfg.bWmtBTWake3Toggle)
				Value = ad->WOW_Cfg.bWmtBTHighActive << 15;
			else
				Value = (ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15;

			wow_wmt_gpio.BT_Level_Wake3_Time1 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);
			wow_wmt_gpio.BT_Level_Wake3_Time2 =
				(Value | ad->WOW_Cfg.nWmtBTWake3HoldTime);
			wow_wmt_gpio.BT_Level_Wake3_Time3 =
				(((ad->WOW_Cfg.bWmtBTHighActive ^ 1) << 15) | 1);

			wmt_gpio_configrue(ad, &wow_wmt_gpio, sizeof(wow_wmt_gpio));

			RtmpOsMsDelay(1);
		} else {
			/* Step 3.2 : Set WOW Wakeup Option */
			NdisZeroMemory(&wow_param, sizeof(wow_param));
			wow_param.Parameter = WOW_WAKEUP; /* Wakeup Option */
			if (ad->WOW_Cfg.bInBand)
#ifdef RTMP_MAC_PCI
				wow_param.Value = WOW_WAKEUP_BY_PCIE;
#else
				wow_param.Value = WOW_WAKEUP_BY_USB;
#endif /* RTMP_MAC_PCI */
			else {
				/* Make sure WoW GPIO 5 set to default value before suspend
				  iwpriv wlan0 mac 120=100000
				  iwpriv wlan0 mac 1b8=FFFFFFDF
				  iwpriv wlan0 mac 1bc=20 for high and 0 for low
				  Should set GPIO level firstly */
				RTMP_IO_READ32(ad, 0x1bc, &Value);
				if (ad->WOW_Cfg.bHighActive)
					Value &= ~(0x20);
				else
					Value |= 0x20;
				RTMP_IO_WRITE32(ad, 0x1bc, Value);

				RTMP_IO_READ32(ad, 0x120, &Value);
				Value |= 0x100000;
				RTMP_IO_WRITE32(ad, 0x120, Value);

				RTMP_IO_READ32(ad, 0x1b8, &Value);
				Value &= ~(0x20);
				RTMP_IO_WRITE32(ad, 0x1b8, Value);

				wow_param.Value = WOW_WAKEUP_BY_GPIO;
				/* GPIO_ACTION - Bit[31]: GPIO High/Low, Bit[15:0]: Duration
					Revert the GPIO if HoldTime is not set to 0 */
				if (ad->WOW_Cfg.nHoldTime == 0) {
					wow_param.GPIO_Action_1 =
						((ad->WOW_Cfg.bHighActive << 31) | 0);
					wow_param.GPIO_Action_2 =
						((ad->WOW_Cfg.bHighActive << 31) | 0);
					wow_param.GPIO_Action_3 =
						((ad->WOW_Cfg.bHighActive << 31) | 0);
				} else {
					wow_param.GPIO_Action_1 =
						((ad->WOW_Cfg.bHighActive << 31) | 0);
					wow_param.GPIO_Action_2 =
						((ad->WOW_Cfg.bHighActive << 31)
							| ad->WOW_Cfg.nHoldTime);
					wow_param.GPIO_Action_3 =
						(((ad->WOW_Cfg.bHighActive ^ 1) << 31) | 0);
				}
				RTMP_IO_READ32(ad, WLAN_FUN_CTRL, &Value);
				DBGPRINT(RT_DEBUG_ERROR,
					("%s: MAC 0x80 = 0x%x\n", __func__, Value));
				Value &= ~0x01010000; /* GPIO0(ouput) --> 0(data) */
				RTMP_IO_WRITE32(ad, WLAN_FUN_CTRL, Value);
			}

			andes_wow_send_config_cmd(ad, CMD_WOW_FEATURE,
				FALSE, 0, FALSE, (PUINT32)&wow_param, sizeof(wow_param));
		}
	}

	/* Offload ARP */
	if (NdisCmpMemory(zeroIP, ad->WOW_Cfg.ArpIP, sizeof(zeroIP)) != 0) {
		NdisZeroMemory(&offload_arp_cfg, sizeof(offload_arp_cfg));
		offload_arp_cfg.Config_Type = WOW_ARP_CFG;
		offload_arp_cfg.IpIdx = 0;
		offload_arp_cfg.IsEnable = 1;
		offload_arp_cfg.OffloadType = 0;
		for (i = 0; i < 4; i++)
			offload_arp_cfg.IP[i] = ad->WOW_Cfg.ArpIP[i];

		andes_wow_send_config_cmd(ad, CMD_WOW_CONFIG,
			FALSE, 0, FALSE, (PUINT32)&offload_arp_cfg, sizeof(offload_arp_cfg));
		DBGPRINT(RT_DEBUG_ERROR, ("ARP IP:[%d.%d.%d.%d]\n",
				offload_arp_cfg.IP[0], offload_arp_cfg.IP[1],
				offload_arp_cfg.IP[2], offload_arp_cfg.IP[3]));
	}

	/* Step 4: Set WOW security configuration if encrypted */
	if (ad->StaCfg.wdev.AuthMode >= Ndis802_11AuthModeWPAPSK) {
		NdisZeroMemory(&sec_cfg, sizeof(sec_cfg));
		sec_cfg.Config_Type = WOW_SEC_CFG;	/* security config */

		if (ad->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPAPSK)
			sec_cfg.WPA_Ver = 0;
		else if (ad->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2PSK)
			sec_cfg.WPA_Ver = 1;

#ifdef RT_CFG80211_SUPPORT
		NdisCopyMemory(sec_cfg.PTK, ad->WOW_Cfg.PTK, LEN_PTK);
		NdisCopyMemory(sec_cfg.R_COUNTER, ad->WOW_Cfg.ReplayCounter, LEN_KEY_DESC_REPLAY);
#else
		NdisCopyMemory(sec_cfg.PTK, pEntry->PTK, LEN_PTK);
		NdisCopyMemory(sec_cfg.R_COUNTER, pEntry->R_Counter, LEN_KEY_DESC_REPLAY);
#endif
		sec_cfg.Key_Id = ad->StaCfg.wdev.DefaultKeyId;
		sec_cfg.Cipher_Alg = pEntry->WepStatus;
		sec_cfg.Group_Cipher = ad->StaCfg.GroupCipher;
		sec_cfg.Aid = pEntry->Aid;

		DBGPRINT(RT_DEBUG_TRACE, ("%s: wep status %d\n", __func__, pEntry->WepStatus));
		DBGPRINT(RT_DEBUG_TRACE, ("%s: group status %d\n", __func__, sec_cfg.Group_Cipher));
		DBGPRINT(RT_DEBUG_TRACE, ("%s: aid %d\n", __func__, pEntry->Aid));
		DBGPRINT(RT_DEBUG_TRACE, ("Key = %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
					sec_cfg.PTK[0], sec_cfg.PTK[1],
					sec_cfg.PTK[2], sec_cfg.PTK[3],
					sec_cfg.PTK[4], sec_cfg.PTK[5],
					sec_cfg.PTK[6], sec_cfg.PTK[7]));
		DBGPRINT(RT_DEBUG_TRACE, (":%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n",
					sec_cfg.PTK[8], sec_cfg.PTK[9],
					sec_cfg.PTK[10], sec_cfg.PTK[11],
					sec_cfg.PTK[12], sec_cfg.PTK[13],
					sec_cfg.PTK[14], sec_cfg.PTK[15]));

		andes_wow_send_config_cmd(ad, CMD_WOW_CONFIG,
			FALSE, 0, FALSE, (PUINT32)&sec_cfg, sizeof(sec_cfg));
	}

	/* Step 5.1 : Set WOW Infra configuration */
	NdisZeroMemory(&infra_cfg, sizeof(infra_cfg));
	infra_cfg.Config_Type = WOW_INFRA_CFG;	/* infra config */
	COPY_MAC_ADDR(infra_cfg.STA_MAC, ad->CurrentAddress);
	COPY_MAC_ADDR(infra_cfg.AP_MAC, ad->CommonCfg.Bssid);
	DBGPRINT(RT_DEBUG_ERROR, ("AP MAC ===>	%x:%x:%x:%x:%x:%x\n",
					PRINT_MAC(infra_cfg.AP_MAC)));
	DBGPRINT(RT_DEBUG_ERROR, ("STA MAC ===>  %x:%x:%x:%x:%x:%x\n",
					PRINT_MAC(infra_cfg.STA_MAC)));

	if (OPSTATUS_TEST_FLAG(ad, fOP_STATUS_MEDIA_STATE_CONNECTED))
		infra_cfg.AP_Status = TRUE;
	else
		infra_cfg.AP_Status = FALSE;

	DBGPRINT(RT_DEBUG_ERROR,
		("WOW config AP status: %s\n", infra_cfg.AP_Status == TRUE?"TRUE":"FALSE"));
	andes_wow_send_config_cmd(ad, CMD_WOW_CONFIG,
		FALSE, 0, FALSE, (PUINT32)&infra_cfg, sizeof(infra_cfg));

	/* Step 5.2: Set AID Channel */
	NdisZeroMemory(&wow_aid_channel, sizeof(wow_aid_channel));
	wow_aid_channel.Parameter = WOW_AID_CHANNEL;	/* AID, CHANNEL assign */
	wow_aid_channel.Aid = pEntry->Aid;				/* AID assigned from AP */
	wow_aid_channel.Channel = ad->WOW_Cfg.CentralChannel;	/* Central Channel */
	wow_aid_channel.extCH = ad->WOW_Cfg.ExtChSetting;	/* Extension Channel Setting */

	DBGPRINT(RT_DEBUG_ERROR, ("aid = %d\n", wow_aid_channel.Aid));
	DBGPRINT(RT_DEBUG_ERROR, ("central channel = %d\n", wow_aid_channel.Channel));
	DBGPRINT(RT_DEBUG_ERROR, ("extension channel setting = %d\n", wow_aid_channel.extCH));
	andes_wow_send_config_cmd(ad, CMD_WOW_FEATURE,
		FALSE, 0, FALSE, (PUINT32)&wow_aid_channel, sizeof(wow_aid_channel));

	/* Step 5.3 : Enable Qos packet and set sequence number  */
	NdisZeroMemory(&wow_qos_seq_cfg, sizeof(wow_qos_seq_cfg));
	wow_qos_seq_cfg.Parameter = WOW_QOS_SEQNUM;
	wow_qos_seq_cfg.Enable_Qos = WOW_QOSPKT_ENABLE;
	wow_qos_seq_cfg.Seq_Num = pEntry->TxSeq[QID_AC_BE];
	DBGPRINT(RT_DEBUG_ERROR,
		("%s: Qos: %d and Sequence number: %d\n",
			__func__, wow_qos_seq_cfg.Enable_Qos, wow_qos_seq_cfg.Seq_Num));
	andes_wow_send_config_cmd(ad, CMD_WOW_FEATURE,
		FALSE, 0, FALSE, (PUINT32)&wow_qos_seq_cfg, sizeof(wow_qos_seq_cfg));

	/* Step 5.4 : Enable wow power-saving */
	andes_pwr_saving(ad, RADIO_ON_ADVANCE, 4, ad->MlmeAux.BeaconPeriod, 0, 0, 0);
	DBGPRINT(RT_DEBUG_ERROR, ("Beacon interval =  %d\n", ad->MlmeAux.BeaconPeriod));

	/* Step 5.5 : Enable USB2 disconnect feature */
	if (ad->dp_ctrl.nUsb2DisconMode != 0)
		usb2_disconnect_cmd(ad, ad->dp_ctrl.nUsb2DisconMode,
					ad->dp_ctrl.nGPIONotifyMode, ad->dp_ctrl.nGPIONotifyPin);

	return NDIS_STATUS_SUCCESS;
}

int andes_wow_enable(struct _RTMP_ADAPTER *ad)
{				/* , RTMP_ADAPTER *ad, u32 cal_id, u32 param) */
	u32 value;
	int ret = 0;
	BOOLEAN Cancelled;
#if (defined(P2P_SUPPORT) || defined(RT_CFG80211_P2P_CONCURRENT_DEVICE)) && defined(WOW_STA_MODE)
	ULONG Addr4;
	PUCHAR pBssid = NULL;
	MAC_TABLE_ENTRY *pEntry = &ad->MacTab.Content[BSSID_WCID];
#endif /* endif */

	DBGPRINT(RT_DEBUG_TRACE, ("%s:--->\n", __func__));

	ad->WOW_Cfg.WakeupReason = 0;

	if (!RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_START_UP)) {
		DBGPRINT(RT_DEBUG_TRACE,
			("RTMP_ADAPTER_START_UP flag has been clear !\n"));
		goto error;
	}
#ifdef NEW_WOW_SUPPORT
	/* Block data EP bulkout before waiting Tx Q empty */
	if (!RTMP_TEST_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING)) {
		RTMP_SET_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING);
	} else {
		CFG80211DBG(RT_DEBUG_ERROR,
			    ("[%s] --------------Duplicate Suspend Operation\n", __func__));
		goto error;
	}
#endif /* NEW_WOW_SUPPORT */

	RTUSBReadMACRegister(ad, 0x738, &value);
	DBGPRINT(RT_DEBUG_TRACE, ("F/W Heart beat(0x738): 0x%x\n", value));

#ifdef RT_CFG80211_SUPPORT
		if (ad->cfg80211_ctrl.FlgCfg80211Scanning == TRUE ||
			RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
			CFG80211DBG(RT_DEBUG_ERROR, ("[%s] Clear ongoing scan\n", __func__));
			CFG80211DRV_OpsScanInLinkDownAction(ad);
		}
#endif /* RT_CFG80211_SUPPORT */

#if defined(RT_CFG80211_P2P_CONCURRENT_DEVICE) && defined(WOW_STA_MODE)
	/* The BSSID is set to MAC 0x1010 & 0x1014.
	If P2P_SUPPORT is enabled, then the BSSID
	would be set to P2P's MAC address but not
	AP's MAC address.
	It would cause FW fail to decrypt the
	broadcast packet from AP due to the BSSID
	is not set to AP's MAC address.
	Therefore, before entering suspend, change
	the BSSID from P2P's MAC to AP's MAC can
	avoid this issue. */

	pBssid = ad->CommonCfg.Bssid;
	DBGPRINT(RT_DEBUG_ERROR, ("===> WOW Set Bssid %x:%x:%x:%x:%x:%x\n",
				PRINT_MAC(pBssid)));

	Addr4 = (ULONG)(pBssid[0]) |
			(ULONG)(pBssid[1] << 8)  |
			(ULONG)(pBssid[2] << 16) |
			(ULONG)(pBssid[3] << 24);
	RTMP_IO_WRITE32(ad, MAC_BSSID_DW0, Addr4);
	RTMP_IO_WRITE32(ad, MAC_BSSID1_DW0, Addr4);

	Addr4 = 0;
	/* always one BSSID in STA mode*/
	Addr4 = (ULONG)(pBssid[4]) | (ULONG)(pBssid[5] << 8);
	RTMP_IO_WRITE32(ad, MAC_BSSID_DW1, Addr4);

	Addr4 = ((ULONG)(pBssid[4]) | (ULONG)(pBssid[5] << 8) | (ULONG)(pEntry->Aid << 16));
	RTMP_IO_WRITE32(ad, MAC_BSSID1_DW1, Addr4);

	/* MAC filter: Enable not my bssid frame drop */
	Addr4 = 0;
	RTMP_IO_READ32(ad, RX_FILTR_CFG, &Addr4);
	RTMP_IO_WRITE32(ad, RX_FILTR_CFG, (Addr4 | BIT3));
#endif


	/* Step 1: Set WOW feature enable */
	andes_wow_feature_enable(ad, TRUE);

	/* Step 2 : Set WOW mask configuration  */
	andes_wow_set_configuration(ad);

	/* Perform HW CR register check before going into suspend */
	andes_suspend_CR_setting(ad);

	/* Step 6 : Set traffic to Andes */
	andes_wow_traffic_switch(ad, WOW_PKT_TO_ANDES);

	/* Dump register value before suspend */
	Set_Register_Dump(ad, NULL);

	ad->WOW_Cfg.bInSuspendMode = WOW_SUSPEND_COMPLETE;
	DBGPRINT(RT_DEBUG_TRACE, ("bInSuspendMode=  %d\n", ad->WOW_Cfg.bInSuspendMode));
	RTMPCancelTimer(&ad->Mlme.PeriodicTimer, &Cancelled);

	RTMP_SET_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDED);
	RTMP_SET_FLAG(ad, fRTMP_ADAPTER_IDLE_RADIO_OFF);

error:
	return ret;
}

#ifdef WOW_BUF_PKT_SUPPORT
static int andes_wow_query_buffered_packet(RTMP_ADAPTER *pAd)
{
	struct NEW_WOW_BUF_PKT_STRUCT buf_pkt;

	/* Retrieve wake-up packet buffered by FW */
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Get buffered packet\n", __func__));
	NdisZeroMemory(&buf_pkt, sizeof(buf_pkt));
	buf_pkt.Config_Type = WOW_QUERY_1ST_WAKEUP_PACKET;
	andes_wow_send_config_cmd(pAd,
		CMD_WOW_QUERY, FALSE, 0, FALSE, (PUINT32)&buf_pkt, sizeof(buf_pkt));

	/* Retrieve wake-up reason */
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Get wakeup reason\n", __func__));
	NdisZeroMemory(&buf_pkt, sizeof(buf_pkt));
	buf_pkt.Config_Type = WOW_QUERY_EVENT;
	andes_wow_send_config_cmd(pAd,
		CMD_WOW_QUERY, FALSE, 0, FALSE, (PUINT32)&buf_pkt, sizeof(buf_pkt));

	return NDIS_STATUS_SUCCESS;
}
#endif /* WOW_BUF_PKT_SUPPORT */

static int andes_wow_query_seqnum_packet(RTMP_ADAPTER *pAd)
{
	struct NEW_WOW_BUF_SEQ_NUM_CFG_STRUCT	buf_seq_num_pkt;
	struct cmd_msg *msg = NULL;
	u32 value, i;
	u32 *data = NULL;
	int ret = NDIS_STATUS_SUCCESS;
	CHAR buffer[10];

	DBGPRINT(RT_DEBUG_ERROR, ("%s: Get sequence number\n", __func__));
	NdisZeroMemory(&buf_seq_num_pkt, sizeof(buf_seq_num_pkt));
	buf_seq_num_pkt.Config_Type = WOW_QUERY_SEQ_NUM;

	msg = andes_alloc_cmd_msg(pAd, sizeof(buf_seq_num_pkt));
	if (!msg) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW andes_alloc_cmd_msg fail\n", __func__));
		return NDIS_STATUS_RESOURCES;
	}

	andes_init_cmd_msg(msg, CMD_WOW_QUERY,
		TRUE, TRAFFIC_SWITCH_COMMAND_TIMEOUT, TRUE, TRUE, 10, buffer, NULL);

	data = (PUINT32)&buf_seq_num_pkt;
	for (i = 0; i < (sizeof(buf_seq_num_pkt)/4); i++) {
		value = cpu2le32(data[i]);
		andes_append_cmd_msg(msg, (char *)&value, 4);
	}

	ret = andes_send_cmd_msg(pAd, msg);
	if (ret) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: WOW send command CMD_WOW_QUERY fail\n", __func__));
		return ret;
	}

	return NDIS_STATUS_SUCCESS;
}

static int andes_wow_set_gpio(RTMP_ADAPTER *pAd)
{
	u32 Value = 0;

	/* restore hardware remote wakeup flag */
	RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &Value);
	DBGPRINT(RT_DEBUG_ERROR, ("%s: MAC:0x80 %08x\n", __func__, Value));
	Value &= ~0x80;
	RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, Value);

	if (pAd->WOW_Cfg.bInBand == FALSE) {
		RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &Value);
		DBGPRINT(RT_DEBUG_ERROR, ("%s: 0x80 = %x\n", __func__, Value));
		Value &= ~0x01010000; /* GPIO0(ouput) --> 0(data) */
		RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, Value);
	}

	if (!pAd->WOW_Cfg.bWmtGPIOCmd) {
		/* Make sure WoW GPIO 5 pull back to default value after resume
		  iwpriv wlan0 mac 120=100000
		  iwpriv wlan0 mac 1b8=FFFFFFDF
		  iwpriv wlan0 mac 1bc=20 for high and 0 for low
		  Should set GPIO level firstly */
		RTMP_IO_READ32(pAd, 0x1bc, &Value);
		if (pAd->WOW_Cfg.bHighActive)
			Value &= ~(0x20);
		else
			Value |= 0x20;
		RTMP_IO_WRITE32(pAd, 0x1bc, Value);

		RTMP_IO_READ32(pAd, 0x120, &Value);
		Value |= 0x100000;
		RTMP_IO_WRITE32(pAd, 0x120, Value);

		RTMP_IO_READ32(pAd, 0x1b8, &Value);
		Value &= ~(0x20);
		RTMP_IO_WRITE32(pAd, 0x1b8, Value);
	}

	return NDIS_STATUS_SUCCESS;
}

static int andes_wow_check_status(RTMP_ADAPTER *pAd)
{
	#define WAIT_RETRY_COUNT 500
	#define WAIT_DELAY_TIME_US	(1000) /* in micro-second */
	UINT32 count = 0, Value = 0;
	ULONG jiff_now = 0;

#ifdef RT_CFG80211_SUPPORT
	if (pAd->cfg80211_ctrl.FlgCfg80211Scanning == TRUE ||
		RTMP_TEST_FLAG(pAd, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
		CFG80211DBG(RT_DEBUG_ERROR, ("[%s] Clear ongoing scan\n", __func__));
		CFG80211DRV_OpsScanInLinkDownAction(pAd);
	}
#endif

	/* Dump Register Value After WoW Disable */
	Set_Register_Dump(pAd, NULL);

	/* MAC Tx/Rx Status Checking */
	count = 0;
	while (count < WAIT_RETRY_COUNT) {
		RtmpusecDelay(WAIT_DELAY_TIME_US); /* 1 ms*/
		RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);

		if ((Value & (BIT2|BIT3)) == (BIT2|BIT3))
			break;

		DBGPRINT(RT_DEBUG_ERROR, ("%s: MAC Tx/Rx not enabled [%X]\n", __func__, Value));

		Value |= (BIT2|BIT3);
		RTMP_IO_WRITE32(pAd, MAC_SYS_CTRL, Value);

		count++;
		if (count == WAIT_RETRY_COUNT && ((Value & (BIT2|BIT3)) != (BIT2|BIT3)))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: MAC Tx/Rx not enabled\n", __func__));
	}

	/* 11/27-As commented by Shangwei, radio on will be done by FW,
	   driver does not need to set radio. We will just read
	   MAC 0x80 and check if BIT0 is ON(1) or OFF(0).
	*/

	 /* Wi-Fi Radio Status Checking */
	count = 0;
	while (count < WAIT_RETRY_COUNT) {
		RtmpusecDelay(WAIT_DELAY_TIME_US);/* 1 ms */
		RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &Value);

		if ((Value & BIT0) == BIT0)
			break;

		DBGPRINT(RT_DEBUG_ERROR, ("%s: Unable to turn radio on [%X]\n", __func__, Value));

		Value |= BIT0;
		RTMP_IO_WRITE32(pAd, WLAN_FUN_CTRL, Value);

		count++;
		if (count == WAIT_RETRY_COUNT && ((Value & BIT0) != BIT0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Unable to turn radio on\n", __func__));
	}

	/* Traffic Switch Status Checking */
	count = 0;
	while (count < WAIT_RETRY_COUNT) {
		RtmpusecDelay(WAIT_DELAY_TIME_US); /* 1 ms*/
		RTMP_IO_READ32(pAd, FCE_L2_STUFF, &Value);

		if ((Value & (BIT24|BIT25)) == (BIT24|BIT25))
			break;

		DBGPRINT(RT_DEBUG_ERROR, ("%s: Unable to switch traffic [%X]\n", __func__, Value));
		Value |= (BIT24|BIT25);
		RTMP_IO_WRITE32(pAd, FCE_L2_STUFF, Value);

		count++;
		if (count == WAIT_RETRY_COUNT && ((Value & (BIT24|BIT25)) != (BIT24|BIT25)))
			DBGPRINT(RT_DEBUG_ERROR,
				("%s: Unable to switch traffic to host\n", __func__));
	}

	 /*
		 Fail recovery.
		 When resume from wow, if driver link is up, but port not secured. It may
		 imply upper layer has stopped to proceed WPA handshake.
		 In such condition, it may cause driver to block subsequent scan requests
		 due to port not secured forever.
		 So we may need to trigger LinkDown in such case, to let driver reset
		 to link down state, and allow new scans.
	 */
	 if ((OPSTATUS_TEST_FLAG(pAd, fOP_STATUS_MEDIA_STATE_CONNECTED)) &&
		 ((pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA) ||
			 (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPAPSK) ||
			 (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2) ||
			 (pAd->StaCfg.wdev.AuthMode == Ndis802_11AuthModeWPA2PSK)) &&
		 (pAd->StaCfg.wdev.PortSecured == WPA_802_1X_PORT_NOT_SECURED)) {
		DBGPRINT(RT_DEBUG_ERROR,
			("%s: Link UP but port not secured, force LinkDown!\n", __func__));
		LinkDown(pAd, FALSE);
	 }

	/* Compensate last Beacon Rx time */
	NdisGetSystemUpTime(&jiff_now);
	MlmeCompensateLastBeaconRxTime(pAd, jiff_now);

	/*
		* BT in low power might disable the thermal clock
		* It might cause vendor req fail on mt76x2_get_current_temp
		* do TEMP_SENSOR_CALIBRATION can enable it
	*/
	RTMP_IO_READ32(pAd, MAC_SYS_CTRL, &Value);
	if (INFRA_ON(pAd) && ((Value & WLAN_FUN_CTRL_THERM_CKEN)
				!= WLAN_FUN_CTRL_THERM_CKEN)) {
		/* TEMP SENSOR */
		CHIP_CALIBRATION(pAd, TEMP_SENSOR_CALIBRATION_7662, 0x00);
		RTMP_IO_READ32(pAd, WLAN_FUN_CTRL, &Value);
		DBGPRINT(RT_DEBUG_OFF, ("%s: 0x80 = %x\n", __func__, Value));
	}

	return NDIS_STATUS_SUCCESS;
}

int andes_wow_disable(struct _RTMP_ADAPTER *ad)
{
	UINT32 Value;
	int ret = 0;

#if (defined(P2P_SUPPORT) || defined(RT_CFG80211_P2P_CONCURRENT_DEVICE)) && defined(WOW_STA_MODE)
	PUCHAR pP2PBssid = NULL;
	UINT32 Addr4;
	UINT32 regValue;
#endif /* endif */

	DBGPRINT(RT_DEBUG_TRACE, ("%s:\n", __func__));
	ad->WOW_Cfg.bInSuspendMode = WOW_NOT_IN_SUSPEND;
	DBGPRINT(RT_DEBUG_TRACE, ("bInSuspendMode=  %d\n", ad->WOW_Cfg.bInSuspendMode));

	/* Dump Register Value before WoW Disable */
	Set_Register_Dump(ad, NULL);

	/* 11/27 commented by FW member:
	   Driver does not need to stop MAC Tx/Rx when resumed.
	   FW will stop the MAC Rx by its own.
	   Driver needs to send the WOW_PKT_TO_HOST first, then enable
	   MAC Tx/Rx.
	   So we disable the following code block that was trying to diable MAC T/R.
	 */

	andes_resume_CR_setting(ad, WOW_RESUME_TYPE);
#if (defined(P2P_SUPPORT) || defined(RT_CFG80211_P2P_CONCURRENT_DEVICE)) && defined(WOW_STA_MODE)
	/*
	   The BSSID is set to MAC 0x1010 & 0x1014.
	   If P2P_SUPPORT is enabled, then the BSSID would be set to P2P's MAC address but not AP's MAC address.
	   It would cause FW fail to decrypt the broadcast packet from AP due to the BSSID is not set to AP's
	   MAC address.
	   Therefore, before entering suspend, change the BSSID from P2P's MAC to AP's MAC can avoid this issue.
	   And change back to P2P's MAC after resume.
	 */

	Addr4 = 0;
	RTMP_IO_READ32(ad, MAC_BSSID1_DW1, &Addr4);
	Addr4 |= BSSID_EN;
	RTMP_IO_WRITE32(ad, MAC_BSSID1_DW1, Addr4);

	pP2PBssid = &ad->CurrentAddress[0];

	Addr4 = (UINT32) (pP2PBssid[0]) |
	    (UINT32) (pP2PBssid[1] << 8) |
	    (UINT32) (pP2PBssid[2] << 16) | (UINT32) (pP2PBssid[3] << 24);
	RTMP_IO_WRITE32(ad, MAC_BSSID_DW0, Addr4);

	Addr4 = 0;

	/* always one BSSID in STA mode */
	Addr4 = (UINT32) (pP2PBssid[4]) | (ULONG) (pP2PBssid[5] << 8);

	RTMP_IO_WRITE32(ad, MAC_BSSID_DW1, Addr4);

	RTMP_IO_READ32(ad, MAC_BSSID_DW1, &regValue);
	regValue &= 0x0000FFFF;
	regValue |= (1 << 16);
	/*      set as 0/1 bit-21 of MAC_BSSID_DW1(offset: 0x1014)
	   to disable/enable the new MAC address assignment.  */
	if (ad->chipCap.MBSSIDMode == MBSSID_MODE1)
		regValue |= (1 << 21);
	RTMP_IO_WRITE32(ad, MAC_BSSID_DW1, regValue);

	/* MAC filter: Disble not my bssid frame drop */
	Addr4 = 0;
	RTMP_IO_READ32(ad, RX_FILTR_CFG, &Addr4);
	RTMP_IO_WRITE32(ad, RX_FILTR_CFG, (Addr4 & (~BIT3)));
#endif /* endif */

	/*
		Note: As suggested by FW member,
		the suggested sequence for WOW cmds to FW are as below:
			Step 1: Set traffic to Host
			Step 2: Set WOW feature disable
	*/

	/* ---- Step 1 : Set traffic to Host ---- */
	andes_wow_traffic_switch(ad, WOW_PKT_TO_HOST);

	/* ---- Step 2: Set WOW feature disable ---- */
	andes_wow_feature_enable(ad, FALSE);

	/* ---- Step 3: Restore MAC TX/RX, and lunch Data Rx flow ---- */
	DBGPRINT(RT_DEBUG_ERROR, ("Restore MAC Tx/Rx by Host Driver\n"));
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &Value);
	Value |= 0xC;
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, Value);

	/* 11/27-As commented by Shangwei, radio on will be done by FW,
	   driver does not need to set radio. We will just read
	   MAC 0x80 and check if BIT0 is ON(1) or OFF(0).
	 */
	RTUSBBulkReceive(ad);

	/* Query pakcet's sequence number */
	andes_wow_query_seqnum_packet(ad);


#ifdef WOW_BUF_PKT_SUPPORT
	/* ---- Step 4: Query buffered packet (if any) ---- */
	andes_wow_query_buffered_packet(ad);
#endif /* WOW_BUF_PKT_SUPPORT */

	/* ---- Step 5: Set GPIO-related behavior ---- */
	andes_wow_set_gpio(ad);

#if (defined(P2P_SUPPORT) || defined(RT_CFG80211_P2P_CONCURRENT_DEVICE))
		if (ad->cfg80211_ctrl.FlgCfg80211Scanning == TRUE ||
			RTMP_TEST_FLAG(ad, fRTMP_ADAPTER_BSS_SCAN_IN_PROGRESS)) {
			CFG80211DBG(RT_DEBUG_ERROR, ("[%s] Clear ongoing scan\n", __func__));
			CFG80211DRV_OpsScanInLinkDownAction(ad);
		}
#endif /* endif */

	/* ---- Step 6: Check  overall status ---- */
	andes_wow_check_status(ad);

	RTMP_CLEAR_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDING);
	RTMP_CLEAR_SUSPEND_FLAG(ad, fRTMP_ADAPTER_SUSPEND_STATE_SUSPENDED);
	RTMPInitTimer(ad, &ad->Mlme.PeriodicTimer, GET_TIMER_FUNCTION(MlmePeriodicExec), ad, TRUE);
	RTMPSetTimer(&ad->Mlme.PeriodicTimer, MLME_TASK_EXEC_INTV);

	if (INFRA_ON(ad))
		RTMPSendNullFrame(ad, ad->CommonCfg.TxRate, TRUE, FALSE);

	return ret;
}
#endif /* endif */

int andes_load_cr(RTMP_ADAPTER *ad, u32 cr_type, UINT8 temp_level, UINT8 channel)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret = 0;

	DBGPRINT(RT_DEBUG_OFF,
		 ("%s:cr_type(%d) temp_level(%d) channel(%d)\n", __func__, cr_type, temp_level,
		  channel));

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_LOAD_CR, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/* CR type */
	value &= ~LOAD_CR_MODE_MASK;
	value |= LOAD_CR_MODE(cr_type);

	if (cr_type == HL_TEMP_CR_UPDATE) {
		value &= ~LOAD_CR_TEMP_LEVEL_MASK;
		value |= LOAD_CR_TEMP_LEVEL(temp_level);

		value &= ~LOAD_CR_CHL_MASK;
		value |= LOAD_CR_CHL(channel);
	}

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	value = 0x80000000;
	value |= ((ad->EEPROMDefaultValue[EEPROM_NIC_CFG1_OFFSET] >> 8) & 0xFF);
	value |= ((ad->EEPROMDefaultValue[EEPROM_NIC_CFG2_OFFSET] & 0xFF) << 8);
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

int andes_switch_channel(RTMP_ADAPTER *ad, u8 channel, BOOLEAN scan, unsigned int bw,
			 unsigned int tx_rx_setting, u8 bbp_ch_idx)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret;

	DBGPRINT(RT_DEBUG_INFO,
		 ("%s:channel(%d),scan(%d),bw(%d),trx(0x%x)\n", __func__, channel, scan, bw,
		  tx_rx_setting));

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_SWITCH_CHANNEL_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/*
	 * switch channel related param
	 * channel, scan, bw, tx_rx_setting
	 */
	value &= ~SC_PARAM1_CHL_MASK;
	value |= SC_PARAM1_CHL(channel);
	value &= ~SC_PARAM1_SCAN_MASK;
	value |= SC_PARAM1_SCAN(scan);
	value &= ~SC_PARAM1_BW_MASK;
	value |= SC_PARAM1_BW(bw);
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	value = 0;
	value |= SC_PARAM2_TR_SETTING(tx_rx_setting);
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

	mdelay(5);

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_SWITCH_CHANNEL_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/*
	 * switch channel related param
	 * channel, scan, bw, tx_rx_setting, extension channel
	 */
	value &= ~SC_PARAM1_CHL_MASK;
	value |= SC_PARAM1_CHL(channel);
	value &= ~SC_PARAM1_SCAN_MASK;
	value |= SC_PARAM1_SCAN(scan);
	value &= ~SC_PARAM1_BW_MASK;
	value |= SC_PARAM1_BW(bw);
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	value = 0;
	value |= SC_PARAM2_TR_SETTING(tx_rx_setting);
	value &= ~SC_PARAM2_EXTENSION_CHL_MASK;

	if (bbp_ch_idx == 0)
		value |= SC_PARAM2_EXTENSION_CHL(0xe0);
	else if (bbp_ch_idx == 1)
		value |= SC_PARAM2_EXTENSION_CHL(0xe1);
	else if (bbp_ch_idx == 2)
		value |= SC_PARAM2_EXTENSION_CHL(0xe2);
	else if (bbp_ch_idx == 3)
		value |= SC_PARAM2_EXTENSION_CHL(0xe3);

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

#ifdef CONFIG_MULTI_CHANNEL
/* mcc f/w cmd */

int andes_mcc_start(struct _RTMP_ADAPTER *ad,
		    u8 channel_1st,
		    u8 channel_2nd,
		    unsigned int bw_1st,
		    unsigned int bw_2nd,
		    u8 extch_1st,
		    u8 extch_2nd,
		    u8 role_1st,
		    u8 role_2nd,
		    u16 cycle_time_1st,
		    u16 cycle_time_2nd,
		    u16 wait_time,
		    u16 null_cnt,
		    u32 tssi_slope_offset_1st, u32 tssi_slope_offset_2nd, u32 start_tsf)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret;

	DBGPRINT(RT_DEBUG_ERROR,
		 ("\x1b[31m @@@@@ %s:channel(%u,%u), bw(%u,%u), role(%u,%u), cycle_time(%u,%u), wait_time(%u), null_cnt(%u), tssi_slope(0x%04x,%04x), start_tsf(0x%08x) \x1b[m\n",
		  __func__, channel_1st, channel_2nd, bw_1st, bw_2nd, role_1st, role_2nd,
		  cycle_time_1st, cycle_time_2nd, wait_time, null_cnt, tssi_slope_offset_1st,
		  tssi_slope_offset_2nd, start_tsf));

	msg = andes_alloc_cmd_msg(ad, 24);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	/*
	   Buffer 0 : channel_1st, channel_2nd, bw_1st, bw_2nd, role_1st, role_2nd
	 */
	andes_init_cmd_msg(msg, CMD_MCC_START, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	value &= ~MCC_START1_CHL_1ST_MASK;
	value |= MCC_START1_CHL_1ST(channel_1st);
	value &= ~MCC_START1_CHL_2ND_MASK;
	value |= MCC_START1_CHL_2ND(channel_2nd);
	value &= ~MCC_START1_BW_1ST_MASK;
	value |= MCC_START1_BW_1ST(bw_1st);
	value &= ~MCC_START1_BW_2ND_MASK;
	value |= MCC_START1_BW_2ND(bw_2nd);
	value &= ~MCC_START1_ROLE_1ST_MASK;
	value |= MCC_START1_ROLE_1ST(role_1st);
	value &= ~MCC_START1_ROLE_2ND_MASK;
	value |= MCC_START1_ROLE_2ND(role_2nd);

	value &= ~MCC_START1_TRX_1ST_MASK;
	value |= MCC_START1_TRX_1ST(0);
	value &= ~MCC_START1_TRX_2ND_MASK;
	value |= MCC_START1_TRX_2ND(0);

	value &= ~MCC_START1_ACQ_1ST_MASK;
	value |= MCC_START1_ACQ_1ST(2);
	value &= ~MCC_START1_ACQ_2ND_MASK;
	value |= MCC_START1_ACQ_2ND(3);

	DBGPRINT(RT_DEBUG_TRACE, ("%s:Buffer0 =%04x,\n", __func__, value));

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/*
	   Buffer 1 : cycle_time_1st, cycle_time_2nd
	 */
	value = 0;
	value &= ~MCC_START2_CYCLE_1ST_MASK;
	value |= MCC_START2_CYCLE_1ST(cycle_time_1st);
	value &= ~MCC_START2_CYCLE_2ND_MASK;
	value |= MCC_START2_CYCLE_2ND(cycle_time_2nd);

	DBGPRINT(RT_DEBUG_TRACE, ("%s:Buffer1 =%04x,\n", __func__, value));

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/*
	   Buffer 2 : wait_time, null_cnt
	 */
	value = 0;
	value &= ~MCC_START2_WTT_MASK;
	value |= MCC_START2_WTT(wait_time);
	value &= ~MCC_START2_NULL_CNT_MASK;
	value |= MCC_START2_NULL_CNT(null_cnt);

	value &= ~MCC_START2_EXTCH_1ST_MASK;
	value |= MCC_START2_EXTCH_1ST(extch_1st);
	value &= ~MCC_START2_EXTCH_2ND_MASK;
	value |= MCC_START2_EXTCH_2ND(extch_2nd);
/* value = cpu2le32(tssi_slope_offset_1st); */
	value = cpu2le32(value);
	DBGPRINT(RT_DEBUG_TRACE, ("Add extch(%d, %d), value=0x%x\n", extch_1st, extch_2nd, value));

	DBGPRINT(RT_DEBUG_TRACE, ("%s:Buffer2 =%04x,\n", __func__, value));

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/*
	   Buffer 3 : tssi_slope_offset_1st
	 */
	value = cpu2le32(tssi_slope_offset_1st);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/*
	   Buffer 4 : tssi_slope_offset_2nd
	 */
	value = cpu2le32(tssi_slope_offset_2nd);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/*
	   Buffer 5 : MCC start_tsf
	 */
	value = start_tsf;
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

int andes_mcc_stop(struct _RTMP_ADAPTER *ad, u8 parking_channel, u8 auto_resume_mode,
		   u16 auto_resume_time, u32 auto_resume_tsf)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret;

	DBGPRINT(RT_DEBUG_ERROR,
		 ("\x1b[32m @@@@@ %s:parking_channel(%u), auto_resume_mode(%u), auto_resume_time(0x%04x),auto_resume_tsf(0x%08x) \x1b[m\n",
		  __func__, parking_channel, auto_resume_mode, auto_resume_time, auto_resume_tsf));

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_MCC_STOP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

/*
	Buffer 0 : parking_channel, auto_resume_mode, auto_resume_time
*/

	value &= ~MCC_STOP1_PARK_CHL_MASK;
	value |= MCC_STOP1_PARK_CHL(parking_channel);

	value &= ~MCC_STOP1_AUTO_RESUME_MODE_MASK;
	value |= MCC_STOP1_AUTO_RESUME_MODE(auto_resume_mode);

	value &= ~MCC_STOP1_AUTO_RESUME_TIME_MASK;
	value |= MCC_STOP1_AUTO_RESUME_TIME(auto_resume_time);

	DBGPRINT(RT_DEBUG_TRACE, ("%s:Buffer0 =%04x,\n", __func__, value));

	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

/*
	Buffer 1 : auto_resume_tsf
*/
	value = cpu2le32(auto_resume_tsf);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}
#endif /* endif */

int andes_init_gain(RTMP_ADAPTER *ad, UINT8 channel, BOOLEAN force_mode,
		    unsigned int gain_from_e2p)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret = 0;

	DBGPRINT(RT_DEBUG_INFO, ("%s:channel(%d), force mode(%d), init gain parameter(0x%08x)\n",
				 __func__, channel, force_mode, gain_from_e2p));

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_INIT_GAIN_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/* init gain parameter#1 */
	if (force_mode == TRUE)
		value = 0x80000000;

	value |= channel;
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* init gain parameter#2 while force mode is enabled */
	value = gain_from_e2p;
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

#ifndef HOST_DYNAMIC_VGA_SUPPORT
int andes_dynamic_vga(RTMP_ADAPTER *ad, UINT8 channel, BOOLEAN mode, BOOLEAN ext, int rssi,
		      unsigned int false_cca)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int rssi_val = 0, ret = 0;

	DBGPRINT(RT_DEBUG_INFO,
		 ("%s:channel(%d), ap/sta mode(%d), extension(%d), rssi(%d), false cca count(%d)\n",
		  __func__, channel, mode, ext, rssi, false_cca));

	msg = andes_alloc_cmd_msg(ad, 12);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_DYNC_VGA_OP, TRUE, 0, TRUE, TRUE, 0, NULL, NULL);

	/* dynamic VGA parameter#1: TRUE = AP mode ; FALSE = STA mode */
	if (mode == TRUE)
		value |= 0x80000000;

	if (ext == TRUE)
		value |= 0x40000000;

	value |= channel;
	value = cpu2le32(value);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* dynamic VGA parameter#2: RSSI (signed value) */
	rssi_val = cpu2le32(rssi);
	andes_append_cmd_msg(msg, (char *)&rssi_val, 4);

	/* dynamic VGA parameter#3: false CCA count */
	value = cpu2le32(false_cca);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}
#endif /* HOST_DYNAMIC_VGA_SUPPORT */
int andes_led_op(RTMP_ADAPTER *ad, u32 led_idx, u32 link_status)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret = 0;

	DBGPRINT(RT_DEBUG_INFO, ("%s:led_idx(%d), link_status(%d)\n ",
				 __func__, led_idx, link_status));

	msg = andes_alloc_cmd_msg(ad, 8);

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_LED_MODE_OP, FALSE, 0, FALSE, FALSE, 0, NULL, NULL);

	/* Led index */
	value = cpu2le32(led_idx);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	/* Link status */
	value = cpu2le32(link_status);
	andes_append_cmd_msg(msg, (char *)&value, 4);

	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

#ifdef RT_CFG80211_SUPPORT
int andes_tx_status(RTMP_ADAPTER *ad, TX_STATUS_CTRL *TxStatus)
{
	struct cmd_msg *msg;
	u32 value = 0;
	int ret = 0;
	UINT32 *Status = (UINT32 *) TxStatus;

	DBGPRINT(RT_DEBUG_TRACE,
		 ("%s: MgmtID(%d) WCID(%d) Seq(%d) Timeout(%d) PktLen(%d)\n ", __func__,
		  TxStatus->MgmtID, TxStatus->WCID, TxStatus->Seq, TxStatus->Timeout,
		  TxStatus->PktLen));
	msg = andes_alloc_cmd_msg(ad, sizeof(TX_STATUS_CTRL));

	if (!msg) {
		ret = NDIS_STATUS_RESOURCES;
		goto error;
	}

	andes_init_cmd_msg(msg, CMD_TX_STATUS, FALSE, 0, FALSE, TRUE, 0, NULL, NULL);

	/* Tx Status */
	value = cpu2le32(Status[0]);
	andes_append_cmd_msg(msg, (char *)&value, 4);
	value = cpu2le32(Status[1]);
	andes_append_cmd_msg(msg, (char *)&value, 4);
	ret = andes_send_cmd_msg(ad, msg);

error:
	return ret;
}

#endif /* endif */

#ifdef RTMP_PCI_SUPPORT
void andes_pci_fw_init(RTMP_ADAPTER *ad)
{
	u32 value;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));
	/* Enable Interrupt */
	RTMP_IRQ_ENABLE(ad);
	RTMPEnableRxTx(ad);
	RTMP_SET_FLAG(ad, fRTMP_ADAPTER_START_UP);
	RTMP_SET_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	/* clear garbage interrupts */
	RTMP_IO_READ32(ad, 0x1300, &value);
	DBGPRINT(RT_DEBUG_OFF, ("0x1300 = %08x\n", value));

#ifdef HDR_TRANS_SUPPORT
	RTMP_IO_WRITE32(ad, HEADER_TRANS_CTRL_REG, 0X2);
	RTMP_IO_WRITE32(ad, TSO_CTRL, 0x7050);
#else
	RTMP_IO_WRITE32(ad, HEADER_TRANS_CTRL_REG, 0x0);
	RTMP_IO_WRITE32(ad, TSO_CTRL, 0x0);
#endif /* endif */

	andes_fun_set(ad, Q_SELECT, ad->chipCap.CmdRspRxRing);
	PWR_SAVING_OP(ad, RADIO_ON, 0, 0, 0, 0, 0);
}
#endif /* RTMP_PCI_SUPPORT */

#ifdef RTMP_USB_SUPPORT
void andes_usb_fw_init(RTMP_ADAPTER *ad)
{
	UCHAR index = 0;

	DBGPRINT(RT_DEBUG_OFF, ("%s\n", __func__));

#ifdef HDR_TRANS_SUPPORT
	RTMP_IO_WRITE32(ad, HEADER_TRANS_CTRL_REG, 0X2);
	RTMP_IO_WRITE32(ad, TSO_CTRL, 0x7050);
#else
	RTMP_IO_WRITE32(ad, HEADER_TRANS_CTRL_REG, 0x0);
	RTMP_IO_WRITE32(ad, TSO_CTRL, 0x0);
#endif /* endif */

	RT28XXDMAEnable(ad);
	RTMP_SET_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	andes_fun_set(ad, Q_SELECT, ad->chipCap.CmdRspRxRing);
	usb_rx_cmd_msgs_receive(ad);
#ifdef MULTI_URB
		for (index = 0; index < SUBMIT_URB_NUM; index++) {
			DoBulkIn(ad);
			DBGPRINT(RT_DEBUG_TRACE, ("DoBulkIn!\n"));
		}
#else
		for (index = 0; index < ad->CommonCfg.NumOfBulkInIRP; index++) {
			RTUSBBulkReceive(ad);
			DBGPRINT(RT_DEBUG_TRACE, ("RTUSBBulkReceive!\n"));
		}
#endif /* MULTI_URB */

#ifdef NEW_WOW_SUPPORT
	/* For MT76x2 USB IP Toggle Error Issue */
	if (!IS_MT76x2T(ad))
		andes_send_dummy_bulkout(ad);
#endif /* NEW_WOW_SUPPORT */

	PWR_SAVING_OP(ad, RADIO_ON, 0, 0, 0, 0, 0);
}
#endif /* RTMP_USB_SUPPORT */

int andes_check_tx_rx_disable(struct _RTMP_ADAPTER *ad)
{
	UINT32 Value = 0;

	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &Value);

	if ((Value & BIT2) || (Value & BIT3))
		return 0;
	else
		return 1;
}

#ifdef RTMP_USB_SUPPORT
void andes_suspend_CR_setting(struct _RTMP_ADAPTER *ad)
{
	UINT32 Value = 0;
	UINT32 count = 0;

	/* Suspend flow:
	Recommended by DE team:

	1. Check UDMA Tx EP4~EP9 is empty  (0x2240 ~ 0x2290, check BIT17==1)
	2. Wait UDMA Tx state idle (0x9100, check 0x7f00000==0)
	3. Check FCE Tx empty (0x0A30, check 0xFF==0)
	4. check FCE Tx2 empty (0x0A34, check 0xFF00==0)
	5. Disable MAC Tx (0x1004, set BIT2=0)
	6. Polling MAC Tx state to idle (0x1200, check BIT0==0)
	7. Disable MAC Rx (0x1004, set BIT3=0)
	8. Polling MAC Rx state to idle (0x1200, check BIT1==0)
	8.5.Check PBF Rx empty (0x0430, check 0xFF0000 ==0)
	9. Check FCE Rx1 empty (0x0A30, check 0xFF00 ==0)
	10. Check FCE Rx2 empty (0x0A34, check 0xFF == 0)
	11. Wait UDMA Rx state idle (0x9110, check 0x3F00==0)
	12. Check UDMA IN (Rx) EP4~EP5 empty (0x2140 & 0x2150, check BIT24==1)
	*/

	/* ------------------------------------------------------------
	1.Check UDMA Tx EP4~EP9 is empty  (0x2240 ~ 0x2290, check BIT17==1)
	---------------------------------------------------------------*/
	/* Wait UDMA EP4 Tx is Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms*/
		read_reg(ad, 0x40, 0x2240, &Value);

		/* if UDMA EP4 is Empty */
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP4 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP4 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR,
				 ("%s: UDMA EP4-TX Not Empty!!! (seems not a problem though)\n",
				  __func__));
	}

	/* Wait UDMA EP5 Tx is Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x2250, &Value);

		/* if UDMA EP5 is Empty */
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP5 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP5 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP5-TX Not Empty!!!\n",  __func__));
	}

	/* Wait UDMA EP6 Tx is Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x2260, &Value);

		/* if UDMA EP6 is Empty F*/
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP6 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP6 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP6-TX Not Empty!!!\n",  __func__));
	}

	/* Wait UDMA EP7 Tx is Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x2270, &Value);

		/* if UDMA EP7 is Empty */
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP7 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP7 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP7-TX Not Empty!!!\n",  __func__));
	}

	/* Wait UDMA EP8 Tx is Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x2280, &Value);

		/* if UDMA EP8 is Empty */
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP8 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP8 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP8-TX Not Empty!!!\n",  __func__));
	}

	/* Wait UDMA EP9 Tx is Empty */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x2290, &Value);

		/* if UDMA EP9 is Empty */
		if ((Value & BIT17) == BIT17) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP9 Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP9 Tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT17) != BIT17))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP9-TX Not Empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	2.Wait UDMA Tx state idle (0x9100, check 0x7f00000==0)
	---------------------------------------------------------------*/
	/* Wait UDMA Tx State Machine idle  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x9100, &Value);

		/* if UDMA Tx State Machine is idle*/
		if ((Value & 0x7F00000) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA Tx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA Idle\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0x7F00000) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Wait UDMA Idle failed!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	3.Check FCE Tx1 empty (0x0A30, check 0xFF==0)
	---------------------------------------------------------------*/
	/* Wait FCE Tx1 Empty */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTUSBReadMACRegister(ad, 0x0A30, &Value);

		/* if FCE Tx is Empty*/
		if ((Value & 0xFF) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting FCE Tx1 empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait FCE tx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0xFF) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: FCE Tx1(0x0A30) Not Empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	4.Check FCE Tx2 empty (0x0A34, check 0xFF00==0)
	--------------------------------------------------------------*/
	/* Wait FCE Tx2 Empty */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTUSBReadMACRegister(ad, 0x0A34, &Value);

		/* if FCE Tx 2 is Empty*/
		if ((Value & 0xFF00) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting FCE Tx2 empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait FCE tx 2 Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0xFF00) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: FCE Tx2(0x0A34) Not Empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	5.Disable MAC Tx (0x1004, set BIT2=0)
	---------------------------------------------------------------*/
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Disable MAC Tx\n",  __func__));
	/* Disable MAC Tx */
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &Value);
	Value &= (~BIT2);
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, Value);

	/* ------------------------------------------------------------
	6.Polling MAC Tx state to idle (0x1200, check BIT0==0)
	---------------------------------------------------------------*/
	/* Polling MAC Tx status */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTMP_IO_READ32(ad, MAC_STATUS_CFG, &Value);

		/* if MAC is idle*/
		if ((Value & BIT0) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting MAC Tx idle\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait mac tx idle\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT0) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Wait MAC Tx Idle Failed!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	7.Disable MAC Rx (0x1004, set BIT3=0)
	---------------------------------------------------------------*/
	DBGPRINT(RT_DEBUG_ERROR, ("%s: Disable MAC Rx\n",  __func__));
	/* Disable MAC Rx */
	RTMP_IO_READ32(ad, MAC_SYS_CTRL, &Value);
	Value &= (~BIT3);
	RTMP_IO_WRITE32(ad, MAC_SYS_CTRL, Value);

	/* ------------------------------------------------------------
	8.Polling MAC Rx state to idle (0x1200, check BIT1==0)
	---------------------------------------------------------------*/
	/* polling MAC Rx status*/
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTMP_IO_READ32(ad, MAC_STATUS_CFG, &Value);

		/* if MAC is idle*/
		if ((Value & BIT1) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting MAC Rx idle\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait mac rx idle\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT1) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Wait MAC Rx idle failed!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	8.5.Check PBF Rx empty (0x0430, check 0xFF0000 ==0)
	---------------------------------------------------------------*/
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x41, 0x0430, &Value);

		/* if MAC is idle*/
		if ((Value & 0xFF0000) == 0x0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting PBF Rx empty\n", __func__, count));
			break;
		}
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0xFF0000) != 0x0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Fail to wait PBF Rx empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	9.Check FCE Rx1 empty (0x0A30, check 0xFF00 ==0)
	---------------------------------------------------------------*/
	/* Wait FCE Rx Empty */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTUSBReadMACRegister(ad, 0x0A30, &Value);

		/* if FCE Rx is Empty*/
		if ((Value & 0xFF00) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting FCE Rx1 queue empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait FCE rx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0xFF00) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: FCE Rx1 Not Empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	10.Check FCE Rx2 empty (0x0A34, check 0xFF == 0)
	---------------------------------------------------------------*/
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		RTUSBReadMACRegister(ad, 0x0A34, &Value);

		/* if FCE Rx 2 is Empty*/
		if ((Value & 0xFF) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP2 Rx queue empty\n", __func__,
			count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait FCE rx 2 Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0xFF) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: FCE Rx2 Not Empty!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	11.Wait UDMA Rx state idle (0x9110, check 0x3F00==0)
	---------------------------------------------------------------*/
	/* Wait UDMA Rx State Machine idle  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms */
		read_reg(ad, 0x40, 0x9110, &Value);

		/* if UDMA Rx State Machine is idle*/
		if ((Value & 0x3F00) == 0) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP3 Rx empty\n", __func__, count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA Rx idle\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & 0x3F00) != 0))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: Wait UDMA Rx Idle failed!!!\n",  __func__));
	}

	/* ------------------------------------------------------------
	12.Check UDMA IN (Rx) EP4~EP5 empty (0x2140 & 0x2150, check BIT24==1)
	---------------------------------------------------------------*/
	/* Wait UDMA EP4 IN Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms*/
		read_reg(ad, 0x40, 0x2140, &Value);

		/* if UDMA Rx State Machine is idle*/
		if ((Value & BIT24) == BIT24) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP4 Rx queue empty\n", __func__,
			count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP4 Rx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT24) != BIT24))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP4-IN Not Empty!!!\n",  __func__));
	}

	/* Wait UDMA EP5 IN Empty  */
	count = 0;
	while (count < WAIT_EMPTY_RETRY_COUNT) {
		RtmpusecDelay(WAIT_EMPTY_DELAY_TIME_US); /* 1 ms*/
		read_reg(ad, 0x40, 0x2150, &Value);

		/* if UDMA Rx State Machine is idle*/
		if ((Value & BIT24) == BIT24) {
			DBGPRINT(RT_DEBUG_TRACE,
			("%s: %dms for waiting UDMA EP5 Rx queue empty\n", __func__,
			count));
			break;
		}
		/* DBGPRINT(RT_DEBUG_ERROR, ("wait UDMA EP5 Rx Empty\n")); */
		count++;
		if (count == WAIT_EMPTY_RETRY_COUNT && ((Value & BIT24) != BIT24))
			DBGPRINT(RT_DEBUG_ERROR, ("%s: UDMA EP5-IN Not Empty!!!\n",  __func__));
		}

}

void andes_resume_CR_setting(struct _RTMP_ADAPTER *ad, u8 type)
{
	/* Reset Driver Tx/Rx Info */
	reset_TxRx_Info(ad);

	/* Start Bulk-In Routine for command response */
	usb_rx_cmd_msgs_receive(ad);

	/*
		Clean BulkIn Reset flag.
		This makes Bulk-out data able to be send from Host to Device.
		Check out RTUSBBulkOutDataPacket function.
	*/
	RTMP_CLEAR_FLAG(ad, fRTMP_ADAPTER_IDLE_RADIO_OFF);

	if (type == RADIO_OFF_TYPE) {
#ifdef CONFIG_STA_SUPPORT
		IF_DEV_CONFIG_OPMODE_ON_STA(ad) {
			RTUSBBulkReceive(ad);
		}
#endif /* CONFIG_STA_SUPPORT */

		RTMP_SET_FLAG(ad, fRTMP_ADAPTER_MCU_SEND_IN_BAND_CMD);
	}

	/* For MT76x2 USB IP Toggle Error Issue */
#ifdef NEW_WOW_SUPPORT
	if (!IS_MT76x2T(ad))
		andes_send_dummy_bulkout(ad);
#endif /* NEW_WOW_SUPPORT */

}
#endif
