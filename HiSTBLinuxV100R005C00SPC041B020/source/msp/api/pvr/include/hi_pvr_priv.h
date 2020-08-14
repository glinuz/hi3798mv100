/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_priv.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   :
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_PRIV_H__
#define __HI_PVR_PRIV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hi_type.h"
#include "hi_mpi_pvr.h"
#include "hi_pvr_debug.h"
#include "hi_pvr_intf.h"
#include "hi_pvr_cipher_cfg.h"
#include "hi_drv_pvr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef STATIC
#define STATIC static
#endif

#ifndef INLINE
#if HI_OS_TYPE == HI_OS_LINUX
#define INLINE inline
#elif HI_OS_TYPE == HI_OS_WIN32
#define INLINE __inline
#endif
#endif

#if 0
#define PVR_LOCK(p_mutex)        HI_INFO_PVR("==>\n");(void)pthread_mutex_lock(p_mutex);HI_INFO_PVR("==|\n")
#define PVR_UNLOCK(p_mutex)      HI_INFO_PVR("<==\n");(void)pthread_mutex_unlock(p_mutex);HI_INFO_PVR("==|\n")
#else
#define PVR_LOCK(p_mutex)       (void)pthread_mutex_lock(p_mutex)
#define PVR_UNLOCK(p_mutex)     (void)pthread_mutex_unlock(p_mutex)
#endif

/* length of TS package                                                     */
#define PVR_TS_LEN           188

/**  */
#define PVR_REC_MIN_DAV_BUF  (PVR_TS_LEN*1024)
/** */
#define PVR_REC_MAX_DAV_BUF  (PVR_TS_LEN*256*1024)
/** */
#define PVR_REC_MIN_SC_BUF   (7*4*1024)
/** */
#define PVR_REC_MAX_SC_BUF   (7*32*1024)


#define PVR_SC_SIZE                     4         /* Byte */
#define PVR_TS_HEAD_SIZE                4
#define PVR_TS_PD_SIZE_POS              4         /* the fifth byte in TS header of the length area of padding */
#define PVR_TS_PD_FLAG_POS              5         /* the sixth byte in TS header of the flag area of padding  */
#define PVR_TS_MIN_PD_SIZE              2
#define PVR_VIDEO_TYPE_TOTAL_NUM        (40)    /*this macro should sync to HI_UNF_VCODEC_TYPE_E*/
#define PVR_PLAY_MAX_STATUS_NUM         (12)
/*
Table 2-6 -- Adaptation field control values
value   description
00  reserved for future use by ISO/IEC
01  no adaptation_field, payload only
10  adaptation_field only, no payload
11  adaptation_field followed by payload
*/
#define PVR_TS_ADAPT_RESERVED    0x0
#define PVR_TS_ADAPT_PLD_ONLY    0x1
#define PVR_TS_ADAPT_ADAPT_ONLY  0x2
#define PVR_TS_ADAPT_BOTH        0x3

#define PVR_TS_ADAPT_HAVE_PLD(flag)   (flag & PVR_TS_ADAPT_PLD_ONLY)
#define PVR_TS_ADAPT_HAVE_ADAPT(flag) (flag & PVR_TS_ADAPT_ADAPT_ONLY)

#define PVR_TIME_CTRL_TIMEBASE_NS      64000000

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define PVR_PROC_SUPPORT
#endif
#ifdef PVR_PROC_SUPPORT
#define PVR_USR_PROC_DIR "pvr"
#define PVR_USR_PROC_REC_ENTRY_NAME "pvr_rec"
#define PVR_USR_PROC_PLAY_ENTRY_NAME "pvr_play"
#endif

/* cipher buffer for decrypt                                                */
typedef struct hiPVR_PHY_BUF_S
{
    HI_U8   *pu8Addr;                                        /* buffer address */
    HI_U32   u32PhyAddr;                                     /* physical address of buffer */
    HI_U32   u32Size;                                        /* buffer size */
} PVR_PHY_BUF_S;

#if 0 
#define PVR_USLEEP  usleep
#else
#define PVR_USLEEP(usecs)\
    do{\
        struct timespec stTime;\
        stTime.tv_sec = 0;\
        stTime.tv_nsec = 1000*(long)(usecs);\
        (HI_VOID)nanosleep(&stTime, HI_NULL);\
    }while(0)
#endif
#define DO_FUNC(fun) \
    do{ \
        HI_S32 l_ret = fun; \
        if (l_ret != HI_SUCCESS) \
        { \
            HI_ERR_PVR("%s failed, ERRNO:%#x.\n", #fun, l_ret); \
            return l_ret; \
        } \
    }while(0)

#define DO_FUNC_UNLOCK(fun, pLock) \
    do{ \
        HI_S32 l_ret = fun; \
        if (l_ret != HI_SUCCESS) \
        { \
            HI_ERR_PVR("%s failed, ERRNO:%#x.\n", #fun, l_ret); \
            PVR_UNLOCK(pLock);\
            return l_ret; \
        } \
    }while(0)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifdef __HI_PVR_PRIV_H__ */

