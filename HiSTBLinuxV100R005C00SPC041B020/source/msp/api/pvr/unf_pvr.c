/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_pvr.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/09
  Description   : call hi_pvr_api.c functions directly
  History       :
  1.Date        : 2008/04/09
    Author      : q46153
    Modification: Created file

******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_mpi_pvr.h"
#include "hi_unf_cipher.h"

/***********************************************************
                API Define
 ***********************************************************/

/***** APIs for PVR recode *****/

/* initialize and de-initialize of record module                            */
HI_S32 HI_UNF_PVR_RecInit(HI_VOID)
{
    /*w37134 for AI7D05516 pvr need to call cipher function,cipher function is offered inner application not client application , it should be called by inner module  */
      /*CNcomment:需要使用加密接口，而加密接口不提供给客户，需要内部打开*/
    (void)HI_UNF_CIPHER_Open();

    return HI_PVR_RecInit();
}

HI_S32 HI_UNF_PVR_RecDeInit(HI_VOID)
{
    //HI_UNF_CIPHER_Close();
    return HI_PVR_RecDeInit();
}

/* applay and release new record channel                                    */
HI_S32 HI_UNF_PVR_RecCreateChn(HI_U32 *pu32ChnID, const HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    return HI_PVR_RecCreateChn(pu32ChnID, pstRecAttr);
}

HI_S32 HI_UNF_PVR_RecDestroyChn(HI_U32 u32ChnID)
{
    return HI_PVR_RecDestroyChn(u32ChnID);
}

/* set and get attributes of record channel                                 */
HI_S32 HI_UNF_PVR_RecSetChn(HI_U32 u32ChnID, const HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    return HI_PVR_RecSetChn(u32ChnID, pstRecAttr);
}

HI_S32 HI_UNF_PVR_RecGetChn(HI_U32 u32ChnID, HI_UNF_PVR_REC_ATTR_S *pstRecAttr)
{
    return HI_PVR_RecGetChn(u32ChnID, pstRecAttr);
}

/* start and stop record channel                                            */
HI_S32 HI_UNF_PVR_RecStartChn(HI_U32 u32ChnID)
{
    return HI_PVR_RecStartChn(u32ChnID);
}

HI_S32 HI_UNF_PVR_RecStopChn(HI_U32 u32ChnID)
{
    return HI_PVR_RecStopChn(u32ChnID);
}

HI_S32 HI_UNF_PVR_RecPauseChn(HI_U32 u32ChnID)
{
    return HI_PVR_RecPauseChn(u32ChnID);
}

HI_S32 HI_UNF_PVR_RecResumeChn(HI_U32 u32ChnID)
{
    return HI_PVR_RecResumeChn(u32ChnID);
}


/* get record status                                                        */
HI_S32 HI_UNF_PVR_RecGetStatus(HI_U32 u32ChnID, HI_UNF_PVR_REC_STATUS_S *pstRecStatus)
{
    return HI_PVR_RecGetStatus(u32ChnID, pstRecStatus);
}


/***** APIs for PVR play *****/

/* initialize and de-initialize of play module                              */
HI_S32 HI_UNF_PVR_PlayInit(HI_VOID)
{
    /*w37134 for AI7D05516 pvr need to call cipher function,cipher function is offered inner application not client application , it should be called by inner module  */
      /*CNcomment:需要使用加密接口，而加密接口不提供给客户，需要内部打开*/
    (void)HI_UNF_CIPHER_Open();

    return HI_PVR_PlayInit();
}

HI_S32 HI_UNF_PVR_PlayDeInit(HI_VOID)
{
    //HI_UNF_CIPHER_Close();
    return HI_PVR_PlayDeInit();
}

/* apply and release new play channel                                       */
HI_S32 HI_UNF_PVR_PlayCreateChn(HI_U32 *pu32ChnID, const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer)
{
    return HI_PVR_PlayCreateChn(pu32ChnID, pstPlayAttr, hAvplay, hTsBuffer);
}

HI_S32 HI_UNF_PVR_PlayDestroyChn(HI_U32 u32ChnID)
{
    return HI_PVR_PlayDestroyChn(u32ChnID);
}

/* start and stop time shift                                                */
HI_S32 HI_UNF_PVR_PlayStartTimeShift(HI_U32 *pu32PlayChnID, HI_U32 u32RecChnID, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer)
{

    return HI_PVR_PlayStartTimeShift(pu32PlayChnID, u32RecChnID, hAvplay, hTsBuffer);
}

HI_S32 HI_UNF_PVR_PlayStopTimeShift(HI_U32 u32PlayChnID,  const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt)
{
    return HI_PVR_PlayStopTimeShift(u32PlayChnID, pstStopOpt);
}

/* set and get attributes of play channel                                   */
HI_S32 HI_UNF_PVR_PlaySetChn(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr)
{
    return HI_PVR_PlaySetChn(u32ChnID, pstPlayAttr);
}

HI_S32 HI_UNF_PVR_PlayGetChn(HI_U32 u32ChnID, HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr)
{
    return HI_PVR_PlayGetChn(u32ChnID, pstPlayAttr);
}

/* start and stop play channel                                              */
HI_S32 HI_UNF_PVR_PlayStartChn(HI_U32 u32ChnID)
{
    return HI_PVR_PlayStartChn(u32ChnID);
}

HI_S32 HI_UNF_PVR_PlayStopChn(HI_U32 u32ChnID,  const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt)
{
    return HI_PVR_PlayStopChn(u32ChnID, pstStopOpt);
}

/* pause and resume play channel                                            */
HI_S32 HI_UNF_PVR_PlayPauseChn(HI_U32 u32ChnID)
{
    return HI_PVR_PlayPauseChn(u32ChnID);
}

HI_S32 HI_UNF_PVR_PlayResumeChn(HI_U32 u32ChnID)
{
    return HI_PVR_PlayResumeChn(u32ChnID);
}

/* get play status                                                          */
HI_S32 HI_UNF_PVR_PlayGetStatus(HI_U32 u32ChnID, HI_UNF_PVR_PLAY_STATUS_S *pstStatus)
{
    return HI_PVR_PlayGetStatus(u32ChnID, pstStatus);
}

/* locate play position                                                     */
HI_S32 HI_UNF_PVR_PlaySeek(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_POSITION_S *pstPosition)
{
    return HI_PVR_PlaySeek(u32ChnID, pstPosition);
}

/* start trick mode of playing                                              */
HI_S32 HI_UNF_PVR_PlayTPlay(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_MODE_S *pstTrickMode)
{
    return HI_PVR_PlayTrickMode(u32ChnID, pstTrickMode);
}

/* start step back frame by frame                                           */
HI_S32 HI_UNF_PVR_PlayStep(HI_U32 u32ChnID, HI_S32 s32Direction)
{
    return HI_PVR_PlayStep(u32ChnID, s32Direction);
}

/* get file info */
HI_S32 HI_UNF_PVR_PlayGetFileAttr(HI_U32 u32ChnID, HI_UNF_PVR_FILE_ATTR_S *pAttr)
{
    return HI_PVR_PlayGetFileAttr(u32ChnID, pAttr);
}

/* get file attr, no need to new play channel */
HI_S32 HI_UNF_PVR_GetFileAttrByFileName(const HI_CHAR *pFileName, HI_UNF_PVR_FILE_ATTR_S *pAttr)
{
    return HI_PVR_GetFileAttrByFileName(pFileName, pAttr);
}

HI_S32 HI_UNF_PVR_SetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32UsrDataLen)
{
    return HI_PVR_SetUsrDataInfoByFileName(pFileName, pInfo, u32UsrDataLen);
}

HI_S32 HI_UNF_PVR_GetUsrDataInfoByFileName(const HI_CHAR *pFileName, HI_U8 *pInfo, HI_U32 u32BufLen, HI_U32* pUsrDataLen)
{
    return HI_PVR_GetUsrDataInfoByFileName(pFileName, pInfo, u32BufLen, pUsrDataLen);
}


/***** APIs for PVR event callback *****/
/* register and un-register envent callback function                        */
HI_S32 HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_E enEventType, eventCallBack callBack, HI_VOID *args)
{
    return HI_PVR_RegisterEvent(enEventType, callBack, args);
}

HI_S32 HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_E enEventType)
{
    return HI_PVR_UnRegisterEvent(enEventType);
}

HI_S32 HI_UNF_PVR_CreateIdxFile(HI_U8* pstTsFileName, HI_U8* pstIdxFileName, HI_UNF_PVR_GEN_IDX_ATTR_S* pAttr)
{
    return HI_PVR_CreateIdxFile(pstTsFileName, pstIdxFileName, pAttr);
}

HI_VOID HI_UNF_PVR_ConfigDebugInfo(HI_LOG_LEVEL_E enDebugLevel)
{
    HI_PVR_ConfigDebugInfo(enDebugLevel);
}

HI_VOID HI_UNF_PVR_RemoveFile(const HI_CHAR *pFileName)
{
    HI_PVR_RemoveFile(pFileName);
}
HI_S32 HI_UNF_PVR_RegisterExtraCallback(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E enExtraCallbackType, ExtraCallBack fCallback, HI_VOID *args)
{
    return HI_PVR_RegisterExtraCallback(u32ChnID, enExtraCallbackType, fCallback, args);
}

HI_S32 HI_UNF_PVR_UnRegisterExtraCallBack(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E enExtraCallbackType)
{
    return HI_PVR_UnRegisterExtraCallBack(u32ChnID, enExtraCallbackType);
}

HI_S32 HI_UNF_PVR_RegisterExtendCallback(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E enExtraCallbackType, ExtendCallBack fCallback, HI_VOID *pUserData)
{
    return HI_ERR_PVR_NOT_SUPPORT;//HI_PVR_RegisterExtendCallback(u32ChnID, enExtraCallbackType, fCallback, pUserData);
}

HI_S32 HI_UNF_PVR_UnRegisterExtendCallback(HI_U32 u32ChnID, HI_UNF_PVR_EXTRA_CALLBACK_E enExtraCallbackType, ExtendCallBack fCallback)
{
    return HI_ERR_PVR_NOT_SUPPORT;//HI_PVR_UnRegisterExtendCallBack(u32ChnID, enExtraCallbackType, fCallback);
}

HI_S32 HI_UNF_PVR_RecAddPID(HI_U32 u32ChnID, HI_U32 u32Pid)
{
    return HI_PVR_RecAddPid(u32ChnID, u32Pid);
}

HI_S32 HI_UNF_PVR_RecDelPID(HI_U32 u32ChnID, HI_U32 u32Pid)
{
    return HI_PVR_RecDelPid(u32ChnID, u32Pid);
}

HI_S32 HI_UNF_PVR_RecDelAllPID(HI_U32 u32ChnID)
{
    return HI_PVR_RecDelAllPid(u32ChnID);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


