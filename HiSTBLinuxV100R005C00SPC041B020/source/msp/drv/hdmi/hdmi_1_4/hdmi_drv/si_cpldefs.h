/***********************************************************************************/
/*  Copyright (c) 2002-2007, Silicon Image, Inc.  All rights reserved.             */
/*  No part of this work may be reproduced, modifi d, distributed, transmitted,    */
/*  transcribed, or translated6into any language or computer format, in any form   */
/*  or by any means without written permission of: Silicon Image, Inc.,            */
/*  1060 East Arques Avenue, Sunnyvale, California 94085                           */
/***********************************************************************************/
#ifndef _CPLDEFS_
#define _CPLDEFS_

#undef  _DDCIIC_                        // No DDC bus connected to micro controller

#define _9032_

#ifdef _9032_

    ///    #define _9032_SHA_                  // If defined, use the HW SHA calculator

#if 0 /*-- s40v200 无向下兼容需求，去掉了一些版本兼容性宏 --*/
    #define _9032_AN_STOP_FIX_     //over     // If defined, no need to toggle Repeat bit to write AN_STOP to 0

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #define _9032_M0_     //over              // 9132 - If defined, then M0 must be made "external" before reading

										// 9034/9134 - THIS FLAG MUST BE DEFINED FOR 9034/9134 IN ORDER TO ALLOW 
										//             REPEATER AUTHENTICATION  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    #define _9032_AUTO_RI_    //over          // If defined, turn on Auto Ri Checking
    #define _9032_BCAP_    //over             // If defined, use the Auto BCAP interrupt

    #define _9032_WRITEBLOCKP0_   //over      // Must be turned on for either _9032_AUTO_RI_ OR _9032_BCAP_
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

#endif

#define _9132_
#ifdef _9132_

///    #define _9132_DIAG_PD_DEFAULT_      // Default for this register is different from other TX's - #define for Fred and for Fred/Dino FPGAs. Undefine for Dino 

#if 0 /*-- s40v200 无向下兼容需求，去掉了一些版本兼容性宏 --*/
    #define _9132_SPDIF_DECODER_  //over      // SPDIF decoder is on this board
    #define _9132_SACD_  //over               // Support for SACD
    #define _9132_USE_SOURCE_TERMINATION_  //over  // source termination enabled
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

#endif

/// //#define _FPGA_FRED2_                 // fRED 2 FPGA specifics

///#define _FPGA_DINO_                     // Dino specifics
/// //#define _DINO_ON_FRED_BOARD_

#endif

