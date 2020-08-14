/*
  Copyright (C) 2006-2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmUnitParams.h
 *
 *                %version: CFR_FEAP#4 %
 *
 * \date          %modify_time%
 *
 * \author        Christophe CAZETTES
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
*/

#ifndef _TM_UNIT_PARAMS_H
#define _TM_UNIT_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/


/******************************************************************************/
/** \brief "These macros map to tmUnitSelect_t variables parts"
*
******************************************************************************/

#define UNIT_VALID(_tUnIt)                      (((_tUnIt)&0x80000000)==0)

#define UNIT_PATH_INDEX_MASK                    (0x0000001F)
#define UNIT_PATH_INDEX_POS                     (0)

#define UNIT_PATH_TYPE_MASK                     (0x000003E0)
#define UNIT_PATH_TYPE_POS                      (5)

#define UNIT_PATH_CONFIG_MASK                   (0x0003FC00)
#define UNIT_PATH_CONFIG_POS                    (10)

#define UNIT_SYSTEM_INDEX_MASK                  (0x007C0000)
#define UNIT_SYSTEM_INDEX_POS                   (18)

#define UNIT_SYSTEM_CONFIG_MASK                 (0x7F800000)
#define UNIT_SYSTEM_CONFIG_POS                  (23)




#define UNIT_PATH_INDEX_GET(_tUnIt)             ((_tUnIt)&UNIT_PATH_INDEX_MASK)
#define UNIT_PATH_INDEX_VAL(_val)               (((_val)<<UNIT_PATH_INDEX_POS)&UNIT_PATH_INDEX_MASK)
#define UNIT_PATH_INDEX_SET(_tUnIt, _val)       ( ((_tUnIt)&~UNIT_PATH_INDEX_MASK) | UNIT_PATH_INDEX_VAL(_val) )
#define UNIT_PATH_INDEX_VAL_GET(_val)           (UNIT_PATH_INDEX_VAL(UNIT_PATH_INDEX_GET(_val)))

#define UNIT_PATH_TYPE_GET(_tUnIt)              (((_tUnIt)&UNIT_PATH_TYPE_MASK) >> UNIT_PATH_TYPE_POS)
#define UNIT_PATH_TYPE_VAL(_val)                (((_val)<<UNIT_PATH_TYPE_POS)&UNIT_PATH_TYPE_MASK)
#define UNIT_PATH_TYPE_SET(_tUnIt, _val)        ( ((_tUnIt)&~UNIT_PATH_TYPE_MASK) | UNIT_PATH_TYPE_VAL(_val) )
#define UNIT_PATH_TYPE_VAL_GET(_val)            (UNIT_PATH_TYPE_VAL(UNIT_PATH_TYPE_GET(_val)))


#define UNIT_PATH_CONFIG_GET(_tUnIt)            (((_tUnIt)&UNIT_PATH_CONFIG_MASK) >> UNIT_PATH_CONFIG_POS)
#define UNIT_PATH_CONFIG_VAL(_val)              (((_val)<<UNIT_PATH_CONFIG_POS)&UNIT_PATH_CONFIG_MASK)
#define UNIT_PATH_CONFIG_SET(_tUnIt, _val)      ( ((_tUnIt)&~UNIT_PATH_CONFIG_MASK) | UNIT_PATH_CONFIG_VAL(_val) )
#define UNIT_PATH_CONFIG_VAL_GET(_val)          (UNIT_PATH_CONFIG_VAL(UNIT_PATH_CONFIG_GET(_val)))

#define UNIT_SYSTEM_INDEX_GET(_tUnIt)           (((_tUnIt)&UNIT_SYSTEM_INDEX_MASK) >> UNIT_SYSTEM_INDEX_POS)
#define UNIT_SYSTEM_INDEX_VAL(_val)             (((_val)<<UNIT_SYSTEM_INDEX_POS)&UNIT_SYSTEM_INDEX_MASK)
#define UNIT_SYSTEM_INDEX_SET(_tUnIt, _val)     ( ((_tUnIt)&~UNIT_SYSTEM_INDEX_MASK) | UNIT_SYSTEM_INDEX_VAL(_val) )
#define UNIT_SYSTEM_INDEX_VAL_GET(_val)         (UNIT_SYSTEM_INDEX_VAL(UNIT_SYSTEM_INDEX_GET(_val)))

#define UNIT_SYSTEM_CONFIG_GET(_tUnIt)          (((_tUnIt)&UNIT_SYSTEM_CONFIG_MASK) >> UNIT_SYSTEM_CONFIG_POS)
#define UNIT_SYSTEM_CONFIG_VAL(_val)            (((_val)<<UNIT_SYSTEM_CONFIG_POS)&UNIT_SYSTEM_CONFIG_MASK)
#define UNIT_SYSTEM_CONFIG_SET(_tUnIt, _val)    ( ((_tUnIt)&~UNIT_SYSTEM_CONFIG_MASK) | UNIT_SYSTEM_CONFIG_POS(_val) )
#define UNIT_SYSTEM_CONFIG_VAL_GET(_val)        (UNIT_SYSTEM_CONFIG_VAL(UNIT_SYSTEM_CONFIG_GET(_val)))


#define GET_WHOLE_SYSTEM_TUNIT(_tUnIt)          (UNIT_SYSTEM_CONFIG_VAL_GET(_tUnIt)|UNIT_SYSTEM_INDEX_VAL_GET(_tUnIt))

#define GET_SYSTEM_TUNIT(_tUnIt)                (UNIT_SYSTEM_CONFIG_VAL_GET(_tUnIt)|UNIT_SYSTEM_INDEX_VAL_GET(_tUnIt)|UNIT_PATH_INDEX_VAL_GET(_tUnIt))

#define GET_INDEX_TUNIT(_tUnIt)                 (UNIT_SYSTEM_INDEX_VAL_GET(_tUnIt)|UNIT_PATH_INDEX_VAL_GET(_tUnIt))

#define GET_INDEX_TYPE_TUNIT(_tUnIt)            (UNIT_SYSTEM_INDEX_VAL_GET(_tUnIt)|UNIT_PATH_INDEX_VAL_GET(_tUnIt)|UNIT_PATH_TYPE_VAL_GET(_tUnIt))

#define XFER_DISABLED_FLAG                      (UNIT_PATH_CONFIG_VAL(0x80))
#define GET_XFER_DISABLED_FLAG_TUNIT(_tUnIt)    (((_tUnIt)&XFER_DISABLED_FLAG)!=0)


/*============================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* _TM_UNIT_PARAMS_H */

