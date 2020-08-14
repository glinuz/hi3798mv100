/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef __MDDC_H__
#define __MDDC_H__


#include "si_typedefs.h"
#include "si_defsmddc.h"
#include "si_hdmitx.h"
#include "drv_reg_proc.h"

#define MDDCWriteOffset(SGM_OFFSET)  WriteByteHDMITXP0(MDDC_SEGMENT_ADDR, SGM_OFFSET)
#define MDDCCommand( CMD ) WriteByteHDMITXP0(MDDC_COMMAND_ADDR, CMD)
#define ClrManualCtrlReg() WriteByteHDMITXP0(MDDC_MANUAL_ADDR, 0)
#define MDDCReadOffset()  ReadByteHDMITXP0(MDDC_SEGMENT_ADDR)

typedef struct {
    HI_U8 SlaveAddr;
    HI_U8 Offset;
    HI_U8 RegAddr;
    HI_U8 NBytesLSB;
    HI_U8 NBytesMSB;
    HI_U8 Dummy;
    HI_U8 Cmd;
    HI_U8 * PData;
    HI_U8 Data[6];
} MDDCType;


HI_U8 SI_BlockRead_MDDC(MDDCType * );
HI_U8 SI_BlockWrite_MDDC(MDDCType * );
HI_U8 SI_BlockWriteHDMITXP0(HI_U8 , HI_U8 , HI_U8 *);
HI_U8 SI_BlockReadHDMITXP0(HI_U8 , HI_U8 , HI_U8 *);
HI_U8 SI_BlockReadMDDCFifo(HI_U8 ucLen, HI_U8 *pucData);

#endif

