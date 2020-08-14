/******************************************************************************
*
* Copyright (C) 2015-2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_mng_tnr.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/1/9
  Description   :

******************************************************************************/
#include "pq_hal_comm.h"
#include "pq_mng_tnr.h"
#include "drv_pq_table.h"
#include "hi_math.h"
#include "pq_hal_algchange.h"

PQ_MMZ_BUF_S g_stMBufRef;
PQ_MMZ_BUF_S g_stMBufMad;
HI_BOOL g_bVdpTnrInitFlag = HI_FALSE;

#ifdef HI_PQ_4K_TNR_SUPPORT
HI_U32  VdpBufSize;
HI_U32  VdpMadBufSize;
HI_U32  VdpBufStride;
HI_U32  VdpMadBufStride;
HI_BOOL bV0TnrMmuEn = HI_FALSE;
#endif

static HI_BOOL g_bTNRInitFlag = HI_FALSE;
static HI_BOOL sg_bTnrEn      = HI_FALSE;
static HI_BOOL sg_bTnrDemoEn  = HI_FALSE;

static HI_S32 sg_u32TnrStr;
static PQ_DEMO_MODE_E sg_enTnrDemoMode = PQ_DEMO_ENABLE_R;

static HI_U8 TNR_CbCr[16][32] =
{
    /*0   1   2     3     4     5     6    7   8   9   10 11  12 13  14  15 16    17  18  19   20   21    22   23  24  25 26 27  28  29  30  31*/
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 0
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 1
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 2
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 3
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 4
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 5
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 6
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 7
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 8
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 10, 10, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 9
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 10
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 11
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 12
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 13
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}, // 14
    {0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0}  // 15
};


HI_S32 PQ_MNG_InitTnr(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;
    HI_U32 u32HandleNo = 0;
    sg_u32TnrStr = 50;
    sg_bTnrEn = HI_TRUE;
    sg_bTnrDemoEn = HI_FALSE;

    if (HI_TRUE == g_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_HAL_SetTNRCbCrLut(u32HandleNo, TNR_CbCr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_HAL_SetTNRCbCrLut error!\n");
        return HI_FAILURE;
    }

    PQ_TABLE_InitPhyList(u32HandleNo, HI_PQ_MODULE_TNR, SOURCE_MODE_NO, OUTPUT_MODE_NO);

    PQ_HAL_SetTnrMidStr(u32HandleNo);

#ifdef HI_PQ_4K_TNR_SUPPORT
    VdpBufStride = ((((3840 * 3) / 2) + 15) / 16) * 16;
    VdpBufSize   = VdpBufStride * 2160;
    VdpMadBufStride = ((3840 + 15) / 16 * 16);
    VdpMadBufSize = VdpMadBufStride * 2160;

#ifdef 	HI_PQ_SMMU_SUPPORT
    bV0TnrMmuEn = HI_TRUE;
#else
    bV0TnrMmuEn = HI_FALSE;
#endif

    memset(&g_stMBufRef, 0, sizeof(PQ_MMZ_BUF_S));
    s32Ret = PQ_HAL_AllocAndMap("PQ_V0TNR_REF", HI_NULL, VdpBufSize * 2 + 0x1000 , 0, &(g_stMBufRef));
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("Get V0TnrRef_BUF failed\n");
        return HI_FAILURE;
    }

    memset(&g_stMBufMad, 0, sizeof(PQ_MMZ_BUF_S));
    s32Ret = PQ_HAL_AllocAndMap("PQ_V0TNR_MAD", HI_NULL, VdpMadBufSize, 0, &(g_stMBufMad));
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("Get V0TnrMad_buf failed\n");
        return HI_FAILURE;
    }
#endif
    g_bTNRInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitTnr(HI_VOID)
{
    if (HI_FALSE == g_bTNRInitFlag)
    {
        return HI_SUCCESS;
    }

#ifdef HI_PQ_4K_TNR_SUPPORT
    /* release Vdp tnr memory */
    if (HI_NULL != g_stMBufRef.pu8StartVirAddr)
    {
        PQ_HAL_UnmapAndRelease(&g_stMBufRef);
        g_stMBufRef.pu8StartVirAddr = HI_NULL;
    }

    if (HI_NULL != g_stMBufMad.pu8StartVirAddr)
    {
        PQ_HAL_UnmapAndRelease(&g_stMBufMad);
        g_stMBufMad.pu8StartVirAddr = HI_NULL;
    }
#endif
    g_bTNRInitFlag = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableTnr(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_TNR, enSourceMode)))
    {
        bOnOff = HI_FALSE;
    }

    sg_bTnrEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableTNR(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableTNR(u32Vid, bOnOff);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrEnableFlag(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bTnrEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableTnrDemo(HI_BOOL bOnOff)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_bTnrDemoEn = bOnOff;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_EnableTNRDemo(u32HandleNo, bOnOff);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_EnableTNRDemo(u32Vid, bOnOff);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetTnrDemo(HI_BOOL* pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bTnrDemoEn;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrDemoMode(PQ_DEMO_MODE_E enMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    sg_enTnrDemoMode = enMode;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTNRDemoMode(u32HandleNo, (TNR_DEMO_MODE_E)enMode);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTNRDemoMode(u32Vid, (TNR_DEMO_MODE_E)enMode);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetTnrDemoMode(PQ_DEMO_MODE_E* penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enTnrDemoMode;
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrStrength(HI_U32 u32TnrStr)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (u32TnrStr > 100)
    {
        HI_ERR_PQ("the value[%d] of setting NR strength is out of range!\n", u32TnrStr);
        return HI_FAILURE;
    }

    sg_u32TnrStr = u32TnrStr;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrStr(u32HandleNo, u32TnrStr);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrStr(u32Vid, u32TnrStr);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrStrength(HI_U32* pu32TnrStr)
{
    PQ_CHECK_NULL_PTR(pu32TnrStr);
    *pu32TnrStr = sg_u32TnrStr;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTnrYMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrYMotionPixmeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrYMotionPixMeanRatio(u32HandleNo, pstTnrYMotionPixmeanRatio);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrYMotionPixMeanRatio(u32Vid, pstTnrYMotionPixmeanRatio);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrYMotionPixMeanRatio(TNR_MAPPING_S* pstTnrYMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrYMotionPixmeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrYMotionPixMeanRatio(u32HandleNo, pstTnrYMotionPixmeanRatio);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrYMotionPixMeanRatio(u32Vid, pstTnrYMotionPixmeanRatio);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTnrCMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrCMotionPixmeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrCMotionPixMeanRatio(u32HandleNo, pstTnrCMotionPixmeanRatio);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrCMotionPixMeanRatio(u32Vid, pstTnrCMotionPixmeanRatio);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrCMotionPixMeanRatio(TNR_MAPPING_S* pstTnrCMotionPixmeanRatio)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrCMotionPixmeanRatio);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrCMotionPixMeanRatio(u32HandleNo, pstTnrCMotionPixmeanRatio);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrCMotionPixMeanRatio(u32Vid, pstTnrCMotionPixmeanRatio);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrYMotionMapping(TNR_MAPPING_S* pstTnrYMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrYMotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrYMotionMapping(u32HandleNo, pstTnrYMotionMapping);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrYMotionMapping(u32Vid, pstTnrYMotionMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrYMotionMapping(TNR_MAPPING_S* pstTnrYMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrYMotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrYMotionMapping(u32HandleNo, pstTnrYMotionMapping);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrYMotionMapping(u32Vid, pstTnrYMotionMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrCMotionMapping(TNR_MAPPING_S* pstTnrCMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrCMotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrCMotionMapping(u32HandleNo, pstTnrCMotionMapping);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrCMotionMapping(u32Vid, pstTnrCMotionMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrCMotionMapping(TNR_MAPPING_S* pstTnrCMotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrCMotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrCMotionMapping(u32HandleNo, pstTnrCMotionMapping);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrCMotionMapping(u32Vid, pstTnrCMotionMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTnrFnlMotionYMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFnlMotionYMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrFnlMotionYMapping(u32HandleNo, pstTnrFnlMotionYMapping);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrFnlMotionYMapping(u32Vid, pstTnrFnlMotionYMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_GetTnrFnlMotionYMapping(TNR_MAPPING_S* pstTnrFnlMotionYMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFnlMotionYMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrFnlMotionYMapping(u32HandleNo, pstTnrFnlMotionYMapping);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrFnlMotionYMapping(u32Vid, pstTnrFnlMotionYMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTnrFnlMotionCMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFnlMotionCMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {

        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrFnlMotionCMapping(u32HandleNo, pstTnrFnlMotionCMapping);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrFnlMotionCMapping(u32Vid, pstTnrFnlMotionCMapping);
    }

    return s32Ret;

}

HI_S32 PQ_MNG_GetTnrFnlMotionCMapping(TNR_MAPPING_S* pstTnrFnlMotionCMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFnlMotionCMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrFnlMotionCMapping(u32HandleNo, pstTnrFnlMotionCMapping);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrFnlMotionCMapping(u32Vid, pstTnrFnlMotionCMapping);
    }

    return s32Ret;

}

HI_S32 PQ_MNG_SetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTnrFmotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFmotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrFmotionMapping(u32HandleNo, pstTnrFmotionMapping);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrFmotionMapping(u32Vid, pstTnrFmotionMapping);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_GetTnrFmotionMapping(TNR_FMOTION_MAPPING_S* pstTnrFmotionMapping)
{
    HI_U32 u32HandleNo;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    PQ_CHECK_NULL_PTR(pstTnrFmotionMapping);

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_GetTnrFmotionMapping(u32HandleNo, pstTnrFmotionMapping);
                break;
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_GetTnrFmotionMapping(u32Vid, pstTnrFmotionMapping);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTnrDemoModeCoor (HI_U32 u32XPos)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            if (HI_TRUE == PQ_HAL_CheckVpssValid(u32HandleNo))
            {
                s32Ret = PQ_HAL_SetTnrDemoModeCoor(u32HandleNo, u32XPos);
            }
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTnrDemoModeCoor(u32Vid, u32XPos);
    }

    return s32Ret;
}

HI_S32 PQ_MNG_SetTNRCbCrLut (HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32HandleNo;
    HI_U32 u32Vid = PQ_VDP_LAYER_VID0;

    if (REG_TYPE_VPSS == PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_TNR))
    {
        for (u32HandleNo = 0; u32HandleNo < VPSS_HANDLE_NUM; u32HandleNo++)
        {
            s32Ret = PQ_HAL_SetTNRCbCrLut(u32HandleNo, TNR_CbCr);
        }
    }
    else
    {
        s32Ret = PQ_HAL_SetTNRCbCrLut(u32Vid, TNR_CbCr);
    }

    return s32Ret;
}


HI_S32 PQ_MNG_InitVdpTnr(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

#ifdef  HI_PQ_4K_TNR_SUPPORT
    HI_BOOL bCmpEnable = HI_TRUE;
    HI_BOOL bMadMode   = HI_TRUE;

    if (HI_TRUE == g_bVdpTnrInitFlag)
    {
        return HI_SUCCESS;
    }

    PQ_HAL_SetTnrEnable(PQ_VDP_LAYER_VID0, HI_TRUE);

    /* wbc_ref */
    PQ_HAL_SetTnrWbcEnable(PQ_VDP_LAYER_VID0, HI_TRUE);
    PQ_HAL_SetTnrWbcCEnable(PQ_VDP_LAYER_VID0, HI_TRUE);

    if (HI_TRUE == bV0TnrMmuEn)
    {
        PQ_HAL_SetSmmuCBypass(PQ_LAYER_WBC_FI, HI_FALSE);
        PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_FI, HI_FALSE);
    }
    else
    {
        PQ_HAL_SetSmmuCBypass(PQ_LAYER_WBC_FI, HI_TRUE);
        PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_FI, HI_TRUE);
    }

    PQ_HAL_SetTnrWbcAddr(PQ_VDP_LAYER_VID0, g_stMBufRef.u32StartPhyAddr, g_stMBufRef.u32StartPhyAddr + VdpBufSize);

    PQ_HAL_SetTnrWbcStride(PQ_VDP_LAYER_VID0, VdpBufStride, VdpBufStride);

    PQ_HAL_SetTnrWbcCmpEnable(PQ_VDP_LAYER_VID0, bCmpEnable);
    PQ_HAL_SetTnrWbcDataWidth(PQ_VDP_LAYER_VID0, HI_TRUE);

    /* wbc_mad */
    PQ_HAL_SetMadWbcEnable(PQ_VDP_LAYER_VID0, HI_TRUE);
    PQ_HAL_SetMadWbcDataMode(PQ_VDP_LAYER_VID0, bMadMode); /*5 bit per 1 pix */


    if (HI_TRUE == bV0TnrMmuEn)
    {
        PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_ME, HI_FALSE);
    }
    else
    {
        PQ_HAL_SetSmmuLBypass(PQ_LAYER_WBC_ME, HI_TRUE);
    }
    PQ_HAL_SetMadWbcAddr(PQ_VDP_LAYER_VID0, g_stMBufMad.u32StartPhyAddr);

    PQ_HAL_SetMadWbcStride(PQ_VDP_LAYER_VID0, VdpMadBufStride);

    /* REF read */
    PQ_HAL_SetRefDcmpEnable(PQ_VDP_LAYER_VID0, bCmpEnable);
    PQ_HAL_SetTnrCEnable(PQ_VDP_LAYER_VID0, HI_TRUE);

    if (HI_TRUE == bV0TnrMmuEn)
    {
        PQ_HAL_Set16RegionSmmuLumBypass (PQ_VDP_LAYER_VID0, 3, HI_FALSE);
        PQ_HAL_Set16RegionSmmuChromBypass (PQ_VDP_LAYER_VID0, 3, HI_FALSE);
    }
    else
    {
        PQ_HAL_Set16RegionSmmuLumBypass (PQ_VDP_LAYER_VID0, 3, HI_TRUE);
        PQ_HAL_Set16RegionSmmuChromBypass (PQ_VDP_LAYER_VID0, 3, HI_TRUE);
    }

    PQ_HAL_SetTnrPreAddr(PQ_VDP_LAYER_VID0, g_stMBufRef.u32StartPhyAddr, g_stMBufRef.u32StartPhyAddr + VdpBufSize, VdpBufStride, VdpBufStride);

    PQ_HAL_SetTnrRefReqCtrl(PQ_VDP_LAYER_VID0, 0x2);
    PQ_HAL_SetTnrRefDataWidth(PQ_VDP_LAYER_VID0, HI_TRUE);

    /* MAD read */
    PQ_HAL_SetTnrMadDataMode(PQ_VDP_LAYER_VID0, bMadMode);

    if (HI_TRUE == bV0TnrMmuEn)
    {
        PQ_HAL_Set16RegionSmmuLumBypass (PQ_VDP_LAYER_VID0, 2, HI_FALSE);
    }
    else
    {
        PQ_HAL_Set16RegionSmmuLumBypass (PQ_VDP_LAYER_VID0, 2, HI_TRUE);
    }

    PQ_HAL_SetTnrMadAddr(PQ_VDP_LAYER_VID0, g_stMBufMad.u32StartPhyAddr, VdpMadBufStride);

    PQ_HAL_SetTnrMadDbmReqCtrl(PQ_VDP_LAYER_VID0, 0x1);

    /* regup */
    PQ_HAL_SetTnrWbcRegUp(PQ_VDP_LAYER_VID0);
    PQ_HAL_SetMadWbcRegUp(PQ_VDP_LAYER_VID0);

    PQ_HAL_InitVdpTnr();
#endif
    return s32Ret;
}

HI_S32 PQ_MNG_DeInitVdpTnr(HI_VOID)
{
    if (HI_FALSE == g_bVdpTnrInitFlag)
    {
        return HI_FAILURE;
    }

    g_bVdpTnrInitFlag = HI_FALSE;

    return HI_SUCCESS;

}

HI_S32 PQ_MNG_SetVdpTnrParaUpd(HI_U32 u32Data)
{
    return PQ_HAL_SetVdpTnrParaUpd(u32Data);
}

HI_S32 PQ_MNG_SetVdpTnrLutUpd(HI_U32 u32Data)
{
    return PQ_HAL_SetVdpTnrLutUpd(u32Data);
}

static stPQAlgFuncs stTNRFuncs =
{
    .Init               = PQ_MNG_InitTnr,
    .DeInit             = PQ_MNG_DeInitTnr,
    .SetEnable          = PQ_MNG_EnableTnr,
    .GetEnable          = PQ_MNG_GetTnrEnableFlag,
    .SetDemo            = PQ_MNG_EnableTnrDemo,
    .GetDemo            = PQ_MNG_GetTnrDemo,
    .SetDemoMode        = PQ_MNG_SetTnrDemoMode,
    .GetDemoMode        = PQ_MNG_GetTnrDemoMode,
    .SetStrength        = PQ_MNG_SetTnrStrength,
    .GetStrength        = PQ_MNG_GetTnrStrength,
    .SetDemoModeCoor    = PQ_MNG_SetTnrDemoModeCoor,
#ifdef  HI_PQ_4K_TNR_SUPPORT
    .InitVdpTnr         = PQ_MNG_InitVdpTnr,
    .DeInitVdpTnr       = PQ_MNG_DeInitVdpTnr,
    .SetVdpTnrParaUpd   = PQ_MNG_SetVdpTnrParaUpd,
    .SetVdpTnrLutUpd    = PQ_MNG_SetVdpTnrLutUpd,
#endif
};

HI_S32 PQ_MNG_RegisterTNR(PQ_REG_TYPE_E  enType)
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgRegister(HI_PQ_MODULE_TNR, enType, PQ_BIN_ADAPT_SINGLE, "tnr", HI_NULL, &stTNRFuncs);

    return s32Ret;
}

HI_S32 PQ_MNG_UnRegisterTNR()
{
    HI_S32 s32Ret;

    s32Ret = PQ_COMM_AlgUnRegister(HI_PQ_MODULE_TNR);

    return s32Ret;
}

