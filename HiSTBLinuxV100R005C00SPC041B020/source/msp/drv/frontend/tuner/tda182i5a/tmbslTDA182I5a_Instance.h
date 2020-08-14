/*
  Copyright (C) 2012 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA182I5a_Instance.h
 *
 * \date          %modify_time%
 *
 * \author        Michael VANNIER
 *                
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA182I5a_Driver_User_Guide.pdf
 *
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA182I5a_INSTANCE_H
#define _TMBSL_TDA182I5a_INSTANCE_H

#ifdef __cplusplus
extern "C"
{
#endif


tmErrorCode_t
iTDA182I5a_AllocInstance(tmUnitSelect_t tUnit, ppTDA182I5aObject_t ppDrvObject);

tmErrorCode_t
iTDA182I5a_DeAllocInstance(pTDA182I5aObject_t pDrvObject);

tmErrorCode_t
iTDA182I5a_GetInstance(tmUnitSelect_t tUnit, ppTDA182I5aObject_t ppDrvObject);

tmErrorCode_t
iTDA182I5a_ResetInstance(pTDA182I5aObject_t pDrvObject);


#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA182I5a_INSTANCE_H */

