#ifndef __DRV_WIN_FRC_H__
#define __DRV_WIN_FRC_H__

#include "hi_common.h"
#include <linux/string.h>
#include"hi_drv_mem.h"
#include"hi_drv_log.h"
#include"hi_drv_sys.h"
#include"hi_drv_stat.h"
#include"hi_drv_video.h"
#include "hi_debug.h"
#include <linux/list.h>
#include <linux/io.h>
#include <linux/delay.h>
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_WARN_FRC(fmt...)    HI_WARN_PRINT(HI_ID_VO, fmt)
#define HI_INFO_FRC(fmt...)    HI_INFO_PRINT(HI_ID_VO, fmt)
#define HI_ERR_FRC(fmt...)     HI_ERR_PRINT(HI_ID_VO, fmt)
#define HI_FATAL_FRC(fmt...)   HI_FATAL_PRINT(HI_ID_VO, fmt)

#define FRC_MAX_NUM     32

typedef struct
{
    HI_HANDLE   hFrc;
    HI_U32      CurID;       /* current insert or drop position in a FRC cycle */
    HI_U32      InputCount;  /* input counter */
}FRC_S;

typedef struct 
{
    FRC_S               *Frc;
}FRC_INFO_S;

#if 0
static FRC_S            g_FrcCtx[FRC_MAX_NUM] = {{0, 0, 0}};
static FRC_INFO_S       g_Frc[FRC_MAX_NUM] = {{HI_NULL}};
#define FRC_INST_LOCK_CHECK(Handle, Id)\
    do\
    {\
        if (Id > FRC_MAX_NUM)\
        {\
            HI_ERR_FRC("FRC %d err\n", Id);\
            return HI_FAILURE;\
        }\
        pFrc = g_Frc[Id].Frc;\
        if (HI_NULL == pFrc)\
        {\
            HI_ERR_FRC("pFrc is Null\n");\
            return HI_FAILURE;\
        }\
        if (Handle != pFrc->hFrc)\
        {\
            HI_ERR_FRC("handle is invalid\n");\
            return HI_FAILURE;\
        }\
    }while(0)
#endif

#define HI_ID_FRC               0xC0
#define GET_FRC_HANDLE(Id)      ((HI_ID_FRC << 16) | (Id))

#define GET_FRC_ID(Handle)      ((Handle) & 0xff)

HI_S32 DRV_WIN_FRC_Init(HI_VOID);
HI_S32 DRV_WIN_FRC_DeInit(HI_VOID);
HI_S32 DRV_WIN_FRC_Create(HI_HANDLE *phFrc);
HI_S32 DRV_WIN_FRC_Destroy(HI_HANDLE hFrc);
HI_S32 DRV_WIN_FRC_Reset(HI_HANDLE hFrc);
HI_S32 DRV_WIN_FRC_Calculate(HI_HANDLE hFrc, HI_U32 InRate, HI_U32 OutRate, HI_U32 *NeedPlayCnt);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
