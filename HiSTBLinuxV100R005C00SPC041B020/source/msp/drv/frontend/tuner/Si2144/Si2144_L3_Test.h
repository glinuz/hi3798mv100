/***************************************************************************************
                  Silicon Laboratories Broadcast Si2144 Layer 3 console application

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L3 sample application header
   FILE: Si2144_L3_Test.h
   Supported IC : Si2144
   Compiled for ROM 62 firmware 2_0_build_2
   Revision: REVISION_NAME
   Date: November 06 2014
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
****************************************************************************************/

#ifndef   Si2144_L3_TEST_H
#define   Si2144_L3_TEST_H

#define   Si2144_COMMAND_PROTOTYPES

#include "Si2144_L2_API.h"
#include "Si2144_Commands_Prototypes.h"

#define   Si2144_BASE_ADDRESS 0xc0
#define   SYSTEM_PAUSE system("pause")

void Si2144_configure_i2c_communication    (L1_Si2144_Context *api);

#ifdef    Si2144_COMMAND_LINE_APPLICATION

int  Si2144_DisplayStatusByte              (L1_Si2144_Context *api);
int  Si2144_GetRevision                    (L1_Si2144_Context *api);
int  Si2144_TunerStatus                    (L1_Si2144_Context *Si2144);
int  Si2144_GetUserFrequency               (unsigned long *fIn);
int  Si2144_help                           (void);
void Si2144_menu                           (unsigned char full);
int  Si2144_demoLoop                       (L1_Si2144_Context *api, char* choice);

#endif /* Si2144_COMMAND_LINE_APPLICATION */

#endif /* Si2144_L3_TEST_H */







