#ifndef     _DOLBY_SERCICE_H
#define     _DOLBY_SERCICE_H

#include "hi_type.h"
#include "hi_unf_sound.h"
#include "hi_adp_search.h"
#include "string.h"
#include "dolby_dvbplay.h"



#define PassThrough_MAX_SIZE 14
#define ARRAY_MAX_SIZE 25

typedef struct hi_PROGSEARCH_S
{
	HI_U32   u32Freq;                           /* search frequency */
	HI_U16   u16symborate;                      /* symbol rate */
	HI_CHAR  acQam[MAX_QAM_LENGTH];             /* QAM */
    HI_CHAR  acTuner[MAX_TUNER_LENGTH];         /* tuner type */
} PROGSEARCH_S;

#define MAX_PMT_CNT  (16)
typedef struct hi_PMT_COMPONENT_TBL_S
{
    HI_U32            prog_num;
    PMT_COMPACT_PROG  proginfo[MAX_PMT_CNT];
} PMT_COMPONENT_TBL;

typedef struct hi_PASSTHROUGH_S
{
	HI_CHAR         acAOPort0[PassThrough_MAX_SIZE];      /* audio output port 0 */
	HI_CHAR         acAOPort1[PassThrough_MAX_SIZE];      /* audio output port 1 */
	HI_CHAR	        acPassThrough0[PassThrough_MAX_SIZE]; /* audio pass through para 0*/
    HI_CHAR         acPassThrough1[PassThrough_MAX_SIZE]; /* audio pass through para 1*/
} PASSTHROUGH_S;

typedef struct hi_CHANNELBAR_S
{
	HI_CHAR              acAduioTypeString[ARRAY_MAX_SIZE];  /* audio type string */	
	HI_CHAR              acMonoString[ARRAY_MAX_SIZE];       /* mono type string */
	HI_UNF_TRACK_MODE_E  enTrackMode;                        /* track mode */
	HI_U32               u32Volume;
    HI_CHAR              acVideoTypeString[ARRAY_MAX_SIZE];  /* video type string */
} CHNBAR_S;



/*---------------------------------Common Interface--------- ---------------------*/

/**
 * Initialise OSD
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_OSDInit(HI_VOID);

/**
 * Deinit OSD
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_OSDDeinit(HI_VOID);

/**
 * Get video type
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_CHAR* HI_DOLBY_GetVideoType(HI_VOID);

/**
 * Get audio type
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_CHAR* HI_DOLBY_GetAudioType(HI_VOID);

/**
 * Get mono type
 * @param  u32Dolby_Acmod: mono type
 * @return HI_CHAR*: mono type string
*/
HI_CHAR* HI_DOLBY_GetMonoType(HI_U32 u32Dolby_Acmod);

/**
 * DVB play event
 * @param  pWindHandle: window handle
 * @param  u32KeyId: key ID
  * @param phAvplay: pointer to avplay
 * @param  pstChannelbar: pointer to channel bar
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_DvbKeyEvent(HI_VOID* pWindHandle,HI_U32 u32KeyId,HI_HANDLE* phAvplay,CHNBAR_S  *pstChannelbar,HI_BOOL* pbUpdteChanBar);

/**
 * program search and play process
 * @param  pWindHandle: window handle
 * @param phAvplay: pointer to avplay
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32 HI_DOLBY_ProcProgSearchAndPlay(HI_VOID* pWindHandle,HI_HANDLE* phAvplay,PMT_COMPACT_TBL	**ppstProgTbl);
/**
 * Detect ts change process
 * @param  u32DmxId: Demux ID
 * @param  phAvplay: pointer to avplay
 * @return: success--HI_SUCCESS; failure--HI_FAILURE
*/
HI_S32   HI_DOLBY_DetectTsChng(HI_U32 u32DmxId, HI_HANDLE* phAvplay,HI_VOID* pWindHandle);

#endif
