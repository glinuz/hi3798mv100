/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :mpi_hiao_track.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/12
  Description   :
  History       :
  1.Date        : 2012/12/12
    Author      : w0019655
    Modification: Created file

 *******************************************************************************/

#ifndef  __MPI_HIAO_VIRTUAL_H__
#define  __MPI_HIAO_VIRTUAL_H__

#include "hi_mpi_ao.h"
#include "hi_drv_ao.h"

#define VIR_MIN_OUTBUF_SIZE   (32 * 1024)
#define VIR_MAX_OUTBUF_SIZE   (512 * 1024)
#define VIR_MAX_FRAME_SIZE    (32 * 1024)   //dra multichn   1024*8*sizeof(HI_S32) = 32K

#define CHECK_VIRTUAL_Track(track)                          \
    do {                                                    \
            if(((track & AO_TRACK_CHNID_MASK) >= AO_MAX_REAL_TRACK_NUM) && ((track & AO_TRACK_CHNID_MASK) <= AO_MAX_REAL_TRACK_NUM + AO_MAX_VIRTUAL_TRACK_NUM))              \
            {                                               \
                HI_INFO_AO("Virtual Track don't support this function\n");  \
                return HI_SUCCESS;                          \
            }                                               \
         } while(0)

#define CHECK_REAL_Track(track)                          \
    do {                                                    \
            if((track & AO_TRACK_CHNID_MASK) < AO_MAX_REAL_TRACK_NUM)              \
            {                                               \
                HI_ERR_AO("Real Track don't support this function\n");  \
                return HI_FAILURE;                          \
            }                                               \
         } while(0)

#define CHECK_Track(track)                          \
    do {                                                    \
            if((track & AO_TRACK_CHNID_MASK) >= (AO_MAX_REAL_TRACK_NUM + AO_MAX_VIRTUAL_TRACK_NUM))              \
            {                                               \
                HI_ERR_AO("Virtual Track don't support this function\n");  \
                return HI_FAILURE;                          \
            }                                               \
         } while(0)

#define VIR_MAX_STORED_PTS_NUM   (2*1024)   //(512 *1024) / 320  (VIRTUAL_MAX_OUTBUF_SIZE/MIN_FRAME_SIZE)

typedef struct hiVIR_PTS_S
{
    HI_U32 u32PtsMs;        /* Play Time Stamp  */
    HI_U32 u32BegPtr;      /* Stream start address of PTS */
    HI_U32 u32EndPtr;      /* Stream end   address of PTS */
} VIR_PTS_S;

typedef struct hiVIR_PTS_QUE_S
{
    HI_U32   u32LastPtsMs;
    HI_U32   u32PTSreadIdx;     /* PTS buffer read  ptr */
    HI_U32   u32PTSwriteIdx;    /* PTS buffer write ptr */
    VIR_PTS_S stPTSArry[VIR_MAX_STORED_PTS_NUM];
} VIR_PTS_QUE_S;

typedef struct hiVIR_BUFFUR_S
{
    /*buf ptr info*/
    HI_U8 *      pu8BufBase;
    HI_U32       u32Start;
    HI_U32       u32End;
    HI_U32       u32Write;
    HI_U32       u32Read;

    /*buf data info*/
    HI_U32       u32Channel;
    HI_S32       s32BitPerSample;
    HI_U32       u32SampleRate;
    HI_U32       u32PcmSamplesPerFrame;

    VIR_PTS_QUE_S stPTSQue;
}VIR_BUFFUR_S;

typedef struct
{
    HI_UNF_AUDIOTRACK_ATTR_S stTrackAttr;
    VIR_BUFFUR_S             *pstBuf;
    HI_U32                   u32BufSize;
} VIR_TRACK_STATE_S;

HI_VOID VIR_InitRS(HI_VOID);
HI_VOID VIR_DeInitRS(HI_VOID);
HI_S32  VIR_CreateTrack(const HI_UNF_AUDIOTRACK_ATTR_S *pstTrackAttr, HI_HANDLE *phTrack);
HI_S32  VIR_DestroyTrack(HI_HANDLE hTrack);
HI_S32  VIR_GetAttr(HI_HANDLE hTrack, HI_UNF_AUDIOTRACK_ATTR_S *pstAttr);
HI_S32  VIR_SendData(HI_HANDLE hTrack, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
HI_S32  VIR_AcquireFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S *pstAOFrame);
HI_S32  VIR_ReleaseFrame(HI_HANDLE hTrack, HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

#endif
