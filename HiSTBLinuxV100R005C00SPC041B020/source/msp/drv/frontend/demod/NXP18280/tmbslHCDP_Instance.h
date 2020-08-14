/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP_Instance.h
 *
 *                %version: 1 %
 *
 * \date          %modify_time%
 *
 * \author        David LEGENDRE
 *                Michael VANNIER
 *                Christophe CAZETTES
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                HCDP_Driver_User_Guide.pdf
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/

#pragma once
#ifndef _TMBSL_HCDP_INSTANCE_H
#define _TMBSL_HCDP_INSTANCE_H

#ifdef __cplusplus
extern "C"
{
#endif


tmErrorCode_t
iHCDP_AllocInstance(tmUnitSelect_t tUnit, ppHCDPObject_t ppDrvObject);

tmErrorCode_t
iHCDP_DeAllocInstance(pHCDPObject_t pDrvObject);

tmErrorCode_t
iHCDP_GetInstance(tmUnitSelect_t tUnit, ppHCDPObject_t ppDrvObject);

tmErrorCode_t
iHCDP_ResetInstance(pHCDPObject_t pDrvObject);


#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_HCDP_INSTANCE_H */

