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
#include "si_eedefs.h"
#include "si_video.h"
#include "si_delay.h"
#include "si_eeprom.h"
#include "si_txvptbl.h"
#include "si_txapidefs.h"

#include "hi_unf_hdmi.h"

HI_U8 DefaultAVIInfoFrame[0xd] = {
    0, 0, 0, 0,  // Color Space YC24 (YCbCr 4:2:2),
    0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00
};

HI_U8 DefaultTXVPath[4] = {
                        // INPUT           OUTPUT
  //0x3, 0x00,          // YC 24      |    RGB24
  //0x3, 0x01,          // YC 24      |    YCbCr24
    0x2, 0x01,          // YCbCr24    |    YCbCr24
    0x01,               // falling edge input
    SiI_DeepColor_Off   // Deep Color = off
};

HI_U8 DefaultTXDVDAudio[4] = {
      0x00, // DVDA vs SPDIF    SiI_MultyChannel
      _48kHz, // Fs
      _24Bits | LMax_Minus0,    // Sample Length
      0x40 // Control reg Page 1, 0x1D
};

/* g_au8EepromMem is virual share memory block for HDMI Control chip to get parameter */
static HI_U8            g_au8EepromMem[EE_MAX_SIZE] = {0};

//lc check static pthread_mutex_t  g_hdmiEepromMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_HDMI_EEPROM_LOCK()  	//  (void)pthread_mutex_lock(&g_hdmiEepromMutex)
#define HI_HDMI_EEPROM_UNLOCK() //  (void)pthread_mutex_unlock(&g_hdmiEepromMutex)

//-----------------------------------------------------------
// Read block of DATA from EEPROM

HI_U8 SI_BlockReadEEPROM(HI_U8 NBytes, HI_U16 Addr, HI_U8 * Data)
{
    HI_U8 i;
    HI_HDMI_EEPROM_LOCK();
    for (i = 0; i < NBytes; i++)
    {
        Data[i] = g_au8EepromMem[Addr+i];
    }
    HI_HDMI_EEPROM_UNLOCK();
    return 0;
}

//-----------------------------------------------------------
HI_U8 SI_BlockWriteEEPROM(HI_U8 NBytes, HI_U16 Addr,  HI_U8 *Data)
{
    HI_U8 i;
    
    HI_HDMI_EEPROM_LOCK();
    
    for (i = 0; i < NBytes; i++)
    {
        g_au8EepromMem[Addr+i] = Data[i];
    }
    
    HI_HDMI_EEPROM_UNLOCK();
    return 0;
}

//------------------------------------------------------------------------
HI_U8 SI_ReadByteEEPROM(HI_U8 RegAddr)
{
    return g_au8EepromMem[RegAddr];
}

//------------------------------------------------------------------------
void SI_WriteByteEEPROM(HI_U8 RegAddr, HI_U8 RegData)
{
    HI_HDMI_EEPROM_LOCK();

    g_au8EepromMem[RegAddr] = RegData;    

    HI_HDMI_EEPROM_UNLOCK();
}

HI_U16 SI_ReadWordEEPROM(HI_U8 RegAddr)
{
    HI_U16 data = 0;
    
    HI_HDMI_EEPROM_LOCK();

    data = g_au8EepromMem[RegAddr];
    data = data << 8;
    data |= g_au8EepromMem[RegAddr+1];    

    HI_HDMI_EEPROM_UNLOCK();     

    return data;
}

//------------------------------------------------------------------------
void SI_WriteWordEEPROM(HI_U8 RegAddr, HI_U16 RegData)
{
    HI_HDMI_EEPROM_LOCK();

    g_au8EepromMem[RegAddr] =   (RegData >> 8) & 0xff;    
    g_au8EepromMem[RegAddr+1] = RegData & 0xff;    

    HI_HDMI_EEPROM_UNLOCK();    
}
//------------------------------------------------------------------------

HI_U8 SI_CheckVersionID(void) 
{
    HI_U16 RegVal;
    RegVal = SI_ReadWordEEPROM(VER_ID_ADDR);
    if(RegVal == SiI_ID_EEPROM_VER)
        return 1;
    return 0;
}

//-------------------------------------------------------------------------
void SI_WriteDefaultConfigToEEPROM( void)
{
    SI_WriteWordEEPROM( VER_ID_ADDR, SiI_ID_EEPROM_VER);
    
    SI_WriteByteEEPROM(EE_AVIINFO_ENABLE, 0);
    SI_WriteByteEEPROM(EE_SPDINFO_ENABLE, 0);
    SI_WriteByteEEPROM(EE_AUDINFO_ENABLE, 0);

    SI_WriteByteEEPROM(EE_EDID_PARSER_ADDR, 0); // use LONG parser

    SI_WriteByteEEPROM(EE_TX_VIDEO_MODE_INDEX_ADDR, 1);   // 1==480p // 2==720p

    SI_WriteByteEEPROM(EE_TX_ICLK_ADDR, 0); //ixel data is not replicated 0b01 =
    
    SI_WriteByteEEPROM(EE_TX_DE_ENABLED_ADDR, 0);         // DE disable and force HDMI mode


    SI_WriteByteEEPROM(EE_TX_MCLK_INDEX_ADDR, 0x01);      //256Fs

    SI_WriteByteEEPROM(EE_TX_INTERNAL_MCLK_ADDR,0x01);//MCLK_EN  Internal generate

    SI_WriteByteEEPROM(EE_TX_FPGA, 0xFF);
    SI_WriteByteEEPROM(EE_TX_HDCP, 0/*0xFF*/);      /* hdcp set to default disable */

    SI_BlockWriteEEPROM( 13, EE_TXAVIINFO_ADDR, DefaultAVIInfoFrame);
    SI_BlockWriteEEPROM( 4, EE_TX_VIDEOPATH_ADDR , DefaultTXVPath);
    SI_BlockWriteEEPROM( 4, EE_TX_AUDIOPATH_ADDR , DefaultTXDVDAudio);

}



//-------------------------------------------------------------------------
Bool SI_IsEECfgDVDAudioSource( void )
{
  if(SI_ReadByteEEPROM(EE_TX_AUDIOPATH_ADDR) & 0x01)
       return TRUE;
  return FALSE;
}
//-------------------------------------------------------------------------
Bool SI_IsTXFPGA( void )
{
    if( SI_ReadByteEEPROM(EE_TX_FPGA) & 0x01)
        return TRUE;
    return FALSE;
}

//-------------------------------------------------------------------------
#if defined (HDCP_SUPPORT)
Bool SI_IsTXHDCP_EEsetting( void ) 
{
    HI_U8 ucHdcp = SI_ReadByteEEPROM(EE_TX_HDCP);
    
    if( ucHdcp && 0xFF)
        return TRUE;
    return FALSE;
}

#endif
HI_VOID SI_SetEeprom(HI_U32 startAddr, HI_U32 bytes, HI_U8 value)
{
    HI_U32 i;
    
    HI_HDMI_EEPROM_LOCK();
    
    for (i = 0; i < bytes; i++)
    {
        g_au8EepromMem[startAddr+i] = value;
    }
    
    HI_HDMI_EEPROM_UNLOCK();
    
    return;   
}

