/**
 * Copyright (C) 2005 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          tmbslHCDP_cfg.h
 *
 * \date          29-Jul-2013
 *
 * \brief         -
 *
 * REFERENCE DOCUMENTS :
 *
 * Detailled description may be added here.
 *
 * \section info Change Information
 *
 * \verbatim
   Date          Modified by CRPRNr  TASKNr  Maintenance description
   -------------|-----------|-------|-------|-----------------------------------
   29-July-2013 | B.Lemesle |       |       | Creation
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/


#ifndef TMBSLHCDP_CFG_H 
#define TMBSLHCDP_CFG_H


/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/

/*============================================================================*/
/* Generic Configuration items                                                */
/*============================================================================*/

/*============================================================================*/
/* macro define                                                               */
/*============================================================================*/

/*============================================================================*/
/* bsl type define                                                            */
/*============================================================================*/

/*============================================================================*/
/* SET current configuration HERE                                             */
/*============================================================================*/
/* sampling clock */
#define TMBSLHCDP_CFG_SAMPLING_CLOCK       56250000
#define TMBSLHCDP_CFG_DCLOCK               56250000

/* specific config for demod */
#define HCDP_CFG_CTLACQGAIN_64QAM          0x84
#define HCDP_CFG_CTLACQGAIN_128QAM         0x33
#define HCDP_CFG_CTLACQGAIN_ANXA_256QAM    0x42
#define HCDP_CFG_CTLACQGAIN_ANXB_256QAM    0xB3
#define HCDP_CFG_CTLTRKGAIN_64QAM          0xB3
#define HCDP_CFG_CTLTRKGAIN_128QAM         0x143
#define HCDP_CFG_CTLTRKGAIN_256QAM         0x132

#ifdef __cplusplus
}
#endif
/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/
#endif
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
