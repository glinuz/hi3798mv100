#ifndef __VPSS_CBB_ALG_H__
#define __VPSS_CBB_ALG_H__

#include "vpss_cbb_common.h"


#define VPSS_ALG_MCNR_W_MAX VPSS_HD_HEIGHT
#define VPSS_ALG_MCNR_W_MIN 128
#define VPSS_ALG_ZME_W_MAX  VPSS_FHD_WIDTH

#define VPSS_ALG_IS_SD(w, h) (((w) <= VPSS_PAL_WIDTH) && ((h) <= VPSS_PAL_HEIGHT))

#define VPSS_ALG_IS_MCNR_ENABLE(width, height) (((width) <= VPSS_ALG_MCNR_W_MAX) && ((width) >= VPSS_ALG_MCNR_W_MIN) && ((height) <= VPSS_PAL_HEIGHT))

typedef enum
{
	CBB_ALG_ZME = 0,
	CBB_ALG_DEI,
	CBB_ALG_TNR,
	CBB_ALG_TNR_MC,
	CBB_ALG_SNR,
	CBB_ALG_DB,
	CBB_ALG_DB_DET,
	CBB_ALG_DM,
	CBB_ALG_NR,
	CBB_ALG_DS,
	CBB_ALG_IGBM,
	CBB_ALG_IFMD,
	CBB_ALG_MCDEI,
	CBB_ALG_BUTT,	
}CBB_ALG_E;

typedef enum
{
	ALG_MODE_AUTO = 0,
	ALG_MODE_DEFAULT,
	ALG_MODE_ON,
	ALG_MODE_OFF,
	ALG_MODE_BUTT,
}CBB_ALG_MODE_E;

typedef struct 
{
	HI_BOOL	bEnable;
	HI_U32	u32ParaAddr;
	HI_U32	u32Ymotionmode;
} CBB_ALG_INFO_S;

typedef struct
{
    CBB_ALG_MODE_E enMode;
    HI_BOOL bHlmscEn;
    HI_BOOL bHchmscEn;
    HI_BOOL bHlmidEn;
    HI_BOOL bHchmidEn;
    HI_BOOL bHlfirEn;
    HI_BOOL bHchfirEn;
    HI_U32 u32HfirOrder;
    //HI_U32 u32Hratio;
    HI_U32 u32HorlOffset;
    HI_U32 u32HorcOffset;

    HI_BOOL bVlmscEn;
    HI_BOOL bVchmscEn;
    HI_BOOL bVlmidEn;
    HI_BOOL bVchmidEn;
    HI_BOOL bVlfirEn;
    HI_BOOL bVchfirEn;
    HI_U32 u32VfirOrder;
    //HI_U32 u32Vratio;
    HI_U32 u32VerlOffset;
    HI_U32 u32VercOffset;
}CBB_ALG_ZME_S;

/*
	使用 VPSS_REG_ResetAppReg(pu32AppVir, pstHalInfo->pstPqCfg) 的方式来配置ALG 系数
	此处仅提供算法的开关 和 基本信息的配置(如系数地址)
*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern HI_VOID VPSS_AlgCfg_BaseFunc(CBB_REG_ADDR_S stRegAddr, CBB_ALG_E eAlgId, HI_VOID *pArg1, HI_VOID *pArg2);
extern HI_VOID VPSS_AlgCfg_Zme(CBB_ALG_ZME_S *pstAlgZmeCfg, CBB_REG_ADDR_S stRegAddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
