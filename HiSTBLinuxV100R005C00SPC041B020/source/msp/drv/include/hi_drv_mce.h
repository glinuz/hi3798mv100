#ifndef __HI_DRV_MCE_H__
#define __HI_DRV_MCE_H__

#include "hi_type.h"
#include "hi_module.h"
#include "hi_unf_mce.h"
#include "hi_drv_demux.h"
#include "hi_unf_disp.h"
#include "hi_unf_common.h"
#include "hi_go_surface.h"
#include "hi_flash.h"
#include "hi_debug.h"
#include "hi_drv_pdm.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 0
#define HI_FATAL_MCE(format, arg...)    HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_ERR_MCE(format, arg...)      HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_WARN_MCE(format, arg...)     //HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_MCE(format, arg...)     //HI_PRINT( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#else
#define HI_FATAL_MCE(format...)    HI_FATAL_PRINT(HI_ID_FASTPLAY, format)
#define HI_ERR_MCE(format...)      HI_ERR_PRINT(HI_ID_FASTPLAY, format)
#define HI_WARN_MCE(format...)     HI_WARN_PRINT(HI_ID_FASTPLAY, format)
#define HI_INFO_MCE(format...)     HI_INFO_PRINT(HI_ID_FASTPLAY, format)
#endif

typedef enum tagIOC_MCE_E
{
    IOC_MCE_STOP_MCE,
    IOC_MCE_EXIT_MCE,
    IOC_MCE_CLEAR_LOGO,
    IOC_MCE_BUTT
}IOC_MCE_E;

#define HI_MTD_NAME_LEN     32

typedef struct tagMCE_S
{
    HI_HANDLE               hAvplay;
    HI_HANDLE               hWindow;
    HI_HANDLE               hSnd;
    HI_DISP_PARAM_S         stDispParam;
    HI_MCE_PARAM_S          stMceParam;

    HI_BOOL                 bPlayStop;
    HI_UNF_MCE_STOPPARM_S   stStopParam;

    HI_BOOL                 bMceExit;

    HI_U32                  BeginTime;
    HI_U32                  EndTime;

    HI_BOOL                 playEnd;

    HI_BOOL                 bTsThreadStop;
    HI_BOOL                 bAniThreadStop;

    DMX_MMZ_BUF_S           TsBuf;

    HI_U8                   *PlayDataAddr;

    struct task_struct      *pPlayTask;
}MCE_S;


#define HI_MCE_STOP_FASTPLAY_CMD        _IOW(HI_ID_FASTPLAY, IOC_MCE_STOP_MCE, HI_UNF_MCE_STOPPARM_S)
#define HI_MCE_EXIT_FASTPLAY_CMD        _IOW(HI_ID_FASTPLAY, IOC_MCE_EXIT_MCE, HI_UNF_MCE_EXITPARAM_S)
#define HI_MCE_CLEAR_LOGO_CMD           _IO(HI_ID_FASTPLAY, IOC_MCE_CLEAR_LOGO)

HI_S32 HI_DRV_MCE_Stop(HI_UNF_MCE_STOPPARM_S *pstStopParam);
HI_S32 HI_DRV_MCE_Exit(HI_UNF_MCE_EXITPARAM_S *pstExitParam);

#if    !defined(CHIP_TYPE_hi3716mv410)   \
    && !defined(CHIP_TYPE_hi3716mv420)   \
    && !defined(CHIP_TYPE_hi3798cv200)   \
    && !defined(CHIP_TYPE_hi3798mv200_a)   \
    && !defined(CHIP_TYPE_hi3798mv200)   
    
    #define MCE_ACTIVE_MAIN_AND_SLAVE_WIN_SUPPORT
#endif

#if    defined(CHIP_TYPE_hi3798cv200) \
    || defined(CHIP_TYPE_hi3798mv200) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined(CHIP_TYPE_hi3796mv100) \
    || defined(CHIP_TYPE_hi3716dv100) \
    || defined(CHIP_TYPE_hi3798mv200_a)

    #define MCE_FRAME_RATE_ADJUST_SUPPORT
#endif

#ifdef __cplusplus
}
#endif

#endif
