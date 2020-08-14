/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_ca_adp_pvr.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#ifndef __SAMPLE_PVR_COMMON_H__
#define __SAMPLE_PVR_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_pvr.h"
#include "hi_unf_advca.h"

#include "hi_adp_search.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
#define PVR_FS_DIR              "/tmpfs/"
#define PVR_CIPHER_KEY          "688PVR-KEY-123456789"

#define PVR_DMX_ID_LIVE             0
#define PVR_DMX_PORT_ID_IP          HI_UNF_DMX_PORT_RAM_0
#if defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) 
#define PVR_DMX_PORT_ID_DVB HI_UNF_DMX_PORT_TSI_0
#define PVR_DMX_PORT_ID_PLAYBACK    HI_UNF_DMX_PORT_RAM_1
#else
#ifdef __HI3716MV310__
#define PVR_DMX_PORT_ID_DVB HI_UNF_DMX_PORT_0_TUNER
#define PVR_DMX_PORT_ID_PLAYBACK    HI_UNF_DMX_PORT_3_RAM
#else
#define PVR_DMX_PORT_ID_DVB HI_UNF_DMX_PORT_IF_0
#define PVR_DMX_PORT_ID_PLAYBACK    HI_UNF_DMX_PORT_RAM_1
#endif
#endif

#define PVR_DMX_ID_REC              1

#define PVR_PROG_INFO_MAGIC         0xABCDE
#define DMX_CNT                     5

/*************************** Structure Definition ****************************/
typedef struct hiPVR_PROG_INFO_S
{
    HI_U32                  u32MagicNumber;
    PMT_COMPACT_PROG        stProgInfo;
    HI_UNF_PVR_REC_ATTR_S   stRecAttr;
}PVR_PROG_INFO_S;

/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32 PVR_checkIdx(char *pfileName);
HI_S32 PVR_SavePorgInfo(PVR_PROG_INFO_S *pstProgInfo, HI_CHAR *pszPvrRecFile);
HI_S32 PVR_GetPorgInfo(PVR_PROG_INFO_S *pstProgInfo, const HI_CHAR *pszPvrRecFile);

HI_S32 PVR_RecStart(char *path, PMT_COMPACT_PROG *pstProgInfo, HI_U32 u32DemuxID,
            HI_BOOL bRewind, HI_BOOL bDoCipher, HI_U64 maxSize, HI_U32 *pRecChn,HI_U32 cycle);
HI_S32 PVR_RecStop(HI_U32 u32RecChnID);


HI_S32 PVR_StartLivePlay(HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo);
HI_S32 PVR_StopLivePlay(HI_HANDLE hAvplay);


HI_S32 PVR_StartPlayBack(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn, HI_HANDLE hAvplay, HI_UNF_ADVCA_PVR_PlAY_INFOR_S stPvrPlayInfo);
HI_VOID PVR_StopPlayBack(HI_U32 playChn);
HI_S32 PVR_SwitchDmxSource(HI_U32 dmxId, HI_U32 protId);

HI_S32 PVR_AvplayInit(HI_HANDLE hWin, HI_HANDLE *phAvplay, HI_HANDLE* phSoundTrack);
HI_S32  PVR_AvplayDeInit(HI_HANDLE hAvplay, HI_HANDLE hWin, HI_HANDLE hSoundTrack);

HI_VOID PVR_CallBack(HI_U32 u32ChnID, HI_UNF_PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args);
HI_S32 PVR_RegisterCallBacks(HI_VOID);
HI_S32 PVR_CheckKeyladder(HI_UNF_PVR_CIPHER_S* PvrCipher);

/*for ADVCA*/
HI_S32 CreateTestURI(HI_VOID);
HI_S32 CreateTestFP(HI_VOID);
#ifdef __cplusplus
}
#endif
#endif /* __SAMPL_PVR_COMMON_H__ */



