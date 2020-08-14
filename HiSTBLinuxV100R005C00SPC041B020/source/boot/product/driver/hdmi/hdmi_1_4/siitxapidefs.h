/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
//------------------------------------------------------------------------------
// Module Name: SiITXAPIDefs.h
//
// Module Description: describes SiITXAPIDefs
//-----------------------------------------------------------------------------
// These defines are used for Audio Stream
//-----------------------------------------------------------------------------
typedef enum {
    _22kHz = 0x04,
    _24kHz = 0x06,
    _32kHz = 0x03,
    _44kHz = 0x00,
    _48kHz = 0x02,
    _88kHz = 0x08,
    _96kHz = 0x0A,
    _176kHz = 0x0C,
    _192kHz = 0x0E
}I2SFs;

//-----------------------------------------------------------------------------
typedef enum
{
  _24Max_20 = 0x02,
  _24Max_21 = 0x0c,
  _24Max_22 = 0x04,
  _24Max_23 = 0x08,
  _24Max_24 = 0x0A

}SampleLength24BitMax;
//-----------------------------------------------------------------------------
typedef enum
{
  _20Max_16 = 0x02,
  _20Max_17 = 0x0c,
  _20Max_18 = 0x04,
  _20Max_19 = 0x08,
  _20Max_20 = 0x0A

}SampleLength20BitMax;
//-----------------------------------------------------------------------------
typedef enum {
    LMax_Minus4 = 0x02,
    LMax_Minus3 = 0x0c,
    LMax_Minus2 = 0x04,
    LMax_Minus1 = 0x08,
    LMax_Minus0 = 0x0A

}SampleLength;

//-----------------------------------------------------------------------------
typedef enum {
    _20Bits,
    _24Bits
} MaxLength;

//-----------------------------------------------------------------------------
// These defines are used for Audio Path
//-----------------------------------------------------------------------------
enum SiI_AudioPath {
    SiI_SPDIF,
    SiI_I2S,
    SiI_DSD,
    SiI_HBAudio,
    SiI_AudioModesSelect = 0x0F,   // first four bits used for type of audio interface
    SiI_MultyChannel = 0x80
};

//-----------------------------------------------------------------------------
// These defines are used for Deep Color  (Video Path)
//-----------------------------------------------------------------------------
enum SiI_DeepColor {
    SiI_DeepColor_24bit = 0x00,
    SiI_DeepColor_30bit = 0x01,
    SiI_DeepColor_36bit = 0x02,
    SiI_DeepColor_Off   = 0xFF
};

enum SiI_AuxVidBusInfo {

    SiI_RisingVideoClockEdge = 0x01,
    SiI_ClippingVideo = 0x02,
    SiI_CopmpressingVideo = 0x04

};

enum SiI_SF {
    SiI_SF_32kHz = 0x01,
    SiI_SF_44kHz = 0x02,
    SiI_SF_48kHz = 0x03,
    SiI_SF_88kHz = 0x04,
    SiI_SF_96kHz = 0x05,
    SiI_SF_176kHz = 0x06,
    SiI_SF_192kHz = 0x07
};



