#ifndef __DRV_VPSS_EXT_H__
#define __DRV_VPSS_EXT_H__

#include "hi_type.h"
#include "hi_drv_vpss.h"
#include "hi_drv_dev.h"
#include "drv_pq_define.h"

typedef HI_S32  (*FN_VPSS_GlobalInit)(HI_VOID);
typedef HI_S32  (*FN_VPSS_GlobalDeInit)(HI_VOID);
typedef HI_S32  (*FN_VPSS_GetDefaultCfg)(HI_DRV_VPSS_CFG_S *pstVpssCfg);
typedef HI_S32  (*FN_VPSS_CreateVpss)(HI_DRV_VPSS_CFG_S *pstVpssCfg,VPSS_HANDLE *phVPSS);
typedef HI_S32  (*FN_VPSS_DestroyVpss)(VPSS_HANDLE hVPSS);


typedef HI_S32  (*FN_VPSS_SetVpssCfg)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);
typedef HI_S32  (*FN_VPSS_GetVpssCfg)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_CFG_S *pstVpssCfg);

typedef HI_S32  (*FN_VPSS_GetDefaultPortCfg)(HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
typedef HI_S32  (*FN_VPSS_CreatePort)(VPSS_HANDLE hVPSS,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg,VPSS_HANDLE *phPort);
typedef HI_S32  (*FN_VPSS_DestroyPort)(VPSS_HANDLE hPort);

typedef HI_S32  (*FN_VPSS_GetPortCfg)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
typedef HI_S32  (*FN_VPSS_SetPortCfg)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);

typedef HI_S32  (*FN_VPSS_EnablePort)(VPSS_HANDLE hPort, HI_BOOL bEnable);

typedef HI_S32  (*FN_VPSS_SendCommand)(VPSS_HANDLE hVPSS, HI_DRV_VPSS_USER_COMMAND_E eCommand, HI_VOID *pArgs);

typedef HI_S32  (*FN_VPSS_GetPortFrame)(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);  

typedef HI_S32  (*FN_VPSS_RelPortFrame)(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);

typedef HI_S32  (*FN_VPSS_GetPortBufListState)(VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstVpssBufListState);
typedef HI_S32  (*FN_VPSS_CheckPortBufListFul)(VPSS_HANDLE hPort);
typedef HI_S32  (*FN_VPSS_SetSourceMode)(VPSS_HANDLE hVPSS,
                          HI_DRV_VPSS_SOURCE_MODE_E eSrcMode,
                          HI_DRV_VPSS_SOURCE_FUNC_S* pstRegistSrcFunc);

typedef HI_S32  (*FN_VPSS_PutImage)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);
typedef HI_S32  (*FN_VPSS_GetImage)(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImage);
typedef HI_S32  (*FN_VPSS_RegistHook)(VPSS_HANDLE hVPSS, HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);
typedef HI_S32  (*FN_VPSS_UpdatePqData)(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);

typedef HI_S32  (*FN_VPSS_Suspend)(PM_BASEDEV_S *pdev, pm_message_t state);
typedef HI_S32  (*FN_VPSS_Resume)(PM_BASEDEV_S *pdev);

typedef struct
{
    FN_VPSS_GlobalInit      pfnVpssGlobalInit;
    FN_VPSS_GlobalDeInit    pfnVpssGlobalDeInit;
    
    FN_VPSS_GetDefaultCfg   pfnVpssGetDefaultCfg;
    FN_VPSS_CreateVpss      pfnVpssCreateVpss;
    FN_VPSS_DestroyVpss     pfnVpssDestroyVpss;
    FN_VPSS_SetVpssCfg      pfnVpssSetVpssCfg;
    FN_VPSS_GetVpssCfg      pfnVpssGetVpssCfg;

    FN_VPSS_GetDefaultPortCfg   pfnVpssGetDefaultPortCfg;
    FN_VPSS_CreatePort      pfnVpssCreatePort;
    FN_VPSS_DestroyPort     pfnVpssDestroyPort;
    FN_VPSS_GetPortCfg      pfnVpssGetPortCfg;
    FN_VPSS_SetPortCfg      pfnVpssSetPortCfg;
    FN_VPSS_EnablePort      pfnVpssEnablePort;

    FN_VPSS_SendCommand     pfnVpssSendCommand;

    FN_VPSS_GetPortFrame    pfnVpssGetPortFrame;     
    FN_VPSS_RelPortFrame    pfnVpssRelPortFrame;

    FN_VPSS_GetPortBufListState     pfnVpssGetPortBufListState;
    FN_VPSS_CheckPortBufListFul     pfnVpssCheckPortBufListFul;

    FN_VPSS_SetSourceMode   pfnVpssSetSourceMode;
    FN_VPSS_PutImage        pfnVpssPutImage;
    FN_VPSS_GetImage        pfnVpssGetImage;

    FN_VPSS_RegistHook      pfnVpssRegistHook;
    FN_VPSS_UpdatePqData  pfnVpssUpdatePqData;

    FN_VPSS_Resume          pfnVpssResume;
    FN_VPSS_Suspend          pfnVpssSuspend;

} VPSS_EXPORT_FUNC_S;


HI_S32 HI_DRV_VPSS_Init(HI_VOID);
HI_VOID HI_DRV_VPSS_Exit(HI_VOID);

HI_S32 VPSS_DRV_ModInit(HI_VOID);
HI_VOID VPSS_DRV_ModExit(HI_VOID);

#endif

