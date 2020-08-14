/***********************************************************************************/
/*  Copyright (c) 2002-2006, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modified, distributed, transmitted,    */
/*  transcribed, or translated into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#include "si_infpkttb.h" /* info_pkt_table */

//-----------------------------------------------------------
// Audio contain protecton packet
HI_U8 AudioCP[31] = {
        0x04,
        0x00, // status
        0x00, // reserved
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,  // data
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 // reserved data
};

//-----------------------------------------------------------
HI_U8 ISRC1[31] = {
        0x05,
        0x00, // status
        0x00, // reserved
        1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,  // data
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 // reserved data
};

HI_U8 ISRC2[31] = {
        0x06,
        0x00, // reserved
        0x00, // reserved
        2, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,  // data
        16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 // reserved data
};

