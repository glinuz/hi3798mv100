/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     :mpi_hiao_track.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/12
  Description   :
  History       :
  1.Date        : 2013/12/12
    Author      : w0019655
    Modification: Created file

 *******************************************************************************/

#ifndef  __MPI_AEF_H__
#define  __MPI_AEF_H__


#include "hi_audsp_aoe.h"
#include "hi_audio_effect.h"
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

#define AEF_BUF_DEFAULT_FRAME_NUM  3
#define AEF_FLUSH_PROC_COUNT 6


#define AEF_LOCK_INIT(p_mutex) ;                 \
    (void)pthread_mutex_init(p_mutex, NULL)

#define AEF_LOCK_DESTROY(p_mutex) \
    (void)pthread_mutex_destroy(p_mutex)

#define AEF_LOCK(p_mutex) \
    (void)pthread_mutex_lock(p_mutex)

#define AEF_UNLOCK(p_mutex) \
    (void)pthread_mutex_unlock(p_mutex)

typedef struct
{
    HI_HANDLE                hEntry;
    HI_HANDLE                hHaEffect;
    HI_UNF_SND_E             enSnd;
    HI_BOOL                  bEnable;
    AO_AEF_ATTR_S            stAefAttr;
    AO_AEF_PROC_ITEM_S      *pstProcItem;
    HI_VOID                 *pAefInbuf;
    HI_VOID                 *pAefOutbuf;
} AEF_CHANNEL_S;

typedef struct
{
    HI_VOID                 *pInBufAddr;
    HI_VOID                 *pOutBufAddr;
    HI_VOID                 *pAefSettingAddr;
    S_MIXER_REGS_TYPE       *pEngRegAddr;
    HI_U32                   u32AefMask;
    pthread_t                AefThdInst;
    HI_BOOL                  bAefThreadRun;
    AO_AEF_BUF_ATTR_S        stAefBufAttr;
} SND_AEF_ATTR_S;

HI_S32 HI_MPI_AO_AEF_RegisterAuthLib(const HI_CHAR *pAefLibFileName);
HI_S32 HI_MPI_AO_AEF_Create(HI_UNF_SND_E enSound, HI_UNF_SND_AEF_TYPE_E enAefType, HI_VOID *pstAdvAttr, HI_HANDLE *phAef);
HI_S32 HI_MPI_AO_AEF_Destroy(HI_HANDLE hAef);
HI_S32 HI_MPI_AO_AEF_SetEnable(HI_HANDLE hAef, HI_BOOL bEnable);
HI_S32 HI_MPI_AO_AEF_GetEnable(HI_HANDLE hAef, HI_BOOL *pbEnable);
HI_S32 HI_MPI_AO_AEF_SetParams(HI_HANDLE hAef, HI_U32 u32ParamType, const HI_VOID *pstParms);
HI_S32 HI_MPI_AO_AEF_GetParams(HI_HANDLE hAef, HI_U32 u32ParamType, HI_VOID *pstParms);
HI_S32 HI_MPI_AO_AEF_SetConfig(HI_HANDLE hAef, HI_U32 u32CfgType, const HI_VOID *pstConfig);
HI_S32 HI_MPI_AO_AEF_GetConfig(HI_HANDLE hAef, HI_U32 u32CfgType, HI_VOID *pstConfig);
HI_S32 HI_MPI_AO_AEFFindHaEffectAuth(HI_U32 enEffectID, HI_AEF_AUTHORIZE_S** ppstAefAuth);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
