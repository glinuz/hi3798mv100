#ifndef __HDMI_EXT_H__
#define __HDMI_EXT_H__
#include "hi_unf_hdmi.h"
#include "hi_drv_hdmi.h"
#include "hi_drv_disp.h"
#include "hi_drv_dev.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

HI_S32 HDMI_DRV_ModInit(HI_VOID);
HI_VOID HDMI_DRV_ModExit(HI_VOID);


typedef HI_S32  (*FN_HDMI_Init)(HI_VOID);
typedef HI_VOID (*FN_HDMI_Deinit)(HI_VOID);
typedef HI_S32  (*FN_HDMI_Open)(HI_UNF_HDMI_ID_E enHdmi);
typedef HI_S32  (*FN_HDMI_Close)(HI_UNF_HDMI_ID_E enHdmi);

typedef HI_S32  (*FN_HDMI_GetPlayStus)(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus);
typedef HI_S32  (*FN_HDMI_GetAoAttr)(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);
typedef HI_S32  (*FN_HDMI_GetSinkCapability)(HI_DRV_HDMI_ID_E enHdmi, HI_DRV_HDMI_SINK_CAPABILITY_S *pstSinkCap);
typedef HI_S32  (*FN_HDMI_GetAudioCapability)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_AUDIO_CAPABILITY_S *pstAudCap);
typedef HI_S32  (*FN_HDMI_SetAudioMute)(HI_UNF_HDMI_ID_E enHdmi);
typedef HI_S32  (*FN_HDMI_SetAudioUnMute)(HI_UNF_HDMI_ID_E enHdmi);
typedef HI_S32  (*FN_HDMI_AudioChange)(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);

typedef HI_S32  (*FN_HDMI_Detach)(HI_UNF_HDMI_ID_E enHdmi);
typedef HI_S32  (*FN_HDMI_Suspend)(PM_BASEDEV_S *pdev, pm_message_t state);
typedef HI_S32  (*FN_HDMI_Resume)(PM_BASEDEV_S *pdev);

typedef HI_S32  (*FN_HDMI_SetHdrAttr)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_HDR_ATTR_S *pstHdrAttr);
typedef HI_S32  (*FN_HDMI_SetHdrMode)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_HDR_USERMODE_E enUserHdrMode);
typedef HI_S32  (*FN_HDMI_GetVideoCapability)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_VIDEO_CAPABILITY_S *pstVideoCap);

#if defined(HI_HDMI_SUPPORT_2_0)
typedef HI_S32  (*FN_HDMI_PreFormat)(HI_UNF_HDMI_ID_E enHdmi);
typedef HI_S32  (*FN_HDMI_SetFormat)(HI_UNF_HDMI_ID_E enHdmi, HDMI_VIDEO_ATTR_S *pstVideoAttr);
typedef HI_S32  (*FN_HDMI_Attach)(HI_UNF_HDMI_ID_E enHdmi, HDMI_VIDEO_ATTR_S *pstVideoAttr);
typedef HI_S32  (*FN_HDMI_SoftResume)(HDMI_VIDEO_ATTR_S *pstVideoAttr);


// 1.4b
#else
typedef HI_S32  (*FN_HDMI_PreFormat)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enEncodingFormat);
typedef HI_S32  (*FN_HDMI_SetFormat)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo);
typedef HI_S32  (*FN_HDMI_Attach)(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo);
typedef HI_S32  (*FN_HDMI_SoftResume)(HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo);
#endif

typedef struct
{
    FN_HDMI_Init                pfnHdmiInit;
    FN_HDMI_Deinit              pfnHdmiDeinit;
    FN_HDMI_Open                pfnHdmiOpen;
    FN_HDMI_Close               pfnHdmiClose;
    FN_HDMI_GetPlayStus         pfnHdmiGetPlayStus;
    FN_HDMI_GetAoAttr           pfnHdmiGetAoAttr;
    FN_HDMI_GetSinkCapability   pfnHdmiGetSinkCapability;
	FN_HDMI_GetAudioCapability  pfnHdmiGetAudioCapability;
    FN_HDMI_SetAudioMute        pfnHdmiSetAudioMute;
    FN_HDMI_SetAudioUnMute      pfnHdmiSetAudioUnMute;
    FN_HDMI_AudioChange         pfnHdmiAudioChange;
    FN_HDMI_PreFormat           pfnHdmiPreFormat;
    FN_HDMI_SetFormat           pfnHdmiSetFormat;
    FN_HDMI_Detach              pfnHdmiDetach;
    FN_HDMI_Attach              pfnHdmiAttach;
    FN_HDMI_Resume              pfnHdmiResume;
    FN_HDMI_Suspend             pfnHdmiSuspend;
    FN_HDMI_SoftResume          pfnHdmiSoftResume;
    FN_HDMI_GetVideoCapability  pfnHdmiGetVideoCapability;
    FN_HDMI_SetHdrAttr          pfnHdmiSetHdrAttr;
    FN_HDMI_SetHdrMode          pfnHdmiSetHdrMode;
    
}HDMI_EXPORT_FUNC_S;




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DISP_EXT_H__ */

