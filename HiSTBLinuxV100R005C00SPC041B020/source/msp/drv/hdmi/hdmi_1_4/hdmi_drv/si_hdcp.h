/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_typedefs.h"
#include "si_hlviic.h"
#include "si_globdefs.h"
#include "si_hdcpdefs.h"

#ifndef __SI_HDCP_H__
#define __SI_HDCP_H__

HI_U8 AreR0sMatch(HI_U8 * Match);
HI_U8 AreRiMatch(HI_U8 *);
HI_U8 SI_Authentication( void );
HI_U8 CheckIfHDMIRX(void);
HI_U8 SI_CheckFIFOReady(void);
void AuthHandler(void);
void SI_ReAthentication( void );
void SI_AutoRiCheck( Bool );
void SI_AutoRiFailureHandler( HI_U8 );
void SI_AutoKSVReadyCheck ( Bool );
HI_U8 SI_ReadKSVList_Timeout(void);
HI_U8 MDDCBlockWriteHDCPRX(HI_U8 NBytes, HI_U8 Addr, HI_U8 * pData);
HI_U8 MDDCBlockReadHDCPRX(HI_U8 NBytes, HI_U8 Addr, HI_U8 *pData) ;

extern HI_U8 AuthState;
extern HI_U8 MX_TX[8];

#endif
