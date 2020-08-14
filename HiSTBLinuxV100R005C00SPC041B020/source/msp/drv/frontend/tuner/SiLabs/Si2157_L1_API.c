/*************************************************************************************
                  Silicon Laboratories Broadcast Si2157 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API functions definitions used by commands and properties
   FILE: Si2157_L1_API.c
   Supported IC : Si2157
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Tag:  ROM50_3_4_build_4_V0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2157_COMMAND_PROTOTYPES

#include "Si2157_L1_API.h"


/***********************************************************************************************************************
  Si2157_L1_API_Init function
  Use:        software initialisation function
              Used to initialize the software context
  Returns:    0 if no error
  Comments:   It should be called first and once only when starting the application
  Parameter:   **ppapi         a pointer to the api context to initialize
  Parameter:  add            the Si2157 I2C address
  Porting:    Allocation errors need to be properly managed.
  Porting:    I2C initialization needs to be adapted to use the available I2C functions
 ***********************************************************************************************************************/
unsigned char    Si2157_L1_API_Init      (L1_Si2157_Context *api, int add) {
    api->i2c = &(api->i2cObj);

    L0_Init(api->i2c);
    L0_SetAddress(api->i2c, add, 0);

    api->cmd    = &(api->cmdObj);
    api->rsp    = &(api->rspObj);
    api->prop   = &(api->propObj);
    api->status = &(api->statusObj);
    api->propShadow = &(api->propShadowObj);
/* set the propertyWriteMode to DOWNLOAD_ON_CHANGE to only download property settings on change (recommended) */
/* if the propertyWriteMode is set to DOWNLOAD_ALWAYS the property settings will be written regardless of change */
    api->propertyWriteMode = DOWNLOAD_ON_CHANGE;
    return NO_Si2157_ERROR;
}
/***********************************************************************************************************************
  Si2157_L1_API_Patch function
  Use:        Patch information function
              Used to send a number of bytes to the Si2157. Useful to download the firmware.
  Returns:    0 if no error
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
unsigned char    Si2157_L1_API_Patch     (L1_Si2157_Context *api, int iNbBytes, unsigned char *pucDataBuffer) {
    unsigned char res;
    unsigned char rspByteBuffer[1];

    SiTRACE("Si2157 Patch %d bytes\n",iNbBytes);

    res = L0_WriteCommandBytes(api->i2c, iNbBytes, pucDataBuffer);
    if (res!=iNbBytes) {
      SiTRACE("Si2157_L1_API_Patch error writing bytes: %s\n", Si2157_L1_API_ERROR_TEXT(ERROR_Si2157_LOADING_FIRMWARE) );
      return ERROR_Si2157_LOADING_FIRMWARE;
    }

    res = Si2157_pollForResponse(api, 1, rspByteBuffer);
    if (res != NO_Si2157_ERROR) {
      SiTRACE("Si2157_L1_API_Patch error 0x%02x polling response: %s\n", res, Si2157_L1_API_ERROR_TEXT(res) );
      return ERROR_Si2157_POLLING_RESPONSE;
    }

    return NO_Si2157_ERROR;
}
/***********************************************************************************************************************
  Si2157_L1_CheckStatus function
  Use:        Status information function
              Used to retrieve the status byte
  Returns:    0 if no error
  Parameter:  error_code the error code.
 ***********************************************************************************************************************/
unsigned char    Si2157_L1_CheckStatus   (L1_Si2157_Context *api) {
    unsigned char rspByteBuffer[1];
    return Si2157_pollForResponse(api, 1, rspByteBuffer);
}
/***********************************************************************************************************************
  Si2157_L1_API_ERROR_TEXT function
  Use:        Error information function
              Used to retrieve a text based on an error code
  Returns:    the error text
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
char*            Si2157_L1_API_ERROR_TEXT(int error_code) {
    switch (error_code) {
        case NO_Si2157_ERROR                     : return (char *)"No Si2157 error";
        case ERROR_Si2157_ALLOCATING_CONTEXT     : return (char *)"Error while allocating Si2157 context";
        case ERROR_Si2157_PARAMETER_OUT_OF_RANGE : return (char *)"Si2157 parameter(s) out of range";
        case ERROR_Si2157_SENDING_COMMAND        : return (char *)"Error while sending Si2157 command";
        case ERROR_Si2157_CTS_TIMEOUT            : return (char *)"Si2157 CTS timeout";
        case ERROR_Si2157_ERR                    : return (char *)"Si2157 Error (status 'err' bit 1)";
        case ERROR_Si2157_POLLING_CTS            : return (char *)"Si2157 Error while polling CTS";
        case ERROR_Si2157_POLLING_RESPONSE       : return (char *)"Si2157 Error while polling response";
        case ERROR_Si2157_LOADING_FIRMWARE       : return (char *)"Si2157 Error while loading firmware";
        case ERROR_Si2157_LOADING_BOOTBLOCK      : return (char *)"Si2157 Error while loading bootblock";
        case ERROR_Si2157_STARTING_FIRMWARE      : return (char *)"Si2157 Error while starting firmware";
        case ERROR_Si2157_SW_RESET               : return (char *)"Si2157 Error during software reset";
        case ERROR_Si2157_INCOMPATIBLE_PART      : return (char *)"Si2157 Error Incompatible part";
        case ERROR_Si2157_UNKNOWN_COMMAND        : return (char *)"Si2157 Error unknown command";
        case ERROR_Si2157_UNKNOWN_PROPERTY       : return (char *)"Si2157 Error unknown property";
        case ERROR_Si2157_TUNINT_TIMEOUT         : return (char *)"Si2157 Error TUNINT Timeout";
        case ERROR_Si2157_xTVINT_TIMEOUT         : return (char *)"Si2157 Error xTVINT Timeout";
        default                                  : return (char *)"Unknown Si2157 error code";
    }
}
/***********************************************************************************************************************
  Si2157_L1_API_TAG_TEXT function
  Use:        Error information function
              Used to retrieve a text containing the TAG information (related to the code version)
  Returns:    the TAG text
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
char*            Si2157_L1_API_TAG_TEXT(void) { return (char *)"ROM50_3_4_build_4_V0.1";}








