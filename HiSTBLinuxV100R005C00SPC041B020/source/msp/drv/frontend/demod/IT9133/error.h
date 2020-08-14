/********************************************************************************************/
/**                                                                                         */
/**       These value in this file are automatically generated and used by API.             */
/**       Please not modify the content.                                                    */
/**                                                                                         */
/********************************************************************************************/
#ifndef __ERROR_H__
#define __ERROR_H__

#define Error_NO_ERROR							0x00000000ul
#define Error_RESET_TIMEOUT						0x00000001ul
#define Error_WRITE_REG_TIMEOUT					0x00000002ul
#define Error_WRITE_TUNER_TIMEOUT				0x00000003ul
#define Error_WRITE_TUNER_FAIL					0x00000004ul
#define Error_RSD_COUNTER_NOT_READY				0x00000005ul
#define Error_VTB_COUNTER_NOT_READY				0x00000006ul
#define Error_FEC_MON_NOT_ENABLED				0x00000007ul
#define Error_INVALID_DEV_TYPE					0x00000008ul
#define Error_INVALID_TUNER_TYPE				0x00000009ul
#define Error_OPEN_FILE_FAIL					0x0000000Aul
#define Error_WRITEFILE_FAIL					0x0000000Bul
#define Error_READFILE_FAIL						0x0000000Cul
#define Error_CREATEFILE_FAIL					0x0000000Dul
#define Error_MALLOC_FAIL						0x0000000Eul
#define Error_INVALID_FILE_SIZE					0x0000000Ful
#define Error_INVALID_READ_SIZE					0x00000010ul
#define Error_LOAD_FW_DONE_BUT_FAIL				0x00000011ul
#define Error_NOT_IMPLEMENTED					0x00000012ul
#define Error_NOT_SUPPORT						0x00000013ul
#define Error_WRITE_MBX_TUNER_TIMEOUT			0x00000014ul
#define Error_DIV_MORE_THAN_8_CHIPS				0x00000015ul
#define Error_DIV_NO_CHIPS						0x00000016ul
#define Error_SUPER_FRAME_CNT_0					0x00000017ul
#define Error_INVALID_FFT_MODE					0x00000018ul
#define Error_INVALID_CONSTELLATION_MODE		0x00000019ul
#define Error_RSD_PKT_CNT_0						0x0000001Aul
#define Error_FFT_SHIFT_TIMEOUT					0x0000001Bul
#define Error_WAIT_TPS_TIMEOUT					0x0000001Cul
#define Error_INVALID_BW						0x0000001Dul
#define Error_INVALID_BUF_LEN					0x0000001Eul
#define Error_NULL_PTR							0x0000001Ful
#define Error_INVALID_AGC_VOLT					0x00000020ul
#define Error_MT_OPEN_FAIL						0x00000021ul
#define Error_MT_TUNE_FAIL						0x00000022ul
#define Error_CMD_NOT_SUPPORTED					0x00000023ul
#define Error_CE_NOT_READY						0x00000024ul
#define Error_EMBX_INT_NOT_CLEARED				0x00000025ul
#define Error_INV_PULLUP_VOLT					0x00000026ul
#define Error_FREQ_OUT_OF_RANGE					0x00000027ul
#define Error_INDEX_OUT_OF_RANGE				0x00000028ul
#define Error_NULL_SETTUNER_PTR					0x00000029ul
#define Error_NULL_INITSCRIPT_PTR				0x0000002Aul
#define Error_INVALID_INITSCRIPT_LEN			0x0000002Bul
#define Error_INVALID_POS						0x0000002Cul
#define Error_BACK_TO_BOOTCODE_FAIL				0x0000002Dul
#define Error_GET_BUFFER_VALUE_FAIL				0x0000002Eul
#define Error_INVALID_REG_VALUE					0x0000002Ful
#define Error_INVALID_INDEX						0x00000030ul
#define Error_READ_TUNER_TIMEOUT				0x00000031ul
#define Error_READ_TUNER_FAIL					0x00000032ul
#define Error_UNDEFINED_SAW_BW					0x00000033ul
#define Error_MT_NOT_AVAILABLE					0x00000034ul
#define Error_NO_SUCH_TABLE						0x00000035ul
#define Error_WRONG_CHECKSUM					0x00000036ul
#define Error_INVALID_XTAL_FREQ					0x00000037ul
#define Error_COUNTER_NOT_AVAILABLE				0x00000038ul
#define Error_INVALID_DATA_LENGTH				0x00000039ul
#define Error_BOOT_FAIL							0x0000003Aul
#define Error_BUFFER_INSUFFICIENT				0x0000003Bul
#define Error_NOT_READY							0x0000003Cul
#define Error_DRIVER_INVALID					0x0000003Dul
#define Error_INTERFACE_FAIL					0x0000003Eul
#define Error_PID_FILTER_FULL					0x0000003Ful
#define Error_OPERATION_TIMEOUT					0x00000040ul
#define Error_LOADFIRMWARE_SKIPPED				0x00000041ul
#define Error_REBOOT_FAIL						0x00000042ul
#define Error_PROTOCOL_FORMAT_INVALID			0x00000043ul
#define Error_ACTIVESYNC_ERROR					0x00000044ul
#define Error_CE_READWRITEBUS_ERROR				0x00000045ul
#define Error_CE_NODATA_ERROR					0x00000046ul
#define Error_NULL_FW_SCRIPT					0x00000047ul
#define Error_NULL_TUNER_SCRIPT					0x00000048ul

/** Error Code of Gemini System */
#define Error_INVALID_INDICATOR_TYPE			0x00000101ul
#define Error_INVALID_SC_NUMBER					0x00000102ul
#define Error_INVALID_SC_INFO					0x00000103ul
#define Error_FIGBYPASS_FAIL					0x00000104ul

/** Error Code of Firmware */
#define Error_FIRMWARE_STATUS					0x01000000ul

/** Error Code of I2C Module */
#define Error_I2C_DATA_HIGH_FAIL				0x02001000ul
#define Error_I2C_CLK_HIGH_FAIL					0x02002000ul
#define Error_I2C_WRITE_NO_ACK					0x02003000ul
#define Error_I2C_DATA_LOW_FAIL					0x02004000ul

/** Error Code of USB Module */
#define Error_USB_NULL_HANDLE					0x03010001ul
#define Error_USB_WRITEFILE_FAIL				0x03000002ul
#define Error_USB_READFILE_FAIL					0x03000003ul
#define Error_USB_INVALID_READ_SIZE				0x03000004ul
#define Error_USB_INVALID_STATUS				0x03000005ul
#define Error_USB_INVALID_SN					0x03000006ul
#define Error_USB_INVALID_PKT_SIZE				0x03000007ul
#define Error_USB_INVALID_HEADER				0x03000008ul
#define Error_USB_NO_IR_PKT						0x03000009ul
#define Error_USB_INVALID_IR_PKT				0x0300000Aul
#define Error_USB_INVALID_DATA_LEN				0x0300000Bul
#define Error_USB_EP4_READFILE_FAIL				0x0300000Cul
#define Error_USB_EP$_INVALID_READ_SIZE			0x0300000Dul
#define Error_USB_BOOT_INVALID_PKT_TYPE			0x0300000Eul
#define Error_USB_BOOT_BAD_CONFIG_HEADER		0x0300000Ful
#define Error_USB_BOOT_BAD_CONFIG_SIZE			0x03000010ul
#define Error_USB_BOOT_BAD_CONFIG_SN			0x03000011ul
#define Error_USB_BOOT_BAD_CONFIG_SUBTYPE		0x03000012ul
#define Error_USB_BOOT_BAD_CONFIG_VALUE			0x03000013ul
#define Error_USB_BOOT_BAD_CONFIG_CHKSUM		0x03000014ul
#define Error_USB_BOOT_BAD_CONFIRM_HEADER		0x03000015ul
#define Error_USB_BOOT_BAD_CONFIRM_SIZE			0x03000016ul
#define Error_USB_BOOT_BAD_CONFIRM_SN			0x03000017ul
#define Error_USB_BOOT_BAD_CONFIRM_SUBTYPE		0x03000018ul
#define Error_USB_BOOT_BAD_CONFIRM_VALUE		0x03000019ul
#define Error_USB_BOOT_BAD_CONFIRM_CHKSUM		0x03000020ul
#define Error_USB_BOOT_BAD_BOOT_HEADER			0x03000021ul
#define Error_USB_BOOT_BAD_BOOT_SIZE			0x03000022ul
#define Error_USB_BOOT_BAD_BOOT_SN				0x03000023ul
#define Error_USB_BOOT_BAD_BOOT_PATTERN_01		0x03000024ul
#define Error_USB_BOOT_BAD_BOOT_PATTERN_10		0x03000025ul
#define Error_USB_BOOT_BAD_BOOT_CHKSUM			0x03000026ul
#define Error_USB_INVALID_BOOT_PKT_TYPE			0x03000027ul
#define Error_USB_BOOT_BAD_CONFIG_VAlUE			0x03000028ul
#define Error_USB_COINITIALIZEEX_FAIL			0x03000029ul
#define Error_USB_COCREATEINSTANCE_FAIL			0x0300003Aul
#define Error_USB_COCREATCLSEENUMERATOR_FAIL    0x0300002Bul
#define Error_USB_QUERY_INTERFACE_FAIL			0x0300002Cul
#define Error_USB_PKSCTRL_NULL					0x0300002Dul
#define Error_USB_INVALID_REGMODE				0x0300002Eul
#define Error_USB_INVALID_REG_COUNT				0x0300002Ful
#define Error_USB_INVALID_HANDLE				0x03000100ul
#define Error_USB_WRITE_FAIL					0x03000200ul
#define Error_USB_UNEXPECTED_WRITE_LEN			0x03000300ul
#define Error_USB_READ_FAIL						0x03000400ul

/** Error code of 9035U2I bridge*/
#define Error_AF9035U2I                         0x04000000ul

/** Error code of Omega */						
#define Error_OMEGA_TUNER_INIT_FAIL				0x05000000ul

/** Error code of Castor*/
#define Error_CASTOR                            0x70000000ul
#define Error_CASTOR_BUS_NO_SUPPORT             0x00001000ul

#endif
