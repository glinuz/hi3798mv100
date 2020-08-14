/****************************************************************************
* This product contains one or more programs protected under international
* and U.S. copyright laws as unpublished works.  They are confidential and
* proprietary to Dolby Laboratories.  Their reproduction or disclosure, in
* whole or in part, or the production of derivative works therefrom without
* the express permission of Dolby Laboratories is prohibited.
*
*             Copyright 2011 - 2015 by Dolby Laboratories.
*                     All rights reserved.
****************************************************************************/
#ifndef _VDP_HDR_FPGA_H
#define _VDP_HDR_FPGA_H
//-----HDR--BEGIN------------//
#if 1
typedef   int              int32_t;
typedef   unsigned int     uint32_t;
typedef   unsigned char    uint8_t;
typedef   short   		   int16_t;

typedef   unsigned short   uint16_t;

#define MAX_PIVOT       9
#define MAX_POLY_ORDER  2

#define MAX_DM_EXT_BLOCKS 255
#define DM_VER_CTRL             3    // 3 for 3.x, 2 for 2.x
#define EN_UP_DOWN_SAMPLE_OPTION    1       // up/downsample support
#define DM_DTV                  (0 || (DM_VER_CTRL == 2))

////// Set for reduced complexity
#define REDUCED_COMPLEXITY      1
#define DEF_G2L_LUT_SIZE_2P        8
#define DEF_G2L_LUT_SIZE           (1<<DEF_G2L_LUT_SIZE_2P)

//// tmKs lut size
#define TM1_LUT_MAX_SIZE     512

#define TM2_LUT_MAX_SIZE    TM1_LUT_MAX_SIZE  // for the second input
// TM_LUT_MAX_SIZE: obsolete

#define TM_LUT_MAX_SIZE  ((TM1_LUT_MAX_SIZE >= TM2_LUT_MAX_SIZE) ? TM1_LUT_MAX_SIZE : TM2_LUT_MAX_SIZE)

typedef unsigned char KDtp_t;
typedef unsigned char KClr_t;
typedef unsigned char KChrm_t;
typedef unsigned char KWeav_t;
typedef unsigned char KEotf_t;
typedef unsigned char KLoc_t;
typedef unsigned char KBdp_t;

#define KEotfBt1886   ((KEotf_t)0)
#define KEotfPq       ((KEotf_t)1)
#define KEotfNum      ((KEotf_t)2) 


////// data path module selection
#define DM_SEC_INPUT                1//DM_STB  // if support second input channel

#define EN_420_OUTPUT               (0 && EN_UP_DOWN_SAMPLE_OPTION) // if 420 output, not fully tested
#define EN_MS_OPTION                (DM_DTV || (DM_VER_CTRL == 2))  // multi-scale support
#define EN_EDGE_SIMPLE              1  // use simple edge detect in MS
#define EN_MS_FILTER_CFG            1                        // support configurable ms filter, only 1 for now
#define EN_IPT_PQ_ONLY_OPTION       (0 && !DM_SEC_INPUT && (DM_VER_CTRL == 3))  // only IPT pq input is supported
#define EN_BYPASS_DM                1
//----------end---------------//
#endif

#endif // K_DM_TYPE_FXP_H
