/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _GLOBDEFS_
#define _GLOBDEFS_

#define ON    1
#define OFF   0
#define MDDC_READ  1
#define MDDC_WRITE 0
#define WaitStartRX 0
#define ReadyRX 1
#define START_FRAME_RX 's'

#define SiI_ID_VER_MAJOR		(0x01)
#define SiI_ID_VER_RELEASE		(0x00)
#define SiI_ID_VER_BUILD		(0x33)
#define SiI_ID_API_VER_H		(0x00)
#define SiI_ID_API_VER_L		(0x00)
#define SiI_ID_SII_PART			(0x00)
#define SiI_ID_CP_BOARD			(0x00)
#define SiI_ID_EEPROM_VER		(0x0006)
#define SiI_ID_BUILD_FLAG		(0x00)

// I2C buses configuration
#define BUS_1 1
#define BUS_0 0
#define MDDC_BUS 2
#define EE_BUS BUS_1
#define OSCAR_EE_BUS BUS_0
#define DAC_BUS BUS_0


#define EE_SLVADDR 0xA8
#define DAC_SLVADDR 0x56
#define VITA_SLV 0x98
// PCB
//#define _MPCB_
#define _DVITX_HVRES_

#define CLR_MASK (BIT_INT_HOT_PLUG | BIT_BIPHASE_ERROR | BIT_DROP_SAMPLE | BIT_INT_RSEN)
//test #define CLR_MASK BIT_INT_RSEN // Only test RSEN interrupt signal

#define DVI_CLR_MASK  BIT_INT_HOT_PLUG

#endif

