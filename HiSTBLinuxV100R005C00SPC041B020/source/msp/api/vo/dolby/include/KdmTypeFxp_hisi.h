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
#ifndef K_DM_TYPE_FXP_H
#define K_DM_TYPE_FXP_H

//VDP_CBB_FPGA
#include "vdp_hdr_fpga.h"

#if  1
//--------------HDR BEGIN----------------//
// for pre-post processing: up/down sampling
typedef struct DmKsUdsFxp_t_
{
  //// for UV
  
#if EN_UP_DOWN_SAMPLE_OPTION 
  KChrm_t chrmIn, chrmOut;
#endif

  int16_t minUs, maxUs, minDs, maxDs;

#if EN_UP_DOWN_SAMPLE_OPTION 

  int16_t filterUvRowUsHalfSize;
  int16_t filterUvRowUsScale2P;
  int16_t filterUvRowUs0_m[8];
  int16_t filterUvRowUs1_m[8];

  int16_t filterUvColUsHalfSize;
  int16_t filterUvColUsScale2P;
  int16_t filterUvColUs_m[8];

  int16_t filterUvColDsRadius;
  int16_t filterUvColDsScale2P;
  int16_t filterUvColDs_m[16];
#endif
} DmKsUdsFxp_t;



////// data path/kernel structure: KS
//// E=>O
typedef struct EotfParamFxp_t_
{
  // range
  uint16_t rangeMin, range;
  uint32_t rangeInv;
  uint16_t bdp;
  // gamma stuff
  KEotf_t eotf;   // CEotfBt1886, CEotfPq coded
  uint16_t gamma;
  uint16_t a, b;
  uint32_t g;
} EotfParamFxp_t;

//// input mapping
typedef struct DmKsIMapFxp_t_ {
#if !EN_IPT_PQ_ONLY_OPTION
  KClr_t clr;
  int16_t m33Yuv2RgbScale2P;
  int16_t m33Yuv2Rgb[3][3];
  int32_t v3Yuv2RgbOffInRgb[3];

  EotfParamFxp_t eotfParam;
#if REDUCED_COMPLEXITY
  uint32_t g2L[DEF_G2L_LUT_SIZE];
#endif

  int16_t m33Rgb2LmsScale2P;
  int16_t m33Rgb2Lms[3][3];

  int16_t m33Lms2IptScale2P;
  int16_t m33Lms2Ipt[3][3];
#endif

  int32_t iptScale;
  int32_t v3IptOff[3];
} DmKsIMapFxp_t;

/// tone curve mapping
typedef struct Dm3KsTMapFxp_t_ {

  int16_t tmLutISizeM1;


  int16_t tmLutSSizeM1;
  int16_t tmLutI[TM_LUT_MAX_SIZE];
  int16_t tmLutS[TM_LUT_MAX_SIZE];
  //uint16_t satWeight;

  int16_t smLutISizeM1, smLutSSizeM1;
  int16_t smLutI[TM_LUT_MAX_SIZE];
  int16_t smLutS[TM_LUT_MAX_SIZE];
  //uint16_t chromaWeight;

} DmKsTMapFxp_t;

//// the io frame format
typedef struct DmKsFrmFmt_t_
{
  int rowNum, colNum;  // size
  KDtp_t dtp;
  KWeav_t weav;
  KLoc_t loc;

  KBdp_t bdp;
  KChrm_t chrm;
  KClr_t clr;

  //// that for interleaved one or the first component, and alpha of planar one
  int rowPitch;       // row pitch in byte
  int colPitch;       // col pitch in byte: a componet size for planar, whole pixel size for interleaved
  //// that for the second,third(likely Chroma) in case of planar layout
  int rowPitchC;      // row pitch in byte
} DmKsFrmFmt_t;

// data path contro and global data plan control come here
typedef struct DmKsCtrl_t_ {
  int mainIn; // if main input available
#if DM_SEC_INPUT
  int secIn;  // if second input available
#endif

  int prf;
} DmKsCtrl_t;


//// O=>E
typedef struct OetfParamFxp_t_
{
  //// range
  uint32_t min, max; // in linear output space
  uint16_t rangeMin, range; // in final output
  uint32_t rangeOverOne;
  uint16_t bdp;
  // gamma stuff
  KEotf_t oetf;   // CEotfBt1886, CEotfPq coded
} OetfParamFxp_t;

/// output mapping
typedef struct Dm3KsOMapFxp_t_ {
  // lms => target rgb
  int16_t m33Ipt2LmsScale2P;
  int16_t m33Ipt2Lms[3][3];

  // lms => target rgb
  int16_t m33Lms2RgbScale2P;
  int16_t m33Lms2Rgb[3][3];

  OetfParamFxp_t oetfParam;


  KClr_t clr;
  int16_t m33Rgb2YuvScale2P;
  int16_t m33Rgb2Yuv[3][3];
  int32_t v3Rgb2YuvOff[3];
  int32_t iptScale;
  int32_t v3IptOff[3];
} Dm3KsOMapFxp_t;

typedef struct Dm3KsOMapFxp_t_  DmKsOMapFxp_t_;
typedef Dm3KsOMapFxp_t DmKsOMapFxp_t;

//// the ks collection
typedef struct DmKsFxp_t_
{
//#if (EN_UP_DOWN_SAMPLE_OPTION || EN_MS_OPTION)
#if 1

  //// tmp buf to store (I, P, T), post up sample or pre down sample (Y, U, V)
  int16_t rowPitchNum;   // pitch in index

  uint16_t *frmBuf0, *frmBuf1, *frmBuf2;

#endif

  DmKsUdsFxp_t ksUds;

  DmKsCtrl_t ksDmCtrl;

  DmKsFrmFmt_t ksFrmFmtI;

  DmKsIMapFxp_t  ksIMap;

  DmKsTMapFxp_t ksTMap;

#if DM_SEC_INPUT
  DmKsFrmFmt_t ksFrmFmtG;

  DmKsIMapFxp_t  ksIMapG;

  DmKsTMapFxp_t ksTMapG;
#endif


  DmKsOMapFxp_t ksOMap;

  DmKsFrmFmt_t ksFrmFmtO;

#if EN_BYPASS_DM
  int16_t bypassShift;
#endif

} DmKsFxp_t;

//// the ks collection
/* concern of 64-bits kernel,pointers may couse problems,use uint32_t instead of (uint16_t*) */
typedef struct Hisi_DmKsFxp_t_
{
//#if (EN_UP_DOWN_SAMPLE_OPTION || EN_MS_OPTION)
#if 1

  //// tmp buf to store (I, P, T), post up sample or pre down sample (Y, U, V)
  int16_t rowPitchNum;   // pitch in index

  uint32_t frmBuf0, frmBuf1, frmBuf2;

#endif

  DmKsUdsFxp_t ksUds;

  DmKsCtrl_t ksDmCtrl;

  DmKsFrmFmt_t ksFrmFmtI;

  DmKsIMapFxp_t  ksIMap;

  DmKsTMapFxp_t ksTMap;

#if DM_SEC_INPUT
  DmKsFrmFmt_t ksFrmFmtG;

  DmKsIMapFxp_t  ksIMapG;

  DmKsTMapFxp_t ksTMapG;
#endif


  DmKsOMapFxp_t ksOMap;

  DmKsFrmFmt_t ksFrmFmtO;

#if EN_BYPASS_DM
  int16_t bypassShift;
#endif

} Hisi_DmKsFxp_t;


//---------------HDR¡¡END-------------//
#endif

#endif // K_DM_TYPE_FXP_H
