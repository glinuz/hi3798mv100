/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : stat.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/12/16
  Last Modified :
  Description   : interrupt and thread timing measurement tool
  Function List :
  History       :
  1.Date        : 2008/12/16
    Author      : z42136
    Modification: Created file

******************************************************************************/
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/ctype.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "hi_osal.h"
#include "hi_drv_dev.h"
#include "hi_drv_module.h"
#include "hi_drv_proc.h"
#include "hi_drv_stat.h"
#include "drv_stat_ioctl.h"


typedef char THREAD_NAME[64];

/* for userspace stat */
typedef struct
{
    HI_BOOL bUsed;
    HI_U32 min_time;    // us
    HI_U32 max_time;    // us
    HI_U32 avg_time;    // us

    THREAD_NAME name;
}STAT_USERSPACE_S;


typedef struct
{
    struct timeval tv;
    HI_U32 time;
}STAT_ISR_TIME_S;

typedef struct
{
    HI_U32 EventMs;
    HI_U32 Value1;
    HI_U32 Value2;
}STAT_EVENT_TIME_S;


#define STAT_MAX_THREAD_USERSPACE   32
/* make sure STAT_USERSPACE_TOTAL_SIZE < 4096 */
#define STAT_USERSPACE_TOTAL_SIZE (sizeof(STAT_USERSPACE_S)*STAT_MAX_THREAD_USERSPACE)


static STAT_ISR_TIME_S g_IsrTime[STAT_ISR_BUTT] __attribute__((unused)) ;
static HI_BOOL g_stat_isr_enable  __attribute__((unused)) = HI_FALSE ;
static HI_BOOL g_stat_thread_enable  __attribute__((unused)) = HI_TRUE;

static STAT_EVENT_TIME_S g_EventTime[STAT_EVENT_BUTT]  __attribute__((unused));


static UMAP_DEVICE_S    g_srtuStatDev;

static dma_addr_t g_stat_thread_phyaddr_base = 0;

/* g_stat_userspace point to STAT_MAX_THREAD_USERSPACE * STAT_USERSPACE_S */
static STAT_USERSPACE_S * g_stat_thread_kvirt_base = NULL;

static HI_U64 DIV64(HI_U64 Dividend, HI_U64 Divisor)
{
    do_div(Dividend, Divisor);

    return Dividend;
}

HI_S32 CMPI_STAT_Ioctl(struct inode *inode, struct file *file, HI_U32 cmd, HI_VOID * arg)
{
    HI_U32 tmp;
    int i;

    switch (cmd)
    {
        case UMAPC_CMPI_STAT_REGISTER:
        {
            HI_U32 * p_stat_thread_addr = (HI_U32 * )arg;

            for (i = 0; i < STAT_MAX_THREAD_USERSPACE; i++)
            {
                if(g_stat_thread_kvirt_base[i].bUsed == HI_FALSE)
                {
                    tmp = (HI_U32)g_stat_thread_phyaddr_base + i*sizeof(STAT_USERSPACE_S);
                    HI_WARN_STAT("get free slab(%d)\n", i);
                    HI_WARN_STAT("g_stat_thread_phyaddr_base = %x, tmp = %x\n", (HI_U32)g_stat_thread_phyaddr_base, tmp);

                    *p_stat_thread_addr = tmp;
                    g_stat_thread_kvirt_base[i].bUsed = HI_TRUE;

                    return 0;
                }
            }

            HI_ERR_STAT("no available stat resource\n");
            /* no available stat resource */
            *p_stat_thread_addr = 0;
            return -1;
        }

        case UMAPC_CMPI_STAT_RESETALL:
        {
            for (i = 0; i < STAT_MAX_THREAD_USERSPACE; i++)
            {
                if(g_stat_thread_kvirt_base[i].bUsed == HI_TRUE)
                {
                    g_stat_thread_kvirt_base[i].avg_time = 0;
                    g_stat_thread_kvirt_base[i].min_time = 0;
                    g_stat_thread_kvirt_base[i].max_time = 0;
                }
            }

            break;
        }

        case UMAPC_CMPI_STAT_EVENT:
        {
            STAT_EVENT_S   *pStatEvent;

            pStatEvent = (STAT_EVENT_S *)arg;

            HI_DRV_STAT_Event(pStatEvent->enEvent, pStatEvent->Value);

            break;
        }

        case UMAPC_CMPI_STAT_GETTICK:
        {
            HI_U32 *pTick = (HI_U32*)arg;
            *pTick = HI_DRV_STAT_GetTick();
            break;
        }

        case UMAPC_CMPI_STAT_LD_EVENT:
        {
            HI_DRV_LD_Notify_Event((HI_LD_Event_S*)arg);
            break;
        }

        default:
            return -1;
    }

    return 0;
}

static long DRV_STAT_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_STAT_Ioctl);
}

#ifdef CONFIG_COMPAT
static long DRV_STAT_Compat_Ioctl(struct file *file, HI_U32 cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(file->f_dentry->d_inode, file, cmd, arg, CMPI_STAT_Ioctl);
}
#endif

static HI_S32 DRV_STAT_Release(struct inode * inode, struct file * file)
{
    return 0;
}

static HI_S32 DRV_STAT_Open(struct inode * inode, struct file * file)
{
    return 0;
}

static struct file_operations DRV_stat_Fops=
{
    .owner      = THIS_MODULE,
    .open       = DRV_STAT_Open,
    .unlocked_ioctl      = DRV_STAT_Ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = DRV_STAT_Compat_Ioctl,
#endif
    .release    = DRV_STAT_Release,
};
#if (1 == HI_PROC_SUPPORT)
static HI_S32 stat_proc_read(struct seq_file *s, HI_VOID *pArg)
{
    int i;

    PROC_PRINT(s, "----------- host isr stat -----------\n");
    if(g_stat_isr_enable == HI_FALSE)
    {
        PROC_PRINT(s, "isr stat is disabled!\n");
    }
    else
    {
        PROC_PRINT(s, "audio isr time = %u us\n", g_IsrTime[STAT_ISR_AUDIO].time);
        PROC_PRINT(s, "video isr time = %u us\n", g_IsrTime[STAT_ISR_VIDEO].time);
        PROC_PRINT(s, "demux isr time = %u us\n", g_IsrTime[STAT_ISR_DEMUX].time);
        PROC_PRINT(s, "sync isr time  = %u us\n", g_IsrTime[STAT_ISR_SYNC].time);
        PROC_PRINT(s, "vo isr time    = %u us\n", g_IsrTime[STAT_ISR_VO].time);
        PROC_PRINT(s, "tde isr time   = %u us\n", g_IsrTime[STAT_ISR_TDE].time);
    }

    PROC_PRINT(s, "----------- host thread stat ----------\n");
    if(g_stat_thread_enable == HI_FALSE)
    {
        PROC_PRINT(s, "thread stat is disabled!\n");
    }
    else
    {
        for(i=0; i<STAT_MAX_THREAD_USERSPACE; i++)
        {
            if(g_stat_thread_kvirt_base[i].bUsed == HI_TRUE)
            {
                PROC_PRINT(s, "thread(%s) min=%08dus, avg=%08dus, max=%08d\n",
                                g_stat_thread_kvirt_base[i].name,
                                g_stat_thread_kvirt_base[i].min_time,
                                g_stat_thread_kvirt_base[i].avg_time,
                                g_stat_thread_kvirt_base[i].max_time);
            }
        }
    }

    PROC_PRINT(s, "----------- host event stat ----------\n");

    PROC_PRINT(s, "KEYIN          = %-10u (keyvalue 0x%x)\n", g_EventTime[STAT_EVENT_KEYIN].EventMs, g_EventTime[STAT_EVENT_KEYIN].Value1);
    PROC_PRINT(s, "KEYOUT         = %-10u (keyvalue 0x%x)\n", g_EventTime[STAT_EVENT_KEYOUT].EventMs, g_EventTime[STAT_EVENT_KEYOUT].Value1);
    PROC_PRINT(s, "ASTOP          = %-10u\n", g_EventTime[STAT_EVENT_ASTOP].EventMs);
    PROC_PRINT(s, "VSTOP          = %-10u\n", g_EventTime[STAT_EVENT_VSTOP].EventMs);
    PROC_PRINT(s, "CONNECT        = %-10u\n", g_EventTime[STAT_EVENT_CONNECT].EventMs);
    PROC_PRINT(s, "LOCKED         = %-10u\n", g_EventTime[STAT_EVENT_LOCKED].EventMs);
    PROC_PRINT(s, "ASTART         = %-10u\n", g_EventTime[STAT_EVENT_ASTART].EventMs);
    PROC_PRINT(s, "VSTART         = %-10u\n", g_EventTime[STAT_EVENT_VSTART].EventMs);
    PROC_PRINT(s, "CWSET          = %-10u\n", g_EventTime[STAT_EVENT_CWSET].EventMs);
    PROC_PRINT(s, "STREAMIN       = %-10u\n", g_EventTime[STAT_EVENT_STREAMIN].EventMs);
    PROC_PRINT(s, "ISTREAMGET     = %-10u (size %d)\n", g_EventTime[STAT_EVENT_ISTREAMGET].EventMs,g_EventTime[STAT_EVENT_ISTREAMGET].Value1);
    PROC_PRINT(s, "FRAMEDECED     = %-10u\n", g_EventTime[STAT_EVENT_IFRAMEOUT].EventMs);

    PROC_PRINT(s, "VPSSGET        = %-10u\n", g_EventTime[STAT_EVENT_VPSSGETFRM].EventMs);
    PROC_PRINT(s, "VPSSOUT        = %-10u\n", g_EventTime[STAT_EVENT_VPSSOUTFRM].EventMs);
    PROC_PRINT(s, "AVPLAYGET      = %-10u\n", g_EventTime[STAT_EVENT_AVPLAYGETFRM].EventMs);

    PROC_PRINT(s, "PRESYNC        = %-10u\n", g_EventTime[STAT_EVENT_PRESYNC].EventMs);
    PROC_PRINT(s, "BUFREADY       = %-10u (type %d)\n", g_EventTime[STAT_EVENT_BUFREADY].EventMs,g_EventTime[STAT_EVENT_BUFREADY].Value1);
    PROC_PRINT(s, "FRAMESYNCOK    = %-10u\n", g_EventTime[STAT_EVENT_FRAMESYNCOK].EventMs);

    PROC_PRINT(s, "VOGET          = %-10u\n", g_EventTime[STAT_EVENT_VOGETFRM].EventMs);

    PROC_PRINT(s, "IFRAMEINTER    = %-10u\n", g_EventTime[STAT_EVENT_IFRAMEINTER].Value1);
    PROC_PRINT(s, "TOTAL          = %-10u\n", g_EventTime[STAT_EVENT_VOGETFRM].EventMs - g_EventTime[STAT_EVENT_KEYIN].EventMs);

    return 0;
}
#endif
#if 0
static HI_S32 stat_proc_write(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
    return 0;
}
#endif

/* ------------- low delay statistics ------------------- */

struct low_delay_evt_ext
{
    struct list_head list;
    HI_LD_Event_S  evt;
};

struct low_delay_evt_queue
{
    HI_S32                      head, tail;
    HI_S32                      active;
    spinlock_t                  lock;
    struct low_delay_evt_ext    queue[MAX_EVENT_QUEUE_SIZE];
};

static struct low_delay_evt_head
{
    enum {
        ON = 0,
        OFF = 1,
    } state;
    struct mutex                mutex;
    HI_LD_Scenes_E              scenes_id;
    HI_HANDLE                   filter_handle;
    struct low_delay_evt_queue  evt_queue[MAX_EVENT_TYPE_NR];
} g_active_evt_head = {
    .state          = OFF,
    .mutex          = __MUTEX_INITIALIZER(g_active_evt_head.mutex),
    .evt_queue      = {
        [0 ... (MAX_EVENT_TYPE_NR -1)] = {
           .lock   =  __SPIN_LOCK_UNLOCKED(evt_queue.lock),
        }
    },
};

#define traverse_scenes_each_event(index, evt_id, scenes_id) \
    for (index = 0, evt_id = g_scenes_desc[scenes_id][index]; \
        index < MAX_EVENT_TYPE_NR && scenes_id < SCENES_LD_BUTT && evt_id != EVENT_LD_BUTT ;\
        index++, evt_id = g_scenes_desc[scenes_id][index < MAX_EVENT_TYPE_NR ? index : 0])

#define traverse_each_event_queue(index, evt_queue) \
    for(index = 0, evt_queue = &(g_active_evt_head.evt_queue[index]);\
        index < MAX_EVENT_TYPE_NR; \
        index ++, evt_queue = &(g_active_evt_head.evt_queue[index]))

#define traverse_queue_valid_entry(index, evt_ext, evt_queue) \
    for(index = evt_queue->head, evt_ext = &(evt_queue->queue[index]);\
        evt_queue->tail != index % MAX_EVENT_QUEUE_SIZE; \
        index = (index + 1) % MAX_EVENT_QUEUE_SIZE, evt_ext = &(evt_queue->queue[index]))
/*
 * reset evt statistics state and queue
 */
static HI_VOID reset_evt_statistics_nolock(HI_VOID)
{
    HI_U32 index;
    unsigned long flags;
    struct low_delay_evt_queue * evt_queue;

    g_active_evt_head.state = OFF;

    /* reset ring queue */
    traverse_each_event_queue(index, evt_queue)
    {
        spin_lock_irqsave(&(evt_queue->lock), flags);
        evt_queue->active = 0;
        evt_queue->head = 0;
        evt_queue->tail   = 0;
        spin_unlock_irqrestore(&(evt_queue->lock), flags);
    }
}

/*
 * start new scenes statistics, reset first.
 */
HI_S32 low_delay_start_statistics(HI_LD_Scenes_E scenes_id, HI_HANDLE filter)
{
    HI_U32 index;
    HI_LD_Event_ID_E evt_id;
    unsigned long flags;

    if (!(scenes_id < SCENES_LD_BUTT))
    {
        HI_ERR_STAT("invalid scenes parameter.\n");
        return HI_FAILURE;
    }

    mutex_lock(&(g_active_evt_head.mutex));

    /* disable and reset current statistics */
    reset_evt_statistics_nolock();

    /* save new scenes_id and filter */
    g_active_evt_head.scenes_id     = scenes_id;
    g_active_evt_head.filter_handle = filter;

    /* set evt queue active tag according to scenes */
    traverse_scenes_each_event(index, evt_id, scenes_id)
    {
        struct low_delay_evt_queue * evt_queue = &(g_active_evt_head.evt_queue[evt_id]);

        spin_lock_irqsave(&(evt_queue->lock), flags);
        evt_queue->active = 1;
        spin_unlock_irqrestore(&(evt_queue->lock), flags);
    }

    /* enable statistics */
    g_active_evt_head.state = ON;

    mutex_unlock(&(g_active_evt_head.mutex));

    return HI_SUCCESS;
}

/*
 * stop current scenes statistics.
  */
HI_VOID low_delay_stop_statistics(HI_VOID)
{
    mutex_lock(&(g_active_evt_head.mutex));

    reset_evt_statistics_nolock();

    mutex_unlock(&(g_active_evt_head.mutex));
}
#if (1 == HI_PROC_SUPPORT)
/*
 * capture event statistics snapshot
 */
static HI_S32 capture_statistics_snapshot(struct low_delay_evt_head* dup)
{
    if (!dup)
        return HI_FAILURE;

    mutex_lock(&(g_active_evt_head.mutex));

    if (OFF == g_active_evt_head.state)
    {
        goto fail;
    }

    /* duplicate entire evt queue */
    memcpy(dup, &g_active_evt_head, sizeof(struct low_delay_evt_head));

    mutex_unlock(&(g_active_evt_head.mutex));

    return HI_SUCCESS;

fail:
    mutex_unlock(&(g_active_evt_head.mutex));
    return HI_FAILURE;
}

/*
 * display event head name
 */
static HI_VOID show_event_name(struct seq_file *s, struct low_delay_evt_head* head)
{
    HI_U32 index;
    HI_LD_Event_ID_E evt_id;

    if (!s || !head)
        return ;

    PROC_PRINT(s, "%8s    ", " ");
    traverse_scenes_each_event(index, evt_id, head->scenes_id)
    {
        PROC_PRINT(s, "%10s    ", g_event_name[evt_id]);
    }
    PROC_PRINT(s, "%10s    ", "Total_diff");
    PROC_PRINT(s, "\n");
}

/*
 * relink all event entry from queue into one list.
 */
static HI_S32 relink_low_delay_evt(struct low_delay_evt_head* head, struct list_head * relink_head)
{
    HI_U32 i, j ;
    HI_LD_Event_ID_E evt_id;
    struct low_delay_evt_queue * evt_queue;
    struct low_delay_evt_ext * evt_ext;

    if (!head || !relink_head)
        return HI_FAILURE;

    INIT_LIST_HEAD(relink_head);

    traverse_scenes_each_event(i, evt_id, head->scenes_id)
    {
        evt_queue = &(head->evt_queue[evt_id]);

        traverse_queue_valid_entry(j, evt_ext, evt_queue)
        {
            if (head->filter_handle == evt_ext->evt.handle)
            {
                list_add_tail(&(evt_ext->list), relink_head);
            }
            else
            {
                HI_ERR_STAT("id:%u, handle:0x%x, frame:%u, time:%u dismatch filter handle(0x%x)\n",
                    evt_ext->evt.evt_id, evt_ext->evt.handle, evt_ext->evt.frame, evt_ext->evt.time, head->filter_handle);
            }
        }
    }

    return HI_SUCCESS;
}

/*
 * display collected detail data.
 */
static HI_S32 show_event_detail_data(struct seq_file *s, struct low_delay_evt_head* head)
{
    HI_U32 current_key, i, j;
    HI_BOOL cal_diff_time;
    struct list_head relink_head;
    HI_LD_Event_ID_E evt_id;
    struct low_delay_evt_queue * evt_queue;
    struct low_delay_evt_ext * evt_ext, *first_evt_ext = NULL, *last_evt_ext = NULL;

    if (!head)
        return HI_FAILURE;

    /* relink new event list */
    if (HI_SUCCESS != relink_low_delay_evt(head, &relink_head))
    {
        HI_ERR_STAT("relink event list failed.\n");
        goto out;
    }

    while(!list_empty(&(relink_head)))
    {
        cal_diff_time = HI_TRUE;
        evt_ext = list_first_entry(&(relink_head), struct low_delay_evt_ext, list);
        first_evt_ext = evt_ext;
        current_key = evt_ext->evt.frame;

        PROC_PRINT(s, "%08d    ", current_key);

        traverse_scenes_each_event(i, evt_id, head->scenes_id)
        {
            HI_BOOL found = HI_FALSE;
            evt_queue = &(head->evt_queue[evt_id]);

            traverse_queue_valid_entry(j, evt_ext, evt_queue)
            {
                if (head->filter_handle == evt_ext->evt.handle && current_key == evt_ext->evt.frame && !list_empty(&(evt_ext->list)))
                {
                    PROC_PRINT(s, "%10d    ", evt_ext->evt.time);

                    last_evt_ext = evt_ext;

                    /* this evt is still in the queue, we should escape it next time. see prev list_empty checking */
                    list_del_init(&(evt_ext->list));
                    found = HI_TRUE;
                    break;
                }
            }

            if (HI_FALSE == found)
            {
                PROC_PRINT(s, "%10s    ", "-");
                cal_diff_time = HI_FALSE;
            }

        }

        /* calculate time difference from first event occur to final event finished*/
        if (HI_TRUE == cal_diff_time)
        {
            PROC_PRINT(s, "%10d    ", last_evt_ext ? last_evt_ext->evt.time - first_evt_ext->evt.time : 0);
        }
        else
        {
            PROC_PRINT(s, "%10s    ", "-");
        }

        PROC_PRINT(s, "\n");
    }

    return HI_SUCCESS;

out:
    return  HI_FAILURE;
}

/*
 * show statistics data by 'cat /proc/msp/low_delay_statistics' command
 */

static HI_S32 low_delay_proc_read(struct seq_file *s, HI_VOID *pArg)
{
    struct low_delay_evt_head * dup;

    dup = kmalloc(sizeof(struct low_delay_evt_head), GFP_KERNEL);
    if (!dup)
    {
        HI_ERR_STAT("create dup evt head failed.\n");
        goto out;
    }

    if(HI_SUCCESS != capture_statistics_snapshot(dup))
    {
        PROC_PRINT(s, "low delay statistics disabled now.\n");
        goto out;
    }

    show_event_name(s, dup);

    show_event_detail_data(s, dup);

out:

    if (dup)
        kfree(dup);
    return 0;
}
#endif
/*
 * add new event into low delay framework
 */
HI_VOID low_delay_notify_event(HI_LD_Event_S * evt)
{
    unsigned long flags;
    struct low_delay_evt_queue * evt_queue;
    HI_LD_Event_S * dup;

    if (unlikely(!evt))
        return ;

    /* statistics disabled ? */
    if (likely(OFF == g_active_evt_head.state))
    {
        return ;
    }

    /* filter */
    if (evt->handle != g_active_evt_head.filter_handle)
    {
        HI_DBG_STAT("filter invalid evt[name:'%s', handle:0x%x, frame:%u, time:%u].\n",
            g_event_name[evt->evt_id], evt->handle, evt->frame, evt->time);
        goto out;
    }

    evt_queue = &(g_active_evt_head.evt_queue[evt->evt_id]);

    /* evt is invalid for current scenes */
    if ( !evt_queue->active)
    {
        HI_DBG_STAT("filter inactive evt[name:'%s', handle:0x%x, frame:%u, time:%u].\n",
            g_event_name[evt->evt_id], evt->handle, evt->frame, evt->time);
        goto out;
    }

    spin_lock_irqsave(&(evt_queue->lock), flags);

    /* ring queue is full */
    if ( ( (evt_queue->tail + 1) % MAX_EVENT_QUEUE_SIZE == evt_queue->head ))
    {
        evt_queue->head = (evt_queue->head + 1) % MAX_EVENT_QUEUE_SIZE;
    }

    /* add evt into ring queue */
    dup = &( (evt_queue->queue[evt_queue->tail]).evt);
    memcpy(dup, evt, sizeof(HI_LD_Event_S));

    evt_queue->tail = (evt_queue->tail + 1) % MAX_EVENT_QUEUE_SIZE;

    spin_unlock_irqrestore(&(evt_queue->lock), flags);

out:
    return;
}

HI_VOID stat_event_fun(STAT_EVENT_E enEvent, HI_U32 Value);

HI_S32 HI_DRV_STAT_Init(HI_VOID)
{
    HI_S32 ret;
#if (1 == HI_PROC_SUPPORT)
    DRV_PROC_ITEM_S *item ;
#endif
    int i;

    ret = HI_DRV_MODULE_Register(HI_ID_STAT, "HI_STAT", HI_NULL);
    if (HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    memset((void *)g_IsrTime, 0, sizeof(g_IsrTime));

    g_stat_thread_kvirt_base = dma_alloc_coherent(NULL, STAT_USERSPACE_TOTAL_SIZE, &g_stat_thread_phyaddr_base, GFP_DMA);
    if (NULL == g_stat_thread_kvirt_base)
    {
        return -1;
    }

    memset((void *)g_stat_thread_kvirt_base, 0, STAT_USERSPACE_TOTAL_SIZE);

    for (i = 0; i < STAT_MAX_THREAD_USERSPACE; i++)
    {
        g_stat_thread_kvirt_base[i].bUsed = HI_FALSE;
    }

    HI_OSAL_Snprintf(g_srtuStatDev.devfs_name, sizeof(g_srtuStatDev.devfs_name), "%s", UMAP_DEVNAME_STAT);
    g_srtuStatDev.fops = &DRV_stat_Fops;
    g_srtuStatDev.minor = UMAP_MIN_MINOR_STAT;
    g_srtuStatDev.owner  = THIS_MODULE;
    g_srtuStatDev.drvops = NULL;
    if(HI_DRV_DEV_Register(&g_srtuStatDev) < 0)
    {
        HI_ERR_STAT("cann't register stat dev\n");
        dma_free_coherent(NULL, STAT_USERSPACE_TOTAL_SIZE, g_stat_thread_kvirt_base, g_stat_thread_phyaddr_base);
        return -1;
    }
#if (1 == HI_PROC_SUPPORT)
    item = HI_DRV_PROC_AddModule(HI_MOD_STAT, NULL, NULL);
    if (!item)
    {
        HI_DRV_DEV_UnRegister(&g_srtuStatDev);
        dma_free_coherent(NULL, STAT_USERSPACE_TOTAL_SIZE, g_stat_thread_kvirt_base, g_stat_thread_phyaddr_base);
        return -1;
    }
    item->read = stat_proc_read;
    item->write = NULL;
#endif
    HI_DRV_STAT_EventFunc_Register(stat_event_fun);
#if (1 == HI_PROC_SUPPORT)
    /* register low delay statistics proc interface */
    item = HI_DRV_PROC_AddModule(HI_MOD_EVENT, NULL, NULL);
    if (!item)
    {
        HI_ERR_STAT("cann't register low delay statistics interface.\n");
        HI_DRV_DEV_UnRegister(&g_srtuStatDev);
        dma_free_coherent(NULL, STAT_USERSPACE_TOTAL_SIZE, g_stat_thread_kvirt_base, g_stat_thread_phyaddr_base);
        return -1;
    }
    item->read = low_delay_proc_read;
    item->write = NULL;
#endif
    return 0;
}

HI_VOID HI_DRV_STAT_Exit(HI_VOID)
{
    HI_DRV_PROC_RemoveModule(HI_MOD_EVENT);
    HI_DRV_STAT_EventFunc_UnRegister();
    HI_DRV_PROC_RemoveModule(HI_MOD_STAT);
    HI_DRV_DEV_UnRegister(&g_srtuStatDev);
    dma_free_coherent(NULL, STAT_USERSPACE_TOTAL_SIZE, g_stat_thread_kvirt_base, g_stat_thread_phyaddr_base);
    HI_DRV_MODULE_UnRegister(HI_ID_STAT);
    return;
}

#if defined(HI_STAT_ISR_SUPPORTED)
HI_VOID HI_DRV_STAT_IsrReset(HI_VOID)
{
    memset((void *)g_IsrTime, 0, sizeof(g_IsrTime));
}

HI_VOID HI_DRV_STAT_IsrEnable(HI_VOID)
{
    g_stat_isr_enable = HI_TRUE;
}

HI_VOID HI_DRV_STAT_IsrDisable(HI_VOID)
{
    g_stat_isr_enable = HI_FALSE;
}

HI_VOID HI_DRV_STAT_IsrBegin(STAT_ISR_E isr)
{
    if(g_stat_isr_enable == HI_FALSE)
        return ;

    if(isr < STAT_ISR_BUTT)
        do_gettimeofday(&(g_IsrTime[isr].tv));
}

HI_VOID HI_DRV_STAT_IsrEnd(STAT_ISR_E isr)
{
    if(g_stat_isr_enable == HI_FALSE)
        return ;

    if(isr < STAT_ISR_BUTT)
    {
        struct timeval tv, tv2;
        HI_U32 time = 0;

        tv = g_IsrTime[isr].tv;

        do_gettimeofday(&tv2);

        if(tv2.tv_sec > tv.tv_sec)
        {
            time = (tv2.tv_sec - tv.tv_sec)*1000000 + tv2.tv_usec - tv.tv_usec;
        }
        else
        {
            time = tv2.tv_usec - tv.tv_usec;
        }

        if(time > g_IsrTime[isr].time)
            g_IsrTime[isr].time = time;
    }
}
#endif

HI_VOID stat_event_fun(STAT_EVENT_E enEvent, HI_U32 Value)
{
    HI_U64   SysTime;

    if ( enEvent >= STAT_EVENT_BUTT )
    {
        return;
    }

    SysTime = sched_clock();

    SysTime = DIV64(SysTime, 1000000);

    switch ( enEvent )
    {
        case STAT_EVENT_KEYIN :
        case STAT_EVENT_KEYOUT:
        case STAT_EVENT_ISTREAMGET:
        case STAT_EVENT_BUFREADY:
        case STAT_EVENT_FRAMESYNCOK:
            {
                g_EventTime[enEvent].Value1 = Value;
            }
            break;
        case STAT_EVENT_CONNECT:
            {
                g_EventTime[STAT_EVENT_LOCKED].EventMs = 0;
            }
            break;
        case STAT_EVENT_VSTART:
            {
                HI_S32 Index;
                for (Index = STAT_EVENT_CWSET; Index < STAT_EVENT_BUTT; Index++ )
                {
                    g_EventTime[Index].EventMs = 0;
                    g_EventTime[Index].Value1 = 0;
                    g_EventTime[Index].Value2 = 0;
                }
            }
            break;
        case STAT_EVENT_CWSET :
        case STAT_EVENT_LOCKED :
            {
                if (0 != g_EventTime[enEvent].EventMs)
                {
                    return;
                }
            }
            break;
        case STAT_EVENT_IFRAMEINTER:
            {
                g_EventTime[enEvent].Value1 = Value - g_EventTime[enEvent].Value2;
                g_EventTime[enEvent].Value2 = Value;
            }
            break;
        default:
            break;
    }

    g_EventTime[enEvent].EventMs = (HI_U32)SysTime;

    return;
}

#if defined(HI_STAT_ISR_SUPPORTED)
EXPORT_SYMBOL(HI_DRV_STAT_IsrBegin);
EXPORT_SYMBOL(HI_DRV_STAT_IsrEnd);
#endif