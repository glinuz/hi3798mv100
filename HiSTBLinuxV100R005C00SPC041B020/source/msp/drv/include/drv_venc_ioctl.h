/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : mpi_priv_venc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/31
  Description   :
  History       :
  1.Date        : 2010/03/31
    Author      : j00131665
    Modification: Created file

******************************************************************************/

#ifndef __DRV_VENC_IOCTL_H__
#define __DRV_VENC_IOCTL_H__

#include "hi_debug.h"
#include "hi_unf_venc.h"
#include "hi_drv_venc.h"

#include "drv_venc_ext.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


typedef struct hiVENC_BUF_OFFSET_S
{
    HI_U32 u32StrmBufOffset[2];
} VENC_BUF_OFFSET_S;

typedef struct hiVENC_CHN_VIRADDR_S
{
    void*  pStrmBufVirAddr;
    void*  pStrmBufVirAddrJpeg;
} VENC_CHN_VirAddr_S;

typedef struct hiVENC_CHN_INFO_S
{
    HI_U32        u32StrmBufPhyAddr;
    HI_U32        u32StrmBufPhyAddrJpeg;
    HI_U32        u32BufSize;
    HI_U32        u32BufSizeJpeg;
    HI_HANDLE     handle;
    HI_HANDLE     hSource;
} VENC_CHN_INFO_S;

typedef struct hiVENC_INFO_CREATE_S
{
    HI_HANDLE              hVencChn;
    HI_BOOL                bOMXChn;
    venc_chan_cfg          stAttr;
    VENC_CHN_INFO_S        stVeInfo;
} VENC_INFO_CREATE_S;

typedef struct hi_VENC_INFO_ATTACH_S
{
    HI_HANDLE   hVencChn;
    HI_HANDLE   hSrc;
    HI_MOD_ID_E enModId;
} VENC_INFO_ATTACH_S;

typedef struct hiVENC_INFO_ACQUIRE_STREAM_S
{
    HI_HANDLE           hVencChn;
    VENC_STREAM_S       stStream;
    HI_U32              u32BlockFlag;
    VENC_BUF_OFFSET_S   stBufOffSet;
    HI_VIRT_ADDR_T      omx_stream_buf;                //for omxvenc
    HI_U32              H264_stream_off;
    HI_U32              Jpeg_Phy_Addr;
} VENC_INFO_ACQUIRE_STREAM_S;

typedef struct hiVENC_STATE_S
{
    HI_HANDLE hVencChn;
} VENC_STATE_S;

typedef struct hiVENC_FRAME_INFO_S
{
    HI_HANDLE                 hVencChn;
    HI_UNF_VIDEO_FRAME_INFO_S stVencFrame;
    venc_user_buf             stVencFrame_OMX;         //for omxvenc
} VENC_INFO_QUEUE_FRAME_S;


typedef struct hiVENC_SET_SRC_INFO_S
{
    HI_HANDLE                 hVencChn;
    HI_DRV_VENC_SRC_INFO_S    stVencSrcInfo;
} VENC_SET_SRC_INFO_S;

typedef struct hiVENC_SET_SRC_S
{
    HI_HANDLE                 hVencChn;
    HI_DRV_VENC_SRC_HANDLE_S    stVencSrc;
} VENC_SET_SRC_S;


/////////////////////////////////////////////////////// just for omxvenc
typedef struct hiVENC_GET_INFO_S
{
    HI_HANDLE                 hVencChn;
    venc_msginfo              msg_info_omx;
} VENC_INFO_GET_MSG_S;

typedef struct hiVENC_INFO_MMZ_MAP_S
{
    HI_HANDLE                 hVencChn;
    HI_MMZ_BUF_S              stVencBuf;
} VENC_INFO_MMZ_MAP_S;

typedef struct hiVENC_MMZ_PHY_S
{
    HI_VIRT_ADDR_T vir2phy_offset;
    HI_U32  hVencChn;
    HI_U32  phyaddr;
    HI_U32  bufsize;
}VENC_INFO_VENC_MMZ_PHY_S;

typedef struct hiVENC_INFO_FLUSH_PORT_S
{
    HI_HANDLE                 hVencChn;
    HI_U32                    u32PortIndex;
} VENC_INFO_FLUSH_PORT_S;
/////////////////////////////////////////////////////// end

#define CMD_VENC_SET_CHN_ATTR _IOWR(IOC_TYPE_VENC, 0, VENC_INFO_CREATE_S)
#define CMD_VENC_GET_CHN_ATTR _IOWR(IOC_TYPE_VENC, 1, VENC_INFO_CREATE_S)

#define CMD_VENC_CREATE_CHN _IOWR(IOC_TYPE_VENC, 2, VENC_INFO_CREATE_S)
#define CMD_VENC_DESTROY_CHN _IOWR(IOC_TYPE_VENC, 3, VENC_INFO_CREATE_S)

#define CMD_VENC_ATTACH_INPUT _IOW(IOC_TYPE_VENC, 4, VENC_INFO_ATTACH_S)
#define CMD_VENC_DETACH_INPUT _IOWR(IOC_TYPE_VENC, 5, VENC_INFO_ATTACH_S)

#define CMD_VENC_ACQUIRE_STREAM _IOWR(IOC_TYPE_VENC, 6, VENC_INFO_ACQUIRE_STREAM_S)
#define CMD_VENC_RELEASE_STREAM _IOW(IOC_TYPE_VENC, 7, VENC_INFO_ACQUIRE_STREAM_S)

#define CMD_VENC_START_RECV_PIC _IOW(IOC_TYPE_VENC, 8, HI_HANDLE)
#define CMD_VENC_STOP_RECV_PIC _IOW(IOC_TYPE_VENC, 9, HI_HANDLE)

#define CMD_VENC_SEND_FRAME _IOW(IOC_TYPE_VENC, 0xa, VENC_INFO_ACQUIRE_STREAM_S)
#define CMD_VENC_REQUEST_I_FRAME _IOW(IOC_TYPE_VENC, 0xb, HI_HANDLE)

#define CMD_VENC_QUEUE_FRAME _IOWR(IOC_TYPE_VENC, 0xc, VENC_INFO_QUEUE_FRAME_S)
#define CMD_VENC_DEQUEUE_FRAME _IOWR(IOC_TYPE_VENC, 0xd, VENC_INFO_QUEUE_FRAME_S)

#define CMD_VENC_SET_SRCINFO _IOWR(IOC_TYPE_VENC, 0xe, VENC_SET_SRC_INFO_S)
#define CMD_VENC_SET_SRC _IOWR(IOC_TYPE_VENC, 0x14, VENC_SET_SRC_S)

////////////////////////////////////////////////////////////////////////////// just for omxvenc
#define CMD_VENC_GET_MSG _IOWR(IOC_TYPE_VENC, 0xf,VENC_INFO_GET_MSG_S)
#define CMD_VENC_QUEUE_STREAM _IOWR(IOC_TYPE_VENC, 0x10,VENC_INFO_QUEUE_FRAME_S)

#define CMD_VENC_MMZ_MAP _IOWR(IOC_TYPE_VENC, 0x11, VENC_INFO_VENC_MMZ_PHY_S)
#define CMD_VENC_MMZ_UMMAP _IOWR(IOC_TYPE_VENC, 0x12, VENC_INFO_VENC_MMZ_PHY_S)

#define CMD_VENC_FLUSH_PORT _IOWR(IOC_TYPE_VENC, 0x13,VENC_INFO_FLUSH_PORT_S)
/////////////////////////////////////////////////////////////////////////////// end
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif //__HI_DRV_VENC_H__
