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
#ifndef KDMTYPEFXPCMN_H_
#define KDMTYPEFXPCMN_H_

#include "KCdmModCtrl.h"

////// for fixed point 
//// L2PQ related: since all are fixed, we defined it here
#define DEF_L2PQ_LUT_X_SCALE2P      18  //14.18
#define DEF_L2PQ_LUT_A_SCALE2P      23  // 5.23
#define DEF_L2PQ_LUT_B_SCALE2P      16  // 0.16
#define DEF_L2PQ_LUT_NODES          128

//// L2G related: since all are fixed, we defined it here
#define DEF_L2G_LUT_X_SCALE2P       18  //14.18
#define DEF_L2G_LUT_A_SCALE2P       23  // 5.23
#define DEF_L2G_LUT_B_SCALE2P       16  // 0.16
#define DEF_L2G_LUT_NODES           128

//// PQ2L lut
#define DEF_PQ2L_LUT_SIZE_1024     (1<<10)
#define DEF_PQ2L_LUT_SIZE_4096     (1<<12)

#if REDUCED_COMPLEXITY
#define DEF_G2L_LUT_SIZE_2P        8
#define DEF_G2L_LUT_SIZE           (1<<DEF_G2L_LUT_SIZE_2P)
#endif

//// tone curve mapping related: copy paste from FixedPointQValues.m adding DEF_ as prefix
#define DEF_FXP_TM_LUT_SCALE_2P     12 // must be 12~14 range!

//// De-Gammar
#define DEF_DEGAMMAR_IN_SCALE2P   16 // 0.16
#define DEF_DEGAMMAR_INTERP_BITS   4 // 4 LSB as interpolation bit, 12 MSB as idx to LUT
#define DEF_DEGAMMAR_LOG_SCALE2P  12 // log10 output scale=>4.12  : uint16
#define DEF_DEGAMMAR_LOG_RATE_SCALE2P 16 // log10 rate scale=>1.16  : uint32

#define DEF_DEGAMMAR_LOG_LUT_SIZE (1<<DEF_DEGAMMAR_LOG_SCALE2P)

#define DEF_DEGAMMAR_POWER_IN_BITS  14 // 4.10
#define DEF_DEGAMMAR_POWER_ID_BITS   4 // 4 MSB as table id

#define DEF_DEGAMMAR_POWER_LUT_NUM   ((1 << DEF_DEGAMMAR_POWER_ID_BITS) - 6) // the last 6 luts are zero
#define DEF_DEGAMMAR_POWER_LUT_SIZE  (1 << (DEF_DEGAMMAR_POWER_IN_BITS - DEF_DEGAMMAR_POWER_ID_BITS) )

#define DEF_GAMMAR_SCALE2P      14 // 2.14

#define DEF_DEGAMMAR_OUT_SCALE2P  18 // 14.18

#define OPT_2_OPT_SCALE2P     3 

#define DM_PQ_SCALE_2P        12  // 4095

#define DEF_MSWEIGHT_SCALE        12

// Setting for intermediate PQ bit width (1.12 format) across input mapping, tone mapping and output mapping
// Max final code value diff for 12 bit is about 200, vs, 16 bits is about 100 in extreme case.
#define PQ_BIT_WIDTH_12_BITS    1 // 0: 1.15 format; 1: set by PQ_BIT_SHIFT
#if PQ_BIT_WIDTH_12_BITS
#define PQ_BIT_SHIFT            3 // 3: 1.12 format
#define PQ_UNSIGNED_BLEND       1 // unsigned blending
#endif

// 2^N value
#define TWO_PWR_N(bits) ((uint64_t)(1) << (bits))

// max unsigned value for the number of bits
#define DLB_UINT_MAX(bits) (TWO_PWR_N(bits) - 1)

#define MAX2S(a_, b_)  (((a_) >= (b_)) ? (a_) : (b_))
#define MIN2S(a_, b_)  (((a_) <= (b_)) ? (a_) : (b_))
#define CLAMPS(a_, mn_, mx_)  ( ((a_) >= (mn_)) ? ( ((a_) <= (mx_)) ? (a_) : (mx_) ) : (mn_) )


#endif //KDMTYPEFXPCMN_H_
