#ifndef     _DOLBY_DVBPLAY_H
#define     _DOLBY_DVBPLAY_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "hi_unf_common.h"
#include "hi_unf_avplay.h"
#include "hi_unf_sound.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_demux.h"
#include "hi_unf_hdmi.h"
#include "hi_unf_ecs.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "hi_adp.h"
#include "hi_adp_hdmi.h"
#include "hi_adp_boardcfg.h"
#include "hi_adp_mpi.h"
#include "hi_adp_tuner.h"
#include "hi_adp_mpi.h"


#ifndef DEFAULT_DVB_PORT
#define DEFAULT_DVB_PORT 0
#endif


#define MAX_QAM_LENGTH 7

#define MAX_QAM_MAP_LENGTH 5

#define MAX_TUNER_LENGTH 36


typedef struct hi_QAM_S
{
    HI_U32   u32Qam;
    HI_CHAR  acQam[MAX_QAM_LENGTH];
}QAM_S;

typedef struct hi_TUNERDEVTYPE_S
{
	HI_U32   u32TunerEnum;
	HI_CHAR  acTunerString[MAX_TUNER_LENGTH];
}TUNERDEVTYPE_S;


/*---------------------------------Common Interface--------- ---------------------*/

/**
 * parse PMT
 * @u32DmxId: demux ID
 * @param pat_tb: structure of PAT table
 * @param ppProgTable: pointer to program table
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_ParsePMT(HI_U32 u32DmxId, PAT_TB pat_tb,PMT_COMPACT_TBL **ppProgTable);

/**
 * play dvb
 * @param pcProgSearchArg[]: point to parameter of program search interface: search frequence, symbol rate, modulate mode
 * @param phAvplay: point to avplay handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/

HI_S32 HI_DOLBY_DvbPlay(HI_CHAR *pcProgSearchArg[],HI_HANDLE *phAvplay,PMT_COMPACT_TBL	**ppstProgTbl);

/**
 * de-init function of dvb play
 * @param phAvplay: point to avplay handle
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_DvbPlayDeinit(HI_HANDLE *phAvplay);

#endif
