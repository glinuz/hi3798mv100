#include "hi_type.h"
#include "hi_debug.h"
#include "hi_drv_mem.h"
#include "hi_drv_file.h"
#include "hi_drv_pq.h"

#include "pq_hal_comm.h"
#include "pq_hal_lcacm.h"

static HI_BOOL sg_bColorEn      = HI_TRUE;
static HI_BOOL sg_bColorDemoEn  = HI_FALSE;
static PQ_DEMO_MODE_E sg_enMode = PQ_DEMO_ENABLE_R;


/* Init Color */
static HI_S32 PQ_MNG_LowCostACMParaInit(HI_VOID)
{
    PQ_HAL_ACM_PARA_S stAcmPara;

    stAcmPara.stAcmCfg.acm0_en      = 1;
    stAcmPara.stAcmCfg.acm1_en      = 1;
    stAcmPara.stAcmCfg.acm2_en      = 1;
    stAcmPara.stAcmCfg.acm3_en      = 1;
    stAcmPara.stAcmCfg.bw_pro_mode  = 1;
    stAcmPara.stAcmCfg.acm_dbg_mode = 0;
    stAcmPara.stAcmCfg.acm_test_en  = 0;

    stAcmPara.stAcmCfg.acm0_sec_blk = 6; //sec对应V
    stAcmPara.stAcmCfg.acm0_fir_blk = 8; //fir对应U
    stAcmPara.stAcmCfg.acm1_sec_blk = 7;
    stAcmPara.stAcmCfg.acm1_fir_blk = 6;
    stAcmPara.stAcmCfg.acm2_sec_blk = 7;
    stAcmPara.stAcmCfg.acm2_fir_blk = 8;
    stAcmPara.stAcmCfg.acm3_sec_blk = 8;
    stAcmPara.stAcmCfg.acm3_fir_blk = 8;

    stAcmPara.acm0_d_u_off = 1;
    stAcmPara.acm0_c_u_off = 4;
    stAcmPara.acm0_b_u_off = 1;
    stAcmPara.acm0_a_u_off = 1;
    stAcmPara.acm0_d_v_off = 0;
    stAcmPara.acm0_c_v_off = -3;
    stAcmPara.acm0_b_v_off = -1;
    stAcmPara.acm0_a_v_off = -1;

    stAcmPara.acm1_d_u_off = -1;
    stAcmPara.acm1_c_u_off = -2;
    stAcmPara.acm1_b_u_off = -1;
    stAcmPara.acm1_a_u_off = 0;
    stAcmPara.acm1_d_v_off = -1;
    stAcmPara.acm1_c_v_off = -2;
    stAcmPara.acm1_b_v_off = -1;
    stAcmPara.acm1_a_v_off = 0;

    stAcmPara.acm2_d_u_off = 4;
    stAcmPara.acm2_c_u_off = 1;
    stAcmPara.acm2_b_u_off = 0;
    stAcmPara.acm2_a_u_off = 1;
    stAcmPara.acm2_d_v_off = -3;
    stAcmPara.acm2_c_v_off = -1;
    stAcmPara.acm2_b_v_off = 0;
    stAcmPara.acm2_a_v_off = -1;

    stAcmPara.acm3_d_u_off = 15;
    stAcmPara.acm3_c_u_off = 15;
    stAcmPara.acm3_b_u_off = 15;
    stAcmPara.acm3_a_u_off = 15;
    stAcmPara.acm3_d_v_off = 15;
    stAcmPara.acm3_c_v_off = 15;
    stAcmPara.acm3_b_v_off = 15;
    stAcmPara.acm3_a_v_off = 15;

    PQ_HAL_SetLowCostAcmPara(VDP_LAYER_VP0, &stAcmPara);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_InitLowCostACM(PQ_PARAM_S *pstPqParam, HI_BOOL bDefault)
{
    HI_S32 s32Ret;

    s32Ret = PQ_MNG_LowCostACMParaInit();
    if (HI_FAILURE == s32Ret)
    {
        HI_ERR_PQ("LowCost ACM Para Init failure\n");
        return HI_FAILURE;
    }

    s32Ret = PQ_TABLE_InitPhyList(PQ_VDP_LAYER_VID0, HI_PQ_MODULE_COLOR, SOURCE_MODE_NO, SOURCE_MODE_NO);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Low Cost ACM InitPhyList error\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitLowCostACM(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableLowCostACMDemo(HI_BOOL bOnOff)
{
    PQ_HAL_EnableACMDemo(VDP_LAYER_VP0, bOnOff);
    sg_bColorDemoEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetLowCostACMDemo(HI_BOOL *pbOnOff)
{
    PQ_CHECK_NULL_PTR(pbOnOff);
    *pbOnOff = sg_bColorDemoEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_EnableLowCostACM(HI_BOOL bOnOff)
{
    PQ_SOURCE_MODE_E enSourceMode = PQ_COMM_GetSourceMode();

    sg_bColorEn = bOnOff;

    if ((enSourceMode < SOURCE_MODE_ALL)
        && (HI_FALSE == PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_COLOR, enSourceMode)))
    {
        sg_bColorEn = HI_FALSE;
    }

    PQ_HAL_EnableACM(VDP_LAYER_VP0, sg_bColorEn);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetLowCostACMEnable(HI_U32 *pu32OnOff)
{
    PQ_CHECK_NULL_PTR(pu32OnOff);
    *pu32OnOff = sg_bColorEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetLowCostACMDemoMode(PQ_DEMO_MODE_E enMode)
{
    PQ_HAL_LAYER_VP_E   u32ChId   = VDP_LAYER_VP0;
    HAL_ACM_DEMO_MODE_E enAcmMode = HAL_ACM_DEMO_ENABLE_L;

    sg_enMode = enMode;

    if (enMode == PQ_DEMO_ENABLE_L)
    {
        enAcmMode = HAL_ACM_DEMO_ENABLE_L;
    }
    else if (enMode == PQ_DEMO_ENABLE_R)
    {
        enAcmMode = HAL_ACM_DEMO_ENABLE_R;
    }

    PQ_HAL_SetACMDemoMode(u32ChId, enAcmMode);

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetLowCostACMDemoMode(PQ_DEMO_MODE_E *penMode)
{
    PQ_CHECK_NULL_PTR(penMode);
    *penMode = sg_enMode;

    return HI_SUCCESS;
}


static stPQAlgFuncs stLowCostACMFuncs =
{
    .Init                 = PQ_MNG_InitLowCostACM,
    .DeInit               = PQ_MNG_DeInitLowCostACM,
    .SetEnable            = PQ_MNG_EnableLowCostACM,
    .GetEnable            = PQ_MNG_GetLowCostACMEnable,
    .SetDemo              = PQ_MNG_EnableLowCostACMDemo,
    .GetDemo              = PQ_MNG_GetLowCostACMDemo,
    .SetDemoMode          = PQ_MNG_SetLowCostACMDemoMode,
    .GetDemoMode          = PQ_MNG_GetLowCostACMDemoMode,

};

HI_S32 PQ_MNG_RegisterLowCostACM(PQ_REG_TYPE_E enType)
{
    return PQ_COMM_AlgRegister(HI_PQ_MODULE_COLOR, enType, PQ_BIN_ADAPT_SINGLE, "acm", HI_NULL, &stLowCostACMFuncs);
}

HI_S32 PQ_MNG_UnRegisterLowCostACM(HI_VOID)
{
    return PQ_COMM_AlgUnRegister(HI_PQ_MODULE_COLOR);
}

