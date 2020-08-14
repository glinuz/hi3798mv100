#ifndef __DRV_VI_EXT_H__
#define __DRV_VI_EXT_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif
//add by l00214567
HI_S32 HI_VI_Chan_VOAcqFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame);
HI_S32 HI_VI_Chan_VORlsFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstVpssFrame);
HI_S32 HI_VI_Chan_VOChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo);

typedef int (*PFN_VI_Chan_VOAcqFrame)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VI_Chan_VORlsFrame)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VI_Chan_VOChangeWinInfo)(HI_HANDLE,HI_DRV_WIN_PRIV_INFO_S*);
typedef int (*PFN_VI_DRV_RlsImage)(HI_HANDLE, HI_DRV_VIDEO_FRAME_S*);
typedef int (*PFN_VI_DRV_ChangeVencInfo)(HI_HANDLE, HI_U32, HI_U32, HI_U32);


typedef struct
{
	PFN_VI_Chan_VOAcqFrame      pfnVOAcqFrame;
    PFN_VI_Chan_VORlsFrame      pfnVORlsFrame;
    PFN_VI_Chan_VOChangeWinInfo pfnVOSendWinInfo;
    PFN_VI_DRV_RlsImage         pfnVencRlsFrame;
    PFN_VI_DRV_ChangeVencInfo   pfnDispSetCastAttr;
} VI_EXPORT_FUNC_S;

HI_S32	VI_DRV_ModInit(HI_VOID);
HI_VOID VI_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif //__DRV_VI_EXT_H__
