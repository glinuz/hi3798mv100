/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _TYPEDEFS_
#define _TYPEDEFS_

#include "hi_type.h"      //for printk and putchar
//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "si_cpldefs.h"
#include "drv_global.h"

typedef unsigned int Bool, *pBool;     

#define FALSE  0
#define TRUE  1

#define ROM 
#define IRAM

#define INT_CONTROL	0x02		// Interrupt pin is push-pull and active high (this is normally 0x06)
#define OSC 11059200			// Oscillator frequency of 11.0592 MHz
#define REF_FREQ 92160000       // OSC/12 * 10 (mul by 10 to incr. resolution of VSync )

#define HDMI_PRINT HI_PRINT
//HI_U32 HDMI_DEBUG(const char *format, ...);
#define HDMI_MALLOC malloc

#define DisplEncryptionOn()     //led on 
#define DisplHDMIOn()
#define DisplEncryptionOff() 
#define DisplHDMIOff()
#define ReadTXInt_Pin() 1       //tx interrupt pin

#define SysTimerInterrupts(byteOn)
#define  GetMCUClocksPerVSyncPeriod(void) 1


extern void SI_DumpMem(char *szFun, HI_U8 NBytes, HI_U8 * Data);
#define HDMI_DumpMem(a, b) SI_DumpMem(__FUNCTION__, a, b);

#define hdmi_usleep(x) do { \
    volatile int i = 0;\
    for (i = 0; i< x; i++)\
    {   ;    }\
    }while(0)

//CmdType * CmdPtr;
//sthdmi_ioctl_ioctl


#endif

