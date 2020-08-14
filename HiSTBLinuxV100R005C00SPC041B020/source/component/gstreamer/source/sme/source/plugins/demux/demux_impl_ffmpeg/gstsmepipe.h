/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    gstsmepipe.h
 * @brief   httpsrc 头文件定义
 * @author  zhoubing(z00213950)
 * @date    2014/7/14
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/7/14
 * Author : zhoubing(z00213950)
 * Desc   : Created file
 *
******************************************************************************/


#ifndef __GST_SMEPIPE_H__
#define __GST_SMEPIPE_H__

#include <gst/base/gstadapter.h>
#include <gst/gst.h>

G_BEGIN_DECLS

/* pipe protocol helpers */
#define GST_SME_PIPE_MUTEX_LOCK(m) G_STMT_START {                    \
  GST_LOG_OBJECT (m, "locking tlock from thread %p", g_thread_self ()); \
  g_mutex_lock (&m->tlock);                                              \
  GST_LOG_OBJECT (m, "locked tlock from thread %p", g_thread_self ());  \
} G_STMT_END

#define GST_SME_PIPE_MUTEX_UNLOCK(m) G_STMT_START {                    \
  GST_LOG_OBJECT (m, "unlocking tlock from thread %p", g_thread_self ()); \
  g_mutex_unlock (&m->tlock);                                              \
} G_STMT_END

#define GST_SME_PIPE_WAIT(m) G_STMT_START {                          \
  GST_LOG_OBJECT (m, "thread %p waiting", g_thread_self ());            \
  g_cond_wait (&m->cond, &m->tlock);                                      \
} G_STMT_END

#define GST_SME_PIPE_SIGNAL(m) G_STMT_START {                        \
  GST_LOG_OBJECT (m, "signalling from thread %p", g_thread_self ());    \
  g_cond_signal (&m->cond);                                              \
} G_STMT_END

typedef struct _GstSmePipe GstSmePipe;

struct _GstSmePipe
{
  /* lock for syncing */
  GMutex tlock;
  /* with TLOCK */
  /* signals counterpart thread to have a look */
  GCond cond;
  /* seen eos */
  gboolean eos;
  /** BEGIN: Added for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
  gboolean flush;
  /** END: Added for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
  /* flowreturn obtained by src task */
  GstFlowReturn srcresult;
  /* adpater collecting data */
  GstAdapter *adapter;
  /* amount needed in adapter by src task */
  guint needed;
};

G_END_DECLS

#endif /* __GST_SMEPIPE_H__ */
