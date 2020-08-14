#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <linux/time.h>
#include <asm/uaccess.h>
#include <asm/io.h>
//#include <mach/hardware.h>  //@f00241306 64bit

#include "hi_unf_avplay.h"
#include "hi_error_mpi.h"

#include "vfmw.h"
#include "drv_vdec_ioctl.h"
#include "hi_drv_vdec.h"
#include "drv_vdec_private.h"
#include "hi_drv_stat.h"

// add by w00278582
#define MAX_VID_PROTOCOL_NAME 20
#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* End of #ifdef __cplusplus */

extern HI_U32 g_CHAN_FRAME_RATE[HI_VDEC_MAX_INSTANCE_NEW];

static HI_S32   VDEC_DRV_CtrlWriteProc(struct file * file,
                                   const char __user * buf, size_t count, loff_t *ppos);
static HI_S32   VDEC_DRV_CtrlReadProc(struct seq_file *p, HI_VOID *v);

static HI_S32   VDEC_DRV_WriteProc(struct file * file,
                                   const char __user * buf, size_t count, loff_t *ppos);
static HI_S32   VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v);

static long     VDEC_DRV_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static HI_CHAR *s_aszVdecType[HI_UNF_VCODEC_TYPE_BUTT + 1] =
{
    "MPEG2",
    "MPEG4",
    "AVS",
    "H263",
    "H264",
    "REAL8",
    "REAL9",
    "VC1",
    "VP6",
    "VP6F",
    "VP6A",
    "OTHER",
    "SORENSON",
    "DIVX3",
    "RAW",
    "JPEG",
    "VP8",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "OTHER",
    "HEVC",
    "OTHER",
    "VP9",
    "UNKNOWN",
};

static struct file_operations VdecFileOpts =
{
    .owner          = THIS_MODULE,
    .open           = VDEC_DRV_Open,
    .unlocked_ioctl = VDEC_DRV_Ioctl,/*user space and kernel space same with 32 bit or 64 bit run this function*/
#ifdef CONFIG_COMPAT
    .compat_ioctl   = VDEC_DRV_Ioctl,/*user space 32bit and kernel space 64 bit run this function*/
#endif
    .release        = VDEC_DRV_Release,
};

static PM_BASEOPS_S VdecOps =
{
    .probe          = NULL,
    .remove         = NULL,
    .shutdown       = NULL,
    .prepare        = NULL,
    .complete       = NULL,
    .suspend        = VDEC_DRV_Suspend,
    .suspend_late   = NULL,
    .resume_early   = NULL,
    .resume         = VDEC_DRV_Resume,
};

static VDEC_REGISTER_PARAM_S s_stProcParam =
{
    .pfnCtrlReadProc  = VDEC_DRV_CtrlReadProc,
    .pfnCtrlWriteProc = VDEC_DRV_CtrlWriteProc,
    .pfnReadProc      = VDEC_DRV_ReadProc,
    .pfnWriteProc     = VDEC_DRV_WriteProc,
};

/* save raw/yuv param */
static HI_S8  VdecSavePath[256] = {'/','m','n','t',0};
extern HI_U32 MaskCtrlWord;
extern HI_S32 VdecRawChanNum;
extern HI_S32 VdecYuvChanNum;

HI_BOOL bSaveoneyuv  = HI_FALSE;
HI_U32  u32SaveCnt = 0;
static long VDEC_DRV_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret;

    ret = (long)HI_DRV_UserCopy(filp->f_dentry->d_inode, filp, cmd, arg, VDEC_Ioctl);
    return ret;
}

static __inline__ int str2val(char *str, unsigned int *data)
{
    unsigned int i, d, dat, weight;

    dat = 0;
    if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {
        i = 2;
        weight = 16;
    }
    else
    {
        i = 0;
        weight = 10;
    }

    for(; i < 10; i++)
    {
        if (str[i] < 0x20)
        {
            break;
        }
        else if (weight == 16 && str[i] >= 'a' && str[i] <= 'f')
        {
            d = str[i] - 'a' + 10;
        }
        else if (weight == 16 && str[i] >= 'A' && str[i] <= 'F')
        {
            d = str[i] - 'A' + 10;
        }
        else if (str[i] >= '0' && str[i] <= '9')
        {
            d = str[i] - '0';
        }
        else
        {
            return -1;
        }

        dat = dat * weight + d;
    }

    *data = dat;

    return 0;
}

static HI_S32 VDEC_DRV_CtrlReadProc(struct seq_file *p, HI_VOID *v)
{
#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum
    HI_U32 i;
    VDEC_SPECIAL_FRM_PROC_S stSpecialFrmProc;
    memset(&stSpecialFrmProc, 0, sizeof(VDEC_SPECIAL_FRM_PROC_S));
    VDEC_DRV_GetSpecialFrmInfo(&stSpecialFrmProc);
#endif

    PROC_PRINT(p, "\n");
    PROC_PRINT(p, "%-35s:%d\n", "VDEC VERSION",   VDEC_VERSION);
    PROC_PRINT(p, "%-35s:%d\n", "MaskCtrlWord",   MaskCtrlWord);
    PROC_PRINT(p, "%-35s:%s\n", "VdecSavePath",   VdecSavePath);
    PROC_PRINT(p, "%-35s:%d\n", "VdecRawChanNum", VdecRawChanNum);
    PROC_PRINT(p, "%-35s:%d\n", "VdecYuvChanNum", VdecYuvChanNum);
#ifdef VFMW_VPSS_BYPASS_EN   //specialFrameExist->specialFrameNum
    PROC_PRINT(p, "%-35s:%d\n", "SpeclalFrmNum",  stSpecialFrmProc.u32SpecialFrmNum);

    if (stSpecialFrmProc.u32SpecialFrmNum != 0)
    {
        for (i = 0; i < stSpecialFrmProc.u32SpecialFrmNum; i++)
        {
            PROC_PRINT(p, "%-35s:0x%x/%p/%d\n", "PhyAddr/VirAddr/Size",
                          stSpecialFrmProc.frmBufRec[i].u32StartPhyAddr,
                          stSpecialFrmProc.frmBufRec[i].pu8StartVirAddr,
                          stSpecialFrmProc.frmBufRec[i].u32Size);
        }
    }

#endif
    PROC_PRINT(p, "\n");
    PROC_PRINT(p, "================== param in () is not necessary ==================\n");
    PROC_PRINT(p, "echo  dat0        dat1      dat2   > /proc/msp/vdec_ctrl\n");
    PROC_PRINT(p, "echo  savestream  handle   (path)  -- turn on/off raw save\n");
    PROC_PRINT(p, "echo  saverpu     handle   (path)  -- turn on/off rpu save\n");
    PROC_PRINT(p, "echo  saveyuv     handle   (path)  -- turn on/off yuv save\n");
    PROC_PRINT(p, "echo  saveoneyuv  handle   (path)  -- turn on/off oneyuv save\n");
    PROC_PRINT(p, "echo  maskdfs     on/off           -- enable/disable mask dynamic fs\n");
    PROC_PRINT(p, "echo  maskcmp     on/off           -- enable/disable mask compress\n");
    PROC_PRINT(p, "echo  setfps      handle   fps     -- set frame rate\n");
    PROC_PRINT(p, "===================================================================\n");

    return 0;
}

static HI_S32 VDEC_DRV_GetProcArg(HI_CHAR*  chCmd,HI_CHAR*  chArg,HI_U32 u32ArgIndex)
    {
        HI_U32 u32Count;
        HI_U32 u32CmdCount;
        HI_U32 u32LogCount;
        HI_U32 u32NewFlag;
        HI_CHAR chArg1[VDEC_MAX_PROC_ARGS_SIZE] = {0};
        HI_CHAR chArg2[VDEC_MAX_PROC_ARGS_SIZE] = {0};
        HI_CHAR chArg3[VDEC_MAX_PROC_ARGS_SIZE*8] = {0};

        /*clear empty space*/
        u32Count = 0;
        u32CmdCount = 0;
        u32LogCount = 1;
        u32NewFlag = 0;
        while(chCmd[u32Count] != 0 && chCmd[u32Count] != '\n' )
        {
            if (chCmd[u32Count] != ' ')
            {
                u32NewFlag = 1;
            }
            else
            {
                if(u32NewFlag == 1)
                {
                    u32LogCount++;
                    u32CmdCount= 0;
                    u32NewFlag = 0;
                }
            }

            if (u32NewFlag == 1)
            {
                switch(u32LogCount)
                {
                    case 1:
                        chArg1[u32CmdCount] = chCmd[u32Count];
                        u32CmdCount++;
                        break;
                    case 2:
                        chArg2[u32CmdCount] = chCmd[u32Count];
                        u32CmdCount++;
                        break;
                    case 3:
                        chArg3[u32CmdCount] = chCmd[u32Count];
                        u32CmdCount++;
                        break;
                    default:
                        break;
                }

            }
            u32Count++;
        }

        switch(u32ArgIndex)
        {
            case 1:
                memcpy(chArg,chArg1,sizeof(HI_CHAR)*VDEC_MAX_PROC_ARGS_SIZE);
                break;
            case 2:
                memcpy(chArg,chArg2,sizeof(HI_CHAR)*VDEC_MAX_PROC_ARGS_SIZE);
                break;
            case 3:
                memcpy(chArg,chArg3,sizeof(HI_CHAR)*VDEC_MAX_PROC_ARGS_SIZE*8);
                break;
            default:
                break;
        }
        return HI_SUCCESS;
    }

static HI_VOID SaveStream(HI_CHAR* pdat2, HI_CHAR* pdat3, HI_HANDLE handle)
{
    if (pdat2[0] != 0)
    {
        str2val(pdat2, &handle);

        if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            HI_ERR_VDEC("ERROR:to large handle %x.\n", handle);

            return;
        }
    }

    if (pdat3[0] != 0)
    {
        strncpy(VdecSavePath, pdat3, sizeof(VdecSavePath));
        VdecSavePath[sizeof(VdecSavePath) - 1] = '\0';
    }

    if (BUFMNG_CheckFile(handle, 0) == HI_FALSE)
    {
        HI_S8 str[80];
        struct tm now;

        time_to_tm(get_seconds(), 0, &now);

        snprintf(str, sizeof(str), "%s/vdec%02u_%02u_%02u_%02u.es",
                 (char*)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);

        if (BUFMNG_OpenFile(handle, str, 0) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed create file '%s' for raw stream save!\n", str);
        }
        else
        {
        }
    }
    else if (BUFMNG_CheckFile(handle, 0) == HI_TRUE)
    {
        if (BUFMNG_CloseFile(handle, 0) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed close file for vdec%2d raw stream save!\n", handle);
        }
        else
        {
        }
    }

    return;
}

static HI_VOID SaveRPU(HI_CHAR* pdat2, HI_CHAR* pdat3, HI_HANDLE handle)
{
    if (pdat2[0] != 0)
    {
        str2val(pdat2, &handle);

        if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            HI_ERR_VDEC("[%s]ERROR:to large handle %x.\n", __func__, handle);

            return;
        }
    }

    if (pdat3[0] != 0)
    {
        strncpy(VdecSavePath, pdat3, sizeof(VdecSavePath));
        VdecSavePath[sizeof(VdecSavePath) - 1] = '\0';
    }

    if (BUFMNG_CheckFile(handle, 2) == HI_FALSE)
    {
        HI_S8 str[80];
        struct tm now;

        time_to_tm(get_seconds(), 0, &now);

        snprintf(str, sizeof(str), "%s/vdec%02u_%02u_%02u_%02u.rpu",
                 (char*)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);

        if (BUFMNG_OpenFile(handle, str, 2) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed create file '%s' for rpu save!\n", str);
        }
    }
    else if (BUFMNG_CheckFile(handle, 2) == HI_TRUE)
    {
        if (BUFMNG_CloseFile(handle, 2) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed close file for vdec%2d rpu save!\n", handle);
        }
    }

    return;
}

static HI_VOID SaveYUV(HI_CHAR* pdat2, HI_CHAR* pdat3, HI_HANDLE handle)
{
    if (pdat2[0] != 0)
    {
        str2val(pdat2, &handle);

        if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            HI_ERR_VDEC("[%s]ERROR:to large handle %x.\n", __func__, handle);
            return;
        }
    }

    if (pdat3[0] != 0)
    {
        strncpy(VdecSavePath, pdat3, sizeof(VdecSavePath));
        VdecSavePath[sizeof(VdecSavePath) - 1] = '\0';
    }

    bSaveoneyuv = HI_FALSE;

    if (BUFMNG_CheckFile(handle, 1) == HI_FALSE)
    {
        HI_S8 str[80];

        struct tm now;

        time_to_tm(get_seconds(), 0, &now);

        snprintf(str, sizeof(str), "%s/vdec%02u_%02u_%02u_%02u.yuv",
                 (char*)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);

        if (BUFMNG_OpenFile(handle, str, 1) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed create file '%s' for yuv save!\n", str);
        }
        else
        {
        }
    }
    else if (BUFMNG_CheckFile(handle, 1) == HI_TRUE)
    {
        if (BUFMNG_CloseFile(handle, 1) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed close file for vdec%2d yuv save!\n", handle);
        }
        else
        {
        }
    }

    return;
}

static HI_VOID SaveOneYUVFrame(HI_CHAR* pdat2, HI_CHAR* pdat3, HI_HANDLE handle)
{
    if (pdat2[0] != 0)
    {
        str2val(pdat2, &handle);

        if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            HI_ERR_VDEC("[%s]ERROR:to large handle %x.\n", __func__, handle);

            return;
        }
    }

    if (pdat3[0] != 0)
    {
        strncpy(VdecSavePath, pdat3, sizeof(VdecSavePath));
        VdecSavePath[sizeof(VdecSavePath) - 1] = '\0';
    }

    bSaveoneyuv = HI_TRUE;
    u32SaveCnt = 0;

    if (BUFMNG_CheckFile(handle, 1) == HI_FALSE)
    {
        HI_S8 str[80];
        struct tm now;

        time_to_tm(get_seconds(), 0, &now);

        snprintf(str, sizeof(str), "%s/vdec%02u_%02u_%02u_%02u.yuv",
                 (char*)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);

        if (BUFMNG_OpenFile(handle, str, 1) != HI_SUCCESS)
        {
            HI_FATAL_VDEC("FATAL: failed create file '%s' for yuv save!\n", str);
        }
    }

    return;
}

static HI_VOID SetMaskBit(HI_CHAR* pdat, HI_CHAR* pMaskName, HI_U8 bit)
{
    HI_U32  MaskBit  = 0;

    MaskBit = (1 << bit);

    if (0 == strncmp(pdat, "on", 2))
    {
        MaskCtrlWord = MaskCtrlWord | (MaskBit);
        HI_DRV_PROC_EchoHelper("Mask %s .\n", pMaskName);
    }
    else if (0 == strncmp(pdat, "off", 3))
    {
        MaskCtrlWord = MaskCtrlWord & (~MaskBit);
        HI_DRV_PROC_EchoHelper("Unmask %s .\n", pMaskName);
    }
    else
    {
    }

    return;
}

static HI_VOID SetFps(HI_CHAR* pdat2, HI_CHAR* pdat3, HI_HANDLE handle)
{
    HI_U32 fps = 0;

    if (pdat2[0] != 0)
    {
        str2val(pdat2, &handle);
        if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
        {
            HI_ERR_VDEC("[%s]ERROR:to large handle %x.\n", __func__, handle);

            return;
        }
    }

    if (pdat3[0] != 0)
    {
        str2val(pdat3, &fps);
        g_CHAN_FRAME_RATE[handle] = fps;
    }

    return;
}


static HI_S32 VDEC_DRV_CtrlWriteProc(struct file * file,
                                 const char __user * buffer, size_t count, loff_t *ppos)
{
    HI_CHAR dat1[VDEC_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat2[VDEC_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat3[VDEC_MAX_PROC_ARGS_SIZE * 8] = {0};
    HI_CHAR buf[256] = {0};
    HI_CHAR MaskName[256] = {0};
    HI_HANDLE handle = 0;

    if (count >= sizeof(buf))
    {
        HI_FATAL_VDEC("FATAL: parameter string is too long!\n");
        return 0;
    }

    memset(buf, 0, sizeof(buf));

    if (copy_from_user(buf, buffer, count))
    {
        HI_FATAL_VDEC("FATAL: copy_from_user failed!\n");
        return 0;
    }

    VDEC_DRV_GetProcArg(buf, dat1, 1);
    VDEC_DRV_GetProcArg(buf, dat2, 2);
    VDEC_DRV_GetProcArg(buf, dat3, 3);

    if (0 == strncmp(dat1, "help", 4))
    {
        HI_DRV_PROC_EchoHelper("================== VDEC CTRL INFO ==================\n");
        HI_DRV_PROC_EchoHelper("=========== param in () is not necessary ===========\n");
        HI_DRV_PROC_EchoHelper("echo  dat0           dat1      dat2   > /proc/msp/vdec_ctrl\n");
        HI_DRV_PROC_EchoHelper("echo  savestream     handle    (path)  -- turn on/off raw save\n");
        HI_DRV_PROC_EchoHelper("echo  saverpu        handle    (path)  -- turn on/off rpu save\n");
        HI_DRV_PROC_EchoHelper("echo  saveyuv        handle    (path)  -- turn on/off yuv save\n");
        HI_DRV_PROC_EchoHelper("echo  saveoneyuv     handle    (path)  -- turn on/off oneyuv save\n");
        HI_DRV_PROC_EchoHelper("echo  maskdfs        on/off            -- enable/disable mask dynamic fs\n");
        HI_DRV_PROC_EchoHelper("echo  maskcmp        on/off            -- enable/disable mask compress\n");
        HI_DRV_PROC_EchoHelper("====================================================\n");
    }
    else if (0 == strncmp(dat1, "savestream", 10))
    {
        SaveStream(dat2, dat3, handle);
    }
    else if (strncmp(dat1, "saverpu", 7) == 0)
    {
        SaveRPU(dat2, dat3, handle);
    }
    else if (strncmp(dat1, "saveyuv", 7) == 0)
    {
        SaveYUV(dat2, dat3, handle);
    }
    else if (strncmp(dat1, "saveoneyuv", 10) == 0)
    {
        SaveOneYUVFrame(dat2, dat3, handle);
    }
    else if (strncmp(dat1, "maskdfs", 7) == 0)
    {
        strncpy(MaskName, "maskdfs", 8);
        SetMaskBit(dat2, MaskName, 0);
    }
    else if (0 == strncmp(dat1, "bypass", 7))
    {
        strncpy(MaskName, "bypass", 7);
        SetMaskBit(dat2, MaskName, 1);
    }
    else if (0 == strncmp(dat1, "maskcmp", 7))
    {
        strncpy(MaskName, "maskcmp", 8);
        SetMaskBit(dat2, MaskName, 2);
    }
    else if (0 == strncmp(dat1, "setfps", 7))
    {
        SetFps(dat2, dat3, handle);
    }

    else
    {
        HI_FATAL_VDEC("FATAL: unkown echo cmd '%d'!\n");
    }

    return count;
}

static HI_S32 VDEC_DRV_WriteProc(struct file * file,
                                 const char __user * buffer, size_t count, loff_t *ppos)
{
    struct seq_file* q = file->private_data;
    DRV_PROC_ITEM_S* pProcItem = q->private;
    HI_CHAR dat1[VDEC_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat2[VDEC_MAX_PROC_ARGS_SIZE] = {0};
    HI_CHAR dat3[VDEC_MAX_PROC_ARGS_SIZE * 8] = {0};
    HI_CHAR buf[256] = {0};
    HI_HANDLE hHandle = 0;
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S* pstChan = HI_NULL;
    VFMW_EXPORT_FUNC_S* pfnVfmwFunc = HI_NULL;

    hHandle = (pProcItem->entry_name[4] - 48) * 10 + ((pProcItem->entry_name[5]) - 48);

    pstChan = VDEC_DRV_GetChan(hHandle);
    if (pstChan == HI_NULL)
    {
        HI_ERR_VDEC("ERR: chan %d is not init!\n", hHandle);
        return 0;
    }

    pfnVfmwFunc = VDEC_DRV_GetVfmwExportFunc();
    hHandle = (HI_ID_VDEC << 16) | hHandle;

    if (count >= sizeof(buf))
    {
        HI_FATAL_VDEC("FATAL: parameter string is too long!\n");
        return 0;
    }

    memset(buf, 0, sizeof(buf));

    if (copy_from_user(buf, buffer, count))
    {
        HI_FATAL_VDEC("FATAL: copy_from_user failed!\n");
        return 0;
    }

    VDEC_DRV_GetProcArg(buf, dat1, 1);
    VDEC_DRV_GetProcArg(buf, dat2, 2);
    VDEC_DRV_GetProcArg(buf, dat3, 3);

    if (strncmp(dat1, "help", 4) == 0)
    {
        HI_DRV_PROC_EchoHelper("================== VDEC INFO ==================\n");
        HI_DRV_PROC_EchoHelper("=========== xx is chan num===========\n");
        HI_DRV_PROC_EchoHelper("echo  dat0             dat1    > /proc/msp/vdec[xx]\n");
        HI_DRV_PROC_EchoHelper("echo  low_delay_stat   start    -- start low_delay_stat\n");
        HI_DRV_PROC_EchoHelper("echo  low_delay_stat   stop     -- stop low_delay_stat\n");
        HI_DRV_PROC_EchoHelper("====================================================\n");
    }
    else if (strncmp(dat1, "low_delay_stat", 14) == 0)
    {
        if (strncmp(dat2, "start", 5) == 0)
        {
            s32Ret = (pfnVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_START_LOWDLAY_CALC, &hHandle);

            if (s32Ret != VDEC_OK)
            {
                HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
                return HI_FAILURE;
            }

            HI_DRV_LD_Start_Statistics(SCENES_VID_PLAY, hHandle);
        }
        else if (strncmp(dat2, "stop", 4) == 0)
        {
            s32Ret = (pfnVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_STOP_LOWDLAY_CALC, &hHandle);

            if (s32Ret != VDEC_OK)
            {
                HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
                return HI_FAILURE;
            }

            HI_DRV_LD_Stop_Statistics();
        }
    }
    else
    {
        HI_FATAL_VDEC("FATAL: unkown echo cmd '%d'!\n");
    }

    return count;
    //return HI_DRV_PROC_ModuleWrite(file, buffer, count, ppos, VDEC_DRV_DebugCtrl);
}


static HI_VOID WriteHDR10Proc(struct seq_file *p, HI_DRV_VIDEO_HDR10_INFO_S *pstHDR10Info)
{
    PROC_PRINT(p, "%-35s : %d\n", "TransferCharacteristics", pstHDR10Info->u8TransferCharacteristics);

    if (pstHDR10Info->u8MasteringAvailable == 1)
    {
        PROC_PRINT(p, "%-35s : %d\n", "bMasteringAvailable", pstHDR10Info->u8MasteringAvailable);
        PROC_PRINT(p, "%-35s : (%d, %d, %d)\n", "DisplayPrimaries_x",
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_x[0],
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_x[1],
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_x[2]);

        PROC_PRINT(p, "%-35s : (%d, %d, %d)\n", "DisplayPrimaries_y",
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_y[0],
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_y[1],
                      pstHDR10Info->stMasteringInfo.u16DisplayPrimaries_y[2]);

        PROC_PRINT(p, "%-35s : (%d, %d)\n", "WhitePoint",
                      pstHDR10Info->stMasteringInfo.u16WhitePoint_x,
                      pstHDR10Info->stMasteringInfo.u16WhitePoint_y);

        PROC_PRINT(p, "%-35s : %d\n", "MaxDisplayMasteringLuminance",
                      pstHDR10Info->stMasteringInfo.u32MaxDisplayMasteringLuminance);
        PROC_PRINT(p, "%-35s : %d\n", "MinDisplayMasteringLuminance",
                      pstHDR10Info->stMasteringInfo.u32MinDisplayMasteringLuminance);
    }

    if (pstHDR10Info->u8ContentAvailable == 1)
    {
        PROC_PRINT(p, "%-35s : %d\n", "MaxContentLightLevel",
                      pstHDR10Info->stContentInfo.u32MaxContentLightLevel);
        PROC_PRINT(p, "%-35s : %d\n", "MaxPicAverageLightLevel",
                      pstHDR10Info->stContentInfo.u32MaxPicAverageLightLevel);
    }

    return;
}

static HI_VOID ReadTechnicolorProc(struct seq_file *p, HI_DRV_VIDEO_TECHNICOLOR_INFO_S *pstTechnicolorInfo)
{
    PROC_PRINT(p, "%-35s : %d\n", "YUVRange", pstTechnicolorInfo->stHDR_StaticParms.u8YUVRange);
    PROC_PRINT(p, "%-35s : %d\n", "HDRContainerCS", pstTechnicolorInfo->stHDR_StaticParms.u8HDRContainerCS);
    PROC_PRINT(p, "%-35s : %d\n", "HDRContentCS", pstTechnicolorInfo->stHDR_StaticParms.u8HDRContentCS);
    PROC_PRINT(p, "%-35s : %d\n", "LDRContentCS", pstTechnicolorInfo->stHDR_StaticParms.u8LDRContentCS);
    PROC_PRINT(p, "%-35s : %d\n", "PeakLuma", pstTechnicolorInfo->stHDR_StaticParms.u32PeakLuma);
    PROC_PRINT(p, "%-35s : (%d, %d, %d, %d, %d, %d, %d)\n", "OCT",
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[0],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[1],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[2],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[3],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[4],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[5],
                  pstTechnicolorInfo->stHDR_StaticParms.s32OCT[6]);
    PROC_PRINT(p, "%-35s : %d\n", "BA", pstTechnicolorInfo->stHDR_DanamicParms.u32BA);


    return;
}

static HI_VOID ReadHDRProc(struct seq_file *p, HI_DRV_VIDEO_FRAME_S *pstLastFrm)
{

    switch (pstLastFrm->enSrcFrameType)
    {
        case HI_DRV_VIDEO_FRAME_TYPE_SDR:
            PROC_PRINT(p, "%-35s : SDR\n", "SrcFrmType");
            PROC_PRINT(p, "%-35s : %s\n", "ColorSpace",
                (((HI_DRV_VIDEO_PRIVATE_S*)(pstLastFrm->u32Priv))->eColorSpace == HI_DRV_CS_BT2020_YUV_LIMITED) ? "BT2020" : "UNKNOWN");

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_DOLBY_BL:
            PROC_PRINT(p, "%-35s : DOLBY_BL\n", "SrcFrmType");

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_DOLBY_EL:
            PROC_PRINT(p, "%-35s : DOLBY_EL\n", "SrcFrmType");

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_HDR10:
            PROC_PRINT(p, "%-35s : HDR10\n", "SrcFrmType");
            PROC_PRINT(p, "%-35s : %d\n", "FullRangeFlag", pstLastFrm->stColourInfo.u8FullRangeFlag);
            PROC_PRINT(p, "%-35s : %d\n", "ColourPrimaries", pstLastFrm->stColourInfo.u8ColourPrimaries);
            PROC_PRINT(p, "%-35s : %d\n", "MatrixCoeffs", pstLastFrm->stColourInfo.u8MatrixCoeffs);

            WriteHDR10Proc(p, &(pstLastFrm->unHDRInfo.stHDR10Info));

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_HLG:
            PROC_PRINT(p, "%-35s : HLG\n",    "SrcFrmType");
            PROC_PRINT(p, "%-35s : %d\n", "IsBackwardsCompatible", pstLastFrm->unHDRInfo.stHLGInfo.IsBackwardsCompatible);
            PROC_PRINT(p, "%-35s : %d\n", "FullRange", pstLastFrm->stColourInfo.u8FullRangeFlag);
            PROC_PRINT(p, "%-35s : %d\n", "ColourPrimaries", pstLastFrm->stColourInfo.u8ColourPrimaries);
            PROC_PRINT(p, "%-35s : %d\n", "MatrixCoeffs", pstLastFrm->stColourInfo.u8MatrixCoeffs);

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_SLF:
            PROC_PRINT(p, "%-35s : SLF\n",    "SrcFrmType");
            PROC_PRINT(p, "%-35s : %d\n", "FullRange", pstLastFrm->stColourInfo.u8FullRangeFlag);
            PROC_PRINT(p, "%-35s : %d\n", "ColourPrimaries", pstLastFrm->stColourInfo.u8ColourPrimaries);
            PROC_PRINT(p, "%-35s : %d\n", "MatrixCoeffs", pstLastFrm->stColourInfo.u8MatrixCoeffs);

            break;

        case HI_DRV_VIDEO_FRAME_TYPE_TECHNICOLOR:
            PROC_PRINT(p, "%-35s : TECHNICOLOR\n",    "SrcFrmType");
            ReadTechnicolorProc(p, &(pstLastFrm->unHDRInfo.stTechnicolorInfo));

            break;

        default:
            PROC_PRINT(p, "%-35s : UNKNOWN\n",    "SrcFrmType");

            break;
    }

    return;
}

static HI_VOID GetDecodeMode(HI_UNF_VCODEC_MODE_E enMode, HI_CHAR *pDecMode, HI_U32 Size)
{
    switch (enMode)
    {
        case HI_UNF_VCODEC_MODE_I:
            snprintf(pDecMode, Size, "I");
            break;

        case HI_UNF_VCODEC_MODE_IP:
            snprintf(pDecMode, Size, "IP");
            break;

        case HI_UNF_VCODEC_MODE_NORMAL:
            snprintf(pDecMode, Size, "NORMAL");
            break;

        default:
            snprintf(pDecMode, Size, "UNKNOWN(%d)", enMode);
            break;
    }

    return;
}

static HI_VOID GetDisplayNorm(HI_UNF_ENC_FMT_E enDisplayNorm, HI_CHAR *pDisplayNorm, HI_U32 Size)
{
    switch (enDisplayNorm)
    {
    case HI_UNF_ENC_FMT_PAL:
        snprintf(pDisplayNorm, Size, "PAL");
        break;
    case HI_UNF_ENC_FMT_NTSC:
        snprintf(pDisplayNorm, Size, "NTSC");
        break;
    default:
        snprintf(pDisplayNorm, Size, "OTHER(%d)", enDisplayNorm);
        break;
    }

    return;
}

static HI_VOID GetFramePackType(HI_UNF_VIDEO_FRAME_PACKING_TYPE_E eFramePackType,
                                      HI_CHAR *pFrmPackingType, HI_U32 Size)
{
    switch (eFramePackType)
    {
    case HI_UNF_FRAME_PACKING_TYPE_NONE:
        snprintf(pFrmPackingType, Size, "2D");
        break;
    case HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE:
        snprintf(pFrmPackingType, Size, "SBS");
        break;
    case HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM:
        snprintf(pFrmPackingType, Size, "TAB");
        break;
    case HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED:
        snprintf(pFrmPackingType, Size, "MVC");
        break;
    case HI_UNF_FRAME_PACKING_TYPE_BUTT:
        default:
        snprintf(pFrmPackingType, Size, "Not Set");
        break;
    }

    return;
}

static HI_VOID GetFieldMode(HI_DRV_FIELD_MODE_E  enFieldMode,
                                  HI_CHAR *pFieldMode, HI_U32 Size)
{
    switch (enFieldMode)
    {
    case HI_DRV_FIELD_ALL:
        snprintf(pFieldMode, Size, "Frame");
        break;
    case HI_DRV_FIELD_TOP:
        snprintf(pFieldMode, Size, "Top");
        break;
    case HI_DRV_FIELD_BOTTOM:
        snprintf(pFieldMode, Size, "Bottom");
        break;
    default:
        snprintf(pFieldMode, Size, "UNKNOWN");
        break;
    }

    return;
}

static HI_VOID GetPixFormat(HI_DRV_PIX_FORMAT_E  ePixFormat,
                                  HI_CHAR *pYUVType, HI_U32 Size)
{
    switch (ePixFormat)
    {
        case HI_DRV_PIX_FMT_NV08:
            snprintf(pYUVType, Size, "SP400");
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            snprintf(pYUVType, Size, "SP411");
            break;

        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV21_TILE:
            snprintf(pYUVType, Size, "SP420");
            break;

        case HI_DRV_PIX_FMT_NV16:
            snprintf(pYUVType, Size, "SP422_1X2");
            break;

        case HI_DRV_PIX_FMT_NV16_2X1:
            snprintf(pYUVType, Size, "SP422_2X1");
            break;

        case HI_DRV_PIX_FMT_NV24:
            snprintf(pYUVType, Size, "SP444");
            break;

        case HI_DRV_PIX_FMT_YUV400:
            snprintf(pYUVType, Size, "P400");
            break;

        case HI_DRV_PIX_FMT_YUV411:
            snprintf(pYUVType, Size, "P411");
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            snprintf(pYUVType, Size, "P420");
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            snprintf(pYUVType, Size, "P422_1X2");
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            snprintf(pYUVType, Size, "P422_2X1");
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            snprintf(pYUVType, Size, "P444");
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            snprintf(pYUVType, Size, "P410");
            break;

        default:
            snprintf(pYUVType, Size, "UNKNOWN");
            break;
    }

    return;
}

static HI_VOID GetDecType(HI_UNF_VCODEC_DEC_TYPE_E  enDecType,
                                  HI_CHAR *pDecType, HI_U32 Size)
{
    switch (enDecType)
    {
        case HI_UNF_VCODEC_DEC_TYPE_NORMAL:
            snprintf(pDecType, Size, "NORMAL");
            break;

        case HI_UNF_VCODEC_DEC_TYPE_ISINGLE:
            snprintf(pDecType, Size, "IFRAME");
            break;

        default:
            snprintf(pDecType, Size, "UNKNOWN");
            break;
    }

    return;
}

static HI_VOID GetCapLevel(HI_UNF_VCODEC_CAP_LEVEL_E   enCapLevel,
                                HI_CHAR *pCapLevel, HI_U32 Size)
{
    switch (enCapLevel)
    {
        case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
            snprintf(pCapLevel, Size, "QCIF");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_CIF:
            snprintf(pCapLevel, Size, "CIF");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_D1:
            snprintf(pCapLevel, Size, "D1");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_720P:
            snprintf(pCapLevel, Size, "720P");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
            snprintf(pCapLevel, Size, "FULLHD");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_2160x1280:
            snprintf(pCapLevel, Size, "1280P(1)");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_1280x2160:
            snprintf(pCapLevel, Size, "1280P(2)");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_2160x2160:
            snprintf(pCapLevel, Size, "2160P(1)");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_4096x2160:
            snprintf(pCapLevel, Size, "2160P(2)");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_2160x4096:
            snprintf(pCapLevel, Size, "2160P(3)");
            break;

        case HI_UNF_VCODEC_CAP_LEVEL_4096x4096:
            snprintf(pCapLevel, Size, "4096P");
            break;

        default:
            snprintf(pCapLevel, Size, "UNKNOWN");
            break;
    }

    return;
}

static HI_VOID GetProtocolLevel(HI_UNF_VCODEC_PRTCL_LEVEL_E enProtocolLevel,
                                HI_CHAR* pProtocolLevel, HI_U32 Size)
{
    switch (enProtocolLevel)
    {
        case HI_UNF_VCODEC_PRTCL_LEVEL_MPEG:
            snprintf(pProtocolLevel, Size, "OTHER");
            break;

        case HI_UNF_VCODEC_PRTCL_LEVEL_H264:
            snprintf(pProtocolLevel, Size, "H264");
            break;

        default:
            snprintf(pProtocolLevel, Size, "UNKNOWN");
            break;
    }

    return;
}

static HI_VOID GetAVSPlusInfo(HI_UNF_VCODEC_TYPE_E enType,
                                      HI_CHAR **pCodecType, HI_U8 bAvsPlus)
{
    if (enType <= HI_UNF_VCODEC_TYPE_BUTT)
    {
        if (bAvsPlus == 1 && enType == HI_UNF_VCODEC_TYPE_AVS)
        {
            *pCodecType = "AVS+";
        }
        else
        {
            *pCodecType = s_aszVdecType[enType];
        }
    }
    else
    {
        *pCodecType = "UNKNOW";
    }

    return;
}

static HI_VOID ReadBitDepth(struct seq_file *p,
                            HI_UNF_VCODEC_TYPE_E enType,
                            VDEC_CHANNEL_S* pstChan)
{
    if (enType == HI_UNF_VCODEC_TYPE_HEVC
        || enType == HI_UNF_VCODEC_TYPE_VP9
        || enType == HI_UNF_VCODEC_TYPE_H264)
    {
        PROC_PRINT(p,
                   "LumaBitdepth                        : %d\n"
                   "ChromaBitdepth                      : %d\n",

                   pstChan->u32LastLumaBitdepth,
                   pstChan->u32LastChromaBitdepth
                  );
    }

    return;
}

static HI_VOID ReadStreamBufferInfo(struct seq_file *p,
                                    BUFMNG_STATUS_S *pstBMStatus,
                                    VDEC_CHANNEL_S* pstChan)
{
    HI_U64 u64UsedTmp = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (pstChan->hDmxVidChn == HI_INVALID_HANDLE)
    {
        s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, pstBMStatus);
        if (s32Ret == HI_SUCCESS)
        {
            u64UsedTmp = (HI_U64)(pstBMStatus->u32Used) * 100;
            do_div(u64UsedTmp, (pstBMStatus->u32Free + pstBMStatus->u32Used));
            PROC_PRINT(p,
                       "StreamBufferSize(Total/Used/Persent): 0x%x/0x%x/%lld%%\n",
                       (pstBMStatus->u32Free + pstBMStatus->u32Used),
                       pstBMStatus->u32Used,
                       u64UsedTmp);
        }
    }

    return;
}

static HI_VOID ReadDataFlowInfo(struct seq_file *p,
                                BUFMNG_STATUS_S *pstBMStatus,
                                VDEC_CHAN_STATINFO_S* pstStatInfo,
                                VDEC_CHANNEL_S* pstChan)
{
    if (pstChan->hDmxVidChn == HI_INVALID_HANDLE)
    {
        PROC_PRINT(p,
                   "DMX/USER->VDEC\n"
                   "GetStreamBuffer(Try/OK)             : %d/%d\n"
                   "PutStreamBuffer(Try/OK)             : %d/%d\n",
                   pstBMStatus->u32GetTry, pstBMStatus->u32GetOK,
                   pstBMStatus->u32PutTry, pstBMStatus->u32PutOK);
    }

    if (HI_INVALID_HANDLE == pstChan->hDmxVidChn && HI_INVALID_HANDLE == pstChan->hOmxChn)
    {
        PROC_PRINT(p,
                   "VDEC->VFMW\n"
                   "AcquireStream(Try/OK)               : %d/%d\n"
                   "ReleaseStream(Try/OK)               : %d/%d\n",
                   pstBMStatus->u32RecvTry, pstBMStatus->u32RecvOK,
                   pstBMStatus->u32RlsTry, pstBMStatus->u32RlsOK);
    }
    else
    {
        PROC_PRINT(p,
                   "VDEC->VFMW\n"
                   "AcquireStream(Try/OK)               : %d/%d\n"
                   "ReleaseStream(Try/OK)               : %d/%d\n",
                   pstStatInfo->u32VdecAcqBufTry, pstStatInfo->u32VdecAcqBufOK,
                   pstStatInfo->u32VdecRlsBufTry, pstStatInfo->u32VdecRlsBufOK);
    }

    PROC_PRINT(p,
               "VFMW->VPSS\n"
               "AcquireFrame(Try/OK)                : %d/%d\n"
               "ReleaseFrame(Try/OK)                : %d/%d\n",
               pstStatInfo->u32VdecRcvFrameTry, pstStatInfo->u32VdecRcvFrameOK,
               pstStatInfo->u32VdecRlsFrameTry, pstStatInfo->u32VdecRlsFrameOK);
#if 0
    PROC_PRINT(p,
               "VDEC->VPSS\n"
               "AcquireFrame(Try/OK)            : %d/%d\n"
               "ReleaseFrame(Try/OK)            : %d/%d\n",
               pstStatInfo->u32UserAcqFrameTry, pstStatInfo->u32UserAcqFrameOK,
               pstStatInfo->u32UserRlsFrameTry, pstStatInfo->u32UserRlsFrameOK);
#endif
    PROC_PRINT(p,
               "VPSS->AVPLAY\n"
               "AcquireFrame(Try/OK)                : %d/%d\n"
               "ReleaseFrame(Try/OK)                : %d/%d\n",
               pstStatInfo->u32AvplayRcvFrameTry, pstStatInfo->u32AvplayRcvFrameOK,
               pstStatInfo->u32AvplayRlsFrameTry, pstStatInfo->u32AvplayRlsFrameOK);

    return;
}

static HI_VOID ReadPictureInfo(struct seq_file *p,
                               BUFMNG_STATUS_S *pstBMStatus,
                               VDEC_CHAN_STATINFO_S* pstStatInfo,
                               VDEC_CHANNEL_S* pstChan)
{
    HI_CHAR aszDisplayNorm[32];
    HI_CHAR aszSampleType[32];
    HI_CHAR aszYUVType[32];
    HI_CHAR aszTopFirst[16];
    HI_CHAR aszUserRatio[16];
    HI_CHAR aszDecodeRatio[16];
    HI_CHAR aszFieldMode[16];
    HI_CHAR aszFrmPackingType[16];

    GetDisplayNorm(pstChan->enDisplayNorm, aszDisplayNorm, sizeof(aszDisplayNorm));
    snprintf(aszUserRatio, sizeof(aszUserRatio),  "%d:%d",
                 pstChan->u32UserSetAspectWidth, pstChan->u32UserSetAspectHeight);

    pstChan->stLastFrm.bProgressive == 1 ?
            (snprintf(aszSampleType, sizeof(aszSampleType), "Progressive")) :
            (snprintf(aszSampleType, sizeof(aszSampleType), "Interlace"));

    GetFieldMode(pstChan->stLastFrm.enFieldMode, aszFieldMode, sizeof(aszFieldMode));
    GetPixFormat(pstChan->stLastFrm.ePixFormat, aszYUVType, sizeof(aszYUVType));

    pstChan->stLastFrm.bTopFieldFirst == 1 ?
            (snprintf(aszTopFirst, sizeof(aszTopFirst), "TRUE")):
            (snprintf(aszTopFirst, sizeof(aszTopFirst), "FALSE"));

    snprintf(aszUserRatio, sizeof(aszUserRatio),  "%d:%d",
             pstChan->u32UserSetAspectWidth, pstChan->u32UserSetAspectHeight);
    snprintf(aszDecodeRatio, sizeof(aszDecodeRatio),  "%d:%d",
             pstChan->u32DecodeAspectWidth, pstChan->u32DecodeAspectHeight);

    pstChan->stLastFrm.bTopFieldFirst == 1 ?
            (snprintf(aszTopFirst, sizeof(aszTopFirst), "TRUE")):
            (snprintf(aszTopFirst, sizeof(aszTopFirst), "FALSE"));

    GetFramePackType(pstChan->eFramePackType, aszFrmPackingType, sizeof(aszFrmPackingType));

    PROC_PRINT(p, "--------------------------Picture Information-------------------------\n"
               "Width*Height                        : %d*%d\n"
               "Stride(Y/C)                         : %#x/%#x\n"
               "FrameRate(fps)                      : Real(%u.%u) FrameInfo(%d)\n"
               "PlayFormat                          : %s\n"
               "FrmPackingType                      : %s\n"
               "Aspect(User/Decode)                 : %s/%s\n"
               "FieldMode                           : %s\n"
               "Type                                : %s\n"
               "YUVFormat                           : %s\n"
               "TopFirst                            : %s\n"
               "ErrFrame                            : %u\n"
               "TypeNum(I/P)                        : %u/%u\n"
               "FrameBuffer Range                   : [0x%x,0x%x]\n"
               "VP6 Picture Reversed                : %s\n\n",



               pstChan->stLastFrm.u32Width, pstChan->stLastFrm.u32Height,
               pstChan->stLastFrm.stBufAddr[0].u32Stride_Y,
               pstChan->stLastFrm.stBufAddr[0].u32Stride_C,
               pstStatInfo->u32AvrgVdecFps, pstStatInfo->u32AvrgVdecFpsLittle,
               pstChan->stLastFrm.u32FrameRate,
               aszDisplayNorm, aszFrmPackingType, aszUserRatio, aszDecodeRatio, aszFieldMode,
               aszSampleType, aszYUVType, aszTopFirst,

               pstStatInfo->u32VdecDecErrFrame,//w00271806
               pstStatInfo->u32FrameType[0],
               pstStatInfo->u32FrameType[1],
               pstChan->stVDHMMZBuf.u32StartPhyAddr, (pstChan->stVDHMMZBuf.u32StartPhyAddr + pstChan->stVDHMMZBuf.u32Size),
               (1 == (pstChan->stCurCfg.unExtAttr.stVP6Attr.bReversed && 0x1)) ? "YES" : "NO"

              );

    ReadDataFlowInfo(p, pstBMStatus, pstStatInfo, pstChan);

    return;
}

static HI_S32 VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v)
{
    HI_S32 i;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S* pstChan;
    VDEC_CHAN_STATINFO_S* pstStatInfo;
    DRV_PROC_ITEM_S* pstProcItem;
    BUFMNG_STATUS_S stBMStatus = {0};
    HI_CHAR aszDecMode[32];
    HI_CHAR aszDecType[10];
    HI_CHAR aszCapLevel[10];
    HI_CHAR aszProtocolLevel[10];
    HI_HANDLE hVpss = HI_INVALID_HANDLE;

    HI_CHAR* s_CodecType = HI_NULL;

    pstProcItem = p->private;

    if (strncmp(pstProcItem->entry_name, "vdec_ctrl", 9) == 0)
    {
        return 0;
    }

    i = (pstProcItem->entry_name[4] - 48) * 10 + ((pstProcItem->entry_name[5]) - 48);
    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        PROC_PRINT(p, "Invalid VDEC ID:%d.\n", i);
        return 0;
    }

    pstChan = VDEC_DRV_GetChan(i);
    if (pstChan)
    {
        pstStatInfo = &(pstChan->stStatInfo);
        GetDecodeMode(pstChan->stCurCfg.enMode, aszDecMode, sizeof(aszDecMode));
        GetDecType(pstChan->stUserCfgCap.enDecType, aszDecType, sizeof(aszDecType));
        GetCapLevel(pstChan->stUserCfgCap.enCapLevel, aszCapLevel, sizeof(aszCapLevel));
        GetProtocolLevel(pstChan->stUserCfgCap.enProtocolLevel, aszProtocolLevel, sizeof(aszProtocolLevel));

        s32Ret = VDEC_FindVpssHandleByVdecHandle(i,&hVpss);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle ERR\n");
        }

        GetAVSPlusInfo(pstChan->stCurCfg.enType, &s_CodecType, pstChan->bAvsPlus);

        PROC_PRINT(p, "============================== VDEC%d ================================\n", i);
        PROC_PRINT(p,
                        "Work State                          : %s\n",
                        (VDEC_CHAN_STATE_RUN == pstChan->enCurState) ? "RUN" : "STOP");
        PROC_PRINT(p,
                        "VpssID                              : vpss0%d\n",
                         hVpss);
        PROC_PRINT(p,
                        "VfmwID                              : vfmw0%d\n",
                        pstChan->hChan);
        PROC_PRINT(p,
                        "Codec ID                            : %s(0x%x)\n"
                        "Mode                                : %s\n"
                        "Priority                            : %u\n"
                        "ErrCover                            : %u\n"
                        "OrderOutput                         : %u\n"
                        "CtrlOption*                         : 0x%x\n"
                        "Capability                          : %s/%s/%s\n"
                        "Dynamic Frame Store                 : %s\n"
                        "Secure Codec                        : %s\n"
                        "OMX Tunneled-Playback Mode          : %s\n",
                        s_CodecType,
                        pstChan->stCurCfg.enType,

                        aszDecMode,
                        pstChan->stCurCfg.u32Priority,
                        pstChan->stCurCfg.u32ErrCover,
                        pstChan->stCurCfg.bOrderOutput,
                        pstChan->stCurCfg.s32CtrlOptions,

                        aszDecType, aszCapLevel, aszProtocolLevel,

                        (1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) ? "Enable" : "Disable",
                        (HI_TRUE == pstChan->bTvp) ? "TRUE" : "FALSE",
                        (1 == pstChan->OmxTunnelMode) ? "Open" : "Close");
        PROC_PRINT(p,
                        "Lowdelay                            : %d\n",
                        pstChan->bLowdelay);
        if (pstChan->stOption.u32DynamicFrameStoreAllocEn == 1)
        {
            PROC_PRINT(p,"-------------------Dynamic Frame Store Information--------------------\n"
                         "Dynamic Frame Store Mode            : %s\n"
                         "DFS config Frame Number             : %d\n"
                         "DFS Extra  Frame Number             : %d\n"
                         "DFS Delay  Time(ms)                 : %d\n"
                         "DFS Max Mem Use(byte)               : %#x\n"
                         "DFS Memory PhyAddress               : %#x\n"
                         "DFS Memory Length(byte)             : %d\n",
                         "Self",
                         pstChan->stOption.u32CfgFrameNum,
                         pstChan->stOption.s32ExtraFrameStoreNum,
                         pstChan->stOption.s32DelayTime,
                         pstChan->stOption.u32MaxMemUse,
                         pstChan->stVDHMMZBuf.u32StartPhyAddr,
                         pstChan->stVDHMMZBuf.u32Size
                         );//l00273086
        }

        PROC_PRINT(p,   "--------------------------Stream Information--------------------------\n"
                        "Source                              : %s%d\n"
                        "StreamSize(Total/Current)           : 0x%x/0x%x\n"
                        "BitRate(bps)                        : %u\n",

                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn && HI_INVALID_HANDLE == pstChan->hOmxChn) ? "User" : \
                        ((HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? "OmxChan" : "DemuxChan"),
                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn && HI_INVALID_HANDLE == pstChan->hOmxChn) ? pstChan->hStrmBuf : \
                        ((HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? (pstChan->hOmxChn & 0xff) : (pstChan->hDmxVidChn & 0xff)),

                        pstStatInfo->u32TotalVdecInByte,
                        pstStatInfo->u32TotalVdecHoldByte,

                        pstStatInfo->u32AvrgVdecInBps
                        );

        ReadBitDepth(p, pstChan->stCurCfg.enType, pstChan);
        ReadStreamBufferInfo(p, &stBMStatus, pstChan);

        ReadPictureInfo(p, &stBMStatus, pstStatInfo, pstChan);

        PROC_PRINT(p, "--------------------------Other Information-------------------------\n");
        ReadHDRProc(p, &(pstChan->stLastFrm));

        PROC_PRINT(p, "=======================================================================\n");

    }
    else
    {
        PROC_PRINT(p, "vdec not init!\n" );
    }

    return 0;
}

static UMAP_DEVICE_S    VdecDev;

HI_S32 VDEC_DRV_ModInit(HI_VOID)
{
    int ret;

#ifndef HI_MCE_SUPPORT
    ret = VDEC_DRV_Init();
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("Init drv fail!\n");
        return HI_FAILURE;
    }
#endif

    strncpy(VdecDev.devfs_name, UMAP_DEVNAME_VDEC, sizeof(VdecDev.devfs_name) - 1);
    VdecDev.fops   = &VdecFileOpts;
    VdecDev.minor  = UMAP_MIN_MINOR_VDEC;
    VdecDev.owner  = THIS_MODULE;
    VdecDev.drvops = &VdecOps;

    if (HI_DRV_DEV_Register(&VdecDev) < 0)
    {
        HI_FATAL_VDEC("Reg dev failed\n");
        return HI_FAILURE;
    }

    ret = VDEC_DRV_RegisterProc(&s_stProcParam);
    if (HI_SUCCESS != ret)
    {
        HI_FATAL_VDEC("Reg proc fail!,ret=%d\n",ret);
        return HI_FAILURE;
    }

#ifdef VFMW_VPSS_BYPASS_EN
    VDEC_DRV_SpecialFrmListInit();
#endif

#ifdef MODULE
    HI_PRINT("Load hi_vdec.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_ModExit(HI_VOID)
{
#ifdef VFMW_VPSS_BYPASS_EN
    VDEC_DRV_SpecialFrmListDeinit();
#endif
    VDEC_DRV_UnregisterProc();
    HI_DRV_DEV_UnRegister(&VdecDev);

#ifndef HI_MCE_SUPPORT
    VDEC_DRV_Exit();
#endif

    VDEC_DRV_DestoryTask();

    return;
}

#ifdef MODULE
module_init(VDEC_DRV_ModInit);
module_exit(VDEC_DRV_ModExit);
#endif

MODULE_AUTHOR("HISILICON");
MODULE_LICENSE("GPL");

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* End of #ifdef __cplusplus */
