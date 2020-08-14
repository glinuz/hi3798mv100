#include "hi_type.h"
#include "tee_pvr.h"

#ifndef __TEE_DRV_PVR_H__
#define __TEE_DRV_PVR_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define REE_PVR_MAX_INDEX_NUM       (256)
#define TEE_PVR_TS_PACKET_LEN        (188)  

/*macros below are used to process ts packet*/
/*******************begin**************************/
#define TEE_PVR_TS_PD_SIZE_POS       4            
#define TEE_PVR_TS_PD_FLAG_POS       5          
#define TEE_PVR_TS_MIN_PD_SIZE       2
#define TEE_PVR_TS_HEAD_LEN          (4)         
#define PVR_TEE_TS_ADAPT_RESERVED    0x0
#define PVR_TEE_TS_ADAPT_PLD_ONLY    0x1
#define PVR_TEE_TS_ADAPT_ADAPT_ONLY  0x2
#define PVR_TEE_TS_ADAPT_BOTH        0x3
#define PVR_TEE_TS_ADAPT_HAVE_ADAPT(flag) (flag & PVR_TEE_TS_ADAPT_ADAPT_ONLY)
#define PVR_TEE_GET_TS_HEAD_ADAPTION(pTsHead)           ((((pTsHead)[3]) >> 4) & 0x3)
#define PVR_TEE_SET_TS_HEAD_ADAPTION(pTsHead, flag)     ((((pTsHead)[3])&0xCF)|(((flag)&0x3)<<4))  
#define PVR_TEE_DISORDER_CNT_NUM(pTsHead, u8RandNum)    (((((pTsHead)[3] & 0xF) + (u8RandNum)) &0xF) | ((pTsHead)[3] & 0xF0))
/********************end*************************/

typedef struct tagTEE_DRV_PVR_TS_ADJUST_INFO_S
{
    HI_U64  u64DataGlobalOffset;/*total data received*/
    HI_U64  u64BaseGlobalOffset;/*positon of overflow*/
    HI_U64  u64NextOldOffset;/*offset of next frame from demux*/
    HI_U64  u64NextAdjustOffset;/*offset of next frame after correcting*/
    HI_S32  s32InsertTsPacketNum;/*total number of inserted ts packets*/
    HI_BOOL bUsedNext;/*flags for processing ts data*/
    HI_CHAR chTsPacket[TEE_PVR_TS_PACKET_LEN];/*one ts pacekt buffer*/
}TEE_DRV_PVR_TS_ADJUST_INFO_S;

typedef struct tagTEE_DRV_PVR_REC_STATUS_S
{
    HI_U32 u32TeeBufAddr;/*addr of tee buffer*/
    HI_U32 u32TeeBufLen;/*length of tee buffer*/
    HI_U32 u32ReeBufAddr;/*addr of ree buffer*/
    HI_U32 u32ReeBufLen;/*length of ree buffer*/
    HI_U32 u32TeeReadPos;/*read position of tee buffer*/
    HI_U32 u32TeeWritePos;/*write position of tee buffer*/
    HI_U64 u64DataGlobalOffset;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.u64DataGlobalOffset*/
    HI_U64 u64BaseGlobalOffset;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.u64BaseGlobalOffset*/
    HI_U64 u64NextOldOffset;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.u64NextOldOffset*/
    HI_U64 u64NextAdjustOffset;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.u64NextAdjustOffset*/
    HI_S32 s32InsertTsPacketNum;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.s32InsertTsPacketNum*/
    HI_BOOL bUsedNext;/*reference TEE_DRV_PVR_TS_ADJUST_INFO_S.bUsedNext*/
}TEE_DRV_PVR_REC_STATUS_S;/*statuse info for tee recording channel*/

typedef struct tagTEE_DRV_PVR_SMMU_BUF_S
{
    HI_ULONG  phyaddr;                /**<Physical address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer物理地址 */
    HI_U8  *user_viraddr;           /**<User-state virtual address of an MMZ buffer*/ /**<CNcomment:  MMZ buffer用户态虚拟地址 */
    HI_U32  bufsize;                /**<Size of an MMZ buffer*/ /**<CNcomment:  MMZ buffer大小 */
}TEE_DRV_PVR_SMMU_BUF_S;

/*ring buffer info*/
typedef struct tagTEE_DRV_PVR_BUFFER_INFO_S
{
    TEE_DRV_PVR_SMMU_BUF_S stSmmucBuf; /*addr info*/
    HI_S32 s32Length; /*length*/
    HI_S32 s32Read; /*read position*/
    HI_S32 s32Write;/*write position*/
}TEE_DRV_PVR_BUFFER_INFO_S;    

typedef struct tagTEE_DRV_PVR_REC_DATA_S
{
    HI_U8  *pDataAddr; /*virtul addr*/     
    HI_U32  u32Reserved;/*reserved. it could save phyaddr*/
    HI_U32  u32Len; /*length*/
}TEE_DRV_PVR_REC_DATA_S;  

typedef struct tagTEE_DRV_PVR_INDEX_INFO_S
{
    HI_S32 s32IndexNum;/*number of index*/
    HI_S32 s32TsDataNum; /*number of ts data buffer*/
    HI_U64 u64DropLength;/*the dropped data length while pause the recording*/
    HI_U64 u64GlobalOffset[REE_PVR_MAX_INDEX_NUM];/*offset of the frame from demux*/
    HI_U32 u32FrameSize[REE_PVR_MAX_INDEX_NUM];/*length of the frame from demux*/
    TEE_DRV_PVR_REC_DATA_S stRecData[2]; /*buffer info of ts data from demux. demux use 2, so here is also 2*/
}TEE_DRV_PVR_INDEX_INFO_S;

HI_S32 TEE_DRV_PVR_DeInit();
HI_S32 TEE_DRV_PVR_OpenChannel(HI_U32 *pu32ChnId, HI_S32 s32BufLen, TEE_PVR_TYPE_E enType);
HI_S32 TEE_DRV_PVR_CloseChannel(HI_U32 u32ChnId);
HI_S32 TEE_DRV_PVR_RecProcTsData(HI_U32 u32ChnId, HI_U32 u32NoSecureSmmuAddr);
HI_S32 TEE_DRV_PVR_RecCopyToRee(HI_U32 u32SecureSmmuAddr, HI_U32 u32NoSecureSmmuAddr, HI_U32 *pu32DataLen);
HI_S32 TEE_DRV_PVR_RecGetOutputBufInfo(HI_U32 u32ChnId, HI_U32 *pu32SecureSmmuAddr, HI_U32 *pu32DataLen);
HI_S32 TEE_DRV_PVR_RecGetStatus(HI_U32 u32ChnId, HI_U32 u32NonSecureSmmuAddr);
HI_S32 TEE_DRV_PVR_PlayCopyToTee(HI_U32 u32ReeAddr, HI_U32 u32TeeAddr, HI_U32 u32DataLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
