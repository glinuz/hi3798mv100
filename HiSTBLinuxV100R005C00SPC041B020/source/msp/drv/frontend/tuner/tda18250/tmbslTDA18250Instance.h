/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250Instance.h
 *
 *                1
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

#ifndef _TMBSL_TDA18250_INSTANCE_H
#define _TMBSL_TDA18250_INSTANCE_H


#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

extern tmErrorCode_t TDA18250AllocInstance (tmUnitSelect_t tUnit, pptmTDA18250Object_t ppDrvObject);
extern tmErrorCode_t TDA18250DeAllocInstance (tmUnitSelect_t tUnit);
extern tmErrorCode_t TDA18250GetInstance (tmUnitSelect_t tUnit, pptmTDA18250Object_t ppDrvObject);


#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250_INSTANCE_H */
