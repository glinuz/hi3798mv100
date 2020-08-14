/*========================================================================
  Open MAX   Component: Video Decoder
  This module contains the class definition for openMAX decoder component.
  Author: yangyichang 00226912
==========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>

#ifdef ANDROID
#include "gralloc_priv.h"
#endif

#include "hi_math.h"

#include "omx_vdec.h"
#include "omx_codec_type.h"
#include "omx_dbg.h"
#include "hi_common.h"
#include "hi_drv_stat.h"
#include "hi_mpi_stat.h"

#ifdef HI_OMX_TEE_SUPPORT
#include "sec_mmz.h"
#endif

#ifdef HI_OMX_TEE_SUPPORT
#ifdef DEBUG_SAVE_YUV
OMX_S32 yuv_secure_mem_exchange(OMX_U32 pSrcPhyAddr, HI_MMZ_BUF_S *pBuffer)
{
    HI_S32 ret;

    if (0 == pSrcPhyAddr || NULL == pBuffer)
    {
        DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
        return HI_FAILURE;
    }

    snprintf(pBuffer->bufname, sizeof(pBuffer->bufname), "OMXVDEC_SEC_EXC");

    ret = HI_MMZ_Malloc(pBuffer);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("%s() mmz alloc exchange mem error!\n", __func__);
        return HI_FAILURE;
    }

    ret = HI_SEC_MMZ_TA2CA((unsigned long)pSrcPhyAddr, (unsigned long)pBuffer->phyaddr, (unsigned long)pBuffer->bufsize);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("%s() call HI_SEC_MMZ_TA2CA failed!\n", __func__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID yuv_exchange_mem_free(HI_MMZ_BUF_S *pBuffer)
{
    HI_S32 ret;

    if (NULL == pBuffer)
    {
        DEBUG_PRINT_ERROR("%s() invalid param\n", __func__);
        return;
    }

    ret = HI_MMZ_Free(pBuffer);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("%s() free 0x%x failed.\n", __func__, pBuffer->phyaddr);
    }

    return;
}
#endif
#endif


static OMX_ERRORTYPE omx_report_event(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_IN OMX_EVENTTYPE event_type,
       OMX_IN OMX_U32 param1,
       OMX_IN OMX_U32 param2,
       OMX_IN OMX_PTR pdata)
{
    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(pcom_priv->m_cb.EventHandler == NULL);

    return pcom_priv->m_cb.EventHandler(pcom_priv->m_pcomp, pcom_priv->m_app_data,event_type, param1, param2, pdata);
}


static inline OMX_ERRORTYPE omx_report_error(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_ERRORTYPE  error_type)
{
    return omx_report_event(pcom_priv, OMX_EventError, (OMX_U32)error_type, 0, NULL);
}


static OMX_BOOL port_full(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32  port)
{
    OMX_PORT_PRIVATE *port_priv = &pcom_priv->m_port[port];

    if (port_priv->m_cur_buf_num == port_priv->port_pro.max_count)
    {
        return OMX_TRUE;
    }
    else
    {
        return OMX_FALSE;
    }
}


static OMX_BOOL port_empty(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32  port)
{
    OMX_PORT_PRIVATE *port_priv = &pcom_priv->m_port[port];

    if (port_priv->m_cur_buf_num == 0)
    {
        return OMX_TRUE;
    }
    else
    {
        return OMX_FALSE;
    }
}


static OMX_BOOL ports_all_full(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    if (port_full(pcom_priv, INPUT_PORT_INDEX) != OMX_TRUE)
    {
        return OMX_FALSE;
    }

    return port_full(pcom_priv, OUTPUT_PORT_INDEX);
}


static OMX_BOOL ports_all_empty(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    if (port_empty(pcom_priv, INPUT_PORT_INDEX) != OMX_TRUE)
    {
        return OMX_FALSE;
    }

    return port_empty(pcom_priv, OUTPUT_PORT_INDEX);
}


static OMX_ERRORTYPE post_event(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_U32 param1,
       OMX_U32 param2,
       OMX_U8 id)
{
    OMX_S32 n = -1;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    pthread_mutex_lock(&pcom_priv->m_lock);
    if ((id == OMX_GENERATE_FTB) || (id == OMX_GENERATE_FBD))
    {
        push_entry(&pcom_priv->m_ftb_q, param1, param2, id);
    }
    else if ((id == OMX_GENERATE_ETB) || (id == OMX_GENERATE_EBD))
    {
        push_entry(&pcom_priv->m_etb_q, param1, param2, id);
    }
    else
    {
        push_entry(&pcom_priv->m_cmd_q, param1, param2, id);
    }

    n = write(pcom_priv->m_pipe_out, &id, 1);
    if (n < 0)
    {
        DEBUG_PRINT_ERROR("post message failed,id = %d\n", id);
        ret =  OMX_ErrorUndefined;
    }

    pthread_mutex_unlock(&pcom_priv->m_lock);

    return ret;
}


static OMX_ERRORTYPE fill_buffer_done(OMX_COMPONENT_PRIVATE* pcom_priv, OMX_BUFFERHEADERTYPE *buffer)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

    if (port_priv->m_port_flushing)
    {
        buffer->nFilledLen = 0;
    }

    if (!pcom_priv->m_cb.FillBufferDone)
    {
        DEBUG_PRINT_ERROR("[FBD] FillBufferDone callback NULL!\n");
        return OMX_ErrorUndefined;
    }

    ret = pcom_priv->m_cb.FillBufferDone(pcom_priv->m_pcomp, pcom_priv->m_app_data, buffer);

    DEBUG_PRINT_STREAM("[FBD] success <<< useraddr = %p, data_len: %ld pts:    %lld\n", buffer->pBuffer, buffer->nFilledLen, buffer->nTimeStamp);

    return ret;
}


static OMX_ERRORTYPE empty_buffer_done(OMX_COMPONENT_PRIVATE* pcom_priv, OMX_BUFFERHEADERTYPE *buffer)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    buffer->nFilledLen = 0;

    if (!pcom_priv->m_cb.EmptyBufferDone)
    {
        DEBUG_PRINT_ERROR("[EBD] EmptyBufferDone callback NULL!\n");
        return OMX_ErrorUndefined;
    }

    ret = pcom_priv->m_cb.EmptyBufferDone(pcom_priv->m_pcomp, pcom_priv->m_app_data, buffer);

    DEBUG_PRINT_STREAM("[EBD] success <<< useraddr = %p\n", buffer->pBuffer);

    return ret;
}


// native buffer
#ifdef ANDROID
static OMX_ERRORTYPE use_android_native_buffer_internal(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_INOUT OMX_BUFFERHEADERTYPE **omx_bufhdr,
       OMX_IN OMX_PTR app_data,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_U32 bytes,
       OMX_IN OMX_U8* p_handle)
{
    OMX_U32 i = 0;
    OMX_S32 ret = -1;
    OMX_U32 buf_size = 0, buf_cnt = 0;
    OMX_ERRORTYPE error = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_BUFFERHEADERTYPE *pomx_buf = NULL;
    OMX_U8* UserVirAddr = NULL;
    private_handle_t *p_private_handle = (private_handle_t *)p_handle;

    OMXVDEC_BUF_DESC* pvdec_buf = NULL;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(p_handle  == NULL);
    OMX_CHECK_ARG_RETURN(p_private_handle->ion_phy_addr == 0);
    OMX_CHECK_ARG_RETURN(p_private_handle->ion_metadata_phy_addr == 0);

    port_priv = &pcom_priv->m_port[port];
    buf_cnt = port_priv->port_pro.max_count;

    if (bytes != port_priv->port_pro.buffer_size)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: buf size invalid, bytes = %ld, buf_size = %ld\n", bytes, port_priv->port_pro.buffer_size);
        return OMX_ErrorBadParameter;
    }

    if (p_private_handle->stride != pcom_priv->pic_info.stride)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: stride invalid, p_private_handle->stride = %ld, OMX stride = %ld\n",\
                           p_private_handle->stride, pcom_priv->pic_info.stride);
        return OMX_ErrorBadParameter;
    }

    buf_size = DEFAULT_FRAME_SIZE(p_private_handle->stride, pcom_priv->pic_info.frame_height);

    /* find an idle buffer slot */
    if (port_priv->m_cur_buf_num >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }
    i = port_priv->m_cur_buf_num;

#ifndef HI_OMX_TEE_SUPPORT
    UserVirAddr = p_private_handle->base;

    if(NULL ==  UserVirAddr)
    {
        DEBUG_PRINT_ERROR("%s() get user vir addr failed!\n", __func__);
        return OMX_ErrorUndefined;
    }
#endif

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));
    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: alloc omx buffer pomx_buf failed\n");
        return OMX_ErrorInsufficientResources;
    }
    pvdec_buf = (OMXVDEC_BUF_DESC *)malloc(sizeof(OMXVDEC_BUF_DESC));
    if (!pvdec_buf)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: alloc omx buffer pvdec_buf failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit1;
    }

    memset(pomx_buf,  0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvdec_buf, 0, sizeof(OMXVDEC_BUF_DESC));

    /* skip buffer allocation, direct bind */
    pvdec_buf->dir            = (INPUT_PORT_INDEX == port)? PORT_DIR_INPUT: PORT_DIR_OUTPUT;
    pvdec_buf->bufferaddr       = UserVirAddr;
    pvdec_buf->phyaddr          = p_private_handle->ion_phy_addr;
    pvdec_buf->buffer_len       = buf_size;
    pvdec_buf->client_data      = (OMX_PTR)pomx_buf;
    pvdec_buf->out_frame.stride = p_private_handle->stride;
    pvdec_buf->buffer_type      = OMX_USE_NATIVE;

    //VPSS假4K输出需要额外分配buf传递信息
    if (p_private_handle->ion_metadata_phy_addr != -1 && p_private_handle->ion_metadata_phy_addr != NULL)
    {
        #if 0
            pvdec_buf->private_phyaddr = p_private_handle->ion_metadata_phy_addr + p_private_handle->ion_metadata_size/2;
        pvdec_buf->private_len     = p_private_handle->ion_metadata_size/2;
        #else
            pvdec_buf->private_phyaddr = p_private_handle->ion_metadata_phy_addr + offsetof(private_metadata_t, priv_data_start_pos);
            pvdec_buf->private_len     = p_private_handle->ion_metadata_size - offsetof(private_metadata_t, priv_data_start_pos);
        #endif
    }
    else
    {
        pvdec_buf->private_phyaddr = -1;
        pvdec_buf->private_len = 0;
    }

    if (channel_bind_buffer(&pcom_priv->drv_ctx, pvdec_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[UNB] Error: vdec bind user buf failed\n");
        error = OMX_ErrorUndefined;
        goto error_exit2;
    }

    if (pcom_priv->pic_info.stride != p_private_handle->stride)
    {
        DEBUG_PRINT_ERROR("comp stride(%d) != gpu stride(%d)\n", pcom_priv->pic_info.stride, p_private_handle->stride);
    }

    /* init OMX buffer head */
    pomx_buf->pBuffer            = p_handle;
    pomx_buf->nSize                = sizeof(OMX_BUFFERHEADERTYPE);
    pomx_buf->nVersion.nVersion    = OMX_VERSION;
    pomx_buf->nAllocLen            = buf_size;
    pomx_buf->nOffset             = 0;
    pomx_buf->pAppPrivate        = app_data;

    if (INPUT_PORT_INDEX == port)
    {
        pomx_buf->pOutputPortPrivate = NULL;
        pomx_buf->pInputPortPrivate     = (OMX_PTR)pvdec_buf;
        pomx_buf->nInputPortIndex    = INPUT_PORT_INDEX;
    }
    else
    {
        pomx_buf->pInputPortPrivate  = NULL;
        pomx_buf->pOutputPortPrivate = (OMX_PTR)pvdec_buf;
        pomx_buf->nOutputPortIndex   = OUTPUT_PORT_INDEX;
    }

    *omx_bufhdr = pomx_buf;

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_cur_buf_num++;

    /*DEBUG_PRINT_STREAM("[UNB] Use %s buffer %d success: phyaddr = %x, size = %d\n",
                                           (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i, pvdec_buf->phyaddr, pvdec_buf->buffer_len);*/

    return OMX_ErrorNone;

error_exit2:
    free(pvdec_buf);
error_exit1:
    free(pomx_buf);

    DEBUG_PRINT_ERROR("[UNB] Use %s buffer %d failed\n", (port == OUTPUT_PORT_INDEX) ? "out" : "in", (int)i);

    return error;
}

static OMX_ERRORTYPE describe_color_format(DescribeColorFormatParams *params)
{
    if (params == NULL) {
        DEBUG_PRINT_ERROR("describeColorFormat: invalid params");
        return OMX_ErrorBadParameter;
    }

    MediaImage *img = &(params->sMediaImage);
    switch(params->eColorFormat) {
        case HAL_PIXEL_FORMAT_YCbCr_420_SP://nv12
        {
            img->mType = MEDIA_IMAGE_TYPE_YUV;
            img->mNumPlanes = 3;
            // mWidth and mHeight represent the W x H of the largest plane
            // In our case, this happens to be the Stride x Scanlines of Y plane
            img->mWidth = params->nFrameWidth;
            img->mHeight = params->nFrameHeight;
            OMX_U32 planeWidth = HI_SYS_GET_STRIDE(params->nFrameWidth);
            OMX_U32 planeHeight = params->nFrameHeight;
            img->mBitDepth = 8;
            //Plane 0 (Y)
            img->mPlane[0].mOffset = 0;
            img->mPlane[0].mColInc = 1;
            img->mPlane[0].mRowInc = planeWidth; //same as stride
            img->mPlane[0].mHorizSubsampling = 1;
            img->mPlane[0].mVertSubsampling = 1;
            //Plane 1 (U)
            img->mPlane[1].mOffset = planeWidth * planeHeight;
            img->mPlane[1].mColInc = 2;           //interleaved UV
            img->mPlane[1].mRowInc = planeWidth;
            img->mPlane[1].mHorizSubsampling = 2;
            img->mPlane[1].mVertSubsampling = 2;
            //Plane 2 (V)
            img->mPlane[2].mOffset = planeWidth * planeHeight + 1;
            img->mPlane[2].mColInc = 2;           //interleaved UV
            img->mPlane[2].mRowInc = planeWidth;
            img->mPlane[2].mHorizSubsampling = 2;
            img->mPlane[2].mVertSubsampling = 2;
            break;
        }

       case HAL_PIXEL_FORMAT_YCrCb_420_SP://nv21
        {
            img->mType = MEDIA_IMAGE_TYPE_YUV;
            img->mNumPlanes = 3;
            // mWidth and mHeight represent the W x H of the largest plane
            // In our case, this happens to be the Stride x Scanlines of Y plane
            img->mWidth = params->nFrameWidth;
            img->mHeight = params->nFrameHeight;
            OMX_U32 planeWidth = HI_SYS_GET_STRIDE(params->nFrameWidth);
            OMX_U32 planeHeight = params->nFrameHeight;
            img->mBitDepth = 8;
            //Plane 0 (Y)
            img->mPlane[0].mOffset = 0;
            img->mPlane[0].mColInc = 1;
            img->mPlane[0].mRowInc = planeWidth; //same as stride
            img->mPlane[0].mHorizSubsampling = 1;
            img->mPlane[0].mVertSubsampling = 1;
            //Plane 1 (U)
            img->mPlane[1].mOffset = planeWidth * planeHeight + 1;
            img->mPlane[1].mColInc = 2;           //interleaved UV
            img->mPlane[1].mRowInc = planeWidth;
            img->mPlane[1].mHorizSubsampling = 2;
            img->mPlane[1].mVertSubsampling = 2;
            //Plane 2 (V)
            img->mPlane[2].mOffset = planeWidth * planeHeight;
            img->mPlane[2].mColInc = 2;           //interleaved UV
            img->mPlane[2].mRowInc = planeWidth;
            img->mPlane[2].mHorizSubsampling = 2;
            img->mPlane[2].mVertSubsampling = 2;
            break;
        }

        case OMX_COLOR_FormatYUV420Planar:
        case OMX_COLOR_FormatYUV420SemiPlanar:
            // We need not describe the standard OMX linear formats as these are
            // understood by client. Fail this deliberately to let client fill-in
            return OMX_ErrorUnsupportedSetting;

        default:
            // Rest all formats which are non-linear cannot be described
            DEBUG_PRINT_ERROR("color-format %x is not flexible", params->eColorFormat);
            img->mType = MEDIA_IMAGE_TYPE_UNKNOWN;
            return OMX_ErrorNone;
    };

    //DEBUG_PRINT_ERROR("NOTE: Describe color format : %x", params->eColorFormat);
    //DEBUG_PRINT_ERROR("  FrameWidth x FrameHeight : %d x %d", params->nFrameWidth, params->nFrameHeight);
    //DEBUG_PRINT_ERROR("  YWidth x YHeight : %d x %d", img->mWidth, img->mHeight);

    OMX_U32 i = 0;
    for (i = 0; i < img->mNumPlanes; ++i) {
        DEBUG_PRINT_ALWS("    Plane[%d] : offset=%d / xStep=%d / yStep = %d",
                         i, img->mPlane[i].mOffset, img->mPlane[i].mColInc, img->mPlane[i].mRowInc);
    }

    return OMX_ErrorNone;
}
#endif


static OMX_ERRORTYPE use_buffer_internal(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_INOUT OMX_BUFFERHEADERTYPE **omx_bufhdr,
       OMX_IN OMX_PTR app_data,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_U32 bytes,
       OMX_IN OMX_U8 *buffer)
{
    OMX_U32 i = 0;
    OMX_U32 buf_size = 0, buf_cnt = 0;
    OMX_ERRORTYPE error = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_BUFFERHEADERTYPE *pomx_buf = NULL;
    HI_U32 Phyaddr;

#ifndef HI_OMX_TEE_SUPPORT
    OMX_S32 ret = -1;
    HI_U32 BufSize;
#endif

    OMXVDEC_BUF_DESC* pvdec_buf = NULL;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    port_priv = &pcom_priv->m_port[port];
    buf_size = port_priv->port_pro.buffer_size;
    buf_cnt = port_priv->port_pro.max_count;

    if (bytes != buf_size)
    {
        DEBUG_PRINT_ERROR("[UB] Error: buf size invalid, bytes = %ld, buf_size = %ld\n", bytes, buf_size);
        return OMX_ErrorBadParameter;
    }

    /* find an idle buffer slot */
    if (port_priv->m_cur_buf_num >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[UB] Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }
    i = port_priv->m_cur_buf_num;

#ifdef HI_OMX_TEE_SUPPORT
    Phyaddr = (HI_U32)buffer;
#else
    ret = HI_MMZ_GetPhyaddr(buffer, &Phyaddr, &BufSize);
    if(ret < 0)
    {
        DEBUG_PRINT_ERROR("%s HI_MMZ_GetPhyaddr error!\n", __func__);
        return OMX_ErrorHardware;
    }
#endif

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));
    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[UB] Error: alloc omx buffer pomx_buf failed\n");
        return OMX_ErrorInsufficientResources;
    }
    pvdec_buf = (OMXVDEC_BUF_DESC *)malloc(sizeof(OMXVDEC_BUF_DESC));
    if (!pvdec_buf)
    {
        DEBUG_PRINT_ERROR("[UB] Error: alloc omx buffer pvdec_buf failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }

    memset(pomx_buf,  0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvdec_buf, 0, sizeof(OMXVDEC_BUF_DESC));

    /* skip buffer allocation, direct bind */
    pvdec_buf->dir         = (INPUT_PORT_INDEX == port)?  PORT_DIR_INPUT: PORT_DIR_OUTPUT;
    pvdec_buf->bufferaddr  = buffer;
    pvdec_buf->phyaddr     = Phyaddr;
    pvdec_buf->buffer_len  = bytes;
    pvdec_buf->client_data = (OMX_PTR)pomx_buf;
#ifdef HI_OMX_TEE_SUPPORT
    pvdec_buf->buffer_type = OMX_USE_SECURE;
#else
    pvdec_buf->buffer_type = OMX_USE_NOSECURE;
#endif
    if (channel_bind_buffer(&pcom_priv->drv_ctx, pvdec_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[UB] Error: vdec bind user buf failed\n");
        error = OMX_ErrorUndefined;
        goto error_exit1;
    }

    /* init OMX buffer head */
    pomx_buf->pBuffer            = buffer;
    pomx_buf->nSize                = sizeof(OMX_BUFFERHEADERTYPE);
    pomx_buf->nVersion.nVersion    = OMX_VERSION;
    pomx_buf->nAllocLen            = bytes;
    pomx_buf->nOffset             = 0;
    pomx_buf->pAppPrivate        = app_data;

    if (INPUT_PORT_INDEX == port)
    {
        pomx_buf->pInputPortPrivate  = (OMX_PTR)pvdec_buf;
        pomx_buf->nInputPortIndex    = INPUT_PORT_INDEX;
        pomx_buf->pOutputPortPrivate = NULL;
    }
    else
    {
        pomx_buf->pInputPortPrivate  = NULL;
        pomx_buf->pOutputPortPrivate = (OMX_PTR)pvdec_buf;
        pomx_buf->nOutputPortIndex   = OUTPUT_PORT_INDEX;
    }

    *omx_bufhdr = pomx_buf;

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_cur_buf_num++;

    /*DEBUG_PRINT_STREAM("[UB] Use %s buffer %d success: phyaddr = %x, size = %d\n",
                                           (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i, pvdec_buf->phyaddr, pvdec_buf->buffer_len);*/

    return OMX_ErrorNone;

error_exit1:
    free(pvdec_buf);
    pvdec_buf = NULL;
error_exit0:
    free(pomx_buf);
    pomx_buf = NULL;

    DEBUG_PRINT_ERROR("[UB] Use %s buffer %d failed\n", (port == OUTPUT_PORT_INDEX) ? "out" : "in", (int)i);

    return error;
}


static OMX_ERRORTYPE allocate_buffer_internal(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_INOUT OMX_BUFFERHEADERTYPE **omx_bufhdr,
       OMX_IN OMX_PTR app_data,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_U32 bytes)
{
    OMX_U32 i = 0;
    OMX_U32 buf_size = 0, buf_cnt = 0;
    OMX_BUFFERHEADERTYPE *pomx_buf = NULL;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_ERRORTYPE error = OMX_ErrorNone;

    OMXVDEC_BUF_DESC *pvdec_buf = NULL;

    port_priv = &pcom_priv->m_port[port];
    buf_cnt = port_priv->port_pro.max_count;
    buf_size = port_priv->port_pro.buffer_size;

    /* check alloc bytes */
    if (bytes != buf_size)
    {
        DEBUG_PRINT_ERROR("[AB] Error: buf size invalid, bytes = %ld, buf_size = %ld\n", bytes, buf_size);
        return OMX_ErrorBadParameter;
    }

    /* find an idle buffer slot */
    if (port_priv->m_cur_buf_num >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[AB] Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }
    i = port_priv->m_cur_buf_num;

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));
    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[AB] Error: alloc omx buffer pomx_buf failed\n");
        return OMX_ErrorInsufficientResources;
    }
    pvdec_buf = (OMXVDEC_BUF_DESC *)malloc(sizeof(OMXVDEC_BUF_DESC));
    if (!pvdec_buf)
    {
        DEBUG_PRINT_ERROR("[AB] Error: alloc omx buffer pvdec_buf failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }

    memset(pomx_buf,  0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvdec_buf, 0, sizeof(OMXVDEC_BUF_DESC));

    pvdec_buf->dir              = (INPUT_PORT_INDEX == port_priv->m_port_index)? PORT_DIR_INPUT: PORT_DIR_OUTPUT;
    pvdec_buf->client_data      = (OMX_PTR)pomx_buf;
    pvdec_buf->out_frame.stride = pcom_priv->pic_info.stride;

    pvdec_buf->buffer_len       = buf_size;
    pvdec_buf->align            = port_priv->port_pro.alignment;

#ifdef HI_OMX_TEE_SUPPORT
    pvdec_buf->buffer_type    = OMX_ALLOCATE_SECURE;
    pvdec_buf->is_sec         = OMX_TRUE;
#else
    pvdec_buf->buffer_type    = OMX_ALLOCATE_NONSECURE;
    pvdec_buf->is_sec         = OMX_FALSE;
#endif

    if (channel_alloc_buffer(&pcom_priv->drv_ctx, pvdec_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[AB] Error: alloc_contigous_buffer failed\n");
        error =  OMX_ErrorUndefined;
        goto error_exit1;
    }

#ifdef HI_OMX_TEE_SUPPORT
    //sec mode:do not map usespace vir
#else
    pvdec_buf->bufferaddr = HI_MEM_Map(pvdec_buf->phyaddr, pvdec_buf->buffer_len);
    if (pvdec_buf->bufferaddr == HI_NULL)
    {
        DEBUG_PRINT_ERROR("[AB] Error: map buffer failed\n");
        error =  OMX_ErrorUndefined;
        goto error_exit2;
    }
#endif

    /* bind this buffer to vdec driver */
    if (channel_bind_buffer(&pcom_priv->drv_ctx, pvdec_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[AB] Error: Bind buffer failed\n");
        error =  OMX_ErrorUndefined;
        goto error_exit3;
    }

    /* init buffer head */
    pomx_buf->nSize             = sizeof(OMX_BUFFERHEADERTYPE);
    pomx_buf->nVersion.nVersion = OMX_VERSION;
    pomx_buf->nAllocLen         = pvdec_buf->buffer_len;
    pomx_buf->nOffset           = 0;
    pomx_buf->pAppPrivate       = app_data;

#ifdef HI_OMX_TEE_SUPPORT
    if (pvdec_buf->dir == 0)
    {
        pomx_buf->pBuffer = (OMX_U8*)pvdec_buf->phyaddr;
    }
    else
    {
        pomx_buf->pBuffer = (OMX_U8*)pvdec_buf->bufferaddr;
    }
#else
    pomx_buf->pBuffer = (OMX_U8*)pvdec_buf->bufferaddr;
#endif

    if (port == INPUT_PORT_INDEX)
    {
        pomx_buf->pInputPortPrivate  = (OMX_PTR )pvdec_buf;
        pomx_buf->nInputPortIndex    = INPUT_PORT_INDEX;
        pomx_buf->pOutputPortPrivate = NULL;
    }
    else
    {
        pomx_buf->pInputPortPrivate  = NULL;
        pomx_buf->pOutputPortPrivate = (OMX_PTR )pvdec_buf;
        pomx_buf->nOutputPortIndex   = OUTPUT_PORT_INDEX;
    }

    *omx_bufhdr = pomx_buf;

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_cur_buf_num++;

    /*DEBUG_PRINT_STREAM("[AB] Alloc %s buffer %d success: phyaddr = %x, useraddr = %p, size = %d, pomx_buf=%p\n",
                       (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i,
                       pvdec_buf->phyaddr, pvdec_buf->bufferaddr, pvdec_buf->buffer_len, pomx_buf);*/

    return OMX_ErrorNone;

error_exit3:
#ifndef HI_OMX_TEE_SUPPORT
    HI_MEM_Unmap(pvdec_buf->bufferaddr);
error_exit2:
#endif
    channel_release_buffer(&pcom_priv->drv_ctx, pvdec_buf);
error_exit1:
    free(pvdec_buf);
    pvdec_buf = NULL;
error_exit0:
    free(pomx_buf);
    pomx_buf = NULL;

    DEBUG_PRINT_ERROR("[AB] Alloc %s buffer %d failed\n",(port == OUTPUT_PORT_INDEX) ? "out" : "in", (int)i);

    return error;
}


static OMX_ERRORTYPE free_buffer_internal(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_BUFFERHEADERTYPE *omx_bufhdr)
{
    OMX_U32  i;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMXVDEC_BUF_DESC* puser_buf = NULL;

    if(!omx_bufhdr)
    {
        DEBUG_PRINT_ALWS("free_buffer_internal port[%ld] omx_bufhdr is NULL\n", port);
        return OMX_ErrorNone;
    }

    port_priv = &pcom_priv->m_port[port];

    /* santity check */
    for (i = 0; i < port_priv->port_pro.max_count; i++)
    {
        if (omx_bufhdr == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_pro.max_count)
    {
        DEBUG_PRINT_ERROR("[FB] No buffers found for address[%p], port=%ld\n", omx_bufhdr->pBuffer, port);
        return OMX_ErrorBadParameter;
    }

    /* unbind vdec user buffer */
    if (OUTPUT_PORT_INDEX == port)
    {
        puser_buf = (OMXVDEC_BUF_DESC*)omx_bufhdr->pOutputPortPrivate;
    }
    else
    {
        puser_buf = (OMXVDEC_BUF_DESC*)omx_bufhdr->pInputPortPrivate;
    }

    if (channel_unbind_buffer(&pcom_priv->drv_ctx, puser_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[FB] unbind buffer failed\n");
    }

    if (OMX_ALLOCATE_NONSECURE == puser_buf->buffer_type || OMX_ALLOCATE_SECURE == puser_buf->buffer_type)
    {
    #ifdef HI_OMX_TEE_SUPPORT
        //sec mode:do not unmap usespace vir
    #else
        HI_MEM_Unmap(puser_buf->bufferaddr);
    #endif
        channel_release_buffer(&pcom_priv->drv_ctx, puser_buf);
    }

    free(puser_buf);
    puser_buf = NULL;
    if(omx_bufhdr->pPlatformPrivate)
    {
        free(omx_bufhdr->pPlatformPrivate);
    }
    free(omx_bufhdr);
    omx_bufhdr = NULL;

    if (port_priv->m_port_populated)
    {
        port_priv->m_port_populated = OMX_FALSE;
    }

    port_priv->m_omx_bufhead[i] = NULL;
    port_priv->m_cur_buf_num--;

    /*DEBUG_PRINT_STREAM("[FB] Free %s buffer %d success: phyaddr = %x, useraddr = %p, size = %d, omx_bufhdr=%p\n",
                       (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i,
                       puser_buf->phyaddr, puser_buf->bufferaddr, puser_buf->buffer_len, omx_bufhdr);*/

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE omx_flush_port(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 port)
{
    OMX_PORT_PRIVATE *port_priv = NULL;

    if ((port != OMX_ALL) && (port > OUTPUT_PORT_INDEX))
    {
        DEBUG_PRINT_ERROR("%s: Invalid port index\n", __func__);
        return OMX_ErrorUndefined;
    }

    if (port == INPUT_PORT_INDEX || port == OMX_ALL)
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (port == OUTPUT_PORT_INDEX || port == OMX_ALL)
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (channel_flush_port(&pcom_priv->drv_ctx, port) < 0)
    {
        DEBUG_PRINT_ERROR("%s: channel_flush_port failed\n", __func__);
        omx_report_error(pcom_priv, OMX_ErrorHardware);
        return OMX_ErrorHardware;
    }

    return OMX_ErrorNone;
}


static void return_outbuffers(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    OMX_U32  param1 = 0, param2 = 0, ident = 0;

    pthread_mutex_lock(&pcom_priv->m_lock);
    while (get_q_size(&pcom_priv->m_ftb_q) > 0)
    {
        pop_entry(&pcom_priv->m_ftb_q, &param1, &param2, &ident);
        fill_buffer_done(pcom_priv,(OMX_BUFFERHEADERTYPE *)param1);
    }
    pthread_mutex_unlock(&pcom_priv->m_lock);
}


static void return_inbuffers(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    OMX_U32  param1 = 0, param2 = 0, ident = 0;

    pthread_mutex_lock(&pcom_priv->m_lock);
    while (get_q_size(&pcom_priv->m_etb_q) > 0)
    {
        pop_entry(&pcom_priv->m_etb_q, &param1, &param2, &ident);
        empty_buffer_done(pcom_priv,(OMX_BUFFERHEADERTYPE *)param1);
    }
    pthread_mutex_unlock(&pcom_priv->m_lock);
}

static OMX_ERRORTYPE update_port_definitions(OMX_COMPONENT_PRIVATE *pcomp_priv,
                                             OMX_BOOL update_crop,
                                             OMX_BOOL update_input_size)
{
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_U32 output_buf_size = 0;
    OMX_U32 min_compression_ratio = 0;
    OMX_U32 protocol = 0;

    OMX_CHECK_ARG_RETURN(pcomp_priv == NULL);

    protocol = (OMX_U32)(pcomp_priv->drv_ctx.drv_cfg.chan_cfg.eVidStd);

    /* CodecType Relative */
    switch (protocol)
    {
        case STD_HEVC:
        {
            min_compression_ratio = 4;

            /*if not set the max w&h, use defualt: HEVC\H264:4K other:1080P*/
            pcomp_priv->pic_info.max_frame_width = \
                 (pcomp_priv->pic_info.max_frame_width == 0)? UHD_FRAME_WIDTH:pcomp_priv->pic_info.max_frame_width;

            pcomp_priv->pic_info.max_frame_height = \
                 (pcomp_priv->pic_info.max_frame_height == 0)? UHD_FRAME_HEIGHT:pcomp_priv->pic_info.max_frame_height;

            break;
        }

        case STD_H264:
        case STD_MVC:
        case STD_AVS:
        {
            min_compression_ratio = 3;

            pcomp_priv->pic_info.max_frame_width = \
                 (pcomp_priv->pic_info.max_frame_width == 0)? UHD_FRAME_WIDTH : pcomp_priv->pic_info.max_frame_width;

            pcomp_priv->pic_info.max_frame_height = \
                 (pcomp_priv->pic_info.max_frame_height == 0)? UHD_FRAME_HEIGHT : pcomp_priv->pic_info.max_frame_height;

            break;
        }

        case STD_MPEG4:
        case STD_MPEG2:
        case STD_VC1:
        case STD_H263:
        case STD_DIVX3:
        case STD_REAL8:
        case STD_REAL9:
        case STD_VP6:
        case STD_VP6F:
        case STD_VP6A:
        case STD_VP8:
        case STD_SORENSON:
        {
            min_compression_ratio = 2;

            pcomp_priv->pic_info.max_frame_width = \
                 (pcomp_priv->pic_info.max_frame_width == 0)? HD_FRAME_WIDTH : pcomp_priv->pic_info.max_frame_width;

            pcomp_priv->pic_info.max_frame_height = \
                 (pcomp_priv->pic_info.max_frame_height == 0)? HD_FRAME_HEIGHT : pcomp_priv->pic_info.max_frame_height;

            break;
        }
        default:
            DEBUG_PRINT_ERROR("%s: unkown standard type = %lu\n", __func__, protocol);
            return OMX_ErrorBadParameter;
    }

    /*if is adaptive, use the max w&h to calculate the input buf size*/
    if (pcomp_priv->is_adaptive)
    {
        output_buf_size = (pcomp_priv->pic_info.max_frame_width*pcomp_priv->pic_info.max_frame_height*3)/2;
    }
    else
    {
        output_buf_size = (pcomp_priv->pic_info.frame_width*pcomp_priv->pic_info.frame_height*3)/2;
    }

    if (update_input_size)
    {
        port_priv = &pcomp_priv->m_port[INPUT_PORT_INDEX];
        port_priv->port_pro.buffer_size = MAX(output_buf_size/min_compression_ratio,
                                              port_priv->port_pro.buffer_size);
    }

    return OMX_ErrorNone;
}



static OMX_ERRORTYPE update_picture_info(
       OMX_COMPONENT_PRIVATE *pcomp_priv,
       OMX_U32 width, OMX_U32 height, OMX_U32 stride)
{
    OMX_PORT_PRIVATE *port_priv = NULL;

    if ((width > MAX_SUPPORT_WIDTH) || (height > MAX_SUPPORT_HEIGHT))
    {
        DEBUG_PRINT_ERROR("%s: picture w/h(%ldx%ld) exceed! thred: width(%d), height(%d)\n", __func__, width, height, MAX_SUPPORT_WIDTH, MAX_SUPPORT_HEIGHT);
        return OMX_ErrorUnsupportedSetting;
    }

    pcomp_priv->pic_info.stride             = stride;
    pcomp_priv->pic_info.scan_lines         = height;

    pcomp_priv->pic_info.frame_width     = width;
    pcomp_priv->pic_info.frame_height    = height;

    port_priv = &pcomp_priv->m_port[OUTPUT_PORT_INDEX];

    port_priv->port_pro.buffer_size      = DEFAULT_FRAME_SIZE(stride, height);

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE get_supported_profile_level(OMX_COMPONENT_PRIVATE *pcomp_priv, OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType)
{
    OMX_U32 max_num;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    const struct codec_profile_level *pinfo = NULL;

    OMX_CHECK_ARG_RETURN(pcomp_priv == NULL);
    OMX_CHECK_ARG_RETURN(profileLevelType == NULL);

    if (profileLevelType->nPortIndex != INPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("%s: should be queries on Input port only (%ld)\n", __func__, profileLevelType->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    /* CodecType Relative */
    /* FIXME : profile & level may not correct! */
    if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_AVC, sizeof(OMX_ROLE_VIDEO_DECODER_AVC)))
    {
        pinfo = avc_profile_level_list;
        max_num = COUNTOF(avc_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_VC1, sizeof(OMX_ROLE_VIDEO_DECODER_VC1)))
    {
        pinfo = vc1_profile_level_list;
        max_num = COUNTOF(vc1_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_MPEG4, sizeof(OMX_ROLE_VIDEO_DECODER_MPEG4)))
    {
        pinfo = mpeg4_profile_level_list;
        max_num = COUNTOF(mpeg4_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_MPEG1, sizeof(OMX_ROLE_VIDEO_DECODER_MPEG1)))
    {
        pinfo = mpeg1_profile_level_list;
        max_num = COUNTOF(mpeg1_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_MPEG2, sizeof(OMX_ROLE_VIDEO_DECODER_MPEG2)))
    {
        pinfo = mpeg2_profile_level_list;
        max_num = COUNTOF(mpeg2_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_H263, sizeof(OMX_ROLE_VIDEO_DECODER_H263)))
    {
        pinfo = h263_profile_level_list;
        max_num = COUNTOF(h263_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_DIVX3, sizeof(OMX_ROLE_VIDEO_DECODER_DIVX3)))
    {
        pinfo = divx3_profile_level_list;
        max_num = COUNTOF(divx3_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_VP8, sizeof(OMX_ROLE_VIDEO_DECODER_VP8)))
    {
        pinfo = vp8_profile_level_list;
        max_num = COUNTOF(vp8_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_VP6, sizeof(OMX_ROLE_VIDEO_DECODER_VP6)))
    {
        pinfo = vp6_profile_level_list;
        max_num = COUNTOF(vp6_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_WMV, sizeof(OMX_ROLE_VIDEO_DECODER_WMV)))
    {
        pinfo = wmv_profile_level_list;
        max_num = COUNTOF(wmv_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_AVS, sizeof(OMX_ROLE_VIDEO_DECODER_AVS)))
    {
        pinfo = avs_profile_level_list;
        max_num = COUNTOF(avs_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_SORENSON, sizeof(OMX_ROLE_VIDEO_DECODER_SORENSON)))
    {
        pinfo = sorenson_profile_level_list;
        max_num = COUNTOF(sorenson_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_RV, sizeof(OMX_ROLE_VIDEO_DECODER_RV)))
    {
        pinfo = rv_profile_level_list;
        max_num = COUNTOF(rv_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_HEVC, sizeof(OMX_ROLE_VIDEO_DECODER_HEVC)))
    {
        pinfo = hevc_profile_level_list;
        max_num = COUNTOF(hevc_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_DECODER_MVC, sizeof(OMX_ROLE_VIDEO_DECODER_MVC)))
    {
        pinfo = mvc_profile_level_list;
        max_num = COUNTOF(mvc_profile_level_list);
    }
    else
    {
        DEBUG_PRINT_ERROR("%s: no profile & level found for %ld(%s)\n", __func__, profileLevelType->nPortIndex, (OMX_STRING)pcomp_priv->m_role);
        return OMX_ErrorUndefined;
    }

    if (profileLevelType->nIndex >= max_num)
    {
        DEBUG_PRINT_ERROR("%s: ProfileIndex(%ld) exceed!\n", __func__, profileLevelType->nIndex);
        return OMX_ErrorNoMore;
    }

    pinfo += profileLevelType->nIndex;
    profileLevelType->eProfile = pinfo->profile;
    profileLevelType->eLevel   = pinfo->level;

    return ret;
}


static OMX_ERRORTYPE empty_this_buffer_proxy(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_IN OMX_BUFFERHEADERTYPE *pomx_buf)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    OMXVDEC_BUF_DESC *puser_buf = NULL;

    OMX_PORT_PRIVATE *port_priv = NULL;

    if (!pcom_priv || !pomx_buf)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: bad parameters\n");
        return OMX_ErrorBadParameter;
    }

    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

    if ((port_priv->m_port_flushing == OMX_TRUE) || (port_priv->m_port_reconfig == OMX_TRUE))
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: port flushing or reconfig\n");
#if 0
        ret = OMX_ErrorIncorrectStateOperation;
        goto empty_error;
#endif
        pomx_buf->nFilledLen = 0;
        post_event(pcom_priv, (OMX_U32)pomx_buf, VDEC_S_SUCCESS, OMX_GENERATE_EBD);
        return OMX_ErrorNone;
    }

    puser_buf = (OMXVDEC_BUF_DESC *)pomx_buf->pInputPortPrivate;
    if (!puser_buf)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: invalid user buffer!\n");
        ret =  OMX_ErrorUndefined;
        goto empty_error;
    }

    puser_buf->data_len    = pomx_buf->nFilledLen;
    puser_buf->data_offset  = pomx_buf->nOffset;
    puser_buf->timestamp    = (OMX_S64)(pomx_buf->nTimeStamp);
    puser_buf->flags        = pomx_buf->nFlags;

    if (channel_submit_stream(&pcom_priv->drv_ctx, puser_buf) < 0)
    {
        /*Generate an async error and move to invalid state*/
        DEBUG_PRINT_ERROR("[ETB] ERROR: submit stream failed\n");
        ret = OMX_ErrorUndefined;
        goto empty_error;
    }

    //DEBUG_PRINT_STREAM("[ETB] success >>> phyaddr = %x, data_len = %d\n", puser_buf->phyaddr, puser_buf->data_len);

    return OMX_ErrorNone;

empty_error:
    post_event(pcom_priv, (OMX_U32 )pomx_buf, VDEC_S_FAILED, OMX_GENERATE_EBD);

    return ret;
}


static OMX_ERRORTYPE  fill_this_buffer_proxy(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_IN OMX_BUFFERHEADERTYPE* pomx_buf)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMXVDEC_BUF_DESC *puser_buf = NULL;

    if (!pcom_priv || !pomx_buf)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: bad parameter\n");
        return OMX_ErrorBadParameter;
    }

    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

    if ((port_priv->m_port_flushing == OMX_TRUE) || (port_priv->m_port_reconfig == OMX_TRUE))
    {
        pomx_buf->nFilledLen = 0;
        post_event(pcom_priv, (OMX_U32)pomx_buf, VDEC_S_SUCCESS, OMX_GENERATE_FBD);
        return OMX_ErrorNone;
    }

    puser_buf = (OMXVDEC_BUF_DESC *)pomx_buf->pOutputPortPrivate;
    if (!puser_buf)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: invalid user buf\n");
        ret = OMX_ErrorUndefined;
        goto fill_error;
    }

    puser_buf->data_len    = 0;
    puser_buf->data_offset    = pomx_buf->nOffset;

    if (channel_submit_frame(&pcom_priv->drv_ctx, puser_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: submit frame failed\n");
        return OMX_ErrorHardware;
    }

    //DEBUG_PRINT_STREAM("[FTB] success >>> phyaddr = %x, useraddr = %p\n", puser_buf->phyaddr, puser_buf->bufferaddr);

    return OMX_ErrorNone;

fill_error:
    post_event(pcom_priv, (OMX_U32 )pomx_buf, VDEC_S_FAILED, OMX_GENERATE_FBD);

    return ret;
}

#if (1 == DEBUG_SAVE_YUV)
static OMX_S8 save_this_frame(OMX_COMPONENT_PRIVATE *pcom_priv, OMXVDEC_BUF_DESC *puser_buf)
{
    OMX_U8  *Yaddress;
    OMX_U8  *Caddress;
    OMX_U8   ret                = 0;
    FILE**   ppstYuvFp          = NULL;
    OMX_U8** ppstChroml         = NULL;
    OMX_U32* pChromlSize        = NULL;
    OMXVDEC_FRAME_S* pstFrame  = NULL;
    SaveYuvOption stOption;
#ifdef HI_OMX_TEE_SUPPORT
#ifdef DEBUG_SAVE_YUV
    HI_MMZ_BUF_S stExchangeBuffer;
#endif
#endif

    if (NULL == pcom_priv || NULL == puser_buf)
    {
        DEBUG_PRINT_ERROR("%s INVALID PARAM\n", __func__);
        return -1;
    }

    pstFrame    = &puser_buf->out_frame;
    ppstYuvFp   = (FILE**)(&pcom_priv->drv_ctx.yuv_fp);
    ppstChroml  = &pcom_priv->drv_ctx.chrom_l;
    pChromlSize = (OMX_U32*)(&pcom_priv->drv_ctx.chrom_l_size);

    if (HI_FALSE == pstFrame->save_yuv)
    {
        if(*ppstYuvFp)
        {
            fclose(*ppstYuvFp);
            *ppstYuvFp = NULL;
        }

        if (*ppstChroml)
        {
            free(*ppstChroml);
            *ppstChroml = pcom_priv->drv_ctx.chrom_l = NULL;
            *pChromlSize  = 0;
        }
        return 0;
    }

    if (NULL == (*ppstYuvFp))
    {
        *ppstYuvFp = fopen(pstFrame->save_path, "w");
        if(!(*ppstYuvFp))
        {
            DEBUG_PRINT_ERROR("%s open %s file failed\n", __func__, pstFrame->save_path);
            goto error;
        }
    }

    if (NULL == (*ppstChroml))
    {
        *ppstChroml = (OMX_U8 *)malloc(pstFrame->width*pstFrame->height);
        if (!(*ppstChroml))
        {
            DEBUG_PRINT_ERROR("%s malloc Chroml(%d) failed\n", __func__, pstFrame->width*pstFrame->height);
            goto error1;
        }
        *pChromlSize = pstFrame->width*pstFrame->height;
    }
    else  // 防止变分辨率内存越界// MARK
    {
        if (pstFrame->width*pstFrame->height > *pChromlSize)
        {
            free(*ppstChroml);
            *ppstChroml = pcom_priv->drv_ctx.chrom_l = NULL;
            *pChromlSize = 0;
            *ppstChroml = (OMX_U8 *)malloc(pstFrame->width*pstFrame->height);
            if (!(*ppstChroml))
            {
                DEBUG_PRINT_ERROR("%s malloc Chroml(%d) failed\n", __func__, pstFrame->width*pstFrame->height);
                goto error1;
            }
            *pChromlSize = pstFrame->width*pstFrame->height;
        }
    }

    /* start to save this frame */
#ifdef HI_OMX_TEE_SUPPORT
#ifdef DEBUG_SAVE_YUV
    memset(&stExchangeBuffer, 0, sizeof(HI_MMZ_BUF_S));
    stExchangeBuffer.bufsize = puser_buf->data_len;

    ret = yuv_secure_mem_exchange(puser_buf->phyaddr, &stExchangeBuffer);
    if (ret != HI_SUCCESS)
    {
        DEBUG_PRINT_ERROR("Get yuv secure exchange mem failed!\n");
        goto error2;
    }
    Yaddress = stExchangeBuffer.user_viraddr;
#endif
#else

    Yaddress = (OMX_U8 *)puser_buf->bufferaddr;
#endif

    Caddress = Yaddress + (pstFrame->phyaddr_C - pstFrame->phyaddr_Y);

    stOption.ColorFormat = pcom_priv->drv_ctx.drv_cfg.cfg_color_format;
    stOption.pstChroml   = *ppstChroml;
    stOption.pYaddress   = Yaddress;
    stOption.pCaddress   = Caddress;
    stOption.Width       = pstFrame->width;
    stOption.Height      = pstFrame->height;
    stOption.Stride      = pstFrame->stride;

    ret = channel_save_yuv(*ppstYuvFp, &stOption);

#ifdef HI_OMX_TEE_SUPPORT
#ifdef DEBUG_SAVE_YUV
    yuv_exchange_mem_free(&stExchangeBuffer);
#endif
#endif

    if (ret != 0)
    {
        DEBUG_PRINT_ERROR("%s save this frame failed!\n", __func__);
        goto error2;
    }

    DEBUG_PRINT_ALWS(">> Save one Frame(%dx%d) in %s\n\n", pstFrame->width, pstFrame->height, pstFrame->save_path);

    return 0;

error2:
    free(*ppstChroml);
    *ppstChroml  = pcom_priv->drv_ctx.chrom_l = NULL;
    *pChromlSize = 0;

error1:
    fclose(*ppstYuvFp);
    *ppstYuvFp = NULL;

error:
    return -1;
}
#endif

static OMX_S32 message_process (OMX_COMPONENT_PRIVATE  *pcom_priv, void* message)
{
    OMX_U32 i = 0;
    OMX_U32 width = 0, height = 0, stride = 0;

    OMXVDEC_IMG_SIZE *pimg = NULL;
    OMXVDEC_MSG_INFO *vdec_msg = NULL;
    OMXVDEC_BUF_DESC *puser_buf = NULL;

    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_BUFFERHEADERTYPE *pomx_buf = NULL;

    //HI_LD_Event_S LdEvent;

    if (!pcom_priv || !message)
    {
        DEBUG_PRINT_ERROR("[MP] ERROR: invalid param\n");
        return -1;
    }

    vdec_msg = (OMXVDEC_MSG_INFO *)message;
    switch (vdec_msg->msgcode)
    {
        case VDEC_MSG_RESP_FLUSH_INPUT_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_FLUSH_INPUT_DONE);
            break;

        case VDEC_MSG_RESP_FLUSH_OUTPUT_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_FLUSH_OUTPUT_DONE);
            break;

        case VDEC_MSG_RESP_START_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_START_DONE);
            break;

        case VDEC_MSG_RESP_STOP_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_STOP_DONE);
            break;

        case VDEC_MSG_RESP_PAUSE_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_PAUSE_DONE);
            break;

        case VDEC_MSG_RESP_RESUME_DONE:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_RESUME_DONE);
            break;

        case VDEC_MSG_RESP_INPUT_DONE:
            puser_buf = &vdec_msg->msgdata.buf;
            port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
            if (HI_NULL == puser_buf->client_data)
            {
                DEBUG_PRINT_ERROR("[MP] ERROR: resp input buffer invalid\n");
                post_event(pcom_priv, 0, VDEC_S_FAILED, OMX_GENERATE_EBD);
                break;
            }

            pomx_buf = (OMX_BUFFERHEADERTYPE *)puser_buf->client_data;
            for (i = 0; i < port_priv->port_pro.max_count; i++)
            {
                if (pomx_buf == port_priv->m_omx_bufhead[i])
                {
                    break;
                }
            }

            if (i >= port_priv->port_pro.max_count)
            {
                DEBUG_PRINT_ERROR("[MP] ERROR: buffers[%p] not found", pomx_buf);
                post_event(pcom_priv, 0, VDEC_S_FAILED, OMX_GENERATE_EBD);
                break;
            }

            //DEBUG_PRINT_STREAM("[EBD] post ||| phyaddr = %x, useraddr = %p\n", puser_buf->phyaddr, pomx_buf->pBuffer);

            post_event(pcom_priv, (OMX_U32)pomx_buf, VDEC_S_SUCCESS, OMX_GENERATE_EBD);
            break;

        case VDEC_MSG_RESP_OUTPUT_DONE:
            puser_buf = &vdec_msg->msgdata.buf;
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

            if (port_priv->m_port_flushing)
            {
                puser_buf->data_len = 0;
            }

            if (puser_buf->data_len != 0)
            {
              #if (1 == DEBUG_SAVE_YUV)
                save_this_frame(pcom_priv, puser_buf);
            #endif
            }

            if (HI_NULL == puser_buf->client_data)
            {
                DEBUG_PRINT_ERROR("[MP] ERROR: resp output buffer invalid\n");
                post_event(pcom_priv, 0, VDEC_S_FAILED, OMX_GENERATE_FBD);
                break;
            }


            pcom_priv->m_frame_rate = puser_buf->u32FrameRate;

            //DEBUG_PRINT_ERROR("[MP] frame rate:%d\n", pcom_priv->m_frame_rate);

            pomx_buf = (OMX_BUFFERHEADERTYPE *)puser_buf->client_data;

            /* check buffer validate*/
            for (i = 0; i < port_priv->port_pro.max_count; i++)
            {
                if (pomx_buf == port_priv->m_omx_bufhead[i])
                {
                    break;
                }
            }

            if (i >= port_priv->port_pro.max_count)
            {
                DEBUG_PRINT_ERROR("[MP] ERROR: no buffers found for address[%p]", pomx_buf);
                post_event(pcom_priv, 0, VDEC_S_FAILED, OMX_GENERATE_FBD);
                break;
            }

            pomx_buf->nFilledLen = puser_buf->data_len;
            pomx_buf->nFlags     = puser_buf->flags;

            OMX_S64 interval = (OMX_S64)(1e9 / pcom_priv->m_frame_rate);
            if (puser_buf->timestamp < 0)
            {
                pomx_buf->nTimeStamp = pcom_priv->m_pre_timestamp + interval;
            }
            else
            {
                pomx_buf->nTimeStamp = (OMX_TICKS)(puser_buf->timestamp);
                /*if (pcom_priv->m_pre_timestamp == pomx_buf->nTimeStamp
                 && puser_buf->flags != 0)
                {
                    pomx_buf->nTimeStamp += interval;
                }*/
            }

            /*if (puser_buf->data_len != 0)
            {
                DEBUG_PRINT_ALWS("Out nTimeStamp=%lld, timestamp=%lld, m_pre_timestamp=%lld\n",
                                  pomx_buf->nTimeStamp, puser_buf->timestamp, pcom_priv->m_pre_timestamp);
            }*/

            if (0 == pomx_buf->nTimeStamp && 0 == puser_buf->flags)
            {
                pcom_priv->m_pre_timestamp = -1;
            }
            else
            {
                pcom_priv->m_pre_timestamp = pomx_buf->nTimeStamp;
            }

            //DEBUG_PRINT_STREAM("[FBD] post ||| phyaddr = %x, useraddr = %p, data_len = %ld\n", puser_buf->phyaddr, pomx_buf->pBuffer, pomx_buf->nFilledLen);

            /* //for low delay time proc
            LdEvent.evt_id = EVENT_AVPLAY_FRM_IN;
            LdEvent.frame = puser_buf->u32FrameIndex;
            LdEvent.handle = puser_buf->hTunnelSrc;
            (HI_VOID)HI_SYS_GetTimeStampMs(&(LdEvent.time));
            (HI_VOID)HI_MPI_STAT_NotifyLowDelayEvent(&LdEvent);
            */

            post_event(pcom_priv, (OMX_U32)pomx_buf, VDEC_S_SUCCESS, OMX_GENERATE_FBD);

            if (pomx_buf->nFlags & OMX_BUFFERFLAG_EOS)
            {
                post_event(pcom_priv, 0, 0, OMX_GENERATE_EOS_DONE);
            }
            break;

        case VDEC_EVT_REPORT_IMG_SIZE_CHG:
            pimg   = &vdec_msg->msgdata.img_size;
            width  = (OMX_U32)pimg->frame_width;
            height = (OMX_U32)pimg->frame_height;
            stride = (OMX_U32)pimg->frame_stride;

            if (width != pcom_priv->pic_info.frame_width || height != pcom_priv->pic_info.frame_height ||stride != pcom_priv->pic_info.stride)
            {
                /*DEBUG_PRINT("image size changed: old_w: %d, old_h:%d, new_w:%d, new_h:%d\n",
                        (int)pcom_priv->pic_info.frame_width,
                        (int)pcom_priv->pic_info.frame_height,
                        (int)width, (int)height);*/

                update_picture_info(pcom_priv, width, height, stride);

                post_event(pcom_priv, 0, 0, OMX_GENERATE_IMAGE_SIZE_CHANGE);
            }
            break;

        case VDEC_EVT_REPORT_HW_ERROR:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_HARDWARE_ERROR);
            break;

        case VDEC_ERR_DYNAMIC_ALLOC_FAIL:
            post_event(pcom_priv, 0, vdec_msg->status_code, OMX_GENERATE_DYNAMIC_ALLOC_ERROR);
            break;

        default:
            DEBUG_PRINT_ERROR("[MP] ERROR: msg 0x%08x not process\n", vdec_msg->msgcode);
            break;
    }

    return 0;
}


static OMX_PTR message_thread(OMX_PTR input)
{
    OMX_S32 ret = 0;
    OMXVDEC_MSG_INFO msginfo;
    OMX_COMPONENT_PRIVATE *pcom_priv = (OMX_COMPONENT_PRIVATE *)input;

    while (!pcom_priv->msg_thread_exit)
    {
        ret = channel_get_msg(&pcom_priv->drv_ctx, &msginfo);
        if (HI_SUCCESS != ret)
        {
            if (EAGAIN == errno)
            {
                continue;
            }
            else
            {
                DEBUG_PRINT_WARN("No more message, return %s.\n", ((strerror(errno)!=NULL)? strerror(errno):"nothing"));
                break;
            }
        }
        message_process(pcom_priv, &msginfo);
    }

    return NULL;
}


static OMX_ERRORTYPE generate_command_done(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_U32  param1,
       OMX_U32  param2 )
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(pcom_priv->m_cb.EventHandler == NULL);

    switch (param1)
    {
        case OMX_CommandStateSet:
            pcom_priv->m_state = (OMX_STATETYPE)param2;
            ret = omx_report_event(pcom_priv, OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandPortEnable:
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            if (port_priv->m_port_reconfig)
            {
                port_priv->m_port_reconfig = OMX_FALSE;
            }

            ret = omx_report_event(pcom_priv, OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandPortDisable:
            ret = omx_report_event(pcom_priv, OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandFlush:
        default:
            DEBUG_PRINT_ERROR("unknown genereate event 0x%ld\n", param1);
            ret = OMX_ErrorUndefined;
            break;
    }

    return ret;
}


static OMX_ERRORTYPE handle_command_state_set(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_STATETYPE state,
       OMX_U32 *sem_posted)
{
    OMX_U32 flag = 1;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    //DEBUG_PRINT_STATE("[OmxState] Current State %d, Expected State %d\n", pcom_priv->m_state, state);

    /***************************/
    /* Current State is Loaded */
    /***************************/
    if (pcom_priv->m_state == OMX_StateLoaded)
    {
        /*  Loaded to idle */
        if( state == OMX_StateIdle)
        {
            if (-1 == pcom_priv->drv_ctx.chan_handle)
            {
                if (channel_create(&pcom_priv->drv_ctx) < 0)
                {
                    DEBUG_PRINT_ERROR("[OmxState] ERROR: channel create failed!\n");
                    ret = OMX_ErrorHardware;
                    goto event_post;
                }
            }

            if (OMX_FALSE == pcom_priv->msg_thread_id)
            {
                if (pthread_create(&pcom_priv->msg_thread_id, 0, message_thread, pcom_priv) < 0)
                {
                    DEBUG_PRINT_ERROR("[OmxState] ERROR: failed to create msg thread\n");
                    ret = OMX_ErrorInsufficientResources;
                    goto event_post;
                }
            }

            if (ports_all_full(pcom_priv) ||
                          (pcom_priv->m_port[0].m_port_enabled == OMX_FALSE &&  pcom_priv->m_port[1].m_port_enabled  == OMX_FALSE))
            {
                DEBUG_PRINT_STATE("[OmxState] Loaded --> Idle\n");
            }
            else
            {
                DEBUG_PRINT_STATE("[OmxState] Loaded --> Idle_Pending\n");
                bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
                flag = 0;
            }
        }
        /*  Loaded to Loaded */
        else if (state == OMX_StateLoaded)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Loaded --> Loaded\n");
            ret = OMX_ErrorSameState;
        }
        /*  Loaded to WaitForResources */
        else if (state == OMX_StateWaitForResources)
        {
            DEBUG_PRINT_STATE("[OmxState] Loaded --> WaitForResources\n");
            ret = OMX_ErrorNone;
        }
        /*  Loaded to Executing */
        else if (state == OMX_StateExecuting)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR:Loaded --> Executing\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Loaded to Pause */
        else if (state == OMX_StatePause)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Loaded --> Pause\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        else
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Loaded --> %d Not Handled)\n", state);
            ret = OMX_ErrorUndefined;
        }
    }
    /***************************/
    /* Current State is IDLE */
    /***************************/
    else if (pcom_priv->m_state == OMX_StateIdle)
    {
        /*  Idle to Loaded */
        if (state == OMX_StateLoaded)
        {
            if (ports_all_empty(pcom_priv))
            {
                DEBUG_PRINT_STATE("[OmxState] Idle --> Loaded\n");
            }
            else
            {
                DEBUG_PRINT_STATE("[OmxState] Idle --> Loaded_Pending\n");
                bit_set(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING);
                flag = 0;
            }
        }
        /*  Idle to Executing */
        else if (state == OMX_StateExecuting)
        {
            DEBUG_PRINT_STATE("[OmxState] Idle --> Executing_Pending\n");
            if (channel_port_enable(&pcom_priv->drv_ctx, pcom_priv->m_port[OUTPUT_PORT_INDEX].m_port_enabled) < 0)
            {
                DEBUG_PRINT_ERROR("[OmxState] ERROR: channel_port_enable failed\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }
            if (channel_start(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("[OmxState] ERROR: Channel start failed\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }
            // post event after vdec response.
            bit_set(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING);
            flag = 0;
        }
        /*  Idle to Idle */
        else if (state == OMX_StateIdle)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Idle --> Idle\n");
            ret = OMX_ErrorSameState;
        }
        /*  Idle to WaitForResources */
        else if (state == OMX_StateWaitForResources)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Idle --> WaitForResources\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Idle to Pause */
        else if (state == OMX_StatePause)
        {
            DEBUG_PRINT_STATE("[OmxState] Idle --> Pause\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING);
            flag = 0;
        }
        else
        {
            DEBUG_PRINT_ERROR("[OmxState] ERROR: Idle --> %d Not Handled\n", state);
            ret = OMX_ErrorBadParameter;
        }
    }
    /******************************/
    /* Current State is Executing */
    /******************************/
    else if(pcom_priv->m_state == OMX_StateExecuting)
    {
        /*  Executing to Idle */
        if (state == OMX_StateIdle)
        {
            if (channel_stop(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("[OmxState] ERROR: chan stop failed!\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }

            DEBUG_PRINT_STATE("[OmxState] Executing --> Idle_Pending\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
            flag = 0;

            if (!*sem_posted)
            {
                *sem_posted = 1;
                sem_post(&pcom_priv->m_cmd_lock);
                omx_flush_port(pcom_priv, OMX_ALL);
            }
        }
        /*  Executing to Paused */
        else if (state == OMX_StatePause)
        {
            if (channel_pause(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("[OmxState] ERROR: channel_pause failed!\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }

            DEBUG_PRINT_STATE("[OmxState] Executing --> Pause_Pending\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING);
            flag = 0;

        }
        /*  Executing to Loaded */
        else if (state == OMX_StateLoaded)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Executing --> Loaded\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Executing to WaitForResources */
        else if (state == OMX_StateWaitForResources)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Executing --> WaitForResources\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Executing to Executing */
        else if (state == OMX_StateExecuting)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Executing --> Executing\n");
            ret = OMX_ErrorSameState;
        }
        else
        {
            DEBUG_PRINT_ERROR("[OmxState] ERROR: Executing -->%d Not Handled\n", state);
            ret = OMX_ErrorBadParameter;
        }
    }
    /***************************/
    /* Current State is Pause  */
    /***************************/
    else if (pcom_priv->m_state == OMX_StatePause)
    {
        /*  Pause to Executing */
        if (state == OMX_StateExecuting)
        {
            if (channel_resume(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_STATE("[OmxState] ERROR: channel_resume failed!\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }

            DEBUG_PRINT_STATE("[OmxState] Pause --> Executing_Pending\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING);
            flag = 0;
        }
        /*  Pause to Idle */
        else if (state == OMX_StateIdle)
        {
            if (channel_stop(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("[OmxState] ERROR: channel_stop failed!\n");
                ret = OMX_ErrorHardware;
                goto event_post;
            }

            DEBUG_PRINT_STATE("[OmxState] Pause --> Idle\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
            flag = 0;

            if (!*sem_posted)
            {
                *sem_posted = 1;
                sem_post(&pcom_priv->m_cmd_lock);
                omx_flush_port(pcom_priv, OMX_ALL);
            }

        }
        /*  Pause to loaded */
        else if (state == OMX_StateLoaded)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Pause --> Loaded\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Pause to WaitForResources */
        else if (state == OMX_StateWaitForResources)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Pause --> WaitForResources\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  Pause to Pause */
        else if (state == OMX_StatePause)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Pause --> Pause\n");
            ret = OMX_ErrorSameState;
        }
        else
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: Paused -->%d Not Handled\n",state);
            ret = OMX_ErrorBadParameter;
        }
    }
    /***************************/
    /* Current State is WaitForResources  */
    /***************************/
    else if (pcom_priv->m_state == OMX_StateWaitForResources)
    {
        /*  WaitForResources to Loaded */
        if (state == OMX_StateLoaded)
        {
            DEBUG_PRINT_STATE("[OmxState] WaitForResources --> Loaded\n");
        }
        /*  WaitForResources to WaitForResources */
        else if (state == OMX_StateWaitForResources)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: WaitForResources --> WaitForResources\n");
            ret = OMX_ErrorSameState;
        }
        /*  WaitForResources to Executing */
        else if (state == OMX_StateExecuting)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: WaitForResources --> Executing\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  WaitForResources to Pause */
        else if (state == OMX_StatePause)
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: WaitForResources --> Pause\n");
            ret = OMX_ErrorIncorrectStateTransition;
        }
        /*  WaitForResources to Loaded -
           is NOT tested by Khronos TS */
        else
        {
            DEBUG_PRINT_STATE("[OmxState] ERROR: %d --> %d(NotHandled)\n", pcom_priv->m_state, state);
            ret = OMX_ErrorBadParameter;
        }
    }

event_post:
    if (flag)
    {
        if (ret != OMX_ErrorNone)
        {
            omx_report_error(pcom_priv, ret);
        }
        else
        {
            post_event(pcom_priv, OMX_CommandStateSet, state, OMX_GENERATE_COMMAND_DONE);
        }
    }

    return ret;
}


static OMX_ERRORTYPE handle_command_flush(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_U32 port)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;

    if ((port != OMX_ALL) && (port > OUTPUT_PORT_INDEX))
    {
        DEBUG_PRINT_ERROR("[CmdFlush] Error: Invalid Port %d\n", (int)port);
        return OMX_ErrorBadPortIndex;
    }

    if ((INPUT_PORT_INDEX == port) || (OMX_ALL == port))
    {
        bit_set(&pcom_priv->m_flags, OMX_STATE_INPUT_FLUSH_PENDING);
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }
    if ((OUTPUT_PORT_INDEX == port) || (OMX_ALL == port))
    {
        bit_set(&pcom_priv->m_flags, OMX_STATE_OUTPUT_FLUSH_PENDING);
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (channel_flush_port(&pcom_priv->drv_ctx, port) < 0)
    {
        DEBUG_PRINT_ERROR("[CmdFlush] channel_flush_port failed\n");
        ret = OMX_ErrorHardware;
        /*fixme: should clear flushing pending bits?!!*/
        omx_report_error(pcom_priv, ret);
    }

    return ret;
}


static OMX_ERRORTYPE handle_command_port_disable(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_U32 port,
       OMX_U32 *sem_posted)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_U32 flag = 1;

    if((port == INPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_enabled = OMX_FALSE;

            if (!port_empty(pcom_priv, INPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Input Port Disable --> Enable_Pending!\n");
                bit_set(&pcom_priv->m_flags, OMX_STATE_INPUT_DISABLE_PENDING);

                if((pcom_priv->m_state == OMX_StatePause) || (pcom_priv->m_state == OMX_StateExecuting))
                {
                    if(!*sem_posted)
                    {
                        *sem_posted = 1;
                        sem_post(&pcom_priv->m_cmd_lock);
                    }
                    ret = omx_flush_port(pcom_priv, INPUT_PORT_INDEX);
                }
                flag = 0;
            }
        }
        else
        {
            ret = OMX_ErrorNone;
        }

        if ((ret == OMX_ErrorNone) && flag)
        {
            post_event(pcom_priv, OMX_CommandPortDisable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    flag = 1;
    if((port == OUTPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_enabled = OMX_FALSE;

            if (!port_empty(pcom_priv, OUTPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Output Port Enabled --> Disable_Pending\n");

                bit_set(&pcom_priv->m_flags, OMX_STATE_OUTPUT_DISABLE_PENDING);

                if((pcom_priv->m_state == OMX_StatePause) || (pcom_priv->m_state == OMX_StateExecuting))
                {
                    if (!*sem_posted)
                    {
                        *sem_posted = 1;
                        sem_post(&pcom_priv->m_cmd_lock);
                    }
                    ret = omx_flush_port(pcom_priv, OUTPUT_PORT_INDEX);
                }
                flag = 0;
            }
        }
        else
        {
            ret = OMX_ErrorNone;
        }

        if ((ret == OMX_ErrorNone) && flag)
        {
            post_event(pcom_priv, OMX_CommandPortDisable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }

    }

    return ret;
}


static OMX_ERRORTYPE handle_command_port_enable(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_U32 port)
{
    OMX_U32 flag = 1;
    OMX_S32 ret = -1;
    OMX_PORT_PRIVATE *port_priv = NULL;

    if ((port == INPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        if (!port_priv->m_port_enabled)
        {
            port_priv->m_port_enabled = OMX_TRUE;

            if (!port_full(pcom_priv, INPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Input Port Disabled --> Enabled_Pending\n");
                bit_set(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING);
                flag = 0;
            }
        }
        else
        {
            //DEBUG_PRINT("Inport already enabled\n");
        }

        if(flag)
        {
            post_event(pcom_priv, OMX_CommandPortEnable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    flag = 1;
    if ((port == OUTPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        if (!port_priv->m_port_enabled)
        {
            port_priv->m_port_enabled = OMX_TRUE;

            if (!port_full(pcom_priv, OUTPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Output Port Disabled --> Enabled_Pending\n");
                bit_set(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING);
                flag = 0;
            }
        }
        else
        {
            //DEBUG_PRINT("Output port already enabled\n");
        }

        if(flag)
        {
            ret = channel_port_enable(&pcom_priv->drv_ctx, port_priv->m_port_enabled);
            if (ret != OMX_ErrorNone)
            {
               DEBUG_PRINT_ERROR("call channel_port_enable return failed!\n");
            }
            else
            {
               post_event(pcom_priv, OMX_CommandPortEnable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
            }
        }
    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE  send_command_proxy(
       OMX_COMPONENT_PRIVATE *pcom_priv,
       OMX_IN OMX_COMMANDTYPE cmd,
       OMX_IN OMX_U32 param1)
{
    OMX_U32 sem_posted = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);

    switch (cmd)
    {
        case OMX_CommandStateSet:
            ret = handle_command_state_set(pcom_priv, (OMX_STATETYPE)param1, &sem_posted);
            break;

        case OMX_CommandFlush:
            ret = handle_command_flush(pcom_priv, param1);
            break;

        case OMX_CommandPortEnable:
            ret = handle_command_port_enable(pcom_priv, param1);
            break;

        case OMX_CommandPortDisable:
            ret = handle_command_port_disable(pcom_priv, param1, &sem_posted);
            break;

        default:
            DEBUG_PRINT_ERROR("%s Error: Invalid Command(%d)\n", __func__, cmd);
            ret = OMX_ErrorNotImplemented;
            break;
    }

    if(!sem_posted)
    {
        sem_post(&pcom_priv->m_cmd_lock);
    }

    return ret;
}


static void event_process(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id)
{
    OMX_U32  qsize = 0;
    OMX_U32  p1 = 0, p2 = 0, ident = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;

    if (!pcom_priv || (id >= OMX_GENERATE_UNUSED))
    {
        DEBUG_PRINT_ERROR("[EP] ERROR: invalid param!\n");
        return;
    }

    /* process event from cmd/etb/ftb queue */
    pthread_mutex_lock(&pcom_priv->m_lock);

    if ((id == OMX_GENERATE_FTB) || (id == OMX_GENERATE_FBD))
    {
        qsize = get_q_size(&pcom_priv->m_ftb_q);
        if ((qsize == 0))// || (pcom_priv->m_state == OMX_StatePause))
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);
            return;
        }
        pop_entry(&pcom_priv->m_ftb_q, &p1, &p2, &ident);
    }
    else if ((id == OMX_GENERATE_ETB) || (id == OMX_GENERATE_EBD))
    {
        qsize = get_q_size(&pcom_priv->m_etb_q);
        if ((qsize == 0))// || (pcom_priv->m_state == OMX_StatePause))
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);
            return;
        }
        pop_entry(&pcom_priv->m_etb_q, &p1, &p2, &ident);
    }
    else
    {
        qsize = get_q_size(&pcom_priv->m_cmd_q);
        if (qsize == 0)
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);
            return;
        }
        pop_entry(&pcom_priv->m_cmd_q, &p1, &p2, &ident);
    }
    pthread_mutex_unlock(&pcom_priv->m_lock);

    if (id != ident)
    {
        DEBUG_PRINT_ERROR("[EP] ID not match, id %ld, ident %ld\n", id, ident);
        return;
    }

    /* event process instance */
    switch (id)
       {
        case OMX_GENERATE_COMMAND_DONE:
            (void)generate_command_done(pcom_priv, p1, p2);
            break;

        case OMX_GENERATE_ETB:
            (void)empty_this_buffer_proxy(pcom_priv, (OMX_BUFFERHEADERTYPE *)p1);
            break;

        case OMX_GENERATE_FTB:
            (void)fill_this_buffer_proxy(pcom_priv, (OMX_BUFFERHEADERTYPE *)p1);
            break;

        case OMX_GENERATE_COMMAND:
            (void)send_command_proxy(pcom_priv, (OMX_COMMANDTYPE)p1, (OMX_U32)p2);
            break;

        case OMX_GENERATE_EBD:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] OMX_GENERATE_EBD failed\n");
                omx_report_error(pcom_priv, OMX_ErrorUndefined);
                break;
            }

            ret = empty_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE *)p1);
            if (ret != OMX_ErrorNone)
            {
                DEBUG_PRINT_ERROR("[EP] empty_buffer_done failed\n");
                omx_report_error(pcom_priv, ret);
            }
            break;

        case OMX_GENERATE_FBD:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[FP] OMX_GENERATE_FBD failed\n");
                omx_report_error(pcom_priv, OMX_ErrorUndefined);
                break;
            }

            ret = fill_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE *)p1);
            if (ret != OMX_ErrorNone)
            {
                DEBUG_PRINT_ERROR("[FP] fill_buffer_done failed\n");
                omx_report_error (pcom_priv, ret);
            }
            break;

        case OMX_GENERATE_FLUSH_INPUT_DONE:
            port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
            if (!port_priv->m_port_flushing)
            {
                DEBUG_PRINT_WARN("WARNNING: Unexpected flush input done\n");
                break;
            }
            else
            {
                port_priv->m_port_flushing = OMX_FALSE;
            }

            return_inbuffers(pcom_priv);

            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] input flush failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            /*Check if we need generate event for Flush done*/
            if(bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_FLUSH_PENDING))
            {
                bit_clear (&pcom_priv->m_flags, OMX_STATE_INPUT_FLUSH_PENDING);
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandFlush, INPUT_PORT_INDEX, NULL);
            }

            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            if (!port_priv->m_port_flushing && bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING))
            {
                DEBUG_PRINT_STATE("[OmxState] Idle_Pending --> Idle\n");
                bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);
                pcom_priv->m_state = OMX_StateIdle;
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateIdle, NULL);
            }
            break;

        case OMX_GENERATE_FLUSH_OUTPUT_DONE:
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            if (!port_priv->m_port_flushing)
            {
                DEBUG_PRINT_WARN("WARNING: Unexpected flush output done\n");
                break;
            }
            else
            {
                port_priv->m_port_flushing = OMX_FALSE;
            }

            return_outbuffers(pcom_priv);

            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] output flush failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            /*Check if we need generate event for Disable-op/Flush done*/
            if(bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_FLUSH_PENDING))
            {
                bit_clear (&pcom_priv->m_flags, OMX_STATE_OUTPUT_FLUSH_PENDING);
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandFlush, OUTPUT_PORT_INDEX, NULL);
            }

            port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
            if (!port_priv->m_port_flushing && bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING))
            {
                DEBUG_PRINT_STATE("[OmxState] Idle_Pending --> Idle\n");
                bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);
                pcom_priv->m_state = OMX_StateIdle;
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateIdle, NULL);
            }
            break;

        case OMX_GENERATE_START_DONE:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] OMX_GENERATE_START_DONE failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            if(bit_present(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING))
            {
                DEBUG_PRINT_STATE("[OmxState] Execute_Pending --> Executing\n");
                bit_clear((&pcom_priv->m_flags), OMX_STATE_EXECUTE_PENDING);
                pcom_priv->m_state = OMX_StateExecuting;
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateExecuting, NULL);
            }

            if (bit_present(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING))
            {
                if (channel_pause(&pcom_priv->drv_ctx) < 0)
                {
                    DEBUG_PRINT_ERROR("[EP] channel_pause failed\n");
                    omx_report_error(pcom_priv, OMX_ErrorHardware);
                }
            }
            break;

        case OMX_GENERATE_PAUSE_DONE:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] OMX_GENERATE_PAUSE_DONE failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            if(bit_present(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING))
            {
                bit_clear(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING);
                pcom_priv->m_state = OMX_StatePause;
                DEBUG_PRINT_STATE("[OmxState] Pause_Pending --> Pause\n");
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StatePause, NULL);
            }
            break;

        case OMX_GENERATE_RESUME_DONE:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] OMX_GENERATE_RESUME_DONE failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            if (bit_present(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING))
            {
                DEBUG_PRINT_STATE("Moving to Executing\n");

                bit_clear((&pcom_priv->m_flags), OMX_STATE_EXECUTE_PENDING);
                pcom_priv->m_state = OMX_StateExecuting;
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateExecuting, NULL);
            }
            break;

        case OMX_GENERATE_STOP_DONE:
            if (p2 != VDEC_S_SUCCESS)
            {
                DEBUG_PRINT_ERROR("[EP] OMX_GENERATE_STOP_DONE failed\n");
                omx_report_error(pcom_priv, OMX_ErrorHardware);
                break;
            }

            return_outbuffers(pcom_priv);
            return_inbuffers(pcom_priv);

            if ( bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING)
              && !pcom_priv->m_port[INPUT_PORT_INDEX].m_port_flushing
              && !pcom_priv->m_port[OUTPUT_PORT_INDEX].m_port_flushing)
            {
                DEBUG_PRINT_STATE("[OmxState] Idle_Pending --> Idle\n");
                bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);
                pcom_priv->m_state = OMX_StateIdle;
                omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateIdle, NULL);
            }
            break;

        case OMX_GENERATE_EOS_DONE:
            omx_report_event(pcom_priv, OMX_EventBufferFlag, OUTPUT_PORT_INDEX, OMX_BUFFERFLAG_EOS, NULL);
            break;

        case OMX_GENERATE_IMAGE_SIZE_CHANGE:
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            port_priv->m_port_reconfig = OMX_TRUE;
            omx_report_event(pcom_priv, OMX_EventPortSettingsChanged, OUTPUT_PORT_INDEX, OMX_IndexParamPortDefinition, NULL);
            break;

        case OMX_GENERATE_HARDWARE_ERROR:
            DEBUG_PRINT_ERROR("[EP] HARDWARE ERROR\n");
            omx_report_error(pcom_priv, OMX_ErrorHardware);
            break;

        case OMX_GENERATE_DYNAMIC_ALLOC_ERROR:
            DEBUG_PRINT_ERROR("[EP] DynamicResourcesUnavailable ERROR\n");
            omx_report_error(pcom_priv, OMX_ErrorDynamicResourcesUnavailable);
            break;

        default:
            DEBUG_PRINT_ERROR("[EP] default process for msg %ld\n", id);
            break;
    }

}


static OMX_PTR event_thread(OMX_PTR input)
{
    OMX_S8 msg = 0;
    OMX_S16 n = -1, ret = -1;
    OMX_U8 id;
    OMX_COMPONENT_PRIVATE *pcom_priv = (OMX_COMPONENT_PRIVATE *)input;

    msg = 1;
    ret = write(pcom_priv->m_async_pipe[1],&msg, 1);
    if (ret < 0)
    {
        usleep(50000);
        close(pcom_priv->m_async_pipe[0]);
        close(pcom_priv->m_async_pipe[1]);
    }

    while (!pcom_priv->event_thread_exit)
    {
        n = read(pcom_priv->m_pipe_in, &id, 1);

        if (((n < 0) && (errno != EINTR)) || (n == 0))
        {
            DEBUG_PRINT_WARN("No more event, n=%d, return %s.\n", n, strerror(errno));
            break;
        }
        else if (n > 1)
        {
            DEBUG_PRINT_ERROR("read more than 1 byte\n");
            continue;
        }

        event_process(pcom_priv, id);
    }

    msg = 1;
    ret = write(pcom_priv->m_async_pipe[1],&msg, 1);
    if (ret < 0)
    {
        close(pcom_priv->m_async_pipe[0]);
        close(pcom_priv->m_async_pipe[1]);
    }

    return NULL;
}


static OMX_S32 ports_init(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    OMX_S32 result = -1;
    OMX_PORT_PRIVATE *in_port  = NULL;
    OMX_PORT_PRIVATE *out_port = NULL;

    /* init in port private */
    in_port = &pcom_priv->m_port[INPUT_PORT_INDEX];
    in_port->m_omx_bufhead = (OMX_BUFFERHEADERTYPE**)malloc(sizeof(OMX_BUFFERHEADERTYPE*)*DEF_MAX_IN_BUF_CNT);
    if (!in_port->m_omx_bufhead)
    {
        DEBUG_PRINT_ERROR("No enough memory for in port!\n");
        goto inport_error;
    }

    memset(in_port->m_omx_bufhead, 0, sizeof(OMX_BUFFERHEADERTYPE*)*DEF_MAX_IN_BUF_CNT);

    in_port->m_port_index         = INPUT_PORT_INDEX;
    in_port->port_pro.port_dir    = INPUT_PORT_INDEX;
    in_port->m_cur_buf_num        = 0;
    in_port->m_port_enabled       = OMX_TRUE;
    in_port->m_port_populated     = OMX_FALSE;
    in_port->m_port_reconfig      = OMX_FALSE;
    in_port->m_port_flushing      = OMX_FALSE;
    in_port->port_pro.min_count   = DEF_MIN_IN_BUF_CNT;
    in_port->port_pro.max_count   = DEF_MAX_IN_BUF_CNT;
    in_port->port_pro.buffer_size = DEFAULT_IN_BUF_SIZE;
    in_port->port_pro.alignment   = DEFAULT_ALIGN_SIZE;

    /* init out port private */
    out_port = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
    out_port->m_omx_bufhead = (OMX_BUFFERHEADERTYPE**)malloc(sizeof(OMX_BUFFERHEADERTYPE*)*DEF_MAX_OUT_BUF_CNT);
    if (!out_port->m_omx_bufhead)
    {
        DEBUG_PRINT_ERROR("No enough memory for out port!\n");
        goto outport_error;
    }

    memset(out_port->m_omx_bufhead, 0, sizeof(OMX_BUFFERHEADERTYPE*)*DEF_MAX_OUT_BUF_CNT);

    out_port->m_port_index         = OUTPUT_PORT_INDEX;
    out_port->port_pro.port_dir    = OUTPUT_PORT_INDEX;
    out_port->m_cur_buf_num        = 0;
    out_port->m_port_enabled       = OMX_TRUE;
    out_port->m_port_populated     = OMX_FALSE;
    out_port->m_port_reconfig      = OMX_FALSE;
    out_port->m_port_flushing      = OMX_FALSE;
    out_port->port_pro.min_count   = DEF_MIN_OUT_BUF_CNT;
    out_port->port_pro.max_count   = DEF_MAX_OUT_BUF_CNT;
    out_port->port_pro.buffer_size = DEFAULT_FRAME_SIZE(DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT);
    out_port->port_pro.alignment   = DEFAULT_ALIGN_SIZE;

    return 0;

outport_error:
    free(out_port->m_omx_bufhead);
    out_port->m_omx_bufhead = NULL;

inport_error:
    free(in_port->m_omx_bufhead);
    in_port->m_omx_bufhead= NULL;

    return result;
}


static void ports_deinit(OMX_COMPONENT_PRIVATE *pcom_priv)
{
    OMX_U32 i;
    for (i=0; i < MAX_PORT_NUM; ++i)
    {
        free(pcom_priv->m_port[i].m_omx_bufhead);
        pcom_priv->m_port[i].m_omx_bufhead = NULL;
    }
}


/* ==========================================================================
   FUNCTION
   get_component_version

   DESCRIPTION
   Returns the component specifcation version.

   PARAMETERS
   TBD.

   RETURN VALUE
   OMX_ErrorNone.
   ==========================================================================*/
static OMX_ERRORTYPE  get_component_version(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_OUT OMX_STRING comp_name,
       OMX_OUT OMX_VERSIONTYPE* comp_version,
       OMX_OUT OMX_VERSIONTYPE* spec_version,
       OMX_OUT OMX_UUIDTYPE* componentUUID)
{
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(comp_name == NULL);
    OMX_CHECK_ARG_RETURN(comp_version == NULL);
    OMX_CHECK_ARG_RETURN(spec_version == NULL);
    OMX_CHECK_ARG_RETURN(componentUUID == NULL);

    if (spec_version)
    {
        spec_version->nVersion = OMX_VERSION;
    }

    return OMX_ErrorNone;
}


/* ==========================================================================
   FUNCTION
   send_command

   DESCRIPTION
   process command sent to component

   PARAMETERS
   None.

   RETURN VALUE
   Error None if successful. Or Error Type
   ==========================================================================*/
static OMX_ERRORTYPE  send_command(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_COMMANDTYPE cmd,
       OMX_IN OMX_U32 param1,
       OMX_IN OMX_PTR cmd_data)
{
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    post_event(pcom_priv, (OMX_U32)cmd, (OMX_U32)param1, OMX_GENERATE_COMMAND);

    sem_wait(&pcom_priv->m_cmd_lock);

    return OMX_ErrorNone;
}


static OMX_COLOR_FORMATTYPE convert_omx_to_hal_pixel_fmt(OMX_COMPONENT_PRIVATE *pcomp_priv, OMX_U32 color_format)
{

    OMX_COLOR_FORMATTYPE hal_format;

#ifdef ANDROID
    if (pcomp_priv->m_use_native_buf)
    {
        switch (color_format)
        {
            case OMX_PIX_FMT_NV12:
                hal_format = HAL_PIXEL_FORMAT_YCbCr_420_SP;
                break;
            case OMX_PIX_FMT_NV21:
                hal_format = HAL_PIXEL_FORMAT_YCrCb_420_SP;
                break;
            default:
                hal_format = HAL_PIXEL_FORMAT_YCbCr_420_SP;
                break;
        }
    }
    else //NOT use native buf
#endif
    {
        switch (color_format)
        {
            case OMX_PIX_FMT_NV12:
                hal_format = OMX_COLOR_FormatYUV420SemiPlanar;
                break;
            case OMX_PIX_FMT_NV21:
                hal_format = OMX_QCOM_COLOR_FormatYVU420SemiPlanar;
                break;
            default:
                hal_format = OMX_COLOR_FormatYUV420SemiPlanar;
                break;
        }
    }

    return hal_format;

}



static void config_compress_format(OMX_COMPONENT_PRIVATE *pcomp_priv, OMX_U32 codec_index)
{
    strncpy((OMX_STRING)pcomp_priv->m_role, codec_trans_list[codec_index].role_name, sizeof(pcomp_priv->m_role));
    pcomp_priv->m_role[sizeof(pcomp_priv->m_role)-1] = '\0';
    pcomp_priv->m_dec_fmt = (OMX_VIDEO_CODINGTYPE)codec_trans_list[codec_index].compress_fmt;

    if (pcomp_priv->m_dec_fmt != OMX_VIDEO_CodingRV
     && (OMX_U32)pcomp_priv->m_dec_fmt != ((OMX_U32)OMX_VIDEO_CodingVP6))
    {
        pcomp_priv->drv_ctx.drv_cfg.chan_cfg.eVidStd = codec_trans_list[codec_index].codec_type;
    }
}

/* ==========================================================================
   FUNCTION
   get_parameter

   DESCRIPTION
   OMX Get Parameter method implementation

   PARAMETERS
   <TBD>.

   RETURN VALUE
   Error None if successful. Or Error Type
   ==========================================================================*/
static OMX_ERRORTYPE  get_parameter(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_INDEXTYPE param_index,
       OMX_INOUT OMX_PTR param_data)
{
    OMX_S32 param;
    OMX_ERRORTYPE ret                 = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp          = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(param_data == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    param = (OMX_S32)param_index;
    switch (param)
    {
        case OMX_IndexParamPortDefinition:
        {
            OMX_PARAM_PORTDEFINITIONTYPE *portDefn = (OMX_PARAM_PORTDEFINITIONTYPE *)param_data;
            OMX_PORT_PRIVATE *port_priv = NULL;

            if (portDefn->nPortIndex > OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("OMX_IndexParamPortDefinition: Bad Port Index %d\n", (int)portDefn->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            port_priv = &pcomp_priv->m_port[portDefn->nPortIndex];
            portDefn->nVersion.nVersion = OMX_VERSION;
            portDefn->nSize = sizeof(portDefn);
            portDefn->eDomain = OMX_PortDomainVideo;
            portDefn->format.video.xFramerate = pcomp_priv->m_frame_rate;
            portDefn->format.video.nFrameHeight = pcomp_priv->pic_info.frame_height;
            portDefn->format.video.nFrameWidth = pcomp_priv->pic_info.frame_width;
            portDefn->format.video.nStride = pcomp_priv->pic_info.stride;
            portDefn->format.video.nSliceHeight = pcomp_priv->pic_info.scan_lines;

            if (INPUT_PORT_INDEX == portDefn->nPortIndex)
            {
                portDefn->eDir = OMX_DirInput;
                portDefn->bEnabled = port_priv->m_port_enabled;
                portDefn->bPopulated = port_priv->m_port_populated;
                portDefn->format.video.eColorFormat = OMX_COLOR_FormatUnused;
                portDefn->format.video.eCompressionFormat = pcomp_priv->m_dec_fmt;
                portDefn->nBufferCountActual = port_priv->port_pro.max_count;
                portDefn->nBufferCountMin = port_priv->port_pro.min_count;
                portDefn->nBufferSize = port_priv->port_pro.buffer_size;
            }
            else if (OUTPUT_PORT_INDEX == portDefn->nPortIndex)
            {
               portDefn->eDir = OMX_DirOutput;
               portDefn->bEnabled = port_priv->m_port_enabled;
               portDefn->bPopulated = port_priv->m_port_populated;
               portDefn->format.video.eColorFormat = convert_omx_to_hal_pixel_fmt(pcomp_priv, pcomp_priv->drv_ctx.drv_cfg.cfg_color_format);
               portDefn->format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
               portDefn->nBufferCountActual = port_priv->port_pro.max_count;
               portDefn->nBufferCountMin = port_priv->port_pro.min_count;
               portDefn->nBufferSize = port_priv->port_pro.buffer_size;
            }
            break;
        }

        /*Component come to be a video*/
        case OMX_IndexParamVideoInit:
        {
            OMX_PORT_PARAM_TYPE *portParamType = (OMX_PORT_PARAM_TYPE *) param_data;

            portParamType->nVersion.nVersion = OMX_VERSION;
            portParamType->nSize = sizeof(portParamType);
            portParamType->nPorts = 2;
            portParamType->nStartPortNumber = INPUT_PORT_INDEX;
            break;
        }

        case OMX_IndexParamVideoPortFormat:
        {
            OMX_VIDEO_PARAM_PORTFORMATTYPE *portFmt = (OMX_VIDEO_PARAM_PORTFORMATTYPE *)param_data;

            portFmt->nVersion.nVersion = OMX_VERSION;
            portFmt->nSize = sizeof(portFmt);

            if (INPUT_PORT_INDEX == portFmt->nPortIndex)
            {
                switch (portFmt->nIndex)
                {
                    case 0:
                        portFmt->eColorFormat = OMX_COLOR_FormatUnused;
                        portFmt->eCompressionFormat = pcomp_priv->m_dec_fmt;
                        break;

                    /*case 1:
                        portFmt->eColorFormat = OMX_COLOR_Format32bitARGB8888;
                        portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                        break;*/

                    default:
                        return OMX_ErrorNoMore;
                }
            }
            else if (OUTPUT_PORT_INDEX == portFmt->nPortIndex)
            {
                switch (portFmt->nIndex)
                {
                    case 0:
                        portFmt->eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
                        portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                        break;

                    case 1:
                        portFmt->eColorFormat = OMX_COLOR_FormatYVU420SemiPlanar;
                        portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                        break;

                    //case 2:
                    //    portFmt->eColorFormat = OMX_COLOR_FormatYUV420Planar;
                    //    portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                    //    break;

                    default:
                        return OMX_ErrorNoMore;
                }
            }
            else
            {
                DEBUG_PRINT_ERROR("OMX_IndexParamVideoPortFormat: Bad Port Index, %d\n", (int)portFmt->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }
            break;
        }

        case OMX_IndexParamStandardComponentRole:
        {
            OMX_PARAM_COMPONENTROLETYPE *comp_role = (OMX_PARAM_COMPONENTROLETYPE *)param_data;

            comp_role->nVersion.nVersion = OMX_VERSION;
            comp_role->nSize = sizeof(*comp_role);

            strncpy((OMX_STRING)comp_role->cRole, (OMX_STRING)pcomp_priv->m_role, OMX_MAX_STRINGNAME_SIZE);
            comp_role->cRole[OMX_MAX_STRINGNAME_SIZE-1] = '\0';
            break;
        }

        case OMX_IndexParamVideoProfileLevelQuerySupported:
        {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType = (OMX_VIDEO_PARAM_PROFILELEVELTYPE *)param_data;

            profileLevelType->nVersion.nVersion = OMX_VERSION;
            profileLevelType->nSize = sizeof(*profileLevelType);

            ret = get_supported_profile_level(pcomp_priv, profileLevelType);
            break;
        }

        //extern non-standard index support
        case OMX_HisiIndexChannelAttributes:
        {
            OMX_HISI_PARAM_CHANNELATTRIBUTES *chan_attr = (OMX_HISI_PARAM_CHANNELATTRIBUTES *)param_data;
            VDEC_CHAN_CFG_S *pchan_cfg = &(pcomp_priv->drv_ctx.drv_cfg.chan_cfg);

            chan_attr->nVersion.nVersion        = OMX_VERSION;
            chan_attr->nSize                    = sizeof(*chan_attr);
            chan_attr->nErrorThreshold          = pchan_cfg->s32ChanErrThr;
            chan_attr->nPrior                   = pchan_cfg->s32ChanPriority;
            chan_attr->nStreamOverflowThreshold = pchan_cfg->s32ChanStrmOFThr;
            chan_attr->nDecodeMode              = (OMX_U32)pchan_cfg->s32DecMode;
            chan_attr->nPictureOrder            = pchan_cfg->s32DecOrderOutput;
            chan_attr->nLowdlyEnable            = pchan_cfg->s32LowdlyEnable;
            chan_attr->xFramerate               = pcomp_priv->m_frame_rate;
            break;
        }

#ifdef ANDROID
        case OMX_GoogleIndexGetAndroidNativeBufferUsage:
        {
            struct GetAndroidNativeBufferUsageParams  *pusage = (struct GetAndroidNativeBufferUsageParams *)param_data;

            pusage->nVersion.nVersion = OMX_VERSION;
            pusage->nSize = sizeof(*pusage);

            if ((pusage->nPortIndex != OUTPUT_PORT_INDEX) || !pcomp_priv->m_use_native_buf)
            {
                DEBUG_PRINT_ERROR("Bad conditions: nPortIndex=%ld, m_use_native_buf=%d\n", pusage->nPortIndex ,pcomp_priv->m_use_native_buf);
                return OMX_ErrorUndefined;
            }
            else
            {
                pusage->nUsage = GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_HISI_VDP;
            }

            break;
        }

        case OMX_HISIIndexFlexibleYUVDescription:
        {
            ret = describe_color_format((DescribeColorFormatParams *)param_data);
            break;
        }
#endif
        /* CodecType Relative */
        case OMX_IndexParamVideoAvc:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.avc, sizeof(OMX_VIDEO_PARAM_AVCTYPE));
            break;
        }

        case OMX_IndexParamVideoMpeg4:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.mpeg4, sizeof(OMX_VIDEO_PARAM_MPEG4TYPE));
            break;
        }

        case OMX_IndexParamVideoVC1:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.vc1, sizeof(OMX_VIDEO_PARAM_VC1TYPE));
            break;
        }

        case OMX_IndexParamVideoWmv:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.wmv, sizeof(OMX_VIDEO_PARAM_WMVTYPE));
            break;
        }

        case OMX_IndexParamVideoRv:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.rv, sizeof(OMX_VIDEO_PARAM_RVTYPE));
            break;
        }

        case OMX_IndexParamVideoMpeg2:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.mpeg2, sizeof(OMX_VIDEO_PARAM_MPEG2TYPE));
            break;
        }

        case OMX_IndexParamVideoH263:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.h263, sizeof(OMX_VIDEO_PARAM_H263TYPE));
            break;
        }

        case OMX_IndexParamVideoVp6:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.vp6, sizeof(OMX_VIDEO_PARAM_VP6TYPE));
            break;
        }

        case OMX_IndexParamVideoVp8:
        {
            memcpy(param_data, &pcomp_priv->m_codec_param.vp8, sizeof(OMX_VIDEO_PARAM_VP8TYPE));
            break;
        }

        default:
            DEBUG_PRINT_ERROR("get_paramter: unknown param 0x%08x\n", param_index);
            ret = OMX_ErrorUnsupportedIndex;
            break;
    }

    return ret;
}


/* ==========================================================================
   FUNCTION
   set_parameter

   DESCRIPTION
   OMX Set Parameter method implementation.

   PARAMETERS
   <TBD>.

   RETURN VALUE
   OMX Error None if successful. Or Error Type.
   ==========================================================================*/
static OMX_ERRORTYPE  set_parameter(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_INDEXTYPE param_index,
       OMX_IN OMX_PTR param_data)
{
    OMX_S32 param;
    OMX_ERRORTYPE ret                 = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp          = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;
    VDEC_CHAN_CFG_S *pchan_cfg          = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(param_data == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    if(!pcomp_priv)
    {
        DEBUG_PRINT_ERROR("set_parameter: pcomp_priv is null\n");
        return OMX_ErrorBadParameter;
    }
    pchan_cfg = &(pcomp_priv->drv_ctx.drv_cfg.chan_cfg);

    param = (OMX_S32)param_index;
    switch (param)
    {
        case OMX_IndexParamPortDefinition:
        {
            OMX_PARAM_PORTDEFINITIONTYPE *portDefn = (OMX_PARAM_PORTDEFINITIONTYPE *)param_data;
            OMX_PORT_PRIVATE *port_priv = NULL;
            OMXVDEC_DRV_CFG *drv_cfg  = NULL;

            if (portDefn->nPortIndex > OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("set_parameter: invalid port index %ld\n", portDefn->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            port_priv = &pcomp_priv->m_port[portDefn->nPortIndex];
            if (portDefn->nBufferCountActual < port_priv->port_pro.min_count)
            {
                DEBUG_PRINT_ERROR("set_parameter: setted buf cnt too small! min: %ld\n", port_priv->port_pro.min_count);
                return OMX_ErrorUndefined;
            }

             if (portDefn->format.video.nFrameWidth != pcomp_priv->pic_info.frame_width
             || portDefn->format.video.nFrameHeight != pcomp_priv->pic_info.frame_height)
             {
                pcomp_priv->pic_info.frame_width = portDefn->format.video.nFrameWidth;
                pcomp_priv->pic_info.frame_height = portDefn->format.video.nFrameHeight;

                update_port_definitions(pcomp_priv, OMX_TRUE, OMX_TRUE);
            }

            if (portDefn->format.video.nFrameWidth != 0)
            {
                if (pcomp_priv->m_use_native_buf)
                {
                portDefn->format.video.nStride = HI_SYS_GET_STRIDE(portDefn->format.video.nFrameWidth);
                }
                else
                {
                    portDefn->format.video.nStride = HI_OMX_GET_STRIDE(portDefn->format.video.nFrameWidth);
                }
            }

            ret = update_picture_info(pcomp_priv, portDefn->format.video.nFrameWidth, portDefn->format.video.nFrameHeight, portDefn->format.video.nStride);
            if (ret != OMX_ErrorNone)
            {
                DEBUG_PRINT_ERROR("set_parameter: update_picture_info failed!\n");
                return ret;
            }

            // default behavior is that we only allow buffer size to increase
            if (portDefn->nPortIndex == INPUT_PORT_INDEX
                && portDefn->nBufferSize > port_priv->port_pro.buffer_size)
            {
                port_priv->port_pro.buffer_size = portDefn->nBufferSize;
            }

            if (port_priv->port_pro.max_count < portDefn->nBufferCountActual)
            {

                DEBUG_PRINT_ERROR("set_parameter: setted buf cnt too big! max: %ld\n", port_priv->port_pro.max_count);
                return OMX_ErrorUndefined;
            }

            port_priv->port_pro.max_count = portDefn->nBufferCountActual;
            port_priv->port_pro.min_count = portDefn->nBufferCountMin;

            drv_cfg             = &(pcomp_priv->drv_ctx.drv_cfg);
            drv_cfg->cfg_width  = pcomp_priv->pic_info.frame_width;
            drv_cfg->cfg_height = pcomp_priv->pic_info.frame_height;
            drv_cfg->cfg_stride = pcomp_priv->pic_info.stride;

            if (portDefn->nPortIndex == INPUT_PORT_INDEX)
            {
                drv_cfg->cfg_inbuf_num = port_priv->port_pro.max_count;
            }
            else
            {
                drv_cfg->cfg_outbuf_num = port_priv->port_pro.max_count;
            }

            break;
        }

        case OMX_IndexParamVideoPortFormat:
        {
            OMX_VIDEO_PARAM_PORTFORMATTYPE *portFmt = (OMX_VIDEO_PARAM_PORTFORMATTYPE *)param_data;

            if (INPUT_PORT_INDEX == portFmt->nPortIndex)
            {
                OMX_U32 i;
                for (i = 0; i < COUNTOF(codec_trans_list); i++)
                {
                    if (codec_trans_list[i].compress_fmt == (OMX_S32)portFmt->eCompressionFormat)
                    {
                        break;
                    }
                }
                if (i >= COUNTOF(codec_trans_list))
                {
                    DEBUG_PRINT_ERROR("set_parameter: fmt %d not support\n", portFmt->eCompressionFormat);
                    return OMX_ErrorUnsupportedSetting;
                }
                else
                {
                    config_compress_format(pcomp_priv, i);
                }
            }
            else if (OUTPUT_PORT_INDEX == portFmt->nPortIndex)
            {
                /* now we only support yuv420SemiPlanar */
                // 将来添加新增支持格式
                switch (portFmt->eColorFormat)
                {
                    case OMX_COLOR_FormatYUV420SemiPlanar:

                        pcomp_priv->drv_ctx.drv_cfg.cfg_color_format = OMX_PIX_FMT_NV12;

                    break;
                    //case OMX_COLOR_FormatYUV420Planar:
                    //    pcomp_priv->drv_ctx.drv_cfg.cfg_color_format = OMX_PIX_FMT_YUV420Planar;
                    //    break;

                    default:
                        DEBUG_PRINT_ERROR("set_parameter: Set output format unsupport\n");
                        ret = OMX_ErrorUnsupportedSetting;
                        break;
                }

            }
            else
            {
                DEBUG_PRINT_ERROR("set_parameter: Bad port index %d\n",  (int)portFmt->nPortIndex);
                ret = OMX_ErrorBadPortIndex;
            }
            break;
        }

        case OMX_IndexParamStandardComponentRole:
        {
            OMX_U32 i;
            OMX_PARAM_COMPONENTROLETYPE *comp_role = (OMX_PARAM_COMPONENTROLETYPE *)param_data;

            for (i = 0; i < COUNTOF(codec_trans_list); i++)
            {
                if (!strncmp(codec_trans_list[i].role_name, (OMX_STRING)comp_role->cRole, OMX_MAX_STRINGNAME_SIZE))
                {
                    break;
                }
            }

            if (i >= COUNTOF(codec_trans_list))
            {
                DEBUG_PRINT_ERROR("set_parameter: format %s not support\n", comp_role->cRole);
                ret = OMX_ErrorUnsupportedSetting;
            }
            else
            {
                config_compress_format(pcomp_priv, i);
            }
            break;
        }

        //extern non standard index
        case OMX_HisiIndexChannelAttributes:
        {
            OMX_HISI_PARAM_CHANNELATTRIBUTES *chan_attr = (OMX_HISI_PARAM_CHANNELATTRIBUTES *)param_data;

            VDEC_CHAN_CFG_S *pchan_cfg     = &(pcomp_priv->drv_ctx.drv_cfg.chan_cfg);
            pchan_cfg->s32ChanErrThr     = chan_attr->nErrorThreshold;
            pchan_cfg->s32ChanPriority   = chan_attr->nPrior;
            pchan_cfg->s32ChanStrmOFThr  = chan_attr->nStreamOverflowThreshold;
            pchan_cfg->s32DecMode        = (DEC_MODE_E)chan_attr->nDecodeMode;
            pchan_cfg->s32DecOrderOutput = chan_attr->nPictureOrder;
            break;
        }

#ifdef ANDROID
        case OMX_GoogleIndexEnableAndroidNativeBuffers:
        {
            struct EnableAndroidNativeBuffersParams  *penable = (struct EnableAndroidNativeBuffersParams *)param_data;

            if (penable->nPortIndex != OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("set_parameter: Bad port index %d\n", (int)penable->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }
            pcomp_priv->m_use_native_buf = penable->bEnable;
            pcomp_priv->drv_ctx.drv_cfg.m_use_native_buf = penable->bEnable;

            if (pcomp_priv->pic_info.frame_width != 0)
            {
                OMX_PORT_PRIVATE *port_priv = NULL;

                if (pcomp_priv->m_use_native_buf)
                {
                    pcomp_priv->drv_ctx.drv_cfg.m_use_native_buf = OMX_TRUE;
                    pcomp_priv->pic_info.stride = HI_SYS_GET_STRIDE(pcomp_priv->pic_info.frame_width);
                }
                else
                {
                    pcomp_priv->drv_ctx.drv_cfg.m_use_native_buf = OMX_FALSE;
                    pcomp_priv->pic_info.stride = HI_OMX_GET_STRIDE(pcomp_priv->pic_info.frame_width);
                }

                    port_priv = &pcomp_priv->m_port[OUTPUT_PORT_INDEX];
                    port_priv->port_pro.buffer_size      = DEFAULT_FRAME_SIZE(pcomp_priv->pic_info.stride, pcomp_priv->pic_info.frame_height);
                    DEBUG_PRINT_ALWS("use_native_buf:%d stride:%d buffer size:%d\n", \
                             pcomp_priv->m_use_native_buf, pcomp_priv->pic_info.stride, port_priv->port_pro.buffer_size);
            }



            break;
        }

        case OMX_HISIIndexParamVideoAdaptivePlaybackMode:
        {
            PrepareForAdaptivePlaybackParams* pParams = (PrepareForAdaptivePlaybackParams *) param_data;

            if (pParams->nPortIndex == OUTPUT_PORT_INDEX)
            {
                pcomp_priv->is_adaptive = pParams->bEnable;

                if (!pParams->bEnable)
                {
                    return OMX_ErrorNone;
                }
                else
                {
                    if (pParams->nMaxFrameWidth > MAX_SUPPORT_WIDTH || pParams->nMaxFrameHeight > MAX_SUPPORT_HEIGHT)
                    {
                        DEBUG_PRINT_ERROR("exceed max suport resolution:(%d&%d)>(%d&%d)\n", \
                            pParams->nMaxFrameWidth, pParams->nMaxFrameHeight, MAX_SUPPORT_WIDTH, MAX_SUPPORT_HEIGHT);
                    }
                    else
                    {
                        pcomp_priv->pic_info.max_frame_width  = pParams->nMaxFrameWidth;
                        pcomp_priv->pic_info.max_frame_height = pParams->nMaxFrameHeight;
                    }

                }

                update_port_definitions(pcomp_priv, OMX_TRUE, OMX_TRUE);
            }
            else
            {
                DEBUG_PRINT_ERROR("Prepare for adaptive playback supported only on output port\n");
                ret = OMX_ErrorBadParameter;
            }

            break;
        }

#endif

        case OMX_HisiIndexFastOutputMode:
        {
            OMX_HISI_PARAM_FASTOUTPUT* pFastOutput = (OMX_HISI_PARAM_FASTOUTPUT *)param_data;

            DEBUG_PRINT_ERROR("set_parameter: pFastOutputMode:%d\n", pFastOutput->bEnabled);

            //set lowdlyEnable
            if (pFastOutput->bEnabled == OMX_TRUE)
            {
                pchan_cfg->s32LowdlyEnable = OMX_TRUE;

                DEBUG_PRINT_ERROR("set_parameter: pFastOutputMode == TURE\n");

            }
            else
            {

                pchan_cfg->s32LowdlyEnable = OMX_FALSE;
            }
            break;
        }

        /* CodecType Relative */
        case OMX_IndexParamVideoAvc:
        {
            OMX_VIDEO_PARAM_AVCTYPE *pVideoAvc = NULL;

            pVideoAvc = (OMX_VIDEO_PARAM_AVCTYPE *)param_data;
            if (pVideoAvc->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoAvc->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.avc, pVideoAvc, sizeof(OMX_VIDEO_PARAM_AVCTYPE));
            break;
        }

        case OMX_IndexParamVideoMpeg4:
        {
            OMX_VIDEO_PARAM_MPEG4TYPE *pVideoMpeg4 = NULL;

            pVideoMpeg4 = (OMX_VIDEO_PARAM_MPEG4TYPE *)param_data;
            if (pVideoMpeg4->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoMpeg4->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.mpeg4, pVideoMpeg4, sizeof(OMX_VIDEO_PARAM_MPEG4TYPE));
            break;
        }

        case OMX_IndexParamVideoVC1:
        {
            OMX_VIDEO_PARAM_VC1TYPE *pVideoVC1 = NULL;

            pVideoVC1 = (OMX_VIDEO_PARAM_VC1TYPE *)param_data;
            if (pVideoVC1->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoVC1->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            if (OMX_VIDEO_VC1ProfileSimple == pVideoVC1->eProfile
             || OMX_VIDEO_VC1ProfileMain   == pVideoVC1->eProfile)
            {
                pchan_cfg->eVidStd = STD_VC1;
                pchan_cfg->StdExt.Vc1Ext.IsAdvProfile = 0;
                pchan_cfg->StdExt.Vc1Ext.CodecVersion = 5;
            }
            else if (OMX_VIDEO_VC1ProfileAdvanced == pVideoVC1->eProfile)
            {
                pchan_cfg->eVidStd = STD_VC1;
                pchan_cfg->StdExt.Vc1Ext.IsAdvProfile = 1;
                pchan_cfg->StdExt.Vc1Ext.CodecVersion = 8;
            }
            else
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: unsupport Profile%d\n", __func__, __LINE__, pVideoVC1->eProfile);
                return OMX_ErrorUnsupportedSetting;
            }

            memcpy(&pcomp_priv->m_codec_param.vc1, pVideoVC1, sizeof(OMX_VIDEO_PARAM_VC1TYPE));
            break;
        }

        case OMX_IndexParamVideoWmv:
        {
            OMX_VIDEO_PARAM_WMVTYPE *pVideoWmv = NULL;

            pVideoWmv = (OMX_VIDEO_PARAM_WMVTYPE *)param_data;
            if (pVideoWmv->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoWmv->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.wmv, pVideoWmv, sizeof(OMX_VIDEO_PARAM_WMVTYPE));
            break;
        }

        case OMX_IndexParamVideoRv:
        {
            OMX_VIDEO_PARAM_RVTYPE *pVideoRv = NULL;

            pVideoRv = (OMX_VIDEO_PARAM_RVTYPE *)param_data;
            if (pVideoRv->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoRv->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            if (OMX_VIDEO_RVFormat8 == pVideoRv->eFormat)
            {
                pchan_cfg->eVidStd = STD_REAL8;
            }
            else if (OMX_VIDEO_RVFormat9 == pVideoRv->eFormat)
            {
                pchan_cfg->eVidStd = STD_REAL9;
            }
            else
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: unsupport format%d\n", __func__, __LINE__, pVideoRv->eFormat);
                return OMX_ErrorUnsupportedSetting;
            }

            memcpy(&pcomp_priv->m_codec_param.rv, pVideoRv, sizeof(OMX_VIDEO_PARAM_RVTYPE));
            break;
        }

        case OMX_IndexParamVideoMpeg2:
        {
            OMX_VIDEO_PARAM_MPEG2TYPE *pVideoMpeg2 = NULL;

            pVideoMpeg2 = (OMX_VIDEO_PARAM_MPEG2TYPE *)param_data;
            if (pVideoMpeg2->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoMpeg2->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.mpeg2, pVideoMpeg2, sizeof(OMX_VIDEO_PARAM_MPEG2TYPE));
            break;
        }

        case OMX_IndexParamVideoH263:
        {
            OMX_VIDEO_PARAM_H263TYPE *pVideoH263 = NULL;

            pVideoH263 = (OMX_VIDEO_PARAM_H263TYPE *)param_data;
            if (pVideoH263->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoH263->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.h263, pVideoH263, sizeof(OMX_VIDEO_PARAM_H263TYPE));
            break;
        }

        case OMX_IndexParamVideoVp6:
        {
            OMX_VIDEO_PARAM_VP6TYPE *pVideoVp6 = NULL;

            pVideoVp6 = (OMX_VIDEO_PARAM_VP6TYPE *)param_data;
            if (pVideoVp6->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoVp6->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            if (OMX_VIDEO_VP6 == pVideoVp6->eFormat)
            {
                pchan_cfg->eVidStd = STD_VP6;
            }
            else if (OMX_VIDEO_VP6F == pVideoVp6->eFormat)
            {
                pchan_cfg->eVidStd = STD_VP6F;
            }
            else if (OMX_VIDEO_VP6A == pVideoVp6->eFormat)
            {
                pchan_cfg->eVidStd = STD_VP6A;
            }
            else
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: unsupport format%d\n", __func__, __LINE__, pVideoVp6->eFormat);
                return OMX_ErrorUnsupportedSetting;
            }

            memcpy(&pcomp_priv->m_codec_param.vp6, pVideoVp6, sizeof(OMX_VIDEO_PARAM_VP6TYPE));
            break;
        }

        case OMX_IndexParamVideoVp8:
        {
            OMX_VIDEO_PARAM_VP8TYPE *pVideoVp8 = NULL;

            pVideoVp8 = (OMX_VIDEO_PARAM_VP8TYPE *)param_data;
            if (pVideoVp8->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("%s L%d ERROR: invalid port%lu\n", __func__, __LINE__, pVideoVp8->nPortIndex);
                return OMX_ErrorBadPortIndex;
            }

            memcpy(&pcomp_priv->m_codec_param.vp8, pVideoVp8, sizeof(OMX_VIDEO_PARAM_VP8TYPE));
            break;
        }

        default:
            DEBUG_PRINT_ERROR("set_parameter: unknown param 0x%08x\n", param_index);
            ret = OMX_ErrorUnsupportedIndex;
            break;
    }

    return ret;
}


/* =========================================================================
   FUNCTION
   get_config

   DESCRIPTION
   OMX Get Config Method implementation.

   PARAMETERS
   <TBD>.

   RETURN VALUE
   OMX Error None if successful. Or Error Type.
   =========================================================================*/
static OMX_ERRORTYPE  get_config(
        OMX_IN OMX_HANDLETYPE phandle,
        OMX_IN OMX_INDEXTYPE  config_index,
        OMX_INOUT OMX_PTR     config_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_CONFIG_RECTTYPE *prect = NULL;
    OMX_COMPONENTTYPE   *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(config_data == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    OMXVDEC_DRV_CFG *drv_cfg = &pcomp_priv->drv_ctx.drv_cfg;
    VDEC_CHAN_CFG_S *pchan_cfg = &pcomp_priv->drv_ctx.drv_cfg.chan_cfg;

    switch (config_index)
    {
        case OMX_IndexConfigCommonOutputCrop:
            prect = (OMX_CONFIG_RECTTYPE *)config_data;

            if (prect->nPortIndex != OUTPUT_PORT_INDEX)
            {
                return OMX_ErrorBadPortIndex;
            }

            prect->nLeft = 0;
            prect->nTop = 0;
            prect->nHeight = pcomp_priv->pic_info.frame_height;
            prect->nWidth = pcomp_priv->pic_info.frame_width;
            break;

        case OMX_IndexConfigCommonRotate :
            {
                OMX_CONFIG_ROTATIONTYPE *rotate = (OMX_CONFIG_ROTATIONTYPE*)config_data;
                if (rotate->nPortIndex==OUTPUT_PORT_INDEX)
                {
                    rotate->nRotation = drv_cfg->cfg_rotation;
                }
                else
                {
                    DEBUG_PRINT_ERROR("OMX_IndexConfigCommonRotate: parameter invalid\n");

                    ret = OMX_ErrorBadParameter;
                    break;
                }
                break;
            }

        case OMX_IndexConfigCommonMirror:
            {
                OMX_CONFIG_MIRRORTYPE *nMirror = (OMX_CONFIG_MIRRORTYPE*)config_data;

                if(nMirror->nPortIndex > OUTPUT_PORT_INDEX)
                {
                    DEBUG_PRINT_ERROR("OMX_IndexConfigCommonMirror: Bad Port Index %d\n", (int)nMirror->nPortIndex);
                    return OMX_ErrorBadPortIndex;
                }
                nMirror->eMirror = pchan_cfg->StdExt.Vp6Ext.bReversed;

                break;
            }

        default:
            DEBUG_PRINT_ERROR("get_config: unknown index 0x%08x\n", config_index);
            ret = OMX_ErrorBadParameter;
            break;
    }

    return ret;
}


/* ========================================================================
   FUNCTION
   set_config

   DESCRIPTION
   OMX Set Config method implementation

   PARAMETERS
   <TBD>.

   RETURN VALUE
   OMX Error None if successful.
   ========================================================================*/
static OMX_ERRORTYPE  set_config(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_INDEXTYPE config_index,
       OMX_IN OMX_PTR config_data)
{

    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE   *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(config_data == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    OMXVDEC_DRV_CFG *drv_cfg = &pcomp_priv->drv_ctx.drv_cfg;
    VDEC_CHAN_CFG_S *pchan_cfg = &pcomp_priv->drv_ctx.drv_cfg.chan_cfg;

    switch (config_index)
    {
        case OMX_IndexConfigCommonMirror:
            {
                OMX_CONFIG_MIRRORTYPE *nMirror = (OMX_CONFIG_MIRRORTYPE*)config_data;

                if(nMirror->nPortIndex > OUTPUT_PORT_INDEX)
                {
                    DEBUG_PRINT_ERROR("OMX_IndexConfigCommonMirror: Bad Port Index %d\n", (int)nMirror->nPortIndex);
                    return OMX_ErrorBadPortIndex;
                }

                if (nMirror->eMirror == OMX_MirrorNone)
                {
                    pchan_cfg->StdExt.Vp6Ext.bReversed = OMX_FALSE;
                }
                else if (nMirror->eMirror == OMX_MirrorVertical)
                {
                    pchan_cfg->StdExt.Vp6Ext.bReversed = OMX_TRUE;
                }
                else
                {
                    DEBUG_PRINT_ERROR("OMX_IndexConfigCommonMirror: UNSUPPORT mirror type %d\n", (int)nMirror->eMirror);
                }

                break;
            }

        case OMX_IndexConfigCommonRotate :
            {
                OMX_CONFIG_ROTATIONTYPE *rotate = (OMX_CONFIG_ROTATIONTYPE*)config_data;

                if ((rotate->nPortIndex == OUTPUT_PORT_INDEX) &&
                    (rotate->nRotation == 0 ||
                     rotate->nRotation == 90 ||
                     rotate->nRotation == 180 ||
                     rotate->nRotation == 270))
                {
                    drv_cfg->cfg_rotation = rotate->nRotation;
                }
                else
                {
                    DEBUG_PRINT_ERROR("OMX_IndexConfigCommonRotate: parameter invalid\n");

                    ret = OMX_ErrorBadParameter;
                    break;
                }
                break;
            }

        default:
            {
                DEBUG_PRINT_ERROR("get_config: unknown index 0x%08x\n", config_index);
                ret = OMX_ErrorBadParameter;
                break;
            }
    }

    return ret;
}


/* =========================================================================
   FUNCTION
   get_extension_index

   DESCRIPTION
   OMX GetExtensionIndex method implementaion.  <TBD>

   PARAMETERS
   <TBD>.

   RETURN VALUE
   OMX Error None if everything successful.
   =========================================================================*/
static OMX_ERRORTYPE  get_extension_index(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_STRING param_name,
       OMX_OUT OMX_INDEXTYPE *pindex_type)
{
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(param_name == NULL);
    OMX_CHECK_ARG_RETURN(pindex_type == NULL);

    OMX_U32 param_length = strlen(param_name);

    if (!strncmp(param_name, "OMX.Hisi.Param.Index.ChannelAttributes",\
                 MAX(param_length, strlen("OMX.Hisi.Param.Index.ChannelAttributes"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_HisiIndexChannelAttributes;
    }
    else if (!strncmp(param_name, "OMX.Hisi.Param.Index.FastOutputMode",\
                      MAX(param_length, strlen("OMX.Hisi.Param.Index.FastOutputMode"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_HisiIndexFastOutputMode;
    }
//#if 1  // native buffer
#ifdef ANDROID
    else if (!strncmp(param_name, "OMX.google.android.index.enableAndroidNativeBuffers",\
                      MAX(param_length, strlen("OMX.google.android.index.enableAndroidNativeBuffers"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_GoogleIndexEnableAndroidNativeBuffers;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.getAndroidNativeBufferUsage",\
                      MAX(param_length, strlen("OMX.google.android.index.getAndroidNativeBufferUsage"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_GoogleIndexGetAndroidNativeBufferUsage;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.useAndroidNativeBuffer2",\
                      MAX(param_length, strlen("OMX.google.android.index.useAndroidNativeBuffer2"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_GoogleIndexUseAndroidNativeBuffer2;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.describeColorFormat",\
                      MAX(param_length, strlen("OMX.google.android.index.describeColorFormat"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_HISIIndexFlexibleYUVDescription;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.prepareForAdaptivePlayback",\
                      MAX(param_length, strlen("OMX.google.android.index.prepareForAdaptivePlayback"))))
    {
        *pindex_type = (OMX_INDEXTYPE)OMX_HISIIndexParamVideoAdaptivePlaybackMode;
    }
#endif
    else
    {
        DEBUG_PRINT_ERROR("get_extension_index: %s not implemented!\n", param_name);
        return OMX_ErrorNotImplemented;
    }

    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   get_state

   DESCRIPTION
   Returns the state information back to the caller.<TBD>

   PARAMETERS
   <TBD>.

   RETURN VALUE
   Error None if everything is successful.
   =======================================================================*/
static OMX_ERRORTYPE get_state(
       OMX_IN OMX_HANDLETYPE  phandle,
       OMX_OUT OMX_STATETYPE* state)
{
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(state == NULL);

    pcomp = (OMX_COMPONENTTYPE *)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    *state = pcomp_priv->m_state;
    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   component_tunnel_request

   DESCRIPTION
   OMX Component Tunnel Request method implementation. Now not supported

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything successful.
   =======================================================================*/
static OMX_ERRORTYPE  component_tunnel_request(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_HANDLETYPE peerComponent,
       OMX_IN OMX_U32 peerPort,
       OMX_INOUT OMX_TUNNELSETUPTYPE *tunnelSetup)
{
    DEBUG_PRINT_WARN("Error: Tunnel mode Not Implemented, phandle = %p, port = %ld, peerComponent = %p, peerPort = %ld, tunnelSetup = %p\n", phandle, port, peerComponent, peerPort, tunnelSetup);
    return OMX_ErrorNotImplemented;
}


/* ======================================================================
   FUNCTION
   omx_vdec::AllocateBuffer. API Call

   DESCRIPTION
   None

   PARAMETERS
   None.

   RETURN VALUE
   OMX_TRUE/OMX_FALSE
   =======================================================================*/
static OMX_ERRORTYPE  allocate_buffer(
       OMX_IN OMX_HANDLETYPE  phandle,
       OMX_INOUT OMX_BUFFERHEADERTYPE **omx_bufhdr,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_PTR app_data,
       OMX_IN OMX_U32 bytes)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;
    OMX_PORT_PRIVATE *port_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(omx_bufhdr == NULL);

    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[AB] Error: Invalid Port %d\n",(int)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[AB] ERROR: Invalid State\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateIdle)
    || ( pcom_priv->m_state == OMX_StateExecuting)
    || ( pcom_priv->m_state == OMX_StatePause)
    || ((pcom_priv->m_state == OMX_StateLoaded) && bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING)))
    {
        //DEBUG_PRINT("[AB] Cur State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT_ERROR("[AB] Invalid State %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

    ret = allocate_buffer_internal(pcom_priv, omx_bufhdr, app_data, port, bytes);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("[AB] ERROR: Allocate Buf failed\n");
        return ret;
    }

    if ((port == INPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_populated = OMX_TRUE;
        }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING))
        {
            bit_clear(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING);
            post_event(pcom_priv, OMX_CommandPortEnable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if ((port == OUTPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_populated = OMX_TRUE;
        }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Enable_Pending --> Enable\n");
            bit_clear(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING);
            ret = channel_port_enable(&pcom_priv->drv_ctx, port_priv->m_port_enabled);
            if (ret != OMX_ErrorNone)
            {
               DEBUG_PRINT_ERROR("call channel_port_enable return failed! ret = %d\n",ret);
            }
            else
            {
			    post_event(pcom_priv, OMX_CommandPortEnable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
		    }
	    }
    }

    if (ports_all_full(pcom_priv))
    {
        if (bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING))
        {
            // Send the callback now
            DEBUG_PRINT_STATE("[OmxState] Idle_Pending --> Idle\n");

            bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);
            post_event(pcom_priv, OMX_CommandStateSet, OMX_StateIdle, OMX_GENERATE_COMMAND_DONE);
        }
    }

    return ret;
}


/* ======================================================================
   FUNCTION
   FreeBuffer

   DESCRIPTION

   PARAMETERS
   None.

   RETURN VALUE
   OMX_TRUE/OMX_FALSE
   ======================================================================*/
static OMX_ERRORTYPE  free_buffer(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_U32 port,
       OMX_IN OMX_BUFFERHEADERTYPE* omx_bufhdr)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(omx_bufhdr == NULL);

    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[FB] Error: Invalid Port %d\n",(int)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[FB] ERROR: Invalid State\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateLoaded)
    || ( pcom_priv->m_state == OMX_StateExecuting)
    || ( pcom_priv->m_state == OMX_StatePause)
    || ((pcom_priv->m_state == OMX_StateIdle) && bit_present(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING)))
    {
        //DEBUG_PRINT("[FB] Cur State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT_ERROR("[FB] Invalid State %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

    ret = free_buffer_internal(pcom_priv, port, omx_bufhdr);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("[FB] ERROR: free Buf internal failed\n");
        return ret;
    }

    if((port == INPUT_PORT_INDEX) && port_empty(pcom_priv, port))
    {
        if (bit_present((&pcom_priv->m_flags), OMX_STATE_INPUT_DISABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Disable_Pending --> Disable\n");
            bit_clear((&pcom_priv->m_flags), OMX_STATE_INPUT_DISABLE_PENDING);
            post_event(pcom_priv, OMX_CommandPortDisable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if((port == OUTPUT_PORT_INDEX) && port_empty(pcom_priv, port))
    {

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_DISABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Disable_Pending --> Disable\n");
            bit_clear((&pcom_priv->m_flags), OMX_STATE_OUTPUT_DISABLE_PENDING);
            post_event(pcom_priv, OMX_CommandPortDisable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if(ports_all_empty(pcom_priv))
    {
        if (bit_present(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Loaded_Pending --> Loaded\n");
            bit_clear((&pcom_priv->m_flags), OMX_STATE_LOADING_PENDING);
            post_event(pcom_priv, OMX_CommandStateSet, OMX_StateLoaded, OMX_GENERATE_COMMAND_DONE);
        }
    }

    return ret;
}


/* ======================================================================
   FUNCTION
   omx_vdec::UseBuffer

   DESCRIPTION
   OMX Use Buffer method implementation.

   PARAMETERS
   <TBD>.

   RETURN VALUE
   OMX Error None , if everything successful.
   =====================================================================*/
static OMX_ERRORTYPE use_buffer(
       OMX_IN OMX_HANDLETYPE            phandle,
       OMX_INOUT OMX_BUFFERHEADERTYPE** bufferHdr,
       OMX_IN OMX_U32                   port,
       OMX_IN OMX_PTR                   app_data,
       OMX_IN OMX_U32                   bytes,
       OMX_IN OMX_U8*                   buffer)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone; // OMX return type
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;
    OMX_PORT_PRIVATE *port_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(bufferHdr == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[UB] Error: Invalid Port %d\n",(int)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[UB] ERROR: Invalid State\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateIdle)
    || ( pcom_priv->m_state == OMX_StateExecuting)
    || ( pcom_priv->m_state == OMX_StatePause)
    || ((pcom_priv->m_state == OMX_StateLoaded) && bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING)))
    {
        //DEBUG_PRINT("[UB] Cur State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT_ERROR("[UB] Invalid State %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

//#if 1  // native buffer
#ifdef ANDROID
       if (pcom_priv->m_use_native_buf && OUTPUT_PORT_INDEX == port)
       {
           ret = use_android_native_buffer_internal(pcom_priv, bufferHdr, app_data, port, bytes, buffer);
           if (ret != OMX_ErrorNone)
           {
               DEBUG_PRINT_ERROR("[UNB]ERROR: use_buffer_internal failed\n");
               return OMX_ErrorInsufficientResources;
           }
       }
       else
 #endif
       {
           ret = use_buffer_internal(pcom_priv, bufferHdr, app_data, port, bytes, buffer);
           if (ret != OMX_ErrorNone)
           {
               DEBUG_PRINT_ERROR("[UB]ERROR: use_buffer_internal failed\n");
               return OMX_ErrorInsufficientResources;
           }
       }

    if ((port == INPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_populated = OMX_TRUE;
        }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING))
        {
            bit_clear(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING);
            post_event(pcom_priv, OMX_CommandPortEnable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if ((port == OUTPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->m_port_enabled)
        {
            port_priv->m_port_populated = OMX_TRUE;
        }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Enable_Pending --> Enable\n");
            bit_clear(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING);
            ret = channel_port_enable(&pcom_priv->drv_ctx, port_priv->m_port_enabled);
            if (ret != OMX_ErrorNone)
            {
               DEBUG_PRINT_ERROR("call channel_port_enable return failed! ret = %d\n",ret);
            }
            else
            {
			    post_event(pcom_priv, OMX_CommandPortEnable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
		    }
	    }
    }

    if (ports_all_full(pcom_priv))
    {
        if (bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING))
        {
            // Send the callback now
            DEBUG_PRINT_STATE("[OmxState] Idle_Pending --> Idle\n");

            bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);
            post_event(pcom_priv, OMX_CommandStateSet, OMX_StateIdle, OMX_GENERATE_COMMAND_DONE);
        }
    }

    return  OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   EmptyThisBuffer

   DESCRIPTION
   This routine is used to push the encoded video frames to
   the video decoder.

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything went successful.
   =======================================================================*/
static OMX_ERRORTYPE  empty_this_buffer(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_BUFFERHEADERTYPE *buffer)
{
    OMX_U32  i;
    OMX_COMPONENTTYPE *pcomp         = NULL;
    OMX_PORT_PRIVATE *port_priv      = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(buffer  == NULL);

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

    if(pcom_priv->m_state != OMX_StateExecuting && pcom_priv->m_state != OMX_StatePause)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: Invalid State\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    if (!port_priv->m_port_enabled)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: input port disabled.\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    for (i = 0; i < port_priv->port_pro.max_count; i++)
    {
        if (buffer == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_pro.max_count)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: no buffers found for address[%p], %ld >= max_count(%ld)", buffer, i, port_priv->port_pro.max_count);
        return OMX_ErrorBadParameter;
    }

    if (buffer->nInputPortIndex != INPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[ETB] ERROR: invalid port \n");
        return OMX_ErrorBadPortIndex;
    }

    if (buffer->nFlags & 0x1)
    {
        DEBUG_PRINT_ALWS("[ETB] receive the EOS, nFlags:%x \n", (unsigned int)buffer->nFlags);
    }

    //DEBUG_PRINT_STREAM("[ETB]bufhdr = %p, bufhdr->pbuffer = %p", buffer, buffer->pBuffer);
	DEBUG_PRINT_STREAM(" [ETB] timestamp=       %lld          data_len =%lu\n", buffer->nTimeStamp, buffer->nFilledLen);

    post_event(pcom_priv, (OMX_U32 )buffer, 0, OMX_GENERATE_ETB);
    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   FillThisBuffer

   DESCRIPTION
   IL client uses this method to release the frame buffer
   after displaying them.

   PARAMETERS
   None

   RETURN VALUE
   OMX_TRUE/OMX_FALSE
   =======================================================================*/
static OMX_ERRORTYPE  fill_this_buffer(
       OMX_IN OMX_HANDLETYPE  phandle,
       OMX_IN OMX_BUFFERHEADERTYPE *buffer)
{
    OMX_U32  i;
    OMX_COMPONENTTYPE *pcomp         = NULL;
    OMX_PORT_PRIVATE *port_priv      = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(buffer  == NULL);

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;
    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

    /* check component state */
    if(pcom_priv->m_state != OMX_StateExecuting && pcom_priv->m_state != OMX_StatePause)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: Invalid State\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    /* check port state */
    if (!port_priv->m_port_enabled)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: output port disabled!\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    /* check buffer validate */
    for (i = 0; i < port_priv->port_pro.max_count; i++)
    {
        if (buffer == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_pro.max_count)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: buffers[%p] match failed\n", buffer);
        return OMX_ErrorBadParameter;
    }

    if (buffer->nOutputPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[FTB] ERROR: invalid port\n");
        return OMX_ErrorBadParameter;
    }

    DEBUG_PRINT_STREAM("[FTB] bufhdr = %p, bufhdr->pBuffer = %p\n", buffer, buffer->pBuffer);

    post_event(pcom_priv, (OMX_U32 )buffer, 0, OMX_GENERATE_FTB);

    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   SetCallbacks

   DESCRIPTION
   Set the callbacks.

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything successful.
   =======================================================================*/
static OMX_ERRORTYPE  set_callbacks(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_IN OMX_CALLBACKTYPE* callbacks,
       OMX_IN OMX_PTR app_data)
{
    OMX_COMPONENTTYPE *pcomp         = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle   == NULL);
    OMX_CHECK_ARG_RETURN(callbacks == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->EventHandler    == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->EmptyBufferDone == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->FillBufferDone  == NULL);

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    pcom_priv->m_cb       = *callbacks;
    pcom_priv->m_app_data = app_data;

    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   component_role_enum

   DESCRIPTION
   enum role omx component support

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything successful.
   =======================================================================*/
static OMX_ERRORTYPE  component_role_enum(
       OMX_IN OMX_HANDLETYPE phandle,
       OMX_OUT OMX_U8 *cRole,
       OMX_IN OMX_U32 nIndex)
{
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(cRole   == NULL);

    if (nIndex > COUNTOF(codec_trans_list) - 1)
    {
        DEBUG_PRINT_WARN("component_role_enum: no more roles\n");
        return OMX_ErrorNoMore;
    }

    strncpy((OMX_STRING)cRole, codec_trans_list[nIndex].role_name, OMX_MAX_STRINGNAME_SIZE);

    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   component_deinit

   DESCRIPTION
   component deinit, used to destroy component.

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything successful.
   =======================================================================*/
OMX_ERRORTYPE  component_deinit(OMX_IN OMX_HANDLETYPE phandle)
{
    OMX_U32  i                       = 0;
    OMX_S8  msg                      = 0;
    OMX_ERRORTYPE ret_val            = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp         = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;
    OMX_PORT_PRIVATE *port_priv      = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)pcomp->pComponentPrivate;

    if (OMX_StateLoaded != pcom_priv->m_state)
    {
        DEBUG_PRINT_ERROR("OMX not in Loaded state! cur state %d\n", pcom_priv->m_state);
        if(OMX_StateExecuting == pcom_priv->m_state)
        {
            if (channel_stop(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("%s: channel_stop failed!\n", __func__);
                return OMX_ErrorHardware;
            }

            ret_val = omx_flush_port(pcom_priv, OMX_ALL);
            if (OMX_ErrorNone != ret_val)
            {
                DEBUG_PRINT_ERROR("%s: omx_flush_port failed!\n", __func__);
                return ret_val;
            }

            if (ports_all_empty(pcom_priv))
            {
                if (channel_release(&pcom_priv->drv_ctx) < 0)
                {
                    DEBUG_PRINT_ERROR("%s: channel_release failed!\n", __func__);
                    return OMX_ErrorHardware;
                }
            }
        }
        else if(OMX_StateIdle == pcom_priv->m_state)
        {
            if (ports_all_empty(pcom_priv))
            {
                if (channel_release(&pcom_priv->drv_ctx) < 0)
                {
                    DEBUG_PRINT_ERROR("%s: channel_release failed!\n", __func__);
                    return OMX_ErrorHardware;
                }
            }
        }
        return OMX_ErrorIncorrectStateOperation;
    }

    /* Check port is deinit */
    if (!port_empty(pcom_priv, OUTPUT_PORT_INDEX))
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        for (i = 0; i < port_priv->port_pro.max_count; i++)
        {
            free_buffer_internal(pcom_priv, OUTPUT_PORT_INDEX, port_priv->m_omx_bufhead[i]);
        }
    }

    if (!port_empty(pcom_priv, INPUT_PORT_INDEX))
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        for (i = 0; i < port_priv->port_pro.max_count; i++)
        {
            free_buffer_internal(pcom_priv, INPUT_PORT_INDEX, port_priv->m_omx_bufhead[i]);
        }
    }

    vdec_deinit_drv_context(&pcom_priv->drv_ctx);

    ports_deinit(pcom_priv);

    if (!pcom_priv->msg_thread_exit && pcom_priv->msg_thread_id != OMX_FALSE)
    {
        pcom_priv->msg_thread_exit = OMX_TRUE;
        pthread_join(pcom_priv->msg_thread_id, NULL);
    }

    close(pcom_priv->m_pipe_in);
    close(pcom_priv->m_pipe_out);

    if (read(pcom_priv->m_async_pipe[0],&msg,1) < 0)
    {
        DEBUG_PRINT_WARN("%s: no msg in pipe.\n", __func__);
    }

    if (!pcom_priv->event_thread_exit && pcom_priv->event_thread_id != OMX_FALSE)
    {
        pcom_priv->event_thread_exit = OMX_TRUE;
        pthread_join(pcom_priv->event_thread_id, NULL);
    }

    pthread_mutex_destroy(&pcom_priv->m_lock);
    sem_destroy(&pcom_priv->m_cmd_lock);
    close(pcom_priv->m_async_pipe[0]);
    close(pcom_priv->m_async_pipe[1]);
    pcom_priv->m_async_pipe[0] = -1;
    pcom_priv->m_async_pipe[1] = -1;

    free(pcom_priv);
    pcom_priv = NULL;
    pcomp->pComponentPrivate = NULL;

#ifdef HI_OMX_TEE_SUPPORT
    HI_SEC_MMZ_DeInit();
#endif

    return OMX_ErrorNone;
}


/* ======================================================================
   FUNCTION
   component_init

   DESCRIPTION
   component init

   PARAMETERS
   None.

   RETURN VALUE
   OMX Error None if everything successful.
   =======================================================================*/
OMX_ERRORTYPE component_init(OMX_HANDLETYPE phandle, OMX_STRING comp_name)
{
    int fds[2];
    OMX_S32 result                   = -1;
    OMX_S8 msg                       = 0;
    OMX_ERRORTYPE error              = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp         = NULL;
    OMX_COMPONENT_PRIVATE *pcom_priv = NULL;
    OMX_CHECK_ARG_RETURN(phandle == NULL);

#ifdef HI_OMX_TEE_SUPPORT
    if (strncmp(comp_name, OMX_VDEC_SECURE_COMPONENT_NAME, sizeof(OMX_VDEC_SECURE_COMPONENT_NAME)) != 0)
    {
        DEBUG_PRINT_ERROR("component_init(): secure compname %s not match \n", comp_name);
        return OMX_ErrorBadParameter;
    }
    if (HI_SEC_MMZ_Init() < 0)
    {
        DEBUG_PRINT_ERROR("component_init(): init sec mmz failed!\n");
        return OMX_ErrorUndefined;
    }
#else
    if (strncmp(comp_name, OMX_VDEC_NORMAL_COMPONENT_NAME, sizeof(OMX_VDEC_NORMAL_COMPONENT_NAME)) != 0)
    {
        DEBUG_PRINT_ERROR("component_init(): normal compname %s not match \n", comp_name);
        return OMX_ErrorBadParameter;
    }
#endif

    DEBUG_PRINT_STATE("Init OMX component %s\n", comp_name);

    pcomp     = (OMX_COMPONENTTYPE *)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE *)malloc(sizeof(OMX_COMPONENT_PRIVATE));
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("component_init(): malloc failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit;
    }

    memset(pcom_priv, 0 ,sizeof(OMX_COMPONENT_PRIVATE));

    pcom_priv->m_flags                    = 0;
    pcom_priv->event_thread_exit        = OMX_FALSE;
    pcom_priv->msg_thread_exit            = OMX_FALSE;
    pcom_priv->pic_info.frame_width     = DEFAULT_FRAME_WIDTH;
    pcom_priv->pic_info.frame_height    = DEFAULT_FRAME_HEIGHT;
    pcom_priv->pic_info.stride          = HI_SYS_GET_STRIDE(DEFAULT_FRAME_WIDTH);
    pcom_priv->pic_info.scan_lines      = DEFAULT_FRAME_HEIGHT;//ALIGN_UP(DEFAULT_FRAME_HEIGHT, 16);
    pcom_priv->m_pre_timestamp            = -1;
    pcom_priv->m_frame_rate                = DEFAULT_FPS;
    pcom_priv->m_dec_fmt                = OMX_VIDEO_CodingUnused;

    init_event_queue(&pcom_priv->m_cmd_q);
    init_event_queue(&pcom_priv->m_ftb_q);
    init_event_queue(&pcom_priv->m_etb_q);

    result = pthread_mutex_init(&pcom_priv->m_lock, NULL);
    if (result != 0)
    {
        DEBUG_PRINT_ERROR("pthread_mutex_init failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit;
    }
    sem_init(&pcom_priv->m_cmd_lock, 0, 0);

    result = pipe(pcom_priv->m_async_pipe);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("component_init() ERROR: failed to create async pipe\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }

    result = pipe(fds);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("component_init() ERROR: pipe create failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit1;
    }

    pcom_priv->m_pipe_in    = fds[0];
    pcom_priv->m_pipe_out    = fds[1];

    /* create event theard */
    if (pcom_priv->event_thread_id == OMX_FALSE)
    {
        result = pthread_create(&pcom_priv->event_thread_id, 0, event_thread, pcom_priv);
        if (result < 0)
        {
            DEBUG_PRINT_ERROR("component_init() ERROR: failed to create event thread\n");
            error = OMX_ErrorInsufficientResources;
            goto error_exit0;
        }
    }
    else
    {
        DEBUG_PRINT_ERROR("%s: event thread has been create.\n", __func__);
        //goto error_exit0;
    }


    if (read(pcom_priv->m_async_pipe[0],&msg,1) < 0)
    {
        DEBUG_PRINT_WARN("%s: no msg in pipe.\n", __func__);
    }

    //update_picture_info(pcom_priv, DEFAULT_FRAME_WIDTH, DEFAULT_FRAME_HEIGHT);
    result = ports_init(pcom_priv);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("component_init() ERROR: pipe create failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit2;
    }

    result = vdec_init_drv_context(&pcom_priv->drv_ctx);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("component_init() ERROR: drv ctx init failed\n");
        error = OMX_ErrorUndefined;
        goto error_exit3;
    }

    /* init component callbacks */
    pcomp->SetCallbacks           = set_callbacks;
    pcomp->GetComponentVersion    = get_component_version;
    pcomp->SendCommand            = send_command;
    pcomp->GetParameter           = get_parameter;
    pcomp->SetParameter           = set_parameter;
    pcomp->GetConfig              = get_config;
    pcomp->SetConfig              = set_config;
    pcomp->GetState               = get_state;
    pcomp->ComponentTunnelRequest = component_tunnel_request;
    pcomp->UseBuffer              = use_buffer;
    pcomp->AllocateBuffer         = allocate_buffer;
    pcomp->FreeBuffer             = free_buffer;
    pcomp->EmptyThisBuffer        = empty_this_buffer;
    pcomp->FillThisBuffer         = fill_this_buffer;
    pcomp->ComponentDeInit        = component_deinit;
    pcomp->GetExtensionIndex      = get_extension_index;
    pcomp->ComponentRoleEnum      = component_role_enum;
    pcomp->pComponentPrivate      = pcom_priv;

    pcom_priv->m_pcomp            = pcomp;
    pcom_priv->m_state            = OMX_StateLoaded;

    return OMX_ErrorNone;

error_exit3:
    ports_deinit(pcom_priv);

error_exit2:
    close(pcom_priv->m_pipe_in);
    close(pcom_priv->m_pipe_out);

error_exit1:
    if (pcom_priv->event_thread_id != OMX_FALSE)
    {
        pthread_join(pcom_priv->event_thread_id, NULL);
    }
error_exit0:
    pthread_mutex_destroy(&pcom_priv->m_lock);
    sem_destroy(&pcom_priv->m_cmd_lock);
    free(pcom_priv);
    pcomp->pComponentPrivate = NULL;

error_exit:
#ifdef HI_OMX_TEE_SUPPORT
    HI_SEC_MMZ_DeInit();
#endif

    return error;
}



