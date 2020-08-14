/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
//#include "typedefs.h"
#include "globdefs.h"
#include "hlviic.h"
#include "defstx.h"

#ifndef __HDMI_TX_H__
#define __HDMI_TX_H__

#ifdef _9032_WRITEBLOCKP0_
void WriteBlockHDMITXP0(HI_U8 , HI_U8 , HI_U8 * );
#endif
void WriteBlockHDMITXP1(HI_U8 , HI_U8 , HI_U8 * );

void DelayMS (HI_U16 count);
HI_U8 IsNeedToForceInDVIMode(void);
void InitHDMITX(void);
void InitDVITX(void);
void HW_ResetHDMITX( void );
void TransmiterToPD(void);
void WakeUpHDMITX(void);
void PowerDownHdmiTx(void);
void SetMCLK(HI_U8);
void SetIClk(HI_U8);
HI_U8 IsRXAttached( void );
HI_U8 IsTXInHDMIMode( void );
HI_U8 IsRXAttachedWithD(void);
HI_U8 IsHPDPinHigh( void );
void WriteNValue(HI_U32);
void SW_ResetHDMITX(void);
void AssertHDMITX_SWReset( HI_U8 );
void ReleaseHDMITX_SWReset( HI_U8 );

void HDMI_ProcessCableIn(void);
void TX_SetHDMIMode(HI_U8 Enabled);

HI_S32 TX_CTRL_DVIEncoder(HI_BOOL bEnable);
HI_U32 TX_RSEN_Status(void);


void ReadBlockHDMITXP0(HI_U8 Addr, HI_U8 NBytes, HI_U8 * Data );

#define ReadByteHDMITXP0(ADDR) ReadByte_8BA(BUS_1, TX_SLV0, ADDR)
#define WriteByteHDMITXP0(ADDR, DATA) WriteByte_8BA(BUS_1, TX_SLV0, ADDR, DATA)
#define ReadByteHDMITXP1(ADDR) ReadByte_8BA(BUS_1, TX_SLV1, ADDR)
#define WriteByteHDMITXP1(ADDR, DATA) WriteByte_8BA(BUS_1, TX_SLV1, ADDR, DATA)
#define ReadByteHDMITXCEC(ADDR) ReadByte_8BA(BUS_1, HDMI_DEV_CEC, ADDR)
#define WriteByteHDMITXCEC(ADDR, DATA) WriteByte_8BA(BUS_1, HDMI_DEV_CEC, ADDR, DATA)


#define ReadWordHDMITXP0(ADDR) ReadWord_8BA(BUS_1, TX_SLV0, ADDR)
#define WriteWordHDMITXP0(ADDR, DATA) WriteWord_8BA(BUS_1, TX_SLV0, ADDR, DATA)
#define WriteWordHDMITXP1(ADDR, DATA) WriteWord_8BA(BUS_1, TX_SLV1, ADDR, DATA)
#define ReadWordHDMITXP1(ADDR) ReadWord_8BA(BUS_1, TX_SLV1, ADDR)


/*
#define WaitForAckHDMITX( TO ) WaitForAck(BUS_1, TX_SLV0, TO)
*/
#define GetSysStat() ReadByteHDMITXP0(TX_STAT_ADDR)
#define GetRevId() ReadByteHDMITXP0(0x04)
#define ReadCTSValue() (HI_U32)(ReadWordHDMITXP1(HWCTS_VAL_ADDR))|((HI_U32)(ReadByteHDMITXP1(HWCTS_VAL_ADDR+2)& 0x0F)<<16))
#define WritePixPerSPDIF( PPSPDIF )   WriteByte_8BA(BUS_1, TX_SLV1, SPDIF_SSMPL_ADDR, PPSPDIF)
#define ReadChipInfo( DATA) ReadBlockHDMITXP0( 0x02, 3 ,  DATA)

#define CLR_BITS_1_0	0xFC

#define WAIT_FOR_PIXEL_CK	0x01
#define P_STABLE_BIT		0x01
#define WAIT_V_SYNC			0x02




#endif

