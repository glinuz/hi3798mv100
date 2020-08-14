/***************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 2 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L2 API for commands and properties
   FILE: Si2147_L2_API.c
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 11 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/
//#include <string.h>
/* Si2147 API Defines */
 /* define this if using the ALIF video filter */
#undef USING_ALIF_FILTER
/* define this if using the DTV video filter */
#undef USING_DLIF_FILTER
//#define USING_DLIF_FILTER
/************************************************************************************************************************/
/* Si2147 API Specific Includes */
#include "Si2147_L2_API.h"               /* Include file for this code */
#include "Si2147_firmware_3_4_build_4.h"       /* dummy firmware file for future patch replacement */
#define Si2147_BYTES_PER_LINE 8
#ifdef USING_DLIF_FILTER
#include "write_DLIF_video_coeffs.h"   /* .h file from custom Video filter Tool output */
#endif
/************************************************************************************************************************
  NAME: Si2147_Configure
  DESCRIPTION: Setup Si2147 video filters, GPIOs/clocks, Common Properties startup, etc.
  Parameter:  Pointer to Si2147 Context
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_Configure           (L1_Si2147_Context *api)
{
    int return_code;
    return_code = NO_Si2147_ERROR;
  /* load DTV video filter file */
  #ifdef USING_DLIF_FILTER
     if ((return_code = Si2147_LoadVideofilter(api,DLIF_VIDFILT_TABLE,DLIF_VIDFILT_LINES)) != NO_Si2147_ERROR)
       return return_code;
  #endif

    /* Set Properties startup configuration         */
    Si2147_storePropertiesDefaults (api->propShadow);
    /* Edit the procedure below if you have any properties settings different from the standard defaults */
    Si2147_storeUserProperties     (api->prop);
    /* Download properties different from 'default' */
    Si2147_downloadAllProperties(api);

    return return_code;
}
/************************************************************************************************************************
  NAME: Si2147_PowerUpWithPatch
  DESCRIPTION: Send Si2147 API PowerUp Command with PowerUp to bootloader,
  Check the Chip rev and part, and ROMID are compared to expected values.
  Load the Firmware Patch then Start the Firmware.
  Programming Guide Reference:    Flowchart A.2 (POWER_UP with patch flowchart)

  Parameter:  pointer to Si2147 Context
  Returns:    Si2147/I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_PowerUpWithPatch    (L1_Si2147_Context *api)
{
    int return_code;
    return_code = NO_Si2147_ERROR;
#if 0
    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2147_pollForCTS  (api)) != NO_Si2147_ERROR) {
        HI_PRINT ("Si2147_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }
#endif
    if ((return_code = Si2147_L1_POWER_UP (api,
                            Si2147_POWER_UP_CMD_SUBCODE_CODE,
                            api->cmd->power_up.clock_mode,
                            api->cmd->power_up.en_xout,
                            Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2147_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2147_POWER_UP_CMD_RESET_RESET,
                            Si2147_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2147_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2147_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2147_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2147_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2147_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2147_ERROR)
    {
        //HI_PRINT ("Si2147_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2147_L1_PART_INFO(api)) != NO_Si2147_ERROR) {
        //HI_PRINT ("Si2147_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    HI_PRINT("chiprev %d\n",        api->rsp->part_info.chiprev);
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
    HI_PRINT("romid %3d/0x%02x\n",  api->rsp->part_info.romid,  api->rsp->part_info.romid );
    /* Load the Firmware */

    if (api->rsp->part_info.romid == 0x50) {

       /*if ((return_code = Si2147_LoadFirmware_16(api, Si2147_FW_3_4b4, FIRMWARE_LINES_3_4b4)) != NO_Si2147_ERROR) {
         SiTRACE ("Si2147_LoadFirmware error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
         return return_code;
       }*/
       //SiTRACE ("No firmware to download. Loading from NVM only\n" );

    }
    else
    {
         SiTRACE ("Invalid ROMID error 0x%02x: ROMID=%02x\n", ERROR_Si2147_INCOMPATIBLE_PART,api->rsp->part_info.romid );
         return ERROR_Si2147_INCOMPATIBLE_PART;
    }

    /*Start the Firmware */
    if ((return_code = Si2147_StartFirmware(api)) != NO_Si2147_ERROR) { /* Start firmware */
        //HI_PRINT ("Si2147_StartFirmware error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    return NO_Si2147_ERROR;
}
/************************************************************************************************************************
  NAME: Si2147_PowerUpUsingBroadcastI2C
  DESCRIPTION: This is similar to PowerUpWithPatch() for 2 tuners but it uses the I2C Broadcast
  command to allow the firmware download simultaneously to both tuners.

  Parameter:  pointer to Tuner1 and Tuner2 L1 Context
  Returns:    Si2147/I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_PowerUpUsingBroadcastI2C    (L1_Si2147_Context *Tuner1, L1_Si2147_Context *Tuner2 )
{
    int return_code;
    unsigned char tuner1_i2c_address;
    return_code = NO_Si2147_ERROR;

    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2147_pollForCTS  (Tuner1)) != NO_Si2147_ERROR) {
        SiTRACE ("Tuner1 Si2147_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2147_L1_POWER_UP (Tuner1,
                            Si2147_POWER_UP_CMD_SUBCODE_CODE,
                            Tuner1->cmd->power_up.clock_mode,
                            Tuner1->cmd->power_up.en_xout,
                            Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2147_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2147_POWER_UP_CMD_RESET_RESET,
                            Si2147_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2147_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2147_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2147_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2147_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2147_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2147_ERROR)
    {
        //SiTRACE ("Tuner1 Si2147_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2147_L1_PART_INFO(Tuner1)) != NO_Si2147_ERROR) {
        //SiTRACE ("Tuner1 Si2147_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    //SiTRACE("chiprev %d\n",        Tuner1->rsp->part_info.chiprev);
    //SiTRACE("part    Si21%d\n",    Tuner1->rsp->part_info.part   );
    //SiTRACE("pmajor  %d\n",        Tuner1->rsp->part_info.pmajor );
    if (Tuner1->rsp->part_info.pmajor >= 0x30) {
    //SiTRACE("pmajor '%c'\n",       Tuner1->rsp->part_info.pmajor );
    }
    //SiTRACE("pminor  %d\n",        Tuner1->rsp->part_info.pminor );
    if (Tuner1->rsp->part_info.pminor >= 0x30) {
    //SiTRACE("pminor '%c'\n",       Tuner1->rsp->part_info.pminor );
    }
    //SiTRACE("romid %3d/0x%02x\n",  Tuner1->rsp->part_info.romid,  Tuner1->rsp->part_info.romid );
// issue the Broadcast command for tuner 1
   if ((return_code = Si2147_L1_CONFIG_I2C(Tuner1,Si2147_CONFIG_I2C_CMD_SUBCODE_CODE,Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED) != NO_Si2147_ERROR))
   {
        //SiTRACE ("Tuner1 Si2147_L1_CONFIG_I2C error 0x%02x\n", return_code);
        return return_code;
   }
/*****************************************************/
/* power up Tuner2 */
/*****************************************************/
    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2147_pollForCTS  (Tuner2)) != NO_Si2147_ERROR) {
        SiTRACE ("Tuner2 Si2147_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2147_L1_POWER_UP (Tuner2,
                            Si2147_POWER_UP_CMD_SUBCODE_CODE,
                            Tuner2->cmd->power_up.clock_mode,
                            Tuner2->cmd->power_up.en_xout,
                            Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2147_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2147_POWER_UP_CMD_RESET_RESET,
                            Si2147_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2147_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2147_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2147_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2147_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2147_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2147_ERROR)
    {
        //SiTRACE ("Tuner2 Si2147_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2147_L1_PART_INFO(Tuner2)) != NO_Si2147_ERROR) {
        //SiTRACE ("Tuner2 Si2147_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    //SiTRACE("chiprev %d\n",        Tuner2->rsp->part_info.chiprev);
    //SiTRACE("part    Si21%d\n",    Tuner2->rsp->part_info.part   );
    //SiTRACE("pmajor  %d\n",        Tuner2->rsp->part_info.pmajor );
    if (Tuner2->rsp->part_info.pmajor >= 0x30) {
    //SiTRACE("pmajor '%c'\n",       Tuner2->rsp->part_info.pmajor );
    }
    //SiTRACE("pminor  %d\n",        Tuner2->rsp->part_info.pminor );
    if (Tuner2->rsp->part_info.pminor >= 0x30) {
    //SiTRACE("pminor '%c'\n",       Tuner2->rsp->part_info.pminor );
    }
    //SiTRACE("romid %3d/0x%02x\n",  Tuner2->rsp->part_info.romid,  Tuner1->rsp->part_info.romid );
/* issue the Broadcast command for tuner 2 */
   if ((return_code = Si2147_L1_CONFIG_I2C(Tuner2,Si2147_CONFIG_I2C_CMD_SUBCODE_CODE,Si2147_CONFIG_I2C_CMD_I2C_BROADCAST_ENABLED) != NO_Si2147_ERROR))
   {
        //SiTRACE ("Tuner2 Si2147_L1_CONFIG_I2C error 0x%02x\n", return_code);
        return return_code;
   }
/* Set the i2c address of Tuner1 to 0xc4 for broadcasting (and then set it back after download fw) */
    tuner1_i2c_address = Tuner1->i2c->address;
    Tuner1->i2c->address = TUNER_BROADCAST_ADDRESS;
      /* Load the Firmware to both devices.  Both Tuners must provide a CTS after each line to be valid*/
    if ((Tuner1->rsp->part_info.romid == 0x50) && Tuner2->rsp->part_info.romid == 0x50)
    {
      /* Load the Firmware to both tuners using the broadcast address 0xc4 */
      if ((return_code = Si2147_LoadFirmware_16(Tuner1, Si2147_FW_3_4b4, FIRMWARE_LINES_3_4b4)) != NO_Si2147_ERROR) {
         //SiTRACE ("Si2147_LoadFirmware error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
         return return_code;
       }
    }
    else
    {
        //SiTRACE ("INCOMPATIBLE PART error Tuner1 ROMID 0x%02x Tuner2 ROMID 0x%02x\n", Tuner1->rsp->part_info.romid,Tuner2->rsp->part_info.romid);
        return ERROR_Si2147_INCOMPATIBLE_PART;
    }

    /*Start the Firmware for both tuners */
    if ((return_code = Si2147_StartFirmware(Tuner1)) != NO_Si2147_ERROR) { /* Start firmware */
        //SiTRACE ("Tuner1 Si2147_StartFirmware error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    /* set the tuner1 address back to the normal (non-broadcast) address. */
    Tuner1->i2c->address = tuner1_i2c_address;

    /* Check the CTS from each tuner individually. */
       if ((return_code = Si2147_pollForCTS  (Tuner1)) != NO_Si2147_ERROR) {
        //SiTRACE ("Tuner1 Si2147_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }
       if ((return_code = Si2147_pollForCTS  (Tuner2)) != NO_Si2147_ERROR) {
        //SiTRACE ("Tuner2 Si2147_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    return NO_Si2147_ERROR;
}

 /************************************************************************************************************************
  NAME: Si2147_LoadFirmware_16
  DESCRIPTION: Load firmware from firmware_struct array in Si2147_Firmware_x_y_build_z.h file into Si2147
              Requires Si2147 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2147 Context (I2C address)
  Parameter:  pointer to firmware_struct array
  Parameter:  number of lines in firmware table array (size in bytes / firmware_struct)
  Returns:    Si2147/I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_LoadFirmware_16        (L1_Si2147_Context *api, firmware_struct fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2147_ERROR;

    SiTRACE ("Si2147_LoadFirmware_16 starting...\n");
    SiTRACE ("Si2147_LoadFirmware_16 nbLines %d\n", nbLines);

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
    if (fw_table[line].firmware_len > 0)  /* don't download if length is 0 , e.g. dummy firmware */
    {
      /* send firmware_len bytes (up to 16) to Si2147 */
      if ((return_code = Si2147_L1_API_Patch(api, fw_table[line].firmware_len, fw_table[line].firmware_table)) != NO_Si2147_ERROR)
      {
        //SiTRACE("Si2147_LoadFirmware_16 error 0x%02x patching line %d: %s\n", return_code, line, Si2147_L1_API_ERROR_TEXT(return_code) );
        if (line == 0) {
        //SiTRACE("The firmware is incompatible with the part!\n");
        }
        return ERROR_Si2147_LOADING_FIRMWARE;
      }
     
    }
    }
    
    //SiTRACE ("Si2147_LoadFirmware_16 complete...\n");
    return NO_Si2147_ERROR;
}
/************************************************************************************************************************
*******  Deprecated LoadFirmware Function *******
*******  Please use LoadFirmware_16 replacement *****
  NAME: Si2147_LoadFirmware
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2147_Firmware_x_y_build_z.h file into Si2147
              Requires Si2147 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2147 Context (I2C address)
  Parameter:  pointer to firmware table array
  Parameter:  number of lines in firmware table array (size in bytes / BYTES_PER_LINE)
  Returns:    Si2147/I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_LoadFirmware        (L1_Si2147_Context *api, unsigned char fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2147_ERROR;

    //SiTRACE ("Si2147_LoadFirmware starting...\n");
    //SiTRACE ("Si2147_LoadFirmware nbLines %d\n", nbLines);

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
        /* send Si2147_BYTES_PER_LINE fw bytes to Si2147 */
        if ((return_code = Si2147_L1_API_Patch(api, Si2147_BYTES_PER_LINE, fw_table + Si2147_BYTES_PER_LINE*line)) != NO_Si2147_ERROR)
        {
          //SiTRACE("Si2147_LoadFirmware error 0x%02x patching line %d: %s\n", return_code, line, Si2147_L1_API_ERROR_TEXT(return_code) );
          if (line == 0) {
          //SiTRACE("The firmware is incompatible with the part!\n");
          }
          return ERROR_Si2147_LOADING_FIRMWARE;
        }
      }
    
    //SiTRACE ("Si2147_LoadFirmware complete...\n");
    return NO_Si2147_ERROR;
}
/************************************************************************************************************************
  NAME: Si2147_StartFirmware
  DESCRIPTION: Start Si2147 firmware (put the Si2147 into run mode)
  Parameter:   Si2147 Context (I2C address)
  Parameter (passed by Reference):   ExitBootloadeer Response Status byte : tunint, atvint, dtvint, err, cts
  Returns:     I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_StartFirmware       (L1_Si2147_Context *api)
{

    if (Si2147_L1_EXIT_BOOTLOADER(api, Si2147_EXIT_BOOTLOADER_CMD_FUNC_TUNER, Si2147_EXIT_BOOTLOADER_CMD_CTSIEN_OFF) != NO_Si2147_ERROR)
    {
        return ERROR_Si2147_STARTING_FIRMWARE;
    }

    return NO_Si2147_ERROR;
}
/************************************************************************************************************************
  NAME: Si2147_Init
  DESCRIPTION:Reset and Initialize Si2147
  Parameter:  Si2147 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_Init                (L1_Si2147_Context *api)
{
    int return_code;
    SiTRACE("Si2147_Init starting...\n");
    api->cmd->power_up.clock_mode = Si2147_POWER_UP_CMD_CLOCK_MODE_XTAL;
    api->cmd->power_up.en_xout = Si2147_POWER_UP_CMD_EN_XOUT_DIS_XOUT;

    #if 0
    if ((return_code = Si2147_PowerUpWithPatch(api)) != NO_Si2147_ERROR) {   /* PowerUp into bootloader */
        SiTRACE ("Si2147_PowerUpWithPatch error 0x%02x: %s\n", return_code, Si2147_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    #endif

	return_code = Si2147_L1_POWER_UP (api,
                            Si2147_POWER_UP_CMD_SUBCODE_CODE,
                            api->cmd->power_up.clock_mode,
                            api->cmd->power_up.en_xout,
                            Si2147_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2147_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2147_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2147_POWER_UP_CMD_RESET_RESET,
                            Si2147_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2147_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2147_POWER_UP_CMD_FUNC_NORMAL,
                            Si2147_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2147_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2147_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            );
    if(return_code  != NO_Si2147_ERROR)
    {
        SiTRACE ("Si2147_L1_POWER_UP error 0x%02x\n", return_code);
        return return_code;
    }
    /* At this point, FW is loaded and started.  */
    return_code = Si2147_Configure(api);
    if(return_code  != NO_Si2147_ERROR)
    {
        SiTRACE ("Si2147_Configure error 0x%02x\n", return_code);
        return return_code;
    }
    SiTRACE("Si2147_Init complete...\n");
    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_UpdateChannelScanFrequency
  DESCRIPTION:  This routine allows the user to implement an update of the current channel scan operation with
                the current frequency being scanned.
  Parameter:    Current frequency of the scan routine
  Parameter:  channelsFound = 0 if channel not found at that frequency , channelsFound > 0  the number of channels
        currently found( including this frequency )
  Returns:      1 if scan abort requested, 0 if ok.
************************************************************************************************************************/
int Si2147_UpdateChannelScanFrequency(int freq,int channelsFound)
{
  /*int abort_scan = 0;*/
  static int previousFrequency;
/*** Insert user code to display realtime updates of the frequency being scanned and channel status (number found ) **/
/* add check for user abort here */
  if (channelsFound)
  {
    /* Terminate the previous scan message */
    HI_PRINT("Found Frequency %d\n",freq);
  }
  else
  {
    if (freq==previousFrequency)
      HI_PRINT("Not Found\n");
    else
      HI_PRINT("Scanning Frequency %d, ",freq);
  }
  previousFrequency=freq;

  /*if (abort_scan)
    return 1;
  else*/
    return 0;
  }
 /************************************************************************************************************************
  NAME: Si2147_GetRF
  DESCRIPTIION: Retrieve Si2147 tune freq

  Parameter:  Pointer to Si2147 Context (I2C address)
  Returns  :  frequency (Hz) as an int
************************************************************************************************************************/
int  Si2147_GetRF             (L1_Si2147_Context *api)
{
    Si2147_L1_TUNER_STATUS (api, Si2147_TUNER_STATUS_CMD_INTACK_OK);
        return api->rsp->tuner_status.freq;
}
 /************************************************************************************************************************
  NAME: Si2147_Tune
  DESCRIPTIION: Tune Si2147 in specified mode (ATV/DTV) at center frequency, wait for TUNINT and xTVINT with timeout

  Parameter:  Pointer to Si2147 Context (I2C address)
  Parameter:  Mode (ATV or DTV) use Si2147_TUNER_TUNE_FREQ_CMD_MODE_ATV or Si2147_TUNER_TUNE_FREQ_CMD_MODE_DTV constants
  Parameter:  frequency (Hz) as a unsigned long integer
  Returns:    0 if channel found.  A nonzero value means either an error occurred or channel not locked.
  Programming Guide Reference:    Flowchart A.7 (Tune flowchart)
************************************************************************************************************************/
 int  Si2147_Tune              (L1_Si2147_Context *api, unsigned char mode, unsigned long freq)
{
    int start_time  = 0;
    int return_code = 0;
    int timeout     = 36;

    if (Si2147_L1_TUNER_TUNE_FREQ (api,
                                   mode,
                                   freq) != NO_Si2147_ERROR)
    {
        return ERROR_Si2147_SENDING_COMMAND;
    }

    start_time = system_time();

    /* wait for TUNINT, timeout is 36 ms */
    while ( (system_time() - start_time) < timeout )
    {
        if ((return_code = Si2147_L1_CheckStatus(api)) != NO_Si2147_ERROR)
            return return_code;
        if (api->status->tunint)
            break;
    }
    if (!api->status->tunint) {
      SiTRACE("Timeout waiting for TUNINT\n");
      return ERROR_Si2147_TUNINT_TIMEOUT;
    }

    /* wait for xTVINT, timeout is 110ms for ATVINT and 10 ms for DTVINT */
    start_time = system_time();
    timeout    = 10;
    while ( (system_time() - start_time) < timeout )
    {
        if ((return_code = Si2147_L1_CheckStatus(api)) != NO_Si2147_ERROR)
            return return_code;
         if (api->status->dtvint)
            break;
    }

    if (api->status->dtvint)
    {
      SiTRACE("DTV Tune Successful\n");
      return NO_Si2147_ERROR;
    }
    else
      SiTRACE("Timeout waiting for DTVINT\n");

    return ERROR_Si2147_xTVINT_TIMEOUT;
}
  /************************************************************************************************************************
  NAME: Si2147_DTVTune
  DESCRIPTION: Update DTV_MODE and tune DTV mode at center frequency
  Parameter:  Pointer to Si2147 Context (I2C address)
  Parameter:  frequency (Hz)
  Parameter:  bandwidth , 6,7 or 8 MHz
  Parameter:  modulation,  e.g. use constant Si2147_DTV_MODE_PROP_MODULATION_DVBT for DVBT mode
  Parameter:  invert_spectrum, 0= normal, 1= inverted
  Returns:    I2C transaction error code, 0 if successful
  Programming Guide Reference:    Flowchart A.7 (Tune flowchart)
************************************************************************************************************************/
int  Si2147_DTVTune           (L1_Si2147_Context *api, unsigned long freq, unsigned char bw, unsigned char modulation, unsigned char invert_spectrum)
{
    int return_code = NO_Si2147_ERROR;   
    
    unsigned char  dByteBuffer[7] = {0x44, 0x00, 0x01, 0x05, 0x06, 0x00, 0x00};
    L0_WriteBytes(api->i2c, 0x00, 0x07, dByteBuffer);


    /* update DTV_MODE_PROP property */
    api->prop->dtv_mode.bw = bw;
    api->prop->dtv_mode.invert_spectrum = invert_spectrum;
    api->prop->dtv_mode.modulation = modulation;
    if (Si2147_L1_SetProperty2(api, Si2147_DTV_MODE_PROP) != NO_Si2147_ERROR)
    {
      return ERROR_Si2147_SENDING_COMMAND;
    }

    return_code = Si2147_Tune (api, Si2147_TUNER_TUNE_FREQ_CMD_MODE_DTV, freq);

    return return_code;
}
 /************************************************************************************************************************
  NAME: Si2147_XoutOn
  Parameter:  Pointer to Si2147 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_XoutOn             (L1_Si2147_Context *api)
{
    int return_code;
    SiTRACE("Si2147_XoutOn:  Turning Xout ON\n");

    if ((return_code = Si2147_L1_CONFIG_CLOCKS(api,
                                              Si2147_CONFIG_CLOCKS_CMD_SUBCODE_CODE,
                                              api->cmd->config_clocks.clock_mode,
                                              Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_EN_XOUT)) != NO_Si2147_ERROR)
    return return_code;

    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_XoutOff
  Parameter:  Pointer to Si2147 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_XoutOff            (L1_Si2147_Context *api)
{
    int return_code;
    SiTRACE("Si2147_XoutOff:  Turning Xout OFF\n");

    if ((return_code = Si2147_L1_CONFIG_CLOCKS(api,
                                              Si2147_CONFIG_CLOCKS_CMD_SUBCODE_CODE,
                                              api->cmd->config_clocks.clock_mode,
                                              Si2147_CONFIG_CLOCKS_CMD_EN_XOUT_DIS_XOUT)) != NO_Si2147_ERROR)
    return return_code;

    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_Standby
  Parameter:  Pointer to Si2147 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_Standby         (L1_Si2147_Context *api)
{
    int return_code;
    SiTRACE("Si2147_Standby: Going to Standby. The part will wake on the next command\n");

    if ((return_code = Si2147_L1_STANDBY(api, Si2147_STANDBY_CMD_TYPE_LNA_ON)) != NO_Si2147_ERROR)
    return return_code;

    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_Powerdown
  Parameter:  Pointer to Si2147 Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_Powerdown         (L1_Si2147_Context *api)
{
    int return_code;
    SiTRACE("Si2147_Powerdown: Powering down the part. Select InitAndConfig to reload.\n");

    if ((return_code = Si2147_L1_POWER_DOWN(api)) != NO_Si2147_ERROR)
    return return_code;

    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_LoadVideofilter
  DESCRIPTION:        Load video filters from vidfiltTable in Si2147_write_xTV_video_coeffs.h file into Si2147
  Programming Guide Reference:    Flowchart A.4 (Download Video Filters flowchart)

  Parameter:  Si2147 Context (I2C address)
  Parameter:  pointer to video filter table array
  Parameter:  number of lines in video filter table array(size in bytes / atv_vid_filt_struct)
  Returns:    Si2147/I2C transaction error code, NO_Si2147_ERROR if successful
************************************************************************************************************************/
int Si2147_LoadVideofilter     (L1_Si2147_Context *api, vid_filt_struct vidFiltTable[], int lines)
{
    int line;
    /* for each line in VIDFILT_TABLE  (max 16 bytes) */
    for (line = 0; line < lines; line++)
    {
        /* send up to 16 byte I2C command to Si2147 */
        if (Si2147_L1_API_Patch(api, vidFiltTable[line].vid_filt_len, vidFiltTable[line].vid_filt_table) != NO_Si2147_ERROR)
        {
            return ERROR_Si2147_SENDING_COMMAND;
        }
    }
    return NO_Si2147_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2147_AGC_Override
  Parameter:  Pointer to Si2147 Context (I2C address)
  Parameter:  Mode 0=Normal, 1= Max, 2=TOP
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
int Si2147_AGC_Override    (L1_Si2147_Context *api, unsigned char mode )
{
    int return_code;
    SiTRACE("Si2147_AGC_Override: mode = %d\n",mode);
    switch (mode)
    {
        case  Si2147_FORCE_NORMAL_AGC:
            if ((return_code = Si2147_L1_AGC_OVERRIDE (api, Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE, Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE)) != NO_Si2147_ERROR)
                return return_code;
            break;
        case  Si2147_FORCE_MAX_AGC:
            if ((return_code = Si2147_L1_AGC_OVERRIDE (api, Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_ENABLE, Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_DISABLE)) != NO_Si2147_ERROR)
                return return_code;
            break;
        case  Si2147_FORCE_TOP_AGC:
            if ((return_code = Si2147_L1_AGC_OVERRIDE (api, Si2147_AGC_OVERRIDE_CMD_FORCE_MAX_GAIN_DISABLE, Si2147_AGC_OVERRIDE_CMD_FORCE_TOP_GAIN_ENABLE)) != NO_Si2147_ERROR)
                return return_code;
            break;
        default:
            return ERROR_Si2147_PARAMETER_OUT_OF_RANGE;
    }
    return NO_Si2147_ERROR;
  }







