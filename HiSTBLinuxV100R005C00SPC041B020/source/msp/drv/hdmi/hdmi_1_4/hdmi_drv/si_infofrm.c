/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/string.h>
#include <linux/delay.h>
#include "si_typedefs.h"
#include "si_infofrm.h"
#include "si_isr.h"
#include "si_hlviic.h"
#include "si_video.h"
#include "si_delay.h"
#include "si_defstx.h"
#include "si_hdmitx.h"
#include "si_eeprom.h"
#include "si_timer.h"
#include "si_mpi_hdmi.h"

extern HDMIIntType HDMIIntRX;

//----------------------------------------------------------------------------------
static Bool WaitBuffReady(HI_U16 CtrInfoBits)
{
    HI_U16 RegVal, RptBitMsk, EnBitMask;
    HI_U8 TimeOutCount = 60;
    Bool Result = TRUE;

    RptBitMsk = (CtrInfoBits >> 1) & CtrInfoBits;
    EnBitMask = (CtrInfoBits << 1) & CtrInfoBits;
    RegVal = ReadWordHDMITXP1( INF_CTRL1);
    if ( RegVal & CtrInfoBits )
    {
        if ( RegVal & RptBitMsk )
        {
            WriteWordHDMITXP1( INF_CTRL1, RegVal & (~RptBitMsk)); // Clear reapet bit
            udelay(1);
        }
        while(TimeOutCount--)
        {
            if( !(ReadWordHDMITXP1( INF_CTRL1) & EnBitMask))
                break;
            //DelayMS(1);
        }      
        if(TimeOutCount)
            Result = TRUE; // ready to rewrite data buffer
        else
        {
#if 0//lc
            Result = FALSE; // repeat bit is stuck, not ready to write into the buffer
#else
            COM_ERR("force to set Infoframe buffer ready\n");
            Result = TRUE;
#endif   
        }
    }
    return Result;
}
//----------------------------------------------------------------------------------

static void CalcCheckSumIFPacket( HeaderType * Header, HI_U8 * InfoFramePacket)
{
    HI_U8 i;
    // HI_U8 2 length
    // HI_U8 3 checksum
    Header->CheckSum = Header->Type + Header->Length + Header->Version;

    for (i = 0; i < Header->Length; i++ )
        Header->CheckSum += InfoFramePacket[i];
    Header->CheckSum =  0x100 - Header->CheckSum;
}

static Bool GetRestOfInfoFrameHeader( HeaderType * Header )
{
    Bool Result = TRUE;

    if( Header->Type == AVI_TYPE)
    {
        Header->Version = 2 ;
        Header->Length = 13;
    }
    else if( Header->Type == SPD_TYPE)
    {
        Header->Version = 1;
        Header->Length = 25;
    }
    else if( Header->Type == AUD_TYPE)
    {
        Header->Version = 1 ;
        Header->Length = 10;
    }
    else if( Header->Type == MPEG_TYPE)
    {
        Header->Version = 1;
        Header->Length = 10;
    }
    else if( Header->Type == VENDORSPEC_TYPE)
    {
        Header->Version = 1;
        Header->Length = 9;// will be change
    }
    else
        Result = FALSE;
    return Result;
}
//---------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static Bool GetInfoFrameMapAddr(HI_U8 InfoType, InfoMapType * InfoMap )
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
    else if( InfoType == MPEG_TYPE)
    {
        InfoMap->CtrlBitsAddr = BIT_MPEG_EN_REPEAT;
        InfoMap->DestAddr = MPEG_IF_ADDR;
    }
    else if(InfoType == VENDORSPEC_TYPE)
    {
        //Use MPEG INforframe option
        InfoMap->CtrlBitsAddr = BIT_MPEG_EN_REPEAT;
        InfoMap->DestAddr = MPEG_IF_ADDR;
    }
    else
        Result = FALSE;
    return Result;
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#define WR_IN_PD       1
#define RPT_BIT_STUCK  2
#define EN_BIT_STUCK   3
#define WRGN_IFR_TYPE  4
#define WRNG_IPK_ID    5
#define BUF_SIZE_ERR   6
#define UNSTABLE_TCLK  7
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
///#define siiIsTClockStable()  ( ReadByteHDMITXP0( TX_STAT_ADDR ) & 0x01 )

HI_U8 SI_TX_SendInfoFrame( HI_U8 bIFType, HI_U8 * InfoFramePacket )
{
    HI_U8 ErrNo = 0;
    HI_U8 index;
    InfoMapType InfoMap;
    HeaderType IFHeader;
    HI_U8 TXAddr;

    HI_U16 RegVal;
    
    memset(&IFHeader, 0, sizeof(HeaderType));
    if( GetInfoFrameMapAddr( bIFType, &InfoMap) )
    {
        IFHeader.Type = bIFType;
        GetRestOfInfoFrameHeader(  &IFHeader );
        if ( !siiIsTClockStable() )
            ErrNo = UNSTABLE_TCLK;
        else if ( !WaitBuffReady(InfoMap.CtrlBitsAddr ) )
            ErrNo = EN_BIT_STUCK;

        CalcCheckSumIFPacket( &IFHeader, InfoFramePacket );

        TXAddr = InfoMap.DestAddr;
        /* Fill header */
        WriteByteHDMITXP1(TXAddr++, IFHeader.Type);
        WriteByteHDMITXP1(TXAddr++, IFHeader.Version);
        WriteByteHDMITXP1(TXAddr++, IFHeader.Length);
        WriteByteHDMITXP1(TXAddr++, IFHeader.CheckSum);
        /* Fill Body */
        for(index = 0; index < IFHeader.Length; index ++)
        {
            WriteByteHDMITXP1(TXAddr++, InfoFramePacket[index]);
        }
        
        TXAddr = InfoMap.DestAddr;
#if defined (DEBUG_SUPPORT)
        COM_INFO("Debug Info frame output length:%d\n", (IFHeader.Length + 4));
        for(index = 0; index < (IFHeader.Length + 4); index++ )
        {
            RegVal = ReadByteHDMITXP1( TXAddr++);
            COM_INFO("0x%02x\n", RegVal);
        }
        COM_INFO("End of Info frame\n");
#endif
        if ( !ErrNo )
        {
            RegVal = ReadWordHDMITXP1( INF_CTRL1);
            WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
            udelay(1);//lc add
        }
    }
    else
        ErrNo = WRGN_IFR_TYPE;
    return ErrNo;

}

HI_U8 SI_TX_SendInfoFramex( HI_U8 bIFType, HI_U8 * InfoFramePacket, HI_U8 length)
{
    HI_U8 ErrNo = 0;
    HI_U8 index;
    InfoMapType InfoMap;
    HeaderType IFHeader;
    HI_U8 TXAddr;

    HI_U16 RegVal;
    
    memset(&IFHeader, 0, sizeof(HeaderType));
    if( GetInfoFrameMapAddr( bIFType, &InfoMap) )
    {
        IFHeader.Type = bIFType;
        GetRestOfInfoFrameHeader(  &IFHeader );
        IFHeader.Length = length;
        if ( !siiIsTClockStable() )
            ErrNo = UNSTABLE_TCLK;
        else if ( !WaitBuffReady(InfoMap.CtrlBitsAddr ) )
            ErrNo = EN_BIT_STUCK;

        CalcCheckSumIFPacket( &IFHeader, InfoFramePacket );

        TXAddr = InfoMap.DestAddr;
        /* Fill header */
        WriteByteHDMITXP1(TXAddr++, IFHeader.Type);
        WriteByteHDMITXP1(TXAddr++, IFHeader.Version);
        WriteByteHDMITXP1(TXAddr++, IFHeader.Length);
        WriteByteHDMITXP1(TXAddr++, IFHeader.CheckSum);
        /* Fill Body */
        for(index = 0; index < IFHeader.Length; index ++)
        {
            WriteByteHDMITXP1(TXAddr++, InfoFramePacket[index]);
        }
        
        TXAddr = InfoMap.DestAddr;
#if defined (DEBUG_SUPPORT)
        COM_INFO("Debug Info frame output length:%d\n", (IFHeader.Length + 4));
        for(index = 0; index < (IFHeader.Length + 4); index++ )
        {
            RegVal = ReadByteHDMITXP1( TXAddr++);
            COM_INFO("0x%02x\n", RegVal);
        }
        COM_INFO("End of Info frame\n");
#endif
        if ( !ErrNo )
        {
            RegVal = ReadWordHDMITXP1( INF_CTRL1);
            WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
            udelay(1);//lc add
        }
    }
    else
        ErrNo = WRGN_IFR_TYPE;
    return ErrNo;

}

//----------------------------------------------------------------------------------
// Get Buffer Address
//----------------------------------------------------------------------------------
static HI_U8 GetInfoPacketMapAddr(HI_U8 BlockBuffer, InfoMapType * InfoMap )
{
    Bool Result = TRUE;

    if( BlockBuffer == 1 )
    {
        InfoMap->CtrlBitsAddr = BIT_AVI_EN_REPEAT;
        InfoMap->DestAddr =  AVI_IF_ADDR;
        InfoMap->BlockSize = AVI_SIZE;
    }
    else if( BlockBuffer == 2 )
    {
        InfoMap->CtrlBitsAddr = BIT_SPD_EN_REPEAT;
        InfoMap->DestAddr = SPD_IF_ADDR;
        InfoMap->BlockSize = SPD_IF_SIZE;
    }
    else if( BlockBuffer == 3 )
    {
        InfoMap->CtrlBitsAddr = BIT_AUD_EN_REPEAT;
        InfoMap->DestAddr = AUD_IF_ADDR;
        InfoMap->BlockSize = AUD_IF_SIZE;
    }
    else if( BlockBuffer == 4 )
    {
        InfoMap->CtrlBitsAddr = BIT_MPEG_EN_REPEAT;
        InfoMap->DestAddr = MPEG_IF_ADDR;
        InfoMap->BlockSize = MPEG_IF_SIZE;
    }
    else if ( BlockBuffer == 5 )
    {
        InfoMap->CtrlBitsAddr = BIT_GENERIC1_EN_REPEAT;
        InfoMap->DestAddr = GENERIC1_IF_ADDR;
        InfoMap->BlockSize = GENERIC1_IF_SIZE;
    }
    else if ( BlockBuffer == 6 )
    {
        InfoMap->CtrlBitsAddr = BIT_GENERIC2_EN_REPEAT;
        InfoMap->DestAddr = GENERIC2_IF_ADDR;
        InfoMap->BlockSize = GENERIC2_IF_SIZE;
    }
    else
        Result = FALSE;
    return Result;

}
//----------------------------------------------------------------------------------
// Send InfoPackets, the main difference compare to InfoFrames, there no check sums
//----------------------------------------------------------------------------------
HI_U8 SI_SendInfoPacket(HI_U8 * Data, HI_U8 PcktSize, HI_U8 PacketId, HI_U8 Mode)
{
    InfoMapType InfoMap;
    HI_U8 ErrNo = 0;
    HI_U16 RegVal;


    if (GetInfoPacketMapAddr( PacketId, &InfoMap))
    {
        if(PcktSize > InfoMap.BlockSize)
            ErrNo = BUF_SIZE_ERR;
        else
        {
            if(Mode == WR_IN_PD)
            {      // Write in PD mode
                SI_WriteBlockHDMITXP1( InfoMap.DestAddr, PcktSize, Data); // Write Info packet
            }
            else
            {
                if(WaitBuffReady(InfoMap.CtrlBitsAddr))
                {
                    SI_WriteBlockHDMITXP1( InfoMap.DestAddr, PcktSize, Data); // Send Header
                    RegVal = ReadWordHDMITXP1( INF_CTRL1);
                    WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
                   // udelay(1);//lc add
                }
                else
                    ErrNo = RPT_BIT_STUCK;
            }
        }
    }
    else
        ErrNo = WRNG_IPK_ID;
    
    return ErrNo;
}
//----------------------------------------------------------------------------------
// Enable Info Packets
//----------------------------------------------------------------------------------
Bool SI_EnableInfoPacket( HI_U8 PacketId )
{
    InfoMapType InfoMap;
    Bool Result = FALSE;
    HI_U16 RegVal;

    RegVal = ReadWordHDMITXP1( INF_CTRL1);
    if(GetInfoPacketMapAddr( PacketId, &InfoMap))
    {

        if(WaitBuffReady(InfoMap.CtrlBitsAddr))
        {
            WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
            Result = TRUE;
        }
    }
    return Result;
}
//----------------------------------------------------------------------------------
// Bisable Info Packets
//----------------------------------------------------------------------------------
Bool SI_DisableInfoPacket( HI_U8 PacketId )
{
    InfoMapType InfoMap;
    Bool Result = FALSE;
    HI_U16 RegVal;

    RegVal = ReadWordHDMITXP1( INF_CTRL1);
    if (GetInfoPacketMapAddr( PacketId, &InfoMap))
    {

        WriteWordHDMITXP1( INF_CTRL1, RegVal & (~ InfoMap.CtrlBitsAddr) );
        Result = TRUE;
    }
    return Result;
}

//----------------------------------------------------------------------------------
// InfoFrame will be disabled, don't call in PD mode
//----------------------------------------------------------------------------------
Bool SI_DisableInfoFrame(HI_U8 InfoFrameType)
{
    InfoMapType InfoMap;
    HI_U16 RegVal;
    Bool Result = FALSE;

    RegVal = ReadWordHDMITXP1( INF_CTRL1);
    if(GetInfoFrameMapAddr( InfoFrameType, &InfoMap))
    {
        WriteWordHDMITXP1( INF_CTRL1, RegVal & (~InfoMap.CtrlBitsAddr) );
        Result = TRUE;
    }
    return Result;

}


//----------------------------------------------------------------------------------
// InfoFrame will be enable and set repeat property, InfoFrame cannot be enabled at Power Down mode
//----------------------------------------------------------------------------------
Bool SI_EnableInfoFrame(HI_U8 InfoFrameType)
{
    InfoMapType InfoMap;
    HI_U16 RegVal;
    Bool Result = FALSE;

    RegVal = ReadWordHDMITXP1( INF_CTRL1);
    if(GetInfoFrameMapAddr( InfoFrameType, &InfoMap))
    {
        WriteWordHDMITXP1( INF_CTRL1, RegVal | InfoMap.CtrlBitsAddr );
        DelayMS(1);
        Result = TRUE;
    }
    return Result;
}

//----------------------------------------------------------------------------------
// This function is used to send AVI from TX

void SI_SendAVIInfoFrame(void)
{
    HI_U8 CRC, RegVal, TXAddr, EEAddr,i;

    TXAddr = AVI_IF_ADDR;
    EEAddr  = EE_TXAVIINFO_ADDR;

    CRC = 0x82 + 0x02 + 13;

    for(i = 0; i < 13; i++ )
        CRC += SI_ReadByteEEPROM ( EEAddr++);
    CRC = 0x100 - CRC;
    RegVal = ReadByteHDMITXP1( INF_CTRL1 );

    // To send a new AVI InfoFrame, must first clera RPT bit, then wait for EN bit to go to 0, then
    // OK to set EN and RPT bits:

    RegVal &= (~BIT_AVI_REPEAT);	// Clear RPT bit
    RegVal &= (~BIT_AVI_ENABLE);	// Clear Enable bit
    WriteByteHDMITXP1(INF_CTRL1, RegVal);

#if 0 //lc change 
    DelayMS(64); // Allow EN bit to clear
    if( ReadByteHDMITXP1(INF_CTRL1) & BIT_AVI_ENABLE) //EN bit did not clear
    {
        COM_ERR(" Send AVI Err:0x%x, En not Zero! :0x%x\n", RegVal, ReadByteHDMITXP1(INF_CTRL1));
        //return;
    }
#endif
    DelayMS(5);

    WriteByteHDMITXP1( TXAddr++ , 0x82);
    WriteByteHDMITXP1( TXAddr++ , 0x02);
    WriteByteHDMITXP1( TXAddr++ , 13);     //0x0D
    WriteByteHDMITXP1( TXAddr++ , CRC);    // Send Header

    EEAddr  = EE_TXAVIINFO_ADDR;

    COM_INFO("Read AVI Data\n");
    for(i = 0; i < 13; i++ )
    {
        RegVal = SI_ReadByteEEPROM ( EEAddr++);
        WriteByteHDMITXP1( TXAddr++ , RegVal);
    }
    COM_INFO("open AVI packet en/repeat\n");
    RegVal = ReadByteHDMITXP1( INF_CTRL1);
    COM_INFO("open AVI packet en/repeat2\n");
    WriteByteHDMITXP1(INF_CTRL1, RegVal | ( BIT_AVI_ENABLE | BIT_AVI_REPEAT));
    COM_INFO("open AVI packet en/repeat3\n");
    RegVal = ReadByteHDMITXP1( INF_CTRL1);
    COM_INFO("open AVI packet en/repeat4\n");

    TXAddr = AVI_IF_ADDR;
#if defined (DEBUG_SUPPORT)
    COM_INFO("Debug AVI output:\n");
    for(i = 0; i < 17; i++ )
    {
        RegVal = ReadByteHDMITXP1( TXAddr++);
        COM_INFO("0x%02x\n", RegVal);
    }
    COM_INFO("End of AVI inforframe\n");
#endif
    return;
}



extern HI_BOOL get_current_rgb_mode(HI_UNF_HDMI_ID_E enHdmi);
//-------------------------------------------------------------------------------
void SI_SendCP_Packet(HI_U8 bMuteAv)
{
	//5次循环CP Packet ERR产生原因还未定位出来，后续需要继续跟终
    //HI_U8 RegVal, TimeOutCount = 5;
    HI_U8 RegVal, TimeOutCount = 64;
    HI_U8 BlankValue[3];

    COM_INFO("MUTE=%u.\n", bMuteAv);

    if(get_current_rgb_mode(HI_UNF_HDMI_ID_0))
    {
        BlankValue[0] = 0;
        BlankValue[1] = 0;
        BlankValue[2] = 0;
    }
    else
    {
        BlankValue[0] = 128;
        BlankValue[1] = 16;
        BlankValue[2] = 128;
    }
    
    /* also mute av in DVI mode  */
    if (SI_IsTXInHDMIMode())				// Send CP packets only if in HDMI mode
    {
        SI_WriteBlockHDMITXP0(TX_VID_BLANK1_BLUE, 3, BlankValue);
    	RegVal = ReadByteHDMITXP1( INF_CTRL2 );
        // Before enabling Control Packet transmission (CP_EN, CP_RPT), the firmware must write 0x10 or 0x01 to this register. The 
        // default value of 0x00 is not valid in a transmitted Control Packet. 
    	// Oscar Added 20090828 for ATC 7-19 failed issue
    	WriteByteHDMITXP1(INF_CTRL2, RegVal & (~(BIT_CP_ENABLE | BIT_CP_REPEAT)));        
        while(--TimeOutCount)
        {
            if( !(ReadByteHDMITXP1( INF_CTRL2) & BIT_CP_ENABLE))
            {
                break;
            }                
            DelayMS(1);
        }
        if(TimeOutCount == 0)
        {
            COM_INFO("CP_Packet do not DiSable before setting\n");
        }
        
        if(bMuteAv)
        {
            SI_SetHdmiVideo(HI_FALSE);
            WriteByteHDMITXP1( CP_IF_ADDR, BIT_CP_AVI_MUTE_SET);
        }
        else
        {   
            SI_SetHdmiVideo(HI_TRUE);
            WriteByteHDMITXP1( CP_IF_ADDR, BIT_CP_AVI_MUTE_CLEAR);
        }
        
        WriteByteHDMITXP1(INF_CTRL2, RegVal | BIT_CP_ENABLE | BIT_CP_REPEAT);
    }
#if defined (DVI_SUPPORT)
    else   /* DVI Mode */
    {
        SI_WriteBlockHDMITXP0(TX_VID_BLANK1_BLUE, 3, BlankValue);
        msleep(30);
        if(bMuteAv == HI_TRUE)
        {
            SI_SetHdmiVideo(HI_FALSE);
            WriteByteHDMITXP1( CP_IF_ADDR, BIT_CP_AVI_MUTE_SET);
        }
        else
        {
            SI_SetHdmiVideo(HI_TRUE);
            WriteByteHDMITXP1( CP_IF_ADDR, BIT_CP_AVI_MUTE_CLEAR);
        }
    }
#endif
}

HI_U8 SI_SendGamutMeta_Packet(HI_U8 u8Is7109)
{
    HI_U8 DestAddr = 0x00;
    HI_U8 u8GamutMetaData[0X1F];
    HI_U8 dataindex, BlockSize = 0X1F;	    

    HI_U8 RegVal;
    HI_BOOL Result = HI_FALSE;

    HI_U8 Format_Flag, GBD_Color_Precision, GBD_Color_Space;
    HI_U8 Next_Field, GBD_profile, Affected_Gamut_Seq_Num;
    HI_U8 No_Current_GBD, Packet_Seq, Current_Gamut_Seq_Num;

    /* Note: Register 0x72:0xFF must be set appropriately to access page 2. */

    dataindex = 0;
    /* Fill Gamut Meta Packet */
    memset(u8GamutMetaData, 0, 0x1f);
    u8GamutMetaData[0] = 0X0A;     //HB0:0X0A

    Next_Field = 0x80;   //HB0:Bit7
    /*
      The lowest speed transmission profile is P0, 
      transmitting at a rate of one Gamut Metadata Packet per video field.
      P0 metadata fits completely within that one packet.

      no HDMI Source may transmit a GBD with more than four vertices or containing facet data,
      nor may any Sink indicate support for transmission profiles other than P0 in the Colorimetry Data Block in the EDID.
    */
    GBD_profile = 0X00;  //must be 0:P0, HB0:Bit4~6, 0:P0,1:P1,2:P2,3:P3
    Affected_Gamut_Seq_Num = 0X01; //HB0:Bit0~4
    u8GamutMetaData[1] = Next_Field | GBD_profile| Affected_Gamut_Seq_Num;

    No_Current_GBD = 0X00; //HB0:Bit7
    /* Packet_Seq setting:
        (0b00) Intermediate packet in sequence
        (0b01) First packet in sequence
        (0b10) Last packet in sequence
        (0b11) Only packet in sequence (i.e. P0) 
    */
    Packet_Seq = 0X30; //must be 0b11, HB0:Bit4~5 
    Current_Gamut_Seq_Num = 0X01; //HB0:Bit0~4
    u8GamutMetaData[2] = No_Current_GBD | Packet_Seq | Current_Gamut_Seq_Num;

    Format_Flag         = 0x80; //1:Facets are included
    GBD_Color_Precision = 0x10;
    if (u8Is7109 == HI_TRUE)
    {
        GBD_Color_Space = 0x02;
    }
    else
    {
        GBD_Color_Space = 0x01;
    }
    u8GamutMetaData[3] = Format_Flag | GBD_Color_Precision | GBD_Color_Space;
    //Color-Component Component-Bit-Depth Black-level Nominal-Peak-(White level) Black-level Nominal-Peak(White-level) Valid Range
    //R / G / B           12                  0                 4095               256             3760                 16 to 4079
    dataindex = 4;
#if 0    
    u8GamutMetaData[dataindex ++] = 16 / 16;   //Min_Red_Data_H
    u8GamutMetaData[dataindex ++] = (16 % 16) | (4079 / 256);  //Min_Red_Data_L||Max_Red_Data_H
    u8GamutMetaData[dataindex ++] = (4079 % 256);  //Max_Red_Data_L
    u8GamutMetaData[dataindex ++] = 16 / 16;   //Min_Green_Data_H
    u8GamutMetaData[dataindex ++] = (16 % 16) | (4079 / 256);  //Min_Green_Data_L||Max_Green_Data_H
    u8GamutMetaData[dataindex ++] = (4079 % 256);  //Max_Green_Data_L
    u8GamutMetaData[dataindex ++] = 16 / 16;   //Min_Blue_Data_H
    u8GamutMetaData[dataindex ++] = (16 % 16) | (4079 / 256);  //Min_Blue_Data_L||Max_Blue_Data_H
    u8GamutMetaData[dataindex ++] = (4079 % 256);  //Max_Blue_Data_L    
#else
    u8GamutMetaData[dataindex ++] = 0x9b; //Min_Red_Data_H
    u8GamutMetaData[dataindex ++] = 0x52; //Min_Red_Data_L||Max_Red_Data_H
    u8GamutMetaData[dataindex ++] = 0xf4; //Max_Red_Data_L
    u8GamutMetaData[dataindex ++] = 0x8d; //Min_Green_Data_H
    u8GamutMetaData[dataindex ++] = 0x72; //Min_Green_Data_L||Max_Green_Data_H
    u8GamutMetaData[dataindex ++] = 0x96; //Max_Green_Data_L
    u8GamutMetaData[dataindex ++] = 0x8c; //Min_Blue_Data_H
    u8GamutMetaData[dataindex ++] = 0xc2; //Min_Blue_Data_L||Max_Blue_Data_H
    u8GamutMetaData[dataindex ++] = 0x92; //Max_Blue_Data_L
#endif

    /* Write Gamut Meta Packet to Gamut Metadata Registers */
    //WriteByteHDMITXP0( 0xFF, 0x01 );  //REG_BANK0 0: bank0 (Page 0) 1: bank1 (Page 2)
    //DEBUG_PRINTK("Before Write Gamut Meta Packet data is:\n");
    for(dataindex = 0; dataindex < BlockSize; dataindex ++)
    {
        //DEBUG_PRINTK("0x%02x, ", u8GamutMetaData[dataindex]);
        //WriteByteHDMITXP0((DestAddr + dataindex), u8GamutMetaData[dataindex]);
        WriteByte_Region2_HDMITXP0((DestAddr + dataindex), u8GamutMetaData[dataindex]);
    }
    //DEBUG_PRINTK("\n");
    //WriteByteHDMITXP0( 0xFF, 0x00 );  //REG_BANK0 0: bank0 (Page 0) 1: bank1 (Page 2)
    
    /* Enable and repeat send Gamut Metadata packet */
    RegVal = ReadByteHDMITXP1( INF_CTRL2);
    WriteByteHDMITXP1( INF_CTRL2, RegVal | GAMUT_METADATA_EN_REPEAT );

    //WriteByteHDMITXP0( 0xFF, 0x01 );  //REG_BANK0 0: bank0 (Page 0) 1: bank1 (Page 2)
#if defined (DEBUG_SUPPORT)
    COM_INFO("After write Gamut Meta Packet data is:\n");
    memset(u8GamutMetaData, 0, 0x1f);
    for(dataindex = 0; dataindex < BlockSize; dataindex ++)
    {
        //u8GamutMetaData[dataindex] = ReadByteHDMITXP0((DestAddr + dataindex));
        u8GamutMetaData[dataindex] = ReadByte_Region2_HDMITXP0((DestAddr + dataindex));
        COM_INFO("0x%02x, ", u8GamutMetaData[dataindex]);
    }
    COM_INFO("\n");
    //WriteByteHDMITXP0( 0xFF, 0x00 );  //REG_BANK0 0: bank0 (Page 0) 1: bank1 (Page 2)

    RegVal = 0;
    RegVal = ReadByteHDMITXP1(INF_CTRL2);
    COM_INFO("INF_CTRL2:0x%x\n", RegVal);
#endif
    return Result;
}

HI_U8 SI_3D_Setting(HI_U32 u83DFormat)
{
    switch(u83DFormat)
    {
    case HI_UNF_EDID_3D_FRAME_PACKETING:
        SI_Set_VSI_3D_FramePacking();
        break;
#if defined (SUPPORT_3D)
    case HI_UNF_EDID_3D_FIELD_ALTERNATIVE:
        SI_Set_VSI_3D_FieldAlt();
        break;
    case HI_UNF_EDID_3D_LINE_ALTERNATIVE:
        SI_Set_VSI_3D_LineAlt();
        break;
    case HI_UNF_EDID_3D_L_DEPTH:
        SI_Set_VSI_3D_L_Depth();
        break;
    case HI_UNF_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH:
        SI_Set_VSI_3D_L_Depth_2Graphic_Depth();
        break;
    case HI_UNF_EDID_3D_SIDE_BY_SIDE_FULL:
        SI_Set_VSI_3D_SidebySide_Full();
        break;
#endif
    case HI_UNF_EDID_3D_TOP_AND_BOTTOM:
        SI_Set_VSI_3D_TopandBottom_Half();
        break;
    case HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF:
        SI_Set_VSI_3D_SidebySide_Half();
        break;
    default:
        COM_ERR("Unknown 3D Mode \n");
        return HI_SUCCESS;
    }
        
    SI_EnableInfoFrame(VENDORSPEC_TYPE);
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_FramePacking(void)
{
    HI_U8 VendorBody[20], offset = 0;
    HI_U8 U8HDMI_Video_Format = 0, u83D_Structure = 0;

    COM_INFO("3D FramePacking Mode\n");
    offset = 0;
    //IEEE
    VendorBody[offset++] = 0x03;
    VendorBody[offset++] = 0x0c;
    VendorBody[offset++] = 0x00;
    
    U8HDMI_Video_Format = 0x02;//3D present
    u83D_Structure = 0X00;   //FramePacking
    
    VendorBody[offset++] = (U8HDMI_Video_Format & 0x3) << 5;
    VendorBody[offset++] = (u83D_Structure & 0x0f) << 4;
    SI_TX_SendInfoFramex(VENDORSPEC_TYPE, VendorBody, offset);
    
    return HI_SUCCESS;
}

#if defined (SUPPORT_3D)
HI_U8 SI_Set_VSI_3D_FieldAlt(void)
{
    HI_U8 VendorBody[20], offset = 0;
    
    offset = 0;

    SI_TX_SendInfoFrame(VENDORSPEC_TYPE, VendorBody);
    
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_LineAlt(void)
{
    HI_U8 VendorBody[20], offset = 0;
    
    offset = 0;

    SI_TX_SendInfoFrame(VENDORSPEC_TYPE, VendorBody);
    
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_SidebySide_Full(void)
{
    HI_U8 VendorBody[20], offset = 0;
    
    offset = 0;

    SI_TX_SendInfoFrame(VENDORSPEC_TYPE, VendorBody);
    
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_L_Depth(void)
{
    HI_U8 VendorBody[20], offset = 0;
    
    offset = 0;

    SI_TX_SendInfoFrame(VENDORSPEC_TYPE, VendorBody);
    
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_L_Depth_2Graphic_Depth(void)
{
    HI_U8 VendorBody[20], offset = 0;
    
    offset = 0;

    SI_TX_SendInfoFrame(VENDORSPEC_TYPE, VendorBody);
    
    return HI_SUCCESS;
}
#endif

HI_U8 SI_Set_VSI_3D_TopandBottom_Half(void)
{
    HI_U8 VendorBody[20], offset = 0;
    HI_U8 U8HDMI_Video_Format = 0, u83D_Structure = 0;

    //COM_INFO("3D Top-and-Bottom Mode\n");
    COM_INFO("3D Top-and-Bottom Mode\n");
    offset = 0;
    //IEEE
    VendorBody[offset++] = 0x03;
    VendorBody[offset++] = 0x0c;
    VendorBody[offset++] = 0x00;
    
    U8HDMI_Video_Format = 0x02;//3D present
    u83D_Structure = 0X06;   //Top-and-Bottom
    
    VendorBody[offset++] = (U8HDMI_Video_Format & 0x3) << 5;
    VendorBody[offset++] = (u83D_Structure & 0x0f) << 4;
    SI_TX_SendInfoFramex(VENDORSPEC_TYPE, VendorBody, offset);
    
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_3D_SidebySide_Half(void)
{
    HI_U8 VendorBody[20], offset = 0;
    HI_U8 U8HDMI_Video_Format = 0, U8HDMI_VIC = 0, u83D_Structure = 0;
    HI_U8 u83D_Meta_present = 0, u83D_Metadat_type = 0, u83D_Ext_Data = 0;
    //HI_U8 index, u83D_Metadata_Length = 0, u83D_Metadat[32];
    HI_U8 u83D_Metadata_Length = 0, u83D_Metadat[32];

    COM_INFO("3D side-by-side Mode\n");
    offset = 0;
    //IEEE
    VendorBody[offset++] = 0x03;
    VendorBody[offset++] = 0x0c;
    VendorBody[offset++] = 0x00;
    
    U8HDMI_Video_Format = 0x02;//3D present
    //U8HDMI_VIC = 16;//1080p_60Hz
    U8HDMI_VIC = 2;//480p_60Hz
    u83D_Structure = 0X08;   //side-by-side(half)
    u83D_Meta_present = 0x00;
    u83D_Ext_Data = 0x00; //Old/Left picture, Odd/Right picture
    u83D_Metadat_type = 0x0;
    u83D_Metadata_Length = 0;
    memset(u83D_Metadat, 0, 32);
    
    VendorBody[offset++] = (U8HDMI_Video_Format & 0x3) << 5;    
    //VendorBody[offset++] = U8HDMI_VIC;//No 3D
    VendorBody[offset++] = ((u83D_Structure & 0x0f) << 4) | ((u83D_Meta_present & 0x01) << 3);
    VendorBody[offset++] = ((u83D_Ext_Data & 0x0f) << 4);
    VendorBody[offset++] = ((u83D_Metadat_type & 0x7) << 5) | (u83D_Metadata_Length & 0x1f);

/*-- we are not using Metadataextend now.if need set it,then open the remark-*/  
/*-- don't delete it -*/  
#if 0 
    for(index = 0; index < u83D_Metadata_Length; index ++)
    {
        VendorBody[offset++] = u83D_Metadat[index];
    }
#endif

    SI_TX_SendInfoFramex(VENDORSPEC_TYPE, VendorBody, offset);
    
    return HI_SUCCESS;
}

HI_U8 SI_4K_Setting(HI_U8 u324KFormat)
{
    switch(u324KFormat)
    {
    case HI_DRV_DISP_FMT_3840X2160_30:
    case HI_DRV_DISP_FMT_3840X2160_25:
    case HI_DRV_DISP_FMT_3840X2160_24:
    case HI_DRV_DISP_FMT_4096X2160_24:
        // 4k setting 
        SI_Set_VSI_4K2K(u324KFormat);
        break;
    default:
        COM_ERR("Unknown 4K Mode \n");
        return HI_SUCCESS;
    }
    SI_EnableInfoFrame(VENDORSPEC_TYPE);
    return HI_SUCCESS;
}

HI_U8 SI_Set_VSI_4K2K(HI_U32 u324KFormat)
{
    HI_U8 VendorBody[20], offset = 0;
    HI_U8 U8HDMI_Video_Format = 0, u8HDMI_VIC = 0;

    COM_INFO("3D FramePacking Mode\n");
    offset = 0;
    //IEEE
    VendorBody[offset++] = 0x03;
    VendorBody[offset++] = 0x0c;
    VendorBody[offset++] = 0x00;
    
    U8HDMI_Video_Format = 0x01;//Extended resolution format (e.g. used for 4K x 2K video) 

    if(u324KFormat == HI_DRV_DISP_FMT_3840X2160_30)
    {
        u8HDMI_VIC = 0X01;   //2160P30
    }
    else if(u324KFormat == HI_DRV_DISP_FMT_3840X2160_25)
    {
        u8HDMI_VIC = 0X02;   //2160P25
    }
    else if(u324KFormat == HI_DRV_DISP_FMT_3840X2160_24)
    {
        u8HDMI_VIC = 0X03;   //2160P24
    }
    else if(u324KFormat == HI_DRV_DISP_FMT_4096X2160_24)
    {
        u8HDMI_VIC = 0X04;   //4096*2160@24
    }
    

    VendorBody[offset++] = (U8HDMI_Video_Format & 0x3) << 5;
    VendorBody[offset++] = u8HDMI_VIC;
    SI_TX_SendInfoFramex(VENDORSPEC_TYPE, VendorBody, offset);
    
    return HI_SUCCESS;
}

HI_U8 SI_GetAVIInfoFrameVID(void)
{
    HI_U8 u8Value;
    //Read VIC form AVI Inforframe
    u8Value = ReadByteHDMITXP1(0x47);
    //DRV_HDMI_ReadRegister((HI_U32)0x1017051C, &u32Value);
    return u8Value;
}

//set vsdb mode,and base on vsdb mode, to setting 3d infoframe or 4k infoframe
HI_U8 SI_VSDB_Setting(VSDB_MODE_E mode,HI_U32 u32Fmt)
{
    VSDB_MODE_E forMode = DRV_Get_VSDBMode(HI_UNF_HDMI_ID_0);
    VSDB_MODE_E curMode = mode;

    COM_INFO("VSDB mode:%d , u8Fmt:%d \n",mode,u32Fmt);
    switch(curMode)
    {
    case VSDB_MODE_3D:
        SI_3D_Setting(u32Fmt);
        break;
    case VSDB_MODE_4K:
        SI_4K_Setting(u32Fmt);
        break;
    case VSDB_MODE_NONE:
        SI_DisableInfoFrame(VENDORSPEC_TYPE);          
        if(forMode != curMode)
        {
            SI_DisableHdmiDevice();
    	    msleep(100);
    	    SI_EnableHdmiDevice();
    	}   
        break;
    default:
 	    COM_ERR("Unknown VSDB Mode !\n");
        break;
    }

    DRV_Set_VSDBMode(HI_UNF_HDMI_ID_0,curMode);

    return HI_SUCCESS;
}


//----------------------------------------------------------------------------------

