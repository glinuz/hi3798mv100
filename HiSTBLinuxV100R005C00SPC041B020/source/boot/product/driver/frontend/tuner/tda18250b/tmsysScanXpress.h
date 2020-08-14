/**
  Copyright (C) 2008 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmdlScanXpress.h
 *                
 *
 * \date          %date_modified%
 *
 * \brief         Describe briefly the purpose of this file.
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
   18-DEC-2008  | X.RAZAVET |       |       | CREATION OF ARCHITECTURE 2.0.0
   -------------|-----------|-------|-------|-----------------------------------
                |           |       |       |
   -------------|-----------|-------|-------|-----------------------------------
   \endverbatim
 *
*/


#ifndef TMSYSSCANXPRESS_H
#define TMSYSSCANXPRESS_H

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*/
/*                       INCLUDE FILES                                        */
/*============================================================================*/


/************************************************************************/
/*                                                                      */
/* Description:                                                         */
/*  Describes the ScanXpress structure.                                 */
/*                                                                      */
/* Settings:                                                            */
/*  bScanXpressMode       - ScanXpress Mode (True/False).               */
/*  pFw_code              - Table containing the firmware.              */
/*  Fw_code_size          - Size of the firmware.                       */
/*                                                                      */
/************************************************************************/
typedef struct _tmsysScanXpressConfig_t
{
    Bool    bScanXpressMode;    
    UInt8*  puFw_code;
    UInt32  uFw_code_size;
} tmsysScanXpressConfig_t, *ptmsysScanXpressConfig_t;

/************************************************************************/
/*                                                                      */
/* Description:                                                         */
/*  Describes the ScanXpress structure.                                 */
/*                                                                      */
/* Settings:                                                            */
/*  uFrequency            - Frequency of the ScanXpress Request.        */
/*  uCS                   - Channel bandwidth.                          */
/*  uSpectralInversion    - Spectral inversion.                         */
/*  Confidence            - Confidence threshold.                       */
/*  ChannelType           - Channel type.                               */
/*                                                                      */
/************************************************************************/
typedef struct _tmsysScanXpressRequest_t
{
    UInt32                      uFrequency;
    UInt32                      uCS;
    UInt32                      uSpectralInversion;
    tmFrontEndConfidence_t      eConfidence;        /* TODO: remove, not used anymore */
    /* output value */
    Int32                       TunerStep;
    tmFrontEndChannelType_t     eChannelType;
    UInt32                      uAnlgFrequency;
    tmbslFrontEndTVStandard_t   eTVStandard;
    UInt32                      uDgtlFrequency;
    UInt32                      uDgtlBandwidth;
} tmsysScanXpressRequest_t, *ptmsysScanXpressRequest_t;

/************************************************************************/
/*                                                                      */
/* Description:                                                         */
/*  Describes the ScanXpressChannelFound structure.                     */
/*                                                                      */
/* Settings:                                                            */
/*  eChannelType         - AChannel type.                               */
/*  uAnlgFrequency       - Frequency in Hz.                             */
/*  uDgtlFrequency       - Frequency in Hz.                             */
/*                                                                      */
/************************************************************************/
typedef struct _tmsysScanXpressFoundChannel_t
{
    tmFrontEndChannelType_t     eChannelType;
    UInt32                      uAnlgFrequency;
    tmbslFrontEndTVStandard_t   eTVStandard;
    UInt32                      uDgtlFrequency;
    UInt32                      uDgtlBandwidth;
} tmsysScanXpressFoundChannel_t, *ptmsysScanXpressFoundChannel_t;    

#ifdef __cplusplus
}
#endif

#endif /* TMDLSCANXPRESS_H */
/*============================================================================*/
/*                            END OF FILE                                     */
/*============================================================================*/

