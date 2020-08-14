/*************************************************************************************
                  Silicon Laboratories Broadcast Si2147 Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties functions definitions
   FILE: Si2147_Properties_Functions.h
   Supported IC : Si2147
   Compiled for ROM 50 firmware 3_4_build_4
   Revision: 0.1
   Date: November 06 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef   _Si2147_PROPERTIES_FUNCTIONS_H_
#define   _Si2147_PROPERTIES_FUNCTIONS_H_

void          Si2147_storeUserProperties    (Si2147_PropObj   *prop);
unsigned char Si2147_PackProperty                  (Si2147_PropObj   *prop, unsigned int prop_code, int *data);
unsigned char Si2147_UnpackProperty                (Si2147_PropObj   *prop, unsigned int prop_code, int  data);
void          Si2147_storePropertiesDefaults       (Si2147_PropObj   *prop);

int  Si2147_downloadATVProperties   (L1_Si2147_Context *api);
int  Si2147_downloadCOMMONProperties(L1_Si2147_Context *api);
int  Si2147_downloadDTVProperties   (L1_Si2147_Context *api);
int  Si2147_downloadTUNERProperties (L1_Si2147_Context *api);
int  Si2147_downloadAllProperties   (L1_Si2147_Context *api);

#endif /* _Si2147_PROPERTIES_FUNCTIONS_H_ */







