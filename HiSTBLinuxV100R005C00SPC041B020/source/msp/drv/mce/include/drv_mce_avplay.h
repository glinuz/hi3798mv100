#ifndef __DRV_MCE_AVPLAY_H__
#define __DRV_MCE_AVPLAY_H__

#include <linux/semaphore.h>

#include "hi_type.h"
#include "hi_mpi_vdec.h"
#include "hi_unf_avplay.h"
#include "hi_unf_common.h"
#include "hi_drv_sync.h"
#include "drv_mce_frc.h"
#include "drv_demux_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    AVPLAY_PROC_ADEC_AO,
    AVPLAY_PROC_DMX_ADEC,
    AVPLAY_PROC_VDEC_VO,
    AVPLAY_PROC_BUTT
} AVPLAY_PROC_ID_E;

typedef struct
{
    HI_HANDLE   hWindow;
    HI_HANDLE   hPort;
} AVPLAY_VID_PORT_AND_WIN_S;

typedef struct tagMCE_AVPLAY_S
{
    HI_UNF_AVPLAY_ATTR_S       AvplayAttr;
    HI_HANDLE                  hAvplay;

    HI_HANDLE                  hVdec;
    HI_UNF_VCODEC_ATTR_S       VdecAttr;
    HI_HANDLE                  hDmxVid;
    HI_U32                     DmxVidPid;

    HI_HANDLE                  hAdec;
    HI_U32                     AdecType; 

    HI_HANDLE                  hDmxAud;
    HI_U32                     DmxAudPid;
    
    HI_HANDLE                  hWindow;
    HI_HANDLE                  hSnd;

    HI_HANDLE                  hSync;

    HI_BOOL                    VidEnable;
    HI_BOOL                    AudEnable;

	HI_BOOL                    bStepMode;
    HI_BOOL                    bStepPlay;

	HI_BOOL					   bSetEosFlag;

    HI_BOOL                    AvplayProcContinue;
    HI_BOOL                    AvplayProcDataFlag[AVPLAY_PROC_BUTT];

    VDEC_ES_BUF_S              AvplayVidEsBuf;
    HI_UNF_STREAM_BUF_S        AvplayAudEsBuf;

    DMX_Stream_S               AvplayDmxEsBuf;
    
    AO_FRAMEINFO_S             AvplayAudFrm;
    SYNC_AUD_INFO_S            AudInfo;
    SYNC_AUD_OPT_S             AudOpt;

	HI_BOOL                    bFrcEnable;
    AVPLAY_FRC_CFG_S           FrcParamCfg;        /* config frc param */ /*CNcomment: 配置的frc参数 */
    AVPLAY_ALG_FRC_S           FrcCalAlg;          /* frc used rate info */ /*CNcomment: frc正在使用的帧率信息 */
    AVPLAY_FRC_CTRL_S          FrcCtrlInfo;        /* frc control */ /*CNcomment: frc控制信息 */

    HI_U32                     FrcNeedPlayCnt;     /* this frame need to play time*/ /*CNcomment:该帧需要播几次 */
    HI_U32                     FrcCurPlayCnt;     /* this frame had played time*/   /*CNcomment:该帧实际播到第几次*/

	HI_DRV_VIDEO_FRAME_PACKAGE_S CurFrmPack;
    HI_DRV_VIDEO_FRAME_PACKAGE_S LstFrmPack;

	HI_BOOL						bTBMatch;

	SYNC_VID_INFO_S            VidInfo;
    SYNC_VID_OPT_S             VidOpt;

	AVPLAY_VID_PORT_AND_WIN_S   MasterFrmChn;
    AVPLAY_VID_PORT_AND_WIN_S   SlaveFrmChn;
	HI_HANDLE                   hSharedOrgWin;  /*Original window of homologous*/
    
    HI_BOOL                    AvplayThreadRun;
    struct task_struct         *pAvplayThreadInst;
    struct semaphore           AvplayThreadMutex;
    struct semaphore           AvplayMutex;
    
}MCE_AVPLAY_S;

HI_S32 HI_DRV_AVPLAY_Init(HI_VOID);
HI_S32 HI_DRV_AVPLAY_DeInit(HI_VOID);
HI_S32 HI_DRV_AVPLAY_GetDefaultConfig(HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_UNF_AVPLAY_STREAM_TYPE_E enCfg);
HI_S32 HI_DRV_AVPLAY_Create(const HI_UNF_AVPLAY_ATTR_S *pstAvAttr, HI_HANDLE *phAvplay);
HI_S32 HI_DRV_AVPLAY_Destroy(HI_HANDLE hAvplay);
HI_S32 HI_DRV_AVPLAY_ChnOpen(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_VOID *pPara);
HI_S32 HI_DRV_AVPLAY_ChnClose(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_DRV_AVPLAY_SetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_DRV_AVPLAY_GetAttr(HI_HANDLE hAvplay, HI_UNF_AVPLAY_ATTR_ID_E enAttrID, HI_VOID *pPara);
HI_S32 HI_DRV_AVPLAY_Start(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn);
HI_S32 HI_DRV_AVPLAY_Stop(HI_HANDLE hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_E enChn, const HI_UNF_AVPLAY_STOP_OPT_S *pStop);
HI_S32 HI_DRV_AVPLAY_GetSyncVdecHandle(HI_HANDLE hAvplay, HI_HANDLE *phVdec, HI_HANDLE *phSync);
HI_S32 HI_DRV_AVPLAY_GetSndHandle(HI_HANDLE hAvplay, HI_HANDLE *phSnd);
HI_S32 HI_DRV_AVPLAY_AttachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow);
HI_S32 HI_DRV_AVPLAY_DetachWindow(HI_HANDLE hAvplay, HI_HANDLE hWindow);
HI_S32 HI_DRV_AVPLAY_AttachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd);
HI_S32 HI_DRV_AVPLAY_DetachSnd(HI_HANDLE hAvplay, HI_HANDLE hSnd);
HI_S32 HI_DRV_AVPLAY_IsBufEmpty(HI_HANDLE hAvplay, HI_BOOL *pbIsEmpty);
HI_S32 HI_DRV_AVPLAY_Step(HI_HANDLE hAvplay, const HI_UNF_AVPLAY_STEP_OPT_S *pstStepOpt);
HI_S32 HI_DRV_AVPLAY_GetBuf(HI_HANDLE  hAvplay,
                            HI_UNF_AVPLAY_BUFID_E enBufId,
                            HI_U32                u32ReqLen,
                            HI_UNF_STREAM_BUF_S  *pstData,
                            HI_U32                u32TimeOutMs);

HI_S32 HI_DRV_AVPLAY_PutBuf(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId,
                                       HI_U32 u32ValidDataLen, HI_U32 u32Pts, HI_UNF_AVPLAY_PUTBUFEX_OPT_S *pstExOpt);

HI_S32 HI_DRV_AVPLAY_ProcStatus(HI_HANDLE hAvplay, HI_UNF_AVPLAY_BUFID_E enBufId, HI_BOOL *pbStatus);
HI_S32 HI_DRV_AVPLAY_FlushStream(HI_HANDLE hAvplay);
HI_S32 HI_DRV_AVPLAY_SetProgressive(HI_HANDLE hAvplay, HI_BOOL bProgressive);

#ifdef __cplusplus
}
#endif

#endif

