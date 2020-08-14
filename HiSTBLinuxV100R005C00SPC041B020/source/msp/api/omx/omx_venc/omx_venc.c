/*========================================================================
  Open MAX   Component: Video Encoder
  This module contains the class definition for openMAX decoder component.
author: liminqi 00228308
==========================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/file.h>


#include "omx_dbg.h"
#include "omx_venc.h"
#include "hi_unf_common.h"
#include "OMX_VideoExt.h"
#include "OMX_RoleNames.h"

#ifdef ANDROID
#include "gralloc_priv.h"
#include "graphics.h"
#include "hi_tde_api.h"
#endif

#define PIC_ALIGN_SIZE (4)

#define VENC_DONE_SUCCESS (0)
#define VENC_DONE_FAILED  (-1)

//#define __VENC_DBG_DELAY_STAT__
//#define  __OMXVENC_FILE_OUT_SAVE__
//#define  __OMXVENC_FILE_IN_SAVE__
//#define __OMXVENC_RGB_BUFFER__

#ifdef  __VENC_DBG_DELAY_STAT__
static  struct timeval TmpTime[100][6] = {0};
static HI_U32 etb_id = 0;          //0
static HI_U32 etb_x_id = 0;          //5
static HI_U32 etb_done_id = 0;          //0
static HI_U32 ftb_id = 0;          //5
static HI_U32 ftb_x_id = 0;          //5
static HI_U32 ftb_done_id = 0;          //5

static HI_U32 flag   = 1;
#endif

static HI_U32 g_end_pts[2] = {0xffffffff, 0xffffffff};
static HI_BOOL g_IsEosFull = 0;
#if 1
HI_U32 g_etb_cnt = 0;
HI_U32 g_ftb_cnt = 0;
#endif

#ifdef  __OMXVENC_FILE_OUT_SAVE__
FILE*       g_pFiletoSave;
#endif

#ifdef __OMXVENC_FILE_IN_SAVE__
FILE*       g_pFiletoEnc;
HI_U8*      g_pU;
HI_U8*      g_pV;

OMX_U32 g_u32CountNum = 0;
OMX_U32 g_u32CountNum_etb = 0;

#endif

#ifdef __OMXRGB_FILE_IN_SAVE__
FILE*       g_rgbtoSave;
#endif

///////////////////////////////////////////////
enum
{
    OMX_STATE_IDLE_PENDING	= 1,
    OMX_STATE_LOADING_PENDING	,
    OMX_STATE_INPUT_ENABLE_PENDING,
    OMX_STATE_OUTPUT_ENABLE_PENDING,
    OMX_STATE_OUTPUT_DISABLE_PENDING,
    OMX_STATE_INPUT_DISABLE_PENDING,
    OMX_STATE_OUTPUT_FLUSH_PENDING,
    OMX_STATE_INPUT_FLUSH_PENDING,
    OMX_STATE_PAUSE_PENDING,
    OMX_STATE_EXECUTE_PENDING
};
// Deferred callback identifiers
enum                                                          //用于表示各个 CMD 类型,以便在插入队列的时候判断要把消息的处理是要插入哪个队列
{
    OMX_GENERATE_COMMAND_DONE           = 0x1,
    OMX_GENERATE_FTB					= 0x2,
    OMX_GENERATE_ETB					= 0x3,
    OMX_GENERATE_COMMAND				= 0x4,
    OMX_GENERATE_EBD					= 0x5,
    OMX_GENERATE_FBD					= 0x6,
    OMX_GENERATE_FLUSH_INPUT_DONE		= 0x7,
    OMX_GENERATE_FLUSH_OUTPUT_DONE		= 0x8,
    OMX_GENERATE_START_DONE				= 0x9,
    OMX_GENERATE_PAUSE_DONE				= 0xA,
    OMX_GENERATE_RESUME_DONE			= 0xB,
    OMX_GENERATE_STOP_DONE				= 0xC,
    OMX_GENERATE_EOS_DONE				= 0xD,
    OMX_GENERATE_HARDWARE_ERROR			= 0xE,
    OMX_GENERATE_IMAGE_SIZE_CHANGE		= 0xF,
    OMX_GENERATE_CROP_RECT_CHANGE		= 0x10,
    OMX_GENERATE_UNUSED					= 0x11
};

/*编码类型列表: *//*元素说明:自定义协议名称，omx标准协议枚举的内容，原来unf接口对应的枚举*/
static const struct codec_info codec_trans_list[] =
{
    {OMX_ROLE_VIDEO_ENCODER_AVC, OMX_VIDEO_CodingAVC, HI_UNF_VCODEC_TYPE_H264},
    {OMX_ROLE_VIDEO_ENCODER_HEVC, OMX_VIDEO_CodingHEVC, HI_UNF_VCODEC_TYPE_HEVC}
};

static const struct codec_profile_level avc_profile_level_list[] =
{
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel1},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel2},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel21},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel3},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel31},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel32},
    {OMX_VIDEO_AVCProfileBaseline, OMX_VIDEO_AVCLevel41},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel1},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel2},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel21},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel3},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel31},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel32},
    {OMX_VIDEO_AVCProfileMain, OMX_VIDEO_AVCLevel41},
};

/* CodecType Relative */
/*fix here*/
static const struct codec_profile_level hevc_profile_level_list[] =
{
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel1},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel1},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel2},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel2},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel21},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel21},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel3},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel3},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel31},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel31},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel4},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel4},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCMainTierLevel41},
    {OMX_VIDEO_HEVCProfileMain,     OMX_VIDEO_HEVCHighTierLevel41},
};

enum
{
    OMX_VENC_YUV_420    = 0,
    OMX_VENC_YUV_422    = 1,
    OMX_VENC_YUV_444    = 2,
    OMX_VENC_YUV_NONE   = 3
};

enum
{
    OMX_VENC_V_U        = 0,
    OMX_VENC_U_V        = 1
};

enum
{
    OMX_VENC_STORE_SEMIPLANNAR  = 0,
    OMX_VENC_STORE_PACKAGE      = 1,
    OMX_VENC_STORE_PLANNAR      = 2
};

enum
{
    OMX_VENC_PACKAGE_UY0VY1     = 141/*0b10001101*/,
    OMX_VENC_PACKAGE_Y0UY1V     = 216/*0b11011000*/,
    OMX_VENC_PACKAGE_Y0VY1U     = 120/*0b01111000*/
};

#ifdef __VENC_DBG_DELAY_STAT__
////////////////////////////////////////////////////////////////////////  just for debug
static int write_output(void)
{
    int ret = 0;
    int i = 0;
    unsigned int diff1 = 0, diff2 = 0, diff3 = 0, diff4 = 0, diff5 = 0, diff6 = 0, total = 0;

    for (i = 0; i < 100; i++)
    {

        diff1 = (TmpTime[i][1].tv_sec == TmpTime[i][0].tv_sec) ? (TmpTime[i][1].tv_usec - TmpTime[i][0].tv_usec) : (TmpTime[i][1].tv_usec + 1000000 - TmpTime[i][0].tv_usec);
        diff2 = (TmpTime[i][2].tv_sec == TmpTime[i][1].tv_sec) ? (TmpTime[i][2].tv_usec - TmpTime[i][1].tv_usec) : (TmpTime[i][2].tv_usec + 1000000 - TmpTime[i][1].tv_usec);
        diff3 = (TmpTime[i][4].tv_sec == TmpTime[i][3].tv_sec) ? (TmpTime[i][4].tv_usec - TmpTime[i][3].tv_usec) : (TmpTime[i][4].tv_usec + 1000000 - TmpTime[i][3].tv_usec);
        diff4 = (TmpTime[i][5].tv_sec == TmpTime[i][4].tv_sec) ? (TmpTime[i][5].tv_usec - TmpTime[i][4].tv_usec) : (TmpTime[i][5].tv_usec + 1000000 - TmpTime[i][4].tv_usec);
        diff5 = (TmpTime[i][5].tv_sec == TmpTime[i][2].tv_sec) ? (TmpTime[i][5].tv_usec - TmpTime[i][2].tv_usec) : (TmpTime[i][5].tv_usec + 1000000 - TmpTime[i][2].tv_usec);
        //diff6=(TmpTime[i][3].tv_sec == TmpTime[i][1].tv_sec)?(TmpTime[i][3].tv_usec - TmpTime[i][1].tv_usec):(TmpTime[i][3].tv_usec+1000000-TmpTime[i][1].tv_usec);
        total = (TmpTime[i][5].tv_sec == TmpTime[i][0].tv_sec) ? (TmpTime[i][5].tv_usec - TmpTime[i][0].tv_usec) : (TmpTime[i][5].tv_usec + 1000000 - TmpTime[i][0].tv_usec);
    }

}
///////////////////////////////////////////////////////////////////////
#endif


static OMX_ERRORTYPE omx_check_param_videoAvc(OMX_VIDEO_PARAM_AVCTYPE* pAvcType)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_S32 bError = 0;

    bError |= (OMX_S32)(pAvcType->nSliceHeaderSpacing   != 0);
    bError |= (OMX_S32)(pAvcType->nBFrames              != 0);
    //bError |= (OMX_S32)(pAvcType->nRefFrames            != 1);
    bError |= (OMX_S32)(pAvcType->nRefIdx10ActiveMinus1 != 0);   //support B frame
    bError |= (OMX_S32)(pAvcType->nRefIdx11ActiveMinus1 != 0);   //support B frame
    bError |= (OMX_S32)(pAvcType->bEnableUEP            != OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->bEnableFMO            != OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->bEnableASO            != OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->bEnableRS             != OMX_FALSE);
    bError |= (OMX_S32)((pAvcType->eProfile              == OMX_VIDEO_AVCProfileExtended) || (pAvcType->eProfile > OMX_VIDEO_AVCProfileHigh));
    bError |= (OMX_S32)((pAvcType->nAllowedPictureTypes  & 0xfffffffc) || (pAvcType->nAllowedPictureTypes == OMX_VIDEO_PictureTypeP));
    //bError |= (OMX_S32)(pAvcType->bFrameMBsOnly         != OMX_TRUE);
    //bError |= (OMX_S32)(pAvcType->bMBAFF                != OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->eProfile == OMX_VIDEO_AVCProfileBaseline) ? (pAvcType->bEntropyCodingCABAC != OMX_FALSE) : (OMX_FALSE);
    //bError |= (OMX_S32)(pAvcType->bWeightedPPrediction  != OMX_FALSE);
    //bError |= (OMX_S32)(pAvcType->nWeightedBipredicitonMode != OMX_FALSE);
    //bError |= (pAvcType->bconstIpred               != OMX_TRUE);
    bError |= (OMX_S32)(pAvcType->bDirect8x8Inference != OMX_FALSE) ? (pAvcType->eProfile != OMX_VIDEO_AVCProfileHigh) : (OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->bDirectSpatialTemporal != OMX_FALSE);
    bError |= (OMX_S32)(pAvcType->nCabacInitIdc          != 0);
    //bError |= (OMX_S32)(pAvcType->eLoopFilterMode        != OMX_VIDEO_AVCLoopFilterEnable);

    if ((pAvcType->nAllowedPictureTypes == OMX_VIDEO_PictureTypeI) && (pAvcType->nPFrames != 0 ))
    {
        bError |= 1;
    }
    else if ((pAvcType->nAllowedPictureTypes == 0x3) && (pAvcType->nPFrames == 0 ))
    {
        bError |= 1;
    }
    /*switch(pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.h264Level)
    {
        case 10:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel1;
    	  break;
        case 20:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel2;
    	  break;
        case 21:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel21;
    	  break;
        case 30:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel3;
    	  break;
        case 31:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel31;
    	  break;
        case 32:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel32;
    	  break;
        case 41:
          pAvcType->eLevel = OMX_VIDEO_AVCLevel41;
    	  break;
    	default:
    	  DEBUG_PRINT_ERROR("Bad parm of Level %ld\n", (OMX_S32)pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.h264Level);
          pAvcType->eLevel = OMX_VIDEO_AVCLevelMax;
    	  break;
    }*/

    if (bError != 0)
    {
        ret = OMX_ErrorBadParameter;
    }

    return ret;
}



static OMX_ERRORTYPE omx_report_event(OMX_COMPONENT_PRIVATE* pcom_priv,
                                      OMX_IN OMX_EVENTTYPE event_type, OMX_IN OMX_U32 param1,
                                      OMX_IN OMX_U32 param2, OMX_IN OMX_PTR pdata)
{
    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(pcom_priv->m_cb.EventHandler == NULL);

    return pcom_priv->m_cb.EventHandler(pcom_priv->m_pcomp,
                                        pcom_priv->m_app_data, event_type,
                                        param1, param2, pdata);
}


static inline OMX_ERRORTYPE omx_report_error(OMX_COMPONENT_PRIVATE* pcom_priv,
        OMX_S32  error_type)
{
    return omx_report_event(
               pcom_priv, OMX_EventError, error_type, 0, NULL);
}


static OMX_BOOL port_full(OMX_COMPONENT_PRIVATE* pcom_priv, OMX_U32 port)
{
    OMX_PORT_PRIVATE* port_priv = &pcom_priv->m_port[port];
    OMX_U32 i;

    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (bit_absent(&port_priv->m_buf_cnt, i))
        {
            return OMX_FALSE;
        }
    }
    return OMX_TRUE;
}


static OMX_BOOL port_empty(OMX_COMPONENT_PRIVATE* pcom_priv, OMX_U32 port)
{
    OMX_PORT_PRIVATE* port_priv = &pcom_priv->m_port[port];
    OMX_U32 i;

    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (bit_present(&port_priv->m_buf_cnt, i))
        {
            return OMX_FALSE;
        }
    }
    return OMX_TRUE;
}


static OMX_BOOL ports_all_full(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    if (port_full(pcom_priv, INPUT_PORT_INDEX) != OMX_TRUE)
    { return OMX_FALSE; }

    return port_full(pcom_priv, OUTPUT_PORT_INDEX);
}


static OMX_BOOL ports_all_empty(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    if (port_empty(pcom_priv, INPUT_PORT_INDEX) != OMX_TRUE)
    { return OMX_FALSE; }

    return port_empty(pcom_priv, OUTPUT_PORT_INDEX);
}


static OMX_ERRORTYPE post_event(OMX_COMPONENT_PRIVATE* pcom_priv,
                                OMX_U32 param1, OMX_U32 param2, OMX_U8 id)                //压入队列，写入管道
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_S32 n = -1;
    pthread_mutex_lock(&pcom_priv->m_lock);
    if ((id == OMX_GENERATE_FTB) || (id == OMX_GENERATE_FBD))
    {
        push_entry(&pcom_priv->m_ftb_q, param1, param2, id);
    }
    else if ((id == OMX_GENERATE_ETB) || (id == OMX_GENERATE_EBD))
    {
        push_entry(&pcom_priv->m_etb_q, param1, param2, id);                       //param1:对应命令 ，param2:该命令所带参数
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


static OMX_ERRORTYPE fill_buffer_done(OMX_COMPONENT_PRIVATE* pcom_priv,
                                      OMX_BUFFERHEADERTYPE* buffer)
{
    OMX_ERRORTYPE ret;
    OMX_PORT_PRIVATE* port_priv = NULL;
    //venc_drv_context *drv_ctx;
    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    /*DEBUG_PRINT_STREAM("[FBD] omxhdr: %p,pBuffer: %p"
    		",nFilelen: %lu,nOffset:%lu\n",buffer,
    		buffer->pBuffer, buffer->nFilledLen, buffer->nOffset);
    DEBUG_PRINT_ERROR("XXX [FBD] pBuffer: %p,", buffer->pBuffer);*/

    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

    if (port_priv->m_port_flushing)
    {
        buffer->nFilledLen = 0;
    }

    port_priv->m_buf_pend_cnt--;

    //drv_ctx = &pcom_priv->drv_ctx;

    /*if( channel_release_stream(drv_ctx, buffer->pOutputPortPrivate) < 0)              //add by liminqi
    {
        DEBUG_PRINT_ERROR("[EBD] channel_release_stream faild!\n");
    	return OMX_ErrorUndefined;
    }*/

    if (!pcom_priv->m_cb.FillBufferDone)
    {
        DEBUG_PRINT_ERROR("[FBD] FillBufferDone callback NULL!\n");
        return OMX_ErrorUndefined;
    }

#ifdef __VENC_DBG_DELAY_STAT__
    if (ftb_done_id <= 100)
    {
        ret = gettimeofday (&TmpTime[ftb_done_id][5] , NULL);
        if (ret == 0)
        {
            ftb_done_id++;
        }
    }
    else if (flag)
    {
        write_output();
        flag = 0;
    }
#endif

#ifdef __OMXVENC_FILE_OUT_SAVE__
    if ((g_pFiletoSave != NULL) && (buffer->nFilledLen != 0))
    {
        fwrite(buffer->pBuffer, 1, buffer->nFilledLen, g_pFiletoSave);
    }
#endif
    DEBUG_PRINT_EFTB("[FBD] VirAddr = %p,flag = 0x%lx,cnt = %d\n",buffer->pBuffer,buffer->nFlags,g_ftb_cnt);
    ret = pcom_priv->m_cb.FillBufferDone(pcom_priv->m_pcomp, pcom_priv->m_app_data, buffer);

    return ret;
}


static OMX_ERRORTYPE empty_buffer_done(OMX_COMPONENT_PRIVATE* pcom_priv,
                                       OMX_BUFFERHEADERTYPE* buffer)
{
    OMX_ERRORTYPE ret;
    OMX_PORT_PRIVATE* port_priv = NULL;
    //venc_drv_context *drv_ctx;
    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);


    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

    port_priv->m_buf_pend_cnt--;

    buffer->nFilledLen = 0;

    //drv_ctx = &pcom_priv->drv_ctx;

    if (!pcom_priv->m_cb.EmptyBufferDone)
    {
        DEBUG_PRINT_ERROR("[EBD] EmptyBufferDone callback NULL!\n");
        return OMX_ErrorUndefined;
    }
#ifdef __VENC_DBG_DELAY_STAT__
    if (etb_done_id <= 100)
    {
        OMX_S32 ret = 0;
        ret = gettimeofday (&TmpTime[etb_done_id][2] , NULL);
        if (ret == 0)
        {
            etb_done_id++;
        }
    }
#endif
    DEBUG_PRINT_EFTB("[EBD] VirAddr = %p,flag = 0x%lx,num = %d\n",
               buffer->pBuffer,buffer->nFlags,g_etb_cnt);
    ret = pcom_priv->m_cb.EmptyBufferDone(pcom_priv->m_pcomp, pcom_priv->m_app_data, buffer);

    return ret;
}

#ifdef ANDROID

#ifdef __OMXVENC_FILE_IN_SAVE__
/////////////////////////////////// just for debug
typedef struct BMP
{
    uint8_t   BM[2];
    uint32_t  fsize;
    uint16_t  resv0;
    uint16_t  resv1;
    uint32_t  offset;
    uint32_t  resv2;
    uint32_t  w;
    uint32_t  h;
    uint16_t  resv3;
    uint16_t  resv4;
    uint32_t  resv5;
    uint32_t  datasize;
    uint32_t  resv6;
    uint32_t  resv7;
    uint32_t  resv8;
    uint32_t  resv9;
} BMP;

void bmp_save2(const char* filename, unsigned char* buffer, int w, int h, int bits)
{
    int x, y, v;
    FILE* f;
    char fname[40];
    snprintf(fname, sizeof(fname), "%s_%d.bmp", filename, g_u32CountNum_etb);
    BMP bmp;

    bmp.BM[0]   = 'B';
    bmp.BM[1]   = 'M';
    bmp.fsize = w * h * 4 + 54;
    bmp.resv0 = 0;
    bmp.resv1 = 0;
    bmp.offset = 54;
    bmp.resv2 = 0x28;
    bmp.w    = w;
    bmp.h    = h;
    bmp.resv3 = 0x01;
    bmp.resv4 = bits; //8,16,32
    bmp.resv5 = 0x00;
    bmp.datasize = w * h;
    bmp.resv6 = 0;
    bmp.resv7 = 0;
    bmp.resv8 = 0;
    bmp.resv9 = 0;

    f = fopen(fname, "wb");
    if (!f)
    {
        perror(fname);
        return;
    }
    fwrite(&bmp.BM, 2, 1, f);
    fwrite(&bmp.fsize, 4, 1, f);
    fwrite(&bmp.resv0, 2, 1, f);
    fwrite(&bmp.resv1, 2, 1, f);
    fwrite(&bmp.offset, 4, 1, f);
    fwrite(&bmp.resv2, 4, 1, f);
    fwrite(&bmp.w    , 4, 1, f);
    fwrite(&bmp.h    , 4, 1, f);
    fwrite(&bmp.resv3, 2, 1, f);
    fwrite(&bmp.resv4, 2, 1, f);
    fwrite(&bmp.resv5, 4, 1, f);
    fwrite(&bmp.datasize, 4, 1, f);
    fwrite(&bmp.resv6, 4, 1, f);
    fwrite(&bmp.resv7, 4, 1, f);
    fwrite(&bmp.resv8, 4, 1, f);
    fwrite(&bmp.resv9, 4, 1, f);

    for (x = h - 1; x >= 0; x--)
    { fwrite((uint8_t*)((uint8_t*)(buffer) + x * w * 4), w * 4, 1, f); }
    fclose(f);

}
#endif

/////////////////////////////////////

HI_U32 conver_format_forMetaData(HI_U32 MetaDataFormat, HI_U32 flag) //0:storeType 1:sample   2:packageSel
{
    HI_U32 RetFormat;

    switch (MetaDataFormat)
    {
        case HAL_PIXEL_FORMAT_RGBA_8888:
            if (flag == 0)
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            else if (flag == 1)
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            else
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            break;
        case HAL_PIXEL_FORMAT_RGBX_8888:
            if (flag == 0)
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            else if (flag == 1)
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            else
            {
                RetFormat = TDE2_COLOR_FMT_ABGR8888;
            }
            break;
        case HAL_PIXEL_FORMAT_YV12:    //planer 420 YCrCb
            if (flag == 0)
            {
                RetFormat = OMX_VENC_STORE_PLANNAR;
            }
            else if (flag == 1)
            {
                RetFormat = OMX_VENC_YUV_420;
            }
            else
            {
                RetFormat = 0;
            }
            break;
        case HAL_PIXEL_FORMAT_YCbCr_422_SP:
            if (flag == 0)
            {
                RetFormat = OMX_VENC_STORE_SEMIPLANNAR;
            }
            else if (flag == 1)
            {
                RetFormat = OMX_VENC_YUV_422;
            }
            else
            {
                RetFormat = OMX_VENC_V_U;
            }
            break;
        case HAL_PIXEL_FORMAT_YCrCb_420_SP:
            if (flag == 0)
            {
                RetFormat = OMX_VENC_STORE_SEMIPLANNAR;
            }
            else if (flag == 1)
            {
                RetFormat = OMX_VENC_YUV_420;
            }
            else
            {
                RetFormat = OMX_VENC_V_U;
            }
            break;
        default:
            DEBUG_PRINT_ERROR("the MetaData format(0x%x) can't found!! use the format = HAL_PIXEL_FORMAT_RGBA_8888!");
            RetFormat = TDE2_COLOR_FMT_ABGR8888;
            break;
    }

    return RetFormat;
}
#define TDE_CSCTABLE_SIZE 7
HI_U32 s_Rgb2YuvCsc[TDE_CSCTABLE_SIZE] =
{0x00810042, 0xffda0019, 0x0070ffb6, 0xffa20070, 0x0000ffee, 0x00000000, 0x02010010};

uint8_t omxvenc_extractGrallocData(void* data, venc_user_info* buffer, OMX_COMPONENT_PRIVATE* pcom_priv)
{
    TDE_HANDLE handle;
    HI_S32 s32Ret = 0;
    TDE2_SURFACE_S stForeGround;
    TDE2_SURFACE_S stDst;
    TDE2_RECT_S stForeGroundRect;
    TDE2_RECT_S stDstRect;
    TDE2_OPT_S stOpt ;
    HI_BOOL bBlock = HI_TRUE;
    HI_U32 u32TimeOut = 50;
    FILE* fp = NULL;
    HI_U32 retLen = 0;
    HI_U32* pCscTableVir;
    HI_MMZ_BUF_S stBuf = {0};

    OMX_U32 type = *(OMX_U32*)data;
    OMX_U8* UserVirAddr = NULL;

    buffer_handle_t imgBuffer = *(buffer_handle_t*)((uint8_t*)data + 4);
    private_handle_t* p_private_handle = (private_handle_t*)imgBuffer;

    DEBUG_PRINT("%s:p_private_handle = 0x%x,(st_size = %d),payaddr = 0x%x,offset = %d,width = %d,height = %d,size = %d,format=%d\n",
                __func__,
                p_private_handle, sizeof(private_handle_t), p_private_handle->ion_phy_addr, p_private_handle->offset,
                p_private_handle->width,
                p_private_handle->height, p_private_handle->size, p_private_handle->format);

#ifdef __OMXVENC_FILE_IN_SAVE__ 

    UserVirAddr = p_private_handle->base;//HI_MEM_Map(p_private_handle->ion_phy_addr, p_private_handle->size);
    if (NULL ==  UserVirAddr)
    {
        DEBUG_PRINT_ERROR("%s() HI_MEM_Map error!\n", __func__);
        return OMX_ErrorUndefined;
    }
#endif

    if (p_private_handle->format == HAL_PIXEL_FORMAT_RGBA_8888)
    {
        memset(&stOpt, 0, sizeof(TDE2_OPT_S));
        memset(&stForeGround, 0, sizeof(TDE2_SURFACE_S));
        memset(&stDst, 0, sizeof(TDE2_SURFACE_S));
    #ifdef __OMXRGB_FILE_IN_SAVE__
        if ((g_rgbtoSave != NULL) && (p_private_handle->size != 0))
            {
                fwrite(UserVirAddr, 1, p_private_handle->size, g_rgbtoSave);
            }
    #endif
        if (p_private_handle->ion_phy_addr == -1)   /*the rgb buffer not continuous*/
        {
#ifdef __OMXVENC_RGB_BUFFER__
            if ((pcom_priv->bufferaddr_RGB != 0) && (pcom_priv->bufferaddr_RGB != -1))
            {
                 DEBUG_PRINT("%s,%d,bufferaddr_RGB = %p, base_addr = %p,size = %d\n",__func__,__LINE__,
                    pcom_priv->bufferaddr_RGB,p_private_handle->base,p_private_handle->size);
                 memcpy(pcom_priv->bufferaddr_RGB,p_private_handle->base,p_private_handle->size);
            }
            else
            {
                DEBUG_PRINT_ERROR("%s() p_private_handle->ion_phy_addr = -1,the rgb buffer not continuous!\n", __func__);
                return OMX_ErrorUndefined;
            }
#else
            DEBUG_PRINT_ERROR("%s() p_private_handle->ion_phy_addr = -1,the rgb buffer not continuous!\n", __func__);
            return OMX_ErrorUndefined;
#endif
        }

        snprintf(stBuf.bufname, 16, "%s", "OMXVENC_CSC");
        stBuf.bufsize = 28;

        s32Ret = HI_MMZ_Malloc(&stBuf);
        if (0 != s32Ret)
        {
            DEBUG_PRINT_ERROR("%s:ERROR: HI_MMZ_Malloc Failed!! Ret:%ld\n", __func__, s32Ret);
            return -1;
        }

        buffer->user_buf.MetaDateFlag      = 0;
        buffer->ion_handle        = p_private_handle;

        buffer->user_buf.picWidth          = p_private_handle->width;
        buffer->user_buf.picHeight         = p_private_handle->height;

        buffer->user_buf.store_type        = OMX_VENC_STORE_PACKAGE;
        buffer->user_buf.sample_type       = OMX_VENC_YUV_422;
        buffer->user_buf.package_sel       = OMX_VENC_PACKAGE_UY0VY1;

        buffer->user_buf.strideY           = ALIGN_UP(buffer->user_buf.picWidth, 16) * 2;
        buffer->user_buf.data_len          = buffer->user_buf.picHeight * buffer->user_buf.strideY;
        buffer->user_buf.offset            = 64;

        /**************** set TDE process paramter **************/
        //input RGB
        stForeGround.u32PhyAddr   = (p_private_handle->ion_phy_addr != -1)? 
                                   p_private_handle->ion_phy_addr:pcom_priv->bufferaddr_Phy_RGB;
        stForeGround.enColorFmt   = conver_format_forMetaData(p_private_handle->format, 0);
        stForeGround.u32Width     = buffer->user_buf.picWidth ;
        stForeGround.u32Height    = buffer->user_buf.picHeight;
        stForeGround.u32Stride    = p_private_handle->stride * 4;

        stForeGroundRect.s32Xpos = 0;
        stForeGroundRect.s32Ypos = 0;
        stForeGroundRect.u32Height = stForeGround.u32Height;
        stForeGroundRect.u32Width = stForeGround.u32Width;

        //output YUV(package422 YUYV)
        stDst.u32PhyAddr   = buffer->user_buf.bufferaddr_Phy + buffer->user_buf.offset;   /*64 is the offset of struct header*/
        stDst.enColorFmt   = TDE2_COLOR_FMT_YCbCr422;
        stDst.u32Width     = buffer->user_buf.picWidth ;
        stDst.u32Height    = buffer->user_buf.picHeight;
        stDst.u32Stride    = ALIGN_UP(buffer->user_buf.picWidth, 16) * 2;

        stDstRect = stForeGroundRect;
        stOpt.stCscOpt.bICSCUserEnable  = 1;
        stOpt.stCscOpt.bICSCParamReload = 1;
        stOpt.stCscOpt.u32ICSCParamAddr = stBuf.phyaddr;

        memcpy(stBuf.user_viraddr, s_Rgb2YuvCsc, 28);

        HI_TDE2_Open();
        pcom_priv->hTDE = HI_TDE2_BeginJob();
        if ((pcom_priv->hTDE == HI_NULL) || (pcom_priv->hTDE == HI_INVALID_HANDLE))
        {
            DEBUG_PRINT_ERROR("%s,TDE failed to get Handle(ret = 0x%x)%d\n", __func__, pcom_priv->hTDE);
            return OMX_ErrorHardware;
        }
        s32Ret = HI_TDE2_Bitblit((TDE_HANDLE)pcom_priv->hTDE, HI_NULL, HI_NULL, &stForeGround, &stForeGroundRect,
                                 &stDst, &stDstRect, &stOpt);

        HI_TDE2_EndJob(pcom_priv->hTDE, HI_FALSE, HI_TRUE, 50);
        pcom_priv->hTDE = HI_INVALID_HANDLE;
        HI_TDE2_Close();

#ifdef __OMXVENC_FILE_IN_SAVE__

        if ((g_pFiletoEnc != NULL) && (buffer->user_buf.data_len != 0))
        {
            fwrite(buffer->user_buf.bufferaddr + buffer->user_buf.offset, 1, buffer->user_buf.picWidth * buffer->user_buf.picHeight * 2, g_pFiletoEnc);
        }
#endif

        s32Ret = HI_MMZ_Free(&stBuf);
        if (HI_FAILURE == s32Ret)
        {
            DEBUG_PRINT_ERROR("%s,%d,HI_MMZ_Free failed!!\n", __func__, __LINE__);
        }
    }
    else if (p_private_handle->format == HAL_PIXEL_FORMAT_YCrCb_420_SP)  /*sp420_vu*/
    {

        venc_metadata_buf* pMetaData = &(buffer->user_buf.stMetaData);
        pMetaData->bufferaddr_Phy = p_private_handle->ion_phy_addr + p_private_handle->offset;
        //pMetaData->bufferaddr     = -1;//UserVirAddr + p_private_handle->offset;  //vir addr
        pMetaData->buffer_size    = p_private_handle->size;

        buffer->user_buf.MetaDateFlag      = 1;
        buffer->ion_handle        = p_private_handle;

        buffer->user_buf.picWidth          = p_private_handle->width;
        buffer->user_buf.picHeight         = p_private_handle->height;

        buffer->user_buf.store_type        = conver_format_forMetaData(p_private_handle->format, 0);
        buffer->user_buf.sample_type       = conver_format_forMetaData(p_private_handle->format, 1);
        buffer->user_buf.package_sel       = conver_format_forMetaData(p_private_handle->format, 2);
        buffer->user_buf.data_len          = p_private_handle->height * p_private_handle->stride * 3 / 2;
        buffer->user_buf.strideY           = p_private_handle->stride;//ALIGN_UP(buffer->picWidth,16);
        buffer->user_buf.strideC           = buffer->user_buf.strideY;
        buffer->user_buf.offset            = 0;
        buffer->user_buf.offset_YC         = p_private_handle->height * p_private_handle->stride;  //YC分量的偏移

        DEBUG_PRINT("%s:%d,payaddr = 0x%x,width = %d,height = %d,store_type = %d,sample_type = %d,package_sel = %d\n",
                    __func__, __LINE__,
                    pMetaData->bufferaddr_Phy, buffer->user_buf.picWidth, buffer->user_buf.picHeight,
                    buffer->user_buf.store_type, buffer->user_buf.sample_type, buffer->user_buf.package_sel);
        DEBUG_PRINT("%s:%d,strideY= %d,strideC = %d , offset_YC = %d\n",
                    __func__, __LINE__,
                    buffer->user_buf.strideY, buffer->user_buf.strideC, buffer->user_buf.offset_YC);

#ifdef __OMXVENC_FILE_IN_SAVE__

        if ((g_pFiletoEnc != NULL) && (buffer->user_buf.data_len != 0))
        {
           // fwrite(pMetaData->bufferaddr, 1, buffer->data_len, g_pFiletoEnc);
        }
#endif
    }
    else
    {
        DEBUG_PRINT_ERROR("%s,%d,format = %d not support storeMateDate now!!\n", __func__, __LINE__, p_private_handle->format);
    }

    return 0;
}

#endif




static OMX_ERRORTYPE use_buffer_internal(OMX_COMPONENT_PRIVATE* pcom_priv,
        OMX_INOUT OMX_BUFFERHEADERTYPE** omx_bufhdr,
        OMX_IN OMX_PTR app_data,
        OMX_IN OMX_U32 port,
        OMX_IN OMX_U32 bytes,
        OMX_IN OMX_U8* buffer)
{
    OMX_ERRORTYPE error = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pomx_buf = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;
    OMX_U32 i = 0;
    OMX_S32 ret = -1;
    OMX_U32 buf_size = 0, buf_cnt = 0;

    HI_U32 Phyaddr, BufSize;

    venc_user_info* pvenc_buf = NULL;          //??

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    port_priv = &pcom_priv->m_port[port];
    buf_cnt   = port_priv->port_def.nBufferCountActual;
    buf_size  = port_priv->port_def.nBufferSize;

    if (bytes != buf_size)
    {
        DEBUG_PRINT_ERROR("[UB] Error: buf size invalid, bytes = %lu, buf_size = %lu\n", bytes, buf_size);
        return OMX_ErrorBadParameter;
    }

    /* find an idle buffer slot */
    for (i = 0; i < buf_cnt; i++)
    {
        if (bit_absent(&port_priv->m_buf_cnt, i))
        { break; }
    }

    if (i >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[UB] Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }

    //DEBUG_PRINT("[UB]~~~~~~~~~~~~~~~~~~~ use buffer(addr) = 0x%x \n",buffer);
    ret = HI_MMZ_GetPhyaddr(buffer, &Phyaddr, &BufSize);
    if (ret < 0)
    {
        DEBUG_PRINT_ERROR("%s() HI_MMZ_GetPhyaddr error!\n", __func__);
        return OMX_ErrorHardware;
    }

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));

    pvenc_buf = (venc_user_info*)malloc(sizeof(venc_user_info));

    if (!pomx_buf || !pvenc_buf)
    {
        DEBUG_PRINT_ERROR("[UB] Error: alloc omx buffer head failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit;
    }
    memset(pomx_buf, 0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvenc_buf, 0, sizeof(venc_user_info));

    /* skip buffer allocation, direct bind */
    pvenc_buf->user_buf.dir         = (enum venc_port_dir)port_priv->port_def.eDir;
    pvenc_buf->user_buf.client_data    = (HI_VIRT_ADDR_T)(long)pomx_buf;

    pvenc_buf->ion_handle     = NULL;
    pvenc_buf->bufferaddr	  = buffer;
    pvenc_buf->user_buf.bufferaddr_Phy = Phyaddr;
    pvenc_buf->user_buf.buffer_size    = bytes;

    pvenc_buf->user_buf.data_len  	  = 0;
    pvenc_buf->user_buf.offset	  = 0;
    pvenc_buf->user_buf.pmem_fd	  = 0;
    pvenc_buf->user_buf.mmaped_size	  = bytes;

    if (pvenc_buf->user_buf.dir == PORT_DIR_INPUT)
    {
        pvenc_buf->user_buf.offset_YC   = ALIGN_UP((pcom_priv->m_port[0].port_def.format.video.nFrameWidth * pcom_priv->m_port[0].port_def.format.video.nFrameHeight), 16);
    }

    if (PORT_DIR_OUTPUT == pvenc_buf->user_buf.dir)
    {
        if (venc_mmap_kerel_vir(&(pcom_priv->drv_ctx), &(pvenc_buf->user_buf)) < 0)
        {
            DEBUG_PRINT_ERROR("[AB]Error: venc_mmap_kerel_vir failed\n");
            error =  OMX_ErrorUndefined;
            goto error_exit;
        }
    }
    /*DEBUG_PRINT("XXX [UseBuf] pBuffer_virAddr: 0x%x, len = %d, buf_heard(=client_data) :0x%x,vir2phy_offset = 0x%x\n", pvenc_buf->bufferaddr, pvenc_buf->buffer_size,pvenc_buf->client_data,pvenc_buf->vir2phy_offset);*/

#if 0
    if (channel_bind_buffer(&pcom_priv->drv_ctx, pvdec_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[UB] Error: vdec bind user buf failed\n");
        error = OMX_ErrorUndefined;
        goto error_exit;
    }
#endif

    /* init OMX buffer head */
    CONFIG_VERSION_SIZE(pomx_buf , OMX_BUFFERHEADERTYPE);
    pomx_buf->pBuffer				= pvenc_buf->bufferaddr;  //用户态虚拟地址
    pomx_buf->nAllocLen			    = pvenc_buf->user_buf.buffer_size;  //申请长度
    pomx_buf->nOffset				= 0;
    pomx_buf->pAppPrivate			= app_data;

    if (INPUT_PORT_INDEX == port)
    {
        pomx_buf->pOutputPortPrivate	= NULL;
        pomx_buf->pInputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nInputPortIndex    = port_priv->port_def.nPortIndex;
    }
    else
    {
        pomx_buf->pInputPortPrivate	= NULL;
        pomx_buf->pOutputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nOutputPortIndex   = port_priv->port_def.nPortIndex;
    }

    *omx_bufhdr = pomx_buf;
    bit_set(&port_priv->m_buf_cnt, i);

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_venc_bufhead[i] = pvenc_buf;

    DEBUG_PRINT_STREAM("[UB] Use %s buffer %d success: phyaddr = %x, size = %d\n",
                       (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i, pvenc_buf->user_buf.bufferaddr_Phy, pvenc_buf->user_buf.buffer_size);

    return OMX_ErrorNone;

error_exit:
    free(pvenc_buf);
    free(pomx_buf);
    pvenc_buf = NULL;
    pomx_buf = NULL;

    DEBUG_PRINT_ERROR("[UB] Use %s buffer %d failed\n", (port == OUTPUT_PORT_INDEX) ? "out" : "in", (int)i);

    return error;
}

#ifdef ANDROID
static OMX_ERRORTYPE use_android_native_buffer_internal(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_INOUT OMX_BUFFERHEADERTYPE** omx_bufhdr,
    OMX_IN OMX_PTR app_data,
    OMX_IN OMX_U32 port,
    OMX_IN OMX_U32 bytes,
    OMX_IN OMX_U8* p_handle)
{
    OMX_U32 i = 0;
    OMX_S32 ret = -1;
    OMX_U32 buf_size = 0, buf_cnt = 0;
    OMX_ERRORTYPE error = OMX_ErrorNone;
    OMX_PORT_PRIVATE* port_priv = NULL;
    OMX_BUFFERHEADERTYPE* pomx_buf = NULL;
    OMX_U8* UserVirAddr = NULL;

    private_handle_t* p_private_handle = (private_handle_t*)p_handle;

    venc_user_info* pvenc_buf = NULL;          //??

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(p_handle == NULL);

    DEBUG_PRINT("p_private_handle = 0x%x,payaddr = %d,width = %d,height = %d\n",
                p_private_handle, p_private_handle->ion_phy_addr,
                p_private_handle->width,
                p_private_handle->height);
    OMX_CHECK_ARG_RETURN(p_private_handle->ion_phy_addr == 0);

    port_priv = &pcom_priv->m_port[port];
    buf_size = port_priv->port_def.nBufferSize;
    buf_cnt = port_priv->port_def.nBufferCountActual;

    if (bytes != buf_size)
    {
        DEBUG_PRINT_ERROR("[UB] Error: buf size invalid, bytes = %ld, buf_size = %ld\n", bytes, buf_size);
        return OMX_ErrorBadParameter;
    }

    /* find an idle buffer slot */
    /* find an idle buffer slot */
    for (i = 0; i < buf_cnt; i++)
    {
        if (bit_absent(&port_priv->m_buf_cnt, i))
        { break; }
    }

    if (i >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[UB] Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }

    UserVirAddr = p_private_handle->base;//HI_MEM_Map(p_private_handle->ion_phy_addr, bytes);
    if (NULL ==  UserVirAddr)
    {
        DEBUG_PRINT_ERROR("%s() HI_MEM_Map error!\n", __func__);
        return OMX_ErrorUndefined;
    }

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));
    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[UB] Error: alloc omx buffer pomx_buf failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }

    pvenc_buf = (venc_user_info*)malloc(sizeof(venc_user_info));
    if (!pvenc_buf)
    {
        DEBUG_PRINT_ERROR("[UB] Error: alloc omx buffer pvenc_buf failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit1;
    }

    memset(pomx_buf,  0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvenc_buf, 0, sizeof(venc_user_info));

    /* skip buffer allocation, direct bind */
    pvenc_buf->user_buf.dir         = (enum venc_port_dir)port_priv->port_def.eDir;
    pvenc_buf->user_buf.client_data    = (HI_VIRT_ADDR_T)pomx_buf;

    pvenc_buf->ion_handle     = p_handle;
    pvenc_buf->bufferaddr	  = UserVirAddr;
    pvenc_buf->user_buf.bufferaddr_Phy = p_private_handle->ion_phy_addr;//Phyaddr;
    pvenc_buf->user_buf.buffer_size    = bytes;

    pvenc_buf->user_buf.data_len  	  = 0;
    pvenc_buf->user_buf.offset	      = 0;
    pvenc_buf->user_buf.pmem_fd	      = 0;
    pvenc_buf->user_buf.mmaped_size	  = bytes;

    if (pvenc_buf->user_buf.dir == INPUT_PORT_INDEX)
    {
        pvenc_buf->user_buf.offset_YC   = ALIGN_UP((pcom_priv->m_port[0].port_def.format.video.nFrameWidth * pcom_priv->m_port[0].port_def.format.video.nFrameHeight), 16);
    }

    if (PORT_DIR_OUTPUT == pvenc_buf->user_buf.dir)
    {
        if (venc_mmap_kerel_vir(&(pcom_priv->drv_ctx), &(pvenc_buf->user_buf)) < 0)
        {
            DEBUG_PRINT_ERROR("[AB]Error: venc_mmap_kerel_vir failed\n");
            error =  OMX_ErrorUndefined;
            goto error_exit2;
        }
    }

    /* init OMX buffer head */
    CONFIG_VERSION_SIZE(pomx_buf , OMX_BUFFERHEADERTYPE);
    pomx_buf->pBuffer				= p_handle; //pvenc_buf->bufferaddr;   //not 用户态虚拟地址
    pomx_buf->nSize				    = sizeof(OMX_BUFFERHEADERTYPE);
    pomx_buf->nVersion.nVersion   	= OMX_VERSION;
    pomx_buf->nAllocLen			    = bytes;  //申请长度
    pomx_buf->nOffset				= 0;
    pomx_buf->pAppPrivate			= app_data;

    if (INPUT_PORT_INDEX == port)
    {
        pomx_buf->pOutputPortPrivate	= NULL;
        pomx_buf->pInputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nInputPortIndex    = port_priv->port_def.nPortIndex;
    }
    else
    {
        pomx_buf->pInputPortPrivate	   = NULL;
        pomx_buf->pOutputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nOutputPortIndex   = port_priv->port_def.nPortIndex;
    }

    *omx_bufhdr = pomx_buf;
    bit_set(&port_priv->m_buf_cnt, i);

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_venc_bufhead[i] = pvenc_buf;

    DEBUG_PRINT_STREAM("[UB] Use %s buffer %d success: phyaddr = %x, size = %d\n",
                       (port == INPUT_PORT_INDEX) ? "in" : "out", (int)i, pvenc_buf->user_buf.bufferaddr_Phy, pvenc_buf->user_buf.buffer_size);

    return OMX_ErrorNone;

error_exit2:
    free(pvenc_buf);
error_exit1:
    free(pomx_buf);
error_exit0:
    //HI_MEM_Unmap(UserVirAddr);

    DEBUG_PRINT_ERROR("[UB] Use %s buffer %d failed\n", (port == OUTPUT_PORT_INDEX) ? "out" : "in", (int)i);

    return error;
}
#endif

static OMX_ERRORTYPE allocate_buffer_internal(OMX_COMPONENT_PRIVATE* pcom_priv,
        OMX_INOUT OMX_BUFFERHEADERTYPE** omx_bufhdr,
        OMX_IN OMX_PTR app_data,
        OMX_IN OMX_U32 port,
        OMX_IN OMX_U32 bytes)
{
    OMX_ERRORTYPE error = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pomx_buf = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_U32 i = 0;
    OMX_U32 buf_size = 0, buf_cnt = 0;

    venc_user_info* pvenc_buf = NULL;          //??

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);

    port_priv = &pcom_priv->m_port[port];
    buf_cnt   = port_priv->port_def.nBufferCountActual;

    /*be careful that the bytes isn't match with the allac size in fact*/
    if (port == INPUT_PORT_INDEX)
    {
        buf_size  = FRAME_SIZE(port_priv->port_def.format.video.nFrameHeight, port_priv->port_def.format.video.nStride) + 64;
    }
    else
    {
        buf_size  = port_priv->port_def.nBufferSize;
        /* check alloc bytes */
        if (bytes < buf_size)
        {
            DEBUG_PRINT_ERROR("[AB]Error: Requested %ld, espected %ld\n",
                              (OMX_S32)bytes, (OMX_S32)port_priv->port_def.nBufferSize);
            return OMX_ErrorBadParameter;
        }
    }

    /* find an idle buffer slot */
    for (i = 0; i < buf_cnt; i++)
    {
        if (bit_absent(&port_priv->m_buf_cnt, i))
        { break; }
    }

    if (i >= buf_cnt)
    {
        DEBUG_PRINT_ERROR("[AB]Error: no Free buffer heads found\n");
        return OMX_ErrorInsufficientResources;
    }

    pomx_buf = (OMX_BUFFERHEADERTYPE*)malloc(sizeof(OMX_BUFFERHEADERTYPE));

    pvenc_buf = (venc_user_info*)malloc(sizeof(venc_user_info));

    if (!pomx_buf || !pvenc_buf)
    {
        DEBUG_PRINT_ERROR("[AB]Error:alloc omx buffer head failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }
    memset(pomx_buf, 0, sizeof(OMX_BUFFERHEADERTYPE));
    memset(pvenc_buf, 0, sizeof(venc_user_info));

    pvenc_buf->user_buf.dir         = (enum venc_port_dir)port_priv->port_def.eDir;

    if (alloc_contigous_buffer(buf_size, port_priv->port_def.nBufferAlignment, pvenc_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[AB]Error: alloc_contigous_buffer failed\n");
        error =  OMX_ErrorUndefined;
        goto error_exit0;
    }

    pvenc_buf->user_buf.client_data = (HI_VIRT_ADDR_T)(long)pomx_buf;
    if (pvenc_buf->user_buf.dir == PORT_DIR_INPUT)
    {
        pvenc_buf->user_buf.offset_YC   = ALIGN_UP((pcom_priv->m_port[0].port_def.format.video.nFrameWidth * pcom_priv->m_port[0].port_def.format.video.nFrameHeight), 16);
    }

    if (PORT_DIR_OUTPUT == pvenc_buf->user_buf.dir)
    {
        if (venc_mmap_kerel_vir(&(pcom_priv->drv_ctx), &(pvenc_buf->user_buf)) < 0)
        {
            DEBUG_PRINT_ERROR("[AB]Error: venc_mmap_kerel_vir failed\n");
            error =  OMX_ErrorUndefined;
            goto error_exit1;
        }
    }
    /*DEBUG_PRINT("XXX [AllocBuf] pBuffer_virAddr: 0x%x, len = %d, buf_heard(=client_data) :0x%x,vir2phy_offset = 0x%x\n", pvenc_buf->bufferaddr, pvenc_buf->buffer_size,pvenc_buf->client_data,pvenc_buf->vir2phy_offset);*/


#if 0       //需要把端口与buffer绑定?
    /* bind this buffer to vdec driver */
    if (channel_bind_buffer(&pcom_priv->drv_ctx, pvenc_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[AB]Error: Bind buffer failed\n");
        error =  OMX_ErrorUndefined;
        goto error_exit1;
    }
#endif

    /* init buffer head */
    CONFIG_VERSION_SIZE(pomx_buf, OMX_BUFFERHEADERTYPE);
    pomx_buf->pBuffer				= pvenc_buf->bufferaddr;  //用户态虚拟地址
    pomx_buf->nAllocLen			    = pvenc_buf->user_buf.buffer_size;  //申请长度
    pomx_buf->nOffset				= 0;
    pomx_buf->pAppPrivate			= app_data;

    if (port == INPUT_PORT_INDEX)
    {
        pomx_buf->pOutputPortPrivate = NULL;
        pomx_buf->pInputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nInputPortIndex    = port_priv->port_def.nPortIndex;
    }
    else
    {
        pomx_buf->pInputPortPrivate  = NULL;
        pomx_buf->pOutputPortPrivate	= (OMX_PTR)pvenc_buf;
        pomx_buf->nOutputPortIndex   = port_priv->port_def.nPortIndex;
    }

    *omx_bufhdr = pomx_buf;

    /* mark buffer to be allocated, used */
    bit_set(&port_priv->m_usage_bitmap, i);
    bit_set(&port_priv->m_buf_cnt, i);

    port_priv->m_omx_bufhead[i] = pomx_buf;
    port_priv->m_venc_bufhead[i] = pvenc_buf;

    //DEBUG_PRINT("[AB] alloc buffer_size = %d,bytes = %d!\n", pomx_buf->nAllocLen,bytes);

    DEBUG_PRINT("alloc %s buffer %ld success\n",
                (port == INPUT_PORT_INDEX) ? "input" : "output", (OMX_S32)i);
    return OMX_ErrorNone;

error_exit1:
    free_contigous_buffer(pvenc_buf);
error_exit0:
    free(pvenc_buf);
    free(pomx_buf);
    pomx_buf = NULL;
    pvenc_buf = NULL;
    DEBUG_PRINT_ERROR("[AB]alloc %s buffer %ld failed\n",
                      (port == OUTPUT_PORT_INDEX) ? "output" : "input", (OMX_S32)i);
    return error;
}


static OMX_ERRORTYPE free_buffer_internal(OMX_COMPONENT_PRIVATE* pcom_priv,
        OMX_IN OMX_U32 port, OMX_IN OMX_BUFFERHEADERTYPE* omx_bufhdr)
{
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_U32 i;

    venc_user_info* puser_buf = NULL;

    port_priv = &pcom_priv->m_port[port];

    /* santity check */
    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (omx_bufhdr == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_def.nBufferCountActual)
    {
        DEBUG_PRINT_ERROR("no buffers found for address[%p]", omx_bufhdr);
        return OMX_ErrorBadParameter;
    }

    if (bit_absent(&port_priv->m_buf_cnt, i))
    {
        DEBUG_PRINT_ERROR("buffer 0x%p not been usaged?", omx_bufhdr);
        return OMX_ErrorBadParameter;
    }
    else
    {
        bit_clear(&port_priv->m_buf_cnt, i);
    }

    if (OUTPUT_PORT_INDEX == port)
    {
        puser_buf = (venc_user_info*)(omx_bufhdr->pOutputPortPrivate);
    }
    else
    {
        puser_buf = (venc_user_info*)(omx_bufhdr->pInputPortPrivate);
    }

#if 0       //取消 buffer对通道的绑定 /* unbind venc user buffer */
    if (channel_unbind_buffer(&pcom_priv->drv_ctx, puser_buf) < 0)
    {
        DEBUG_PRINT_ERROR("[FB] unbind buffer failed\n");
        return OMX_ErrorUndefined;
    }
#endif

    if (port_priv->port_def.bPopulated)
    {
        port_priv->port_def.bPopulated = OMX_FALSE;
    }

    if (bit_present(&port_priv->m_usage_bitmap, i))
    {
        if (PORT_DIR_OUTPUT == puser_buf->user_buf.dir)
        {
            if (venc_unmmap_kerel_vir(&(pcom_priv->drv_ctx), &(puser_buf->user_buf)) < 0)
            {
                DEBUG_PRINT_ERROR("[AB]Error: venc_unmmap_kerel_vir failed\n");
                return OMX_ErrorHardware;
            }
        }/**/

        free_contigous_buffer(puser_buf);
        bit_clear(&port_priv->m_usage_bitmap, i);
    }

    free(puser_buf);
    free(omx_bufhdr);
    puser_buf = NULL;
    omx_bufhdr = NULL;

    port_priv->m_omx_bufhead[i] = NULL;
    port_priv->m_venc_bufhead[i] = NULL;

    DEBUG_PRINT("[FB] %s exit!\n", __func__);
    return OMX_ErrorNone;
}


static OMX_ERRORTYPE omx_flush_port( OMX_COMPONENT_PRIVATE* pcom_priv,
                                     OMX_U32 port)
{
    OMX_PORT_PRIVATE* port_priv = NULL;

    if (port == OUTPUT_PORT_INDEX || port == OMX_ALL)
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (port == INPUT_PORT_INDEX || port == OMX_ALL)
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (channel_flush_port(&pcom_priv->drv_ctx, port) < 0)
    {
        DEBUG_PRINT_ERROR("ioctl flush port failed\n");
        return OMX_ErrorHardware;
    }

    return OMX_ErrorNone;
}


static void return_outbuffers(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    OMX_PORT_PRIVATE* port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
    OMX_U32 param1 = 0, param2 = 0, ident = 0;
    pthread_mutex_lock(&pcom_priv->m_lock);
    while (get_q_size(&pcom_priv->m_ftb_q) > 0)
    {
        pop_entry(&pcom_priv->m_ftb_q, &param1, &param2, &ident);
        if (ident == OMX_GENERATE_FTB )
        {
            port_priv->m_buf_pend_cnt++;
        }
        fill_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE*)param1);
    }
    pthread_mutex_unlock(&pcom_priv->m_lock);
}


static void return_inbuffers(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    OMX_PORT_PRIVATE* port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
    OMX_U32 param1 = 0, param2 = 0, ident = 0;
    pthread_mutex_lock(&pcom_priv->m_lock);
    while (get_q_size(&pcom_priv->m_etb_q) > 0)
    {
        pop_entry(&pcom_priv->m_etb_q, &param1, &param2, &ident);
        if (ident == OMX_GENERATE_ETB)
        {
            port_priv->m_buf_pend_cnt++;
        }
        empty_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE*)param1);
    }
    pthread_mutex_unlock(&pcom_priv->m_lock);
}

static OMX_ERRORTYPE get_supported_profile_level(OMX_COMPONENT_PRIVATE* pcomp_priv, OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileLevelType)
{
    OMX_U32 max_num;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    const struct codec_profile_level* pinfo = NULL;

    OMX_CHECK_ARG_RETURN(pcomp_priv == NULL);
    OMX_CHECK_ARG_RETURN(profileLevelType == NULL);

    if (profileLevelType->nPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("%s(): should be queries on Input port only (%lu)\n", __func__, profileLevelType->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    /* FIXME : profile & level may not correct! */
    if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_AVC, sizeof(OMX_ROLE_VIDEO_ENCODER_AVC)))
    {
        pinfo = avc_profile_level_list;
        max_num = COUNTOF(avc_profile_level_list);
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_HEVC, sizeof(OMX_ROLE_VIDEO_ENCODER_HEVC)))
    {
        pinfo = hevc_profile_level_list;
        max_num = COUNTOF(hevc_profile_level_list);
    }
    else
    {
        DEBUG_PRINT_ERROR("%s(): no profile & level found for %lu(%s)\n", __func__, profileLevelType->nPortIndex, (OMX_STRING)pcomp_priv->m_role);
        return OMX_ErrorUndefined;
    }

    if (profileLevelType->nIndex >= max_num)
    {
        DEBUG_PRINT_ERROR("%s(): ProfileIndex(%lu) exceed!\n", __func__, profileLevelType->nIndex);
        return OMX_ErrorNoMore;
    }

    pinfo += profileLevelType->nIndex;
    profileLevelType->eProfile = pinfo->profile;
    profileLevelType->eLevel   = pinfo->level;

    /*DEBUG_PRINT("%s(),profileIndex = %lu,profile = %lu,level = %lu\n",
    	__func__,profileLevelType->nIndex,profileLevelType->eProfile,profileLevelType->eLevel);*/
    return ret;
}

static OMX_ERRORTYPE get_current_profile_level(OMX_COMPONENT_PRIVATE* pcomp_priv, OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileLevelType)
{
    venc_chan_cfg* pVencChnCfg = NULL;
    OMX_CHECK_ARG_RETURN(pcomp_priv == NULL);
    OMX_CHECK_ARG_RETURN(profileLevelType == NULL);

    if (profileLevelType->nPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("%s(): should be queries on Input port only (%lu)\n", __func__, profileLevelType->nPortIndex);
        return  OMX_ErrorBadPortIndex;
    }

    pVencChnCfg = &(pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg);
    
    /* FIXME : profile & level may not correct! */
    if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_AVC, sizeof(OMX_ROLE_VIDEO_ENCODER_AVC)))
    {
        switch (pVencChnCfg->VencUnfAttr.enVencProfile)
        {
            case HI_UNF_H264_PROFILE_BASELINE:
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileBaseline;
                break;
        
            case HI_UNF_H264_PROFILE_MAIN:
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMain;
                break;
        
            case HI_UNF_H264_PROFILE_HIGH:
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileHigh;
                break;
        
            default:
                DEBUG_PRINT_ERROR("Bad parm of VencProfile %ld\n", (OMX_S32)pVencChnCfg->VencUnfAttr.enVencProfile);
                profileLevelType->eProfile = OMX_VIDEO_AVCProfileMax;
                break;
        }
        
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_HEVC, sizeof(OMX_ROLE_VIDEO_ENCODER_HEVC)))
    {
        profileLevelType->eProfile = OMX_VIDEO_HEVCProfileMain;
    }
    else
    {
        DEBUG_PRINT_ERROR("%s(): component roles error (%s)\n", __func__, pcomp_priv->m_role);        
        return  OMX_ErrorBadParameter;
    }
        
    pVencChnCfg->h264Level = channel_get_current_level(pVencChnCfg->VencUnfAttr.u32Width, pVencChnCfg->VencUnfAttr.u32Height);
    switch (pVencChnCfg->h264Level)
    {
        case 10:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel1;
            break;
    
        case 20:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel2;
            break;
    
        case 21:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel21;
            break;
    
        case 30:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel3;
            break;
    
        case 31:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel31;
            break;
    
        case 32:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel32;
            break;
    
        case 41:
            profileLevelType->eLevel = OMX_VIDEO_AVCLevel41;
            break;
    
        default:
            DEBUG_PRINT_ERROR("Bad parm of Level %ld\n", (OMX_S32)pVencChnCfg->h264Level);
            profileLevelType->eLevel = OMX_VIDEO_AVCLevelMax;
            break;
    }
    
    /*DEBUG_PRINT("%s(),profileIndex = %lu,profile = %lu,level = %lu\n",
    	__func__,profileLevelType->nIndex,profileLevelType->eProfile,profileLevelType->eLevel);*/
    return OMX_ErrorNone;
}

static OMX_ERRORTYPE set_current_profile_level(OMX_COMPONENT_PRIVATE* pcomp_priv, OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileLevelType)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    venc_chan_cfg* pVencChnCfg = NULL;
    OMX_CHECK_ARG_RETURN(pcomp_priv == NULL);
    OMX_CHECK_ARG_RETURN(profileLevelType == NULL);

    if (profileLevelType->nPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("%s(): should be queries on Input port only (%lu)\n", __func__, profileLevelType->nPortIndex);
        return  OMX_ErrorBadPortIndex;
    }

    if (1 == pcomp_priv->drv_ctx.venc_chan_attr.state) /* just can change profile when venc channel idle*/
    {
        ret = OMX_ErrorUnsupportedSetting;
        omx_report_error(pcomp_priv, ret);
        return ret;
    }

    pVencChnCfg = &(pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg);
    
    if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_AVC, sizeof(OMX_ROLE_VIDEO_ENCODER_AVC)))
    {
        switch (profileLevelType->eProfile)
        {
            case OMX_VIDEO_AVCProfileBaseline:
                pVencChnCfg->VencUnfAttr.enVencProfile = HI_UNF_H264_PROFILE_BASELINE;
                break;

            case OMX_VIDEO_AVCProfileMain:
                pVencChnCfg->VencUnfAttr.enVencProfile = HI_UNF_H264_PROFILE_MAIN;
                break;

            case OMX_VIDEO_AVCProfileHigh:
                pVencChnCfg->VencUnfAttr.enVencProfile = HI_UNF_H264_PROFILE_HIGH;
                break;

            default:
                DEBUG_PRINT_ERROR("Bad parm of VencProfile %ld\n", (OMX_S32)profileLevelType->eProfile);
                
                return OMX_ErrorBadParameter;
        }       
        
    }
    else if (!strncmp((OMX_STRING)pcomp_priv->m_role, OMX_ROLE_VIDEO_ENCODER_HEVC, sizeof(OMX_ROLE_VIDEO_ENCODER_HEVC)))
    {
        switch (profileLevelType->eProfile)
        {
            case OMX_VIDEO_HEVCProfileMain:
                pVencChnCfg->VencUnfAttr.enVencProfile = HI_UNF_H264_PROFILE_MAIN;  //fix me
                break;

            default:
                DEBUG_PRINT_ERROR("Bad parm of VencProfile %ld\n", (OMX_S32)profileLevelType->eProfile);
                
                return OMX_ErrorBadParameter;
        }          
    }
    else
    {
        DEBUG_PRINT_ERROR("%s(): component roles error (%s)\n", __func__, pcomp_priv->m_role); 
        
        return  OMX_ErrorBadParameter;
    }

    if ((pcomp_priv->m_state >= OMX_StateIdle) && (pcomp_priv->m_state <= OMX_StateWaitForResources))
    {
        if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
        {
            DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
            omx_report_error(pcomp_priv, OMX_ErrorUnsupportedSetting);
            return OMX_ErrorUnsupportedSetting;
        }
    } 

    /*DEBUG_PRINT("%s(),profileIndex = %lu,profile = %lu,level = %lu\n",
    	__func__,profileLevelType->nIndex,profileLevelType->eProfile,profileLevelType->eLevel);*/
    return OMX_ErrorNone;
}


#if 0
static OMX_ERRORTYPE update_picture_info(OMX_COMPONENT_PRIVATE* pcomp_priv,
        OMX_U32 width, OMX_U32 height)
{
    OMX_U32 align_width, align_height;
    OMX_PORT_PRIVATE* port_priv = NULL;

    align_width = ALIGN_UP(width, 16);                 //多少对齐?
    align_height = ALIGN_UP(height, 16);

    if ((align_height > MAX_FRAME_HEIGHT) || (align_width > MAX_FRAME_WIDTH))
    {
        DEBUG_PRINT_ERROR("video frame w/h exceed!\n");
        return OMX_ErrorUnsupportedSetting;
    }

    port_priv = &pcomp_priv->m_port[INPUT_PORT_INDEX];
    port_priv->pic_info.frame_width	    = width;
    port_priv->pic_info.frame_height	= height;
    port_priv->pic_info.stride			= align_width;

    /*pcomp_priv->pic_info.scan_lines		= align_height;
    pcomp_priv->pic_info.crop_top 	    = 0;
    pcomp_priv->pic_info.crop_left	    = 0;
    pcomp_priv->pic_info.crop_width    	= width;
    pcomp_priv->pic_info.crop_height	= height;*/

    port_priv->port_pro.buffer_size		= FRAME_SIZE(align_width, align_height);
    return OMX_ErrorNone;
}

static OMX_ERRORTYPE check_port_parameter(OMX_COMPONENT_PRIVATE* pcomp_priv,
        OMX_U32 width, OMX_U32 height)
{

    return OMX_ErrorNone;
}
#endif


static OMX_U32 OMX_Convert_PIX_Format_StoreType(OMX_COLOR_FORMATTYPE Format, OMX_U32 flag)
{
    OMX_U32 Ret = HI_SUCCESS;

    switch (Format)
    {
        case OMX_COLOR_FormatYUV420Planar:
            Ret = OMX_VENC_STORE_PLANNAR;
            break;
        case OMX_COLOR_FormatYUV420SemiPlanar:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
        case OMX_QCOM_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
        case OMX_COLOR_FormatYUV422Planar:
            Ret = OMX_VENC_STORE_PLANNAR;
            break;
        case OMX_COLOR_FormatYUV422SemiPlanar:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
        case OMX_COLOR_FormatYCbYCr:
            Ret = OMX_VENC_STORE_PACKAGE;
            break;
        case OMX_COLOR_FormatYCrYCb :
            Ret = OMX_VENC_STORE_PACKAGE;
            break;
        case OMX_COLOR_FormatCbYCrY:
            Ret = OMX_VENC_STORE_PACKAGE;
            break;
        case OMX_COLOR_FormatCrYCbY:
            Ret = OMX_VENC_STORE_PACKAGE;
            break;
        case OMX_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
        case OMX_COLOR_FormatYVU422SemiPlanar:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
        case OMX_COLOR_FormatYVU420Planar:
            Ret = OMX_VENC_STORE_PLANNAR;
            break;
        default:
            Ret = OMX_VENC_STORE_SEMIPLANNAR;
            break;
    }

    return Ret;
}

static OMX_U32 OMX_Convert_PIX_Format_SampleType(OMX_COLOR_FORMATTYPE Format, OMX_U32 flag)
{
    OMX_U32 Ret = HI_SUCCESS;

    switch (Format)
    {
        case OMX_COLOR_FormatYUV420Planar:
            Ret = OMX_VENC_YUV_420;
            break;
        case OMX_COLOR_FormatYUV420SemiPlanar:
            Ret = OMX_VENC_YUV_420;
            break;
        case OMX_QCOM_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_YUV_420;
            break;				
        case OMX_COLOR_FormatYUV422Planar:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatYUV422SemiPlanar:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatYCbYCr:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatYCrYCb :
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatCbYCrY:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatCrYCbY:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_YUV_420;
            break;
        case OMX_COLOR_FormatYVU422SemiPlanar:
            Ret = OMX_VENC_YUV_422;
            break;
        case OMX_COLOR_FormatYVU420Planar:
            Ret = OMX_VENC_YUV_420;
            break;
        default:
            Ret = OMX_VENC_YUV_420;
            break;
    }

    return Ret;
}

static OMX_U32 OMX_Convert_PIX_Format_PackageSel(OMX_COLOR_FORMATTYPE Format, OMX_U32 flag)
{
    OMX_U32 Ret = HI_SUCCESS;

    switch (Format)
    {
        case OMX_COLOR_FormatYUV420Planar:
            Ret = OMX_VENC_U_V;
            break;
        case OMX_COLOR_FormatYUV420SemiPlanar:
            Ret = OMX_VENC_U_V;
            break;
        case OMX_QCOM_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_V_U;
            break;
        case OMX_COLOR_FormatYUV422Planar:
            Ret = OMX_VENC_U_V;
            break;
        case OMX_COLOR_FormatYUV422SemiPlanar:
            Ret = OMX_VENC_U_V;
            break;
        case OMX_COLOR_FormatYCbYCr:
            Ret = OMX_VENC_PACKAGE_Y0UY1V;
            break;
        case OMX_COLOR_FormatYCrYCb :
            Ret = OMX_VENC_PACKAGE_Y0VY1U;
            break;
        case OMX_COLOR_FormatCbYCrY:
            Ret = OMX_VENC_PACKAGE_UY0VY1;
            break;
        case OMX_COLOR_FormatCrYCbY:
            Ret = OMX_VENC_PACKAGE_UY0VY1;      //QS
            break;
        case OMX_COLOR_FormatYVU420SemiPlanar:
            Ret = OMX_VENC_V_U;
            break;
        case OMX_COLOR_FormatYVU422SemiPlanar:
            Ret = OMX_VENC_V_U;
            break;
        case OMX_COLOR_FormatYVU420Planar:
            Ret = OMX_VENC_V_U;
            break;
        default:
            Ret = OMX_VENC_U_V;
            break;
    }

    return Ret;
}

static OMX_U32 OMX_Convert_PIX_Format(OMX_COLOR_FORMATTYPE Format, OMX_U32 flag)
{
    OMX_U32 Ret = HI_SUCCESS;

    if (flag == 0) /*YUVStoreType*/
    {
        Ret = OMX_Convert_PIX_Format_StoreType(Format, flag);
    }
    else if (flag == 1) /*YUVSampleType*/
    {
        Ret = OMX_Convert_PIX_Format_SampleType(Format, flag);
    }
    else if (flag == 2) /*PackageSel*/
    {
        Ret = OMX_Convert_PIX_Format_PackageSel(Format, flag);
    }
    else
    {
        Ret = HI_FALSE;
    }

    return Ret;
}


static void convert_format_info(OMX_IN OMX_BUFFERHEADERTYPE *pomx_buf, 
                    venc_user_info *puser_buf, OMX_PORT_PRIVATE *port_priv)
{
    puser_buf->user_buf.picWidth     = port_priv->port_def.format.video.nFrameWidth;
    puser_buf->user_buf.picHeight    = port_priv->port_def.format.video.nFrameHeight;
    puser_buf->user_buf.store_type   = OMX_Convert_PIX_Format(port_priv->port_def.format.video.eColorFormat, 0);
    puser_buf->user_buf.sample_type  = OMX_Convert_PIX_Format(port_priv->port_def.format.video.eColorFormat, 1);
    puser_buf->user_buf.package_sel  = OMX_Convert_PIX_Format(port_priv->port_def.format.video.eColorFormat, 2);
    puser_buf->user_buf.data_len	 = pomx_buf->nFilledLen;
    puser_buf->user_buf.offset       = pomx_buf->nOffset;

#ifdef __OMXVENC_FILE_IN_SAVE__
    if (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar)
    {
        char* YuvFile = puser_buf->bufferaddr;
        char* U_temp = g_pU;
        char* V_temp = g_pV;
        int i_temp = 0;
        int Y_size = (puser_buf->user_buf.picWidth * puser_buf->user_buf.picHeight);
        YuvFile += Y_size;
        for (i_temp = 0; i_temp < (Y_size / 4); i_temp++)
        {
            memcpy(U_temp, YuvFile, sizeof(HI_U8));
            YuvFile++;
            U_temp++;
            memcpy(V_temp, YuvFile, sizeof(HI_U8));
            YuvFile++;
            V_temp++;
        }
        if ((g_pFiletoEnc != NULL) && (pomx_buf->nFilledLen != 0))
        {
            fwrite(puser_buf->bufferaddr, 1, Y_size, g_pFiletoEnc);
            fwrite(g_pU, 1, Y_size / 4, g_pFiletoEnc);
            fwrite(g_pV, 1, Y_size / 4, g_pFiletoEnc);
        }
    }
	else if (port_priv->port_def.format.video.eColorFormat == OMX_QCOM_COLOR_FormatYVU420SemiPlanar)
	{
        char* YuvFile = puser_buf->bufferaddr;
        char* U_temp = g_pU;
        char* V_temp = g_pV;
        int i_temp = 0;
        int Y_size = (puser_buf->user_buf.picWidth * puser_buf->user_buf.picHeight);
        YuvFile += Y_size;
        for (i_temp = 0; i_temp < (Y_size / 4); i_temp++)
        {
            memcpy(V_temp, YuvFile, sizeof(HI_U8));
            YuvFile++;
            V_temp++;
            memcpy(U_temp, YuvFile, sizeof(HI_U8));
            YuvFile++;
            U_temp++;					
        }
        if ((g_pFiletoEnc != NULL) && (pomx_buf->nFilledLen != 0))
        {
            fwrite(puser_buf->bufferaddr, 1, Y_size, g_pFiletoEnc);
            fwrite(g_pU, 1, Y_size / 4, g_pFiletoEnc);
            fwrite(g_pV, 1, Y_size / 4, g_pFiletoEnc);
        }			
	}
    else if (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYUV420Planar)
    {
        fwrite(puser_buf->bufferaddr, 1, puser_buf->user_buf.data_len, g_pFiletoEnc);
    }
#endif

    if ((port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar)
        || (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYVU420SemiPlanar)
        || (port_priv->port_def.format.video.eColorFormat == OMX_QCOM_COLOR_FormatYVU420SemiPlanar))   //QS!!
    {
        puser_buf->user_buf.strideY   = ALIGN_UP(port_priv->port_def.format.video.nFrameWidth, 16);
        puser_buf->user_buf.strideC   = ALIGN_UP(port_priv->port_def.format.video.nFrameWidth, 16);
        puser_buf->user_buf.offset_YC = puser_buf->user_buf.strideY * port_priv->port_def.format.video.nFrameHeight;
    }
    else if ((port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYUV420Planar)
             || (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYVU420Planar))
    {
        puser_buf->user_buf.strideY = ALIGN_UP(port_priv->port_def.format.video.nFrameWidth, 16);
        puser_buf->user_buf.strideC = ALIGN_UP((port_priv->port_def.format.video.nFrameWidth / 2), 16);

        if (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYUV420Planar)
        {
            puser_buf->user_buf.offset_YC  = puser_buf->user_buf.strideY * port_priv->port_def.format.video.nFrameHeight;
            puser_buf->user_buf.offset_YCr = puser_buf->user_buf.offset_YC + puser_buf->user_buf.strideC * port_priv->port_def.format.video.nFrameHeight / 2;
        }
        else
        {
            puser_buf->user_buf.offset_YC  = puser_buf->user_buf.strideY * port_priv->port_def.format.video.nFrameHeight
                                           + puser_buf->user_buf.strideC * port_priv->port_def.format.video.nFrameHeight / 2;
            puser_buf->user_buf.offset_YCr = puser_buf->user_buf.strideY * port_priv->port_def.format.video.nFrameHeight;

        }
    }
    else if ((port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYCbYCr)
             || (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatYCrYCb)
             || (port_priv->port_def.format.video.eColorFormat == OMX_COLOR_FormatCbYCrY))
    {
        puser_buf->user_buf.strideY   = ALIGN_UP((port_priv->port_def.format.video.nFrameWidth * 2), 16);
        puser_buf->user_buf.strideC   = 0;
        puser_buf->user_buf.offset_YC = 0;

    }
}

static OMX_ERRORTYPE empty_this_buffer_proxy(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_IN OMX_BUFFERHEADERTYPE* pomx_buf)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    venc_user_info* puser_buf = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;
    int  ret_val = 0;
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[FTB]Proxy: bad parameter:null PTR\n");
        return OMX_ErrorBadParameter;
    }

    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[ETB]Proxy: bad parameters\n");
        ret = OMX_ErrorBadParameter;
        goto empty_error;
    }
#ifdef ENABLE_BUFFER_LOCK
    ret_val = sem_trywait(&pcom_priv->m_buf_lock);
    if(0 != ret_val)
    {
        DEBUG_PRINT_ERROR("[ETB]Proxy: give up the frame (pts = %lld)!!\n",pomx_buf->nTimeStamp);
        return pcom_priv->m_cb.EmptyBufferDone(pcom_priv->m_pcomp, pcom_priv->m_app_data, pomx_buf);
    }
#endif

    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

    if ((port_priv->m_port_flushing == OMX_TRUE)
        || (port_priv->m_port_reconfig == OMX_TRUE))
    {
        DEBUG_PRINT_ERROR("[ETB]Proxy: port flushing or disabled\n");
#if 0
        ret = OMX_ErrorIncorrectStateOperation;
        goto empty_error;
#endif
        pomx_buf->nFilledLen = 0;
        post_event(pcom_priv, (OMX_U32)pomx_buf, VENC_DONE_SUCCESS, OMX_GENERATE_EBD);
        return OMX_ErrorNone;                                  //add by l00228308

    }

    ///////////////////////////////////////////////////////////////////////    must be change to venc
    if (!pomx_buf->pInputPortPrivate)
    {
        DEBUG_PRINT_ERROR("[ETB]Proxy: invalid user buffer!\n");
        ret =  OMX_ErrorUndefined;
        goto empty_error;
    }

    puser_buf = (venc_user_info*)pomx_buf->pInputPortPrivate;

#ifdef ANDROID
    if (pcom_priv->m_store_metadata_buf == HI_TRUE)
    {
        if (pomx_buf->nFilledLen != 8)
        {
            if (pomx_buf->nFilledLen != 0)
            {
                DEBUG_PRINT("MetaData buffer is wrong size! "
                            "(got %lu bytes, expected 8)", pomx_buf->nFilledLen);
                ret =  OMX_ErrorUndefined;
                goto empty_error;
            }
        }

        if (pomx_buf->nFilledLen != 0)
        {
            omxvenc_extractGrallocData(pomx_buf->pBuffer + pomx_buf->nOffset, puser_buf, pcom_priv);
        }
        else
        {
            puser_buf->user_buf.picWidth     = port_priv->port_def.format.video.nFrameWidth;
            puser_buf->user_buf.picHeight    = port_priv->port_def.format.video.nFrameHeight;
            puser_buf->user_buf.store_type   = OMX_VENC_STORE_PACKAGE;
            puser_buf->user_buf.sample_type  = OMX_VENC_YUV_422;
            puser_buf->user_buf.package_sel  = OMX_VENC_PACKAGE_Y0UY1V;
            puser_buf->user_buf.data_len	    = pomx_buf->nFilledLen;
            puser_buf->user_buf.offset       = pomx_buf->nOffset;
        }
        // TODO: Verify/convert pixel format enum
    }
#endif

    if (pcom_priv->m_store_metadata_buf != OMX_TRUE)
    {
        convert_format_info(pomx_buf, puser_buf, port_priv);
    }

    puser_buf->user_buf.timestamp0   = (HI_U32)((HI_U64)pomx_buf->nTimeStamp & 0x00000000ffffffff) ;//pomx_buf->nTimeStamp / 1e3;
    puser_buf->user_buf.timestamp1   = (HI_U32)((HI_U64)pomx_buf->nTimeStamp >> 32) ;
    puser_buf->user_buf.flags        = pomx_buf->nFlags;


    if (puser_buf->user_buf.flags & OMX_BUFFERFLAG_EOS )
    {
        g_end_pts[0] = puser_buf->user_buf.timestamp0;
        g_end_pts[1] = puser_buf->user_buf.timestamp1;
	 if (puser_buf->user_buf.data_len)
	 {
	      g_IsEosFull = 1;
	 }
	 else
	 {
	      g_IsEosFull = 0;
        }

        DEBUG_PRINT_EFTB("[ETB]get EOS buffer !PTS = %d, %d, len= %d", g_end_pts[0],g_end_pts[1],puser_buf->user_buf.data_len);
    }

    g_etb_cnt++;
    DEBUG_PRINT_EFTB("[ETB] VirAddr = %p,flag = 0x%lx,pts = %lld,cnt = %d\n",pomx_buf->pBuffer,
        pomx_buf->nFlags,pomx_buf->nTimeStamp,g_etb_cnt);

    DEBUG_PRINT("%s(),%d, nFilledLen:%d,nOffset:%d, timestamp0 = %d,timestamp1 = %d,bufferaddr_Phy = 0x%x\n", __func__, __LINE__,
                puser_buf->user_buf.data_len , puser_buf->user_buf.offset, puser_buf->user_buf.timestamp0, \
                puser_buf->user_buf.timestamp1, puser_buf->user_buf.bufferaddr_Phy);
    DEBUG_PRINT("%s(),%d, strideY:%d,strideC:%d, offsetYC = %d,offsetYCr= %d\n", __func__, __LINE__,
                puser_buf->user_buf.strideY , puser_buf->user_buf.strideC, puser_buf->user_buf.offset_YC, puser_buf->user_buf.offset_YCr);

    if (channel_queue_Image(&pcom_priv->drv_ctx, &(puser_buf->user_buf)) < 0)      //change to queueFrame
    {
        /*Generate an async error and move to invalid state*/
        DEBUG_PRINT_ERROR("[ETB]proxy: queue_Image failed\n");
        ret = OMX_ErrorUndefined;
        goto empty_error;
    }

#ifdef __VENC_DBG_DELAY_STAT__
    if (etb_x_id < 100)
    {
        OMX_S32 ret = 0;
        ret = gettimeofday (&TmpTime[etb_x_id][1] , NULL);
        if (ret == 0)
        {
            etb_x_id++;
        }
    }
#endif

    /////////////////////////////////////////////////////////////////////////         end
    port_priv->m_buf_pend_cnt++;

    /*DEBUG_PRINT("[ETB] %s() success,bufheard :0x%x\n", __func__,pomx_buf);*/
    return OMX_ErrorNone;

empty_error:
    post_event(pcom_priv, (OMX_U32)pomx_buf, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_EBD);
    return ret;
}


static OMX_ERRORTYPE  fill_this_buffer_proxy(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_IN OMX_BUFFERHEADERTYPE* pomx_buf)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    venc_user_info* puser_buf = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[FTB]Proxy: bad parameter:null PTR\n");
        return OMX_ErrorBadParameter;
    }

    if (!pomx_buf)
    {
        DEBUG_PRINT_ERROR("[FTB]Proxy: bad parameter\n");
        ret = OMX_ErrorBadParameter;
        goto fill_error;
    }

    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];

    if ((port_priv->m_port_flushing == OMX_TRUE) ||
        (port_priv->m_port_reconfig == OMX_TRUE))
    {
        DEBUG_PRINT("[FTB]Proxy: port is flushing or reconfig\n");
        pomx_buf->nFilledLen = 0;
        post_event(pcom_priv, (OMX_U32)pomx_buf, (OMX_U32)VENC_DONE_SUCCESS, OMX_GENERATE_FBD);
        return OMX_ErrorNone;
    }

    ///////////////////////////////////////////////////////////////////////////////  must be change
    puser_buf = (venc_user_info*)pomx_buf->pOutputPortPrivate;
    if (!puser_buf)
    {
        DEBUG_PRINT_ERROR("[FTB]Proxy: invalid user buf\n");
        ret = OMX_ErrorUndefined;
        goto fill_error;
    }
    puser_buf->user_buf.data_len = 0;
    pomx_buf->nFilledLen = 0;

    g_ftb_cnt++;
    DEBUG_PRINT_EFTB("[FTB] VirAddr = %p,flag = 0x%lx,cnt = %d\n",pomx_buf->pBuffer,pomx_buf->nFlags,g_ftb_cnt);
    if (channel_queue_stream(&pcom_priv->drv_ctx, &(puser_buf->user_buf)) < 0)
    {
        DEBUG_PRINT_ERROR("[FTB]Proxy: fill_stream_buffer failed\n");
        return OMX_ErrorHardware;
    }
    ///////////////////////////////////////////////////////////////////////////////////// end

#ifdef __VENC_DBG_DELAY_STAT__
    if (ftb_x_id <= 100)
    {
        ret = gettimeofday (&TmpTime[ftb_x_id][4] , NULL);
        if (ret == 0)
        {
            ftb_x_id++;
        }
    }
#endif

    port_priv->m_buf_pend_cnt++;

    DEBUG_PRINT("[FTB]Proxy: fill this buffer success!\n");
    return OMX_ErrorNone;

fill_error:
    post_event(pcom_priv, (OMX_U32)pomx_buf, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_FBD);
    return ret;
}


static void message_process_case_output_done (OMX_COMPONENT_PRIVATE *pcom_priv, venc_msginfo *venc_msg)
{
    OMX_BUFFERHEADERTYPE *pomx_buf = NULL;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_U32 i = 0;
    venc_user_buf *puser_buf = NULL;

    DEBUG_PRINT("%s,VENC resp output done\n", __func__);
    puser_buf = &venc_msg->buf;
    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
    if (puser_buf->client_data == 0)
    {
        DEBUG_PRINT_ERROR(" %s,venc resp out buffer invalid\n", __func__);
        post_event(pcom_priv, 0, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_FBD);

        return;
    }

    pomx_buf = (OMX_BUFFERHEADERTYPE*)(long)puser_buf->client_data;

    /* check buffer validate*/
    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (pomx_buf == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_def.nBufferCountActual)
    {
        DEBUG_PRINT_ERROR("%s[%d]no buffers found for address[%p]", __func__, __LINE__, pomx_buf);
        post_event(pcom_priv, 0, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_FBD);

        return;
    }

    DEBUG_PRINT_STREAM("[FBD] venc resp out buffer %p,"
                       " buferaddr_phy:%x, data_len:%ld\n", pomx_buf,
                       puser_buf->bufferaddr_Phy, (OMX_S32)puser_buf->data_len);

    {
        HI_U64 u64Pts0 = (HI_U64)puser_buf->timestamp0;
        HI_U64 u64Pts1 = (HI_U64)puser_buf->timestamp1;
        HI_U64 u64Pts = ((u64Pts1 << 32) & 0xffffffff00000000LL) | u64Pts0;
        
        pomx_buf->nFilledLen = puser_buf->data_len;
        pomx_buf->nFlags     = (OMX_U32)puser_buf->flags;
        pomx_buf->nTimeStamp = (OMX_TICKS)u64Pts;
    }

    if ((puser_buf->timestamp0 == g_end_pts[0]) && (puser_buf->timestamp1 == g_end_pts[1]))
    {
        pomx_buf->nFlags |= OMX_BUFFERFLAG_EOS;
        g_end_pts[0] = 0xffffffff;
        g_end_pts[1] = 0xffffffff;
        if (!g_IsEosFull)
        {
            pomx_buf->nFilledLen = 0;
        }
        DEBUG_PRINT_STREAM("[FBD]report EOS buffer !PTS = %d, %d, len= %d", puser_buf->timestamp0,puser_buf->timestamp1,pomx_buf->nFilledLen);
    }
    post_event(pcom_priv, (OMX_U32)pomx_buf, venc_msg->status_code, OMX_GENERATE_FBD);
}

static OMX_S32 message_process (OMX_COMPONENT_PRIVATE*  pcom_priv, void* message)
{
    OMX_BUFFERHEADERTYPE* pomx_buf = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_U32 i = 0;

    venc_user_buf* puser_buf = NULL;
    venc_msginfo* venc_msg = NULL;
    //struct image_size *pimg = NULL;
    //struct crop_rect *pcrop = NULL;

    if (!pcom_priv || !message)
    {
        DEBUG_PRINT_ERROR("async_message_process() invalid param\n");
        return -1;
    }

    venc_msg = (venc_msginfo*)message;

    switch (venc_msg->msgcode)
    {
        case VENC_MSG_RESP_FLUSH_INPUT_DONE:
            post_event(pcom_priv, 0, venc_msg->status_code, OMX_GENERATE_FLUSH_INPUT_DONE);

            if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_DISABLE_PENDING))
            {
                post_event(pcom_priv, (OMX_U32)OMX_CommandPortDisable, (OMX_U32)OUTPUT_PORT_INDEX,
                           OMX_GENERATE_COMMAND_DONE);
            }
            break;

        case VENC_MSG_RESP_FLUSH_OUTPUT_DONE:
            post_event(pcom_priv,
                       0, venc_msg->status_code, OMX_GENERATE_FLUSH_OUTPUT_DONE);

            if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_DISABLE_PENDING))
            {
                post_event(pcom_priv,
                           (OMX_U32)OMX_CommandPortDisable, (OMX_U32)INPUT_PORT_INDEX,
                           OMX_GENERATE_COMMAND_DONE);
            }
            break;

        case VENC_MSG_RESP_START_DONE:
            post_event(pcom_priv,
                       0, venc_msg->status_code, OMX_GENERATE_START_DONE);
            break;

        case VENC_MSG_RESP_STOP_DONE:
            post_event(pcom_priv,
                       0, venc_msg->status_code, OMX_GENERATE_STOP_DONE);
            break;

        case VENC_MSG_RESP_PAUSE_DONE:
            post_event(pcom_priv,
                       0, venc_msg->status_code, OMX_GENERATE_PAUSE_DONE);
            break;

        case VENC_MSG_RESP_RESUME_DONE:
            post_event(pcom_priv,
                       0, venc_msg->status_code, OMX_GENERATE_RESUME_DONE);
            break;

        case VENC_MSG_RESP_INPUT_DONE:
            puser_buf = &venc_msg->buf;
            port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

            if (puser_buf->client_data == 0)
            {
                DEBUG_PRINT_ERROR("venc resp buf struct invalid\n");
                post_event(pcom_priv, 0, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_EBD);
                break;
            }

            pomx_buf = (OMX_BUFFERHEADERTYPE*)(long)puser_buf->client_data;
            for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
            {
                if (pomx_buf == port_priv->m_omx_bufhead[i])
                {
                    break;
                }
            }

            if (i >= port_priv->port_def.nBufferCountActual)
            {
                DEBUG_PRINT_ERROR("buffers[%p] no found", pomx_buf);
                post_event(pcom_priv,
                           0, (OMX_U32)VENC_DONE_FAILED, OMX_GENERATE_EBD);
                break;
            }

            DEBUG_PRINT("[EBD]venc resp in buffer %p,buferaddr:%p\n",
                        pomx_buf, pomx_buf->pBuffer);

            post_event(pcom_priv,
                       (OMX_U32)pomx_buf, (OMX_U32)VENC_DONE_SUCCESS, OMX_GENERATE_EBD);
            break;

        case VENC_MSG_RESP_OUTPUT_DONE:
            message_process_case_output_done (pcom_priv, venc_msg);
            break;

        default:
            DEBUG_PRINT_ERROR("msg 0x%08x not process\n",
                              venc_msg->msgcode);
            break;
    }

    return 0;
}


static void* message_thread(void* input)
{
    OMX_COMPONENT_PRIVATE* pcom_priv = (OMX_COMPONENT_PRIVATE*)input;
    OMX_S32 error_code = 0;
    venc_msginfo msginfo;

    memset(&msginfo, 0, sizeof(venc_msginfo));
    DEBUG_PRINT("Message thread start!\n");

    while (!pcom_priv->msg_thread_exit)
    {
        memset(&msginfo, 0, sizeof(venc_msginfo));
        //DEBUG_PRINT("[%s][A]: msginfo.msgcode = 0x%08x",__func__,msginfo.msgcode);
        error_code = channel_get_msg(&pcom_priv->drv_ctx, &msginfo);
        //DEBUG_PRINT("[%s][B]: msginfo.msgcode = 0x%08x",__func__,msginfo.msgcode);
        if (error_code == HI_FAILURE)
        {
            DEBUG_PRINT("[%s]:get msg failed:%ld", __func__, error_code);
            continue;
        }
        message_process(pcom_priv, &msginfo);
    }

    DEBUG_PRINT("Message thread exit!\n");
    return NULL;
}


static OMX_ERRORTYPE generate_command_done(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_U32 param1, OMX_U32 param2 )
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);
    OMX_CHECK_ARG_RETURN(pcom_priv->m_cb.EventHandler == NULL);

    switch (param1)
    {
        case OMX_CommandStateSet:
            pcom_priv->m_state = (OMX_STATETYPE)param2;
            ret = omx_report_event(pcom_priv,
                                   OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandPortEnable:
            DEBUG_PRINT("OMX_CommandPortEnable complete\n");
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            if (port_priv->m_port_reconfig)
            {
                DEBUG_PRINT("clear flags for port reconfig!\n");
                port_priv->m_port_reconfig = OMX_FALSE;
            }

            ret = omx_report_event(pcom_priv,
                                   OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandPortDisable:
            DEBUG_PRINT("OMX_CommandPortDisable complete\n");
            ret = omx_report_event(pcom_priv,
                                   OMX_EventCmdComplete, param1, param2, NULL);
            break;

        case OMX_CommandFlush:

        default:
            DEBUG_PRINT_ERROR("unknown genereate event %lu\n", param1);
            ret = OMX_ErrorUndefined;
            break;
    }
    return ret;
}


static OMX_ERRORTYPE handle_command_state_set_loaded(
    OMX_COMPONENT_PRIVATE *pcom_priv,
    OMX_STATETYPE state, OMX_U32 *sem_posted, OMX_U32 *pflag)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 flag = 1;
#ifdef __OMXVENC_RGB_BUFFER__ 
    HI_MMZ_BUF_S stBuf_RGB;
    HI_UNF_VENC_CHN_ATTR_S *pstVencCfg;
#endif

    /*  Loaded to idle */
    if (state == OMX_StateIdle)
    {
        if (channel_create(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("ERROR: channel create failed!\n");
            ret = OMX_ErrorHardware;

            return ret;
        }

#ifdef __OMXVENC_RGB_BUFFER__
        pstVencCfg = &pcom_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;
        snprintf(stBuf_RGB.bufname, 16, "%s", "OMXVENC_RGB");
        stBuf_RGB.bufsize = ALIGN_UP(pstVencCfg->u32Width, 256) * pstVencCfg->u32Height * 4;
        if (pcom_priv->m_store_metadata_buf)
        {
            if (HI_MMZ_Malloc(&stBuf_RGB) != 0)
            {
                DEBUG_PRINT_ERROR("ERROR: channel create RGB buffer for StoreMetaData failed!\n");
			    ret = OMX_ErrorHardware;

			    return ret;
            }
            pcom_priv->bufferaddr_Phy_RGB = stBuf_RGB.phyaddr;
            pcom_priv->bufferaddr_RGB     = stBuf_RGB.user_viraddr;
            pcom_priv->buffer_size_RGB    = stBuf_RGB.bufsize;
        } 
#endif

        if (ports_all_full(pcom_priv) ||
            (pcom_priv->m_port[0].port_def.bEnabled == OMX_FALSE &&
             pcom_priv->m_port[1].port_def.bEnabled == OMX_FALSE))
        {
            DEBUG_PRINT_STATE("Loaded-->Idle\n");
        }
        else
        {
            DEBUG_PRINT_STATE("Loaded-->Idle-Pending\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
            flag = 0;
        }
    }
    /*  Loaded to Loaded */
    else if (state == OMX_StateLoaded)
    {
        DEBUG_PRINT_STATE("ERROR: Loaded-->Loaded\n");
        ret = OMX_ErrorSameState;
    }
    /*  Loaded to WaitForResources */
    else if (state == OMX_StateWaitForResources)
    {
        DEBUG_PRINT_STATE("Loaded-->WaitForResources\n");
        ret = OMX_ErrorNone;
    }
    /*  Loaded to Executing */
    else if (state == OMX_StateExecuting)
    {
        DEBUG_PRINT_STATE("ERROR:Loaded-->Executing\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Loaded to Pause */
    else if (state == OMX_StatePause)
    {
        DEBUG_PRINT_STATE("ERROR: Loaded-->Pause\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    else
    {
        DEBUG_PRINT_STATE("Loaded-->%d Not Handled)\n", state);
        ret = OMX_ErrorUndefined;
    }

    *pflag = flag;

    return ret;
}

static OMX_ERRORTYPE handle_command_state_set_idle(
    OMX_COMPONENT_PRIVATE *pcom_priv,
    OMX_STATETYPE state, OMX_U32 *sem_posted, OMX_U32 *pflag)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 flag = 1;
#ifdef __OMXVENC_RGB_BUFFER__ 
    HI_MMZ_BUF_S stBuf_RGB;
#endif

    /*  Idle to Loaded */
    if (state == OMX_StateLoaded)
    {
        if (ports_all_empty(pcom_priv))
        {
            if (channel_destroy(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("ERROR: channel create failed!\n");
                ret = OMX_ErrorHardware;

                return ret;
            }
#ifdef __OMXVENC_RGB_BUFFER__
        stBuf_RGB.phyaddr      = pcom_priv->bufferaddr_Phy_RGB;
        stBuf_RGB.user_viraddr = pcom_priv->bufferaddr_RGB;
        stBuf_RGB.bufsize      = pcom_priv->buffer_size_RGB;
        if (pcom_priv->m_store_metadata_buf)
        {
            if (HI_MMZ_Free(&stBuf_RGB) != 0)
            {
                DEBUG_PRINT_ERROR("ERROR: channel free RGB buffer for StoreMetaData failed!\n");
			    ret = OMX_ErrorHardware;
            }
            pcom_priv->bufferaddr_Phy_RGB = 0;
            pcom_priv->bufferaddr_RGB     = 0;
            pcom_priv->buffer_size_RGB    = 0;
        } 
#endif
            pcom_priv->msg_thread_exit = OMX_TRUE;
            pthread_join(pcom_priv->msg_thread_id, NULL);

            DEBUG_PRINT_STATE("idle-->Loaded\n");
        }
        else
        {
            DEBUG_PRINT_STATE("idle-->Loaded-Pending\n");
            bit_set(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING);
            flag = 0;
        }
    }
    /*  Idle to Executing */
    else if (state == OMX_StateExecuting)
    {
        DEBUG_PRINT_STATE("idle-->Executing-Pending\n");
        if (channel_start(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("Channel start failed\n");
            ret = OMX_ErrorHardware;

            return ret;
        }

        pcom_priv->msg_thread_exit	= OMX_FALSE;
        if (pthread_create(&pcom_priv->msg_thread_id,
                           0, message_thread, pcom_priv) < 0)
        {
            DEBUG_PRINT_STATE("ERROR: failed to create msg thread\n");
            ret = OMX_ErrorInsufficientResources;

            return ret;
        }

        DEBUG_PRINT("message thread create ok\n");
        flag = 1;
    }
    /*  Idle to Idle */
    else if (state == OMX_StateIdle)
    {
        DEBUG_PRINT_STATE("ERROR: Idle-->Idle\n");
        ret = OMX_ErrorSameState;
    }
    /*  Idle to WaitForResources */
    else if (state == OMX_StateWaitForResources)
    {
        DEBUG_PRINT_STATE("ERROR: Idle-->WaitForResources\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Idle to Pause */
    else if (state == OMX_StatePause)
    {
        DEBUG_PRINT_STATE("Idle-->Pause\n");
        bit_set(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING);
        flag = 0;
    }
    else
    {
        DEBUG_PRINT_ERROR("Idle --> %d Not Handled\n", state);
        ret = OMX_ErrorBadParameter;
    }

    *pflag = flag;

    return ret;
}

static OMX_ERRORTYPE handle_command_state_set_executing(
    OMX_COMPONENT_PRIVATE *pcom_priv,
    OMX_STATETYPE state, OMX_U32 *sem_posted, OMX_U32 *pflag)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 flag = 1;

    /*  Executing to Idle */
    if (state == OMX_StateIdle)
    {
        if (channel_stop(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("chan stop failed\n");
            ret = OMX_ErrorHardware;

            return ret;
        }

        DEBUG_PRINT_STATE("Executing-->Idle OK! \n");
        bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
        flag = 0;
    }
    /*  Executing to Paused */
    else if (state == OMX_StatePause)
    {
        if (channel_stop(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("Error In Pause State\n");
            ret = OMX_ErrorHardware;

            return ret;
        }

        DEBUG_PRINT_STATE("Executing-->Pause OK!\n");
        bit_set(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING);
        flag = 0;
    }
    /*  Executing to Loaded */
    else if (state == OMX_StateLoaded)
    {
        DEBUG_PRINT_STATE("Executing --> Loaded \n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Executing to WaitForResources */
    else if (state == OMX_StateWaitForResources)
    {
        DEBUG_PRINT_STATE("Executing --> WaitForResources \n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Executing to Executing */
    else if (state == OMX_StateExecuting)
    {
        DEBUG_PRINT_STATE("Executing --> Executing \n");
        ret = OMX_ErrorSameState;
    }
    else
    {
        DEBUG_PRINT_ERROR("Executing -->%d Not Handled\n", state);
        ret = OMX_ErrorBadParameter;
    }

    *pflag = flag;

    return ret;
}

static OMX_ERRORTYPE handle_command_state_set_pause(
    OMX_COMPONENT_PRIVATE *pcom_priv,
    OMX_STATETYPE state, OMX_U32 *sem_posted, OMX_U32 *pflag)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 flag = 1;

    /*  Pause to Executing */
    if (state == OMX_StateExecuting)
    {
        if (channel_start(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("Channel start failed\n");
            ret = OMX_ErrorHardware;

            return ret;
        }
        DEBUG_PRINT_STATE("Pause-->Executing OK! \n");
        flag = 1;

    }
    /*  Pause to Idle */
    else if (state == OMX_StateIdle)
    {
        if (channel_stop(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("channel_stop failed\n");
            ret = OMX_ErrorHardware;

            return ret;
        }

        DEBUG_PRINT_STATE("Pause --> Idle!\n");
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
        DEBUG_PRINT_STATE("Pause --> loaded \n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Pause to WaitForResources */
    else if (state == OMX_StateWaitForResources)
    {
        DEBUG_PRINT_STATE("Pause --> WaitForResources\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  Pause to Pause */
    else if (state == OMX_StatePause)
    {
        DEBUG_PRINT_STATE("Pause --> Pause\n");
        ret = OMX_ErrorSameState;
    }
    else
    {
        DEBUG_PRINT_STATE("ERROR:Paused -->%d Not Handled\n", state);
        ret = OMX_ErrorBadParameter;
    }

    *pflag = flag;

    return ret;
}

static OMX_ERRORTYPE handle_command_state_set_resources(
    OMX_COMPONENT_PRIVATE *pcom_priv,
    OMX_STATETYPE state, OMX_U32 *sem_posted)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    /*  WaitForResources to Loaded */
    if (state == OMX_StateLoaded)
    {
        DEBUG_PRINT_STATE("WaitForResources-->Loaded\n");
    }
    if (state == OMX_StateIdle)
    {
        DEBUG_PRINT_STATE("WaitForResources-->Idle\n");
    }
    /*  WaitForResources to WaitForResources */
    else if (state == OMX_StateWaitForResources)
    {
        DEBUG_PRINT_STATE("ERROR: WaitForResources-->WaitForResources\n");
        ret = OMX_ErrorSameState;
    }
    /*  WaitForResources to Executing */
    else if (state == OMX_StateExecuting)
    {
        DEBUG_PRINT_STATE("ERROR: WaitForResources-->Executing\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  WaitForResources to Pause */
    else if (state == OMX_StatePause)
    {
        DEBUG_PRINT_STATE("ERROR: WaitForResources-->Pause\n");
        ret = OMX_ErrorIncorrectStateTransition;
    }
    /*  WaitForResources to Loaded -
       is NOT tested by Khronos TS */
    else
    {
        DEBUG_PRINT_STATE("ERROR: %d --> %d(NotHandled)\n",
                          pcom_priv->m_state, state);
        ret = OMX_ErrorBadParameter;
    }

    return ret;
}

static OMX_ERRORTYPE handle_command_state_set(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_STATETYPE state, OMX_U32* sem_posted)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 flag = 1;

    /***************************/
    /* Current State is Loaded */
    /***************************/
    if (pcom_priv->m_state == OMX_StateLoaded)
    {
        ret = handle_command_state_set_loaded(pcom_priv, state, sem_posted, &flag);
    }
    /***************************/
    /* Current State is IDLE */
    /***************************/
    else if (pcom_priv->m_state == OMX_StateIdle)
    {
        ret = handle_command_state_set_idle(pcom_priv, state, sem_posted, &flag);
    }
    /******************************/
    /* Current State is Executing */
    /******************************/
    else if (pcom_priv->m_state == OMX_StateExecuting)
    {
        ret = handle_command_state_set_executing(pcom_priv, state, sem_posted, &flag);
    }
    /***************************/
    /* Current State is Pause  */
    /***************************/
    else if (pcom_priv->m_state == OMX_StatePause)
    {
        ret = handle_command_state_set_pause(pcom_priv, state, sem_posted, &flag);
    }
    /***************************/
    /* Current State is WaitForResources  */
    /***************************/
    else if (pcom_priv->m_state == OMX_StateWaitForResources)
    {
        ret = handle_command_state_set_resources(pcom_priv, state, sem_posted);
    }

    if (flag)
    {
        if (ret != OMX_ErrorNone)                     //异常退出处理分支
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
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_U32 port, OMX_U32* sem_posted)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE* port_priv = NULL;

    DEBUG_PRINT("OMX_CommandFlush!\n");
    if ((port != OMX_ALL) && (port > OUTPUT_PORT_INDEX))
    {
        DEBUG_PRINT_ERROR("Error: Invalid Port %ld\n", (OMX_S32)port);
        return OMX_ErrorBadPortIndex;
    }

    if ((INPUT_PORT_INDEX == port) || (OMX_ALL == port))
    {
        bit_set(&pcom_priv->m_flags,
                OMX_STATE_INPUT_FLUSH_PENDING);
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }
    if ((OUTPUT_PORT_INDEX == port) || (OMX_ALL == port))
    {
        bit_set(&pcom_priv->m_flags,
                OMX_STATE_OUTPUT_FLUSH_PENDING);
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        port_priv->m_port_flushing = OMX_TRUE;
    }

    if (channel_flush_port(&pcom_priv->drv_ctx, port) < 0)
    {
        DEBUG_PRINT_ERROR("ioctl flush port failed\n");
        ret = OMX_ErrorHardware;
        /*fixme: should clear flushing pending bits?!!*/
        omx_report_error(pcom_priv, ret);
    }
    else
    { DEBUG_PRINT("command flush ok!\n"); }
    return ret;
}


static OMX_ERRORTYPE handle_command_port_disable(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_U32 port, OMX_U32* sem_posted)
{
    //OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE* port_priv = NULL;
    OMX_U32 flag = 1;

    DEBUG_PRINT("OMX_CommandPortDisable\n");
    if ((port == INPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        DEBUG_PRINT("Disable input port!\n");

        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        if (port_priv->port_def.bEnabled)
        {
            port_priv->port_def.bEnabled = OMX_FALSE;

            if (!port_empty(pcom_priv, INPUT_PORT_INDEX)) //port buffer 非空
            {
                DEBUG_PRINT_STATE("in port disable->enable pending!\n");
                bit_set(&pcom_priv->m_flags,
                        OMX_STATE_INPUT_DISABLE_PENDING);

                if ((pcom_priv->m_state == OMX_StatePause)
                    || (pcom_priv->m_state == OMX_StateExecuting))
                {
                    if (!*sem_posted)
                    {
                        *sem_posted = 1;
                        sem_post (&pcom_priv->m_cmd_lock);
                    }
                    omx_flush_port(pcom_priv, INPUT_PORT_INDEX);
                }
                flag = 0;
            }
        }
        else
        {
            DEBUG_PRINT("output already disabled\n");
            //ret = OMX_ErrorNone;
        }

        if (flag)
        {
            post_event(pcom_priv,
                       OMX_CommandPortDisable, INPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }
    }

    flag = 1;
    if ((port == OUTPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        DEBUG_PRINT("Disable output port!\n");
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        if (port_priv->port_def.bEnabled)
        {
            port_priv->port_def.bEnabled = OMX_FALSE;

            if (!port_empty(pcom_priv, OUTPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("out port Enabled-->Disable Pending\n");

                bit_set(&pcom_priv->m_flags,
                        OMX_STATE_OUTPUT_DISABLE_PENDING);

                if ((pcom_priv->m_state == OMX_StatePause) ||
                    (pcom_priv->m_state == OMX_StateExecuting))
                {
                    if (!*sem_posted)
                    {
                        *sem_posted = 1;
                        sem_post (&pcom_priv->m_cmd_lock);
                    }
                    omx_flush_port(pcom_priv, OUTPUT_PORT_INDEX);
                }
                flag = 0;
            }
        }
        else
        {
            DEBUG_PRINT("output already disabled\n");
            //ret = OMX_ErrorNone;
        }

        if (flag)
        {
            post_event(pcom_priv,
                       OMX_CommandPortDisable, OUTPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }

    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE handle_command_port_enable(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_U32 port, OMX_U32* sem_posted)
{
    //OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE* port_priv = NULL;
    OMX_U32 flag = 1;

    DEBUG_PRINT("OMX_CommandPortEnable\n");

    if ((port == INPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        DEBUG_PRINT("Enable input port!\n");

        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        if (!port_priv->port_def.bEnabled)
        {
            port_priv->port_def.bEnabled = OMX_TRUE;

            if (!port_full(pcom_priv, INPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Disabled-->Enabled Pending\n");
                bit_set(&pcom_priv->m_flags,
                        OMX_STATE_INPUT_ENABLE_PENDING);
                flag = 0;
            }
        }
        else
        {
            DEBUG_PRINT("Inport already enabled\n");
            //ret = OMX_ErrorNone;
        }

        if (flag)
        {
            post_event(pcom_priv,
                       OMX_CommandPortEnable, INPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }
    }

    flag = 1;
    if ((port == OUTPUT_PORT_INDEX) || (port == OMX_ALL))
    {
        DEBUG_PRINT("Enable output port!\n");

        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        if (!port_priv->port_def.bEnabled)
        {
            port_priv->port_def.bEnabled = OMX_TRUE;

            if (!port_full(pcom_priv, OUTPUT_PORT_INDEX))
            {
                DEBUG_PRINT_STATE("Disabled-->Enabled Pending\n");
                bit_set(&pcom_priv->m_flags,
                        OMX_STATE_OUTPUT_ENABLE_PENDING);
                flag = 0;
            }
        }
        else
        {
            DEBUG_PRINT("Out port already enabled\n");
            //ret = OMX_ErrorNone;
        }

        if (flag)
        {
            post_event(pcom_priv,
                       OMX_CommandPortEnable, OUTPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }
    }

    return OMX_ErrorNone;
}


static OMX_ERRORTYPE  send_command_proxy(
    OMX_COMPONENT_PRIVATE* pcom_priv,
    OMX_IN OMX_COMMANDTYPE cmd,
    OMX_IN OMX_U32 param1, OMX_IN OMX_PTR cmd_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_U32 sem_posted = 0;

    OMX_CHECK_ARG_RETURN(pcom_priv == NULL);

    switch (cmd)
    {
        case OMX_CommandStateSet:
            ret = handle_command_state_set(
                      pcom_priv, (OMX_STATETYPE)param1, &sem_posted);
            break;

        case OMX_CommandFlush:
            ret = handle_command_flush(
                      pcom_priv, param1, &sem_posted);
            break;

        case OMX_CommandPortEnable:
            ret = handle_command_port_enable(
                      pcom_priv, param1, &sem_posted);
            break;

        case OMX_CommandPortDisable:
            ret = handle_command_port_disable(
                      pcom_priv, param1, &sem_posted);
            break;

        default:
            DEBUG_PRINT_ERROR("Error: Invalid Command(%d)\n", cmd);
            ret = OMX_ErrorNotImplemented;
            break;
    }

    if (!sem_posted)
    {
        sem_post(&pcom_priv->m_cmd_lock);
    }

    return ret;
}


static OMX_U32 event_process_get_event_para(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 *pP1, OMX_U32 *pP2)
{
    OMX_U32 p1 = 0;
    OMX_U32 p2 = 0;
    OMX_U32 ident = 0;
    OMX_U32 qsize = 0;

    if (!pcom_priv || (id >= OMX_GENERATE_UNUSED))
    {
        DEBUG_PRINT_ERROR("ERROR: %s() invalid param!\n", __func__);

        return HI_FAILURE;
    }

    /* process event from cmd/etb/ftb queue */
    pthread_mutex_lock(&pcom_priv->m_lock);
    if ((id == OMX_GENERATE_FTB) || (id == OMX_GENERATE_FBD))
    {
        qsize = get_q_size(&pcom_priv->m_ftb_q);
        if (qsize == 0)
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);

            return HI_FAILURE;
        }
        pop_entry(&pcom_priv->m_ftb_q, &p1, &p2, &ident);
    }
    else if ((id == OMX_GENERATE_ETB) || (id == OMX_GENERATE_EBD))
    {
        qsize = get_q_size(&pcom_priv->m_etb_q);
        if (qsize == 0)
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);

            return HI_FAILURE;
        }
        pop_entry(&pcom_priv->m_etb_q, &p1, &p2, &ident);
    }
    else
    {
        qsize = get_q_size(&pcom_priv->m_cmd_q);
        if (qsize == 0)
        {
            pthread_mutex_unlock(&pcom_priv->m_lock);

            return HI_FAILURE;
        }
        pop_entry(&pcom_priv->m_cmd_q, &p1, &p2, &ident);
    }

    pthread_mutex_unlock(&pcom_priv->m_lock);

    if (id != ident)
    {
        DEBUG_PRINT_ERROR("ID cannot match, id %lu, ident %lu\n", id, ident);

        return HI_FAILURE;
    }

    *pP1 = p1;
    *pP2 = p2;

    return HI_SUCCESS;
}

static void event_process_case_ebd(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p1, OMX_U32 p2)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_EBD failure\n");
        omx_report_error(pcom_priv, OMX_ErrorUndefined);

        return;
    }

    ret = empty_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE*)p1);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("empty_buffer_done failure\n");
        omx_report_error(pcom_priv, ret);
    }
}

static void event_process_case_fbd(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p1, OMX_U32 p2)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_FBD failure %s,%d\n", __func__, __LINE__);
        omx_report_error(pcom_priv, OMX_ErrorUndefined);

        return;
    }

    ret = fill_buffer_done(pcom_priv, (OMX_BUFFERHEADERTYPE*)p1);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("fill_buffer_done failure\n");
        omx_report_error (pcom_priv, ret);
    }
}

static void event_process_case_flush_input_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    OMX_PORT_PRIVATE *port_priv = NULL;

    DEBUG_PRINT("Driver flush i/p Port complete\n");
    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
    if (!port_priv->m_port_flushing)
    {
        DEBUG_PRINT("WARNING: Unexpected flush cmd\n");

        return;
    }
    else
    {
        port_priv->m_port_flushing = OMX_FALSE;
    }

    return_inbuffers(pcom_priv);

    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("input flush failure\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    /*Check if we need generate event for Flush done*/
    if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_FLUSH_PENDING))
    {
        DEBUG_PRINT("Input Flush completed - Notify Client\n");
        bit_clear(&pcom_priv->m_flags, OMX_STATE_INPUT_FLUSH_PENDING);

        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandFlush, INPUT_PORT_INDEX, NULL);
    }
}

static void event_process_case_flush_output_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    OMX_PORT_PRIVATE *port_priv = NULL;

    DEBUG_PRINT("Driver flush o/p Port complete\n");
    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
    if (!port_priv->m_port_flushing)
    {
        DEBUG_PRINT("WARNING: Unexpected flush cmd\n");

        return;
    }
    else
    {
        port_priv->m_port_flushing = OMX_FALSE;
    }
    return_outbuffers(pcom_priv);
    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("output flush failure\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    /*Check if we need generate event for Flush done*/
    if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_FLUSH_PENDING))
    {
        DEBUG_PRINT("Notify Output Flush done\n");
        bit_clear (&pcom_priv->m_flags, OMX_STATE_OUTPUT_FLUSH_PENDING);
        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandFlush, OUTPUT_PORT_INDEX, NULL);
    }
}

static void event_process_case_start_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_START_DONE Failure\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    if (bit_present(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING))
    {
        DEBUG_PRINT_STATE("Move to executing\n");
        bit_clear((&pcom_priv->m_flags), OMX_STATE_EXECUTE_PENDING);
        pcom_priv->m_state = OMX_StateExecuting;
        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateExecuting, NULL);
    }

    if (bit_present(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING))
    {
        if (channel_stop(&pcom_priv->drv_ctx) < 0)
        {
            DEBUG_PRINT_ERROR("channel_pause failed\n");
            omx_report_error(pcom_priv, OMX_ErrorHardware);
        }
    }
}

static void event_process_case_pause_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_PAUSE_DONE failed\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    if (bit_present(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING))
    {
        DEBUG_PRINT("OMX_GENERATE_PAUSE_DONE nofity\n");

        bit_clear(&pcom_priv->m_flags, OMX_STATE_PAUSE_PENDING);

        pcom_priv->m_state = OMX_StatePause;
        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StatePause, NULL);
    }
}

static void event_process_case_resume_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_RESUME_DONE failed\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    if (bit_present(&pcom_priv->m_flags, OMX_STATE_EXECUTE_PENDING))
    {
        DEBUG_PRINT_STATE("Moving to execute state\n");

        bit_clear((&pcom_priv->m_flags), OMX_STATE_EXECUTE_PENDING);
        pcom_priv->m_state = OMX_StateExecuting;
        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateExecuting, NULL);
    }
}

static void event_process_case_stop_done(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id, OMX_U32 p2)
{
    if (p2 != VENC_DONE_SUCCESS)
    {
        DEBUG_PRINT_ERROR("OMX_GENERATE_STOP_DONE failed\n");
        omx_report_error(pcom_priv, OMX_ErrorHardware);

        return;
    }

    return_outbuffers(pcom_priv);
    return_inbuffers(pcom_priv);

    if (bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING))
    {
        bit_clear((&pcom_priv->m_flags), OMX_STATE_IDLE_PENDING);

        pcom_priv->m_state = OMX_StateIdle;

        DEBUG_PRINT_STATE("State Idle-pending -> Idle\n");

        omx_report_event(pcom_priv, OMX_EventCmdComplete, OMX_CommandStateSet, OMX_StateIdle, NULL);
    }
}

static void event_process(OMX_COMPONENT_PRIVATE *pcom_priv, OMX_U32 id)
{
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_U32 p1 = 0;
    OMX_U32 p2 = 0;

    if(event_process_get_event_para(pcom_priv, id, &p1, &p2) == HI_FAILURE)
    {
        return;
    }

    /* event process instance */
    switch (id)
    {
        case OMX_GENERATE_COMMAND_DONE:
            generate_command_done(pcom_priv, p1, p2);
            break;

        case OMX_GENERATE_ETB:
            empty_this_buffer_proxy(pcom_priv, (OMX_BUFFERHEADERTYPE*)p1);
            break;

        case OMX_GENERATE_FTB:
            fill_this_buffer_proxy(pcom_priv, (OMX_BUFFERHEADERTYPE*)p1);
            break;

        case OMX_GENERATE_COMMAND:
            send_command_proxy(pcom_priv, (OMX_COMMANDTYPE)p1, (OMX_U32)p2, (OMX_PTR)NULL);
            break;

        case OMX_GENERATE_EBD:
            event_process_case_ebd(pcom_priv, id, p1, p2);
            break;

        case OMX_GENERATE_FBD:
            event_process_case_fbd(pcom_priv, id, p1, p2);
            break;

        case OMX_GENERATE_FLUSH_INPUT_DONE:
            event_process_case_flush_input_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_FLUSH_OUTPUT_DONE:
            event_process_case_flush_output_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_START_DONE:
            event_process_case_start_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_PAUSE_DONE:
            event_process_case_pause_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_RESUME_DONE:
            event_process_case_resume_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_STOP_DONE:
            event_process_case_stop_done(pcom_priv, id, p2);
            break;

        case OMX_GENERATE_EOS_DONE:
            DEBUG_PRINT("Rxd OMX_GENERATE_EOS_DONE\n");
            omx_report_event(pcom_priv, OMX_EventBufferFlag, OUTPUT_PORT_INDEX, OMX_BUFFERFLAG_EOS, NULL);
            break;

        case OMX_GENERATE_IMAGE_SIZE_CHANGE:
            DEBUG_PRINT("image size changed!\n");
            port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
            port_priv->m_port_reconfig = OMX_TRUE;
            omx_report_event(pcom_priv, OMX_EventPortSettingsChanged, OUTPUT_PORT_INDEX, OMX_IndexParamPortDefinition, NULL);
            break;

        case OMX_GENERATE_CROP_RECT_CHANGE:
            DEBUG_PRINT("crop rect changed!\n");
            omx_report_event(pcom_priv, OMX_EventPortSettingsChanged, OUTPUT_PORT_INDEX, OMX_IndexConfigCommonOutputCrop, NULL);
            break;

        case OMX_GENERATE_HARDWARE_ERROR:
            DEBUG_PRINT_ERROR("hardware failed\n");
            omx_report_error(pcom_priv, OMX_ErrorHardware);
            break;

        default:
            DEBUG_PRINT_ERROR("default process for msg %lu\n", id);
            break;
    }
}


static void* event_thread(void* input)
{
    OMX_COMPONENT_PRIVATE* pcom_priv =
        (OMX_COMPONENT_PRIVATE*)input;
    OMX_U8 id;
    OMX_S32 n = -1;
    sem_wait(&pcom_priv->m_async_sem);                                  //等外部释放m_async_sem信号量才正式执行线程
    DEBUG_PRINT("Event thread start!\n");
    while (!pcom_priv->event_thread_exit)
    {
        sem_post(&pcom_priv->m_async_sem);
        n = read(pcom_priv->m_pipe_in, &id, 1);
        if (((n < 0) && (errno != EINTR)) || (n == 0))                   //errno!=EINTR -- 如果错误号码不等于 （中断）事件发生，即如果是由于中断引起的错误，继续执行
        {
            DEBUG_PRINT_ERROR("read from pipe failed, ret:%ld\n", n);
            break;
        }
        else if (n > 1)
        {
            DEBUG_PRINT_ERROR("read more than 1 byte\n");
            continue;
        }
        event_process(pcom_priv, id);

        sem_wait(&pcom_priv->m_async_sem);
    }

    sem_post(&pcom_priv->m_async_sem);
    DEBUG_PRINT("Event thread exit!\n");
    return NULL;
}


static OMX_S32 ports_init(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    OMX_PORT_PRIVATE* in_port  = NULL;
    OMX_PORT_PRIVATE* out_port = NULL;
    OMX_S32 result = -1;

    /* init in port private */
    in_port = &pcom_priv->m_port[INPUT_PORT_INDEX];
    in_port->m_omx_bufhead = (OMX_BUFFERHEADERTYPE**)malloc(sizeof(OMX_BUFFERHEADERTYPE*)* MAX_BUF_NUM_INPUT);

    in_port->m_venc_bufhead = (venc_user_info**)malloc(sizeof(venc_user_info*) * MAX_BUF_NUM_INPUT);
    if (!in_port->m_omx_bufhead || !in_port->m_venc_bufhead)
    {
        DEBUG_PRINT_ERROR("not enough memory!\n");
        goto inport_error;
    }

    in_port->m_buf_cnt				= 0;
    in_port->m_buf_pend_cnt			= 0;
    in_port->m_port_reconfig		= OMX_FALSE;
    in_port->m_port_flushing		= OMX_FALSE;

    //CONFIG_VERSION_SIZE(&(in_port->port_def), OMX_PARAM_PORTDEFINITIONTYPE);
    in_port->port_def.nVersion.nVersion                 = OMX_SPEC_VERSION;
    in_port->port_def.nSize                             = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);

    in_port->port_def.nPortIndex                        = INPUT_PORT_INDEX;
    in_port->port_def.eDir                              = OMX_DirInput;
    in_port->port_def.nBufferCountActual                = DEF_MAX_IN_BUF_CNT;
    in_port->port_def.nBufferCountMin                   = DEF_MIN_IN_BUF_CNT;
    in_port->port_def.nBufferSize                       = FRAME_SIZE(MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);   //暂时开辟了编码最大性能的buffer_size
    in_port->port_def.bEnabled                          = OMX_TRUE;
    in_port->port_def.bPopulated                        = OMX_FALSE;
    in_port->port_def.eDomain                           = OMX_PortDomainVideo;
    in_port->port_def.bBuffersContiguous                = OMX_TRUE;
    in_port->port_def.nBufferAlignment                  = DEFAULT_ALIGN_SIZE;

    in_port->port_def.format.video.nFrameWidth          = MAX_FRAME_WIDTH;
    in_port->port_def.format.video.nFrameHeight         = MAX_FRAME_HEIGHT;
    in_port->port_def.format.video.xFramerate           = DEFAULT_FPS << 16;
    in_port->port_def.format.video.nStride              = MAX_FRAME_WIDTH;
    in_port->port_def.format.video.nSliceHeight         = MAX_FRAME_HEIGHT;
    in_port->port_def.format.video.eColorFormat         = OMX_COLOR_FormatYUV420SemiPlanar;
    in_port->port_def.format.video.eCompressionFormat   = OMX_VIDEO_CodingUnused;
    in_port->port_def.format.video.bFlagErrorConcealment = OMX_FALSE;

    /* init out port private */
    out_port = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
    out_port->m_omx_bufhead = (OMX_BUFFERHEADERTYPE**)malloc(sizeof(OMX_BUFFERHEADERTYPE*)* MAX_BUF_NUM_OUTPUT);
    out_port->m_venc_bufhead = (venc_user_info**)malloc(sizeof(venc_user_info*) * MAX_BUF_NUM_OUTPUT);
    if (!out_port->m_omx_bufhead || !out_port->m_venc_bufhead)
    {
        DEBUG_PRINT_ERROR("not enough memory!\n");
        goto outport_error;
    }

    out_port->m_buf_cnt				= 0;
    out_port->m_buf_pend_cnt		= 0;
    out_port->m_port_reconfig		= OMX_FALSE;
    out_port->m_port_flushing		= OMX_FALSE;

    //CONFIG_VERSION_SIZE(&(in_port->port_def), OMX_PARAM_PORTDEFINITIONTYPE);
    out_port->port_def.nVersion.nVersion                 = OMX_SPEC_VERSION;
    out_port->port_def.nSize                             = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
    out_port->port_def.nPortIndex                        = OUTPUT_PORT_INDEX;
    out_port->port_def.eDir                              = OMX_DirOutput;
    out_port->port_def.nBufferCountActual                = DEF_MAX_OUT_BUF_CNT;
    out_port->port_def.nBufferCountMin                   = DEF_MIN_OUT_BUF_CNT;
    out_port->port_def.nBufferSize                       = DEF_OUT_BUFFER_SIZE;//FRAME_SIZE(MAX_FRAME_WIDTH, MAX_FRAME_HEIGHT);   //暂时开辟了编码最大性能的buffer_size
    out_port->port_def.bEnabled                          = OMX_TRUE;
    out_port->port_def.bPopulated                        = OMX_FALSE;
    out_port->port_def.eDomain                           = OMX_PortDomainVideo;
    out_port->port_def.bBuffersContiguous                = OMX_TRUE;
    out_port->port_def.nBufferAlignment                  = DEFAULT_ALIGN_SIZE;

    out_port->port_def.format.video.nFrameWidth          = MAX_FRAME_WIDTH;
    out_port->port_def.format.video.nFrameHeight         = MAX_FRAME_HEIGHT;
    out_port->port_def.format.video.xFramerate           = DEFAULT_FPS << 16;
    out_port->port_def.format.video.nSliceHeight         = 0;                             // ??
    out_port->port_def.format.video.nBitrate             = DEFAULT_BITRATE;
    out_port->port_def.format.video.eColorFormat         = OMX_COLOR_FormatUnused;
    out_port->port_def.format.video.eCompressionFormat   = OMX_VIDEO_CodingAVC;
    out_port->port_def.format.video.bFlagErrorConcealment = OMX_FALSE;

    DEBUG_PRINT("exit %s()", __func__);
    return 0;

outport_error:
    free(out_port->m_omx_bufhead);
    free(out_port->m_venc_bufhead);
    out_port->m_omx_bufhead = NULL;
    out_port->m_venc_bufhead = NULL;

inport_error:
    free(in_port->m_omx_bufhead);
    free(in_port->m_venc_bufhead);
    in_port->m_omx_bufhead = NULL;
    in_port->m_venc_bufhead = NULL;
    
    return result;
}


static void ports_deinit(OMX_COMPONENT_PRIVATE* pcom_priv)
{
    OMX_S32 i = 0;
    for (; i < MAX_PORT_NUM; ++i)
    {
        free(pcom_priv->m_port[i].m_venc_bufhead);
        free(pcom_priv->m_port[i].m_omx_bufhead);
        pcom_priv->m_port[i].m_venc_bufhead = NULL;
        pcom_priv->m_port[i].m_omx_bufhead = NULL;
    }
    DEBUG_PRINT("exit %s()", __func__);
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
    OMX_IN  OMX_HANDLETYPE phandle,
    OMX_OUT OMX_STRING comp_name,
    OMX_OUT OMX_VERSIONTYPE* comp_version,
    OMX_OUT OMX_VERSIONTYPE* spec_version,
    OMX_OUT OMX_UUIDTYPE* componentUUID)
{
    OMX_COMPONENTTYPE* pcomp         = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(comp_name == NULL);
    OMX_CHECK_ARG_RETURN(comp_version == NULL);
    OMX_CHECK_ARG_RETURN(spec_version == NULL);
    OMX_CHECK_ARG_RETURN(componentUUID == NULL);


    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    strncpy((char*)comp_name, (char*)pcom_priv->m_comp_name , OMX_MAX_STRINGNAME_SIZE - 1);

    comp_version->nVersion = pcomp->nVersion.nVersion;
    spec_version->nVersion = OMX_SPEC_VERSION;
    //componentUUID          = ??;

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
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    pcomp     = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    post_event(pcom_priv,
               (OMX_U32)cmd, (OMX_U32)param1, OMX_GENERATE_COMMAND);      //把用户这个命令压入队列和管道，实际处理是由内部的 send_command_proxy来处理的,等到内部send_command_proxy处理完，释放信号量，此处获得信号量，操作成功。
    DEBUG_PRINT("send_command : cmd =%d\n", cmd);
    sem_wait(&pcom_priv->m_cmd_lock);
    return OMX_ErrorNone;
}


static OMX_ERRORTYPE get_parameter_VideoAvc(OMX_INOUT OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_VIDEO_PARAM_AVCTYPE *pAvcType = NULL;
    HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs = NULL;

    pAvcType = (OMX_VIDEO_PARAM_AVCTYPE*)param_data;
    pVencUnfAttrs = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;
    DEBUG_PRINT("get_param:OMX_IndexParamVideoAvc,PortIndex = %ld,state = %ld\n", (OMX_S32)pAvcType->nPortIndex, (OMX_S32)pcomp_priv->m_state);

    if ((pAvcType->nPortIndex > OUTPUT_PORT_INDEX) || (pAvcType->nPortIndex == INPUT_PORT_INDEX))
    {
        DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)pAvcType->nPortIndex);
        ret = OMX_ErrorBadPortIndex;

        return ret;
    }

    CONFIG_VERSION_SIZE(pAvcType , OMX_VIDEO_PARAM_AVCTYPE);

    pAvcType->nSliceHeaderSpacing   = 0;
    pAvcType->nPFrames              = pVencUnfAttrs->u32Gop - 1;
    pAvcType->nBFrames              = 0;
    pAvcType->bUseHadamard          = OMX_TRUE;
    pAvcType->nRefFrames            = 1;
    pAvcType->nRefIdx10ActiveMinus1 = 0;
    pAvcType->nRefIdx11ActiveMinus1 = 0;
    pAvcType->bEnableUEP            = OMX_FALSE;
    pAvcType->bEnableFMO            = OMX_FALSE;
    pAvcType->bEnableASO            = OMX_FALSE;
    pAvcType->bEnableRS             = OMX_FALSE;
    pAvcType->nAllowedPictureTypes  = (pAvcType->nPFrames == 0) ? (OMX_VIDEO_PictureTypeI) : ((OMX_U32)OMX_VIDEO_PictureTypeI | (OMX_U32)OMX_VIDEO_PictureTypeP);
    pAvcType->bFrameMBsOnly         = OMX_TRUE;
    pAvcType->bMBAFF                = OMX_FALSE;
    pAvcType->bEntropyCodingCABAC   = (pVencUnfAttrs->enVencProfile != HI_UNF_H264_PROFILE_BASELINE) ? (OMX_TRUE) : (OMX_FALSE);
    pAvcType->bWeightedPPrediction  = OMX_FALSE;
    pAvcType->nWeightedBipredicitonMode = OMX_FALSE;
    pAvcType->bconstIpred               = OMX_TRUE;
    pAvcType->bDirect8x8Inference       = OMX_FALSE;
    pAvcType->bDirectSpatialTemporal    = OMX_FALSE;
    pAvcType->nCabacInitIdc             = 0;
    pAvcType->eLoopFilterMode           = OMX_VIDEO_AVCLoopFilterEnable;

    switch (pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.h264Level)
    {
        case 10:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel1;
            break;
        case 20:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel2;
            break;
        case 21:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel21;
            break;
        case 30:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel3;
            break;
        case 31:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel31;
            break;
        case 32:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel32;
            break;
        case 41:
            pAvcType->eLevel = OMX_VIDEO_AVCLevel41;
            break;
        default:
            DEBUG_PRINT_ERROR("Bad parm of Level %ld\n", (OMX_S32)pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.h264Level);
            pAvcType->eLevel = OMX_VIDEO_AVCLevelMax;
            break;
    }
    switch (pVencUnfAttrs->enVencProfile)
    {
        case HI_UNF_H264_PROFILE_BASELINE:
            pAvcType->eProfile = OMX_VIDEO_AVCProfileBaseline;
            break;
        case HI_UNF_H264_PROFILE_MAIN:
            pAvcType->eProfile = OMX_VIDEO_AVCProfileMain;
            break;
        case HI_UNF_H264_PROFILE_HIGH:
            pAvcType->eProfile = OMX_VIDEO_AVCProfileHigh;
            break;
        default:
            DEBUG_PRINT_ERROR("Bad parm of VencProfile %ld\n", (OMX_S32)pVencUnfAttrs->enVencProfile);
            pAvcType->eProfile = OMX_VIDEO_AVCProfileMax;
            break;
    }

    return ret;
}

static OMX_ERRORTYPE get_parameter_VideoPortFormat(OMX_INOUT OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_VIDEO_PARAM_PORTFORMATTYPE *portFmt = NULL;

    portFmt = (OMX_VIDEO_PARAM_PORTFORMATTYPE*)param_data;
    DEBUG_PRINT("get_param:OMX_IndexParamVideoPortFormat,PortIndex = %ld,state = %ld\n",
                (OMX_S32)portFmt->nPortIndex, (OMX_S32)pcomp_priv->m_state);

    CONFIG_VERSION_SIZE(portFmt, OMX_VIDEO_PARAM_PORTFORMATTYPE);

    if (INPUT_PORT_INDEX == portFmt->nPortIndex)
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
            case 2:
                portFmt->eColorFormat = OMX_COLOR_FormatYUV420Planar;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 3:
                portFmt->eColorFormat = OMX_COLOR_FormatYVU420Planar;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 4:
                portFmt->eColorFormat = OMX_COLOR_FormatYCbYCr;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 5:
                portFmt->eColorFormat = OMX_COLOR_FormatYCrYCb;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 6:
                portFmt->eColorFormat = OMX_COLOR_FormatCbYCrY;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 7:
                portFmt->eColorFormat = OMX_COLOR_FormatYUV422SemiPlanar;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 8:
                portFmt->eColorFormat = OMX_COLOR_FormatYVU422SemiPlanar;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;
            case 9:
                portFmt->eColorFormat = OMX_QCOM_COLOR_FormatYVU420SemiPlanar;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                break;						
            case 10:
                portFmt->eColorFormat = OMX_COLOR_FormatAndroidOpaque;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingUnused;
                DEBUG_PRINT("input format support : OMX_COLOR_FormatAndroidOpaque!\n");
                break;

            default:
                DEBUG_PRINT("no more input format\n");
                ret = OMX_ErrorNoMore;
                break;
        }
    }
    else if (OUTPUT_PORT_INDEX == portFmt->nPortIndex)
    {
        switch (portFmt->nIndex)
        {
            case 0:
                portFmt->eColorFormat = OMX_COLOR_FormatUnused;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingAVC;
                break;

            case 1:
                portFmt->eColorFormat = OMX_COLOR_FormatUnused;
                portFmt->eCompressionFormat = OMX_VIDEO_CodingHEVC;
                break;

            default:
                DEBUG_PRINT("no more output format\n");
                ret = OMX_ErrorNoMore;
                break;
        }
    }
    else
    {
        DEBUG_PRINT_ERROR("OMX_IndexParamVideoPortFormat: Bad Port Index, %d\n", (int)portFmt->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    return ret;
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
static OMX_ERRORTYPE  get_parameter(OMX_IN OMX_HANDLETYPE phandle,
                                    OMX_IN OMX_INDEXTYPE param_index,
                                    OMX_INOUT OMX_PTR param_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    pcomp      = (OMX_COMPONENTTYPE*)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    switch (param_index)
    {
        case OMX_IndexParamPortDefinition:                                          //注意:此时输入参数应该已经把输入参数param_data中对应端口属性的nPortIndex设置好，表示获取哪个port的属性
        {
            OMX_PARAM_PORTDEFINITIONTYPE* portDefn =
                (OMX_PARAM_PORTDEFINITIONTYPE*)param_data;
            OMX_PORT_PRIVATE* port_priv = NULL;
            DEBUG_PRINT("get_param: OMX_IndexParamPortDefinition,PortIndex = %ld,state = %ld\n",
                        (OMX_S32)portDefn->nPortIndex, (OMX_S32)pcomp_priv->m_state);
            if (portDefn->nPortIndex > OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)portDefn->nPortIndex);
                ret = OMX_ErrorBadPortIndex;
                break;
            }

            port_priv = &pcomp_priv->m_port[portDefn->nPortIndex];

            *portDefn = port_priv->port_def;
            DEBUG_PRINT("get_param:OMX_IndexParamPortDefinition, eCompressionFormat(%ld),eColorFormat(%ld),portId(%ld),buffersize(%ld)\n",
                        (OMX_S32)portDefn->format.video.eCompressionFormat,
                        (OMX_S32)portDefn->format.video.eColorFormat,
                        (OMX_S32)portDefn->nPortIndex,
                        (OMX_S32)portDefn->nBufferSize);
            break;
        }

        /*Component come to be a video*/
        case OMX_IndexParamVideoInit:
        {
            OMX_PORT_PARAM_TYPE* portParamType =
                (OMX_PORT_PARAM_TYPE*) param_data;

            DEBUG_PRINT("get_param:OMX_IndexParamVideoInit,state = %ld\n", (OMX_S32)pcomp_priv->m_state);

            CONFIG_VERSION_SIZE(portParamType , OMX_PORT_PARAM_TYPE);

            portParamType->nPorts           = COUNTOF(pcomp_priv->m_port);   //portParamType->nPorts = 2;
            portParamType->nStartPortNumber = INPUT_PORT_INDEX;
            break;
        }
        case OMX_IndexParamVideoAvc:
        {
            ret = get_parameter_VideoAvc(param_data, pcomp_priv);
            break;
        }
        case OMX_IndexParamVideoPortFormat:
        {
            ret = get_parameter_VideoPortFormat(param_data, pcomp_priv);
            break;
        }
#ifdef KHRONOS_1_1
        case OMX_IndexParamStandardComponentRole:
        {
            OMX_PARAM_COMPONENTROLETYPE* comp_role =
                (OMX_PARAM_COMPONENTROLETYPE*)param_data;

            DEBUG_PRINT("get_param: OMX_IndexParamStandardComponentRole\n");

            /*comp_role->nVersion.nVersion = OMX_SPEC_VERSION;
            comp_role->nSize = sizeof(*comp_role);*/
            CONFIG_VERSION_SIZE(comp_role , OMX_PARAM_COMPONENTROLETYPE);
            strncpy((char*)comp_role->cRole,
                    (char*)pcomp_priv->m_role, OMX_MAX_STRINGNAME_SIZE - 1);
            break;
        }
#endif
        case OMX_IndexParamVideoProfileLevelQuerySupported:                     //这个分支需要先输入设置好profileLevelType->nProfileIndex和nPortIndex。
        {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileLevelType =
                (OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param_data;

            DEBUG_PRINT("get_param: OMX_IndexParamVideoProfileLevelQuerySupported,PortIndex = %ld,state = %ld\n",
                        (OMX_S32)profileLevelType->nPortIndex, (OMX_S32)pcomp_priv->m_state);


            CONFIG_VERSION_SIZE(profileLevelType , OMX_VIDEO_PARAM_PROFILELEVELTYPE);

            ret = get_supported_profile_level(pcomp_priv, profileLevelType);
            break;
        }
        case OMX_IndexParamVideoProfileLevelCurrent:
        {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE* profileLevelType =
                (OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param_data;

            DEBUG_PRINT("get_param: OMX_IndexParamVideoProfileLevelCurrent,PortIndex = %ld,state = %ld\n",
                        (OMX_S32)profileLevelType->nPortIndex, (OMX_S32)pcomp_priv->m_state);

            if (profileLevelType->nPortIndex != OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)profileLevelType->nPortIndex);
                ret = OMX_ErrorBadPortIndex;
            }
            CONFIG_VERSION_SIZE(profileLevelType , OMX_VIDEO_PARAM_PROFILELEVELTYPE);

            ret = get_current_profile_level(pcomp_priv, profileLevelType);
            break;
        }
        case OMX_IndexParamVideoBitrate:
        {
            OMX_VIDEO_PARAM_BITRATETYPE* pVideoBitRate =
                (OMX_VIDEO_PARAM_BITRATETYPE*)param_data;

#if 0
            DEBUG_PRINT("get_param: OMX_IndexParamVideoBitrate,PortIndex = %ld,state = %ld\n",
                        (OMX_S32)pVideoBitRate->nPortIndex, (OMX_S32)pcomp_priv->m_state);
#endif

            if (pVideoBitRate->nPortIndex != OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)pVideoBitRate->nPortIndex);
                ret = OMX_ErrorBadPortIndex;
            }
            CONFIG_VERSION_SIZE(pVideoBitRate , OMX_VIDEO_PARAM_BITRATETYPE);

            pVideoBitRate->eControlRate   = (OMX_VIDEO_CONTROLRATETYPE)pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.ControlRateType;
            pVideoBitRate->nTargetBitrate = pcomp_priv->m_port[pVideoBitRate->nPortIndex].port_def.format.video.nBitrate;
#if 0
            DEBUG_PRINT("get_param: OMX_IndexParamVideoBitrate  success!->eControlRate(%d),nTargetBitrate(%lu)\n",
                        pVideoBitRate->eControlRate, (OMX_S32)pVideoBitRate->nTargetBitrate);
#endif
            break;
        }
#ifdef ANDROID        
        case OMX_HisiIndexGetParameterSet:
        {
            OMX_VIDEO_PPS_SPS_DATA* pParameterSet =
                (OMX_VIDEO_PPS_SPS_DATA*)param_data;

            DEBUG_PRINT("get_param: OMX_HisiIndexGetParameterSet\n");

            if ((pcomp_priv->m_state >= OMX_StateLoaded) && (pcomp_priv->m_state <= OMX_StateWaitForResources))
            {
                if (channel_get_sps_pps(&(pcomp_priv->drv_ctx), (DRV_VIDEO_PPS_SPS_DATA*)pParameterSet) < 0)
                {
                    DEBUG_PRINT_ERROR("ERROR: channel get SPS&PPS  failed!\n");
                    ret = OMX_ErrorHardware;
                    omx_report_error(pcomp_priv, ret);
                }
            }
            break;

        }
        case OMX_HisiIndexAutoRequestIFrm:
        {
            struct AutoRequestIFrmParams* penable = (struct AutoRequestIFrmParams*)param_data;
            venc_chan_cfg* pVencChnAttr = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg;

            DEBUG_PRINT("get_parameter: OMX_HisiIndexAutoRequestIFrm\n");

            penable->nSize             = sizeof(AutoRequestIFrmParams);
            penable->bEnable = pVencChnAttr->bAutoRequestIFrm;
            break;
        }
        case OMX_GoogleIndexPrependSPSPPSToIDRFrames:
        {
            struct PrependSPSPPSToIDRFramesParams*  penable = (struct PrependSPSPPSToIDRFramesParams*)param_data;

            DEBUG_PRINT("get_parameter: OMX_GoogleIndexPrependSPSPPSToIDRFrames-> enable(%d)\n",(int)pcomp_priv->m_prepend_sps_pps);
            penable->bEnable = pcomp_priv->m_prepend_sps_pps;
            break;
        }
		
        //extern non-standard index support
#endif        
        default:
            DEBUG_PRINT_ERROR("get_param: unknown param %08x\n",
                              param_index);
            ret = OMX_ErrorUnsupportedIndex;
            break;
    }

    return ret;
}


static OMX_ERRORTYPE set_parameter_PortDefinition(OMX_IN OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PARAM_PORTDEFINITIONTYPE *portDefn = NULL;
    OMX_PORT_PRIVATE *port_priv = NULL;
    venc_chan_cfg *pvenc_attr = NULL;

    portDefn = (OMX_PARAM_PORTDEFINITIONTYPE*)param_data;

    DEBUG_PRINT("set_param: OMX_IndexParamPortDefinition,PortIndex = %ld,state = %ld\n",
                (OMX_S32)portDefn->nPortIndex, (OMX_S32)pcomp_priv->m_state);

    DEBUG_PRINT("set_param: OMX_IndexParamPortDefinition,PortIndex(%ld),state(%ld),nBufferCountActual(%ld),nBufferCountMin(%ld),nBufferSize(%ld)\n",
                (OMX_S32)portDefn->nPortIndex, (OMX_S32)pcomp_priv->m_state,
                portDefn->nBufferCountActual,
                portDefn->nBufferCountMin,
                portDefn->nBufferSize);
    DEBUG_PRINT("set_param: OMX_IndexParamPortDefinition,bEnabled(%d),bPopulated(%d),bBuffersContiguous(%d),nBufferAlignment(%ld)\n",
                portDefn->bEnabled, portDefn->bPopulated,
                portDefn->bBuffersContiguous,
                portDefn->nBufferAlignment);
    DEBUG_PRINT("set_param: OMX_IndexParamPortDefinition,nFrameWidth(%ld),nFrameHeight(%ld),nStride(%ld),nSliceHeight(%ld),nBitrate(%ld),eCompressionFormat(%d),eColorFormat(%d)\n",
                (OMX_S32)portDefn->format.video.nFrameWidth, portDefn->format.video.nFrameHeight,
                portDefn->format.video.nStride, portDefn->format.video.nSliceHeight, portDefn->format.video.nBitrate,
                portDefn->format.video.eCompressionFormat, portDefn->format.video.eColorFormat);

    if (portDefn->nPortIndex > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT("invalid port index!\n");

        return OMX_ErrorBadPortIndex;
    }

    port_priv = &pcomp_priv->m_port[portDefn->nPortIndex];
    pvenc_attr = &(pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg);

    if (portDefn->nBufferCountActual < portDefn->nBufferCountMin)
    {
        DEBUG_PRINT("setted buf cnt too small! nBufferCountActual[%lu] should > nBufferCountMin[%lu]!\n",
                    portDefn->nBufferCountActual, portDefn->nBufferCountMin);
        return OMX_ErrorUndefined;
    }

    if ((portDefn->format.video.nFrameWidth % PIC_ALIGN_SIZE) || (portDefn->format.video.nFrameHeight % PIC_ALIGN_SIZE))
    {
        DEBUG_PRINT_ERROR("Picture Width(%lu) or Heigth(%lu) invalid, should N*%d.\n", portDefn->format.video.nFrameWidth,
                          portDefn->format.video.nFrameHeight, PIC_ALIGN_SIZE);
        return OMX_ErrorUndefined;
    }

    port_priv->port_def = *portDefn ;

    if (portDefn->nPortIndex == OUTPUT_PORT_INDEX)
    {
        port_priv->port_def.nBufferSize   = portDefn->nBufferSize;
        pvenc_attr->VencUnfAttr.u32Width  = portDefn->format.video.nFrameWidth;
        pvenc_attr->VencUnfAttr.u32Height = portDefn->format.video.nFrameHeight;
        pvenc_attr->VencUnfAttr.u32TargetFrmRate = pcomp_priv->m_port[0].port_def.format.video.xFramerate >> 16;
        if (pvenc_attr->VencUnfAttr.u32TargetFrmRate > HI_VENC_MAX_fps)
        {
            pvenc_attr->VencUnfAttr.u32TargetFrmRate = HI_VENC_MAX_fps;
        }

        pcomp_priv->m_port[1].port_def.format.video.xFramerate = pcomp_priv->m_port[0].port_def.format.video.xFramerate;
        switch (portDefn->format.video.eCompressionFormat)
        {
            case OMX_VIDEO_CodingAVC:
                pvenc_attr->VencUnfAttr.enVencType = HI_UNF_VCODEC_TYPE_H264;
                break;

            case OMX_VIDEO_CodingHEVC:
                pvenc_attr->VencUnfAttr.enVencType = HI_UNF_VCODEC_TYPE_HEVC;
                break;

            default:
                DEBUG_PRINT_ERROR("NOT support the role of %d\n", portDefn->format.video.eCompressionFormat);
                break;
        }
    }
    else
    {
        pvenc_attr->VencUnfAttr.u32InputFrmRate = portDefn->format.video.xFramerate >> 16;
        if (pvenc_attr->VencUnfAttr.u32InputFrmRate > HI_VENC_MAX_fps)
        {
            pvenc_attr->VencUnfAttr.u32InputFrmRate = HI_VENC_MAX_fps;
        }
    }

    return ret;
}

static OMX_ERRORTYPE set_parameter_VideoAVC(OMX_IN OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_VIDEO_PARAM_AVCTYPE *pAvcType = NULL;
    HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs = NULL;

    pAvcType = (OMX_VIDEO_PARAM_AVCTYPE*)param_data;
    pVencUnfAttrs = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;

    DEBUG_PRINT("set_param:OMX_IndexParamVideoAvc,PortIndex = %ld,state=%ld\n",
                (OMX_S32)pAvcType->nPortIndex, (OMX_S32)pcomp_priv->m_state);

    if ((pAvcType->nPortIndex > OUTPUT_PORT_INDEX) || (pAvcType->nPortIndex == INPUT_PORT_INDEX))
    {
        DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)pAvcType->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    if (OMX_ErrorNone != omx_check_param_videoAvc(pAvcType))
    {
        DEBUG_PRINT_ERROR(" Parameter of AVC not support!\n");
        ret = OMX_ErrorBadParameter;
    }

    pVencUnfAttrs->u32Gop = pAvcType->nPFrames + 1;

    switch (pAvcType->eProfile)
    {
        case OMX_VIDEO_AVCProfileBaseline:
            pVencUnfAttrs->enVencProfile = HI_UNF_H264_PROFILE_BASELINE;
            break;
        case OMX_VIDEO_AVCProfileMain:
            pVencUnfAttrs->enVencProfile = HI_UNF_H264_PROFILE_MAIN;
            break;
        case OMX_VIDEO_AVCProfileHigh:
            pVencUnfAttrs->enVencProfile = HI_UNF_H264_PROFILE_HIGH;
            break;
        default:
            DEBUG_PRINT_ERROR("profile : %d not support!\n", pAvcType->eProfile);
            ret = OMX_ErrorBadParameter;
            break;
    }

    if ((pcomp_priv->m_state >= OMX_StateIdle) && (pcomp_priv->m_state <= OMX_StateWaitForResources))
    {
        if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
        {
            DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
            ret = OMX_ErrorHardware;
            omx_report_error(pcomp_priv, ret);
        }
    }

    DEBUG_PRINT("set_param:OMX_IndexParamVideoAvc,PortIndex = %ld,state=%ld ret = %d\n",
                (OMX_S32)pAvcType->nPortIndex, (OMX_S32)pcomp_priv->m_state, ret);
    return ret;
}

static OMX_ERRORTYPE set_parameter_VideoBitrate(OMX_IN OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_VIDEO_PARAM_BITRATETYPE *pVideoBitRate = NULL;
    HI_UNF_VENC_CHN_ATTR_S *pVencUnfAttrs = NULL;

    pVideoBitRate = (OMX_VIDEO_PARAM_BITRATETYPE*)param_data;
    pVencUnfAttrs = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;

    DEBUG_PRINT("set_param: OMX_IndexParamVideoBitrate,PortIndex = %lu,state = %d\n",
                pVideoBitRate->nPortIndex, pcomp_priv->m_state);
    DEBUG_PRINT("set_param: OMX_IndexParamVideoBitrate  ->eControlRate(%d),nTargetBitrate(%ld)\n",
                pVideoBitRate->eControlRate, pVideoBitRate->nTargetBitrate);

    if (pVideoBitRate->nPortIndex != OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)pVideoBitRate->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.ControlRateType = (OMX_U32)pVideoBitRate->eControlRate;

    switch (pVideoBitRate->eControlRate)
    {
        case OMX_Video_ControlRateConstant:
            pVencUnfAttrs->u32MinQp   = 10;
            pVencUnfAttrs->u32MaxQp   = 50;
            break;
        case OMX_Video_ControlRateVariable:
            pVencUnfAttrs->u32MinQp   = 20;
            pVencUnfAttrs->u32MaxQp   = 30;
            break;
        default:
            DEBUG_PRINT_ERROR("ERROR: eControlRate Type(%d) can't support!!\n", pVideoBitRate->eControlRate);
            break;
    }

    pVencUnfAttrs->u32TargetBitRate   = pVideoBitRate->nTargetBitrate;
    pcomp_priv->m_port[pVideoBitRate->nPortIndex].port_def.format.video.nBitrate = pVideoBitRate->nTargetBitrate;

    if ((pcomp_priv->m_state >= OMX_StateIdle) && (pcomp_priv->m_state <= OMX_StateWaitForResources))
    {
        if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
        {
            DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
            ret = OMX_ErrorHardware;
            omx_report_error(pcomp_priv, ret);
        }
    }
    DEBUG_PRINT("set_param: OMX_IndexParamVideoBitrate  ->eControlRate(%lu),nTargetBitrate(%lu)\n",
                (OMX_U32)pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.ControlRateType, (OMX_U32)pVencUnfAttrs->u32TargetBitRate);
    return ret;
}

static OMX_ERRORTYPE set_parameter_VideoPortFormat(OMX_IN OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PORT_PRIVATE *port_priv = NULL;
    OMX_VIDEO_PARAM_PORTFORMATTYPE *portFmt = NULL;

    portFmt = (OMX_VIDEO_PARAM_PORTFORMATTYPE*)param_data;
    if (portFmt->nPortIndex > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT("invalid port index!\n");

        return OMX_ErrorBadPortIndex;
    }

    port_priv = &pcomp_priv->m_port[portFmt->nPortIndex];
    if (OUTPUT_PORT_INDEX == portFmt->nPortIndex)
    {
        OMX_U32 i;
        for (i = 0; i < COUNTOF(codec_trans_list); i++)
        {
            if (codec_trans_list[i].compress_fmt == portFmt->eCompressionFormat)
            { break; }
        }

        if (i >= COUNTOF(codec_trans_list))
        {
            DEBUG_PRINT_ERROR("fmt %d not support\n",
                              portFmt->eCompressionFormat);
            ret = OMX_ErrorUnsupportedSetting;
        }
        else
        {
            strncpy((char*)pcomp_priv->m_role,
                    (char*)codec_trans_list[i].role_name, OMX_MAX_STRINGNAME_SIZE - 1);

            pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr.enVencType =
                codec_trans_list[i].codec_type;

            pcomp_priv->m_encoder_fmt =
                codec_trans_list[i].compress_fmt;
        }
        port_priv->port_def.format.video.eColorFormat       = OMX_COLOR_FormatUnused;
        port_priv->port_def.format.video.eCompressionFormat = portFmt->eCompressionFormat;

    }
    else if (INPUT_PORT_INDEX == portFmt->nPortIndex)
    {
        if ((portFmt->eColorFormat    != OMX_COLOR_FormatYUV420SemiPlanar)  /* YCbCr 420 SP*/
            && (portFmt->eColorFormat != OMX_QCOM_COLOR_FormatYVU420SemiPlanar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYUV420Planar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYCbYCr)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYCrYCb)
            && (portFmt->eColorFormat != OMX_COLOR_FormatCbYCrY)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYUV422SemiPlanar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYVU420SemiPlanar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYVU422SemiPlanar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatYVU420Planar)
            && (portFmt->eColorFormat != OMX_COLOR_FormatAndroidOpaque))
        {
            DEBUG_PRINT_ERROR("Set output format failed\n");
            ret = OMX_ErrorUnsupportedSetting;
        }

        port_priv->port_def.format.video.eColorFormat       = portFmt->eColorFormat;
        port_priv->port_def.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
    }
    else
    {
        DEBUG_PRINT_ERROR("get_parameter: Bad port index %ld\n",
                          (OMX_S32)portFmt->nPortIndex);
        ret = OMX_ErrorBadPortIndex;
    }

    return ret;
}

static OMX_ERRORTYPE set_parameter_StandardComponentRole(OMX_IN OMX_PTR param_data, OMX_COMPONENT_PRIVATE *pcomp_priv)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_PARAM_COMPONENTROLETYPE *comp_role = NULL;
    OMX_U32 i = 0;

    comp_role = (OMX_PARAM_COMPONENTROLETYPE*)param_data;
    DEBUG_PRINT("set_param: OMX_IndexParamStandardComponentRole\n");

    for (i = 0; i < COUNTOF(codec_trans_list); i++)
    {
        if (!strncmp((char*)codec_trans_list[i].role_name,
                     (char*)comp_role->cRole, OMX_MAX_STRINGNAME_SIZE))
        {
            break;
        }
    }

    if (i >= COUNTOF(codec_trans_list))
    {
        DEBUG_PRINT_ERROR("fmt %s not support\n", comp_role->cRole);
        ret = OMX_ErrorUnsupportedSetting;
    }
    else
    {
        strncpy((char*)pcomp_priv->m_role,
                (char*)codec_trans_list[i].role_name, OMX_MAX_STRINGNAME_SIZE - 1);

        pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr.enVencType =
            codec_trans_list[i].codec_type;

        pcomp_priv->m_encoder_fmt = codec_trans_list[i].compress_fmt;
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
static OMX_ERRORTYPE  set_parameter(OMX_IN OMX_HANDLETYPE phandle,
                                    OMX_IN OMX_INDEXTYPE param_index,
                                    OMX_IN OMX_PTR param_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE *pcomp = NULL;
    OMX_COMPONENT_PRIVATE *pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(param_data == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    if (!pcomp_priv)
    {
        DEBUG_PRINT_ERROR("set_parameter: pcomp_priv is null\n");

        return OMX_ErrorBadParameter;
    }

    switch (param_index)
    {
        case OMX_IndexParamPortDefinition:
        {
            ret = set_parameter_PortDefinition(param_data, pcomp_priv);
            break;
        }
        case OMX_IndexParamVideoAvc:
        {
            ret = set_parameter_VideoAVC(param_data, pcomp_priv);
            break;
        }
        case OMX_IndexParamVideoBitrate:
        {
            ret = set_parameter_VideoBitrate(param_data, pcomp_priv);
            break;
        }

        case OMX_IndexParamVideoPortFormat:
        {
            ret = set_parameter_VideoPortFormat(param_data, pcomp_priv);
            break;
        }

#ifdef KHRONOS_1_1
        case OMX_IndexParamStandardComponentRole:
        {
            ret = set_parameter_StandardComponentRole(param_data, pcomp_priv);
            break;
        }
#endif

        case OMX_IndexParamVideoProfileLevelCurrent:
        {
            OMX_VIDEO_PARAM_PROFILELEVELTYPE *profileLevelType =
                (OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param_data;

            DEBUG_PRINT("set_param: OMX_IndexParamVideoProfileLevelCurrent,PortIndex = %ld,state = %ld\n",
                        (OMX_S32)profileLevelType->nPortIndex, (OMX_S32)pcomp_priv->m_state);

            if (profileLevelType->nPortIndex != OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("Bad Port idx %ld\n", (OMX_S32)profileLevelType->nPortIndex);
                ret = OMX_ErrorBadPortIndex;
            }

            ret = set_current_profile_level(pcomp_priv, profileLevelType);
            break;
        }

#ifdef ANDROID
        case OMX_GoogleIndexEnableAndroidNativeBuffers:
        {
            struct EnableAndroidNativeBuffersParams *penable = (struct EnableAndroidNativeBuffersParams*)param_data;

            DEBUG_PRINT("set_parameter: OMX_GoogleIndexEnableAndroidNativeBuffers-> nPortIndex (%d),enable(%d)\n",
                        (int)penable->nPortIndex, (int)penable->bEnable);

            if (penable->nPortIndex != OUTPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("set_parameter: Bad port index %d\n", (int)penable->nPortIndex);

                return OMX_ErrorBadPortIndex;
            }

            pcomp_priv->m_use_native_buf = penable->bEnable;

            break;
        }

        case OMX_GoogleIndexStoreMetaDataInBuffers:
        {
            struct StoreMetaDataInBuffersParams *penable = (struct StoreMetaDataInBuffersParams*)param_data;

            DEBUG_PRINT("set_parameter: OMX_GoogleIndexStoreMetaDataInBuffers-> nPortIndex (%d),enable(%d)\n",
                        (int)penable->nPortIndex, (int)penable->bStoreMetaData);

            if (penable->nPortIndex != INPUT_PORT_INDEX)
            {
                DEBUG_PRINT_ERROR("set_parameter: Bad port index %d\n", (int)penable->nPortIndex);

                return OMX_ErrorBadPortIndex;
            }

            pcomp_priv->m_store_metadata_buf = penable->bStoreMetaData;

            break;
        }

        case OMX_GoogleIndexPrependSPSPPSToIDRFrames:
        {
            struct PrependSPSPPSToIDRFramesParams *penable = (struct PrependSPSPPSToIDRFramesParams*)param_data;
            venc_chan_cfg *pVencChnAttr = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg;

            DEBUG_PRINT("set_parameter: OMX_GoogleIndexPrependSPSPPSToIDRFrames-> enable(%d)\n", (int)penable->bEnable);

            if (0 == pcomp_priv->drv_ctx.venc_chan_attr.state)   //not create Chn yet
            {
                pVencChnAttr->bPrependSpsPps = penable->bEnable;
            }
            else
            {
                DEBUG_PRINT_ERROR("ERROR: set_parameter: OMX_HisiIndexAutoRequestIFrm-> enable(%d) no use!\n", penable->bEnable);
            }

            pcomp_priv->m_prepend_sps_pps = penable->bEnable;

            break;
        }

        case OMX_HisiIndexAutoRequestIFrm:
        {
            struct AutoRequestIFrmParams *penable = (struct AutoRequestIFrmParams*)param_data;
            venc_chan_cfg *pVencChnAttr = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg;

            DEBUG_PRINT("set_parameter: OMX_HisiIndexAutoRequestIFrm-> enable(%d)\n", (int)penable->bEnable);

            if (0 == pcomp_priv->drv_ctx.venc_chan_attr.state)   //not create Chn yet
            {
                pVencChnAttr->bAutoRequestIFrm = penable->bEnable;
            }
            else
            {
                DEBUG_PRINT_ERROR("ERROR: set_parameter: OMX_HisiIndexAutoRequestIFrm-> enable(%d) no use!\n", penable->bEnable);
            }
            break;
        }
#endif		
        default:
            DEBUG_PRINT_ERROR("set_param: unknown param 0x%08x\n", param_index);
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
static OMX_ERRORTYPE  get_config(OMX_IN OMX_HANDLETYPE   phandle,
                                 OMX_IN OMX_INDEXTYPE config_index,
                                 OMX_INOUT OMX_PTR     config_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcomp_priv = NULL;
    //OMX_CONFIG_RECTTYPE *prect = NULL;
    HI_UNF_VENC_CHN_ATTR_S* pVencUnfAttrs = NULL;
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(config_data == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    if (!pcomp_priv)
    {
        DEBUG_PRINT_ERROR("get_config: pcomp_priv = NULL!\n");
        return OMX_ErrorBadParameter;
    }

    pVencUnfAttrs = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;

    switch (config_index)
    {
        case OMX_IndexConfigCommonOutputCrop:
        {
            OMX_CONFIG_RECTTYPE* prect = (OMX_CONFIG_RECTTYPE*)config_data;

            DEBUG_PRINT("get_config: OMX_IndexConfigCommonOutputCrop\n");

            if (prect->nPortIndex != OUTPUT_PORT_INDEX)
            { return OMX_ErrorBadPortIndex; }
#if 0
            prect->nLeft    = pcomp_priv->pic_info.crop_left;
            prect->nTop     = pcomp_priv->pic_info.crop_top;
            prect->nHeight  = pcomp_priv->pic_info.crop_height;
            prect->nWidth   = pcomp_priv->pic_info.crop_width;
#endif
            break;
        }
        case OMX_IndexConfigVideoAVCIntraPeriod:
        {
            OMX_VIDEO_CONFIG_AVCINTRAPERIOD* ptr_gop = (OMX_VIDEO_CONFIG_AVCINTRAPERIOD*)config_data;
            DEBUG_PRINT("get_config: OMX_IndexConfigVideoAVCIntraPeriod\n");
            if (ptr_gop->nPortIndex != OUTPUT_PORT_INDEX)
            { return OMX_ErrorBadPortIndex; }
            ptr_gop->nIDRPeriod = pVencUnfAttrs->u32Gop;
            ptr_gop->nPFrames   = pVencUnfAttrs->u32Gop - 1;

            break;
        }
        case OMX_IndexConfigVideoFramerate:
        {
            OMX_CONFIG_FRAMERATETYPE* ptr_framerate = (OMX_CONFIG_FRAMERATETYPE*)config_data;
            DEBUG_PRINT("get_config: OMX_IndexConfigVideoFramerate\n");
            if (ptr_framerate->nPortIndex == OUTPUT_PORT_INDEX)  //get the output framerate
            {
                ptr_framerate->xEncodeFramerate = pVencUnfAttrs->u32TargetFrmRate << 16;
            }
            else                                       //get the input  framerate
            {
                ptr_framerate->xEncodeFramerate = pVencUnfAttrs->u32InputFrmRate << 16;
            }
            break;
        }
        case OMX_IndexConfigVideoBitrate:
        {
            OMX_VIDEO_CONFIG_BITRATETYPE* ptr_bitrate =  (OMX_VIDEO_CONFIG_BITRATETYPE*)config_data;
            DEBUG_PRINT("get_config: OMX_IndexConfigVideoBitrate\n");
            if (ptr_bitrate->nPortIndex != OUTPUT_PORT_INDEX)  //get the output framerate
            {
                return OMX_ErrorBadPortIndex;
            }

            ptr_bitrate->nEncodeBitrate = pVencUnfAttrs->u32TargetBitRate;
            break;
        }
        default:
            DEBUG_PRINT_ERROR("get_config: unknown index 0x%08x\n",
                              config_index);
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
static OMX_ERRORTYPE  set_config(OMX_IN OMX_HANDLETYPE phandle,
                                 OMX_IN OMX_INDEXTYPE config_index, OMX_IN OMX_PTR config_data)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcomp_priv = NULL;
    HI_UNF_VENC_CHN_ATTR_S* pVencUnfAttrs = NULL;
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(config_data == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    if (!pcomp_priv)
    {
        DEBUG_PRINT_ERROR("get_config: pcomp_priv = NULL!\n");
        return OMX_ErrorBadParameter;
    }

    pVencUnfAttrs = &pcomp_priv->drv_ctx.venc_chan_attr.chan_cfg.VencUnfAttr;
    switch (config_index)
    {
        case OMX_IndexConfigCommonOutputCrop:
        {
            OMX_CONFIG_RECTTYPE* prect = (OMX_CONFIG_RECTTYPE*)config_data;

            DEBUG_PRINT("set_config: OMX_IndexConfigCommonOutputCrop\n");

            if (prect->nPortIndex != OUTPUT_PORT_INDEX)
            { return OMX_ErrorBadPortIndex; }
#if 0
            prect->nLeft    = pcomp_priv->pic_info.crop_left;
            prect->nTop     = pcomp_priv->pic_info.crop_top;
            prect->nHeight  = pcomp_priv->pic_info.crop_height;
            prect->nWidth   = pcomp_priv->pic_info.crop_width;
#endif
            break;
        }
        case OMX_IndexConfigVideoAVCIntraPeriod:
        {
            OMX_VIDEO_CONFIG_AVCINTRAPERIOD* ptr_gop = (OMX_VIDEO_CONFIG_AVCINTRAPERIOD*)config_data;
            DEBUG_PRINT("set_config: OMX_IndexConfigVideoAVCIntraPeriod\n");
            if (ptr_gop->nPortIndex != OUTPUT_PORT_INDEX)
            { return OMX_ErrorBadPortIndex; }
            pVencUnfAttrs->u32Gop = ptr_gop->nIDRPeriod;

            if (0 != pcomp_priv->drv_ctx.venc_chan_attr.state)
            {
                if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
                {
                    DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
                    ret = OMX_ErrorHardware;
                    omx_report_error(pcomp_priv, ret);
                }
            }
            break;
        }
        case OMX_IndexConfigVideoFramerate:
        {
            OMX_CONFIG_FRAMERATETYPE* ptr_framerate = (OMX_CONFIG_FRAMERATETYPE*)config_data;
            DEBUG_PRINT("set_config: OMX_IndexConfigVideoFramerate:port(%lu)\n", ptr_framerate->nPortIndex);
#if 0	 /*omx don't use frameRate control*/
            if (ptr_framerate->nPortIndex == OUTPUT_PORT_INDEX)  //get the output framerate
            {
                pVencUnfAttrs->u32TargetFrmRate = ptr_framerate->xEncodeFramerate >> 16 ;
            }
            else                                       //get the input  framerate
            {
                pVencUnfAttrs->u32InputFrmRate = ptr_framerate->xEncodeFramerate >> 16;
            }
#else
            if (ptr_framerate->nPortIndex > OUTPUT_PORT_INDEX)
            {
                return OMX_ErrorBadPortIndex;
            }
            pVencUnfAttrs->u32TargetFrmRate = ptr_framerate->xEncodeFramerate >> 16 ;
            pVencUnfAttrs->u32InputFrmRate  = ptr_framerate->xEncodeFramerate >> 16 ;
            pcomp_priv->m_port[ptr_framerate->nPortIndex].port_def.format.video.xFramerate = ptr_framerate->xEncodeFramerate;
#endif
            if (0 != pcomp_priv->drv_ctx.venc_chan_attr.state)
            {
                if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
                {
                    DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
                    ret = OMX_ErrorHardware;
                    omx_report_error(pcomp_priv, ret);
                    return ret;
                }
            }


            break;
        }
        case OMX_IndexConfigVideoBitrate:
        {
            OMX_VIDEO_CONFIG_BITRATETYPE* ptr_bitrate =  (OMX_VIDEO_CONFIG_BITRATETYPE*)config_data;
            DEBUG_PRINT("set_config: OMX_IndexConfigVideoBitrate\n");
            if (ptr_bitrate->nPortIndex != OUTPUT_PORT_INDEX)  //get the output framerate
            {
                return OMX_ErrorBadPortIndex;
            }
            pVencUnfAttrs->u32TargetBitRate = ptr_bitrate->nEncodeBitrate ;
            pcomp_priv->m_port[ptr_bitrate->nPortIndex].port_def.format.video.nBitrate = ptr_bitrate->nEncodeBitrate;
            if (0 != pcomp_priv->drv_ctx.venc_chan_attr.state)
            {
                if (channel_set_attr(&(pcomp_priv->drv_ctx)) < 0)
                {
                    DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
                    ret = OMX_ErrorHardware;
                    omx_report_error(pcomp_priv, ret);
                }
            }
            break;
        }
        case OMX_IndexConfigVideoIntraVOPRefresh:
        {
            OMX_CONFIG_INTRAREFRESHVOPTYPE* pVideoIntraVOPRefresh = (OMX_CONFIG_INTRAREFRESHVOPTYPE*)config_data;
            DEBUG_PRINT("set_config: OMX_IndexConfigVideoIntraVOPRefresh\n");
            if (pVideoIntraVOPRefresh->nPortIndex != OUTPUT_PORT_INDEX)
            {
                return OMX_ErrorBadPortIndex;
            }

            if (pVideoIntraVOPRefresh->IntraRefreshVOP)
            {
                if (1 == pcomp_priv->drv_ctx.venc_chan_attr.state) /*if component's state is Excuting*/
                {
                    if (channel_request_IFrame(&(pcomp_priv->drv_ctx)) < 0)
                    {
                        DEBUG_PRINT_ERROR("ERROR: channel set attr failed!\n");
                        ret = OMX_ErrorHardware;
                        omx_report_error(pcomp_priv, ret);
                    }
                }
            }
            break;
        }
        default:
            DEBUG_PRINT_ERROR("set_config: unknown index 0x%08x\n",
                              config_index);
            ret = OMX_ErrorBadParameter;
            break;
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
static OMX_ERRORTYPE  get_extension_index(OMX_IN OMX_HANDLETYPE phandle,
        OMX_IN OMX_STRING param_name,
        OMX_OUT OMX_INDEXTYPE* pindex_type)
{
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(param_name == NULL);
    OMX_CHECK_ARG_RETURN(pindex_type == NULL);


    OMX_U32 param_length = strlen(param_name);

    if (!strncmp(param_name, "OMX.Hisi.Param.Index.ChannelAttributes",\
                MAX(param_length, strlen("OMX.Hisi.Param.Index.ChannelAttributes"))))
    {
        *pindex_type = OMX_HisiIndexChannelAttributes;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.prependSPSPPSToIDRFrames",\
                      MAX(param_length, strlen("OMX.google.android.index.prependSPSPPSToIDRFrames"))))
    {
        *pindex_type = OMX_GoogleIndexPrependSPSPPSToIDRFrames;
    }	
    //#ifdef _ANDROID_
    else if (!strncmp(param_name, "OMX.google.android.index.enableAndroidNativeBuffers",\
                      MAX(param_length, strlen("OMX.google.android.index.enableAndroidNativeBuffers"))))
    {
        *pindex_type = OMX_GoogleIndexEnableAndroidNativeBuffers;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.storeMetaDataInBuffers",\
                      MAX(param_length, strlen("OMX.google.android.index.storeMetaDataInBuffers"))))
    {
        *pindex_type = OMX_GoogleIndexStoreMetaDataInBuffers;
    }
    else if (!strncmp(param_name, "OMX.google.android.index.useAndroidNativeBuffer2",\
                      MAX(param_length, strlen("OMX.google.android.index.useAndroidNativeBuffer2"))))
    {
        *pindex_type = OMX_GoogleIndexUseAndroidNativeBuffer;
    }
    else if (!strncmp(param_name, "OMX.Hisi.android.index.getParameterSet",\
                      MAX(param_length, strlen("OMX.Hisi.android.index.getParameterSet"))))
    {
        *pindex_type = OMX_HisiIndexGetParameterSet;
    }
    else if (!strncmp(param_name, "OMX.Hisi.android.index.autoRequestIFrmScnChg",\
                      MAX(param_length, strlen("OMX.Hisi.android.index.autoRequestIFrmScnChg"))))
    {
        *pindex_type = OMX_HisiIndexAutoRequestIFrm;
    }
    //#endif
    else
    {
        DEBUG_PRINT_ERROR("%s not implemented!\n", param_name);
        return OMX_ErrorNotImplemented;

    }

    DEBUG_PRINT("get_extension_index : param_name:%s pindex_type = %d!\n", param_name, *pindex_type);
    
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
static OMX_ERRORTYPE get_state(OMX_IN OMX_HANDLETYPE  phandle,
                               OMX_OUT OMX_STATETYPE* state)
{
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcomp_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(state == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcomp_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

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
    OMX_INOUT OMX_TUNNELSETUPTYPE* tunnelSetup)
{
    DEBUG_PRINT_ERROR("Error: Tunnel mode Not Implemented\n");
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
static OMX_ERRORTYPE  allocate_buffer(OMX_IN OMX_HANDLETYPE  phandle,
                                      OMX_INOUT OMX_BUFFERHEADERTYPE** omx_bufhdr,
                                      OMX_IN OMX_U32 port,
                                      OMX_IN OMX_PTR app_data,
                                      OMX_IN OMX_U32 bytes)
{
    OMX_ERRORTYPE ret                = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pcomp         = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
    OMX_PORT_PRIVATE* port_priv      = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(omx_bufhdr == NULL);

    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[AB]Error: Invalid Port %ld\n", (OMX_S32)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp     = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[AB] ERROR: pcom_priv = NULL!\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateIdle) ||
        (pcom_priv->m_state == OMX_StateExecuting) ||
        ((pcom_priv->m_state == OMX_StateLoaded) &&
         bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING)))                      //只有这三种情况下才能申请内存?
    {
        DEBUG_PRINT("[AB] Current State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT("[AB] Invalid State %d to alloc the buffer !\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

    ret = allocate_buffer_internal(                                                             //内部申请buffer函数
              pcom_priv, omx_bufhdr, app_data, port, bytes);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("[AB] ERROR: Allocate Buf failed\n");
        return ret;
    }

    if ((port == INPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->port_def.bEnabled)
        { port_priv->port_def.bPopulated = OMX_TRUE; }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING))
        {
            bit_clear(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING);

            post_event(pcom_priv,                                                        //把 命令:OMX_CommandPortEnable 压入队列，写入管道
                       OMX_CommandPortEnable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if ((port == OUTPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->port_def.bEnabled)
        { port_priv->port_def.bPopulated = OMX_TRUE; }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Enable_Pending --> Enable\n");
            bit_clear(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING);

            post_event(pcom_priv,
                       OMX_CommandPortEnable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
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
   FreeBuffer

   DESCRIPTION

   PARAMETERS
   None.

   RETURN VALUE
   OMX_TRUE/OMX_FALSE
   ======================================================================*/
static OMX_ERRORTYPE  free_buffer(OMX_IN OMX_HANDLETYPE phandle,
                                  OMX_IN OMX_U32 port,
                                  OMX_IN OMX_BUFFERHEADERTYPE* omx_bufhdr)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
#ifdef __OMXVENC_RGB_BUFFER__    
    HI_MMZ_BUF_S stBuf_RGB;
#endif

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(omx_bufhdr == NULL);
    DEBUG_PRINT(" free_buffer : %lu!\n", port);
    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[AB]Error: Invalid Port %ld\n", (OMX_S32)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[AB] ERROR: Invalid State\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateLoaded) ||
        (pcom_priv->m_state == OMX_StateExecuting) ||
        ((pcom_priv->m_state == OMX_StateIdle) &&
         bit_present(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING)))
    {
        DEBUG_PRINT("[AB] Current State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT("[AB] Invalid State %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

    ret = free_buffer_internal(pcom_priv, port, omx_bufhdr);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("[FB]ERROR: free Buf internal failed\n");
        return ret;
    }

    if ((port == INPUT_PORT_INDEX) && port_empty(pcom_priv, port))
    {
        if (bit_present((&pcom_priv->m_flags),
                        OMX_STATE_INPUT_DISABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[ST]port state Disable Pending ->Disable\n");
            bit_clear((&pcom_priv->m_flags),
                      OMX_STATE_INPUT_DISABLE_PENDING);

            post_event(pcom_priv, OMX_CommandPortDisable,
                       INPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }
    }

    if ((port == OUTPUT_PORT_INDEX) && port_empty(pcom_priv, port))
    {

        if (bit_present(&pcom_priv->m_flags,
                        OMX_STATE_OUTPUT_DISABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[ST]port state Disable Pending ->Disable\n");
            bit_clear((&pcom_priv->m_flags),
                      OMX_STATE_OUTPUT_DISABLE_PENDING);

            post_event(pcom_priv, OMX_CommandPortDisable,
                       OUTPUT_PORT_INDEX,
                       OMX_GENERATE_COMMAND_DONE);
        }
    }

    if (ports_all_empty(pcom_priv))
    {
        if (bit_present(&pcom_priv->m_flags, OMX_STATE_LOADING_PENDING))
        {
            DEBUG_PRINT_STATE("loaded-pending ->loaded\n");
            bit_clear((&pcom_priv->m_flags),
                      OMX_STATE_LOADING_PENDING);

            post_event(pcom_priv, OMX_CommandStateSet,
                       OMX_StateLoaded,
                       OMX_GENERATE_COMMAND_DONE);

            pcom_priv->msg_thread_exit = OMX_TRUE;
            //pthread_join(pcom_priv->msg_thread_id, NULL);

            if ( channel_destroy(&pcom_priv->drv_ctx) < 0)
            {
                DEBUG_PRINT_ERROR("ERROR: channel create failed!\n");
                ret = OMX_ErrorHardware;
            }
#ifdef __OMXVENC_RGB_BUFFER__
            stBuf_RGB.phyaddr      = pcom_priv->bufferaddr_Phy_RGB;
            stBuf_RGB.user_viraddr = pcom_priv->bufferaddr_RGB;
            stBuf_RGB.bufsize      = pcom_priv->buffer_size_RGB;
            if (pcom_priv->m_store_metadata_buf)
            {
                if (HI_MMZ_Free(&stBuf_RGB) != 0)
                {
                    DEBUG_PRINT_ERROR("ERROR: channel free RGB buffer for StoreMetaData failed!\n");
				    ret = OMX_ErrorHardware;
                }
                pcom_priv->bufferaddr_Phy_RGB = 0;
                pcom_priv->bufferaddr_RGB     = 0;
                pcom_priv->buffer_size_RGB    = 0;
            } 
#endif
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
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(bufferHdr == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    if (port > OUTPUT_PORT_INDEX)
    {
        DEBUG_PRINT_ERROR("[UB] Error: Invalid Port %d\n", (int)port);
        return OMX_ErrorBadPortIndex;
    }

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    //state check
    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("[UB] ERROR: Invalid State\n");
        return OMX_ErrorBadParameter;
    }

    if ((pcom_priv->m_state == OMX_StateIdle) || (pcom_priv->m_state == OMX_StateExecuting) ||
        ((pcom_priv->m_state == OMX_StateLoaded) && bit_present(&pcom_priv->m_flags, OMX_STATE_IDLE_PENDING)))
    {
        DEBUG_PRINT("[UB] Cur State %d\n", pcom_priv->m_state);
    }
    else
    {
        DEBUG_PRINT_ERROR("[UB] Invalid State %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }

#ifdef ANDROID
    if (pcom_priv->m_use_native_buf /*&& OUTPUT_PORT_INDEX == port*/)
    {
        ret = use_android_native_buffer_internal(pcom_priv, bufferHdr, app_data, port, bytes, buffer);
        if (ret != OMX_ErrorNone)
        {
            DEBUG_PRINT_ERROR("[UB]ERROR: use_android_native_buffer_internal failed\n");
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

        if (port_priv->port_def.bEnabled)
        { port_priv->port_def.bPopulated = OMX_TRUE; }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING))
        {
            bit_clear(&pcom_priv->m_flags, OMX_STATE_INPUT_ENABLE_PENDING);
            post_event(pcom_priv,                                                        //把 命令:OMX_CommandPortEnable 压入队列，写入管道
                       OMX_CommandPortEnable, INPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
        }
    }

    if ((port == OUTPUT_PORT_INDEX) && port_full(pcom_priv, port))
    {
        port_priv = &pcom_priv->m_port[port];

        if (port_priv->port_def.bEnabled)
        { port_priv->port_def.bPopulated = OMX_TRUE; }

        if (bit_present(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING))
        {
            DEBUG_PRINT_STATE("[OmxState] Enable_Pending --> Enable\n");
            bit_clear(&pcom_priv->m_flags, OMX_STATE_OUTPUT_ENABLE_PENDING);

            post_event(pcom_priv,
                       OMX_CommandPortEnable, OUTPUT_PORT_INDEX, OMX_GENERATE_COMMAND_DONE);
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
static OMX_ERRORTYPE  empty_this_buffer(OMX_IN OMX_HANDLETYPE phandle,
                                        OMX_IN OMX_BUFFERHEADERTYPE* buffer)
{
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_U32 i;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;
    port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];

    if (pcom_priv->m_state != OMX_StateExecuting)
    {
        DEBUG_PRINT_ERROR("[ETB]ERROR: Invalid State\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    if (!port_priv->port_def.bEnabled)
    {
        DEBUG_PRINT_ERROR("[ETB]ERROR: in port disabled.\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (buffer == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_def.nBufferCountActual)
    {
        DEBUG_PRINT_ERROR("no buffers found for address[%p]", buffer);
        return OMX_ErrorBadParameter;
    }

    if (buffer->nInputPortIndex != port_priv->port_def.nPortIndex/*INPUT_PORT_INDEX*/)
    {
        DEBUG_PRINT_ERROR("[ETB]ERROR:ETB invalid port \n");
        return OMX_ErrorBadPortIndex;
    }

    DEBUG_PRINT("[ETB]bufhdr = %p, bufhdr->pbuffer = %p,nTimeStamp = %lld\n", buffer, buffer->pBuffer, buffer->nTimeStamp);
    post_event(pcom_priv, (OMX_U32)buffer, 0, OMX_GENERATE_ETB);

#ifdef __VENC_DBG_DELAY_STAT__
    if (etb_id < 100)
    {
        OMX_S32 ret = 0;
        ret = gettimeofday (&TmpTime[etb_id][0] , NULL);
        if (ret == 0)
        {
            etb_id++;
        }
    }
#endif

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
static OMX_ERRORTYPE  fill_this_buffer(OMX_IN OMX_HANDLETYPE  phandle,
                                       OMX_IN OMX_BUFFERHEADERTYPE* buffer)
{
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;
    OMX_U32 i;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(buffer == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;
    port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];         //should be :port_priv = &pcom_priv->m_port[buffer->nOutputPortIndex]

    /* check component state */
    if (pcom_priv->m_state != OMX_StateExecuting)
    {
        DEBUG_PRINT_ERROR("[FTB]ERROR:Invalid State\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    /* check port state */
    if (!port_priv->port_def.bEnabled)
    {
        DEBUG_PRINT_ERROR("[FTB]ERROR: out port disabled!\n");
        return OMX_ErrorIncorrectStateOperation;
    }

    /* check buffer validate */
    for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
    {
        if (buffer == port_priv->m_omx_bufhead[i])
        {
            break;
        }
    }

    if (i >= port_priv->port_def.nBufferCountActual)
    {
        DEBUG_PRINT_ERROR("[FTB]ERROR: buffers[%p] match failed\n", buffer);
        return OMX_ErrorBadParameter;
    }

    if (buffer->nOutputPortIndex != port_priv->port_def.nPortIndex/*OUTPUT_PORT_INDEX*/)
    {
        DEBUG_PRINT_ERROR("[FTB]ERROR:FTB invalid port\n");
        return OMX_ErrorBadParameter;
    }

    DEBUG_PRINT_STREAM("[FTB] bufhdr = %p, bufhdr->pBuffer = %p\n",
                       buffer, buffer->pBuffer);
    /*DEBUG_PRINT_STREAM("buf_len = %lu, offset = %lu\n",
    		buffer->nAllocLen, buffer->nOffset);    */

    post_event(pcom_priv, (OMX_U32)buffer, 0, OMX_GENERATE_FTB);
#ifdef ENABLE_BUFFER_LOCK
    sem_post(&pcom_priv->m_buf_lock);
#endif
#ifdef __VENC_DBG_DELAY_STAT__
    if (ftb_id <= 100)
    {
        int ret = 0;
        ret = gettimeofday (&TmpTime[ftb_id][3] , NULL);
        if (ret == 0)
        {
            ftb_id++;
        }
    }
#endif

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
static OMX_ERRORTYPE  set_callbacks(OMX_IN OMX_HANDLETYPE phandle,
                                    OMX_IN OMX_CALLBACKTYPE* callbacks, OMX_IN OMX_PTR app_data)
{
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;

    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(callbacks == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->EventHandler == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->EmptyBufferDone == NULL);
    OMX_CHECK_ARG_RETURN(callbacks->FillBufferDone == NULL);

    pcomp = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

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
    OMX_IN OMX_HANDLETYPE phandle, OMX_OUT OMX_U8* role,
    OMX_IN OMX_U32 nindex)
{
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_CHECK_ARG_RETURN(phandle == NULL);
    OMX_CHECK_ARG_RETURN(role == NULL);

    if (nindex > COUNTOF(codec_trans_list) - 1)
    {
        DEBUG_PRINT("component_role_enum: no more roles\n");
        return OMX_ErrorNoMore;
    }

    strncpy((char*)role,
            (char*)codec_trans_list[nindex].role_name, OMX_MAX_STRINGNAME_SIZE - 1);

    return ret;
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
    OMX_COMPONENTTYPE* pcomp = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;
    OMX_PORT_PRIVATE* port_priv = NULL;

    OMX_U32 i = 0;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    pcomp     = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)pcomp->pComponentPrivate;

    if (OMX_StateLoaded != pcom_priv->m_state)                           //just can deinit the component when it is in the state of OMX_StateLoaded
    {
        DEBUG_PRINT_ERROR("OMX not in LOADED state!\n");
        DEBUG_PRINT_ERROR("current state %d\n", pcom_priv->m_state);
        return OMX_ErrorIncorrectStateOperation;
    }
    venc_deinit_drv_context(&pcom_priv->drv_ctx);                        // the venc hardwork device should be deinit
#ifdef __OMXVENC_RGB_BUFFER__
    {
        HI_MMZ_BUF_S stBuf_RGB; 
        if (pcom_priv->bufferaddr_Phy_RGB != 0)
        {
            stBuf_RGB.phyaddr      = pcom_priv->bufferaddr_Phy_RGB;
            stBuf_RGB.user_viraddr = pcom_priv->bufferaddr_RGB;
            stBuf_RGB.bufsize      = pcom_priv->buffer_size_RGB; 
            if (HI_MMZ_Free(&stBuf_RGB) != 0)
            {
                DEBUG_PRINT_ERROR("ERROR: channel free RGB buffer for StoreMetaData failed!\n");
            }
            pcom_priv->bufferaddr_Phy_RGB = 0;
            pcom_priv->bufferaddr_RGB     = 0;
            pcom_priv->buffer_size_RGB    = 0;
        } 
    }
#endif
    /* Check port is deinit */
    if (!port_empty(pcom_priv, OUTPUT_PORT_INDEX))
    {
        port_priv = &pcom_priv->m_port[OUTPUT_PORT_INDEX];
        for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
        {
            free_buffer_internal(pcom_priv,
                                 OUTPUT_PORT_INDEX, port_priv->m_omx_bufhead[i]);
        }
    }
    if (!port_empty(pcom_priv, INPUT_PORT_INDEX))
    {
        port_priv = &pcom_priv->m_port[INPUT_PORT_INDEX];
        for (i = 0; i < port_priv->port_def.nBufferCountActual; i++)
        {
            free_buffer_internal(pcom_priv,
                                 INPUT_PORT_INDEX, port_priv->m_omx_bufhead[i]);
        }
    }

    ports_deinit(pcom_priv);
    if (!pcom_priv->msg_thread_exit)
    {
        pcom_priv->msg_thread_exit = OMX_TRUE;
        pthread_join(pcom_priv->msg_thread_id, NULL);
    }

    ///////////////////// for tde close
    //if ((pcom_priv->hTDE != 0) || (pcom_priv->hTDE != HI_INVALID_HANDLE))
    if (pcom_priv->m_store_metadata_buf == 1)
    {
        //HI_TDE2_Close();
    }
    /////////////////////
    close(pcom_priv->m_pipe_in);
    close(pcom_priv->m_pipe_out);
    sem_wait(&pcom_priv->m_async_sem);
    if (!pcom_priv->event_thread_exit)
    {
        pcom_priv->event_thread_exit = OMX_TRUE;
		sem_post(&pcom_priv->m_async_sem);
        pthread_join(pcom_priv->event_thread_id, NULL);
    }
	else
	{
	    sem_post(&pcom_priv->m_async_sem);
	}
    pthread_mutex_destroy(&pcom_priv->m_lock);
    sem_destroy(&pcom_priv->m_cmd_lock);
    sem_destroy(&pcom_priv->m_async_sem);
#ifdef ENABLE_BUFFER_LOCK
    sem_post(&pcom_priv->m_buf_lock);
    sem_destroy(&pcom_priv->m_buf_lock);
#endif
    free(pcom_priv);
    pcom_priv = NULL;
    pcomp->pComponentPrivate = NULL;

#ifdef __OMXVENC_FILE_OUT_SAVE__
    if (g_pFiletoSave != NULL)
    {
        fclose(g_pFiletoSave);
    }
#endif

#ifdef __OMXVENC_FILE_IN_SAVE__
    if (g_pFiletoEnc != NULL)
    {
        fclose(g_pFiletoEnc);
        free(g_pU);
        free(g_pV);
    }

#endif
#ifdef  __OMXRGB_FILE_IN_SAVE__
    if(g_rgbtoSave != NULL)
    {
        fclose(g_rgbtoSave);
    }
#endif
    DEBUG_PRINT("Hisilicon venc Omx Component exit!\n");
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
OMX_ERRORTYPE component_init(OMX_HANDLETYPE phandle,
                             OMX_STRING comp_name)
{

    OMX_ERRORTYPE          error     = OMX_ErrorNone;
    OMX_COMPONENTTYPE*     pcomp     = NULL;
    OMX_COMPONENT_PRIVATE* pcom_priv = NULL;

    int fds[2];
    OMX_S32 result = -1;

    OMX_CHECK_ARG_RETURN(phandle == NULL);

    if (strncmp(comp_name, OMX_VENC_COMPONENT_NAME, sizeof(OMX_VENC_COMPONENT_NAME)) != 0)
    {
        DEBUG_PRINT_ERROR("compname:  %s not match \n", comp_name);
        return OMX_ErrorBadParameter;
    }

    pcomp     = (OMX_COMPONENTTYPE*)phandle;
    pcom_priv = (OMX_COMPONENT_PRIVATE*)malloc(sizeof(OMX_COMPONENT_PRIVATE));

    if (!pcom_priv)
    {
        DEBUG_PRINT_ERROR("component_init(): malloc failed\n");
        return OMX_ErrorInsufficientResources;
    }

    memset(pcom_priv, 0 , sizeof(OMX_COMPONENT_PRIVATE));

    pcom_priv->m_flags					= 0;
    pcom_priv->event_thread_exit		= OMX_FALSE;
    //pcom_priv->msg_thread_exit			= OMX_FALSE;
    pcom_priv->m_pre_timestamp			= 0;                             //PTS

    pcom_priv->m_encoder_fmt			= OMX_VIDEO_CodingUnused;


    strncpy((char*)pcom_priv->m_comp_name, (char*)comp_name, OMX_MAX_STRINGNAME_SIZE - 1); //add by liminqi

    init_event_queue(&pcom_priv->m_cmd_q);
    init_event_queue(&pcom_priv->m_ftb_q);
    init_event_queue(&pcom_priv->m_etb_q);

    if (pthread_mutex_init(&pcom_priv->m_lock, NULL))
    {
        error = OMX_ErrorInsufficientResources;
        goto error_exit;
    }
    sem_init(&pcom_priv->m_cmd_lock,  0, 0);
    sem_init(&pcom_priv->m_async_sem, 0, 0);
#ifdef ENABLE_BUFFER_LOCK
    sem_init(&pcom_priv->m_buf_lock, 0, 100);
#endif
    /* create event theard */
    result = pthread_create(&pcom_priv->event_thread_id,
                            0, event_thread, pcom_priv);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("ERROR: failed to create event thread\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit0;
    }

    result = pipe(fds);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("pipe creation failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit1;
    }
    pcom_priv->m_pipe_in	= fds[0];
    pcom_priv->m_pipe_out	= fds[1];


    /******* set the default port attr *******************/
    result = ports_init(pcom_priv);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("pipe creation failed\n");
        error = OMX_ErrorInsufficientResources;
        goto error_exit2;
    }

    result = venc_init_drv_context(&pcom_priv->drv_ctx);
    if (result < 0)
    {
        DEBUG_PRINT_ERROR("drv ctx init failed\n");
        error = OMX_ErrorUndefined;
        goto error_exit4;
    }

    venc_get_default_attr(&pcom_priv->drv_ctx);

    /* init component callbacks */
    CONFIG_VERSION_SIZE(pcomp , OMX_COMPONENTTYPE);

    pcomp->SetCallbacks				= set_callbacks;
    pcomp->GetComponentVersion		= get_component_version;
    pcomp->SendCommand				= send_command;
    pcomp->GetParameter				= get_parameter;
    pcomp->SetParameter				= set_parameter;
    pcomp->GetState					= get_state;
    pcomp->AllocateBuffer			= allocate_buffer;
    pcomp->FreeBuffer				= free_buffer;
    pcomp->EmptyThisBuffer			= empty_this_buffer;
    pcomp->FillThisBuffer           = fill_this_buffer;
    pcomp->ComponentDeInit			= component_deinit;

    pcomp->GetConfig				= get_config;
    pcomp->SetConfig				= set_config;
    pcomp->ComponentTunnelRequest	= component_tunnel_request;         //not support yet
    pcomp->UseBuffer				= use_buffer;                       //not support yet
    pcomp->GetExtensionIndex		= get_extension_index;              //not support yet
#ifdef KHRONOS_1_1
    pcomp->ComponentRoleEnum		= component_role_enum;
#endif

    pcomp->pComponentPrivate        = pcom_priv;
    pcom_priv->m_pcomp		        = pcomp;

    pcom_priv->m_state		        = OMX_StateLoaded;

    sem_post(&pcom_priv->m_async_sem);

#ifdef __OMXVENC_FILE_OUT_SAVE__
    g_pFiletoSave = fopen("/mnt/sdcard/omx_venc_out.h264", "wb+");
    if (g_pFiletoSave == NULL)
    {
        DEBUG_PRINT_ERROR("FiletoSave: /mnt/sdcard/omx_venc_out.h264 open error!!");
    }
    else
    {
        DEBUG_PRINT("FiletoSave: /mnt/sdcard/omx_venc_out.h264 open OK!!");
    }
#endif

#ifdef __OMXVENC_FILE_IN_SAVE__
    g_pFiletoEnc  = fopen("/mnt/omx_venc_in.yuv", "wb+");
    if (g_pFiletoEnc == NULL)
    {
        DEBUG_PRINT_ERROR("FiletoEnc: /mnt/omx_venc_in.yuv open error!!");
    }
    else
    {
        DEBUG_PRINT("FiletoEnc: /mnt/omx_venc_in.yuv open OK!!");
    }

    g_pU = malloc(1280 * 720 / 4 * sizeof(HI_U8));
    g_pV = malloc(1280 * 720 / 4 * sizeof(HI_U8));

    g_u32CountNum = 0;
    g_u32CountNum_etb = 0;
#endif
#ifdef __OMXRGB_FILE_IN_SAVE__
    g_rgbtoSave = fopen("/data/omx_venc_rgb_in.rgb", "wb+");
#endif
    DEBUG_PRINT("Hisilicon Video Encoder Omx Component Verision 1.0");


    return OMX_ErrorNone;

error_exit4:
    ports_deinit(pcom_priv);
error_exit2:
    close(pcom_priv->m_pipe_in);
    close(pcom_priv->m_pipe_out);
error_exit1:
    sem_post(&pcom_priv->m_async_sem);
    pcom_priv->event_thread_exit		= OMX_TRUE;
    pthread_join(pcom_priv->event_thread_id, NULL);
error_exit0:
    pthread_mutex_destroy(&pcom_priv->m_lock);
#ifdef ENABLE_BUFFER_LOCK
    sem_destroy(&pcom_priv->m_buf_lock);
#endif
    sem_destroy(&pcom_priv->m_cmd_lock);
    sem_destroy(&pcom_priv->m_async_sem);
error_exit:
    free(pcom_priv);
    pcom_priv = NULL;
    pcomp->pComponentPrivate = NULL;

    return error;
}
