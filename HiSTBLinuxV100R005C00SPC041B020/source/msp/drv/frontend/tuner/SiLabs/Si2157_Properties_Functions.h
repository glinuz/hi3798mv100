/*************************************************************************************
                  Silicon Laboratories Broadcast Si2157 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties functions definitions
   FILE: Si2157_Properties_Functions.h
   Supported IC : Si2157
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2157_PROPERTIES_FUNCTIONS_H_
#define   _Si2157_PROPERTIES_FUNCTIONS_H_

void          Si2157_storeUserProperties    (Si2157_PropObj   *prop);
unsigned char Si2157_PackProperty                  (Si2157_PropObj   *prop, unsigned int prop_code, int *data);
unsigned char Si2157_UnpackProperty                (Si2157_PropObj   *prop, unsigned int prop_code, int  data);
void          Si2157_storePropertiesDefaults       (Si2157_PropObj   *prop);

int  Si2157_downloadATVProperties   (L1_Si2157_Context *api);
int  Si2157_downloadCOMMONProperties(L1_Si2157_Context *api);
int  Si2157_downloadDTVProperties   (L1_Si2157_Context *api);
int  Si2157_downloadTUNERProperties (L1_Si2157_Context *api);
int  Si2157_downloadAllProperties   (L1_Si2157_Context *api);

#endif /* _Si2157_PROPERTIES_FUNCTIONS_H_ */







