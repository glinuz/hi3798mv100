#ifndef __HI_PVR_TEE_H__
#define __HI_PVR_TEE_H__

#include <errno.h>
#include <stdlib.h>
#include "hi_type.h"
#include "hi_unf_common.h"
#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"
#include "mpi_mmz.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define REE_PVR_BUFFER_MAX_LEN      (64*188*1024)  
#define REE_PVR_BUFFER_MIN_LEN      (4*188*256)  
#define REE_PVR_BUFFER_ALIGN        (47*1024)
#define REE_PVR_MAX_INDEX_NUM       (256)

#define HI_TEE_ERROR_PVR_BASE               (0x81300000)
#define HI_TEE_ERROR_PVR_NOT_SUPPORT        (HI_TEE_ERROR_PVR_BASE + 1)
#define HI_TEE_ERROR_PVR_NULL_PTR           (HI_TEE_ERROR_PVR_BASE + 2)
#define HI_TEE_ERROR_PVR_NO_ENOUGH_DATA     (HI_TEE_ERROR_PVR_BASE + 3)
#define HI_TEE_ERROR_PVR_INVALID_PARAM      (HI_TEE_ERROR_PVR_BASE + 4)
#define HI_TEE_ERROR_PVR_NO_MEM             (HI_TEE_ERROR_PVR_BASE + 5)
#define HI_TEE_ERROR_PVR_INVALID_LEN        (HI_TEE_ERROR_PVR_BASE + 6)
#define HI_TEE_ERROR_PVR_LOW_BUFFER         (HI_TEE_ERROR_PVR_BASE + 7)
#define HI_TEE_ERROR_PVR_INDEX_MORE         (HI_TEE_ERROR_PVR_BASE + 8)
#define HI_TEE_ERROR_NO_CHANNEL             (HI_TEE_ERROR_PVR_BASE + 9)


typedef struct hiPVR_REE_BUFFER_INFO_S
{
    HI_U32 u32SmmuAddr;
    HI_VOID *pUserAddr;
    HI_U32 u32Size;
    HI_U32 u32Used;
}PVR_REE_BUFFER_INFO_S;

HI_S32 HI_PVR_TeeInit(HI_VOID);
HI_S32 HI_PVR_TeeDeInit(HI_VOID);
HI_S32 HI_PVR_TeeOpenTeeChn(HI_U32 *pu32TeeChnId, HI_S32 s32Length);
HI_S32 HI_PVR_TeeCloseTeeChn(HI_U32 u32TeeChnId);
HI_S32 HI_PVR_TeeAttach(HI_U32 u32TeeChnId, HI_HANDLE hDemuxHandle);
HI_S32 HI_PVR_TeeGetReeBuffInfo(PVR_REE_BUFFER_INFO_S *psteeBuf, 
                                HI_U8 **ppu8DataAddr, HI_U32 *pu32Len);
HI_S32 HI_PVR_TeeAdjustTsData(HI_MPI_DMX_REC_DATA_INDEX_S *pstDmxRecData, 
                              HI_U32 u32TeeChnId, 
                              HI_U64 u64DropLength,
                              PVR_REE_BUFFER_INFO_S *pstReeBuff);
HI_S32 HI_PVR_TeeCopyDataFromRee(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_U32 u32PhyAddr, HI_S32 s32Len);
HI_S32 HI_PVR_TeeGetTeeState(HI_U32 u32TeeChnId, PVR_REE_BUFFER_INFO_S *pstTeeState);
HI_S32 HI_PVR_TeeMallocReeBuffer(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_S32 s32Length, HI_BOOL bShare, HI_CHAR *pchName);
HI_S32 HI_PVR_TeeFreeReeBuffer(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_BOOL bShare);
HI_S32 HI_PVR_TeeGetRecTeeBuf(HI_U32 u32TeeChnId, HI_U8 **ppu8VirAddr, HI_U32 *pu32PhyAddr, HI_U32 *pu32Len);
HI_S32 HI_PVR_TeeCopyToREE_Test(PVR_REE_BUFFER_INFO_S *pstReeBuf, HI_U32 u32TeeChnId);

 

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif   //__HI_PVR_TEE_H__


