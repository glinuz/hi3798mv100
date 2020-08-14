/**
 * Copyright (C) 2005 Koninklijke Philips Electronics N.V., All Rights Reserved.
 * This source code and any compilation or derivative thereof is the proprietary
 * information of Koninklijke Philips Electronics N.V. and is confidential in
 * nature. Under no circumstances is this software to be  exposed to or placed
 * under an Open Source License of any type without the expressed written
 * permission of Koninklijke Philips Electronics N.V.
 *
 * \file          tmbslOOBIp_cfg.h
 *
 * \date          02-Oct-2007
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
   26-July-2010 | A.TANT    |       |       | Creation
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/


#ifndef TMBSLOOBIP_CFG_H 
#define TMBSLOOBIP_CFG_H


/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*/
/*                       ENUM OR TYPE DEFINITION                              */
/*============================================================================*/
typedef enum _tmbslOOBIp_Cfg_Err_t
{
    tmbslOOBIp_Cfg_NoError_E,
    tmbslOOBIp_Cfg_InvalidValue_E
} tmbslOOBIp_Cfg_Err_t;


/*============================================================================*/
/* Generic Configuration items                                                */
/*============================================================================*/


typedef struct _tmbslOOBIp_Cfg_t
{
    unsigned int    CompatibilityNb_U;
    unsigned short  uOOBIpStartAddress;
} tmbslOOBIp_Cfg_t;

/*============================================================================*/
/* macro define                                                               */
/*============================================================================*/

/*============================================================================*/
/* bsl type define                                                            */
/*============================================================================*/

#define TMBSLOOBIP_CFG_VERSION 2

/*============================================================================*/
/* SET current configuration HERE                                             */
/*============================================================================*/

/* IP address */
#define TMBSLOOBIP_START_ADDRESS 0x1000

/* Sampling clock */
#define TMBSLOOBIp_CFG_SAMPLING_FREQ_ES1       60000000
#define TMBSLOOBIp_CFG_SAMPLING_FREQ_ES2       56250000

/*============================================================================*/
/*                       EXTERN FUNCTION PROTOTYPES                           */
/*============================================================================*/

/******************************************************************************/
/** Configure the consumer parameters
 *
 *
 * @param ptmdlFrontEndCfg_S (In) : parameters structure
 *
 *
 ******************************************************************************/
extern tmbslOOBIp_Cfg_Err_t
tmbslOOBIp_CFG_GetConfig(
        tmbslOOBIp_Cfg_t* pConfig
        );


#ifdef __cplusplus
}
#endif

#endif
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/
