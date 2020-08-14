/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_eedefs.h"
#include "si_video.h"

#ifndef __EEPROM_H_20100414_
#define __EEPROM_H_20100414_

HI_U8 SI_CheckVersionID(void);
HI_U8 CheckCommModeOnly(void);
void SI_WriteDefaultConfigToEEPROM( void);
void GetSavedVideoCfg(VideoCfgType *);
void SaveVideoCfg(VideoCfgType *);
HI_U8 SI_ReadByteEEPROM(HI_U8);
void SI_WriteByteEEPROM(HI_U8, HI_U8);
HI_U8 SI_BlockWriteEEPROM(HI_U8 NBytes, HI_U16 Addr,  HI_U8 *Data);
HI_U8 SI_BlockReadEEPROM(HI_U8 NBytes, HI_U16 Addr, HI_U8 * Data);
void SI_WriteWordEEPROM(HI_U8 RegAddr, HI_U16 data);
Bool SI_IsEECfgDVDAudioSource( void );
Bool SI_IsTXFPGA( void );
Bool SI_IsTXHDCP_EEsetting( void );

HI_U16 SI_ReadWordEEPROM(HI_U8 RegAddr);

HI_VOID SI_SetEeprom(HI_U32 startAddr, HI_U32 bytes, HI_U8 value);


#define ReadEECVMPol(OFFSET)     SI_ReadByteEEPROM( EE_CUSTVMODE + OFFSET )
#define ReadFullEDIDParserType() SI_ReadByteEEPROM(EE_EDID_PARSER_ADDR)
#define ReadEEHStartPos(OFFSET)  SI_ReadWordEEPROM( EE_CUSTVMODE + OFFSET + HSTART_OFFSET )
#define ReadEEVStartPos(OFFSET)  SI_ReadWordEEPROM( EE_CUSTVMODE + OFFSET + VSTART_OFFSET )
#define ReadEEHRes(OFFSET)       SI_ReadWordEEPROM( EE_CUSTVMODE + OFFSET + HRES_OFFSET )
#define ReadEEVRes(OFFSET)       SI_ReadWordEEPROM( EE_CUSTVMODE + OFFSET + VRES_OFFSET )
#define ReadEEPixClock(OFFSET)   SI_ReadWordEEPROM( EE_CUSTVMODE + OFFSET + FPIX_OFFSET)

#endif

