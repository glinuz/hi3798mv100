/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_index.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   :
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_INDEX_H_INTER__
#define __HI_PVR_INDEX_H_INTER__

#include <sys/ioctl.h>

#include "hi_type.h"
#include "hi_pvr_priv.h"
#include "hi_pvr_fifo.h"
#include "hi_pvr_index.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define PVR_CYC_INFO_OFFSET      (2 * sizeof(HI_U32)) 

#define PVR_GET_USR_DATA_OFFSET(headInfo) (sizeof(PVR_IDX_HEADER_INFO_S) + PVR_MAX_CADATA_LEN)
#define PVR_GET_CA_DATA_OFFSET() (sizeof(PVR_IDX_HEADER_INFO_S))
#define PVR_GET_IDX_INFO_OFFSET(headInfo) (sizeof(PVR_IDX_HEADER_INFO_S) + (headInfo.u32CADataInfoLen) - sizeof(PVR_REC_INDEX_INFO_S))

#define PVR_IDX_NEXT_POS_IN_CYC(CurPos, CycLen)           ((CurPos + 1) % CycLen)
#define PVR_IDX_NEXT_N_POS_IN_CYC(CurPos, n, CycLen)      ((CurPos + n) % CycLen)
#define PVR_IDX_PREV_POS_IN_CYC(CurPos, CycLen)           ((CurPos + CycLen - 1) % CycLen)
#define PVR_IDX_PREV_N_POS_IN_CYC(CurPos, n, CycLen)      ((CurPos + CycLen - n) % CycLen)
#define PVR_IDX_DISTANCE_IN_CYC(DstPos, SrcPos, CycLen)   ((DstPos + CycLen - SrcPos) % CycLen)

#define PVR_IDX_ALIGN_DATA(data) (data + (PVR_INDEX_LEAST_COMMON_MULTIPLE - data % PVR_INDEX_LEAST_COMMON_MULTIPLE))

#define PVR_INDEX_GET_FRAMETYPE(u32Version, u16FrameTypeAndGop) \
    ((0 == (u32Version)) ? (((u16FrameTypeAndGop) >> 14) & 0x3UL) : (((u16FrameTypeAndGop) >> 13) & 0x7UL))
#define PVRINDEX_IS_CERTAINTYPE(u32Version, u16FrameTypeAndGop, type)\
    (PVR_INDEX_GET_FRAMETYPE(u32Version, u16FrameTypeAndGop) == (type))
#define PVRINDEX_GET_GLOBALOFFSET(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u64GlobalOffset : (pstEntry)->stEntry2.u64GlobalOffset)
#define PVRINDEX_GET_DISPTIME(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u32DisplayTimeMs : (pstEntry)->stEntry2.u32DisplayTimeMs)       
#define PVRINDEX_GET_FRAMESIZE(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u32FrameSize : (pstEntry)->stEntry2.u32FrameSize)
#define PVRINDEX_GET_OFFSET(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u64Offset : (pstEntry)->stEntry2.u64Offset)     
#define PVRINDEX_GET_PTS(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u32PtsMs : (pstEntry)->stEntry2.u32PtsMs)   
#define PVRINDEX_GET_INDEXTYPE(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u16IndexType : (pstEntry)->stEntry2.u16IndexType)   
#define PVRINDEX_GET_UPOVERFLOW(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u16UpFlowFlag : (pstEntry)->stEntry2.u16UpFlowFlag) 
#define PVRINDEX_GET_CYCTIMES(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.s32CycTimes : (pstEntry)->stEntry2.s32CycTimes)  
#define PVRINDEX_GET_FRAMETYPEANDGOP(u32Version, pstEntry) \
    ((0 == (u32Version)) ? (pstEntry)->stEntry1.u16FrameTypeAndGop : (pstEntry)->stEntry2.u16FrameTypeAndGop)   
#define PVRINDEX_GET_PREIOFFSET(u32Version, pstEntry) \
    (PVRINDEX_GET_FRAMETYPEANDGOP(u32Version, pstEntry) & 0x1fffUL)
#define PVRINDEX_GET_ENTRY_BYNUM(handle, pEntry, u32FrameNum, ret) \
    do{\
        if (0 == (handle)->u32EntryVersion)\
        {\
            (ret) = PVRIndex_GetEntryByNum0(handle, pEntry, u32FrameNum);\
        }\
        else\
        {\
            (ret) = PVRIndex_GetEntryByNum1(handle, pEntry, u32FrameNum);\
        }\
    }while(0)
#define PVRINDEX_SET_GLOBALOFFSET(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u64GlobalOffset = (value);\
        else\
            (pstEntry)->stEntry2.u64GlobalOffset = (value);\
    }while(0)
#define PVRINDEX_SET_DISPTIME(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u32DisplayTimeMs = (value);\
        else\
            (pstEntry)->stEntry2.u32DisplayTimeMs = (value);\
    }while(0)
#define PVRINDEX_SET_FRAMESIZE(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u32FrameSize = (value);\
        else\
            (pstEntry)->stEntry2.u32FrameSize = (value);\
    }while(0)
#define PVRINDEX_SET_OFFSET(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u64Offset = (value);\
        else\
            (pstEntry)->stEntry2.u64Offset = (value);\
    }while(0)
#define PVRINDEX_SET_PTS(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u32PtsMs = (value);\
        else\
            (pstEntry)->stEntry2.u32PtsMs = (value);\
    }while(0)
#define PVRINDEX_SET_INDEXTYPE(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u16IndexType = (value);\
        else\
            (pstEntry)->stEntry2.u16IndexType = (value);\
    }while(0)
#define PVRINDEX_SET_UPOVERFLOW(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u16UpFlowFlag = (value);\
        else\
            (pstEntry)->stEntry2.u16UpFlowFlag = (value);\
    }while(0)
#define PVRINDEX_SET_CYCTIMES(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.s32CycTimes = (value);\
        else\
            (pstEntry)->stEntry2.s32CycTimes = (value);\
    }while(0) 
#define PVRINDEX_SET_FRAMETYPEANDGOP(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u16FrameTypeAndGop = (value);\
        else\
            (pstEntry)->stEntry2.u16FrameTypeAndGop = (value);\
    }while(0) 
#define PVRINDEX_SET_LASTFRAMEOFTT(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            (pstEntry)->stEntry1.u161stFrameOfTT = (value);\
        else\
            (pstEntry)->stEntry2.u161stFrameOfTT = (value);\
    }while(0) 
#define PVRINDEX_SET_CURPOC(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            HI_ERR_PVR("This version entry not support CurPoc\n");\
        else\
            (pstEntry)->stEntry2.s32Poc = (value);\
    }while(0)   
#define PVRINDEX_SET_REFCNT(u32Version, pstEntry, value) \
    do{\
        if (0 == (u32Version))\
            HI_ERR_PVR("This version entry not support RefCnt");\
        else\
            (pstEntry)->stEntry2.u8RefCnt = (value);\
    }while(0)   
#define PVRINDEX_SET_REFINFO(u32Version, pstEntry, value, index) \
    do{\
        if (0 == (u32Version))\
            HI_ERR_PVR("This version entry not support RefInfo");\
        else\
            (pstEntry)->stEntry2.au32RefNum[index] = (value);\
    }while(0)       

#define PVR_IS_CYC_READFRAME_INVLD(start, end, read) \
(((end) > (start) && ((read) < (start) || (read) > (end)))\
     || ((end) < (start) && ((read) < (start) && (read) > (end))) )

#define PVR_IDX_ENABLE_REWIND(handle) ((handle)->stCycMgr.bIsRewind)

#define PVR_WRITE_INDEX(saveSz, wantSz, buf, fd, offset, handle) \
do{\
saveSz = PVR_WRITE(buf, wantSz, fd, (off_t)(offset + handle->u32IdxStartOffsetLen));\
if (saveSz != (ssize_t)wantSz)\
{\
    if (NULL != &errno)\
    {\
        if (ENOSPC == errno)\
        {\
            HI_ERR_PVR("PVR_WRITE fail:%d, want:%u\n", saveSz, wantSz);\
            PVR_IDX_CACHE_UNLOCK(&(handle->stIdxWriteCache.stCacheMutex));\
            return HI_ERR_PVR_FILE_DISC_FULL;\
        }\
        else\
        {\
            HI_ERR_PVR("PVR_WRITE fail:%d, want:%u\n", saveSz, wantSz);\
            PVR_IDX_CACHE_UNLOCK(&(handle->stIdxWriteCache.stCacheMutex));\
            return HI_ERR_PVR_FILE_CANT_WRITE;\
        }\
    }\
}\
}while(0)


#define PVR_READ_INDEX(readSz, pstEntryBuf, s32Num, fd, offset, handle)\
    do{\
        if (0 == handle->u32EntryVersion)\
            readSz = PVRIndex_ReadIndex0(pstEntryBuf,s32Num,fd,offset, handle);\
        else\
            readSz = PVRIndex_ReadIndex1(pstEntryBuf,s32Num,fd,offset, handle);\
    }while(0)

#define PVR_IDX_CHECK_CYC_SIZE(pstRecAttr) \
    do{\
        if ((pstRecAttr->u64MaxFileSize == 0) && (pstRecAttr->u64MaxTimeInMs == 0))\
        {\
            HI_WARN_PVR("invalidate u64MaxFileSize and u64MaxTimeInMs error!\n");\
        }\
        else if ((pstRecAttr->u64MaxFileSize > 0) && (pstRecAttr->u64MaxTimeInMs > 0))\
        {\
            if ((pstRecAttr->u64MaxFileSize < PVR_MIN_CYC_SIZE) || (pstRecAttr->u64MaxTimeInMs < PVR_MIN_CYC_TIMEMS))\
            {\
                HI_WARN_PVR("invalidate u64MaxFileSize and u64MaxTimeInMs error!\n");\
            }\
        }\
        else if ((pstRecAttr->u64MaxFileSize > 0) && (pstRecAttr->u64MaxFileSize < PVR_MIN_CYC_SIZE))\
        {\
            HI_WARN_PVR("invalidate u64MaxFileSize and u64MaxTimeInMs error!\n");\
        }\
        else if ((pstRecAttr->u64MaxTimeInMs > 0) && (pstRecAttr->u64MaxTimeInMs < PVR_MIN_CYC_TIMEMS))\
        {\
            HI_WARN_PVR("invalidate u64MaxFileSize and u64MaxTimeInMs error!\n");\
        }\
    }while(0)


#define PVR_IDX_CHECK_REWINDTYPE_BYBOTH(handle, pstIdxEntry,pstCycMgr) \
    do\
    {\
        if(PVRINDEX_GET_DISPTIME(handle->u32EntryVersion, pstIdxEntry) > pstCycMgr->u32MaxCycTimeInMs) \
        {\
            pstCycMgr->enRewindType = PVR_INDEX_REWIND_BY_TIME;\
        }\
        if (PVRINDEX_GET_GLOBALOFFSET(handle->u32EntryVersion, pstIdxEntry) > pstCycMgr->u64MaxCycSize)\
        {\
            pstCycMgr->enRewindType = PVR_INDEX_REWIND_BY_SIZE;\
        }\
    }while(0)
#define PVR_READ_INDEX_DIRECTLY(readSz,buf, size, fd,  offset, handle)\
        do{\
        if (PVR_Index_IfOffsetInWriteCache(handle,offset,size))\
        {\
            PVR_Index_FlushIdxWriteCache(handle);\
        }\
        readSz = PVR_READALL(buf, size, fd,  (off_t)(offset + handle->u32IdxStartOffsetLen));\
        }while(0)

#ifndef HI_PVR_EXTRA_BUF_SUPPORT
#if 0  /*obsolete APIs*/
HI_S32 PVRIndex_CacheWriteIdx0(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S* pstEntry, 
                        HI_S32 s32Num, HI_U32 u32Offset, HI_BOOL* bWriteFlag);
#endif
HI_S32 PVRIndex_CacheWriteIdx1(PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S* pstEntry, 
                        HI_S32 s32Num, HI_U32 u32Offset, HI_BOOL* bWriteFlag);
#endif

size_t PVRIndex_ReadIndex0(PVR_INDEX_ENTRY_NEW_S *pstEntryBuf, HI_S32 s32Num, PVR_FILE fd, 
                        HI_U64 u64Offset, PVR_INDEX_HANDLE handle);
HI_S32 PVRIndex_GetFrameInfoByHeaderInfo0(PVR_IDX_HEADER_INFO_S *pstHeaderInfo, 
                        PVR_INDEX_ENTRY_NEW_S *pstStartEntry,PVR_INDEX_ENTRY_NEW_S*pstEndEntry,HI_S32 s32FdIdx);
HI_S32 PVRIndex_GetEntryByNum0(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry, HI_U32 u32FrameNum);

size_t PVRIndex_ReadIndex1(PVR_INDEX_ENTRY_NEW_S *pstEntryBuf, HI_S32 s32Num, PVR_FILE fd, 
                        HI_U64 u64Offset, PVR_INDEX_HANDLE handle);
HI_S32 PVRIndex_GetFrameInfoByHeaderInfo1(PVR_IDX_HEADER_INFO_S *pstHeaderInfo, 
                        PVR_INDEX_ENTRY_NEW_S *pstStartEntry,PVR_INDEX_ENTRY_NEW_S*pstEndEntry,HI_S32 s32FdIdx);
HI_S32 PVRIndex_GetEntryByNum1(const PVR_INDEX_HANDLE handle, PVR_INDEX_ENTRY_NEW_S *pEntry, HI_U32 u32FrameNum);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifdef __HI_PVR_INDEX_H__ */

