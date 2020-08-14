/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _EEDEFS_
#define _EEDEFS_

#define VER_ID_ADDR 0x00
#define VIDEO_CFG_ADDR 0x10
#define PEDESTAL_PROP_ADDR 0x20
#define HDMICAP_ADDR 0x30

#define EE_AVIINFO_ENABLE 0X40
#define EE_SPDINFO_ENABLE 0X41
#define EE_AUDINFO_ENABLE 0X42

#define CFG_ADDR 0xFF

#define EE_EDID_PARSER_ADDR 0x8F
#define EE_TX_VIDEO_MODE_INDEX_ADDR 0x90
#define EE_TX_DE_ENABLED_ADDR 0x91
#define EE_TX_ICLK_ADDR       0x92
#define EE_TX_MCLK_INDEX_ADDR 0x93
#define EE_TX_INTERNAL_MCLK_ADDR 0x94 // s40 新特性 内部mclk
#define EE_TX_VIDEO_MODE_ADDR 0xA0
#define EE_TX_FPGA 0x95
#define EE_TX_HDCP 0x96

#define EE_TX_SYS_CTRL1_ADDR 0xA2
#define EE_FREQ_SVAL_ADDR 0xB0
#define EE_TX_VIDEOPATH_ADDR 0xC0
#define EE_TX_AUDIOPATH_ADDR 0x98

#define EE_TXAVIINFO_ADDR 0xD0
#define EE_CUSTVMODE 0xE0
#define CUST_MODE_SIZE 10
#define FPIX_OFFSET 1
#define HRES_OFFSET 3
#define VRES_OFFSET 5
#define HSTART_OFFSET 7
#define VSTART_OFFSET 9
#define COMM_MODE_ONLY 0x01

#define EE_TXAUDIOINFO_ADDR 0xF0

#define EE_SINK_CAP_ADDR 0X110


#define EE_EDID_DATA  0x200   // size 512Byte  0x200 - 0x3ff

#define EE_MAX_SIZE 0X1000


#endif

