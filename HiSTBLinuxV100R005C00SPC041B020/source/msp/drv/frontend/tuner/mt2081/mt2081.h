/*****************************************************************************
**
**  Name: mt2081.h
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
**  Name: mt2081.c
**
**  Description:    Microtune MT2081 Tuner software interface
**
**  Functions
**  Implemented:    mt_uint32_t  MT2081_Open
**                  mt_uint32_t  MT2081_Close
**                  mt_uint32_t  MT2081_RegisterSlaveTuner
**                  mt_uint32_t  MT2081_SetSROMode
**                  mt_uint32_t  MT2081_Identify
**                  mt_uint32_t  MT2081_GetReg
**                  mt_uint32_t  MT2081_TriggerRegDump
**                  mt_uint32_t  MT2081_GetRegDump
**                  mt_uint32_t  MT2081_GetLocked
**                  mt_uint32_t  MT2081_GetParam
**                  mt_uint32_t  MT2081_GetUserData
**                  mt_uint32_t  MT2081_ReInit
**                  mt_uint32_t  MT2081_SetParam
**                  mt_uint32_t  MT2081_SetReg
**                  mt_uint32_t  MT2081_Tune
**
**  References:     AN-00xxx: MT2081 Programming Procedures Application Note
**                  MicroTune, Inc.
**                  AN-00010: MicroTuner Serial Interface Application Note
**                  MicroTune, Inc.
**
**  Exports:        None
**
**  Dependencies:   MT_WriteRead(hUserData, IC_Addr, *pData_w, cnt_w, *pData_r, cnt_r);
**                  - Write + Read byte of data to/from the two-wire bus.
**
**                  MT_Sleep(hUserData, nMinDelayTime);
**                  - Delay execution for nMinDelayTime milliseconds
**
**  CVS ID:         $Id: mt2081.h,v 1.1 2010/09/06 09:12:56 software Exp $
**  CVS Source:     $Source: /home/cvs/software/tuners/MT2081/mt2081.h,v $
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
*****************************************************************************/

//#include "mt2081_userdef.h"
#if !defined( __MT2081_H )
#define __MT2081_H

#include "mt2081_userdef.h"

#ifndef MT2081_RESULT
#define MT2081_RESULT mt_uint32_t
#endif

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
** Chip revision
**
*/
typedef enum
{
    MT2081_XX = 0,
    MT2081_B0 = 208110
} MT2081_REVCODE;


/*
**  Parameter for selecting receiver mode
**  Set with e.g.:
**  status |= MT2081_SetParam(hMT2081,MT2081_RCVR_MODE, MT2081_CABLE_QAM);
*/
typedef enum
{
    MT2081_CABLE_QAM = 0,            /* Digital cable EU / Asia     */
    MT2081_CABLE_QAM_NA,             /* Digital cable North America */
    MT2081_NUM_RCVR_MODES
} MT2081_RCVR_MODES;

/*
**  Parameter for function MT2081_GetParam & MT2081_SetParam that
**  specifies the tuning algorithm parameter to be read/written.
**  For a detailed description and available parameters, please refer
**  to the AN-00xxx: MT2081 MT2084 Software API Manual
*/
typedef enum
{
    /*  version of the API, e.g. 10302 = 1.32                                 */
    MT2081_VERSION,

    /*  tuner address                                 !!!set by MT2081_Open() */
    MT2081_IC_ADDR,

    /*  tuner revision code  (see enum  MT2081_REVCODE for values)            */
    MT2081_IC_REV,

    /*  max number of MT2081 tuners     set by MT_TUNER_CNT in mt_userdef.h   */
    MT2081_MAX_OPEN,

    /*  current number of open MT2081 tuners           !!set by MT2081_Open() */
    MT2081_NUM_OPEN,

    /*  IC temperature readback           (steps are 0, 25, 50, 75, 100, 125) */
    MT2081_TEMPERATURE,

    /*  IC standby                           (1 for standby, 0 for wake-up) */
    MT2081_STANDBY,

    /*  crystal frequency                            !!(default: 25000000 Hz) */
    MT2081_SRO_FREQ,

    /*  input center frequency                         !!set by MT2081_Tune() */
    MT2081_INPUT_FREQ,

    /*  LO Frequency                                  !!set by MT2081_Tune()  */
    MT2081_LO_FREQ,

    /*  VCO Frequency divided by 4                   !! set by MT2081_Tune()  */
    MT2081_VCO_FREQ4,

    /*  output bandwidth                             !!  set by MT2081_Tune() */
    MT2081_OUTPUT_BW,

    /*  output freq                                                           */
    MT2081_OUTPUT_FREQ,

    /*  receiver mode                                                         */
    MT2081_RCVR_MODE,

    /*  RFLT Enable                                                           */
    MT2081_RFLT_ENABLE,

    /*  freeze AGC                                                            */
    MT2081_AGC_FREEZE,

    /*  AGC acquistion mode enable                                            */
    MT2081_AGC_ACQ,

    /*  System Clock frequency                                                */
    MT2081_F_SYSCLK,

    /*  ADC Clock frequency                                                   */
    MT2081_F_ADCCLK,

    /*  SA_PROFILE                                                            */
    MT2081_SA_PROFILE,

    /*  SPLT_ATN                                                              */
    MT2081_SPLT_ATN,

    /*  LNA_ATN                                                               */
    MT2081_LNA_ATN,

    /*  BBAGC_ATN                                                             */
    MT2081_BBAGC_ATN,

    /*  N1A Profile                                                           */
    MT2081_N1A_PROFILE,

    /*  N1A_PREAGC_LVL                                                        */
    MT2081_N1A_PREAGC_LVL,

    /* Received signal strength indication                                    */
    MT2081_RSSI,

    /* Received signal strength indication - measured value for correction    */
    MT2081_RSSI_MEASURED,

    /* VCO control: 0 - by IC (default), 1 - manual by software               */
    MT2081_VCO_MANUAL_CONTROL,

    MT2081_EOP                       /*  last entry in enumerated list        */

} MT2081_Param;


/* ====== Functions which are declared in mt2081.c File ======= */
/******************************************************************************
**
**  Name: MT2081_Open
**
**  Description:    Start up of the driver and initialization of the tuner.
**
**  Parameters:     MT2081_Addr  - Serial bus address of the tuner.
**                  hMT2081      - Tuner handle passed back.
**                  hUserData    - User-defined data, if needed for the
**                                 MT_WriteRead() function.
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_ARG_NULL       - Null pointer argument passed
**                      MT_COMM_ERR       - Serial bus communications error
**                      MT_TUNER_CNT_ERR  - Too many tuners open
**                      MT_TUNER_ID_ERR   - Tuner Part/Rev code mismatch
**                      MT_TUNER_INIT_ERR - Tuner initialization failed
**
**  Dependencies:   MT_WriteRead - Access to the  the two-wire bus
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
******************************************************************************/
MT2081_RESULT MT2081_Open( mt_uint32_t MT2081_Addr,
                          mt_handle_t* hMT2081,
                             mt_user_t hUserData );

/******************************************************************************
**
**  Name: MT2081_Close
**
**  Description:    Release the handle to the tuner.
**
**  Parameters:     h            - Handle to the MT2081/4 tuner
**
**  Returns:        status:
**                      MT_OK         - No errors
**                      MT_INV_HANDLE - Invalid tuner handle
**
**  Dependencies:   mt_errordef.h - definition of error codes
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
******************************************************************************/
MT2081_RESULT MT2081_Close(mt_handle_t h);

/******************************************************************************
**
**  Name: MT2081_Identify
**
**  Description:    Indentifies the Tuner
**
**
**  Dependencies:   MT_WriteRead - Access to the two-wire bus
**                  (MT2081_Open() is not needed to be called)
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
******************************************************************************/
MT2081_RESULT MT2081_Identify( mt_uint32_t MT2081_Addr,
                              mt_uint32_t* tuner_id,
                                 mt_user_t hUserData );

/****************************************************************************
**
**  Name: MT2081_GetReg
**
**  Description:    Gets an MT2081 register.
**
**                  Use this function if you need to read a register from
**                  the MT2081.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                  regaddr     - MT2081 register/subaddress location
**                  *val        - MT2081 register/subaddress value
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_ARG_NULL      - Null pointer argument passed
**                      MT_ARG_RANGE     - Argument out of range
**
**  Dependencies:   USERS MUST CALL MT2081_Open() FIRST!
**                  MT_WriteRead - Access to the  the two-wire bus
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetReg(mt_handle_t h,
                            mt_uint16_t regaddr,
                           mt_uint16_t* val);

/****************************************************************************
**
**  Name: MT2081_TriggerRegDump
**
**  Description:    Triggers a regdump
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_UPC_UNLOCK    - Upconverter PLL unlocked
**                      MT_DNC_UNLOCK    - Downconverter PLL unlocked
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_INV_HANDLE    - Invalid tuner handle
**
**  Dependencies:   MT_WriteRead - Access to the  the two-wire bus
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_TriggerRegDump(mt_handle_t h, mt_uint32_t* num_regs);

/****************************************************************************
**
**  Name: MT2081_GetRegDump
**
**  Description:    Get an element of the regdump
**                  (which needs before triggered by MT2081_TriggerRegDump)
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_UPC_UNLOCK    - Upconverter PLL unlocked
**                      MT_DNC_UNLOCK    - Downconverter PLL unlocked
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_INV_HANDLE    - Invalid tuner handle
**
**  Dependencies:   MT_WriteRead - Access to the  the two-wire bus
**                  MT_Sleep      - Delay execution for x milliseconds
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetRegDump(mt_handle_t h, mt_uint16_t reg_num,
                                mt_uint16_t* reg_addr, mt_uint16_t* reg_val);

/****************************************************************************
**
**  Name: MT2081_GetLocked
**
**  Description:    Checks to see if LO1 and LO2 are locked.
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_UPC_UNLOCK    - Upconverter PLL unlocked
**                      MT_DNC_UNLOCK    - Downconverter PLL unlocked
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_INV_HANDLE    - Invalid tuner handle
**
**  Dependencies:   MT_WriteRead - Access to the  the two-wire bus
**                  MT_Sleep      - Delay execution for x milliseconds
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetLocked( mt_handle_t h,
                               mt_uint32_t* islocked );

/****************************************************************************
**
**  Name: MT2081_GetParam
**
**  Description:    Gets a tuning algorithm parameter.
**
**                  This function provides access to the internals of the
**                  tuning algorithm - mostly for testing purposes.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                  param       - Tuning algorithm parameter
**                                (see enum MT2081_Param)
**                  pValue      - ptr to returned value
**
**                  param                     Description
**                  ----------------------    --------------------------------
**                  MT2081_VERSION            Version of the API code
**                  MT2081_IC_ADDR            Serial Bus address of this tuner
**                  MT2081_IC_REV             Tuner revision code
**                                            (see enum MT2081_REVCODE)
**                  MT2081_MAX_OPEN           Max # of MT2081's allowed open
**                  MT2081_NUM_OPEN           # of MT2081's open
**                  MT2081_SRO_FREQ           crystal frequency
**                  MT2081_INPUT_FREQ         input center frequency
**                  MT2081_STANDBY            set tuner in standby
**                  MT2081_LO_FREQ            LO Frequency
**                  MT2081_TEMPERATURE        IC temperature
**                  MT2081_VCO_FREQ4          VCO Frequency diveded by 4
**                  MT2081_OUTPUT_BW          Output bandwidth
**                  MT2081_OUTPUT_FREQ        Output frequency
**                  MT2081_AGC_FREEZE         Freeze Tuner AGC
**                  MT2081_AGC_ACQ            AGC acquisition mode
**                  MT2081_F_SYSCLK           System Clock frequency
**                  MT2081_F_ADCCLK           ADC Clock frequency
**                  MT2081_RFLT_ENABLE        RFLT enable/disable
**                  MT2081_SPLT_CONFIG        Splitter configuration
**                  MT2081_SPLT_ATN           Splitter attenuation
**                  MT2081_LNA_ATN            LNA attenuation
**                  MT2081_BBAGC_ATN          BBAGC attenuation
**                  MT2081_N1A_PROFILE        N1A Profile
**                  MT2081_N1A_PREAGC_LVL     Level before N1A (needed for
**                                            Powerreporting)
**                  MT2081_RSSI               Received signal strength indication
**                  MT2081_RSSI_MEASURED      RSSI - Measured value at 50dBuv
**
**  Usage:          status |= MT2081_GetParam(hMT2081,
**                                            MT2081_VERSION,
**                                            &versionid);
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_ARG_NULL      - Null pointer argument passed
**                      MT_ARG_RANGE     - Invalid parameter requested
**
**  Dependencies:   USERS MUST CALL MT2081_Open() FIRST!
**                  MT_WriteRead - Access to the  the two-wire bus
*
**  See Also:       MT2081_SetParam, MT2081_Open
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetParam( mt_handle_t h,
                              MT2081_Param param,
                              mt_uint32_t* pValue );

/****************************************************************************
**
**  Name: MT2081_GetUserData
**
**  Description:    Gets the user-defined data item.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_ARG_NULL      - Null pointer argument passed
**
**  Dependencies:   USERS MUST CALL MT2081_Open() FIRST!
**
**                  The hUserData parameter is a user-specific argument
**                  that is stored internally with the other tuner-
**                  specific information.
**
**                  For example, if additional arguments are needed
**                  for the user to identify the device communicating
**                  with the tuner, this argument can be used to supply
**                  the necessary information.
**
**                  The hUserData parameter is initialized in the tuner's
**                  Open function to NULL.
**
**  See Also:       MT2081_Open
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetUserData( mt_handle_t h,
                                   mt_user_t* hUserData );

/******************************************************************************
**
**  Name: MT2081_ReInit
**
**  Description:    Initialize and reset the tuner's register values.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_TUNER_ID_ERR   - Tuner Part/Rev code mismatch
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_COMM_ERR      - Serial bus communications error
**
**  Dependencies:   MT_WriteRead - Access to the  the two-wire bus
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
******************************************************************************/
MT2081_RESULT MT2081_ReInit(mt_handle_t h);

/****************************************************************************
**
**  Name: MT2081_SetParam
**
**  Description:    Sets a tuning algorithm parameter.
**
**                  This function provides access to the internals of the
**                  tuning algorithm.  You can override many of the tuning
**                  algorithm defaults using this function.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                  param       - Tuning algorithm parameter
**                                (see enum MT2081_Param)
**                  nValue      - value to be set
**
**                  param                     Description
**                  ----------------------    --------------------------------
**                  MT2081_SRO_FREQ           crystal frequency
**                  MT2081_OUTPUT_BW          output bandwidth
**                  MT2081_OUTPUT_FREQ        output bandwidth
**                  MT2081_RFLT_ENABLE        RFLT enable/disable
**                  MT2081_SPLT_CONFIG        splitter configuration
**                  MT2081_STANDBY            standby Tuner for powersave
**                  MT2081_AGC_FREEZE         Freeze Tuner AGC
**                  MT2081_AGC_ACQ            AGC acquistion mode
**                  MT2081_SPLT_ATN           SPLT attenuation
**                  MT2081_LNA_ATN            LNA attenuation
**                  MT2081_BBAGC_ATN          BBAGC attenuation
**                  MT2081_N1A_PROFILE        N1A profile
**                  MT2081_RSSI_MEASURED      RSSI - Measured value at 50dBuv
**
**
**  Usage:          status |= MT2081_SetParam(hMT2081,
**                                            MT2081_OUTPUT_BW,
**                                            8000000);
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_ARG_NULL      - Null pointer argument passed
**                      MT_ARG_RANGE     - Invalid parameter requested
**                                         or set value out of range
**                                         or non-writable parameter
**
**  Dependencies:   USERS MUST CALL MT2081_Open() FIRST!
**                  MT_WriteRead - Access to the  the two-wire bus
**
**  See Also:       MT2081_GetParam, MT2081_Open
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_SetParam( mt_handle_t h,
                              MT2081_Param param,
                               mt_uint32_t nValue );

/****************************************************************************
**
**  Name: MT2081_SetReg
**
**  Description:    Set a MT2081 register.
**
**                  Use this function if you need to override
**                  register value.
**
**                  ATTENTION: This could distrub the function of the API
**                             code or the tuner IC!
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                  regaddr     - MT2081 register/subaddress location
**                  val         - MT2081 register/subaddress value
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_ARG_RANGE     - Argument out of range
**
**  Dependencies:   USERS MUST CALL MT2081_Open() FIRST!
**                  MT_WriteRead - Access to the  the two-wire bus
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_SetReg( mt_handle_t h,
                             mt_uint16_t regaddr,
                             mt_uint16_t val );

/****************************************************************************
**
**  Name: MT2081_Tune
**
**  Description:    Change the tuner's tuned frequency to RFin.
**
**  Parameters:     h           - Open handle to the tuner (from MT2081_Open).
**                  f_in        - RF input center frequency (in Hz).
**
**  Returns:        status:
**                      MT_OK            - No errors
**                      MT_INV_HANDLE    - Invalid tuner handle
**                      MT_COMM_ERR      - Serial bus communications error
**                      MT_SPUR_PRESENT  - LO spur possible in output
**                      MT_FIN_RANGE     - Input freq out of range
**                      MT_FOUT_RANGE    - Output freq out of range
**
**  Dependencies:   MUST CALL MT2081_Open BEFORE MT2081_Tune!
**
**                  MT_WriteRead - Access to the  the two-wire bus
**                  MT_Sleep         - Delay execution for x milliseconds
**                  MT2081_GetLocked - Checks to see if LO1 and LO2 are locked
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_Tune(mt_handle_t h, mt_uint32_t f_in);

#if defined( __cplusplus )
}
#endif

#endif

