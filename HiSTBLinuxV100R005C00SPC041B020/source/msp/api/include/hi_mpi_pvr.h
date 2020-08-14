/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_pvr.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/09
  Description   : hi_pvr_api.h header file
  History       :
  1.Date        : 2008/04/09
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifndef __HI_MPI_PVR_H__
#define __HI_MPI_PVR_H__


#include "hi_type.h"

#include "hi_unf_pvr.h"
#include "hi_error_mpi.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PVR_FIFO_DIO_LEN     512
#define PVR_FIFO_SECTOR_SIZE 512
#define PVR_FIFO_WRITE_BLOCK_SIZE (PVR_FIFO_DIO_LEN * 2 * 188LLU) /* 188k:the bigger block make more efficient*/

typedef struct hiUNF_PVR_CBBUF_INFO_S
{
    HI_U8 *pu8VirAddr;
    HI_U32 u32PhyAddr;
    HI_U32 u32Len;
}HI_UNF_PVR_CBBUF_INFO_S;
typedef HI_S32 (*ExtendCallBack_New)(HI_UNF_PVR_DATA_ATTR_S *pstDataAttr, 
                              HI_UNF_PVR_CBBUF_INFO_S *pstDstBuf, 
                              HI_UNF_PVR_CBBUF_INFO_S *pstSrcBuf, 
                              HI_U32 u32Offset, 
                              HI_U32 u32DataSize, 
                              HI_VOID *pUserData);

/***********************************************************
                API Declaration
 ***********************************************************/

/***** APIs for PVR recode *****/

/* initialize and de-initialize of record module                            */
HI_S32 HI_PVR_RecInit(HI_VOID);
HI_S32 HI_PVR_RecDeInit(HI_VOID);

/* applay and release new record channel                                    */
HI_S32 HI_PVR_RecCreateChn(HI_U32 *pu32ChnID, const HI_UNF_PVR_REC_ATTR_S *pstRecAttr);
HI_S32 HI_PVR_RecDestroyChn(HI_U32 u32ChnID);

/* set and get attributes of record channel                                 */
HI_S32 HI_PVR_RecSetChn(HI_U32 u32ChnID, const HI_UNF_PVR_REC_ATTR_S *pstRecAttr);
HI_S32 HI_PVR_RecGetChn(HI_U32 u32ChnID, HI_UNF_PVR_REC_ATTR_S *pstRecAttr);

/* start and stop record channel                                            */
HI_S32 HI_PVR_RecStartChn(HI_U32 u32ChnID);
HI_S32 HI_PVR_RecStopChn(HI_U32 u32ChnID);
HI_S32 HI_PVR_RecPauseChn(HI_U32 u32ChnID);
HI_S32 HI_PVR_RecResumeChn(HI_U32 u32ChnID);


/* get record status                                                        */
HI_S32 HI_PVR_RecGetStatus(HI_U32 u32ChnID, HI_UNF_PVR_REC_STATUS_S *pstRecStatus);


/***** APIs for PVR play *****/

/* initialize and de-initialize of play module                              */
HI_S32 HI_PVR_PlayInit(HI_VOID);
HI_S32 HI_PVR_PlayDeInit(HI_VOID);

/* apply and release new play channel                                       */
HI_S32 HI_PVR_PlayCreateChn(HI_U32 *pChn, const HI_UNF_PVR_PLAY_ATTR_S *pAttr, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer);
HI_S32 HI_PVR_PlayDestroyChn(HI_U32 u32ChnID);

/* start and stop time shift                                                */
HI_S32 HI_PVR_PlayStartTimeShift(HI_U32 *pu32PlayChnID, HI_U32 u32RecChnID, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer);
HI_S32 HI_PVR_PlayStopTimeShift(HI_U32 u32PlayChnID, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt);

/* set and get attributes of play channel                                   */
HI_S32 HI_PVR_PlaySetChn(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr);
HI_S32 HI_PVR_PlayGetChn(HI_U32 u32ChnID, HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr);

/* start and stop play channel                                              */
HI_S32 HI_PVR_PlayStartChn(HI_U32 u32ChnID);
HI_S32 HI_PVR_PlayStopChn(HI_U32 u32ChnID, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt);

/* pause and resume play channel                                            */
HI_S32 HI_PVR_PlayPauseChn(HI_U32 u32ChnID);
HI_S32 HI_PVR_PlayResumeChn(HI_U32 u32ChnID);

/* get play status                                                          */
HI_S32 HI_PVR_PlayGetStatus(HI_U32 u32ChnID, HI_UNF_PVR_PLAY_STATUS_S *pstStatus);

/* locate play position                                                     */
HI_S32 HI_PVR_PlaySeek(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_POSITION_S *pstPosition);

/* start trick mode of playing                                              */
HI_S32 HI_PVR_PlayTrickMode(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_MODE_S *pstTrickMode);

/* start step back frame by frame                                           */
HI_S32 HI_PVR_PlayStep(HI_U32 u32ChnID, HI_S32 s32Direction);

/* get file info */
HI_S32 HI_PVR_PlayGetFileAttr(HI_U32 u32ChnID, HI_UNF_PVR_FILE_ATTR_S *pAttr);

/* get file attr, no need to new play channel */
HI_S32 HI_PVR_GetFileAttrByFileName(const HI_CHAR *pFileName, HI_UNF_PVR_FILE_ATTR_S *pAttr);

HI_S32 HI_PVR_SetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32UsrDataLen);

HI_S32 HI_PVR_GetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* pUsrDataLen);

HI_S32 HI_PVR_SetCAData(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32CADataLen);

HI_S32 HI_PVR_GetCAData(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* u32CADataLen);

/***** APIs for PVR event callback *****/

/* register and un-register envent callback function    AI7D02612                    */
HI_S32 HI_PVR_RegisterEvent(HI_UNF_PVR_EVENT_E enEventType, eventCallBack callBack, HI_VOID *args);
HI_S32 HI_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_E enEventType);

#if 0 /*obsolete APIs*/
HI_S32 HI_PVR_RegisterEventByChn(HI_U32 u32ChnId, eventCallBack callBack, HI_VOID *args);
HI_S32 HI_PVR_UnRegisterEventByChn(HI_U32 u32ChnId);
#endif
HI_S32 HI_PVR_CreateIdxFile(HI_U8* pstTsFileName, HI_U8* pstIdxFileName, HI_UNF_PVR_GEN_IDX_ATTR_S* pAttr);

HI_VOID HI_PVR_ConfigDebugInfo(HI_LOG_LEVEL_E u32DebugLevel);

HI_VOID HI_PVR_RemoveFile(const HI_CHAR *pFileName);

HI_S32 HI_PVR_RegisterExtraCallback(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E eExtraCallbackType, ExtraCallBack fCallback, HI_VOID *args);

HI_S32 HI_PVR_UnRegisterExtraCallBack(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E eExtraCallbackType);

HI_S32 HI_PVR_RegisterExtendCallback(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E eExtraCallbackType, ExtendCallBack_New fCallback, HI_VOID *pUserData);

HI_S32 HI_PVR_UnRegisterExtendCallBack(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E eExtraCallbackType, ExtendCallBack_New fCallback);

HI_S32 HI_PVR_RecAddPid(HI_U32 u32ChnID, HI_U32 u32Pid);

HI_S32 HI_PVR_RecDelPid(HI_U32 u32ChnID, HI_U32 u32Pid);

HI_S32 HI_PVR_RecDelAllPid(HI_U32 u32ChnID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  // __HI_MPI_PVR_H__

