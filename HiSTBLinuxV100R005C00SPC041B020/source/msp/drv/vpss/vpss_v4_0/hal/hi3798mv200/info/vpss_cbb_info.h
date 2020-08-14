#ifndef __VPSS_CBB_INFO_H__
#define __VPSS_CBB_INFO_H__

#include "vpss_cbb_common.h"

#if 0
typedef struct
{
    HI_U32 u32VirAddr;
    HI_U32 u32VirAddr;
    HI_U32 u32VirAddr;
    HI_U32 u32VirAddr;
}CBB_INFO_ADDR;

typedef struct
{
    CBB_INFO_ADDR stPrjH[3];
    CBB_INFO_ADDR stPrjV[3];
    HI_U32 u32Stride;
    HI_BOOL bMMU;
    HI_BOOL bSecure;
}CBB_INFO_PRJ;
#endif

typedef struct
{
    HI_U32 u32WPhyAddr;
    HI_U8* pu8WVirAddr;
    HI_U32 u32RPhyAddr;
    HI_U8* pu8RVirAddr;
    HI_U32 u32ByteSize;
    HI_U32 u32Stride;
	HI_BOOL bSmmu;			//底层根据mmu标志 设置通道的mmu bypass属性
	HI_BOOL bSecure;
}CBB_INFO_S;

typedef struct
{
	//HI_U32		u32Tnr_cfg_addr;
	//HI_U32		u32Tnr_clut_addr;
    CBB_INFO_S stBaseInfo;
}CBB_INFO_TNR_S;

typedef struct
{
	//HI_U32		u32Snr_cfg_addr;
    CBB_INFO_S stBaseInfo;
}CBB_INFO_SNR_S;


typedef struct
{
	HI_U32 u32CurReadAddr;  //Cur读地址
	HI_U32 u32Nx2WriteAddr; //Nx2写地址
	HI_U32 u32Stride;       //运动指数行间距
	HI_BOOL bSmmu;
	HI_BOOL bSecure;
}CBB_INFO_PRJV_S;

typedef struct
{
	HI_U32 u32CurReadAddr;  //Cur读地址 
	HI_U32 u32Nx2WriteAddr; //Nx2写地址 
	HI_U32 u32Stride;       //运动指数行间距
	HI_BOOL bSmmu;
	HI_BOOL bSecure;
}CBB_INFO_PRJH_S;

typedef struct
{
	HI_U32 u32RefReadAddr;  //Ref读地址 
	HI_U32 u32CurReadAddr;  //Cur读地址 
	HI_U32 u32Nx1ReadAddr;  //Nx1读地址 
	HI_U32 u32Nx2WriteAddr; //Nx2写地址 
    HI_U32 u32Stride;       //运动指数行间距
	HI_BOOL bSmmu;
	HI_BOOL bSecure;
}CBB_INFO_BLKMV_S;

typedef struct
{
	HI_U32 u32CurReadAddr;  //Cur读地址 
	HI_U32 u32Nx1ReadAddr;  //Nx1读地址 
	HI_U32 u32Nx2WriteAddr; //Nx2写地址 
	HI_U8 *pu8CurReadAddr;  //Cur读地址 
	HI_U8 *pu8Nx1ReadAddr;  //Nx1读地址 
	HI_U8 *pu8Nx2WriteAddr; //Nx2写地址 
    HI_U32 u32Stride;       //运动指数行间距
	HI_BOOL bSmmu;
	HI_BOOL bSecure;
}CBB_INFO_RGMV_S;

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern HI_VOID VPSS_InfoCfg_Blkmv(CBB_INFO_BLKMV_S *pstInfoBlkmvCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Dei_ST(CBB_INFO_S *pstInfoDeiCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Prjh(CBB_INFO_PRJH_S *pstInfoPrjhCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Prjv(CBB_INFO_PRJV_S *pstInfoPrjvCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Rgmv(CBB_INFO_RGMV_S *pstInfoRgmvCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Snr_Mad(CBB_INFO_S *pstInfoSnrCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_INFOCfg_SttInfo(CBB_INFO_S *pstInfoStt, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoCfg_Tnr_Mad(CBB_INFO_S *pstInfoTnrCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_InfoGet_All(S_STT_REGS_TYPE *pstSttReg, CBB_INFO_S *pstInfoStt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif
