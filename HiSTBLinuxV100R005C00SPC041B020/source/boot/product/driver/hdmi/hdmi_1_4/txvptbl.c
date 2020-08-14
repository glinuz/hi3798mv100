/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "typedefs.h"
#include "txvptbl.h"
#include "siitxapidefs.h"

#define BYPASS 0x00
/* TODO */
/* Add code fo RBG to YCbCr Converter and Down Sampleing here */
/* tables to be modified                                      */
/* inRGB24[]                                                  */
/* inRGBDVO12[]                                               */
/* inYCbCr24[]                                                */

HI_U8 DefaultTXVPath[4] = {
                        // INPUT           OUTPUT
  //0x3, 0x00,          // YC 24      |    RGB24
    0x3, 0x01,          // YC 24      |    YCbCr24
    0x01,               // falling edge input
    SiI_DeepColor_Off   // Deep Color = off
};

ROM const HI_U8 inRGB24[] = {
      BYPASS        // RGB24 out
};

ROM const HI_U8 inRGBDVO12[] = {
      TX_DVO                        // RGB DVO 12 out
};

ROM const HI_U8 inYCbCr24[] = {
      TX_YCbCrToRGB | TX_Dither,   // RGB24 out
      BYPASS                       // YCbCr24 Out
};

ROM const HI_U8 inYC24[] = {
      TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_422to444 | TX_Dither,                 // YCbCr24 out
      BYPASS                                   // YC24 out
};

ROM const HI_U8 inYCMuxed12[] = {
      TX_DeMux | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_DeMux | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_DeMux                                            // YC24 out
};

ROM const HI_U8 inYCMuxed656_12[] = {
      TX_DeMux | TX_SyncExtr | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_DeMux | TX_SyncExtr | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_DeMux | TX_SyncExtr                                            // YC24 out
};

ROM const HI_U8 inYC656_24[] = {
      TX_SyncExtr | TX_422to444 | TX_YCbCrToRGB | TX_Dither, // RGB24 out
      TX_SyncExtr | TX_422to444 | TX_Dither,                 // YCbCr24 out
      TX_SyncExtr                                            // YC24 out
};

ROM const HI_U8 * const inMode[] = {
      inRGB24, inRGBDVO12, inYCbCr24, inYC24, inYCMuxed12, inYCMuxed656_12, inYC656_24
};

HI_U8 DefaultTXDVDAudio[4] = {
      0x00, // DVDA vs SPDIF    SiI_MultyChannel
      _48kHz, // Fs
      ((HI_U8)_24Bits |(HI_U8) LMax_Minus0),    // Sample Length
      0x40 // Control reg Page 1, 0x1D
};


