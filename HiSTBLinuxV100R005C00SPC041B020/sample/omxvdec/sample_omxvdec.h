/*--------------------------------------------------------------------------
Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Code Aurora nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--------------------------------------------------------------------------*/
#ifndef SAMPLE_OMX_H
#define SAMPLE_OMX_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
//#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/time.h>
//#include <linux/android_pmem.h>

#include <signal.h>

#include "OMX_Core.h"
#include "OMX_Component.h"
#include "OMX_IVCommon.h"
#include "OMX_VideoExt.h"
#include "OMX_IndexExt.h"
#include "hi_common.h"

#include "sample_tidx.h"

#define MAX_INST_NUM 20

//#define CalcFrameRate

typedef enum {
    CODEC_FORMAT_H264 = 0x0,
    CODEC_FORMAT_MP2,
    CODEC_FORMAT_MP4,
    CODEC_FORMAT_H263,
    CODEC_FORMAT_SORENSON,
    CODEC_FORMAT_AVS,
    CODEC_FORMAT_VC1AP,
    CODEC_FORMAT_VC1SMP,
    CODEC_FORMAT_REAL8,
    CODEC_FORMAT_REAL9,
    CODEC_FORMAT_DIVX3,
    CODEC_FORMAT_VP6,
    CODEC_FORMAT_VP6F,
    CODEC_FORMAT_VP6A,
    CODEC_FORMAT_VP8,
    CODEC_FORMAT_MJPEG,
    CODEC_FORMAT_MP1,
    CODEC_FORMAT_HEVC,
    CODEC_FORMAT_MVC,
    CODEC_FORMAT_VP9,

    CODEC_FORMAT_MAX,
} codec_format;

typedef enum {
    GOOD_STATE = 0,
    PORT_SETTING_CHANGE_STATE,
    PORT_SETTING_RECONFIG_STATE,
    ERROR_STATE
} inst_status;

typedef enum {
    THREAD_INVALID = 0,
    THREAD_WAITING,
    THREAD_RUNNING,
    THREAD_BUTT
} thread_status;

typedef enum {
    FREE_HANDLE_AT_LOADED = 1,
    FREE_HANDLE_AT_IDLE,
    FREE_HANDLE_AT_EXECUTING,
    FREE_HANDLE_AT_PAUSE
} freeHandle_test;

enum {
    false    =    0,
    true    =    1
};

struct buf_info {
    int index;
    unsigned int length;
    char *start;
};

typedef int    bool;
typedef OMX_U8* OMX_U8_PTR;

typedef struct
{
    int inst_no;
    int is_open;
    int event_is_done;

    FILE  *inputBufferFileFd;

    Queue *etb_queue;
    Queue *ftb_queue;

    pthread_t ebd_thread_id;
    pthread_t fbd_thread_id;

    //MUTEXT
    pthread_mutex_t    etb_lock;
    pthread_mutex_t    ftb_lock;
    pthread_mutex_t    event_lock;
    pthread_cond_t    event_cond;

    //SEMA
    sem_t etb_sem;
    sem_t ftb_sem;
    sem_t in_flush_sem;
    sem_t out_flush_sem;
    sem_t seek_sem;

    //OMX VAR
    OMX_PARAM_COMPONENTROLETYPE  role;
    OMX_PARAM_PORTDEFINITIONTYPE portFmt;
    OMX_PORT_PARAM_TYPE portParam;

    OMX_COLOR_FORMATTYPE color_fmt;
    //OMX_S64 timeStampLfile;
    OMX_COMPONENTTYPE *dec_handle;
    OMX_BUFFERHEADERTYPE **pInputBufHdrs;
    OMX_BUFFERHEADERTYPE **pOutYUVBufHdrs;

    int height;
    int width;
    int sliceheight;
    int stride;

    int used_ip_buf_cnt;
    int used_op_buf_cnt;
    int free_ip_buf_cnt;
    int free_op_buf_cnt;
    int receive_frame_cnt;

    int bInputEosReached;
    int bOutputEosReached;
    int flush_input_progress;
    int flush_output_progress;
    int seeking_progress;
    int fbd_thread_exit;
    int ebd_thread_exit;

    char in_filename[512];

    int test_option;
    int alloc_use_option;
    int sent_disabled;
    codec_format codec_format_option;
    freeHandle_test freeHandle_option;

    inst_status currentStatus;
    inst_status preStatus;
    thread_status  EtbStatus;
    thread_status  FtbStatus;

    bool frame_in_packet;
    bool readsize_add_in_stream;

    int last_cmd;
       int start_time;
    int stop_time;
    int frame_flag;
    int send_cnt;
    unsigned long frame_len;
    unsigned timestampInterval;

    int tvp_option;
    HI_MMZ_BUF_S  pCAStreamBuf;
    HI_MMZ_BUF_S buffer[10];
    STR_CONTEXT_S stContext;

    struct timeval t_first_send;
    struct timeval t_last_get;
    struct timeval t_cur_get;
}OmxTestInfo_S;


/************************************************************************/
/*              GLOBAL FUNC DECL                        */
/************************************************************************/
static void* ebd_thread(void*);
static void* fbd_thread(void*);
static int disable_output_port(OmxTestInfo_S *);
static int enable_output_port(OmxTestInfo_S *);
static int output_port_reconfig(OmxTestInfo_S *);
static int seek_progress(OmxTestInfo_S *);
static int Init_Decoder(OmxTestInfo_S *);
static int Play_Decoder(OmxTestInfo_S *);
static int open_video_file (OmxTestInfo_S *);
static void loop_function(void);
static void exit_loop(OmxTestInfo_S *);
static void Init_OmxInst(void);
#ifdef CalcFrameRate
static int GetTimeInMs(void);
#endif

static OMX_ERRORTYPE Allocate_Buffers(OmxTestInfo_S * pOmxTestInfo,
        OMX_COMPONENTTYPE *dec_handle,
        OMX_BUFFERHEADERTYPE  ***pBufHdrs,
        OMX_U32 nPortIndex,
        long bufCntMin,
        long bufSize);

static OMX_ERRORTYPE Use_Buffers(OMX_COMPONENTTYPE *dec_handle,
        OMX_BUFFERHEADERTYPE  ***pBufHdrs,
        OMX_U32 nPortIndex,
        long bufCntMin,
        long bufSize);

static OMX_ERRORTYPE EventHandler(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_EVENTTYPE eEvent,
        OMX_IN OMX_U32 nData1,
        OMX_IN OMX_U32 nData2,
        OMX_IN OMX_PTR pEventData);

static OMX_ERRORTYPE EmptyBufferDone(OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);

static OMX_ERRORTYPE FillBufferDone(OMX_OUT OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_PTR pAppData,
        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

static void do_freeHandle_and_clean_up(OmxTestInfo_S *pOmxTestInfo, int isDueToError);

static inline int clip2(int x)
{
    x = x - 1;
    x = x | x >> 1;
    x = x | x >> 2;
    x = x | x >> 4;
    x = x | x >> 16;
    x = x + 1;
    return x;
}



#endif
