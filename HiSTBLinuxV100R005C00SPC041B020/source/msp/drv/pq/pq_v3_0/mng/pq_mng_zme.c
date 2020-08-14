#include "hi_drv_mmz.h"
#include "pq_mng_zme.h"


/* VDP ZME COMMON SET: fixed to false; interspersed format */
#define SRC_COSITED  0

/* Filter and Mid Flag */
static HI_BOOL g_bVdpZmeFirEn  = HI_TRUE;
static HI_BOOL g_bVdpZmeMedEn  = HI_FALSE;
static HI_BOOL g_bVpssZmeFirEn = HI_TRUE;
static HI_BOOL g_bVpssZmeMedEn = HI_FALSE;

static HI_BOOL g_bZmeInitFlag         = HI_FALSE;
static HI_BOOL g_bVdpZmeInitFlag      = HI_FALSE;
static HI_BOOL g_bVpssZmeInitFlag     = HI_FALSE;
static ZME_TUN_MODE_E sg_enZMETunMode = ZME_TUN_NORMAL;


ALG_VZME_MEM_S g_stVZMEVdpInstance;
ALG_VZME_MEM_S g_stVZMEVpssInstance;


typedef  HI_VOID (*FN_Zme_CoefCalculate)(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara);
typedef  HI_VOID (*FN_Zme_VdpRegCfg)(HI_U32 u32LayerId, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL bFirEnable);
typedef  HI_VOID (*FN_Zme_VpssRegCfg)(HI_U32 u32LayerId, S_CAS_REGS_TYPE* pstReg, ALG_VZME_RTL_PARA_S* pstZmeRtlPara, HI_BOOL  bFirEnable);

typedef struct
{
    FN_Zme_CoefCalculate   pfnZme_CoefCalculate;
    FN_Zme_VdpRegCfg       pfnZme_VdpRegCfg;
} ZME_VDP_FUNC_S;

typedef struct
{
    FN_Zme_CoefCalculate   pfnZme_CoefCalculate;
    FN_Zme_VpssRegCfg      pfnZme_VpssRegCfg;
} ZME_VPSS_FUNC_S;

typedef struct hiPQ_MMZ_BUF_S
{
    HI_U8* pu8StartVirAddr;
    HI_U32 u32StartPhyAddr;
    HI_U32 u32Size;
} PQ_MMZ_BUF_S;

HI_S32 PQ_MNG_SetZmeTunMode(ZME_TUN_MODE_E enZMETunMode)
{
    sg_enZMETunMode = enZMETunMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetZmeTunMode(ZME_TUN_MODE_E* enZMETunMode)
{
    *enZMETunMode = sg_enZMETunMode;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetZmeDefault(HI_BOOL bOnOff)
{
    if (ZME_TUN_NORMAL == sg_enZMETunMode)
    {
        PQ_HAL_SetZmeDefault(bOnOff);
    }
    else
    {
        PQ_HAL_SetZmeDefault(HI_FALSE);/* When Debug rwzb no using */
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetVdpZmeFirEn(HI_BOOL bOnOff)
{
    g_bVdpZmeFirEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetVdpZmeFirEn(HI_BOOL* bOnOff)
{
    *bOnOff = g_bVdpZmeFirEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetVdpZmeMedEn(HI_BOOL bOnOff)
{
    g_bVdpZmeMedEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetVdpZmeMedEn(HI_BOOL* bOnOff)
{
    *bOnOff = g_bVdpZmeMedEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetVpssZmeFirEn(HI_BOOL bOnOff)
{
    g_bVpssZmeFirEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetVpssZmeFirEn(HI_BOOL* bOnOff)
{
    *bOnOff = g_bVpssZmeFirEn;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_SetVpssZmeMedEn(HI_BOOL bOnOff)
{
    g_bVpssZmeMedEn = bOnOff;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_GetVpssZmeMedEn(HI_BOOL* bOnOff)
{
    *bOnOff = g_bVpssZmeMedEn;

    return HI_SUCCESS;
}

/* common zme coefficient memory intial; get a static address pointer;
   several zme modules can use one memory block to save coefficient.
   */
static HI_S32 PQ_MNG_VdpZmeComnInit(ALG_VZME_MEM_S* pstVZmeVdpCoefMem)
{
    /* load zme coefficient into the memory */
    PQ_HAL_VdpLoadCoef(pstVZmeVdpCoefMem);

    /* Filter Mode and Median Filter */
    PQ_MNG_SetVdpZmeFirEn(HI_TRUE);
    PQ_MNG_SetVdpZmeMedEn(HI_FALSE);

    return HI_SUCCESS;
}

static HI_VOID PQ_MNG_VdpZmeComnDeInit(ALG_VZME_MEM_S* pstVZmeVdpCoefMem)
{
    return;
}

/* common zme coefficient memory intial; get a static address pointer;
   several zme modules can use one memory block to save coefficient
   */
static HI_S32 PQ_MNG_VpssZmeComnInit(ALG_VZME_MEM_S* pstVZmeVpssCoefMem)
{
    /* load zme coefficient into the memory */
    PQ_HAL_VpssLoadCoef((HI_U32*)(pstVZmeVpssCoefMem->stMBuf.u32StartVirAddr),
                        pstVZmeVpssCoefMem->stMBuf.u32StartPhyAddr, &(pstVZmeVpssCoefMem->stZmeCoefAddr));

    /* Filter Mode and Median Filter */
    PQ_MNG_SetVpssZmeFirEn(HI_TRUE);
    PQ_MNG_SetVpssZmeMedEn(HI_FALSE);

    return HI_SUCCESS;
}

static HI_VOID PQ_MNG_VpssZmeComnDeInit(ALG_VZME_MEM_S* pstVZmeVpssCoefMem)
{
    return;
}

static HI_S32 GetVerticalScalerOffset(HI_U32 u32VRatio, ALG_VZME_DRV_PARA_S* pstZmeDrvPara)
{
    HI_S32 s32VOffset = 0;

    if (pstZmeDrvPara->bZmeFrmFmtIn) /* processive input */
    {
        if (pstZmeDrvPara->bZmeFrmFmtOut) /* processive output */
        {
            s32VOffset = 0;
        }
        else /* interlaced output */
        {
            if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
            {
                s32VOffset = (HI_S32)u32VRatio >> 1;
            }
            else /*top field output */
            {
                s32VOffset = 0;
            }
        }
    }
    else /* interlaced input */
    {
        if (pstZmeDrvPara->bZmeFrmFmtOut) /* processive output */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input */
            {
                s32VOffset = -(ALG_V_VZME_PRECISION >> 1); /* -2048 */
            }
            else /* top field input */
            {
                s32VOffset = 0;
            }
        }
        else /* interlaced output */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input */
            {
                if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
                {
                    s32VOffset = ((HI_S32)u32VRatio - ALG_V_VZME_PRECISION) >> 1;
                }
                else /* top field output */
                {
                    s32VOffset = -(ALG_V_VZME_PRECISION >> 1); /* -2048 */
                }
            }
            else /* top field input */
            {
                if (pstZmeDrvPara->bZmeBFOut) /* bottom field output */
                {
                    s32VOffset = (HI_S32)u32VRatio >> 1;
                }
                else /* top field output */
                {
                    s32VOffset = 0;
                }
            }
        }
    }

    return s32VOffset;
}

/* NOTICE:
   Vdp video zme module is the same as graphic layer G0/G1;
   software don't need separate the progressive frome interlace of the output, hardware process it.
   */
HI_VOID PQ_MNG_VdpZmeComnSet(ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    HI_U32 u32ZmeCWIn, u32ZmeCWOut, u32ZmeCHIn, u32ZmeCHOut;
    HI_S32 s32VOffset, s32HOffset;
    HI_U32 u32ZmeRatioVLReal, u32ZmeRatioVCReal;

    /* config zme enable */
    pstZmeRtlPara->bZmeEnHL = HI_TRUE;
    pstZmeRtlPara->bZmeEnHC = HI_TRUE;
    pstZmeRtlPara->bZmeEnVL = HI_TRUE;
    pstZmeRtlPara->bZmeEnVC = HI_TRUE;

    /* config zme median filter enable */
    pstZmeRtlPara->bZmeMedHL = g_bVdpZmeMedEn; /* HI_FALSE */
    pstZmeRtlPara->bZmeMedHC = g_bVdpZmeMedEn;
    pstZmeRtlPara->bZmeMedVL = g_bVdpZmeMedEn;
    pstZmeRtlPara->bZmeMedVC = g_bVdpZmeMedEn;

    /* calculate luma real zme resolution of input and output */
    pstZmeRtlPara->u32ZmeWIn  = pstZmeDrvPara->u32ZmeFrmWIn;
    pstZmeRtlPara->u32ZmeWOut = pstZmeDrvPara->u32ZmeFrmWOut;
    pstZmeRtlPara->u32ZmeHIn  = pstZmeDrvPara->u32ZmeFrmHIn;
    pstZmeRtlPara->u32ZmeHOut = pstZmeDrvPara->u32ZmeFrmHOut;
    //pstZmeRtlPara->u32ZmeHIn  = ( pstZmeDrvPara->bZmeFrmFmtIn  == 1 ) ? pstZmeDrvPara->u32ZmeFrmHIn: pstZmeDrvPara->u32ZmeFrmHIn/2;
    //pstZmeRtlPara->u32ZmeHOut = ( pstZmeDrvPara->bZmeFrmFmtOut == 1 ) ? pstZmeDrvPara->u32ZmeFrmHOut: pstZmeDrvPara->u32ZmeFrmHOut/2;

    /* when wide > 4096, overflow will occurs */
    if (pstZmeRtlPara->u32ZmeWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHL = (pstZmeRtlPara->u32ZmeWIn / 2 * ALG_V_HZME_PRECISION) / pstZmeRtlPara->u32ZmeWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHL = pstZmeRtlPara->u32ZmeWIn * ALG_V_HZME_PRECISION / pstZmeRtlPara->u32ZmeWOut;
    }
    pstZmeRtlPara->u32ZmeRatioVL = pstZmeRtlPara->u32ZmeHIn * ALG_V_VZME_PRECISION / pstZmeRtlPara->u32ZmeHOut;

    /* calculate chroma zme ratio */
    u32ZmeCWIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == VDP_PROC_FMT_SP_444 ) ? pstZmeRtlPara->u32ZmeWIn : pstZmeRtlPara->u32ZmeWIn / 2;
    u32ZmeCWOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == VDP_PROC_FMT_SP_444 ) ? pstZmeRtlPara->u32ZmeWOut : pstZmeRtlPara->u32ZmeWOut / 2;
    u32ZmeCHIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == VDP_PROC_FMT_SP_420 ) ? pstZmeRtlPara->u32ZmeHIn / 2 : pstZmeRtlPara->u32ZmeHIn;
    u32ZmeCHOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == VDP_PROC_FMT_SP_420 ) ? pstZmeRtlPara->u32ZmeHOut / 2 : pstZmeRtlPara->u32ZmeHOut;

    /* when wide > 4096, overflow will occurs */
    if (u32ZmeCWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHC = (u32ZmeCWIn / 2 * ALG_V_HZME_PRECISION) / u32ZmeCWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHC = u32ZmeCWIn * ALG_V_HZME_PRECISION / u32ZmeCWOut;
    }
    pstZmeRtlPara->u32ZmeRatioVC = u32ZmeCHIn * ALG_V_VZME_PRECISION / u32ZmeCHOut;

    /* Input Progressive and Output Interlace */
    if (1 == pstZmeDrvPara->bZmeFrmFmtIn && 0 == pstZmeDrvPara->bZmeFrmFmtOut)
    {
        u32ZmeRatioVLReal = pstZmeRtlPara->u32ZmeRatioVL * 2;
        u32ZmeRatioVCReal = pstZmeRtlPara->u32ZmeRatioVC * 2;
    }
    else
    {
        u32ZmeRatioVLReal = pstZmeRtlPara->u32ZmeRatioVL;
        u32ZmeRatioVCReal = pstZmeRtlPara->u32ZmeRatioVC;
    }

    /* calculate luma zme offset */
    pstZmeRtlPara->s32ZmeOffsetHL = 0;

    /* Set Top filelds calc ZmeoffsetVL */
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    /* mv410 need distinguish read top or bottom from vdp add 20150626 */
    pstZmeDrvPara->bZmeBFOut = 0; /* output top field */
#endif
    pstZmeRtlPara->s32ZmeOffsetVL = GetVerticalScalerOffset(u32ZmeRatioVLReal, pstZmeDrvPara);

    /* calculate chroma zme offset */
    s32VOffset = 0, s32HOffset = 0;
    if (1 == pstZmeDrvPara->u8ZmeYCFmtIn) /* 4:2:0 */
    {
        /* horizontal offset for cosited or interspersed format *//* fixed to false; interspersed format */
#if SRC_COSITED
        s32HOffset = -(ALG_V_HZME_PRECISION >> 1); /* -2048 */
#else
        s32HOffset = 0;
#endif
        /* vertical offset for processive or interlaced input format */
        if (pstZmeDrvPara->bZmeFrmFmtIn) /* processive input for zme */
        {
            s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; /* -1024 >> 1 */
        }
        else /* interlaced input for zme */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input for zme */
            {
                s32VOffset = -((3 * ALG_V_VZME_PRECISION) >> 2) >> 1; /* -3072 >> 1 */
            }
            else /* top field input for zme */
            {
                s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; /* -1024 >> 1 */
            }
        }
    }

    pstZmeRtlPara->s32ZmeOffsetHC = s32HOffset;
    pstZmeRtlPara->s32ZmeOffsetVC = s32VOffset + GetVerticalScalerOffset(u32ZmeRatioVCReal, pstZmeDrvPara);

    /* Set Bottom filelds calc s32ZmeOffsetVLBtm */ /* Calc Offset At one Function */
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    /* mv410 need distinguish read top or bottom from vdp add 20150626 */
    pstZmeDrvPara->bZmeBFOut = 1; /* output bottom field */
#endif
    pstZmeRtlPara->s32ZmeOffsetVLBtm = GetVerticalScalerOffset(u32ZmeRatioVLReal, pstZmeDrvPara);
    pstZmeRtlPara->s32ZmeOffsetVCBtm = s32VOffset + GetVerticalScalerOffset(u32ZmeRatioVCReal, pstZmeDrvPara);

    /* config zme mode */
    pstZmeRtlPara->bZmeMdHL = (ALG_V_HZME_PRECISION == pstZmeRtlPara->u32ZmeRatioHL) ? HI_FALSE : HI_TRUE;
    pstZmeRtlPara->bZmeMdHC = (ALG_V_HZME_PRECISION == pstZmeRtlPara->u32ZmeRatioHC) ? HI_FALSE : HI_TRUE;
    pstZmeRtlPara->bZmeMdHL = (pstZmeRtlPara->bZmeMdHL) && g_bVdpZmeFirEn;
    pstZmeRtlPara->bZmeMdHC = (pstZmeRtlPara->bZmeMdHC) && g_bVdpZmeFirEn;

    /* when 1:1 vertical scaler, directly copy */
    if (ALG_V_VZME_PRECISION == pstZmeRtlPara->u32ZmeRatioVL && 0 == pstZmeRtlPara->s32ZmeOffsetVL)
    {
        pstZmeRtlPara->bZmeMdVL = HI_FALSE;
    }
    else
    {
        pstZmeRtlPara->bZmeMdVL = HI_TRUE && g_bVdpZmeFirEn;
    }
    /* when 1:1 vertical scaler, directly copy */
    if (ALG_V_VZME_PRECISION == pstZmeRtlPara->u32ZmeRatioVC && 0 == pstZmeRtlPara->s32ZmeOffsetVC)
    {
        pstZmeRtlPara->bZmeMdVC = HI_FALSE;
    }
    else
    {
        pstZmeRtlPara->bZmeMdVC = HI_TRUE && g_bVdpZmeFirEn;
    }

    if (HI_FALSE == pstZmeDrvPara->bDispProgressive && 0 == pstZmeDrvPara->u32Fidelity) /* interlace output and no rwzb */
    {
        if ( (pstZmeDrvPara->stOriRect.s32Width == pstZmeDrvPara->u32ZmeFrmWOut
              && pstZmeDrvPara->stOriRect.s32Height == pstZmeDrvPara->u32ZmeFrmHOut
              && pstZmeDrvPara->u32InRate != pstZmeDrvPara->u32OutRate)
             || (pstZmeDrvPara->stOriRect.s32Height > pstZmeDrvPara->u32ZmeFrmHOut) )
        {
            pstZmeRtlPara->bZmeMdHL = HI_TRUE && g_bVdpZmeFirEn;
            pstZmeRtlPara->bZmeMdHC = HI_TRUE && g_bVdpZmeFirEn;
            pstZmeRtlPara->bZmeMdVL = HI_TRUE && g_bVdpZmeFirEn;
            pstZmeRtlPara->bZmeMdVC = HI_TRUE && g_bVdpZmeFirEn;
        }
        else
        {}
    }
    else
    {}

    /* config vertical chroma zme tap and zme order */
    pstZmeRtlPara->bZmeTapVC = 0;
    pstZmeRtlPara->bZmeOrder = (pstZmeRtlPara->u32ZmeRatioHL >= ALG_V_HZME_PRECISION) ? 0 : 1;

    pstZmeRtlPara->u8ZmeYCFmtIn  = pstZmeDrvPara->u8ZmeYCFmtIn;
    pstZmeRtlPara->u8ZmeYCFmtOut = pstZmeDrvPara->u8ZmeYCFmtOut;

    return;
}

HI_S32 PQ_MNG_VpssZmeComnSet(ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    /* for TQE bSrcCosited is fixed to false */
#if 0
    HI_BOOL bSrcCosited = HI_FALSE;
#endif
    HI_U32 u32ZmeCWIn, u32ZmeCWOut, u32ZmeCHIn, u32ZmeCHOut;
    HI_S32 s32VOffset, s32HOffset;

    /* config zme enable */
    pstZmeRtlPara->bZmeEnHL = HI_TRUE;
    pstZmeRtlPara->bZmeEnHC = HI_TRUE;
    pstZmeRtlPara->bZmeEnVL = HI_TRUE;
    pstZmeRtlPara->bZmeEnVC = HI_TRUE;

    /* config zme median filter enable */
    pstZmeRtlPara->bZmeMedHL = g_bVpssZmeMedEn; /* HI_FALSE */
    pstZmeRtlPara->bZmeMedHC = g_bVpssZmeMedEn;
    pstZmeRtlPara->bZmeMedVL = g_bVpssZmeMedEn;
    pstZmeRtlPara->bZmeMedVC = g_bVpssZmeMedEn;

    /* calculate luma real zme resolution of input and output */
    pstZmeRtlPara->u32ZmeWIn  = pstZmeDrvPara->u32ZmeFrmWIn;
    pstZmeRtlPara->u32ZmeWOut = pstZmeDrvPara->u32ZmeFrmWOut;
    pstZmeRtlPara->u32ZmeHIn  = ( pstZmeDrvPara->bZmeFrmFmtIn  == 1 ) ? pstZmeDrvPara->u32ZmeFrmHIn : pstZmeDrvPara->u32ZmeFrmHIn / 2;
    pstZmeRtlPara->u32ZmeHOut = ( pstZmeDrvPara->bZmeFrmFmtOut == 1 ) ? pstZmeDrvPara->u32ZmeFrmHOut : pstZmeDrvPara->u32ZmeFrmHOut / 2;

    /* when wide > 4096, overflow will occurs */
    if (pstZmeRtlPara->u32ZmeWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHL = (pstZmeRtlPara->u32ZmeWIn / 2 * ALG_V_HZME_PRECISION) / pstZmeRtlPara->u32ZmeWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHL = pstZmeRtlPara->u32ZmeWIn * ALG_V_HZME_PRECISION / pstZmeRtlPara->u32ZmeWOut;
    }
    pstZmeRtlPara->u32ZmeRatioVL = pstZmeRtlPara->u32ZmeHIn * ALG_V_VZME_PRECISION / pstZmeRtlPara->u32ZmeHOut;

    /* calculate chroma zme ratio */
    u32ZmeCWIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == PQ_ALG_ZME_PIX_FORMAT_SP444) ? pstZmeRtlPara->u32ZmeWIn : pstZmeRtlPara->u32ZmeWIn / 2;
    u32ZmeCWOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == PQ_ALG_ZME_PIX_FORMAT_SP444) ? pstZmeRtlPara->u32ZmeWOut : pstZmeRtlPara->u32ZmeWOut / 2;
    u32ZmeCHIn  = ( pstZmeDrvPara->u8ZmeYCFmtIn  == PQ_ALG_ZME_PIX_FORMAT_SP420) ? pstZmeRtlPara->u32ZmeHIn / 2 : pstZmeRtlPara->u32ZmeHIn;
    u32ZmeCHOut = ( pstZmeDrvPara->u8ZmeYCFmtOut == PQ_ALG_ZME_PIX_FORMAT_SP420) ? pstZmeRtlPara->u32ZmeHOut / 2 : pstZmeRtlPara->u32ZmeHOut;

    /* when wide > 4096, overflow will occurs */
    if (pstZmeRtlPara->u32ZmeWIn >= 4096)
    {
        pstZmeRtlPara->u32ZmeRatioHC = (u32ZmeCWIn / 2 * ALG_V_HZME_PRECISION) / u32ZmeCWOut * 2;
    }
    else
    {
        pstZmeRtlPara->u32ZmeRatioHC = u32ZmeCWIn * ALG_V_HZME_PRECISION / u32ZmeCWOut;
    }

    pstZmeRtlPara->u32ZmeRatioVC = u32ZmeCHIn * ALG_V_VZME_PRECISION / u32ZmeCHOut;

    /* calculate luma zme offset */
    pstZmeRtlPara->s32ZmeOffsetHL = 0;
    pstZmeRtlPara->s32ZmeOffsetVL = GetVerticalScalerOffset(pstZmeRtlPara->u32ZmeRatioVL, pstZmeDrvPara);

    /* calculate chroma zme offset */
    s32VOffset = 0, s32HOffset = 0;
    /* it may influence rwzb,chrome linhang chuanrao */
#if 0
    if (pstZmeDrvPara->eZmeYCFmtIn == HI_DRV_PIX_FMT_NV21) /* 4:2:0 */
    {
        /* horizontal offset for cosited or interspersed format *//* for TQE bSrcCosited is fixed to false */
#if 0
        if (bSrcCosited) /* fixed to false*/ /* interspersed format */
        {
            s32HOffset = -(ALG_V_HZME_PRECISION >> 1); //-2048;
        }
        else
        {
            s32HOffset = 0;
        }
#else
        s32HOffset = 0;
#endif
        /* vertical offset for processive or interlaced input format */
        if (pstZmeDrvPara->bZmeFrmFmtIn) /* processive input for zme */
        {
            s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; //-1024 >> 1;
        }
        else /* interlaced input for zme */
        {
            if (pstZmeDrvPara->bZmeBFIn) /* bottom field input for zme */
            {
                s32VOffset = -((3 * ALG_V_VZME_PRECISION) >> 2) >> 1; //-3072 >> 1;
            }
            else /* top field input for zme */
            {
                s32VOffset = -(ALG_V_VZME_PRECISION >> 2) >> 1; //-1024 >> 1;
            }
        }
    }
#endif

    pstZmeRtlPara->s32ZmeOffsetHC = s32HOffset;
    pstZmeRtlPara->s32ZmeOffsetVC = s32VOffset + GetVerticalScalerOffset(pstZmeRtlPara->u32ZmeRatioVC, pstZmeDrvPara);
    /* config zme mode */
    pstZmeRtlPara->bZmeMdHL = (ALG_V_HZME_PRECISION == pstZmeRtlPara->u32ZmeRatioHL) ? HI_FALSE : HI_TRUE;
    pstZmeRtlPara->bZmeMdHC = (ALG_V_HZME_PRECISION == pstZmeRtlPara->u32ZmeRatioHC) ? HI_FALSE : HI_TRUE;
    pstZmeRtlPara->bZmeMdHL = (pstZmeRtlPara->bZmeMdHL) && g_bVpssZmeFirEn; /* HI_TRUE */
    pstZmeRtlPara->bZmeMdHC = (pstZmeRtlPara->bZmeMdHC) && g_bVpssZmeFirEn;
    /* when 1:1 vertical scaler, directly copy */
    if (ALG_V_VZME_PRECISION == pstZmeRtlPara->u32ZmeRatioVL && 0 == pstZmeRtlPara->s32ZmeOffsetVL)
    {
        pstZmeRtlPara->bZmeMdVL = HI_FALSE;
    }
    else
    {
        pstZmeRtlPara->bZmeMdVL = HI_TRUE && g_bVpssZmeFirEn;
    }

    /* when 1:1 vertical scaler, directly copy */
    if (ALG_V_VZME_PRECISION == pstZmeRtlPara->u32ZmeRatioVC && 0 == pstZmeRtlPara->s32ZmeOffsetVC)
    {
        pstZmeRtlPara->bZmeMdVC = HI_FALSE;
    }
    else
    {
        pstZmeRtlPara->bZmeMdVC = HI_TRUE && g_bVpssZmeFirEn;
    }

    /* config vertical chroma zme tap and zme order */
    pstZmeRtlPara->bZmeTapVC = 0;
    pstZmeRtlPara->bZmeOrder = (pstZmeRtlPara->u32ZmeRatioHL >= ALG_V_HZME_PRECISION) ? 0 : 1;

    pstZmeRtlPara->u8ZmeYCFmtIn  = pstZmeDrvPara->u8ZmeYCFmtIn;
    pstZmeRtlPara->u8ZmeYCFmtOut = pstZmeDrvPara->u8ZmeYCFmtOut;

    return HI_SUCCESS;
}

/* Tmp No Using */
static HI_U32 GetHLCfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / u32Ratio;

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8C4_0;   }

    return u32CoefAddr;
}

/* Tmp No Using */
static HI_U32 GetVLCfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, VZME_PICKCOEF_PARA_S stPickCoef)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / (stPickCoef.u32Ratio);

    if ( (stPickCoef.u32TapL) == 6)
    {
        if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_1;   }
        else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_E1;  }
        else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_075; }
        else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_05;  }
        else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_033; }
        else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_025; }
        else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6C4_0;   }
    }
    else
    {
        if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_1;   }
        else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_E1;  }
        else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_075; }
        else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_05;  }
        else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_033; }
        else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_025; }
        else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4C4_0;   }
    }

    return u32CoefAddr;
}

/* commonly used */
static HI_U32 GetHLfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / u32Ratio;

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHL8_0;   }

    return u32CoefAddr;
}

/* commonly used */
static HI_U32 GetHCfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / u32Ratio;

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC4_0;   }

    return u32CoefAddr;
}

/* commonly used */
static HI_U32 GetVCfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / (u32Ratio);

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC4_0;   }

    return u32CoefAddr;
}

/* 98M And MV410 */
static HI_U32 GetVLfirCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / (u32Ratio);

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL4_0;   }

    return u32CoefAddr;
}

/* Add from 3798c and hiFoneB2 */
static HI_U32 GetVLFilterCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / (u32Ratio);

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVL6_0;   }

    return u32CoefAddr;
}

/* 98CV200_A */
static HI_U32 GetHCFilterCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / u32Ratio;

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrHC8_0;   }

    return u32CoefAddr;
}

/* Add from 3798c and hiFoneB2 *//* 98CV200_A */
static HI_U32 GetVCFilterCoef(ALG_VZME_COEF_ADDR_S* pstCoefAddr, HI_U32 u32Ratio)
{
    HI_U32 u32CoefAddr;
    HI_U32 s32TmpRatio = 4096 * 4096 / (u32Ratio);

    if      (s32TmpRatio > 4096 ) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_1;   }
    else if (s32TmpRatio == 4096) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_E1;  }
    else if (s32TmpRatio >= 3072) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_075; }
    else if (s32TmpRatio >= 2048) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_05;  }
    else if (s32TmpRatio >= 1365) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_033; }
    else if (s32TmpRatio >= 1024) { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_025; }
    else                          { u32CoefAddr = pstCoefAddr->u32ZmeCoefAddrVC6_0;   }

    return u32CoefAddr;
}

/* Mixed HLC84 VLC64 *//* Tmp No Using */
HI_VOID PQ_MNG_VdpZmeSQSetAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    VZME_PICKCOEF_PARA_S stPickCoef;

    memset(&stPickCoef, 0, sizeof(VZME_PICKCOEF_PARA_S));
    PQ_MNG_VdpZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );

    stPickCoef.u32Ratio = pstZmeRtlPara->u32ZmeRatioVL;
    stPickCoef.u32TapL  = 4;
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLCfirCoef( &(pstMem->stZmeCoefAddr), stPickCoef );

    return;
}

/* Separated HL8 HC4 VL4 VC4 *//* 98M And MV410 */
HI_VOID PQ_MNG_VdpZmeSQSetSptAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    PQ_MNG_VdpZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    /* Hor */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 );

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL );
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC );

    return;
}

/* add from 3716CV200 VPSS, 98MV100 is the same: in 98MV100 HScalar also use the Horizontal Coef HL8 HC4 */
/* Separated HL8 HC4 VL6 VC4 *//* 98M */
HI_VOID PQ_MNG_VpssZmeHQSetSptAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    /* fit refer to input H and V */
    PQ_MNG_VpssZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    /* Hor */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 );

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL );
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC );
    return;
}

/* add from 3798cv100 VDP *//* Separated HL8 HC8 VL6 VC6 *//* 98CV200_A */
HI_VOID PQ_MNG_VdpZmeHQSetSptAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    PQ_MNG_VdpZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    /* Hor */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 );

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL );
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC );

    return;
}

/* add from HiFoneB2 VPSS *//* Separated HL8 HC8 VL6 VC6 *//* 98CV200_A */
HI_VOID PQ_MNG_VpssZmeSetSptAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    /* fit refer to input H and V */
    PQ_MNG_VpssZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    /* Hor */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 );

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL );
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC );

    return;
}

/* Separated HL8 HC4 VL6 VC4 *//* HL8 HC4 VL6 VC4 in 3716mv410 vdp *//* 98CV200_A And MV410 */
HI_VOID PQ_MNG_VdpZmeHQSetAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    PQ_MNG_VdpZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* config zme coefficient address */
    /* Hor 20 bits precision in logic but 12 bits precision in calculation */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 );
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 );

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLFilterCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL );
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC );

    return;
}

/* add from Hi3716mv410 VPSS *//* Separated HL8 HC4 VL4 VC4 *//* MV410 */
HI_VOID PQ_MNG_VpssZmeSQSetSptAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    PQ_MNG_VpssZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);/* fit refer to input H and V */

    /* config zme coefficient address */
    /* Hor */
    pstZmeRtlPara->u32ZmeCoefAddrHL = GetHLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHL >> 8 ); /* HL8 */
    pstZmeRtlPara->u32ZmeCoefAddrHC = GetHCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioHC >> 8 ); /* HC4 */

    /* Vert */
    pstZmeRtlPara->u32ZmeCoefAddrVL = GetVLfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVL ); /* VL4 */
    pstZmeRtlPara->u32ZmeCoefAddrVC = GetVCfirCoef( &(pstMem->stZmeCoefAddr), pstZmeRtlPara->u32ZmeRatioVC ); /* VC4 */

    return;
}

/* add from HiFoneB2 SR *//* Separated HL8 HC8 VL6 VC6 */ /* 98CV200_A And 98M */
HI_VOID PQ_MNG_VdpSRZmeSetAddr(ALG_VZME_MEM_S* pstMem, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, ALG_VZME_RTL_PARA_S* pstZmeRtlPara)
{
    PQ_MNG_VdpZmeComnSet(pstZmeDrvPara, pstZmeRtlPara);

    /* Set SR zme coefficient */
    /* Hor and Vert */

    return;
}

static HI_S32 PQ_MNG_MMZ_AllocAndMap(const char* bufname, char* zone_name, HI_U32 size, int align, PQ_MMZ_BUF_S* pstMBuf)
{
    MMZ_BUFFER_S stMMZ;
    HI_S32 s32Ret;

    s32Ret = HI_DRV_MMZ_AllocAndMap(bufname, zone_name, size, align, &stMMZ);
    if (!s32Ret)
    {
		pstMBuf->u32StartPhyAddr = stMMZ.u32StartPhyAddr;
        pstMBuf->pu8StartVirAddr = stMMZ.pu8StartVirAddr;
        pstMBuf->u32Size         = stMMZ.u32Size;
    }

    return s32Ret;
}

static HI_VOID PQ_MNG_MMZ_UnmapAndRelease(PQ_MMZ_BUF_S* pstMBuf)
{
    MMZ_BUFFER_S stMMZ;

    stMMZ.u32StartPhyAddr = pstMBuf->u32StartPhyAddr;
    stMMZ.pu8StartVirAddr = pstMBuf->pu8StartVirAddr;
    stMMZ.u32Size         = pstMBuf->u32Size;

    HI_DRV_MMZ_UnmapAndRelease(&stMMZ);

    return;
}

HI_S32 PQ_MNG_InitVdpZme(HI_VOID)
{
    HI_S32 s32Ret;

    if (g_bVdpZmeInitFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = PQ_MNG_MMZ_AllocAndMap("PQ_VdpZmeCoef", HI_NULL, VDP_MMZ_SIZE, 16, (PQ_MMZ_BUF_S*)&g_stVZMEVdpInstance.stMBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("Get PQ_Zme failed\n");
        return HI_FAILURE;
    }

    /* load Vdp zme coef; Init Vdp Zme Coef */
    s32Ret = PQ_MNG_VdpZmeComnInit(&g_stVZMEVdpInstance);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_VdpZmeComnInit failed!\n");
        PQ_MNG_MMZ_UnmapAndRelease((PQ_MMZ_BUF_S*)&g_stVZMEVdpInstance.stMBuf);
        g_stVZMEVdpInstance.stMBuf.u32StartVirAddr = 0;

        return HI_FAILURE;
    }

    g_bVdpZmeInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_InitVpssZme(HI_VOID)
{
    ALG_MMZ_BUF_S* pstMBuf;
    HI_S32 s32Ret;

    if (g_bVpssZmeInitFlag)
    {
        return HI_SUCCESS;
    }

    /* apply memory for zme coefficient, and get the address */
    pstMBuf = &(g_stVZMEVpssInstance.stMBuf);

    s32Ret = PQ_MNG_MMZ_AllocAndMap("PQ_VpssZmeCoef", HI_NULL, VPSS_MMZ_SIZE, 0, (PQ_MMZ_BUF_S*)pstMBuf);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_MMZ_AllocAndMap failed!\n");
        return HI_FAILURE;
    }
    /* load Vpss zme coef; Init Vdp Zme Coef */
    s32Ret = PQ_MNG_VpssZmeComnInit(&g_stVZMEVpssInstance);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("PQ_MNG_VpssZmeComnInit failed!\n");
        PQ_MNG_MMZ_UnmapAndRelease((PQ_MMZ_BUF_S*)pstMBuf);
        //g_stVZMEVpssInstance.stMBuf.u32StartVirAddr = 0;

        return HI_FAILURE;
    }
    g_bVpssZmeInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitVdpZme(HI_VOID)
{
    if (g_bVdpZmeInitFlag)
    {
        PQ_MNG_VdpZmeComnDeInit(&g_stVZMEVdpInstance);
        /* release Vdp zme coefficient memory */
        if (0 != g_stVZMEVdpInstance.stMBuf.u32StartVirAddr)
        {
            PQ_MNG_MMZ_UnmapAndRelease((PQ_MMZ_BUF_S*)&g_stVZMEVdpInstance.stMBuf);
            g_stVZMEVdpInstance.stMBuf.u32StartVirAddr = 0;
        }
        g_bVdpZmeInitFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 PQ_MNG_DeInitVpssZme(HI_VOID)
{
    if (g_bVpssZmeInitFlag)
    {
        PQ_MNG_VpssZmeComnDeInit(&g_stVZMEVpssInstance);
        /* release Vpss zme coefficient memory */
        if (0 != g_stVZMEVpssInstance.stMBuf.u32StartVirAddr)
        {
            PQ_MNG_MMZ_UnmapAndRelease((PQ_MMZ_BUF_S*)&g_stVZMEVpssInstance.stMBuf);
            g_stVZMEVpssInstance.stMBuf.u32StartVirAddr = 0;
        }
        g_bVpssZmeInitFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_InitZme(HI_VOID)
{
    if (g_bZmeInitFlag)
    {
        return HI_SUCCESS;
    }

    (HI_VOID)PQ_MNG_InitVpssZme();
    (HI_VOID)PQ_MNG_InitVdpZme();

    g_bZmeInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 PQ_MNG_DeInitZme(HI_VOID)
{
    if (g_bZmeInitFlag)
    {
        (HI_VOID)PQ_MNG_DeInitVpssZme();
        (HI_VOID)PQ_MNG_DeInitVdpZme();

        g_bZmeInitFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

ZME_VDP_FUNC_S stZmeVdpFunc[HI_PQ_DISP_LAYER_ZME_BUTT]  =
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V0  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V1  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V2  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V3  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V4  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_WbcZmeRegCfg},   /* WBC */
    {PQ_MNG_VdpSRZmeSetAddr,    PQ_HAL_SRZmeRegCfg },   /* SR  */
#elif defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    {PQ_MNG_VdpZmeHQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V0  */
    {PQ_MNG_VdpZmeHQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V1  */
    {PQ_MNG_VdpZmeHQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V2  */
    {PQ_MNG_VdpZmeHQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V3  */
    {PQ_MNG_VdpZmeHQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V4  */
    {PQ_MNG_VdpZmeHQSetAddr,    PQ_HAL_WbcZmeRegCfg},   /* WBC */
    {PQ_MNG_VdpSRZmeSetAddr,    PQ_HAL_SRZmeRegCfg },   /* SR  */
#elif defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    {PQ_MNG_VdpZmeHQSetAddr,    PQ_HAL_VdpZmeRegCfg},   /* V0  */
    {PQ_MNG_VdpZmeHQSetAddr,    PQ_HAL_VdpZmeRegCfg},   /* V1  */
    {PQ_MNG_VdpZmeHQSetAddr,    PQ_HAL_VdpZmeRegCfg},   /* V2  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V3  */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* V4  *//* It does not exist V4 in mv410 */
    {PQ_MNG_VdpZmeSQSetSptAddr, PQ_HAL_VdpZmeRegCfg},   /* WBC *//* WBC is same to V3 */
    {HI_NULL, HI_NULL},                                 /* SR  */
#else
    {HI_NULL, HI_NULL},                                 /* V0  */
    {HI_NULL, HI_NULL},                                 /* V1  */
    {HI_NULL, HI_NULL},                                 /* V2  */
    {HI_NULL, HI_NULL},                                 /* V3  */
    {HI_NULL, HI_NULL},                                 /* V4  */
    {HI_NULL, HI_NULL},                                 /* WBC */
    {HI_NULL, HI_NULL},                                 /* SR  */
#endif
};

ZME_VPSS_FUNC_S stZmeVpssFunc[HI_PQ_VPSS_LAYER_ZME_BUTT]  =
{
#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    {PQ_MNG_VpssZmeHQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT0 */
    {PQ_MNG_VpssZmeHQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT1 */
    {PQ_MNG_VpssZmeHQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT2 */
    {PQ_MNG_VpssZmeHQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* HSCL  */
#elif defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    {PQ_MNG_VpssZmeSetSptAddr,   PQ_HAL_VpssZmeRegCfg}, /* PORT0 */
    {PQ_MNG_VpssZmeSetSptAddr,   PQ_HAL_VpssZmeRegCfg}, /* PORT1 */
    {PQ_MNG_VpssZmeSetSptAddr,   PQ_HAL_VpssZmeRegCfg}, /* PORT2 */
    {HI_NULL, HI_NULL},                                 /* HSCL  */
#elif defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    {PQ_MNG_VpssZmeSQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT0 */
    {PQ_MNG_VpssZmeSQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT0 */
    {PQ_MNG_VpssZmeSQSetSptAddr, PQ_HAL_VpssZmeRegCfg}, /* PORT0 */
    {HI_NULL, HI_NULL},                                 /* HSCL  */
#else
    {HI_NULL, HI_NULL},                                 /* PORT0 */
    {HI_NULL, HI_NULL},                                 /* PORT1 */
    {HI_NULL, HI_NULL},                                 /* PORT2 */
    {HI_NULL, HI_NULL},                                 /* HSCL  */
#endif
};

HI_VOID PQ_MNG_SetVdpZme(HI_U32 u32LayerId, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, HI_BOOL bFirEnable)
{
    ALG_VZME_RTL_PARA_S stZmeRtlParam;

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    if (HI_PQ_DISP_V3_LAYER_ZME == u32LayerId)
    {
        return;
    }
#endif
    memset((void*)&stZmeRtlParam, 0, sizeof(ALG_VZME_RTL_PARA_S));
    stZmeVdpFunc[u32LayerId].pfnZme_CoefCalculate(&g_stVZMEVdpInstance, pstZmeDrvPara, &stZmeRtlParam);

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    stZmeRtlParam.u8ZmeYCFmtIn  = pstZmeDrvPara->u8ZmeYCFmtIn;
    stZmeRtlParam.u8ZmeYCFmtOut = pstZmeDrvPara->u8ZmeYCFmtOut;

    if (HI_PQ_DISP_WBC0_LAYER_ZME == u32LayerId)
    {
        stZmeVdpFunc[u32LayerId].pfnZme_VdpRegCfg(HI_PQ_DISP_V3_LAYER_ZME, &stZmeRtlParam, bFirEnable);
    }
    else
    {
        stZmeVdpFunc[u32LayerId].pfnZme_VdpRegCfg(u32LayerId, &stZmeRtlParam, bFirEnable);
    }
#endif

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200_b)
    stZmeVdpFunc[u32LayerId].pfnZme_VdpRegCfg(u32LayerId, &stZmeRtlParam, bFirEnable);
#endif
    return;
}

HI_VOID PQ_MNG_SetVpssZme(HI_U32 u32LayerId, S_CAS_REGS_TYPE* pstReg, ALG_VZME_DRV_PARA_S* pstZmeDrvPara, HI_BOOL bFirEnable)
{
    ALG_VZME_RTL_PARA_S stZmeRtlParam;

    memset((void*)&stZmeRtlParam, 0, sizeof(ALG_VZME_RTL_PARA_S));
    stZmeVpssFunc[u32LayerId].pfnZme_CoefCalculate(&g_stVZMEVpssInstance, pstZmeDrvPara, &stZmeRtlParam);
    stZmeVpssFunc[u32LayerId].pfnZme_VpssRegCfg(u32LayerId, pstReg, &stZmeRtlParam, bFirEnable);

    return;
}

