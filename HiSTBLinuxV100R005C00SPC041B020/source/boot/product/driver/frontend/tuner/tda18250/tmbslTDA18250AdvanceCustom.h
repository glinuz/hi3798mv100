/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250AdvanceCustom.h
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA18250_ADVANCED_CUSTOM_H
#define _TMBSL_TDA18250_ADVANCED_CUSTOM_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Exported advanced custom functions:                                        */
/*============================================================================*/

extern tmErrorCode_t
tmbslTDA18250SetAGC1
(
    tmUnitSelect_t           tUnit,     /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t agc1Gain   /* I: AGC1 gain   */
);

extern tmErrorCode_t
tmbslTDA18250GetAGC1
(
    tmUnitSelect_t            tUnit,     /* I: Unit number */
    tmddTDA18250_AGC1_Gain_t* pAgc1Gain  /* O: AGC1 gain   */
);

extern tmErrorCode_t
tmbslTDA18250SetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,     /* I: Unit number */
    UInt8           agc2Gain   /* I: AGC2 gain   */
);

extern tmErrorCode_t
tmbslTDA18250GetAGC2b_Gain
(
    tmUnitSelect_t  tUnit,     /* I: Unit number */
    UInt8*          pAgc2Gain  /* O: AGC2 gain   */
);

extern tmErrorCode_t
tmbslTDA18250SetAGC2b_Raw
(
    tmUnitSelect_t  tUnit,     /* I: Unit number */
    UInt8            Agc2Raw   /* I: AGC2 gain   */
);

extern tmErrorCode_t
tmbslTDA18250GetAGC2b_Raw
(
    tmUnitSelect_t  tUnit,     /* I: Unit number */
    UInt8            *pAgc2Raw   /* I: AGC2 gain   */
);


extern tmErrorCode_t
tmbslTDA18250SetAGC1Freeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
);


extern tmErrorCode_t
tmbslTDA18250SetAGC1Unfreeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
);

extern tmErrorCode_t
tmbslTDA18250SetAGC2Unfreeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
);

extern tmErrorCode_t
tmbslTDA18250SetAGC2Freeze
(
    tmUnitSelect_t      tUnit,  /* I: Unit number */
    tmTDA18250_State_t  uState  /* I: State value */
);
extern tmErrorCode_t
tmbslTDA18250SetLoFreq
(
    tmUnitSelect_t  tUnit,      /* I: Unit number      */
    UInt32          uRF         /* I: Frequency in Hz  */
);

extern tmErrorCode_t
tmbslTDA18250GetVCO
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    Int32*          puVCO       /* O: VCO in KHz  */
);

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250_ADVANCED_CUSTOM_H */
