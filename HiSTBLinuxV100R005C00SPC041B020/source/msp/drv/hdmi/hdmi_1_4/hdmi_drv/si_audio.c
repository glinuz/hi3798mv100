/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include <linux/string.h>
#include "si_audio.h"
#include "si_hdmitx.h"
#include "si_delay.h"
#include "si_vmode.h"
#include "si_eeprom.h"
#include "si_infofrm.h"
#include "si_txapidefs.h"
#include "si_defstx.h"

HI_U8 SI_FindFs(void);
void SI_Set_N_Fs(void);

extern Bool F_TxInit;
extern const  HI_U32 N_Val[];
extern const  HI_U32 N_Val_4K[];
extern const  PixPerSPDIFType PixPerSPDIF[4];

#define CONFIRM_CTS_TRESHOLD 5

HI_U8 ResChangeTimeOut = 0;
Bool F_ResChanged;
//CmdType * CmdPtr;
extern void SI_UpdateTX_DE(HI_U8 VMode);
extern void SI_UpdateTX_656(HI_U8 VMode);
extern void SI_SetDE(HI_U8 DE_Enabled);
extern Bool SI_WaitNewVSync( void );

//------------------------------------------------------------------------------
// Function Name: SI_Set_AudioInfoFramePacket
// Function Description:  This function will set Audio Info Fame Packets. In chip has non
//                        VSync or in Power Down mode we should call Enable Sending Audio Info Frame packet
//                        again
// Accepts: Audio path parameters
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
void SI_Set_AudioInfoFramePacket ( HI_U8 * abAudioPath, HI_U8 bChSpkAlloc, HI_U8 bDM_LSV )
{

    HI_U8 abData [10];
	
    memset(abData, 0, 10);
    memcpy(abData, abAudioPath, 10);
    SI_TX_SendInfoFrame( AUD_TYPE, abData );
	
    return;
}

//------------------------------------------------------------------------------
// Function Name: SI_SetAudioPath
// Function Description: This functiom selects audio path, for I2S/DSD oe SPDIF audio
//                       for I2S DSD Audio channel status registers must be set (Fs/length)
//                       for I2S input config must be set
//
// Called ONLY when TxAPI is enabled.
//
// Accepts: Audio path parameters
// Returns: none
// Globals: none
//------------------------------------------------------------------------------
void SI_SetAudioPath ( HI_U8 * abAudioPath )
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // abAudioPath[0] = Audio format:       0 - SPDIF;  1 - I2S;    2 - DSD;    3 - HBR
    //                  Bit 7:      0 - Layout 0 (stereo);  1 - Layout 1 (up to 8 channels)
    // abAudioPath[1] = Sampling Freq Fs:  0 - 44KHz;   2 - 48KHz ...
    // abAudioPath[2] = Sample length : 2, 4, 8, 0xA, 0xC +1 for Max 24. Even values are Max 20. Odd: Max 24.
    // abAudioPath[3] = I2S control bits (for 0x7A:0x1D)
    // 验证时使用过，正式版本暂时不确定是否需要mclk abAudioPath[4] = 0b000 = MCLK is 128*Fs 0b001 = MCLK is 256*Fs0  b010 = MCLK is 384*Fs  ...
    // abAudioPath[5] = 0x01 内部mclk 0x00 外部mclk
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    HI_U8 bRegVal1, bRegVal2, bRegVal3;
    HI_U8 bAudioModes;
    COM_INFO("abAudioPath setting auido path:0x%02x,0x%02x,0x%02x,0x%02x\n", abAudioPath[0], abAudioPath[1], abAudioPath[2], abAudioPath[3]);

    // Disable audio to allow FIFO to flush:
    bRegVal1 = ReadByteHDMITXP1 (AUD_MODE_ADDR);          // 0x7A:0x14
    bRegVal1 &= ~BIT_EN_AUDIO; 							  // disable audio input stream
    WriteByteHDMITXP1(AUD_MODE_ADDR, bRegVal1);
    //WriteByteHDMITXP1(DATA_CTRL_ADDR, MUTE_AUDIO);		  // disable output audio packets

    SI_BlockWriteEEPROM(4, EE_TX_AUDIOPATH_ADDR, abAudioPath);

    SI_Set_N_Fs();

    bAudioModes = abAudioPath[0] & SiI_AudioModesSelect;   // Input audio format

    bRegVal1 = ReadByteHDMITXP1 ( AUD_MODE_ADDR );          // 0x7A:0x14
    bRegVal2 = ReadByteHDMITXP1 ( AUDP_TXCTRL_ADDR );       // 0x7A:0x2F

    if (bAudioModes == SiI_SPDIF)
    {
        bRegVal2 &= (~BIT_LAYOUT1);
        bRegVal1 &= (~SD_0_3_EN);
/*
#if    defined(CHIP_TYPE_hi3716cv200)   \
            || defined(CHIP_TYPE_hi3716mv400)   \
            || defined(CHIP_TYPE_hi3718cv100)   \
            || defined(CHIP_TYPE_hi3719cv100)   \
            || defined(CHIP_TYPE_hi3719mv100_a)\
            || defined(CHIP_TYPE_hi3718mv100) \
            || defined(CHIP_TYPE_hi3719mv100) 
*/
#if 1            
        bRegVal1 |= BIT_SPDIF_SELECT;  
        bRegVal3 = ( SCK_RISING |  BIT_COMPRESSED);
        WriteByteHDMITXP1( I2S_IN_CTRL_ADDR, bRegVal3);


        WriteByteHDMITXP1( I2S_IN_LEN, HBRA_IN_CTRL_VAL);
        bRegVal2 |= HBRA_ZERO_PLUS;

        WriteByteHDMITXP1(I2S_CHST0_ADDR, NON_PCM_TYPE);
        WriteByteHDMITXP1(I2S_CHST4_ADDR, HBRA_FOR_CHST4);
        WriteByteHDMITXP1(I2S_CHST5_ADDR, HBRA_FOR_CHST5);
        WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, HBR_SPR_MASK);
#else        
        bRegVal1 &= (~BIT_SPDIF_SELECT);//LBR use IIS chanel
        bRegVal1 |= SD_0;//LBR use IIS chanel
        bRegVal3 = ( SCK_RISING |  BIT_COMPRESSED);
        WriteByteHDMITXP1( I2S_IN_CTRL_ADDR, bRegVal3);
        WriteByteHDMITXP1(I2S_CHST0_ADDR, NON_PCM_TYPE);
        WriteByteHDMITXP1( I2S_CHST4_ADDR, abAudioPath[1]);   // Fs
        WriteByteHDMITXP1( I2S_CHST5_ADDR, (abAudioPath[2] | ((abAudioPath[1] << 4) & 0xF0)));   // "Original Fs" and Length
#endif
        COM_INFO("use SPDIF\n");
    }
    else                                // not SPDIF
    {
        bRegVal3 = abAudioPath[2];             // Input I2S sample length
        WriteByteHDMITXP1(I2S_IN_LEN, bRegVal3);

        bRegVal1 &= (~BIT_SPDIF_SELECT);
        bRegVal1 &= (~SD_0_3_EN);       // Clear bits 7:4

        if (abAudioPath[0] & SiI_MultyChannel)
        {
            bRegVal1 |= SD_0_3_EN;          // All other modes need to enable I2S channel inputs
            bRegVal2 |= BIT_LAYOUT1;
        }
        else
        {
            bRegVal1 |= SD_0;          // All other modes need to enable I2S channel inputs
            bRegVal2 &= (~BIT_LAYOUT1);
        }
        
        if (bAudioModes == SiI_DSD)   // DSD Audio
        {
            bRegVal1 |= BIT_DSD_SELECT;
        }
        else                            // Not DSD Audio. Could be I2S or HBR
        {
            bRegVal1 &= (~BIT_DSD_SELECT);

            if (bAudioModes == SiI_I2S)
            {
                WriteByteHDMITXP1( I2S_CHST4_ADDR, abAudioPath[1]);   // Fs
                WriteByteHDMITXP1( I2S_CHST5_ADDR, (abAudioPath[2] | ((abAudioPath[1] << 4) & 0xF0)));   // "Original Fs" and Length

                WriteByteHDMITXP1( I2S_IN_CTRL_ADDR, abAudioPath[3]);
            }
            else                        // HBRA
            {
                COM_INFO("set HBRA in Audio Path\n");
                bRegVal3 = (BIT_HBR_ON | SCK_RISING | BIT_CBIT_ORDER | BIT_COMPRESSED);
                WriteByteHDMITXP1( I2S_IN_CTRL_ADDR, bRegVal3);		     	// Write 0xF0 to 0x7A:0x1D

                bRegVal1 |= SETUP_ENABLE_HBRA;                       		// Write 0xF1 to 0x7A:0x14
                WriteByteHDMITXP1( I2S_IN_LEN, HBRA_IN_CTRL_VAL);   		// Write 0x92 to 0x7A:0x24
                //bRegVal2 = HBRA_ZERO_PLUS;                          		// Write 0x01 to0x7A:0x2F
                bRegVal2 &= (~BIT_LAYOUT1);

                WriteByteHDMITXP1(I2S_CHST0_ADDR, NON_PCM_TYPE); 			// Write 0x04 to0x7A:0x1E to set 0x1E[1] (workaround)
                WriteByteHDMITXP1(I2S_CHST4_ADDR, HBRA_FOR_CHST4); 			// Write 0x09 to0x7A:0x21
                WriteByteHDMITXP1(I2S_CHST5_ADDR, HBRA_FOR_CHST5); 			// Write 0xE2 to0x7A:0x22
                WriteByteHDMITXP1(SAMPLE_RATE_CONVERSION, HBR_SPR_MASK);	// Write 0x0 to 0x7A:0x23
            }
        }
        COM_INFO("use I2S\n");
    }

    DelayMS(1); 											// allow FIFO to flush
    bRegVal1 |= BIT_EN_AUDIO; 								// Enable audio
    WriteByteHDMITXP1( AUD_MODE_ADDR, bRegVal1);
    COM_INFO("Set AUDP_TXCTRL_ADDR to AUD_MODE_ADDR:0x%x AUDP_TXCTRL_ADDR:0x%x\n", bRegVal1, bRegVal2);
    WriteByteHDMITXP1 ( AUDP_TXCTRL_ADDR , bRegVal2 );
    //WriteByteHDMITXP1(DATA_CTRL_ADDR, ENABLE_ALL);		  // Enable output audio packets
    SI_SetNCtsEnable(HI_TRUE);
    return;
}

HI_U8 SI_FindFs(void)
{
    HI_U8 tmpArray[2] = {0}, Fs;
    SI_BlockReadEEPROM ( 2, EE_TX_AUDIOPATH_ADDR, tmpArray );
    if ((tmpArray[0] & SiI_AudioModesSelect) == SiI_SPDIF)  // if input audio format is SPDIF
    {
        if (F_TxInit)
            Fs = tmpArray[1] & 0x0F;
        else
            Fs = tmpArray[1] & 0x0F;
		
        WriteByteHDMITXP1(AUD_MODE_ADDR, 0x03);     // En.SPDIF
    }
    else
        Fs = tmpArray[1] & 0x0F;

    return Fs;
}


void SI_Set_N_Fs(void)
{
    HI_U8 F_Sampling;
    HDMI_VIDEO_ATTR_S   *pstVidAttr = DRV_Get_VideoAttr(HI_UNF_HDMI_ID_0);

    F_Sampling = SI_FindFs();
    //if(DRV_Get_VSDBMode(HI_UNF_HDMI_ID_0) == VSDB_MODE_4K)
    if(DRV_Get_Is4KFmt(pstVidAttr->enVideoFmt))
    {
        COM_INFO("4K N Value \n");
        SI_WriteNValue(N_Val_4K[F_Sampling]);
    }
    else
    {
        COM_INFO("Non 4K N Value \n");
        SI_WriteNValue(N_Val[F_Sampling]);
    }
    WriteByteHDMITXP1( I2S_CHST4_ADDR, F_Sampling);
}


//------------------------------------------------------------------------------
// Function Name: SI_GetAudioPath
// Function Description: This functiom is used to get Audio Path parameters
// Accepts: pointer on Audio path parameters
// Returns: pointer on Audio path parameters
// Globals: none
//------------------------------------------------------------------------------
void SI_GetAudioPath ( HI_U8 * abAudioPath )
{
    HI_U8 bRegVal;

    abAudioPath[0] = 0;

    bRegVal = ReadByteHDMITXP1 ( AUD_MODE_ADDR );

    if ( bRegVal & BIT_DSD_SELECT )
        abAudioPath[0] = SiI_DSD;
    else
    {
        if ( bRegVal & BIT_SPDIF_SELECT )
            abAudioPath[0] = SiI_SPDIF;
        else
            abAudioPath[0] = SiI_I2S;
    }

    bRegVal = ReadByteHDMITXP1 ( AUDP_TXCTRL_ADDR );
    if ( bRegVal & BIT_LAYOUT1 )
        abAudioPath[0] |= SiI_MultyChannel;

    abAudioPath[1] = ReadByteHDMITXP1( I2S_CHST4_ADDR );
    abAudioPath[2] = ReadByteHDMITXP1( I2S_CHST5_ADDR );
    abAudioPath[3] = ReadByteHDMITXP1( I2S_IN_CTRL_ADDR );

}


//-------------------------------------------------------------
Bool SI_IsSPDIFSelected( void )
{
    if( ReadByteHDMITXP1(AUD_MODE_ADDR) & BIT_SPDIF_SELECT )
        return TRUE;
    return FALSE;
}
//-------------------------------------------------------------
void SI_TXAudio(HI_U8 On)
{
    HI_U8 RegVal;

    RegVal = ReadByteHDMITXP1(AUD_MODE_ADDR);
    if(On)
        WriteByteHDMITXP1(AUD_MODE_ADDR, RegVal | BIT_AUDIO_EN);
    else
        WriteByteHDMITXP1(AUD_MODE_ADDR, RegVal & (~BIT_AUDIO_EN));
}
//-------------------------------------------------------------

/*--------------------------END-------------------------------*/
