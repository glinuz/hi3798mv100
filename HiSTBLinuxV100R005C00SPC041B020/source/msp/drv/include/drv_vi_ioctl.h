/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  drv_vi_ioctl.h
* Description:
*
***********************************************************************************/

#ifndef __DRV_VI_IOCTL_H__
#define __DRV_VI_IOCTL_H__

#include "hi_debug.h"
#include "hi_unf_vi.h"
#include "hi_drv_vpss.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/**********************************************************
vi handle defination:
1) real vi
handle = VI_ID(16bits) + PORT(8bits) + CHAN(8bits, fix to 0)
2) virtual vi
handle = VI_ID(16bits) + PORT(8bits) + CHAN(8bits, max is 2)
 ***********************************************************/

#define MAX_VI_CHN 2
#define MAX_VI_PORT 2

#if !defined(CHIP_TYPE_hi3716mv410) && !defined(CHIP_TYPE_hi3716mv420) && !defined(CHIP_TYPE_hi3798cv200)
#define VI_MAX_VPSS_PORT 3
#else
#define VI_MAX_VPSS_PORT 2
#endif

typedef struct hiUNF_VI_CREATE_S
{
    HI_UNF_VI_E      enPort;
    HI_HANDLE        hVi;
    VPSS_HANDLE      hVpss;
    HI_UNF_VI_ATTR_S stViAttr;
} VI_CREATE_S;

typedef struct hiVI_ATTR_S
{
    HI_HANDLE        hVi;
    HI_UNF_VI_ATTR_S stAttr;
} VI_ATTR_S;

typedef struct hiVI_BUF_ATTR_S
{
    HI_HANDLE               hVi;
    HI_UNF_VI_BUFFER_ATTR_S stBufAttr;
} VI_BUF_ATTR_S;

typedef struct hiVI_FRAME_S
{
    HI_HANDLE                 hVi;
    HI_UNF_VIDEO_FRAME_INFO_S stViFrame;
    HI_U32                    u32TimeoutMs;
} VI_FRAME_S;

typedef int (*PFN_VI_DRV_RlsImage)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VI_DRV_ChangeVencInfo)(HI_HANDLE, HI_U32, HI_U32, HI_U32);
typedef int (*PFN_VI_DRV_ChangeWinInfo)(HI_HANDLE, HI_DRV_WIN_PRIV_INFO_S*);

typedef struct hiVI_VPSS_PORT_PARAM_S
{
    HI_HANDLE                 hDst;
    HI_HANDLE                 hPort;
    HI_BOOL                   bEnable;
//    PFN_VI_DRV_RlsImage       pfRlsImage;
//    PFN_VI_DRV_ChangeVencInfo pfChangeVencInfo;
//    PFN_VI_DRV_ChangeWinInfo  pfChangeWinInfo;
} VI_VPSS_PORT_PARAM_S;

typedef struct hiVI_VPSS_PORT_S
{
    HI_HANDLE            hVi;
    VPSS_HANDLE          hVpss;
    VI_VPSS_PORT_PARAM_S stPortParam;
} VI_VPSS_PORT_S;

typedef struct hiVI_VPSS_PORT_AND_WIN_S
{
    HI_HANDLE hPort;
    HI_HANDLE hWindow;
} VI_VPSS_PORT_AND_WIN_S;

typedef enum hiVI_ENGINE_STATE_E
{
    VI_ENGINE_ATTACHED = 0x1,
    VI_ENGINE_STARTED = 0x2,
    VI_ENGINE_BUFSET = 0x4,
    VI_ENGINE_BUTT = 0x0
} VI_ENGINE_STATE_E;

typedef struct hiVI_S
{
    HI_HANDLE   hVi;
    VPSS_HANDLE hVpss;

    HI_U32           u32State;
    HI_UNF_VI_ATTR_S stAttr;

    VI_VPSS_PORT_PARAM_S stPortParam[VI_MAX_VPSS_PORT];
} VI_S;

typedef enum hiIOC_VI_E
{
    IOC_VI_CREATE = 0,
    IOC_VI_DESTROY,
    IOC_VI_SET_ATTR,
    IOC_VI_SET_BUF,
    IOC_VI_CREATE_VPSS_PORT,
    IOC_VI_DESTROY_VPSS_PORT,
    IOC_VI_START,
    IOC_VI_STOP,
    IOC_VI_Q_FRAME,
    IOC_VI_DQ_FRAME,
    IOC_VI_ACQUIRE_FRAME,
    IOC_VI_RELEASE_FRAME,
    IOC_VI_BUTT,
} IOC_VI_E;

#define CMD_VI_CREATE _IOWR(HI_ID_VI, IOC_VI_CREATE, VI_CREATE_S)
#define CMD_VI_DESTROY _IOWR(HI_ID_VI, IOC_VI_DESTROY, HI_HANDLE)

#define CMD_VI_SET_ATTR _IOWR(HI_ID_VI, IOC_VI_SET_ATTR, VI_ATTR_S)
#define CMD_VI_SET_BUF _IOWR(HI_ID_VI, IOC_VI_SET_BUF, VI_BUF_ATTR_S)

#define CMD_VI_CREATE_VPSS_PORT _IOWR(HI_ID_VI, IOC_VI_CREATE_VPSS_PORT, VI_VPSS_PORT_S)
#define CMD_VI_DESTROY_VPSS_PORT _IOWR(HI_ID_VI, IOC_VI_DESTROY_VPSS_PORT, VI_VPSS_PORT_S)


#define CMD_VI_START _IOWR(HI_ID_VI, IOC_VI_START, HI_HANDLE)
#define CMD_VI_STOP _IOWR(HI_ID_VI, IOC_VI_STOP, HI_HANDLE)

#define CMD_VI_Q_FRAME _IOWR(HI_ID_VI, IOC_VI_Q_FRAME, VI_FRAME_S)
#define CMD_VI_DQ_FRAME _IOWR(HI_ID_VI, IOC_VI_DQ_FRAME, VI_FRAME_S)

#define CMD_VI_ACQUIRE_FRAME _IOWR(HI_ID_VI, IOC_VI_ACQUIRE_FRAME, VI_FRAME_S)
#define CMD_VI_RELEASE_FRAME _IOWR(HI_ID_VI, IOC_VI_RELEASE_FRAME, VI_FRAME_S)

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__DRV_VI_IOCTL_H__
