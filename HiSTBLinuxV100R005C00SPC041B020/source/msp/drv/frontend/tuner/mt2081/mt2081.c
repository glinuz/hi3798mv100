/*****************************************************************************
**
**  Name: mt2081.c
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
**  References:     AN-00xxx: MT2081 Software API Manual
**                  MicroTune, Inc.
**                  AN-00010: MicroTuner Serial Interface Application Note
**                  MicroTune, Inc.
**
**  Exports:        None
**
**  Dependencies:   MT_WriteRead(hUserData, IC_Addr, *pData_w, cnt_w, *pData_r, cnt_r);
**                  - Write + Read byte of data to/from the two-wire bus.
**
**                  MT2081_Sleep(hUserData, nMinDelayTime);
**                  - Delay execution for nMinDelayTime milliseconds
**
**  CVS ID:         $Id: mt2081.c,v 1.2 2010/09/06 10:28:29 software Exp $
**  CVS Source:     $Source: /home/cvs/software/tuners/MT2081/mt2081.c,v $
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   09-02-2010   PINZ    Ver 1.31: Initial.
*****************************************************************************/

#include "mt2081.h"
#include "mt2081_data.h"


#if defined(MT2081_INCLUDE_POWER_REPORTING)
#include "mt2081_pwrrpt.h"
#endif

/*  Version of this module                          */
#define VERSION 10031             /*  Version 01.31 */

#ifndef MT2081_CNT
#error You must define MT2081_CNT in the "mt2081_userdef.h" file
#endif

/*
** Constants values used over various functions
*/
#define REF_FREQ          (25000000)  /* Reference oscillator Frequency (in Hz) */
#define MIN_FIN_FREQ      (42000000)  /* Minimum input frequency: 42MHz (in Hz)  */
#define MAX_FIN_FREQ    (1002000000)  /* Maximum input frequency: 1.002GHz (in Hz)  */
#define VHF_FIN_FREQ     (350000000)  /* Maximum VHF frequeny: (in Hz) */
#define MIN_FOUT_FREQ      (3000000)  /* Minimum output frequency: 3MHz (in Hz) */
#define MAX_FOUT_FREQ     (60000000)  /* Maximum output frequency: 60MHz (in Hz)*/
#define F_ADCCLK_CNT             (8)  /* Maximum number of usable adcclk values */
#define F_SYSCLK_CNT             (5)  /* Maximum number of usable sysclk values */
#define MAX_HARMONIC            (25)  /* Number of harmonics to check for spurs */
#define MAX_F_SYSCLK     (263000000)  /* Maximum Sysclk (in Hz) */
#define MIN_F_SYSCLK     (211760000)  /* Minimum Sysclk (in Hz) */
#define MAX_F_ADCCLK      (60000000)  /* Maximum ADCclk (in Hz) */
#define MIN_F_ADCCLK      (44000000)  /* Minimum ADCclk (in Hz) */
#define GAP_F_ADCCLK        (250000)  /* ADCCLK keepout area (in Hz -> 2*f_ref +/- GAP_F_ADCCLK) */
#define FRACN_AVOID_MODULUS   (2048)  /* Modulus of FracN avoidance areas */


/*
** Macros for determing the IC type
*/
#define IS_MT2081(mInfo)   ( ((mInfo)->tuner_id == MT2081_B0) )
#define IS_MT208XB0(mInfo) ( ((mInfo)->tuner_id == MT2081_B0) )

/*
**  Define the number of regs for the MT2081
*/
#define NUM_REGS        (MT2081_REG__MAX)

typedef struct
{
    mt_handle_t     handle;
    mt_user_t       hUserData;
    mt_uint32_t     address;
    mt_uint32_t     version;
    mt_uint32_t     tuner_id;
    mt_uint32_t     f_ref;
    mt_uint32_t     f_in;
    mt_uint32_t     f_lo;
    mt_uint32_t     f_vco4;
    mt_uint8_t      vco_div;
    mt_uint8_t      f_adcclk_idx;
    mt_uint32_t     f_adcclk[F_ADCCLK_CNT];
    mt_uint16_t     adcclkdiv[F_ADCCLK_CNT];
    mt_uint8_t      f_sysclk_idx;
    mt_uint32_t     f_sysclk[F_SYSCLK_CNT];
    mt_uint16_t     sysclkdiv[F_SYSCLK_CNT];
    mt_uint8_t      adc_sysclk_idx[F_ADCCLK_CNT]; /* Which f_sysclk_idx fits to f_adcclk_idx[n] */
    mt_uint32_t     f_out_bw;
    mt_uint32_t     f_out;
    mt_uint32_t     rcvr_mode;
    mt_uint32_t     splt_config;
    mt_uint32_t     agc_freeze;
    mt_uint32_t     is_standby;
    mt_uint32_t     val_rc_cal;
    mt_uint32_t     n1a_profile;
    mt_int32_t      rssi_delta;
    mt_uint32_t     fracn_avoid_zone;
    mt_uint32_t     sa_profile;
    mt_uint32_t     vco_profile;
    mt_uint32_t     num_regs;
#if defined(MT2081_INCLUDE_POWER_REPORTING)
    MT2081_GainCal_t  gaincal_table;
#endif
    mt_uint16_t     reg[NUM_REGS];
}  MT2081_Info_t;

/* Generic definition for lookup tables */
struct lookuptable {
    mt_uint32_t num;
    mt_uint32_t result;
};

static mt_uint8_t nMaxTuners = MT2081_CNT;
static MT2081_Info_t MT2081_Info[MT2081_CNT];
static MT2081_Info_t *Avail[MT2081_CNT];
static mt_uint8_t nOpenTuners = 0;

/*  Forward declaration(s) of internal functions:  */
static mt_uint32_t mt2081_read(MT2081_Info_t *pInfo, mt_uint16_t field, mt_uint16_t field_addr, mt_uint16_t field_mask, mt_uint16_t field_shift, mt_uint16_t *var, mt_uint8_t do_cache);
static mt_uint32_t mt2081_write(MT2081_Info_t *pInfo, mt_uint16_t field, mt_uint16_t field_addr, mt_uint16_t field_mask, mt_uint16_t field_shift, mt_uint16_t var, mt_uint8_t cache_flag);
static mt_uint32_t mt2081_readblock(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, mt_uint16_t* reg_data, mt_uint32_t length);
static mt_uint32_t mt2081_writeblock(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, const mt_uint16_t* reg_data, mt_uint32_t length);
static mt_uint32_t mt2081_regaddr2num(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, mt_uint16_t* reg_num);
static mt_uint32_t mt2081_regnum2addr(MT2081_Info_t *pInfo, mt_uint16_t reg_num, mt_uint16_t* reg_addr);
static mt_uint32_t mt2081_update_reg_cache(MT2081_Info_t *pInfo);
static mt_uint32_t mt2081_initialize(mt_handle_t hMT2081);
static mt_uint32_t FindClkSrc(MT2081_Info_t *pInfo);
static mt_uint32_t mt2081_internal_standby(MT2081_Info_t *pInfo, mt_uint32_t standby);
static mt_uint32_t mt2081_splt_standby(MT2081_Info_t *pInfo, mt_uint32_t standby);
static mt_uint32_t mt2081_standby(MT2081_Info_t *pInfo, mt_uint32_t standby);
static mt_uint32_t mt2081_iqe_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze);
static mt_uint32_t mt2081_bbagc_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze);
static mt_uint32_t mt2081_rfagc_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze);
static mt_uint32_t mt2081_update_chf(MT2081_Info_t *pInfo);
static mt_uint32_t mt2081_set_fout_bw(MT2081_Info_t *pInfo, mt_uint32_t f_out_bw);
static mt_uint32_t mt2081_set_fout(MT2081_Info_t *pInfo, mt_uint32_t f_out);
static mt_uint32_t CalcLOMult(mt_uint16_t *lo_div_ret, mt_uint16_t *lo_fracn_ret, mt_uint32_t  f_LO, mt_uint32_t  f_Ref, mt_uint32_t lo_denom, mt_uint32_t fracn_avoid_zone);
static mt_uint32_t fLO_FractionalTerm(mt_uint32_t f_ref, mt_uint32_t num, mt_uint32_t denom);
static mt_uint32_t mt2081_set_lnacfg(MT2081_Info_t *pInfo, mt_uint32_t f_in);
static mt_uint32_t mt2081_set_lofreq(MT2081_Info_t *pInfo, mt_uint32_t f_lo, mt_uint32_t *islocked);
static mt_uint32_t mt2081_enable_dfe(MT2081_Info_t *pInfo, mt_uint32_t enable);
static mt_uint32_t mt2081_n1a_preagc_lvl(MT2081_Info_t *pInfo, mt_uint32_t *lvl);
static mt_uint32_t mt2081_get_rssi(MT2081_Info_t *pInfo, mt_uint32_t *rssi);



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

MT2081_RESULT MT2081_Open(mt_uint32_t MT2081_Addr,
                         mt_handle_t* hMT2081,
                            mt_user_t hUserData)
{
    mt_uint32_t status = MT_OK; /*  Status to be returned.  */
    mt_int32_t i;
    MT2081_Info_t* pInfo = NULL;

    /*  Check the argument before using  */
    if (hMT2081 == NULL)
        return MT_ARG_NULL;

    /*  Default tuner handle to NULL.  If successful, it will be reassigned  */
    *hMT2081 = NULL;

    /*
    **  If this is our first tuner, initialize the address fields and
    **  the list of available control blocks.
    */
    if (nOpenTuners == 0)
    {
        for (i = MT2081_CNT - 1; i >= 0; i--)
        {
            MT2081_Info[i].handle = NULL;
            MT2081_Info[i].address = (mt_uint32_t)MAX_UDATA;
            MT2081_Info[i].hUserData = NULL;
            Avail[i] = &MT2081_Info[i];
        }
    }

    /*
    **  Look for an existing MT2081_State_t entry with this address.
    */
    for (i = MT2081_CNT - 1; i >= 0; i--)
    {
        /*
        **  If an open'ed handle provided, we'll re-initialize that structure.
        **
        **  We recognize an open tuner because the address and hUserData are
        **  the same as one that has already been opened
        */
        if ((MT2081_Info[i].address == MT2081_Addr) && (MT2081_Info[i].hUserData == hUserData))
        {
            pInfo = &MT2081_Info[i];
            break;
        }
    }

    /*  If not found, choose an empty spot.  */
    if (pInfo == NULL)
    {
        /*  Check to see that we're not over-allocating  */
        if (nOpenTuners >= MT2081_CNT)
            return MT_TUNER_CNT_ERR;

        /* Use the next available block from the list */
        pInfo = Avail[nOpenTuners];
        nOpenTuners++;
    }

    if (MT_NO_ERROR(status))
    {
        pInfo->handle = (mt_handle_t)pInfo;
        pInfo->hUserData = hUserData;
        pInfo->address = MT2081_Addr;
        status |= MT2081_Identify(pInfo->address, &pInfo->tuner_id, pInfo->hUserData);
    }

    if (MT_NO_ERROR(status))
    {
        status |= MT2081_ReInit((mt_handle_t)pInfo);
    }

    if (MT_IS_ERROR(status))
        /*  MT2081_Close handles the un-registration of the tuner  */
        (void) MT2081_Close((mt_handle_t)pInfo);
    else
        *hMT2081 = pInfo->handle;

    return status;
}


static mt_uint32_t IsValidHandle(MT2081_Info_t* handle)
{
    return (mt_uint32_t)(((handle != NULL) && (handle->handle == handle)) ? 1 : 0);
}

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
MT2081_RESULT MT2081_Close(mt_handle_t h)
{
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    mt_uint32_t status = MT_OK; /*  Status to be returned.  */

    if (!IsValidHandle(pInfo))
        status |= MT_INV_HANDLE;

    /* Remove the tuner from our internal list of tuners */
    pInfo->handle = NULL;
    pInfo->address = (mt_uint32_t)MAX_UDATA;
    pInfo->hUserData = NULL;
    nOpenTuners--;
    /* Return control block to available list */
    Avail[nOpenTuners] = pInfo;

    return status;
}

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
MT2081_RESULT MT2081_Identify(mt_uint32_t MT2081_Addr,
                             mt_uint32_t* tuner_id,
                                mt_user_t hUserData)
{
    mt_uint32_t status = MT_OK; /* Status to be returned */
    mt_uint8_t buf[4] = { 0x00, 0x00, 0x00, 0x00 };

    /* Test connector for pre-release testing of the API code, do not change */
    #ifndef MT2081_IDX
      #define MT2081_IDX MT2081_XX
    #endif
    #ifndef MT2081_STATUSX
      #define MT2081_STATUSX MT_TUNER_ID_ERR
    #endif

    if (tuner_id == NULL)
        status |= MT_ARG_NULL;

    /*  Read the Part/Rev code from the tuner */
    if (MT_NO_ERROR(status))
	
		status |= MT_WriteRead(hUserData, MT2081_Addr, &buf[0], 2, &buf[2], 2);					

    /* Check the Part/Rev code */
    if (MT_NO_ERROR(status))
    {
             if (buf[3] == 0x62 && buf[2] == 0xe9) /*  MT2081 B0  */
            *tuner_id = MT2081_B0;
        else
        {
            /*  Unknown tuner Part/Rev code */
            *tuner_id = MT2081_IDX;
            status |= MT2081_STATUSX;
        }
    }

    return status;
}


/******************************************************************************
**
** Beginning of local functions:
**
** Serial bus communication and register handling
**
******************************************************************************/

/******************************************************************************
**
**  Name: mt2081_regaddr2num
**
**  Description:    Search the cache position for a register address (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_regaddr2num(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, mt_uint16_t* reg_num)
{
    mt_uint32_t status = MT_OK;

    mt_uint16_t num_blocks;
    mt_uint16_t i;
    mt_uint16_t reg_sum = 0;

    *reg_num = 0;

    num_blocks = sizeof(mt2081b0_blocks) / (sizeof(mt2081b0_blocks[0]));
    for (i = 0; i < num_blocks; i++)
    {
        if (reg_addr < mt2081b0_blocks[i].offset + (2 * mt2081b0_blocks[i].length) )
            break;
        reg_sum += mt2081b0_blocks[i].length;
    }

    if (i == num_blocks)
        return status | MT_ARG_RANGE;

    *reg_num = reg_sum +  ( (reg_addr - mt2081b0_blocks[i].offset) / 2);

    if (*reg_num >= MT2081_REG__MAX)
        return status | MT_ARG_RANGE;

    if (*reg_num >= sizeof(pInfo->reg) / sizeof(pInfo->reg[0]))
        return (mt_uint32_t)(MT_UNKNOWN | status);

    return status;
}


/******************************************************************************
**
**  Name: mt2081_regnum2addr
**
**  Description:    Search the cache position for a register address (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_regnum2addr(MT2081_Info_t *pInfo, mt_uint16_t reg_num, mt_uint16_t* reg_addr)
{
    mt_uint32_t status = MT_OK;

    mt_uint16_t num_blocks;
    mt_uint16_t i;
    mt_uint16_t reg_sum = 0;

    *reg_addr = 0;

    if (reg_num >= MT2081_REG__MAX)
        return status | MT_ARG_RANGE;

    if (reg_num >= sizeof(pInfo->reg) / sizeof(pInfo->reg[0]))
        return (mt_uint32_t)(MT_UNKNOWN | status);

    num_blocks = sizeof(mt2081b0_blocks) / (sizeof(mt2081b0_blocks[0]));

    /* Search the block in which the register is */
    for (i = 0; i < num_blocks; i++)
    {
        if (reg_num < reg_sum + mt2081b0_blocks[i].length )
            break;
        reg_sum += mt2081b0_blocks[i].length;
    }

    if (i == num_blocks)
        return status | MT_ARG_RANGE;

    *reg_addr = mt2081b0_blocks[i].offset + ( (reg_num-reg_sum) *2 );

    return status;
}



/******************************************************************************
**
**  Name: mt2081_read
**
**  Description:    Read of a register of the MT2081 (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_read(MT2081_Info_t *pInfo,
                                         mt_uint16_t field,
                                         mt_uint16_t field_addr,
                                         mt_uint16_t field_mask,
                                         mt_uint16_t field_shift,
                                         mt_uint16_t *var,
                                         mt_uint8_t do_cache)
{
    mt_uint32_t status = MT_OK;
    mt_uint8_t buf[4];

    buf[0] = (field_addr & 0xff00) >> 8;
    buf[1] = field_addr & 0x00ff;

    status = MT_WriteRead(pInfo->hUserData, pInfo->address, &buf[0], 2, &buf[2], 2);

    if (MT_NO_ERROR(status))
    {
        *var = ((mt_uint16_t)(buf[2]) << 8) | (mt_uint16_t)(buf[3]);
        if (do_cache)
            pInfo->reg[field] = *var;
        *var = (*var & field_mask) >> field_shift;
    }
    return status;
}

/*
** Internal function for field based register access - read & update cache
*/
#define MT_READ_CACHED(pInfo, field, var) \
    mt2081_read(pInfo, field, field##__ADDR_, field##__MASK_, field##__SHIFT_, var, 1)

/*
** Internal function for field based register access - read & NOT update cache
*/
/* May be removed if not neccessary in future, not neccessary at the moment */
#define MT_READ_UNCACHED(pInfo, field, var) \
    mt2081_read(pInfo, field, field##__ADDR_, field##__MASK_, field##__SHIFT_, var, 0)

/*
** Internal function for field based register access - read only from cache, not from hardware
*/
#define MT_READ_FROM_CACHE(pInfo, field, var) \
    *(var) = ((pInfo->reg[field] & field##__MASK_) >> field##__SHIFT_)

/******************************************************************************
**
**  Name: mt2081_write
**
**  Description:    Write to a register of the MT2081 (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_write(MT2081_Info_t *pInfo,
                                          mt_uint16_t field,
                                          mt_uint16_t field_addr,
                                          mt_uint16_t field_mask,
                                          mt_uint16_t field_shift,
                                          mt_uint16_t var,
                                          mt_uint8_t cache_flag)
{
    mt_uint32_t status = MT_OK;
    mt_uint8_t buf[4];

    var = (pInfo->reg[field] & ~field_mask) | ((var << field_shift) & field_mask);
    buf[0] = (field_addr & 0xff00) >> 8;
    buf[1] = field_addr & 0x00ff;
    buf[2] = (var & 0xff00) >> 8;
    buf[3] = var & 0x00ff;

    if ( ( (cache_flag == 1) || (var != pInfo->reg[field]) ) && (cache_flag != 2) )
        status = MT_WriteRead(pInfo->hUserData, pInfo->address, &buf[0], 4, &buf[0], 0);
    if (MT_NO_ERROR(status))
        pInfo->reg[field] = var;
    return status;
}


/*
** Internal function for field based register access - write and update cache
*/
#define MT_WRITE_CACHED(pInfo, field, var) \
    mt2081_write(pInfo, field, field##__ADDR_, field##__MASK_, field##__SHIFT_, var, 1)

/*
** Internal function for field based register access - write ONLY to cache (use with care !)
*/
#define MT_WRITE_TOCACHE(pInfo, field, var) \
    mt2081_write(pInfo, field, field##__ADDR_, field##__MASK_, field##__SHIFT_, var, 2)

/*
** Internal function for field based register access - write and update cache,
** if value is not the same as in the cache
*/
#define MT_WRITE_ONDEMAND(pInfo, field, var) \
    mt2081_write(pInfo, field, field##__ADDR_, field##__MASK_, field##__SHIFT_, var, 0)


/******************************************************************************
**
**  Name: mt2081_readblock
**
**  Description:    Read registers blockwise (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_readblock(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, mt_uint16_t* reg_data, mt_uint32_t length)
{
    mt_uint32_t status = MT_OK;

    mt_uint16_t j;
    mt_uint16_t k;
    mt_uint8_t  reg_addr_buf[2];
    mt_uint8_t  reg_val_buf[MT2081_WRITE_READ_MTU*2];
    mt_uint32_t reg_val_length=0;
    mt_uint16_t reg_num=0;


    for (j = 0; j < length; j = j + MT2081_WRITE_READ_MTU )
    {
        reg_addr_buf[0] = ( (reg_addr+j*2) & 0xff00) >> 8;
        reg_addr_buf[1] =   (reg_addr+j*2) & 0x00ff;

        if ( (length-j) > MT2081_WRITE_READ_MTU )
            reg_val_length = MT2081_WRITE_READ_MTU*2;
        else
            reg_val_length = (length-j)*2;

        status |= MT_WriteRead(pInfo->hUserData, pInfo->address, &reg_addr_buf[0], 2, &reg_val_buf[0], reg_val_length);

        if (MT_IS_ERROR(status))
            return status;

        for (k=0;k<reg_val_length/2;k++)
        {
            reg_data[j+k] = (mt_uint16_t)(reg_val_buf[k*2]<<8) + (mt_uint16_t)(reg_val_buf[(k*2)+1]);
        }
    }

    /* Update register cache */
    status |= mt2081_regaddr2num(pInfo, reg_addr, &reg_num);
    if (MT_NO_ERROR(status))
    {
            if((reg_num + length) <= NUM_REGS)
            {
                for (j = 0; j < length; j++)
                pInfo->reg[reg_num + j] = reg_data[j];  
            }
    }

    return status;
}

/******************************************************************************
**
**  Name: mt2081_writeblock
**
**  Description:    Write registers blockwise (internal function)
**
**  Returns:        status:
**                      MT_OK             - No errors
**                      MT_COMM_ERR       - Serial bus communications error
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
static mt_uint32_t mt2081_writeblock(MT2081_Info_t *pInfo, mt_uint16_t reg_addr, const mt_uint16_t* reg_data, mt_uint32_t length)
{
    mt_uint32_t status = MT_OK;

    mt_uint16_t j;
    mt_uint16_t k;
    mt_uint8_t  reg_val_buf[MT2081_WRITE_READ_MTU*2];
    mt_uint32_t reg_val_length=0;
    mt_uint16_t reg_num=0;

    /*
    ** MT2081_WRITE_READ_MTU needs to be at least 2 (=4 bytes) , since we need one
    ** 2 bytes for the subaddress and 2 for the value
    */
    #if MT2081_WRITE_READ_MTU < 2
    #error mt2081.c: MT2081_WRITE_READ_MTU nneeds to be at least set to 2
    #endif

    for (j = 0; j < length; j = j + (MT2081_WRITE_READ_MTU-1) )
    {
        reg_val_buf[0] = ( (reg_addr+j*2) & 0xff00) >> 8;
        reg_val_buf[1] =   (reg_addr+j*2) & 0x00ff;

        if ( (length-j) > (MT2081_WRITE_READ_MTU-1) )
            reg_val_length = (MT2081_WRITE_READ_MTU)*2;
        else
            reg_val_length = ( (length-j)*2 + 2);

        for (k=1;k<((reg_val_length/2));k++)
        {
            reg_val_buf[k*2]     = ( (reg_data[(k-1)+j]) & 0xff00) >> 8;
            reg_val_buf[(k*2)+1] =   (reg_data[(k-1)+j]) & 0x00ff;
        }

        status |= MT_WriteRead(pInfo->hUserData, pInfo->address, &reg_val_buf[0], reg_val_length, &reg_val_buf[0], 0);

        if (MT_IS_ERROR(status))
            return status;

    }

    /* Update register cache */
    status |= mt2081_regaddr2num(pInfo, reg_addr, &reg_num);
    if (MT_NO_ERROR(status))
    {
        if((reg_num + length) <= NUM_REGS)
        {
            for (j = 0; j < length; j++)
                pInfo->reg[reg_num + j] = reg_data[j];
        }
    }

    return status;
}

/****************************************************************************
**
**  Name: mt2081_update_reg_cache
**
**  Description:    Updates register cache
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
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
static mt_uint32_t mt2081_update_reg_cache(MT2081_Info_t* pInfo)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    mt_uint16_t i;
    mt_uint16_t reg_num = 0;
    mt_uint16_t reg_addr = 0;
    mt_uint16_t num_blocks = 0;

    num_blocks = sizeof(mt2081b0_blocks) / (sizeof(mt2081b0_blocks[0]));

    /* Read all registers from each block (non-linear) and put them */
    /* into the linear cache (aka pInfo->reg[x] )                   */
    for (i = 0; i < num_blocks; i++)
    {
        reg_addr = mt2081b0_blocks[i].offset;

        status |= mt2081_readblock(pInfo, reg_addr, &pInfo->reg[reg_num], mt2081b0_blocks[i].length);
        reg_num += mt2081b0_blocks[i].length;

        if (MT_IS_ERROR(status))
            return status;
    }

    return status;
}

/******************************************************************************
**
** End of local functions:
**
** Serial bus communication and register handling
**
******************************************************************************/



/****************************************************************************
**
**  Name: FindClkSrc
**
**  Description:    Determine, if clock comes from crystal or via clock
**                  distribution (needed for crystal sharing)
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
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
static mt_uint32_t FindClkSrc(MT2081_Info_t* pInfo)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint16_t i;
    mt_uint16_t j;

    /*  SRO needs to be enabled to read full regmap */
    /*  First read Reg 0x0000 to have the value in the cache */
    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN, &j);
    /*  Turn on SRO, if not enabled */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EBFE, 0);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EXTEN, 0);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN, 1);

    if (MT_IS_ERROR(status))
        return status;

    /*  If FCLK or LO_SELECT are both 0, enable FCLK */
    MT_READ_FROM_CACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, &i);
    MT_READ_FROM_CACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, &j);
    if (i == 0 && j == 0)
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 1);

    return status;
}

/****************************************************************************
**
**  Name: mt2081_initialize
**
**  Description:    Initialize the pInfo structure and updates register cache
**
**  Parameters:     h            - Open handle to the tuner (from MT2081_Open).
**
**  Returns:        status:
**                      MT_OK            - No errors
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
static mt_uint32_t mt2081_initialize(mt_handle_t h)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (MT_NO_ERROR(status))
        status |= mt2081_update_reg_cache(pInfo);

    if (MT_NO_ERROR(status))
        status |= MT2081_Identify(pInfo->address, &pInfo->tuner_id, pInfo->hUserData);

    if (MT_NO_ERROR(status))
    {
        /*  Initialize the tuner state. These are also the init default */
        pInfo->version = VERSION;
        pInfo->f_ref = REF_FREQ;
        pInfo->f_out_bw = 5800000;
        pInfo->f_out = 44000000;
        pInfo->f_lo = 375000000;
        pInfo->f_in = 375000000;
        pInfo->rcvr_mode = MT2081_CABLE_QAM_NA;
        pInfo->fracn_avoid_zone = 0;
        pInfo->vco_div = 0;
        pInfo->f_adcclk_idx = 0;
        pInfo->f_sysclk_idx = 0;
        pInfo->agc_freeze = 0;
        pInfo->val_rc_cal = 114;
        pInfo->n1a_profile = 0;
        pInfo->rssi_delta = 0;
        pInfo->sa_profile = 0;
        pInfo->vco_profile = 0;
        pInfo->num_regs = NUM_REGS;
    }

#if defined(MT2081_INCLUDE_POWER_REPORTING)
        /*  Initialize the tuner gain calibration table  */
        MT2081_GCInstallDefaultTable(h);
#endif
    return status;
}

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
                           mt_uint16_t* val)
{
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint8_t buf[4];
    mt_uint16_t regnum = 0;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (val == NULL)
        status |= MT_ARG_NULL;

    if (MT_IS_ERROR(status))
        return status;

    status |= mt2081_regaddr2num(pInfo, regaddr, &regnum);

    if (regnum >= MT2081_REG__MAX || MT_IS_ERROR(status) )
        return status | MT_ARG_RANGE;

    buf[0] = (regaddr & 0xff00) >> 8;
    buf[1] = regaddr & 0x00ff;
    status |= MT_WriteRead(pInfo->hUserData, pInfo->address, &buf[0], 2, &buf[2], 2);

    if (MT_NO_ERROR(status))
    {
        *val = ((mt_uint16_t)(buf[2]) << 8) | (mt_uint16_t)(buf[3]);
        pInfo->reg[regnum] = *val;
    }

    return status;
}

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
MT2081_RESULT MT2081_TriggerRegDump(mt_handle_t h, mt_uint32_t* num_regs)
{
    mt_uint32_t status = MT_OK;
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (num_regs == NULL)
        status |= MT_ARG_NULL;
    else
        *num_regs = 0;

    if (MT_NO_ERROR(status))
        status |= mt2081_update_reg_cache(pInfo);

    if (MT_NO_ERROR(status))
        *num_regs = MT2081_REG__MAX;

    return status;
}

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
**                  MT2081_Sleep      - Delay execution for x milliseconds
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetRegDump(mt_handle_t h, mt_uint16_t reg_num, mt_uint16_t* reg_addr, mt_uint16_t* reg_val)
{
    mt_uint32_t status = MT_OK;

    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (reg_addr == NULL || reg_val == NULL)
        status |= MT_ARG_NULL;

    if (reg_num >= MT2081_REG__MAX && MT_NO_ERROR(status))
        status |= MT_ARG_RANGE;

    /* Get the address for this register */
    if (MT_NO_ERROR(status))
        status |= mt2081_regnum2addr(pInfo, reg_num, reg_addr);

    /* Get the value for the register out ouf the cache */
    if (MT_NO_ERROR(status))
        *reg_val = pInfo->reg[reg_num];

    return status;
}


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
**                  MT2081_Sleep      - Delay execution for x milliseconds
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_GetLocked(mt_handle_t h,
                              mt_uint32_t* islocked)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint32_t nDelays = 0;
    mt_uint16_t lo_lk = 0;

    const mt_uint32_t nMaxWait = 500; /*  wait a maximum of 100 msec   */
    const mt_uint32_t nPollRate = 1;  /*  poll status bits every 1 ms */
    const mt_uint32_t nMaxLoops = nMaxWait / nPollRate;

    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (islocked == NULL)
        status |= MT_ARG_NULL;
    else
    {
        *islocked = 0;
        do
        {
            status |= MT_READ_CACHED(pInfo,MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_LK,&lo_lk);

            if (MT_IS_ERROR(status))
                break;

            if (lo_lk)
            {
                *islocked = 1;
                break;
            }

            MT2081_Sleep(pInfo->hUserData, nPollRate); /*  Wait between retries  */
        }
        while (++nDelays < nMaxLoops);
    }

    return status;
}

/****************************************************************************
**
**  Name: lin_interp
**
**  Description:    linear interpolation between two values
**   Parameters:
**
**                   x_1          x_act             x_2
**        -----------+-------------|----------------+----------
**                  y_1        return_value        y_2
**
**  All parameters should be smaller than mt_int32_t / 100
**
**  Caution: The function works in integer logic, take care about
**                the precision ( 0.4 will be handled as 0 )
**
****************************************************************************/
static mt_int32_t lin_interp(mt_int32_t y_1, mt_int32_t y_2, mt_int32_t x_1, mt_int32_t x_2, mt_int32_t x_act) {
    mt_int32_t x_per = 0;
    mt_int32_t y_act = 0;

    if (x_2 - x_1 == 0) return (mt_int32_t)y_1;

    x_per = (100*(x_act - x_1)) / (x_2 - x_1); /* Get a percental value */

    y_act = (y_2 - y_1) * x_per;

    y_act /= 100; /* down again */
    y_act += y_1;

    return (mt_int32_t)y_act;
}

static void find_in_table(const struct lookuptable *tbl, mt_uint32_t tablesize, mt_uint32_t a, mt_uint32_t *idx1, mt_uint32_t *idx2) {
    mt_uint32_t curidx = tablesize / 2;
    mt_uint8_t curstepsize = 128;
    mt_uint8_t i;

    /* Binary search in lookup table */
    for (i = 0; i < 9; i++)
    {
        if (curidx == (tablesize - 1) && a >= tbl[curidx].num)
        {
            *idx1 = curidx - 1;
            *idx2 = curidx;
            return;
        } else if (curidx == 0 && a <= tbl[curidx].num)
        {
            *idx1 = curidx;
            *idx2 = curidx + 1;
            return;
        }

        if (a < tbl[curidx].num) {
        /* Step downwards */
        if (curstepsize >= curidx)
        {
            curidx = 0;
        } else
        {
            curidx -= curstepsize;
        }
        } else if (a > tbl[curidx + 1].num)
        {
            /* Step upwards */
            if ((curidx + curstepsize) >= (tablesize - 1U))
            {
                curidx = tablesize - 1;
            } else
            {
                curidx += curstepsize;
            }
        } else
        {
            break;
        }

        curstepsize = (curstepsize + 1) / 2;
    }

    *idx1 = curidx;
    *idx2 = curidx + 1;
}


/******************************************************************************
**
**  Name: mt2081_n1a_preagc_lvl
**
**  Description:    Get the RSSI value
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
static mt_uint32_t mt2081_n1a_preagc_lvl(MT2081_Info_t *pInfo, mt_uint32_t *lvl)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    mt_uint16_t n1a_gain = 0;
    mt_uint16_t n1a_chf_gain = 0;
    mt_uint16_t n1a_multi = 0;
    mt_uint16_t n1a_mag_detect = 0;
    mt_uint32_t val = 0 ;
    mt_uint32_t idx1;
    mt_uint32_t idx2;
    mt_uint32_t temp;
    mt_uint8_t average;
    mt_uint8_t i;

    static const struct lookuptable lookuptable_n1a_multi[] = {
        { 1318, 219 }, { 1357, 244 }, { 1397, 269 }, { 1438, 294 }, { 1480, 319 },
        { 1524, 345 }, { 1569, 370 }, { 1615, 395 }, { 1663, 421 }, { 1712, 446 },
        { 1762, 471 }, { 1814, 496 }, { 1867, 521 }, { 1922, 546 }, { 1979, 572 },
        { 2037, 597 }, { 2097, 622 }, { 2159, 647 }, { 2223, 673 }, { 2288, 698 },
        { 2355, 723 }, { 2424, 748 }, { 2495, 773 }, { 2568, 798 }, { 2643, 823 },
        { 2721, 848 }, { 2801, 874 }, { 2883, 899 }, { 2968, 924 }, { 3055, 949 },
        { 3145, 974 }, { 3237, 999 }, { 3332,1024 }, { 3430,1049 }, { 3531,1075 },
        { 3635,1100 }, { 3742,1125 }, { 3852,1150 }, { 3965,1175 }, { 4081,1200 },
        { 4201,1226 }, { 4324,1251 }, { 4451,1276 }, { 4581,1301 }, { 4715,1326 },
        { 4853,1351 }, { 4995,1376 }, { 5141,1401 }, { 5292,1426 }, { 5447,1451 },
        { 5607,1476 }, { 5771,1501 }, { 5940,1526 }, { 6114,1552 }, { 6293,1577 },
        { 6477,1602 }, { 6667,1627 }, { 6862,1652 }, { 7063,1677 }, { 7270,1702 },
        { 7483,1727 }, { 7702,1752 }, { 7927,1777 }, { 8159,1802 }, { 8398,1827 },
        { 8644,1852 }, { 8897,1877 }, { 9157,1902 }, { 9425,1927 }, { 9701,1953 },
        { 9985,1978 }, {10277,2003 }, {10578,2028 }, {10887,2053 }, {11205,2078 },
        {11533,2103 }, {11870,2128 }, {12217,2153 }, {12574,2178 }, {12942,2203 },
        {13320,2228 }, {13709,2253 }, {14110,2278 }, {14523,2303 }, {14948,2328 },
        {15385,2353 }, {15835,2378 }, {16298,2403 }, {16774,2428 }, {17264,2453 },
        {17769,2478 }, {18288,2503 }, {18823,2528 }, {19373,2553 }, {19939,2578 },
        {20522,2603 }, {21122,2628 }, {21739,2653 }, {22374,2678 }, {23028,2703 },
        { 23701, 2728 },  { 24394, 2753 },  { 25107, 2778 },  { 25841, 2804 },
        { 26596, 2829 },  { 27373, 2854 },  { 28173, 2879 },  { 28996, 2904 },
        { 29843, 2929 },  { 30715, 2954 },  { 31612, 2979 },  { 32536, 3004 },
        { 33487, 3029 },  { 34465, 3054 },  { 35472, 3079 },  { 36508, 3104 },
        { 37575, 3129 },  { 38673, 3154 },  { 39803, 3179 },  { 40966, 3204 },
        { 42163, 3229 },  { 43395, 3254 },  { 44663, 3279 },  { 45968, 3304 },
        { 47311, 3329 },  { 48693, 3354 },  { 50115, 3379 },  { 51579, 3404 },
        { 53086, 3429 },  { 54637, 3454 },  { 56233, 3479 },  { 57876, 3504 },
        { 59567, 3529 },  { 61307, 3554 },  { 63098, 3579 },  { 64941, 3604 },
        { 66838, 3629 },  { 68790, 3654 },  { 70799, 3679 },  { 72867, 3704 },
        { 74995, 3729 },  { 77185, 3754 },  { 79439, 3779 },  { 81759, 3804 },
        { 84147, 3829 },  { 86605, 3854 },  { 89134, 3879 },  { 91737, 3904 },
        { 94416, 3929 },  { 97173, 3954 },  { 100011, 3979 }, { 102932, 4004 },
        { 105938, 4029 }, { 109032, 4054 }, { 112216, 4079 }, { 115493, 4104 },
        { 118866, 4129 }, { 122337, 4154 }, { 125910, 4179 }, { 129587, 4204 },
        { 133372, 4229 }, { 137267, 4254 }, { 141276, 4279 }, { 145402, 4304 },
        { 149648, 4329 }, { 154018, 4354 }, { 158516, 4379 }, { 163145, 4404 },
        { 167909, 4429 }, { 172813, 4454 }, { 177860, 4479 }, { 183054, 4504 },
        { 188400, 4529 }, { 193902, 4554 }, { 199565, 4579 }, { 205393, 4604 },
        { 211391, 4629 }, { 217564, 4654 }, { 223917, 4679 }, { 230456, 4704 },
        { 237186, 4729 }, { 244112, 4754 }, { 251241, 4779 }, { 258578, 4804 },
        { 266129, 4829 }, { 273901, 4854 }, { 281900, 4879 }, { 290132, 4904 },
        { 298605, 4929 }, { 307325, 4954 }, { 316300, 4979 }, { 325537, 5004 },
        { 335043, 5029 }, { 344827, 5054 }, { 354897, 5079 }, { 365261, 5104 },
        { 375927, 5129 }, { 386905, 5154 }, { 398203, 5179 }, { 409831, 5204 },
        { 421799, 5229 }, { 434116, 5254 }, { 446793, 5279 }, { 459840, 5304 },
        { 473268, 5329 }, { 487088, 5354 }, { 501312, 5379 }, { 515951, 5404 },
        { 531018, 5429 }, { 546525, 5454 }, { 562484, 5479 }, { 578909, 5504 },
        { 595814, 5529 }, { 613213, 5554 }, { 631120, 5579 }, { 649550, 5604 },
        { 668518, 5629 }, { 688040, 5654 }, { 708132, 5679 }, { 728810, 5704 },
        { 750092, 5729 }, { 771996, 5754 }, { 794539, 5779 }, { 817740, 5804 },
        { 841619, 5829 }, { 866195, 5854 }, { 891489, 5879 }, { 917521, 5904 },
        { 944314, 5929 }, { 971889, 5954 }
    };

    static const struct lookuptable lookuptable_n1a_detect[] = {
        { 500, 7398 },{ 503, 7403 },{ 506, 7408 },{ 509, 7413 },{ 512, 7419 },
        { 515, 7424 },{ 518, 7429 },{ 521, 7434 },{ 524, 7439 },{ 527, 7444 },
        { 530, 7449 },{ 533, 7453 },{ 536, 7458 },{ 539, 7463 },{ 542, 7468 },
        { 545, 7473 },{ 548, 7478 },{ 551, 7482 },{ 554, 7487 },{ 557, 7492 },
        { 560, 7496 },{ 563, 7501 },{ 566, 7506 },{ 569, 7510 },{ 572, 7515 },
        { 575, 7519 },{ 578, 7524 },{ 581, 7528 },{ 584, 7533 },{ 587, 7537 },
        { 590, 7542 },{ 593, 7546 },{ 596, 7550 },{ 599, 7555 },{ 602, 7559 },
        { 605, 7564 },{ 608, 7568 },{ 611, 7572 },{ 614, 7576 },{ 617, 7581 },
        { 620, 7585 },{ 623, 7589 },{ 626, 7593 },{ 629, 7597 },{ 632, 7601 },
        { 635, 7606 },{ 638, 7610 },{ 641, 7614 },{ 644, 7618 },{ 647, 7622 },
        { 650, 7626 },{ 653, 7630 },{ 656, 7634 },{ 659, 7638 },{ 662, 7642 },
        { 665, 7646 },{ 668, 7650 },{ 671, 7653 },{ 674, 7657 },{ 677, 7661 },
        { 680, 7665 },{ 683, 7669 },{ 686, 7673 },{ 689, 7676 },{ 692, 7680 },
        { 695, 7684 },{ 698, 7688 },{ 701, 7691 },{ 704, 7695 },{ 707, 7699 },
        { 710, 7703 },{ 713, 7706 },{ 716, 7710 },{ 719, 7713 },{ 722, 7717 },
        { 725, 7721 },{ 728, 7724 },{ 731, 7728 },{ 734, 7731 },{ 737, 7735 },
        { 740, 7738 },{ 743, 7742 },{ 746, 7745 },{ 749, 7749 },{ 752, 7752 },
        { 755, 7756 },{ 758, 7759 },{ 761, 7763 },{ 764, 7766 },{ 767, 7770 },
        { 770, 7773 },{ 773, 7776 },{ 776, 7780 },{ 779, 7783 },{ 782, 7786 },
        { 785, 7790 },{ 788, 7793 },{ 791, 7796 },{ 794, 7800 },{ 797, 7803 },
        { 800, 7806 },{ 803, 7809 },{ 806, 7813 },{ 809, 7816 },{ 812, 7819 },
        { 815, 7822 },{ 818, 7826 },{ 821, 7829 },{ 824, 7832 },{ 827, 7835 },
        { 830, 7838 },{ 833, 7841 },{ 836, 7844 },{ 839, 7848 },{ 842, 7851 },
        { 845, 7854 },{ 848, 7857 },{ 851, 7860 },{ 854, 7863 },{ 857, 7866 },
        { 860, 7869 },{ 863, 7872 },{ 866, 7875 },{ 869, 7878 },{ 872, 7881 },
        { 875, 7884 },{ 878, 7887 },{ 881, 7890 },{ 884, 7893 },{ 887, 7896 },
        { 890, 7899 },{ 893, 7902 },{ 896, 7905 },{ 899, 7908 },{ 902, 7910 },
        { 905, 7913 },{ 908, 7916 },{ 911, 7919 },{ 914, 7922 },{ 917, 7925 },
        { 920, 7928 },{ 923, 7930 },{ 926, 7933 },{ 929, 7936 },{ 932, 7939 },
        { 935, 7942 },{ 938, 7944 },{ 941, 7947 },{ 944, 7950 },{ 947, 7953 },
        { 950, 7955 },{ 953, 7958 },{ 956, 7961 },{ 959, 7964 },{ 962, 7966 },
        { 965, 7969 },{ 968, 7972 },{ 971, 7974 },{ 974, 7977 },{ 977, 7980 },
        { 980, 7982 },{ 983, 7985 },{ 986, 7988 },{ 989, 7990 },{ 992, 7993 },
        { 995, 7996 },{ 998, 7998 },{1001, 8001 },{1004, 8003 },{1007, 8006 },
        {1010, 8009 },{1013, 8011 },{1016, 8014 },{1019, 8016 },{1022, 8019 },
        {1025, 8021 },{1028, 8024 },{1031, 8027 },{1034, 8029 },{1037, 8032 },
        {1040, 8034 },{1043, 8037 },{1046, 8039 },{1049, 8042 },{1052, 8044 },
        {1055, 8047 },{1058, 8049 },{1061, 8051 },{1064, 8054 },{1067, 8056 },
        {1070, 8059 },{1073, 8061 },{1076, 8064 },{1079, 8066 },{1082, 8068 },
        {1085, 8071 },{1088, 8073 },{1091, 8076 },{1094, 8078 },{1097, 8080 },
        {1100, 8083 },{1103, 8085 },{1106, 8088 },{1109, 8090 },{1112, 8092 },
        {1115, 8095 },{1118, 8097 },{1121, 8099 },{1124, 8102 },{1127, 8104 },
        {1130, 8106 },{1133, 8108 },{1136, 8111 },{1139, 8113 },{1142, 8115 },
        {1145, 8118 },{1148, 8120 },{1151, 8122 },{1154, 8124 },{1157, 8127 },
        {1160, 8129 },{1163, 8131 },{1166, 8133 },{1169, 8136 },{1172, 8138 },
        {1175, 8140 },{1178, 8142 },{1181, 8144 },{1184, 8147 },{1187, 8149 },
        {1190, 8151 },{1193, 8153 },{1196, 8155 },{1199, 8158 }
    };

    *lvl = 0;
    average = 1;

    /* Do a averaging */
    for (i=0;i<average;i++)
    {
        /* All three values are in one register, read them at once to avoid toggling */
        status |= MT_READ_CACHED(pInfo, MT2081_REG_N1A_AUTO_GAINS, &n1a_gain);
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_N1A_IQ_MAG_DETECTED__VAL, &n1a_mag_detect);

        if (MT_IS_ERROR(status))
            return status;

        n1a_multi = (n1a_gain & 0x07ff);
        n1a_chf_gain = (mt_uint16_t)( (n1a_gain & 0xC000)>>14 );
        n1a_gain = (mt_uint16_t)( (n1a_gain & 0x3800)>>11 );

        n1a_chf_gain = (mt_uint16_t)( 1<<n1a_chf_gain );
        n1a_gain = (mt_uint16_t)( 1<<n1a_gain );

        val = n1a_chf_gain * n1a_gain * n1a_multi;

        /* printf("%d,%d,%d,%d,",n1a_chf_gain,n1a_gain,n1a_multi,n1a_mag_detect); */

        if (n1a_mag_detect>=1199)
            temp=8158;
        else if (n1a_mag_detect<=500)
            temp=7398;
        else
        {
            /* Float version would be like */
            /* temp = 2000 + (mt_uint32_t)((double)( 100 * 20 * log10(  (double)n1a_mag_detect )) );  */

            find_in_table(lookuptable_n1a_detect, sizeof(lookuptable_n1a_detect) / sizeof(lookuptable_n1a_detect[0]), n1a_mag_detect, &idx1, &idx2);
            temp =  lin_interp(lookuptable_n1a_detect[idx1].result,
                                        lookuptable_n1a_detect[idx2].result,
                                        lookuptable_n1a_detect[idx1].num,
                                        lookuptable_n1a_detect[idx2].num,
                                        n1a_mag_detect);
        }

        if ( val>=971889 )
            temp -= 5954;
        else if ( val<=1318 )
            temp -= 219;
        else
        {
            /* Float version would be */
            /* temp -= (mt_uint32_t)((double)( 100 * 20 * log10(  (double)val/1024 )) ) ; */

            find_in_table(lookuptable_n1a_multi, sizeof(lookuptable_n1a_multi) / sizeof(lookuptable_n1a_multi[0]), val, &idx1, &idx2);
            temp -=  lin_interp(lookuptable_n1a_multi[idx1].result,
                                        lookuptable_n1a_multi[idx2].result,
                                        lookuptable_n1a_multi[idx1].num,
                                        lookuptable_n1a_multi[idx2].num,
                                        val);
        }

        /* Slope correction factor */
        temp = (temp * 1000)/993;

        *lvl += temp;
    }

    *lvl /= average;

    return status;
}


/******************************************************************************
**
**  Name: mt2081_get_rssi
**
**  Description:    Get the RSSI value
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
static mt_uint32_t mt2081_get_rssi(MT2081_Info_t *pInfo, mt_uint32_t *rssi)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    mt_uint32_t splt_atn=0;
    mt_uint32_t lna_atn=0;
    mt_uint32_t bbagc_atn=0;
    mt_uint32_t n1a_preagc_lvl = 0;

    mt_int32_t splt_gain = 0;
    mt_int32_t lna_gain = 0;
    mt_int32_t bbagc_gain = 0;
    mt_int32_t tvg_gain = 0;
    mt_uint32_t idx1;
    mt_uint32_t idx2;

    static const struct lookuptable splt_lut[] = {{0,0},{90,187},{230,585},{450,1410},{640,2260},{800,2900}};
    static const struct lookuptable lna_lut[] = {{0,0},{100,340},{230,800},{360,1360},{505,2000},{560,2450}};
    static const struct lookuptable bbagc_lut[] = {{0,2980},{11520,1980},{22400,1420},{39040,940},{51200,390},{65535,0}};
    static const struct lookuptable tvg_lut[] = {{ 60, 272},{348, 427},{352, 472},{368, 374},{400, 281},{430, 218},
                                                 {450, 191},{487,  89},{523, 111},{570,  71},{620,  42},{900,  27},{1000, 228} };

    status |= MT2081_GetParam(pInfo->handle, MT2081_SPLT_ATN, &splt_atn);
    status |= MT2081_GetParam(pInfo->handle, MT2081_LNA_ATN, &lna_atn);
    status |= MT2081_GetParam(pInfo->handle, MT2081_BBAGC_ATN, &bbagc_atn);

    status |= mt2081_n1a_preagc_lvl(pInfo, &n1a_preagc_lvl);

    if (MT_IS_ERROR(status))
    {
        *rssi = 0;
        return status;
    }

    find_in_table(bbagc_lut, sizeof(bbagc_lut) / sizeof(bbagc_lut[0]), bbagc_atn, &idx1, &idx2);
    bbagc_gain =  lin_interp(bbagc_lut[idx1].result, bbagc_lut[idx2].result,
                                bbagc_lut[idx1].num,    bbagc_lut[idx2].num,  bbagc_atn);

    find_in_table(lna_lut, sizeof(lna_lut) / sizeof(lna_lut[0]), lna_atn, &idx1, &idx2);
    lna_gain =  lin_interp(lna_lut[idx1].result, lna_lut[idx2].result,
                            lna_lut[idx1].num,    lna_lut[idx2].num,  lna_atn);

    find_in_table(splt_lut, sizeof(splt_lut) / sizeof(splt_lut[0]), splt_atn, &idx1, &idx2);
    splt_gain =  lin_interp(splt_lut[idx1].result, splt_lut[idx2].result,
                            splt_lut[idx1].num,    splt_lut[idx2].num,  splt_atn);

    find_in_table(tvg_lut, sizeof(tvg_lut) / sizeof(tvg_lut[0]), (pInfo->f_in / 1000000), &idx1, &idx2);
    tvg_gain =  lin_interp(tvg_lut[idx1].result, tvg_lut[idx2].result,
                            tvg_lut[idx1].num,    tvg_lut[idx2].num,  (pInfo->f_in / 1000000) );

    *rssi = (n1a_preagc_lvl-2200) + bbagc_gain + lna_gain + splt_gain + tvg_gain + pInfo->rssi_delta;

    *rssi = (*rssi * 950) / 1000;

    return status;
}

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
**                  MT2081_RCVR_MODE          Receiver mode
**                  MT2081_AGC_FREEZE         Freeze Tuner AGC
**                  MT2081_AGC_ACQ            AGC acquisition mode
**                  MT2081_F_SYSCLK           System Clock frequency
**                  MT2081_F_ADCCLK           ADC Clock frequency
**                  MT2081_RFLT_ENABLE        RFLT enable/disable
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
MT2081_RESULT MT2081_GetParam(mt_handle_t h,
                             MT2081_Param param,
                             mt_uint32_t* pValue)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    if (pValue == NULL)
        status |= MT_ARG_NULL;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (MT_NO_ERROR(status))
    {
        switch (param)
        {
        /*  version of the API, e.g. 10302 = 1.32 */
        case MT2081_VERSION:
            *pValue = pInfo->version;
            break;

        /*  Serial Bus address of this tuner      */
        case MT2081_IC_ADDR:
            *pValue = pInfo->address;
            break;

        /*  tuner revision code (see enum  MT2081_REVCODE for values) */
        case MT2081_IC_REV:
            *pValue = pInfo->tuner_id;
            break;

        /*  Max # of MT2081's allowed to be open  */
        case MT2081_MAX_OPEN:
            *pValue = nMaxTuners;
            break;

        /*  # of MT2081's open                    */
        case MT2081_NUM_OPEN:
            *pValue = nOpenTuners;
            break;

        /*  crystal frequency                     */
        case MT2081_SRO_FREQ:
            *pValue = pInfo->f_ref;
            break;

        /*  input center frequency                */
        case MT2081_INPUT_FREQ:
            *pValue = pInfo->f_in;
            break;

        /*  IC standby                           */
        case MT2081_STANDBY:
            *pValue = pInfo->is_standby;
            break;

        /*  input center frequency                */
        case MT2081_TEMPERATURE:
        {
            mt_uint16_t reg_temp_cfg;
                                                 /* 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12, 13 */
            mt_uint16_t tempdata[14] = {0, 6,31,18,81,68,43,56, 0, 0, 0, 0,93,100};
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ANLG_TEMP_CFG__RD_DATA, &reg_temp_cfg);

            *pValue = tempdata[reg_temp_cfg];
            break;
        }

        /*  LO Frequency                         */
        case MT2081_LO_FREQ:
        {
            mt_uint16_t reg_vco_div;
            mt_uint16_t vco_div = 1;
            mt_uint16_t i = 0;
            mt_uint32_t f_vco4;

            status |= MT2081_GetParam(pInfo->handle, MT2081_VCO_FREQ4, &f_vco4);
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_VCO_DIV__VAL, &reg_vco_div);

            for (i = 0; i < reg_vco_div; i++)
                vco_div = vco_div * 2;

            pInfo->f_lo = f_vco4 / vco_div;
            pInfo->vco_div = (mt_uint8_t)vco_div;
            *pValue = pInfo->f_lo;
            break;
        }

        /*  VCO Frequency                         */
        case MT2081_VCO_FREQ4:
        {
            mt_uint16_t reg_lo_n_div;
            mt_uint16_t reg_lo_fracn_num;
            mt_uint16_t reg_lo_denom;

            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_LO_FN_CONFIG__DMAX, &reg_lo_denom);
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ANLG_LO_DIV_N_DIV__VAL, &reg_lo_n_div);
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ANLG_LO_FRACN_NUM__VAL, &reg_lo_fracn_num);

            pInfo->f_vco4 = (pInfo->f_ref/2 * reg_lo_n_div) + fLO_FractionalTerm(pInfo->f_ref/2, reg_lo_fracn_num, (8191 + reg_lo_denom));

            *pValue = pInfo->f_vco4;
            break;
        }

        /*  output bandwidth                      */
        case MT2081_OUTPUT_BW:
            *pValue = pInfo->f_out_bw;
            break;

        /*  output frequency                      */
        case MT2081_OUTPUT_FREQ:
            *pValue = pInfo->f_out;
            break;

        /*  Receiver mode                         */
        case MT2081_RCVR_MODE:
            *pValue = pInfo->rcvr_mode;
            break;

        /*  Freeze AGC                            */
        case MT2081_AGC_FREEZE:
            *pValue = pInfo->agc_freeze;
            break;

        /*  AGC accquistion mode enable           */
        case MT2081_AGC_ACQ:
        {
            mt_uint16_t reg_rfa_lock;
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_RFA_RFA_LOCK_STAT__VAL, &reg_rfa_lock);
            *pValue = (mt_uint32_t)(reg_rfa_lock ? 0 : 1);
            break;
        }

        /*  SYSCLK                                */
        case MT2081_F_SYSCLK:
        {
            *pValue = pInfo->f_sysclk[pInfo->f_sysclk_idx];
            break;
        }

        /*  ADCCLK                                */
        case MT2081_F_ADCCLK:
        {
            *pValue = pInfo->f_adcclk[pInfo->f_adcclk_idx];
            break;
        }

        /*  SA_PROFILE  */
        case MT2081_SA_PROFILE:
        {
            *pValue = pInfo->sa_profile;
            break;
        }

        /*  RFLT enable           */
        case MT2081_RFLT_ENABLE:
        {
            mt_uint16_t reg_rflt_dis=0;
            status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__RFLT_DIS, &reg_rflt_dis);

            if (reg_rflt_dis == 0)
            {
                *pValue = 1;
            }
            else
            {
                *pValue = 0;
            }
            break;
        }

        /*  SPLT_ATN                               */
        case MT2081_SPLT_ATN:
        {
            mt_uint16_t regval;
            status |= MT_READ_CACHED(h, MT2081_FIELD_RFA_SPLT_ATN_STAT__SPLT_ATN, &regval);
            *pValue = regval;
            break;
        }

        /*  LNA_ATN                                 */
        case MT2081_LNA_ATN:
        {
            mt_uint16_t regval;
            status |= MT_READ_CACHED(h, MT2081_FIELD_RFA_LN_ATN_STAT__LN_ATN, &regval);
            *pValue = regval;
            break;
        }

        /*  BBAGC_ATN                                 */
        case MT2081_BBAGC_ATN:
        {
            mt_uint16_t regval;
            status |= MT_READ_CACHED(h, MT2081_FIELD_AGC_BB_GAIN_CUR__VAL, &regval);
            *pValue = regval;
            break;
        }

        /*  N1A Profile */
        case MT2081_N1A_PROFILE:
        {
            *pValue = pInfo->n1a_profile;
            break;
        }

        /*  N1A_PREAGC_LVL                             */
        case MT2081_N1A_PREAGC_LVL:
        {
            mt_uint32_t level = 0;

            status |= mt2081_n1a_preagc_lvl(pInfo, &level);

            *pValue = level;
            break;
        }

        /*  Current RSSI value in dBuV  */
        case MT2081_RSSI:
        {
            mt_uint32_t rssi = 0;

            status |= mt2081_get_rssi(pInfo, &rssi);

            *pValue = rssi;
            break;
        }

        /*  Measured values of RSSI at 50dBuV  */
        case MT2081_RSSI_MEASURED:
        {
            *pValue = 5000 - pInfo->rssi_delta;
            break;
        }

        case MT2081_EOP:
        default:
            status |= MT_ARG_RANGE;
        }
    }
    return status;
}

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
MT2081_RESULT MT2081_GetUserData(mt_handle_t h,
                                  mt_user_t* hUserData )
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status = MT_INV_HANDLE;

    if (hUserData == NULL)
        status |= MT_ARG_NULL;

    if (MT_NO_ERROR(status))
        *hUserData = pInfo->hUserData;

    return status;
}

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
MT2081_RESULT MT2081_ReInit(mt_handle_t h)
{
    mt_uint32_t status = MT_OK; /* Status to be returned */
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;
    mt_uint32_t islocked = 0;
    mt_uint16_t i;
    mt_uint16_t j;

    const mt_uint16_t irf_coeff_norm_a[6] = {0xffe4, 0x0084, 0xfec7, 0x004d, 0xfe86, 0x00c2 };
    const mt_uint16_t irf_coeff_norm_b[6] = {0x02b0, 0xfa24, 0x140d, 0xff04, 0x012a, 0x1ec8 };

    const mt_uint16_t upf_coeff[24] = {0xfffe, 0xffff, 0x0004, 0xfff9, 0xfffc, 0x0001, 0xfffe, 0x0002,
                                       0x000f, 0xffd7, 0x0046, 0xffaf, 0xfffc, 0xfff9, 0x005f, 0xFF11,
                                       0xfff6, 0x0131, 0xfc0d, 0x0afa, 0x01ab, 0xfdc3, 0x0142, 0x0ffc};

    /*  Verify that the handle passed points to a valid tuner */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    /*  Verify tuner ID */
    if (MT_NO_ERROR(status))
        status |= MT2081_Identify(pInfo->address, &pInfo->tuner_id, pInfo->hUserData);

    if (MT_IS_ERROR(status))
        return status;

    switch (pInfo->tuner_id)
    {
    case MT2081_B0:
    {
        status |= FindClkSrc(pInfo);

        /* Read to update reg 0x0000 in cache */
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, &i);
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_EN, &j);
        /* Set reg 0x0004 to default values */
        status |= MT_WRITE_CACHED(pInfo, MT2081_REG_0004,1);

        /* Do not set Register ASYNC_CLOCKCONTROL at once, SRO_EBFE and SRO_EXTEN   */
        /* need to be preserved */
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_0002_05, 3);

        /* Disable DFE, in case it was running before */
        if ( (i == 1) && (j == 1) )
        {
            status |= MT_READ_CACHED(pInfo, MT2081_REG_COM_ADCCTRL, &i);
            status |= MT_READ_CACHED(pInfo, MT2081_REG_COM_DPCTRL, &i);
            status |= mt2081_enable_dfe(pInfo, 0);
        }

        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, 0);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_EN, 0);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 1);
        status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_0002_03, 1);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EBFE, 0);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EXTEN, 0);
        status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN, 1);

        /* Check if DFE is running */
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_COM_PING__VAL, &i);
        if (i != 0xffff)
        {
            status |= MT_TUNER_INIT_ERR;
            break;
        }

        /* Trigger reset */
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_COM_SOFT_RESET__VAL, 1);
        break;
    }

    default:
        status |= MT_TUNER_ID_ERR;
        break;
    }

    if (MT_IS_ERROR(status))
        return status;

    /* Update Cache and pInfo structure */
    if (MT_NO_ERROR(status))
        status |= mt2081_initialize(h);

    if (MT_IS_ERROR(status))
        return status;

    /* Main init sequence  */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_EN, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10E_04, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10E_08, 0);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10C_00, 1);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_01, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_00, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_11, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_09, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_08, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_E110_04, 15);
    status |= MT_WRITE_CACHED( pInfo, MT2081_FIELD_E110_10, 0);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E112_02, 2);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10E_05, 1);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP2_LO_MISC__SYSDIV_CLK_PHS, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_DAC_CONFIG_1__EN, 1);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E108_00, 150);

    /* Start up at 375MHZ */
    status |= mt2081_set_lofreq(pInfo, pInfo->f_lo, &islocked);

    if (MT_IS_ERROR(status))
        return status;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP2_LO_CLK_CNTRL__SYSCLK_DIV, 14);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_DPCTRL__OSR_8_10, 1);

    /* if LO is locked, go on sysclk */
    if ( islocked )
    {
        /* Switch on LO for clocking, disable FCLK */
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, 1);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 0);
    }

    if (MT_IS_ERROR(status))
        return status;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__EN, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E000_03, 1);

    /* Splitter  */
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__RFLT_DIS, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__SLV_DIS, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__AMP_EN, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__BUF_EN1, 0);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__BUF_EN2, 0);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_FILT_AMP__ENABLE, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_DCOC__ENABLE, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, 15);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_D000_04, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ANLG_ADC_CFG__BG_EN, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ANLG_ADC_CFG__EN, 1);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_D000_03, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_D002_00, 0xf0);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_D004_00, 0x0f);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_HRM_CNTRL__EN, 1);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__PD_EN, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__ADC_REFERENCE, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__RMS_OFFSET, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__RMS_GAIN, 3);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__PEAK_DISCHARGE, 2);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__PREAMP_EN, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_MISCBITS__RMS_BW, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_MISCBITS__PEAK_BYPASS, 0);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_E024_08, 0);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__EN, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_ACP1_CAP__SH, 18);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP1_CAP__FB, 24);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_D028_14, 1);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_D028_08,4);

    /* RF AGC */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_LNA_ATN_START__LNA_ATN_START, 0);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_5000_08, 1);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_5000_04, 0);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_LNLIMIT__LN_ATN_LIMIT, 280);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_LNMAX__LN_ATN_MAX, 0x230);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_SPLTLIMIT__SPLT_ATN_LIMIT, 800);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_SPLTMAX__SPLT_ATN_MAX, 800);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5030_04, 0x8);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5030_00, 0xa);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5028_00, 0x2c);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5014_00, 80);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5016_00, 500);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5026_00, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_501C_00, 0xf);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_501E_00, 0x3000);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5022_08, 0x30);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5022_00, 0x20);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5024_08, 0x10);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5018_00, 0x20);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_5018_08, 0x20);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_501A_00, 0x08);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_501A_08, 0x08);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__RF_AGC_EN, 1);

    /* BBAGC  */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C00C_08, 255);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C00C_00, 1);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_C010_15, 0);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_C010_00, 0x1000);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C012_00, 127);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C004_00, 2048);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_AGC_MAG_TARG__VAL, 1637);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_C006_12, 1);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_C006_00, 1);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C00A_00, 64);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_C000_04, 1);

    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_C000_04, 0);
    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_AGC_CFG__EN, 1);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_DCOC__ENABLE, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6002_00, 0x80);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6008_00, 0x08);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_600A_00, 0x800);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_600C_00, 16);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_601A_00, 4);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_601C_08, 8);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_601E_08, 20);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_11, 1);
    status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_07, 0);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6000_03, 1);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_7008_00 , 1023);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_7008_15 , 1);

    status |= mt2081_writeblock(pInfo, MT2081_REG_8000__ADDR_, &irf_coeff_norm_a[0], (mt_uint32_t)( sizeof(irf_coeff_norm_a) / sizeof(irf_coeff_norm_a[0] ) ));
    status |= mt2081_writeblock(pInfo, MT2081_REG_8010__ADDR_, &irf_coeff_norm_b[0], (mt_uint32_t)( sizeof(irf_coeff_norm_b) / sizeof(irf_coeff_norm_b[0] ) ));

    /* N1A  */
    status |= MT2081_SetParam(h, MT2081_N1A_PROFILE, 0);

    status |= mt2081_writeblock(pInfo, MT2081_REG_B000__ADDR_, &upf_coeff[0], (mt_uint32_t)( sizeof(upf_coeff) / sizeof(upf_coeff[0] ) ));

    /* Enable temperature sensor */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_TEMP_CFG__EN, 1);

    /* RC Cal. needs FCLK */
    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, &j);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 1);

    /* Do RC Calibration for DAC */
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_D02A_12, 1);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_D02A_13, 1);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_D02A_13, 0);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_D02A_12, 0);

    /* RC cal for BBF */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_RC_CAL__EN, 1);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__BBF_RST_RCCAL, 1);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__BBF_RST_RCCAL, 0);
    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP1_BBF_RC_CAL__BBF_RCCAL_RD, &i);
    pInfo->val_rc_cal = i;
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_RC_CAL__EN, 0);

    /* Restore former state of FCLK */
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, j);

    /* Tune to 375 / 44 / 6 MHz - should only set registers which are not already set above */
    status |= mt2081_set_fout_bw(pInfo, pInfo->f_out_bw);
    status |= MT2081_Tune(h, pInfo->f_in);

    return status;
}


/******************************************************************************
**
**  Name: mt2081_internal_standby
**
**  Description:    Set the tuner internal blocks into powersave mode.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                                0 - all on
**                                1 - all standby
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
static mt_uint32_t mt2081_internal_standby(MT2081_Info_t *pInfo, mt_uint32_t standby)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    if (standby == 1)
    {
        /* Set Standby bits */
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_TEMP_CFG__EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_DAC_CONFIG_1__EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_ADC_CFG__BG_EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_ADC_CFG__EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_FILT_AMP__ENABLE, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_HRM_CNTRL__EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_EN, 0);
    }
    else
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_HRM_CNTRL__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_FILT_AMP__ENABLE, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_ADC_CFG__BG_EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_ADC_CFG__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_DAC_CONFIG_1__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_DAC_CONFIG_1__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_TEMP_CFG__EN, 1);
    }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_splt_standby
**
**  Description:    Set the tuner splitter blocks into powersave mode.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                                0 - on
**                                1 - standby
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
static mt_uint32_t mt2081_splt_standby(MT2081_Info_t *pInfo, mt_uint32_t standby)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    /* Only effective for an MT2081 */
    if ( !IS_MT2081(pInfo) )
        return status;

    if (standby == 1)
    {
        /* Set Standby bits */
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__AMP_EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__PREAMP_EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__PD_EN, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__EN, 0);
    }
    else
    {
        /* Unset Standby bits */
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__PREAMP_EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_PD_CNTRL__PD_EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__EN, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__AMP_EN, 1);
    }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_standby
**
**  Description:    Set the tuner into powersave mode.
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                  standby     - MT2081: 0 - all on
**                                        1 - all standby
**                                        2 - splt on, internal off
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
static mt_uint32_t mt2081_standby(MT2081_Info_t *pInfo, mt_uint32_t standby)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint32_t islocked;

    switch (pInfo->tuner_id)
    {
        case MT2081_B0:
        {
            if (standby >= 3)
                status |= MT_ARG_RANGE;

            if (standby == 2)
            {
                /* Internal off, but splt on */
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__FREEZE, 1);
                status |= mt2081_iqe_freeze(pInfo, 1);
                status |= mt2081_bbagc_freeze(pInfo, 1);
                status |= mt2081_rfagc_freeze(pInfo, 0);

                status |= mt2081_enable_dfe(pInfo, 0);
                status |= mt2081_splt_standby(pInfo, 0);
                status |= mt2081_internal_standby(pInfo, 1);
            }
            else if (standby == 1)
            {
                /* Internal and splt off */
                status |= MT2081_SetParam(pInfo->handle,MT2081_AGC_FREEZE, 1);
                status |= mt2081_enable_dfe(pInfo, 0);
                status |= mt2081_splt_standby(pInfo, 1);
                status |= mt2081_internal_standby(pInfo, 1);
            }
            else
            {
                /* All on */
                status |= mt2081_internal_standby(pInfo, 0);
                status |= mt2081_splt_standby(pInfo, 0);

                /*
                **  Check for LO's locking
                */
                status |= MT2081_GetLocked(pInfo->handle, &islocked);

                /*
                **  If the LO is locked, enable LO clock for digital parts
                */
                if (islocked)
                {
                    status |= mt2081_enable_dfe(pInfo, 1);

                }
                else
                    return (status | MT_DNC_UNLOCK);

                status |= MT2081_SetParam(pInfo->handle,MT2081_AGC_FREEZE, 0);
            }

            break;
        }

        default:
        {
            status |= MT_TUNER_ID_ERR;
            break;
        }
    }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_iqe_freeze
**
**  Description:    Freeze IQE
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
static mt_uint32_t mt2081_iqe_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint16_t swap=0;
    mt_uint16_t val_i=0;
    mt_uint16_t val_q=0;

    if (freeze == 1)
    {
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6000_11, &swap);

        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6024_00, &val_i);
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6026_00, &val_q);
        val_i = ( (val_i&0x100)>>8 ) ? 0x100 | (256 - (val_i&0xff)) : val_i ;
        val_q = ( (val_q&0x100)>>8 ) ? 0x100 | (256 - (val_q&0xff)) : val_q ;

        if (swap)
        {
            status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6020_00, val_q);
            status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6022_00, val_i);
        }
        else
        {
            status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6020_00, val_i);
            status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6022_00, val_q);
        }


        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6010_00, &val_i);
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6012_00, &val_q);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6004_00, val_i);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_6006_00, val_q);

        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6014_00, &val_i);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_600A_00, val_i);
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_6016_00, &val_i);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_600E_00, val_i);

        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_00, 0);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_01, 0);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_02, 0);
        status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_6000_03, 0);
    }
    else
    {
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_00, 1);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_01, 1);
        status |= MT_WRITE_TOCACHE(pInfo, MT2081_FIELD_6000_02, 1);
        status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_6000_03, 1);
    }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_bbagc_freeze
**
**  Description:    Freeze BB AGC
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
static mt_uint32_t mt2081_bbagc_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint32_t bbagc;

    if (freeze == 1)
    {
        status |= MT2081_GetParam(pInfo->handle, MT2081_BBAGC_ATN, &bbagc);
        status |= MT2081_SetParam(pInfo->handle, MT2081_BBAGC_ATN, bbagc);
    }
    else
    {
        status |= MT2081_SetParam(pInfo->handle, MT2081_BBAGC_ATN, 0xffffffffU);
    }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_rfagc_freeze
**
**  Description:    Freeze RF AGC
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
static mt_uint32_t mt2081_rfagc_freeze(MT2081_Info_t *pInfo, mt_uint32_t freeze)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint32_t ln_atn;
    mt_uint32_t splt_atn;

    if (freeze == 1)
    {
        status |= MT2081_GetParam(pInfo->handle, MT2081_SPLT_ATN, &splt_atn);
        status |= MT2081_SetParam(pInfo->handle, MT2081_SPLT_ATN, splt_atn);

        status |= MT2081_GetParam(pInfo->handle, MT2081_LNA_ATN, &ln_atn);
        status |= MT2081_SetParam(pInfo->handle, MT2081_LNA_ATN, ln_atn);

        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__RF_AGC_EN, 0);
    }
    else
    {
        status |= MT2081_SetParam(pInfo->handle, MT2081_SPLT_ATN, 0xffffffffU);

        status |= MT2081_SetParam(pInfo->handle, MT2081_LNA_ATN, 0xffffffffU);

        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__RF_AGC_EN, 1);
   }

    return status;
}


/******************************************************************************
**
**  Name: mt2081_set_fout_bw
**
**  Description:    Freeze Output bandwidth
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
**   N/A   07-12-2010   JOH     7.8 MHz CHF coefficients from Michael S
**
******************************************************************************/
const mt_uint16_t BBF_rccalmax_6mhz[] = {  94,  95,  96,  97, 100, 101, 102, 103, 106, 107, 109, 111,
                                          113, 115, 118, 120, 123, 125, 128, 131, 135, 138, 143, 146, 154};
const mt_uint16_t BBF_code_6mhz[] =     {   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
                                           12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24};
const mt_uint16_t BBF_rccalmax_7mhz[] = { 101, 102, 103, 106, 107, 109, 111, 113, 115,
                                          118, 120, 123, 125, 128, 131, 135, 138, 143, 146 };
const mt_uint16_t BBF_code_7mhz[] =     {  10,  11,  12,  13,  14,  15,  16,  17,  18,
                                           19,  20,  21,  22,  23,  24,  25,  26,  27,  28 };
const mt_uint16_t BBF_rccalmax_8mhz[] = {  89,  92,  95,  97, 101, 104, 108, 112, 116,
                                          120, 125, 130, 136, 142, 149, 156, 164 };
const mt_uint16_t BBF_code_8mhz[] =     {  15,  16,  17,  18,  19,  20,  21,  22,  23,
                                           24,  25,  26,  27,  28,  29,  30,  31 };

const mt_uint16_t rescurve_dvbc_1p7mhz[64]={16383,16227,16046,16179,16378,16245,16048,16267,16230,14191,9507,4076,623,169,123,77,167,53,164,42,161,35,169,48,162,53,162,66,155,75,148,80,147,93,140,100,138,119,118,115,116,122,113,135,101,140,94,150,78,148,73,154,64,155,63,172,41,170,32,177,7,162,7,163};
const mt_uint16_t rescurve_j83b_5p8mhz[64]={16382,16208,16015,16167,16370,16236,16020,16136,16364,16273,16037,16107,16348,16298,16050,16077,16326,16316,16063,16053,16312,16339,16074,16032,16300,16336,16062,16111,16383,15329,11706,6526,2265,301,21,33,12,23,23,10,31,10,26,3,26,1,26,5,33,2,29,3,29,4,32,16,20,9,27,21,21,30,6,28};
const mt_uint16_t rescurve_dvbc_6mhz[64]={16151,16198,16236,16175,16144,16222,16259,16172,16136,16239,16277,16153,16104,16251,16321,16154,16053,16236,16383,16201,16005,16160,16380,16248,16015,16139,16378,16237,16015,16260,16221,14136,9569,4450,1120,34,5,17,35,6,23,10,17,20,14,25,11,27,8,31,11,24,5,44,16,26,3,31,4,32,5,30,5,26};
const mt_uint16_t rescurve_j83b_6mhz[64]={15995,16171,16375,16234,16017,16131,16366,16284,16034,16076,16331,16329,16079,16047,16289,16349,16113,16022,16246,16363,16148,16000,16202,16383,16205,16010,16176,16377,16195,16006,16275,16168,14029,9534,4529,1212,61,2,24,22,3,29,4,28,8,24,13,24,16,29,10,21,19,27,19,36,8,30,10,33,4,28,11,40};
const mt_uint16_t rescurve_dvbc_7mhz[64]={16285,16193,16088,16170,16298,16240,16097,16129,16284,16277,16113,16087,16255,16312,16143,16054,16225,16358,16196,16005,16129,16359,16288,16041,16069,16326,16341,16083,16039,16298,16352,16080,16078,16383,15612,12382,7337,2818,479,28,30,3,31,7,11,25,13,25,8,26,4,28,1,31,4,28,2,27,3,32,2,28,1,30};
const mt_uint16_t rescurve_dvbc_7p8mhz[64]={16008,16187,16382,16220,16013,16161,16383,16255,16020,16128,16375,16293,16039,16096,16350,16313,16057,16073,16333,16341,16082,16049,16302,16359,16117,16047,16286,16375,16140,16034,16260,16375,16146,16019,16253,16380,16126,16051,16369,15800,12799,7792,3111,582,19,27,5,31,11,23,18,16,28,24,19,15,28,28,16,27,9,23,9,29};
const mt_uint16_t rescurve_dvbc_8mhz[64]={16383,16199,16004,16172,16376,16218,16007,16154,16375,16240,16008,16127,16367,16266,16022,16109,16354,16278,16027,16089,16346,16302,16046,16077,16333,16311,16047,16052,16316,16329,16076,16062,16318,16330,16066,16056,16325,16306,16030,16158,16340,14818,10633,5345,1532,102,5,27,26,8,22,14,25,18,19,22,11,31,16,22,2,32,4,30};


static mt_uint32_t mt2081_set_fout_bw(MT2081_Info_t *pInfo, mt_uint32_t f_out_bw)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    mt_uint16_t idx;

    switch (f_out_bw)
    {
    case 8000000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_8mhz) / sizeof(BBF_rccalmax_8mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_8mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_8mhz) / sizeof(BBF_rccalmax_8mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_8mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_dvbc_8mhz[0], (mt_uint32_t)( sizeof(rescurve_dvbc_8mhz) / sizeof(rescurve_dvbc_8mhz[0]) ) );

        break;
    case 7800000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_8mhz) / sizeof(BBF_rccalmax_8mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_8mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_8mhz) / sizeof(BBF_rccalmax_8mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_8mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_dvbc_7p8mhz[0], (mt_uint32_t)( sizeof(rescurve_dvbc_7p8mhz) / sizeof(rescurve_dvbc_7p8mhz[0]) ) );

        break;
    case 7000000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_7mhz) / sizeof(BBF_rccalmax_7mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_7mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_7mhz) / sizeof(BBF_rccalmax_7mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_7mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_dvbc_7mhz[0], (mt_uint32_t)( sizeof(rescurve_j83b_6mhz) / sizeof(rescurve_dvbc_7mhz[0]) ) );
        break;
    case 6200000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_6mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_6mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_j83b_6mhz[0], (mt_uint32_t)( sizeof(rescurve_j83b_6mhz) / sizeof(rescurve_j83b_6mhz[0]) ) );
        break;
    case 6000000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_6mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_6mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_dvbc_6mhz[0], (mt_uint32_t)( sizeof(rescurve_dvbc_6mhz) / sizeof(rescurve_dvbc_6mhz[0]) ) );
        break;
    case 5800000:
        pInfo->f_out_bw = f_out_bw;

        for (idx = 0; idx < sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]); idx++)
        {
            if (pInfo->val_rc_cal <= BBF_rccalmax_6mhz[idx])
                break;
        }
        if(idx == sizeof(BBF_rccalmax_6mhz) / sizeof(BBF_rccalmax_6mhz[0]))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, BBF_code_6mhz[idx]);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_j83b_5p8mhz[0], (mt_uint32_t)( sizeof(rescurve_j83b_5p8mhz) / sizeof(rescurve_j83b_5p8mhz[0]) ) );
        break;
    case 1700000:
        pInfo->f_out_bw = f_out_bw;

        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_BBF_BW__VAL, 0);

        status |= mt2081_writeblock(pInfo, MT2081_REG_3300__ADDR_, &rescurve_dvbc_1p7mhz[0], (mt_uint32_t)( sizeof(rescurve_dvbc_6mhz) / sizeof(rescurve_dvbc_1p7mhz[0]) ) );
        break;
    default:
        /* If not a known BW setting, don't change anything and return an error.*/
        return (status | MT_ARG_RANGE);
    }

    status |= mt2081_update_chf(pInfo);

    return status;
}


/******************************************************************************
**
**  Name: mt2081_update_chf
**
**  Description:    Freeze Output bandwidth
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
static mt_uint32_t mt2081_update_chf(MT2081_Info_t *pInfo)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    mt_uint32_t reg_chf_resfreqfact;

    /* Set CHF rescaler */
    reg_chf_resfreqfact = ((pInfo->f_adcclk[pInfo->f_adcclk_idx] / 2) / (pInfo->f_ref / 10000)) * 16383;
    reg_chf_resfreqfact = reg_chf_resfreqfact / 10000;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_CHF_RESFREQFACT__VAL, (mt_uint16_t)reg_chf_resfreqfact);
    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_CHF_CNTRL__RESSTART, 1);

    return status;
}


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
MT2081_RESULT MT2081_SetParam(mt_handle_t h,
                             MT2081_Param param,
                              mt_uint32_t nValue)
{
    mt_uint32_t status = MT_OK; /* Status to be returned        */

    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    if (MT_NO_ERROR(status))
    {
        switch (param)
        {
        /*  crystal frequency                      */
        case MT2081_SRO_FREQ:
        {
            /* Limit range to avoid wrong states in frequency calculations */
            if (nValue < 24997500 || nValue > 25000000)
                status |= MT_ARG_RANGE;
            else
                pInfo->f_ref = nValue;
            break;
        }

        /*  output bandwidth                      */
        case MT2081_OUTPUT_BW:
        {
            status |= mt2081_set_fout_bw(pInfo,nValue);
            break;
        }

        /*  output frequency                      */
        case MT2081_OUTPUT_FREQ:
        {
            status |= mt2081_set_fout(pInfo, nValue);
            break;
        }

        /*  Receiver mode                         */
        case MT2081_RCVR_MODE:
        {
            switch (nValue)
            {
                case MT2081_CABLE_QAM:
                {
                    status |= mt2081_set_fout_bw(pInfo, 7800000);
                    status |= mt2081_set_fout(pInfo, 36125000);
                    break;
                }
                case MT2081_CABLE_QAM_NA:
                {
                    status |= mt2081_set_fout_bw(pInfo, 5800000);
                    status |= mt2081_set_fout(pInfo, 44000000);
                    break;
                }
                default:
                {
                    status |= MT_ARG_RANGE;
                    break;
                }
            }

            if (MT_NO_ERROR(status))
                pInfo->rcvr_mode = nValue;
            break;
        }

        /*  SA_PROFILE  */
        case MT2081_SA_PROFILE:
        {
            pInfo->sa_profile = nValue;
            break;
        }

        /*  RFLT enable/disable                   */
        case MT2081_RFLT_ENABLE:
        {
            if (nValue == 1)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__RFLT_DIS, 0);
            }
            else
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_SPLT_CNTRL__RFLT_DIS, 1);
            }
            break;
        }

        /*  IC standby                           */
        case MT2081_STANDBY:
        {
            if (nValue == 0)
            {
                /* Wake up tuner and SPLT */
                status |= mt2081_standby(pInfo, 0);

                if (MT_NO_ERROR(status))
                    pInfo->is_standby = 0;
            }
            else
            {
                status |= mt2081_standby(pInfo, 1); /* Full standby */

                /* status |= mt2081_standby(pInfo, 2); *//* Keep SPLT enabled */

                if (MT_NO_ERROR(status))
                    pInfo->is_standby = 1;
            }

            break;
        }

        /*  freeze AGC                            */
        case MT2081_AGC_FREEZE:
        {
            if (nValue == 1)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__FREEZE, 1);
                status |= mt2081_iqe_freeze(pInfo, 1);
                status |= mt2081_bbagc_freeze(pInfo, 1);
                status |= mt2081_rfagc_freeze(pInfo, 1);

                pInfo->agc_freeze = 1;
            }
            else
            {
                status |= mt2081_rfagc_freeze(pInfo, 0);
                status |= mt2081_bbagc_freeze(pInfo, 0);
                status |= mt2081_iqe_freeze(pInfo, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__FREEZE, 0);

                pInfo->agc_freeze = 0;
            }
            break;
        }


        /*  AGC acquistion mode enable           */
        case MT2081_AGC_ACQ:
        {
            if (nValue == 1)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__RF_AGC_EN, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__RF_AGC_EN, 1);
            }
            break;
        }

        /*  set Splitter attenuator in manual mode  (0xffffffff is auto)         */
        case MT2081_SPLT_ATN:
        {
            mt_uint16_t splt_atn;
            if (nValue == 0xffffffffU)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_SPLT_ATN_START__SPLT_ATN_START, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__SPLT_ATN_RST, 0);
            }
            else
            {
                splt_atn = (nValue & 0x03ff);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_SPLT_ATN_START__SPLT_ATN_START, splt_atn);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__SPLT_ATN_RST, 1);
            }
        break;
        }

        /*  set LNA attenuator in manual mode  (0xffffffff is auto)               */
        case MT2081_LNA_ATN:
        {
            mt_uint16_t lna_atn;
            if (nValue == 0xffffffffU)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_LNA_ATN_START__LNA_ATN_START, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__LNA_ATN_RST, 0);
            }
            else
            {
                lna_atn = (nValue & 0x03ff);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_LNA_ATN_START__LNA_ATN_START, lna_atn);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_RFA_RFAGCCONTROL__LNA_ATN_RST, 1);
            }
            break;
        }

        /*  set BBAGC in manual mode  (0xffffffff is auto)               */
        case MT2081_BBAGC_ATN:
        {
            mt_uint16_t bbagc_atn;
            if (nValue == 0xffffffffU)
            {
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_AGC_BB_GAIN_INIT__VAL, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_AGC_CFG__EN, 1);
            }
            else
            {
                bbagc_atn = (nValue & 0xffff);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_AGC_CFG__EN, 0);
                status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_AGC_BB_GAIN_INIT__VAL, bbagc_atn);
            }
            break;
        }

        /*  N1A Profile */
        case MT2081_N1A_PROFILE:
        {
            if (nValue > 2)
                break;

            switch (nValue)
            {
                case 0 :
                {
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3200_00, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3202_00, 1);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_IQ_MAG_TARGET__VAL,0x250);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400E_00,0x3fe);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4004_00,0x0e);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4006_00,2);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400C_00,0x65);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400A_00,0x3f);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4008_00,0x07);

                    if (IS_MT208XB0(pInfo))
                        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_401A_00,20);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE,1);
                    break;
                }
                case 1 :
                {
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3200_00, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3202_00, 1);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_IQ_MAG_TARGET__VAL,0x300);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400E_00,0x3fe);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4004_00,25);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4006_00,25);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400C_00,1);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400A_00,0x20);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4008_00,10);

                    if (IS_MT208XB0(pInfo))
                        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_401A_00,16);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE,1);
                    break;
                }
                case 2 : /* Preserve of profile 0 w/o hw avg */
                {
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3200_00, 0);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_3202_00, 1);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_IQ_MAG_TARGET__VAL,0x250);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400E_00,0x3fe);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4004_00,0x0e);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4006_00,2);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400C_00,0x65);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_400A_00,0x3f);
                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_4008_00,0x07);

                    if (IS_MT208XB0(pInfo))
                        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_401A_00,0);

                    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_N1A_CONTROL__ENABLE,1);
                    break;
                }

            }

            pInfo->n1a_profile = nValue;
            break;
        }

        /*  RSSI - Measured value at 50dBuV */
        case MT2081_RSSI_MEASURED:
        {
            pInfo->rssi_delta = pInfo->rssi_delta + (5000 - (mt_int32_t)nValue);
            break;
        }

        /*  These parameters are read-only  */
        case MT2081_VERSION:
        case MT2081_IC_ADDR:
        case MT2081_IC_REV:
        case MT2081_MAX_OPEN:
        case MT2081_NUM_OPEN:
        case MT2081_TEMPERATURE:
        case MT2081_LO_FREQ:
        case MT2081_INPUT_FREQ:
        case MT2081_VCO_FREQ4:
        case MT2081_F_ADCCLK:
        case MT2081_F_SYSCLK:
        case MT2081_N1A_PREAGC_LVL:
        case MT2081_RSSI:
        case MT2081_EOP:
        default:
            status |= MT_ARG_RANGE;
        }
    }

    return status;
}

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
MT2081_RESULT MT2081_SetReg(mt_handle_t h,
                            mt_uint16_t regaddr,
                            mt_uint16_t val)
{
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;
    mt_uint32_t status = MT_OK; /* Status to be returned        */
    mt_uint8_t buf[4];
    mt_uint16_t regnum = 0;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    status |= mt2081_regaddr2num(pInfo,regaddr, &regnum);

    if (regnum >= MT2081_REG__MAX || MT_IS_ERROR(status) )
        return (status | MT_ARG_RANGE);

    buf[0] = (regaddr & 0xff00) >> 8;
    buf[1] = regaddr & 0x00ff;
    buf[2] = (val & 0xff00) >> 8;
    buf[3] = val & 0x00ff;
    status |= MT_WriteRead(pInfo->hUserData, pInfo->address, &buf[0], 4, &buf[0], 0);

    if (MT_NO_ERROR(status))
        pInfo->reg[regnum] = val;

    return status;
}


/****************************************************************************
**
**  Name: fLO_FractionalTerm
**
**  Description:    Calculates the portion contributed by FracN / denom.
**
**                  This function preserves maximum precision without
**                  risk of overflow.  It accurately calculates
**                  f_ref * num / denom to within 1 HZ with fixed math.
**
**  Parameters:     num       - Fractional portion of the multiplier
**                  denom     - denominator portion of the ratio
**                              This routine successfully handles denom values
**                              up to and including 2^18.
**                  f_Ref     - SRO frequency.  This calculation handles
**                              f_ref as two separate 14-bit fields.
**                              Therefore, a maximum value of 2^28-1
**                              may safely be used for f_ref.  This is
**                              the genesis of the magic number "14" and the
**                              magic mask value of 0x03FFF.
**
**  Returns:        f_ref * num / denom
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
static mt_uint32_t fLO_FractionalTerm(mt_uint32_t f_ref,
                                      mt_uint32_t num,
                                      mt_uint32_t denom )
{
    mt_uint32_t t1 = (f_ref >> 14) * num;
    mt_uint32_t term1 = t1 / denom;
    mt_uint32_t loss = t1 % denom;
    mt_uint32_t term2 = (((f_ref & 0x00003FFF) * num + (loss << 14)) + (denom / 2)) / denom;
    return ((term1 << 14) + term2);
}


/****************************************************************************
**
**  Name: CalcLOMult
**
**  Description:    Calculates Integer divider value and the numerator
**                  value for a FracN PLL.
**
**                  This function assumes that the f_LO and f_Ref are
**                  evenly divisible by f_LO_Step.
**
**  Parameters:     lo_div_ret   - OUTPUT: Whole number portion of the multiplier
**                  lo_fracn_ret - OUTPUT: Fractional portion of the multiplier
**                  f_lo         - desired LO frequency.
**                  f_ref        - SRO frequency.
**                  lo_denom     - LO denominator (8191 or 8192)
**
**  Returns:        Recalculated LO frequency.
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
static mt_uint32_t CalcLOMult(mt_uint16_t *lo_div_ret,
                              mt_uint16_t *lo_fracn_ret,
                              mt_uint32_t f_lo,
                              mt_uint32_t f_ref,
                              mt_uint32_t lo_denom,
                              mt_uint32_t fracn_avoid_zone
                             )
{
    mt_uint32_t lo_div = 0;
    mt_uint32_t lo_fracn = 0;

    /* Calculate FracN value */
    lo_fracn =  ( ( (f_lo % f_ref)/50 ) * lo_denom  )  / (f_ref / 500);
    lo_fracn = (lo_fracn + 5)/10;

    /* Overflow handling */
    if (lo_fracn >= lo_denom)
    {
        lo_div = lo_fracn / lo_denom;
        lo_fracn = lo_fracn % lo_denom;
    }

    /* Apply FracN keepout area */
    if (  (lo_fracn % FRACN_AVOID_MODULUS) < (fracn_avoid_zone)  )
        lo_fracn += ( (fracn_avoid_zone) - (lo_fracn % FRACN_AVOID_MODULUS) );
    else if (  (lo_fracn % FRACN_AVOID_MODULUS) > (FRACN_AVOID_MODULUS - (fracn_avoid_zone) )-1  )
        lo_fracn -= ( (fracn_avoid_zone) - (FRACN_AVOID_MODULUS - (lo_fracn % FRACN_AVOID_MODULUS) ) ) + 1;

    /*  Calculate the whole number portion of the divider */
    lo_div += f_lo / f_ref;

    *lo_div_ret = (mt_uint16_t)lo_div;
    *lo_fracn_ret = (mt_uint16_t)lo_fracn;

    return (f_ref * (lo_div)) + fLO_FractionalTerm(f_ref, lo_fracn, lo_denom);
}

/****************************************************************************
**
**  Name: CheckADCCLKHarmonics
**
**  Description:  Calculates the harmonics of the ADCCLK
**
**  Parameters:   *pInfo                 - Datastructure of actual tuner.
**                f_test                 - frequency to test on harmonics
**                dc_test                - If dc-case should be enabled as non-spur
**                f_adcclk_first_idx     - first entry in this array to use
**                f_adcclk_last_idx      - last entry in this array to use
**                max_harmonic           - number of harmonics to consider
**
**                *best_f_adcclk_idx     - array entry suggested to use
**                                         (Output)
**                *best_f_adcclk_harmonic- closest harmonic to f_in
**                                         (Output)
**                *best_f_adcclk_dist    - Distance of harmonic to f_in
**                                         (Output)
**
**  Returns:      status:
**                   MT_OK            - No errors
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
static mt_uint32_t CheckADCCLKHarmonics(MT2081_Info_t *pInfo,
                                        mt_uint32_t f_test,
                                        mt_uint8_t dc_test,
                                        mt_uint8_t f_adcclk_first_idx,
                                        mt_uint8_t f_adcclk_last_idx,
                                        mt_uint8_t max_harmonic,
                                        mt_uint8_t* best_f_adcclk_idx,
                                        mt_uint8_t* best_f_adcclk_harmonic,
                                        mt_uint32_t* best_f_adcclk_dist )
{
    mt_uint8_t i;
    mt_uint8_t k;
    mt_uint32_t min_dist = 0;
    mt_uint8_t min_dist_harmonic  = 0;
    mt_uint8_t dc_case = 0;


    if (best_f_adcclk_idx != NULL)
        *best_f_adcclk_idx = 0;
    if (best_f_adcclk_harmonic != NULL)
        *best_f_adcclk_harmonic = 0;

    *best_f_adcclk_dist = 0;

    for (i = f_adcclk_first_idx; i <= f_adcclk_last_idx; i++)
    {
        min_dist = (((mt_uint32_t)MAX_UDATA) / 2) - 1;
        /* Search trough all harmonics and find the closest to f_in */
        for (k = 0; k <= max_harmonic; k++)
        {
            /* If the harmonic already bigger than f_in+margin -> break */
            if (pInfo->f_adcclk[i] * k > (f_test + pInfo->f_adcclk[i]))
                break;
            dc_case = 0;
            if ( dc_test )
                if ( (pInfo->sysclkdiv[pInfo->adc_sysclk_idx[i]] * pInfo->adcclkdiv[i]) == (k * 4 * pInfo->vco_div) )
                    dc_case = 1;
            /* Find the "closest harmonic" for this f_adcclk[i] */
            if ( ((mt_uint32_t)( abs((mt_int32_t)(f_test - pInfo->f_adcclk[i] * k) )) < min_dist) && (dc_case == 0) )
            {
                min_dist = (mt_uint32_t)( abs((mt_int32_t)( f_test - pInfo->f_adcclk[i] * k ) ) );
                min_dist_harmonic = k;
            }
        }

        /* If this closest harmonic for this adcclkdiv/sysclkdiv is farest */
        /* from f_in, take this setting */
        if (min_dist >= *best_f_adcclk_dist)
        {
            if (best_f_adcclk_idx != NULL)
                *best_f_adcclk_idx = i;
            if (best_f_adcclk_harmonic != NULL)
                *best_f_adcclk_harmonic = min_dist_harmonic;

            *best_f_adcclk_dist = min_dist;
        }
    }
    return MT_OK;
}

/****************************************************************************
**
**  Name: mt2081_set_lnacfg
**
**  Description:  Set LNA configuration.
**
**  Parameters:   *pInfo                 - Datastructure of actual tuner.
**                f_int                  - RF input center frequency (in Hz).
**
**  Returns:      status:
**                   MT_OK            - No errors
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
    /* Takeover frequencies for CT filter */
    const mt_uint16_t CT_VHF_Takeover[] =  {       0,    0,    0,    0,    0,    0,    0,
                                             0,    0,    0,  500,  540,  560,  600,  640,
                                           800,  820,  900,  940, 1040, 1080, 1120, 1200,
                                          1380, 1420, 1580, 1740, 2080, 2220, 2620 };
    const mt_uint16_t CT_UHF_Takeover[] =  {3320, 3400, 3520, 3830, 4170, 4620, 5120, 5590,
                                            6020, 6420, 6920, 7430, 7930, 8390, 8750 };

    const mt_uint16_t HRF_Pole_Takeover[] = { 9840, 8120, 6980, 6050, 5390, 0};

    const mt_uint16_t HRF_CAP_RCCAL[] =      {  0,   98, 104, 109, 115, 121, 127, 133, 140, 146};
    const mt_uint16_t HRF_CAP_SH[] =         {  55, 110, 104,  98,  93,  88,  84,  80,  76,  73,
                                                60, 100,  94,  89,  85,  80,  76,  73,  69,  66,
                                                65,  91,  86,  82,  77,  73,  70,  66,  63,  60,
                                                70,  84,  79,  75,  71,  68,  64,  61,  58,  56,
                                                75,  78,  73,  69,  66,  62,  59,  57,  54,  51,
                                                80,  72,  68,  65,  61,  58,  55,  52,  50,  48,
                                                85,  67,  64,  60,  57,  54,  51,  49,  47,  44,
                                                90,  63,  60,  56,  53,  51,  48,  46,  44,  42,
                                                95,  59,  56,  53,  50,  48,  45,  43,  41,  39,
                                               100,  56,  53,  50,  47,  45,  42,  40,  38,  37,
                                               110,  50,  47,  44,  42,  40,  38,  36,  34,  33,
                                               120,  45,  42,  40,  38,  36,  34,  32,  31,  29,
                                               130,  41,  38,  36,  34,  32,  31,  29,  28,  27,
                                               140,  37,  35,  33,  31,  29,  28,  27,  25,  24,
                                               150,  34,  32,  30,  28,  27,  26,  24,  23,  22,
                                               160,  31,  29,  28,  26,  25,  23,  22,  21,  20,
                                               170,  29,  27,  25,  24,  23,  22,  20,  19,  19,
                                               180,  27,  25,  24,  22,  21,  20,  19,  18,  17,
                                               190,  25,  23,  22,  21,  19,  18,  17,  17,  16,
                                               200,  23,  22,  20,  19,  18,  17,  16,  15,  15,
                                               220,  20,  19,  18,  17,  16,  15,  14,  13,  13,
                                               240,  17,  16,  15,  14,  14,  13,  12,  12,  11,
                                               260,  15,  14,  13,  13,  12,  11,  11,  10,  10,
                                               280,  13,  13,  12,  11,  10,  10,   9,   9,   8,
                                               300,  12,  11,  10,  10,   9,   9,   8,   8,   7,
                                               320,  11,  10,   9,   9,   8,   8,   7,   7,   6,
                                               340,   9,   9,   8,   8,   7,   7,   6,   6,   6,
                                               360,   8,   8,   7,   7,   6,   6,   5,   5,   5,
                                               380,   7,   7,   6,   6,   5,   5,   5,   4,   4,
                                               400,   6,   6,   5,   5,   5,   4,   4,   4,   4,
                                               420,   6,   5,   5,   4,   4,   4,   4,   3,   3,
                                               440,   5,   5,   4,   4,   4,   3,   3,   2,   2,
                                               460,   4,   4,   4,   3,   2,   2,   2,   2,   2,
                                               480,   4,   3,   3,   2,   2,   2,   2,   1,   1,
                                               500,   3,   2,   2,   2,   2,   1,   1,   1,   1 };

    const mt_uint16_t HRF_CAP_FB[] =        {    55, 127, 127, 127, 127, 122, 116, 111, 105, 100,
                                                 60, 127, 127, 124, 118, 112, 106, 101,  96,  92,
                                                 65, 127, 121, 115, 108, 103,  98,  93,  88,  84,
                                                 70, 119, 112, 106, 100,  95,  90,  86,  82,  78,
                                                 75, 111, 104,  99,  93,  88,  84,  80,  76,  72,
                                                 80, 103,  97,  92,  87,  83,  78,  74,  71,  67,
                                                 85,  97,  91,  86,  82,  77,  73,  70,  66,  63,
                                                 90,  91,  86,  81,  77,  73,  69,  66,  62,  59,
                                                 95,  86,  81,  76,  72,  69,  65,  62,  59,  56,
                                                100,  81,  77,  72,  68,  65,  61,  58,  55,  53,
                                                110,  73,  69,  65,  61,  58,  55,  52,  50,  47,
                                                120,  66,  63,  59,  56,  53,  50,  47,  45,  43,
                                                130,  61,  57,  54,  51,  48,  45,  43,  41,  39,
                                                140,  56,  52,  49,  47,  44,  42,  39,  37,  36,
                                                150,  51,  48,  45,  43,  40,  38,  36,  34,  33,
                                                160,  48,  45,  42,  40,  37,  35,  33,  32,  30,
                                                170,  44,  41,  39,  37,  35,  33,  31,  29,  28,
                                                180,  41,  39,  36,  34,  32,  30,  29,  27,  26,
                                                190,  38,  36,  34,  32,  30,  28,  27,  25,  24,
                                                200,  36,  34,  32,  30,  28,  26,  25,  24,  22,
                                                220,  32,  30,  28,  26,  25,  23,  22,  21,  19,
                                                240,  28,  26,  25,  23,  22,  20,  19,  18,  17,
                                                260,  25,  23,  22,  20,  19,  18,  17,  16,  15,
                                                280,  22,  21,  19,  18,  17,  16,  15,  14,  13,
                                                300,  20,  19,  17,  16,  15,  14,  13,  12,  11,
                                                320,  18,  17,  15,  14,  13,  12,  12,  11,  10,
                                                340,  16,  15,  14,  13,  12,  11,  10,   9,   9,
                                                360,  14,  13,  12,  11,  10,  10,   9,   8,   8,
                                                380,  13,  12,  11,  10,   9,   8,   8,   7,   7,
                                                400,  12,  11,  10,   9,   8,   7,   7,   6,   6,
                                                420,  10,   9,   8,   8,   7,   6,   6,   5,   5,
                                                440,   9,   8,   7,   7,   6,   5,   5,   4,   3,
                                                460,   8,   7,   6,   6,   4,   4,   3,   3,   3,
                                                480,   7,   6,   5,   4,   4,   3,   3,   2,   2,
                                                500,   6,   5,   4,   3,   3,   2,   2,   2,   1 };

#define HRF_CAP_RCCAL_POINTS sizeof(HRF_CAP_RCCAL) / sizeof(HRF_CAP_RCCAL[0])
#define HRF_CAP_SH_FREQS     sizeof(HRF_CAP_SH) / sizeof(HRF_CAP_RCCAL)
#define HRF_CAP_FB_FREQS     sizeof(HRF_CAP_FB) / sizeof(HRF_CAP_RCCAL)

static mt_uint32_t mt2081_set_lnacfg(MT2081_Info_t *pInfo, mt_uint32_t f_in)
{
    mt_uint32_t status = MT_OK;
    mt_uint16_t ct_filt = 0;
    mt_uint16_t hrf_row = 0;
    mt_uint16_t hrf = 0;
    mt_uint16_t hrf_line = 0;


    /*
    ** Find and write CT setting, select LNA input
    */
    if (f_in > VHF_FIN_FREQ)
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__BAND, 2);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__INPUT, 1);

        for (ct_filt = 0; ct_filt < (sizeof(CT_UHF_Takeover) / sizeof(CT_UHF_Takeover[0])); ct_filt++)
        {
            if (f_in / 100000 < CT_UHF_Takeover[ct_filt])
                break;
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_BAND_SEL__LB_CT, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_BAND_SEL__HB_CT, ct_filt);
    }
    else
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_CFG__BAND, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__INPUT, 0);

        for (ct_filt = 0; ct_filt < (sizeof(CT_VHF_Takeover) / sizeof(CT_VHF_Takeover[0])); ct_filt++)
        {
            if (f_in / 100000 < CT_VHF_Takeover[ct_filt])
                break;
        }

        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_BAND_SEL__LB_CT, ct_filt);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_LNA_BAND_SEL__HB_CT, 0);
    }

    /*
    ** Set HRF configuration
    */

    if (f_in > 500000000)
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__BYPASS, 1);

        for (hrf = 0; hrf < (sizeof(HRF_Pole_Takeover) / sizeof(HRF_Pole_Takeover[0])); hrf++)
        {
            if (f_in / 100000 > HRF_Pole_Takeover[hrf])
                break;
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__POLE, hrf);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CAP__SH, 0 );
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CAP__FB, 0 );
    }
    else
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__BYPASS, 0);
        for (hrf_row = 1; hrf_row < HRF_CAP_RCCAL_POINTS; hrf_row++)
        {
            if (pInfo->val_rc_cal <= HRF_CAP_RCCAL[hrf_row])
                break;

        }
        for (hrf_line = 0; hrf_line < HRF_CAP_SH_FREQS; hrf_line++)
        {
            if (f_in / 1000000 <= HRF_CAP_SH[hrf_line * HRF_CAP_RCCAL_POINTS])
                break;
        }
        if((hrf_line*HRF_CAP_RCCAL_POINTS + hrf_row) >= (HRF_CAP_RCCAL_POINTS * HRF_CAP_SH_FREQS))
        {
            return (status | MT_ARG_RANGE);
        }
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CNTRL__POLE, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CAP__SH, HRF_CAP_SH[hrf_line*HRF_CAP_RCCAL_POINTS+hrf_row] );
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP1_CAP__FB, HRF_CAP_FB[hrf_line*HRF_CAP_RCCAL_POINTS+hrf_row] );

    }

    return status;
}


/****************************************************************************
**
**  Name: mt2081_set_lofreq
**
**  Description:  Set LO frequency.
**
**  Parameters:   *pInfo                 - Datastructure of actual tuner.
**                f_int                  - RF input center frequency (in Hz).
**
**                *islocked              - LO lock status
**                                         (Output)
**
**  Returns:      status:
**                   MT_OK            - No errors
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
static mt_uint32_t mt2081_set_lofreq(MT2081_Info_t *pInfo, mt_uint32_t f_lo, mt_uint32_t *islocked)
{
    mt_uint32_t status = MT_OK;

    mt_uint16_t vco_div = 0;
    mt_uint16_t reg_vco_div = 0;
    mt_uint16_t reg_lo_n_div;
    mt_uint16_t reg_lo_fracn_num;
    mt_uint16_t reg_lo_denom = 0;
    mt_uint16_t reg_csel_a = 0;
    mt_uint16_t reg_csel_b = 0;

    for (vco_div = 1; vco_div <= 16; vco_div = vco_div * 2)
    {
        if (((mt_uint32_t)(f_lo * vco_div) >= 625000000) && ((mt_uint32_t)(f_lo * vco_div) <= 1250000000))
        {
            status &= ~MT_ARG_RANGE;
            break;
        }
        else
            status |= MT_ARG_RANGE;
        reg_vco_div++;
    }

    if (MT_IS_ERROR(status))
        return status;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP2_VCO_DIV__VAL, (mt_int16_t)reg_vco_div);

    if (MT_IS_ERROR(status))
        return status;

    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_LO_FN_CONFIG__DMAX, &reg_lo_denom);


    pInfo->f_vco4 = CalcLOMult(&reg_lo_n_div, &reg_lo_fracn_num,
                            f_lo * vco_div , pInfo->f_ref/2, (8191 + reg_lo_denom), pInfo->fracn_avoid_zone);

    pInfo->f_lo = pInfo->f_vco4 / vco_div;
    pInfo->vco_div = (mt_uint8_t)vco_div;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_LO_DIV_N_DIV__VAL, (mt_int16_t)reg_lo_n_div);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ANLG_LO_FRACN_NUM__VAL, (mt_int16_t)reg_lo_fracn_num);

    status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__FN_LD, 1);

    status |=  MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__CAP_RST, 1);

    /*
    **  Check for LO's locking
    */
    status |= MT2081_GetLocked(pInfo->handle, islocked);

    if (*islocked != 1)
        status |= MT_DNC_UNLOCK;

    if (MT_IS_ERROR(status))
        return status;

    /* Enable automatic adjustment of CSEL/RSEL */
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10E_09, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_E10E_10, 1);

    /* Wait 5ms until capselect finally is settled */
    MT2081_Sleep(pInfo->hUserData, 5);
    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL_RD__VAL, &reg_csel_a);

    if (reg_csel_a >= 123 && reg_csel_a <= 127)
    {
        reg_csel_b = 140;
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL__VAL, reg_csel_b);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__CAP_RST, 1);

        status |= MT2081_GetLocked(pInfo->handle, islocked);

        if (*islocked != 1)
            status |= MT_DNC_UNLOCK;

        if (MT_IS_ERROR(status))
            return status;

        /* Wait 5ms until capselect finally is settled */
        MT2081_Sleep(pInfo->hUserData, 5);
    }
    else if (reg_csel_a >= 128 && reg_csel_a <= 132)
    {
        reg_csel_b = 0;
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL__VAL, reg_csel_b);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__CAP_RST, 1);

        status |= MT2081_GetLocked(pInfo->handle, islocked);

        if (*islocked != 1)
            status |= MT_DNC_UNLOCK;

        if (MT_IS_ERROR(status))
            return status;

        /* Wait 5ms until capselect finally is settled */
        MT2081_Sleep(pInfo->hUserData, 5);
    }
    else
        status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL__VAL, &reg_csel_b);

    status |= MT_READ_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL_RD__VAL, &reg_csel_a);

    if ( (reg_csel_a&0x80) != (reg_csel_b&0x80) )
    {
        if (reg_csel_a & 0x80)
            reg_csel_b = 140;
        else
            reg_csel_b = 0;

        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ACP2_LO_CSEL__VAL, reg_csel_b);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ANLG_ONE_SHOTS__CAP_RST, 1);

        status |= MT2081_GetLocked(pInfo->handle, islocked);

        if (*islocked != 1)
            status |= MT_DNC_UNLOCK;

        if (MT_IS_ERROR(status))
            return status;
    }

    return status;
}

/****************************************************************************
**
**  Name: mt2081_set_fout
**
**  Description:  Set LO frequency.
**
**  Parameters:   *pInfo                 - Datastructure of actual tuner.
**                f_int                  - RF input center frequency (in Hz).
**
**                *islocked              - LO lock status
**                                         (Output)
**
**  Returns:      status:
**                   MT_OK            - No errors
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
static mt_uint32_t mt2081_set_fout(MT2081_Info_t *pInfo, mt_uint32_t f_out)
{
    mt_uint32_t status = MT_OK;

    mt_uint32_t i = 0;
    mt_uint32_t reg_ncod_inc = 0;
    mt_uint32_t reg_ncod_inc_lsb = 0;
    mt_uint16_t reg_ncod_gain = 0;

    const mt_uint16_t lut_ncod_inc[32] = { 0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700,
                                          0x0800, 0x0900, 0x0a00, 0x0b00, 0x0c00, 0x0d00, 0x0e00, 0x0f00,
                                          0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700,
                                          0x1800, 0x1900, 0x1a00, 0x1b00, 0x1c00, 0x1d00, 0x1e00, 0x1f00 };

    const mt_uint16_t lut_nco_gain8[32]  = { 0x82, 0x82, 0x82, 0x82, 0x81, 0x81, 0x80, 0x7f,
                                             0x7e, 0x7d, 0x7c, 0x7a, 0x79, 0x78, 0x77, 0x75,
                                             0x74, 0x73, 0x71, 0x70, 0x6f, 0x6e, 0x6d, 0x6c,
                                             0x6b, 0x6b, 0x6a, 0x6a, 0x69, 0x69, 0x68, 0x68 };

    const mt_uint16_t lut_nco_gain10[32] = { 0xab, 0xab, 0xab, 0xaa, 0xaa, 0xa9, 0xa8, 0xa6,
                                             0xa5, 0xa4, 0xa2, 0xa1, 0x9f, 0x9d, 0x9b, 0x9a,
                                             0x98, 0x96, 0x95, 0x93, 0x92, 0x91, 0x8f, 0x8e,
                                             0x8d, 0x8c, 0x8b, 0x8a, 0x8a, 0x89, 0x89, 0x89 };

    /*  Range check */
    if (f_out >= MIN_FOUT_FREQ && f_out <= MAX_FOUT_FREQ)
    {
        pInfo->f_out = f_out;
    }
    else
        status |= MT_FOUT_RANGE;

    if (MT_IS_ERROR(status))
        return status;

    /* Calc NCOD */
    reg_ncod_inc = (f_out << 6);
    reg_ncod_inc = reg_ncod_inc / (pInfo->f_sysclk[pInfo->f_sysclk_idx] >> 13);
    reg_ncod_inc = reg_ncod_inc << 9;
    reg_ncod_inc_lsb = reg_ncod_inc & 0x3fff;
    reg_ncod_inc = reg_ncod_inc >> 14;

    /* Calc NCOD_GAIN */
    for (i=0;i< (sizeof(lut_ncod_inc) / sizeof(lut_ncod_inc[0]));i++)
        if (lut_ncod_inc[i] >= reg_ncod_inc)
            break;
    if(i == (sizeof(lut_ncod_inc) / sizeof(lut_ncod_inc[0])))
    {
        return (status | MT_ARG_RANGE);
    }
    reg_ncod_gain = (mt_uint16_t)(lut_nco_gain10[i] << 8 | lut_nco_gain8[i]);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_NCOD_CTRL__EN, 1);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_NCOD_INC__VAL, (mt_uint16_t)reg_ncod_inc);

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_REG_NCOD_GAIN, reg_ncod_gain);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_NCOD_INC_LSB__VAL, (mt_uint16_t)reg_ncod_inc_lsb);

    return status;
}



/******************************************************************************
**
**  Name: mt2081_enable_dfe
**
**  Description:    Enable signal processing
**
**  Parameters:     h           - Tuner handle (returned by MT2081_Open)
**                                0 - on
**                                1 - off
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
static mt_uint32_t mt2081_enable_dfe(MT2081_Info_t *pInfo, mt_uint32_t enable)
{
    mt_uint32_t status = MT_OK;

    if (enable == 1)
    {
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, 1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_DPCTRL__DVALID_DISABLE, 0);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_ADCCTRL__UPF_INP_SEL,0);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_ADCCTRL__INP_SEL,0);
    }
    else
    {
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_ADCCTRL__INP_SEL,2);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_ADCCTRL__UPF_INP_SEL,1);
        status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_DPCTRL__DVALID_DISABLE, 1);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__SRO_EN_FCLK, 1);
        status |= MT_WRITE_CACHED(pInfo, MT2081_FIELD_ASYNC_CLOCKCONTROL__LO_SELECT, 0);
    }

    return status;
}

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
**                  MT2081_Sleep         - Delay execution for x milliseconds
**                  MT2081_GetLocked - Checks to see if LO1 and LO2 are locked
**
**  Revision History:
**
**   SCR      Date      Author  Description
**  -------------------------------------------------------------------------
**   N/A   05-12-2010   PINZ    Ver 1.00: Initial.
**
****************************************************************************/
MT2081_RESULT MT2081_Tune(mt_handle_t h, mt_uint32_t f_in)
{
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    mt_uint32_t status = MT_OK; /*  status of operation             */
    mt_uint32_t i = 0;
    mt_uint32_t f_sysclktemp;
    mt_uint8_t f_sysclk_num = 0;
    mt_uint8_t f_adcclk_num = 0;
    mt_uint32_t max_dist = 0;
    mt_uint8_t max_dist_adcclk_idx = 0;
    mt_uint8_t max_dist_harmonic = 0;
    /* mt_uint8_t spur_in_band = 0; */
    mt_uint32_t islocked = 0;
    mt_uint16_t reg_adc_div;
    mt_uint16_t reg_sysclk_div;

    const mt_uint16_t sysclk_div_mapping_table[28] = {  0,  1,  2,  3,  4,  5,  6,  7,
                                                        8,  8,  9, 10, 11, 13, 14, 15,
                                                       17, 18, 19, 21, 22, 23, 25, 26,
                                                       27, 29, 30, 31 };


    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        status |= MT_INV_HANDLE;

    /*  Range check */
    if (MT_NO_ERROR(status) && f_in >= MIN_FIN_FREQ && f_in <= MAX_FIN_FREQ)
    {
        pInfo->f_in = f_in;
    }
    else
        status |= MT_FIN_RANGE;

    if (MT_IS_ERROR(status))
        return status;

    /*
    ** Switch back to FCLK
    */
    status |= mt2081_enable_dfe(pInfo, 0);

    if (MT_IS_ERROR(status))
        return status;

    status |= mt2081_set_lnacfg(pInfo, f_in);

    /*
    **  Set LO frequency
    */
    if (MT_NO_ERROR(status))
    {
        status |= mt2081_set_lofreq(pInfo, f_in, &islocked);
    }

    /* Search for sysclk frequencies within the limits */
    for (i = 10; i <= 25; i++)
    {
        f_sysclktemp = (pInfo->f_vco4 / i);
        if ( (f_sysclktemp >= MIN_F_SYSCLK / 4)
          && (f_sysclktemp <= MAX_F_SYSCLK / 4)
          && (f_sysclk_num < F_SYSCLK_CNT) )
        {
            if ( ( (f_sysclktemp+1) * i) - pInfo->f_vco4 < pInfo->f_vco4 - (f_sysclktemp * i) )
                pInfo->f_sysclk[f_sysclk_num] = (f_sysclktemp+1) * 4;
            else
                pInfo->f_sysclk[f_sysclk_num] = f_sysclktemp * 4; /* *4 since f_vco4 is /4 */

            pInfo->sysclkdiv[f_sysclk_num] = (mt_uint16_t)i;
            f_sysclk_num++;
        }
    }
    /* no suitable sysclkdiv values found */
    if (f_sysclk_num == 0)
        return (status | MT_ARG_RANGE);

    /* Search for adcclk frequencies within the limits */
    for (i = 0; i < f_sysclk_num; i++)
    {
        if  (  (pInfo->f_sysclk[i] >= MIN_F_ADCCLK* 4) && (pInfo->f_sysclk[i] <= MAX_F_ADCCLK* 4)
           && ((pInfo->f_sysclk[i] >= ( (2 * pInfo->f_ref) + GAP_F_ADCCLK)* 4) || (pInfo->f_sysclk[i] <= ( (2 * pInfo->f_ref) - GAP_F_ADCCLK)* 4))
           && f_adcclk_num < F_ADCCLK_CNT)
        {
            pInfo->f_adcclk[f_adcclk_num] = pInfo->f_sysclk[i] / 4;
            pInfo->adc_sysclk_idx[f_adcclk_num] = (mt_uint8_t)i;
            pInfo->adcclkdiv[f_adcclk_num] = 4;
            f_adcclk_num++;
        }
        if (  (pInfo->f_sysclk[i] >= MIN_F_ADCCLK* 5) && (pInfo->f_sysclk[i] <= MAX_F_ADCCLK* 5)
          && ((pInfo->f_sysclk[i] >= ( (2 * pInfo->f_ref) + GAP_F_ADCCLK)* 5) || (pInfo->f_sysclk[i] <= ( (2 * pInfo->f_ref) - GAP_F_ADCCLK)* 5))
          && f_adcclk_num < F_ADCCLK_CNT)
        {
            pInfo->f_adcclk[f_adcclk_num] = pInfo->f_sysclk[i] / 5;
            pInfo->adc_sysclk_idx[f_adcclk_num] = (mt_uint8_t)i;
            pInfo->adcclkdiv[f_adcclk_num] = 5;
            f_adcclk_num++;
        }
    }

    /* no usable adcclkdiv solutions found */
    if (f_adcclk_num == 0)
        return (status | MT_ARG_RANGE);

    /* Evaluate ADCCLK harmonics and return best result */
    /*
    **  Algorithm for one tuner:
    **                +----------------------------------------------->f
    **  f_adcclk[1]*k |  |        |        +         |         |
    **  f_adcclk[2]*k |          |                +                 |
    **  f_adcclk[3]*k |    |                              *
    **                |                  [...|...]
    **                |              fin +- f_out_bw/2
    **
    **  | = harmonic spur
    **  + = "closest harmonic" to f_in for ONE f_adcclk[] value
    **  * = farest "closest harmonic" of ALL f_adcclk[] values -> thats our result
    */
    status |= CheckADCCLKHarmonics(pInfo, pInfo->f_lo, 1 , 0, f_adcclk_num - 1, MAX_HARMONIC, &max_dist_adcclk_idx, &max_dist_harmonic, &max_dist);

    /* Check if spur is in band of signal */
    /*
    if (pInfo->f_adcclk[max_dist_adcclk_idx] * max_dist_harmonic >= f_in - (pInfo->f_out_bw) / 2 && pInfo->f_adcclk[max_dist_adcclk_idx] * max_dist_harmonic <= f_in + (pInfo->f_out_bw) / 2)
        spur_in_band = 1;
    else
        spur_in_band = 0;
    */

    /* Set values to registers */
    pInfo->f_adcclk_idx = max_dist_adcclk_idx;
    pInfo->f_sysclk_idx = pInfo->adc_sysclk_idx[max_dist_adcclk_idx];

    if (pInfo->adcclkdiv[pInfo->f_adcclk_idx] == 4)
        reg_adc_div = 1;
    else
        reg_adc_div = 0;

    /* Mapping table */
    reg_sysclk_div = sysclk_div_mapping_table[pInfo->sysclkdiv[pInfo->f_sysclk_idx]];

    if (MT_IS_ERROR(status))
        return status;

    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_ACP2_LO_CLK_CNTRL__SYSCLK_DIV, reg_sysclk_div);
    status |= MT_WRITE_ONDEMAND(pInfo, MT2081_FIELD_COM_DPCTRL__OSR_8_10, reg_adc_div);

    status |= mt2081_update_chf(pInfo);
    status |= mt2081_set_fout(pInfo, pInfo->f_out);

    if (MT_IS_ERROR(status))
        return status;

    /*
    **  If the LO is locked, enable LO clock for digital parts
    */
    if (islocked)
    {
        status |= mt2081_enable_dfe(pInfo, 1);
    }

    return status;
}

#if defined(MT2081_INCLUDE_POWER_REPORTING)

mt_uint32_t MT2081_GetMasterGCTable(mt_handle_t h, MT2081_GainCal_t **table)
{
    MT2081_Info_t* pInfo = (MT2081_Info_t*) h;

    /*  Verify that the handle passed points to a valid tuner         */
    if (IsValidHandle(pInfo) == 0)
        return MT_INV_HANDLE;

    *table = &pInfo->gaincal_table;

    return MT_OK;
}

#endif


