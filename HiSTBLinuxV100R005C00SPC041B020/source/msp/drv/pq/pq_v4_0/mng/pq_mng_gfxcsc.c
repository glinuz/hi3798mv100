/******************************************************************************
*
* Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_gfxcsc.c
  Version       : Initial Draft
  Author        : pengjunwei 00203646
  Created       : 2015/10/15
  Description   :

******************************************************************************/

#include "pq_hal_gfxcsc.h"
#ifndef HI_BUILD_IN_BOOT
#include "pq_hal_gfxbt2020.h"
#include "pq_mng_gfxcsc.h"
#include <linux/kernel.h>
#else
#include "drv_pq_ext.h"
#include "hi_type.h"
#include "uboot.h"
#endif


static HI_BOOL      bIsBGRIn       = HI_FALSE;
PQ_GFX_CSC_TUNING_S stGfxCscTuning = {50, 50, 50, 50, 50, 50, 50};

static HI_PQ_PICTURE_SETTING_S   sg_stSDPictureSetting = {50, 50, 50, 50};
static HI_PQ_PICTURE_SETTING_S   sg_stHDPictureSetting = {50, 50, 50, 50};

/*************************** Structure Definition ****************************/
typedef enum
{
    PQ_GFX_CS_eUnknown      = 0,
    PQ_GFX_CS_eYCbCr_709    = 1,
    PQ_GFX_CS_eYCbCr_601    = 2,
    PQ_GFX_CS_eYCbCr_2020   = 3, /* new add */
    PQ_GFX_CS_eRGB_709      = 4,
    PQ_GFX_CS_eRGB_601      = 5, /* new add */
    PQ_GFX_CS_eRGB_2020     = 6, /* new add */

    PQ_GFX_CS_TYPE_BUTT

} PQ_GFX_CS_TYPE_E;

typedef enum
{
    PQ_GFX_CS_RANGE_eUnknown  = 0,
    PQ_GFX_CS_RANGE_FULL      = 1,
    PQ_GFX_CS_RANGE_LMTD      = 2,
    PQ_GFX_CS_RANGE_BUTT

} PQ_GFX_CS_RANGE_E;

typedef struct
{
    PQ_GFX_CS_TYPE_E  eCsType;
    PQ_GFX_CS_RANGE_E eCsRange;
} PQ_GFX_CS_S;

typedef struct
{
    PQ_GFX_CS_S stGfxCscInputCs;
    PQ_GFX_CS_S stGfxCscOutputCs;

} PQ_GFX_CSC_MODE_S;


const HI_S32 PQ_GFX_SIN_TABLE[61] =
{
    -500, -485, -469, -454, -438, -422, -407, -391,
    -374, -358, -342, -325, -309, -292, -276, -259,
    -242, -225, -208, -191, -174, -156, -139, -122,
    -104,  -87,  -70,  -52,  -35,  -17,    0,   17,
    35,     52,   70,   87,  104,  122,  139,  156,
    174,   191,  208,  225,  242,  259,  276,  292,
    309,   325,  342,  358,  374,  391,  407,  422,
    438,   454,  469,  485,  500
};

const HI_U32 PQ_GFX_COS_TABLE[61] =
{
    866, 875, 883, 891, 899,  906,  914,  921,
    927, 934, 940, 946, 951,  956,  961,  966,
    970, 974, 978, 982, 985,  988,  990,  993,
    995, 996, 998, 999, 999, 1000, 1000, 1000,
    999, 999, 998, 996, 995,  993,  990,  988,
    985, 982, 978, 974, 970,  966,  961,  956,
    951, 946, 940, 934, 927,  921,  914,  906,
    899, 891, 883, 875, 866
};



/*******************************************************************************
           color space conversion coefficient/ 10bit decimal precision
********************************************************************************/


/**************************************************
         CSC Matrix for [YCbCr]->[YCbCr]
**************************************************/
/* YCbCr_601L (i.e. SD) -> YCbCr_709L (i.e. HD) */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_YCbCr601L_to_YCbCr709L =
{
    1024, -118, -213,
    0,    1043,  117,
    0,      77, 1050
};

/* YCbCr_709L (i.e. HD) -> YCbCr_601L (i.e. SD) */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_YCbCr709L_to_YCbCr601L =
{
    1024, 102,  196,
    0,   1014, -113,
    0,    -74, 1007
};


/**************************************************
         CSC Matrix for [RGB]->[YCbCr]
**************************************************/
/* sRGBF -> YCbCr_709L (i.e. HD) */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_sRGBF_to_YCbCr709L =
{
    188,   629,  63,
    -103, -347, 450,
    450,  -409, -41
}; /* range[0,255]->range[16,235] */
/*{
    218,   732,  74,
    -118, -394, 512,
    512,  -465, -47
};*/ /* range[0,255]->range[0,255] */

/* sRGBF -> YCbCr_601L (i.e. SD) */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_sRGBF_to_YCbCr601L =
{
    264,   516, 100,
    -152, -298, 450,
    450,  -377, -73
}; /* range[0,255]->range[16,235] */
/*{
    306,   601, 117,
    -173, -339, 512,
    512,  -429, -83
};*/ /* range[0,255]->range[0,255] */


/**************************************************
          CSC Matrix for [YCbCr]->[RGB]
**************************************************/
/* YCbCr_709L (i.e. HD) -> sRGBF */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_YCbCr709L_to_sRGBF =
{
    1192,    0, 1836,
    1192, -218, -547,
    1192, 2166,    0
}; /* range[0,255]->range[16,235] */
/*{
    1024,    0, 1613,
    1024, -191, -479,
    1024, 1901,    0
};*/ /* range[0,255]->range[0,255] */

/* YCbCr_601L (i.e. SD) -> sRGBF */
const PQ_GFX_CSC_COEF_S  PQ_GFX_CSCTable_YCbCr601L_to_sRGBF =
{
    1192,    0, 1634,
    1192, -400, -833,
    1192, 2066,    0
}; /* range[0,255]->range[16,235] */
/*{
    1024,    0, 1436,
    1024, -352, -731,
    1024, 1815,    0
};*/ /* range[0,255]->range[0,255] */

/**************************************************
       CSC Matrix for no change of color space
**************************************************/
/* identity matrix */
const PQ_GFX_CSC_COEF_S PQ_GFX_CSCTable_Identity =
{
    1024,   0,   0,
    0,   1024,   0,
    0,      0, 1024
};


/* the compositor color matrices table with Color Primaries Matching */
/* add 2020YUV、2020RGB、 601YUV */
const static PQ_GFX_CSC_COEF_S *PQ_GFX_CSC_MatrixTbl[PQ_GFX_CS_TYPE_BUTT][PQ_GFX_CS_TYPE_BUTT] =
{
    /* PQ_GFX_CS_eUnknown = 0 */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> Unknown;   Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> YCbCr709;  Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> YCbCr601;  Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> YCbCr2020; Identity */ /* impossible condition */
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> sRGB709;   Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Unknown -> sRGB601;   Identity */ /* impossible condition */
        &PQ_GFX_CSCTable_Identity                 /* Unknown -> sRGB2020;  Identity */ /* impossible condition */
    },

    /* PQ_GFX_CS_eYCbCr_709 = 1 */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr709 -> unknown   */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr709 -> YCbCr709  */
        &PQ_GFX_CSCTable_YCbCr709L_to_YCbCr601L,  /* YCbCr709 -> YCbCr601  */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr709 -> YCbCr2020 */ /* impossible condition */
        &PQ_GFX_CSCTable_YCbCr709L_to_sRGBF    ,  /* YCbCr709 -> sRGB709   */
        &PQ_GFX_CSCTable_YCbCr709L_to_sRGBF    ,  /* YCbCr709 -> sRGB601   */ /* impossible condition */
        &PQ_GFX_CSCTable_YCbCr709L_to_sRGBF       /* YCbCr709 -> sRGB2020  */ /* impossible condition */
    },

    /* PQ_GFX_CS_eYCbCr_601 = 2 */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr601 -> unknown   */
        &PQ_GFX_CSCTable_YCbCr601L_to_YCbCr709L,  /* YCbCr601 -> YCbCr709  */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr601 -> YCbCr601  */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr601 -> YCbCr2020 */ /* impossible condition */
        &PQ_GFX_CSCTable_YCbCr601L_to_sRGBF    ,  /* YCbCr601 -> sRGB709   */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr601 -> sRGB601   */ /* impossible condition */
        &PQ_GFX_CSCTable_Identity                 /* YCbCr601 -> sRGB2020  */ /* impossible condition */
    },
    /* PQ_GFX_CS_eYCbCr_2020 = 3 *//* impossible condition */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> Unknown;   Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> YCbCr709;  Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> YCbCr601;  Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> YCbCr2020; Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> sRGB709;   Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* YCbCr2020 -> sRGB601;   Identity */
        &PQ_GFX_CSCTable_Identity                 /* YCbCr2020 -> sRGB2020;  Identity */
    },
    /* PQ_GFX_CS_eRGB_709 = 4 */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB709 -> unknown   */
        &PQ_GFX_CSCTable_sRGBF_to_YCbCr709L    ,  /* sRGB709 -> YCbCr709  */
        &PQ_GFX_CSCTable_sRGBF_to_YCbCr601L    ,  /* sRGB709 -> YCbCr601  */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB709 -> YCbCr2020 */ /* impossible condition */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB709 -> sRGB709   */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB709 -> sRGB601   */ /* impossible condition */
        &PQ_GFX_CSCTable_Identity                 /* sRGB709 -> sRGB2020  */ /* impossible condition */
    },
    /* PQ_GFX_CS_eRGB_601 = 5 */ /* impossible condition */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> unknown   */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> YCbCr709  */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> YCbCr601  */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> YCbCr2020 */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> sRGB709   */
        &PQ_GFX_CSCTable_Identity              ,  /* sRGB601 -> sRGB601   */
        &PQ_GFX_CSCTable_Identity                 /* sRGB601 -> sRGB2020  */
    },
    /* PQ_GFX_CS_eRGB_2020 = 6 */ /* impossible condition */
    {
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity              ,  /* Identity */
        &PQ_GFX_CSCTable_Identity                 /* Identity */
    },
};

/***************************************************************************************/


HI_VOID GetCscType(HI_PQ_GFX_COLOR_SPACE_E eCSDrv, PQ_GFX_CS_S *pstCsAlg)
{
    switch (eCSDrv)
    {
        case PQ_GFX_CS_BT601_YUV_LIMITED:
            pstCsAlg->eCsType  = PQ_GFX_CS_eYCbCr_601;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_LMTD;
            break;
        case PQ_GFX_CS_BT601_YUV_FULL:
            pstCsAlg->eCsType  = PQ_GFX_CS_eYCbCr_601;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_FULL;
            break;
        case PQ_GFX_CS_BT709_YUV_LIMITED:
            pstCsAlg->eCsType  = PQ_GFX_CS_eYCbCr_709;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_LMTD;
            break;
        case PQ_GFX_CS_BT709_YUV_FULL:
            pstCsAlg->eCsType  = PQ_GFX_CS_eYCbCr_709;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_FULL;
            break;
        case PQ_GFX_CS_BT709_RGB_FULL:
            pstCsAlg->eCsType  = PQ_GFX_CS_eRGB_709;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_FULL;
            break;
        case PQ_GFX_CS_BT2020_YUV_LIMITED:
            pstCsAlg->eCsType  = PQ_GFX_CS_eYCbCr_2020;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_LMTD;
            break;
        case PQ_GFX_CS_BT2020_RGB_FULL:
            pstCsAlg->eCsType  = PQ_GFX_CS_eRGB_2020;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_FULL;
            break;
        default:
            pstCsAlg->eCsType  = PQ_GFX_CS_eUnknown;
            pstCsAlg->eCsRange = PQ_GFX_CS_RANGE_eUnknown;
            break;
    }
    return;
}

static HI_VOID pq_mng_CalcCscDcYUV2YUV(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    HI_S32 tmpBright  = (HI_S32)stGfxCscTuning.u32Bright - 50;

    switch (pstGfxCscMode->stGfxCscInputCs.eCsRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_F -> YUV_FL */
            pstCscDcCoef->csc_in_dc0 = 0;
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_L -> YUV_FL */
            pstCscDcCoef->csc_in_dc0 = -16;
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
        default:
            pstCscDcCoef->csc_in_dc0 = -16;
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
    }

    switch (pstGfxCscMode->stGfxCscOutputCs.eCsRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_FL -> YUV_F */
            pstCscDcCoef->csc_out_dc0 = tmpBright;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_FL -> YUV_L */
            pstCscDcCoef->csc_out_dc0 = tmpBright + 16;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
        default:
            pstCscDcCoef->csc_out_dc0 = tmpBright + 16;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcCscDcYUV2RGB(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    HI_S32 tmpBright  = (HI_S32)stGfxCscTuning.u32Bright - 50;
    HI_S32 tmpContrst = (HI_S32)stGfxCscTuning.u32Contrst;

    if (0 == tmpContrst)
    {
        tmpContrst = 1;
    }

    switch (pstGfxCscMode->stGfxCscInputCs.eCsRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_F -> RGB_FL */
            pstCscDcCoef->csc_in_dc0 = tmpBright * 50 / (HI_S32)tmpContrst;
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_L -> RGB_FL */
            pstCscDcCoef->csc_in_dc0 = -16 + (tmpBright * 50 / (HI_S32)tmpContrst);
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
        default:
            pstCscDcCoef->csc_in_dc0 = -16 + (tmpBright * 50 / (HI_S32)tmpContrst);
            pstCscDcCoef->csc_in_dc1 = -128;
            pstCscDcCoef->csc_in_dc2 = -128;
            break;
    }

    switch (pstGfxCscMode->stGfxCscInputCs.eCsRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_FL -> RGB_F */
            pstCscDcCoef->csc_out_dc0 = 0;
            pstCscDcCoef->csc_out_dc1 = 0;
            pstCscDcCoef->csc_out_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_FL -> RGB_L */
            pstCscDcCoef->csc_out_dc0 = 16;
            pstCscDcCoef->csc_out_dc1 = 16;
            pstCscDcCoef->csc_out_dc2 = 16;
            break;
        default:
            pstCscDcCoef->csc_out_dc0 = 0;
            pstCscDcCoef->csc_out_dc1 = 0;
            pstCscDcCoef->csc_out_dc2 = 0;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcCscDcRGB2YUV(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    HI_S32 tmpBright  = (HI_S32)stGfxCscTuning.u32Bright - 50;

    switch (pstGfxCscMode->stGfxCscInputCs.eCsRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_F -> YUV_FL */
            pstCscDcCoef->csc_in_dc0 = 0;
            pstCscDcCoef->csc_in_dc1 = 0;
            pstCscDcCoef->csc_in_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_L -> YUV_FL */
            pstCscDcCoef->csc_in_dc0 = 16;
            pstCscDcCoef->csc_in_dc1 = 16;
            pstCscDcCoef->csc_in_dc2 = 16;
            break;
        default:
            pstCscDcCoef->csc_in_dc0 = 0;
            pstCscDcCoef->csc_in_dc1 = 0;
            pstCscDcCoef->csc_in_dc2 = 0;
            break;
    }

    switch (pstGfxCscMode->stGfxCscOutputCs.eCsRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_FL -> YUV_F */
            pstCscDcCoef->csc_out_dc0 = tmpBright;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_FL -> YUV_L */
            pstCscDcCoef->csc_out_dc0 = tmpBright + 16;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
        default:
            pstCscDcCoef->csc_out_dc0 = tmpBright + 16;
            pstCscDcCoef->csc_out_dc1 = 128;
            pstCscDcCoef->csc_out_dc2 = 128;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcCscDcRGB2RGB(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    HI_S32 tmpBright  = (HI_S32)stGfxCscTuning.u32Bright - 50;

    switch (pstGfxCscMode->stGfxCscInputCs.eCsRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_F -> RGB_FL */
            pstCscDcCoef->csc_in_dc0 = 0;
            pstCscDcCoef->csc_in_dc1 = 0;
            pstCscDcCoef->csc_in_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_L -> RGB_FL */
            pstCscDcCoef->csc_in_dc0 = 16;
            pstCscDcCoef->csc_in_dc1 = 16;
            pstCscDcCoef->csc_in_dc2 = 16;
            break;
        default:
            pstCscDcCoef->csc_in_dc0 = 0;
            pstCscDcCoef->csc_in_dc1 = 0;
            pstCscDcCoef->csc_in_dc2 = 0;
            break;
    }

    switch (pstGfxCscMode->stGfxCscOutputCs.eCsRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_FL -> RGB_F */
            pstCscDcCoef->csc_out_dc0 = (tmpBright * 298 + 128) / 256;
            pstCscDcCoef->csc_out_dc1 = (tmpBright * 298 + 128) / 256;
            pstCscDcCoef->csc_out_dc2 = (tmpBright * 298 + 128) / 256;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_FL -> RGB_L */
            pstCscDcCoef->csc_out_dc0 = (tmpBright * 298 + 128) / 256 + 16; /* need check ?? */
            pstCscDcCoef->csc_out_dc1 = (tmpBright * 298 + 128) / 256 + 16; /* need check ?? */
            pstCscDcCoef->csc_out_dc2 = (tmpBright * 298 + 128) / 256 + 16; /* need check ?? */
            break;
        default:
            pstCscDcCoef->csc_out_dc0 = (tmpBright * 298 + 128) / 256;
            pstCscDcCoef->csc_out_dc1 = (tmpBright * 298 + 128) / 256;
            pstCscDcCoef->csc_out_dc2 = (tmpBright * 298 + 128) / 256;
            break;
    }

    return;
}


/***************************************************************************************
* func          : PQ_MNG_CalcCscDc
* description   : CNcomment: 计算DC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_MNG_CalcCscDc(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    if ((pstGfxCscMode->stGfxCscInputCs.eCsType >= PQ_GFX_CS_eYCbCr_709)
        && (pstGfxCscMode->stGfxCscInputCs.eCsType <= PQ_GFX_CS_eYCbCr_2020)) /* input YUV color space */
    {
        if ((pstGfxCscMode->stGfxCscOutputCs.eCsType >= PQ_GFX_CS_eYCbCr_709)
            && (pstGfxCscMode->stGfxCscOutputCs.eCsType <= PQ_GFX_CS_eYCbCr_2020)) /* YUV->YUV */
        {
            pq_mng_CalcCscDcYUV2YUV(pstGfxCscMode, pstCscDcCoef);
        }
        else /* YUV -> RGB */
        {
            pq_mng_CalcCscDcYUV2RGB(pstGfxCscMode, pstCscDcCoef);
        }
    }
    else /* input RGB color space */
    {
        if ((pstGfxCscMode->stGfxCscOutputCs.eCsType >= PQ_GFX_CS_eYCbCr_709)
            && (pstGfxCscMode->stGfxCscOutputCs.eCsType <= PQ_GFX_CS_eYCbCr_601)) /* RGB->YUV */
        {
            pq_mng_CalcCscDcRGB2YUV(pstGfxCscMode, pstCscDcCoef);
        }
        else /* RGB -> RGB */
        {
            pq_mng_CalcCscDcRGB2RGB(pstGfxCscMode, pstCscDcCoef);
        }
    }

    pstCscDcCoef->csc_in_dc0  = (HI_S32)(pstCscDcCoef->csc_in_dc0  * 4);
    pstCscDcCoef->csc_in_dc1  = (HI_S32)(pstCscDcCoef->csc_in_dc1  * 4);
    pstCscDcCoef->csc_in_dc2  = (HI_S32)(pstCscDcCoef->csc_in_dc2  * 4);
    pstCscDcCoef->csc_out_dc0 = (HI_S32)(pstCscDcCoef->csc_out_dc0 * 4);
    pstCscDcCoef->csc_out_dc1 = (HI_S32)(pstCscDcCoef->csc_out_dc1 * 4);
    pstCscDcCoef->csc_out_dc2 = (HI_S32)(pstCscDcCoef->csc_out_dc2 * 4);

    return;
}


/* for RGB -> RGB */
static HI_VOID PQ_MNG_CalcCscCoef_RGBtoRGB(PQ_GFX_CSC_TUNING_S *pstGfxCscTuning, const PQ_GFX_CSC_COEF_S *pMatrixOri, PQ_GFX_CSC_COEF_S *pMatrixDst)
{
    HI_S32 s32ContrstAdj, s32SatAdj, s32Rgain, s32Ggain, s32Bgain;
    HI_U32 u32Hue;


    /* 0-100 */
    s32ContrstAdj = (HI_S32)(pstGfxCscTuning->u32Contrst) - 50;
    s32ContrstAdj = 2 * s32ContrstAdj + 100;
    s32SatAdj     = (HI_S32)(pstGfxCscTuning->u32Satur) - 50;
    s32SatAdj     = 2 * s32SatAdj + 100;
    u32Hue        = pstGfxCscTuning->u32Hue * 60 / 100;

    s32Rgain = (HI_S32)(pstGfxCscTuning->u32Kr) - 50;
    s32Rgain = 2 * s32Rgain + 100;
    s32Ggain = (HI_S32)(pstGfxCscTuning->u32Kg) - 50;
    s32Ggain = 2 * s32Ggain + 100;
    s32Bgain = (HI_S32)(pstGfxCscTuning->u32Kb) - 50;
    s32Bgain = 2 * s32Bgain + 100;


    pMatrixDst->csc_coef00 = (HI_S32)(306 + (HI_S32)(242 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      + (HI_S32)(718 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Rgain / 10000;
    pMatrixDst->csc_coef01 = (HI_S32)(601 + (HI_S32)(475 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(601 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Ggain / 10000;
    pMatrixDst->csc_coef02 = (HI_S32)(117 - (HI_S32)(718 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(117 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Bgain / 10000;

    pMatrixDst->csc_coef10 = (HI_S32)(306 - (HI_S32)(299 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(306 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Rgain / 10000;
    pMatrixDst->csc_coef11 = (HI_S32)(601 - (HI_S32)(95  * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      + (HI_S32)(423 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Ggain / 10000;
    pMatrixDst->csc_coef12 = (HI_S32)(117 + (HI_S32)(394 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(117 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Bgain / 10000;

    pMatrixDst->csc_coef20 = (HI_S32)(306 + (HI_S32)(906 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(306 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Rgain / 10000;
    pMatrixDst->csc_coef21 = (HI_S32)(601 - (HI_S32)(760 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      - (HI_S32)(601 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Ggain / 10000;
    pMatrixDst->csc_coef22 = (HI_S32)(117 - (HI_S32)(146 * PQ_GFX_SIN_TABLE[u32Hue] * s32SatAdj) / 100000
                                      + (HI_S32)(907 * PQ_GFX_COS_TABLE[u32Hue] * s32SatAdj) / 100000)
                             * s32ContrstAdj * s32Bgain / 10000;

    return;
}

/* for RGB -> YCbCr */
static HI_VOID PQ_MNG_CalcCscCoef_RGBtoYCbCr(PQ_GFX_CSC_TUNING_S *pstGfxCscTuning, const PQ_GFX_CSC_COEF_S *pMatrixOri, PQ_GFX_CSC_COEF_S *pMatrixDst)
{
    HI_S32 s32ContrstAdj, s32SatAdj, s32Rgain, s32Ggain, s32Bgain;
    HI_U32 u32Hue;


    /* 0-100 */
    s32ContrstAdj = (HI_S32)(pstGfxCscTuning->u32Contrst) - 50;
    s32ContrstAdj = 2 * s32ContrstAdj + 100;
    s32SatAdj     = (HI_S32)(pstGfxCscTuning->u32Satur) - 50;
    s32SatAdj     = 2 * s32SatAdj + 100;
    u32Hue        = pstGfxCscTuning->u32Hue * 60 / 100;

    s32Rgain = (HI_S32)(pstGfxCscTuning->u32Kr) - 50;
    s32Rgain = 2 * s32Rgain + 100;
    s32Ggain = (HI_S32)(pstGfxCscTuning->u32Kg) - 50;
    s32Ggain = 2 * s32Ggain + 100;
    s32Bgain = (HI_S32)(pstGfxCscTuning->u32Kb) - 50;
    s32Bgain = 2 * s32Bgain + 100;


    pMatrixDst->csc_coef00 = pMatrixOri->csc_coef00 * s32ContrstAdj * s32Rgain / 10000;
    pMatrixDst->csc_coef01 = pMatrixOri->csc_coef01 * s32ContrstAdj * s32Ggain / 10000;
    pMatrixDst->csc_coef02 = pMatrixOri->csc_coef02 * s32ContrstAdj * s32Bgain / 10000;

    pMatrixDst->csc_coef10 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef10 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef20 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Rgain) / 1000000;
    pMatrixDst->csc_coef11 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Ggain) / 1000000;
    pMatrixDst->csc_coef12 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Bgain) / 1000000;

    pMatrixDst->csc_coef20 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef20 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef10 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Rgain) / 1000000;
    pMatrixDst->csc_coef21 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Ggain) / 1000000;
    pMatrixDst->csc_coef22 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Bgain) / 1000000;

    return;
}

/* for YCbCr -> RGB */
static HI_VOID PQ_MNG_CalcCscCoef_YCbCrtoRGB(PQ_GFX_CSC_TUNING_S *pstGfxCscTuning, const PQ_GFX_CSC_COEF_S *pMatrixOri, PQ_GFX_CSC_COEF_S *pMatrixDst)
{
    HI_S32 s32ContrstAdj, s32SatAdj, s32Rgain, s32Ggain, s32Bgain;
    HI_U32 u32Hue;

    /* 0-100 */
    s32ContrstAdj = (HI_S32)(pstGfxCscTuning->u32Contrst) - 50;
    s32ContrstAdj = 2 * s32ContrstAdj + 100;
    s32SatAdj     = (HI_S32)(pstGfxCscTuning->u32Satur) - 50;
    s32SatAdj     = 2 * s32SatAdj + 100;
    u32Hue        = pstGfxCscTuning->u32Hue * 60 / 100;

    s32Rgain = (HI_S32)(pstGfxCscTuning->u32Kr) - 50;
    s32Rgain = 2 * s32Rgain + 100;
    s32Ggain = (HI_S32)(pstGfxCscTuning->u32Kg) - 50;
    s32Ggain = 2 * s32Ggain + 100;
    s32Bgain = (HI_S32)(pstGfxCscTuning->u32Kb) - 50;
    s32Bgain = 2 * s32Bgain + 100;


    pMatrixDst->csc_coef00 = pMatrixOri->csc_coef00 * s32ContrstAdj * s32Rgain / 10000;
    pMatrixDst->csc_coef01 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef01 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - ((HI_S32)(pMatrixOri->csc_coef02 * PQ_GFX_SIN_TABLE[u32Hue])) * s32SatAdj / 100000)
                                      * s32ContrstAdj * s32Rgain) / 10000;
    pMatrixDst->csc_coef02 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef01 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000
                                       + ((HI_S32)(pMatrixOri->csc_coef02 * PQ_GFX_COS_TABLE[u32Hue])) * s32SatAdj / 100000)
                                      * s32ContrstAdj * s32Rgain) / 10000;

    pMatrixDst->csc_coef10 = pMatrixOri->csc_coef10 * s32ContrstAdj * s32Ggain / 10000;
    pMatrixDst->csc_coef11 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Ggain) / 1000000;
    pMatrixDst->csc_coef12 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_COS_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj * s32Ggain) / 1000000;

    pMatrixDst->csc_coef20 = pMatrixOri->csc_coef20 * s32ContrstAdj * s32Bgain / 10000;
    pMatrixDst->csc_coef21 = (HI_S32)((((HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_COS_TABLE[u32Hue])) * s32SatAdj / 100000
                                       - (HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32Bgain) / 10000;
    pMatrixDst->csc_coef22 = (HI_S32)((((HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_SIN_TABLE[u32Hue])) * s32SatAdj / 100000
                                       - (HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_COS_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32Bgain) / 10000;

    return;
}

/* 计算色温变换矩阵(for YCbCr->YCbCr 调节色温) */
static HI_VOID PQ_MNG_CalcCscTmprtMat(PQ_GFX_CSC_TUNING_S *pstGfxCscTuning, PQ_GFX_CSC_COEF_S *pMatrixOri, PQ_GFX_CSC_COEF_S *pMatrixDst)
{
    HI_S32 s32Rgain, s32Ggain, s32Bgain;
    PQ_GFX_CSC_COEF_S stYCbCrColrTemp;

    /* 0-100 */
    s32Rgain = (HI_S32)(pstGfxCscTuning->u32Kr) - 50;
    s32Rgain = 2 * s32Rgain + 100;
    s32Ggain = (HI_S32)(pstGfxCscTuning->u32Kg) - 50;
    s32Ggain = 2 * s32Ggain + 100;
    s32Bgain = (HI_S32)(pstGfxCscTuning->u32Kb) - 50;
    s32Bgain = 2 * s32Bgain + 100;


    /* Kr系列精度100, 色温矩阵系数精度1000 */
    stYCbCrColrTemp.csc_coef00 =  (114 * s32Bgain + 587 * s32Ggain + 299 * s32Rgain) / 100;
    stYCbCrColrTemp.csc_coef01 =  (232 * s32Bgain - 232 * s32Ggain) / 100;
    stYCbCrColrTemp.csc_coef02 = -(341 * s32Ggain - 341 * s32Rgain) / 100;

    stYCbCrColrTemp.csc_coef10 = -(289 * s32Ggain - 436 * s32Bgain + 147 * s32Rgain) / 100;
    stYCbCrColrTemp.csc_coef11 =  (886 * s32Bgain + 114 * s32Ggain) / 100;
    stYCbCrColrTemp.csc_coef12 =  (168 * s32Ggain - 168 * s32Rgain) / 100;

    stYCbCrColrTemp.csc_coef20 = -(100 * s32Bgain + 515 * s32Ggain - 615 * s32Rgain) / 100;
    stYCbCrColrTemp.csc_coef21 = -(203 * s32Bgain - 203 * s32Ggain) / 100;
    stYCbCrColrTemp.csc_coef22 =  (299 * s32Ggain + 701 * s32Rgain) / 100;

    pMatrixDst->csc_coef00 = (pMatrixOri->csc_coef00 * stYCbCrColrTemp.csc_coef00 +
                              pMatrixOri->csc_coef01 * stYCbCrColrTemp.csc_coef10 +
                              pMatrixOri->csc_coef02 * stYCbCrColrTemp.csc_coef20) / 1000;
    pMatrixDst->csc_coef01 = (pMatrixOri->csc_coef00 * stYCbCrColrTemp.csc_coef01 +
                              pMatrixOri->csc_coef01 * stYCbCrColrTemp.csc_coef11 +
                              pMatrixOri->csc_coef02 * stYCbCrColrTemp.csc_coef21) / 1000;
    pMatrixDst->csc_coef02 = (pMatrixOri->csc_coef00 * stYCbCrColrTemp.csc_coef02 +
                              pMatrixOri->csc_coef01 * stYCbCrColrTemp.csc_coef12 +
                              pMatrixOri->csc_coef02 * stYCbCrColrTemp.csc_coef22) / 1000;

    pMatrixDst->csc_coef10 = (pMatrixOri->csc_coef10 * stYCbCrColrTemp.csc_coef00 +
                              pMatrixOri->csc_coef11 * stYCbCrColrTemp.csc_coef10 +
                              pMatrixOri->csc_coef12 * stYCbCrColrTemp.csc_coef20) / 1000;
    pMatrixDst->csc_coef11 = (pMatrixOri->csc_coef10 * stYCbCrColrTemp.csc_coef01 +
                              pMatrixOri->csc_coef11 * stYCbCrColrTemp.csc_coef11 +
                              pMatrixOri->csc_coef12 * stYCbCrColrTemp.csc_coef21) / 1000;
    pMatrixDst->csc_coef12 = (pMatrixOri->csc_coef10 * stYCbCrColrTemp.csc_coef02 +
                              pMatrixOri->csc_coef11 * stYCbCrColrTemp.csc_coef12 +
                              pMatrixOri->csc_coef12 * stYCbCrColrTemp.csc_coef22) / 1000;

    pMatrixDst->csc_coef20 = (pMatrixOri->csc_coef20 * stYCbCrColrTemp.csc_coef00 +
                              pMatrixOri->csc_coef21 * stYCbCrColrTemp.csc_coef10 +
                              pMatrixOri->csc_coef22 * stYCbCrColrTemp.csc_coef20) / 1000;
    pMatrixDst->csc_coef21 = (pMatrixOri->csc_coef20 * stYCbCrColrTemp.csc_coef01 +
                              pMatrixOri->csc_coef21 * stYCbCrColrTemp.csc_coef11 +
                              pMatrixOri->csc_coef22 * stYCbCrColrTemp.csc_coef21) / 1000;
    pMatrixDst->csc_coef22 = (pMatrixOri->csc_coef20 * stYCbCrColrTemp.csc_coef02 +
                              pMatrixOri->csc_coef21 * stYCbCrColrTemp.csc_coef12 +
                              pMatrixOri->csc_coef22 * stYCbCrColrTemp.csc_coef22) / 1000;

    return;
}

/* 计算色彩空间转换系数 for YCbCr->YCbCr */
static HI_VOID PQ_MNG_CalcCscCoef_YCbCrtoYCbCr(PQ_GFX_CSC_TUNING_S *pstGfxCscTuning, const PQ_GFX_CSC_COEF_S *pMatrixOri, PQ_GFX_CSC_COEF_S *pMatrixDst)
{
    HI_S32 s32ContrstAdj, s32SatAdj;
    HI_U32 u32Hue;
    PQ_GFX_CSC_COEF_S DstCSCTableTmp;

    /* 0-100 */
    s32ContrstAdj = (HI_S32)(pstGfxCscTuning->u32Contrst) - 50;
    s32ContrstAdj = 2 * s32ContrstAdj + 100;
    s32SatAdj     = (HI_S32)(pstGfxCscTuning->u32Satur) - 50;
    s32SatAdj     = 2 * s32SatAdj + 100;
    u32Hue        = pstGfxCscTuning->u32Hue * 60 / 100;


    pMatrixDst->csc_coef00 = pMatrixOri->csc_coef00 * s32ContrstAdj / 100;
    pMatrixDst->csc_coef01 = pMatrixOri->csc_coef01 * s32ContrstAdj / 100;
    pMatrixDst->csc_coef02 = pMatrixOri->csc_coef02 * s32ContrstAdj / 100;

    pMatrixDst->csc_coef10 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef10 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef20 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;
    pMatrixDst->csc_coef11 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;
    pMatrixDst->csc_coef12 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       + (HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;

    pMatrixDst->csc_coef20 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef20 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef10 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;
    pMatrixDst->csc_coef21 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef21 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef11 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;
    pMatrixDst->csc_coef22 = (HI_S32)(((HI_S32)(pMatrixOri->csc_coef22 * PQ_GFX_COS_TABLE[u32Hue]) / 1000
                                       - (HI_S32)(pMatrixOri->csc_coef12 * PQ_GFX_SIN_TABLE[u32Hue]) / 1000)
                                      * s32ContrstAdj * s32SatAdj) / 10000;

    PQ_MNG_CalcCscTmprtMat(pstGfxCscTuning, pMatrixDst, &DstCSCTableTmp);

    pMatrixDst->csc_coef00 = DstCSCTableTmp.csc_coef00;
    pMatrixDst->csc_coef01 = DstCSCTableTmp.csc_coef01;
    pMatrixDst->csc_coef02 = DstCSCTableTmp.csc_coef02;
    pMatrixDst->csc_coef10 = DstCSCTableTmp.csc_coef10;
    pMatrixDst->csc_coef11 = DstCSCTableTmp.csc_coef11;
    pMatrixDst->csc_coef12 = DstCSCTableTmp.csc_coef12;
    pMatrixDst->csc_coef20 = DstCSCTableTmp.csc_coef20;
    pMatrixDst->csc_coef21 = DstCSCTableTmp.csc_coef21;
    pMatrixDst->csc_coef22 = DstCSCTableTmp.csc_coef22;

    return;
}

/***************************************************************************************
* func          : PQ_MNG_CalcCscMatrix
* description   : CNcomment: 计算CSC距阵 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/

/* Note: 这部分的计算, 精度还是按照原来的10bits配置, 可以在运算的最后乘上精度 */
HI_VOID PQ_MNG_CalcCscMatrix(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_COEF_S *pstGfxCscCoef, HI_BOOL bIsBGRIn)
{
    const PQ_GFX_CSC_COEF_S *pCSCMatrixOri  = HI_NULL;
    PQ_GFX_CSC_COEF_S        stCscTbl       = {0};

    if ((pstGfxCscMode == HI_NULL)
        || (pstGfxCscCoef == HI_NULL))
    {
        return;
    }

    if ((pstGfxCscMode->stGfxCscInputCs.eCsType == PQ_GFX_CS_TYPE_BUTT)
        || (pstGfxCscMode->stGfxCscOutputCs.eCsType == PQ_GFX_CS_TYPE_BUTT))
    {
        return;
    }

    pCSCMatrixOri = PQ_GFX_CSC_MatrixTbl[pstGfxCscMode->stGfxCscInputCs.eCsType][pstGfxCscMode->stGfxCscOutputCs.eCsType];
    /* correct the 3*3 matrix according to the value range of input and output of stCscTbl */
    if (pstGfxCscMode->stGfxCscInputCs.eCsType >= PQ_GFX_CS_eRGB_709) /* input color space RGB */
    {
        if (pstGfxCscMode->stGfxCscOutputCs.eCsType >= PQ_GFX_CS_eRGB_709) /* RGB -> RGB */
        {
            PQ_MNG_CalcCscCoef_RGBtoRGB(&stGfxCscTuning, pCSCMatrixOri, &stCscTbl);
        }
        else /* RGB -> YCbCr */
        {
            PQ_MNG_CalcCscCoef_RGBtoYCbCr(&stGfxCscTuning, pCSCMatrixOri, &stCscTbl);
        }
    }
    else /* input color space YCbCr */
    {
        if (pstGfxCscMode->stGfxCscOutputCs.eCsType >= PQ_GFX_CS_eRGB_709) /* YCbCr -> RGB */
        {
            PQ_MNG_CalcCscCoef_YCbCrtoRGB(&stGfxCscTuning, pCSCMatrixOri, &stCscTbl);
        }
        else /* YCbCr -> YCbCr */
        {
            PQ_MNG_CalcCscCoef_YCbCrtoYCbCr(&stGfxCscTuning, pCSCMatrixOri, &stCscTbl);
        }
    }
    if (HI_FALSE == bIsBGRIn)
    {
        pstGfxCscCoef->csc_coef00 = stCscTbl.csc_coef00;
        pstGfxCscCoef->csc_coef01 = stCscTbl.csc_coef01;
        pstGfxCscCoef->csc_coef02 = stCscTbl.csc_coef02;
        pstGfxCscCoef->csc_coef10 = stCscTbl.csc_coef10;
        pstGfxCscCoef->csc_coef11 = stCscTbl.csc_coef11;
        pstGfxCscCoef->csc_coef12 = stCscTbl.csc_coef12;
        pstGfxCscCoef->csc_coef20 = stCscTbl.csc_coef20;
        pstGfxCscCoef->csc_coef21 = stCscTbl.csc_coef21;
        pstGfxCscCoef->csc_coef22 = stCscTbl.csc_coef22;
    }
    else
    {
        pstGfxCscCoef->csc_coef00 = stCscTbl.csc_coef02;
        pstGfxCscCoef->csc_coef01 = stCscTbl.csc_coef01;
        pstGfxCscCoef->csc_coef02 = stCscTbl.csc_coef00;
        pstGfxCscCoef->csc_coef10 = stCscTbl.csc_coef12;
        pstGfxCscCoef->csc_coef11 = stCscTbl.csc_coef11;
        pstGfxCscCoef->csc_coef12 = stCscTbl.csc_coef10;
        pstGfxCscCoef->csc_coef20 = stCscTbl.csc_coef22;
        pstGfxCscCoef->csc_coef21 = stCscTbl.csc_coef21;
        pstGfxCscCoef->csc_coef22 = stCscTbl.csc_coef20;
    }

    return;
}


/***************************************************************************************
* func          : PQ_MNG_CalcWcgMatrix
* description   : CNcomment: 计算CSC距阵 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_MNG_CalcWcgMatrix(PQ_GFX_CSC_MODE_S *pstGfxCscMode, PQ_GFX_CSC_COEF_S *pstGfxCscCoef)
{
    PQ_GFX_CS_TYPE_E eGfxInputType, eGfxOutputType;
    eGfxInputType  = pstGfxCscMode->stGfxCscInputCs.eCsType;
    eGfxOutputType = pstGfxCscMode->stGfxCscOutputCs.eCsType;

    /* List all possible condition; Note: when RGB -> YCbCr, there in not tran like "709/601 <--> 2020" */
    if (PQ_GFX_CS_eRGB_2020 == eGfxInputType) /* Input RGB_2020 */
    {
        if (PQ_GFX_CS_eYCbCr_2020 == eGfxOutputType) /* RGB_2020 -> YCbCr_2020 */
        {
            pstGfxCscCoef->csc_coef00 =  0.2256;
            pstGfxCscCoef->csc_coef01 =  0.5823;
            pstGfxCscCoef->csc_coef02 =  0.0509;
            pstGfxCscCoef->csc_coef10 = -0.1227;
            pstGfxCscCoef->csc_coef11 = -0.3166;
            pstGfxCscCoef->csc_coef12 =  0.4392;
            pstGfxCscCoef->csc_coef20 =  0.4392;
            pstGfxCscCoef->csc_coef21 = -0.4039;
            pstGfxCscCoef->csc_coef22 = -0.0353;
        }
        else if (PQ_GFX_CS_eRGB_2020 == eGfxOutputType) /* RGB_2020 -> RGB_2020 */ /* identity */
        {
            pstGfxCscCoef->csc_coef00 = 1;
            pstGfxCscCoef->csc_coef01 = 0;
            pstGfxCscCoef->csc_coef02 = 0;
            pstGfxCscCoef->csc_coef10 = 0;
            pstGfxCscCoef->csc_coef11 = 1;
            pstGfxCscCoef->csc_coef12 = 0;
            pstGfxCscCoef->csc_coef20 = 0;
            pstGfxCscCoef->csc_coef21 = 0;
            pstGfxCscCoef->csc_coef22 = 1;
        }
        else if (PQ_GFX_CS_eRGB_709 == eGfxOutputType) /* RGB_2020 -> RGB_709 */
        {
            pstGfxCscCoef->csc_coef00 =  1.6605;
            pstGfxCscCoef->csc_coef01 = -0.5877;
            pstGfxCscCoef->csc_coef02 = -0.0728;
            pstGfxCscCoef->csc_coef10 = -0.1246;
            pstGfxCscCoef->csc_coef11 =  1.1329;
            pstGfxCscCoef->csc_coef12 = -0.0083;
            pstGfxCscCoef->csc_coef20 = -0.0182;
            pstGfxCscCoef->csc_coef21 = -0.1006;
            pstGfxCscCoef->csc_coef22 =  1.1188;
        }
        else if (PQ_GFX_CS_eRGB_601 == eGfxOutputType) /* RGB_2020 -> RGB_601 */ /* same to RGB_2020 -> RGB_709 */
        {
            pstGfxCscCoef->csc_coef00 =  1.6605;
            pstGfxCscCoef->csc_coef01 = -0.5877;
            pstGfxCscCoef->csc_coef02 = -0.0728;
            pstGfxCscCoef->csc_coef10 = -0.1246;
            pstGfxCscCoef->csc_coef11 =  1.1329;
            pstGfxCscCoef->csc_coef12 = -0.0083;
            pstGfxCscCoef->csc_coef20 = -0.0182;
            pstGfxCscCoef->csc_coef21 = -0.1006;
            pstGfxCscCoef->csc_coef22 =  1.1188;
        }
        else /* identity */
        {
            pstGfxCscCoef->csc_coef00 = 1;
            pstGfxCscCoef->csc_coef01 = 0;
            pstGfxCscCoef->csc_coef02 = 0;
            pstGfxCscCoef->csc_coef10 = 0;
            pstGfxCscCoef->csc_coef11 = 1;
            pstGfxCscCoef->csc_coef12 = 0;
            pstGfxCscCoef->csc_coef20 = 0;
            pstGfxCscCoef->csc_coef21 = 0;
            pstGfxCscCoef->csc_coef22 = 1;
        }
    }
    else if (PQ_GFX_CS_eRGB_709 == eGfxInputType) /* Input RGB_709 */
    {
        if (PQ_GFX_CS_eYCbCr_709 == eGfxOutputType) /* RGB_709 -> YCbCr_709 */
        {
            pstGfxCscCoef->csc_coef00 =  0.1826;
            pstGfxCscCoef->csc_coef01 =  0.6142;
            pstGfxCscCoef->csc_coef02 =  0.0620;
            pstGfxCscCoef->csc_coef10 = -0.1006;
            pstGfxCscCoef->csc_coef11 = -0.3386;
            pstGfxCscCoef->csc_coef12 =  0.4392;
            pstGfxCscCoef->csc_coef20 =  0.4392;
            pstGfxCscCoef->csc_coef21 = -0.3989;
            pstGfxCscCoef->csc_coef22 = -0.0403;
        }
        else if (PQ_GFX_CS_eRGB_2020 == eGfxOutputType) /* RGB_709 -> RGB_2020 */
        {
            pstGfxCscCoef->csc_coef00 = 0.6274;
            pstGfxCscCoef->csc_coef01 = 0.3293;
            pstGfxCscCoef->csc_coef02 = 0.0433;
            pstGfxCscCoef->csc_coef10 = 0.0692;
            pstGfxCscCoef->csc_coef11 = 0.9194;
            pstGfxCscCoef->csc_coef12 = 0.0114;
            pstGfxCscCoef->csc_coef20 = 0.0163;
            pstGfxCscCoef->csc_coef21 = 0.0880;
            pstGfxCscCoef->csc_coef22 = 0.8956;
        }
        else /* identity */
        {
            pstGfxCscCoef->csc_coef00 = 1;
            pstGfxCscCoef->csc_coef01 = 0;
            pstGfxCscCoef->csc_coef02 = 0;
            pstGfxCscCoef->csc_coef10 = 0;
            pstGfxCscCoef->csc_coef11 = 1;
            pstGfxCscCoef->csc_coef12 = 0;
            pstGfxCscCoef->csc_coef20 = 0;
            pstGfxCscCoef->csc_coef21 = 0;
            pstGfxCscCoef->csc_coef22 = 1;
        }
    }
    else if (PQ_GFX_CS_eRGB_601 == eGfxInputType) /* Input RGB_601; do not insure effect */
    {
        if (PQ_GFX_CS_eRGB_2020 == eGfxOutputType) /* RGB_601 -> RGB_2020; same to RGB_709 -> RGB_2020 */
        {
            pstGfxCscCoef->csc_coef00 = 0.6274;
            pstGfxCscCoef->csc_coef01 = 0.3293;
            pstGfxCscCoef->csc_coef02 = 0.0433;
            pstGfxCscCoef->csc_coef10 = 0.0692;
            pstGfxCscCoef->csc_coef11 = 0.9194;
            pstGfxCscCoef->csc_coef12 = 0.0114;
            pstGfxCscCoef->csc_coef20 = 0.0163;
            pstGfxCscCoef->csc_coef21 = 0.0880;
            pstGfxCscCoef->csc_coef22 = 0.8956;
        }
        else /* Null */
        {
            return;
        }
    }
    else /* identity; impossible condition: Input YCbCr_709; Input YCbCr_601; Input YCbCr_2020 */
    {
        pstGfxCscCoef->csc_coef00 = 1;
        pstGfxCscCoef->csc_coef01 = 0;
        pstGfxCscCoef->csc_coef02 = 0;
        pstGfxCscCoef->csc_coef10 = 0;
        pstGfxCscCoef->csc_coef11 = 1;
        pstGfxCscCoef->csc_coef12 = 0;
        pstGfxCscCoef->csc_coef20 = 0;
        pstGfxCscCoef->csc_coef21 = 0;
        pstGfxCscCoef->csc_coef22 = 1;
    }

    pstGfxCscCoef->csc_coef00 = (HI_S32)(pstGfxCscCoef->csc_coef00 * CscPre);
    pstGfxCscCoef->csc_coef01 = (HI_S32)(pstGfxCscCoef->csc_coef01 * CscPre);
    pstGfxCscCoef->csc_coef02 = (HI_S32)(pstGfxCscCoef->csc_coef02 * CscPre);
    pstGfxCscCoef->csc_coef10 = (HI_S32)(pstGfxCscCoef->csc_coef10 * CscPre);
    pstGfxCscCoef->csc_coef11 = (HI_S32)(pstGfxCscCoef->csc_coef11 * CscPre);
    pstGfxCscCoef->csc_coef12 = (HI_S32)(pstGfxCscCoef->csc_coef12 * CscPre);
    pstGfxCscCoef->csc_coef20 = (HI_S32)(pstGfxCscCoef->csc_coef20 * CscPre);
    pstGfxCscCoef->csc_coef21 = (HI_S32)(pstGfxCscCoef->csc_coef21 * CscPre);
    pstGfxCscCoef->csc_coef22 = (HI_S32)(pstGfxCscCoef->csc_coef22 * CscPre);

    return;
}

static HI_VOID pq_mng_CalcWcgDcYUV2YUV(PQ_GFX_CSC_MODE_S *pstCscMode, PQ_GFX_CSC_DCCOEF_S *pstGfxCscDcCoef)
{
    PQ_GFX_CS_RANGE_E eGfxInputRange  = pstCscMode->stGfxCscInputCs.eCsRange;
    PQ_GFX_CS_RANGE_E eGfxOutputRange = pstCscMode->stGfxCscOutputCs.eCsRange;

    switch (eGfxInputRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_F -> YUV_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_L -> YUV_FL */
            pstGfxCscDcCoef->csc_in_dc0 = -16;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
        default:
            pstGfxCscDcCoef->csc_in_dc0 = -16;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
    }

    switch (eGfxOutputRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_FL -> YUV_F */
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_FL -> YUV_L */
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
        default:
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcWcgDcYUV2RGB(PQ_GFX_CSC_MODE_S *pstCscMode, PQ_GFX_CSC_DCCOEF_S *pstGfxCscDcCoef)
{
    PQ_GFX_CS_RANGE_E eGfxInputRange  = pstCscMode->stGfxCscInputCs.eCsRange;
    PQ_GFX_CS_RANGE_E eGfxOutputRange = pstCscMode->stGfxCscOutputCs.eCsRange;

    switch (eGfxInputRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_F -> RGB_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_L -> RGB_FL */
            pstGfxCscDcCoef->csc_in_dc0 = -16;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
        default:
            pstGfxCscDcCoef->csc_in_dc0 = -16;
            pstGfxCscDcCoef->csc_in_dc1 = -128;
            pstGfxCscDcCoef->csc_in_dc2 = -128;
            break;
    }

    switch (eGfxOutputRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* YUV_FL -> RGB_F */
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 0;
            pstGfxCscDcCoef->csc_out_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* YUV_FL -> RGB_L */
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 16;
            pstGfxCscDcCoef->csc_out_dc2 = 16;
            break;
        default:
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 0;
            pstGfxCscDcCoef->csc_out_dc2 = 0;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcWcgDcRGB2YUV(PQ_GFX_CSC_MODE_S *pstCscMode, PQ_GFX_CSC_DCCOEF_S *pstGfxCscDcCoef)
{
    PQ_GFX_CS_RANGE_E eGfxInputRange  = pstCscMode->stGfxCscInputCs.eCsRange;
    PQ_GFX_CS_RANGE_E eGfxOutputRange = pstCscMode->stGfxCscOutputCs.eCsRange;

    switch (eGfxInputRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_F -> YUV_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = 0;
            pstGfxCscDcCoef->csc_in_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_L -> YUV_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 16;
            pstGfxCscDcCoef->csc_in_dc1 = 16;
            pstGfxCscDcCoef->csc_in_dc2 = 16;
            break;
        default:
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = 0;
            pstGfxCscDcCoef->csc_in_dc2 = 0;
            break;
    }

    switch (eGfxOutputRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_FL -> YUV_F */
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_FL -> YUV_L */
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
        default:
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 128;
            pstGfxCscDcCoef->csc_out_dc2 = 128;
            break;
    }

    return;
}

static HI_VOID pq_mng_CalcWcgDcRGB2RGB(PQ_GFX_CSC_MODE_S *pstCscMode, PQ_GFX_CSC_DCCOEF_S *pstGfxCscDcCoef)
{
    PQ_GFX_CS_RANGE_E eGfxInputRange  = pstCscMode->stGfxCscInputCs.eCsRange;
    PQ_GFX_CS_RANGE_E eGfxOutputRange = pstCscMode->stGfxCscOutputCs.eCsRange;

    switch (eGfxInputRange) /* input DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_F -> RGB_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = 0;
            pstGfxCscDcCoef->csc_in_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_L -> RGB_FL */
            pstGfxCscDcCoef->csc_in_dc0 = 16;
            pstGfxCscDcCoef->csc_in_dc1 = 16;
            pstGfxCscDcCoef->csc_in_dc2 = 16;
            break;
        default:
            pstGfxCscDcCoef->csc_in_dc0 = 0;
            pstGfxCscDcCoef->csc_in_dc1 = 0;
            pstGfxCscDcCoef->csc_in_dc2 = 0;
            break;
    }

    switch (eGfxOutputRange) /* output DC */
    {
        case PQ_GFX_CS_RANGE_FULL: /* RGB_FL -> RGB_F */
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 0;
            pstGfxCscDcCoef->csc_out_dc2 = 0;
            break;
        case PQ_GFX_CS_RANGE_LMTD: /* RGB_FL -> RGB_L */
            pstGfxCscDcCoef->csc_out_dc0 = 16;
            pstGfxCscDcCoef->csc_out_dc1 = 16;
            pstGfxCscDcCoef->csc_out_dc2 = 16;
            break;
        default:
            pstGfxCscDcCoef->csc_out_dc0 = 0;
            pstGfxCscDcCoef->csc_out_dc1 = 0;
            pstGfxCscDcCoef->csc_out_dc2 = 0;
            break;
    }

    return;
}


/***************************************************************************************
* func          : PQ_MNG_CalcWcgDc
* description   : CNcomment: 计算DC系数 CNend\n
* param[in]     : HI_VOID
* retval        : NA
* others:       : NA
***************************************************************************************/
HI_VOID PQ_MNG_CalcWcgDc(PQ_GFX_CSC_MODE_S *pstCscMode, PQ_GFX_CSC_DCCOEF_S *pstGfxCscDcCoef)
{
    PQ_GFX_CS_TYPE_E eGfxInputType   = pstCscMode->stGfxCscInputCs.eCsType;
    PQ_GFX_CS_TYPE_E eGfxOutputType  = pstCscMode->stGfxCscOutputCs.eCsType;

    /* List all possible condition */
    if (eGfxInputType >= PQ_GFX_CS_eYCbCr_709 && eGfxInputType <= PQ_GFX_CS_eYCbCr_2020) /* input YUV color space */
    {
        if (eGfxOutputType >= PQ_GFX_CS_eYCbCr_709 && eGfxOutputType <= PQ_GFX_CS_eYCbCr_2020) /* YUV -> YUV */
        {
            pq_mng_CalcWcgDcYUV2YUV(pstCscMode, pstGfxCscDcCoef);
        }
        else /* YUV -> RGB */
        {
            pq_mng_CalcWcgDcYUV2RGB(pstCscMode, pstGfxCscDcCoef);
        }
    }
    else /* input RGB color space */
    {
        if (eGfxOutputType >= PQ_GFX_CS_eYCbCr_709 && eGfxOutputType <= PQ_GFX_CS_eYCbCr_601) /* RGB -> YUV */
        {
            pq_mng_CalcWcgDcRGB2YUV(pstCscMode, pstGfxCscDcCoef);
        }
        else /* RGB -> RGB */
        {
            pq_mng_CalcWcgDcRGB2RGB(pstCscMode, pstGfxCscDcCoef);
        }
    }

    pstGfxCscDcCoef->csc_in_dc0  = (HI_S32)(pstGfxCscDcCoef->csc_in_dc0  * 4);
    pstGfxCscDcCoef->csc_in_dc1  = (HI_S32)(pstGfxCscDcCoef->csc_in_dc1  * 4);
    pstGfxCscDcCoef->csc_in_dc2  = (HI_S32)(pstGfxCscDcCoef->csc_in_dc2  * 4);
    pstGfxCscDcCoef->csc_out_dc0 = (HI_S32)(pstGfxCscDcCoef->csc_out_dc0 * 4);
    pstGfxCscDcCoef->csc_out_dc1 = (HI_S32)(pstGfxCscDcCoef->csc_out_dc1 * 4);
    pstGfxCscDcCoef->csc_out_dc2 = (HI_S32)(pstGfxCscDcCoef->csc_out_dc2 * 4);

    return;
}

static HI_S32 pq_mng_CalcGfxCscCoef(HI_PQ_CSC_CRTL_S *pstCscCtrl, HI_PQ_GFX_CSC_MODE_S *pstGfxCscPara, PQ_GFX_CSC_COEF_S *pstCscCoef, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    PQ_GFX_CSC_MODE_S      stGfxCscMode;
    PQ_GFX_CS_S            stAlgCSIn, stAlgCSOut;

    /* Tran Mode, CsType and CsRange */
    GetCscType(pstGfxCscPara->eInputCS,  &stAlgCSIn);
    GetCscType(pstGfxCscPara->eOutputCS, &stAlgCSOut);
    /* CSC InputMode and Output Mode */
    stGfxCscMode.stGfxCscInputCs.eCsType   = stAlgCSIn.eCsType;
    stGfxCscMode.stGfxCscInputCs.eCsRange  = stAlgCSIn.eCsRange;
    stGfxCscMode.stGfxCscOutputCs.eCsType  = stAlgCSOut.eCsType;
    stGfxCscMode.stGfxCscOutputCs.eCsRange = stAlgCSOut.eCsRange;

    if (HI_PQ_CSC_NORMAL_SETTING == pstCscCtrl->enCscType) /* It does not be tuned */
    {
        /* About 2020 */ /* Don not care impossible  */
        if ((PQ_GFX_CS_eYCbCr_2020 == stAlgCSIn.eCsType ) || (PQ_GFX_CS_eRGB_2020 == stAlgCSIn.eCsType)
            || (PQ_GFX_CS_eYCbCr_2020 == stAlgCSOut.eCsType) || (PQ_GFX_CS_eRGB_2020 == stAlgCSOut.eCsType))
        {
            PQ_MNG_CalcWcgDc    (&stGfxCscMode, pstCscDcCoef);
            PQ_MNG_CalcWcgMatrix(&stGfxCscMode, pstCscCoef);
        }
        else /* Not About 2020 */
        {
            PQ_MNG_CalcCscDc    (&stGfxCscMode, pstCscDcCoef);
            PQ_MNG_CalcCscMatrix(&stGfxCscMode, pstCscCoef, bIsBGRIn);
        }
    }
    else /* may need tuning; Calc */
    {
        if (HI_PQ_CSC_TUNING_GP0 == pstCscCtrl->enCscType)
        {
            stGfxCscTuning.u32Bright  = sg_stHDPictureSetting.u16Brightness;
            stGfxCscTuning.u32Contrst = sg_stHDPictureSetting.u16Contrast;
            stGfxCscTuning.u32Hue     = sg_stHDPictureSetting.u16Hue;
            stGfxCscTuning.u32Satur   = sg_stHDPictureSetting.u16Saturation;
            stGfxCscTuning.u32Kr      = 50;
            stGfxCscTuning.u32Kg      = 50;
            stGfxCscTuning.u32Kb      = 50;
        }
        else if (HI_PQ_CSC_TUNING_GP1 == pstCscCtrl->enCscType)
        {
            stGfxCscTuning.u32Bright  = sg_stSDPictureSetting.u16Brightness;
            stGfxCscTuning.u32Contrst = sg_stSDPictureSetting.u16Contrast;
            stGfxCscTuning.u32Hue     = sg_stSDPictureSetting.u16Hue;
            stGfxCscTuning.u32Satur   = sg_stSDPictureSetting.u16Saturation;
            stGfxCscTuning.u32Kr      = 50;
            stGfxCscTuning.u32Kg      = 50;
            stGfxCscTuning.u32Kb      = 50;
        }

        if (HI_TRUE == pstCscCtrl->bCSCEn) /* reload picture setting for DTV request */
        {
            PQ_MNG_CalcCscDc    (&stGfxCscMode, pstCscDcCoef);
            PQ_MNG_CalcCscMatrix(&stGfxCscMode, pstCscCoef, bIsBGRIn);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 pq_mng_TransGfxCscCoef(HI_PQ_CSC_CRTL_S *pstCscCtrl,
                                     PQ_GFX_CSC_COEF_S *pstInCscCoef,
                                     PQ_GFX_CSC_DCCOEF_S *pstInCscDcCoef,
                                     PQ_GFX_CSC_COEF_S *pstOutCscCoef,
                                     PQ_GFX_CSC_DCCOEF_S *pstOutCscDcCoef)
{
#if defined(CHIP_TYPE_hi3798cv200)
    if (10 < pstCscCtrl->u32CscPre) /* the precision of GP1 is fixed at 10 */
    {
        HI_U32 u32CscScalePre = 1 << (pstCscCtrl->u32CscPre - 10);

        pstOutCscCoef->csc_coef00 = (HI_S32)(pstInCscCoef->csc_coef00 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef01 = (HI_S32)(pstInCscCoef->csc_coef01 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef02 = (HI_S32)(pstInCscCoef->csc_coef02 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef10 = (HI_S32)(pstInCscCoef->csc_coef10 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef11 = (HI_S32)(pstInCscCoef->csc_coef11 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef12 = (HI_S32)(pstInCscCoef->csc_coef12 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef20 = (HI_S32)(pstInCscCoef->csc_coef20 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef21 = (HI_S32)(pstInCscCoef->csc_coef21 * u32CscScalePre / 2);
        pstOutCscCoef->csc_coef22 = (HI_S32)(pstInCscCoef->csc_coef22 * u32CscScalePre / 2);

        pstOutCscDcCoef->csc_in_dc0  = (HI_S32)(pstInCscDcCoef->csc_in_dc0  * u32CscScalePre);
        pstOutCscDcCoef->csc_in_dc1  = (HI_S32)(pstInCscDcCoef->csc_in_dc1  * u32CscScalePre);
        pstOutCscDcCoef->csc_in_dc2  = (HI_S32)(pstInCscDcCoef->csc_in_dc2  * u32CscScalePre);
        pstOutCscDcCoef->csc_out_dc0 = (HI_S32)(pstInCscDcCoef->csc_out_dc0 * u32CscScalePre);
        pstOutCscDcCoef->csc_out_dc1 = (HI_S32)(pstInCscDcCoef->csc_out_dc1 * u32CscScalePre);
        pstOutCscDcCoef->csc_out_dc2 = (HI_S32)(pstInCscDcCoef->csc_out_dc2 * u32CscScalePre);
    }
    else
#endif
    {
        pstOutCscCoef->csc_coef00 = (HI_S32)(pstInCscCoef->csc_coef00);
        pstOutCscCoef->csc_coef01 = (HI_S32)(pstInCscCoef->csc_coef01);
        pstOutCscCoef->csc_coef02 = (HI_S32)(pstInCscCoef->csc_coef02);
        pstOutCscCoef->csc_coef10 = (HI_S32)(pstInCscCoef->csc_coef10);
        pstOutCscCoef->csc_coef11 = (HI_S32)(pstInCscCoef->csc_coef11);
        pstOutCscCoef->csc_coef12 = (HI_S32)(pstInCscCoef->csc_coef12);
        pstOutCscCoef->csc_coef20 = (HI_S32)(pstInCscCoef->csc_coef20);
        pstOutCscCoef->csc_coef21 = (HI_S32)(pstInCscCoef->csc_coef21);
        pstOutCscCoef->csc_coef22 = (HI_S32)(pstInCscCoef->csc_coef22);

        pstOutCscDcCoef->csc_in_dc0  = (HI_S32)(pstInCscDcCoef->csc_in_dc0);
        pstOutCscDcCoef->csc_in_dc1  = (HI_S32)(pstInCscDcCoef->csc_in_dc1);
        pstOutCscDcCoef->csc_in_dc2  = (HI_S32)(pstInCscDcCoef->csc_in_dc2);
        pstOutCscDcCoef->csc_out_dc0 = (HI_S32)(pstInCscDcCoef->csc_out_dc0);
        pstOutCscDcCoef->csc_out_dc1 = (HI_S32)(pstInCscDcCoef->csc_out_dc1);
        pstOutCscDcCoef->csc_out_dc2 = (HI_S32)(pstInCscDcCoef->csc_out_dc2);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetGfxCscCoef(HI_PQ_CSC_CRTL_S *pstCscCtrl, HI_PQ_GFX_CSC_MODE_S *pstGfxCscPara, PQ_GFX_CSC_COEF_S *pstCscCoef, PQ_GFX_CSC_DCCOEF_S *pstCscDcCoef)
{
    PQ_GFX_CSC_COEF_S      stPQGfxCscCoef   = {0};
    PQ_GFX_CSC_DCCOEF_S    stPQGfxCscDcCoef = {0};

    pq_mng_CalcGfxCscCoef(pstCscCtrl, pstGfxCscPara, &stPQGfxCscCoef, &stPQGfxCscDcCoef);
    pq_mng_TransGfxCscCoef(pstCscCtrl, &stPQGfxCscCoef, &stPQGfxCscDcCoef, pstCscCoef, pstCscDcCoef);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetGfxCscCoef(HI_PQ_GFX_LAYER_E enGfxCscLayer, HI_PQ_GFX_CSC_MODE_S *pstGfxCscMode)
{
    HI_PQ_CSC_CRTL_S      stCscCtrl;
    HI_PQ_GFX_CSC_MODE_S  stGfxCscPara;
    PQ_GFX_CSC_COEF_S     stCscCoef;
    PQ_GFX_CSC_DCCOEF_S   stCscDcCoef;


    /* according to the input and output csc mode of layer, get the strategy of layer */
    if (PQ_GFX_GP0 == enGfxCscLayer)
    {
        /* Data come from framebuffer, so input must be 709_RGB_F; output need mix with VP, so output is yuv */
        if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
            && (PQ_GFX_CS_BT2020_YUV_LIMITED == pstGfxCscMode->eOutputCS))
        {
#ifndef HI_BUILD_IN_BOOT
#if defined(CHIP_TYPE_hi3798cv200)
            PQ_HAL_GFX_CSC_MODE_S stHalGfxCscPara;
            stHalGfxCscPara.eInputCS  = PQ_HAL_GFX_CS_BT709_RGB_FULL;
            stHalGfxCscPara.eOutputCS = PQ_HAL_GFX_CS_BT2020_RGB_FULL;
            PQ_HAL_SetGfxBt2020Mode(&stHalGfxCscPara);

            stHalGfxCscPara.eInputCS  = PQ_HAL_GFX_CS_BT2020_RGB_FULL;
            stHalGfxCscPara.eOutputCS = PQ_HAL_GFX_CS_BT2020_YUV_LIMITED;
            PQ_HAL_SetGfxRgbtYuvMode(&stHalGfxCscPara);
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
            PQ_HAL_GFX_CSC_MODE_S stHalGfxCscPara;
            stHalGfxCscPara.eInputCS  = PQ_HAL_GFX_CS_BT709_RGB_FULL;
            stHalGfxCscPara.eOutputCS = PQ_HAL_GFX_CS_BT2020_YUV_LIMITED;
            PQ_HAL_SetGfxBt2020Mode(&stHalGfxCscPara);
#endif
#endif
        }
        else
        {
            stCscCtrl.bCSCEn    = HI_TRUE;
            stCscCtrl.u32CscPre = 15; /* ToDo: 98CV200的精度与98MV200不一致, 现在将98MV200的精度转换移到了hal层，合回主干时看有无必要 */
            stCscCtrl.enCscType = HI_PQ_CSC_TUNING_GP0; /* may need tuning */
            if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                && (PQ_GFX_CS_BT709_YUV_FULL == pstGfxCscMode->eOutputCS))
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT709_YUV_FULL;
            }
            else if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                     && (PQ_GFX_CS_BT709_YUV_LIMITED == pstGfxCscMode->eOutputCS))
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT709_YUV_LIMITED;
            }
            else if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                     && (PQ_GFX_CS_BT601_YUV_LIMITED == pstGfxCscMode->eOutputCS))
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT601_YUV_LIMITED;
            }
            else if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                     && (PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eOutputCS))
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT709_RGB_FULL;
            }
            else if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                     && (PQ_GFX_CS_BT601_RGB_FULL == pstGfxCscMode->eOutputCS))
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT601_RGB_FULL;
            }
            else
            {
                stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
                stGfxCscPara.eOutputCS = PQ_GFX_CS_BT709_YUV_LIMITED;
            }
            PQ_MNG_GetGfxCscCoef(&stCscCtrl, &stGfxCscPara, &stCscCoef, &stCscDcCoef);
#if defined(CHIP_TYPE_hi3798cv200)
            PQ_HAL_SetGfxCvmCscCoef(&stCscCoef, &stCscDcCoef);
            PQ_HAL_SetGdmCtrl(HI_FALSE); /* Close Gdm of 2020 module */
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
            PQ_HAL_SetGfxGdmCsc(HAL_GFX_GP0, stCscCtrl.bCSCEn, &stCscCoef, &stCscDcCoef);
#endif
        }
    }
    else
    {
        /* The Data of GP1 come from GP0(mixture of G0 and G1, and before access of GDM) or come from framebuffer;
           So no matter WBC_GP or GP1; the data must be RGB709_F; */
        stCscCtrl.bCSCEn    = HI_TRUE;
        stCscCtrl.u32CscPre = 10;
        stCscCtrl.enCscType = HI_PQ_CSC_TUNING_GP1;

        if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
            && (PQ_GFX_CS_BT601_YUV_LIMITED == pstGfxCscMode->eOutputCS))
        {
            stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
            stGfxCscPara.eOutputCS = PQ_GFX_CS_BT601_YUV_LIMITED;
        }
        else if ((PQ_GFX_CS_BT709_RGB_FULL == pstGfxCscMode->eInputCS)
                 && (PQ_GFX_CS_BT601_YUV_FULL == pstGfxCscMode->eOutputCS))
        {
            stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
            stGfxCscPara.eOutputCS = PQ_GFX_CS_BT601_YUV_FULL;
        }
        else
        {
            stGfxCscPara.eInputCS  = PQ_GFX_CS_BT709_RGB_FULL;
            stGfxCscPara.eOutputCS = PQ_GFX_CS_BT601_YUV_LIMITED;
        }
        PQ_MNG_GetGfxCscCoef(&stCscCtrl, &stGfxCscPara, &stCscCoef, &stCscDcCoef);
        PQ_HAL_SetGfxCsc(enGfxCscLayer, stCscCtrl.bCSCEn, &stCscCoef, &stCscDcCoef);
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetGfxCsc(HI_PQ_GFX_LAYER_E enGfxCscLayer, HI_PQ_GFX_CSC_PARA_S *pstGfxCscPara)
{
    HI_PQ_GFX_CSC_MODE_S stGfxCscMode;

    bIsBGRIn = pstGfxCscPara->bIsBGRIn;

    stGfxCscMode.eInputCS  = pstGfxCscPara->sGfxCscMode.eInputCS;
    stGfxCscMode.eOutputCS = pstGfxCscPara->sGfxCscMode.eOutputCS;

    PQ_MNG_SetGfxCscCoef(enGfxCscLayer, &stGfxCscMode);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetGfxCSCPictureSetting(HI_PQ_DISPLAY_E enDisp, HI_PQ_PICTURE_SETTING_S *pstPicSetting)
{
    if ((pstPicSetting == HI_NULL))
    {
        /* can not build HI_ERR_PQ at Boot */
        //HI_ERR_PQ("pstPicSetting is Null!\n");
        return HI_FAILURE;
    }

    if (HI_PQ_DISPLAY_0 == enDisp) /* SD */
    {
        sg_stSDPictureSetting.u16Brightness = pstPicSetting->u16Brightness;
        sg_stSDPictureSetting.u16Contrast   = pstPicSetting->u16Contrast;
        sg_stSDPictureSetting.u16Saturation = pstPicSetting->u16Saturation;
        sg_stSDPictureSetting.u16Hue        = pstPicSetting->u16Hue;

        /*
        pqprint(PQ_PRN_CSC, "set MNG SD Brightness=%d,Contrast=%d,Saturation=%d,Hue=%d\n",
                sg_stSDPictureSetting.u16Brightness, sg_stSDPictureSetting.u16Contrast,
                sg_stSDPictureSetting.u16Saturation, sg_stSDPictureSetting.u16Hue);
                */
    }
    else /* HD */
    {
        sg_stHDPictureSetting.u16Brightness = pstPicSetting->u16Brightness;
        sg_stHDPictureSetting.u16Contrast   = pstPicSetting->u16Contrast;
        sg_stHDPictureSetting.u16Saturation = pstPicSetting->u16Saturation;
        sg_stHDPictureSetting.u16Hue        = pstPicSetting->u16Hue;

        /*
        pqprint(PQ_PRN_CSC, "set MNG HD Brightness=%d,Contrast=%d,Saturation=%d,Hue=%d\n",
                sg_stHDPictureSetting.u16Brightness, sg_stHDPictureSetting.u16Contrast,
                sg_stHDPictureSetting.u16Saturation, sg_stHDPictureSetting.u16Hue);
                */
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetGfxCSCPictureSetting(HI_PQ_DISPLAY_E enDisp, HI_PQ_PICTURE_SETTING_S *pstPicSetting)
{
    if (HI_PQ_DISPLAY_0 == enDisp) /* SD */
    {
        pstPicSetting->u16Brightness = sg_stSDPictureSetting.u16Brightness;
        pstPicSetting->u16Contrast   = sg_stSDPictureSetting.u16Contrast;
        pstPicSetting->u16Hue        = sg_stSDPictureSetting.u16Hue;
        pstPicSetting->u16Saturation = sg_stSDPictureSetting.u16Saturation;
    }
    else /* HD */
    {
        pstPicSetting->u16Brightness = sg_stHDPictureSetting.u16Brightness;
        pstPicSetting->u16Contrast   = sg_stHDPictureSetting.u16Contrast;
        pstPicSetting->u16Hue        = sg_stHDPictureSetting.u16Hue;
        pstPicSetting->u16Saturation = sg_stHDPictureSetting.u16Saturation;
    }

    return HI_SUCCESS;
}

#ifdef HI_BUILD_IN_BOOT
HI_S32 DRV_PQ_SetGfxCscCoef(HI_PQ_GFX_LAYER_E enGfxCscLayer, HI_PQ_GFX_CSC_PARA_S *pstGfxCscPara)
{
    HI_S32 s32Ret = HI_FAILURE;

    /* Read para from base param; Set to param */
    s32Ret = PQ_MNG_SetGfxCsc(enGfxCscLayer, pstGfxCscPara);

    return s32Ret;
}
#else

/* Init BT2020 */
HI_S32 PQ_MNG_InitGfxCSC(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;

    s32Ret = PQ_HAL_GfxBT2020MemInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("BT2020 HAL Init error!\n");
        return HI_FAILURE;
    }
    PQ_HAL_GfxGamLutInit();

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitGfxCSC(HI_VOID)
{
    PQ_HAL_GfxBT2020DeInit();

    return HI_SUCCESS;
}

static stPQAlgFuncs stGfxCSCFuncs =
{
    .Init                     = PQ_MNG_InitGfxCSC,
    .DeInit                   = PQ_MNG_DeInitGfxCSC,
    .SetGfxCscCoef            = PQ_MNG_SetGfxCsc,
    .SetGfxCSCPictureSetting  = PQ_MNG_SetGfxCSCPictureSetting,
    .GetGfxCSCPictureSetting  = PQ_MNG_GetGfxCSCPictureSetting,
};

HI_S32 PQ_MNG_RegisterGfxCSC()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_GFXCSC, REG_TYPE_BUTT, PQ_BIN_ADAPT_SINGLE, "gfx csc", HI_NULL, &stGfxCSCFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterGfxCSC()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_GFXCSC);

    return s32Ret;
}
#endif

