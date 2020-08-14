/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    decoder_vfmw.c
 *
 * Purpose: omxvdec decoder vfmw functions
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#include "hi_drv_module.h"  // HI_DRV_MODULE_GetFunction 依赖
#include "hi_module.h"      // HI_ID_OMXVDEC
#include "hi_drv_stat.h"
#include "decoder.h"
#include "vfmw_ext.h"


/*================ EXTERN VALUE ================*/
extern OMXVDEC_ENTRY *g_OmxVdec;
extern OMXVDEC_FUNC g_stOmxFunc;

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
extern MMZ_BUFFER_S g_stVDHMMZ;
extern HI_BOOL g_bVdecPreVDHMMZUsed;
extern HI_U32 g_VdecPreVDHMMZUsedSize;
extern VDEC_PREMMZ_NODE_S st_VdecChanPreUsedMMZInfo[];
extern HI_U32 g_VdecPreMMZNodeNum;
#endif

HI_S32 processor_inform_img_ready(OMXVDEC_CHAN_CTX *pchan);

/*================ STATIC VALUE ================*/
VFMW_EXPORT_FUNC_S* pVfmwFunc = HI_NULL;


/*=================== MACRO ====================*/
#define SOFT_DEC_MIN_MEM_SD     (22*1024*1024)
#define SOFT_DEC_MIN_MEM_HD     (45*1024*1024)

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
#define HI_VDEC_MAX_PRENODE_NUM 100
#endif


/*================ GLOBAL VALUE ================*/
HI_U32   g_DispNum               = 4;
HI_U32   g_SegSize               = 2;         // (M)
HI_BOOL  g_DynamicFsEnable       = HI_TRUE;
HI_BOOL  g_LowDelayStatistics    = HI_FALSE;  //HI_TRUE;
HI_BOOL  g_RawMoveEnable         = HI_TRUE;   // 码流搬移使能标志，解决scd切割失败不释放码流的情况
HI_BOOL  g_FastOutputMode        = HI_FALSE;
HI_BOOL  g_CompressEnable        = VFMW_COMPRESS_SUPPORT;


/*================== DATA TYPE =================*/
typedef enum {
    CFG_CAP_SD = 0,
    CFG_CAP_HD,
    CFG_CAP_UHD,
    CFG_CAP_BUTT,
}eCFG_CAP;


/*============== INTERNAL FUNCTION =============*/
#ifdef VFMW_VPSS_BYPASS_EN
HI_S32 decoder_find_special2Normal_Index(OMXVDEC_CHAN_CTX *pchan,HI_U32 u32Phyaddr, HI_U32 *pIndex)
{
    HI_U32 index;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    D_OMXVDEC_CHECK_PTR_RET(pchan);

    for (index=0; index < MAX_DFS_BUF_NUM; index++)
    {
        pBufSlot = &(pchan->dfs.single_buf[index]);

        if (pBufSlot->frm_buf.u32StartPhyAddr  == u32Phyaddr)
        {
           *pIndex = index;
           return HI_SUCCESS;
        }
    }
    OmxPrint(OMX_FATAL,"find normal frame buffer index error!Phyaddr = 0x%x\n",u32Phyaddr);
    return HI_FAILURE;
}


HI_S32 decoder_record_occoupied_frame(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;
    VDEC_SPECIAL_FRM_S stReportFrmRec;
    SINT32 s32ImgOutputEn = 0;
    HI_U32 i;
    HI_U32 index = 0;
    unsigned long   flags;
    OMXVDEC_FRM_INFO_S stSpecialFrmInfo;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    memset(&stReportFrmRec, 0, sizeof(VDEC_SPECIAL_FRM_S));

    spin_lock_irqsave(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);

    ret = (pVfmwFunc->pfnVfmwControl)( pchan->decoder_id, VDEC_CID_SET_IMG_OUTPUT_INFO, &s32ImgOutputEn);
    if (VDEC_OK != ret)
    {
        OmxPrint(OMX_ERR, "Chan %d VDEC_CID_SET_IMG_OUTPUT_INFO (enable = %d) err!\n",  pchan->decoder_id,s32ImgOutputEn);
    }

    ret = (pVfmwFunc->pfnVfmwControl)( pchan->decoder_id, VDEC_CID_REPORT_OCCUQIED_FRM, &stReportFrmRec);
    if (VDEC_OK != ret)
    {
        OmxPrint(OMX_ERR, "Chan %d VDEC_CID_REPORT_OCCUQIED_FRM err!\n", pchan->decoder_id);
    }
    else
    {
        OmxPrint(OMX_INFO,"Chan %d VDEC_CID_REPORT_OCCUQIED_FRM occupied frame = %d!\n", pchan->decoder_id,stReportFrmRec.specialFrameNum);

        for (i = 0; i < stReportFrmRec.specialFrameNum ; i++)
        {
            stSpecialFrmInfo.bCanRls                   = HI_FALSE;
            stSpecialFrmInfo.frmBufRec.u32StartPhyAddr = stReportFrmRec.specialFrmRec[i].PhyAddr;
            stSpecialFrmInfo.frmBufRec.pu8StartVirAddr = (HI_U8*)(HI_SIZE_T)stReportFrmRec.specialFrmRec[i].VirAddr;
            stSpecialFrmInfo.frmBufRec.u32Size         = stReportFrmRec.specialFrmRec[i].Size;

            if (OMXVDEC_Frame_in_List(&g_OmxVdec->stRemainFrmList, &stSpecialFrmInfo) == HI_SUCCESS)
            {
                continue;
            }

            if (HI_SUCCESS == decoder_find_special2Normal_Index(pchan, stSpecialFrmInfo.frmBufRec.u32StartPhyAddr, &index))
            {
                stSpecialFrmInfo.enbufferNodeStatus       =  pchan->dfs.single_buf[index].frm_buf_type;
                pchan->dfs.single_buf[index].is_CanRls    = HI_FALSE;
            }
            else
            {
                OmxPrint(OMX_ERR,"special Frame 0x%x,can't find match single buffer node\n", stSpecialFrmInfo.frmBufRec.u32StartPhyAddr);
                stSpecialFrmInfo.enbufferNodeStatus       = ALLOC_INVALID;
            }

            /*fix me!! 安全的内存能区分吗?*/
            OmxPrint(OMX_INFO, "add list  pstChan->specialFrmRec[%d],u32StartPhyAddr= 0x%x,u32StartVirAddr = 0x%p,u32Size = %d\n",
                        i,stSpecialFrmInfo.frmBufRec.u32StartPhyAddr ,stSpecialFrmInfo.frmBufRec.pu8StartVirAddr,stSpecialFrmInfo.frmBufRec.u32Size);
            ret = OMXVDEC_List_Add(&g_OmxVdec->stRemainFrmList, &stSpecialFrmInfo);
            if (ret != HI_SUCCESS)
            {
                OmxPrint(OMX_ERR,"OMXVDEC_List_Add error!!!\n");
            }
        }
    }
    s32ImgOutputEn = 1;
    ret = (pVfmwFunc->pfnVfmwControl)( pchan->decoder_id, VDEC_CID_SET_IMG_OUTPUT_INFO, &s32ImgOutputEn);

    if (VDEC_OK != ret)
    {
        OmxPrint(OMX_INFO, "Chan %d VDEC_CID_SET_IMG_OUTPUT_INFO (enable = %d) err!\n",  pchan->decoder_id, s32ImgOutputEn);
    }

    spin_unlock_irqrestore(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);

    OmxPrint(OMX_TRACE, "%s exit!\n", __func__);

    return ret;
}

/******************************************************/
/** 内部还帧释放帧存接口                            ***/
/** 在omx的线程中调用                               ***/
/******************************************************/
HI_S32 decoder_global_release_frame_inter(HI_U32 u32Index)
{
    HI_S32 s32Ret = HI_FAILURE;
    OMXVDEC_FRM_INFO_S* pSpecialFrmInfo;

    if (u32Index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
    {
        OmxPrint(OMX_FATAL,"want to release index(%d) >= %d(max), error!!!\n", u32Index, OMXVDEC_MAX_REMAIN_FRM_NUM);
        return HI_FAILURE;
    }

    pSpecialFrmInfo = &g_OmxVdec->stRemainFrmList.stSpecialFrmRec[u32Index];

    if (pSpecialFrmInfo->frmBufRec.u32StartPhyAddr != 0)
    {
        OmxPrint(OMX_INFO, "%s vir:0x%p phy:0x%x  type:%d\n", __func__, pSpecialFrmInfo->frmBufRec.pu8StartVirAddr, \
                 pSpecialFrmInfo->frmBufRec.u32StartPhyAddr, pSpecialFrmInfo->enbufferNodeStatus);

        omxvdec_release_mem(&pSpecialFrmInfo->frmBufRec, pSpecialFrmInfo->enbufferNodeStatus);
    }

    s32Ret = OMXVDEC_List_Del(&g_OmxVdec->stRemainFrmList, u32Index);

    return s32Ret;
}

/******************************************************/
/***             全局还帧接口                       ***/
/** 此处输入的 pstFrame 应对应metadata强转中的信息  ***/
/******************************************************/
HI_S32 decoder_global_release_frame(HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32Index = 0;
    OMXVDEC_FRM_INFO_S* pSpecialFrmInfo;
    D_OMXVDEC_CHECK_PTR_RET(pstFrame);

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (!((0 != g_OmxVdec->stRemainFrmList.s32Num) && (channel_IsOccupiedFrm(pstFrame->stBufAddr[0].u32PhyAddr_YHead, &u32Index))))
    {
        /* this frame isn't occupied frame */
        OmxPrint(OMX_FATAL, "%s frame phy(0x%x) is invalid! remain num:%d\n", __func__, \
                 pstFrame->stBufAddr[0].u32PhyAddr_Y, g_OmxVdec->stRemainFrmList.s32Num);

        return HI_FAILURE;
    }

    if (u32Index >= OMXVDEC_MAX_REMAIN_FRM_NUM)
    {
        OmxPrint(OMX_FATAL, "%s u32Index = %d is invalid!\n", __func__, u32Index);

        return HI_FAILURE;
    }
    else
    {
        pSpecialFrmInfo = &g_OmxVdec->stRemainFrmList.stSpecialFrmRec[u32Index];
    }

    if (pSpecialFrmInfo->frmBufRec.u32StartPhyAddr != 0)
    {
        OmxPrint(OMX_ERR, "%s vir:0x%p phy:0x%x  type:%d\n", __func__, pSpecialFrmInfo->frmBufRec.pu8StartVirAddr, \
            pSpecialFrmInfo->frmBufRec.u32StartPhyAddr, pSpecialFrmInfo->enbufferNodeStatus);

        omxvdec_release_mem(&pSpecialFrmInfo->frmBufRec, pSpecialFrmInfo->enbufferNodeStatus);
    }

    s32Ret = OMXVDEC_List_Del(&g_OmxVdec->stRemainFrmList, u32Index);

    OmxPrint(OMX_TRACE, "%s exit!\n", __func__);

    return s32Ret;
}

EXPORT_SYMBOL(decoder_global_release_frame);

#endif

static HI_S32 decoder_event_new_image(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    IMAGE *pstImg = (IMAGE *)(pargs);

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    if ((pchan->bLowdelay || g_FastOutputMode) && (NULL != pchan->omx_vdec_lowdelay_proc_rec))
    {
        channel_add_lowdelay_tag_time(pchan, pstImg->Usertag, OMX_LOWDELAY_REC_VFMW_RPO_IMG_TIME, OMX_GetTimeInMs());
    }
    pchan->omx_chan_statinfo.DecoderOut++;

    return processor_inform_img_ready(pchan);
}

static HI_S32 decoder_event_need_arrange(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    unsigned long flags;

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    OmxPrint(OMX_INFO, "Get EVNT_NEED_ARRANGE Report!\n");
    HI_DRV_SYS_GetTimeStampMs(&pchan->dfs.delay_time);

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    pchan->dfs.dfs_state     = DFS_WAIT_RESPONSE_0;
    pchan->dfs.need_frm_num  = ((HI_U32*)pargs)[0];
    pchan->dfs.need_frm_size = ((HI_U32*)pargs)[1];

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    g_OmxVdec->task.task_state = TASK_STATE_ONCALL;
    wake_up(&g_OmxVdec->task.task_wait);

    return HI_SUCCESS;
}

static HI_S32 decoder_event_arrange_frame_buffer(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    unsigned long flags;
    HI_S32 ret = HI_FAILURE;
    HI_U32 need_frm_num = 0;

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    OmxPrint(OMX_INFO, "Get EVNT_ARRANGE_FRAME_BUF Report!\n");
    decoder_record_occoupied_frame(pchan);
    HI_DRV_SYS_GetTimeStampMs(&pchan->dfs.delay_time);

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    pchan->dfs.dfs_state      = DFS_WAIT_RESPONSE_1;
    pchan->dfs.need_frm_num   = ((HI_U32*)pargs)[0];
    pchan->dfs.need_frm_size  = ((HI_U32*)pargs)[1];
    pchan->dfs.need_pmv_num   = ((HI_U32*)pargs)[2];
    pchan->dfs.need_pmv_size  = ((HI_U32*)pargs)[3];
    pchan->dfs.alloc_frame_only = 0;

#ifdef HI_TVOS_SUPPORT
    if (pchan->bVpssBypass)
#else
    if (pchan->is_overlay && pchan->bVpssBypass)
#endif
    {
        pchan->dfs.need_frm_num += MAX(g_DispNum, 6);
    }
    else
    {
        pchan->dfs.need_frm_num += g_DispNum;
    }

    //SMMU下，pchan的成员不能直接传递到安全侧，否则会导致无法读到正确值
    need_frm_num = pchan->dfs.need_frm_num;

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_SET_FRAME_BUFFER_NUM, (HI_VOID *)&need_frm_num);
    if(ret != VDEC_OK)
    {
        OmxPrint(OMX_ERR, "call vfmw command VDEC_CID_SET_FRAME_BUFFER_NUM err!\n");

        return HI_FAILURE;
    }

    g_OmxVdec->task.task_state = TASK_STATE_ONCALL;
    wake_up(&g_OmxVdec->task.task_wait);

    return HI_SUCCESS;
}

static HI_S32 decoder_event_alloc_frame_buffer_only(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    unsigned long flags;

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    pchan->dfs.dfs_state        = DFS_WAIT_RESPONSE_1;
    pchan->dfs.image_num_only   = ((HI_U32*)pargs)[0];; //前次没有申请的帧存
    pchan->dfs.image_size_only  = ((HI_U32*)pargs)[1];
    pchan->dfs.alloc_frame_only = 1;

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    g_OmxVdec->task.task_state = TASK_STATE_ONCALL;
    wake_up(&g_OmxVdec->task.task_wait);

    return HI_SUCCESS;
}

static HI_S32 decoder_event_release_frame_buffer_only(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    HI_U32 index = 0;
    unsigned long flags;
    HI_U32 release_phy_addr = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    release_phy_addr = (HI_U32)((HI_U64 *)pargs)[0];

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    pchan->dfs.dfs_state        = DFS_WAIT_RESPONSE_1;
    pchan->dfs.alloc_frame_only = 1;

    for(index = 0; index < MAX_DFS_BUF_NUM; index++)
    {
        pBufSlot = &(pchan->dfs.single_buf[index]);

        if(pBufSlot->frm_buf.u32StartPhyAddr == release_phy_addr)
        {
            pBufSlot->is_wait_release = HI_TRUE;    //节点帧存等待释放

            break;
        }
    }

    if(index >= MAX_DFS_BUF_NUM)
    {
        OmxPrint(OMX_ERR, "EVNT_RELSE_ONLY  PhyAddr=%x not exist !!!\n", release_phy_addr);
    }

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    g_OmxVdec->task.task_state = TASK_STATE_ONCALL;
    wake_up(&g_OmxVdec->task.task_wait);

    return HI_SUCCESS;
}

static HI_S32 decoder_event_last_frame(OMXVDEC_CHAN_CTX  *pchan, HI_VOID *pargs)
{
    HI_U32 *ptemp = (HI_U32 *)pargs;

    OMXVDEC_ASSERT_RETURN(pargs != HI_NULL, "pargs = NULL");

    OmxPrint(OMX_INFO, "Get Last Frame Report!\n");

    /* pargs[0]-> 0: success, 1: fail,  2+: report last frame image id */
    pchan->last_frame_info[0] = DECODER_REPORT_LAST_FRAME;
    pchan->last_frame_info[1] = ptemp[0];
    if (REPORT_LAST_FRAME_SUCCESS == ptemp[0])
    {
        OmxPrint(OMX_INFO, "Last frame report success!\n");
    }
    else if (REPORT_LAST_FRAME_FAIL == ptemp[0])
    {
        OmxPrint(OMX_ERR, "Last frame report failed!\n");
    }
    else
    {
        pchan->last_frame_info[1]  = REPORT_LAST_FRAME_WITH_ID;
        pchan->last_frame_image_id = ptemp[0];
        OmxPrint(OMX_ERR, "Last frame report image id %d!\n", pchan->last_frame_image_id);
    }

    return HI_SUCCESS;
}

static HI_S32 decoder_event_handler(HI_S32 chan_id, HI_S32 event_type, HI_VOID *pargs)
{
    HI_S32 ret = HI_FAILURE;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    pchan = channel_find_inst_by_decoder_id(g_OmxVdec, chan_id);
    if (HI_NULL == pchan)
    {
        OmxPrint(OMX_FATAL, "%s can't find Chan(%d).\n", __func__, chan_id);
        return HI_FAILURE;
    }

    if(pchan->TunnelModeEnable)
    {
        return HI_SUCCESS;
    }

    switch (event_type)
    {
        case EVNT_NEW_IMAGE:
            ret = decoder_event_new_image(pchan, pargs);
            break;

        case EVNT_NEED_ARRANGE:
            ret = decoder_event_need_arrange(pchan, pargs);
            break;

        case EVNT_ARRANGE_FRAME_BUF:
            ret = decoder_event_arrange_frame_buffer(pchan, pargs);
            break;

        case EVNT_ALLOC_FRAME_BUF_ONLY:
            ret = decoder_event_alloc_frame_buffer_only(pchan, pargs);
            break;

        case EVNT_RELSE_FRAME_BUF_ONLY:
            ret = decoder_event_release_frame_buffer_only(pchan, pargs);
            break;

        case EVNT_LAST_FRAME:
            ret = decoder_event_last_frame(pchan, pargs);
            break;

        default:
            //OmxPrint(OMX_INFO, "\nUnsupport Event Type: 0x%4.4x, arg=%p\n", event_type, pargs);
            break;
    }

    return ret;
}

static eCFG_CAP decoder_get_config_cap(OMXVDEC_CHAN_CTX *pchan)
{
    eCFG_CAP ConfigCap = CFG_CAP_HD;

    if (pchan->out_width*pchan->out_height > HD_FRAME_WIDTH*HD_FRAME_HEIGHT)
    {
        ConfigCap = CFG_CAP_UHD;
    }
    else if (pchan->out_width*pchan->out_height > SD_FRAME_WIDTH*SD_FRAME_HEIGHT)
    {
        ConfigCap = CFG_CAP_HD;
    }
    else
    {
        ConfigCap = CFG_CAP_SD;
    }

    if (pchan->protocol == STD_H263 || pchan->protocol == STD_SORENSON)
    {
        ConfigCap = CFG_CAP_HD;
    }

    return ConfigCap;
}

static HI_VOID decoder_init_capability(OMXVDEC_CHAN_CTX *pchan, VDEC_CHAN_CFG_S *pchan_cfg, VDEC_CHAN_CAP_LEVEL_E *pFmwCap, VDEC_CHAN_OPTION_S *pstOption)
{
    HI_S32 MaxWidth;
    HI_S32 MaxHeight;
    HI_S32 SCDBufSize;
    HI_S32 MaxRefFrameNum;
    eCFG_CAP ConfigCap = decoder_get_config_cap(pchan);

    switch (ConfigCap)
    {
        case CFG_CAP_SD:
            MaxWidth   = SD_FRAME_WIDTH;
            MaxHeight  = SD_FRAME_HEIGHT;
            SCDBufSize = g_SegSize * 1024 * 1024;
            MaxRefFrameNum = 12;

            if (pchan_cfg->eVidStd == STD_HEVC)
                *pFmwCap = CAP_LEVEL_HEVC_720;
            else if (pchan_cfg->eVidStd == STD_H264)
                *pFmwCap = CAP_LEVEL_H264_720;
            else
                *pFmwCap = CAP_LEVEL_MPEG_720;

            break;

        case CFG_CAP_HD:
        default:
            MaxWidth   = HD_FRAME_WIDTH;
            MaxHeight  = HD_FRAME_HEIGHT;
            SCDBufSize = g_SegSize*1024*1024;
            MaxRefFrameNum = 10;

            if (pchan_cfg->eVidStd == STD_HEVC)
                *pFmwCap = CAP_LEVEL_HEVC_FHD;
            else if (pchan_cfg->eVidStd == STD_H264)
                *pFmwCap = CAP_LEVEL_H264_FHD;
            else
                *pFmwCap = CAP_LEVEL_MPEG_FHD;

            break;

        case CFG_CAP_UHD:
            MaxWidth   = UHD_FRAME_WIDTH;
            MaxHeight  = UHD_FRAME_HEIGHT;
            SCDBufSize = 4 * g_SegSize * 1024 * 1024;
            MaxRefFrameNum = 4;

            if (pchan_cfg->eVidStd == STD_HEVC)
                *pFmwCap = CAP_LEVEL_HEVC_UHD;
            else
                *pFmwCap = CAP_LEVEL_4096x2160;

            break;
    }

    if (pchan_cfg->eVidStd == STD_MVC)
    {
        *pFmwCap = CAP_LEVEL_MVC_FHD;
    }

    pstOption->s32MaxWidth   = MAX(MaxWidth,  pchan->out_width);
    pstOption->s32MaxHeight  = MAX(MaxHeight, pchan->out_height);
    pstOption->s32SCDBufSize = SCDBufSize;

    if (pchan_cfg->eVidStd == STD_HEVC || pchan_cfg->eVidStd == STD_H264 || pchan_cfg->eVidStd == STD_MVC)
    {
        pstOption->s32SupportH264 = 1;
        pstOption->s32MaxSliceNum = 136;
        pstOption->s32MaxSpsNum   = 32;
        pstOption->s32MaxPpsNum   = 256;
        pstOption->s32MaxRefFrameNum = MaxRefFrameNum;
    }
    else
    {
        if (pchan_cfg->eVidStd == STD_VP8)
        {
            pstOption->s32MaxRefFrameNum = 3;
        }
        else if ((pchan_cfg->eVidStd != STD_H263) && (pchan_cfg->eVidStd != STD_SORENSON))
        {
            pstOption->s32MaxRefFrameNum = 2;
        }
    }

    return;
}

static HI_S32 decoder_init_option(OMXVDEC_CHAN_CTX *pchan, VDEC_CHAN_CFG_S *pchan_cfg, VDEC_CHAN_CAP_LEVEL_E *pFmwCap, VDEC_CHAN_OPTION_S *pstOption)
{
    if (pchan == HI_NULL || pchan_cfg == HI_NULL || pFmwCap == HI_NULL || pstOption == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s param invalid!\n", __func__);

        return HI_FAILURE;
    }

    memset(pstOption, 0, sizeof(VDEC_CHAN_OPTION_S));

    pstOption->eAdapterType         = ADAPTER_TYPE_OMXVDEC;
    pstOption->MemAllocMode         = MODE_PART_BY_SDK;
    pstOption->s32SupportBFrame     = 1;
    pstOption->s32ReRangeEn         = 1;
    pstOption->s32DisplayFrameNum   = g_DispNum;
    pstOption->s32IsSecMode         = pchan->is_tvp;

    decoder_init_capability(pchan, pchan_cfg, pFmwCap, pstOption);

    if (g_DynamicFsEnable == HI_TRUE && (pchan->protocol != STD_H263 && pchan->protocol != STD_SORENSON))
    {
        pstOption->u32DynamicFrameStoreAllocEn = 1;
        pstOption->s32ExtraFrameStoreNum       = g_DispNum;
        pstOption->u32CfgFrameNum              = 0;
        pstOption->s32DelayTime                = 0;
        pstOption->u32MaxMemUse                = -1;
    }

    return HI_SUCCESS;
}

static HI_S32 decoder_get_stream(HI_S32 chan_id, STREAM_DATA_S *stream_data)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    OMXVDEC_BUF_S *pbuf     = HI_NULL;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    pchan = channel_find_inst_by_channel_id(g_OmxVdec, chan_id);
    if (pchan == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s can't find Chan(%d).\n", __func__, chan_id);

        return HI_FAILURE;
    }

    spin_lock_irqsave(&pchan->chan_lock, flags);
    if (pchan->state != CHAN_STATE_WORK)
    {
        spin_unlock_irqrestore(&pchan->chan_lock, flags);

        return HI_FAILURE;
    }
    spin_unlock_irqrestore(&pchan->chan_lock, flags);

    spin_lock_irqsave(&pchan->raw_lock, flags);
    if (list_empty(&pchan->raw_queue))
    {
        spin_unlock_irqrestore(&pchan->raw_lock, flags);

        return HI_FAILURE;
    }

    if (pchan->input_flush_pending)
    {
        spin_unlock_irqrestore(&pchan->raw_lock, flags);
        OmxPrint(OMX_INBUF, "Invalid: input_flush_pending\n");

        return HI_FAILURE;
    }

    list_for_each_entry(pbuf, &pchan->raw_queue, list)
    {
        if(BUF_STATE_USING == pbuf->status)
        {
            continue;
        }

        memset(stream_data, 0, sizeof(STREAM_DATA_S));

        pbuf->status                      = BUF_STATE_USING;
        stream_data->PhyAddr              = pbuf->phy_addr + pbuf->offset;
        stream_data->VirAddr              = (UINT64)(HI_SIZE_T)(pbuf->kern_vaddr + pbuf->offset);
        stream_data->Length               = pbuf->act_len;
        stream_data->Pts                  = pbuf->time_stamp;
        stream_data->RawExt.Flags         = pbuf->flags;
        stream_data->RawExt.BufLen        = pbuf->buf_len;
        stream_data->RawExt.CfgWidth      = pchan->out_width;
        stream_data->RawExt.CfgHeight     = pchan->out_height;
        stream_data->UserTag              = pbuf->usr_tag;

        if (pchan->seek_pending)
        {
            stream_data->RawExt.IsSeekPending = 1;
            pchan->seek_pending = 0;
        }

        if (pbuf->flags & VDEC_BUFFERFLAG_ENDOFFRAME)
        {
            stream_data->is_not_last_packet_flag = 0;
        }
        else
        {
            stream_data->is_not_last_packet_flag = 1;
        }

        if (pbuf->buf_id == LAST_FRAME_BUF_ID)
        {
            OmxPrint(OMX_INFO, "vfmw read last frame.\n");
            stream_data->is_stream_end_flag = 1;
        }

        pchan->raw_use_cnt++;

        OmxPrint(OMX_PTS, "Get Time Stamp: %lld\n", stream_data->Pts);

        ret = HI_SUCCESS;

        if ((pchan->bLowdelay || g_FastOutputMode) && (NULL != pchan->omx_vdec_lowdelay_proc_rec))
        {
            channel_add_lowdelay_tag_time(pchan, stream_data->UserTag, OMX_LOWDELAY_REC_VFMW_RCV_STRM_TIME, OMX_GetTimeInMs());
        }

        OmxPrint(OMX_INBUF, "VFMW got stream: PhyAddr = 0x%08x, Len = %d\n",
                 stream_data->PhyAddr, stream_data->Length);
        pchan->omx_chan_statinfo.DecoderIn++;
        break;
    }

    spin_unlock_irqrestore(&pchan->raw_lock, flags);

    return ret;
}

HI_S32 decoder_get_stream_ex(HI_S32 chan_id, HI_VOID* stream_data)
{
    HI_S32 ret;

    ret = decoder_get_stream(chan_id, (STREAM_DATA_S*) stream_data);
    if (ret == HI_SUCCESS)
    {
        if(HI_INVALID_PTS != ((STREAM_DATA_S*)stream_data)->Pts)
        {
            do_div(((STREAM_DATA_S*)stream_data)->Pts, 1000);
        }
    }

    return ret;
}
static HI_S32 decoder_release_stream(HI_S32 chan_id, STREAM_DATA_S *stream_data)
{
    unsigned long flags;
    HI_S32        is_find = 0;
    OMXVDEC_BUF_S *pbuf = HI_NULL;
    OMXVDEC_BUF_S *ptmp = HI_NULL;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;
    OMXVDEC_BUF_DESC  user_buf;

    if (stream_data == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s stream_data = NULL.\n", __func__);

        return HI_FAILURE;
    }

    pchan = channel_find_inst_by_channel_id(g_OmxVdec, chan_id);
    if (pchan == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s can't find Chan(%d).\n", __func__, chan_id);

        return HI_FAILURE;
    }

    /* for we del element during, so use safe methods for list */
    spin_lock_irqsave(&pchan->raw_lock, flags);
    if (list_empty(&pchan->raw_queue))
    {
        spin_unlock_irqrestore(&pchan->raw_lock, flags);
        OmxPrint(OMX_ERR, "%s: list is empty\n", __func__);

        return HI_SUCCESS;
    }

    list_for_each_entry_safe(pbuf, ptmp, &pchan->raw_queue, list)
    {
        if (stream_data->PhyAddr == (pbuf->phy_addr + pbuf->offset))
        {
            if (pbuf->status != BUF_STATE_USING)
            {
               OmxPrint(OMX_ERR, "%s: buf(0x%08x) flag confused!\n",__func__,  stream_data->PhyAddr);
            }

            pbuf->status = BUF_STATE_IDLE;
            list_del(&pbuf->list);
            is_find = 1;
            break;
        }
    }

    if (!is_find)
    {
        spin_unlock_irqrestore(&pchan->raw_lock, flags);
        OmxPrint(OMX_ERR, "%s: buffer(0x%08x) not in queue!\n", __func__, stream_data->PhyAddr);

        return HI_FAILURE;
    }

    if (pbuf->buf_id != LAST_FRAME_BUF_ID)
    {
        /* let msg to indicate buffer was given back */

        user_buf.dir         = PORT_DIR_INPUT;
        user_buf.bufferaddr  = (HI_SIZE_T)pbuf->user_vaddr;
        user_buf.buffer_len  = pbuf->buf_len;
        user_buf.client_data = (HI_SIZE_T)pbuf->client_data;
        user_buf.data_len    = 0;
        user_buf.timestamp   = 0;
        user_buf.phyaddr     = pbuf->phy_addr;

        pbuf->act_len        = user_buf.data_len;

        message_queue(pchan->msg_queue, VDEC_MSG_RESP_INPUT_DONE, VDEC_S_SUCCESS, (HI_VOID *)&user_buf);
        pchan->omx_chan_statinfo.EBD++;
    }
    else
    {
        pchan->eos_in_list = 0;
        OmxPrint(OMX_INFO, "vfmw release last frame.\n");
    }

    pchan->raw_use_cnt = (pchan->raw_use_cnt > 0) ? (pchan->raw_use_cnt-1) : 0;

    if (pchan->input_flush_pending && (pchan->raw_use_cnt == 0))
    {
        OmxPrint(OMX_INBUF, "%s: input flush done!\n", __func__);
        message_queue(pchan->msg_queue, VDEC_MSG_RESP_FLUSH_INPUT_DONE, VDEC_S_SUCCESS, HI_NULL);
        pchan->input_flush_pending = 0;
    }

    if ((pchan->bLowdelay || g_FastOutputMode) && (NULL != pchan->omx_vdec_lowdelay_proc_rec))
    {
        channel_add_lowdelay_tag_time(pchan, stream_data->UserTag, OMX_LOWDELAY_REC_VFMW_RLS_STRM_TIME, OMX_GetTimeInMs());
    }
    spin_unlock_irqrestore(&pchan->raw_lock, flags);

    OmxPrint(OMX_INBUF, "VFMW release stream: PhyAddr = 0x%08x, Len = %d\n",
                        stream_data->PhyAddr, stream_data->Length);

    return HI_SUCCESS;

}

HI_S32 decoder_release_stream_ex(HI_S32 chan_id, HI_VOID *stream_data)
{
    return decoder_release_stream(chan_id, (STREAM_DATA_S*) stream_data);
}

static inline HI_S32 decoder_clear_stream(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_RELEASE_STREAM, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s release stream failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

static inline HI_S32 decoder_alloc_mem(OMXVDEC_CHAN_CTX *pchan, HI_VOID *pFsParam)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_ALLOC_MEM_TO_CHANNEL, pFsParam);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s alloc mem to channel failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

static inline HI_S32 decoder_config_mem(OMXVDEC_CHAN_CTX *pchan, HI_VOID *pFsParam)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_ALLOC_MEM_TO_DECODER, pFsParam);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s config mem to channel failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_acquire_resource(OMXVDEC_CHAN_CTX *pchan, VDEC_CHAN_CAP_LEVEL_E enFmwCap, DETAIL_MEM_SIZE *pMemSize, VDEC_CHAN_OPTION_S *pOption)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32VDHSize = 0;
    HI_BOOL IsSoftDecFlag = (STD_H263 == pchan->protocol || STD_SORENSON == pchan->protocol);

#ifdef HI_OMX_TEE_SUPPORT
    if (HI_TRUE == pchan->is_tvp)
    {
        pOption->s32IsSecMode = 1;
        pOption->MemAllocMode = MODE_ALL_BY_MYSELF;

        return HI_SUCCESS;
    }
    else
#endif
    {
        pOption->s32IsSecMode = 0;

        /* Alloc VDH buffer */
        if (IsSoftDecFlag == HI_TRUE)
        {
            u32VDHSize = (enFmwCap < CAP_LEVEL_MPEG_FHD) ? SOFT_DEC_MIN_MEM_SD : SOFT_DEC_MIN_MEM_HD;
            u32VDHSize = MAX(pMemSize->VdhDetailMem, u32VDHSize);
        }
        else
        {
            u32VDHSize = (pOption->u32DynamicFrameStoreAllocEn == 1) ? 0 : pMemSize->VdhDetailMem;
        }

        if (u32VDHSize > 0)
        {
            if (IsSoftDecFlag == HI_TRUE)
            {
                ret = HI_DRV_OMXVDEC_Alloc("OMXVDEC_SOFTVDH", HI_NULL, u32VDHSize, 0, &pchan->decoder_vdh_buf, 0);
                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for VDH failed\n", __func__);
                    goto error0;
                }

                pchan->eVDHMemAlloc = ALLOC_BY_MMZ_UNMAP;

                ret = HI_DRV_OMXVDEC_MapCache(&pchan->decoder_vdh_buf);
                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for soft VDH failed\n", __func__);
                    goto error1;
                }

                pchan->eVDHMemAlloc = ALLOC_BY_MMZ;
            }
            else
            {
            #if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
                pchan->decoder_vdh_buf.u32Size = u32VDHSize;
                ret = VDEC_Chan_FindPreMMZ(&pchan->decoder_vdh_buf);

                //if pre alloc mmz not available,alloc it by mmz
                if (ret == HI_SUCCESS)
                {
                    pchan->eVDHMemAlloc = ALLOC_BY_PRE;
                }
                else
            #endif
                {
                    ret = HI_DRV_OMXVDEC_Alloc("OMXVDEC_VDH", HI_NULL, u32VDHSize, 0, &pchan->decoder_vdh_buf, 0);
                    pchan->eVDHMemAlloc = ALLOC_BY_MMZ_UNMAP;
                }

                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for VDH failed\n", __func__);
                    goto error0;
                }
            }

            pOption->MemDetail.ChanMemVdh.Length  = pchan->decoder_vdh_buf.u32Size;
            pOption->MemDetail.ChanMemVdh.PhyAddr = pchan->decoder_vdh_buf.u32StartPhyAddr;
            pOption->MemDetail.ChanMemVdh.VirAddr = (UINT64)(HI_SIZE_T)pchan->decoder_vdh_buf.pu8StartVirAddr;
        }

        /* Alloc SCD buffer */
        if (pMemSize->ScdDetailMem > 0)
        {
            if (IsSoftDecFlag == HI_TRUE)
            {
                ret = HI_DRV_OMXVDEC_Alloc("OMXVDEC_SOFTSCD", HI_NULL, pMemSize->ScdDetailMem, 0, &pchan->decoder_scd_buf, 0);
                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for soft SCD failed\n", __func__);
                    goto error1;
                }
                pchan->eSCDMemAlloc = ALLOC_BY_MMZ_UNMAP;

                ret = HI_DRV_OMXVDEC_MapCache(&pchan->decoder_scd_buf);
                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for soft SCD failed\n", __func__);
                    goto error2;
                }
                pchan->eSCDMemAlloc = ALLOC_BY_MMZ;
            }
            else
            {
                ret = HI_DRV_OMXVDEC_AllocAndMap("OMXVDEC_SCD", HI_NULL, pMemSize->ScdDetailMem, 0, &pchan->decoder_scd_buf);
                if (ret != HI_SUCCESS)
                {
                    OmxPrint(OMX_FATAL, "%s alloc mem for SCD failed\n", __func__);
                    goto error1;
                }

                pchan->eSCDMemAlloc = ALLOC_BY_MMZ;
            }

            pOption->MemDetail.ChanMemScd.Length  = pchan->decoder_scd_buf.u32Size;
            pOption->MemDetail.ChanMemScd.PhyAddr = pchan->decoder_scd_buf.u32StartPhyAddr;
            pOption->MemDetail.ChanMemScd.VirAddr = (UINT64)(HI_SIZE_T)pchan->decoder_scd_buf.pu8StartVirAddr;
        }
    }

    return HI_SUCCESS;

error2:
    omxvdec_release_mem(&pchan->decoder_scd_buf, pchan->eSCDMemAlloc);

error1:
    omxvdec_release_mem(&pchan->decoder_vdh_buf, pchan->eVDHMemAlloc);

error0:
    return HI_FAILURE;
}

/*============ EXPORT INTERFACE =============*/
HI_S32 decoder_init(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    VDEC_OPERATION_S stInitParam;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    pVfmwFunc = HI_NULL;

    ret = HI_DRV_MODULE_GetFunction(HI_ID_VFMW, (HI_VOID**)&g_stOmxFunc.pDecoderFunc);
    if (ret != HI_SUCCESS || g_stOmxFunc.pDecoderFunc == HI_NULL)
    {
        OmxPrint(OMX_FATAL, "%s get vfmw function failed!\n", __func__);

        return HI_FAILURE;
    }

    pVfmwFunc = (VFMW_EXPORT_FUNC_S *)(g_stOmxFunc.pDecoderFunc);

    memset(&stInitParam, 0, sizeof(VDEC_OPERATION_S));

    stInitParam.is_secure    = HI_FALSE;
    stInitParam.eAdapterType = ADAPTER_TYPE_OMXVDEC;
    stInitParam.mem_malloc   = HI_NULL;
    stInitParam.mem_free     = HI_NULL;
    stInitParam.VdecCallback = decoder_event_handler;

    ret = (pVfmwFunc->pfnVfmwInit)(&stInitParam);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s init vfmw failed!\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_exit(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwExit)(HI_FALSE);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s exit vfmw failed!\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}


HI_S32 decoder_init_trusted(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    VDEC_OPERATION_S stInitParam;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    memset(&stInitParam, 0, sizeof(VDEC_OPERATION_S));

    stInitParam.is_secure = HI_TRUE;

    stInitParam.eAdapterType = ADAPTER_TYPE_OMXVDEC;
    stInitParam.VdecCallback = decoder_event_handler;

    stInitParam.mem_malloc   = HI_NULL;
    stInitParam.mem_free     = HI_NULL;
    ret = (pVfmwFunc->pfnVfmwInit)(&stInitParam);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "Init trusted decoder failed!n");

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_exit_trusted(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwExit)(HI_TRUE);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "Exit trusted decoder failed\n");

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}


HI_S32 decoder_create_inst(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_DRV_CFG *pdrv_cfg)
{
    HI_S32 ret = HI_FAILURE;
    HI_S8 as8TmpBuf[128];
    VDEC_CHAN_CAP_LEVEL_E enFmwCap;
    VDEC_CHAN_OPTION_S    stOption;
    DETAIL_MEM_SIZE       stMemSize;
    VDEC_CHAN_CFG_S      *pchan_cfg = HI_NULL;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (pchan->is_tvp == HI_TRUE)
    {
        ret = decoder_init_trusted();
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_FATAL, "%s call decoder_init_vfmw failed\n", __func__);
            return HI_FAILURE;
        }
    }

    pchan_cfg = &pdrv_cfg->chan_cfg;
    ret = decoder_init_option(pchan, pchan_cfg, &enFmwCap, &stOption);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call decoder_init_option failed\n", __func__);
        goto error0;
    }

    ((HI_U64*)as8TmpBuf)[0] = (HI_U64)enFmwCap;
    ((HI_U64*)as8TmpBuf)[1] = (HI_U64)(HI_SIZE_T)&stOption; //@l00284814 64bit

    ret = (pVfmwFunc->pfnVfmwControl)(-1, VDEC_CID_GET_CHAN_DETAIL_MEMSIZE_WITH_OPTION, as8TmpBuf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call GET_CHAN_DETAIL_MEMSIZE failed\n", __func__);
        goto error0;
    }

    stMemSize = *(DETAIL_MEM_SIZE *)as8TmpBuf;

    ret = decoder_acquire_resource(pchan, enFmwCap, &stMemSize, &stOption);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s call decoder_acquire_resource failed\n", __func__);
        goto error0;
    }

    ((HI_U64*)as8TmpBuf)[0] = (HI_U64)enFmwCap;
    ((HI_U64*)as8TmpBuf)[1] = (HI_U64)(HI_SIZE_T)&stOption; //@l00284814 64bit

    ret = (pVfmwFunc->pfnVfmwControl)(-1, VDEC_CID_CREATE_CHAN_WITH_OPTION, as8TmpBuf);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s CREATE_CHAN_WITH_OPTION failed:%#x\n", __func__, ret);
        goto error1;
    }

    pchan->decoder_id = *(HI_U32 *)as8TmpBuf;
    OmxPrint(OMX_INFO, "Create decoder %d success!\n", pchan->decoder_id);

    // 如果不使能码流移动功能，将码流包个数设置最大即可
    if (g_RawMoveEnable == HI_FALSE)
    {
        pchan_cfg->s32MaxRawPacketNum = -1;
    }

    /* 在云游戏的场景，视频为H264，只有IP帧，如果使能快速输出模式，
       设置IP_MODE,SIMPLE_DPB,SCD lowdly
    */
    if (pchan->bLowdelay == HI_TRUE || g_FastOutputMode == HI_TRUE)
    {
        pchan_cfg->s32DecMode         = IP_MODE;

        /* set to 1 means Orderoutput */
        pchan_cfg->s32DecOrderOutput  = 1;
        pchan_cfg->s32LowdlyEnable    = HI_FALSE;
        pchan_cfg->s32ModuleLowlyEnable = HI_TRUE;
        pchan_cfg->s32ChanLowlyEnable = HI_TRUE;
    }

    //set the decode max w&h
    pchan_cfg->s32MaxWidth = UHD_FRAME_WIDTH;
    pchan_cfg->s32MaxHeight = UHD_FRAME_HEIGHT;
    pchan_cfg->s32ExtraFrameStoreNum = g_DispNum;
    pchan_cfg->s32SedOnlyEnable = SEDONLY_ENABLE;

    /* Config decode compress attr */
    pchan->is_compress = g_CompressEnable;
    pchan_cfg->s32VcmpEn = pchan->is_compress;

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_CFG_CHAN, pchan_cfg);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s CFG_CHAN failed\n", __func__);
        goto error2;
    }

    pchan->stream_ops.stream_provider_inst_id = pchan->channel_id;
    pchan->stream_ops.read_stream = decoder_get_stream;
    pchan->stream_ops.release_stream = decoder_release_stream;
    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_SET_STREAM_INTF, &pchan->stream_ops);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s SET_STREAM_INTF failed\n", __func__);
        goto error2;
    }

    pchan->image_ops.image_provider_inst_id = pchan->channel_id;
    pchan->image_ops.read_image = pVfmwFunc->pfnVfmwGetImage;
    pchan->image_ops.release_image = pVfmwFunc->pfnVfmwReleaseImage;

    if (g_LowDelayStatistics == HI_TRUE)
    {
        HI_HANDLE hHandle = 0;

        hHandle = (HI_ID_VDEC << 16) | ((HI_U32)pchan->channel_id);

        ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_START_LOWDLAY_CALC, &hHandle);
        if (HI_SUCCESS != ret)
        {
            OmxPrint(OMX_FATAL, "%s call VDEC_CID_START_LOWDLAY_CALC failed, ret = %d!\n", __func__, ret);
            return HI_FAILURE;
        }
        HI_DRV_LD_Start_Statistics(SCENES_VID_PLAY, hHandle);
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;

error2:
    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_DESTROY_CHAN, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s DESTROY_CHAN failed\n", __func__);
    }

error1:
    omxvdec_release_mem(&pchan->decoder_scd_buf, pchan->eSCDMemAlloc);
    omxvdec_release_mem(&pchan->decoder_vdh_buf, pchan->eVDHMemAlloc);

error0:
    if (HI_TRUE == pchan->is_tvp)
    {
        ret = decoder_exit_trusted();
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_FATAL, "decoder_exit_trusted failed\n");
        }
    }

    return HI_FAILURE;
}

HI_S32 decoder_release_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_U32 i = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_DESTROY_CHAN, HI_NULL);
    if (ret != HI_SUCCESS)
    {
       OmxPrint(OMX_FATAL, "%s destroy vfmw failed\n", __func__);
    }

    if (pchan->is_tvp == HI_TRUE)
    {
        ret = decoder_exit_trusted();
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_FATAL, "decoder_exit_trusted failed\n");
        }
    }

    omxvdec_release_mem(&pchan->decoder_scd_buf, pchan->eSCDMemAlloc);
    omxvdec_release_mem(&pchan->decoder_vdh_buf, pchan->eVDHMemAlloc);

#ifdef VFMW_VPSS_BYPASS_EN
    for (i=0; i < MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);

        /*VP9变分辨率时，会出现一个槽位有pmv但是没有帧存，这种情况也需要释放*/
        if ((pBufSlot->frm_buf.u32StartPhyAddr == 0 || pBufSlot->frm_buf.u32Size == 0)
         && (pBufSlot->pmv_buf.u32StartPhyAddr == 0 || pBufSlot->pmv_buf.u32Size == 0))
        {
            continue;
        }

        if (pBufSlot->pmv_buf.u32StartPhyAddr != 0 && pBufSlot->pmv_buf.u32Size != 0)
        {
            omxvdec_release_mem(&pBufSlot->pmv_buf, pBufSlot->pmv_buf_type);
            pBufSlot->pmv_buf_type = ALLOC_INVALID;
        }

        if (pBufSlot->frm_buf.u32StartPhyAddr != 0 && pBufSlot->frm_buf.u32Size != 0)
        {
            if(!((0 != g_OmxVdec->stRemainFrmList.s32Num) && (channel_IsOccupiedFrm(pBufSlot->frm_buf.u32StartPhyAddr, HI_NULL))))
            {
                omxvdec_release_mem(&pBufSlot->frm_buf, pBufSlot->frm_buf_type);
            }
            /*special frame already record in global list,so clear the channel record here, change by l00228308*/
            pBufSlot->frm_buf_type = ALLOC_INVALID;
            pBufSlot->is_available  = HI_FALSE;
            pBufSlot->is_configured = HI_FALSE;
            /*change end*/
        }
    }
#else
    for (i=0; i < MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);
        if (pBufSlot->pmv_buf.u32StartPhyAddr != 0 && pBufSlot->pmv_buf.u32Size != 0)
        {
            omxvdec_release_mem(&pBufSlot->pmv_buf, pBufSlot->pmv_buf_type);
        }
        if (pBufSlot->frm_buf.u32StartPhyAddr != 0 && pBufSlot->frm_buf.u32Size != 0)
        {
            omxvdec_release_mem(&pBufSlot->frm_buf, pBufSlot->frm_buf_type);
        }
    }
#endif

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return ret;
}

HI_S32 decoder_start_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_START_CHAN, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s start vfmw failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_stop_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_STOP_CHAN, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s stop vfmw failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_reset_inst(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_RESET_CHAN, HI_NULL);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s reset vfmw failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_reset_inst_with_option(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;
    VDEC_CHAN_RESET_OPTION_S  Option;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    Option.s32KeepBS     = 0;
    Option.s32KeepSPSPPS = 1;
    Option.s32KeepFSP    = 1;

    ret = (pVfmwFunc->pfnVfmwControl)(pchan->decoder_id, VDEC_CID_RESET_CHAN_WITH_OPTION, &Option);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_FATAL, "%s reset vfmw with option failed\n", __func__);

        return HI_FAILURE;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 decoder_command_handler(OMXVDEC_CHAN_CTX *pchan, DECODER_CMD_E eCmd, HI_VOID* pArgs)
{
    HI_S32 ret = HI_FAILURE;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    switch(eCmd)
    {
        case DEC_CMD_CLEAR_STREAM:
            ret = decoder_clear_stream(pchan);
            break;

        case DEC_CMD_ALLOC_MEM:
            ret = decoder_alloc_mem(pchan, pArgs);
            break;

        case DEC_CMD_CONFIG_MEM:
            ret = decoder_config_mem(pchan, pArgs);
            break;

        default:
            OmxPrint(OMX_FATAL, "%s unkown command %d\n", __func__, eCmd);
            break;
    }

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return ret;
}


