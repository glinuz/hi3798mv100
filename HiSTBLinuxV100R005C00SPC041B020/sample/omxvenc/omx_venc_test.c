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
/*
    An Open max test application ....
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h> //gettimeofday
#include <linux/videodev2.h>
#include <linux/fb.h>
#include "OMX_Core.h"
#include "OMX_Component.h"
#include "OMX_IVCommon.h"

#include "queue.h"
#include "hi_type.h"
/************************************************************************/

//#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT   printf
#else
#define DEBUG_PRINT
#endif

#define DEBUG_PRINT_ERROR printf

#define OMX_COMPONENTROLES_H264           "video_encoder.avc"
#define OMX_COMPONENTROLES_Hevc           "video_encoder.hevc"

#define OMX_COMPONENTNAME_H264   "OMX.hisi.video.encoder.avc"
#define OMX_COMPONENTNAME_Hevc   "OMX.hisi.video.encoder.hevc"


/************************************************************************/
#define FAILED(result) (result != OMX_ErrorNone)
#define SUCCEEDED(result) (result == OMX_ErrorNone)

#define DEFAULT_WIDTH	1920
#define DEFAULT_HEIGHT	1080
#define DEFAULT_FRMERATE 30
/************************************************************************/
//* OMX Spec Version supported by the wrappers. Version = 1.1 */
const OMX_U32 CURRENT_OMX_SPEC_VERSION = 0x00000101;

#define CONFIG_VERSION_SIZE(param) \
    param.nVersion.nVersion = CURRENT_OMX_SPEC_VERSION;\
    param.nSize = sizeof(param);
/************************************************************************/
typedef enum
{
    GOOD_STATE = 0,
    PORT_SETTING_CHANGE_STATE,
    ERROR_STATE
} test_status;

typedef enum
{
    CODEC_FORMAT_H264	= 0x0,
    CODEC_FORMAT_H263	= 0x1,
    CODEC_FORMAT_MP4	= 0x2,
    CODEC_FORMAT_MAX
} codec_format;

typedef enum
{
    false	=	0,
    true	=	1
} bool;

enum
{
    INPUT_PORT_INDEX	= 0,
    OUTPUT_PORT_INDEX	= 1
};

enum
{
    OMX_V_U	            = 0,
    OMX_U_V	            = 1
};

enum
{
    YUV_SEMIPLANNER_420_UV	= 0,
    YUV_SEMIPLANNER_420_VU	= 1,
    YUV_PLANNER_420	        = 2,
    YUV_PACKAGE_422_YUYV    = 3,
    YUV_PACKAGE_422_YVYU    = 4,
    YUV_PACKAGE_422_UYVY    = 5,
    //YUV_SEMIPLANNER_422_UV	= 6,
    //YUV_SEMIPLANNER_422_VU	= 7,
    YUV_UNKNOW_BUIT
};

#define SAMPLE_VEDU_UY0VY1 0x8d
#define SAMPLE_VEDU_Y0UY1V 0xd8
#define SAMPLE_VEDU_Y0VY1U 0x78

#define SAMPLE_VEDU_YUV420       0
#define SAMPLE_VEDU_YUV422       1
#define SAMPLE_VEDU_YUV444       2

#define SAMPLE_VEDU_SEMIPLANNAR  0
#define SAMPLE_VEDU_PACKAGE      1
#define SAMPLE_VEDU_PLANNAR      2


struct buf_info
{
    int index;
    unsigned int length;
    char* start;
};
typedef OMX_U8* OMX_U8_PTR;
static int (*Read_Buffer)(OMX_BUFFERHEADERTYPE*  pBufHdr );
/************************************************************************/
volatile int event_is_done = 0;

//static int inputBufferFileFd;
static FILE* inputBufferFile;

static FILE* outputBufferFile;
static int take264Log = 0;
static int test_option = 0;
static int YUV_store_type = 0;
static Queue* etb_queue = NULL;
static Queue* fbd_queue = NULL;

static pthread_t ebd_thread_id;
static pthread_t fbd_thread_id;

//MUTEXT
static pthread_mutex_t	etb_lock;        /*empty_this_buffer lock*/
static pthread_mutex_t	fbd_lock;        /*fill_this_buffer lock*/
static pthread_mutex_t	lock;
static pthread_cond_t	cond;
static pthread_mutex_t	eos_lock;
static pthread_cond_t   eos_cond;
static pthread_mutex_t  enable_lock;

//SEMA
static sem_t etb_sem;
static sem_t fbd_sem;
static sem_t in_flush_sem;
static sem_t out_flush_sem;

//OMX VAR
OMX_PARAM_COMPONENTROLETYPE  role;
OMX_PARAM_PORTDEFINITIONTYPE portFmt;
OMX_PORT_PARAM_TYPE portParam;
OMX_ERRORTYPE error;
OMX_COLOR_FORMATTYPE color_fmt;
OMX_COMPONENTTYPE* venc_handle = 0;
OMX_BUFFERHEADERTYPE** pInputYUVBufHdrs = NULL;
OMX_BUFFERHEADERTYPE** pOutputBufHdrs = NULL;
static int height = 0;
static int width = 0;
static int CodeType = 0; /*0:H264 ; 1:HEVC*/
static int sliceheight = 0;
static int stride = 0;
static int used_ip_buf_cnt = 0;
static int used_op_buf_cnt = 0;
static int ebd_cnt = 0;
static int fbd_cnt = 0;
static int bInputEosReached = 0;
static int bOutputEosReached = 0;
static int flush_input_progress = 0;
static int flush_output_progress = 0;
static int fbd_thread_exit = 0;
static int ebd_thread_exit = 0;

static unsigned cmd_data = ~(unsigned)0;
static unsigned etb_count = 0;
static unsigned free_op_buf_cnt = 0;
static unsigned free_ip_buf_cnt = 0;

static char in_filename[512];
static char Input_Filename_table[128];
static char curr_seq_command[512];

static int fps = 30;
static unsigned int timestampInterval = 33333;
static codec_format codec_format_option;
//static freeHandle_test freeHandle_option;
static int etb_disabled = 0;
static int ftb_disabled = 0;
static int waitForPortSettingsChanged = 1;
static test_status currentStatus = GOOD_STATE;

#if 0
static int vd_fd;
struct v4l2_requestbuffers reqbuf;
struct v4l2_format format;
struct v4l2_buffer buf;
#endif

struct timeval t_first = {0, 0};

#define OMXVENC_H264 (0)
#define OMXVENC_H265 (1)
#define OMX_COMMPONT_NAME_H264    "OMX.hisi.video.encoder.avc"
#define OMX_COMMPONT_NAME_HEVC    "OMX.hisi.video.encoder.hevc"

#define ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )
/************************************************************************/
/*              GLOBAL FUNC DECL                                        */
/************************************************************************/
static void* ebd_thread(void*);
static void* fbd_thread(void*);
static int disable_output_port(void);
static int enable_output_port(void);
static int output_port_reconfig(void);
static void free_output_buffers(void);
static int Init_Encoder(void);
static int Play_Encoder(void);
static int open_video_file (void);
static void loop_function(void);

static OMX_ERRORTYPE Allocate_Buffers ( OMX_COMPONENTTYPE* venc_handle,
                                        OMX_BUFFERHEADERTYPE**  *pBufHdrs,
                                        OMX_U32 nPortIndex,
                                        long bufCntMin, long bufSize);

static OMX_ERRORTYPE EventHandler(OMX_IN OMX_HANDLETYPE hComponent,
                                  OMX_IN OMX_PTR pAppData,
                                  OMX_IN OMX_EVENTTYPE eEvent,
                                  OMX_IN OMX_U32 nData1, OMX_IN OMX_U32 nData2,
                                  OMX_IN OMX_PTR pEventData);

static OMX_ERRORTYPE EmptyBufferDone(OMX_IN OMX_HANDLETYPE hComponent,
                                     OMX_IN OMX_PTR pAppData,
                                     OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);

static OMX_ERRORTYPE FillBufferDone(OMX_OUT OMX_HANDLETYPE hComponent,
                                    OMX_OUT OMX_PTR pAppData,
                                    OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

static void do_freeHandle_and_clean_up(bool isDueToError);

static int last_cmd;
static void wait_for_event(int cmd)
{
    pthread_mutex_lock(&lock);

    while (1)
    {
        if ((event_is_done == 1) && (last_cmd == cmd))
        {
            event_is_done = 0;
            break;
        }

        pthread_cond_wait(&cond, &lock);

    }

    pthread_mutex_unlock(&lock);
}

static void event_complete(int cmd)
{
    pthread_mutex_lock(&lock);

    if (event_is_done == 0)
    { event_is_done = 1; }

    pthread_cond_broadcast(&cond);
    DEBUG_PRINT("\n\nevent_complete :: cmd =%d\n", cmd);
    last_cmd = cmd;
    pthread_mutex_unlock(&lock);
}

#define CHECK_EQ(X,Y)\
    do{\
        if (X != Y)\
        {\
            DEBUG_PRINT_ERROR("CHECK_EQ error! %s %d\n",__func__,__LINE__);\
            return -1;\
        }\
    }while(0)

#define CHECK_RET(ret)\
    do{\
        if (ret != 0)\
        {\
            DEBUG_PRINT_ERROR("CHECK_RET error! %s %d\n",__func__,__LINE__);\
            return -1;\
        }\
    }while(0)

typedef struct
{
    OMX_U32 mProfile;
    OMX_U32 mLevel;
} CodecProfileLevel;

static int getVideoProfileLevel(CodecProfileLevel* defaultProfileLevel, CodecProfileLevel* profileLevel)
{
    int  profile, level;
    int  totalMB = 0;
    int  ret = 0;
    OMX_VIDEO_PARAM_PROFILELEVELTYPE param;
    param.nPortIndex = OUTPUT_PORT_INDEX;

    DEBUG_PRINT("Default profile: %ld, level %ld\n", defaultProfileLevel->mProfile, defaultProfileLevel->mLevel);

    // Are the default profile and level overwriten?
    profile = OMX_VIDEO_AVCProfileBaseline;
    totalMB = (width * height) / 256;

    if (totalMB <= 99)
    {
        level = OMX_VIDEO_AVCLevel1;
    }
    else if (totalMB <= 396)
    {
        level = OMX_VIDEO_AVCLevel2;
    }
    else if (totalMB <= 792)
    {
        level = OMX_VIDEO_AVCLevel21;
    }
    else if (totalMB <= 1620)
    {
        level = OMX_VIDEO_AVCLevel3;
    }
    else if (totalMB <= 3600)
    {
        level = OMX_VIDEO_AVCLevel31;
    }
    else if (totalMB <= 5120)
    {
        level = OMX_VIDEO_AVCLevel32;
    }
    else if (totalMB <= 8192)
    {
        level = OMX_VIDEO_AVCLevel41;
    }

    DEBUG_PRINT("Target profile: %d, level: %d\n", profile, level);

    // Are the target profile and level supported by the encoder?
    for (param.nIndex = 0;; ++param.nIndex)
    {
        ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoProfileLevelQuerySupported, &param);

        if (ret) { break; }

        int supportedProfile = (param.eProfile);
        int supportedLevel   = (param.eLevel);
        DEBUG_PRINT("Supported profile: %d, level %d,index = %d,%s,%d,\n", supportedProfile, supportedLevel, param.nIndex, __func__, __LINE__);

        if (profile == supportedProfile && level <= supportedLevel)
        {
            profileLevel->mProfile = profile;
            profileLevel->mLevel = level;
            return 0;
        }
    }

    DEBUG_PRINT_ERROR("Target profile (%d) and level (%d) is not supported\n", profile, level);
    return -1;
}

/*************************************************************************
				file operation functions
************************************************************************/

static int open_video_file ()
{
    int error_code = 0;

    DEBUG_PRINT("Inside %s filename=%s\n",
                __FUNCTION__, in_filename);

#if 0
    inputBufferFileFd = open(in_filename, O_RDONLY);

    if (inputBufferFileFd < 0)
    {
        DEBUG_PRINT_ERROR("Error: i/p file %s open failed!\n", in_filename);
        error_code = -1;
    }

#else
    inputBufferFile = fopen (in_filename, "rb");

    if (inputBufferFile == NULL)
    {
        DEBUG_PRINT_ERROR("Error: i/p file %s open failed!\n", in_filename);
        error_code = -1;
    }

#endif

    if (take264Log)
    {
        switch (CodeType)
        {
            case OMXVENC_H264:
               outputBufferFile = fopen ("./venc_test.h264", "wb+");
               break;
            case OMXVENC_H265:
               outputBufferFile = fopen ("./venc_test.h265", "wb+");
               break;
            default:
               DEBUG_PRINT_ERROR("ERROR: CodeType = %d not support\n",CodeType);
               break;
        }
        

        if (outputBufferFile == NULL)
        {
            DEBUG_PRINT_ERROR("ERROR: o/p file open failed\n");
            error_code = -1;
        }
    }

    return error_code;
}

#if 0
static  int SampleTransFormat(unsigned char* srcData , unsigned width, unsigned height, unsigned mode)
{
    unsigned char* olddata;
    olddata = ( unsigned char*)malloc(width * height * 2 * sizeof(unsigned char));

    if (NULL == olddata) { return 1; }

    long i, temp;
    long offset = 0;

    switch (mode)
    {
        case 0:  //planer420->sp420
            memcpy(olddata, srcData , width * height * 3 / 2);
            offset = width * height * 3 / 2;

            for (i = width * height; i < offset; i = i + 2)
            {
                temp = width * height + (i - width * height) / 2;
                *(srcData + i + 1) = *(olddata + temp);
                temp = width * height + (i - width * height) / 2 + width * height / 4;
                *(srcData + i + 0) = *(olddata + temp);
            }

            free(olddata);
            break;

        case 1:  //planer420->planer420
            memcpy(olddata, srcData , width * height * 3 / 2);
            offset = width * height + width * height / 4;

            for (i = width * height; i < offset ; i++)
            {
                temp = width * height + (i - width * height) / 2;
                *(srcData + i + 1) = *(olddata + temp);
                temp = width * height + (i - width * height) / 2 + width * height / 4;
                *(srcData + i + 0) = *(olddata + temp);
            }

            free(olddata);
            break;

        default:
            break;
    }

    return 0;
}
#endif


static int g_pts = 0;

static int yuvfile_to_memory( FILE*  fpYUV,
                              HI_U8* yAddr,      /* only by plannar, semiplannar, package */
                              HI_U8* cAddr,      /* only by plannar, semiplannar */
                              HI_U8* vAddr,      /* only by plannar */
                              int    yuvSample,  /* 420 422 444 */
                              int    yuvStore,   /* semiplannar, plannar, package */
                              int    w,
                              int    h,
                              int    ystride,     /* only by semiplannar, plannar, package */
                              int    cstride,     /* only by semiplannar, plannar */
                              int    sel)
{
#define ERROR_READ { printf("error:read yuv\n"); return HI_FAILURE; }

    int    wY = w, wC, i;
    int    hY = h, hC, j;

    HI_U8* pY = yAddr;
    HI_U8* pU = cAddr;
    HI_U8* pV = vAddr, *pUV;

    if      ( yuvSample == SAMPLE_VEDU_YUV420 ) { wC = wY >> 1, hC = hY >> 1; }
    else if ( yuvSample == SAMPLE_VEDU_YUV422 ) { wC = wY >> 1, hC = hY >> 0; }
    else if ( yuvSample == SAMPLE_VEDU_YUV444 ) { wC = wY >> 0, hC = hY >> 0; }
    else    { return HI_FAILURE; }

    if ( yuvStore == SAMPLE_VEDU_PLANNAR )
    {
        for ( j = 0; j < hY; j++, pY += ystride ) { if ( 1 != fread(pY, wY, 1, fpYUV) ) { ERROR_READ; } }

        for ( j = 0; j < hC; j++, pU += cstride ) { if ( 1 != fread(pU, wC, 1, fpYUV) ) { ERROR_READ; } }

        for ( j = 0; j < hC; j++, pV += cstride ) { if ( 1 != fread(pV, wC, 1, fpYUV) ) { ERROR_READ; } }
    }
    else if ( yuvStore == SAMPLE_VEDU_SEMIPLANNAR )
    {
        int t0 = sel & 1;
        int t1 = 1 - t0;

        if ( NULL == (pUV = (HI_U8*)malloc(cstride)) )
        {
            printf("error: malloc @ read yuv\n");
            return HI_FAILURE;
        }

        pV = cAddr + cstride / 2;

        for ( j = 0; j < hY; j++, pY += ystride ) { if ( 1 != fread(pY, wY, 1, fpYUV) ) { ERROR_READ; } }

        for ( j = 0; j < hC; j++, pU += cstride ) { if ( 1 != fread(pU, wC, 1, fpYUV) ) { ERROR_READ; } }

        for ( j = 0; j < hC; j++, pV += cstride ) { if ( 1 != fread(pV, wC, 1, fpYUV) ) { ERROR_READ; } }

        pU = cAddr;
        pV = cAddr + cstride / 2;

        for (j = 0; j < hC; j++, pU += cstride, pV += cstride)
        {
            for ( i = 0; i < wC; i++ )
            {
                pUV[i * 2 + t0] = *(pV + i);
                pUV[i * 2 + t1] = *(pU + i);
            }

            memcpy(pU, pUV, 2 * wC);
        }

        free(pUV);
    }
    else if ( yuvStore == SAMPLE_VEDU_PACKAGE )
    {
        int y0sel = (sel >> 0) & 3;
        int y1sel = (sel >> 2) & 3;
        int u0sel = (sel >> 4) & 3;
        int v0sel = (sel >> 6) & 3;

        if ( NULL == (pUV = (HI_U8*)malloc(wY)) )
        {
            printf("error: malloc @ read yuv\n");
            return HI_FAILURE;
        }

        if ( yuvSample == SAMPLE_VEDU_YUV420 )
        {
            for ( j = 0; j < hY; j++, pY += ystride )
            {
                if ( 1 != fread(pUV, wY, 1, fpYUV) ) { ERROR_READ; }

                for ( i = wY / 2 - 1; i >= 0; i-- )
                {
                    *(pY + i * 4 + y0sel) = *(pUV + i * 2    );
                    *(pY + i * 4 + y1sel) = *(pUV + i * 2 + 1);
                }
            }

            pU = yAddr + u0sel;

            for ( j = 0; j < hC; j++, pU += ystride * 2 )
            {
                if ( 1 != fread(pUV, wC, 1, fpYUV) ) { ERROR_READ; }

                for ( i = wC - 1; i >= 0; i-- )
                {
                    *(pU + i * 4) = *(pUV + i);
                }
            }

            pV = yAddr + v0sel;

            for ( j = 0; j < hC; j++, pV += ystride * 2 )
            {
                if ( 1 != fread(pUV, wC, 1, fpYUV) ) { ERROR_READ; }

                for ( i = wC - 1; i >= 0; i-- )
                {
                    *(pV + i * 4) = *(pUV + i);
                }
            }
        }
        else
        {
            //int t = ( yuvSample == VEDU_YUV444 ? 4 : 2 );

            for ( j = 0; j < hY; j++, pY += ystride )
            {
                if ( 1 != fread(pUV, wY, 1, fpYUV) ) { ERROR_READ; }

                if ( yuvSample == SAMPLE_VEDU_YUV422 )
                    for ( i = wY / 2 - 1; i >= 0; i-- )
                    {
                        *(pY + i * 4 + y0sel) = *(pUV + i * 2    );
                        *(pY + i * 4 + y1sel) = *(pUV + i * 2 + 1);
                    }
                else
                    for ( i = wY - 1; i >= 0; i-- )
                    {
                        *(pY + i * 4 + y0sel) = *(pUV + i);
                    }
            }

            /*
                        pU = yAddr + u0sel;
                        for( j = 0; j < hC; j++, pU += ystride )
                        {
                            if( 1 != fread(pUV, wC, 1, fpYUV) ) ERROR_READ;

                            for( i = wC - 1; i >= 0; i-- )
                            {
                                *(pU + i * 4) = *(pUV + i);
                            }
                        }

                        pV = yAddr + v0sel;
                        for( j = 0; j < hC; j++, pV += ystride )
                        {
                            if( 1 != fread(pUV, wC, 1, fpYUV) ) ERROR_READ;

                            for( i = wC - 1; i >= 0; i-- )
                            {
                                *(pV + i * 4) = *(pUV + i);
                            }
                        }
            */
            pU = yAddr + u0sel;

            for ( j = 0; j < hC; j++, pU += ystride )
            {
                if (j % 2 == 0)
                {
                    if ( 1 != fread(pUV, wC, 1, fpYUV) ) { ERROR_READ; }
                }

                for ( i = wC - 1; i >= 0; i-- )
                {
                    *(pU + i * 4) = *(pUV + i);
                }
            }

            pV = yAddr + v0sel;

            for ( j = 0; j < hC; j++, pV += ystride )
            {
                if (j % 2 == 0)
                {
                    if ( 1 != fread(pUV, wC, 1, fpYUV) ) { ERROR_READ; }
                }

                for ( i = wC - 1; i >= 0; i-- )
                {
                    *(pV + i * 4) = *(pUV + i);
                }
            }
        }


        free(pUV);
    }
    else { return HI_FAILURE; }

    return HI_SUCCESS;
}


static int Read_Buffer_from_YUVFile (OMX_BUFFERHEADERTYPE*  pBufHdr)
{
    int bytes_read = 0, bytes_unit = 0;
    HI_U8* u32TmpData = malloc(1920 * 1088 * 3);

    int frame_size;
    int offsetYC;
    int offsetYCr;
    HI_U8* yAddr;
    HI_U8* cAddr;
    HI_U8* vAddr;
    int yuvSample;;
    int yuvStore;;
    int w;
    int h;
    int ystride;
    int cstride;
    int sel;

    switch (YUV_store_type)
    {
        case YUV_SEMIPLANNER_420_UV:
            ystride   = ALIGN_UP(width, 16);
            cstride   = ystride;
            offsetYC  = ystride * height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = OMX_U_V;
            frame_size = ystride * height + cstride * height / 2;
            break;

        case YUV_SEMIPLANNER_420_VU:
            ystride   = ALIGN_UP(width, 16);
            cstride   = ystride;
            offsetYC  = ystride * height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = OMX_V_U;
            frame_size = ystride * height + cstride * height / 2;
            break;

        case YUV_PLANNER_420:
            ystride   = ALIGN_UP(width, 16);
            cstride   = ALIGN_UP(width / 2, 16);
            offsetYC  = ystride * height;
            offsetYCr = cstride * height / 2;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_PLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = cAddr + offsetYCr;
            sel       = 0;
            frame_size = ystride * height + cstride * height;
            break;
#if 0

        case YUV_SEMIPLANNER_422_UV:
            ystride   = ALIGN_UP(width, 16);
            cstride   = ALIGN_UP(width, 16);	      //??
            offsetYC  = ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = OMX_U_V;
            frame_size = ystride * picHeight + cstride * picHeight;
            break;

        case YUV_SEMIPLANNER_422_VU:
            ystride   = ALIGN_UP(width, 16);
            cstride   = ALIGN_UP(width, 16);	      //??
            offsetYC  = ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = OMX_V_U;
            frame_size = ystride * picHeight + cstride * picHeight;
            break;
#endif

        case YUV_PACKAGE_422_YUYV:
            ystride   = ALIGN_UP(width * 2, 16);
            cstride   = 0;	      //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_Y0UY1V;    //??
            frame_size = ystride * height;
            break;

        case YUV_PACKAGE_422_YVYU:
            ystride   = ALIGN_UP(width * 2, 16);
            cstride   = 0;	      //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_Y0VY1U;    //??
            frame_size = ystride * height;
            break;

        case YUV_PACKAGE_422_UYVY:
            ystride   = ALIGN_UP(width * 2, 16);
            cstride   = 0;	      //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_UY0VY1;    //??
            frame_size = ystride * height;
            break;
    }

    if (HI_SUCCESS != yuvfile_to_memory(inputBufferFile, yAddr, cAddr, vAddr, yuvSample, yuvStore, width, height, ystride, cstride, sel))
    {
        DEBUG_PRINT("read bytes reach end\n");

        if (test_option)
        {
            rewind(inputBufferFile);
            //lseek(inputBufferFile,0,SEEK_SET);
            printf("Bytes read Zero After Read frame Size\n");

            if (HI_SUCCESS != yuvfile_to_memory(inputBufferFile, yAddr, cAddr, vAddr, yuvSample, yuvStore, width, height, ystride, cstride, sel))
            {
                printf("%s,%d,miss one FTB buffer!\n", __func__, __LINE__);
                goto error_1;
            }
        }
        else
        {
            pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
            goto error_1;
        }
    }

    DEBUG_PRINT("%s,%d! bufferLen = %d,frameSize = %d\n", __func__, __LINE__, pBufHdr->nAllocLen, frame_size);
    memcpy(pBufHdr->pBuffer, u32TmpData , frame_size);

    bytes_read = frame_size;
    /*if (1 == bytes_unit)
    {
    	DEBUG_PRINT("read bytes %d\n",frame_size);
    }
    else if(bytes_unit == 0)
    {
    	DEBUG_PRINT("Bytes read the end!!\n");
        pBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
        return 0;
    }*/

error_1:

    free(u32TmpData);
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
    DEBUG_PRINT("Function %s\n", __FUNCTION__);

    switch (eEvent)
    {
        case OMX_EventCmdComplete:

            DEBUG_PRINT("OMX_EventCmdComplete\n");

            if (OMX_CommandPortDisable == (OMX_COMMANDTYPE)nData1)
            {
                printf("*********************************************\n");
                printf("Recieved DISABLE Event Command Complete[%d]\n", nData2);
                printf("*********************************************\n");

            }
            else if (OMX_CommandPortEnable == (OMX_COMMANDTYPE)nData1)
            {
                printf("*********************************************\n");
                printf("Recieved ENABLE Event Command Complete[%d]\n", nData2);
                printf("*********************************************\n");

                pthread_mutex_lock(&enable_lock);
                ftb_disabled = 0;
                pthread_mutex_unlock(&enable_lock);
            }
            else if (OMX_CommandFlush == (OMX_COMMANDTYPE)nData1)
            {
                printf("*********************************************\n");
                printf("Received FLUSH Event Command Complete[%d]\n", nData2);
                printf("*********************************************\n");

                if (nData2 == 0)
                {
                    printf("****flush in complete\n");
                    flush_input_progress = 0;
                    sem_post(&in_flush_sem);
                }
                else if (nData2 == 1)
                {
                    printf("****flush out complete\n");
                    flush_output_progress = 0;
                    sem_post(&out_flush_sem);
                }
            }

            if (!flush_input_progress && !flush_output_progress)
            { event_complete(nData1); }

            break;

        case OMX_EventError:

            printf("*********************************************\n");
            printf("Received OMX_EventError Event Command !\n");
            printf("*********************************************\n");

            currentStatus = ERROR_STATE;

            if (OMX_ErrorIncorrectStateOperation == (OMX_ERRORTYPE)nData1 ||
                OMX_ErrorHardware == (OMX_ERRORTYPE)nData1)
            {
                DEBUG_PRINT_ERROR("Invalid State or hardware error\n");

                if (event_is_done == 0)
                {
                    DEBUG_PRINT("Event error in the middle of Encode\n");

                    pthread_mutex_lock(&eos_lock);
                    bOutputEosReached = true;
                    pthread_mutex_unlock(&eos_lock);
                }
            }

            if (waitForPortSettingsChanged)
            {
                waitForPortSettingsChanged = 0;
                event_complete(-1);
            }

            break;

        case OMX_EventPortSettingsChanged:

            DEBUG_PRINT("OMX_EventPortSettingsChanged\n", nData1);

            currentStatus = PORT_SETTING_CHANGE_STATE;

            if (waitForPortSettingsChanged)
            {
                waitForPortSettingsChanged = 0;
                event_complete(-1);
            }
            else
            {
                pthread_mutex_lock(&eos_lock);
                pthread_cond_broadcast(&eos_cond);
                pthread_mutex_unlock(&eos_lock);
            }

            break;

        case OMX_EventBufferFlag:

            DEBUG_PRINT("OMX_EventBufferFlag\n", nData1, nData2);

            if (nData1 == 1 && (nData2 & OMX_BUFFERFLAG_EOS))
            {
                pthread_mutex_lock(&eos_lock);
                bOutputEosReached = true;
                pthread_mutex_unlock(&eos_lock);
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
    DEBUG_PRINT("Function %s cnt[%d]\n", __FUNCTION__, ebd_cnt);
    int readBytes = 0;
    int bufCnt = 0;
    OMX_ERRORTYPE result;

    ebd_cnt++;

    if (bInputEosReached)
    {
        DEBUG_PRINT("*****EBD:Input EoS Reached************\n");
        return OMX_ErrorNone;
    }

    pthread_mutex_lock(&etb_lock);
    free_ip_buf_cnt++;

    if (!etb_disabled)
    {
        DEBUG_PRINT("********************** %s(),%d,etb_disabled=%d,pBuffer = %x", __func__, __LINE__, etb_disabled, pBuffer);

        if (push(etb_queue, (void*) pBuffer) < 0)                                     //压入队列
        {
            printf("Error in enqueue  ebd data\n");
            pthread_mutex_unlock(&etb_lock);
            return OMX_ErrorUndefined;
        }
    }

    pthread_mutex_unlock(&etb_lock);
    sem_post(&etb_sem);
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
    int ret;
    //DEBUG_PRINT("fill buffer done\n");

    pthread_mutex_lock(&fbd_lock);

    free_op_buf_cnt++;

    if (NULL == pBuffer->pOutputPortPrivate)
    {
        DEBUG_PRINT_ERROR("Error::pBuffer->pOutputPortPrivate = 0.\n");
        pthread_mutex_unlock(&fbd_lock);
        return OMX_ErrorUndefined;
    }

    if (!ftb_disabled)
    {
        if (push(fbd_queue, (void*)pBuffer) < 0)
        {
            pthread_mutex_unlock(&fbd_lock);
            DEBUG_PRINT_ERROR("Error in enqueueing fbd_data\n");
            return OMX_ErrorUndefined;
        }

        //sem_post(&fbd_sem);
    }

    pthread_mutex_unlock(&fbd_lock);
    sem_post(&fbd_sem);
    DEBUG_PRINT("################ [FBD] pts= %lld\n", pBuffer->nTimeStamp);

    return OMX_ErrorNone;
}


static int Init_Encoder(void)
{

    OMX_ERRORTYPE omxresult;
    OMX_U32 total = 0;
    int i = 0, is_found = 0;
    long bufCnt = 0;

    char vencCompNames[OMX_MAX_STRINGNAME_SIZE];
    char compRole[OMX_MAX_STRINGNAME_SIZE];
    int roles;

    static OMX_CALLBACKTYPE call_back =
    {
        &EventHandler,
        &EmptyBufferDone,
        &FillBufferDone
    };

    DEBUG_PRINT("Inside %s\n", __FUNCTION__);

    /* Init. the OpenMAX Core */
    DEBUG_PRINT("Initializing OpenMAX Core....\n");
    omxresult = OMX_Init();

    if (OMX_ErrorNone != omxresult)
    {
        DEBUG_PRINT_ERROR("Failed to Init OpenMAX core\n");
        return -1;
    }

    DEBUG_PRINT("OpenMAX Core Init sucess!\n");
    
    switch (CodeType)
    {
        case OMXVENC_H264:
            strncpy(vencCompNames, OMX_COMPONENTNAME_H264, OMX_MAX_STRINGNAME_SIZE);
            strncpy(compRole, OMX_COMPONENTROLES_H264, OMX_MAX_STRINGNAME_SIZE);
            portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;
            break;
        case OMXVENC_H265:
            strncpy(vencCompNames, OMX_COMPONENTNAME_Hevc, OMX_MAX_STRINGNAME_SIZE);
            strncpy(compRole, OMX_COMPONENTROLES_Hevc, OMX_MAX_STRINGNAME_SIZE);
            portFmt.format.video.eCompressionFormat = OMX_VIDEO_CodingHEVC;            
            break;
        default:
            DEBUG_PRINT_ERROR("CodeType %d err just support 0(H264), 1(h265)\n", CodeType);
            break;
    }

    for ( i = 0; ; i++ )   /*function OMX_ComponentNameEnum can break this cycle !*/
    {
        char enumCompName[OMX_MAX_STRINGNAME_SIZE];
        memset(enumCompName, 0 , OMX_MAX_STRINGNAME_SIZE);

        omxresult = OMX_ComponentNameEnum(enumCompName,
                                          OMX_MAX_STRINGNAME_SIZE, i);

        if (OMX_ErrorNone != omxresult)
        { break; }

        DEBUG_PRINT("%s,%d,==>i = %d. NAME = %s\n", __func__, __LINE__, i, enumCompName);

        if (!strncmp(enumCompName, vencCompNames, OMX_MAX_STRINGNAME_SIZE))
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

    /* Query for video encoders*/
    is_found = 0;
    OMX_GetRolesOfComponent(vencCompNames, &total, NULL);
    DEBUG_PRINT("\nOMX_GetRolesOfComponent %s, nums=%d\n", vencCompNames, total);

    if (total)
    {
        /* Allocate memory for pointers to component name */
        OMX_U8** role = (OMX_U8**)malloc((sizeof(OMX_U8*)) * total);

        for (i = 0; i < total; ++i)
        {
            role[i] = (OMX_U8*)malloc(sizeof(OMX_U8) * OMX_MAX_STRINGNAME_SIZE);
        }

        OMX_GetRolesOfComponent(vencCompNames, &total, role);

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
            DEBUG_PRINT_ERROR("No Role finded \n");
            return -1;
        }

    }
    else
    {
        DEBUG_PRINT_ERROR("No components found with Role:%s\n", vencCompNames);
        return -1;
    }

    DEBUG_PRINT("OpenMAX OMX_GetHandle....\n");

    omxresult = OMX_GetHandle((OMX_HANDLETYPE*)(&venc_handle),
                              (OMX_STRING)vencCompNames, NULL, &call_back);                                  /*key!!*/

    if (FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("Failed to Load the component:%s\n", vencCompNames);
        return -1;
    }

    DEBUG_PRINT("OMX_GetHandle success\n", vencCompNames);

    CONFIG_VERSION_SIZE(role);
    strncpy((char*)role.cRole, compRole, OMX_MAX_STRINGNAME_SIZE);
    omxresult = OMX_SetParameter(venc_handle, OMX_IndexParamStandardComponentRole, &role);             //设置编码器编码的协议类型

    if (FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to set param!\n");
        return -1;
    }

    DEBUG_PRINT("current role of component is %s\n", (char*)role.cRole);

    omxresult = OMX_GetParameter(venc_handle, OMX_IndexParamStandardComponentRole, &role);  //仅用于检查

    if (FAILED(omxresult))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to get role!\n");
        return -1;
    }

    if (strncmp((char*)role.cRole, compRole, OMX_MAX_STRINGNAME_SIZE))
    {
        DEBUG_PRINT_ERROR("ERROR - Set and Get not the same!!(line:%d)\n", __LINE__);
    }

    return 0;
}


static int Play_Encoder()
{
    OMX_VIDEO_PARAM_PORTFORMATTYPE videoportFmt = {0};
    int i = 0, bufCnt, index = 0;
    int frameSize = 0;
    int flag = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;
    OMX_BUFFERHEADERTYPE* pBuffer = NULL;
    OMX_STATETYPE state = OMX_StateReserved_0x00000000;
    OMX_VIDEO_PARAM_BITRATETYPE BitrateConfig;
    OMX_VIDEO_CONFIG_AVCINTRAPERIOD GopConfig;
    OMX_VIDEO_PARAM_PORTFORMATTYPE PortVideoFormat;
    OMX_VIDEO_CODINGTYPE TargetCompressionFormat;

    DEBUG_PRINT("Inside %s\n", __FUNCTION__);

    /* open the i/p and o/p files*/
    if (open_video_file() < 0)
    {
        DEBUG_PRINT_ERROR("Error in opening video file");
        return -1;
    }

    /* Get the port information */
    CONFIG_VERSION_SIZE(portParam);
    ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoInit,
                           (OMX_PTR)&portParam);

    if (FAILED(ret))
    {
        DEBUG_PRINT_ERROR("ERROR - Failed to get Port Param\n");
        return -1;
    }

    DEBUG_PRINT("portParam.nPorts:%d\n", portParam.nPorts);
    DEBUG_PRINT("portParam.nStartPortNumber:%d\n", portParam.nStartPortNumber);

    //=================================================== setVideoPortFormatType ====================================================//
    /* get support input port Video format*/
    PortVideoFormat.nPortIndex = portParam.nStartPortNumber;

    for (i = 0; (ret == 0) && (!flag); i++)
    {
        PortVideoFormat.nIndex = i;
        ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoPortFormat, &PortVideoFormat);

        if (!ret)
        {
            switch (YUV_store_type)
            {
                case YUV_SEMIPLANNER_420_UV:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                case YUV_SEMIPLANNER_420_VU:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYVU420SemiPlanar)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                case YUV_PLANNER_420:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYUV420Planar)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;
#if 0

                case YUV_SEMIPLANNER_422_UV:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYUV422SemiPlanar)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                case YUV_SEMIPLANNER_422_VU:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYVU422SemiPlanar)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;
#endif

                case YUV_PACKAGE_422_YUYV:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYCbYCr)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                case YUV_PACKAGE_422_YVYU:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatYCrYCb)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                case YUV_PACKAGE_422_UYVY:
                    if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatCbYCrY)
                        && (PortVideoFormat.eCompressionFormat == OMX_VIDEO_CodingUnused))
                    {
                        DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                        flag = 1;
                    }

                    break;

                default:
                    DEBUG_PRINT("not find such YUV_store_type for index(%d)\n", i);
                    break;
            }
        }
    }

    if (!flag)
    {
        DEBUG_PRINT_ERROR("Error: can't find suport video format for %s port! expect YUV_store_type(%d)\n",
                          PortVideoFormat.nPortIndex ? "input" : "output", YUV_store_type);
        return -1;
    }

    /* set input port Video format*/
    ret = OMX_SetParameter(venc_handle, OMX_IndexParamVideoPortFormat, &PortVideoFormat);

    if (ret)
    {
        DEBUG_PRINT_ERROR("Error:OMX_SetParameter ID:OMX_IndexParamVideoPortFormat.ret = %d,line %d\n", ret, __LINE__);
    }
    else
    {
        DEBUG_PRINT("OMX_SetParameter ID:OMX_IndexParamVideoPortFormat for %s port success! eCompressionFormat(%d),eColorFormat(%d),line%d\n",
                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eCompressionFormat, PortVideoFormat.eColorFormat, __LINE__);
    }

    /* Query the encoder input's  buf requirements */                                //设置输入端口的属性
    CONFIG_VERSION_SIZE(portFmt);
    portFmt.nPortIndex = portParam.nStartPortNumber;
    ret = OMX_GetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);
    CHECK_RET(ret);

    DEBUG_PRINT("Enc_Get(port:%d): Min Buffer Count %d\n", portFmt.nPortIndex, portFmt.nBufferCountMin);
    DEBUG_PRINT("Enc_Get(port:%d): Act Buffer Count %d\n", portFmt.nPortIndex, portFmt.nBufferCountActual);
    DEBUG_PRINT("Enc_Get(port:%d): Buffer Size %d\n", portFmt.nPortIndex, portFmt.nBufferSize);

    if (OMX_DirInput != portFmt.eDir)
    {
        DEBUG_PRINT_ERROR ("Enc: Expect Input Port\n");
        return -1;
    }

    CHECK_EQ(portFmt.format.video.eColorFormat, PortVideoFormat.eColorFormat);
    CHECK_EQ(portFmt.format.video.eCompressionFormat, PortVideoFormat.eCompressionFormat);

    portFmt.format.video.nFrameWidth  = width;
    portFmt.format.video.nFrameHeight = height;
    portFmt.format.video.xFramerate   = DEFAULT_FRMERATE << 16;                    //输入帧率
    portFmt.format.video.nSliceHeight = ALIGN_UP(height, 16);

    switch (portFmt.format.video.eColorFormat)
    {
        case OMX_COLOR_FormatYUV420SemiPlanar:
            portFmt.format.video.nStride      = ALIGN_UP(width, 16);
            portFmt.nBufferSize = ALIGN_UP(width, 16) * ALIGN_UP(height, 16) * 3 / 2;
            break;

        case OMX_COLOR_FormatYUV420Planar:
            portFmt.format.video.nStride      = ALIGN_UP(width, 16);
            portFmt.nBufferSize = portFmt.format.video.nStride * height + ALIGN_UP(width / 2, 16) * height;
            break;

        case OMX_COLOR_FormatYUV422SemiPlanar:
            portFmt.format.video.nStride      = ALIGN_UP(width, 16);
            portFmt.nBufferSize = ALIGN_UP(width, 16) * ALIGN_UP(height, 16) * 2;    //be careful
            break;

        case OMX_COLOR_FormatYUV422PackedSemiPlanar:
            portFmt.format.video.nStride      = ALIGN_UP(width, 16) * 2;
            portFmt.nBufferSize = ALIGN_UP(width * 2, 16) * height;
            break;

        default:
            break;
    }

    ret = OMX_SetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);
    CHECK_RET(ret);
    DEBUG_PRINT("input Video format: W x H (%d x %d).bufferSize = %d,stride = %d\n",
                portFmt.format.video.nFrameWidth,
                portFmt.format.video.nFrameHeight,
                portFmt.nBufferSize,
                portFmt.format.video.nStride);

    //=================================================== setVideoPortFormatType ====================================================//
    /* get support output port Video format*/
    ret = 0;
    i   = 0;
    flag = 0;
    PortVideoFormat.nPortIndex = portParam.nStartPortNumber + 1;
    TargetCompressionFormat = (CodeType == OMXVENC_H265) ? OMX_VIDEO_CodingHEVC : OMX_VIDEO_CodingAVC;

    for (i = 0; (ret == 0) && (!flag); i++)
    {
        PortVideoFormat.nIndex = i;
        ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoPortFormat, &PortVideoFormat);
        DEBUG_PRINT("get suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d) ==>TargetCompressionFormat = %d\n",
                                    PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat,TargetCompressionFormat);

        if (!ret)
        {
            if ((PortVideoFormat.eColorFormat == OMX_COLOR_FormatUnused) 
                && (PortVideoFormat.eCompressionFormat == TargetCompressionFormat))
            {
                DEBUG_PRINT("find suport video format for %s port:eColorFormat(%d),eCompressionFormat(%d)\n",
                            PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eColorFormat, PortVideoFormat.eCompressionFormat);
                flag = 1;
            }
        }
    }

    if (!flag)
    {
        DEBUG_PRINT_ERROR("Error: can't find suport video format for %s port!just support H264\n",
                          PortVideoFormat.nPortIndex ? "input" : "output");
        return -1;
    }

    /* set output port Video format*/
    ret = OMX_SetParameter(venc_handle, OMX_IndexParamVideoPortFormat, &PortVideoFormat);
    CHECK_RET(ret);
    DEBUG_PRINT("OMX_SetParameter ID:OMX_IndexParamVideoPortFormat for %s port success! eCompressionFormat(%d),eColorFormat(%d),line%d\n",
                PortVideoFormat.nPortIndex ? "input" : "output", PortVideoFormat.eCompressionFormat, PortVideoFormat.eColorFormat, __LINE__);

    /* Query the encoder output's  buf requirements */                                //设置输出端口的属性
    portFmt.nPortIndex = portParam.nStartPortNumber + 1;
    OMX_GetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);

    DEBUG_PRINT("Enc_Get(port:%d): Min Buffer Count %d\n", portFmt.nPortIndex, portFmt.nBufferCountMin);
    DEBUG_PRINT("Enc_Get(port:%d): Act Buffer Count %d\n", portFmt.nPortIndex, portFmt.nBufferCountActual);
    DEBUG_PRINT("Enc_Get(port:%d): Buffer Size %d\n", portFmt.nPortIndex, portFmt.nBufferSize);

    if (OMX_DirOutput != portFmt.eDir)
    {
        DEBUG_PRINT_ERROR ("Enc: Expect Output Port\n");
        return -1;
    }

    CHECK_EQ(portFmt.format.video.eColorFormat, PortVideoFormat.eColorFormat);
    CHECK_EQ(portFmt.format.video.eCompressionFormat, PortVideoFormat.eCompressionFormat);

    portFmt.format.video.nFrameWidth  = width;
    portFmt.format.video.nFrameHeight = height;
    portFmt.format.video.xFramerate   = 0;//DEFAULT_FRMERATE<<16;                      //输入帧率
    portFmt.format.video.nSliceHeight = 0;//ALIGN_UP(height, 16);

    if ( portFmt.format.video.nFrameWidth >= 1920)
    {
        portFmt.format.video.nBitrate  = 8 * 1024 * 1024;

    }
    else if (portFmt.format.video.nFrameWidth >= 1280)
    {
        portFmt.format.video.nBitrate  = 5 * 1024 * 1024;
    }
    else if (portFmt.format.video.nFrameWidth >= 720)
    {
        portFmt.format.video.nBitrate  = 3 * 1024 * 1024;
    }
    else
    {
        portFmt.format.video.nBitrate  = 1 * 1024 * 1024;
    }

    portFmt.nBufferSize =  portFmt.format.video.nBitrate / 20;

    ret = OMX_SetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);
    CHECK_RET(ret);
    DEBUG_PRINT("output stream format: W x H (%d x %d)\n",
                portFmt.format.video.nFrameWidth,
                portFmt.format.video.nFrameHeight);

    //=================================================== setupAVCEncoderParameters ====================================================//
    if (CodeType == OMXVENC_H264)
    {
        OMX_VIDEO_PARAM_AVCTYPE AVCType;
        AVCType.nPortIndex = OUTPUT_PORT_INDEX;
        ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoAvc, &AVCType);
        CHECK_RET(ret);

        AVCType.nAllowedPictureTypes =
            OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP;

        // Check profile and level parameters
        CodecProfileLevel defaultProfileLevel, profileLevel;
        defaultProfileLevel.mProfile = AVCType.eProfile;
        defaultProfileLevel.mLevel = AVCType.eLevel;
        ret = getVideoProfileLevel(&defaultProfileLevel, &profileLevel);
        CHECK_RET(ret);
        AVCType.eProfile = (profileLevel.mProfile);
        AVCType.eLevel   = (profileLevel.mLevel);

        // XXX
        if (AVCType.eProfile != OMX_VIDEO_AVCProfileBaseline)
        {
            printf("Use baseline profile instead of %d for AVC recording\n", AVCType.eProfile);
            AVCType.eProfile = OMX_VIDEO_AVCProfileBaseline;
        }

        if (AVCType.eProfile == OMX_VIDEO_AVCProfileBaseline)
        {
            AVCType.nSliceHeaderSpacing = 0;
            AVCType.bUseHadamard = OMX_TRUE;
            AVCType.nRefFrames = 1;
            AVCType.nBFrames = 0;
            AVCType.nPFrames = DEFAULT_FRMERATE * 1 - 1;  //多少毫秒一个I帧

            if (AVCType.nPFrames == 0)
            {
                AVCType.nAllowedPictureTypes = OMX_VIDEO_PictureTypeI;
            }

            AVCType.nRefIdx10ActiveMinus1 = 0;
            AVCType.nRefIdx11ActiveMinus1 = 0;
            AVCType.bEntropyCodingCABAC = OMX_FALSE;
            AVCType.bWeightedPPrediction = OMX_FALSE;
            AVCType.bconstIpred = OMX_FALSE;
            AVCType.bDirect8x8Inference = OMX_FALSE;
            AVCType.bDirectSpatialTemporal = OMX_FALSE;
            AVCType.nCabacInitIdc = 0;
        }

        if (AVCType.nBFrames != 0)
        {
            AVCType.nAllowedPictureTypes |= OMX_VIDEO_PictureTypeB;
        }

        AVCType.bEnableUEP = OMX_FALSE;
        AVCType.bEnableFMO = OMX_FALSE;
        AVCType.bEnableASO = OMX_FALSE;
        AVCType.bEnableRS = OMX_FALSE;
        AVCType.bFrameMBsOnly = OMX_TRUE;
        AVCType.bMBAFF = OMX_FALSE;
        AVCType.eLoopFilterMode = OMX_VIDEO_AVCLoopFilterEnable;

        ret = OMX_SetParameter(venc_handle, OMX_IndexParamVideoAvc, &AVCType);
        CHECK_RET(ret);
    }

    //=================================================== setupBitRate ====================================================//
    /* set the bitrate of output port*/
    CONFIG_VERSION_SIZE(BitrateConfig);
    BitrateConfig.nPortIndex = OUTPUT_PORT_INDEX;

    ret = OMX_GetParameter(venc_handle, OMX_IndexParamVideoBitrate, &BitrateConfig);
    CHECK_RET(ret);

    BitrateConfig.eControlRate   = OMX_Video_ControlRateConstant;
    BitrateConfig.nTargetBitrate = portFmt.format.video.nBitrate;

    ret = OMX_SetParameter(venc_handle, OMX_IndexParamVideoBitrate, &BitrateConfig);
    CHECK_RET(ret);

#if 0
    /* set the GOP of output port*/
    CONFIG_VERSION_SIZE(GopConfig);
    GopConfig.nPortIndex = OUTPUT_PORT_INDEX;
    OMX_GetConfig(venc_handle, OMX_IndexConfigVideoAVCIntraPeriod, &GopConfig);
    GopConfig.nIDRPeriod = 100;

    if (OMX_SetConfig(venc_handle, OMX_IndexConfigVideoAVCIntraPeriod, &GopConfig) != 0)
    {
        printf("ERROR: OMX_SetConfig faild,%d...\n", __LINE__);
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    printf("OMX_SetConfig:gop change SUCESS!\n");
#endif

    ////////////////////////////////////////////////////////////////////////////////////////
    DEBUG_PRINT("OMX_SendCommand Encoder -> IDLE\n");

    OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateIdle, 0);                      //  LOAD -> IDLE   :: creat channel

    // Allocate buffer on encoder's i/p port
    portFmt.nPortIndex = INPUT_PORT_INDEX;
    OMX_GetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);

    free_ip_buf_cnt = portFmt.nBufferCountActual;
    used_ip_buf_cnt = portFmt.nBufferCountActual;

    DEBUG_PRINT("input buffer Size=%d\n", portFmt.nBufferSize);
    DEBUG_PRINT("intput buffer cnt=%d\n", portFmt.nBufferCountActual);

    printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&& input buffer Size = %d\n", portFmt.nBufferSize);
    error = Allocate_Buffers(venc_handle, &pInputYUVBufHdrs, portFmt.nPortIndex, used_ip_buf_cnt, portFmt.nBufferSize);

    if (error != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("Error - OMX_AllocateBuffer Input buffer error\n");
        return -1;
    }

    DEBUG_PRINT("OMX_AllocateBuffer Input buffer success\n");

    //Allocate buffer on encoder's o/p port
    portFmt.nPortIndex = OUTPUT_PORT_INDEX;
    OMX_GetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);

    free_op_buf_cnt = portFmt.nBufferCountActual;
    used_op_buf_cnt = portFmt.nBufferCountActual;

    DEBUG_PRINT("output buffer Size=%d", portFmt.nBufferSize);
    DEBUG_PRINT("output buffer cnt=%d", portFmt.nBufferCountActual);

    error = Allocate_Buffers(venc_handle, &pOutputBufHdrs, portFmt.nPortIndex,
                             used_op_buf_cnt, portFmt.nBufferSize);

    if (error != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("Error - OMX_AllocateBuffer Output buffer error\n");
        return -1;
    }

    DEBUG_PRINT("OMX_AllocateBuffer Output buffer success\n");

    /* wait component state change from loaded->idle */
    wait_for_event(OMX_CommandStateSet);

    if (currentStatus == ERROR_STATE)
    {
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    OMX_GetState(venc_handle, &state);

    if (state != OMX_StateIdle)
    {
        DEBUG_PRINT_ERROR("Error - OMX state error\n");
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    DEBUG_PRINT("OMX_SendCommand Complete: Encoder -> Idle\n");

#if 0

    /* init v4l2 */
    if (displayYuv)
    {
        int i;
        struct buf_info buf_info[10];

        DEBUG_PRINT("prepare to sync user ptr with v4l2\n");

        for (i = 0; i < used_op_buf_cnt; i++)
        {
            buf_info[i].start  = pOutputBufHdrs[i]->pBuffer;
            buf_info[i].length = portFmt.nBufferSize;
            buf_info[i].index  = i;

            DEBUG_PRINT("buf[%d] start:%p, length:%d\n",
                        i, buf_info[i].start, buf_info[i].length);
        }

        if (v4l2_init(DEFAULT_WIDTH, DEFAULT_HEIGHT,
                      used_op_buf_cnt, buf_info) < 0)
        { return -1; }

        DEBUG_PRINT("v4l2 init ok!!\n");
    }

    if (freeHandle_option == FREE_HANDLE_AT_IDLE)
    {
        OMX_STATETYPE state = OMX_StateMax;
        OMX_GetState(venc_handle, &state);

        if (state == OMX_StateIdle)
        {
            DEBUG_PRINT("Comp OMX_StateIdle,trying to call OMX_FreeHandle\n");
            do_freeHandle_and_clean_up(false);
        }
        else
        {
            DEBUG_PRINT_ERROR("Comp: Decoder in state %d, "
                              "trying to call OMX_FreeHandle\n", state);
            do_freeHandle_and_clean_up(true);
        }

        return -1;
    }

#endif

    DEBUG_PRINT("OMX_SendCommand Encoder -> Executing\n");
    OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateExecuting, 0);                //start venc channel

    wait_for_event(OMX_CommandStateSet);

    if (currentStatus == ERROR_STATE)
    {
        DEBUG_PRINT_ERROR("OMX_SendCommand Encoder -> Executing\n");
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    OMX_GetState(venc_handle, &state);

    if (state != OMX_StateExecuting)
    {
        DEBUG_PRINT_ERROR("Error - OMX state error, state %d\n", state);
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    /////////////////////////////////////////////////////////////////////////

    DEBUG_PRINT("\nstart OMX_FillThisBuffer ...\n");

    for (bufCnt = 0; bufCnt < used_op_buf_cnt; ++bufCnt)
    {
        DEBUG_PRINT("OMX_FillThisBuffer on output buf no.%d\n", bufCnt);

        pOutputBufHdrs[bufCnt]->nOutputPortIndex = 1;
        pOutputBufHdrs[bufCnt]->nFlags &= ~OMX_BUFFERFLAG_EOS;

        ret = OMX_FillThisBuffer(venc_handle, pOutputBufHdrs[bufCnt]);

        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("Error - OMX_FillThisBuffer failed!!\n");
            //do_freeHandle_and_clean_up(true);                                          //qs
            //return -1;
        }
        else
        {
            DEBUG_PRINT("OMX_FillThisBuffer success! buffer heard :: %p\n", pOutputBufHdrs[bufCnt]);
            free_op_buf_cnt--;
        }

    }

    ////////////////////////////////////////////////////////////////////////
    DEBUG_PRINT("\nstart OMX_EmptyThisBuffer!\n");

    for (bufCnt = 0; bufCnt < used_ip_buf_cnt; bufCnt++)    //一次插入4帧
    {
        pInputYUVBufHdrs[bufCnt]->nInputPortIndex = 0;
        pInputYUVBufHdrs[bufCnt]->nOffset = 0;

        frameSize = Read_Buffer(pInputYUVBufHdrs[bufCnt]);

        if (frameSize <= 0)
        {

            DEBUG_PRINT("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ NO FRAME READ\n");
            pInputYUVBufHdrs[bufCnt]->nFilledLen = frameSize;
            pInputYUVBufHdrs[bufCnt]->nInputPortIndex = 0;
            pInputYUVBufHdrs[bufCnt]->nFlags |= OMX_BUFFERFLAG_EOS;;
            //pInputYUVBufHdrs = 1;

            OMX_EmptyThisBuffer(venc_handle, pInputYUVBufHdrs[bufCnt]);
            free_ip_buf_cnt--;
            etb_count++;
            DEBUG_PRINT("File is small::Either EOS or Error while reading file\n");
            break;
        }

        pInputYUVBufHdrs[bufCnt]->nFilledLen = frameSize;
        pInputYUVBufHdrs[bufCnt]->nInputPortIndex = 0;
        pInputYUVBufHdrs[bufCnt]->nFlags = 0;
        pInputYUVBufHdrs[bufCnt]->nTimeStamp = g_pts++;

        DEBUG_PRINT("~~~~~~~~~~~~ [etb]  PTS = %lld\n", pInputYUVBufHdrs[bufCnt]->nTimeStamp);
        ret = OMX_EmptyThisBuffer(venc_handle, pInputYUVBufHdrs[bufCnt]);

        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("ERROR: OMX_EmptyThisBuffer failed\n");
            do_freeHandle_and_clean_up(true);
            return -1;
        }
        else
        {
            /*if (bufCnt == 0)
            {
            	gettimeofday(&t_first, NULL);       //??
            }*/
            DEBUG_PRINT("OMX_EmptyThisBuffer success!\n");
            //printf("~~~~~~~~~~~~ [etb]  PTS = %d\n",pInputYUVBufHdrs[bufCnt]->nTimeStamp);
            free_ip_buf_cnt--;
            etb_count++;
        }
    }

    if (0 != pthread_create(&ebd_thread_id, NULL, ebd_thread, NULL))
    {
        DEBUG_PRINT_ERROR("Error in Creating ebd_thread\n");
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    return 0;
}

static int get_next_command(FILE* seq_file)
{
    int i = -1;

    do
    {
        i++;

        if (fread(&curr_seq_command[i], 1, 1, seq_file) != 1)
        { return -1; }
    }
    while (curr_seq_command[i] != '\n');

    curr_seq_command[i] = 0;

    DEBUG_PRINT("**cmd_str = %s**\n", curr_seq_command);

    return 0;
}
static int process_current_command(const char* seq_command)
{
    char* data_str = NULL;
    unsigned int data = 0, bufCnt = 0, i = 0;
    int frameSize;
    OMX_STATETYPE state = OMX_StateMax;
    OMX_VIDEO_CONFIG_AVCINTRAPERIOD omx_gop_config;
    OMX_VIDEO_CONFIG_BITRATETYPE omx_bitrate_config;
    OMX_CONFIG_FRAMERATETYPE     omx_framerate_config;

    if (strcmp(seq_command, "pause") == 0)
    {
        printf("$$$$$   PAUSE    $$$$$\n");
        printf("Sending PAUSE cmd to OMX compt\n");
        OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StatePause, 0);
        wait_for_event(OMX_CommandStateSet);
        OMX_GetState(venc_handle, &state);

        if (OMX_StatePause == state)
        {
            printf("***************************\n");
            printf("EventHandler for PAUSE DONE\n");
            printf("***************************\n");
        }
        else
        {
            printf("*****************************\n");
            printf("EventHandler for PAUSE FAILED\n");
            printf("Curent State is %d\n", state);
            printf("*****************************\n");
        }

    }
    else if (strcmp(seq_command, "resume") == 0)
    {
        printf("$$$$$   RESUME    $$$$$\n");
        printf("Sending RESUME cmd to OMX compt\n");
        OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateExecuting, 0);
        wait_for_event(OMX_CommandStateSet);
        OMX_GetState(venc_handle, &state);

        if (OMX_StateExecuting == state)
        {
            printf("***************************\n");
            printf("EventHandler for RESUME DONE\n");
            printf("***************************\n");
        }
        else
        {
            printf("*****************************\n");
            printf("EventHandler for RESUME FAILED\n");
            printf("Curent State is %d\n", state);
            printf("*****************************\n");
        }
    }
    else if (strcmp(seq_command, "flush_out") == 0)
    {
        printf("$$$$$   FLUSH    $$$$$\n");
        printf("Sending FLUSH cmd to OMX compt\n");

        flush_output_progress = 1;
        OMX_SendCommand(venc_handle, OMX_CommandFlush, OUTPUT_PORT_INDEX, 0);
        wait_for_event(OMX_CommandFlush);
        printf("Sending FLUSH OP cmd ok!\n");
    }
    else if (strcmp(seq_command, "flush_in") == 0)
    {
        printf("$$$$$   FLUSH    $$$$$\n");
        printf("Sending FLUSH cmd to OMX compt\n");

        flush_input_progress = 1;
        OMX_SendCommand(venc_handle, OMX_CommandFlush, INPUT_PORT_INDEX, 0);
        wait_for_event(OMX_CommandFlush);
        printf("Sending FLUSH IP cmd ok!\n");
    }
    else if (strcmp(seq_command, "flush_all") == 0)
    {
        printf("$$$$$   FLUSH    $$$$$\n");
        printf("Sending FLUSH cmd to OMX compt\n");

        flush_output_progress = 1;
        flush_input_progress  = 1;
        OMX_SendCommand(venc_handle, OMX_CommandFlush, 0xFFFFFFFF, 0);
        wait_for_event(OMX_CommandFlush);
        printf("Sending FLUSH ALL cmd ok!\n");
    }
    else if (strcmp(seq_command, "dis-op") == 0)
    {
        printf("$$$$$   DISABLE OP PORT   $$$$$\n");
        printf("Sending DISABLE OP cmd to OMX compt\n");

        if (disable_output_port() != 0)
        {
            printf("ERROR: While DISABLE OP...\n");
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("***************************\n");
        printf("DISABLE OP PORT SUCESS!\n");
        printf("***************************\n");
    }
    else if (strstr(seq_command, "en-op") == seq_command)
    {
        printf("$$$$$   ENABLE OP PORT    $$$$$\n");
        printf("Sending ENABLE OP cmd to OMX compt\n");

        if (enable_output_port() != 0)
        {
            printf("ERROR: While ENABLE OP...\n");
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("***************************\n");
        printf("ENABLE OP PORT SUCESS!\n");
        printf("***************************\n");
    }
    else if (strstr(seq_command, "Gop_to_50") == seq_command)
    {
        printf("$$$$$   CHANGE GOP      $$$$$\n");
        //printf("Sending ENABLE OP cmd to OMX compt\n");
        CONFIG_VERSION_SIZE(omx_gop_config);
        omx_gop_config.nPortIndex = OUTPUT_PORT_INDEX;
        OMX_GetConfig(venc_handle, OMX_IndexConfigVideoAVCIntraPeriod, &omx_gop_config);
        omx_gop_config.nIDRPeriod = 50;

        if (OMX_SetConfig(venc_handle, OMX_IndexConfigVideoAVCIntraPeriod, &omx_gop_config) != 0)
        {
            printf("ERROR: OMX_SetConfig faild,%d...\n", __LINE__);
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("OMX_SetConfig:gop change SUCESS!\n");
    }
    else if (strstr(seq_command, "inFrmRate_to_30") == seq_command)
    {
        printf("$$$$$   CHANGE input_FrmRate      $$$$$\n");
        CONFIG_VERSION_SIZE(omx_framerate_config);
        omx_framerate_config.nPortIndex = INPUT_PORT_INDEX;
        OMX_GetConfig(venc_handle, OMX_IndexConfigVideoFramerate, &omx_framerate_config);
        omx_framerate_config.xEncodeFramerate = 30;

        if (OMX_SetConfig(venc_handle, OMX_IndexConfigVideoFramerate, &omx_framerate_config) != 0)
        {
            printf("ERROR: OMX_SetConfig faild,%d...\n", __LINE__);
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("OMX_SetConfig:input FrmRate change SUCESS!\n");
    }
    else if (strstr(seq_command, "outFrmRate_to_30") == seq_command)
    {
        printf("$$$$$   CHANGE output_FrmRate      $$$$$\n");
        CONFIG_VERSION_SIZE(omx_framerate_config);
        omx_framerate_config.nPortIndex = OUTPUT_PORT_INDEX;
        OMX_GetConfig(venc_handle, OMX_IndexConfigVideoFramerate, &omx_framerate_config);
        omx_framerate_config.xEncodeFramerate = 30;

        if (OMX_SetConfig(venc_handle, OMX_IndexConfigVideoFramerate, &omx_framerate_config) != 0)
        {
            printf("ERROR: OMX_SetConfig faild,%d...\n", __LINE__);
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("OMX_SetConfig:output FrmRate change SUCESS!\n");
    }
    else if (strstr(seq_command, "BitRate_to_2M") == seq_command)
    {
        printf("$$$$$   CHANGE BitRate      $$$$$\n");
        CONFIG_VERSION_SIZE(omx_bitrate_config);
        omx_bitrate_config.nPortIndex = OUTPUT_PORT_INDEX;
        OMX_GetConfig(venc_handle, OMX_IndexConfigVideoBitrate, &omx_bitrate_config);
        omx_bitrate_config.nEncodeBitrate = 2 * 1024 * 1024;

        if (OMX_SetConfig(venc_handle, OMX_IndexConfigVideoBitrate, &omx_bitrate_config) != 0)
        {
            printf("ERROR: OMX_SetConfig faild,%d...\n", __LINE__);
            do_freeHandle_and_clean_up(true);
            return -1;
        }

        printf("OMX_SetConfig:BitRate change SUCESS!\n");
    }
    else
    {
        DEBUG_PRINT_ERROR(" $$$$$   INVALID CMD    $$$$$\n");
        DEBUG_PRINT_ERROR("seq_command[%s] is invalid\n", seq_command);
    }

    return 0;
}

static void* ebd_thread(void* pArg)
{
    int ret;
    OMX_STATETYPE current_state = OMX_StateMax;

    while (currentStatus != ERROR_STATE)
    {
        int readBytes = 0;
        OMX_BUFFERHEADERTYPE* pBuffer = NULL;

        if (flush_input_progress)
        {
            DEBUG_PRINT("EBD_thread flush wait start\n");
            sem_wait(&in_flush_sem);
            DEBUG_PRINT("EBD_thread flush wait complete\n");
        }

        sem_wait(&etb_sem);

        if (ebd_thread_exit)  { break; }

        if (bInputEosReached ) { break; }

        pthread_mutex_lock(&etb_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE*) pop(etb_queue);
        pthread_mutex_unlock(&etb_lock);

        if (pBuffer == NULL)
        {
            DEBUG_PRINT_ERROR("Error - No etb pBuffer to dequeue\n");
            continue;
        }

        pBuffer->nOffset = 0;

        if ((readBytes = Read_Buffer(pBuffer)) > 0)
        {
            pBuffer->nFilledLen = readBytes;

            /*DEBUG_PRINT("%s: Timestamp sent(%lld)\n", __func__,
            	pBuffer->nTimeStamp);*/

            pBuffer->nTimeStamp = g_pts++;
            ret = OMX_EmptyThisBuffer(venc_handle, pBuffer);

            if (OMX_ErrorIncorrectStateOperation == ret)
            {
                do
                {
                    sleep(1);
                    OMX_GetState(venc_handle, &current_state);
                    DEBUG_PRINT("%d,current_state = %d\n", __LINE__, current_state);
                }
                while ((current_state != OMX_StateExecuting) && (currentStatus != ERROR_STATE));

                ret = OMX_EmptyThisBuffer(venc_handle, pBuffer);
            }


            DEBUG_PRINT("~~~~~~~~~~~~ [etb]  PTS = %lld\n", pBuffer->nTimeStamp);
            etb_count++;
            free_ip_buf_cnt--;
        }
        else
        {
            if (test_option) /*recycle to read the YUV file*/
            {
                sem_post(&etb_sem);
                continue;
            }

            pBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
            bInputEosReached = true;
            pBuffer->nFilledLen = readBytes;

            OMX_EmptyThisBuffer(venc_handle, pBuffer);

            printf/*DEBUG_PRINT*/("EBD::Either EOS or Some Error while reading file\n");
            etb_count++;
            break;
        }
    }

    return NULL;
}


static void* fbd_thread(void* pArg)                     /*fill_buffer_done Thread*/
{
    //struct timeval t_before = {0, 0};
    //struct timeval t_after  = {0, 0};

    OMX_BUFFERHEADERTYPE* pBuffer = NULL;
    int free_done = 0;
    int ret = 0;
    static int first = 1;

    DEBUG_PRINT("First Inside %s", __FUNCTION__);

    //pthread_mutex_lock(&eos_lock);

    while (currentStatus != ERROR_STATE)
    {
        //pthread_mutex_unlock(&eos_lock);
        if (flush_output_progress)
        {
            DEBUG_PRINT("FBD_thread flush wait start\n");
            sem_wait(&out_flush_sem);
            DEBUG_PRINT("FBD_thread flush wait complete\n");
        }

        sem_wait(&fbd_sem);

        if (fbd_thread_exit)
        { break; }

        if (bInputEosReached ) { break; }

        pthread_mutex_lock(&fbd_lock);
        pBuffer = (OMX_BUFFERHEADERTYPE*)pop(fbd_queue);
        pthread_mutex_unlock(&fbd_lock);

        if (!pBuffer)
        {
            DEBUG_PRINT("Error - No pBuffer to dequeue\n");
            //pthread_mutex_lock(&eos_lock);
            continue;
        }

        if (pBuffer->nFilledLen > 0)
        {
            if (take264Log)
            {
                size_t bytes_written;
                bytes_written = fwrite((const char*)pBuffer->pBuffer, 1, pBuffer->nFilledLen, outputBufferFile);

                if (bytes_written != pBuffer->nFilledLen)
                {
                    DEBUG_PRINT_ERROR("\nfwrite err, write size=%d, slclen=%d\n", bytes_written, pBuffer->nFilledLen);
                }
                else { DEBUG_PRINT("\nfwrite success, write size=%d, slclen=%d\n", bytes_written, pBuffer->nFilledLen); }
            }

            ++fbd_cnt;
        }

        /********************************************************************/
        /* De-Initializing the open max and relasing the buffers and */
        /* closing the files.*/
        /********************************************************************/
        pthread_mutex_lock(&enable_lock);

        if (flush_output_progress)
        {
            pBuffer->nFilledLen = 0;
            pBuffer->nFlags &= ~OMX_BUFFERFLAG_EXTRADATA;

            pthread_mutex_lock(&fbd_lock);

            if (push(fbd_queue, (void*)pBuffer) < 0)
            {
                DEBUG_PRINT_ERROR("Error in enqueueing fbd_data\n");
            }
            else
            { sem_post(&fbd_sem); }

            pthread_mutex_unlock(&fbd_lock);
        }
        else
        {
            //pthread_mutex_lock(&fbd_lock);
            //pthread_mutex_lock(&eos_lock);

            if ( OMX_FillThisBuffer(venc_handle, pBuffer) == OMX_ErrorNone )    //KEYPOINT
            { free_op_buf_cnt--; }

            //pthread_mutex_unlock(&eos_lock);
            //pthread_mutex_unlock(&fbd_lock);
        }

        pthread_mutex_unlock(&enable_lock);
        //pthread_mutex_lock(&eos_lock);
    }

    //pthread_cond_broadcast(&eos_cond);

    //pthread_mutex_unlock(&eos_lock);
    return NULL;
}


int main(int argc, char** argv)
{
    int i = 0;
    int bufCnt = 0;
    int num = 0;
    unsigned int outputOption = 0;
    int pic_order = 0;
    OMX_ERRORTYPE result;

    //sliceheight = height = DEFAULT_HEIGHT;
    //stride      = width   = DEFAULT_WIDTH;
    //waitForPortSettingsChanged = 1;

    if (argc != 5)
    {
        printf("\n      Usage1: %s ./filename.yuv  Picture_Width  Picture_Height  CodeType(0:h264;1:h265)\n", argv[0]);
        return -1;
    }
    else
    {
        width = strtol(argv[2], NULL, 0);
        height = strtol(argv[3], NULL, 0);
        CodeType = strtol(argv[4], NULL, 0);
    }

    strncpy(in_filename, argv[1], strlen(argv[1]) + 1);
    Input_Filename_table[127] = '\0';
    printf("Input values: inputfilename[%s]\n", in_filename);
    printf("Input values: input YUV:%dX%d\n",  width, height);
    codec_format_option  = 0 ;

    printf("*********************************************\n");
    printf("please enter output take option:\n");
    printf("*********************************************\n");
    printf("0 --> Take h264 stream file\n");
    printf("1 --> NO h264 stream output\n");
    fflush(stdin);
    scanf("%d", &outputOption);
    fflush(stdin);

    printf("*********************************************\n");
    printf("please enter test option\n");
    printf("*********************************************\n");
    printf("0 --> Play the clip till the end\n");
    printf("1 --> Recycle play the clip.\n");
    fflush(stdin);
    scanf("%d", &test_option);
    fflush(stdin);

    printf("*********************************************\n");
    printf("please enter input YUV store/sample type\n");
    printf("*********************************************\n");
    printf("0 --> SP420_YCbCr\n");
    printf("1 --> SP420_YCrCb\n");
    printf("2 --> Planner 420\n");
    printf("3 --> Package422_YUYV\n");
    printf("4 --> Package422_YVYU\n");
    printf("5 --> Package422_UYVY\n");
    //printf("6 --> SP422_YCbCr\n");
    //printf("7 --> SP422_YCrCb\n");

    fflush(stdin);
    scanf("%d", &YUV_store_type);
    fflush(stdin);

    if (YUV_store_type >= YUV_UNKNOW_BUIT)
    {
        printf("Wrong option of YUV store/sample type! type(%d) not support!\n", YUV_store_type);
        return -1;
    }

    if (outputOption == 0)
    {
        take264Log = 1;
    }
    else if (outputOption == 1)
    {
        take264Log = 0;
    }
    else
    {
        printf("Wrong option of output option. Assume you want to take the h264 stream file\n");
        take264Log = 1;
    }

    if (test_option > 1)
    {
        printf("Wrong option of test option. Assume you want to Play the clip till the end\n");
        test_option = 0;
    }

    DEBUG_PRINT("*********get cmd line ok! *******\n");

    pthread_cond_init(&cond, 0);
    pthread_cond_init(&eos_cond, 0);
    pthread_mutex_init(&eos_lock, 0);
    pthread_mutex_init(&lock, 0);
    pthread_mutex_init(&etb_lock, 0);
    pthread_mutex_init(&fbd_lock, 0);
    pthread_mutex_init(&enable_lock, 0);

    if (-1 == sem_init(&etb_sem, 0, 0))
    {
        DEBUG_PRINT_ERROR("Error - sem_init failed %d\n", errno);
    }

    if (-1 == sem_init(&fbd_sem, 0, 0))
    {
        DEBUG_PRINT_ERROR("Error - sem_init failed %d\n", errno);
    }

    if (-1 == sem_init(&in_flush_sem, 0, 0))
    {
        DEBUG_PRINT_ERROR("Error - sem_init failed %d\n", errno);
    }

    if (-1 == sem_init(&out_flush_sem, 0, 0))
    {
        DEBUG_PRINT_ERROR("Error - sem_init failed %d\n", errno);
    }

    DEBUG_PRINT("%s %d\n", __func__, __LINE__);

    etb_queue = alloc_queue();

    if (etb_queue == NULL)
    {
        DEBUG_PRINT_ERROR("Error in Creating etb_queue\n");
        return -1;
    }

    fbd_queue = alloc_queue();

    if (fbd_queue == NULL)
    {
        DEBUG_PRINT_ERROR("Error in Creating fbd_queue\n");
        free_queue(etb_queue);
        return -1;
    }

    if (0 != pthread_create(&fbd_thread_id, NULL, fbd_thread, NULL))
    {
        DEBUG_PRINT_ERROR("Error in Creating fbd_thread\n");
        free_queue(etb_queue);
        free_queue(fbd_queue);
        return -1;
    }

    Read_Buffer = Read_Buffer_from_YUVFile;

    if (Init_Encoder() < 0)
    {
        DEBUG_PRINT_ERROR("Error - Encoder Init failed\n");
        return -1;
    }

    if (Play_Encoder() < 0)
    {
        DEBUG_PRINT_ERROR("Error - Encoder Play failed\n");
        return -1;
    }

    printf("*********prepare to run test *******\n");

    /////////////////////////////////////////

    loop_function();

    /////////////////////////////////////////  收尾工作

    ebd_thread_exit = 1;
    fbd_thread_exit = 1;
    //sem_post(&etb_sem);
    //sem_post(&fbd_sem);
    pthread_join(ebd_thread_id, NULL);
    printf("%s %d\n", __func__, __LINE__);
    pthread_join(fbd_thread_id, NULL);
    printf("%s %d\n", __func__, __LINE__);
    do_freeHandle_and_clean_up(false);



    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&eos_cond);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&etb_lock);
    pthread_mutex_destroy(&fbd_lock);
    pthread_mutex_destroy(&enable_lock);

    pthread_mutex_destroy(&eos_lock);

    if (-1 == sem_destroy(&etb_sem))
    {
        DEBUG_PRINT_ERROR("Error - sem_destroy failed %d\n", errno);
    }

    if (-1 == sem_destroy(&fbd_sem))
    {
        DEBUG_PRINT_ERROR("Error - sem_destroy failed %d\n", errno);
    }

    if (-1 == sem_destroy(&in_flush_sem))
    {
        DEBUG_PRINT_ERROR("Error - sem_destroy failed %d\n", errno);
    }

    if (-1 == sem_destroy(&out_flush_sem))
    {
        DEBUG_PRINT_ERROR("Error - sem_destroy failed %d\n", errno);
    }

    return 0;
}


static void loop_function(void)
{
    int cmd_error = 0;

    printf("Cmd test for control, cmds as follows:        \n");
    printf("** pause, resume                              **\n");
    printf("** flush_output ,flush_input, flush_all       **\n");
    printf("** dis-op, en-op                              **\n");
    printf("** Gop_to_50,inFrmRate_to_30,outFrmRate_to_30 **\n");
    printf("** BitRate_to_2M                              **\n");
    printf("Note: use \"exit\"to esc**\n\n\n");

    while (bOutputEosReached == false && (cmd_error == 0))
    {
        fflush(stdin);
        scanf("%s", curr_seq_command);

        if (!strcmp(curr_seq_command, "exit"))
        {
            printf("test-case exit!!\n");
            break;
        }

        cmd_error = process_current_command(curr_seq_command);

    }
}


static OMX_ERRORTYPE Allocate_Buffers ( OMX_COMPONENTTYPE* venc_handle,
                                        OMX_BUFFERHEADERTYPE**  *pBufHdrs,
                                        OMX_U32 nPortIndex,
                                        long bufCntMin, long bufSize)
{
    DEBUG_PRINT("Inside %s \n", __FUNCTION__);
    OMX_ERRORTYPE error = OMX_ErrorNone;
    long bufCnt = 0;

    DEBUG_PRINT("pBufHdrs = %x,bufCntMin = %d\n", pBufHdrs, bufCntMin);
    *pBufHdrs = (OMX_BUFFERHEADERTYPE**) malloc(sizeof(OMX_BUFFERHEADERTYPE) * bufCntMin);

    for (bufCnt = 0; bufCnt < bufCntMin; ++bufCnt)
    {
        DEBUG_PRINT("\nOMX_AllocateBuffer No %d\n", bufCnt);
        error = OMX_AllocateBuffer(venc_handle, &((*pBufHdrs)[bufCnt]),
                                   nPortIndex, NULL, bufSize);
    }

    return error;
}


static void free_output_buffers()
{
    int index = 0;
    OMX_BUFFERHEADERTYPE* pBuffer = NULL;

    pBuffer = (OMX_BUFFERHEADERTYPE*)pop(fbd_queue);

    while (index < portFmt.nBufferCountActual)
    {
        if (pBuffer)
        {
            DEBUG_PRINT("Free output buffer[%d] addr %p\n", index, pBuffer);
            OMX_FreeBuffer(venc_handle, 1, pBuffer);
            index++;
        }

        pBuffer = (OMX_BUFFERHEADERTYPE*)pop(fbd_queue);
    }
}



static void do_freeHandle_and_clean_up(bool isDueToError)
{
    int bufCnt = 0;
    int ret = 0;
    OMX_STATETYPE state = OMX_StateMax;
    OMX_ERRORTYPE result = OMX_ErrorNone;

    OMX_GetState(venc_handle, &state);

    if (state == OMX_StateExecuting || state == OMX_StatePause)
    {
        etb_disabled = 1;
        ftb_disabled = 1;

        DEBUG_PRINT("Requesting transition to Idle\n");

        ret = OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateIdle, 0);

        wait_for_event(OMX_CommandStateSet);

        if (currentStatus == ERROR_STATE)
        {
            DEBUG_PRINT_ERROR("executing -> idle state trans error\n");
            return;
        }

        printf("%s %d\n", __func__, __LINE__);
        OMX_GetState(venc_handle, &state);

        if (state != OMX_StateIdle)
        {
            DEBUG_PRINT_ERROR("current component state :%d error!\n", state);
            return;
        }

        DEBUG_PRINT("current component state :%d\n", state);

        //usleep(30*1000);  //check if the flush work!
        if ((free_ip_buf_cnt != used_ip_buf_cnt) || (free_op_buf_cnt != used_op_buf_cnt))
        {
            printf("########################## ERROR: %s(),%d,buffer not flush!!!\n", __func__, __LINE__);
            printf("########################## input:%d/%d;output:%d/%d\n", used_ip_buf_cnt, free_ip_buf_cnt, used_op_buf_cnt, free_op_buf_cnt);
        }
    }

    if (state == OMX_StateIdle)
    {
        DEBUG_PRINT("Requesting transition to Loaded\n");
        OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateLoaded, 0);

        for (bufCnt = 0; bufCnt < used_ip_buf_cnt; ++bufCnt)
        {
            OMX_FreeBuffer(venc_handle, 0, pInputYUVBufHdrs[bufCnt]);
        }

        if (pInputYUVBufHdrs)
        {
            free(pInputYUVBufHdrs);
            pInputYUVBufHdrs = NULL;
        }

        DEBUG_PRINT("free ip buffer ok!\n");

        for (bufCnt = 0; bufCnt < used_op_buf_cnt; ++bufCnt)
        {
            OMX_FreeBuffer(venc_handle, 1, pOutputBufHdrs[bufCnt]);
        }

        if (pOutputBufHdrs)
        {
            free(pOutputBufHdrs);
            pOutputBufHdrs = NULL;
        }

        free_op_buf_cnt = 0;

        DEBUG_PRINT("free op buffer ok!\n");

        wait_for_event(OMX_CommandStateSet);

        if (currentStatus == ERROR_STATE)
        {
            DEBUG_PRINT_ERROR("idle -> loaded state trans error\n");
            return;
        }

        OMX_GetState(venc_handle, &state);

        if (state != OMX_StateLoaded)
        {
            DEBUG_PRINT_ERROR("current component state :%d error!\n", state);
            return;
        }

        DEBUG_PRINT("current component state :%d\n", state);
    }

    DEBUG_PRINT("[OMX VENC Test] - Free omx handle\n");

    result = OMX_FreeHandle(venc_handle);

    if (result != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("OMX_FreeHandle error. Error code: %d\n", result);
        return;
    }

    DEBUG_PRINT("[OMX VENC Test] - Free omx handle ok!!\n");

    venc_handle = NULL;

    /* Deinit OpenMAX */
    DEBUG_PRINT("De-initializing OMX \n");
    OMX_Deinit();

    //printf(" De-intailizing v4l2 \n");
    //v4l2_deinit();

    DEBUG_PRINT("closing all files \n");

    /*if(inputBufferFileFd > 0)
    {
    	close(inputBufferFileFd);
    	inputBufferFileFd = -1;
    }*/
    if (inputBufferFile != NULL)
    {
        fclose(inputBufferFile);
        inputBufferFile = NULL;
    }

    printf("after free inputfile \n");

    if (take264Log && outputBufferFile)
    {
        fclose(outputBufferFile);
        outputBufferFile = NULL;
    }

    DEBUG_PRINT("after free outputfile \n");/**/

    if (etb_queue)
    {
        free_queue(etb_queue);
        etb_queue = NULL;
    }

    DEBUG_PRINT("after free etb_queue\n");

    if (fbd_queue)
    {
        free_queue(fbd_queue);
        fbd_queue = NULL;
    }

    DEBUG_PRINT("after free iftb_queue\n");

    printf("*****************************************\n");

    if (isDueToError)
    { printf("************...TEST FAILED...************\n"); }
    else
    { printf("**********...TEST SUCCESSFULL...*********\n"); }

    printf("*****************************************\n");
}

#if 0
static void stopOmxComponent_1(void)
{
    int ret = 0;

    switch (mState)
    {
        case EXECUTING:
        {
            setState(EXECUTING_TO_IDLE);

            if (0/*mQuirks & kRequiresFlushBeforeShutdown*/)
            {
#if 0
                bool emulateInputFlushCompletion =
                    !flushPortAsync(kPortIndexInput);   /*flush the ip/*/
                bool emulateOutputFlushCompletion =
                    !flushPortAsync(kPortIndexOutput);

                if (emulateInputFlushCompletion)
                {
                    onCmdComplete(OMX_CommandFlush, kPortIndexInput);
                }

                if (emulateOutputFlushCompletion)
                {
                    onCmdComplete(OMX_CommandFlush, kPortIndexOutput);
                }

#endif
            }
            else
            {
                etb_disabled = 1;
                ftb_disabled = 1;

                ret = OMX_SendCommand(venc_handle, OMX_CommandStateSet, OMX_StateIdle, 0);;
                CHECK_RET(ret);
            }

            break;
        }

        default:
            printf("should not be here!\n");
            break;
    }

    DEBUG_PRINT("exit %s() %d success!\n", __func__, __LINE__);
}
#endif

static int disable_output_port(void)
{
    DEBUG_PRINT("prepre to disable output port\n");

    // Send DISABLE command.
    pthread_mutex_lock(&enable_lock);
    ftb_disabled = 1;
    pthread_mutex_unlock(&enable_lock);

    printf("%s %d\n", __func__, __LINE__);
    DEBUG_PRINT("%s %d\n", __func__, __LINE__);

    OMX_SendCommand(venc_handle, OMX_CommandPortDisable, OUTPUT_PORT_INDEX, 0);
    wait_for_event(OMX_CommandPortDisable);

    if (currentStatus == ERROR_STATE)
    {
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    DEBUG_PRINT("%s %d\n", __func__, __LINE__);

    return 0;
}


static int enable_output_port(void)
{
    int bufCnt = 0;
    OMX_ERRORTYPE ret = OMX_ErrorNone;

    DEBUG_PRINT("prepare to enable output port\n");

    // Send Enable command
    OMX_SendCommand(venc_handle, OMX_CommandPortEnable, OUTPUT_PORT_INDEX, 0);

    /* Allocate buffer on decoder's o/p port */
    portFmt.nPortIndex = 1;
    error = Allocate_Buffers(venc_handle, &pOutputBufHdrs, portFmt.nPortIndex,
                             portFmt.nBufferCountActual, portFmt.nBufferSize);

    if (error != OMX_ErrorNone)
    {
        DEBUG_PRINT_ERROR("OMX_AllocateBuffer Output buffer error\n");
        return -1;
    }
    else
    {
        DEBUG_PRINT("OMX_AllocateBuffer Output buffer success\n");
        free_op_buf_cnt = portFmt.nBufferCountActual;
    }

    // wait for enable event to come back
    DEBUG_PRINT("waiting enable done....\n");

    wait_for_event(OMX_CommandPortEnable);

    if (currentStatus == ERROR_STATE)
    {
        DEBUG_PRINT_ERROR("start error!\n");
        do_freeHandle_and_clean_up(true);
        return -1;
    }

    DEBUG_PRINT("wake up to fill buffers\n");

    for (bufCnt = 0; bufCnt < portFmt.nBufferCountActual; ++bufCnt)
    {
        DEBUG_PRINT("OMX_FillThisBuffer on output buf no.%d\n", bufCnt);

        pOutputBufHdrs[bufCnt]->nOutputPortIndex = 1;
        pOutputBufHdrs[bufCnt]->nFlags &= ~OMX_BUFFERFLAG_EOS;

        ret = OMX_FillThisBuffer(venc_handle, pOutputBufHdrs[bufCnt]);

        if (OMX_ErrorNone != ret)
        {
            DEBUG_PRINT_ERROR("OMX_FillThisBuffer failed, result %d\n", ret);
        }
        else
        {
            DEBUG_PRINT("OMX_FillThisBuffer success!\n");
            free_op_buf_cnt--;
        }
    }

    DEBUG_PRINT("OP PORT ENABLED!\n");
    return 0;
}


static int output_port_reconfig()
{
    DEBUG_PRINT("PORT_SETTING_CHANGE_STATE\n");

    if (disable_output_port() != 0)
    {
        DEBUG_PRINT_ERROR("disable output port failed\n");
        return -1;
    }

    /* Port for which the Client needs to obtain info */

    portFmt.nPortIndex = 1;

    OMX_GetParameter(venc_handle, OMX_IndexParamPortDefinition, &portFmt);

    DEBUG_PRINT("Min Buffer Count=%d\n", portFmt.nBufferCountMin);
    DEBUG_PRINT("Buffer Size=%d\n", portFmt.nBufferSize);

    if (OMX_DirOutput != portFmt.eDir)
    {
        DEBUG_PRINT_ERROR("Error - Expect Output Port\n");
        return -1;
    }

    height = portFmt.format.video.nFrameHeight;
    width = portFmt.format.video.nFrameWidth;
    stride = portFmt.format.video.nStride;
    sliceheight = portFmt.format.video.nSliceHeight;

    /*fix me : reinit v4l2 devices !!! -- liucan*/

    if (enable_output_port() != 0)
    {
        DEBUG_PRINT_ERROR("enable output port failed\n");
        return -1;
    }

    DEBUG_PRINT("PORT_SETTING_CHANGE DONE!\n");
    return 0;
}
