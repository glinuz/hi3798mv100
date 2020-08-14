/***************************************************************************************
                  Silicon Laboratories Broadcast Si2157 Layer 2 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L2 API for commands and properties
   FILE: Si2157_L2_API.c
   Supported IC : Si2157
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
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
#include <string.h>

/* Si2157 API Defines */
 /* define this if using the ALIF video filter */
#undef USING_ALIF_FILTER
/* define this if using the DTV video filter */
#undef USING_DLIF_FILTER
/************************************************************************************************************************/
/* Si2157 API Specific Includes */
#include "Si2157_L2_API.h"               /* Include file for this code */
#include "Si2157_firmware_3_4_build_4.h"       /* firmware file */
#define Si2157_BYTES_PER_LINE 8
/* _filtering_include_insertion_start */
#ifdef    USING_ALIF_FILTER
#include "write_ALIF_video_coeffs.h"   /* .h file from custom Video filter Tool output */
#endif /* USING_ALIF_FILTER */
#ifdef USING_DLIF_FILTER
#include "write_DLIF_video_coeffs.h"   /* .h file from custom Video filter Tool output */
#endif
/************************************************************************************************************************
  NAME: Si2157_Configure
  DESCRIPTION: Setup Si2157 video filters, GPIOs/clocks, Common Properties startup, etc.
  Parameter:  Pointer to Si2157 Context
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_Configure           (L1_Si2157_Context *api)
{
    int return_code;
    return_code = NO_Si2157_ERROR;

 #ifdef    USING_ALIF_FILTER
    if ((return_code = Si2157_LoadVideofilter(api,ALIF_VIDFILT_TABLE,ALIF_VIDFILT_LINES)) != NO_Si2157_ERROR)
       return return_code;
  #endif /* USING_ALIF_FILTER */
 /* load DTV video filter file */
  #ifdef USING_DLIF_FILTER
     if ((return_code = Si2157_LoadVideofilter(api,DLIF_VIDFILT_TABLE,DLIF_VIDFILT_LINES)) != NO_Si2157_ERROR)
       return return_code;
  #endif
    /* _specific_configure_insertion_point */

    /* Set Properties startup configuration         */
    Si2157_storePropertiesDefaults (api->propShadow);
    /* Edit the procedure below if you have any properties settings different from the standard defaults */
    Si2157_storeUserProperties     (api->prop);
    /* Download properties different from 'default' */
    Si2157_downloadAllProperties(api);
    return return_code;
}
/************************************************************************************************************************
  NAME: Si2157_PowerUpWithPatch
  DESCRIPTION: Send Si2157 API PowerUp Command with PowerUp to bootloader,
  Check the Chip rev and part, and ROMID are compared to expected values.
  Load the Firmware Patch then Start the Firmware.
  Programming Guide Reference:    Flowchart A.2 (POWER_UP with patch flowchart)

  Parameter:  pointer to Si2157 Context
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_PowerUpWithPatch    (L1_Si2157_Context *api)
{
    int return_code;
    return_code = NO_Si2157_ERROR;

    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2157_pollForCTS  (api)) != NO_Si2157_ERROR) {
        SiTRACE ("Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2157_L1_POWER_UP (api,
                            Si2157_POWER_UP_CMD_SUBCODE_CODE,
                            api->cmd->power_up.clock_mode,
                            api->cmd->power_up.en_xout,
                            Si2157_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2157_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2157_POWER_UP_CMD_RESET_RESET,
                            Si2157_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2157_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2157_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2157_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2157_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2157_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2157_ERROR)
    {
        SiTRACE ("Si2157_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2157_L1_PART_INFO(api)) != NO_Si2157_ERROR) {
        SiTRACE ("Si2157_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    SiTRACE("chiprev %d\n",        api->rsp->part_info.chiprev);
    SiTRACE("part    Si21%d\n",    api->rsp->part_info.part   );
    SiTRACE("pmajor  %d\n",        api->rsp->part_info.pmajor );
    if (api->rsp->part_info.pmajor >= 0x30) {
    SiTRACE("pmajor '%c'\n",       api->rsp->part_info.pmajor );
    }
    SiTRACE("pminor  %d\n",        api->rsp->part_info.pminor );
    if (api->rsp->part_info.pminor >= 0x30) {
    SiTRACE("pminor '%c'\n",       api->rsp->part_info.pminor );
    }
    SiTRACE("pbuild %d\n",         api->rsp->part_info.pbuild );
    SiTRACE("romid %3d/0x%02x\n",  api->rsp->part_info.romid,  api->rsp->part_info.romid );
    /* Load the Firmware */

    if (api->rsp->part_info.romid == 0x50) {

       if ((return_code = Si2157_LoadFirmware_16(api, Si2157_FW_3_4b4, FIRMWARE_LINES_3_4b4)) != NO_Si2157_ERROR) {
         SiTRACE ("Si2157_LoadFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
         return return_code;
       }
    }
    else
    {
         SiTRACE ("Invalid ROMID error 0x%02x: ROMID=%02x\n", ERROR_Si2157_INCOMPATIBLE_PART,api->rsp->part_info.romid );
         return ERROR_Si2157_INCOMPATIBLE_PART;
    }

    /*Start the Firmware */
    if ((return_code = Si2157_StartFirmware(api)) != NO_Si2157_ERROR) { /* Start firmware */
        SiTRACE ("Si2157_StartFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
  NAME: Si2157_PowerUpUsingBroadcastI2C
  DESCRIPTION: This is similar to PowerUpWithPatch() for 2 tuners but it uses the I2C Broadcast
  command to allow the firmware download simultaneously to both tuners.

  Parameter:  pointer to Tuner1 and Tuner2 L1 Context
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_PowerUpUsingBroadcastI2C    (L1_Si2157_Context *Tuner1, L1_Si2157_Context *Tuner2 )
{
    int return_code;
    unsigned char tuner1_i2c_address;
    return_code = NO_Si2157_ERROR;

    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2157_pollForCTS  (Tuner1)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner1 Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2157_L1_POWER_UP (Tuner1,
                            Si2157_POWER_UP_CMD_SUBCODE_CODE,
                            Tuner1->cmd->power_up.clock_mode,
                            Tuner1->cmd->power_up.en_xout,
                            Si2157_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2157_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2157_POWER_UP_CMD_RESET_RESET,
                            Si2157_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2157_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2157_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2157_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2157_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2157_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2157_ERROR)
    {
        SiTRACE ("Tuner1 Si2157_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2157_L1_PART_INFO(Tuner1)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner1 Si2157_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    SiTRACE("chiprev %d\n",        Tuner1->rsp->part_info.chiprev);
    SiTRACE("part    Si21%d\n",    Tuner1->rsp->part_info.part   );
    SiTRACE("pmajor  %d\n",        Tuner1->rsp->part_info.pmajor );
    if (Tuner1->rsp->part_info.pmajor >= 0x30) {
    SiTRACE("pmajor '%c'\n",       Tuner1->rsp->part_info.pmajor );
    }
    SiTRACE("pminor  %d\n",        Tuner1->rsp->part_info.pminor );
    if (Tuner1->rsp->part_info.pminor >= 0x30) {
    SiTRACE("pminor '%c'\n",       Tuner1->rsp->part_info.pminor );
    }
    SiTRACE("romid %3d/0x%02x\n",  Tuner1->rsp->part_info.romid,  Tuner1->rsp->part_info.romid );
// issue the Broadcast command for tuner 1
   if ((return_code = Si2157_L1_CONFIG_I2C(Tuner1,Si2157_CONFIG_I2C_CMD_SUBCODE_CODE,Si2157_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED) != NO_Si2157_ERROR))
   {
        SiTRACE ("Tuner1 Si2157_L1_CONFIG_I2C error 0x%02x\n", return_code);
        return return_code;
   }
/*****************************************************/
/* power up Tuner2 */
/*****************************************************/
    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2157_pollForCTS  (Tuner2)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner2 Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2157_L1_POWER_UP (Tuner2,
                            Si2157_POWER_UP_CMD_SUBCODE_CODE,
                            Tuner2->cmd->power_up.clock_mode,
                            Tuner2->cmd->power_up.en_xout,
                            Si2157_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2157_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2157_POWER_UP_CMD_RESET_RESET,
                            Si2157_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2157_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2157_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2157_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2157_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2157_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2157_ERROR)
    {
        SiTRACE ("Tuner2 Si2157_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2157_L1_PART_INFO(Tuner2)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner2 Si2157_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    SiTRACE("chiprev %d\n",        Tuner2->rsp->part_info.chiprev);
    SiTRACE("part    Si21%d\n",    Tuner2->rsp->part_info.part   );
    SiTRACE("pmajor  %d\n",        Tuner2->rsp->part_info.pmajor );
    if (Tuner2->rsp->part_info.pmajor >= 0x30) {
    SiTRACE("pmajor '%c'\n",       Tuner2->rsp->part_info.pmajor );
    }
    SiTRACE("pminor  %d\n",        Tuner2->rsp->part_info.pminor );
    if (Tuner2->rsp->part_info.pminor >= 0x30) {
    SiTRACE("pminor '%c'\n",       Tuner2->rsp->part_info.pminor );
    }
    SiTRACE("romid %3d/0x%02x\n",  Tuner2->rsp->part_info.romid,  Tuner1->rsp->part_info.romid );
/* issue the Broadcast command for tuner 2 */
   if ((return_code = Si2157_L1_CONFIG_I2C(Tuner2,Si2157_CONFIG_I2C_CMD_SUBCODE_CODE,Si2157_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED) != NO_Si2157_ERROR))
   {
        SiTRACE ("Tuner2 Si2157_L1_CONFIG_I2C error 0x%02x\n", return_code);
        return return_code;
   }
/* Set the i2c address of Tuner1 to 0xc4 for broadcasting (and then set it back after download fw) */
    tuner1_i2c_address = Tuner1->i2c->address;
    Tuner1->i2c->address = TUNER_BROADCAST_ADDRESS;
      /* Load the Firmware to both devices.  Both Tuners must provide a CTS after each line to be valid*/
    if ((Tuner1->rsp->part_info.romid == 0x50) && Tuner2->rsp->part_info.romid == 0x50)
    {
      /* Load the Firmware to both tuners using the broadcast address 0xc4 */
      if ((return_code = Si2157_LoadFirmware_16(Tuner1, Si2157_FW_3_4b4, FIRMWARE_LINES_3_4b4)) != NO_Si2157_ERROR) {
         SiTRACE ("Si2157_LoadFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
         return return_code;
       }
    }
    else
    {
        SiTRACE ("INCOMPATIBLE PART error Tuner1 ROMID 0x%02x Tuner2 ROMID 0x%02x\n", Tuner1->rsp->part_info.romid,Tuner2->rsp->part_info.romid);
        return ERROR_Si2157_INCOMPATIBLE_PART;
    }

    /*Start the Firmware for both tuners */
    if ((return_code = Si2157_StartFirmware(Tuner1)) != NO_Si2157_ERROR) { /* Start firmware */
        SiTRACE ("Tuner1 Si2157_StartFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    /* set the tuner1 address back to the normal (non-broadcast) address. */
    Tuner1->i2c->address = tuner1_i2c_address;

    /* Check the CTS from each tuner individually. */
       if ((return_code = Si2157_pollForCTS  (Tuner1)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner1 Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }
       if ((return_code = Si2157_pollForCTS  (Tuner2)) != NO_Si2157_ERROR) {
        SiTRACE ("Tuner2 Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    return NO_Si2157_ERROR;
}

 /************************************************************************************************************************
  NAME: Si2157_LoadFirmware_16
  DESCRIPTION: Load firmware from firmware_struct array in Si2157_Firmware_x_y_build_z.h file into Si2157
              Requires Si2157 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to firmware_struct array
  Parameter:  number of lines in firmware table array (size in bytes / firmware_struct)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadFirmware_16        (L1_Si2157_Context *api, firmware_struct fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2157_ERROR;

    SiTRACE ("Si2157_LoadFirmware_16 starting...\n");
    SiTRACE ("Si2157_LoadFirmware_16 nbLines %d\n", nbLines);

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
    if (fw_table[line].firmware_len > 0)  /* don't download if length is 0 , e.g. dummy firmware */
    {
      /* send firmware_len bytes (up to 16) to Si2157 */
      if ((return_code = Si2157_L1_API_Patch(api, fw_table[line].firmware_len, fw_table[line].firmware_table)) != NO_Si2157_ERROR)
      {
        SiTRACE("Si2157_LoadFirmware_16 error 0x%02x patching line %d: %s\n", return_code, line, Si2157_L1_API_ERROR_TEXT(return_code) );
        if (line == 0) {
        SiTRACE("The firmware is incompatible with the part!\n");
        }
        return ERROR_Si2157_LOADING_FIRMWARE;
      }
      if (line==3) SiTraceConfiguration("traces suspend");
    }
    }
    SiTraceConfiguration("traces resume");
    SiTRACE ("Si2157_LoadFirmware_16 complete...\n");
    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
*******  Deprecated LoadFirmware Function *******
*******  Please use LoadFirmware_16 replacement *****
  NAME: Si2157_LoadFirmware
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2157_Firmware_x_y_build_z.h file into Si2157
              Requires Si2157 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to firmware table array
  Parameter:  number of lines in firmware table array (size in bytes / BYTES_PER_LINE)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadFirmware        (L1_Si2157_Context *api, unsigned char fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2157_ERROR;

    SiTRACE ("Si2157_LoadFirmware starting...\n");
    SiTRACE ("Si2157_LoadFirmware nbLines %d\n", nbLines);

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
        /* send Si2157_BYTES_PER_LINE fw bytes to Si2157 */
        if ((return_code = Si2157_L1_API_Patch(api, Si2157_BYTES_PER_LINE, fw_table + Si2157_BYTES_PER_LINE*line)) != NO_Si2157_ERROR)
        {
          SiTRACE("Si2157_LoadFirmware error 0x%02x patching line %d: %s\n", return_code, line, Si2157_L1_API_ERROR_TEXT(return_code) );
          if (line == 0) {
          SiTRACE("The firmware is incompatible with the part!\n");
          }
          return ERROR_Si2157_LOADING_FIRMWARE;
        }
        if (line==3) {SiTraceConfiguration("traces suspend");}
    }
    SiTraceConfiguration("traces resume");
    SiTRACE ("Si2157_LoadFirmware complete...\n");
    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
  NAME: Si2157_StartFirmware
  DESCRIPTION: Start Si2157 firmware (put the Si2157 into run mode)
  Parameter:   Si2157 Context (I2C address)
  Parameter (passed by Reference):   ExitBootloadeer Response Status byte : tunint, atvint, dtvint, err, cts
  Returns:     I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_StartFirmware       (L1_Si2157_Context *api)
{

    if (Si2157_L1_EXIT_BOOTLOADER(api, Si2157_EXIT_BOOTLOADER_CMD_FUNC_TUNER, Si2157_EXIT_BOOTLOADER_CMD_CTSIEN_OFF) != NO_Si2157_ERROR)
    {
        return ERROR_Si2157_STARTING_FIRMWARE;
    }

    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
  NAME: Si2157_Init
  DESCRIPTION:Reset and Initialize Si2157
  Parameter:  Si2157 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_Init                (L1_Si2157_Context *api)
{
    int return_code;
    SiTRACE("Si2157_Init starting...\n");
    api->cmd->power_up.clock_mode = Si2157_POWER_UP_CMD_CLOCK_MODE_XTAL;
    api->cmd->power_up.en_xout = Si2157_POWER_UP_CMD_EN_XOUT_DIS_XOUT;
    if ((return_code = Si2157_PowerUpWithPatch(api)) != NO_Si2157_ERROR) {   /* PowerUp into bootloader */
        SiTRACE ("Si2157_PowerUpWithPatch error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    /* At this point, FW is loaded and started.  */
    Si2157_Configure(api);
    SiTRACE("Si2157_Init complete...\n");
    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_UpdateChannelScanFrequency
  DESCRIPTION:  This routine allows the user to implement an update of the current channel scan operation with
                the current frequency being scanned.
  Parameter:    Current frequency of the scan routine
  Parameter:  channelsFound = 0 if channel not found at that frequency , channelsFound > 0  the number of channels
        currently found( including this frequency )
  Returns:      1 if scan abort requested, 0 if ok.
************************************************************************************************************************/
int Si2157_UpdateChannelScanFrequency(int freq,int channelsFound)
{
  int abort_scan = 0;
  static int previousFrequency;
/*** Insert user code to display realtime updates of the frequency being scanned and channel status (number found ) **/
/* add check for user abort here */
  if (channelsFound)
  {
    /* Terminate the previous scan message */
    HI_INFO_TUNER("Found Frequency %d\n",freq);
  }
  else
  {
    if (freq==previousFrequency)
      HI_INFO_TUNER("Not Found\n");
    else
      HI_INFO_TUNER("Scanning Frequency %d, ",freq);
  }
  previousFrequency=freq;

  if (abort_scan)
    return 1;
  else
    return 0;
  }
 /************************************************************************************************************************
  NAME: Si2157_GetRF
  DESCRIPTIION: Retrieve Si2157 tune freq

  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns  :  frequency (Hz) as an int
************************************************************************************************************************/
int  Si2157_GetRF             (L1_Si2157_Context *api)
{
    Si2157_L1_TUNER_STATUS (api, Si2157_TUNER_STATUS_CMD_INTACK_OK);
        return api->rsp->tuner_status.freq;
}
 /************************************************************************************************************************
  NAME: Si2157_Tune
  DESCRIPTIION: Tune Si2157 in specified mode (ATV/DTV) at center frequency, wait for TUNINT and xTVINT with timeout

  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  Mode (ATV or DTV) use Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV or Si2157_TUNER_TUNE_FREQ_CMD_MODE_DTV constants
  Parameter:  frequency (Hz) as a unsigned long integer
  Returns:    0 if channel found.  A nonzero value means either an error occurred or channel not locked.
  Programming Guide Reference:    Flowchart A.7 (Tune flowchart)
************************************************************************************************************************/
 int  Si2157_Tune              (L1_Si2157_Context *api, unsigned char mode, unsigned long freq)
{
    int start_time  = 0;
    int return_code = 0;
    int timeout     = 36;

    if (Si2157_L1_TUNER_TUNE_FREQ (api,
                                   mode,
                                   freq) != NO_Si2157_ERROR)
    {
        return ERROR_Si2157_SENDING_COMMAND;
    }

    start_time = system_time();

    /* wait for TUNINT, timeout is 36 ms */
    while ( (system_time() - start_time) < timeout )
    {
        if ((return_code = Si2157_L1_CheckStatus(api)) != NO_Si2157_ERROR)
            return return_code;
        if (api->status->tunint)
            break;
    }
    if (!api->status->tunint) {
      SiTRACE("Timeout waiting for TUNINT\n");
      return ERROR_Si2157_TUNINT_TIMEOUT;
    }

    /* wait for xTVINT, timeout is 110ms for ATVINT and 10 ms for DTVINT */
    start_time = system_time();
    timeout    = ((mode==Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV) ? 110 : 10);
    while ( (system_time() - start_time) < timeout )
    {
        if ((return_code = Si2157_L1_CheckStatus(api)) != NO_Si2157_ERROR)
            return return_code;
        if (mode==Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV)
        {
         if (api->status->atvint)
            break;
        }
        else
        {
         if (api->status->dtvint)
            break;
        }
    }

    if (mode==Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV)
    {
      if (api->status->atvint)
      {
        SiTRACE("ATV Tune Successful\n");
        return NO_Si2157_ERROR;
      }
      else
        SiTRACE("Timeout waiting for ATVINT\n");
    }
    else
    {
        if (api->status->dtvint)
        {
          SiTRACE("DTV Tune Successful\n");
          return NO_Si2157_ERROR;
        }
        else
          SiTRACE("Timeout waiting for DTVINT\n");
    }

    return ERROR_Si2157_xTVINT_TIMEOUT;
}
 /************************************************************************************************************************
  NAME: Si2157_ATVTune
  DESCRIPTION:Update ATV_VIDEO_MODE and tune ATV mode at center frequency
  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  frequency (Hz)
  Parameter:  Video system , e.g. use constant Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M for system M
  Parameter:  transport,  e.g. use constant Si2157_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL for terrestrial
  Parameter:  color , e.g. use constant Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC for PAL or NTSC
  Parameter:  invert_signal, 0= normal, 1= inverted
  Parameter:  rsp - commandResp structure to returns tune status info.
  Returns:    I2C transaction error code, 0 if successful
  Programming Guide Reference:    Flowchart A.7 (Tune flowchart)
************************************************************************************************************************/
int  Si2157_ATVTune           (L1_Si2157_Context *api, unsigned long freq, unsigned char video_sys, unsigned char color, unsigned char invert_spectrum)
{
    /* update ATV_VIDEO_MODE property */
    api->prop->atv_video_mode.video_sys = video_sys;
    api->prop->atv_video_mode.color = color;
    api->prop->atv_video_mode.invert_spectrum=invert_spectrum;
    if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != NO_Si2157_ERROR)
    {
      return ERROR_Si2157_SENDING_COMMAND;
    }

    return Si2157_Tune (api, Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV, freq);
}
 /************************************************************************************************************************
  NAME: Si2157_DTVTune
  DESCRIPTION: Update DTV_MODE and tune DTV mode at center frequency
  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  frequency (Hz)
  Parameter:  bandwidth , 6,7 or 8 MHz
  Parameter:  modulation,  e.g. use constant Si2157_DTV_MODE_PROP_MODULATION_DVBT for DVBT mode
  Parameter:  rsp - commandResp structure to returns tune status info.
  Returns:    I2C transaction error code, 0 if successful
  Programming Guide Reference:    Flowchart A.7 (Tune flowchart)
************************************************************************************************************************/
int  Si2157_DTVTune           (L1_Si2157_Context *api, unsigned long freq, unsigned char bw, unsigned char modulation, unsigned char invert_signal)
{
    int return_code;
    return_code = NO_Si2157_ERROR;

    /* update DTV_MODE_PROP property */
    api->prop->dtv_mode.bw = bw;
    api->prop->dtv_mode.invert_spectrum = invert_signal;
    api->prop->dtv_mode.modulation = modulation;
    if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != NO_Si2157_ERROR)
    {
      return ERROR_Si2157_SENDING_COMMAND;
    }

    return_code = Si2157_Tune (api, Si2157_TUNER_TUNE_FREQ_CMD_MODE_DTV, freq);

    return return_code;
}
 /************************************************************************************************************************
  NAME: Si2157_XoutOn
  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_XoutOn             (L1_Si2157_Context *api)
{
    int return_code;
    SiTRACE("Si2157_XoutOn:  Turning Xout ON\n");

    if ((return_code = Si2157_L1_CONFIG_CLOCKS(api,
                                              Si2157_CONFIG_CLOCKS_CMD_SUBCODE_CODE,
                                              api->cmd->config_clocks.clock_mode,
                                              Si2157_CONFIG_CLOCKS_CMD_EN_XOUT_EN_XOUT)) != NO_Si2157_ERROR)
    return return_code;

    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_XoutOff
  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_XoutOff            (L1_Si2157_Context *api)
{
    int return_code;
    SiTRACE("Si2157_XoutOff:  Turning Xout OFF\n");

    if ((return_code = Si2157_L1_CONFIG_CLOCKS(api,
                                              Si2157_CONFIG_CLOCKS_CMD_SUBCODE_CODE,
                                              api->cmd->config_clocks.clock_mode,
                                              Si2157_CONFIG_CLOCKS_CMD_EN_XOUT_DIS_XOUT)) != NO_Si2157_ERROR)
    return return_code;

    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_Standby
  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_Standby         (L1_Si2157_Context *api)
{
    int return_code;
    SiTRACE("Si2157_Standby: Going to Standby. The part will wake on the next command\n");

    if ((return_code = Si2157_L1_STANDBY(api, Si2157_STANDBY_CMD_TYPE_LNA_ON)) != NO_Si2157_ERROR)
    return return_code;

    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_Powerdown
  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_Powerdown         (L1_Si2157_Context *api)
{
    int return_code;
    SiTRACE("Si2157_Powerdown: Powering down the part. Select InitAndConfig to reload.\n");

    if ((return_code = Si2157_L1_POWER_DOWN(api)) != NO_Si2157_ERROR)
    return return_code;

    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_LoadVideofilter
  DESCRIPTION:        Load video filters from vidfiltTable in Si2157_write_xTV_video_coeffs.h file into Si2157
  Programming Guide Reference:    Flowchart A.4 (Download Video Filters flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to video filter table array
  Parameter:  number of lines in video filter table array(size in bytes / atv_vid_filt_struct)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadVideofilter     (L1_Si2157_Context *api, vid_filt_struct vidFiltTable[], int lines)
{
    int line;
    /* for each line in VIDFILT_TABLE  (max 16 bytes) */
    for (line = 0; line < lines; line++)
    {
        /* send up to 16 byte I2C command to Si2157 */
        if (Si2157_L1_API_Patch(api, vidFiltTable[line].vid_filt_len, vidFiltTable[line].vid_filt_table) != NO_Si2157_ERROR)
        {
            return ERROR_Si2157_SENDING_COMMAND;
        }
    }
    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_ATV_Channel_Scan_M
  DESCRIPTION: Performs a channel scan for NTSC (System M) of the band
  Programming Guide Reference:    Flowchart A.11.0 and A11.1 (ATV Channel Scan flowchart for System M)

  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  starting Frequency Hz
  Parameter:  ending Frequency Hz
  Parameter:  min RSSI dBm
  Parameter:  max RSSI dBm
  Parameter:  min SNR 1/2 dB
  Parameter:  max SNR 1/2 dB
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
int Si2157_ATV_Channel_Scan_M (L1_Si2157_Context *api, unsigned long rangeMinHz, unsigned long rangeMaxHz, int minRSSIdBm, int maxRSSIdBm)
{
    #define SCAN_INTERVAL     1000000
    #define CHANNEL_BANDWIDTH 6000000
    #define CHANNEL_NOT_FOUND       0

    unsigned long freq;
    int i;

    /*Clear the channel list size and channel array */
    api->ChannelListSize=0;

    for (i=0; i< MAX_POSSIBLE_CHANNELS;i++)
        api->ChannelList[i]=0;
    /* configure the VideoMode property to System M, NTSC*/
    api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M;       /* M ATV demodulation       */
    api->prop->atv_video_mode.color     = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;    /* PAL_NTSC color system    */
    if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }
    /* configure the RSQ / RSSI threshold properties */
    api->prop->atv_rsq_rssi_threshold.lo = minRSSIdBm;
    api->prop->atv_rsq_rssi_threshold.hi = maxRSSIdBm;

    if (Si2157_L1_SetProperty2(api, Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }

  /* setup AFC acquistion range property to 1.5MHz for scanning */
    api->prop->atv_afc_range.range_khz    =  1500;
    if (Si2157_L1_SetProperty2(api, Si2157_ATV_AFC_RANGE_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }
   /* Start Scanning */
    for (freq=rangeMinHz; freq < rangeMaxHz; )
    {

    /* before calling tune provide a callback stub that the user can update the frequency */
    /* if user requested abort then break from the loop */
    if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
      break;

    /* Call the Tune command to tune the frequency */
    /* if successful (a station was found) then the return value will be 0. */
    if (!Si2157_Tune (api, Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV, freq))
    {
            /* Get ATV status */
      if (Si2157_L1_ATV_STATUS (api, Si2157_ATV_STATUS_CMD_INTACK_OK))
      {
        return ERROR_Si2157_SENDING_COMMAND;
      }

      /* Add the afc_freq (khz) to the center frequency and add to the channel list */
      api->ChannelList[api->ChannelListSize]= freq + (api->rsp->atv_status.afc_freq * 1000);
      /* Update the callback to indicate the channel is found */
      /* if user requested abort then break from the loop */
      if (Si2157_UpdateChannelScanFrequency(freq + (api->rsp->atv_status.afc_freq * 1000), api->ChannelListSize+1))
        break;

            freq = api->ChannelList[api->ChannelListSize++] + CHANNEL_BANDWIDTH;
         }
         else  /* We didn't find a station at this frequency so increment and move on */
         {
        /* if user requested abort then break from the loop */
      if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
        break;
      /*  channel not found! **/
             freq += SCAN_INTERVAL;
         }
    }

 return NO_Si2157_ERROR;
;
}
 /************************************************************************************************************************
  NAME: Si2157_ATV_Channel_Scan_PAL
  DESCRIPTION: Performs a channel scan for PAL (Systems B/G, D/K, L/L', and I) of the band
  Programming Guide Reference:    Flowchart A.10.0 - A10.3 (ATV Channel Scan flowchart for PAL)

  NOTE: this function requires an external sound processor to determine the PAL standard.  The user is
  required to implement the functions: L0_InitSoundProcessor(); and L0_AcquireSoundStandard(..);

  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  starting Frequency Hz
  Parameter:  ending Frequency Hz
  Parameter:  min RSSI dBm
  Parameter:  max RSSI dBm
  Parameter:  min SNR 1/2 dB
  Parameter:  max SNR 1/2 dB
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
int Si2157_ATV_Channel_Scan_PAL(L1_Si2157_Context *api, unsigned long rangeMinHz, unsigned long rangeMaxHz, int minRSSIdBm, int maxRSSIdBm)
{

  #define VHF_MAX 300000000
  #define SCAN_INTERVAL 1000000
  #define CHANNEL_NOT_FOUND 0
  char standard = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B;
  unsigned long freq;
  unsigned long channelIncrement;
  char posModulation;  /* flag for pos or neg modulation */
  int i;

  /*Clear the channel list size and channel array */
  api->ChannelListSize=0;

  for (i=0; i< MAX_POSSIBLE_CHANNELS;i++)
  {
    api->ChannelList[i]=0;
    api->ChannelType[i][0]='\0';
  }
    /* configure the RSQ / RSSI threshold properties */
    api->prop->atv_rsq_rssi_threshold.lo = minRSSIdBm;
    api->prop->atv_rsq_rssi_threshold.hi = maxRSSIdBm;

    if (Si2157_L1_SetProperty2(api, Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }

   /* setup AFC acquistion range property to 1.5MHz for scanning */
    api->prop->atv_afc_range.range_khz    =  1500;
    if (Si2157_L1_SetProperty2(api, Si2157_ATV_AFC_RANGE_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }
   /* Start Scanning */
    for (freq=rangeMinHz; freq < rangeMaxHz; )
    {
    /* before calling tune provide a callback stub that the user can update the frequency */
    /* if user requested abort then break from the loop */
    if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
      break;
    /* set the modulation flag to 0 (neg) */
    posModulation=0;
    /* Negative Modulation configure the VideoMode property to System DK, PAL*/
    api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK;        /* M ATV demodulation      */
    api->prop->atv_video_mode.color     = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;      /* PAL_NTSC color system    */
    if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != NO_Si2157_ERROR)
    {
       return ERROR_Si2157_SENDING_COMMAND;
    }

    /* Call the Tune command to tune the frequency */
    /* if successful (a station was found) then the return value will be 0. */
    if (!Si2157_Tune (api, Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV, freq))
    {
      posModulation = 0;
    }
    else  /* We didn't find a station so try positive modulation */
     {
      /* Pos Modulation configure the VideoMode property to System DK, PAL*/
      api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L;       /* L ATV demodulation       */
      api->prop->atv_video_mode.color     = Si2157_ATV_VIDEO_MODE_PROP_COLOR_SECAM;       /* SECAM color system       */
      if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != NO_Si2157_ERROR)
      {
         return ERROR_Si2157_SENDING_COMMAND;
      }
      /* Call the Tune command to tune the frequency */
      /* if successful (a station was found) then the return value will be 0. */
      if (!Si2157_Tune (api, Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV, freq))
      {
        posModulation=1;
      }
      else
      {
            /* if user requested abort then break from the loop */
          if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
            break;

        /* we didn't find any channels goto flowchart section B */
        freq += SCAN_INTERVAL;
        continue;
      }
    }
    /* Initialize the sound processor.   This may or may not be required for your application */
/*    L1_InitSoundProcessor();*/
    /* Run the sound processor and return the standard(s) */
    #define SOUND_PROCESSOR_TIMEOUT 10
    /*L1_AcquireSoundStandard(SOUND_PROCESSOR_TIMEOUT , &standard);*/
    if ((standard == Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B) || (standard == Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH))
      {
      /* check for positive modulation */
      if (posModulation)
    {
          /* if user requested abort then break from the loop */
          if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
            break;

      /* goto flowchart section B */
      freq += SCAN_INTERVAL;
      continue;
    }
      else
    {
        if (freq < VHF_MAX)
    {
          api->ChannelType[api->ChannelListSize][0]='B';
          api->ChannelType[api->ChannelListSize][1]='\0';
    }
    else
    {
          api->ChannelType[api->ChannelListSize][0]='G';
          api->ChannelType[api->ChannelListSize][1]='H';
          api->ChannelType[api->ChannelListSize][2]='\0';
    }
    }
    }
    else if (standard == Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I)
    {
        if (posModulation)
    {
                /* if user requested abort then break from the loop */
              if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
                break;

          /* goto flowchart section B */
          freq += SCAN_INTERVAL;
          continue;
    }
        else
    {
          api->ChannelType[api->ChannelListSize][0]='I';
          api->ChannelType[api->ChannelListSize][1]='\0';
    }
    }
    else if ((standard == Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L) || (standard == Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK))
    {
        if (posModulation)
    {
          api->ChannelType[api->ChannelListSize][0]='L';
          api->ChannelType[api->ChannelListSize][1]='\0';
    }
        else
    {
          api->ChannelType[api->ChannelListSize][0]='D';
          api->ChannelType[api->ChannelListSize][1]='K';
          api->ChannelType[api->ChannelListSize][2]='\0';
    }
    }
    else
    {
      /* standard not found */
    /* if user requested abort then break from the loop */
    if (Si2157_UpdateChannelScanFrequency(freq, CHANNEL_NOT_FOUND))
      break;

      /* goto flowchart section B */
      freq += SCAN_INTERVAL;
      continue;
    }
    /* if we got here, then we have a valid channel with a channelType */
            /* Get ATV status */
    if (Si2157_L1_ATV_STATUS (api, Si2157_ATV_STATUS_CMD_INTACK_OK))
            {
               return ERROR_Si2157_SENDING_COMMAND;
            }
    if (api->ChannelType[api->ChannelListSize][0]=='B')
    {
      /* Add the afc_freq (khz) to the center frequency and add to the channel list */
      api->ChannelList[api->ChannelListSize]= freq + (api->rsp->atv_status.afc_freq * 1000)-500000;
      channelIncrement = 7000000;
    }
    else
    {
    /* Add the afc_freq (khz) to the center frequency and add to the channel list */
      api->ChannelList[api->ChannelListSize]= freq + (api->rsp->atv_status.afc_freq * 1000);
      channelIncrement = 8000000;
    }
    /* Update the callback to indicate the channel is found */
    /* if user requested abort then break from the loop */
    if (Si2157_UpdateChannelScanFrequency(api->ChannelList[api->ChannelListSize], api->ChannelListSize+1))
      break;
    /* go to the next frequency in the loop */
    freq = api->ChannelList[api->ChannelListSize++] + channelIncrement;
    }

 return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_AGC_Override
  Parameter:  Pointer to Si2157 Context (I2C address)
  Parameter:  Mode 0=Normal, 1= Max, 2=TOP
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
int Si2157_AGC_Override    (L1_Si2157_Context *api, unsigned char mode )
{
    int return_code;
    SiTRACE("Si2157_AGC_Override: mode = %d\n",mode);
    switch (mode)
    {
        case  Si2157_FORCE_NORMAL_AGC:
            if ((return_code = Si2157_L1_AGC_OVERRIDE (api, Si2157_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE, Si2157_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE)) != NO_Si2157_ERROR)
                return return_code;
            break;
        case  Si2157_FORCE_MAX_AGC:
            if ((return_code = Si2157_L1_AGC_OVERRIDE (api, Si2157_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_ENABLE, Si2157_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE)) != NO_Si2157_ERROR)
                return return_code;
            break;
        case  Si2157_FORCE_TOP_AGC:
            if ((return_code = Si2157_L1_AGC_OVERRIDE (api, Si2157_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE, Si2157_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_ENABLE)) != NO_Si2157_ERROR)
                return return_code;
            break;
        default:
            return ERROR_Si2157_PARAMETER_OUT_OF_RANGE;
    }
    return NO_Si2157_ERROR;
  }







