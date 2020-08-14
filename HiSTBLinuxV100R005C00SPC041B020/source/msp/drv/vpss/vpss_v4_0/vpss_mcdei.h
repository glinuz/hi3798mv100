#ifndef __VPSS_MCDEI_H__
#define __VPSS_MCDEI_H__

#include "vpss_stt_mcdei.h"
#include "vpss_wbc_mcdei.h"
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#include "vpss_cbb_hal.h"
#else
#include "vpss_hal_3798cv200.h"
#endif





#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

typedef struct hiVPSS_MCDEI_INFO_S
{
    VPSS_RGME_S          stRgmeWbcInfo;//rgme回写队列
    VPSS_BLEND_S         stBlendWbcInfo;//blend回写队列 
    VPSS_ST_RGME_S       stRgmeInfo;   //rgme运动队列
    VPSS_ST_BLKMV_S      stBlkmvInfo;  //blkmv运动队列
    VPSS_ST_PRJH_S       stPrjhInfo;  //prjh运动队列
    VPSS_ST_PRJV_S       stPrjvInfo;  //prjv运动队列
    HI_BOOL              bMcEnable;   //MCDI是否使能
#ifdef VPSS_SUPPORT_ALG_MCNR
    HI_BOOL              bMcNrEnable;
#endif
} VPSS_MCDEI_INFO_S;

HI_BOOL VPSS_INST_McDeiGetMedsEn(HI_DRV_VIDEO_FRAME_S *pstSrcImage);
HI_S32 VPSS_INST_McDeiInit(VPSS_MCDEI_INFO_S *pstMcdeiInfo, HI_DRV_VIDEO_FRAME_S *pstSrcImage);
HI_S32 VPSS_INST_McDeiDeInit(VPSS_MCDEI_INFO_S *pstMcdeiInfo);
HI_S32 VPSS_INST_McDeiComplete(VPSS_MCDEI_INFO_S *pstMcdeiInfo);
HI_S32 VPSS_INST_McDeiReset(VPSS_MCDEI_INFO_S *pstMcdeiInfo);
HI_S32 VPSS_INST_SetHalMcdeiInfo(VPSS_MCDEI_INFO_S * pstMcdeiInfo,VPSS_HAL_INFO_S *pstHalInfo,
    HI_DRV_BUF_ADDR_E enLR, HI_DRV_VIDEO_FRAME_S *pstSrcImage);
#ifdef VPSS_SUPPORT_ALG_MCNR
HI_S32 VPSS_INST_SetHalMcNrInfo(VPSS_MCDEI_INFO_S * pstMcdeiInfo,VPSS_HAL_INFO_S *pstHalInfo,
    HI_DRV_BUF_ADDR_E enLR, HI_DRV_VIDEO_FRAME_S *pstSrcImage);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__VPSS_STT_MCDEI_H__*/

