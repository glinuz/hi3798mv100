/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : drv_sil9293.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/03/06
  Description   :
  History       :
  1.Date        : 2013/03/06
    Author      : j00169368
    Modification: Created file

******************************************************************************/

#ifndef __DRV_SIL9293_H__
#define __DRV_SIL9293_H__

#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/interrupt.h>

#include "hi_debug.h"
#include "hal_sil9293.h"
#include "hi_drv_hdmirx.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define HDMIRX_WILL_STABLE_CNT 40
#define HDMIRX_BEEN_STABLE_CNT 43

#define HDMIRX_CHECK_NULL_PTR(ptr) \
    do {\
        if (NULL == ptr){\
            HI_ERR_HDMIRX("[%s] failed, pointer is NULL!\n", __FUNCTION__); \
            return HI_FAILURE; \
        } \
    } while (0)

#define HDMIRX_CHECK_ENABLE(bEnable) \
    do {\
        if (HI_FALSE == bEnable){\
            HI_ERR_HDMIRX("[%s] failed, HDMIRX is not enable!\n", __FUNCTION__); \
            return HI_FAILURE; \
        } \
    } while (0)

#define HDMIRX_CHECK_BOOL_VAILD(bValue, name) \
    do {\
        if ((HI_TRUE != bValue) && (HI_FALSE != bValue)){\
            HI_ERR_HDMIRX("%s [%d] out of range! FUNC[%s]\n", name, bValue, __FUNCTION__); \
            return HI_FAILURE; \
        } \
    } while (0)

typedef enum hiHDMIRX_IP_STATE_E
{
    HDMIRX_IP_STATE_WAITCONNET = 0,
    HDMIRX_IP_STATE_NOSTABLE,
    HDMIRX_IP_STATE_STABLE,
    HDMIRX_IP_STATE_BUTT
} HDMIRX_IP_STATE_E;

typedef enum hiHDMIRX_STABLE_E
{
    HDMIRX_STABLE_READY = 0,
    HDMIRX_STABLE_PREPROCESS,
    HDMIRX_STABLE_START,
    HDMIRX_STABLE_WORKING,
    HDMIRX_STABLE_BUTT
} HDMIRX_STABLE_E;

typedef struct hiHDMIRX_CONTEXT_S
{
    HI_BOOL bOpened;
    HI_BOOL bLoopThreadEn;
    HI_U32  u32LoopCnt;

    HI_BOOL           bHasClkEven;
    HI_U32            u32StableCnt;
    HDMIRX_IP_STATE_E enIpState;
    HI_HDCP_STATE_E   enHdcpState;
    HDMIRX_STABLE_E   enStabState;

    HI_U32                  u32AudioFs;
    HI_SIG_STATUS_E         enSigStatus;
    HI_HDMIRX_TIMING_INFO_S stTiming;

    struct task_struct *pstLoopThread;
} HDMIRX_CONTEXT_S;
#if (1 == HI_PROC_SUPPORT)
HI_S32	SIL9293_ProcShow(struct seq_file *p);
#endif
HI_S32	SIL9293_Init(HI_VOID);
HI_VOID SIL9293_DeInit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif // __DRV_SIL9293_H__
