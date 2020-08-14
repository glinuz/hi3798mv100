/*
    An Open max test application ....
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>

#include "OMX_Core.h"
#include "OMX_Component.h"
#include "OMX_IVCommon.h"
#include "OMX_VideoExt.h"
#include "OMX_IndexExt.h"
#include "OMX_RoleNames.h"
#include "sample_queue.h"
#include "sample_tidx.h"

#include "sample_omxvdec.h"

#ifdef HI_OMX_TEE_SUPPORT
#include "tee_client_api.h"
#include "mpi_mmz.h"
#endif

/**********************************************************
//#define SupportNative         //是否支持native buffer测试
//#define EnableTest            //是否支持异常退出测试
//#define EnableSetColor        //是否支持输出格式设置
//#define CalcFrameRate         //是否统计帧率
**********************************************************/

OmxTestInfo_S OmxTestInfo[MAX_INST_NUM];

/************************************************************************/
#define  SAMPLETRACE()        printf("fun: %s, line: %d\n", __func__, __LINE__)
#define  DEBUG_PRINT(format...)  //printf
#define  DEBUG_PRINT_ERROR    printf
#define  DEBUG_PRINT_ALWS     printf

/************************************************************************/

#define FAILED(result) (result != OMX_ErrorNone)
#define SUCCEEDED(result) (result == OMX_ErrorNone)

#define MAX_WIDTH        (4096)
#define MAX_HEIGHT        (2304)
#define DEFAULT_WIDTH    (1920)
#define DEFAULT_HEIGHT    (1088)

#define ALIGN_SIZE        (4096)

#define EnableAndroidNativeBuffers         "OMX.google.android.index.enableAndroidNativeBuffers"
#define GetAndroidNativeBufferUsage        "OMX.google.android.index.getAndroidNativeBufferUsage"
#define UseAndroidNativeBuffer2            "OMX.google.android.index.useAndroidNativeBuffer2"

#define MAX_COMP_NUM                       (30)//(30)  // total component num
#define MAX_COMP_NAME_LEN                  (50)  // max len of one component name

/************************************************************************/

#define CONFIG_VERSION_SIZE(param) \
    param.nVersion.nVersion = OMX_VERSION;\
    param.nSize = sizeof(param);

#define SWAPBYTES(ptrA, ptrB) { char t = *ptrA; *ptrA = *ptrB; *ptrB = t;}

#ifdef CalcFrameRate
static bool g_PrintAlready = false;
#endif

static int g_TestInstNum = 0;

OMX_ERRORTYPE error;

#ifdef HI_OMX_TEE_SUPPORT
#define TEEC_CMD_COM_CA2TA_DATA  18

static TEEC_Context  g_TeeContext;
static TEEC_Session g_teeSession;

#endif
/* CodecType Relative */
static char *tComponentName[MAX_COMP_NUM][2] = {

    /*video decoder components */
    {"OMX.hisi.video.decoder.avc",    OMX_ROLE_VIDEO_DECODER_AVC},
    {"OMX.hisi.video.decoder.vc1",    OMX_ROLE_VIDEO_DECODER_VC1},
    {"OMX.hisi.video.decoder.mpeg1",  OMX_ROLE_VIDEO_DECODER_MPEG1},
    {"OMX.hisi.video.decoder.mpeg2",  OMX_ROLE_VIDEO_DECODER_MPEG2},
    {"OMX.hisi.video.decoder.mpeg4",  OMX_ROLE_VIDEO_DECODER_MPEG4},
    {"OMX.hisi.video.decoder.h263",   OMX_ROLE_VIDEO_DECODER_H263},
    {"OMX.hisi.video.decoder.divx3",  OMX_ROLE_VIDEO_DECODER_DIVX3},
    {"OMX.hisi.video.decoder.vp8",    OMX_ROLE_VIDEO_DECODER_VP8},
    {"OMX.hisi.video.decoder.vp9",    OMX_ROLE_VIDEO_DECODER_VP9},

    {"OMX.hisi.video.decoder.vp6",    OMX_ROLE_VIDEO_DECODER_VP6},
    {"OMX.hisi.video.decoder.wmv",    OMX_ROLE_VIDEO_DECODER_WMV},
    {"OMX.hisi.video.decoder.avs",    OMX_ROLE_VIDEO_DECODER_AVS},
    {"OMX.hisi.video.decoder.sorenson",  OMX_ROLE_VIDEO_DECODER_SORENSON},
    {"OMX.hisi.video.decoder.hevc",   OMX_ROLE_VIDEO_DECODER_HEVC},
    {"OMX.hisi.video.decoder.mvc",    OMX_ROLE_VIDEO_DECODER_MVC},

  #if (defined(REAL8_SUPPORT) || defined(REAL9_SUPPORT))
    /* For copyright, vfmw didn't support std real. */
    {"OMX.hisi.video.decoder.real",  OMX_ROLE_VIDEO_DECODER_RV},
    {"OMX.hisi.video.decoder.real.secure",  OMX_ROLE_VIDEO_DECODER_RV},
  #endif

    /*secure video decoder components */
    {"OMX.hisi.video.decoder.avc.secure",    OMX_ROLE_VIDEO_DECODER_AVC},
    {"OMX.hisi.video.decoder.vc1.secure",    OMX_ROLE_VIDEO_DECODER_VC1},
    {"OMX.hisi.video.decoder.mpeg1.secure",  OMX_ROLE_VIDEO_DECODER_MPEG1},
    {"OMX.hisi.video.decoder.mpeg2.secure",  OMX_ROLE_VIDEO_DECODER_MPEG2},
    {"OMX.hisi.video.decoder.mpeg4.secure",  OMX_ROLE_VIDEO_DECODER_MPEG4},
    {"OMX.hisi.video.decoder.divx3.secure",  OMX_ROLE_VIDEO_DECODER_DIVX3},
    {"OMX.hisi.video.decoder.vp8.secure",    OMX_ROLE_VIDEO_DECODER_VP8},
    {"OMX.hisi.video.decoder.vp9.secure",    OMX_ROLE_VIDEO_DECODER_VP9},
    {"OMX.hisi.video.decoder.vp6.secure",    OMX_ROLE_VIDEO_DECODER_VP6},
    {"OMX.hisi.video.decoder.wmv.secure",    OMX_ROLE_VIDEO_DECODER_WMV},
    {"OMX.hisi.video.decoder.avs.secure",    OMX_ROLE_VIDEO_DECODER_AVS},
    {"OMX.hisi.video.decoder.hevc.secure",   OMX_ROLE_VIDEO_DECODER_HEVC},

    /* terminate the table */
    {NULL, NULL},

};


static int get_componet_name_by_role(char *vdecCompNames, const char *compRole)
{
    int i, ret = 0;

    int count = sizeof(tComponentName)/(sizeof(tComponentName[0]));

    for (i = 0; i< count; i++)
    {
        if (!strcmp(compRole, tComponentName[i][1]))
        {
            strcpy(vdecCompNames, tComponentName[i][0]);

            break;
        }
    }

    if (i == count)
    {
        DEBUG_PRINT(" %s can NOT find component name by role:%s\n", __FUNCTION__, compRole);

        vdecCompNames = NULL;
        ret = -1;
    }

    return ret;
}



/****************************************************************************/
/* 获取系统时间  单位：毫秒                                                 */
/****************************************************************************/
#ifdef CalcFrameRate
static int GetTimeInMs(void)
{
    int CurrMs = 0;
    struct timeval CurrentTime;

    gettimeofday(&CurrentTime, 0);
    CurrMs = (int)(CurrentTime.tv_sec*1000 + CurrentTime.tv_usec/1000);

    return CurrMs;
}
#endif

static void wait_for_event(OmxTestInfo_S * pOmxTestInfo, int cmd)
{
    pthread_mutex_lock(&pOmxTestInfo->event_lock);
    while(1)
    {
        if((pOmxTestInfo->event_is_done == 1) && (pOmxTestInfo->last_cmd == cmd || pOmxTestInfo->last_cmd == -1))
        {
            pOmxTestInfo->event_is_done = 0;
            break;
        }
        pthread_cond_wait(&pOmxTestInfo->event_cond, &pOmxTestInfo->event_lock);

    }
    pthread_mutex_unlock(&pOmxTestInfo->event_lock);
}

static void event_complete(OmxTestInfo_S *pOmxTestInfo, int cmd)
{
    pthread_mutex_lock(&pOmxTestInfo->event_lock);
    if(pOmxTestInfo->event_is_done == 0)
        pOmxTestInfo->event_is_done = 1;

    pthread_cond_broadcast(&pOmxTestInfo->event_cond);
    pOmxTestInfo->last_cmd = cmd;
    pthread_mutex_unlock(&pOmxTestInfo->event_lock);
}


/************************************************************************
                file operation functions
************************************************************************/

static int open_video_file (OmxTestInfo_S * pOmxTestInfo)
{
    int error_code = 0;
    char tidxfile[512] = {0};

    DEBUG_PRINT("Inside %s filename=%s\n", __FUNCTION__, pOmxTestInfo->in_filename);

    pOmxTestInfo->inputBufferFileFd = fopen(pOmxTestInfo->in_filename, "rb");
    if (pOmxTestInfo->inputBufferFileFd == NULL)
    {
        DEBUG_PRINT_ERROR("Error: i/p file %s open failed, errno = %d\n", pOmxTestInfo->in_filename, errno);
        error_code = -1;
    }

    if (true == pOmxTestInfo->frame_in_packet)
    {
        if (false == pOmxTestInfo->readsize_add_in_stream) // FIX
        {
            memset(&pOmxTestInfo->stContext, 0, sizeof(STR_CONTEXT_S));
            sprintf(tidxfile, "%s.tidx", pOmxTestInfo->in_filename);
            pOmxTestInfo->stContext.fptidxFile = fopen(tidxfile, "r");
            if (NULL == pOmxTestInfo->stContext.fptidxFile)
            {
                DEBUG_PRINT_ERROR("Error: i/p tidx file %s open failed, errno = %d\n",tidxfile, errno);
                error_code = -1;
            }
            ReadFrameInfoFromtidx(&pOmxTestInfo->stContext);
        }
    }

    return error_code;
}


unsigned long char_to_long(char *buf, int len)
{
    int i;
    long frame_len = 0;

    for (i=0; i<len; i++)
    {
        frame_len += (buf[i] << 8*i);
    }

    return frame_len;
}

int VP9_Read_EsRawStream(OmxTestInfo_S *pOmxTestInfo, OMX_BUFFERHEADERTYPE *pBufHdr)
{
    int bytes_read = 0;
    char tmp_buf[64];
    OMX_U8* pBuffer = NULL;
    unsigned long read_len = 0;

    pBuffer = pBufHdr->pBuffer;

    //第一次读文件，需要先读取文件头的32bit
    if (0 == pOmxTestInfo->send_cnt)
    {
        bytes_read = fread(tmp_buf, 1, 32, pOmxTestInfo->inputBufferFileFd);

        if (bytes_read < 32)
        {
            DEBUG_PRINT_ALWS("Less than 32 byte(%d), stream send over!\n", bytes_read);
            pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
        }

        if (memcmp("DKIF", tmp_buf, 4) != 0)
        {
            DEBUG_PRINT_ERROR("VP9 file is not IVF file ERROR.\n");

            return 0;
        }

        pBufHdr->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
    }

    //VP9 每一帧的前12bit ， 4bit表示该帧的长度
    if (0 == pOmxTestInfo->frame_flag)
    {
        bytes_read = fread(tmp_buf, 1, 12, pOmxTestInfo->inputBufferFileFd);
        if (bytes_read < 12)
        {
            DEBUG_PRINT_ALWS("Less than 12 byte(%d), stream send over!\n", bytes_read);
            pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
        }
        else
        {
            pOmxTestInfo->frame_len  = tmp_buf[3] << 24;
            pOmxTestInfo->frame_len |= tmp_buf[2] << 16;
            pOmxTestInfo->frame_len |= tmp_buf[1] << 8;
            pOmxTestInfo->frame_len |= tmp_buf[0];

            DEBUG_PRINT_ALWS("frame_len %lu\n", pOmxTestInfo->frame_len);
            if (pOmxTestInfo->frame_len > 0)
            {
                pOmxTestInfo->frame_flag = 1;
            }
            else
            {
                DEBUG_PRINT_ALWS("Frame Len(%lu) invalid, stream send over!\n", pOmxTestInfo->frame_len);
                pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
            }
        }
    }

       if (1 == pOmxTestInfo->frame_flag)
    {
        if (pOmxTestInfo->frame_len > pBufHdr->nAllocLen)
        {
            DEBUG_PRINT_ALWS("WARNNING: frame_len(%lu) > pBufHdr->nAllocLen(%lu)\n", pOmxTestInfo->frame_len, pBufHdr->nAllocLen);
            read_len =  pBufHdr->nAllocLen;
            pOmxTestInfo->frame_len = pOmxTestInfo->frame_len - read_len;
        }
        else
        {
            read_len = pOmxTestInfo->frame_len;
            pOmxTestInfo->frame_len = 0;
            pOmxTestInfo->frame_flag = 0;
        }

        bytes_read = fread(pBuffer, 1, read_len, pOmxTestInfo->inputBufferFileFd);
        if (bytes_read < read_len)
        {
            DEBUG_PRINT_ALWS("ReadByte(%d) < NeedByte(%lu), stream send over!\n", bytes_read, read_len);
            pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
        }

        if (0 == pOmxTestInfo->frame_flag)
        {
            //DEBUG_PRINT_ALWS("Packet Stream send: %d\n", pOmxTestInfo->send_cnt);
            pBufHdr->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
        }
    }

    pOmxTestInfo->send_cnt++;

    return bytes_read;
}

static int Read_Buffer_from_EsRawStream(OmxTestInfo_S * pOmxTestInfo, OMX_BUFFERHEADERTYPE  *pBufHdr)
{
    int bytes_read = 0;
    char tmp_buf[4];
    OMX_U8* pBuffer = NULL;

    unsigned long read_len = 0;
    //unsigned long frame_offset = 0;

    DEBUG_PRINT("Inside Read_Buffer_from_EsRawStream\n");

#ifdef HI_OMX_TEE_SUPPORT

    if (1 == pOmxTestInfo->tvp_option)
    {
        pBuffer = (OMX_U8*)pOmxTestInfo->pCAStreamBuf.user_viraddr;
    }
    else
#endif
    {
        pBuffer = pBufHdr->pBuffer;
    }

    /* VP9读码流比较特殊，单独走一个分支 */
    if (CODEC_FORMAT_VP9 == pOmxTestInfo->codec_format_option)
    {
        bytes_read = VP9_Read_EsRawStream(pOmxTestInfo, pBufHdr);

        goto EXIT;
    }

     /* 读取码流的长度，有些码流的长度在前四个字节 */
    if (true == pOmxTestInfo->readsize_add_in_stream)
    {
       if (0 == pOmxTestInfo->frame_flag)
       {
           bytes_read = fread(tmp_buf, 1, 4, pOmxTestInfo->inputBufferFileFd);
           if (bytes_read < 4)
           {
               DEBUG_PRINT_ALWS("Less than 4 byte(%d), stream send over!\n", bytes_read);
               pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
           }
           else
           {
               pOmxTestInfo->frame_len = char_to_long(tmp_buf, 4);
               //DEBUG_PRINT_ALWS("frame_len %d\n", pOmxTestInfo->frame_len);
               if (pOmxTestInfo->frame_len > 0)
               {
                   pOmxTestInfo->frame_flag = 1;
               }
               else
               {
                   DEBUG_PRINT_ALWS("Frame Len(%ld) invalid, stream send over!\n", pOmxTestInfo->frame_len);
                   pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
               }
           }
       }

       if (1 == pOmxTestInfo->frame_flag)
       {
           if (pOmxTestInfo->frame_len > pBufHdr->nAllocLen)
           {
               DEBUG_PRINT_ALWS("WARNNING: frame_len(%lu) > pBufHdr->nAllocLen(%lu)\n", pOmxTestInfo->frame_len, pBufHdr->nAllocLen);
               read_len =  pBufHdr->nAllocLen;
               pOmxTestInfo->frame_len = pOmxTestInfo->frame_len - read_len;
           }
           else
           {
               read_len = pOmxTestInfo->frame_len;
               pOmxTestInfo->frame_len = 0;
               pOmxTestInfo->frame_flag = 0;
           }

           bytes_read = fread(pBuffer, 1, read_len, pOmxTestInfo->inputBufferFileFd);
           if (bytes_read < (int)read_len)
           {
               DEBUG_PRINT_ALWS("ReadByte(%d) < NeedByte(%ld), stream send over!\n", bytes_read, read_len);
               pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
           }

           if (0 == pOmxTestInfo->frame_flag)
           {
               pOmxTestInfo->send_cnt++;
               //DEBUG_PRINT_ALWS("Packet Stream send: %d\n", pOmxTestInfo->send_cnt);
               pBufHdr->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
           }
       }
    }
    else
    {    /* 当按帧大小送流时，从tidx文件读取每次送流的大小 */
        if (true == pOmxTestInfo->frame_in_packet)
        {
           if  (0 != pOmxTestInfo->stContext.stFrameInfotidx[pOmxTestInfo->send_cnt].s32FrameSize)
           {
               read_len = pOmxTestInfo->stContext.stFrameInfotidx[pOmxTestInfo->send_cnt].s32FrameSize;
               //DEBUG_PRINT_ALWS("L%d: send_cnt = %d, read_len = %d\n", __LINE__,pOmxTestInfo->send_cnt, read_len);
           }

         #if 0  // 在带offset的tidx文件中才打开
           frame_offset = pOmxTestInfo->stContext.stFrameInfotidx[pOmxTestInfo->send_cnt].s32Frameoffset;
           if (0 != frame_offset)
           {
               lseek(pOmxTestInfo->inputBufferFileFd, frame_offset, 0);
           }
         #endif

           bytes_read = fread(pBuffer, 1, read_len, pOmxTestInfo->inputBufferFileFd);
           //DEBUG_PRINT_ALWS("Frame Stream bytes_read: %d\n", bytes_read);
           if (bytes_read < (int)read_len)
           {
               DEBUG_PRINT_ALWS("ReadByte(%d) < FrameSize(%ld), stream send over!\n", bytes_read, read_len);
               pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
           }

           if (0 == pOmxTestInfo->send_cnt)
           {
               if (CODEC_FORMAT_VC1SMP == pOmxTestInfo->codec_format_option
                || CODEC_FORMAT_REAL8  == pOmxTestInfo->codec_format_option)
               {
                   pBufHdr->nFlags |= OMX_BUFFERFLAG_CODECCONFIG;
               }
           }

           //DEBUG_PRINT_ALWS("Frame Stream send: %d\n", pOmxTestInfo->send_cnt);
           pBufHdr->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
           pOmxTestInfo->send_cnt++;

           if (pOmxTestInfo->stContext.stFrameInfotidx[pOmxTestInfo->send_cnt].s32FrameNum <= 0)
           {
               DEBUG_PRINT_ALWS("FrameNum(%d) in FrameInfo invalid, stream send over!\n",
                                    pOmxTestInfo->stContext.stFrameInfotidx[pOmxTestInfo->send_cnt].s32FrameNum);
               pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
           }
        }
        else
        {
            bytes_read = fread(pBuffer, 1, pBufHdr->nAllocLen, pOmxTestInfo->inputBufferFileFd);
            if (bytes_read < (int)pBufHdr->nAllocLen)
            {
                DEBUG_PRINT_ALWS("ReadByte(%d) < AllocLen(%lu), stream send over!\n", bytes_read, pBufHdr->nAllocLen);
                pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
            }
            //DEBUG_PRINT_ALWS("Random Stream send: %d\n", send_cnt);
            pOmxTestInfo->send_cnt++;
        }
    }

#ifdef HI_OMX_TEE_SUPPORT
    if (1 == pOmxTestInfo->tvp_option && bytes_read > 0)
    {
        TEEC_Result result;
        TEEC_Operation operation;

        operation.started = 1;
        operation.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_VALUE_INPUT, TEEC_NONE, TEEC_NONE);
        operation.params[0].value.a = (HI_U32)(long)(pBufHdr->pBuffer);
        operation.params[0].value.b = (HI_U32)(pOmxTestInfo->pCAStreamBuf.phyaddr);
        operation.params[1].value.a = bytes_read;
        operation.params[1].value.b = TEEC_VALUE_UNDEF;

        DEBUG_PRINT_ERROR("\n\n\n------ca phy:0x%x  sec vir:0x%p len:%d -----\n", (pOmxTestInfo->pCAStreamBuf.phyaddr), pBufHdr->pBuffer, bytes_read);

        result = TEEC_InvokeCommand(&g_teeSession, TEEC_CMD_COM_CA2TA_DATA, &operation, HI_NULL);
        if (TEEC_SUCCESS != result)
        {
            DEBUG_PRINT_ERROR("sec memcpy Failed!\n");
            exit(0);
        }

        DEBUG_PRINT_ERROR("RawPlay sec memcpy 2\n");
    }

#endif

EXIT:
    return bytes_read;
}


/*************************************************************************
                omx interface functions
************************************************************************/

OMX_ERRORTYPE EventHandler(OMX_IN OMX_HANDLETYPE hComponent,
                           OMX_IN OMX_PTR pAppData,
                           OMX_IN OMX_EVENTTYPE eEvent,
                           OMX_IN OMX_U32 nData1, OMX_IN OMX_U32 nData2,
                           OMX_IN OMX_PTR pEventData)
{
    DEBUG_PRINT("Function %s, eEvent = %d, nData1 = %d\n", __FUNCTION__, eEvent, nData1);

    OmxTestInfo_S *pOmxTestInfo = NULL;

    int index = 0;

    for (index = 0; index < g_TestInstNum; index++)
    {
        if (OmxTestInfo[index].dec_handle == (OMX_COMPONENTTYPE *)hComponent)
        {
            pOmxTestInfo = &OmxTestInfo[index];
            break;
        }
    }

    if (index == g_TestInstNum)
    {
        DEBUG_PRINT_ERROR("invalid parameter!\n");
        return OMX_ErrorBadParameter;
    }

    switch(eEvent)
    {
    case OMX_EventCmdComplete:

        DEBUG_PRINT("OMX_EventCmdComplete\n");

        if(OMX_CommandPortDisable == (OMX_COMMANDTYPE)nData1)
        {
            DEBUG_PRINT("*********************************************\n");
            DEBUG_PRINT("Recieved DISABLE Event Command Complete[%d]\n",nData2);
            DEBUG_PRINT("*********************************************\n");
            event_complete(pOmxTestInfo, nData1);
        }
        else if(OMX_CommandPortEnable == (OMX_COMMANDTYPE)nData1)
        {
            DEBUG_PRINT("*********************************************\n");
            DEBUG_PRINT("Recieved ENABLE Event Command Complete[%d]\n",nData2);
            DEBUG_PRINT("*********************************************\n");

        //pthread_mutex_lock(&enable_lock);
            pOmxTestInfo->sent_disabled = 0;
        //pthread_mutex_unlock(&enable_lock);
            event_complete(pOmxTestInfo, nData1);
        }
        else if(OMX_CommandFlush == (OMX_COMMANDTYPE)nData1)
        {
            DEBUG_PRINT("*********************************************\n");
            DEBUG_PRINT("Received FLUSH Event Command Complete[%d]\n",nData2);
            DEBUG_PRINT("*********************************************\n");

            if (nData2 == 0) {
                DEBUG_PRINT("**** Flush In Complete ****\n");
                pOmxTestInfo->flush_input_progress = 0;
                sem_post(&pOmxTestInfo->in_flush_sem);
            }
            else if (nData2 == 1) {
                DEBUG_PRINT("**** Flush Out Complete ****\n");
                pOmxTestInfo->flush_output_progress = 0;
                sem_post(&pOmxTestInfo->out_flush_sem);
            }

            if (!pOmxTestInfo->flush_input_progress && !pOmxTestInfo->flush_output_progress)
            {
                event_complete(pOmxTestInfo, nData1);
            }
        }
        else
        {
            DEBUG_PRINT("*********************************************\n");
            DEBUG_PRINT("Received Event Command %d Complete[%d]\n",nData1, nData2);
            DEBUG_PRINT("*********************************************\n");
            event_complete(pOmxTestInfo, nData1);
        }

        break;

    case OMX_EventError:

        DEBUG_PRINT("*********************************************\n");
        DEBUG_PRINT("Received OMX_EventError Event Command ! Error = 0x%x\n", nData1);
        DEBUG_PRINT("*********************************************\n");

        pOmxTestInfo->currentStatus = ERROR_STATE;

        if (OMX_ErrorIncorrectStateOperation == (OMX_ERRORTYPE)nData1 ||
        OMX_ErrorHardware == (OMX_ERRORTYPE)nData1)
        {
            DEBUG_PRINT_ERROR("Invalid State or hardware error\n");

            if(pOmxTestInfo->event_is_done == 0)
            {
                DEBUG_PRINT("Event error in the middle of Decode\n");

                pOmxTestInfo->bOutputEosReached = true;
            }
        }

        event_complete(pOmxTestInfo, -1);

        break;

    case OMX_EventPortSettingsChanged:

        DEBUG_PRINT("OMX_EventPortSettingsChanged\n");

        pOmxTestInfo->preStatus = pOmxTestInfo->currentStatus;
        pOmxTestInfo->currentStatus = PORT_SETTING_CHANGE_STATE;

        sem_post(&pOmxTestInfo->ftb_sem);
        break;

    case OMX_EventBufferFlag:

        DEBUG_PRINT("OMX_EventBufferFlag %d %d\n", nData1, nData2);

        if (nData1 == 1 && (nData2 & OMX_BUFFERFLAG_EOS))
        {
            pOmxTestInfo->bOutputEosReached = true;

        #ifdef CalcFrameRate
            if (!g_PrintAlready)
            {
                int spend_time_ms = 0;

                for (index = 0; index < g_TestInstNum; index++)
                {
                    OmxTestInfo[index].stop_time = GetTimeInMs();

                    if (OmxTestInfo[index].stop_time > OmxTestInfo[index].start_time)
                    {
                        spend_time_ms = OmxTestInfo[index].stop_time - OmxTestInfo[index].start_time;

                        DEBUG_PRINT_ALWS("INSTANCE NO:%d fram rate:%f\n", OmxTestInfo[index].inst_no,\
                                            (float)(OmxTestInfo[index].receive_frame_cnt*1000)/spend_time_ms);
                    }
                }
                g_PrintAlready = true;
            }
        #endif
            DEBUG_PRINT_ALWS("Inst %d receive last frame\n", pOmxTestInfo->inst_no);
        }
        else
        {
            DEBUG_PRINT("OMX_EventBufferFlag Event not handled\n");
        }
        break;

    default:
        DEBUG_PRINT_ERROR("ERROR - Unknown Event\n");
        break;
    }

    return OMX_ErrorNone;
}


OMX_ERRORTYPE EmptyBufferDone(OMX_IN OMX_HANDLETYPE hComponent,
                              OMX_IN OMX_PTR pAppData,
                              OMX_IN OMX_BUFFERHEADERTYPE* pBuffer)
{
    int index     = 0;
    OmxTestInfo_S *pOmxTestInfo = NULL;

    DEBUG_PRINT("Enter %s\n", __FUNCTION__);

    for (index = 0; index < g_TestInstNum; index++)
    {
        if (OmxTestInfo[index].dec_handle == (OMX_COMPONENTTYPE *)hComponent)
        {
            pOmxTestInfo = &OmxTestInfo[index];
            break;
        }
    }

    if (index == g_TestInstNum)
    {
        DEBUG_PRINT_ERROR("invalid parameter!\n");
        return OMX_ErrorBadParameter;
    }

    pOmxTestInfo->free_ip_buf_cnt++;

    if(pOmxTestInfo->bInputEosReached)
    {
        DEBUG_PRINT("EBD: Input EoS Reached.\n");
        return OMX_ErrorNone;
    }

    if(pOmxTestInfo->seeking_progress)
    {
        DEBUG_PRINT("EBD: Seeking Pending.\n");
        return OMX_ErrorNone;
    }

    if(pOmxTestInfo->flush_input_progress)
    {
        DEBUG_PRINT("EBD: Input Flushing.\n");
        return OMX_ErrorNone;
    }

    pthread_mutex_lock(&pOmxTestInfo->etb_lock);
    if(push(pOmxTestInfo->etb_queue, (void *) pBuffer) < 0)
    {
        DEBUG_PRINT_ERROR("Error in enqueue  ebd data\n");
        return OMX_ErrorUndefined;
    }

    pthread_mutex_unlock(&pOmxTestInfo->etb_lock);
    sem_post(&pOmxTestInfo->etb_sem);

    return OMX_ErrorNone;
}


OMX_ERRORTYPE FillBufferDone(OMX_OUT OMX_HANDLETYPE hComponent,
                             OMX_OUT OMX_PTR pAppData,
                             OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
    /* Test app will assume there is a dynamic port setting
    * In case that there is no dynamic port setting, OMX will not call event cb,
    * instead OMX will send empty this buffer directly and we need to clear an event here
    */
    //int ms;

    OmxTestInfo_S *pOmxTestInfo = NULL;

    int index = 0;

    DEBUG_PRINT("Enter %s\n", __FUNCTION__);

    for (index = 0; index < g_TestInstNum; index++)
    {
        if (OmxTestInfo[index].dec_handle == (OMX_COMPONENTTYPE *)hComponent)
        {
            pOmxTestInfo = &OmxTestInfo[index];
            break;
        }
    }

    if (index == g_TestInstNum)
    {
        DEBUG_PRINT_ERROR("invalid parameter!\n");
        return OMX_ErrorBadParameter;
    }

    DEBUG_PRINT("Fill buffer done\n");

    pOmxTestInfo->free_op_buf_cnt++;

    if (pBuffer->nFilledLen != 0)
    {
        gettimeofday(&pOmxTestInfo->t_cur_get, NULL);
        /*
        if (pOmxTestInfo->receive_frame_cnt == 0 )
        {
            ms = (pOmxTestInfo->t_cur_get.tv_sec - pOmxTestInfo->t_first_send.tv_sec)*1000 +
                (pOmxTestInfo->t_cur_get.tv_usec - pOmxTestInfo->t_first_send.tv_usec)/1000;
            //DEBUG_PRINT_ALWS("\nREPORT: first frame delay time:%dms\n\n", ms);
        }
        else
        {
            ms = (pOmxTestInfo->t_cur_get.tv_sec - pOmxTestInfo->t_last_get.tv_sec)*1000 +
                (pOmxTestInfo->t_cur_get.tv_usec - pOmxTestInfo->t_last_get.tv_usec)/1000;
            //DEBUG_PRINT_ALWS("REPORT: this frame use time:%dms\n", ms);
        }
        */

    #ifdef CalcFrameRate
        if (pOmxTestInfo->receive_frame_cnt == 0)
        {
            pOmxTestInfo->start_time = GetTimeInMs();
        }
    #endif

        DEBUG_PRINT_ALWS("Receive Inst %d: Frame %d\n", pOmxTestInfo->inst_no, pOmxTestInfo->receive_frame_cnt);

        pOmxTestInfo->receive_frame_cnt++;
    }

    if (pOmxTestInfo->flush_output_progress)
    {
        DEBUG_PRINT("FBD: Output Flushing.\n");
        return OMX_ErrorNone;
    }

    pthread_mutex_lock(&pOmxTestInfo->ftb_lock);

    if (!pOmxTestInfo->sent_disabled)
    {
        if(push(pOmxTestInfo->ftb_queue, (void *)pBuffer) < 0)
        {
            pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);
            DEBUG_PRINT_ERROR("Error in enqueueing fbd_data\n");
            return OMX_ErrorUndefined;
        }
        sem_post(&pOmxTestInfo->ftb_sem);
    }
    else
    {
       if (0 == pOmxTestInfo->alloc_use_option)
       {
           OMX_FreeBuffer(pOmxTestInfo->dec_handle, 1, pBuffer);
       }
       else
       {
           OMX_FreeBuffer(pOmxTestInfo->dec_handle, 1, pBuffer);
           HI_MMZ_Free(&pOmxTestInfo->buffer[pBuffer->nTickCount]);
       }
    }

    pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);

    if (pBuffer->nFilledLen != 0)
    {
        gettimeofday(&pOmxTestInfo->t_cur_get, NULL);
        memcpy(&pOmxTestInfo->t_last_get, &pOmxTestInfo->t_cur_get, sizeof(struct timeval));
    }

    return OMX_ErrorNone;
}


static int Init_Decoder(OmxTestInfo_S * pOmxTestInfo)
{

    OMX_ERRORTYPE omxresult;
    OMX_U32 total = 0;
    OMX_U32 i = 0, is_found = 0;

    char vdecCompNames[MAX_COMP_NAME_LEN];// = "OMX.hisi.video.decoder";
    //char *vdecCompNames = (char *)malloc(sizeof(char)*MAX_COMP_NAME_LEN);
    char compRole[OMX_MAX_STRINGNAME_SIZE];

    OMX_CALLBACKTYPE call_back = {
        &EventHandler,
        &EmptyBufferDone,
        &FillBufferDone
    };

    DEBUG_PRINT("Inside %s\n", __FUNCTION__);

    /* Init. the OpenMAX Core */
    DEBUG_PRINT("Initializing OpenMAX Core....\n");
    omxresult = OMX_Init();
    if(OMX_ErrorNone != omxresult) {
        DEBUG_PRINT_ERROR("Failed to Init OpenMAX core\n");
        return -1;
    }

    DEBUG_PRINT("OpenMAX Core Init sucess!\n");

    /* CodecType Relative */
    if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_HEVC)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_HEVC, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingHEVC;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_MVC)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_MVC, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingMVC;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_H264)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_AVC, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VC1AP)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VC1, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVC1;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_MP4)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_MPEG4, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG4;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_MP1)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_MPEG1, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG1;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_MP2)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_MPEG2, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_H263)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_H263, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingH263;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_DIVX3)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_DIVX3, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingDIVX3;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP8)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VP8, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVP8;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VP6, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVP6;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6F)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VP6, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVP6;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6A)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VP6, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVP6;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VC1SMP)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VC1, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVC1;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_AVS)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_AVS, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingAVS;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_SORENSON)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_SORENSON, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingSorenson;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_REAL8)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_RV, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingRV;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_REAL9)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_RV, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingRV;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_MJPEG)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_MJPEG, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingMJPEG;
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP9)
    {
        strncpy(compRole, OMX_ROLE_VIDEO_DECODER_VP9, OMX_MAX_STRINGNAME_SIZE);
        pOmxTestInfo->portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingVP9;
    }
    else
    {
        DEBUG_PRINT_ERROR("Error: Unsupported codec %d\n", pOmxTestInfo->codec_format_option);
        return -1;
    }

    get_componet_name_by_role(vdecCompNames, compRole);

#ifdef HI_OMX_TEE_SUPPORT
    if (1 == pOmxTestInfo->tvp_option)
    {
        strncat(vdecCompNames, ".secure", 10);
    }
#endif

    for( i = 0; ; i++ )
    {
        char enumCompName[OMX_MAX_STRINGNAME_SIZE];
        memset(enumCompName, 0 , OMX_MAX_STRINGNAME_SIZE);

        omxresult = OMX_ComponentNameEnum(enumCompName,
            OMX_MAX_STRINGNAME_SIZE, i);

        if(OMX_ErrorNone != omxresult)
            break;

        if (!strncmp(enumCompName, vdecCompNames, OMX_MAX_STRINGNAME_SIZE))
        {
            is_found = 1;
            break;
        }
    }

    if (!is_found)
    {
        DEBUG_PRINT_ERROR("Error: cannot find match component!\n");
        return -1;
    }

    /* Query for video decoders*/
    is_found = 0;
    OMX_GetRolesOfComponent(vdecCompNames, &total, NULL);
    DEBUG_PRINT("OMX_GetRolesOfComponent %s, nums=%d\n", vdecCompNames, total);

    if (total)
    {
        /* Allocate memory for pointers to component name */
        char **role = (char**)malloc((sizeof(char*)) * total);

        for (i = 0; i < total; ++i)
        {
            role[i] = (char*)malloc(sizeof(char) * OMX_MAX_STRINGNAME_SIZE);
        }

        OMX_GetRolesOfComponent(vdecCompNames, &total, (OMX_U8 **)role);

        for (i = 0; i < total; ++i)
        {
            DEBUG_PRINT("role name is %s \n", role[i]);

            if (!strncmp(role[i], compRole, OMX_MAX_STRINGNAME_SIZE))
            {
                is_found = 1;
            }

            free(role[i]);
        }

        free(role);

        if (!is_found)
        {
            DEBUG_PRINT_ERROR("No Role found \n");
            return -1;
        }

    }
    else
    {
        DEBUG_PRINT_ERROR("No components found with Role:%s\n", vdecCompNames);
        return -1;
    }

    DEBUG_PRINT("OpenMAX OMX_GetHandle....\n");

    omxresult = OMX_GetHandle((OMX_HANDLETYPE*)(&pOmxTestInfo->dec_handle),
                          (OMX_STRING)vdecCompNames, NULL, &call_back);
    if (FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("Failed to Load the component:%s\n", vdecCompNames);
        return -1;
    }

    DEBUG_PRINT("%s OMX_GetHandle success\n", vdecCompNames);

    CONFIG_VERSION_SIZE(pOmxTestInfo->role);
    strncpy((char*)pOmxTestInfo->role.cRole, compRole, OMX_MAX_STRINGNAME_SIZE);

    omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle,
            OMX_IndexParamStandardComponentRole, &pOmxTestInfo->role);
    if(FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to set param!\n");
        return -1;
    }

    omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle,
            OMX_IndexParamStandardComponentRole, &pOmxTestInfo->role);

    if(FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to get role!\n");
        return -1;
    }

    DEBUG_PRINT("cur role of component is %s\n", (char *)pOmxTestInfo->role.cRole);

    /* CodecType Relative */
    OMX_VIDEO_PARAM_VP6TYPE vp6_param;
    OMX_VIDEO_PARAM_VC1TYPE vc1_param;
    OMX_VIDEO_PARAM_RVTYPE  rv_param;
    if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        vp6_param.eFormat = OMX_VIDEO_VP6;
        vp6_param.nSize   = sizeof(OMX_VIDEO_PARAM_VP6TYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6F)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        vp6_param.eFormat = OMX_VIDEO_VP6F;
        vp6_param.nSize   = sizeof(OMX_VIDEO_PARAM_VP6TYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VP6A)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        vp6_param.eFormat = OMX_VIDEO_VP6A;
        vp6_param.nSize   = sizeof(OMX_VIDEO_PARAM_VP6TYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVp6, &vp6_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VC1AP)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVC1, &vc1_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        vc1_param.eProfile = OMX_VIDEO_VC1ProfileAdvanced;
        vc1_param.nSize   = sizeof(OMX_VIDEO_PARAM_VC1TYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVC1, &vc1_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_VC1SMP)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVC1, &vc1_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        vc1_param.eProfile = OMX_VIDEO_VC1ProfileMain;
        vc1_param.nSize   = sizeof(OMX_VIDEO_PARAM_VC1TYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoVC1, &vc1_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_REAL8)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoRv, &rv_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        rv_param.eFormat = OMX_VIDEO_RVFormat8;
        rv_param.nSize   = sizeof(OMX_VIDEO_PARAM_RVTYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoRv, &rv_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }
    else if (pOmxTestInfo->codec_format_option == CODEC_FORMAT_REAL9)
    {
        omxresult = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoRv, &rv_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to get Parameter!\n", __LINE__);
            return -1;
        }

        rv_param.eFormat = OMX_VIDEO_RVFormat9;
        rv_param.nSize   = sizeof(OMX_VIDEO_PARAM_RVTYPE);
        omxresult = OMX_SetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoRv, &rv_param);
        if(FAILED(omxresult))
        {
            DEBUG_PRINT_ERROR("L%d: ERROR - Failed to set Parameter!\n", __LINE__);
            return -1;
        }
    }

    return 0;
}


static int Play_Decoder(OmxTestInfo_S * pOmxTestInfo)
{
    OMX_VIDEO_PARAM_PORTFORMATTYPE videoportFmt = {0};
    int bufCnt, index = 0;
    int frameSize = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_STATETYPE state = OMX_StateMax;
#ifdef SupportNative
    OMX_INDEXTYPE index_type;
#endif

    DEBUG_PRINT("Inside %s\n", __FUNCTION__);

    /* open the i/p and o/p files*/
    if(open_video_file(pOmxTestInfo) < 0)
    {
        DEBUG_PRINT_ERROR("Error in opening video file");
        return -1;
    }

    /* Get the port information */
    CONFIG_VERSION_SIZE(pOmxTestInfo->portParam);
    ret = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoInit,
                            (OMX_PTR)&pOmxTestInfo->portParam);
    if(FAILED(ret))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to get Port Param\n");
        return -1;
    }

    DEBUG_PRINT("portParam.nPorts:%d\n", pOmxTestInfo->portParam.nPorts);
    DEBUG_PRINT("portParam.nStartPortNumber:%d\n", pOmxTestInfo->portParam.nStartPortNumber);

    /* Query the decoder input's  buf requirements */
    CONFIG_VERSION_SIZE(pOmxTestInfo->portFmt);
    pOmxTestInfo->portFmt.nPortIndex = pOmxTestInfo->portParam.nStartPortNumber;

    OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamPortDefinition, &pOmxTestInfo->portFmt);

    DEBUG_PRINT("Dec: Min Buffer Count %d\n", pOmxTestInfo->portFmt.nBufferCountMin);
    DEBUG_PRINT("Dec: Act Buffer Count %d\n", pOmxTestInfo->portFmt.nBufferCountActual);
    DEBUG_PRINT("Dec: Buffer Size %d\n", pOmxTestInfo->portFmt.nBufferSize);

    if(OMX_DirInput != pOmxTestInfo->portFmt.eDir)
    {
        DEBUG_PRINT_ERROR ("Dec: Expect Input Port\n");
        return -1;
    }

    pOmxTestInfo->portFmt.format.video.nFrameWidth  = pOmxTestInfo->width;
    pOmxTestInfo->portFmt.format.video.nFrameHeight = pOmxTestInfo->height;
    pOmxTestInfo->portFmt.format.video.xFramerate   = 30;
    //pOmxTestInfo->portFmt.nBufferSize = CAL_FRAME_SIZE(pOmxTestInfo->portFmt.format.video.nStride,pOmxTestInfo->portFmt.format.video.nFrameHeight);
    /*if (true == readsize_add_in_stream)
    {
       pOmxTestInfo->portFmt.nBufferSize = (unsigned int)(20 << 10);
       pOmxTestInfo->portFmt.nBufferCountActual = 8;
    }*/
    /*else if (CODEC_FORMAT_MP4 == codec_format_option)
    {
       pOmxTestInfo->portFmt.nBufferSize = (unsigned int)(250 << 10);
    }*/

    //pOmxTestInfo->portFmt.nBufferSize = (unsigned int)(20 << 10);
    //pOmxTestInfo->portFmt.nBufferCountActual = 8;
    OMX_SetParameter(pOmxTestInfo->dec_handle,OMX_IndexParamPortDefinition, &pOmxTestInfo->portFmt);

    /* get again to check */
    OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamPortDefinition, &pOmxTestInfo->portFmt);
    DEBUG_PRINT("Dec: New Min Buffer Count %d\n", pOmxTestInfo->portFmt.nBufferCountMin);

    /* set component video fmt */
    CONFIG_VERSION_SIZE(videoportFmt);

    pOmxTestInfo->color_fmt = OMX_COLOR_FormatYUV420SemiPlanar;
    while (ret == OMX_ErrorNone)
    {
        videoportFmt.nPortIndex = 1;
        videoportFmt.nIndex = index;
        ret = OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamVideoPortFormat,
                (OMX_PTR)&videoportFmt);

        if((ret == OMX_ErrorNone) && (videoportFmt.eColorFormat ==
            pOmxTestInfo->color_fmt))
        {
            DEBUG_PRINT("Format[%u] supported by OMX Decoder\n", pOmxTestInfo->color_fmt);
            break;
        }
        index++;
    }

    if(ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("Error in retrieving supported color formats\n");
        return -1;
    }

    ret = OMX_SetParameter(pOmxTestInfo->dec_handle,
                           OMX_IndexParamVideoPortFormat, (OMX_PTR)&videoportFmt);
    if (ret != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("Setting Tile format failed\n");
        return -1;
    }

    DEBUG_PRINT("Video format: W x H (%d x %d)\n",
                pOmxTestInfo->portFmt.format.video.nFrameWidth,
                pOmxTestInfo->portFmt.format.video.nFrameHeight);

    DEBUG_PRINT("OMX_SendCommand Decoder -> IDLE\n");

    OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StateIdle,0);

    // Allocate buffer on decoder's i/p port
    error = Allocate_Buffers(pOmxTestInfo, pOmxTestInfo->dec_handle, &pOmxTestInfo->pInputBufHdrs, pOmxTestInfo->portFmt.nPortIndex, \
                             pOmxTestInfo->portFmt.nBufferCountActual, pOmxTestInfo->portFmt.nBufferSize);
    if (error != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("Error - OMX_AllocateBuffer Input buffer error\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }
    DEBUG_PRINT("OMX_AllocateBuffer Input buffer success\n");

    pOmxTestInfo->free_ip_buf_cnt = pOmxTestInfo->portFmt.nBufferCountActual;

    //Allocate buffer on decoder's o/p port
    pOmxTestInfo->portFmt.nPortIndex = pOmxTestInfo->portParam.nStartPortNumber + 1;
    OMX_GetParameter(pOmxTestInfo->dec_handle, OMX_IndexParamPortDefinition, &pOmxTestInfo->portFmt);
    if(OMX_DirOutput != pOmxTestInfo->portFmt.eDir)
    {
        DEBUG_PRINT_ERROR("Error - Expect Output Port\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    DEBUG_PRINT("buffer Size=%d\n", pOmxTestInfo->portFmt.nBufferSize);
    DEBUG_PRINT("buffer cnt=%d\n", pOmxTestInfo->portFmt.nBufferCountActual);

   if (0 == pOmxTestInfo->alloc_use_option)
   {
       error = Allocate_Buffers(pOmxTestInfo, pOmxTestInfo->dec_handle, &pOmxTestInfo->pOutYUVBufHdrs, pOmxTestInfo->portFmt.nPortIndex, \
                                   pOmxTestInfo->portFmt.nBufferCountActual, pOmxTestInfo->portFmt.nBufferSize);
       if (error != OMX_ErrorNone)
       {
           DEBUG_PRINT_ERROR("Error - OMX_AllocateBuffer Output buffer error\n");
           do_freeHandle_and_clean_up(pOmxTestInfo, true);
           return -1;
       }
       DEBUG_PRINT("OMX_AllocateBuffer Output buffer success\n");
   }

#ifdef SupportNative
   else
   {
        error = OMX_GetExtensionIndex(pOmxTestInfo->dec_handle, EnableAndroidNativeBuffers, &index_type);
        if (error != OMX_ErrorNone)
        {
            DEBUG_PRINT_ERROR("Error - OMX_GetExtensionIndex EnableAndroidNativeBuffers error\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }

        EnableAndroidNativeBuffersParams param_data;
        param_data.nPortIndex = pOmxTestInfo->portFmt.nPortIndex;
        param_data.bEnable = true;
        OMX_SetParameter(pOmxTestInfo->dec_handle, index_type, &param_data);

        error = OMX_GetExtensionIndex(pOmxTestInfo->dec_handle, UseAndroidNativeBuffer2, &index_type);
        if (error != OMX_ErrorNone)
        {
            DEBUG_PRINT_ERROR("Error - OMX_GetExtensionIndex UseAndroidNativeBuffer2 error\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }

        error = Use_Buffers(pOmxTestInfo->dec_handle, &pOmxTestInfo->pOutYUVBufHdrs, pOmxTestInfo->portFmt.nPortIndex, \
                            pOmxTestInfo->portFmt.nBufferCountActual, pOmxTestInfo->portFmt.nBufferSize);
        if (error != OMX_ErrorNone)
        {
            DEBUG_PRINT_ERROR("Error - OMX_UseBuffer Output buffer error\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }
        DEBUG_PRINT("OMX_UseBuffer Output buffer success\n");
   }
#endif

    pOmxTestInfo->free_op_buf_cnt = pOmxTestInfo->portFmt.nBufferCountActual;

    /* wait component state change from loaded->idle */
    wait_for_event(pOmxTestInfo, OMX_CommandStateSet);
    if (pOmxTestInfo->currentStatus == ERROR_STATE)
    {
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    OMX_GetState(pOmxTestInfo->dec_handle, &state);
    if (state != OMX_StateIdle)
    {
        DEBUG_PRINT_ERROR("Error - OMX state error\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    if (pOmxTestInfo->freeHandle_option == FREE_HANDLE_AT_IDLE)
    {
        OMX_STATETYPE state = OMX_StateMax;
        OMX_GetState(pOmxTestInfo->dec_handle, &state);

        if (state == OMX_StateIdle)
        {
            DEBUG_PRINT("Comp OMX_StateIdle,trying to call OMX_FreeHandle\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, false);
        }
        else
        {
            DEBUG_PRINT_ERROR("Comp: Decoder in state %d, "
                "trying to call OMX_FreeHandle\n", state);
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
        }
        return -1;
    }

    DEBUG_PRINT("OMX_SendCommand Decoder -> Executing\n");

    OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StateExecuting, 0);

    DEBUG_PRINT("OMX_SendCommand Decoder -> Executing, wait_for_event\n");
    wait_for_event(pOmxTestInfo, OMX_CommandStateSet);

    DEBUG_PRINT("OMX_SendCommand Decoder -> Executing, wait_for_event ok\n");
    if (pOmxTestInfo->currentStatus == ERROR_STATE)
    {
        DEBUG_PRINT_ERROR("OMX_SendCommand Decoder -> Executing\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    OMX_GetState(pOmxTestInfo->dec_handle, &state);

    if (state != OMX_StateExecuting)
    {
        DEBUG_PRINT_ERROR("Error - OMX state error, state %d\n", state);
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    DEBUG_PRINT("start OMX_FillThisBuffer ...\n");

    for(bufCnt = 0; bufCnt < pOmxTestInfo->used_op_buf_cnt; ++bufCnt)
    {
        DEBUG_PRINT("OMX_FillThisBuffer on output buf no.%d\n",bufCnt);

        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nOutputPortIndex = 1;
        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nFlags &= ~OMX_BUFFERFLAG_EOS;

        ret = OMX_FillThisBuffer(pOmxTestInfo->dec_handle, pOmxTestInfo->pOutYUVBufHdrs[bufCnt]);

        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("Error - OMX_FillThisBuffer failed!!\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }
        else
        {
            DEBUG_PRINT("OMX_FillThisBuffer success!\n");
            pOmxTestInfo->free_op_buf_cnt--;
        }
    }

    DEBUG_PRINT("start OMX_EmptyThisBuffer!\n");

    for (bufCnt = 0; bufCnt < pOmxTestInfo->used_ip_buf_cnt; bufCnt++)
    {
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nInputPortIndex = 0;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nOffset = 0;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nFlags = 0;

        frameSize = Read_Buffer_from_EsRawStream(pOmxTestInfo, pOmxTestInfo->pInputBufHdrs[bufCnt]);

        pOmxTestInfo->pInputBufHdrs[bufCnt]->nFilledLen = frameSize;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nInputPortIndex = 0;

        //DEBUG_PRINT_ALWS("%s: Timestamp sent(%lld)\n", __func__, pOmxTestInfo->pInputBufHdrs[bufCnt]->nTimeStamp);

        ret = OMX_EmptyThisBuffer(pOmxTestInfo->dec_handle, pOmxTestInfo->pInputBufHdrs[bufCnt]);
        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("ERROR: OMX_EmptyThisBuffer failed\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }
        else
        {
            if (bufCnt == 0)
            {
                gettimeofday(&pOmxTestInfo->t_first_send, NULL);
            }
            DEBUG_PRINT("OMX_EmptyThisBuffer success!\n");
            pOmxTestInfo->free_ip_buf_cnt--;
        }

        if (pOmxTestInfo->pInputBufHdrs[bufCnt]->nFlags & OMX_BUFFERFLAG_EOS)
        {
            pOmxTestInfo->bInputEosReached = true;
            break;
        }
    }

    if(0 != pthread_create(&pOmxTestInfo->ebd_thread_id, NULL, ebd_thread, pOmxTestInfo))
    {
        DEBUG_PRINT_ERROR("Error in Creating fbd_thread\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    if (pOmxTestInfo->freeHandle_option == FREE_HANDLE_AT_EXECUTING)
    {
        OMX_STATETYPE state = OMX_StateMax;
        OMX_GetState(pOmxTestInfo->dec_handle, &state);
        if (state == OMX_StateExecuting)
        {
            DEBUG_PRINT("State OMX_StateExecuting, trying OMX_FreeHandle\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, false);
        }
        else
        {
            DEBUG_PRINT_ERROR("Error: state %d , trying OMX_FreeHandle\n", state);
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
        }
        return -1;
    }
    else if (pOmxTestInfo->freeHandle_option == FREE_HANDLE_AT_PAUSE)
    {
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StatePause,0);
        wait_for_event(pOmxTestInfo, OMX_CommandStateSet);

        OMX_STATETYPE state = OMX_StateMax;
        OMX_GetState(pOmxTestInfo->dec_handle, &state);

        if (state == OMX_StatePause)
        {
            DEBUG_PRINT("Decoder is in OMX_StatePause , call OMX_FreeHandle\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, false);
        }
        else
        {
            DEBUG_PRINT_ERROR("Error - Decoder is in state %d ,call OMX_FreeHandle\n", state);
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
        }
        return -1;
    }

    return 0;
}

/*
static int get_next_command(OmxTestInfo_S * pOmxTestInfo, FILE *seq_file)
{
    int i = -1;

    do {
        i++;
        if(fread(curr_seq_command[i], 1, 1, seq_file) != 1)
            return -1;
    } while(curr_seq_command[i] != '\n');

    curr_seq_command[i] = 0;

    printf("**cmd_str = %s**\n", curr_seq_command);

    return 0;
}
*/

static int process_current_command(OmxTestInfo_S * pOmxTestInfo, const char *seq_command)
{
    if (pOmxTestInfo->currentStatus == PORT_SETTING_CHANGE_STATE)
    {
        DEBUG_PRINT_ERROR("\nCurrent Status = PORT_SETTING_CHANGE_STATE, Command %s Rejected !\n", seq_command);
        return 0;
    }

    if(strcmp(seq_command, "p") == 0)
    {
        printf("$$$$$   PAUSE    $$$$$\n");
        printf("Sending PAUSE cmd to OMX compt\n");
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StatePause,0);
        wait_for_event(pOmxTestInfo, OMX_CommandStateSet);
        printf("EventHandler for PAUSE DONE\n\n");
    }
    else if(strcmp(seq_command, "r") == 0)
    {
        printf("$$$$$   RESUME    $$$$$\n");
        printf("Sending RESUME cmd to OMX compt\n");
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StateExecuting,0);
        wait_for_event(pOmxTestInfo, OMX_CommandStateSet);
        printf("EventHandler for RESUME DONE\n\n");
    }
    else if(strcmp(seq_command, "s") == 0)
    {
        printf("$$$$$   SEEK    $$$$$\n");
        printf("Sending SEEK cmd to OMX compt\n");
        pOmxTestInfo->seeking_progress = 1;
        pOmxTestInfo->flush_input_progress = 1;
        pOmxTestInfo->flush_output_progress = 1;
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandFlush, OMX_ALL, 0);
        wait_for_event(pOmxTestInfo, OMX_CommandFlush);

        seek_progress(pOmxTestInfo);
        pOmxTestInfo->seeking_progress = 0;
        sem_post(&pOmxTestInfo->seek_sem);
        printf("EventHandler for SEEK DONE\n\n");
    }
    else if(strcmp(seq_command, "f") == 0)
    {
        printf("$$$$$   FLUSH    $$$$$\n");
        printf("Sending FLUSH cmd to OMX compt\n");

        pOmxTestInfo->flush_input_progress = 1;
        pOmxTestInfo->flush_output_progress = 1;
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandFlush, OMX_ALL, 0);
        wait_for_event(pOmxTestInfo, OMX_CommandFlush);
        printf("Sending FLUSH cmd DONE\n\n");
    }
    else if(strcmp(seq_command, "f0") == 0)
    {
        printf("$$$$$   FLUSH-IN    $$$$$\n");
        printf("Sending FLUSH-IN cmd to OMX compt\n");

        pOmxTestInfo->flush_input_progress = 1;
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandFlush, 0, 0);
        wait_for_event(pOmxTestInfo, OMX_CommandFlush);
        printf("Sending FLUSH-IN cmd DONE\n\n");
    }
    else if(strcmp(seq_command, "f1") == 0)
    {
        printf("$$$$$   FLUSH-OUT    $$$$$\n");
        printf("Sending FLUSH-OUT cmd to OMX compt\n");

        pOmxTestInfo->flush_output_progress = 1;
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandFlush, 1, 0);
        wait_for_event(pOmxTestInfo, OMX_CommandFlush);
        printf("Sending FLUSH-OUT cmd DONE\n\n");
    }
    /*else if(strcmp(seq_command, "disable-op") == 0)
    {
        printf("$$$$$   DISABLE OP PORT   $$$$$\n");
        printf("Sending DISABLE OP cmd to OMX compt\n");

        if (disable_output_port() != 0)
        {
            printf("ERROR: While DISABLE OP...\n");
            do_freeHandle_and_clean_up(true);
            return -1;
        }
        printf("DISABLE OP PORT SUCESS!\n\n");
    }
    else if(strstr(seq_command, "enable-op") == seq_command)
    {
        printf("$$$$$   ENABLE OP PORT    $$$$$\n");
        printf("Sending ENABLE OP cmd to OMX compt\n");

        if (enable_output_port() != 0)
        {
            printf("ERROR: While ENABLE OP...\n");
            do_freeHandle_and_clean_up(true);
            return -1;
        }
        printf("ENABLE OP PORT SUCESS!\n\n");
    }*/
    else
    {
        DEBUG_PRINT_ERROR("$$$$$   INVALID CMD [%s]   $$$$$\n", seq_command);
    }

    return 0;
}


static void* ebd_thread(void* pArg)
{
    OmxTestInfo_S * pOmxTestInfo = (OmxTestInfo_S *)pArg;

    while(pOmxTestInfo->currentStatus != ERROR_STATE)
    {
        int readBytes =0;
        OMX_BUFFERHEADERTYPE* pBuffer = NULL;

        pOmxTestInfo->EtbStatus = THREAD_WAITING;

        if(pOmxTestInfo->flush_input_progress)
        {
            sem_wait(&pOmxTestInfo->in_flush_sem);
        }

        sem_wait(&pOmxTestInfo->etb_sem);

        if(pOmxTestInfo->seeking_progress)
        {
            sem_wait(&pOmxTestInfo->seek_sem);
        }

        if (pOmxTestInfo->ebd_thread_exit)
            break;

        if (pOmxTestInfo->bInputEosReached)
            continue;

        pthread_mutex_lock(&pOmxTestInfo->etb_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE *) pop(pOmxTestInfo->etb_queue);
        pthread_mutex_unlock(&pOmxTestInfo->etb_lock);

        if(pBuffer == NULL)
        {
            DEBUG_PRINT_ERROR("Error - No etb pBuffer to dequeue\n");
            continue;
        }

        pOmxTestInfo->EtbStatus = THREAD_RUNNING;

        pBuffer->nOffset = 0;
        pBuffer->nFlags  = 0;

        readBytes = Read_Buffer_from_EsRawStream(pOmxTestInfo, pBuffer);

        if(pOmxTestInfo->seeking_progress)
        {
            DEBUG_PRINT("Read es done meet seeking.\n");
            continue;
        }

        if(pOmxTestInfo->flush_input_progress)
        {
            DEBUG_PRINT("Read es done meet input flushing.\n");
            continue;
        }

        if (pOmxTestInfo->ebd_thread_exit)
        {
            break;
        }

        if(readBytes > 0)
        {
            DEBUG_PRINT("%s: Timestamp sent(%lld)\n", __func__, pBuffer->nTimeStamp);
            if (pBuffer->nFlags & OMX_BUFFERFLAG_EOS)
            {
                pOmxTestInfo->bInputEosReached = true;
            }
        }
        else
        {
            DEBUG_PRINT("EBD::Either EOS or Some Error while reading file\n");
            pOmxTestInfo->bInputEosReached = true;
        }

        pBuffer->nFilledLen = readBytes;
        OMX_EmptyThisBuffer(pOmxTestInfo->dec_handle,pBuffer);
        pOmxTestInfo->free_ip_buf_cnt--;
    }

    pOmxTestInfo->EtbStatus = THREAD_INVALID;

    return NULL;
}


static void* fbd_thread(void* pArg)
{
    OmxTestInfo_S * pOmxTestInfo = (OmxTestInfo_S *)pArg;
    OMX_BUFFERHEADERTYPE *pBuffer = NULL;

    DEBUG_PRINT("First Inside %s", __FUNCTION__);

    while(pOmxTestInfo->currentStatus != ERROR_STATE)
    {
        pOmxTestInfo->FtbStatus = THREAD_WAITING;

        if(pOmxTestInfo->flush_output_progress)
        {
            sem_wait(&pOmxTestInfo->out_flush_sem);
        }

        sem_wait(&pOmxTestInfo->ftb_sem);

        if (pOmxTestInfo->fbd_thread_exit)
        {
            break;
        }

        if (pOmxTestInfo->currentStatus == PORT_SETTING_CHANGE_STATE)
        {
            pOmxTestInfo->currentStatus = PORT_SETTING_RECONFIG_STATE;
            DEBUG_PRINT("\nRECONFIG OP PORT\n\n");
            if (output_port_reconfig(pOmxTestInfo) != 0)
            {
                DEBUG_PRINT("ERROR: While Reconfig OP...\n");
                pOmxTestInfo->currentStatus = pOmxTestInfo->preStatus;
                do_freeHandle_and_clean_up(pOmxTestInfo, true);
                break;
            }
            DEBUG_PRINT("\nRECONFIG OP PORT DONE!\n\n");
            pOmxTestInfo->currentStatus = pOmxTestInfo->preStatus;
        }

        if (pOmxTestInfo->fbd_thread_exit)
        {
            break;
        }

        pthread_mutex_lock(&pOmxTestInfo->ftb_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE *)pop(pOmxTestInfo->ftb_queue);
        pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);

        if (!pBuffer)
        {
            DEBUG_PRINT("Info - No pBuffer to dequeue\n");
            continue;
        }

        pOmxTestInfo->FtbStatus = THREAD_RUNNING;

        /********************************************************************/
        /* De-Initializing the open max and relasing the buffers and */
        /* closing the files.*/
        /********************************************************************/
        if (pOmxTestInfo->flush_output_progress)
        {
        #if 0  // continue to send buffer after output flush

                pBuffer->nFilledLen = 0;
                pBuffer->nFlags &= ~OMX_BUFFERFLAG_EXTRADATA;

                pthread_mutex_lock(&pOmxTestInfo->ftb_lock);

                if(push(pOmxTestInfo->fbd_queue, (void *)pBuffer) < 0)
                {
                    DEBUG_PRINT_ERROR("Error in enqueueing fbd_data\n");
                }
                else
                {
                    sem_post(&pOmxTestInfo->ftb_sem);
                }
                pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);

        #else  // not send buffer after output flush

            DEBUG_PRINT("Fill this buffer meet output flushing.\n");
            continue;

        #endif
        }
        else if (pOmxTestInfo->seeking_progress)
        {
            DEBUG_PRINT("Fill this buffer meet seek pending.\n");
            continue;
        }
        else
        {
            pBuffer->nFilledLen = 0;
            if ( OMX_FillThisBuffer(pOmxTestInfo->dec_handle, pBuffer) == OMX_ErrorNone )
            {
                pOmxTestInfo->free_op_buf_cnt--;
            }
        }
    }

    pOmxTestInfo->FtbStatus = THREAD_INVALID;

    return NULL;
}

/****************************************************************************/
/* 捕获ctrl + c 信号量                                                      */
/****************************************************************************/
void SignalHandle(int sig)
{
    int i = 0;

    printf("Signal Handle - I got signal %d\n", sig);
    (void) signal(SIGINT,SIG_IGN);

    for (i=0; i<MAX_INST_NUM; i++)
    {
        if (1 == OmxTestInfo[i].is_open)
        {
            printf("Sample free channel %d.\n", i);
            do_freeHandle_and_clean_up(&OmxTestInfo[i], true); //需要增加实例参数，比较麻烦，暂时先屏蔽
        }
    }
    (void) signal(SIGINT, SIG_DFL);
}


static void Init_OmxInst()
{
    int i = 0;

    memset(OmxTestInfo, 0, g_TestInstNum * sizeof(OmxTestInfo_S));

    for (i = 0; i < g_TestInstNum; i++)
    {
        OmxTestInfo[i].timestampInterval = 33333;
        OmxTestInfo[i].currentStatus = GOOD_STATE;

        OmxTestInfo[i].sliceheight  = DEFAULT_HEIGHT;
        OmxTestInfo[i].height       = DEFAULT_HEIGHT;
        OmxTestInfo[i].stride       = DEFAULT_WIDTH;
        OmxTestInfo[i].width        = DEFAULT_WIDTH;

        OmxTestInfo[i].readsize_add_in_stream = false;
    }

    return;
}



int get_resolution(char *pFileName, int *pWidth, int *pHeight)
{
    int   i;
    int   len       = 0;
    int   found_num = 0;
    char  tmpBuf[10];

    if (NULL == pFileName || NULL == pWidth || NULL == pHeight)
    {
        printf("get_resolution with invalid param, return error.\n");
        return -1;
    }

    len  = strlen(pFileName);

    for (i=len-1; i>0; i--)
    {
        if ('x' == pFileName[i])
        {
            strncpy(tmpBuf, &pFileName[i+1], 4);
            tmpBuf[4] = '\0';
            *pHeight  = atoi(tmpBuf);
            found_num = 1;
        }

        if ('_' == pFileName[i] && found_num)
        {
            strncpy(tmpBuf, &pFileName[i+1], 4);
            tmpBuf[4] = '\0';
            *pWidth  = atoi(tmpBuf);
            found_num = 2;
            break;
        }
    }

    if (2 == found_num)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int is_framepacket_standard(codec_format format)
{
    if( CODEC_FORMAT_H263     == format
     || CODEC_FORMAT_DIVX3    == format
     || CODEC_FORMAT_VP8      == format
     || CODEC_FORMAT_VP6      == format
     || CODEC_FORMAT_VP6F     == format
     || CODEC_FORMAT_VP6A     == format
     || CODEC_FORMAT_VC1SMP   == format
     || CODEC_FORMAT_SORENSON == format
     || CODEC_FORMAT_REAL8    == format
     || CODEC_FORMAT_REAL9    == format
     || CODEC_FORMAT_VP9      == format )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_tidx_file_exist(char *psrc_file)
{
    FILE *fptidxFile = NULL;
    char tidxfile[512] = {0};

    snprintf(tidxfile, sizeof(tidxfile), "%s.tidx", psrc_file);

    fptidxFile = fopen(tidxfile, "r");
    if(NULL == fptidxFile)
    {
        return 0;
    }
    else
    {
        fclose(fptidxFile);
        return 1;
    }
}


#ifdef HI_OMX_TEE_SUPPORT

HI_S32 InitSecEnvironment(HI_VOID)
{
    DEBUG_PRINT_ERROR("%s enter \n", __func__);

    TEEC_Result result;
    uint32_t origin;
    TEEC_UUID svc_id = {0x0D0D0D0D, 0x0D0D, 0x0D0D,
                        {0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D, 0x0D}
                       };

    TEEC_Operation session_operation;

    memset(&session_operation, 0, sizeof(TEEC_Operation));

    session_operation.started = 1;
    session_operation.paramTypes = TEEC_PARAM_TYPES(
                               TEEC_NONE,
                               TEEC_NONE,
                               TEEC_MEMREF_TEMP_INPUT,
                               TEEC_MEMREF_TEMP_INPUT);

    if (TEEC_InitializeContext(HI_NULL, &g_TeeContext) != TEEC_SUCCESS)
    {
        DEBUG_PRINT_ERROR("TEEC_InitializeContext Failed!\n");
        return HI_FAILURE;
    }

    result = TEEC_OpenSession(&g_TeeContext, &g_teeSession, &svc_id, TEEC_LOGIN_IDENTIFY, HI_NULL, &session_operation, &origin);
    if (result != TEEC_SUCCESS)
    {
        DEBUG_PRINT_ERROR("TEEC_OpenSession Failed!\n");
        TEEC_FinalizeContext(&g_TeeContext);

        exit(0);
    }

    DEBUG_PRINT_ERROR("%s exit \n", __func__);

    return HI_SUCCESS;
}


HI_VOID DeInitSecEnvironment(HI_VOID)
{
    TEEC_CloseSession(&g_teeSession);

    TEEC_FinalizeContext(&g_TeeContext);
}

#endif

int main(int argc, char **argv)
{
    int i = 0;
    int width = 0;
    int height = 0;

    if (argc > MAX_INST_NUM + 1)
    {
        printf("Command line argument is not available\n");
        printf("To use it: ./sample_omx <in-filename 1> <in-filename 2> ... <in-filename n> \n");
        return -1;
    }

    g_TestInstNum = argc - 1;

    Init_OmxInst();

    for (i = 0; i < g_TestInstNum; i++)
    {
        OmxTestInfo[i].inst_no = i; //记录实例号
        strncpy(OmxTestInfo[i].in_filename, argv[i+1], strlen(argv[i+1])+1);
        printf("Input values: inputfilename[%s]\n", OmxTestInfo[i].in_filename);

        if (get_resolution(OmxTestInfo[i].in_filename, &width, &height) == 0)
        {
            if (width*height <= MAX_WIDTH*MAX_HEIGHT && width > 0 && height > 0)
            {
                OmxTestInfo[i].width  = width;
                OmxTestInfo[i].height = height;
                printf("get_resolution: width=%d, height=%d\n", OmxTestInfo[i].width, OmxTestInfo[i].height);
            }
        }

        /* 1. Set Codec Format */
        printf("Command line argument is available\n");
        printf("*********************************************\n");
        printf("please enter codec format option for Instance [%d]\n", i);
           printf("*********************************************\n");
        printf("0 --> H264\n");
        printf("1 --> MPEG2\n");
        printf("2 --> MPEG4\n");
        printf("3 --> H263\n");
        printf("4 --> SORENSON\n");
        printf("5 --> AVS\n");
        printf("6 --> VC1ap\n");
        printf("7 --> VC1smp\n");
        printf("8 --> REAL8\n");
        printf("9 --> REAL9\n");
        printf("10--> DIVX3\n");
        printf("11--> VP6\n");
        printf("12--> VP6F\n");
        printf("13--> VP6A\n");
        printf("14--> VP8\n");
        printf("15--> MJPEG\n");
        printf("16--> MPEG1\n");
        printf("17--> HEVC\n");
        printf("18--> MVC\n");
        printf("19--> VP9\n");

        fflush(stdin);
        scanf("%d", (int *)(&OmxTestInfo[i].codec_format_option));

        if (OmxTestInfo[i].codec_format_option >= CODEC_FORMAT_MAX)
        {
            printf("Wrong test case...[%d]\n", OmxTestInfo[i].codec_format_option);
            return -1;
        }

    #ifdef EnableSetColor
        /* 2. Set Color Format */
        printf("Set Output Color Format:\n");
        printf("0 --> YUV420 SemiPlanar\n");
        printf("1 --> YUV420 Planar\n");
        fflush(stdin);
        scanf("%d", &g_ColorFormat);
        switch (g_ColorFormat)
        {
            case 0:
                g_ColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
                break;
            case 1:
                g_ColorFormat = OMX_COLOR_FormatYUV420Planar;
                break;
            default:
                printf("Wrong config...[%d]\n", g_ColorFormat);
                return -1;
        }
    #endif

    #ifdef SupportNative
        DEBUG_PRINT_ALWS("*********************************************\n");
         DEBUG_PRINT_ALWS("please enter alloc/use option for Instance [%d]\n", i);
         DEBUG_PRINT_ALWS("*********************************************\n");
         DEBUG_PRINT_ALWS("0--> Alloc Buffers\n");
         DEBUG_PRINT_ALWS("1--> Use Buffers\n");

         fflush(stdin);
         scanf("%d", &(OmxTestInfo[i].alloc_use_option));

         fflush(stdin);
         if (OmxTestInfo[i].alloc_use_option != 0 && OmxTestInfo[i].alloc_use_option != 1)
         {
             DEBUG_PRINT_ERROR("Wrong test option...[%d]\n", OmxTestInfo[i].alloc_use_option);
             return -1;
         }
    #endif

    #ifdef HI_OMX_TEE_SUPPORT
        DEBUG_PRINT_ALWS("*********************************************\n");
        DEBUG_PRINT_ALWS("please enter tvp option for Instance [%d]\n", i);
        DEBUG_PRINT_ALWS("*********************************************\n");
        DEBUG_PRINT_ALWS("0 --> disable tvp option.\n");
        DEBUG_PRINT_ALWS("1 --> enable  tvp option.\n");
        fflush(stdin);
        scanf("%d", &(OmxTestInfo[i].tvp_option));
        fflush(stdin);
         if (OmxTestInfo[i].tvp_option != 0 && OmxTestInfo[i].tvp_option != 1)
         {
             DEBUG_PRINT_ERROR("Wrong tvp option...[%d]\n", OmxTestInfo[i].tvp_option);
             return -1;
         }
    #endif

    #ifdef EnableTest
        DEBUG_PRINT_ALWS("*********************************************\n");
           DEBUG_PRINT_ALWS("please enter test option for Instance [%d]\n", i);
        DEBUG_PRINT_ALWS("*********************************************\n");
        DEBUG_PRINT_ALWS("0 --> Play the clip till the end\n");
        DEBUG_PRINT_ALWS("1 --> Run compliance test. Do NOT expect any display for most option.\n");
        DEBUG_PRINT_ALWS("Please only see \"TEST SUCCESSFULL\"to indicate test pass\n");
        DEBUG_PRINT_ALWS(stdin);
        scanf("%d", &(OmxTestInfo[i].test_option));
           fflush(stdin);
    #endif

        /* 3. Set Extra Option */
        if (is_framepacket_standard(OmxTestInfo[i].codec_format_option))
        {
            if (is_tidx_file_exist(OmxTestInfo[i].in_filename))
            {
                OmxTestInfo[i].frame_in_packet = 1;
                OmxTestInfo[i].readsize_add_in_stream = false;
            }
            else
            {
                OmxTestInfo[i].frame_in_packet = 0;
                OmxTestInfo[i].readsize_add_in_stream = true;
            }
        }

        /* CodecType Relative */
        switch (OmxTestInfo[i].codec_format_option)
        {
        case CODEC_FORMAT_HEVC:
            DEBUG_PRINT("codec HEVC selected\n");
            break;
        case CODEC_FORMAT_H264:
            DEBUG_PRINT("codec H264 selected\n");
            break;
        case CODEC_FORMAT_MVC:
            DEBUG_PRINT("codec MVC selected\n");
            break;
        case CODEC_FORMAT_VC1AP:
            DEBUG_PRINT("codec VC1ap selected\n");
            break;
        case CODEC_FORMAT_MP4:
            DEBUG_PRINT("codec MP4 selected\n");
            break;
        case CODEC_FORMAT_MP1:
            DEBUG_PRINT("codec MP1 selected\n");
            break;
        case CODEC_FORMAT_MP2:
            DEBUG_PRINT("codec MP2 selected\n");
            break;
        case CODEC_FORMAT_H263:
            DEBUG_PRINT("codec H263 selected\n");
            break;
        case CODEC_FORMAT_DIVX3:
            DEBUG_PRINT("codec DIVX3 selected\n");
            break;
        case CODEC_FORMAT_VP8:
            DEBUG_PRINT("codec VP8 selected\n");
            break;
        case CODEC_FORMAT_VP6:
            DEBUG_PRINT("codec VP6 selected\n");
            break;
        case CODEC_FORMAT_VP6F:
            DEBUG_PRINT("codec VP6F selected\n");
            break;
        case CODEC_FORMAT_VP6A:
            DEBUG_PRINT("codec VP6A selected\n");
            break;
        case CODEC_FORMAT_VC1SMP:
            DEBUG_PRINT("codec VC1smp selected\n");
            break;
        case CODEC_FORMAT_AVS:
            DEBUG_PRINT("codec AVS selected\n");
            break;
        case CODEC_FORMAT_SORENSON:
            DEBUG_PRINT("codec SORENSON selected\n");
            break;
        case CODEC_FORMAT_REAL8:
            DEBUG_PRINT("codec REAL8 selected\n");
            break;
        case CODEC_FORMAT_REAL9:
            DEBUG_PRINT("codec REAL9 selected\n");
            break;
        case CODEC_FORMAT_MJPEG:
            DEBUG_PRINT("codec MJPEG selected\n");
            break;
        case CODEC_FORMAT_VP9:
            DEBUG_PRINT("codec VP9 selected\n");
            break;
        default:
            DEBUG_PRINT_ERROR("Error: Unknown code %d\n", OmxTestInfo[i].codec_format_option);
            return -1;
        }

        if (OmxTestInfo[i].test_option == 1)
        {
            printf("*********************************************\n");
            printf("ENTER THE COMPLIANCE TEST YOU WOULD LIKE TO EXECUTE\n");
            printf("*********************************************\n");
            printf("1 --> Call Free Handle at the OMX_StateLoaded\n");
            printf("2 --> Call Free Handle at the OMX_StateIdle\n");
            printf("3 --> Call Free Handle at the OMX_StateExecuting\n");
            printf("4 --> Call Free Handle at the OMX_StatePause\n");

            fflush(stdin);
            scanf("%d", (int *)(&OmxTestInfo[i].freeHandle_option));
            fflush(stdin);
        }
        else
        {
            OmxTestInfo[i].freeHandle_option = (freeHandle_test)0;
        }

        DEBUG_PRINT("*********get cmd line ok! *******\n");

         pthread_cond_init(&(OmxTestInfo[i].event_cond), 0);
         pthread_mutex_init(&OmxTestInfo[i].event_lock, 0);
         pthread_mutex_init(&OmxTestInfo[i].etb_lock, 0);
         pthread_mutex_init(&OmxTestInfo[i].ftb_lock, 0);

         if (-1 == sem_init(&OmxTestInfo[i].etb_sem, 0, 0))
         {
             DEBUG_PRINT("Error - sem_init failed %d\n", errno);
         }

         if (-1 == sem_init(&OmxTestInfo[i].ftb_sem, 0, 0))
         {
             DEBUG_PRINT("Error - sem_init failed %d\n", errno);
         }

         if (-1 == sem_init(&OmxTestInfo[i].in_flush_sem, 0, 0))
         {
             DEBUG_PRINT("Error - sem_init failed %d\n", errno);
         }

         if (-1 == sem_init(&OmxTestInfo[i].out_flush_sem, 0, 0))
         {
             DEBUG_PRINT("Error - sem_init failed %d\n", errno);
         }

         if (-1 == sem_init(&OmxTestInfo[i].seek_sem, 0, 0))
         {
             DEBUG_PRINT("Error - sem_init failed %d\n", errno);
         }

        /* 注册ctrl c 函数 */
        (void) signal(SIGINT, SignalHandle);

         OmxTestInfo[i].etb_queue = alloc_queue();
         if (OmxTestInfo[i].etb_queue == NULL)
         {
             DEBUG_PRINT_ERROR("Error in Creating etb_queue\n");
             return -1;
         }

         OmxTestInfo[i].ftb_queue = alloc_queue();
         if (OmxTestInfo[i].ftb_queue == NULL)
         {
             DEBUG_PRINT_ERROR("Error in Creating ftb_queue\n");
             free_queue(OmxTestInfo[i].etb_queue);
             return -1;
         }
    }

    for (i = 0; i < g_TestInstNum; i++)
    {
        if(0 != pthread_create(&OmxTestInfo[i].fbd_thread_id, NULL, fbd_thread, &OmxTestInfo[i]))
        {
            DEBUG_PRINT_ERROR("Error in Creating fbd_thread\n");
            free_queue(OmxTestInfo[i].etb_queue);
            free_queue(OmxTestInfo[i].ftb_queue);
            return -1;
        }

        //OmxTestInfo[i].Read_Buffer = Read_Buffer_from_EsRawStream;
        if(Init_Decoder(&OmxTestInfo[i]) < 0)
        {
            DEBUG_PRINT_ERROR("Error - Decoder Init failed\n");
            return -1;
        }

    #ifdef HI_OMX_TEE_SUPPORT
        if (1 == OmxTestInfo[i].tvp_option)
        {
            strncpy(OmxTestInfo[i].pCAStreamBuf.bufname, "OmxSampleEsBuf", sizeof(OmxTestInfo[i].pCAStreamBuf.bufname));
            OmxTestInfo[i].pCAStreamBuf.overflow_threshold = 100;
            OmxTestInfo[i].pCAStreamBuf.underflow_threshold = 0;
            OmxTestInfo[i].pCAStreamBuf.bufsize = 4*1024*1024;
            if (HI_SUCCESS != HI_MPI_MMZ_Malloc(&OmxTestInfo[i].pCAStreamBuf))
            {
                DEBUG_PRINT_ERROR("Alloc OmxSampleEsBuf failed\n");

                return -1;
            }

            if (HI_SUCCESS != InitSecEnvironment())
            {
                DEBUG_PRINT_ERROR("call HI_UNF_AVPLAY_Start failed.\n");
                HI_MPI_MMZ_Free(&OmxTestInfo[i].pCAStreamBuf);

                return -1;
            }
        }
    #endif

        if(Play_Decoder(&OmxTestInfo[i]) < 0)
        {
            DEBUG_PRINT_ERROR("Error - Decode Play failed\n");
    #ifdef HI_OMX_TEE_SUPPORT
            if (1 == OmxTestInfo[i].tvp_option)
            {
                DeInitSecEnvironment();
            }
    #endif
            return -1;
        }

        OmxTestInfo[i].is_open = 1;
    }

    loop_function();

    return 0;
}

static void exit_loop(OmxTestInfo_S * pOmxTestInfo)
{
    do_freeHandle_and_clean_up(pOmxTestInfo, (pOmxTestInfo->currentStatus == ERROR_STATE));

    return;
}


static void loop_function(void)
{
    int i = 0;
    int cmd_error = 0;

    char curr_seq_command[512];
    int command_inst_no = 0; //命令对应的实例; 等于0表示对所有实例有效

    DEBUG_PRINT("\nTest for control, cmds as follows:\n");
    DEBUG_PRINT("First input the command type, Then input the instance num.\n");

    DEBUG_PRINT("\nPlease input the COMMAND:\n");
    DEBUG_PRINT("q (exit), p (pause), r (resume), f (flush all), f0 (flush in), f1 (flush out), s (seek)\n\n");

    while (cmd_error == 0)
    {
        for (i = 0; i < g_TestInstNum; i++)
        {
            if (OmxTestInfo[i].bOutputEosReached)
            {
                //goto exit1;
            }
        }

        fflush(stdin);
        scanf("%s", curr_seq_command);

        command_inst_no = 0;
        if (g_TestInstNum > 1)
        {
            printf("\nPlease input the instance No: [0, %d], %d for all instance.\n", g_TestInstNum, g_TestInstNum);
            fflush(stdin);
            scanf("%d", &command_inst_no);
            DEBUG_PRINT_ALWS("command :%s inst:%d\n", curr_seq_command, command_inst_no);

            if (command_inst_no < 0 || command_inst_no > g_TestInstNum)
            {
                DEBUG_PRINT_ERROR("invalid parameter!\n");
                continue;
            }
        }

        if (!strcmp(curr_seq_command, "q"))
        {
            if (g_TestInstNum > 1)  // multi inst on run
            {
                if (g_TestInstNum == command_inst_no)
                {
                    printf("All test-case exit!\n");
                    goto exit1;
                }
                else // command on one inst
                {
                    printf("Test-case %d exit!\n", command_inst_no);
                    if (1 == OmxTestInfo[command_inst_no].is_open)
                    {
                        exit_loop(&OmxTestInfo[command_inst_no]);
                    }
                    for (i = 0; i < MAX_INST_NUM; i++)
                    {
                        if (1 == OmxTestInfo[i].is_open)
                        {
                            break;
                        }
                    }
                    if (i >= MAX_INST_NUM)
                    {
                        printf("All test-case finish, exit!\n");
                        return;
                    }
                }
            }
            else // single inst on run
            {
                printf("Test-case exit!\n");
                goto exit1;
            }

        }
        else
        {
            if (g_TestInstNum > 1)  // multi inst on run
            {
                if (g_TestInstNum == command_inst_no)  // command on all valid insts
                {
                    for (i = 0; i < MAX_INST_NUM; i++)
                    {
                        if (1 == OmxTestInfo[i].is_open)
                        {
                            cmd_error = process_current_command(&OmxTestInfo[i], curr_seq_command);
                        }
                    }
                }
                else  // command on one inst
                {
                    cmd_error = process_current_command(&OmxTestInfo[command_inst_no], curr_seq_command);
                }
            }
            else // single inst on run
            {
                cmd_error = process_current_command(&OmxTestInfo[command_inst_no], curr_seq_command);
            }
        }
   }


exit1:

    for (i = 0; i < MAX_INST_NUM; i++)
    {
        if (1 == OmxTestInfo[i].is_open)
        {
            exit_loop(&OmxTestInfo[i]);
        }
    }

    return;
}


static OMX_ERRORTYPE Allocate_Buffers (OmxTestInfo_S * pOmxTestInfo,
                                       OMX_COMPONENTTYPE *dec_handle,
                                       OMX_BUFFERHEADERTYPE  ***pBufHdrs,
                                       OMX_U32 nPortIndex,
                                       long bufCntMin, long bufSize)
{
    DEBUG_PRINT("Inside %s \n", __FUNCTION__);
    OMX_ERRORTYPE error = OMX_ErrorNone;
    long bufCnt=0;

    *pBufHdrs = (OMX_BUFFERHEADERTYPE **)malloc(sizeof(OMX_BUFFERHEADERTYPE*)*bufCntMin);

    if (0 == nPortIndex)
    {
        pOmxTestInfo->used_ip_buf_cnt = 0;
    }
    else
    {
        pOmxTestInfo->used_op_buf_cnt = 0;
    }
    for(bufCnt=0; bufCnt < bufCntMin; ++bufCnt)
    {
        DEBUG_PRINT("OMX_AllocateBuffer No %ld\n", bufCnt);
        error = OMX_AllocateBuffer(dec_handle, &((*pBufHdrs)[bufCnt]), nPortIndex, NULL, bufSize);

        if (error != OMX_ErrorNone)
        {
            DEBUG_PRINT_ERROR("OMX_AllocateBuffer No %ld failed!\n", bufCnt);
            break;
        }
        else
        {
            if (0 == nPortIndex)
            {
                pOmxTestInfo->used_ip_buf_cnt++;
            }
            else
            {
                pOmxTestInfo->used_op_buf_cnt++;
            }
        }
    }

    return error;
}


static OMX_ERRORTYPE Use_Buffers ( OMX_COMPONENTTYPE *dec_handle,
                                       OMX_BUFFERHEADERTYPE  ***pBufHdrs,
                                       OMX_U32 nPortIndex,
                                       long bufCntMin, long bufSize)
{
    OMX_ERRORTYPE error = OMX_ErrorNone;

#ifdef SupportNative
    private_handle_t private_handle;
    OmxTestInfo_S *pOmxTestInfo = NULL;
    int index = 0;

    DEBUG_PRINT("Inside %s pBufHdrs = %p, nPortIndex = %d, bufCntMin = %ld, bufSize = %ld\n", __FUNCTION__, pBufHdrs, nPortIndex, bufCntMin, bufSize);

    for (index = 0; index < g_TestInstNum; index++)
    {
        if (OmxTestInfo[index].dec_handle == (OMX_COMPONENTTYPE *)dec_handle)
        {
            pOmxTestInfo = &OmxTestInfo[index];
            break;
        }
    }

    if (index == g_TestInstNum)
    {
        DEBUG_PRINT_ERROR("invalid parameter!\n");
        return OMX_ErrorBadParameter;
    }

    DEBUG_PRINT("pBufHdrs = %x,bufCntMin = %d\n", pBufHdrs, bufCntMin);
    *pBufHdrs= (OMX_BUFFERHEADERTYPE **)malloc(sizeof(OMX_BUFFERHEADERTYPE*)*bufCntMin);

    for(bufCnt=0; bufCnt < bufCntMin; ++bufCnt)
       {
        DEBUG_PRINT("OMX_UseBuffer No %d\n", bufCnt);
              HI_MMZ_Malloc(&pOmxTestInfo->buffer[bufCnt]);
              pOmxTestInfo->buffer[bufCnt].bufsize = (bufSize + ALIGN_SIZE - 1) & ~(ALIGN_SIZE - 1);
              pOmxTestInfo->buffer[bufCnt].bufsize += 0x40;
              if (0 == nPortIndex)
              {
                  strcpy(pOmxTestInfo->buffer[bufCnt].bufname, "OMX_VDEC_IN");
              }
              else
              {
                  strcpy(pOmxTestInfo->buffer[bufCnt].bufname, "OMX_VDEC_OUT");
              }
              private_handle.ion_phy_addr = pOmxTestInfo->buffer[bufCnt].phyaddr;
        error = OMX_UseBuffer(dec_handle, &((*pBufHdrs)[bufCnt]),
                               nPortIndex, NULL, bufSize, (OMX_U8*)&private_handle);
              (*pBufHdrs)[bufCnt]->nTickCount = bufCnt;  //暂时找个地方存一下物理地址
    }
#endif

    return error;
}


static int disable_output_port(OmxTestInfo_S * pOmxTestInfo)
{
       int q_flag = 0;
    OMX_BUFFERHEADERTYPE *pBuffer = NULL;

    DEBUG_PRINT("prepre to disable output port\n");

    // Send DISABLE command.
    pOmxTestInfo->sent_disabled = 1;

    OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandPortDisable, 1, 0);

    do
    {
       pthread_mutex_lock(&pOmxTestInfo->ftb_lock);
       pBuffer = (OMX_BUFFERHEADERTYPE *)pop(pOmxTestInfo->ftb_queue);
       pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);

       if (NULL != pBuffer)
       {
           if (0 == pOmxTestInfo->alloc_use_option)
           {
               OMX_FreeBuffer(pOmxTestInfo->dec_handle, 1, pBuffer);
           }
           else
           {
               OMX_FreeBuffer(pOmxTestInfo->dec_handle, 1, pBuffer);
               HI_MMZ_Free(&pOmxTestInfo->buffer[pBuffer->nTickCount]);
           }
       }
       else
       {
           q_flag = 1;
       }
    }while (!q_flag);

    wait_for_event(pOmxTestInfo, OMX_CommandPortDisable);

    if (pOmxTestInfo->currentStatus == ERROR_STATE)
    {
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    return 0;
}


static int enable_output_port(OmxTestInfo_S * pOmxTestInfo)
{
    OMX_U32 bufCnt = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    DEBUG_PRINT("prepare to enable output port\n");

    // Send Enable command
    OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandPortEnable, 1, 0);

    /* Allocate buffer on decoder's o/p port */
    pOmxTestInfo->portFmt.nPortIndex = 1;
    if (0 == pOmxTestInfo->alloc_use_option)
    {
        error = Allocate_Buffers(pOmxTestInfo, pOmxTestInfo->dec_handle, &pOmxTestInfo->pOutYUVBufHdrs, pOmxTestInfo->portFmt.nPortIndex, \
                                 pOmxTestInfo->portFmt.nBufferCountActual, pOmxTestInfo->portFmt.nBufferSize);

        if (error != OMX_ErrorNone)
        {
           DEBUG_PRINT_ERROR("OMX_AllocateBuffer Output buffer error\n");
           return -1;
        }
        else
        {
           DEBUG_PRINT("OMX_AllocateBuffer Output buffer success\n");
           pOmxTestInfo->free_op_buf_cnt = pOmxTestInfo->portFmt.nBufferCountActual;
        }
    }
    else
    {
        error = Use_Buffers(pOmxTestInfo->dec_handle, &pOmxTestInfo->pOutYUVBufHdrs, pOmxTestInfo->portFmt.nPortIndex,
                            pOmxTestInfo->portFmt.nBufferCountActual, pOmxTestInfo->portFmt.nBufferSize);

        if (error != OMX_ErrorNone)
        {
           DEBUG_PRINT_ERROR("OMX_UseBuffer Output buffer error\n");
           return -1;
        }
        else
        {
           DEBUG_PRINT("OMX_UseBuffer Output buffer success\n");
           pOmxTestInfo->free_op_buf_cnt = pOmxTestInfo->portFmt.nBufferCountActual;
        }
    }

    if (pOmxTestInfo->fbd_thread_exit)
    {
        DEBUG_PRINT("while enable_output_port meet fbd_thread_exit!\n");
        return 0;
    }

    // wait for enable event to come back
    DEBUG_PRINT("waiting enable done....\n");

    wait_for_event(pOmxTestInfo, OMX_CommandPortEnable);

    if (pOmxTestInfo->currentStatus == ERROR_STATE)
    {
        DEBUG_PRINT_ERROR("start error!\n");
        do_freeHandle_and_clean_up(pOmxTestInfo, true);
        return -1;
    }

    DEBUG_PRINT("wake up to fill buffers\n");

    for(bufCnt=0; bufCnt < pOmxTestInfo->portFmt.nBufferCountActual; ++bufCnt)
    {
        DEBUG_PRINT("OMX_FillThisBuffer on output buf no.%d\n",bufCnt);

        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nOutputPortIndex = 1;
        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nFlags &= ~OMX_BUFFERFLAG_EOS;

        ret = OMX_FillThisBuffer(pOmxTestInfo->dec_handle, pOmxTestInfo->pOutYUVBufHdrs[bufCnt]);
        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("OMX_FillThisBuffer failed, result %d\n", ret);
        }
        else
        {
            DEBUG_PRINT("OMX_FillThisBuffer success!\n");
            pOmxTestInfo->free_op_buf_cnt--;
        }
    }

    DEBUG_PRINT("OP PORT ENABLED!\n");
    return 0;
}


static int output_port_reconfig(OmxTestInfo_S * pOmxTestInfo)
{
    DEBUG_PRINT("PORT_SETTING_CHANGE_STATE\n");

    if (disable_output_port(pOmxTestInfo) != 0)
    {
        DEBUG_PRINT_ERROR("disable output port failed\n");
        return -1;
    }
    /* Port for which the client needs to obtain info */

    pOmxTestInfo->portFmt.nPortIndex = 1;

    OMX_GetParameter(pOmxTestInfo->dec_handle,OMX_IndexParamPortDefinition,&pOmxTestInfo->portFmt);

    DEBUG_PRINT("Min Buffer Count = %d\n", pOmxTestInfo->portFmt.nBufferCountMin);
    DEBUG_PRINT("Buffer Size = %d\n", pOmxTestInfo->portFmt.nBufferSize);

#if 0
    // for test change the buffer num in component
    DEBUG_PRINT_ALWS("Min Buffer Count = %d, Act Buffer Count = %d\n", pOmxTestInfo->portFmt.nBufferCountMin, pOmxTestInfo->portFmt.nBufferCountActual);
    DEBUG_PRINT_ALWS("nBufferSize = %d\n", pOmxTestInfo->portFmt.nBufferSize);
    pOmxTestInfo->portFmt.nBufferCountActual += 2;
    OMX_SetParameter(pOmxTestInfo->dec_handle,OMX_IndexParamPortDefinition, &pOmxTestInfo->portFmt);
#endif

    if(OMX_DirOutput != pOmxTestInfo->portFmt.eDir)
    {
        DEBUG_PRINT_ERROR("Error - Expect Output Port\n");
        return -1;
    }

    if (enable_output_port(pOmxTestInfo) != 0)
    {
        DEBUG_PRINT_ERROR("enable output port failed\n");
        return -1;
    }

    DEBUG_PRINT("PORT_SETTING_CHANGE DONE!\n");
    return 0;
}


static int seek_progress(OmxTestInfo_S * pOmxTestInfo)
{
    int bufCnt = 0;
    int frameSize = 0;
    OMX_BUFFERHEADERTYPE *pBuffer = NULL;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    DEBUG_PRINT("Enter seek_progress!\n");

    if (pOmxTestInfo->EtbStatus == THREAD_RUNNING || pOmxTestInfo->FtbStatus == THREAD_RUNNING)
    {
        DEBUG_PRINT_ERROR("EtbStatus/FtbStatus = THREAD_RUNNING, sleep for a while.\n");
        do
        {
            sleep(1);
        }while(pOmxTestInfo->EtbStatus == THREAD_RUNNING || pOmxTestInfo->FtbStatus == THREAD_RUNNING);

        DEBUG_PRINT_ERROR("EtbStatus&FtbStatus != THREAD_RUNNING, wake up.\n");
    }

    do {
        pthread_mutex_lock(&pOmxTestInfo->etb_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE *) pop(pOmxTestInfo->etb_queue);
        pthread_mutex_unlock(&pOmxTestInfo->etb_lock);
    }while(pBuffer != NULL);

    do {
        pthread_mutex_lock(&pOmxTestInfo->ftb_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE *) pop(pOmxTestInfo->ftb_queue);
        pthread_mutex_unlock(&pOmxTestInfo->ftb_lock);
    }while(pBuffer != NULL);

    rewind(pOmxTestInfo->inputBufferFileFd);
    pOmxTestInfo->frame_flag = 0;
    pOmxTestInfo->send_cnt   = 0;
    pOmxTestInfo->receive_frame_cnt = 0;
    pOmxTestInfo->bInputEosReached = false;

    if (pOmxTestInfo->used_op_buf_cnt != pOmxTestInfo->free_op_buf_cnt)
    {
        DEBUG_PRINT_ERROR("ERROR: seek_progress used_op_buf_cnt = %d, free_op_buf_cnt = %d!\n", pOmxTestInfo->used_op_buf_cnt, pOmxTestInfo->free_op_buf_cnt);
        sleep(3);
    }

    for(bufCnt = 0; bufCnt < pOmxTestInfo->used_op_buf_cnt; ++bufCnt)
    {
        DEBUG_PRINT("OMX_FillThisBuffer on output buf no.%d\n",bufCnt);

        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nOutputPortIndex = 1;
        pOmxTestInfo->pOutYUVBufHdrs[bufCnt]->nFlags &= ~OMX_BUFFERFLAG_EOS;

        ret = OMX_FillThisBuffer(pOmxTestInfo->dec_handle, pOmxTestInfo->pOutYUVBufHdrs[bufCnt]);

        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("Error - OMX_FillThisBuffer failed!!\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }
        else
        {
            pOmxTestInfo->free_op_buf_cnt--;
        }
    }

    if (pOmxTestInfo->used_ip_buf_cnt != pOmxTestInfo->free_ip_buf_cnt)
    {
        DEBUG_PRINT_ERROR("ERROR: seek_progress used_ip_buf_cnt = %d, free_ip_buf_cnt = %d!\n", pOmxTestInfo->used_ip_buf_cnt, pOmxTestInfo->free_ip_buf_cnt);
        sleep(3);
    }

    for (bufCnt = 0; bufCnt < pOmxTestInfo->used_ip_buf_cnt; bufCnt++)
    {
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nInputPortIndex = 0;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nOffset = 0;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nFlags = 0;

        frameSize = Read_Buffer_from_EsRawStream(pOmxTestInfo, pOmxTestInfo->pInputBufHdrs[bufCnt]);

        pOmxTestInfo->pInputBufHdrs[bufCnt]->nFilledLen = frameSize;
        pOmxTestInfo->pInputBufHdrs[bufCnt]->nInputPortIndex = 0;

        DEBUG_PRINT("%s: Timestamp sent(%lld)\n", __func__, pOmxTestInfo->pInputBufHdrs[bufCnt]->nTimeStamp);

        ret = OMX_EmptyThisBuffer(pOmxTestInfo->dec_handle, pOmxTestInfo->pInputBufHdrs[bufCnt]);
        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("ERROR: OMX_EmptyThisBuffer failed\n");
            do_freeHandle_and_clean_up(pOmxTestInfo, true);
            return -1;
        }
        else
        {
            pOmxTestInfo->free_ip_buf_cnt--;
        }

        if (pOmxTestInfo->pInputBufHdrs[bufCnt]->nFlags & OMX_BUFFERFLAG_EOS)
        {
            pOmxTestInfo->bInputEosReached = true;
            break;
        }
    }

    DEBUG_PRINT("SEEK PROGRESS DONE!\n");
    return 0;
}

static void do_freeHandle_and_clean_up(OmxTestInfo_S * pOmxTestInfo, int isDueToError)
{
    int bufCnt = 0;
    OMX_STATETYPE state = OMX_StateLoaded;
    OMX_ERRORTYPE result = OMX_ErrorNone;

    DEBUG_PRINT("Inst %d ready to quit.\n", pOmxTestInfo->inst_no);

    pOmxTestInfo->ebd_thread_exit = 1;
    pOmxTestInfo->fbd_thread_exit = 1;
    sem_post(&pOmxTestInfo->etb_sem);
    sem_post(&pOmxTestInfo->ftb_sem);

    OMX_GetState(pOmxTestInfo->dec_handle, &state);

    pOmxTestInfo->flush_input_progress = 1;
    pOmxTestInfo->flush_output_progress = 1;
    OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandFlush, OMX_ALL, 0);
    wait_for_event(pOmxTestInfo, OMX_CommandFlush);

    DEBUG_PRINT("Flush All done.\n");

    if (pOmxTestInfo->currentStatus == PORT_SETTING_RECONFIG_STATE)
    {
        DEBUG_PRINT_ERROR("currentStatus = PORT_SETTING_RECONFIG_STATE, sleep for a while.\n");
        do {
            sleep(1);
        }while(pOmxTestInfo->currentStatus == PORT_SETTING_RECONFIG_STATE);
        DEBUG_PRINT_ERROR("currentStatus != PORT_SETTING_RECONFIG_STATE, wake up.\n");
    }

    if (state == OMX_StateExecuting || state == OMX_StatePause)
    {
        DEBUG_PRINT("Requesting transition to Idle\n");
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StateIdle, 0);
        wait_for_event(pOmxTestInfo, OMX_CommandStateSet);

        DEBUG_PRINT("In Idle state.\n");

        OMX_GetState(pOmxTestInfo->dec_handle, &state);
        if (state != OMX_StateIdle)
        {
            DEBUG_PRINT_ERROR("current component state :%d error!\n", state);
        }

        DEBUG_PRINT("current component state :%d\n", state);
    }

    if (state == OMX_StateIdle)
    {
        DEBUG_PRINT("Requesting transition to Loaded\n");
        OMX_SendCommand(pOmxTestInfo->dec_handle, OMX_CommandStateSet, OMX_StateLoaded, 0);

        for(bufCnt=0; bufCnt < pOmxTestInfo->used_ip_buf_cnt; ++bufCnt)
        {
            OMX_FreeBuffer(pOmxTestInfo->dec_handle, 0, pOmxTestInfo->pInputBufHdrs[bufCnt]);
        }

        if (pOmxTestInfo->pInputBufHdrs)
        {
            free(pOmxTestInfo->pInputBufHdrs);
            pOmxTestInfo->pInputBufHdrs = NULL;
        }

        DEBUG_PRINT("free ip buffer ok!\n");

        for(bufCnt = 0; bufCnt < pOmxTestInfo->used_op_buf_cnt; ++bufCnt)
        {
             OMX_FreeBuffer(pOmxTestInfo->dec_handle, 1, pOmxTestInfo->pOutYUVBufHdrs[bufCnt]);
        }

        if (pOmxTestInfo->pOutYUVBufHdrs)
        {
            free(pOmxTestInfo->pOutYUVBufHdrs);
            pOmxTestInfo->pOutYUVBufHdrs = NULL;
        }

        pOmxTestInfo->free_op_buf_cnt = 0;

        DEBUG_PRINT("free op buffer ok!\n");

        wait_for_event(pOmxTestInfo, OMX_CommandStateSet);

        DEBUG_PRINT("In Loaded state.\n");

        OMX_GetState(pOmxTestInfo->dec_handle, &state);
        if (state != OMX_StateLoaded)
        {
            DEBUG_PRINT_ERROR("current component state :%d error!\n", state);
        }

              if (pOmxTestInfo->alloc_use_option)
              {
                 for(bufCnt=0; bufCnt < pOmxTestInfo->used_op_buf_cnt; ++bufCnt)
                 {
                     HI_MMZ_Free(&pOmxTestInfo->buffer[bufCnt]);
                 }
              }

        DEBUG_PRINT("current component state :%d\n", state);

    }

    DEBUG_PRINT("[OMX Vdec Test] - Free omx handle start \n");

    result = OMX_FreeHandle(pOmxTestInfo->dec_handle);
    if (result != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("OMX_FreeHandle error. Error code: %x\n", result);
    }

    DEBUG_PRINT("[OMX Vdec Test] - Free omx handle end !!\n");

    pOmxTestInfo->dec_handle = NULL;

    /* Deinit OpenMAX */
    DEBUG_PRINT("De-initializing OMX \n");
    OMX_Deinit();

    DEBUG_PRINT(" closing all files \n");
    if(pOmxTestInfo->inputBufferFileFd != NULL)
    {
        fclose(pOmxTestInfo->inputBufferFileFd);
        pOmxTestInfo->inputBufferFileFd = NULL;
    }
    DEBUG_PRINT(" after free inputfile \n");

    if(pOmxTestInfo->etb_queue)
    {
        free_queue(pOmxTestInfo->etb_queue);
        pOmxTestInfo->etb_queue = NULL;
    }

    DEBUG_PRINT("after free etb_queue\n");
    if(pOmxTestInfo->ftb_queue)
    {
        free_queue(pOmxTestInfo->ftb_queue);
        pOmxTestInfo->ftb_queue = NULL;
    }
    DEBUG_PRINT("after free ftb_queue\n");

#ifdef HI_OMX_TEE_SUPPORT
    if (1 == pOmxTestInfo->tvp_option)
    {
        HI_MPI_MMZ_Free(&pOmxTestInfo->pCAStreamBuf);

        DeInitSecEnvironment();
    }
#endif

    pthread_join(pOmxTestInfo->ebd_thread_id, NULL);

    DEBUG_PRINT("after join ebd thread\n");

    pthread_join(pOmxTestInfo->fbd_thread_id, NULL);

    DEBUG_PRINT("after join fbd thread\n");

    pthread_cond_destroy(&pOmxTestInfo->event_cond);

    pthread_mutex_destroy(&pOmxTestInfo->event_lock);
    pthread_mutex_destroy(&pOmxTestInfo->etb_lock);
    pthread_mutex_destroy(&pOmxTestInfo->ftb_lock);

    DEBUG_PRINT("after destroy mutex\n");

    if (-1 == sem_destroy(&pOmxTestInfo->etb_sem))
    {
        DEBUG_PRINT_ERROR("L%d Error - sem_destroy failed %d\n", __LINE__, errno);
    }

    if (-1 == sem_destroy(&pOmxTestInfo->ftb_sem))
    {
        DEBUG_PRINT_ERROR("L%d Error - sem_destroy failed %d\n", __LINE__, errno);
    }

    if (-1 == sem_destroy(&pOmxTestInfo->in_flush_sem))
    {
        DEBUG_PRINT_ERROR("L%d Error - sem_destroy failed %d\n", __LINE__, errno);
    }

    if (-1 == sem_destroy(&pOmxTestInfo->out_flush_sem))
    {
        DEBUG_PRINT_ERROR("L%d Error - sem_destroy failed %d\n", __LINE__, errno);
    }

    if (-1 == sem_destroy(&pOmxTestInfo->seek_sem))
    {
        DEBUG_PRINT_ERROR("L%d Error - sem_destroy failed %d\n", __LINE__, errno);
    }

    close((long)pOmxTestInfo->in_filename);

    pOmxTestInfo->is_open = 0;

    DEBUG_PRINT_ALWS("*********************************************\n");

    if (isDueToError)
        DEBUG_PRINT_ALWS("**************...TEST FAILED...**************\n");
    else
        DEBUG_PRINT_ALWS("************...TEST SUCCESSFULL...***********\n");

    DEBUG_PRINT_ALWS("*********************************************\n\n\n");

    return;
}

