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
#ifndef K_C_DM_MOD_CTRL_H
#define K_C_DM_MOD_CTRL_H

////// DM version
#define DM_VER_CTRL             3    // 3 for 3.x, 2 for 2.x
# define DM_VER_CTRL_286        (1 && (DM_VER_CTRL == 2))    // backward compatible to DM v2.8.6

////// For a typical set up, enable one of the following two macro.
// disable both will result in a dtv only solution
// enable both will get you an all in one solution, 
// multi-scale is enabled when DM_DTV is on. to turn multi-scale off, use -msOff.
#define DM_STB                  (1 && (DM_VER_CTRL == 3))
#define DM_DTV                  (0 || (DM_VER_CTRL == 2))


////// Set for reduced complexity
#define REDUCED_COMPLEXITY      1

////// Set for matching float kernel to fixed
#define K_FLOAT_MATCH_FIXED     1

////// debug control
// to save binary debug result, VdrDM.cpp and kernels have to be synced. so define it here
// 1: enable save result in FloatComp_t into debug buf and then write to file
#define ENABLE_DBG_BUF          0

// use external ccm, ignor metadata bit stream
#define DM_EXTERNAL_CCM         (1 && DM_FULL_SRC_SPEC) 


////// float or fix point impl control


#ifndef DM_EN_LUT_CTRL
# define DM_EN_LUT_CTRL         0
#endif

////// metadata bitstream endian
#define BIN_MD_BE                   1 // it is a big endian

////// if need specify full source info
#define DM_FULL_SRC_SPEC            1 // 1: can run without mds=>need CCM only

////// data path module selection
#define DM_SEC_INPUT                1//DM_STB  // if support second input channel
#define EN_UP_DOWN_SAMPLE_OPTION    1       // up/downsample support
#define EN_420_OUTPUT               (0 && EN_UP_DOWN_SAMPLE_OPTION) // if 420 output, not fully tested
#define EN_MS_OPTION                (DM_DTV || (DM_VER_CTRL == 2))  // multi-scale support
#define EN_EDGE_SIMPLE              1  // use simple edge detect in MS
#define EN_MS_FILTER_CFG            1                        // support configurable ms filter, only 1 for now
#define EN_IPT_PQ_ONLY_OPTION       (0 && !DM_SEC_INPUT && (DM_VER_CTRL == 3))  // only IPT pq input is supported
#define EN_BYPASS_DM                1

#define MS_VERT_FILTER_FIRST        REDUCED_COMPLEXITY       // This applies fixed point only
#if REDUCED_COMPLEXITY
//Pick either 2 taps or 4 taps YUV chroma up sample filter
#define _USE_2_TAP_CHROMA_VERT_FILTER_
//#define _USE_4_TAP_CHROMA_VERT_FILTER_
#endif


////// to support CUDA code
#ifndef __CUDA_ARCH__
# define CONST_MODIFIER const
#else
# define CONST_MODIFIER __constant__
#endif

# define DM_FUNC_CUDA_VER(funcNm_, funcParams_)


////// limits
//// maximum supported frame size
#define DM_FRAME_SIZE_1080P       0
#define DM_FRAME_SIZE_2K          1
#define DM_FRAME_SIZE_4K          2

#define DM_FRAME_SIZE_SUPPORT      DM_FRAME_SIZE_4K

#define DM_ROW_NUM_4K    2160
#define DM_COL_NUM_4K    4096

#define DM_ROW_NUM_2K    1080
#define DM_COL_NUM_2K    2048

#define DM_ROW_NUM_1080P  1080
#define DM_COL_NUM_1080P  1920

//// in pixel size: up to 4K for now
#if (DM_FRAME_SIZE_SUPPORT == DM_FRAME_SIZE_4K)
# define DM_MAX_ROW_NUM    DM_ROW_NUM_4K
# define DM_MAX_COL_NUM    DM_COL_NUM_4K
#elif (DM_FRAME_SIZE_SUPPORT == DM_FRAME_SIZE_2K)
# define DM_MAX_ROW_NUM    DM_ROW_NUM_2K
# define DM_MAX_COL_NUM    DM_COL_NUM_2K
#else
# define DM_MAX_ROW_NUM    DM_ROW_NUM_1080P
# define DM_MAX_COL_NUM    DM_COL_NUM_1080P
#endif

#define DM_MAX_IMG_SIZE  (DM_MAX_ROW_NUM * DM_MAX_COL_NUM)

//// tmKs lut size
# define TM1_LUT_MAX_SIZE     512

#define TM2_LUT_MAX_SIZE    TM1_LUT_MAX_SIZE  // for the second input

// TM_LUT_MAX_SIZE: obsolete
#define TM_LUT_MAX_SIZE  ((TM1_LUT_MAX_SIZE >= TM2_LUT_MAX_SIZE) ? TM1_LUT_MAX_SIZE : TM2_LUT_MAX_SIZE)


//// 3D gmLut size

#if DM_EN_LUT_CTRL
// to support generating LUT with floating for fix point
# define CDM_FXP_LUT_BY_FLT   0
# define CDM_FLT_IN_FXP       (!CDM_FLOAT_POINT && !KDM_FLOAT_POINT && CDM_FXP_LUT_BY_FLT)
#endif

#endif // K_C_DM_MOD_CTRL_H
