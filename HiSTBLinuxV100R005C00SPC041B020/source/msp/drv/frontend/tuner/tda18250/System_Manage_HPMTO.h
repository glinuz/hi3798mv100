/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          System_Manage_HPMTO.h
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

#ifndef _TMBSL_MANAGE_HPMTO_H
#define _TMBSL_MANAGE_HPMTO_H

#ifdef __cplusplus
extern "C"
{
#endif

tmErrorCode_t tmSystem_Manage_HPMTO(
                                  	tmUnitSelect_t			TunerUnit		// I: Tuner unit number
                                    );

#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_MANAGE_HPMTO_H */
