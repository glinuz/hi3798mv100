/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    omxvdec.c
 *
 * Purpose: omxvdec main entry
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#include <linux/platform_device.h>

#include "omxvdec.h"
#include "channel.h"
#include "task.h"
#include "drv_omxvdec_ext.h"
#include "decoder.h"


/*================ EXTERN VALUE ================*/
extern HI_BOOL  g_FrameRateLimit;
extern HI_BOOL  g_DeInterlaceEnable;
extern HI_BOOL  g_BypassVpss;
extern HI_BOOL  g_DynamicFsEnable;
extern HI_BOOL  g_LowDelayStatistics;
extern HI_U32   g_DispNum;
extern HI_U32   g_SegSize;         // (M)
extern HI_BOOL  g_RawMoveEnable;   // 码流搬移使能标志，解决scd切割失败不释放码流的情况
extern HI_BOOL  g_FastOutputMode;
extern HI_U32   g_CompressEnable;


/*=================== MACRO ====================*/
#if (1 == HI_PROC_SUPPORT)
#define DBG_CMD_NUM                        (2)
#define DBG_CMD_LEN                        (256)

#define DBG_CMD_START                      "start"
#define DBG_CMD_STOP                       "stop"
#define DBG_CMD_ON                         "on"
#define DBG_CMD_OFF                        "off"

#define DBG_CMD_SET_PRINT                  "set_print"
#define DBG_CMD_SET_DISPNUM                "set_dispnum"
#define DBG_CMD_SET_SEGSIZE                "set_segsize"
#define DBG_CMD_SET_LOWDELAY_FRAME         "set_lowdelay_frame"

#define DBG_CMD_SAVE_RAW                   "save_raw"
#define DBG_CMD_SAVE_YUV                   "save_yuv"
#define DBG_CMD_SAVE_IMG                   "save_img"
#define DBG_CMD_SAVE_NAME                  "save_name"
#define DBG_CMD_SAVE_PATH                  "save_path"

#define DBG_CMD_TURN_FRAMERATE             "turn_framerate"
#define DBG_CMD_TURN_RAWMOVE               "turn_rawmove"
#define DBG_CMD_TURN_DEI                   "turn_dei"
#define DBG_CMD_TURN_FASTOUTPUT            "turn_fastoutput"
#define DBG_CMD_TURN_DFS                   "turn_dfs"
#define DBG_CMD_TURN_BYPASS                "turn_bypass"
#define DBG_CMD_TURN_COMPRESS              "turn_compress"
#define DBG_CMD_TURN_LOWDELAY              "turn_lowdelay"

#define DBG_CMD_HELP                       "help"
#endif


/*================ GLOBAL VALUE ================*/
HI_U32  g_TraceOption           = 0; //(1<<OMX_FATAL)+(1<<OMX_ERR);
HI_BOOL g_SaveRawEnable         = HI_FALSE;
HI_BOOL g_SaveYuvEnable         = HI_FALSE;
HI_BOOL g_SaveSrcYuvEnable      = HI_FALSE;
HI_U32  g_SaveRawChanNum        = -1;
HI_U32  g_SaveYuvChanNum        = -1;
HI_U32  g_SaveSrcYuvChanNum     = -1;
struct file *g_SaveRawFile      = HI_NULL;
struct file *g_SaveSrcYuvFile   = HI_NULL;

#if (1 == HI_PROC_SUPPORT)
HI_CHAR  g_SavePath[PATH_LEN]    = {'/','m','n','t','\0'};
HI_CHAR  g_SaveName[NAME_LEN]    = {'o','m','x','\0'};
HI_U32   g_SaveNum               = 0;

typedef HI_S32 (*FN_PROC_CMD_HANDLER)(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN]);
typedef struct
{
    HI_CHAR *pCmdName;
    FN_PROC_CMD_HANDLER pHandler;

}PROC_COMMAND_NODE;
#endif

HI_U32   g_LowDelayCountFrame = 100;
extern HI_BOOL  g_FastOutputMode;
OMXVDEC_ENTRY  *g_OmxVdec = HI_NULL;
OMXVDEC_FUNC    g_stOmxFunc = {HI_NULL};
OMX_EXPORT_FUNC_S s_OmxExportFuncs =
{
    .pfnOmxAcquireStream    = decoder_get_stream_ex,
    .pfnOmxReleaseStream    = decoder_release_stream_ex,
    .pfnOmxVdecEventReport  = channel_Vdec_Report,
};


/*================ STATIC VALUE ================*/
static struct class *g_OmxVdecClass     = HI_NULL;
static const HI_CHAR g_OmxVdecDrvName[] = OMXVDEC_NAME;
static dev_t         g_OmxVdecDevNum;

typedef HI_S32 (*FN_IOCTL_HANDLER)(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg);
typedef struct
{
    HI_U32 Code;
    FN_IOCTL_HANDLER pHandler;

}IOCTL_COMMAND_NODE;

MODULE_DESCRIPTION("omxvdec driver");
MODULE_AUTHOR("y00226912, 2013-03-14");
MODULE_LICENSE("GPL");

#ifdef VFMW_VPSS_BYPASS_EN
static HI_VOID OMXVDEC_List_Init(OMXVDEC_List_S *pList);
static HI_VOID OMXVDEC_List_Deinit(OMXVDEC_List_S *pList);
static HI_S32 omxvdec_get_occoupied_frame_info(OMXVDEC_PROC_OCCOUPY_FRAME_INFO *p_proc_occoupy_frame);
#endif


/* ==========================================================================
 * FUNTION
 * =========================================================================*/
HI_VOID omxvdec_release_mem(HI_VOID *pBuffer, eMEM_ALLOC eMemAlloc)
{
    MMZ_BUFFER_S *pTmpBuffer = HI_NULL;

    OMXVDEC_ASSERT_RETURN_NULL(pBuffer != HI_NULL, "invalid param");

    pTmpBuffer= (MMZ_BUFFER_S *)pBuffer;

    if (pTmpBuffer->u32StartPhyAddr == HI_NULL)
    {
        OmxPrint(OMX_WARN, "%s Can NOT releae mem: size:%d addr:0x%x\n", __func__, pTmpBuffer->u32Size, pTmpBuffer->u32StartPhyAddr);

        return;
    }

    switch (eMemAlloc)
    {
        case ALLOC_BY_MMZ:
            {
                HI_DRV_OMXVDEC_UnmapAndRelease(pTmpBuffer);
                break;
            }

        case ALLOC_BY_MMZ_UNMAP:
            {
                HI_DRV_OMXVDEC_Release(pTmpBuffer, 0);
                break;
            }

        case ALLOC_BY_SEC:
            {
                HI_DRV_OMXVDEC_Release(pTmpBuffer, 1);
                break;
            }

    #if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
        case ALLOC_BY_PRE:
            {
                VDEC_Chan_ReleasePreMMZ(pTmpBuffer);
                break;
            }
    #endif

        default:
            {
                OmxPrint(OMX_FATAL, "%s invalid eMemAlloc = %d\n", __func__, eMemAlloc);
                break;
            }
    }

    memset(pBuffer, 0, sizeof(MMZ_BUFFER_S));

    return;
}

/* ==========================================================================
 * omxvdec device mode
 * =========================================================================*/
static HI_S32 omxvdec_setup_cdev(OMXVDEC_ENTRY *omxvdec, const struct file_operations *fops)
{
    HI_S32 rc = -ENODEV;
    struct device *dev;

    g_OmxVdecClass = class_create(THIS_MODULE, "omxvdec_class");
    if (IS_ERR(g_OmxVdecClass))
    {
        rc = PTR_ERR(g_OmxVdecClass);
        g_OmxVdecClass = HI_NULL;
        OmxPrint(OMX_FATAL, "%s call class_create failed, rc = %d\n", __func__, rc);
        return rc;
    }

    rc = alloc_chrdev_region(&g_OmxVdecDevNum, 0, 1, "hisi video decoder");
    if (rc)
    {
        OmxPrint(OMX_FATAL, "%s call alloc_chrdev_region failed, rc = %d\n", __func__, rc);
        goto cls_destroy;
    }

    dev = device_create(g_OmxVdecClass, NULL, g_OmxVdecDevNum, NULL, OMXVDEC_NAME);
    if (IS_ERR(dev))
    {
        rc = PTR_ERR(dev);
        OmxPrint(OMX_FATAL, "%s call device_create failed, rc = %d\n", __func__, rc);
        goto unregister_region;
    }

    cdev_init(&omxvdec->cdev, fops);
    omxvdec->cdev.owner = THIS_MODULE;
    omxvdec->cdev.ops = fops;
    rc = cdev_add(&omxvdec->cdev, g_OmxVdecDevNum , 1);
    if (rc < 0)
    {
        OmxPrint(OMX_FATAL, "%s call cdev_add failed, rc = %d\n", __func__, rc);
        goto dev_destroy;
    }

    return HI_SUCCESS;

dev_destroy:
    device_destroy(g_OmxVdecClass, g_OmxVdecDevNum);
unregister_region:
    unregister_chrdev_region(g_OmxVdecDevNum, 1);
cls_destroy:
    class_destroy(g_OmxVdecClass);
    g_OmxVdecClass = HI_NULL;

    return rc;
}

static HI_S32 omxvdec_cleanup_cdev(OMXVDEC_ENTRY *omxvdec)
{
    if (HI_NULL == g_OmxVdecClass)
    {
        OmxPrint(OMX_FATAL, "%s FATAL: g_OmxVdecClass = NULL", __func__);
        return HI_FAILURE;
    }
    else
    {
        cdev_del(&omxvdec->cdev);
        device_destroy(g_OmxVdecClass, g_OmxVdecDevNum);
        unregister_chrdev_region(g_OmxVdecDevNum, 1);
        class_destroy(g_OmxVdecClass);
        return HI_SUCCESS;
    }
}

/* ==========================================================================
 * HI_CHAR device ops functions
 * =========================================================================*/
static HI_S32 omxvdec_open(struct inode *inode, struct file *fd)
{
    HI_S32  ret = -EBUSY;
    unsigned long  flags;
    OMXVDEC_ENTRY *omxvdec = HI_NULL;

    OmxPrint(OMX_TRACE, "omxvdec prepare to open.\n");

    omxvdec = container_of(inode->i_cdev, OMXVDEC_ENTRY, cdev);

    spin_lock_irqsave(&omxvdec->lock, flags);
    if (omxvdec->open_count < MAX_OPEN_COUNT)
    {
        omxvdec->open_count++;

        if (1 == omxvdec->open_count)
        {
           spin_unlock_irqrestore(&omxvdec->lock, flags);

           memset(&g_stOmxFunc, 0, sizeof(OMXVDEC_FUNC));

           ret = channel_init();
           if(ret != HI_SUCCESS)
           {
               OmxPrint(OMX_FATAL, "%s init channel failed!\n", __func__);
               goto error;
           }

           if (HI_TRUE == g_DynamicFsEnable && HI_NULL == omxvdec->task.task_thread)
           {
               ret = task_create_thread(omxvdec);
               if (ret != HI_SUCCESS)
               {
                   OmxPrint(OMX_FATAL, "%s call channel_create_task failed!\n", __func__);
                   goto error1;
               }
           }

           OMX_PTSREC_Init();

           ret = HI_DRV_MODULE_GetFunction(HI_ID_VDEC, (HI_VOID**)&g_stOmxFunc.pVdecFunc);
           if (HI_SUCCESS != ret || HI_NULL == g_stOmxFunc.pVdecFunc)
           {
               OmxPrint(OMX_FATAL, "%s get vdec function failed!\n", __func__);
               goto error1;
           }
           (((VDEC_EXPORT_FUNC_S*)(g_stOmxFunc.pVdecFunc))->pfnVDEC_SetOmxCallBacks)(&s_OmxExportFuncs);
           spin_lock_irqsave(&omxvdec->lock, flags);
        }

        fd->private_data = omxvdec;

        OmxPrint(OMX_TRACE, "omxvdec open ok.\n");
        ret = HI_SUCCESS;
    }
    else
    {
        OmxPrint(OMX_FATAL, "%s open omxvdec instance too much! \n", __func__);
        ret = -EBUSY;
    }
    spin_unlock_irqrestore(&omxvdec->lock, flags);

    return ret;

error1:
    channel_exit();
error:
    omxvdec->open_count--;
    return ret;

}

static HI_S32 omxvdec_release(struct inode *inode, struct file *fd)
{
    unsigned long     flags;
    OMXVDEC_ENTRY    *omxvdec = HI_NULL;
    OMXVDEC_CHAN_CTX *pchan   = HI_NULL;
    OMXVDEC_CHAN_CTX *n       = HI_NULL;

    OmxPrint(OMX_TRACE, "omxvdec prepare to release.\n");

    omxvdec = fd->private_data;
    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_FATAL, "%s: omxvdec = null, error!\n", __func__);
        return -EFAULT;
    }

    spin_lock_irqsave(&omxvdec->channel_lock, flags);
    if (!list_empty(&omxvdec->chan_list))
    {
        list_for_each_entry_safe(pchan, n, &omxvdec->chan_list, chan_list)
        {
             if ((pchan->file_dec == (HI_U32*)fd) && (CHAN_STATE_INVALID != pchan->state))
             {
                 spin_unlock_irqrestore(&omxvdec->channel_lock, flags);
                 channel_release_inst(pchan);
                 spin_lock_irqsave(&omxvdec->channel_lock, flags);
             }
        }
    }
    spin_unlock_irqrestore(&omxvdec->channel_lock, flags);

    spin_lock_irqsave(&omxvdec->lock, flags);
    if(omxvdec->open_count > 0)
    {
        omxvdec->open_count--;
    }
    spin_unlock_irqrestore(&omxvdec->lock, flags);

    if(0 == omxvdec->open_count)
    {
        channel_exit();
        task_destroy_thread(omxvdec);
        OMX_PTSREC_DeInit();
    }

    fd->private_data = HI_NULL;
    memset(&g_stOmxFunc, 0, sizeof(OMXVDEC_FUNC));

    OmxPrint(OMX_TRACE, "omxvdec release ok.\n");

    return 0;
}

static OMXVDEC_CHAN_CTX *omxvdec_ioctl_get_chan_context(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 chan_id = -1;
    OMXVDEC_ENTRY *omxvdec = fd->private_data;

    chan_id = pMsg->chan_num;
    if (chan_id < 0)
    {
        OmxPrint(OMX_FATAL, "%s invalid chan id: %d.\n", __func__, chan_id);

        return HI_NULL;
    }

    return channel_find_inst_by_channel_id(omxvdec, chan_id);
}

static HI_S32 omxvdec_ioctl_chan_create(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 chan_id = -1;
    OMXVDEC_DRV_CFG chan_cfg;

    if (copy_from_user(&chan_cfg, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_DRV_CFG)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }

    chan_id = channel_create_inst(fd, &chan_cfg);
    if (chan_id < 0)
    {
        OmxPrint(OMX_FATAL, "%s: call channel_create_inst failed!\n", __func__);

        return -EFAULT;
    }

    if (copy_to_user((VOID *)(HI_SIZE_T)pMsg->out, &chan_id, sizeof(HI_S32)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_release(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_release_inst(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call release failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_start(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_start_inst(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call start failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_stop(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_stop_inst(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call stop failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_pause(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_pause_inst(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call pause failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_resume(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_resume_inst(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call resume failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_alloc_buffer(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }

    ret = channel_alloc_buf(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call channel_alloc_buf failed!\n", __func__);

        return -EFAULT;
    }

    if (copy_to_user((VOID *)(HI_SIZE_T)pMsg->out, &user_buf, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_to_user failed!\n", __func__);

        return -EIO;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_release_buffer(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }

    ret = channel_release_buf(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call channel_release_buf failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_bind_buffer(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_bind_user_buffer(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call bind_buffer failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_unbind_buffer(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_unbind_user_buffer(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: case call unbind_buffer failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_empty_input_stream(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_empty_this_stream(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call empty_stream failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_fill_output_frame(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_fill_this_frame(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call fill_frame failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_flush_port(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    ePORT_DIR flush_dir;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&flush_dir, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(ePORT_DIR)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }

    ret = channel_flush_inst(pchan, flush_dir);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call flush_port failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_get_msg(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_MSG_INFO msg;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_get_message(pchan, &msg);
    if (ret != HI_SUCCESS)
    {
        if (ret == -EAGAIN)
        {
            OmxPrint(OMX_WARN, "%s: no msg found, try again.\n", __func__);

            return -EAGAIN;
        }
        else
        {
            OmxPrint(OMX_WARN, "%s: get msg error!\n", __func__);

            return -EFAULT;
        }
    }

    if (copy_to_user((VOID *)(HI_SIZE_T)pMsg->out, &msg, sizeof(OMXVDEC_MSG_INFO)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_port_enable(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_BOOL  bPortEnable;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&bPortEnable, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(HI_BOOL)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }

    pchan->port_enable_flag = bPortEnable;
    OmxPrint(OMX_INFO, "%s %d: set port enable -> %d!\n", __func__, __LINE__,pchan->port_enable_flag);

    return 0;
}

#ifdef VFMW_VPSS_BYPASS_EN
static HI_S32 omxvdec_ioctl_chan_release_frame(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_release_frame(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call VDEC_IOCTL_CHAN_RELEASE_FRAME failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_record_occupied_frame(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    ret = channel_record_occupied_frame(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call VDEC_IOCTL_CHAN_RECORD_OCCUPIED_FRAME failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}

#ifdef HI_TVOS_SUPPORT
static HI_S32 omxvdec_ioctl_chan_global_release_frame(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_BUF_DESC user_buf;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&user_buf, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_BUF_DESC)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    ret = channel_global_release_frame(pchan, &user_buf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call channel_global_release_frame failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}
#endif

static HI_S32 omxvdec_ioctl_chan_get_bypass_info(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_DRV_CFG chan_cfg;
    HI_BOOL VpssBypassEn = HI_FALSE;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&chan_cfg, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_DRV_CFG)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }
    ret = channel_get_processor_bypass(pchan, &chan_cfg);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call VDEC_IOCTL_CHAN_GET_BYPASS_INFO failed!\n", __func__);

        return -EFAULT;
    }

    VpssBypassEn = pchan->bVpssBypass;

    if (copy_to_user((VOID *)(HI_SIZE_T)pMsg->out, &VpssBypassEn, sizeof(HI_BOOL)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EIO;
    }

    return 0;
}

static HI_S32 omxvdec_ioctl_chan_set_bypass_info(struct file *fd, OMXVDEC_IOCTL_MSG *pMsg)
{
    HI_S32 ret;
    OMXVDEC_DRV_CFG chan_cfg;
    OMXVDEC_CHAN_CTX *pchan = omxvdec_ioctl_get_chan_context(fd, pMsg);

    OMXVDEC_ASSERT_RETURN(pchan != HI_NULL, "chan context is null");

    if (copy_from_user(&chan_cfg, (VOID *)(HI_SIZE_T)pMsg->in, sizeof(OMXVDEC_DRV_CFG)))
    {
        OmxPrint(OMX_FATAL, "%s: call copy_from_user failed!\n", __func__);

        return -EFAULT;
    }
    pchan->bVpssBypass = chan_cfg.bVpssBypass;
    ret = channel_set_processor_bypass(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s: call VDEC_IOCTL_CHAN_SET_BYPASS_INFO failed!\n", __func__);

        return -EFAULT;
    }

    return 0;
}
#endif

static const IOCTL_COMMAND_NODE g_IOCTL_CommandTable[] =
{
    {VDEC_IOCTL_CHAN_CREATE,                 omxvdec_ioctl_chan_create},
    {VDEC_IOCTL_CHAN_RELEASE,                omxvdec_ioctl_chan_release},
    {VDEC_IOCTL_CHAN_START,                  omxvdec_ioctl_chan_start},
    {VDEC_IOCTL_CHAN_STOP,                   omxvdec_ioctl_chan_stop},
    {VDEC_IOCTL_CHAN_PAUSE,                  omxvdec_ioctl_chan_pause},
    {VDEC_IOCTL_CHAN_RESUME,                 omxvdec_ioctl_chan_resume},
    {VDEC_IOCTL_CHAN_ALLOC_BUF,              omxvdec_ioctl_chan_alloc_buffer},
    {VDEC_IOCTL_CHAN_RELEASE_BUF,            omxvdec_ioctl_chan_release_buffer},
    {VDEC_IOCTL_CHAN_BIND_BUFFER,            omxvdec_ioctl_chan_bind_buffer},
    {VDEC_IOCTL_CHAN_UNBIND_BUFFER,          omxvdec_ioctl_chan_unbind_buffer},
    {VDEC_IOCTL_EMPTY_INPUT_STREAM,          omxvdec_ioctl_empty_input_stream},
    {VDEC_IOCTL_FILL_OUTPUT_FRAME,           omxvdec_ioctl_fill_output_frame},
    {VDEC_IOCTL_FLUSH_PORT,                  omxvdec_ioctl_flush_port},
    {VDEC_IOCTL_CHAN_GET_MSG,                omxvdec_ioctl_chan_get_msg},
    {VDEC_IOCTL_CHAN_PORT_ENABLE,            omxvdec_ioctl_chan_port_enable},

#ifdef VFMW_VPSS_BYPASS_EN
    {VDEC_IOCTL_CHAN_RELEASE_FRAME,          omxvdec_ioctl_chan_release_frame},
    {VDEC_IOCTL_CHAN_RECORD_OCCUPIED_FRAME,  omxvdec_ioctl_chan_record_occupied_frame},
#ifdef HI_TVOS_SUPPORT
    {VDEC_IOCTL_CHAN_GLOBAL_RELEASE_FRAME,   omxvdec_ioctl_chan_global_release_frame},
#endif
    {VDEC_IOCTL_CHAN_GET_BYPASS_INFO,        omxvdec_ioctl_chan_get_bypass_info},
    {VDEC_IOCTL_CHAN_SET_BYPASS_INFO,        omxvdec_ioctl_chan_set_bypass_info},
#endif

    {0,                                      HI_NULL}, //terminal element
};

FN_IOCTL_HANDLER omxvdec_ioctl_get_handler(HI_U32 Code)
{
    HI_U32 Index = 0;
    FN_IOCTL_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_IOCTL_CommandTable[Index].Code == 0 || g_IOCTL_CommandTable[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (Code == g_IOCTL_CommandTable[Index].Code)
        {
            pTargetHandler = g_IOCTL_CommandTable[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

static long omxvdec_ioctl(struct file *fd, unsigned int code, unsigned long arg)
{
    OMXVDEC_IOCTL_MSG vdec_msg;
    HI_VOID *u_arg = (HI_VOID *)arg;
    FN_IOCTL_HANDLER pIOCTL_Handler = HI_NULL;

    if (copy_from_user(&vdec_msg, u_arg, sizeof(OMXVDEC_IOCTL_MSG)))
    {
        OmxPrint(OMX_FATAL, "%s call copy_from_user failed! \n", __func__);

        return -EFAULT;
    }

    pIOCTL_Handler = omxvdec_ioctl_get_handler(code);
    if (pIOCTL_Handler == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s: ERROR cmd %d is not supported!\n", __func__, _IOC_NR(code));

        return -ENOTTY;
    }

    return pIOCTL_Handler(fd, &vdec_msg);
}

static const struct file_operations omxvdec_fops = {

    .owner             = THIS_MODULE,
    .open              = omxvdec_open,
    .unlocked_ioctl    = omxvdec_ioctl,/*user space and kernel space same with 32 bit or 64 bit run this function*/
#ifdef CONFIG_COMPAT
    .compat_ioctl      = omxvdec_ioctl,/*user space 32bit and kernel space 64 bit run this function*/
#endif
    .release           = omxvdec_release,
};

static HI_S32 omxvdec_probe(struct platform_device * pltdev)
{
    HI_S32         ret     = HI_FAILURE;
    OMXVDEC_ENTRY *omxvdec = HI_NULL;

    OmxPrint(OMX_TRACE, "omxvdec prepare to probe.\n");

    platform_set_drvdata(pltdev, HI_NULL);

    omxvdec = kzalloc(sizeof(OMXVDEC_ENTRY), GFP_KERNEL);
    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_FATAL, "%s call kzalloc failed!\n", __func__);
        return -ENOMEM;
    }

    INIT_LIST_HEAD(&omxvdec->chan_list);
    spin_lock_init(&omxvdec->lock);
    spin_lock_init(&omxvdec->channel_lock);

    ret = omxvdec_setup_cdev(omxvdec, &omxvdec_fops);
    if(ret < 0)
    {
        OmxPrint(OMX_FATAL, "%s call omxvdec_setup_cdev failed!\n", __func__);
        goto cleanup;
    }

    omxvdec->device = &pltdev->dev;
    platform_set_drvdata(pltdev, omxvdec);
    g_OmxVdec = omxvdec;

#ifdef VFMW_VPSS_BYPASS_EN
    OMXVDEC_List_Init(&g_OmxVdec->stRemainFrmList);
#endif

    OmxPrint(OMX_TRACE, "omxvdec probe ok.\n");

    return 0;

cleanup:
    kfree(omxvdec);

    return ret;
}

static HI_S32 omxvdec_remove(struct platform_device *pltdev)
{
    OMXVDEC_ENTRY *omxvdec = HI_NULL;

    OmxPrint(OMX_TRACE, "omxvdec prepare to remove.\n");

#ifdef VFMW_VPSS_BYPASS_EN
    OMXVDEC_List_Deinit(&g_OmxVdec->stRemainFrmList);
#endif

    omxvdec = platform_get_drvdata(pltdev);
    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_ERR, "call platform_get_drvdata err!\n");
    }
    else if (IS_ERR(omxvdec))
    {
        OmxPrint(OMX_ERR, "call platform_get_drvdata err, errno = %ld!\n", PTR_ERR(omxvdec));
    }
    else
    {
        omxvdec_cleanup_cdev(omxvdec);
        kfree(omxvdec);
        g_OmxVdec = HI_NULL;
    }

    platform_set_drvdata(pltdev, HI_NULL);

    OmxPrint(OMX_TRACE, "remove omxvdec ok.\n");

    return 0;
}

UINT32 OMX_GetTimeInMs(VOID)
{
    UINT64   SysTime;

    SysTime = sched_clock();
    do_div(SysTime, 1000000);
    return (UINT32)SysTime;
}

/* ==========================================================================
 * omxvdec proc entrance
 * =========================================================================*/
static inline HI_S32 omxvdec_string_to_value(HI_PCHAR str, HI_U32 *data)
{
    HI_U32 i, d, dat, weight;

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
        if(str[i] < 0x20)
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

    return HI_SUCCESS;
}

#if (1 == HI_PROC_SUPPORT)
HI_VOID omxvdec_help_proc(HI_VOID)
{
    HI_DRV_PROC_EchoHelper(
    "\n"
    "================== OMXVDEC HELP ===========\n"
    "USAGE:echo [cmd] [para] > /proc/msp/omxvdec\n\n"
    "cmd = set_print,          para = value        :set print   = value\n"
    "cmd = set_dispnum,        para = value        :set DispNum = value\n"
    "cmd = set_segsize,        para = value        :set SegSize = value\n"
    "cmd = set_lowdelay_frame, para = value        :set LowDelayCountFrame = value\n");

    HI_DRV_PROC_EchoHelper(
    "cmd = save_raw,           para = start/stop   :start/stop to save raw data\n"
    "cmd = save_yuv,           para = start/stop   :start/stop to save yuv data\n"
    "cmd = save_img,           para = start/stop   :start/stop to save decoder output data\n"
    "cmd = save_name,          para = name         :config name to save data\n"
    "cmd = save_path,          para = path         :config path to save data\n");

    HI_DRV_PROC_EchoHelper(
    "cmd = turn_framerate,     para = on/off       :enable/disable frame rate limit\n"
    "cmd = turn_rawmove,       para = on/off       :enable/disable raw move\n"
    "cmd = turn_dei,           para = on/off       :enable/disable deinterlace\n"
    "cmd = turn_fastoutput,    para = on/off       :enable/disable fast output\n");

    HI_DRV_PROC_EchoHelper(
    "cmd = turn_dfs,           para = on/off       :enable/disable dynamic frame store\n"
    "cmd = turn_bypass,        para = on/off       :enable/disable bypass\n"
    "cmd = turn_compress,      para = on/off       :enable/disable compress\n"
    "cmd = turn_lowdelay,      para = on/off       :enable/disable lowdelay statistics\n\n");

    HI_DRV_PROC_EchoHelper(
    "TraceOption(32bits):\n"
    "0(FATAL)   1(ERR)     2(WARN)     3(INFO)  \n"
    "4(TRACE)   5(INBUF)   6(OUTBUF)   7(PP)    \n"
    "8(VERBOSE) 9(PTS)                          \n"
    "-------------------------------------------\n\n");
}


HI_BOOL omxvdec_check_save_file(HI_S32 Handle, eSAVE_FLIE SaveType)
{
    HI_BOOL ret = HI_FALSE;

    switch (SaveType)
    {
        case SAVE_FLIE_RAW:
            if (HI_NULL != g_SaveRawFile && Handle == g_SaveRawChanNum)
            {
                ret = HI_TRUE;
            }

            break;

        case SAVE_FLIE_YUV:
            if (HI_FALSE != g_SaveYuvEnable && Handle == g_SaveYuvChanNum)
            {
                ret = HI_TRUE;
            }

            break;

        case SAVE_FLIE_IMG:
            if (g_SaveSrcYuvFile != HI_NULL && g_SaveSrcYuvChanNum == Handle)
            {
                ret = HI_TRUE;
            }

            break;
    }

    return ret;
}


HI_S32 omxvdec_open_save_file(HI_S32 Handle, eSAVE_FLIE SaveType)
{
    HI_S32 ret = HI_FAILURE;
    HI_CHAR FilePath[PATH_LEN];

    switch (SaveType)
    {
        case SAVE_FLIE_RAW:
            snprintf(FilePath, sizeof(FilePath), "%s/%s_%d.raw", g_SavePath, g_SaveName, g_SaveNum);

            g_SaveRawFile = filp_open(FilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRWXO);

            if (IS_ERR(g_SaveRawFile))
            {
                g_SaveRawFile = HI_NULL;
            }
            else
            {
                g_SaveRawChanNum = Handle;
                ret = HI_SUCCESS;
            }

            break;

        case SAVE_FLIE_IMG:
            snprintf(FilePath, sizeof(FilePath), "%s/%s_%d_src.yuv", g_SavePath, g_SaveName, g_SaveNum);

            g_SaveSrcYuvFile = filp_open(FilePath, O_RDWR | O_CREAT | O_TRUNC, S_IRWXO);

            if (IS_ERR(g_SaveSrcYuvFile))
            {
                g_SaveSrcYuvFile = HI_NULL;
            }
            else
            {
                g_SaveSrcYuvChanNum = Handle;
                ret = HI_SUCCESS;
            }

            break;

        default:

            break;
    }

    if (HI_SUCCESS == ret)
    {
        OmxPrint(OMX_ALWS, "Open file %s of inst %d success.\n", FilePath, Handle);
    }
    else
    {
        OmxPrint(OMX_ALWS, "Open file %s of inst %d failed.\n", FilePath, Handle);
    }

    return ret;
}


HI_S32 omxvdec_close_save_file(HI_S32 Handle, eSAVE_FLIE SaveType)
{
    HI_S32 ret = HI_FAILURE;

    switch (SaveType)
    {
        case SAVE_FLIE_RAW:
            if (Handle == g_SaveRawChanNum)
            {
                filp_close(g_SaveRawFile, HI_NULL);
                OmxPrint(OMX_ALWS, "Close raw file of inst %d.\n", g_SaveRawChanNum);
                g_SaveRawFile = HI_NULL;
                g_SaveRawChanNum = -1;
                ret = HI_SUCCESS;
            }

            break;

        case SAVE_FLIE_IMG:
            if (g_SaveSrcYuvChanNum == Handle)
            {
                filp_close(g_SaveSrcYuvFile, HI_NULL);
                OmxPrint(OMX_ALWS, "Close src yuv of inst %d.\n", g_SaveSrcYuvChanNum);
                g_SaveSrcYuvFile = HI_NULL;
                g_SaveSrcYuvChanNum = -1;

                ret = HI_SUCCESS;
            }

            break;

        default:

            break;
    }

    return ret;
}


static HI_S32 omxvdec_proc_cmd_set_print(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 Data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &Data) != 0)
    {
        return HI_FAILURE;
    }

    OmxPrint(OMX_ALWS, "set print: %u\n", Data);
    g_TraceOption = Data;

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_set_dispnum(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 Data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &Data) != 0)
    {
        return HI_FAILURE;
    }

    OmxPrint(OMX_ALWS, "set DispNum: %u\n", Data);
    g_DispNum = Data;

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_set_segsize(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 Data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &Data) != 0)
    {
        return HI_FAILURE;
    }

    OmxPrint(OMX_ALWS, "set SegSize: %u\n", Data);
    g_SegSize = Data;

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_set_lowdelay_frame(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 Data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &Data) != 0)
    {
        return HI_FAILURE;
    }

    OmxPrint(OMX_ALWS, "set LowDelayCountFrame: %u\n", Data);
    g_LowDelayCountFrame = Data;

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_save_raw(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &data) != 0)
    {
        return HI_FAILURE;
    }

    if (omxvdec_check_save_file(data, SAVE_FLIE_RAW) == HI_FALSE)
    {
        OmxPrint(OMX_ALWS, "Enable raw save.\n");
        omxvdec_open_save_file(data, SAVE_FLIE_RAW);
        g_SaveRawEnable = HI_TRUE;
        g_SaveNum++;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Disable raw save.\n");
        omxvdec_close_save_file(data, SAVE_FLIE_RAW);
        g_SaveRawEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_save_yuv(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &data) != 0)
    {
        return HI_FAILURE;
    }

    if (omxvdec_check_save_file(data, SAVE_FLIE_YUV) == HI_FALSE)
    {
        OmxPrint(OMX_ALWS, "Enable yuv save.\n");
        g_SaveYuvEnable = HI_TRUE;
        g_SaveYuvChanNum = data;
        g_SaveNum++;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Disable yuv save.\n");
        g_SaveYuvChanNum = -1;
        g_SaveYuvEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_save_img(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 data = 0;

    if(omxvdec_string_to_value(CmdStr[1], &data) != 0)
    {
        return HI_FAILURE;
    }

    if (omxvdec_check_save_file(data, SAVE_FLIE_IMG) == HI_FALSE)
    {
        OmxPrint(OMX_ALWS, "Enable image save.\n");
        omxvdec_open_save_file(data, SAVE_FLIE_IMG);
        g_SaveSrcYuvEnable = HI_TRUE;
        g_SaveNum++;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Disable image save.\n");
        omxvdec_close_save_file(data, SAVE_FLIE_IMG);
        g_SaveSrcYuvEnable = HI_FALSE;
    }

    return HI_SUCCESS;

}

static HI_S32 omxvdec_proc_cmd_save_name(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 StrLen = strlen(CmdStr[1]);

    if (StrLen > 0 && StrLen < sizeof(g_SaveName))
    {
        strncpy(g_SaveName, CmdStr[1], NAME_LEN);
        g_SaveName[NAME_LEN-1] = '\0';
        OmxPrint(OMX_ALWS, "SaveName: %s\n", g_SaveName);
    }
    else
    {
        OmxPrint(OMX_ALWS, "Invalid name \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_save_path(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 StrLen = strlen(CmdStr[1]);

    if (StrLen > 0 && StrLen < sizeof(g_SavePath) && CmdStr[1][0] == '/')
    {
        if(CmdStr[1][StrLen-1] == '/')
        {
           CmdStr[1][StrLen-1] = 0;
        }
        strncpy(g_SavePath, CmdStr[1], PATH_LEN);
        g_SavePath[PATH_LEN-1] = '\0';
        OmxPrint(OMX_ALWS, "SavePath: %s\n", g_SavePath);
    }
    else
    {
        OmxPrint(OMX_ALWS, "Invalid path \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_framerate(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable frame rate limit.\n");
        g_FrameRateLimit = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable frame rate limit.\n");
        g_FrameRateLimit = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_rawmove(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable Raw Move.\n");
        g_RawMoveEnable = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable Raw Move.\n");
        g_RawMoveEnable = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_dei(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable DeInterlace.\n");
        g_DeInterlaceEnable = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable DeInterlace.\n");
        g_DeInterlaceEnable = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_fastoutput(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable FastOutputMode.\n");
        g_FastOutputMode = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable FastOutputMode.\n");
        g_FastOutputMode = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_dfs(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable DynamicFs.\n");
        g_DynamicFsEnable = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable DynamicFs.\n");
        g_DynamicFsEnable = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_bypass(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Without vpss.\n");
        g_BypassVpss = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "With vpss.\n");
        g_BypassVpss = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_compress(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable Compress.\n");
        g_CompressEnable = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable Compress.\n");
        g_CompressEnable = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_turn_lowdelay(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    if (!strncmp(CmdStr[1], DBG_CMD_ON, sizeof(DBG_CMD_ON)))
    {
        OmxPrint(OMX_ALWS, "Enable low_delay_statistics.\n");
        g_LowDelayStatistics = HI_TRUE;
    }
    else if (!strncmp(CmdStr[1], DBG_CMD_OFF, sizeof(DBG_CMD_OFF)))
    {
        OmxPrint(OMX_ALWS, "Disable low_delay_statistics.\n");
        g_LowDelayStatistics = HI_FALSE;
    }
    else
    {
        OmxPrint(OMX_ALWS, "Unkown command \"%s\".\n", CmdStr[1]);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 omxvdec_proc_cmd_help(HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    omxvdec_help_proc();

    return HI_SUCCESS;
}

static const PROC_COMMAND_NODE g_ProcCommandTable[] =
{
    {DBG_CMD_SET_PRINT,                omxvdec_proc_cmd_set_print},
    {DBG_CMD_SET_DISPNUM,              omxvdec_proc_cmd_set_dispnum},
    {DBG_CMD_SET_SEGSIZE,              omxvdec_proc_cmd_set_segsize},
    {DBG_CMD_SET_LOWDELAY_FRAME,       omxvdec_proc_cmd_set_lowdelay_frame},

    {DBG_CMD_SAVE_RAW,                 omxvdec_proc_cmd_save_raw},
    {DBG_CMD_SAVE_YUV,                 omxvdec_proc_cmd_save_yuv},
    {DBG_CMD_SAVE_IMG,                 omxvdec_proc_cmd_save_img},
    {DBG_CMD_SAVE_NAME,                omxvdec_proc_cmd_save_name},
    {DBG_CMD_SAVE_PATH,                omxvdec_proc_cmd_save_path},

    {DBG_CMD_TURN_FRAMERATE,           omxvdec_proc_cmd_turn_framerate},
    {DBG_CMD_TURN_RAWMOVE,             omxvdec_proc_cmd_turn_rawmove},
    {DBG_CMD_TURN_DEI,                 omxvdec_proc_cmd_turn_dei},
    {DBG_CMD_TURN_FASTOUTPUT,          omxvdec_proc_cmd_turn_fastoutput},
    {DBG_CMD_TURN_DFS,                 omxvdec_proc_cmd_turn_dfs},
    {DBG_CMD_TURN_BYPASS,              omxvdec_proc_cmd_turn_bypass},
    {DBG_CMD_TURN_COMPRESS,            omxvdec_proc_cmd_turn_compress},
    {DBG_CMD_TURN_LOWDELAY,            omxvdec_proc_cmd_turn_lowdelay},

    {DBG_CMD_HELP,                     omxvdec_proc_cmd_help},

    {HI_NULL,                          HI_NULL}, //terminal element
};

FN_PROC_CMD_HANDLER omxvdec_proc_get_handler(HI_CHAR *pCmdStr)
{
    HI_U32 Index = 0;
    FN_PROC_CMD_HANDLER pTargetHandler = HI_NULL;

    while(1)
    {
        if (g_ProcCommandTable[Index].pCmdName == HI_NULL || g_ProcCommandTable[Index].pHandler == HI_NULL)
        {
            break;
        }

        if (!strncmp(pCmdStr, g_ProcCommandTable[Index].pCmdName, DBG_CMD_LEN))
        {
            pTargetHandler = g_ProcCommandTable[Index].pHandler;
            break;
        }

        Index++;
    }

    return pTargetHandler;
}

HI_VOID omxvdec_proc_parse_param(HI_CHAR *pParam, HI_CHAR pCmdStr[DBG_CMD_NUM][DBG_CMD_LEN])
{
    HI_U32 ParsePos = 0;
    HI_U32 ParseNum = 0;
    HI_U32 StrLen = 0;
    HI_CHAR *pCurStr = HI_NULL;

    do
    {
        StrLen = 0;
        pCurStr = pCmdStr[ParseNum];
        for (; ParsePos < strlen(pParam); ParsePos++)
        {
            if (StrLen == 0 && pParam[ParsePos] == ' ')
            {
                continue;
            }
            if (pParam[ParsePos] > ' ')
            {
                pCurStr[StrLen++] = pParam[ParsePos];
            }
            if (StrLen > 0 && pParam[ParsePos] == ' ')
            {
                break;
            }
        }
        pCurStr[StrLen] = 0;
        ParseNum++;

    }while(ParseNum < DBG_CMD_NUM);

    return;
}

HI_S32 omxvdec_write_proc(struct file *file, const char __user *buffer, size_t count, loff_t *data)
{
    HI_CHAR Param[DBG_CMD_LEN];
    HI_CHAR CmdStr[DBG_CMD_NUM][DBG_CMD_LEN];
    HI_S32 ret = HI_FAILURE;
    FN_PROC_CMD_HANDLER pCommandHandler = HI_NULL;

    if(count < 1 || count >= sizeof(Param))
    {
        OmxPrint(OMX_ALWS, "parameter string invalid!\n");

        return HI_FAILURE;
    }

    if (copy_from_user(Param, buffer, count))
    {
        OmxPrint(OMX_ALWS, "copy_from_user failed!\n");

        return HI_FAILURE;
    }
    Param[count] = 0;

    omxvdec_proc_parse_param(Param, CmdStr);

    pCommandHandler = omxvdec_proc_get_handler(CmdStr[0]);
    if (pCommandHandler == HI_NULL)
    {
        OmxPrint(OMX_ALWS, "Command \"%s\" not registerd!\n", CmdStr[0]);
        goto error;
    }

    ret = pCommandHandler(CmdStr);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ALWS, "Handle command \"%s\" failed!\n", CmdStr[0]);
        goto error;
    }

    return count;

error:
    OmxPrint(OMX_ALWS, "Invalid echo command!\n");
    omxvdec_help_proc();

    return count;
}

static HI_S32 omxvdec_read_proc(struct seq_file *p, HI_VOID *v)
{
    unsigned long flags;
    OMXVDEC_CHAN_CTX *pchan = NULL;

#ifdef VFMW_VPSS_BYPASS_EN
    HI_U32 i = 0;
    OMXVDEC_PROC_OCCOUPY_FRAME_INFO proc_occoupy_frame;
#endif

    if (HI_NULL == g_OmxVdec)
    {
        PROC_PRINT(p, "Warnning: g_OmxVdec = NULL\n");
        return 0;
    }

    PROC_PRINT(p, "\n");
    PROC_PRINT(p, "============ OMXVDEC INFO ============\n");
    PROC_PRINT(p, "%-25s :%d\n", "Version",           OMXVDEC_VERSION);
    PROC_PRINT(p, "%-25s :%d\n", "ActiveChanNum",     g_OmxVdec->total_chan_num);

    PROC_PRINT(p, "%-25s :%d\n", "Print",             g_TraceOption);
    PROC_PRINT(p, "%-25s :%d\n", "SaveRawEnable",     g_SaveRawEnable);
    PROC_PRINT(p, "%-25s :%d\n", "SaveYuvEnable",     g_SaveYuvEnable);
    PROC_PRINT(p, "%-25s :%d\n", "SaveSrcYuvEnable",  g_SaveSrcYuvEnable);
    PROC_PRINT(p, "%-25s :%s\n", "SaveName",          g_SaveName);
    PROC_PRINT(p, "%-25s :%s\n", "SavePath",          g_SavePath);
    PROC_PRINT(p, "%-25s :%d\n", "DispNum",           g_DispNum);
    PROC_PRINT(p, "%-25s :%d\n", "SegSize(M)",        g_SegSize);

    PROC_PRINT(p, "%-25s :%d\n", "RawMoveEnable",     g_RawMoveEnable);
    PROC_PRINT(p, "%-25s :%d\n", "DynamicFsEnable",   g_DynamicFsEnable);
    PROC_PRINT(p, "%-25s :%d\n", "FrameRateLimit",    g_FrameRateLimit);
    PROC_PRINT(p, "%-25s :%d\n", "DeInterlaceEnable", g_DeInterlaceEnable);
    PROC_PRINT(p, "%-25s :%d\n", "BypassVpss",        g_BypassVpss);
    PROC_PRINT(p, "%-25s :%d\n", "FastOutputMode",    g_FastOutputMode);
    PROC_PRINT(p, "%-25s :%d\n", "LowDelayStatistics", g_LowDelayStatistics);
    if (HI_TRUE == g_DynamicFsEnable && HI_NULL != g_OmxVdec->task.task_thread)
    {
        task_proc_entry(p, &g_OmxVdec->task);
    }

#ifdef VFMW_VPSS_BYPASS_EN
    memset(&proc_occoupy_frame, 0, sizeof(proc_occoupy_frame));

    omxvdec_get_occoupied_frame_info(&proc_occoupy_frame);

    PROC_PRINT(p, "%-25s :%d\n", "occoupy frame",  proc_occoupy_frame.occoupy_frame_num);

    if (proc_occoupy_frame.occoupy_frame_num != 0)
    {
        for (i = 0; i < proc_occoupy_frame.occoupy_frame_num; i++)
        {
            PROC_PRINT(p, "%-25s:0x%x/%p/%d\n", "Phy/Vir/Size",  proc_occoupy_frame.frmBufRec[i].u32StartPhyAddr,\
                       proc_occoupy_frame.frmBufRec[i].pu8StartVirAddr, proc_occoupy_frame.frmBufRec[i].u32Size);
        }
    }
#endif
    PROC_PRINT(p, "\n");

    if (0 != g_OmxVdec->total_chan_num)
    {
       spin_lock_irqsave(&g_OmxVdec->channel_lock, flags);
       list_for_each_entry(pchan, &g_OmxVdec->chan_list, chan_list)
       {
           channel_proc_entry(p, pchan);
       }
       spin_unlock_irqrestore(&g_OmxVdec->channel_lock, flags);
    }

    return 0;
}

HI_S32 omxvdec_init_proc (HI_VOID)
{
    HI_CHAR aszBuf[16];
    DRV_PROC_ITEM_S *pstItem;

    /* Create proc */
    snprintf(aszBuf, sizeof(aszBuf), "omxvdec");
    pstItem = HI_DRV_PROC_AddModule(aszBuf, HI_NULL, HI_NULL);
    if (!pstItem)
    {
        OmxPrint(OMX_FATAL, "Create omxvdec proc entry fail!\n");
        return HI_FAILURE;
    }

    /* Set functions */
    pstItem->read  = omxvdec_read_proc;
    pstItem->write = omxvdec_write_proc;

    return HI_SUCCESS;
}

HI_VOID omxvdec_exit_proc(HI_VOID)
{
    HI_CHAR aszBuf[16];

    snprintf(aszBuf, sizeof(aszBuf), "omxvdec");
    HI_DRV_PROC_RemoveModule(aszBuf);

    return;
}
#endif


static HI_S32 omxvdec_suspend(struct platform_device *pltdev, pm_message_t state)
{
    return HI_SUCCESS;
}

static HI_S32 omxvdec_resume(struct platform_device *pltdev)
{
    return HI_SUCCESS;
}

static HI_VOID omxvdec_device_release(struct device* dev)
{
    return;
}

static struct platform_driver omxvdec_driver = {

    .probe             = omxvdec_probe,
    .remove            = omxvdec_remove,
    .suspend           = omxvdec_suspend,
    .resume            = omxvdec_resume,
    .driver = {
        .name          = (HI_PCHAR)g_OmxVdecDrvName,
        .owner         = THIS_MODULE,
    },
};

static struct platform_device omxvdec_device = {

    .name              = g_OmxVdecDrvName,
    .id                = -1,
    .dev = {
        .platform_data = NULL,
        .release       = omxvdec_device_release,
    },
};

HI_S32 OMXVDEC_DRV_ModInit(HI_VOID)
{
    HI_S32 ret;

    ret = platform_device_register(&omxvdec_device);
    if(ret < 0)
    {
        OmxPrint(OMX_FATAL, "%s call platform_device_register failed!\n", __func__);
        return ret;
    }

    ret = platform_driver_register(&omxvdec_driver);
    if(ret < 0)
    {
        OmxPrint(OMX_FATAL, "%s call platform_driver_register failed!\n", __func__);
        platform_device_unregister(&omxvdec_device);
        return ret;
    }

#if (1 == HI_PROC_SUPPORT)
    ret = omxvdec_init_proc();
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "omxvdec_init_proc failed!\n");
        platform_driver_unregister(&omxvdec_driver);
        platform_device_unregister(&omxvdec_device);
        return ret;
    }

#ifdef MODULE
    HI_PRINT("Load hi_omxvdec.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

    return HI_SUCCESS;
}

HI_VOID OMXVDEC_DRV_ModExit(HI_VOID)
{
    platform_driver_unregister(&omxvdec_driver);
    platform_device_unregister(&omxvdec_device);

#if (1 == HI_PROC_SUPPORT)
    omxvdec_exit_proc();

#ifdef MODULE
    HI_PRINT("Unload hi_omxvdec.ko success.\t(%s)\n", VERSION_STRING);
#endif
#endif

}

#ifdef VFMW_VPSS_BYPASS_EN

static HI_VOID OMXVDEC_List_Init(OMXVDEC_List_S *pList)
{
   D_OMXVDEC_CHECK_PTR(pList);

   if (HI_FALSE == pList->bInit)
   {
      memset(&pList->stSpecialFrmRec[0],0, sizeof(OMXVDEC_FRM_INFO_S)*OMXVDEC_MAX_REMAIN_FRM_NUM);

      spin_lock_init(&g_OmxVdec->stRemainFrmList.bypass_lock);

      pList->bInit = HI_TRUE;
      pList->s32Num = 0;
   }
   return;
}

static HI_VOID OMXVDEC_List_Deinit(OMXVDEC_List_S *pList)
{
    HI_U32 i = 0;
    OMXVDEC_FRM_INFO_S *pSpecialFrmRec;

    D_OMXVDEC_CHECK_PTR(pList);

    for (i = 0; i < OMXVDEC_MAX_REMAIN_FRM_NUM; i++)
    {
        pSpecialFrmRec = &pList->stSpecialFrmRec[i];
        if ((ALLOC_BY_MMZ == pSpecialFrmRec->enbufferNodeStatus || ALLOC_BY_MMZ_UNMAP== pSpecialFrmRec->enbufferNodeStatus)
            &&(pSpecialFrmRec->frmBufRec.u32StartPhyAddr != 0)
            && (pSpecialFrmRec->frmBufRec.u32StartPhyAddr != 0xffffffff))
        {
            omxvdec_release_mem(&pSpecialFrmRec->frmBufRec, pSpecialFrmRec->enbufferNodeStatus);
        }
        else if (ALLOC_BY_SEC == pSpecialFrmRec->enbufferNodeStatus)
        {
            OmxPrint(OMX_FATAL,"%s,bypass not support tvp yet!\n",__func__);
        }
       //清空该节点内容，视为空闲节点
       memset(pSpecialFrmRec, 0, sizeof(OMXVDEC_FRM_INFO_S));
    }
    pList->s32Num = 0;
    pList->bInit = HI_FALSE;
    return ;
}

HI_S32 OMXVDEC_List_FindNode(OMXVDEC_List_S *pList,HI_U32 u32TargetPhyAddr,HI_U32 *pIndex)
{
   HI_U32 index = 0;
   OMXVDEC_FRM_INFO_S *pSpecialFrmRec;
   unsigned long   flags;

   D_OMXVDEC_CHECK_PTR_RET(pList);
   D_OMXVDEC_CHECK_PTR_RET(pIndex);

   spin_lock_irqsave(&pList->bypass_lock, flags);

   for (index = 0; index < OMXVDEC_MAX_REMAIN_FRM_NUM; index++)
   {
       pSpecialFrmRec = &pList->stSpecialFrmRec[index];
       if ( pSpecialFrmRec->frmBufRec.u32StartPhyAddr == u32TargetPhyAddr )
       {
           *pIndex = index;
           break;
       }
   }

   spin_unlock_irqrestore(&pList->bypass_lock, flags);

   if (index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
   {
       return HI_FAILURE;
   }
   else
   {
       return HI_SUCCESS;
   }
}


HI_S32 OMXVDEC_Frame_in_List(OMXVDEC_List_S *pList,OMXVDEC_FRM_INFO_S *pSpecialFrmRec)
{
    HI_U32 index;
    OMXVDEC_FRM_INFO_S *pListRec;

    D_OMXVDEC_CHECK_PTR_RET(pList);
    D_OMXVDEC_CHECK_PTR_RET(pSpecialFrmRec);

    for (index = 0; index < OMXVDEC_MAX_REMAIN_FRM_NUM; index++)
    {
        pListRec = &pList->stSpecialFrmRec[index];
        if (pListRec->frmBufRec.u32StartPhyAddr == pSpecialFrmRec->frmBufRec.u32StartPhyAddr)
        {
            break;
        }
    }

    if (index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
    {
        OmxPrint(OMX_INFO,"frame is NOT in list phy:%x\n", pSpecialFrmRec->frmBufRec.u32StartPhyAddr);

        return HI_FAILURE;
    }
    else
    {
        OmxPrint(OMX_INFO,"frame is in list phy:%x\n", pSpecialFrmRec->frmBufRec.u32StartPhyAddr);

        return HI_SUCCESS;
    }
}


HI_S32 OMXVDEC_List_Add(OMXVDEC_List_S *pList,OMXVDEC_FRM_INFO_S *pSpecialFrmRec)
{

   HI_U32 index;
   OMXVDEC_FRM_INFO_S *pListRec;

   D_OMXVDEC_CHECK_PTR_RET(pList);
   D_OMXVDEC_CHECK_PTR_RET(pSpecialFrmRec);

   for (index = 0; index < OMXVDEC_MAX_REMAIN_FRM_NUM; index++)
   {
       pListRec = &pList->stSpecialFrmRec[index];
       if (pListRec->frmBufRec.u32StartPhyAddr == 0)
       {
           memcpy(pListRec, pSpecialFrmRec, sizeof(OMXVDEC_FRM_INFO_S));
           pList->s32Num++;
           if (pList->s32Num > OMXVDEC_MAX_REMAIN_FRM_NUM)
           {
               OmxPrint(OMX_FATAL,"Remain Frame num = %d larger than Max(%d),force to be %d\n",pList->s32Num,OMXVDEC_MAX_REMAIN_FRM_NUM,OMXVDEC_MAX_REMAIN_FRM_NUM);
               pList->s32Num = OMXVDEC_MAX_REMAIN_FRM_NUM;
           }
           break;
       }
   }
   if (index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
   {
       OmxPrint(OMX_FATAL,"can't find the idle node of the global frame list! Remain Frame num = %d\n",pList->s32Num);
   }
   return HI_SUCCESS;
}

HI_S32 OMXVDEC_List_Del(OMXVDEC_List_S *pList,HI_U32 u32Index)
{
   OMXVDEC_FRM_INFO_S *pSpecialFrmRec;
   unsigned long   flags;

   OmxPrint(OMX_TRACE,"%s enter\n", __func__);

   D_OMXVDEC_CHECK_PTR_RET(pList);

   if (u32Index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
   {
      OmxPrint(OMX_FATAL,"want to delete index(%d) >= %d(max), error!!!\n",u32Index,OMXVDEC_MAX_REMAIN_FRM_NUM);
      return HI_FAILURE;
   }

   spin_lock_irqsave(&pList->bypass_lock, flags);

   pSpecialFrmRec = &pList->stSpecialFrmRec[u32Index];

   //清空该节点内容，视为空闲节点
   memset(pSpecialFrmRec, 0, sizeof(OMXVDEC_FRM_INFO_S));
   pList->s32Num--;
   if (pList->s32Num < 0)
   {
      OmxPrint(OMX_FATAL,"the vdec global remain frame < 0 ??!  force to be 0!\n");
      pList->s32Num = 0;
   }

   OmxPrint(OMX_INFO,"%s phy:0x%x size:%d\n", __func__, pSpecialFrmRec->frmBufRec.u32StartPhyAddr, pSpecialFrmRec->frmBufRec.u32Size);

   spin_unlock_irqrestore(&pList->bypass_lock, flags);

   OmxPrint(OMX_TRACE,"%s exit\n", __func__);

   return HI_SUCCESS;
}

/******************************************************/
/***            获取special帧信息                   ***/
/** 提供给 vdec_ctrl 查看proc信息的函数             ***/
/******************************************************/
HI_S32 omxvdec_get_occoupied_frame_info(OMXVDEC_PROC_OCCOUPY_FRAME_INFO *p_proc_occoupy_frame)
{
    HI_U32 u32Index = 0;
    HI_U32 SpecialFrmCnt = 0;

    OMXVDEC_FRM_INFO_S *p_occoupy_frame;

    p_proc_occoupy_frame->occoupy_frame_num = g_OmxVdec->stRemainFrmList.s32Num;

    if (p_proc_occoupy_frame->occoupy_frame_num != 0)
    {
        for (u32Index = 0; u32Index < OMXVDEC_MAX_REMAIN_FRM_NUM; u32Index++)
        {
            p_occoupy_frame = &g_OmxVdec->stRemainFrmList.stSpecialFrmRec[u32Index];
            if (p_occoupy_frame->frmBufRec.u32StartPhyAddr != 0)
            {
                memcpy(&p_proc_occoupy_frame->frmBufRec[SpecialFrmCnt], &p_occoupy_frame->frmBufRec, sizeof(MMZ_BUFFER_S));
                SpecialFrmCnt++;
            }
        }
    }

    return HI_SUCCESS;
}

#endif

#ifdef MODULE
module_init(OMXVDEC_DRV_ModInit);
module_exit(OMXVDEC_DRV_ModExit);
#endif



