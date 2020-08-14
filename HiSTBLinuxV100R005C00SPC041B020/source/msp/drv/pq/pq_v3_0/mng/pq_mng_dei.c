/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_dei.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/06/16
  Description   :

******************************************************************************/

#include "hi_math.h"

#include "pq_hal_comm.h"
#include "pq_mng_dei.h"
#include "drv_pq_table.h"


#define DEI_CLIP3(low,high,x)  (MAX2(MIN2((high),(x)),(low)))

/* DEI Init Flag */
static HI_BOOL sg_bDeiInitFlag    = HI_FALSE;
static HI_U32  sg_u32gain         = 64;
static DEI_DEMO_MODE_E sg_enMode  = DEI_DEMO_ENABLE_R;
//static HI_U32 sg_u32GlobalMotion = 255; /* For Tnr */

typedef struct
{
    HI_U32 u32GlobalMotion; /* TNR Motion Value */
    HI_U32 u32AdjustGain;   /* Global Motion Adjust Value */
} MOTION_RESULT_S;


/* Set Interlace Global Motion Enable */
static HI_S32 PQ_MNG_SetInterlaceMotionEn(HI_U32 u32HandleNo, HI_BOOL bIglben)
{
    HI_S32 s32Ret;

    s32Ret = PQ_HAL_SetInterlaceMotionEn(u32HandleNo, bIglben);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_HAL_SetInterlaceMotionEn error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID PQ_MNG_GlobalMotionInit(MotionHist_S* pstMotionHist )
{
    /* whatever 8-bit or 10-bit processing bitdepth, motion statistic is based on 10-bit motion value in the logic */
    pstMotionHist->SMALL_MOTION_THD        = 16; //16  //32
    pstMotionHist->LARGE_MOTION_THD        = 16; //256 //256

    /* adjust motion based global motion, whatever 8-bit or 10-bit processing bitdepth, global motion is transformed to 8-bit in the software */
    pstMotionHist->GLOBAL_MOTION_EN        = HI_TRUE;

    pstMotionHist->GLOBAL_MOTION_THD[0]    = 16;  //32  //16
    pstMotionHist->GLOBAL_MOTION_THD[1]    = 192; //64  //48
    pstMotionHist->GLOBAL_MOTION_THD[2]    = 255; //96  //112
    pstMotionHist->GLOBAL_MOTION_THD[3]    = 255; //196 //255
    pstMotionHist->GLOBAL_MOTION_THD[4]    = 255; //255 //255
    pstMotionHist->GLOBAL_MOTION_THD[5]    = 255; //255 //255

    pstMotionHist->GLOBAL_MOTION_SLOPE[0]  = 3; //8 //8
    pstMotionHist->GLOBAL_MOTION_SLOPE[1]  = 0; //0 //4
    pstMotionHist->GLOBAL_MOTION_SLOPE[2]  = 0; //5 //0
    pstMotionHist->GLOBAL_MOTION_SLOPE[3]  = 0; //0 //0

    pstMotionHist->MAX_GLOBAL_MOTION_RATIO = 128;
    pstMotionHist->MIN_GLOBAL_MOTION_RATIO = 32;

    pstMotionHist->GLOBAL_MOTION_RATIO[0]  = 32;
    pstMotionHist->GLOBAL_MOTION_RATIO[1]  = DEI_CLIP3(0, 255, pstMotionHist->GLOBAL_MOTION_RATIO[0]
                                             + ((pstMotionHist->GLOBAL_MOTION_SLOPE[0] * (pstMotionHist->GLOBAL_MOTION_THD[1] - pstMotionHist->GLOBAL_MOTION_THD[0]) + 4) >> 3));
    pstMotionHist->GLOBAL_MOTION_RATIO[2]  = DEI_CLIP3(0, 255, pstMotionHist->GLOBAL_MOTION_RATIO[1]
                                             + ((pstMotionHist->GLOBAL_MOTION_SLOPE[1] * (pstMotionHist->GLOBAL_MOTION_THD[2] - pstMotionHist->GLOBAL_MOTION_THD[1]) + 4) >> 3));
    pstMotionHist->GLOBAL_MOTION_RATIO[3]  = DEI_CLIP3(0, 255, pstMotionHist->GLOBAL_MOTION_RATIO[2]
                                             + ((pstMotionHist->GLOBAL_MOTION_SLOPE[2] * (pstMotionHist->GLOBAL_MOTION_THD[3] - pstMotionHist->GLOBAL_MOTION_THD[2]) + 4) >> 3));
    pstMotionHist->GLOBAL_MOTION_RATIO[4]  = DEI_CLIP3(0, 255, pstMotionHist->GLOBAL_MOTION_RATIO[3]
                                             + ((pstMotionHist->GLOBAL_MOTION_SLOPE[3] * (pstMotionHist->GLOBAL_MOTION_THD[4] - pstMotionHist->GLOBAL_MOTION_THD[3]) + 4) >> 3));
}

/*************************************************************************************
Prototype    : PQ_MNG_LUTMapping
Description  : mapping for lookup table
Input        : const HI_U16 *T
               const HI_U16 *R
               const HI_U16 *K
               const HI_U16 DataMax
               const HI_U16 DataMin
               HI_U16 DataIn
               HI_U8 u8BitDepth
Output:
**************************************************************************************/
static HI_S32 PQ_MNG_LUTMapping(HI_U16* T, HI_U16* R, HI_U16* K, HI_U16 DataMax, HI_U16 DataMin, HI_U16 DataIn, HI_U8 u8BitDepth)
{
    HI_S32 DataOut;
    HI_S32 HalfValue, Shift;
    if (8 == u8BitDepth)
    {
        HalfValue = 4;
        Shift     = 3;
    }
    else /* 10bit */
    {
        HalfValue = 16;
        Shift     = 5;
    }

    if (DataIn <= T[0])
    {
        DataOut = R[0];
    }
    else if (DataIn >= T[0] && DataIn <= T[1])
    {
        DataOut = R[0] + ((K[0] * (DataIn - T[0]) + HalfValue) >> Shift);
    }
    else if (DataIn >= T[1] && DataIn <= T[2])
    {
        DataOut = R[1] + ((K[1] * (DataIn - T[1]) + HalfValue) >> Shift);
    }
    else if (DataIn >= T[2] && DataIn <= T[3])
    {
        DataOut = R[2] + ((K[2] * (DataIn - T[2]) + HalfValue) >> Shift);
    }
    else if (DataIn >= T[3] && DataIn <= T[4])
    {
        DataOut = R[3] + ((K[3] * (DataIn - T[3]) + HalfValue) >> Shift);
    }
    else if (DataIn >= T[4] && DataIn <= T[5])
    {
        DataOut = R[4];
    }
    else
    {
        DataOut = R[4];
    }
    DataOut = DEI_CLIP3(DataMin, DataMax, DataOut);

    return DataOut;
}

/*****************************************************************************
 Prototype    : PQ_MNG_DEI_GlobalMotionGain
 Description  : calculate the motion gain based on the global motion
 Input        : HI_S32 s32WidthY
                HI_S32 s32HeightY
                HI_U8 u8BitDepth
                MotionHist_S *pstMotionHist
 Output       : None
 Return Value :
 Calls        :
 Called By    :

 History      :
 Date         : 2013/10/30
 Author       : t59295
 Modification : Created function

*****************************************************************************/
static HI_U32 PQ_MNG_DEIGlobalMotionGain(HI_S32 s32WidthY, HI_S32 s32HeightY, HI_U8 u8BitDepth, MotionHist_S* pstMotionHist)
{
    HI_U32 gain;
    static HI_U32 global_motion_last = 255;

    if (HI_FALSE == pstMotionHist->GLOBAL_MOTION_EN)
    {
        return gain = 64;
    }

    gain = PQ_MNG_LUTMapping(pstMotionHist->GLOBAL_MOTION_THD, pstMotionHist->GLOBAL_MOTION_RATIO, pstMotionHist->GLOBAL_MOTION_SLOPE,
                             pstMotionHist->MAX_GLOBAL_MOTION_RATIO, pstMotionHist->MIN_GLOBAL_MOTION_RATIO, global_motion_last, u8BitDepth);

    sg_u32gain = gain;

    return gain;
}

/* MOTION_INPUT_S has struct--S_VPSSWB_REGS_TYPE, In 98MV100 and 98M_A, it is null,
   in 98C it is defined for Glboal Motion¡¢IFMD¡¢PFMD and TNR¡¢SNR
   in HiFoneB2, It is defined for Glboal Motion etc.
   */
HI_S32 PQ_MNG_GetGlobalMotion(MOTION_INPUT_S* pstMotionInput)
{
    HI_S32 s32WidthY, s32HeightY, u8BitDepth;
    MotionHist_S    stMotionHist = {0};
    MOTION_RESULT_S stMotionResult;
    s32WidthY  = pstMotionInput->u32Width;
    s32HeightY = pstMotionInput->u32Height;
    u8BitDepth = 8;

    PQ_MNG_GlobalMotionInit(&(stMotionHist));

    if (HI_FALSE == PQ_HAL_GetInterlaceMotionEn(pstMotionInput->u32HandleNo))
    {
        stMotionResult.u32AdjustGain = 64;
        //stMotionResult.u32GlobalMotion = 255; /* TNR Motion Value */

        PQ_HAL_SetDeiAdjustGain(pstMotionInput->u32HandleNo, stMotionResult.u32AdjustGain);
        /* Open The on-off of Glboal Motion Enable */
        PQ_MNG_SetInterlaceMotionEn(pstMotionInput->u32HandleNo, HI_TRUE);

        return HI_SUCCESS;
    }

    PQ_HAL_GetMotionHistReg(pstMotionInput, &(stMotionHist));
    PQ_MNG_DEIGlobalMotionGain(s32WidthY, s32HeightY, u8BitDepth, &(stMotionHist));
    stMotionResult.u32AdjustGain = sg_u32gain;

    /* in 98M pstMotionResult->u32GlobalMotion is useless, There is not TNR */
    PQ_HAL_SetDeiAdjustGain(pstMotionInput->u32HandleNo, stMotionResult.u32AdjustGain);
    
    pqprint(PQ_PRN_DEI, "[%s]: GloGain:%d\n",__FUNCTION__,stMotionResult.u32AdjustGain);
    
    return HI_SUCCESS;
}


/* Init DEI */
HI_S32 PQ_MNG_InitDei(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;

    if (HI_TRUE == sg_bDeiInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_HAL_SetInterlaceMotionEn(u32HandleNo, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_HAL_SetInterlaceMotionEn error\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_MNG_InitPhyList(u32HandleNo, REG_TYPE_VPSS, SOURCE_MODE_NO, OUTPUT_MODE_NO, PQ_BIN_MODULE_DEI);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("dei InitPhyList error\n");
        return HI_FAILURE;
    }

    sg_bDeiInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

/* DeInit DEI */
HI_S32 PQ_MNG_DeInitDei(HI_VOID)
{
    if (HI_FALSE == sg_bDeiInitFlag)
    {
        return HI_SUCCESS;
    }

    sg_bDeiInitFlag = HI_FALSE;
    return HI_SUCCESS;
}

/* enable or disable dei demo *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_EnableDeiDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_EnableDeiDemo(u32HandleNo, bOnOff);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* set dei demo mode *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_SetDeiDemoMode(DEI_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetDeiDemoMode(u32HandleNo, enMode);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    sg_enMode = enMode;
    
    return HI_SUCCESS;
}

/* get dei demo mode *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_GetDeiDemoMode(DEI_DEMO_MODE_E* penMode)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;

    PQ_CHECK_NULL_PTR(penMode);
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = HI_SUCCESS;
            break;
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("error in get dei demo mode!\n");
        return HI_FAILURE;
    }

    *penMode = sg_enMode;
    
    return HI_SUCCESS;
}

/* set vpss market coordinate *//* Take effect inMv410; But no practical meaning In 98M and HiFone */
HI_S32 PQ_MNG_SetVpssMktCoordinate(HI_U32 u32DbgPos)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
    {
        if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
        {
            s32Ret = PQ_HAL_SetVpssMktCoordinate(u32HandleNo, u32DbgPos);
        }
    }

    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetGlobMotiGain(HI_U32* pu32Gain)
{
    PQ_CHECK_NULL_PTR(pu32Gain);
	
    *pu32Gain = sg_u32gain;

    return HI_SUCCESS;
}


