#include <linux/kernel.h>
#include <linux/string.h>

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

typedef enum tagPQ_COEF_LUT_MODE
{
    COEF_DATA_TYPE_U8  = 0,
    COEF_DATA_TYPE_S8     ,
    COEF_DATA_TYPE_U16    ,
    COEF_DATA_TYPE_S16    ,
    COEF_DATA_TYPE_U32    ,
    COEF_DATA_TYPE_S32    ,

    COEF_DATA_TYPE_BUTT
} PQ_COEF_DATA_TYPE_E;

typedef enum tagPQ_STI_FILE_TYPE_E
{
    STI_FILE_DATA_VID0        = 0,
    STI_FILE_DATA_VID1           ,
    STI_FILE_DATA_VID2           ,
    STI_FILE_DATA_VID3           ,
    STI_FILE_DATA_VID4           ,
    STI_FILE_DATA_GFX0           ,
    STI_FILE_DATA_GFX1           ,
    STI_FILE_DATA_GFX2           ,
    STI_FILE_DATA_GFX3           ,
    STI_FILE_DATA_GFX4           ,

    STI_FILE_COEF_ZME_V0         ,
    STI_FILE_COEF_ACM            ,
    STI_FILE_COEF_DCI            ,
    STI_FILE_COEF_DIM            ,
    STI_FILE_COEF_GMM            ,
    STI_FILE_COEF_OD             ,
    STI_FILE_COEF_RGBW           ,
    STI_FILE_COEF_DEMURA         ,
    STI_FILE_TYPE_NODEF          ,
    STI_FILE_TILE_64_CMP_Y       ,
    STI_FILE_TILE_64_CMP_C       ,
    STI_FILE_TILE_64_CMP_Y_2BIT  ,
    STI_FILE_TILE_64_CMP_C_2BIT  ,
    STI_FILE_TILE_64_CMP_Y_HEAD  ,
    STI_FILE_TILE_64_CMP_C_HEAD  ,
    STI_FILE_OSD_CMP             ,
    STI_FILE_COEF_DM             ,
    STI_FILE_COEF_GDM            ,
    STI_FILE_COEF_METADATA       ,
    STI_FILE_DATA_TNR_REF        ,
    STI_FILE_DATA_TNR_MAD        ,
    STI_FILE_DATA_VDH_DB         ,

    STI_FILE_DATA_VMX            ,

    STI_FILE_TYPE_BUTT
} PQ_STI_FILE_TYPE_E;

typedef struct
{
    HI_U8                  *coef_addr       ;
    PQ_STI_FILE_TYPE_E      sti_type        ;
    FILE                   *fp_coef         ;
    HI_U32                  lut_num         ;
    HI_U32                  burst_num       ;
    HI_U32                  cycle_num       ;
    void                  **p_coef_array    ;
    HI_U32                 *lut_length      ;
    HI_U32                 *coef_bit_length ;
    PQ_COEF_DATA_TYPE_E     data_type       ;
} PQ_COEF_SEND_CFG;


//U12.0 [0,4095]
static const HI_U32 au32GdmPQLut[64] =
{
    205 ,
    264 ,
    320 ,
    370 ,
    420 ,
    657 ,
    749 ,
    820 ,
    878 ,
    927 ,
    970 ,
    1008,
    1129,
    1220,
    1354,
    1454,
    1533,
    1600,
    1708,
    1794,
    1865,
    1926,
    1980,
    2146,
    2267,
    2361,
    2440,
    2506,
    2564,
    2616,
    2662,
    2704,
    2742,
    2777,
    2810,
    2841,
    2869,
    2969,
    3050,
    3119,
    3178,
    3231,
    3278,
    3321,
    3360,
    3395,
    3428,
    3459,
    3488,
    3515,
    3540,
    3564,
    3587,
    3668,
    3736,
    3794,
    3846,
    3892,
    3933,
    3971,
    4006,
    4038,
    4067,
    4095
};


HI_VOID PQ_HAL_SetGdmGdmTmapInBdp(HI_U32 u32Data, HI_U32 gdm_tmap_in_bdp)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CTRL.bits.gdm_tmap_in_bdp = gdm_tmap_in_bdp;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmGmmEn(HI_U32 u32Data, HI_U32 gdm_gmm_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CTRL.bits.gdm_gmm_en = gdm_gmm_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapEn(HI_U32 u32Data, HI_U32 gdm_tmap_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CTRL.bits.gdm_tmap_en = gdm_tmap_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDegmmEn(HI_U32 u32Data, HI_U32 gdm_degmm_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CTRL.bits.gdm_degmm_en = gdm_degmm_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmEn(HI_U32 u32Data, HI_U32 gdm_en)
{
    U_GDM_CTRL GDM_CTRL;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CTRL.bits.gdm_en = gdm_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmLumaCoef0Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef0_tmap)
{
    U_GDM_TMAP_LUMA_COEF0 GDM_TMAP_LUMA_COEF0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_LUMA_COEF0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_LUMA_COEF0.bits.gdm_luma_coef0_tmap = gdm_luma_coef0_tmap;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF0.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_LUMA_COEF0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmLumaCoef1Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef1_tmap)
{
    U_GDM_TMAP_LUMA_COEF1 GDM_TMAP_LUMA_COEF1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_LUMA_COEF1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_LUMA_COEF1.bits.gdm_luma_coef1_tmap = gdm_luma_coef1_tmap;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF1.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_LUMA_COEF1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmLumaCoef2Tmap(HI_U32 u32Data, HI_U32 gdm_luma_coef2_tmap)
{
    U_GDM_TMAP_LUMA_COEF2 GDM_TMAP_LUMA_COEF2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_LUMA_COEF2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_LUMA_COEF2.bits.gdm_luma_coef2_tmap = gdm_luma_coef2_tmap;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_COEF2.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_LUMA_COEF2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapLumaScale(HI_U32 u32Data, HI_U32 gdm_tmap_luma_scale)
{
    U_GDM_TMAP_LUMA_SCALE GDM_TMAP_LUMA_SCALE;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_LUMA_SCALE.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_SCALE.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_LUMA_SCALE.bits.gdm_tmap_luma_scale = gdm_tmap_luma_scale;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_LUMA_SCALE.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_LUMA_SCALE.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapCoefScale(HI_U32 u32Data, HI_U32 gdm_tmap_coef_scale)
{
    U_GDM_TMAP_COEF_SCALE GDM_TMAP_COEF_SCALE;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_COEF_SCALE.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_COEF_SCALE.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_COEF_SCALE.bits.gdm_tmap_coef_scale = gdm_tmap_coef_scale;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_COEF_SCALE.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_COEF_SCALE.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapOutClipMin(HI_U32 u32Data, HI_U32 gdm_tmap_out_clip_min)
{
    U_GDM_TMAP_OUT_CLIP_MIN GDM_TMAP_OUT_CLIP_MIN;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_OUT_CLIP_MIN.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_OUT_CLIP_MIN.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_OUT_CLIP_MIN.bits.gdm_tmap_out_clip_min = gdm_tmap_out_clip_min;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_OUT_CLIP_MIN.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_OUT_CLIP_MIN.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapOutClipMax(HI_U32 u32Data, HI_U32 gdm_tmap_out_clip_max)
{
    U_GDM_TMAP_OUT_CLIP_MAX GDM_TMAP_OUT_CLIP_MAX;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TMAP_OUT_CLIP_MAX.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_OUT_CLIP_MAX.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TMAP_OUT_CLIP_MAX.bits.gdm_tmap_out_clip_max = gdm_tmap_out_clip_max;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TMAP_OUT_CLIP_MAX.u32)) + u32Data * PQ_GP_OFFSET), GDM_TMAP_OUT_CLIP_MAX.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmParaGmmAddr(HI_U32 u32Data, HI_U32 gdm_para_gmm_addr)
{
    U_GDM_PARA_GMM_ADDR GDM_PARA_GMM_ADDR;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_PARA_GMM_ADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_GMM_ADDR.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_PARA_GMM_ADDR.bits.gdm_para_gmm_addr = gdm_para_gmm_addr;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_GMM_ADDR.u32)) + u32Data * PQ_GP_OFFSET), GDM_PARA_GMM_ADDR.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmParaTmapAddr(HI_U32 u32Data, HI_U32 gdm_para_tmap_addr)
{
    U_GDM_PARA_TMAP_ADDR GDM_PARA_TMAP_ADDR;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_PARA_TMAP_ADDR.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_TMAP_ADDR.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_PARA_TMAP_ADDR.bits.gdm_para_tmap_addr = gdm_para_tmap_addr;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_TMAP_ADDR.u32)) + u32Data * PQ_GP_OFFSET), GDM_PARA_TMAP_ADDR.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmParaTmapUpd(HI_U32 u32Data, HI_U32 gdm_para_tmap_upd)
{
    U_GDM_PARA_UP GDM_PARA_UP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_PARA_UP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_UP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_PARA_UP.bits.gdm_para_tmap_upd = gdm_para_tmap_upd;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_UP.u32)) + u32Data * PQ_GP_OFFSET), GDM_PARA_UP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmParaGmmUpd(HI_U32 u32Data, HI_U32 gdm_para_gmm_upd)
{
    U_GDM_PARA_UP GDM_PARA_UP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_PARA_UP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_UP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_PARA_UP.bits.gdm_para_gmm_upd = gdm_para_gmm_upd;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_PARA_UP.u32)) + u32Data * PQ_GP_OFFSET), GDM_PARA_UP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX4Step(HI_U32 u32Data, HI_U32 g_degmm_x4_step)
{
    U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_STEP.bits.g_degmm_x4_step = g_degmm_x4_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX3Step(HI_U32 u32Data, HI_U32 g_degmm_x3_step)
{
    U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_STEP.bits.g_degmm_x3_step = g_degmm_x3_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX2Step(HI_U32 u32Data, HI_U32 g_degmm_x2_step)
{
    U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_STEP.bits.g_degmm_x2_step = g_degmm_x2_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX1Step(HI_U32 u32Data, HI_U32 g_degmm_x1_step)
{
    U_GDM_DEGAMMA_STEP GDM_DEGAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_STEP.bits.g_degmm_x1_step = g_degmm_x1_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX2Pos(HI_U32 u32Data, HI_U32 g_degmm_x2_pos)
{
    U_GDM_DEGAMMA_POS0 GDM_DEGAMMA_POS0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_POS0.bits.g_degmm_x2_pos = g_degmm_x2_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_POS0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX1Pos(HI_U32 u32Data, HI_U32 g_degmm_x1_pos)
{
    U_GDM_DEGAMMA_POS0 GDM_DEGAMMA_POS0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_POS0.bits.g_degmm_x1_pos = g_degmm_x1_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_POS0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX4Pos(HI_U32 u32Data, HI_U32 g_degmm_x4_pos)
{
    U_GDM_DEGAMMA_POS1 GDM_DEGAMMA_POS1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_POS1.bits.g_degmm_x4_pos = g_degmm_x4_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_POS1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX3Pos(HI_U32 u32Data, HI_U32 g_degmm_x3_pos)
{
    U_GDM_DEGAMMA_POS1 GDM_DEGAMMA_POS1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_POS1.bits.g_degmm_x3_pos = g_degmm_x3_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_POS1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX4Num(HI_U32 u32Data, HI_U32 g_degmm_x4_num)
{
    U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_NUM.bits.g_degmm_x4_num = g_degmm_x4_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX3Num(HI_U32 u32Data, HI_U32 g_degmm_x3_num)
{
    U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_NUM.bits.g_degmm_x3_num = g_degmm_x3_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX2Num(HI_U32 u32Data, HI_U32 g_degmm_x2_num)
{
    U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_NUM.bits.g_degmm_x2_num = g_degmm_x2_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGDegmmX1Num(HI_U32 u32Data, HI_U32 g_degmm_x1_num)
{
    U_GDM_DEGAMMA_NUM GDM_DEGAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DEGAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DEGAMMA_NUM.bits.g_degmm_x1_num = g_degmm_x1_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DEGAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_DEGAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapRdEn(HI_U32 u32Data, HI_U32 gdm_tmap_rd_en)
{
    U_GDM_TONEMAP_REN GDM_TONEMAP_REN;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_REN.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_REN.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_REN.bits.gdm_tmap_rd_en = gdm_tmap_rd_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_REN.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_REN.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmTmapParaData(HI_U32 u32Data, HI_U32 gdm_tmap_para_data)
{
    U_GDM_TONEMAP_DATA GDM_TONEMAP_DATA;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_DATA.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_DATA.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_DATA.bits.gdm_tmap_para_data = gdm_tmap_para_data;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_DATA.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_DATA.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX4Step(HI_U32 u32Data, HI_U32 g_tmap_x4_step)
{
    U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_STEP.bits.g_tmap_x4_step = g_tmap_x4_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX3Step(HI_U32 u32Data, HI_U32 g_tmap_x3_step)
{
    U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_STEP.bits.g_tmap_x3_step = g_tmap_x3_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX2Step(HI_U32 u32Data, HI_U32 g_tmap_x2_step)
{
    U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_STEP.bits.g_tmap_x2_step = g_tmap_x2_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX1Step(HI_U32 u32Data, HI_U32 g_tmap_x1_step)
{
    U_GDM_TONEMAP_STEP GDM_TONEMAP_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_STEP.bits.g_tmap_x1_step = g_tmap_x1_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX1Pos(HI_U32 u32Data, HI_U32 g_tmap_x1_pos)
{
    U_GDM_TONEMAP_POS0 GDM_TONEMAP_POS0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_POS0.bits.g_tmap_x1_pos = g_tmap_x1_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS0.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_POS0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX2Pos(HI_U32 u32Data, HI_U32 g_tmap_x2_pos)
{
    U_GDM_TONEMAP_POS1 GDM_TONEMAP_POS1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_POS1.bits.g_tmap_x2_pos = g_tmap_x2_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS1.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_POS1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX4Num(HI_U32 u32Data, HI_U32 g_tmap_x4_num)
{
    U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_NUM.bits.g_tmap_x4_num = g_tmap_x4_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX3Num(HI_U32 u32Data, HI_U32 g_tmap_x3_num)
{
    U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_NUM.bits.g_tmap_x3_num = g_tmap_x3_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX2Num(HI_U32 u32Data, HI_U32 g_tmap_x2_num)
{
    U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_NUM.bits.g_tmap_x2_num = g_tmap_x2_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX1Num(HI_U32 u32Data, HI_U32 g_tmap_x1_num)
{
    U_GDM_TONEMAP_NUM GDM_TONEMAP_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_NUM.bits.g_tmap_x1_num = g_tmap_x1_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX3Pos(HI_U32 u32Data, HI_U32 g_tmap_x3_pos)
{
    U_GDM_TONEMAP_POS2 GDM_TONEMAP_POS2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_POS2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_POS2.bits.g_tmap_x3_pos = g_tmap_x3_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS2.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_POS2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGTmapX4Pos(HI_U32 u32Data, HI_U32 g_tmap_x4_pos)
{
    U_GDM_TONEMAP_POS3 GDM_TONEMAP_POS3;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_TONEMAP_POS3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_TONEMAP_POS3.bits.g_tmap_x4_pos = g_tmap_x4_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_TONEMAP_POS3.u32)) + u32Data * PQ_GP_OFFSET), GDM_TONEMAP_POS3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmGammaRdEn(HI_U32 u32Data, HI_U32 gdm_gamma_rd_en)
{
    U_GDM_GAMMA_REN GDM_GAMMA_REN;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_REN.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_REN.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_REN.bits.gdm_gamma_rd_en = gdm_gamma_rd_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_REN.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_REN.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmGammaParaData(HI_U32 u32Data, HI_U32 gdm_gamma_para_data)
{
    U_GDM_GAMMMA_DATA GDM_GAMMMA_DATA;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMMA_DATA.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMMA_DATA.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMMA_DATA.bits.gdm_gamma_para_data = gdm_gamma_para_data;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMMA_DATA.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMMA_DATA.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX4Step(HI_U32 u32Data, HI_U32 g_gmm_x4_step)
{
    U_GDM_GAMMA_STEP GDM_GAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP.bits.g_gmm_x4_step = g_gmm_x4_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX3Step(HI_U32 u32Data, HI_U32 g_gmm_x3_step)
{
    U_GDM_GAMMA_STEP GDM_GAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP.bits.g_gmm_x3_step = g_gmm_x3_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX2Step(HI_U32 u32Data, HI_U32 g_gmm_x2_step)
{
    U_GDM_GAMMA_STEP GDM_GAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP.bits.g_gmm_x2_step = g_gmm_x2_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX1Step(HI_U32 u32Data, HI_U32 g_gmm_x1_step)
{
    U_GDM_GAMMA_STEP GDM_GAMMA_STEP;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP.bits.g_gmm_x1_step = g_gmm_x1_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX2Pos(HI_U32 u32Data, HI_U32 g_gmm_x2_pos)
{
    U_GDM_GAMMA_POS0 GDM_GAMMA_POS0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS0.bits.g_gmm_x2_pos = g_gmm_x2_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX1Pos(HI_U32 u32Data, HI_U32 g_gmm_x1_pos)
{
    U_GDM_GAMMA_POS0 GDM_GAMMA_POS0;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS0.bits.g_gmm_x1_pos = g_gmm_x1_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS0.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX4Pos(HI_U32 u32Data, HI_U32 g_gmm_x4_pos)
{
    U_GDM_GAMMA_POS1 GDM_GAMMA_POS1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS1.bits.g_gmm_x4_pos = g_gmm_x4_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX3Pos(HI_U32 u32Data, HI_U32 g_gmm_x3_pos)
{
    U_GDM_GAMMA_POS1 GDM_GAMMA_POS1;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS1.bits.g_gmm_x3_pos = g_gmm_x3_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS1.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX4Num(HI_U32 u32Data, HI_U32 g_gmm_x4_num)
{
    U_GDM_GAMMA_NUM GDM_GAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM.bits.g_gmm_x4_num = g_gmm_x4_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX3Num(HI_U32 u32Data, HI_U32 g_gmm_x3_num)
{
    U_GDM_GAMMA_NUM GDM_GAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM.bits.g_gmm_x3_num = g_gmm_x3_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX2Num(HI_U32 u32Data, HI_U32 g_gmm_x2_num)
{
    U_GDM_GAMMA_NUM GDM_GAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM.bits.g_gmm_x2_num = g_gmm_x2_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX1Num(HI_U32 u32Data, HI_U32 g_gmm_x1_num)
{
    U_GDM_GAMMA_NUM GDM_GAMMA_NUM;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM.bits.g_gmm_x1_num = g_gmm_x1_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmGGmmX8Step(HI_U32 u32Data, HI_U32 g_gmm_x8_step)
{
    U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP2.bits.g_gmm_x8_step = g_gmm_x8_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX7Step(HI_U32 u32Data, HI_U32 g_gmm_x7_step)
{
    U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP2.bits.g_gmm_x7_step = g_gmm_x7_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX6Step(HI_U32 u32Data, HI_U32 g_gmm_x6_step)
{
    U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP2.bits.g_gmm_x6_step = g_gmm_x6_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX5Step(HI_U32 u32Data, HI_U32 g_gmm_x5_step)
{
    U_GDM_GAMMA_STEP2 GDM_GAMMA_STEP2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_STEP2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_STEP2.bits.g_gmm_x5_step = g_gmm_x5_step;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_STEP2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_STEP2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX6Pos(HI_U32 u32Data, HI_U32 g_gmm_x6_pos)
{
    U_GDM_GAMMA_POS2 GDM_GAMMA_POS2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS2.bits.g_gmm_x6_pos = g_gmm_x6_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX5Pos(HI_U32 u32Data, HI_U32 g_gmm_x5_pos)
{
    U_GDM_GAMMA_POS2 GDM_GAMMA_POS2;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS2.bits.g_gmm_x5_pos = g_gmm_x5_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX8Pos(HI_U32 u32Data, HI_U32 g_gmm_x8_pos)
{
    U_GDM_GAMMA_POS3 GDM_GAMMA_POS3;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS3.bits.g_gmm_x8_pos = g_gmm_x8_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS3.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX7Pos(HI_U32 u32Data, HI_U32 g_gmm_x7_pos)
{
    U_GDM_GAMMA_POS3 GDM_GAMMA_POS3;

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_POS3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_POS3.bits.g_gmm_x7_pos = g_gmm_x7_pos;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_POS3.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_POS3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX8Num(HI_U32 u32Data, HI_U32 g_gmm_x8_num)
{
    U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGGmmX8Num Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM2.bits.g_gmm_x8_num = g_gmm_x8_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX7Num(HI_U32 u32Data, HI_U32 g_gmm_x7_num)
{
    U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGGmmX7Num Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM2.bits.g_gmm_x7_num = g_gmm_x7_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX6Num(HI_U32 u32Data, HI_U32 g_gmm_x6_num)
{
    U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGGmmX6Num Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM2.bits.g_gmm_x6_num = g_gmm_x6_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGGmmX5Num(HI_U32 u32Data, HI_U32 g_gmm_x5_num)
{
    U_GDM_GAMMA_NUM2 GDM_GAMMA_NUM2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGGmmX5Num Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_GAMMA_NUM2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_GAMMA_NUM2.bits.g_gmm_x5_num = g_gmm_x5_num;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_GAMMA_NUM2.u32)) + u32Data * PQ_GP_OFFSET), GDM_GAMMA_NUM2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCscEn(HI_U32 u32Data, HI_U32 gdm_csc_en)
{
    U_GDM_CSC_CTRL GDM_CSC_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCscEn Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_CTRL.bits.gdm_csc_en = gdm_csc_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef00Csc(HI_U32 u32Data, HI_U32 gdm_coef00_csc)
{
    U_GDM_CSC_COEF00 GDM_CSC_COEF00;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef00Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF00.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF00.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF00.bits.gdm_coef00_csc = gdm_coef00_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF00.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF00.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef01Csc(HI_U32 u32Data, HI_U32 gdm_coef01_csc)
{
    U_GDM_CSC_COEF01 GDM_CSC_COEF01;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef01Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF01.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF01.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF01.bits.gdm_coef01_csc = gdm_coef01_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF01.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF01.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef02Csc(HI_U32 u32Data, HI_U32 gdm_coef02_csc)
{
    U_GDM_CSC_COEF02 GDM_CSC_COEF02;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef02Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF02.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF02.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF02.bits.gdm_coef02_csc = gdm_coef02_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF02.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF02.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef10Csc(HI_U32 u32Data, HI_U32 gdm_coef10_csc)
{
    U_GDM_CSC_COEF10 GDM_CSC_COEF10;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef10Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF10.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF10.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF10.bits.gdm_coef10_csc = gdm_coef10_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF10.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF10.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef11Csc(HI_U32 u32Data, HI_U32 gdm_coef11_csc)
{
    U_GDM_CSC_COEF11 GDM_CSC_COEF11;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef11Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF11.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF11.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF11.bits.gdm_coef11_csc = gdm_coef11_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF11.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF11.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef12Csc(HI_U32 u32Data, HI_U32 gdm_coef12_csc)
{
    U_GDM_CSC_COEF12 GDM_CSC_COEF12;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef12Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF12.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF12.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF12.bits.gdm_coef12_csc = gdm_coef12_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF12.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF12.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef20Csc(HI_U32 u32Data, HI_U32 gdm_coef20_csc)
{
    U_GDM_CSC_COEF20 GDM_CSC_COEF20;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef20Csc Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF20.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF20.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF20.bits.gdm_coef20_csc = gdm_coef20_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF20.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF20.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef21Csc(HI_U32 u32Data, HI_U32 gdm_coef21_csc)
{
    U_GDM_CSC_COEF21 GDM_CSC_COEF21;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef21Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF21.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF21.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF21.bits.gdm_coef21_csc = gdm_coef21_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF21.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF21.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCoef22Csc(HI_U32 u32Data, HI_U32 gdm_coef22_csc)
{
    U_GDM_CSC_COEF22 GDM_CSC_COEF22;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCoef22Csc Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_COEF22.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF22.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_COEF22.bits.gdm_coef22_csc = gdm_coef22_csc;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_COEF22.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_COEF22.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCscScale(HI_U32 u32Data, HI_U32 gdm_csc_scale)
{
    U_GDM_CSC_SCALE GDM_CSC_SCALE;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCscScale Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_SCALE.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_SCALE.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_SCALE.bits.gdm_csc_scale = gdm_csc_scale;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_SCALE.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_SCALE.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCscClipMin(HI_U32 u32Data, HI_U32 gdm_csc_clip_min)
{
    U_GDM_CSC_CLIP_MIN GDM_CSC_CLIP_MIN;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCscClipMin Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_CLIP_MIN.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CLIP_MIN.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_CLIP_MIN.bits.gdm_csc_clip_min = gdm_csc_clip_min;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CLIP_MIN.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_CLIP_MIN.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmCscClipMax(HI_U32 u32Data, HI_U32 gdm_csc_clip_max)
{
    U_GDM_CSC_CLIP_MAX GDM_CSC_CLIP_MAX;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmCscClipMax Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_CSC_CLIP_MAX.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CLIP_MAX.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_CSC_CLIP_MAX.bits.gdm_csc_clip_max = gdm_csc_clip_max;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_CSC_CLIP_MAX.u32)) + u32Data * PQ_GP_OFFSET), GDM_CSC_CLIP_MAX.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherRoundUnlim(HI_U32 u32Data, HI_U32 gdm_dither_round_unlim)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherRoundUnlim Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_round_unlim = gdm_dither_round_unlim;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherEn(HI_U32 u32Data, HI_U32 gdm_dither_en)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherEn Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_en = gdm_dither_en;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherMode(HI_U32 u32Data, HI_U32 gdm_dither_mode)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherMode Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_mode = gdm_dither_mode;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherRound(HI_U32 u32Data, HI_U32 gdm_dither_round)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherTapMode Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_round = gdm_dither_round;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherDomainMode(HI_U32 u32Data, HI_U32 gdm_dither_domain_mode)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherTapMode Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_domain_mode = gdm_dither_domain_mode;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherTapMode(HI_U32 u32Data, HI_U32 gdm_dither_tap_mode)
{
    U_GDM_DITHER_CTRL GDM_DITHER_CTRL;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherTapMode Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_CTRL.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_CTRL.bits.gdm_dither_tap_mode = gdm_dither_tap_mode;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_CTRL.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_CTRL.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedY0(HI_U32 u32Data, HI_U32 gdm_dither_sed_y0)
{
    U_GDM_DITHER_SED_Y0 GDM_DITHER_SED_Y0;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedY0 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_Y0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_Y0.bits.gdm_dither_sed_y0 = gdm_dither_sed_y0;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_Y0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedU0(HI_U32 u32Data, HI_U32 gdm_dither_sed_u0)
{
    U_GDM_DITHER_SED_U0 GDM_DITHER_SED_U0;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedU0 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_U0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_U0.bits.gdm_dither_sed_u0 = gdm_dither_sed_u0;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_U0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedV0(HI_U32 u32Data, HI_U32 gdm_dither_sed_v0)
{
    U_GDM_DITHER_SED_V0 GDM_DITHER_SED_V0;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedV0 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_V0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_V0.bits.gdm_dither_sed_v0 = gdm_dither_sed_v0;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_V0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedW0(HI_U32 u32Data, HI_U32 gdm_dither_sed_w0)
{
    U_GDM_DITHER_SED_W0 GDM_DITHER_SED_W0;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedW0 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_W0.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W0.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_W0.bits.gdm_dither_sed_w0 = gdm_dither_sed_w0;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W0.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_W0.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedY1(HI_U32 u32Data, HI_U32 gdm_dither_sed_y1)
{
    U_GDM_DITHER_SED_Y1 GDM_DITHER_SED_Y1;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedY1 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_Y1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_Y1.bits.gdm_dither_sed_y1 = gdm_dither_sed_y1;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_Y1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedU1(HI_U32 u32Data, HI_U32 gdm_dither_sed_u1)
{
    U_GDM_DITHER_SED_U1 GDM_DITHER_SED_U1;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedU1 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_U1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_U1.bits.gdm_dither_sed_u1 = gdm_dither_sed_u1;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_U1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedV1(HI_U32 u32Data, HI_U32 gdm_dither_sed_v1)
{
    U_GDM_DITHER_SED_V1 GDM_DITHER_SED_V1;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedV1 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_V1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_V1.bits.gdm_dither_sed_v1 = gdm_dither_sed_v1;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_V1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedW1(HI_U32 u32Data, HI_U32 gdm_dither_sed_w1)
{
    U_GDM_DITHER_SED_W1 GDM_DITHER_SED_W1;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedW1 Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_W1.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W1.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_W1.bits.gdm_dither_sed_w1 = gdm_dither_sed_w1;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W1.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_W1.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedY2(HI_U32 u32Data, HI_U32 gdm_dither_sed_y2)
{
    U_GDM_DITHER_SED_Y2 GDM_DITHER_SED_Y2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedY2 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_Y2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_Y2.bits.gdm_dither_sed_y2 = gdm_dither_sed_y2;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y2.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_Y2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedU2(HI_U32 u32Data, HI_U32 gdm_dither_sed_u2)
{
    U_GDM_DITHER_SED_U2 GDM_DITHER_SED_U2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedU2 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_U2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_U2.bits.gdm_dither_sed_u2 = gdm_dither_sed_u2;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U2.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_U2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedV2(HI_U32 u32Data, HI_U32 gdm_dither_sed_v2)
{
    U_GDM_DITHER_SED_V2 GDM_DITHER_SED_V2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedV2 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_V2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_V2.bits.gdm_dither_sed_v2 = gdm_dither_sed_v2;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V2.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_V2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedW2(HI_U32 u32Data, HI_U32 gdm_dither_sed_w2)
{
    U_GDM_DITHER_SED_W2 GDM_DITHER_SED_W2;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedW2 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_W2.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W2.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_W2.bits.gdm_dither_sed_w2 = gdm_dither_sed_w2;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W2.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_W2.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedY3(HI_U32 u32Data, HI_U32 gdm_dither_sed_y3)
{
    U_GDM_DITHER_SED_Y3 GDM_DITHER_SED_Y3;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedY3 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_Y3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_Y3.bits.gdm_dither_sed_y3 = gdm_dither_sed_y3;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_Y3.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_Y3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedU3(HI_U32 u32Data, HI_U32 gdm_dither_sed_u3)
{
    U_GDM_DITHER_SED_U3 GDM_DITHER_SED_U3;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedU3 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_U3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_U3.bits.gdm_dither_sed_u3 = gdm_dither_sed_u3;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_U3.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_U3.u32);

    return;
}


HI_VOID PQ_HAL_SetGdmGdmDitherSedV3(HI_U32 u32Data, HI_U32 gdm_dither_sed_v3)
{
    U_GDM_DITHER_SED_V3 GDM_DITHER_SED_V3;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedV3 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_V3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_V3.bits.gdm_dither_sed_v3 = gdm_dither_sed_v3;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_V3.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_V3.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmGdmDitherSedW3(HI_U32 u32Data, HI_U32 gdm_dither_sed_w3)
{
    U_GDM_DITHER_SED_W3 GDM_DITHER_SED_W3;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherSedW3 Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_SED_W3.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W3.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_SED_W3.bits.gdm_dither_sed_w3 = gdm_dither_sed_w3;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_SED_W3.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_SED_W3.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmGdmDitherThrMax(HI_U32 u32Data, HI_U32 gdm_dither_thr_max)
{
    U_GDM_DITHER_THR GDM_DITHER_THR;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherThrMax Select Wrong Layer ID\n");
    }

    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_THR.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_THR.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_THR.bits.gdm_dither_thr_max = gdm_dither_thr_max;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_THR.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_THR.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmGdmDitherThrMin(HI_U32 u32Data, HI_U32 gdm_dither_thr_min)
{
    U_GDM_DITHER_THR GDM_DITHER_THR;

    if (u32Data >= PQ_GP_MAX)
    {
        HI_ERR_PQ("Error, SetGdmGdmDitherThrMin Select Wrong Layer ID\n");
    }
    pVdpReg = PQ_HAL_GetVdpReg();

    GDM_DITHER_THR.u32 = PQ_HAL_RegRead((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_THR.u32)) + u32Data * PQ_GP_OFFSET));
    GDM_DITHER_THR.bits.gdm_dither_thr_min = gdm_dither_thr_min;
    PQ_HAL_RegWrite((HI_U32 *)((unsigned long)(&(pVdpReg->GDM_DITHER_THR.u32)) + u32Data * PQ_GP_OFFSET), GDM_DITHER_THR.u32);

    return;
}

HI_VOID PQ_HAL_SetGdmPara(HI_U32 enLayer, PQ_HAL_GDMCFG_S *pstGDmCfg)
{
    /* filed declare */
    HI_U32 gdm_en          =  0;
    HI_U32 gdm_degmm_en    =  0;
    HI_U32 gdm_csc_en      =  0;
    HI_U32 gdm_tmap_en     =  0;
    HI_U32 gdm_gmm_en      =  0;
    HI_U32 gdm_dither_en   =  0;
    HI_U32 gdm_rgb2yuv_en  =  0;

    HI_U32 gdm_tmap_in_bdp =  0;
    HI_U32 gdm_dither_mode =  0;

    HI_U16 gdm_luma_coef0_tmap   =  0;
    HI_U16 gdm_luma_coef1_tmap   =  0;
    HI_U16 gdm_luma_coef2_tmap   =  0;
    HI_U16 gdm_tmap_luma_scale   =  0;
    HI_U16 gdm_tmap_coef_scale   =  0;
    HI_U16 gdm_tmap_out_clip_min =  0;
    HI_U16 gdm_tmap_out_clip_max =  0;
    //HI_U32 gdm_para_gmm_addr   =  0;
    //HI_U32 gdm_para_tmap_addr  =  0;
    //HI_U32 gdm_para_tmap_upd   =  1;
    //HI_U32 gdm_para_gmm_upd    =  1;
    HI_U32 g_degmm_x4_step     =  0;
    HI_U32 g_degmm_x3_step     =  0;
    HI_U32 g_degmm_x2_step     =  0;
    HI_U32 g_degmm_x1_step     =  0;
    HI_U32 g_degmm_x2_pos      =  0;
    HI_U32 g_degmm_x1_pos      =  0;
    HI_U32 g_degmm_x4_pos      =  0;
    HI_U32 g_degmm_x3_pos      =  0;
    HI_U32 g_degmm_x4_num      =  0;
    HI_U32 g_degmm_x3_num      =  0;
    HI_U32 g_degmm_x2_num      =  0;
    HI_U32 g_degmm_x1_num      =  0;
    HI_U32 gdm_tmap_rd_en      =  0;
    HI_U32 gdm_tmap_para_data  =  0;
    HI_U32 g_tmap_x4_step      =  0;
    HI_U32 g_tmap_x3_step      =  0;
    HI_U32 g_tmap_x2_step      =  0;
    HI_U32 g_tmap_x1_step      =  0;
    HI_U32 g_tmap_x1_pos       =  0;
    HI_U32 g_tmap_x2_pos       =  0;
    HI_U32 g_tmap_x4_num       =  0;
    HI_U32 g_tmap_x3_num       =  0;
    HI_U32 g_tmap_x2_num       =  0;
    HI_U32 g_tmap_x1_num       =  0;
    HI_U32 g_tmap_x3_pos       =  0;
    HI_U32 g_tmap_x4_pos       =  0;
    HI_U32 gdm_gamma_rd_en     =  0;
    HI_U32 gdm_gamma_para_data =  0;

    //add_regs
    HI_U32 g_gmm_x8_step = 0;
    HI_U32 g_gmm_x7_step = 0;
    HI_U32 g_gmm_x6_step = 0;
    HI_U32 g_gmm_x5_step = 0;
    HI_U32 g_gmm_x6_pos  = 0;
    HI_U32 g_gmm_x5_pos  = 0;
    HI_U32 g_gmm_x8_pos  = 0;
    HI_U32 g_gmm_x7_pos  = 0;
    HI_U32 g_gmm_x8_num  = 0;
    HI_U32 g_gmm_x7_num  = 0;
    HI_U32 g_gmm_x6_num  = 0;
    HI_U32 g_gmm_x5_num  = 0;

    HI_U32 g_gmm_x4_step    =  0;
    HI_U32 g_gmm_x3_step    =  0;
    HI_U32 g_gmm_x2_step    =  0;
    HI_U32 g_gmm_x1_step    =  0;
    HI_U32 g_gmm_x2_pos     =  0;
    HI_U32 g_gmm_x1_pos     =  0;
    HI_U32 g_gmm_x4_pos     =  0;
    HI_U32 g_gmm_x3_pos     =  0;
    HI_U32 g_gmm_x4_num     =  0;
    HI_U32 g_gmm_x3_num     =  0;
    HI_U32 g_gmm_x2_num     =  0;
    HI_U32 g_gmm_x1_num     =  0;
    HI_S16 gdm_coef00_csc   =  0;
    HI_S16 gdm_coef01_csc   =  0;
    HI_S16 gdm_coef02_csc   =  0;
    HI_S16 gdm_coef10_csc   =  0;
    HI_S16 gdm_coef11_csc   =  0;
    HI_S16 gdm_coef12_csc   =  0;
    HI_S16 gdm_coef20_csc   =  0;
    HI_S16 gdm_coef21_csc   =  0;
    HI_S16 gdm_coef22_csc   =  0;
    HI_U16 gdm_csc_scale    =  0;
    HI_U32 gdm_csc_clip_min =  0;
    HI_U32 gdm_csc_clip_max =  0;

    HI_S16 gdm_00_rgb2yuv   =  0;
    HI_S16 gdm_01_rgb2yuv   =  0;
    HI_S16 gdm_02_rgb2yuv   =  0;
    HI_S16 gdm_10_rgb2yuv   =  0;
    HI_S16 gdm_11_rgb2yuv   =  0;
    HI_S16 gdm_12_rgb2yuv   =  0;
    HI_S16 gdm_20_rgb2yuv   =  0;
    HI_S16 gdm_21_rgb2yuv   =  0;
    HI_S16 gdm_22_rgb2yuv   =  0;

    HI_U16 gdm_rgb2yuvscale2p     =  0;
    HI_S16 gdm_rgb2yuv_0_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_1_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_2_in_dc    =  0;
    HI_S16 gdm_rgb2yuv_0_out_dc   =  0;
    HI_S16 gdm_rgb2yuv_1_out_dc   =  0;
    HI_S16 gdm_rgb2yuv_2_out_dc   =  0;
    HI_U16 gdm_rgb2yuv_clip_min   =  0;
    HI_U16 gdm_rgb2yuv_clip_max   =  0;

    HI_U32 gdm_dither_round_unlim =  0;
    HI_U32 gdm_dither_round       =  0;
    HI_U32 gdm_dither_domain_mode =  0;
    HI_U32 gdm_dither_tap_mode    =  0;
    HI_U32 gdm_dither_sed_y0      =  0;
    HI_U32 gdm_dither_sed_u0      =  0;
    HI_U32 gdm_dither_sed_v0      =  0;
    HI_U32 gdm_dither_sed_w0      =  0;
    HI_U32 gdm_dither_sed_y1      =  0;
    HI_U32 gdm_dither_sed_u1      =  0;
    HI_U32 gdm_dither_sed_v1      =  0;
    HI_U32 gdm_dither_sed_w1      =  0;
    HI_U32 gdm_dither_sed_y2      =  0;
    HI_U32 gdm_dither_sed_u2      =  0;
    HI_U32 gdm_dither_sed_v2      =  0;
    HI_U32 gdm_dither_sed_w2      =  0;
    HI_U32 gdm_dither_sed_y3      =  0;
    HI_U32 gdm_dither_sed_u3      =  0;
    HI_U32 gdm_dither_sed_v3      =  0;
    HI_U32 gdm_dither_sed_w3      =  0;
    HI_U32 gdm_dither_thr_max     =  0;
    HI_U32 gdm_dither_thr_min     =  0;


    gdm_tmap_in_bdp   = pstGDmCfg->vdm_tmap_in_bdp ;
    gdm_gmm_en        = pstGDmCfg->vdm_gmm_en      ;
    gdm_tmap_en       = pstGDmCfg->vdm_tmap_en     ;
    gdm_degmm_en      = pstGDmCfg->vdm_degmm_en    ;
    gdm_en            = pstGDmCfg->vdm_en          ;
    gdm_rgb2yuv_en    = pstGDmCfg->vdm_rgb2yuv_en  ;
    gdm_csc_en        = pstGDmCfg->vdm_csc_en      ;
    gdm_dither_en     = pstGDmCfg->vdm_dither_en   ;
    gdm_dither_mode   = pstGDmCfg->vdm_dither_mode ;

    gdm_luma_coef0_tmap   = pstGDmCfg->u16M3LumaCal[0] ;
    gdm_luma_coef1_tmap   = pstGDmCfg->u16M3LumaCal[1] ;
    gdm_luma_coef2_tmap   = pstGDmCfg->u16M3LumaCal[2] ;
    gdm_tmap_luma_scale   = pstGDmCfg->u16ScaleLumaCal ;
    gdm_tmap_coef_scale   = pstGDmCfg->u16ScaleCoefTM  ;
    gdm_tmap_out_clip_min = pstGDmCfg->u16ClipMinTM    ;
    gdm_tmap_out_clip_max = pstGDmCfg->u16ClipMaxTM    ;

    g_degmm_x1_step   = pstGDmCfg->DePQPara.gmm_x1_step;
    g_degmm_x2_step   = pstGDmCfg->DePQPara.gmm_x2_step;
    g_degmm_x3_step   = pstGDmCfg->DePQPara.gmm_x3_step;
    g_degmm_x4_step   = pstGDmCfg->DePQPara.gmm_x4_step;
    g_degmm_x1_pos    = pstGDmCfg->DePQPara.gmm_x1_pos ;
    g_degmm_x2_pos    = pstGDmCfg->DePQPara.gmm_x2_pos ;
    g_degmm_x3_pos    = pstGDmCfg->DePQPara.gmm_x3_pos ;
    g_degmm_x4_pos    = pstGDmCfg->DePQPara.gmm_x4_pos ;
    g_degmm_x1_num    = pstGDmCfg->DePQPara.gmm_x1_num ;
    g_degmm_x2_num    = pstGDmCfg->DePQPara.gmm_x2_num ;
    g_degmm_x3_num    = pstGDmCfg->DePQPara.gmm_x3_num ;
    g_degmm_x4_num    = pstGDmCfg->DePQPara.gmm_x4_num ;


    g_tmap_x1_step     = pstGDmCfg->TMPara.gmm_x1_step ;
    g_tmap_x2_step     = pstGDmCfg->TMPara.gmm_x2_step ;
    g_tmap_x3_step     = pstGDmCfg->TMPara.gmm_x3_step ;
    g_tmap_x4_step     = pstGDmCfg->TMPara.gmm_x4_step ;
    g_tmap_x1_pos      = pstGDmCfg->TMPara.gmm_x1_pos  ;
    g_tmap_x2_pos      = pstGDmCfg->TMPara.gmm_x2_pos  ;
    g_tmap_x3_pos      = pstGDmCfg->TMPara.gmm_x3_pos  ;
    g_tmap_x4_pos      = pstGDmCfg->TMPara.gmm_x4_pos  ;
    g_tmap_x1_num      = pstGDmCfg->TMPara.gmm_x1_num  ;
    g_tmap_x2_num      = pstGDmCfg->TMPara.gmm_x2_num  ;
    g_tmap_x3_num      = pstGDmCfg->TMPara.gmm_x3_num  ;
    g_tmap_x4_num      = pstGDmCfg->TMPara.gmm_x4_num  ;

    g_gmm_x1_step     = pstGDmCfg->GammaPara.gmm_x1_step ;
    g_gmm_x2_step     = pstGDmCfg->GammaPara.gmm_x2_step ;
    g_gmm_x3_step     = pstGDmCfg->GammaPara.gmm_x3_step ;
    g_gmm_x4_step     = pstGDmCfg->GammaPara.gmm_x4_step ;
    g_gmm_x5_step     = pstGDmCfg->GammaPara.gmm_x5_step ;
    g_gmm_x6_step     = pstGDmCfg->GammaPara.gmm_x6_step ;
    g_gmm_x7_step     = pstGDmCfg->GammaPara.gmm_x7_step ;
    g_gmm_x8_step     = pstGDmCfg->GammaPara.gmm_x8_step ;
    g_gmm_x1_pos      = pstGDmCfg->GammaPara.gmm_x1_pos  ;
    g_gmm_x2_pos      = pstGDmCfg->GammaPara.gmm_x2_pos  ;
    g_gmm_x3_pos      = pstGDmCfg->GammaPara.gmm_x3_pos  ;
    g_gmm_x4_pos      = pstGDmCfg->GammaPara.gmm_x4_pos  ;
    g_gmm_x5_pos      = pstGDmCfg->GammaPara.gmm_x5_pos  ;
    g_gmm_x6_pos      = pstGDmCfg->GammaPara.gmm_x6_pos  ;
    g_gmm_x7_pos      = pstGDmCfg->GammaPara.gmm_x7_pos  ;
    g_gmm_x8_pos      = pstGDmCfg->GammaPara.gmm_x8_pos  ;
    g_gmm_x1_num      = pstGDmCfg->GammaPara.gmm_x1_num  ;
    g_gmm_x2_num      = pstGDmCfg->GammaPara.gmm_x2_num  ;
    g_gmm_x3_num      = pstGDmCfg->GammaPara.gmm_x3_num  ;
    g_gmm_x4_num      = pstGDmCfg->GammaPara.gmm_x4_num  ;
    g_gmm_x5_num      = pstGDmCfg->GammaPara.gmm_x5_num  ;
    g_gmm_x6_num      = pstGDmCfg->GammaPara.gmm_x6_num  ;
    g_gmm_x7_num      = pstGDmCfg->GammaPara.gmm_x7_num  ;
    g_gmm_x8_num      = pstGDmCfg->GammaPara.gmm_x8_num  ;

    gdm_coef00_csc    = pstGDmCfg->s16M33Src2Disp[0][0] ;
    gdm_coef01_csc    = pstGDmCfg->s16M33Src2Disp[0][1] ;
    gdm_coef02_csc    = pstGDmCfg->s16M33Src2Disp[0][2] ;
    gdm_coef10_csc    = pstGDmCfg->s16M33Src2Disp[1][0] ;
    gdm_coef11_csc    = pstGDmCfg->s16M33Src2Disp[1][1] ;
    gdm_coef12_csc    = pstGDmCfg->s16M33Src2Disp[1][2] ;
    gdm_coef20_csc    = pstGDmCfg->s16M33Src2Disp[2][0] ;
    gdm_coef21_csc    = pstGDmCfg->s16M33Src2Disp[2][1] ;
    gdm_coef22_csc    = pstGDmCfg->s16M33Src2Disp[2][2] ;
    gdm_csc_scale     = pstGDmCfg->u16ScaleSrc2Disp     ;
    gdm_csc_clip_min  = pstGDmCfg->u32ClipMinCSC        ;
    gdm_csc_clip_max  = pstGDmCfg->u32ClipMaxCSC        ;

    gdm_rgb2yuvscale2p     = pstGDmCfg->u16ScaleRGB2YUV         ;
    gdm_rgb2yuv_0_in_dc    = pstGDmCfg->s16InOffsetRGB2YUV[0]   ;
    gdm_rgb2yuv_1_in_dc    = pstGDmCfg->s16InOffsetRGB2YUV[1]   ;
    gdm_rgb2yuv_2_in_dc    = pstGDmCfg->s16InOffsetRGB2YUV[2]   ;
    gdm_rgb2yuv_0_out_dc   = pstGDmCfg->s16OutOffsetRGB2YUV[0]  ;
    gdm_rgb2yuv_1_out_dc   = pstGDmCfg->s16OutOffsetRGB2YUV[1]  ;
    gdm_rgb2yuv_2_out_dc   = pstGDmCfg->s16OutOffsetRGB2YUV[2]  ;
    gdm_rgb2yuv_clip_min   = pstGDmCfg->u16ClipMinRGB2YUV       ;
    gdm_rgb2yuv_clip_max   = pstGDmCfg->u16ClipMaxRGB2YUV       ;

    gdm_00_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[0][0];
    gdm_01_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[0][1];
    gdm_02_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[0][2];
    gdm_10_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[1][0];
    gdm_11_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[1][1];
    gdm_12_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[1][2];
    gdm_20_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[2][0];
    gdm_21_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[2][1];
    gdm_22_rgb2yuv         = pstGDmCfg->s16M33RGB2YUV[2][2];

    gdm_dither_round       = pstGDmCfg->stDitherCfg.u32_dither_round       ;
    gdm_dither_domain_mode = pstGDmCfg->stDitherCfg.u32_dither_domain_mode ;
    gdm_dither_tap_mode    = pstGDmCfg->stDitherCfg.u32_dither_tap_mode    ;
    gdm_dither_round_unlim = pstGDmCfg->stDitherCfg.u32_dither_round_unlim ;
    gdm_dither_sed_y0      = pstGDmCfg->stDitherCfg.u32_dither_sed_y0      ;
    gdm_dither_sed_u0      = pstGDmCfg->stDitherCfg.u32_dither_sed_u0      ;
    gdm_dither_sed_v0      = pstGDmCfg->stDitherCfg.u32_dither_sed_v0      ;
    gdm_dither_sed_w0      = pstGDmCfg->stDitherCfg.u32_dither_sed_w0      ;
    gdm_dither_sed_y1      = pstGDmCfg->stDitherCfg.u32_dither_sed_y1      ;
    gdm_dither_sed_u1      = pstGDmCfg->stDitherCfg.u32_dither_sed_u1      ;
    gdm_dither_sed_v1      = pstGDmCfg->stDitherCfg.u32_dither_sed_v1      ;
    gdm_dither_sed_w1      = pstGDmCfg->stDitherCfg.u32_dither_sed_w1      ;
    gdm_dither_thr_max     = pstGDmCfg->stDitherCfg.u32_dither_thr_max     ;
    gdm_dither_thr_min     = pstGDmCfg->stDitherCfg.u32_dither_thr_min     ;


    //drv transfer
    PQ_HAL_SetGdmGdmTmapInBdp(enLayer, gdm_tmap_in_bdp);
    PQ_HAL_SetGdmGdmGmmEn(enLayer,     gdm_gmm_en);
    PQ_HAL_SetGdmGdmTmapEn(enLayer,    gdm_tmap_en);
    PQ_HAL_SetGdmGdmDegmmEn(enLayer,   gdm_degmm_en);
    PQ_HAL_SetGdmGdmEn(enLayer,        gdm_en);
    PQ_HAL_SetGdmGdmLumaCoef0Tmap(enLayer,  gdm_luma_coef0_tmap);
    PQ_HAL_SetGdmGdmLumaCoef1Tmap(enLayer,  gdm_luma_coef1_tmap);
    PQ_HAL_SetGdmGdmLumaCoef2Tmap(enLayer,  gdm_luma_coef2_tmap);
    PQ_HAL_SetGdmGdmTmapLumaScale(enLayer,  gdm_tmap_luma_scale);
    PQ_HAL_SetGdmGdmTmapCoefScale(enLayer,  gdm_tmap_coef_scale);
    PQ_HAL_SetGdmGdmTmapOutClipMin(enLayer, gdm_tmap_out_clip_min);
    PQ_HAL_SetGdmGdmTmapOutClipMax(enLayer, gdm_tmap_out_clip_max);

    //PQ_HAL_SetGdmGdmParaGmmAddr(enLayer,  gdm_para_gmm_addr);
    //PQ_HAL_SetGdmGdmParaTmapAddr(enLayer, gdm_para_tmap_addr);
    //PQ_HAL_SetGdmGdmParaTmapUpd(enLayer,  gdm_para_tmap_upd);
    //PQ_HAL_SetGdmGdmParaGmmUpd(enLayer,   gdm_para_gmm_upd);

    //add_regs
    PQ_HAL_SetGdmGGmmX5Num(enLayer,  g_gmm_x5_num);
    PQ_HAL_SetGdmGGmmX6Num(enLayer,  g_gmm_x6_num);
    PQ_HAL_SetGdmGGmmX7Num(enLayer,  g_gmm_x7_num);
    PQ_HAL_SetGdmGGmmX8Num(enLayer,  g_gmm_x8_num);
    PQ_HAL_SetGdmGGmmX7Pos(enLayer,  g_gmm_x7_pos);
    PQ_HAL_SetGdmGGmmX8Pos(enLayer,  g_gmm_x8_pos);
    PQ_HAL_SetGdmGGmmX5Pos(enLayer,  g_gmm_x5_pos);
    PQ_HAL_SetGdmGGmmX6Pos(enLayer,  g_gmm_x6_pos);
    PQ_HAL_SetGdmGGmmX5Step(enLayer, g_gmm_x5_step);
    PQ_HAL_SetGdmGGmmX6Step(enLayer, g_gmm_x6_step);
    PQ_HAL_SetGdmGGmmX7Step(enLayer, g_gmm_x7_step);
    PQ_HAL_SetGdmGGmmX8Step(enLayer, g_gmm_x8_step);

    PQ_HAL_SetGdmGDegmmX4Step(enLayer, g_degmm_x4_step);
    PQ_HAL_SetGdmGDegmmX3Step(enLayer, g_degmm_x3_step);
    PQ_HAL_SetGdmGDegmmX2Step(enLayer, g_degmm_x2_step);
    PQ_HAL_SetGdmGDegmmX1Step(enLayer, g_degmm_x1_step);
    PQ_HAL_SetGdmGDegmmX2Pos(enLayer,  g_degmm_x2_pos);
    PQ_HAL_SetGdmGDegmmX1Pos(enLayer,  g_degmm_x1_pos);
    PQ_HAL_SetGdmGDegmmX4Pos(enLayer,  g_degmm_x4_pos);
    PQ_HAL_SetGdmGDegmmX3Pos(enLayer,  g_degmm_x3_pos);
    PQ_HAL_SetGdmGDegmmX4Num(enLayer,  g_degmm_x4_num);
    PQ_HAL_SetGdmGDegmmX3Num(enLayer,  g_degmm_x3_num);
    PQ_HAL_SetGdmGDegmmX2Num(enLayer,  g_degmm_x2_num);
    PQ_HAL_SetGdmGDegmmX1Num(enLayer,  g_degmm_x1_num);
    PQ_HAL_SetGdmGdmTmapRdEn(enLayer,  gdm_tmap_rd_en);

    PQ_HAL_SetGdmGdmTmapParaData(enLayer, gdm_tmap_para_data);

    PQ_HAL_SetGdmGTmapX4Step(enLayer, g_tmap_x4_step);
    PQ_HAL_SetGdmGTmapX3Step(enLayer, g_tmap_x3_step);
    PQ_HAL_SetGdmGTmapX2Step(enLayer, g_tmap_x2_step);
    PQ_HAL_SetGdmGTmapX1Step(enLayer, g_tmap_x1_step);
    PQ_HAL_SetGdmGTmapX1Pos(enLayer,  g_tmap_x1_pos);
    PQ_HAL_SetGdmGTmapX2Pos(enLayer,  g_tmap_x2_pos);
    PQ_HAL_SetGdmGTmapX4Num(enLayer,  g_tmap_x4_num);
    PQ_HAL_SetGdmGTmapX3Num(enLayer,  g_tmap_x3_num);
    PQ_HAL_SetGdmGTmapX2Num(enLayer,  g_tmap_x2_num);
    PQ_HAL_SetGdmGTmapX1Num(enLayer,  g_tmap_x1_num);
    PQ_HAL_SetGdmGTmapX3Pos(enLayer,  g_tmap_x3_pos);
    PQ_HAL_SetGdmGTmapX4Pos(enLayer,  g_tmap_x4_pos);

    PQ_HAL_SetGdmGdmGammaRdEn(enLayer, gdm_gamma_rd_en);
    PQ_HAL_SetGdmGdmGammaParaData(enLayer, gdm_gamma_para_data);

    PQ_HAL_SetGdmGGmmX4Step(enLayer, g_gmm_x4_step);
    PQ_HAL_SetGdmGGmmX3Step(enLayer, g_gmm_x3_step);
    PQ_HAL_SetGdmGGmmX2Step(enLayer, g_gmm_x2_step);
    PQ_HAL_SetGdmGGmmX1Step(enLayer, g_gmm_x1_step);
    PQ_HAL_SetGdmGGmmX2Pos(enLayer,  g_gmm_x2_pos);
    PQ_HAL_SetGdmGGmmX1Pos(enLayer,  g_gmm_x1_pos);
    PQ_HAL_SetGdmGGmmX4Pos(enLayer,  g_gmm_x4_pos);
    PQ_HAL_SetGdmGGmmX3Pos(enLayer,  g_gmm_x3_pos);
    PQ_HAL_SetGdmGGmmX4Num(enLayer,  g_gmm_x4_num);
    PQ_HAL_SetGdmGGmmX3Num(enLayer,  g_gmm_x3_num);
    PQ_HAL_SetGdmGGmmX2Num(enLayer,  g_gmm_x2_num);
    PQ_HAL_SetGdmGGmmX1Num(enLayer,  g_gmm_x1_num);

    PQ_HAL_SetGdmGdmCscEn(enLayer,     gdm_csc_en);
    PQ_HAL_SetGdmGdmCoef00Csc(enLayer, gdm_coef00_csc);
    PQ_HAL_SetGdmGdmCoef01Csc(enLayer, gdm_coef01_csc);
    PQ_HAL_SetGdmGdmCoef02Csc(enLayer, gdm_coef02_csc);
    PQ_HAL_SetGdmGdmCoef10Csc(enLayer, gdm_coef10_csc);
    PQ_HAL_SetGdmGdmCoef11Csc(enLayer, gdm_coef11_csc);
    PQ_HAL_SetGdmGdmCoef12Csc(enLayer, gdm_coef12_csc);
    PQ_HAL_SetGdmGdmCoef20Csc(enLayer, gdm_coef20_csc);
    PQ_HAL_SetGdmGdmCoef21Csc(enLayer, gdm_coef21_csc);
    PQ_HAL_SetGdmGdmCoef22Csc(enLayer, gdm_coef22_csc);

    PQ_HAL_SetGdmGdmCscScale(enLayer,   gdm_csc_scale);
    PQ_HAL_SetGdmGdmCscClipMin(enLayer, gdm_csc_clip_min);
    PQ_HAL_SetGdmGdmCscClipMax(enLayer, gdm_csc_clip_max);

    PQ_HAL_SetGdmGdmRgb2yuvEn(enLayer, gdm_rgb2yuv_en);
    PQ_HAL_SetGdmGdm00Rgb2yuv(enLayer, gdm_00_rgb2yuv);
    PQ_HAL_SetGdmGdm01Rgb2yuv(enLayer, gdm_01_rgb2yuv);
    PQ_HAL_SetGdmGdm02Rgb2yuv(enLayer, gdm_02_rgb2yuv);
    PQ_HAL_SetGdmGdm10Rgb2yuv(enLayer, gdm_10_rgb2yuv);
    PQ_HAL_SetGdmGdm11Rgb2yuv(enLayer, gdm_11_rgb2yuv);
    PQ_HAL_SetGdmGdm12Rgb2yuv(enLayer, gdm_12_rgb2yuv);
    PQ_HAL_SetGdmGdm20Rgb2yuv(enLayer, gdm_20_rgb2yuv);
    PQ_HAL_SetGdmGdm21Rgb2yuv(enLayer, gdm_21_rgb2yuv);
    PQ_HAL_SetGdmGdm22Rgb2yuv(enLayer, gdm_22_rgb2yuv);

    PQ_HAL_SetGdmGdmRgb2yuvscale2p(enLayer, gdm_rgb2yuvscale2p);
    PQ_HAL_SetGdmGdmRgb2yuv0InDc(enLayer,   gdm_rgb2yuv_0_in_dc);
    PQ_HAL_SetGdmGdmRgb2yuv1InDc(enLayer,   gdm_rgb2yuv_1_in_dc);
    PQ_HAL_SetGdmGdmRgb2yuv2InDc(enLayer,   gdm_rgb2yuv_2_in_dc);
    PQ_HAL_SetGdmGdmRgb2yuv0OutDc(enLayer,  gdm_rgb2yuv_0_out_dc);
    PQ_HAL_SetGdmGdmRgb2yuv1OutDc(enLayer,  gdm_rgb2yuv_1_out_dc);
    PQ_HAL_SetGdmGdmRgb2yuv2OutDc(enLayer,  gdm_rgb2yuv_2_out_dc);
    PQ_HAL_SetGdmGdmRgb2yuvClipMin(enLayer, gdm_rgb2yuv_clip_min);
    PQ_HAL_SetGdmGdmRgb2yuvClipMax(enLayer, gdm_rgb2yuv_clip_max);

    PQ_HAL_SetGdmGdmDitherRoundUnlim(enLayer, gdm_dither_round_unlim);
    PQ_HAL_SetGdmGdmDitherEn(enLayer,         gdm_dither_en);
    PQ_HAL_SetGdmGdmDitherMode(enLayer,       gdm_dither_mode);
    PQ_HAL_SetGdmGdmDitherRound(enLayer,      gdm_dither_round);
    PQ_HAL_SetGdmGdmDitherDomainMode(enLayer, gdm_dither_domain_mode);
    PQ_HAL_SetGdmGdmDitherTapMode(enLayer,    gdm_dither_tap_mode);
    PQ_HAL_SetGdmGdmDitherSedY0(enLayer,  gdm_dither_sed_y0);
    PQ_HAL_SetGdmGdmDitherSedU0(enLayer,  gdm_dither_sed_u0);
    PQ_HAL_SetGdmGdmDitherSedV0(enLayer,  gdm_dither_sed_v0);
    PQ_HAL_SetGdmGdmDitherSedW0(enLayer,  gdm_dither_sed_w0);
    PQ_HAL_SetGdmGdmDitherSedY1(enLayer,  gdm_dither_sed_y1);
    PQ_HAL_SetGdmGdmDitherSedU1(enLayer,  gdm_dither_sed_u1);
    PQ_HAL_SetGdmGdmDitherSedV1(enLayer,  gdm_dither_sed_v1);
    PQ_HAL_SetGdmGdmDitherSedW1(enLayer,  gdm_dither_sed_w1);
    PQ_HAL_SetGdmGdmDitherSedY2(enLayer,  gdm_dither_sed_y2);
    PQ_HAL_SetGdmGdmDitherSedU2(enLayer,  gdm_dither_sed_u2);
    PQ_HAL_SetGdmGdmDitherSedV2(enLayer,  gdm_dither_sed_v2);
    PQ_HAL_SetGdmGdmDitherSedW2(enLayer,  gdm_dither_sed_w2);
    PQ_HAL_SetGdmGdmDitherSedY3(enLayer,  gdm_dither_sed_y3);
    PQ_HAL_SetGdmGdmDitherSedU3(enLayer,  gdm_dither_sed_u3);
    PQ_HAL_SetGdmGdmDitherSedV3(enLayer,  gdm_dither_sed_v3);
    PQ_HAL_SetGdmGdmDitherSedW3(enLayer,  gdm_dither_sed_w3);
    PQ_HAL_SetGdmGdmDitherThrMax(enLayer, gdm_dither_thr_max);
    PQ_HAL_SetGdmGdmDitherThrMin(enLayer, gdm_dither_thr_min);

    return;
}


/* 图形只存在RGB_709 -->YUV_2020的转换; RGB_709 --> YUV_2020 = RGB_709-->RGB_2020-->YUV_2020 */
HI_VOID PQ_HAL_SetGfxBt2020Para(HI_VOID)
{
    PQ_HAL_GDMCFG_S stGDmCfg;

    stGDmCfg.u16ClipMinTM           = 0     ;
    stGDmCfg.u16ClipMaxTM           = 65535 ;
    stGDmCfg.u32ClipMinCSC          = 0     ;
    stGDmCfg.u32ClipMaxCSC          = 65535 ; //16bits
    stGDmCfg.u16ClipMinRGB2YUV      = 0     ;
    stGDmCfg.u16ClipMaxRGB2YUV      = 1023  ;

    //typ mode
    stGDmCfg.vdm_en                 = 1;
    stGDmCfg.vdm_degmm_en           = 1;
    stGDmCfg.vdm_csc_en             = 1;
    stGDmCfg.vdm_tmap_en            = 0;
    stGDmCfg.vdm_gmm_en             = 1;
    stGDmCfg.vdm_dither_en          = 1;
    stGDmCfg.vdm_rgb2yuv_en         = 1;
    stGDmCfg.vdm_tmap_in_bdp        = 0; /* 1:20bit for vdm, 0:16bit for gdm */
    stGDmCfg.vdm_dither_mode        = 0; //12bit->10bit;
    stGDmCfg.gdm_r2y_premult_en     = 0;

    stGDmCfg.u16M3LumaCal[0]        = 1076  ;
    stGDmCfg.u16M3LumaCal[1]        = 2777  ;
    stGDmCfg.u16M3LumaCal[2]        = 243   ;
    stGDmCfg.u16ScaleLumaCal        = 12    ;
    stGDmCfg.u16ScaleCoefTM         = 8     ;

    stGDmCfg.DePQPara.gmm_x1_step   = 4     ;
    stGDmCfg.DePQPara.gmm_x2_step   = 4     ;
    stGDmCfg.DePQPara.gmm_x3_step   = 4     ;
    stGDmCfg.DePQPara.gmm_x4_step   = 5     ;
    stGDmCfg.DePQPara.gmm_x1_pos    = 672   ;
    stGDmCfg.DePQPara.gmm_x2_pos    = 832   ;
    stGDmCfg.DePQPara.gmm_x3_pos    = 992   ;
    stGDmCfg.DePQPara.gmm_x4_pos    = 1023  ;
    stGDmCfg.DePQPara.gmm_x1_num    = 42    ;
    stGDmCfg.DePQPara.gmm_x2_num    = 10    ;
    stGDmCfg.DePQPara.gmm_x3_num    = 10    ;
    stGDmCfg.DePQPara.gmm_x4_num    = 1     ;

    stGDmCfg.TMPara.gmm_x1_step     = 9     ;
    stGDmCfg.TMPara.gmm_x2_step     = 10    ;
    stGDmCfg.TMPara.gmm_x3_step     = 12    ;
    stGDmCfg.TMPara.gmm_x4_step     = 13    ;
    stGDmCfg.TMPara.gmm_x1_pos      = 8192  ;
    stGDmCfg.TMPara.gmm_x2_pos      = 16384 ;
    stGDmCfg.TMPara.gmm_x3_pos      = 24576 ;
    stGDmCfg.TMPara.gmm_x4_pos      = 65535 ;
    stGDmCfg.TMPara.gmm_x1_num      = 16    ;
    stGDmCfg.TMPara.gmm_x2_num      = 8     ;
    stGDmCfg.TMPara.gmm_x3_num      = 2     ;
    stGDmCfg.TMPara.gmm_x4_num      = 5     ;

    stGDmCfg.GammaPara.gmm_x1_step  = 0     ;
    stGDmCfg.GammaPara.gmm_x2_step  = 2     ;
    stGDmCfg.GammaPara.gmm_x3_step  = 4     ;
    stGDmCfg.GammaPara.gmm_x4_step  = 5     ;
    stGDmCfg.GammaPara.gmm_x5_step  = 6     ;
    stGDmCfg.GammaPara.gmm_x6_step  = 8     ;
    stGDmCfg.GammaPara.gmm_x7_step  = 10    ;
    stGDmCfg.GammaPara.gmm_x8_step  = 12    ;
    stGDmCfg.GammaPara.gmm_x1_pos   = 4     ;
    stGDmCfg.GammaPara.gmm_x2_pos   = 32    ;
    stGDmCfg.GammaPara.gmm_x3_pos   = 64    ;
    stGDmCfg.GammaPara.gmm_x4_pos   = 192   ;
    stGDmCfg.GammaPara.gmm_x5_pos   = 512   ;
    stGDmCfg.GammaPara.gmm_x6_pos   = 4096  ;
    stGDmCfg.GammaPara.gmm_x7_pos   = 20480 ;
    stGDmCfg.GammaPara.gmm_x8_pos   = 65535 ;
    stGDmCfg.GammaPara.gmm_x1_num   = 4     ;
    stGDmCfg.GammaPara.gmm_x2_num   = 11    ;
    stGDmCfg.GammaPara.gmm_x3_num   = 13    ;
    stGDmCfg.GammaPara.gmm_x4_num   = 17    ;
    stGDmCfg.GammaPara.gmm_x5_num   = 22    ;
    stGDmCfg.GammaPara.gmm_x6_num   = 36    ;
    stGDmCfg.GammaPara.gmm_x7_num   = 52    ;
    stGDmCfg.GammaPara.gmm_x8_num   = 63    ;


    stGDmCfg.s16M33Src2Disp[0][0]   = 27209 ;
    stGDmCfg.s16M33Src2Disp[0][1]   = -9632 ;
    stGDmCfg.s16M33Src2Disp[0][2]   = -1194 ;
    stGDmCfg.s16M33Src2Disp[1][0]   = -2045 ;
    stGDmCfg.s16M33Src2Disp[1][1]   = 18565 ;
    stGDmCfg.s16M33Src2Disp[1][2]   = -138  ;
    stGDmCfg.s16M33Src2Disp[2][0]   = -297  ;
    stGDmCfg.s16M33Src2Disp[2][1]   = -1648 ;
    stGDmCfg.s16M33Src2Disp[2][2]   = 18330 ;
    stGDmCfg.u16ScaleSrc2Disp       = 14    ;

    stGDmCfg.s16M33RGB2YUV[0][0]    = 2984  ;
    stGDmCfg.s16M33RGB2YUV[0][1]    = 10034 ;
    stGDmCfg.s16M33RGB2YUV[0][2]    = 1013  ;
    stGDmCfg.s16M33RGB2YUV[1][0]    = -1643 ;
    stGDmCfg.s16M33RGB2YUV[1][1]    = -5531 ;
    stGDmCfg.s16M33RGB2YUV[1][2]    = 7175  ;
    stGDmCfg.s16M33RGB2YUV[2][0]    = 7175  ;
    stGDmCfg.s16M33RGB2YUV[2][1]    = -6518 ;
    stGDmCfg.s16M33RGB2YUV[2][2]    = -659  ;
    stGDmCfg.u16ScaleRGB2YUV        = 14    ;
    stGDmCfg.s16InOffsetRGB2YUV[0]  = 0     ;
    stGDmCfg.s16InOffsetRGB2YUV[1]  = 0     ;
    stGDmCfg.s16InOffsetRGB2YUV[2]  = 0     ;
    stGDmCfg.s16OutOffsetRGB2YUV[0] = 64    ;
    stGDmCfg.s16OutOffsetRGB2YUV[1] = 512   ;
    stGDmCfg.s16OutOffsetRGB2YUV[2] = 512   ;

    //dither cfg
    stGDmCfg.stDitherCfg.u32_dither_round        = 1 ;// 0:dither, 1:round
    stGDmCfg.stDitherCfg.u32_dither_domain_mode  = 0 ;// 1
    stGDmCfg.stDitherCfg.u32_dither_tap_mode     = 0 ;
    stGDmCfg.stDitherCfg.u32_dither_round_unlim  = 0 ;
    stGDmCfg.stDitherCfg.u32_dither_thr_max      = 65535;
    stGDmCfg.stDitherCfg.u32_dither_thr_min      = 0 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_y0       = 2147483647 ;//31'h7fffffff;
    stGDmCfg.stDitherCfg.u32_dither_sed_u0       = 2147483647 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_v0       = 2147483647 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_w0       = 2147483647 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_y1       = 2140000000 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_u1       = 2140000000 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_v1       = 2140000000 ;
    stGDmCfg.stDitherCfg.u32_dither_sed_w1       = 2140000000 ;

    PQ_HAL_SetGdmPara(PQ_HAL_LAYER_VID0, &stGDmCfg);

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

    if (pstData128->depth > 128)
    {
        HI_ERR_PQ("Error! PQ_HAL_FUNC_Push128 depth out of range\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
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

    /* data type convertion */
    addr      = addr_base;
    cycle_num = stCfg->cycle_num;

    for (ii = 0; ii < stCfg->lut_num; ii++)
    {
        total_bit_len = total_bit_len + stCfg->coef_bit_length[ii];
    }

    /* send data */
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


HI_VOID PQ_HAL_SetGdmCoef_Gmma(HI_VOID)
{
    HI_U8 *addr;

    PQ_COEF_SEND_CFG  stCoefSend;

    HI_S32  tcLut_s32_1[64];
    HI_VOID *p_coef_array[]   = {tcLut_s32_1};
    HI_U32  lut_length[]      = {64};
    HI_U32  coef_bit_length[] = {12};

    FILE   *fp_hihdr_gdm_gamma_coef = HI_NULL;

    addr = sg_stGfxGamBuf.pu8StartVirAddr;
    PQ_HAL_SetGdmGdmParaGmmAddr(HAL_GFX_GP0, sg_stGfxGamBuf.u32StartPhyAddr);

    p_coef_array[0] = (HI_S32 *)au32GdmPQLut;

    stCoefSend.coef_addr       = addr                   ;
    stCoefSend.sti_type        = STI_FILE_COEF_DM       ;
    stCoefSend.fp_coef         = fp_hihdr_gdm_gamma_coef;
    stCoefSend.lut_num         = 1                      ;
    stCoefSend.burst_num       = 1                      ;
    stCoefSend.cycle_num       = 10                     ;
    stCoefSend.p_coef_array    = p_coef_array           ;
    stCoefSend.lut_length      = lut_length             ;
    stCoefSend.coef_bit_length = coef_bit_length        ;
    stCoefSend.data_type       = COEF_DATA_TYPE_S32     ;

    PQ_HAL_FUNC_SendCoef(&stCoefSend);
    PQ_HAL_SetGdmGdmParaGmmUpd(HAL_GFX_GP0, 1);

    return;
}

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
    PQ_HAL_SetGdmCoef_Gmma();
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

HI_VOID PQ_HAL_SetGfxBt2020Mode(PQ_HAL_GFX_CSC_MODE_S *pstPqCscMode)
{
    PQ_HAL_SetGfxBt2020Para();

    PQ_HAL_SetGdmCoef_Gmma();

    return;
}



