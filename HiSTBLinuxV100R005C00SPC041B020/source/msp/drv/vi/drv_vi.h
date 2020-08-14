#ifndef __DRV_VI_H__
#define __DRV_VI_H__

#include <linux/semaphore.h>

#include "hi_osal.h"
#include "drv_vi_ioctl.h"
#include "drv_vpss_ext.h"
#include "drv_vi_utils.h"
#include "drv_vi_buf.h"
#include "hi_drv_dev.h"
#include "hi_drv_stat.h"
#include "hi_drv_sys.h"

#include "drv_vi_reg_hi3716cv200.h"

#define GET_PORT_CHN(hVi, enPort, ViChn) \
    do { \
        enPort = (HI_UNF_VI_E)(((hVi) & 0xff00) >> 8); \
        ViChn = ((hVi) & 0xff); \
        if (MAX_VI_CHN <= ViChn) \
        { \
            HI_ERR_VI("VI handle(%#x) is invalid, channel error.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
        if (HI_UNF_VI_BUTT <= enPort)\
        {\
            HI_ERR_VI("VI handle(%#x) is invalid, port error.\n", (hVi)); \
            return HI_ERR_VI_CHN_NOT_EXIST; \
        } \
    } while (0)

typedef struct hiVI_STATISTIC_S
{
    HI_U32 Cnt;
    HI_U32 CntLast;
    HI_U32 Unload;

    HI_U32 YBusError;
    HI_U32 CBusError;
    HI_U32 UpdateReg;
    HI_U32 FieldThrow;
    HI_U32 BufOverflow;
    HI_U32 CcInt;
    HI_U32 FrmStart;
    HI_U32 ZeroInt;
    HI_U32 TopField;
    HI_U32 BtmField;

    HI_U32 DequeueTry;
    HI_U32 DequeueOK;

    HI_U32 QueueTry;
    HI_U32 QueueOK;
    HI_U32 QueueOKLast;

    HI_U32 AcquireTry;
    HI_U32 AcquireOK;

    HI_U32 ReleaseTry;
    HI_U32 ReleaseOK;

    HI_U32 UsrAcqTry;
    HI_U32 UsrAcqOK;

    HI_U32 UsrRlsTry;
    HI_U32 UsrRlsOK;

    HI_U32 GetTry;
    HI_U32 GetOK;

    HI_U32 PutTry;
    HI_U32 PutOK;

    HI_U32 QWinTry;
    HI_U32 QWinOK;

    HI_U32 DqWinTry;
    HI_U32 DqWinOK;

    HI_U32 QVencTry;
    HI_U32 QVencOK;

    HI_U32 DqVencTry;
    HI_U32 DqVencOK;
} VI_STATISTIC_S;

typedef struct hiVI_THREAD_S
{
    struct task_struct *pViThreadInst;
    HI_VOID *pLock;
} VI_THREAD_S;

typedef struct hiVI_DRV_S
{
    HI_HANDLE                 hVi;
    HI_HANDLE                 hVicap;
    VPSS_HANDLE               hVpss;
    HI_VOID                  *fileOpened;
    HI_UNF_VI_ATTR_S          stAttr;
    VI_FB_ROOT_S              stFrameBuf;
    VPSS_EXPORT_FUNC_S       *pVpssFunc;
    VI_STATISTIC_S            stStat;
    VI_THREAD_S               stThread;
    VI_EVENT_S                waitFrame;
    VI_REG_S                 *pBaseReg;
    HI_U32                    u32NextIndex;
    HI_BOOL                   bStarted;
    HI_BOOL                   bProgressive;
    HI_BOOL                   bLowDelayStat;
    HI_U32                    u32FrameCnt;
    HI_U32                    u32FrameRate;
    VI_VPSS_PORT_PARAM_S      stPortParam[VI_MAX_VPSS_PORT];
    HI_UNF_VIDEO_FRAME_INFO_S stFrame[MAX_VI_FB_NUM];
    struct timer_list         viTimer;
} VI_DRV_S;

HI_S32                VI_DRV_Open(struct inode *finode, struct file  *ffile);
HI_S32                VI_DRV_Close(struct inode *finode, struct file  *ffile);
long                  VI_DRV_Ioctl(struct file *file, unsigned int cmd, unsigned long arg);
HI_S32                VI_DRV_Ioctl_k(struct inode *inode, struct file  *file, unsigned int cmd, HI_VOID *arg);
HI_S32                VI_DRV_Suspend(PM_BASEDEV_S *pdev, pm_message_t state);
HI_S32                VI_DRV_Resume(PM_BASEDEV_S *pdev);

HI_S32                VI_Convert_FrameInfo(HI_UNF_VIDEO_FRAME_INFO_S *pstUnfFrm, HI_DRV_VIDEO_FRAME_S *pstDrvFrm);
HI_S32                VI_Convert_WinInfo(HI_DRV_WIN_PRIV_INFO_S* pstWinInfo, HI_DRV_VPSS_CFG_S *pstVpssCfg,
                                         HI_DRV_VPSS_PORT_CFG_S* pstPortCfg);

HI_S32                VI_Vpss_Event(HI_HANDLE hVi, HI_DRV_VPSS_EVENT_E enEventID, HI_VOID *pstArgs);
HI_S32                VI_Vpss_Thread(HI_VOID *Arg);
HI_S32                VI_Vpss_Create(HI_HANDLE hVi, const HI_UNF_VI_ATTR_S *pstViAttr, VPSS_HANDLE *phVpss);
HI_S32                VI_Vpss_Destroy(HI_HANDLE hVi, VPSS_HANDLE hVpss);

HI_VOID               VI_PHY_BoardInit(HI_UNF_VI_INPUT_MODE_E enInputMode);
HI_VOID               VI_PHY_BoardDeinit(HI_VOID);
HI_VOID               VI_PHY_BoardDeinit(HI_VOID);
HI_VOID               VI_PHY_Destroy(HI_HANDLE hVi);
irqreturn_t           VI_PHY_InterruptHandler(int irq, void *dev_id);

HI_S32                VI_DRV_ReleaseImage(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S* pstFrame);
HI_S32                VI_DRV_ChangeVencInfo(HI_HANDLE hPort, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32FrmRate);

HI_S32                VI_DRV_ChangeWinInfo(HI_HANDLE hPort, HI_DRV_WIN_PRIV_INFO_S* pstWinInfo);
HI_S32                VI_DRV_AcquireFrame(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S  *pstFrame);
HI_S32                VI_DRV_ReleaseFrame(VPSS_HANDLE hVpss, HI_DRV_VIDEO_FRAME_S  *pstFrame);
HI_U32                VI_DRV_GetFps(HI_UNF_VI_INPUT_MODE_E enInputMode);
HI_S32                VI_DRV_GetWH(HI_UNF_VI_INPUT_MODE_E enInputMode, HI_U32 *pu32Width , HI_U32 *pu32Height);
HI_S32                VI_DRV_DestroyForce(HI_HANDLE hVi);

HI_VOID               VI_DRV_InitParam(HI_VOID);
HI_VOID               VI_DRV_DeInitParam(struct file *file);

HI_S32                VI_DRV_ProcAdd(HI_HANDLE hVi);
HI_S32                VI_DRV_ProcDel(HI_HANDLE hVi);
HI_S32                VI_DRV_ProcTimer(HI_LENGTH_T data);

#endif // __DRV_VI_H__
