/*****************************************************************************
**
**  Name: mt_errordef.h
**
**  Copyright 2006-2010 Microtune, Inc. All Rights Reserved
**
**  This source code file contains confidential information and/or trade
**  secrets of Microtune, Inc. or its affiliates and is subject to the
**  terms of your confidentiality agreement with Microtune, Inc. or one of
**  its affiliates, as applicable and the terms of the end-user software
**  license agreement (EULA).
**
**  Protected by US Patent 7,035,614 and additional patents pending or filed.
**
**  BY INSTALLING, COPYING, OR USING THE MICROTUNE SOFTWARE, YOU AGREE TO BE
**  BOUND BY ALL OF THE TERMS AND CONDITIONS OF THE MICROTUNE, INC. END-USER
**  SOFTWARE LICENSE AGREEMENT (EULA), INCLUDING BUT NOT LIMITED TO, THE
**  CONFIDENTIALITY OBLIGATIONS DESCRIBED THEREIN. IF YOU DO NOT AGREE, DO NOT
**  INSTALL, COPY, OR USE THE MICROTUNE SOFTWARE AND IMMEDIATELY RETURN OR
**  DESTROY ANY COPIES OF MICROTUNE SOFTWARE OR CONFIDENTIAL INFORMATION THAT
**  YOU HAVE IN YOUR POSSESSION.
**
*****************************************************************************
**
**  MICROTUNE, INC.
**  END-USER SOFTWARE LICENSE AGREEMENT
**
**  1. LICENSE GRANT.
**
**     Microtune, Inc., a Delaware corporation, including its subsidiaries
**     ("Licensor"), grants to Licensee a non-exclusive, non-transferable,
**     revocable license to use the Licensed Software (as defined below) for
**     use with Licensor products only (the "License"). The License and the
**     License Agreement (as defined below) shall also apply to any use,
**     import, export, market, offer for sale, sale or other distribution of
**     the Licensed Software by customers of the Licensee or the common
**     customers of Licensor and Licensee; provided, that such use, import,
**     export, market, offer for sale, sale or other distribution of the
**     Licensed Software is solely pursuant to the incorporation of
**     Licensor's products in the end products of the customer, and;
**     provided, further that such customer shall be considered a "Licensee"
**     for all purposes hereunder.
**
**     "Licensed Software" shall mean source code, the algorithms implemented
**     by the source code, and any derivative forms of that source code,
**     including, but not limited to, intermediate object forms and final
**     machine executable forms of that source code.
**
**  2. PROPRIETARY RIGHTS.
**
**     This License is not a sale.  Title and all intellectual property
**     rights to the Licensed Software and any accompanying documentation
**     shall remain with Licensor.
**
**  3. LIMITED WARRANTY AND DISCLAIMER.
**
**     THE LICENSED SOFTWARE DOES NOT CONTAIN ANY OPEN SOURCE SOFTWARE.
**
**     THE LICENSED SOFTWARE IS LICENSED TO LICENSEE "AS IS" AND LICENSOR
**     MAKES AND THE LICENSEE RECEIVES NO WARRANTIES OR CONDITIONS, EXPRESS,
**     IMPLIED, STATUTORY, OR OTHERWISE, AND LICENSOR SPECIFICALLY DISCLAIMS
**     ANY IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS
**     FOR A PARTICULAR PURPOSE.  LICENSOR DOES NOT WARRANT THAT THE LICENSED
**     SOFTWARE WILL MEET LICENSEE'S REQUIREMENTS OR THAT THE OPERATION OF
**     THE LICENSED SOFTWARE WILL BE UNINTERRUPTED OR ERROR FREE.
**
**     SOME STATES DO NOT ALLOW THE EXCLUSION OF IMPLIED WARRANTIES SO THE
**     ABOVE EXCLUSIONS MAY NOT APPLY TO LICENSEE.  THIS WARRANTY GIVES
**     LICENSEE SPECIFIC LEGAL RIGHTS.  LICENSEE MAY ALSO HAVE OTHER RIGHTS,
**     WHICH VARY, FROM STATE TO STATE.
**
**  4. LIMITATION OF LIABILITY.
**
**     IN NO EVENT SHALL LICENSOR BE LIABLE FOR ANY SPECIAL, INDIRECT,
**     CONSEQUENTIAL OR INCIDENTAL DAMAGES, INCLUDING, WITHOUT LIMITATION,
**     COSTS OF PROCUREMENT OF SUBSTITUTE PRODUCTS OR SERVICES OR LOST
**     PROFITS, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, ARISING IN
**     ANY WAY OUT OF THIS AGREEMENT.
**
**     IN NO EVENT SHALL LICENSOR'S LIABILITY FOR DAMAGES UNDER THIS
**     AGREEMENT EXCEED THE LESSER OF (A) THE AMOUNT PAID BY LICENSEE TO
**     LICENSOR FOR LICENSOR PRODUCTS IN THE PREVIOUS TWELVE MONTHS; OR
**     (B) $100,000.
**
**  5. EXPORT RESTRICTION.
**
**     Licensee agrees not to export or re-export the Licensed Software in
**     any form without the appropriate United States and foreign government
**     licenses.  Licensee's failure to comply with this provision is a
**     material breach of the License Agreement.
**
**  6. RESTRICTIONS ON USE
**
**     Use of the Licensed Software with any non-Licensor products or any
**     other products or devices is strictly prohibited.  Use of the Licensed
**     Software with any medical devices is also prohibited. Licensee may not
**     rent, lease or grant sublicenses or other rights to the Licensed
**     Software.  Licensee may not remove any proprietary notices, labels or
**     marks incorporated in, marked on, or fixed to the Licensed Software by
**     Licensor or its suppliers.  Any use or attempted use of the Licensed
**     Software in violation of the restrictions contained in this end-user
**     software license agreement (the "License Agreement") is a breach of
**     the License Agreement and will cause irreparable harm to Licensor
**     entitling Licensor to injunctive relief in addition to all available
**     legal remedies.
**
**     Licensee agrees that to the extent that Licensee provide the Licensed
**     Software to its customers in a form that can be modified by its
**     customers, Licensor will provide such Licensed Software subject to the
**     license requirements contained in this License Agreement.
**
**     Licensee agrees and warrants that it will not remove or alter all
**     markings and copyright notices accompanying the Licensed Software in
**     whatever form they appear.
**
**     Licensee agrees and warrant that, without Licensor's prior written
**     consent, Licensor will not use or combine the Licensed Software with
**     other software subject to an open source license agreement, or
**     otherwise take any action that would alter Licensor's rights in the
**     Licensed Software.
**
**     Licensee agrees not to reverse engineer, de-compile, or disassemble
**     the Licensed Software and any related confidential information.
**     Licensee further agrees not to publish comparison or benchmarking
**     data gathered from the use of the Licensed Software. The Licensed
**     Software and any related confidential information are and at all times
**     will remain the property of Licensor.  No use of the Licensed Software
**     and any related confidential information is permitted except as
**     expressly provided herein, and no grant under any proprietary rights
**     is hereby given or intended, including any license implied or otherwise,
**     except as explicitly described in Section 1 above.  Upon receipt of
**     Licensor's written request, Licensee will return the Licensed Software
**     and any related confidential information, along with all copies and
**     portions thereof.
**
**  7. CHOICE OF LAW.
**
**     This License Agreement and any issues or disputes arising from or
**     relating in any way to the License Agreement or its negotiation,
**     formation, subject, validity, interpretation, performance or enforcement
**     shall be solely and exclusively governed by and enforced in accordance
**     with the laws of the State of Texas, without giving effect to the
**     conflicts of laws provisions thereof.  Provided that a basis for federal
**     subject matter jurisdiction exists, any claim, action or proceeding
**     arising from or relating in any way to this License Agreement or its
**     negotiation, formation, subject, validity, interpretation, performance
**     or enforcement shall be brought solely and exclusively in federal
**     district court in the Eastern District of Texas, Sherman Division, and
**     the parties consent to that exclusive and mandatory venue and to
**     personal jurisdiction therein and further agree that this provision
**     shall provide the basis for transfer or dismissal of any such claim,
**     action or proceeding brought in any other jurisdiction or venue.
**     Alternatively, in the absence of a basis for federal subject matter
**     jurisdiction, any claim, action or proceeding described in this
**     paragraph shall be brought solely and exclusively in the state courts of
**     Texas located in Collin County, Texas, and the parties consent to that
**     exclusive and mandatory venue and to personal jurisdiction therein and
**     further agree that this provision shall provide the basis for transfer
**     or dismissal of any such claim, action or proceeding brought in any
**     other jurisdiction or venue.   Company waives all defenses of lack of
**     personal jurisdiction and forum non conveniens.
**
*****************************************************************************/
/*****************************************************************************
**
**  Name: mt_errordef.h
**
**  Description:    Definition of bits in status/error word used by various
**                  MicroTuner control programs.
**
**  References:     None
**
**  Exports:        None
**
**  CVS ID:         $Id: mt_errordef.h,v 1.2 2010/09/06 10:10:22 software Exp $
**  CVS Source:     $Source: /home/cvs/software/tuners/MT2081/mt_errordef.h,v $
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   09-09-2004    JWS    Original
**   088   01-26-2005    DAD    Added MT_TUNER_INIT_ERR.
**   N/A   12-09-2005    DAD    Added MT_TUNER_TIMEOUT (info).
**   N/A   10-17-2006    JWS    Updated copyright notice.
**   N/A L 02-25-2008    RSK    Correcting Comment Block to match constants.
**   N/A I 03-02-2010    JWS    Clean up tabs and blanks at end of lines.
**
*****************************************************************************/

/*
** Note to users:  DO NOT EDIT THIS FILE
**
** If you wish to rename any of the "user defined" bits,
** it should be done in the user file that includes this
** source file (e.g. mt_userdef.h)
**
*/



/*
**  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
**  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
**  M U <- Info Codes --> <# Spurs> < User> <----- Err Codes ----->
**
**  31 = MT_ERROR - Master Error Flag.  If set, check Err Codes for reason.
**  30 = MT_USER_ERROR - User-declared error flag.
**  29 = Unused
**  28 = Unused
**
**  27 = MT_DNC_RANGE
**  26 = MT_UPC_RANGE
**  25 = MT_FOUT_RANGE
**  24 = MT_FIN_OUT_OF_RANGE
**
**  23 = MT_SPUR_PRESENT - Unavoidable spur in output
**  22 = MT_TUNER_TIMEOUT
**  21 = Unused
**  20 = MT_SPUR_CNT_MASK (MSB) - Count of avoided spurs
**
**  19 = MT_SPUR_CNT_MASK
**  18 = MT_SPUR_CNT_MASK
**  17 = MT_SPUR_CNT_MASK
**  16 = MT_SPUR_CNT_MASK (LSB)
**
**  15 = MT_USER_DEFINED4 - User-definable bit (see MT_Userdef.h)
**  14 = MT_USER_DEFINED3 - User-definable bit (see MT_Userdef.h)
**  13 = MT_USER_DEFINED2 - User-definable bit (see MT_Userdef.h)
**  12 = MT_USER_DEFINED1 - User-definable bit (see MT_Userdef.h)
**
**  11 = Unused
**  10 = Unused
**   9 = MT_TUNER_INIT_ERR - Tuner initialization error
**   8 = MT_TUNER_ID_ERR - Tuner Part Code / Rev Code mismatches expected value
**
**   7 = MT_TUNER_CNT_ERR - Attempt to open more than MT_TUNER_CNT tuners
**   6 = MT_ARG_NULL - Null pointer passed as argument
**   5 = MT_ARG_RANGE - Argument value out of range
**   4 = MT_INV_HANDLE - Tuner handle is invalid
**
**   3 = MT_COMM_ERR - Serial bus communications error
**   2 = MT_DNC_UNLOCK - Downconverter PLL is unlocked
**   1 = MT_UPC_UNLOCK - Upconverter PLL is unlocked
**   0 = MT_UNKNOWN - Unknown error
*/
#define MT_ERROR (1 << 31)
#define MT_USER_ERROR (1 << 30)

/*  Macro to be used to check for errors  */
#define MT_IS_ERROR(s) (((s) >> 30) != 0)
#define MT_NO_ERROR(s) (((s) >> 30) == 0)


#define MT_OK                           (0x00000000)

/*  Unknown error  */
#define MT_UNKNOWN                      (0x80000001)

/*  Error:  Upconverter PLL is not locked  */
#define MT_UPC_UNLOCK                   (0x80000002)

/*  Error:  Downconverter PLL is not locked  */
#define MT_DNC_UNLOCK                   (0x80000004)

/*  Error:  Two-wire serial bus communications error  */
#define MT_COMM_ERR                     (0x80000008)

/*  Error:  Tuner handle passed to function was invalid  */
#define MT_INV_HANDLE                   (0x80000010)

/*  Error:  Function argument is invalid (out of range)  */
#define MT_ARG_RANGE                    (0x80000020)

/*  Error:  Function argument (ptr to return value) was NULL  */
#define MT_ARG_NULL                     (0x80000040)

/*  Error: Attempt to open more than MT_TUNER_CNT tuners  */
#define MT_TUNER_CNT_ERR                (0x80000080)

/*  Error: Tuner Part Code / Rev Code mismatches expected value  */
#define MT_TUNER_ID_ERR                 (0x80000100)

/*  Error: Tuner Initialization failure  */
#define MT_TUNER_INIT_ERR               (0x80000200)

/*  User-definable fields (see mt_userdef.h)  */
#define MT_USER_DEFINED1                (0x00001000)
#define MT_USER_DEFINED2                (0x00002000)
#define MT_USER_DEFINED3                (0x00004000)
#define MT_USER_DEFINED4                (0x00008000)
#define MT_USER_MASK                    (0x4000f000)
#define MT_USER_SHIFT                   (12)

/*  Info: Mask of bits used for # of LO-related spurs that were avoided during tuning  */
#define MT_SPUR_CNT_MASK                (0x001f0000)
#define MT_SPUR_SHIFT                   (16)

/*  Info: Tuner timeout waiting for condition  */
#define MT_TUNER_TIMEOUT                (0x00400000)

/*  Info: Unavoidable LO-related spur may be present in the output  */
#define MT_SPUR_PRESENT                 (0x00800000)

/*  Info: Tuner input frequency is out of range */
#define MT_FIN_RANGE                    (0x01000000)

/*  Info: Tuner output frequency is out of range */
#define MT_FOUT_RANGE                   (0x02000000)

/*  Info: Upconverter frequency is out of range (may be reason for MT_UPC_UNLOCK) */
#define MT_UPC_RANGE                    (0x04000000)

/*  Info: Downconverter frequency is out of range (may be reason for MT_DPC_UNLOCK) */
#define MT_DNC_RANGE                    (0x08000000)

