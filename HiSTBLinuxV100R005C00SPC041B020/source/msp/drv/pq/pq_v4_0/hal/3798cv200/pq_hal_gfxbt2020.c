#include <linux/string.h>
#include <linux/kernel.h>

#include "hi_type.h"
#include "pq_hal_comm.h"
#include "pq_hal_gfxbt2020.h"



#define GFX_GAM_BUFFER        sizeof(HI_U32) * 256 /* Mmz Buffer Size */
#define GFX_DEGAM_BUFFER      sizeof(HI_U32) * 256 /* Mmz Buffer Size */

static HI_BOOL           sg_bGfxCscInitFlag = HI_FALSE;
static PQ_MMZ_BUF_S      sg_stGfxGamBuf;
static PQ_MMZ_BUF_S      sg_stGfxDeGamBuf;
static S_VDP_REGS_TYPE  *pVdpReg;

typedef struct file FILE;

typedef struct
{
    HI_U32 data3;
    HI_U32 data2;
    HI_U32 data1;
    HI_U32 data0;
    HI_U32 depth;
} PQ_U128_S;

typedef enum tagVDP_COEF_LUT_MODE
{
    COEF_DATA_TYPE_U8 = 0,
    COEF_DATA_TYPE_S8    ,
    COEF_DATA_TYPE_U16   ,
    COEF_DATA_TYPE_S16   ,
    COEF_DATA_TYPE_U32   ,
    COEF_DATA_TYPE_S32   ,
    COEF_DATA_TYPE_BUTT

} PQ_COEF_DATA_TYPE_E;

typedef enum tagSTI_FILE_TYPE_E
{
    STI_FILE_DATA_VID0 = 0 ,
    STI_FILE_DATA_VID1     ,
    STI_FILE_DATA_VID2     ,
    STI_FILE_DATA_VID3     ,
    STI_FILE_DATA_VID4     ,
    STI_FILE_DATA_GFX0     ,
    STI_FILE_DATA_GFX1     ,
    STI_FILE_DATA_GFX2     ,
    STI_FILE_DATA_GFX3     ,
    STI_FILE_DATA_GFX4     ,

    STI_FILE_COEF_ZME_V0   ,
    STI_FILE_COEF_ACM      ,
    STI_FILE_COEF_DCI      ,
    STI_FILE_COEF_DIM      ,
    STI_FILE_COEF_GMM      ,
    STI_FILE_COEF_OD       ,
    STI_FILE_COEF_RGBW     ,
    STI_FILE_COEF_DEMURA   ,
    STI_FILE_TYPE_NODEF    ,
    STI_FILE_TILE_64_CMP_Y ,
    STI_FILE_TILE_64_CMP_C ,
    STI_FILE_TILE_64_CMP_Y_2BIT,
    STI_FILE_TILE_64_CMP_C_2BIT,
    STI_FILE_TILE_64_CMP_Y_HEAD,
    STI_FILE_TILE_64_CMP_C_HEAD,
    STI_FILE_OSD_CMP       ,
    STI_FILE_COEF_DM       ,
    STI_FILE_COEF_GDM      ,
    STI_FILE_COEF_METADATA ,
    STI_FILE_DATA_TNR_REF  ,
    STI_FILE_DATA_TNR_MAD  ,
    STI_FILE_DATA_VDH_DB   ,

    STI_FILE_DATA_VMX      ,

    STI_FILE_TYPE_BUTT
} PQ_STI_FILE_TYPE_E;

typedef struct
{
    HI_U8                  *coef_addr       ;
    PQ_STI_FILE_TYPE_E         sti_type        ;
    FILE                   *fp_coef         ;
    HI_U32                  lut_num         ;
    HI_U32                  burst_num       ;
    HI_U32                  cycle_num       ;
    void                  **p_coef_array    ;
    HI_U32                 *lut_length      ;
    HI_U32                 *coef_bit_length ;
    PQ_COEF_DATA_TYPE_E    data_type       ;
} PQ_COEF_SEND_CFG;



static HI_U32 gdm_degmm_lut[64] =
{
    8602 ,
    9591 ,
    9648 ,
    10439,
    11118,
    11709,
    12232,
    12708,
    13139,
    13535,
    13904,
    14254,
    14574,
    14881,
    15171,
    15443,
    15706,
    15954,
    16193,
    16423,
    16641,
    16858,
    17057,
    17256,
    17448,
    17629,
    17810,
    17984,
    18149,
    18314,
    18476,
    18627,
    18778,
    18928,
    19072,
    19210,
    19348,
    19485,
    19614,
    19740,
    21495,
    22867,
    23993,
    24950,
    25785,
    26528,
    27198,
    27807,
    28364,
    28878,
    29357,
    29805,
    30227,
    30624,
    30997,
    31356,
    31693,
    32016,
    32326,
    32620,
    32906,
    33177,
    33441,
    33694,
};

//王正提供rgb->bt2020表
static HI_U32 gdm_gmm_rgb2bt2020lut[64] =
{
    0 ,
    369 ,
    737 ,
    1106 ,
    1475 ,
    1843 ,
    2212 ,
    2580 ,
    2949 ,
    3318 ,
    3686 ,
    4055 ,
    4424 ,
    4792 ,
    5161 ,
    5544 ,
    5898 ,
    6241 ,
    7510 ,
    8653 ,
    9699 ,
    10668,
    11574,
    12428,
    13237,
    14008,
    14745,
    15451,
    16131,
    16787,
    17421,
    18035,
    18631,
    19210,
    19774,
    20323,
    22393,
    24297,
    26066,
    27725,
    29291,
    30778,
    32195,
    33551,
    34854,
    36108,
    37319,
    38490,
    39625,
    40727,
    41798,
    42841,
    43857,
    44849,
    45819,
    46766,
    47694,
    48603,
    52067,
    55297,
    58333,
    61204,
    63933,
    65535,
};


HI_VOID PQ_HAL_HDR_SetGDmGammaEn(HI_U32 g_gamma_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_CTRL.u32))));
    GDM_CTRL.bits.g_gamma_en = g_gamma_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_CTRL.u32))), GDM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetAuthEn(HI_U32 gauth_en)
{
    U_GDM_CTRL GDM_CTRL;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_CTRL.u32) )));
    GDM_CTRL.bits.g_gdm_auth_en = gauth_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_CTRL.u32))), GDM_CTRL.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGdmGamParaUpd(HI_U32 u32Data, HI_BOOL bOnOff)
{
    U_GDM_COEFF_UPD  GDM_COEFF_UPD;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_COEFF_UPD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_COEFF_UPD.u32))));
    GDM_COEFF_UPD.bits.gamm_coeff_upd = bOnOff;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_COEFF_UPD.u32))), GDM_COEFF_UPD.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGdmDeGamParaUpd(HI_U32 u32Data, HI_BOOL bOnOff)
{
    U_GDM_COEFF_UPD  GDM_COEFF_UPD;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_COEFF_UPD.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_COEFF_UPD.u32))));
    GDM_COEFF_UPD.bits.degamm_coeff_upd = bOnOff;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_COEFF_UPD.u32))), GDM_COEFF_UPD.u32);

    return;
}

HI_VOID PQ_HAL_GP_SetGdmDegammAddr(HI_U32 u32Data, HI_U32 u32GdmDegammAddr)
{
    U_GDM_DEGAMM_ADDR GDM_DEGAMM_ADDR;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMM_ADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMM_ADDR.u32))));
    GDM_DEGAMM_ADDR.bits.degamm_coeff_addr = u32GdmDegammAddr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMM_ADDR.u32))), GDM_DEGAMM_ADDR.u32);

    return;
}

HI_VOID PQ_HAL_GP_SetGdmGammAddr(HI_U32 u32Data, HI_U32 u32GdmGammAddr)
{
    U_GDM_GAMM_ADDR GDM_GAMM_ADDR;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMM_ADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMM_ADDR.u32))));
    GDM_GAMM_ADDR.bits.gamm_coeff_addr = u32GdmGammAddr;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMM_ADDR.u32))), GDM_GAMM_ADDR.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmDegmmStep(HI_U32 g_degmm_step[4])
{
    U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_STEP.u32))));
    GDM_DEGAMMA_STEP.bits.g_degmm_x1_step = g_degmm_step[0];
    GDM_DEGAMMA_STEP.bits.g_degmm_x2_step = g_degmm_step[1];
    GDM_DEGAMMA_STEP.bits.g_degmm_x3_step = g_degmm_step[2];
    GDM_DEGAMMA_STEP.bits.g_degmm_x4_step = g_degmm_step[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_STEP.u32))), GDM_DEGAMMA_STEP.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmGammaStep(HI_U32 g_gmm_step[4])
{
    U_GDM_GAMMA_STEP GDM_GAMMA_STEP;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_STEP.u32))));
    GDM_GAMMA_STEP.bits.g_gmm_x1_step = g_gmm_step[0];
    GDM_GAMMA_STEP.bits.g_gmm_x2_step = g_gmm_step[1];
    GDM_GAMMA_STEP.bits.g_gmm_x3_step = g_gmm_step[2];
    GDM_GAMMA_STEP.bits.g_gmm_x4_step = g_gmm_step[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_STEP.u32))), GDM_GAMMA_STEP.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmDegmmNum(HI_U32 g_degmm_num[4])
{
    U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_NUM.u32))));
    GDM_DEGAMMA_NUM.bits.g_degmm_x1_num  = g_degmm_num[0];
    GDM_DEGAMMA_NUM.bits.g_degmm_x2_num  = g_degmm_num[1];
    GDM_DEGAMMA_NUM.bits.g_degmm_x3_num  = g_degmm_num[2];
    GDM_DEGAMMA_NUM.bits.g_degmm_x4_num  = g_degmm_num[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_NUM.u32))), GDM_DEGAMMA_NUM.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmGmmNum(HI_U32 g_gmm_num[4])
{
    U_GDM_GAMMA_NUM GDM_GAMMA_NUM;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_NUM.u32))));
    GDM_GAMMA_NUM.bits.g_gmm_x1_num  = g_gmm_num[0];
    GDM_GAMMA_NUM.bits.g_gmm_x2_num  = g_gmm_num[1];
    GDM_GAMMA_NUM.bits.g_gmm_x3_num  = g_gmm_num[2];
    GDM_GAMMA_NUM.bits.g_gmm_x4_num  = g_gmm_num[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_NUM.u32))), GDM_GAMMA_NUM.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmDegmmPos(HI_U32 g_degmm_pos[4])
{
    U_GDM_DEGAMMA_POS GDM_DEGAMMA_POS;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_POS.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_POS.u32))));
    GDM_DEGAMMA_POS.bits.g_degmm_x1_pos = g_degmm_pos[0];
    GDM_DEGAMMA_POS.bits.g_degmm_x2_pos = g_degmm_pos[1];
    GDM_DEGAMMA_POS.bits.g_degmm_x3_pos = g_degmm_pos[2];
    GDM_DEGAMMA_POS.bits.g_degmm_x4_pos = g_degmm_pos[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_POS.u32))), GDM_DEGAMMA_POS.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmGammaPos(HI_U32 g_gmm_pos[4])
{
    U_GDM_GAMMA_POS0 GDM_GAMMA_POS0;
    U_GDM_GAMMA_POS1 GDM_GAMMA_POS1;
    U_GDM_GAMMA_POS2 GDM_GAMMA_POS2;
    U_GDM_GAMMA_POS3 GDM_GAMMA_POS3;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS0.u32))));
    GDM_GAMMA_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS1.u32))));
    GDM_GAMMA_POS2.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS2.u32))));
    GDM_GAMMA_POS3.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS3.u32))));
    GDM_GAMMA_POS0.bits.g_gmm_x1_pos = g_gmm_pos[0];
    GDM_GAMMA_POS2.bits.g_gmm_x2_pos = g_gmm_pos[1];
    GDM_GAMMA_POS1.bits.g_gmm_x3_pos = g_gmm_pos[2];
    GDM_GAMMA_POS3.bits.g_gmm_x4_pos = g_gmm_pos[3];
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS0.u32))), GDM_GAMMA_POS0.u32);
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS1.u32))), GDM_GAMMA_POS1.u32);
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS2.u32))), GDM_GAMMA_POS2.u32);
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_POS3.u32))), GDM_GAMMA_POS3.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmGammaRdEn(HI_U32 gamma_rd_en)
{
    U_GDM_GAMMA_REN GDM_GAMMA_REN;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_REN.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_REN.u32)) ));
    GDM_GAMMA_REN.bits.gamma_rd_en = gamma_rd_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_GAMMA_REN.u32))), GDM_GAMMA_REN.u32);

    return;
}

HI_VOID PQ_HAL_HDR_SetGDmDegammaRdEn(HI_U32 degamma_rd_en)
{
    U_GDM_DEGAMMA_REN GDM_DEGAMMA_REN;
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_REN.u32 = PQ_HAL_RegRead((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_REN.u32) )));
    GDM_DEGAMMA_REN.bits.degamma_rd_en = degamma_rd_en;
    PQ_HAL_RegWrite((HI_U32 *)((HI_ULONG)(&(pVdpReg->GDM_DEGAMMA_REN.u32))), GDM_DEGAMMA_REN.u32);

    return;
}

HI_S32 PQ_HAL_FUNC_Push128(PQ_U128_S *pstData128, HI_U32 coef_data, HI_U32 bit_len)
{
    coef_data = coef_data & (0xFFFFFFFF >> (32 - bit_len));

    if (pstData128->depth < 32)
    {
        if ((pstData128->depth + bit_len) <= 32)
        {
            pstData128->data0 = (coef_data << pstData128->depth) | pstData128->data0;
        }
        else
        {
            pstData128->data0   = (coef_data << pstData128->depth) | pstData128->data0;
            //pstData128->data1 = coef_data >> (pstData128->depth + bit_len - 32);
            pstData128->data1   = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 32 && pstData128->depth < 64)
    {
        if ((pstData128->depth + bit_len) <= 64)
        {
            pstData128->data1 = (coef_data << pstData128->depth % 32) | pstData128->data1;
        }
        else
        {
            pstData128->data1   = (coef_data << pstData128->depth % 32) | pstData128->data1;
            //pstData128->data2 = coef_data >> (pstData128->depth%32 + bit_len - 64);
            pstData128->data2   = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 64 && pstData128->depth < 96)
    {
        if ((pstData128->depth + bit_len) <= 96)
        {
            pstData128->data2 = (coef_data << pstData128->depth % 32) | pstData128->data2;
        }
        else
        {
            pstData128->data2   = (coef_data << pstData128->depth % 32) | pstData128->data2;
            //pstData128->data3 = coef_data >> (pstData128->depth%32 + bit_len - 96);
            pstData128->data3   = coef_data >> (32 - pstData128->depth % 32);
        }
    }
    else if (pstData128->depth >= 96)
    {
        if ((pstData128->depth + bit_len) <= 128)
        {
            pstData128->data3 = (coef_data << (pstData128->depth % 32)) | pstData128->data3;
        }
    }

    pstData128->depth = pstData128->depth + bit_len;

    if (pstData128->depth <= 128)
    {
        return HI_SUCCESS;
    }
    else
    {
        HI_ERR_PQ("Error! PQ_HAL_FUNC_Push128 depth out of range\n");
        return HI_FAILURE;
    }
}

HI_U32 PQ_HAL_FUNC_WriteDDR(PQ_STI_FILE_TYPE_E enStiType, FILE *fp_coef, HI_U8 *addr, PQ_U128_S *pstData128)
{
    HI_U32 ii = 0;
    HI_U32 u32DataArr[4] = {pstData128->data0, pstData128->data1, pstData128->data2, pstData128->data3};
    HI_U8 *pu8AddrTmp    = 0;

    for (ii = 0; ii < 4; ii++)
    {
        pu8AddrTmp = addr + ii * 4;
        memcpy((HI_VOID *)pu8AddrTmp, (HI_VOID *)&u32DataArr[ii], sizeof(HI_U32));
    }

    return HI_SUCCESS;
}

HI_U32 PQ_HAL_FUNC_FindMax(HI_U32 *u32Array, HI_U32 num)
{
    HI_U32 ii;
    HI_U32 u32TmpData = u32Array[0];

    for (ii = 1; ii < num; ii++)
    {
        if (u32TmpData < u32Array[ii])
        {
            u32TmpData = u32Array[ii];
        }
    }

    return u32TmpData;
}

HI_S32 PQ_HAL_FUNC_SendCoef(PQ_COEF_SEND_CFG *stCfg)
{
    HI_U32 ii, kk, nn, mm;
    HI_U8 *addr_base               = stCfg->coef_addr;
    HI_U32 addr_offset             = 0;
    HI_U8 *addr                    = addr_base;

    HI_U32 cycle_num;
    HI_U32 total_bit_len           = 0;
    HI_U32 coef_cnt                = 0;
    HI_S32 tmp_data                = 0;
    HI_U32 total_burst_num         = 0;
    HI_U32 max_len                 = 0;
    PQ_U128_S stData128            = {0};

    addr_base = stCfg->coef_addr;

    //----------------------------------------------------------------------
    //data type convertion
    //----------------------------------------------------------------------

    addr      = addr_base;
    cycle_num = stCfg->cycle_num;

    //bOpenStiFile(stCfg->sti_type);

    for (ii = 0; ii < stCfg->lut_num; ii++)
    {
        total_bit_len = total_bit_len + stCfg->coef_bit_length[ii];
    }

    //send data
    max_len = PQ_HAL_FUNC_FindMax( stCfg->lut_length , stCfg->lut_num);
    //cycle_num = (128 * stCfg->burst_num) / total_bit_len;

    if (1 != stCfg->burst_num )
    {
        return HI_SUCCESS;
    }

    total_burst_num = (max_len + cycle_num - 1) / cycle_num;

    if (stCfg->data_type == COEF_DATA_TYPE_S16)
    {
        for (kk = 0; kk < total_burst_num; kk++)
        {
            memset((void *)&stData128, 0, sizeof(stData128));
            for (mm = 0; mm < cycle_num; mm++)
            {
                coef_cnt = kk * cycle_num + mm;
                for (nn = 0; nn < stCfg->lut_num; nn++)
                {
                    if (coef_cnt < stCfg->lut_length[nn])
                    {
                        tmp_data = ((HI_S16 **)stCfg->p_coef_array)[nn][coef_cnt];
                    }
                    else
                    {
                        tmp_data = 0;
                    }
                    PQ_HAL_FUNC_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                }
            }
            addr        = addr_base + addr_offset;
            addr_offset = addr_offset + 16;
            PQ_HAL_FUNC_WriteDDR(stCfg->sti_type, stCfg->fp_coef, addr, &stData128);
        }
    }
    else if (stCfg->data_type == COEF_DATA_TYPE_U32)
    {
        for (kk = 0; kk < total_burst_num; kk++)
        {
            memset((void *)&stData128, 0, sizeof(stData128));
            for (mm = 0; mm < cycle_num; mm++)
            {
                coef_cnt = kk * cycle_num + mm;
                for (nn = 0; nn < stCfg->lut_num; nn++)
                {
                    if (coef_cnt < stCfg->lut_length[nn])
                    {
                        tmp_data = ((HI_U32 **)stCfg->p_coef_array)[nn][coef_cnt];
                    }
                    else
                    {
                        tmp_data = 0;
                    }
                    PQ_HAL_FUNC_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                }
            }
            addr        = addr_base + addr_offset;
            addr_offset = addr_offset + 16;
            PQ_HAL_FUNC_WriteDDR(stCfg->sti_type, stCfg->fp_coef, addr, &stData128);
        }
    }
    else
    {
        for (kk = 0; kk < total_burst_num; kk++)
        {
            memset((void *)&stData128, 0, sizeof(stData128));
            for (mm = 0; mm < cycle_num; mm++)
            {
                coef_cnt = kk * cycle_num + mm;
                for (nn = 0; nn < stCfg->lut_num; nn++)
                {
                    if (coef_cnt < stCfg->lut_length[nn])
                    {
                        tmp_data = ((HI_S32 **)stCfg->p_coef_array)[nn][coef_cnt];
                    }
                    else
                    {
                        tmp_data = 0;
                    }
                    PQ_HAL_FUNC_Push128(&stData128, tmp_data, stCfg->coef_bit_length[nn]);
                }
            }
            addr        = addr_base + addr_offset;
            addr_offset = addr_offset + 16;
            PQ_HAL_FUNC_WriteDDR(stCfg->sti_type, stCfg->fp_coef, addr, &stData128);
        }
    }

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGdmgammaCoef(HI_U32 *u32GdmGammaCoef)
{
    PQ_COEF_SEND_CFG   stCoefSend;
    HI_U8 *addr = 0;
    void  *p_coef_array[1]    = {u32GdmGammaCoef};
    HI_U32 lut_length[1]      = {GDM_GMM_LUT_SIZE};
    HI_U32 coef_bit_length[1] = {16};
    FILE  *fp_gdm_gamma_coef  = HI_NULL;

    addr = sg_stGfxGamBuf.pu8StartVirAddr;
    PQ_HAL_GP_SetGdmGammAddr(HAL_GFX_GP0, sg_stGfxGamBuf.u32StartPhyAddr);

    //send coef cfg
    stCoefSend.coef_addr         = addr                       ;
    stCoefSend.sti_type          = STI_FILE_COEF_GDM          ;
    stCoefSend.fp_coef           = fp_gdm_gamma_coef          ;
    stCoefSend.lut_num           = 1                          ;
    stCoefSend.burst_num         = 1                          ;
    stCoefSend.cycle_num         = 8                          ;
    stCoefSend.p_coef_array      = p_coef_array               ;
    stCoefSend.lut_length        = lut_length                 ;
    stCoefSend.coef_bit_length   = coef_bit_length            ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32         ;

    PQ_HAL_FUNC_SendCoef(&stCoefSend);

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetGdmDegammaCoef(HI_U32 *u32GdmDeGammaCoef)
{
    PQ_COEF_SEND_CFG   stCoefSend;
    HI_U8 *addr;
    void  *p_coef_array[1]     = {u32GdmDeGammaCoef};
    HI_U32 lut_length[1]       = {GDM_GMM_LUT_SIZE};
    HI_U32 coef_bit_length[1]  = {16};
    FILE  *fp_gdm_degamma_coef = HI_NULL;

    addr = sg_stGfxDeGamBuf.pu8StartVirAddr;
    PQ_HAL_GP_SetGdmDegammAddr(HAL_GFX_GP0, sg_stGfxDeGamBuf.u32StartPhyAddr);

    //send coef cfg
    stCoefSend.coef_addr         = addr                       ;
    stCoefSend.sti_type          = STI_FILE_COEF_GDM          ;
    stCoefSend.fp_coef           = fp_gdm_degamma_coef        ;
    stCoefSend.lut_num           = 1                          ;
    stCoefSend.burst_num         = 1                          ;
    stCoefSend.cycle_num         = 8                          ;
    stCoefSend.p_coef_array      = p_coef_array               ;
    stCoefSend.lut_length        = lut_length                 ;
    stCoefSend.coef_bit_length   = coef_bit_length            ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32         ;

    PQ_HAL_FUNC_SendCoef(&stCoefSend);

    return HI_SUCCESS;
}

/* 初始化GfxBT2020模块 */
HI_S32 PQ_HAL_GfxBT2020MemInit(HI_VOID)
{
    HI_S32 s32Ret;

    if (HI_TRUE == sg_bGfxCscInitFlag)
    {
        return HI_SUCCESS;
    }

    memset(&sg_stGfxGamBuf,   0, sizeof(PQ_MMZ_BUF_S));
    memset(&sg_stGfxDeGamBuf, 0, sizeof(PQ_MMZ_BUF_S));
    s32Ret  = PQ_HAL_MMZ_AllocAndMap("PQ_GFXGAM",   HI_NULL, GFX_GAM_BUFFER,   0, &sg_stGfxGamBuf);
    s32Ret |= PQ_HAL_MMZ_AllocAndMap("PQ_GFXDEGAM", HI_NULL, GFX_DEGAM_BUFFER, 0, &sg_stGfxDeGamBuf);

    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("GfxCsc memory allocated failed!\n");
        return HI_FAILURE;
    }

    memset(sg_stGfxGamBuf.pu8StartVirAddr,   0, GFX_GAM_BUFFER);
    memset(sg_stGfxDeGamBuf.pu8StartVirAddr, 0, GFX_DEGAM_BUFFER);

    sg_bGfxCscInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_VOID PQ_HAL_GfxGamLutInit(HI_VOID)
{
    PQ_HAL_SetGdmDegammaCoef(gdm_degmm_lut);
    PQ_HAL_SetGdmgammaCoef(gdm_gmm_rgb2bt2020lut);

    return;
}

/* 去初始化GfxBT2020模块 */
HI_S32 PQ_HAL_GfxBT2020DeInit(HI_VOID)
{
    if (HI_FALSE == sg_bGfxCscInitFlag)
    {
        return HI_SUCCESS;
    }

    PQ_HAL_MMZ_UnmapAndRelease(&sg_stGfxGamBuf);
    PQ_HAL_MMZ_UnmapAndRelease(&sg_stGfxDeGamBuf);

    sg_bGfxCscInitFlag = HI_FALSE;

    return HI_SUCCESS;
}


/* 图形只存在RGB_709 -->YUV_2020的转换; RGB_709 --> YUV_2020 = RGB_709-->RGB_2020-->YUV_2020 */
HI_VOID PQ_HAL_SetGfxBt2020Mode(PQ_HAL_GFX_CSC_MODE_S *pstPqCscMode)
{
    HI_U32 g_degamma_en = 1;//stCfg -> degamm_en;
    HI_U32 g_gamma_en   = 1;//stCfg -> gamm_en;
    HI_U32 gcvm_en      = 1;//stCfg -> gcvm_en;
    HI_U32 cvm_sel      = 0;//stCfg -> cvm_sel;
    //HI_U32 cvm_en     = 1;//stCfg -> cvm_en;
    HI_U32 gdm_en       = 1;//stCfg -> gdm_en;
    HI_U32 g_rgb2lms_en = 1;//stCfg -> g_rgb2lms_en;

    int16_t csc_coef[3][3] = {{0}, {0}, {0}};
    HI_S32 csc_offset[3]   = {0};

    //HI_U32 gamma_rd_en;
    //HI_U32 degamma_rd_en;
    //**************-----degamm and gamma---------------
    HI_U32 g_degmm_step[4];
    HI_U32 g_degmm_pos[4];
    HI_U32 g_degmm_num[4];

    HI_U32 g_gmm_step[4];
    HI_U32 g_gmm_pos[4];
    HI_U32 g_gmm_num[4];
    //HI_U32 g_rgb2yuv_en = 1;//stCfg -> g_rgb2yuv_en;

    //****************-----degamm-----**********************
    g_degmm_pos[0]  = 64;
    g_degmm_pos[1]  = 64 * 2;
    g_degmm_pos[2]  = 64 * 3;
    g_degmm_pos[3]  = 252;//255;
    g_degmm_step[0] = 2;
    g_degmm_step[1] = 2;
    g_degmm_step[2] = 2;
    g_degmm_step[3] = 2;
    g_degmm_num[0]  = 16;
    g_degmm_num[1]  = 16;
    g_degmm_num[2]  = 16;
    g_degmm_num[3]  = 15;
    //****************-----degamm end-----**********************
    //****************-----gamm-----**********************
    g_gmm_pos[0]  = 557056;  //1792 ;   //256;    //mod by suyong  //832;    //16384;
    g_gmm_pos[1]  = 2916352; //345856;  //15616;  //mod by suyong  //21312;  //16384*2;
    g_gmm_pos[2]  = 14450688;//4540160; //1326336;//mod by suyong  //1004352;//16384*3;
    g_gmm_pos[3]  = 29130752;//27493888;//mod by suyong //26170716;//16384*3;
    g_gmm_step[0] = 15;//4; //mod by suyong //5;//10;
    g_gmm_step[1] = 17;//16;//10;//mod by suyong //11;//10;
    g_gmm_step[2] = 19;//18;//16;//mod by suyong //16;//10;
    g_gmm_step[3] = 21;//20;//mod by suyong //21;//10;
    g_gmm_num[0]  = 17;//16;//mod by suyong //26;//16;
    g_gmm_num[1]  = 18;//15;//mod by suyong //10;//16;
    g_gmm_num[2]  = 22;//20;//mod by suyong //15;//16;
    g_gmm_num[3]  = 7; //13;//mod by suyong //12;//16;
    //****************-----degamm end-----**********************

    PQ_HAL_SetGdmDegammaCoef(gdm_degmm_lut);
    PQ_HAL_SetGdmgammaCoef(gdm_gmm_rgb2bt2020lut);

    PQ_HAL_HDR_SetGdmGamParaUpd(HAL_GFX_GP0, HI_TRUE);
    PQ_HAL_HDR_SetGdmDeGamParaUpd(HAL_GFX_GP0, HI_TRUE);

    PQ_HAL_HDR_SetGDmEn        ( gdm_en);
    PQ_HAL_HDR_SetGDmDegammaEn ( g_degamma_en);
    PQ_HAL_HDR_SetGDmRgb2lmsEn ( g_rgb2lms_en);


    csc_coef[0][0] = 10279;
    csc_coef[0][1] = 5396;
    csc_coef[0][2] = 710;
    csc_coef[1][0] = 1134;
    csc_coef[1][1] = 15064;
    csc_coef[1][2] = 187;
    csc_coef[2][0] = 268;
    csc_coef[2][1] = 1442;
    csc_coef[2][2] = 14673;

    csc_offset[0] = 0;
    csc_offset[1] = 0;
    csc_offset[2] = 0;


    PQ_HAL_HDR_SetGDmImapRgb2lms        ( csc_coef );
    PQ_HAL_HDR_SetGDmImapRgb2lmsscale2p ( 14 );
    PQ_HAL_HDR_SetGDmImapRgb2lmsmin     ( 0 );
    //PQ_HAL_HDR_SetGDmImapRgb2lmsmax   ( ((1<<25) -1) );
    PQ_HAL_HDR_SetGDmImapRgb2lmsmax     ( 26214400 );
    PQ_HAL_HDR_SetGDmImapRgb2lmsOutDc   ( csc_offset );

    PQ_HAL_HDR_SetGDmDegmmPos  ( g_degmm_pos);
    PQ_HAL_HDR_SetGDmDegmmStep ( g_degmm_step);
    PQ_HAL_HDR_SetGDmDegmmNum  ( g_degmm_num);
    PQ_HAL_HDR_SetGDmGammaEn   ( g_gamma_en);
    PQ_HAL_HDR_SetGDmGammaPos  ( g_gmm_pos);
    PQ_HAL_HDR_SetGDmGammaStep ( g_gmm_step);
    PQ_HAL_HDR_SetGDmGmmNum    ( g_gmm_num);
    // gcvm ------
    PQ_HAL_HDR_SetAuthEn ( gcvm_en);
    PQ_HAL_HDR_SetCvmSel ( cvm_sel);
    //PQ_HAL_HDR_SetCvmEn( cvm_en);

    return;
}

HI_VOID PQ_HAL_SetGfxRgbtYuvMode(PQ_HAL_GFX_CSC_MODE_S *pstPqCscMode)
{
    HI_U32 u32Pre ;
    HI_U32 u32DcPre ;

    HI_U32 u32Scale;
    HI_S32 csc_coef[3][3] = {{0}, {0}, {0}};
    HI_S32 cscDc[3] = {0};

    HI_U16 m33Yuv2RgbScale2P = 15;

    PQ_HAL_HDR_SetCvmEn (HI_TRUE);
    PQ_HAL_HDR_SetGCvmRgb2yuvEn (HI_TRUE);

    u32Pre   = 1 << 10;
    u32Scale = 1 << (m33Yuv2RgbScale2P - 10);
    u32DcPre = 4 * 64;

    if ((pstPqCscMode->eInputCS == PQ_HAL_GFX_CS_BT2020_RGB_FULL)
        && (pstPqCscMode->eOutputCS == PQ_HAL_GFX_CS_BT2020_YUV_LIMITED)) /* RGB_2020 -> YCbCr_2020 L */
    {
        csc_coef[0][0] = (HI_S32)( 0.2249 * u32Pre * u32Scale / 2);
        csc_coef[0][1] = (HI_S32)( 0.5806 * u32Pre * u32Scale / 2);
        csc_coef[0][2] = (HI_S32)( 0.0508 * u32Pre * u32Scale / 2);
        csc_coef[1][0] = (HI_S32)(-0.1223 * u32Pre * u32Scale / 2);
        csc_coef[1][1] = (HI_S32)(-0.3156 * u32Pre * u32Scale / 2);
        csc_coef[1][2] = (HI_S32)( 0.4379 * u32Pre * u32Scale / 2);
        csc_coef[2][0] = (HI_S32)( 0.4379 * u32Pre * u32Scale / 2);
        csc_coef[2][1] = (HI_S32)( -0.4027 * u32Pre * u32Scale / 2);
        csc_coef[2][2] = (HI_S32)(-0.0352 * u32Pre * u32Scale / 2);
        cscDc[0] = (HI_S32)(16  * u32DcPre / 2);
        cscDc[1] = (HI_S32)(128 * u32DcPre / 2);
        cscDc[2] = (HI_S32)(128 * u32DcPre / 2);
    }
    else if ((pstPqCscMode->eInputCS == PQ_HAL_GFX_CS_BT709_RGB_FULL)
             && (pstPqCscMode->eOutputCS == PQ_HAL_GFX_CS_BT709_YUV_LIMITED)) /* RGB -> YCbCr_709 L */
    {
        csc_coef[0][0] = (HI_S32)( 0.213 * u32Pre * u32Scale / 2);
        csc_coef[0][1] = (HI_S32)( 0.715 * u32Pre * u32Scale / 2);
        csc_coef[0][2] = (HI_S32)( 0.072 * u32Pre * u32Scale / 2);
        csc_coef[1][0] = (HI_S32)(-0.117 * u32Pre * u32Scale / 2);
        csc_coef[1][1] = (HI_S32)(-0.394 * u32Pre * u32Scale / 2);
        csc_coef[1][2] = (HI_S32)( 0.511 * u32Pre * u32Scale / 2);
        csc_coef[2][0] = (HI_S32)( 0.511 * u32Pre * u32Scale / 2);
        csc_coef[2][1] = (HI_S32)(-0.464 * u32Pre * u32Scale / 2);
        csc_coef[2][2] = (HI_S32)(-0.047 * u32Pre * u32Scale / 2);
        cscDc[0] = (HI_S32)(16  * u32DcPre / 2);
        cscDc[1] = (HI_S32)(128 * u32DcPre / 2);
        cscDc[2] = (HI_S32)(128 * u32DcPre / 2);
    }

    PQ_HAL_HDR_SetGCvmOmapRgb2yuvscale2p(m33Yuv2RgbScale2P | (1 << 5));
    PQ_HAL_HDR_SetGCvmOmapRgb2yuv       (csc_coef);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvOutDc  (cscDc);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvmin    (0);
    PQ_HAL_HDR_SetGCvmOmapRgb2yuvmax    (0x7FFF);

    return;
}



