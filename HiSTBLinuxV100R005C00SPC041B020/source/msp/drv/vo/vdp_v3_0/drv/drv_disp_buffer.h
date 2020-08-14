
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_buffer.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_DISP_BUFFER_H__
#define __DRV_DISP_BUFFER_H__

#include "hi_type.h"
#include "drv_disp_com.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define WIN_BUFFER_MAX_NUMBER 16
#define WIN_BUFFER_MIN_NUMBER 1
#define WIN_BUFFER_DEFAULT_NUMBER 8

#define WIN_BUFFER_PRIV_DATA_SIZE 32
#define WIN_BUFFER_USER_DATA_SIZE 8

#define WIN_BUFFER_INDEX_PREFIX  ( ( ((HI_U32)'V')<<24 )|( ((HI_U32)'O')<<16 ) )
#define WIN_BUFFER_INDEX_PREFIX_MASK  0xFFFF0000ul
#define WIN_BUFFER_INDEX_SHIFT 8
#define WIN_BUFFER_INDEX_MASK  0x000000FFul
#define WIN_BUFFER_INDEX_INVALID  0xFFFFFFFFul

#define WIN_BUFFER_MAX_W 4096
#define WIN_BUFFER_MIN_W 64
#define WIN_BUFFER_MAX_H 2160
#define WIN_BUFFER_MIN_H 64

#define DISP_CAST_LOWDELAY_MEM_LENGTH   0x1000

typedef struct
{
	HI_BOOL bFbAllocMem;
    HI_U32 u32BufWidth;
    HI_U32 u32BufHeight;
    HI_U32 u32BufStride;
    HI_U32 u32BufSize;
    HI_DRV_PIX_FORMAT_E eDataFormat;

	// user supply mem
}BUF_ALLOC_S;

typedef struct
{
    HI_U32 u32UserData[WIN_BUFFER_USER_DATA_SIZE];
}BUF_USERDATA_S;

typedef enum
{
    BUF_STATE_EMPTY = 0,
    BUF_STATE_READING,
    BUF_STATE_WRITING,
    BUF_STATE_FULL,
    BUF_STATE_DONE,  /* wait to release */
    BUF_STATE_BUTT
}BUF_STATE_E;

typedef enum
{
    BUF_MEM_SRC_SUPPLY = 0,
    BUF_MEM_FB_SUPPLY,
    BUF_MEM_USER_SUPPLY,
    BUF_MEM_TYPE_BUTT
}BUF_MEM_TYPE_E;

typedef struct
{
    HI_U32 u32Index;
    BUF_STATE_E enState;
        
    //HI_U32 u32PrivData[WIN_BUFFER_PRIV_DATA_SIZE];
    HI_DRV_VIDEO_FRAME_S stFrame;
    BUF_USERDATA_S stUserData;
    HI_U32 u32PrivData[WIN_BUFFER_PRIV_DATA_SIZE];

    DISP_MMZ_BUF_S stMem;
}BUF_S;

typedef struct
{
    /* bit[31-16]=index prefix, bit[15-8]=buffer id, bit[7-0]=0 */
    HI_U32 u32Index;
    HI_U32 u32NodeStage; /*for cfg only.*/
}BUF_ID_S;

typedef struct
{
    /* number of frame in buffer currently  */
    HI_U32 u32EmptyDel;
    HI_U32 u32EmptyAdd;
    HI_U32 u32EmptyDoing;

    HI_U32 u32CfgDel;
    HI_U32 u32CfgAdd;
    HI_U32 u32CfgDoing;
    HI_U32 u32FullDel;
    HI_U32 u32FullAdd;
    HI_U32 u32FullDoing;
}BUF_STAT_S;

typedef struct
{
    HI_U32 u32BufNum;
    
    /*maybe it should not be located here, it's just for storing the
      setting passed by user. not real allocated information.*/
    HI_U32  u32BufSize;
    HI_U32  u32BufStride;
    
    //BUF_S stBufQueue[WIN_BUFFER_MAX_NUMBER];
    BUF_S *pstBufQueue;

    //BUF_ID_S stEmptyQueue[WIN_BUFFER_MAX_NUMBER];
    BUF_ID_S *pstEmptyQueue;
    HI_U32 u32EmptyRPtr;
    HI_U32 u32EmptyWPtr;

    //BUF_ID_S stFullQueue[WIN_BUFFER_MAX_NUMBER];
    BUF_ID_S *pstFullQueue;
    HI_U32 u32FullRPtr;
    HI_U32 u32FullWPtr;

    BUF_ID_S *pstCfgWritingQueue;
    HI_U32 u32CfgWritingRPtr;
    HI_U32 u32CfgWritingWPtr;
    HI_BOOL bAllocMemory;
    BUF_ALLOC_S stAlloc;
    BUF_MEM_TYPE_E enMemType;
    BUF_STAT_S stStatistic;
}BUF_POOL_S;

HI_S32 BP_Create(HI_U32 u32BufNum, BUF_ALLOC_S *pstAlloc, BUF_POOL_S *pstBP);
HI_S32 BP_Destroy(BUF_POOL_S *pstBP);
HI_S32 BP_Reset(BUF_POOL_S *pstBP);

// produce and consume empty buffer
HI_S32 BP_GetEmptyBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId);
HI_S32 BP_GetDoneBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId);
HI_S32 BP_DelEmptyBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_S32 BP_AddEmptyBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_S32 BP_SetBufEmpty(BUF_POOL_S *pstBP, HI_U32 u32BufId);

// produce and consume full buffer
HI_S32 BP_GetFullBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId);
HI_S32 BP_DelFullBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_S32 BP_AddFullBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);


// get and set frame to buffer
HI_S32 BP_GetFrame(BUF_POOL_S *pstBP, HI_U32 u32BufId, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 BP_SetFrame(BUF_POOL_S *pstBP, HI_U32 u32BufId, HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 BP_ReAllocBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_S32 BP_ReAllocAllBuf(BUF_POOL_S *pstBP);

// get and set user data to buffer
HI_S32 BP_SetUserData(BUF_POOL_S *pstBP, HI_U32 u32BufId, BUF_USERDATA_S *pstData);
HI_S32 BP_GetUserData(BUF_POOL_S *pstBP, HI_U32 u32BufId, BUF_USERDATA_S *pstData);

HI_S32 BP_GetFullBufNum(BUF_POOL_S *pstBP, HI_U32 *pu32BufNum);
HI_S32 BP_GetEmptyBufNum(BUF_POOL_S *pstBP, HI_U32 *pu32BufNum);

HI_S32 BP_GetCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 *pu32BufId, HI_U32 u32CfgStage,HI_U32 *u32CfgStageAct);
HI_S32 BP_DelCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_VOID BP_IncreaseAllCfgWritingState(BUF_POOL_S *pstBP, HI_U32 *pu32BufId);
HI_S32 BP_GetCfgWritingBuf_JustWriting(BUF_POOL_S *pstBP, HI_U32 *pu32BufId, HI_U32 u32Stage);
HI_S32 BP_AddCfgWritingBuf(BUF_POOL_S *pstBP, HI_U32 u32BufId);
HI_S32 BP_SetBufReading(BUF_POOL_S *pstBP, HI_U32 u32BufId);

typedef struct
{
    HI_U32 u32BufNum;
    HI_U32 enMemType;
    HI_U32 enPixFmt;

    HI_U32 u32EmptyDel;
    HI_U32 u32EmptyAdd;
    HI_U32 u32EmptyDoing;

    HI_U32 u32FullDel;
    HI_U32 u32FullAdd;
    HI_U32 u32FullDoing;

    HI_U32 stBufState[WIN_BUFFER_MAX_NUMBER];

    HI_U32 stEmptyQueue[WIN_BUFFER_MAX_NUMBER];
    HI_U32 u32EmptyRPtr;
    HI_U32 u32EmptyWPtr;

    HI_U32 stFullQueue[WIN_BUFFER_MAX_NUMBER];
    HI_U32 u32FullRPtr;
    HI_U32 u32FullWPtr;

    HI_DRV_VIDEO_FRAME_S stFrame;
}BUF_STT_S;
HI_S32 BP_GetBufState(BUF_POOL_S *pstBP, BUF_STT_S *pstBufState);
HI_S32 BP_CreateBlackFrame(HI_VOID);
HI_S32 BP_DestroyBlackFrame(HI_VOID);
HI_DRV_VIDEO_FRAME_S *BP_GetBlackFrameInfo(HI_VOID);




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_VO_BUFFER_H__  */


