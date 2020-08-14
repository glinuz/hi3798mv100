#ifndef __VPSS_CBB_BASE_H__
#define __VPSS_CBB_BASE_H__

#include "vpss_common.h"
#include "vpss_cbb_common.h"

typedef enum 
{
    CBB_BASEOPT_START_LOGIC,
    CBB_BASEOPT_GET_ISR_STAT,
    CBB_BASEOPT_CLR_ISR_STAT,
    CBB_BASEOPT_SET_ISR_MASK,
    CBB_BASEOPT_GET_ISR_MASK,
    CBB_BASEOPT_CONFIG_AXI,
    CBB_BASEOPT_CONFIG_CLOCK,
    CBB_BASEOPT_BUTT
}CBB_BASEOPT_E;


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern HI_VOID VPSS_BaseOpt_ClrInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_CreateListandStart(CBB_START_CFG_S *pstStartCfg, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_GetMaskInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_GetRawInt(HI_BOOL *pbIsrState, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_SetIntMask(HI_BOOL *pbIsrMask, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_SetOutStd(CBB_OUTSTANDING_S *pOutStd, CBB_REG_ADDR_S stRegAddr);
extern HI_VOID VPSS_BaseOpt_SetBurstLength(HI_U32 u32BurstLength, CBB_REG_ADDR_S stRegAddr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif