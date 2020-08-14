/**************************************************************************//**

  Copyright (C), 2001-2014, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file		  -gstsmeprotocol.c
 * @brief		  -sme demux IO protocol
 * @author	  -fengke(f00165155)
 * @date		  -2014/3/27
 * @version      -V1.00
 *
 * History:
 *
 * Number  : V1.00
 * Date 	 : 2014/3/27
 * Author	 : fengke(f00165155)
 * Desc 	 : Created file
 *
******************************************************************************/


#ifdef HAVE_CONFIG_H
//#include "config.h"
#endif
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <gst/gst.h>
#include "osal_str.h"
#include "gstsme.h"
#include "sme_demux_inf.h"
#include "gstsmepipe.h"

#include "stdint.h"

//lint -e585 (可变参数...报错，为gstreamer中的日志宏，无法修改。)
//lint -e717 //do{}while(0)
//lint -e838 //前一次赋值未使用(通常是在变量定义时赋空值，无需修改)
//lint -esym(40,EINVAL) //系统变量不识别，头文件已包含
//lint -esym(40,EIO) //系统变量不识别，头文件已包含
//lint -esym(754,_tagSmeUrlCtx::av_class)//未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::prot) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::filename)  //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::moved_url) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::is_connected) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::int64FileOffset) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::int64FileLen) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::interrupt_callback) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::headers) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::hoststr) //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::parent)  //未使用的结构体成员
//lint -esym(754,_tagSmeUrlCtx::rw_timeout)  //未使用的结构体成员

typedef struct _GstProtocolInfo GstProtocolInfo;

// for ffmpeg-2.8.x
typedef struct _tagSmeUrlCtx {
    const V_VOID *av_class;    /**< information for av_log(). Set by url_open(). */
    V_VOID *prot;
    void *priv_data;
    V_CHAR *filename;             /**< specified URL */
    V_CHAR *moved_url;            /**< added by hisi, 302 Moved Temporarily keep host name string */
    V_VOID *parent;               /**< added by hisi, record parent context */
    V_INT32 flags;
    V_INT32 max_packet_size;        /**< if non zero, the stream is packetized with this max packet size */
    V_INT32 is_streamed;            /**< true if streamed (no seek possible), default = false */
    V_INT32 is_connected;
    ST_SME_IOINTERRUPTCB interrupt_callback;
    V_INT64 rw_timeout;         /**< maximum time to wait for (network) read/write operation completion, in mcs */
}*PST_SME_URL_CTX;

struct _GstProtocolInfo
{
    GstPad *pad;

    guint64 offset;
    gboolean eos;
    gint set_streamheader;
};

static gint
gst_smedata_open (V_VOID* pvUrlCtx, const gchar* pszUrl, gint i32Flags)
{
    GstProtocolInfo *info;
    GstPad *pad;
    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx || NULL == pszUrl)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    info = g_new0 (GstProtocolInfo, 1);

    info->set_streamheader = i32Flags & GST_SME_URL_STREAMHEADER;
    i32Flags &= ~GST_SME_URL_STREAMHEADER;
    V_UINT32 tempFlags = (V_UINT32)(pstUrlCtx->flags);
    pstUrlCtx->flags = (V_INT32)(tempFlags & ~GST_SME_URL_STREAMHEADER);

    /* we don't support R/W together */
    if (i32Flags != SME_URL_RDONLY && i32Flags != SME_URL_WRONLY) {
        GST_WARNING ("Only read-only or write-only are supported");
        return -EINVAL;
    }

    // "0x" is not recognized by "VOS_Sscanf_S".
    // For example: "gstreamer://0x8b6590"
    if (VOS_Sscanf_S (&pszUrl[14], "%p", &pad) != 1) {
        GST_WARNING ("could not decode pad.");
        return -EIO;
    }

    /* make sure we're a pad and that we're of the right type */
    g_return_val_if_fail (GST_IS_PAD (pad), -EINVAL);

    if (i32Flags == SME_URL_RDONLY) {
        g_return_val_if_fail (GST_PAD_IS_SINK (pad), -EINVAL);
    }else {
        g_return_val_if_fail (GST_PAD_IS_SRC (pad), -EINVAL);
    }

    info->eos = FALSE;
    info->pad = pad;
    info->offset = 0;

    pstUrlCtx->priv_data = (void *) info;
    pstUrlCtx->is_streamed = FALSE;
    pstUrlCtx->max_packet_size = 0;

    return 0;
}

static gint
gst_smedata_peek (V_VOID* pvUrlCtx, guint8* pu8Buf, gint i32Size)
{
    GstProtocolInfo *info;
    GstBuffer *inbuf = NULL;
    GstFlowReturn ret;
    gint total = 0;

    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    g_return_val_if_fail (pstUrlCtx->flags == SME_URL_RDONLY, -1);
    info = (GstProtocolInfo *) pstUrlCtx->priv_data;

    GST_DEBUG ("Pulling %d bytes at position %" G_GUINT64_FORMAT, i32Size,
        info->offset);

    ret = gst_pad_pull_range (info->pad, info->offset, (guint) i32Size, &inbuf);

    switch (ret) {
    case GST_FLOW_OK:
        total = (gint) gst_buffer_get_size (inbuf);
        gst_buffer_extract (inbuf, 0, pu8Buf, (guint)total);
        gst_buffer_unref (inbuf);
        break;
    case GST_FLOW_EOS:
        total = 0;
        break;
    case GST_FLOW_FLUSHING:
        total = -1;
        break;
    default:
    case GST_FLOW_ERROR:
        total = -2;
        break;
    }

    GST_DEBUG ("Got %d return result %d", ret, total);

    return total;
}

static gint
gst_smedata_read (V_VOID* pvUrlCtx, guint8* pu8Buf, gint i32Size)
{
    gint64 res;
    GstProtocolInfo *info;
    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    info = (GstProtocolInfo *) pstUrlCtx->priv_data;

    GST_DEBUG ("Reading %d bytes of data at position %" G_GUINT64_FORMAT, i32Size,
        info->offset);

    res = gst_smedata_peek (pstUrlCtx, pu8Buf, i32Size);
    if (res >= 0)
        info->offset += (guint64)res;

    GST_DEBUG ("Returning %lld bytes", res);

    return (gint)res;
}

static gint
gst_smedata_write (V_VOID* pvUrlCtx, const guint8* pu8Buf, gint i32Size)
{
    GstProtocolInfo *info;
    GstBuffer *outbuf;
    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    GST_DEBUG ("Writing %d bytes", i32Size);
    info = (GstProtocolInfo *) pstUrlCtx->priv_data;

    g_return_val_if_fail (pstUrlCtx->flags != SME_URL_RDONLY, -EIO);

    /* create buffer and push data further */
    outbuf = gst_buffer_new_and_alloc ((gsize)i32Size);

    gst_buffer_fill (outbuf, 0, pu8Buf, (gsize)i32Size);

    if (gst_pad_push (info->pad, outbuf) != GST_FLOW_OK)
        return 0;

    info->offset += (guint64)(gint64)i32Size;
    return i32Size;
}

static int64_t
gst_smedata_seek (V_VOID* pvUrlCtx, int64_t i64Pos, gint32 i32Where)
{
    GstProtocolInfo *info;
    guint64 newpos = 0, oldpos;
    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    GST_DEBUG ("Seeking to %" G_GINT64_FORMAT ", whence=%d",
        (gint64) i64Pos, i32Where);

    info = (GstProtocolInfo *) pstUrlCtx->priv_data;

    /* TODO : if we are push-based, we need to return sensible info */

    switch (pstUrlCtx->flags) {
    case SME_URL_RDONLY:
        {
            /* sinkpad */
            switch (i32Where) {
            case SEEK_SET:
                newpos = (guint64)(gint64)i64Pos;
                break;
            case SEEK_CUR:
                newpos = info->offset + (guint64)(gint64)i64Pos;
                break;
            case SEEK_END:
            case SME_AV_SEEK_SIZE:
                /* ffmpeg wants to know the current end position in bytes ! */
                {
                    gint64 duration;

                    GST_DEBUG ("Seek end");

                    if (gst_pad_is_linked (info->pad))
                        if (gst_pad_query_duration (GST_PAD_PEER (info->pad),
                        GST_FORMAT_BYTES, &duration))
                            newpos = ((guint64) duration) + (guint64)(gint64)i64Pos;
                }
                break;
            default:
                GST_ERROR ("fatal error.");//g_assert (0);
                break;
            }

            /* FIXME : implement case for push-based behaviour */
            if (i32Where != SME_AV_SEEK_SIZE)
                info->offset = newpos;
        }
        break;
    case SME_URL_WRONLY:
    {
        GstSegment segment;

        oldpos = info->offset;

        /* srcpad */
        switch (i32Where) {
        case SEEK_SET:
        {
            info->offset = (guint64)(gint64)i64Pos;
            break;
        }
        case SEEK_CUR:
            info->offset += (guint64)(gint64)i64Pos;
            break;
        default:
            break;
        }
        newpos = info->offset;

        if (newpos != oldpos) {
            gst_segment_init (&segment, GST_FORMAT_BYTES);
            segment.start = newpos;
            segment.time = newpos;
            gst_pad_push_event (info->pad, gst_event_new_segment (&segment));
        }
        break;
    }
    default:
        GST_ERROR ("fatal error.");//g_assert (0);
        break;
	}

    GST_DEBUG ("Now at offset %" G_GUINT64_FORMAT " (returning %" G_GUINT64_FORMAT
        ")", info->offset, newpos);

    return (int64_t)(gint64)newpos;
}

static gint
gst_smedata_close (V_VOID* pvUrlCtx)
{
    GstProtocolInfo *info;
    PST_SME_URL_CTX pstUrlCtx = NULL;

    if (NULL == pvUrlCtx)
    {
        GST_WARNING ("invalid param.");
        return -EINVAL;
    }

    pstUrlCtx = (PST_SME_URL_CTX)pvUrlCtx;

    info = (GstProtocolInfo *) pstUrlCtx->priv_data;
    if (info == NULL)
        return 0;

    GST_LOG ("Closing file");

    switch (pstUrlCtx->flags) {
    case SME_URL_WRONLY:
    {
        /* send EOS - that closes down the stream */
        gst_pad_push_event (info->pad, gst_event_new_eos ());
        break;
    }
    default:
        break;
    }

    /* clean up data */
    g_free (info);
    pstUrlCtx->priv_data = NULL;

    return 0;
}


ST_SME_IOPROTO gst_protocol = {
    /*.name = */ "gstreamer",
    /*.url_open = */   gst_smedata_open,
    /*.url_read = */   gst_smedata_read,
    /*.url_write = */  gst_smedata_write,
    /*.url_seek = */   (V_INT64 (*)(V_VOID*, V_INT64, V_INT32))gst_smedata_seek,
    /*.url_close = */  gst_smedata_close,
};

/* specialized protocol for cross-thread pushing,
 * based on ffmpeg's pipe protocol */

static int
gst_sme_pipe_open (V_VOID * inh, const char *filename, int flags)
{
    GstSmePipe *smepipe;
    PST_SME_URL_CTX h = (PST_SME_URL_CTX)inh;

    /* we don't support W together */
    if (flags != 1) {
        GST_WARNING ("Only read-only is supported");
        return -EINVAL;
    }

    // "0x" is not recognized by "VOS_Sscanf_S".
    if (VOS_Sscanf_S (&filename[12], "%p", &smepipe) != 1) {
        GST_WARNING ("could not decode pipe info.");
        return -EIO;
    }

    /* sanity check */
    g_return_val_if_fail (GST_IS_ADAPTER (smepipe->adapter), -EINVAL);

    h->priv_data = (void *) smepipe;
    h->is_streamed = TRUE;
    h->max_packet_size = 0;

    return 0;
}

static int
gst_sme_pipe_read (V_VOID * inh, unsigned char *buf, int size)
{
    GstSmePipe *smepipe;
    guint available;
    PST_SME_URL_CTX h = (PST_SME_URL_CTX)inh;

    smepipe = (GstSmePipe *) h->priv_data;

    GST_LOG ("requested size %d", size);

    GST_SME_PIPE_MUTEX_LOCK (smepipe);

    /** BEGIN: Modified for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */
    while ((available = gst_adapter_available (smepipe->adapter)) < (guint)size
            && !smepipe->eos && !smepipe->flush) {
        GST_DEBUG ("Available:%d, requested:%d", available, size);
        smepipe->needed = (guint)size;
        GST_SME_PIPE_SIGNAL (smepipe);
        GST_SME_PIPE_WAIT (smepipe);
    }

    if(smepipe->flush){
        gst_adapter_clear (smepipe->adapter);
        smepipe->needed = 0;
        GST_SME_PIPE_MUTEX_UNLOCK (smepipe);
        GST_INFO ("flush");
        return 0;
    }
    /** END: Modified for DTS2014072905447 by zhoubing(z00213950), 2014/7/28 */

    size = MIN ((int)available, size);
    if (size) {
        GST_LOG ("Getting %d bytes", size);
        gst_adapter_copy (smepipe->adapter, buf, 0, (guint)size);
        gst_adapter_flush (smepipe->adapter, (guint)size);
        GST_LOG ("%" G_GSIZE_FORMAT " bytes left in adapter",
                gst_adapter_available (smepipe->adapter));
        smepipe->needed = 0;
    }
    GST_SME_PIPE_MUTEX_UNLOCK (smepipe);

    return size;
}

static int
gst_sme_pipe_close (V_VOID * inh)
{
    GST_LOG ("Closing pipe");
    PST_SME_URL_CTX h = (PST_SME_URL_CTX)inh;

    h->priv_data = NULL;

    return 0;
}

// For HLS/Dash VOD play scenario
ST_SME_IOPROTO gstpipe_protocol = {
    "gstpipe",
    gst_sme_pipe_open,
    gst_sme_pipe_read,
    NULL,
    NULL,
    gst_sme_pipe_close,
};

