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
#ifndef K_DM_TYPE_H
#define K_DM_TYPE_H

#include "KCdmModCtrl.h"

//// for now, these table size/array/matrix(in fact the pre-allocated max size) are fixed
// for ms
#define MS_ROW_FLTR_RADIUS_MAX    5
#define MS_COL_FLTR_RADIUS_MAX    2

typedef unsigned char KDtp_t;
typedef unsigned char KClr_t;
typedef unsigned char KChrm_t;
typedef unsigned char KWeav_t;
typedef unsigned char KEotf_t;
typedef unsigned char KLoc_t;
typedef unsigned char KBdp_t;


////// pixel definition, force to unsigned char
// bit depth is of type unsigned char, numerical value. 
// MSB aliagned will take the whole bits of an interger data type
#define KDtpU16    ((KDtp_t)0)
#define KDtpU8    ((KDtp_t)1)
#define KDtpF32   ((KDtp_t)2)
#define KDtpNum   ((KDtp_t)3)

#define KClrYuv   ((KClr_t)0)
#define KClrRgb   ((KClr_t)1)
#define KClrRgba  ((KClr_t)2)
#define KClrIpt   ((KClr_t)3)
#define KClrNum   ((KClr_t)4)

#define KChrm420  ((KChrm_t)0)
#define KChrm422  ((KChrm_t)1)
#define KChrm444  ((KChrm_t)2)
#define KChrmNum  ((KChrm_t)3)

#define KWeavPlnr ((KWeav_t)0)
#define KWeavIntl ((KWeav_t)1) // per component interleave, YUVYUV..., RGBRGB...
#define KWeavUyVy ((KWeav_t)2) // UYVY and UYVY only
#define KWeavNum  ((KWeav_t)3)

#define KEotfBt1886   ((KEotf_t)0)
#define KEotfPq       ((KEotf_t)1)
#define KEotfNum      ((KEotf_t)2) 

#define KLocHost    ((KLoc_t)0)
#define KLocDev     ((KLoc_t)1)
#define KLocNum     ((KLoc_t)2) 

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
# if DM_SEC_INPUT
  int secIn;  // if second input available
# endif

  int prf;
} DmKsCtrl_t;

#if 1//EN_SKINP_TMAP_MS
typedef enum KTcLutDir_t_{
KTcLutDirFwd=0,
KTcLutDirBwd,	
KTcLutDirPass,
KTcLutDirNum

}KTcLutDir_t;

#endif

#endif // K_DM_TYPE_H
