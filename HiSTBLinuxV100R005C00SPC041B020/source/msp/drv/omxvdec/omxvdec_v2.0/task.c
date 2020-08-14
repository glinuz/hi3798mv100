/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    task.c
 *
 * Purpose: omxvdec task functions
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#include "task.h"
#include "channel.h"
#include "decoder.h"

/*================ EXTERN VALUE ================*/
extern HI_U32   g_DispNum;

#ifdef VFMW_VPSS_BYPASS_EN
extern OMXVDEC_ENTRY  *g_OmxVdec;
#endif
/*============== INTERNAL DEFINE ===============*/
typedef enum
{
    SLOT_FREE = 0,
    SLOT_PMV_ONLY,
}SLOT_TYPE_E;

typedef enum
{
    ALLOC_BUF_INVALID = 0,
    ALLOC_FRM,
    ALLOC_PMV,
}ALLOC_BUF_TYPE_E;



/*=============== INTERNAL MACRO ===============*/
#define RET_NO_MEM   (1)
#define RET_BREAK    (2)
#define RET_RETRY    (3)

#define MAX_RETRY    (10)

#define CHECK_And_SET_DFS_STATE(ExpectState, NewState)        \
do {                                                          \
    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);           \
    if (ExpectState == pchan->dfs.dfs_state)                  \
    {                                                         \
        pchan->dfs.dfs_state = NewState;                      \
    }                                                         \
    else                                                      \
    {                                                         \
        OmxPrint(OMX_ERR, "%s %d another dfs report(dfs_state = %d), return.\n", __func__, __LINE__, pchan->dfs.dfs_state);  \
        spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);  \
        return RET_RETRY;                                            \
    }                                                         \
    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);      \
} while(0)


/*============== INTERNAL FUNCTION =============*/
static inline const HI_PCHAR task_show_state(eTASK_STATE state)
{
    switch (state)
    {
       case TASK_STATE_INVALID:
            return "invalid";
            break;

       case TASK_STATE_SLEEPING:
            return "sleeping";
            break;

       case TASK_STATE_ONCALL:
            return "oncall";
            break;

       case TASK_STATE_RUNNING:
            return "running";
            break;

       default:
            return "unkown";
            break;
    }
}

OMXVDEC_CHAN_CTX* task_find_inst_need_wake_up(OMXVDEC_ENTRY *omxvdec)
{
    HI_S8 find = 0;
    unsigned long flags = 0;
    unsigned long flags_2 = 0;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;

    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_FATAL, "%s: vdec = NULL, invalid\n", __func__);
        return HI_NULL;
    }

    spin_lock_irqsave(&omxvdec->channel_lock, flags);
    if (!list_empty(&omxvdec->chan_list))
    {
        list_for_each_entry(pchan, &omxvdec->chan_list, chan_list)
        {
            spin_lock_irqsave(&pchan->dfs.dfs_lock, flags_2);

            if(DFS_WAIT_RESPONSE_0 == pchan->dfs.dfs_state
            || DFS_WAIT_RESPONSE_1 == pchan->dfs.dfs_state)
            {
                find = 1;
                pchan->dfs.dfs_state = DFS_WAIT_ALLOC;
                spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags_2);

                break;
            }

            spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags_2);
        }
    }
    spin_unlock_irqrestore(&omxvdec->channel_lock, flags);

    if (0 == find)
    {
        pchan = HI_NULL;
    }

    return pchan;
}


static DSF_SINGLE_BUF_S *task_get_free_dfs_slot(OMXVDEC_CHAN_CTX *pchan, SLOT_TYPE_E type)
{
    HI_U32 i = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    for (i=0; i < MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);
        if (pBufSlot->is_available == HI_TRUE || pBufSlot->is_configured == HI_TRUE)
        {
            continue;
        }

        if (SLOT_PMV_ONLY == type
        && (pBufSlot->pmv_buf.u32StartPhyAddr == 0 || pBufSlot->pmv_buf.u32Size == 0))
        {
            continue;
        }

        break;
    }

    if (i >= MAX_DFS_BUF_NUM)
    {
        return HI_NULL;
    }
    else
    {
        return pBufSlot;
    }
}

static HI_S32 task_alloc_buf_for_slot(OMXVDEC_CHAN_CTX *pchan, DSF_SINGLE_BUF_S *pBufSlot, FRAME_BUF_S *pstFrameBuf, ALLOC_BUF_TYPE_E type)
{
    HI_S32 ret = HI_FAILURE;
    HI_BOOL is_sec = HI_FALSE;
    unsigned long flags;

    HI_CHAR buf_name[20] = {0};
    HI_U32 NeededFrmSize;
    HI_U32 NeededPmvSize;
    HI_U32 NeededAllocSize = 0;
    MMZ_BUFFER_S  TmpMMZBuf;

    OMXVDEC_ASSERT_RETURN(pBufSlot != NULL, "param is invalid");
    OMXVDEC_ASSERT_RETURN(pstFrameBuf != NULL, "param is invalid");

    is_sec = pchan->is_tvp;

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    if (type == ALLOC_FRM)
    {
        NeededFrmSize = pchan->dfs.need_frm_size;
        NeededAllocSize = NeededFrmSize;
    }
    else
    {
        NeededPmvSize = pchan->dfs.need_pmv_size;

        NeededAllocSize = NeededPmvSize;
    }

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);


    if (type == ALLOC_FRM)
    {
        snprintf(buf_name, sizeof(buf_name), "DFS_FRM_%d", pchan->dfs.alloc_frm_num);
    }
    else
    {
        snprintf(buf_name, sizeof(buf_name), "DFS_PMV_%d", pchan->dfs.alloc_pmv_num);
    }


    memset(&TmpMMZBuf, 0, sizeof(MMZ_BUFFER_S));

    ret = HI_DRV_OMXVDEC_Alloc(buf_name, HI_NULL, NeededAllocSize, 0, &TmpMMZBuf, is_sec);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ERR, "%s alloc mem for %s failed!\n", __func__, buf_name);
        return RET_NO_MEM;
    }

    if (type == ALLOC_FRM)
    {

        pstFrameBuf->enBufNodeStatus = DRV_VDEC_BUFFER_STATUS_SYSALLOC;
        pstFrameBuf->u32StartPhyAddr = TmpMMZBuf.u32StartPhyAddr;
        pstFrameBuf->u64StartVirAddr = (HI_U64)(HI_SIZE_T)TmpMMZBuf.pu8StartVirAddr;
        pstFrameBuf->u32Size         = TmpMMZBuf.u32Size;

        memcpy(&pBufSlot->frm_buf, &TmpMMZBuf, sizeof(MMZ_BUFFER_S));

        if (is_sec)
        {
            pBufSlot->frm_buf_type = ALLOC_BY_SEC;
        }
        else
        {
            pBufSlot->frm_buf_type = ALLOC_BY_MMZ_UNMAP;
        }
    }
    else
    {
        pstFrameBuf->enBufPmvNodeStatus = DRV_VDEC_BUFFER_STATUS_SYSALLOC;
        pstFrameBuf->u32PmvStartPhyAddr = TmpMMZBuf.u32StartPhyAddr;
        pstFrameBuf->u64PmvStartVirAddr = (HI_U64)(HI_SIZE_T)TmpMMZBuf.pu8StartVirAddr;
        pstFrameBuf->u32PmvSize         = TmpMMZBuf.u32Size;

        memcpy(&pBufSlot->pmv_buf, &TmpMMZBuf, sizeof(MMZ_BUFFER_S));

        if (is_sec)
        {
            pBufSlot->pmv_buf_type = ALLOC_BY_SEC;
        }
        else
        {
            pBufSlot->pmv_buf_type = ALLOC_BY_MMZ_UNMAP;
        }
    }

    return HI_SUCCESS;
}


static HI_S32 task_handle_situation_1(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 i = 0;
    HI_U32 lack_num = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;
    FRAME_BUF_S   stFrameBuf;

    OmxPrint(OMX_TRACE, "%s enter\n", __func__);

    lack_num = (pchan->dfs.alloc_pmv_num > pchan->dfs.config_pmv_num)? pchan->dfs.alloc_pmv_num - pchan->dfs.config_pmv_num: 0;

    for (i=0; i<lack_num; i++)
    {
        /* Get dfs slot with pmv buf only */
        pBufSlot = task_get_free_dfs_slot(pchan, SLOT_PMV_ONLY);
        if (HI_NULL == pBufSlot)
        {
            OmxPrint(OMX_ERR, "%s task_get_free_dfs_slot failed!\n", __func__);
            return HI_FAILURE;
        }

        memset(&stFrameBuf, 0, sizeof(FRAME_BUF_S));
        stFrameBuf.enBufNodeStatus    = DRV_VDEC_BUFFER_STATUS_BUTT;
        stFrameBuf.enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        stFrameBuf.enBufPmvNodeStatus = (ALLOC_BY_PRE == pBufSlot->pmv_buf_type)? DRV_VDEC_BUFFER_STATUS_PREALLOC: DRV_VDEC_BUFFER_STATUS_SYSALLOC;

        stFrameBuf.u32PmvStartPhyAddr = pBufSlot->pmv_buf.u32StartPhyAddr;
        stFrameBuf.u64PmvStartVirAddr = (HI_U64)(HI_SIZE_T)pBufSlot->pmv_buf.pu8StartVirAddr;
        stFrameBuf.u32PmvSize         = pBufSlot->pmv_buf.u32Size;

        ret = task_alloc_buf_for_slot(pchan, pBufSlot, &stFrameBuf, ALLOC_FRM);
        if (ret != HI_SUCCESS)
        {
            return ret;
        }

        pBufSlot->is_available     = HI_TRUE;
        pBufSlot->is_configured    = HI_FALSE;

        pchan->dfs.alloc_frm_num++;

        /* Config this buffer to decoder */
        ret = decoder_command_handler(pchan, DEC_CMD_CONFIG_MEM, &stFrameBuf);
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s config mem for decoder failed!\n", __func__);
            return HI_FAILURE;
        }

        pchan->dfs.config_frm_num++;
        pchan->dfs.config_pmv_num++;
        pBufSlot->is_available  = HI_FALSE;
        pBufSlot->is_configured = HI_TRUE;
    }

    OmxPrint(OMX_TRACE, "%s exit ok\n", __func__);

    return HI_SUCCESS;
}

static HI_U32 task_get_configed_slot_num(OMXVDEC_CHAN_CTX *pchan)
{
    HI_U32 i;
    HI_U32 u32CfgCnt = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    for (i=0; i<MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);

        //查找该分辨率已经配置的帧存
        if(pBufSlot->frm_buf.u32Size == pchan->dfs.image_size_only &&
           pBufSlot->is_configured == HI_TRUE &&
           pBufSlot->is_wait_release == HI_FALSE)
        {
            u32CfgCnt ++;
        }
    }

    return u32CfgCnt;
}

static HI_VOID task_try_release_unused_slot(OMXVDEC_CHAN_CTX *pchan)
{
    HI_U32 i;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    for(i=0; i<MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);

        if(pBufSlot->is_wait_release == HI_FALSE)
        {
            continue;
        }

        omxvdec_release_mem(&pBufSlot->frm_buf, pBufSlot->frm_buf_type);

        pBufSlot->frm_buf_type = ALLOC_INVALID;

        if(pBufSlot->pmv_buf_type == ALLOC_INVALID)
        {
            pBufSlot->is_available = HI_FALSE;
            pBufSlot->is_configured = HI_FALSE;
        }

        pBufSlot->is_wait_release = HI_FALSE;

        if (pchan->dfs.alloc_frm_num > 0)
        {
            pchan->dfs.alloc_frm_num--;
        }
    }

    return;
}

static HI_S32 task_handle_situation_2(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    HI_U32 i = 0, lack_num = 0;
    HI_BOOL bPmvAlloc = HI_FALSE;
    HI_U32 NeededFrmNum, NeededFrmSize;
    HI_U32 NeededPmvNum;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;
    FRAME_BUF_S stFrameBuf;

    OmxPrint(OMX_TRACE, "%s enter\n", __func__);

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    if (pchan->dfs.alloc_frame_only == 1 && pchan->dfs.image_num_only > 0)
    {
        pchan->dfs.config_frm_num = task_get_configed_slot_num(pchan);
        pchan->dfs.need_frm_num = pchan->dfs.image_num_only;
        pchan->dfs.need_frm_size = pchan->dfs.image_size_only;

        pchan->dfs.image_num_only = 0;
    }

    NeededFrmNum  = pchan->dfs.need_frm_num;
    NeededFrmSize = pchan->dfs.need_frm_size;
    NeededPmvNum  = pchan->dfs.need_pmv_num;

    lack_num = (NeededFrmNum > pchan->dfs.config_frm_num)? (NeededFrmNum - pchan->dfs.config_frm_num) : 0;

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    if (pchan->dfs.alloc_frame_only == 1)
    {
        //VP9 变参考帧
        task_try_release_unused_slot(pchan);
    }

    for (i=0; i<lack_num; i++)
    {
        if(pchan->dfs.alloc_frame_only == 1 && pchan->dfs.image_size_only != NeededFrmSize)
        {
            //分配旧帧存过程中分辨率变化应该立即终止
            OmxPrint(OMX_ERR, "%s:%d size change when alloc, sizeonly:%d ned size:%d\n", __func__, __LINE__, \
                              pchan->dfs.image_size_only, NeededFrmSize);

            return HI_FAILURE;
        }

        memset(&stFrameBuf, 0, sizeof(FRAME_BUF_S));
        stFrameBuf.enBufNodeStatus            = DRV_VDEC_BUFFER_STATUS_BUTT;
        stFrameBuf.enBufHdrMetadataNodeStatus = DRV_VDEC_BUFFER_STATUS_BUTT;
        stFrameBuf.enBufPmvNodeStatus         = DRV_VDEC_BUFFER_STATUS_BUTT;

        /* Get free dfs slot */
        pBufSlot = task_get_free_dfs_slot(pchan, SLOT_FREE);
        if (HI_NULL == pBufSlot)
        {
            OmxPrint(OMX_ERR, "%s task_get_free_dfs_slot failed!\n", __func__);
            return HI_FAILURE;
        }

        bPmvAlloc = HI_FALSE;

        if (pchan->dfs.alloc_pmv_num < NeededPmvNum)
        {
            ret = task_alloc_buf_for_slot(pchan, pBufSlot, &stFrameBuf, ALLOC_PMV);
            if (ret != HI_SUCCESS)
            {
                OmxPrint(OMX_ERR, "%s task_alloc_buf_for_slot failed!\n", __func__);

                return ret;
            }

            pchan->dfs.alloc_pmv_num++;
            bPmvAlloc = HI_TRUE;
        }

        if (pchan->dfs.alloc_frm_num < NeededFrmNum || pchan->dfs.alloc_frame_only == 1)
        {
            ret = task_alloc_buf_for_slot(pchan, pBufSlot, &stFrameBuf, ALLOC_FRM);
            if (ret != HI_SUCCESS)
            {
                OmxPrint(OMX_ERR, "%s:%d task_alloc_buf_for_slot failed!\n", __func__, __LINE__);

                return ret;
            }

            pchan->dfs.alloc_frm_num++;
            pBufSlot->is_available     = HI_TRUE;
            pBufSlot->is_configured    = HI_FALSE;
        }

        /* Config this buffer to decoder */
        if (HI_TRUE == pBufSlot->is_available && HI_FALSE == pBufSlot->is_configured)
        {
            ret = decoder_command_handler(pchan, DEC_CMD_CONFIG_MEM, &stFrameBuf);
            if (ret != HI_SUCCESS)
            {
                OmxPrint(OMX_ERR, "%s config mem for decoder failed!\n", __func__);
                return HI_FAILURE;
            }

            if (HI_TRUE == bPmvAlloc)
            {
                pchan->dfs.config_pmv_num++;
            }
            pchan->dfs.config_frm_num++;
            pBufSlot->is_available  = HI_FALSE;
            pBufSlot->is_configured = HI_TRUE;
        }
        else
        {
            OmxPrint(OMX_ERR, "%s shoud not be here! frm needed = %d, alloc = %d, config = %d\n", __func__, \
                          NeededFrmNum, pchan->dfs.alloc_frm_num, pchan->dfs.config_frm_num);

            return HI_FAILURE;
        }
    }

    OmxPrint(OMX_TRACE, "%s exit ok\n", __func__);

    return HI_SUCCESS;
}

static HI_S32 task_release_channel_mem(OMXVDEC_CHAN_CTX *pchan)
{
    HI_U32 i = 0;
    DSF_SINGLE_BUF_S *pBufSlot = HI_NULL;

    for (i=0; i < MAX_DFS_BUF_NUM; i++)
    {
        pBufSlot = &(pchan->dfs.single_buf[i]);

        if (pBufSlot->pmv_buf.u32StartPhyAddr != 0 && pBufSlot->pmv_buf.u32Size != 0)
        {
            omxvdec_release_mem(&pBufSlot->pmv_buf, pBufSlot->pmv_buf_type);
            pBufSlot->pmv_buf_type = ALLOC_INVALID;
        }

        if (pBufSlot->frm_buf.u32StartPhyAddr != 0 && pBufSlot->frm_buf.u32Size != 0)
        {
        #ifdef VFMW_VPSS_BYPASS_EN
            if(!((0 != g_OmxVdec->stRemainFrmList.s32Num) && (channel_IsOccupiedFrm(pBufSlot->frm_buf.u32StartPhyAddr, HI_NULL))))
        #else
            if(!((HI_TRUE == pchan->dfs.occupied_frm_exist) && (pchan->dfs.occupied_frm_phyaddr == pBufSlot->frm_buf.u32StartPhyAddr)))
        #endif
            {
                omxvdec_release_mem(&pBufSlot->frm_buf, pBufSlot->frm_buf_type);
            }

            /*special frame already record in global list,so clear the channel record here, change by l00228308*/
            pBufSlot->frm_buf_type = ALLOC_INVALID;
            pBufSlot->is_available  = HI_FALSE;
            pBufSlot->is_configured = HI_FALSE;
            pBufSlot->is_wait_release = HI_FALSE;
            /*change end*/

        }
    }

    pchan->dfs.alloc_frm_num  = 0;
    pchan->dfs.alloc_pmv_num  = 0;
    pchan->dfs.config_frm_num = 0;
    pchan->dfs.config_pmv_num = 0;
    pchan->dfs.retry_count    = 0;

    return HI_SUCCESS;
}

static HI_S32 task_alloc_channel_mem_single(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret;
    HI_U32 u32DoneTime = 0;
    unsigned long flags = 0;

    /* Release unused mem & record */
    if (DFS_WAIT_ALLOC == pchan->dfs.dfs_state && pchan->dfs.alloc_frame_only == HI_FALSE)
    {
        ret = task_release_channel_mem(pchan);
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s ret=%d,task_release_channel_mem failed!\n", __func__, ret);
            return HI_FAILURE;
        }
    }

    /* Alloc & Config mem */
    /*
       stituation 1: already alloc pmv buf, but failed on alloc frm buf last time,
                     so on this situation, just need to retry alloc frm buf, and config to decoder.
    */
    if (pchan->dfs.alloc_pmv_num > pchan->dfs.config_pmv_num)
    {
        ret = task_handle_situation_1(pchan);
        if (ret != HI_SUCCESS)
        {
            OmxPrint(OMX_ERR, "%s task_handle_situation_1 failed\n", __func__);

            return ret;
        }
    }

    /*
       stituation 2: ordinary situation, should alloc both pmv & frm buf,
                     or alloc frm buf only in case of no more pmv buf needed
    */
    ret = task_handle_situation_2(pchan);
    if (ret != HI_SUCCESS)
    {
        OmxPrint(OMX_ERR, "%s task_handle_situation_2 failed\n", __func__);
        return ret;
    }

    spin_lock_irqsave(&pchan->dfs.dfs_lock, flags);

    if (pchan->dfs.dfs_state == DFS_WAIT_ALLOC)
    {
        pchan->dfs.dfs_state = DFS_DONE;
    }

    spin_unlock_irqrestore(&pchan->dfs.dfs_lock, flags);

    HI_DRV_SYS_GetTimeStampMs(&u32DoneTime);
    pchan->dfs.delay_time = u32DoneTime - pchan->dfs.delay_time;

    return HI_SUCCESS;
}


static HI_S32 task_memory_manager(OMXVDEC_CHAN_CTX *pchan)
{
    HI_S32 ret = HI_SUCCESS;

    VDEC_DOWN_INTERRUPTIBLE(&pchan->chan_mutex);

    if (pchan->state != CHAN_STATE_INVALID)
    {
        ret = task_alloc_channel_mem_single(pchan);
    }

    VDEC_UP_INTERRUPTIBLE(&pchan->chan_mutex);

    return ret;
}

static HI_VOID task_poll_release_remain_frame(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 num = 0;
    HI_U32 u32Index = 0;
    HI_S32 ret = HI_FAILURE;
    unsigned long flags;
    HI_U32 u32StartPhyAddr = 0;

    num = g_OmxVdec->stRemainFrmList.s32Num;

    for (i = 0; i<num && num>0; i++)
    {
        spin_lock_irqsave(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);
        ret = channel_find_nodeId_can_release(&g_OmxVdec->stRemainFrmList, &u32Index);
        spin_unlock_irqrestore(&g_OmxVdec->stRemainFrmList.bypass_lock,flags);

        if (ret == HI_SUCCESS)
        {
            /*find the node which can be release now! release the frameBuffer and del the Note */
            u32StartPhyAddr = g_OmxVdec->stRemainFrmList.stSpecialFrmRec[u32Index].frmBufRec.u32StartPhyAddr;
            OmxPrint(OMX_INFO, "%s,%d,find index(%d), Phyaddr = 0x%x can be release!\n", __func__,__LINE__,u32Index,u32StartPhyAddr);

            ret = decoder_global_release_frame_inter(u32Index);
            if (ret != HI_SUCCESS)
            {
                OmxPrint(OMX_FATAL,"release the special frameBuffer and del Note failed!\n");
            }
            else
            {
                OmxPrint(OMX_FATAL,"release the special frameBuffer success! Phyaddr = 0x%x\n", u32StartPhyAddr);
            }
        }
    }

    return;
}


HI_S32 task_func_entry(HI_VOID* param)
{
    HI_S32 ret = HI_FAILURE;
    OMXVDEC_CHAN_CTX *pchan = HI_NULL;
    OMXVDEC_ENTRY *omxvdec = (OMXVDEC_ENTRY*)param;
    OMXVDEC_TASK *task = &(omxvdec->task);

    OmxPrint(OMX_TRACE, "omx task start!\n");

    task->task_state = TASK_STATE_INVALID;

    while(!kthread_should_stop())
    {
        ret = wait_event_timeout(task->task_wait, (TASK_STATE_ONCALL == task->task_state), msecs_to_jiffies(50));
        task->task_state = TASK_STATE_RUNNING;

        if (0 != g_OmxVdec->stRemainFrmList.s32Num)
        {
            task_poll_release_remain_frame();
        }

        // if is not wake up by event call
        if (ret <= 0)
        {
            continue;
        }

        do {
            pchan = HI_NULL;
            pchan = task_find_inst_need_wake_up(omxvdec);

            if (HI_NULL == pchan)
            {
                break;
            }

            ret = task_memory_manager(pchan);
            if (HI_FAILURE == ret)
            {
                OmxPrint(OMX_FATAL, "Mem manage for chan %d failed\n", pchan->channel_id);
                //message_queue(pchan->msg_queue, VDEC_ERR_DYNAMIC_ALLOC_FAIL, HI_FAILURE, NULL);
                //break;
            }

            msleep(10);
        }while(1);
    }

    task->task_state = TASK_STATE_INVALID;

    OmxPrint(OMX_TRACE, "omx task exit!\n");

    return HI_SUCCESS;
}


/*============== EXPORT FUNCTION =============*/
HI_S32 task_create_thread(OMXVDEC_ENTRY* omxvdec)
{
    OMXVDEC_TASK *task = HI_NULL;

    OmxPrint(OMX_TRACE, "%s enter!\n", __func__);

    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_FATAL, "%s ppTask = NULL!\n", __func__);
        return HI_FAILURE;
    }
    task = &(omxvdec->task);

    init_waitqueue_head(&task->task_wait);
    task->task_thread = kthread_create(task_func_entry, (HI_VOID *)omxvdec, "OmxVdecTask");

    if (HI_NULL == task->task_thread)
    {
        OmxPrint(OMX_FATAL, "%s create task failed!\n", __func__);
        return HI_FAILURE;
    }
    else if (IS_ERR(task->task_thread))
    {
        OmxPrint(OMX_FATAL, "%s create task failed, error no = %ld!\n", __func__, PTR_ERR(task->task_thread));
        task->task_thread = HI_NULL;
        return HI_FAILURE;
    }

    wake_up_process(task->task_thread);

    OmxPrint(OMX_TRACE, "%s exit normally!\n", __func__);

    return HI_SUCCESS;
}

HI_S32 task_destroy_thread(OMXVDEC_ENTRY* omxvdec)
{
    OMXVDEC_TASK *task = HI_NULL;

    if (HI_NULL == omxvdec)
    {
        OmxPrint(OMX_FATAL, "%s omxvdec = NULL!\n", __func__);
        return HI_FAILURE;
    }
    task = &(omxvdec->task);

    if(task->task_thread != HI_NULL)
    {
        kthread_stop(task->task_thread);

        task->task_thread = HI_NULL;
    }

    return HI_SUCCESS;
}

HI_VOID task_proc_entry(struct seq_file *p, OMXVDEC_TASK *ptask)
{
    PROC_PRINT(p, "%-25s :%s\n", "TaskState",         task_show_state(ptask->task_state));
}


