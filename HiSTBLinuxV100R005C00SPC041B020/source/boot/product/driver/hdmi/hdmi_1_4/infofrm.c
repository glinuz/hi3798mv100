/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
//#include "typedefs.h"
#include "infofrm.h"
//#include "isr.h"
#include "hlviic.h"
//#include "video.h"
//#include "delay.h"
#include "defstx.h"
#include "hdmitx.h"
//#include "eeprom.h"
//#include "mpi_hdmi.h"
#include "hi_unf_hdmi.h"

//----------------------------------------------------------------------------------
Bool GetInfoFrameMapAddr(HI_U8 InfoType, InfoMapType * InfoMap )
{
    Bool Result = TRUE;

    if( InfoType == AVI_TYPE)
    {
        InfoMap->CtrlBitsAddr = BIT_AVI_EN_REPEAT;
        InfoMap->DestAddr =  AVI_IF_ADDR;
    }
    else if( InfoType == SPD_TYPE)
    {
        InfoMap->CtrlBitsAddr = BIT_SPD_EN_REPEAT;
        InfoMap->DestAddr = SPD_IF_ADDR;
    }
    else if( InfoType == AUD_TYPE)
    {
        InfoMap->CtrlBitsAddr = BIT_AUD_EN_REPEAT;
        InfoMap->DestAddr = AUD_IF_ADDR;
    }
    else if( (InfoType == MPEG_TYPE) 
        || (InfoType == VENDORSPEC_TYPE))
    {
        InfoMap->CtrlBitsAddr = BIT_MPEG_EN_REPEAT;
        InfoMap->DestAddr = MPEG_IF_ADDR;
    }
    else
        Result = FALSE;
    return Result;
}
//----------------------------------------------------------------------------------
// InfoFrame will be enable and set repeat property, InfoFrame cannot be enabled at Power Down mode
//----------------------------------------------------------------------------------
Bool EnableInfoFrame(HI_U8 InfoFrameType)
{
    InfoMapType InfoMap;
    HI_U16 RegVal;
    Bool Result = FALSE;

    //RegVal = ReadWordHDMITXP1( INF_CTRL1);
    RegVal = ReadByteHDMITXP1( INF_CTRL1);
    if(GetInfoFrameMapAddr( InfoFrameType, &InfoMap))
    {
        //WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
        WriteByteHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
        Result = TRUE;
    }
    return Result;

}

//----------------------------------------------------------------------------------
// This function is used to send AVI from TX




void SendAVIInfoFrame(HI_U8 *Data,HI_U8 Len)
{
    //HI_U8 CRC, RegVal, TXAddr, EEAddr,i;
    HI_U8 CRC, RegVal, TXAddr, i;

    TXAddr = AVI_IF_ADDR;
    //EEAddr  = EE_TXAVIINFO_ADDR;

    CRC = 0x82 + 0x02 + 13;

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < 13; i++ )
        CRC += ReadByteEEPROM ( EEAddr++);
    CRC = 0x100 - CRC;
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < Len; i++ )
        CRC += Data[i];
    CRC = 0x100 - CRC;

    RegVal = ReadByteHDMITXP1( INF_CTRL1 );

    // To send a new AVI InfoFrame, must first clera RPT bit, then wait for EN bit to go to 0, then
    // OK to set EN and RPT bits:

    WriteByteHDMITXP1(INF_CTRL1, RegVal & (~BIT_AVI_REPEAT));   // Clear RPT bit

    //default value is disable aviinfoFrm,so we do not need delay after disable infoFrm(y00229039)
#if 0 
    DelayMS(128); // Allow EN bit to clear

    if( ReadByteHDMITXP1( INF_CTRL1) & BIT_AVI_ENABLE) //EN bit did not clear
    {
        HDMIPrint(" Send AVI Err, En not Zero! ");
        return;
    }
#endif

    WriteByteHDMITXP1( TXAddr++ , 0x82);
    WriteByteHDMITXP1( TXAddr++ , 0x02);
    WriteByteHDMITXP1( TXAddr++ , 13);     //0x0D
    WriteByteHDMITXP1( TXAddr++ , CRC);    // Send Header

    //EEAddr  = EE_TXAVIINFO_ADDR;

    HDMIPrint(" SendAVIInfoFrame\n ");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < 13; i++ )
    {
        RegVal = ReadByteEEPROM ( EEAddr++);
        WriteByteHDMITXP1( TXAddr++ , RegVal);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < Len; i++ )
    {
        RegVal = Data[i];
        WriteByteHDMITXP1( TXAddr++ , RegVal);
    }


    RegVal = ReadByteHDMITXP1( INF_CTRL1);
    WriteByteHDMITXP1(INF_CTRL1, RegVal | ( BIT_AVI_ENABLE | BIT_AVI_REPEAT));
    RegVal = ReadByteHDMITXP1( INF_CTRL1);
}


void Send4KInfoFrame(HI_U8 *Data,HI_U8 Len)
{
    //HI_U8 CRC, RegVal, TXAddr, EEAddr,i;
    HI_U8 CRC, RegVal, TXAddr, i;

    TXAddr = MPEG_IF_ADDR;
    //EEAddr  = EE_TXAVIINFO_ADDR;

    CRC = 0x81 + 0x01 + Len;

    for(i = 0; i < Len; i++ )
        CRC += Data[i];
    CRC = 0x100 - CRC;

    RegVal = ReadByteHDMITXP1( INF_CTRL1 );

    // To send a new AVI InfoFrame, must first clera RPT bit, then wait for EN bit to go to 0, then
    // OK to set EN and RPT bits:

    WriteByteHDMITXP1(INF_CTRL1, RegVal & (~BIT_MPEG_REPEAT));   // Clear RPT bit

    WriteByteHDMITXP1( TXAddr++ , 0x81);
    WriteByteHDMITXP1( TXAddr++ , 0x01);
    WriteByteHDMITXP1( TXAddr++ , Len);     //0x0D
    WriteByteHDMITXP1( TXAddr++ , CRC);    // Send Header

    //EEAddr  = EE_TXAVIINFO_ADDR;

    HDMIPrint(" SendAVIInfoFrame\n ");
#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < 13; i++ )
    {
        RegVal = ReadByteEEPROM ( EEAddr++);
        WriteByteHDMITXP1( TXAddr++ , RegVal);
    }
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    for(i = 0; i < Len; i++ )
    {
        RegVal = Data[i];
        WriteByteHDMITXP1( TXAddr++ , RegVal);
    }
}



