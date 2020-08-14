#ifndef __VPSS_STTINF_H__
#define __VPSS_STTINF_H__
#include "vpss_common.h"
#include "hi_drv_vpss.h"
#include "vpss_osal.h"
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200_b)
#include "vpss_reg_stt.h"
#endif

/* DIE ST */
#define VPSS_DIE_MAX_NODE 3
typedef struct hiVPSS_DIE_DATA_S
{
    HI_U32 u32PhyAddr;  
    LIST node;                       
} VPSS_DIE_DATA_S;

typedef struct hiVPSS_DIESTINFO_S
{
    HI_BOOL bInit;                     //初始化标识    
    HI_U32 u32Cnt;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32Stride;
    HI_U32 u32DieInfoSize;
    VPSS_DIE_DATA_S stDataList[VPSS_DIE_MAX_NODE];
    MMZ_BUFFER_S stMMZBuf;
    LIST* pstFirstRef;
} VPSS_DIESTINFO_S;

typedef struct hiVPSS_DIESTCFG_S
{
    HI_U32 u32CurAddr;
    HI_U32 u32PreAddr;
    HI_U32 u32PPreAddr;
    HI_U32 u32Stride;
} VPSS_DIESTCFG_S;

HI_S32 VPSS_STTINFO_DieInit(VPSS_DIESTINFO_S* pstDieStInfo, HI_U32 u32Width, HI_U32 u32Heigth);
HI_S32 VPSS_STTINFO_DieDeInit(VPSS_DIESTINFO_S* pstDieStInfo);
HI_S32 VPSS_STTINFO_DieGetInfo(VPSS_DIESTINFO_S* pstDieStInfo, VPSS_DIESTCFG_S* pstDieStCfg);
HI_S32 VPSS_STTINFO_DieComplete(VPSS_DIESTINFO_S* pstDieStInfo);
HI_S32 VPSS_STTINFO_DieReset(VPSS_DIESTINFO_S* pstDieStInfo);


/* CCCL CNT */
#define VPSS_CCCL_MAX_NODE 3
typedef struct hiVPSS_CCCL_DATA_S
{
    HI_U32 u32YPhyAddr;  
    HI_U32 u32CPhyAddr;
    LIST node;                       
} VPSS_CCCL_DATA_S;

typedef struct hiVPSS_CCCLCNTINFO_S
{
    HI_BOOL bInit;                     //初始化标识     
    HI_U32 u32Cnt;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_U32 u32CCCLCntSize;
    HI_U32 u32ccnt_stride;
    HI_U32 u32ycnt_stride;
    VPSS_CCCL_DATA_S stDataList[VPSS_CCCL_MAX_NODE];
    MMZ_BUFFER_S stMMZBuf;
    LIST* pstFirstRef;

} VPSS_CCCLCNTINFO_S;

typedef struct hiVPSS_CCCLCNTCFG_S
{
    HI_U32 u32Ycnt_raddr;
    HI_U32 u32Ccnt_raddr;
    HI_U32 u32Ycnt_waddr;
    HI_U32 u32Ccnt_waddr;

    HI_U32 u32Ccnt_stride;
    HI_U32 u32ycnt_stride;
} VPSS_CCCLCNTCFG_S;

HI_S32 VPSS_STTINFO_CcclInit(VPSS_CCCLCNTINFO_S* pstCcclCntInfo, HI_U32 u32Width, HI_U32 u32Heigth);
HI_S32 VPSS_STTINFO_CcclDeInit(VPSS_CCCLCNTINFO_S* pstCcclCntInfo);
HI_S32 VPSS_STTINFO_CcclGetInfo(VPSS_CCCLCNTINFO_S* pstCcclCntInfo,
                               VPSS_CCCLCNTCFG_S* pstCcclCntCfg);
HI_S32 VPSS_STTINFO_CcclComplete(VPSS_CCCLCNTINFO_S* pstCcclCntInfo);
HI_S32 VPSS_STTINFO_CcclReset(VPSS_CCCLCNTINFO_S* pstCcclCntInfo);


/* NR MAD */

#define VPSS_NR_MAX_NODE 4
typedef struct hiVPSS_NR_DATA_S
{
    HI_U32 u32PhyAddr;  
    LIST node;                       
} VPSS_NR_DATA_S;

typedef enum hiVPSS_NR_MODE_E
{
    NR_MODE_FRAME  = 0,
    NR_MODE_5FIELD,
    NR_MODE_3FIELD,
    NR_MODE_BUTT
} VPSS_NR_MODE_E;


typedef struct hiVPSS_NR_ATTR_S
{
    HI_U32 u32Width;
    HI_U32 u32Height;
    VPSS_NR_MODE_E enMode;
} VPSS_NR_ATTR_S;

typedef struct hiVPSS_NRMADINFO_S
{
    HI_BOOL bInit;                     //初始化标识     
    HI_U32 u32Cnt;
    VPSS_NR_ATTR_S stAttr;
    HI_U32 u32NRMADSize;
    HI_U32 u32madstride;
    VPSS_NR_DATA_S stDataList[VPSS_NR_MAX_NODE];
    MMZ_BUFFER_S stMMZBuf;
    LIST* pstFirstRef;
} VPSS_NRMADINFO_S;

typedef struct hiVPSS_NRMADCFG_S
{
    HI_U32 u32Tnrmad_raddr;
    HI_U32 u32Tnrmad_waddr;
    HI_U32 u32Snrmad_raddr;
    HI_U32 u32madstride;
} VPSS_NRMADCFG_S;

HI_S32 VPSS_STTINFO_NrInit(VPSS_NRMADINFO_S* pstNrMadInfo, VPSS_NR_ATTR_S *pstAttr);
HI_S32 VPSS_STTINFO_NrDeInit(VPSS_NRMADINFO_S* pstNrMadInfo);
HI_S32 VPSS_STTINFO_NrGetInfo(VPSS_NRMADINFO_S* pstNrMadInfo,
                             VPSS_NRMADCFG_S* pstNrMadCfg);
HI_S32 VPSS_STTINFO_NrComplete(VPSS_NRMADINFO_S* pstNrMadInfo);
HI_S32 VPSS_STTINFO_NrReset(VPSS_NRMADINFO_S* pstNrMadInfo);

typedef struct hiVPSS_STTWBC_S
{
    HI_BOOL bInit;                     //初始化标识   
    HI_U32 u32Cnt;    
    MMZ_BUFFER_S stMMZBuf;

} VPSS_STTWBC_S;

/* STT WBC */
#define VPSS_STTWBC_SIZE (32*1024)

HI_S32 VPSS_STTINFO_SttWbcInit(VPSS_STTWBC_S* psttWbc);
HI_S32 VPSS_STTINFO_SttWbcDeInit(VPSS_STTWBC_S* psttWbc);
HI_S32 VPSS_STTINFO_SttWbcGetAddr(VPSS_STTWBC_S* psttWbc, 
                                    HI_U32* pu32stt_w_phy_addr,
                                    HI_U32* pu32stt_w_vir_addr);

HI_S32 VPSS_STTINFO_SttWbcComplete(VPSS_STTWBC_S* psttWbc);
HI_S32 VPSS_STTINFO_SttWbcReset(VPSS_STTWBC_S* psttWbc);

#endif
