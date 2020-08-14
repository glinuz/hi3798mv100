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
#include <mach/hardware.h>

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

#ifdef Hi3716MV410
extern HI_U32 g_CHAN_FRAME_RATE[HI_VDEC_MAX_INSTANCE_NEW];
#endif
static HI_S32   VDEC_DRV_CtrlWriteProc(struct file * file,
                                   const char __user * buf, size_t count, loff_t *ppos);
static HI_S32   VDEC_DRV_CtrlReadProc(struct seq_file *p, HI_VOID *v);

static HI_S32   VDEC_DRV_WriteProc(struct file * file,
                                   const char __user * buf, size_t count, loff_t *ppos);
static HI_S32   VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v);

static long     VDEC_DRV_Ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

#if (1 == HI_PROC_SUPPORT)
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
    "UNKNOWN",
};
#endif

static struct file_operations VdecFileOpts = {
    .owner          = THIS_MODULE,
    .open           = VDEC_DRV_Open,
    .unlocked_ioctl = VDEC_DRV_Ioctl,
    .release        = VDEC_DRV_Release,
};

static PM_BASEOPS_S VdecOps = {
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

static VDEC_REGISTER_PARAM_S s_stProcParam = {
    .pfnCtrlReadProc  = VDEC_DRV_CtrlReadProc,
    .pfnCtrlWriteProc = VDEC_DRV_CtrlWriteProc,
    .pfnReadProc      = VDEC_DRV_ReadProc,
    .pfnWriteProc     = VDEC_DRV_WriteProc,
};

#if (1 == HI_PROC_SUPPORT)
/* save raw/yuv param */
static HI_S8  VdecSavePath[256] = {'/','m','n','t',0};
#endif
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
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
    PROC_PRINT(p, "\n");
    PROC_PRINT(p, "%-35s:%d\n", "VDEC VERSION",   VDEC_VERSION);
    PROC_PRINT(p, "%-35s:%d\n", "MaskCtrlWord",   MaskCtrlWord);
    PROC_PRINT(p, "%-35s:%s\n", "VdecSavePath",   VdecSavePath);
    PROC_PRINT(p, "%-35s:%d\n", "VdecRawChanNum", VdecRawChanNum);
    PROC_PRINT(p, "%-35s:%d\n", "VdecYuvChanNum", VdecYuvChanNum);
    PROC_PRINT(p, "\n");
    PROC_PRINT(p, "================== param in () is not necessary ==================\n");
    PROC_PRINT(p, "echo  dat0        dat1      dat2   > /proc/msp/vdec_ctrl\n");
    PROC_PRINT(p, "echo  savestream  handle   (path)  -- turn on/off raw save\n");
    PROC_PRINT(p, "echo  saveyuv     handle   (path)  -- turn on/off yuv save\n");
    PROC_PRINT(p, "echo  saveoneyuv  handle   (path)  -- turn on/off oneyuv save\n");
    PROC_PRINT(p, "echo  maskdfs     on/off           -- enable/disable mask dynamic fs\n");
#ifdef Hi3716MV410
    PROC_PRINT(p, "echo  setfps      handle   fps     -- set frame rate\n");
#endif
    PROC_PRINT(p, "===================================================================\n");
#endif
#endif

    return 0;
}

#if (1 == HI_PROC_SUPPORT)
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
#endif

static HI_S32 VDEC_DRV_CtrlWriteProc(struct file * file,
                                 const char __user * buffer, size_t count, loff_t *ppos)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
    HI_CHAR dat1[VDEC_MAX_PROC_ARGS_SIZE];
    HI_CHAR dat2[VDEC_MAX_PROC_ARGS_SIZE];
    HI_CHAR dat3[VDEC_MAX_PROC_ARGS_SIZE*8];
    HI_CHAR buf[256];
    HI_HANDLE handle = 0;

#ifdef Hi3716MV410
    HI_U32 fps = 0;
#endif

    if(count >= sizeof(buf))
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
    VDEC_DRV_GetProcArg(buf,dat1,1);
    VDEC_DRV_GetProcArg(buf,dat2,2);
    VDEC_DRV_GetProcArg(buf,dat3,3);

    if(0==strncmp(dat1,"help",4))
    {
        HI_DRV_PROC_EchoHelper("================== VDEC CTRL INFO ==================\n");
        HI_DRV_PROC_EchoHelper("=========== param in () is not necessary ===========\n");
        HI_DRV_PROC_EchoHelper("echo  dat0           dat1      dat2   > /proc/msp/vdec_ctrl\n");
        HI_DRV_PROC_EchoHelper("echo  savestream     handle    (path)  -- turn on/off raw save\n");
        HI_DRV_PROC_EchoHelper("echo  saveyuv        handle    (path)  -- turn on/off yuv save\n");
        HI_DRV_PROC_EchoHelper("echo  saveoneyuv     handle    (path)  -- turn on/off oneyuv save\n");
        HI_DRV_PROC_EchoHelper("echo  maskdfs        on/off            -- enable/disable mask dynamic fs\n");
        HI_DRV_PROC_EchoHelper("====================================================\n");
    }
    else if(0==strncmp(dat1,"savestream",10))
    {
        if(dat2[0]!=0)
        {
            str2val(dat2,&handle);
            if(handle >= HI_VDEC_MAX_INSTANCE_NEW)
            {
                HI_ERR_VDEC("ERROR:to large handle %x.\n",handle);
                return 0;
            }
        }
        
        if(dat3[0] !=0)
        {
            strncpy(VdecSavePath, dat3, sizeof(VdecSavePath));
            VdecSavePath[sizeof(VdecSavePath)-1]='\0';
        }
        
        if(HI_FALSE == BUFMNG_CheckFile(handle, 0))
        {
            HI_S8 str[80];

            struct tm now;
            
            time_to_tm(get_seconds(), 0, &now);

            snprintf(str, sizeof(str),"%s/vdec%02u_%02u_%02u_%02u.es", (char *)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);

            if (BUFMNG_OpenFile(handle, str, 0) != HI_SUCCESS)
            {
                HI_FATAL_VDEC("FATAL: failed create file '%s' for raw stream save!\n",str);
            }
            else
            {
            }
        }
        else if (HI_TRUE == BUFMNG_CheckFile(handle, 0))
        {
            if (BUFMNG_CloseFile(handle, 0) != HI_SUCCESS)
            {
                HI_FATAL_VDEC("FATAL: failed close file for vdec%2d raw stream save!\n", handle);
            }
            else
            {
            }
        }
    }
    else if(0==strncmp(dat1,"saveyuv",7))
    {
        if(dat2[0]!=0)
        {
            str2val(dat2,&handle);
            if(handle >= HI_VDEC_MAX_INSTANCE_NEW)
            {
                HI_ERR_VDEC("ERROR:to large handle %x.\n",handle);
                return 0;
            }
        }
        
        if(dat3[0] !=0)
        {
            strncpy(VdecSavePath, dat3, sizeof(VdecSavePath));
            VdecSavePath[sizeof(VdecSavePath)-1]='\0';
        }
         
        bSaveoneyuv = HI_FALSE;
        if(HI_FALSE == BUFMNG_CheckFile(handle, 1))
        {
            HI_S8 str[80];
            
           struct tm now;
            
            time_to_tm(get_seconds(), 0, &now);

            snprintf(str, sizeof(str),"%s/vdec%02u_%02u_%02u_%02u.yuv", (char *)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);
         
            if (BUFMNG_OpenFile(handle, str, 1) != HI_SUCCESS)
            {
                HI_FATAL_VDEC("FATAL: failed create file '%s' for yuv save!\n",str);
            }
            else
            {
            }
        }
        else if (HI_TRUE == BUFMNG_CheckFile(handle, 1))
        {
            if (BUFMNG_CloseFile(handle, 1) != HI_SUCCESS)
            {
                HI_FATAL_VDEC("FATAL: failed close file for vdec%2d yuv save!\n", handle);
            }
            else
            {
            }
        }
    }
    else if(0==strncmp(dat1,"saveoneyuv",10))
    {
        if(dat2[0]!=0)
        {
            str2val(dat2,&handle);
            if(handle >= HI_VDEC_MAX_INSTANCE_NEW)
            {
                HI_ERR_VDEC("ERROR:to large handle %x.\n",handle);
                return 0;
            }
        }
        
        if(dat3[0] !=0)
        {
            strncpy(VdecSavePath, dat3, sizeof(VdecSavePath));
            VdecSavePath[sizeof(VdecSavePath)-1]='\0';
        }
        
        bSaveoneyuv = HI_TRUE;
        u32SaveCnt = 0;
        if(HI_FALSE == BUFMNG_CheckFile(handle, 1))
        {
            HI_S8 str[80];
           struct tm now;
            
            time_to_tm(get_seconds(), 0, &now);

            snprintf(str, sizeof(str),"%s/vdec%02u_%02u_%02u_%02u.yuv", (char *)VdecSavePath, handle, now.tm_hour, now.tm_min, now.tm_sec);
            
            if (BUFMNG_OpenFile(handle, str, 1) != HI_SUCCESS)
            {
                HI_FATAL_VDEC("FATAL: failed create file '%s' for yuv save!\n",str);
            }
        }
    }
    else if(0==strncmp(dat1,"maskdfs",7))
    {
        if(0==strncmp(dat2,"on",2))
        {
            MaskCtrlWord = MaskCtrlWord|0x1;
            HI_DRV_PROC_EchoHelper("Mask dynamic fs.\n");
        }
        else if(0==strncmp(dat2,"off",3))
        {
            MaskCtrlWord = MaskCtrlWord&0xFFFFFFFE;
            HI_DRV_PROC_EchoHelper("Unmask dynamic fs.\n");
        }
        else
        {
        }
    }
#ifdef Hi3716MV410
    else if (0 == strncmp(dat1, "setfps", 7))
    {
        if (dat2[0] != 0)
        {
            str2val(dat2, &handle);

            if (handle >= HI_VDEC_MAX_INSTANCE_NEW)
            {
                HI_ERR_VDEC("ERROR:to large handle %x.\n", handle);
                return 0;
            }
        }

        if (dat3[0] != 0)
        {
            str2val(dat3, &fps);

            g_CHAN_FRAME_RATE[handle] = fps;
        }
    }

#endif
    else
    {
        HI_FATAL_VDEC("FATAL: unkown echo cmd '%d'!\n");
    }
#endif
#endif

    return count;
}

static HI_S32 VDEC_DRV_WriteProc(struct file * file,
                                 const char __user * buffer, size_t count, loff_t *ppos)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
    struct seq_file *q = file->private_data;
    DRV_PROC_ITEM_S *pProcItem = q->private;
    HI_CHAR dat1[VDEC_MAX_PROC_ARGS_SIZE];
    HI_CHAR dat2[VDEC_MAX_PROC_ARGS_SIZE];
    HI_CHAR dat3[VDEC_MAX_PROC_ARGS_SIZE*8];
    HI_CHAR buf[256];
    HI_HANDLE hHandle = 0;
    HI_S32 s32Ret = HI_FAILURE;
    VDEC_CHANNEL_S *pstChan = HI_NULL;
    VFMW_EXPORT_FUNC_S* pfnVfmwFunc = HI_NULL;

    hHandle = (pProcItem->entry_name[4]-48)*10 + ((pProcItem->entry_name[5]) - 48);
    /**
    s32Ret = sscanf(pProcItem->entry_name, "vdec%02d", &hHandle);
    if(s32Ret <=0)
    {
       HI_ERR_VDEC("Invalid VDEC ID.\n");
       return 0;
    }**/
    pstChan = VDEC_DRV_GetChan(hHandle);
    if(HI_NULL == pstChan)
    {
        HI_ERR_VDEC("ERR: chan %d is not init!\n",hHandle);
        return 0;
	}
	
    pfnVfmwFunc = VDEC_DRV_GetVfmwExportFunc();
    hHandle = (HI_ID_VDEC << 16) | hHandle;
    if(count >= sizeof(buf))
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
    VDEC_DRV_GetProcArg(buf,dat1,1);
    VDEC_DRV_GetProcArg(buf,dat2,2);
    VDEC_DRV_GetProcArg(buf,dat3,3);

    if(0==strncmp(dat1,"help",4))
    {
        HI_DRV_PROC_EchoHelper("================== VDEC INFO ==================\n");
        HI_DRV_PROC_EchoHelper("=========== xx is chan num===========\n");
        HI_DRV_PROC_EchoHelper("echo  dat0             dat1    > /proc/msp/vdec[xx]\n");
        HI_DRV_PROC_EchoHelper("echo  low_delay_stat   start    -- start low_delay_stat\n");
        HI_DRV_PROC_EchoHelper("echo  low_delay_stat   stop     -- stop low_delay_stat\n");
        HI_DRV_PROC_EchoHelper("====================================================\n");
    }
    else if(0==strncmp(dat1,"low_delay_stat",14))
    {
         if(0==strncmp(dat2,"start",5))
         {
             s32Ret = (pfnVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_START_LOWDLAY_CALC, &hHandle);
             if (VDEC_OK != s32Ret)
             {
                 HI_ERR_VDEC("VFMW CFG_CHAN err!\n");
                 return HI_FAILURE;
             }
             HI_DRV_LD_Start_Statistics(SCENES_VID_PLAY, hHandle);
         }
         else if (0==strncmp(dat2,"stop",4))
         {
			 s32Ret = (pfnVfmwFunc->pfnVfmwControl)(pstChan->hChan, VDEC_CID_STOP_LOWDLAY_CALC, &hHandle);
             if (VDEC_OK != s32Ret)
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
#endif
#endif

    return count;
    //return HI_DRV_PROC_ModuleWrite(file, buffer, count, ppos, VDEC_DRV_DebugCtrl);
}

static HI_S32 VDEC_DRV_ReadProc(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#if (1 == HI_PROC_SUPPORT)
    HI_S32 i;
    HI_S32 s32Ret;
    VDEC_CHANNEL_S *pstChan;
    VDEC_CHAN_STATINFO_S *pstStatInfo;
    DRV_PROC_ITEM_S *pstProcItem;
    BUFMNG_STATUS_S stBMStatus = {0};
    HI_CHAR aszDecMode[32];
    HI_CHAR aszDisplayNorm[32];
    HI_CHAR aszSampleType[32];
    HI_CHAR aszYUVType[32];
    HI_CHAR aszTopFirst[16];
    HI_CHAR aszUserRatio[16];
    HI_CHAR aszDecodeRatio[16];
    HI_CHAR aszFrmPackingType[16];
    HI_CHAR aszFieldMode[16];
    HI_CHAR aszDecType[10];
    HI_CHAR aszCapLevel[10];
    HI_CHAR aszProtocolLevel[10];
    HI_HANDLE hVpss = HI_INVALID_HANDLE;

	HI_CHAR *s_CodecType = HI_NULL;
		
    pstProcItem = p->private;

    if (0 == strncmp(pstProcItem->entry_name, "vdec_ctrl",9))
    {
        return 0;
    }

    s32Ret = sscanf(pstProcItem->entry_name, "vdec%02d", &i);
    if(s32Ret <=0)
    {
       PROC_PRINT(p, "Invalid VDEC ID.\n");
       return 0;
    }
    if (i >= HI_VDEC_MAX_INSTANCE_NEW)
    {
        PROC_PRINT(p, "Invalid VDEC ID:%d.\n", i);
        return 0;
    }

    pstChan = VDEC_DRV_GetChan(i);
    if (pstChan)
    {
        pstStatInfo = &(pstChan->stStatInfo);
        switch (pstChan->stCurCfg.enMode)
        {
        case HI_UNF_VCODEC_MODE_I:
            snprintf(aszDecMode, sizeof(aszDecMode), "I");
            break;
        case HI_UNF_VCODEC_MODE_IP:
            snprintf(aszDecMode, sizeof(aszDecMode), "IP");
            break;
        case HI_UNF_VCODEC_MODE_NORMAL:
            snprintf(aszDecMode, sizeof(aszDecMode), "NORMAL");
            break;
        default:
            snprintf(aszDecMode, sizeof(aszDecMode), "UNKNOWN(%d)", pstChan->stCurCfg.enMode);
            break;
        }

        switch (pstChan->enDisplayNorm)
        {
        case HI_UNF_ENC_FMT_PAL:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "PAL");
            break;
        case HI_UNF_ENC_FMT_NTSC:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "NTSC");
            break;
        default:
            snprintf(aszDisplayNorm, sizeof(aszDisplayNorm), "OTHER(%d)", pstChan->enDisplayNorm);
            break;
        }

        if (pstChan->stLastFrm.bProgressive)
        {
            snprintf(aszSampleType, sizeof(aszSampleType), "Progressive");
        }
        else
        {
            snprintf(aszSampleType, sizeof(aszSampleType), "Interlace");
        }

        snprintf(aszUserRatio, sizeof(aszUserRatio),  "%d:%d", pstChan->u32UserSetAspectWidth, pstChan->u32UserSetAspectHeight);
        snprintf(aszDecodeRatio, sizeof(aszDecodeRatio),  "%d:%d", pstChan->u32DecodeAspectWidth, pstChan->u32DecodeAspectHeight);
        switch(pstChan->eFramePackType)
        {
        case HI_UNF_FRAME_PACKING_TYPE_NONE:
            snprintf(aszFrmPackingType,sizeof(aszFrmPackingType),"2D");
            break;
        case HI_UNF_FRAME_PACKING_TYPE_SIDE_BY_SIDE:
            snprintf(aszFrmPackingType,sizeof(aszFrmPackingType),"SBS");
            break;
        case HI_UNF_FRAME_PACKING_TYPE_TOP_AND_BOTTOM:
            snprintf(aszFrmPackingType,sizeof(aszFrmPackingType),"TAB");
            break;
        case HI_UNF_FRAME_PACKING_TYPE_TIME_INTERLACED:
            snprintf(aszFrmPackingType,sizeof(aszFrmPackingType),"MVC");
            break;
        case HI_UNF_FRAME_PACKING_TYPE_BUTT:
            default:
            snprintf(aszFrmPackingType,sizeof(aszFrmPackingType),"Not Set");
            break;
        }
        switch (pstChan->stLastFrm.enFieldMode)
        {
        case HI_DRV_FIELD_ALL:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Frame");
            break;
        case HI_DRV_FIELD_TOP:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Top");
            break;
        case HI_DRV_FIELD_BOTTOM:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "Bottom");
            break;
        default:
            snprintf(aszFieldMode, sizeof(aszFieldMode), "UNKNOWN");
            break;
        }

        switch (pstChan->stLastFrm.ePixFormat)
        {
        case HI_DRV_PIX_FMT_NV08:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP400");
            break;
        case HI_DRV_PIX_FMT_NV12_411:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP411");
            break;
        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV21_TILE:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP420");
            break;
        case HI_DRV_PIX_FMT_NV16:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP422_1X2");
            break;
        case HI_DRV_PIX_FMT_NV16_2X1:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP422_2X1");
            break;
        case HI_DRV_PIX_FMT_NV24:
            snprintf(aszYUVType, sizeof(aszYUVType), "SP444");
            break;
        case HI_DRV_PIX_FMT_YUV400:
            snprintf(aszYUVType, sizeof(aszYUVType), "P400");
            break;
        case HI_DRV_PIX_FMT_YUV411:
            snprintf(aszYUVType, sizeof(aszYUVType), "P411");
            break;
        case HI_DRV_PIX_FMT_YUV420p:
            snprintf(aszYUVType, sizeof(aszYUVType), "P420");
            break;
        case HI_DRV_PIX_FMT_YUV422_1X2:
            snprintf(aszYUVType, sizeof(aszYUVType), "P422_1X2");
            break;
        case HI_DRV_PIX_FMT_YUV422_2X1:
            snprintf(aszYUVType, sizeof(aszYUVType), "P422_2X1");
            break;
        case HI_DRV_PIX_FMT_YUV_444:
            snprintf(aszYUVType, sizeof(aszYUVType), "P444");
            break;
        case HI_DRV_PIX_FMT_YUV410p:
            snprintf(aszYUVType, sizeof(aszYUVType), "P410");
            break;
        default:
            snprintf(aszYUVType, sizeof(aszYUVType), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enDecType)
        {
        case HI_UNF_VCODEC_DEC_TYPE_NORMAL:
            snprintf(aszDecType, sizeof(aszDecType), "NORMAL");
            break;
        case HI_UNF_VCODEC_DEC_TYPE_ISINGLE:
            snprintf(aszDecType, sizeof(aszDecType), "IFRAME");
            break;
        default:
            snprintf(aszDecType, sizeof(aszDecType), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enCapLevel)
        {
        case HI_UNF_VCODEC_CAP_LEVEL_QCIF:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "QCIF");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_CIF:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "CIF");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_D1:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "D1");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_720P:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "720P");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_FULLHD:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "FULLHD");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_2160x1280:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "1280P(1)");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_1280x2160:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "1280P(2)");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_2160x2160:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "2160P(1)");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_4096x2160:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "2160P(2)");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_2160x4096:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "2160P(3)");
            break;
        case HI_UNF_VCODEC_CAP_LEVEL_4096x4096:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "4096P");
            break;
        default:
            snprintf(aszCapLevel, sizeof(aszCapLevel), "UNKNOWN");
            break;
        }

        switch (pstChan->stUserCfgCap.enProtocolLevel)
        {
        case HI_UNF_VCODEC_PRTCL_LEVEL_MPEG:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "OTHER");
            break;
        case HI_UNF_VCODEC_PRTCL_LEVEL_H264:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "H264");
            break;
        default:
            snprintf(aszProtocolLevel, sizeof(aszProtocolLevel), "UNKNOWN");
            break;
        }
        if(pstChan->stLastFrm.bTopFieldFirst)
        {
            snprintf(aszTopFirst, sizeof(aszTopFirst), "TRUE");
        }
        else
        {
            snprintf(aszTopFirst, sizeof(aszTopFirst), "FALSE");
        }
        s32Ret = VDEC_FindVpssHandleByVdecHandle(i,&hVpss);
        if(HI_SUCCESS !=s32Ret)
        {
            HI_ERR_VDEC("VDEC_FindVpssHandleByVdecHandle ERR\n");
        }

		if( pstChan->stCurCfg.enType <= HI_UNF_VCODEC_TYPE_BUTT)
		{
			if (pstChan->bAvsPlus == 1 && pstChan->stCurCfg.enType == HI_UNF_VCODEC_TYPE_AVS)
			{
				s_CodecType = "AVS+";
			}
			else
			{
				s_CodecType = s_aszVdecType[pstChan->stCurCfg.enType];
			}
		}
		else
		{
			s_CodecType = "UNKNOW";
		}
		
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
                        "Dynamic Frame Store                 : %s\n",
                        s_CodecType,
                        pstChan->stCurCfg.enType,

                        aszDecMode,
                        pstChan->stCurCfg.u32Priority,
                        pstChan->stCurCfg.u32ErrCover,
                        pstChan->stCurCfg.bOrderOutput,
                        pstChan->stCurCfg.s32CtrlOptions,

                        aszDecType, aszCapLevel, aszProtocolLevel,

                        (1 == pstChan->stOption.u32DynamicFrameStoreAllocEn) ? "Enable" : "Disable");
        if(1 == pstChan->stOption.u32DynamicFrameStoreAllocEn)
        {
            PROC_PRINT(p,"-------------------Dynamic Frame Store Information--------------------\n"
                         "Dynamic Frame Store Mode            : %s\n"
                         "DFS use MMZ                         : %d\n"
                         "DFS config Frame Number             : %d\n"
                         "DFS Extra  Frame Number             : %d\n"
                         "DFS Delay  Time(ms)                 : %d\n"
                         "DFS Max Mem Use(byte)               : %#x\n"
                         "DFS Memory PhyAddress               : %#x\n"
                         "DFS Memory Length(byte)             : %d\n",
                         (1 == pstChan->stOption.u32SelfAdaptionDFS) ? "Self" : "User",
                         pstChan->u8NeedMMZ,
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

                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? "User" : "DemuxChan",
                        (HI_INVALID_HANDLE == pstChan->hDmxVidChn) ? pstChan->hStrmBuf : (pstChan->hDmxVidChn&&0xff),

                        pstStatInfo->u32TotalVdecInByte,
                        pstStatInfo->u32TotalVdecHoldByte,

                        pstStatInfo->u32AvrgVdecInBps
                        );
        if (HI_UNF_VCODEC_TYPE_HEVC == pstChan->stCurCfg.enType)
        {
            PROC_PRINT(p,
                            "LumaBitdepth                        : %d\n"
                            "ChromaBitdepth                      : %d\n",

                            pstChan->u32LastLumaBitdepth,
                            pstChan->u32LastChromaBitdepth
                            );
        }
        else if (HI_UNF_VCODEC_TYPE_H264 == pstChan->stCurCfg.enType)
        {
            PROC_PRINT(p,
                            "LumaBitdepth                        : %d\n"
                            "ChromaBitdepth                      : %d\n",

                            pstChan->u32LastLumaBitdepth,
                            pstChan->u32LastChromaBitdepth
                            );
        }
        if (HI_INVALID_HANDLE == pstChan->hDmxVidChn)
        {
           s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, &stBMStatus);

           if(HI_SUCCESS == s32Ret)
           {
                PROC_PRINT(p,
                        "StreamBufferSize(Total/Used/Persent): 0x%x/0x%x/%d%%\n",
                           (stBMStatus.u32Free+stBMStatus.u32Used),
                           stBMStatus.u32Used,
                           stBMStatus.u32Used*100/(stBMStatus.u32Free+stBMStatus.u32Used));
           }
        }
        PROC_PRINT(p,"--------------------------Picture Information-------------------------\n"
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
                        pstChan->stVDHMMZBuf.u32StartPhyAddr,(pstChan->stVDHMMZBuf.u32StartPhyAddr+pstChan->stVDHMMZBuf.u32Size),
                        (1 == (pstChan->stCurCfg.unExtAttr.stVP6Attr.bReversed && 0x1)) ? "YES" : "NO"

             );

        if (HI_INVALID_HANDLE == pstChan->hDmxVidChn)
        {
            //s32Ret = BUFMNG_GetStatus(pstChan->hStrmBuf, &stBMStatus);
            if(HI_SUCCESS == s32Ret)
            {

                PROC_PRINT(p,
                            "DMX/USER->VDEC\n"
                            "GetStreamBuffer(Try/OK)             : %d/%d\n"
                            "PutStreamBuffer(Try/OK)             : %d/%d\n",
                            stBMStatus.u32GetTry, stBMStatus.u32GetOK,
                            stBMStatus.u32PutTry, stBMStatus.u32PutOK);
            }
        }


        if (HI_INVALID_HANDLE == pstChan->hDmxVidChn)
        {
            PROC_PRINT(p,
                            "VDEC->VFMW\n"
                            "AcquireStream(Try/OK)               : %d/%d\n"
                            "ReleaseStream(Try/OK)               : %d/%d\n",
                            stBMStatus.u32RecvTry, stBMStatus.u32RecvOK,
                            stBMStatus.u32RlsTry, stBMStatus.u32RlsOK);
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
        PROC_PRINT(p, "=======================================================================\n");

    }
    else
    {
        PROC_PRINT(p, "vdec not init!\n" );
    }
#endif
#endif

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

#ifdef MODULE
    HI_PRINT("Load hi_vdec.ko success.\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID VDEC_DRV_ModExit(HI_VOID)
{
    VDEC_DRV_UnregisterProc();
    HI_DRV_DEV_UnRegister(&VdecDev);

#ifndef HI_MCE_SUPPORT
    VDEC_DRV_Exit();
#endif

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
