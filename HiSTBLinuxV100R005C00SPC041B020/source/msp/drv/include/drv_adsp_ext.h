#ifndef __DRV_ADSP_EXT_H__
#define __DRV_ADSP_EXT_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include "hi_drv_dev.h"
#include "hi_type.h"
#include "hi_audsp_common.h"

typedef union hiADSP_VERSIONTYPE_U
{
    struct
    {
        HI_U8 u8VersionMajor;      /**< Major version */
        HI_U8 u8VersionMinor;      /**< Minor version */
        HI_U8 u8Revision;          /**< Revision version */
        HI_U8 u8Step;              /**< Step version */
    } s;
    HI_U32 u32Version;
} ADSP_VERSIONTYPE_U;

typedef struct 
{
    /**< IN. dsp code Id */
    ADSP_CODEID_E u32DspCodeId;

    /**< IN. dsp code version */
    ADSP_VERSIONTYPE_U uVersion;

    /**< IN. Description infomation of the code */
    HI_PCHAR pszDescription;
} ADSP_FIRMWARE_INFO_S;

typedef struct 
{
    HI_U32  u32AoeRegBaseAddr;

    HI_BOOL bAoeSwFlag;
} ADSP_FIRMWARE_AOE_INFO_S;

typedef HI_S32 (*FN_ADSP_DrvResume)(PM_BASEDEV_S *);

typedef HI_S32 (*FN_ADSP_DrvSuspend)(PM_BASEDEV_S * , pm_message_t);

typedef HI_S32 (*FN_ADSP_LoadFrm)(ADSP_CODEID_E);

typedef HI_S32 (*FN_ADSP_UnLoadFrm)(ADSP_CODEID_E);

typedef HI_S32 (*FN_ADSP_GetFrmInfo)(ADSP_CODEID_E, ADSP_FIRMWARE_INFO_S *);

typedef HI_S32 (*FN_ADSP_GetAoeFwmInfo)(ADSP_CODEID_E, ADSP_FIRMWARE_AOE_INFO_S*);

typedef struct tagADSP_EXPORT_FUNC_S
{
    FN_ADSP_LoadFrm pfnADSP_LoadFirmware;
    FN_ADSP_UnLoadFrm pfnADSP_UnLoadFirmware;
    FN_ADSP_GetFrmInfo pfnADSP_GetFirmwareInfo;
    FN_ADSP_GetAoeFwmInfo pfnADSP_GetAoeFwmInfo;

    FN_ADSP_DrvResume  pfnADSP_DrvResume;
    FN_ADSP_DrvSuspend pfnADSP_DrvSuspend;
} ADSP_EXPORT_FUNC_S;

HI_S32  ADSP_DRV_ModInit(HI_VOID);
HI_VOID ADSP_DRV_ModExit(HI_VOID);

#if defined(HI_MCE_SUPPORT)
HI_S32  HI_DRV_ADSP_Init(HI_VOID);
HI_VOID HI_DRV_ADSP_DeInit(HI_VOID);
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __DRV_ADSP_EXT_H__ */
