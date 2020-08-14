/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <linux/fb.h>
#include <sys/file.h>

#include "hi_unf_common.h"
#include "hi_unf_venc.h"
#include "hi_adp.h"


#define QFRAME_NUM      (6)
#define VENC_MAX_CHN    (8)
#define ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )
//#define VENC_PROPERTY_TEST

typedef struct hiThread_INFO_S
{
    HI_U32  RealChnNum;
    HI_HANDLE hVenc[VENC_MAX_CHN];
    FILE*       pFileInput[VENC_MAX_CHN];
    FILE*       pFileOutput[VENC_MAX_CHN];
    HI_U32    u32PicWitdth[VENC_MAX_CHN];
    HI_U32    u32PicHeight[VENC_MAX_CHN];
    HI_U32    u32TimeOut[VENC_MAX_CHN];
} SAMPLE_THREAD_INFO_S;

typedef struct hiQUEUE_THREAD_ARG_S
{
    HI_U32  RealChnNum;
    HI_HANDLE hVenc;
    FILE*       pFiletoEnc;
    HI_U32    u32width;
    HI_U32    u32height;
    HI_U32    u32ChanId;
} QUEUE_THREAD_ARG_S;

static pthread_t g_threadSampleSendFrame[VENC_MAX_CHN];
QUEUE_THREAD_ARG_S g_stSampleSendArgs[VENC_MAX_CHN];
static HI_BOOL g_bSampleSendFrameStop = HI_FALSE;

static  HI_CHAR MMZ_name_table[VENC_MAX_CHN][32] = {"VENC_MMZ_1", "VENC_MMZ_2", "VENC_MMZ_3", "VENC_MMZ_4", "VENC_MMZ_5", "VENC_MMZ_6", "VENC_MMZ_7", "VENC_MMZ_8"};
//static HI_CHAR Output_Filename_table_2[VENC_MAX_CHN][32]={"./venc_0.h264","./venc_1.h264","./venc_2.h264","./venc_3.h264","./venc_4.h264","./venc_5.h264","./venc_6.h264","./venc_7.h264"};

static HI_HANDLE g_hVenc[VENC_MAX_CHN];

static HI_CHAR Input_Filename_table[VENC_MAX_CHN][128];
static HI_CHAR Output_Filename_table[VENC_MAX_CHN][128];
static HI_U32 g_JpegType[VENC_MAX_CHN] = {0};

HI_MMZ_BUF_S g_stFrmBuf[VENC_MAX_CHN];

/******************************************************************
*************************** VENC **********************************
******************************************************************/

typedef struct hiVENC_THREAD_ARG_S
{
    // HI_U32  RealChnNum;
    HI_HANDLE   hVenc;
    FILE*       pFileToSave;
    HI_U32      TimeOut;
    HI_CHAR     FileName[128];
} VENC_THREAD_ARG_S;

static pthread_t g_threadSampleVenc[VENC_MAX_CHN];
static VENC_THREAD_ARG_S g_stSampleVencArgs[VENC_MAX_CHN];
static HI_BOOL g_bSampleVencStop = HI_FALSE;
static int YUV_store_type = 0;

typedef struct hiVENC_SAMPLE_INPUT
{
    HI_U32    u32width_pic;
    HI_U32    u32height_pic;
    HI_U32    u32width_enc;
    HI_U32    u32height_enc;
} VENC_INPUT_INFO_S;


static void* SampleVencRoutine(void* args)
{
    VENC_THREAD_ARG_S* pArgs = (VENC_THREAD_ARG_S*)args;

    HI_S32 ret = HI_FAILURE;
    //HI_U32 i = 0;
    HI_UNF_VENC_STREAM_S vencStream;

    //HI_HANDLE hVenc = pArgs->hVenc;
    //FILE *pFile = pArgs->pFileToSave;
    //FILE *pFile;

    while (!g_bSampleVencStop)
    {
        //        for(i = 0;i < pArgs->RealChnNum;i++)
        //        {
        ret = HI_UNF_VENC_AcquireStream(pArgs->hVenc, &vencStream, pArgs->TimeOut);
        if (HI_SUCCESS != ret)
        {
            usleep(2 * 1000);
            //sleep(1);
            continue;
        }

        if (pArgs->pFileToSave) /* save to file */
        {
            if (vencStream.u32SlcLen > 0)
            {
                ret = fwrite(vencStream.pu8Addr, 1, vencStream.u32SlcLen, pArgs->pFileToSave);
                if (ret != vencStream.u32SlcLen)
                {
                    //         printf("fwrite err, write size=%d, slclen=%d\n",ret, vencStream.u32SlcLen);          //liminqi
                }
                //else  printf("fwrite success, write size=%d, slclen=%d\n",ret, vencStream.u32SlcLen);
            }
        }
        else { printf("ERROR: the pFie is NULL!!\n"); }

        ret = HI_UNF_VENC_ReleaseStream(pArgs->hVenc, &vencStream);
        if (HI_SUCCESS != ret)
        {
            printf("HI_UNF_VENC_ReleaseStream failed, u32Ret=%x\n", ret);
        }
        //}
    }
    return NULL;
}

static void* SampleVencRoutine_mjpge(void* args)
{
    VENC_THREAD_ARG_S* pArgs = (VENC_THREAD_ARG_S*)args;

    HI_S32 ret = HI_FAILURE;
    HI_U32 size = 0;
    HI_UNF_VENC_STREAM_S vencStream0;
    HI_S32 timeoutMs = 0;
    HI_HANDLE hVenc = pArgs->hVenc;
    FILE* pFile = pArgs->pFileToSave;
    //HI_U32 FrameNum = pArgs->FrameNum;
    while (!g_bSampleVencStop) //
    {
        memset(&vencStream0, 0, sizeof(HI_UNF_VENC_STREAM_S));
        ret = HI_UNF_VENC_AcquireStream(hVenc, &vencStream0, timeoutMs);
        if (HI_SUCCESS != ret)
        {
            usleep(3 * 1000);
            //sleep(1);
            continue;
        }

        if (pFile) /* save to file */
        {
            size = vencStream0.u32SlcLen;

            fwrite(&size, 1, sizeof(HI_U32), pArgs->pFileToSave);

            if (vencStream0.u32SlcLen > 0)
            {
                ret = fwrite(vencStream0.pu8Addr, 1, vencStream0.u32SlcLen, pArgs->pFileToSave);
                if (ret != vencStream0.u32SlcLen)
                {
                    printf("fwrite err0, write size=%d, slclen=%d\n", ret, vencStream0.u32SlcLen);         //liminqi
                }
                /*else  printf("fwrite success0, write size=%d, slclen=%d,vencStream0.end = %d,pts = %d\n",
                    ret, vencStream0.u32SlcLen,vencStream0.bFrameEnd,vencStream0.u32PtsMs);*/
            }
        }
        else { printf("ERROR: the pFie is NULL!!\n"); }

        HI_UNF_VENC_ReleaseStream(hVenc, &vencStream0);


    }
    return NULL;
}

static HI_U32 g_JpegNum = 0;
static void* SampleVencRoutine_jpge(void* args)
{
    VENC_THREAD_ARG_S* pArgs = (VENC_THREAD_ARG_S*)args;

    HI_S32 ret = HI_FAILURE;
    HI_UNF_VENC_STREAM_S vencStream0;
    HI_S32 timeoutMs = 0;
    HI_CHAR stFileName[128];
    HI_CHAR stFileID[32];
    HI_HANDLE hVenc = pArgs->hVenc;
    FILE* pFile = NULL;

    while (!g_bSampleVencStop) //
    {
        memset(&vencStream0, 0, sizeof(HI_UNF_VENC_STREAM_S));
        ret = HI_UNF_VENC_AcquireStream(hVenc, &vencStream0, timeoutMs);
        if (HI_SUCCESS != ret)
        {
            usleep(3 * 1000);
            continue;
        }

        //strncpy(stFileName,pArgs->FileName,strlen(pArgs->FileName));
        memcpy(stFileName, pArgs->FileName, sizeof(HI_CHAR) * 128);
        snprintf(stFileID, sizeof(stFileName), "_%d.jpg", g_JpegNum);
        strncat(stFileName, stFileID, strlen(stFileID));
        stFileName[127] = '\0';

        pFile = fopen(stFileName, "wb+");
        if (pFile) /* save to file */
        {
            if (vencStream0.u32SlcLen > 0)
            {
                ret = fwrite(vencStream0.pu8Addr, 1, vencStream0.u32SlcLen, pFile);
                if (ret != vencStream0.u32SlcLen)
                {
                    printf("fwrite err0, write size=%d, slclen=%d\n", ret, vencStream0.u32SlcLen);         //liminqi
                }
            }
        }
        else { printf("ERROR: the pFie is NULL!!\n"); }
        g_JpegNum++;
        if (g_JpegNum >= 9999) { g_JpegNum = 0; }
        HI_UNF_VENC_ReleaseStream(hVenc, &vencStream0);
        fclose(pFile);
        pFile = NULL;
    }
    return NULL;
}



#if 1
/* video frame filed in buffer */
typedef enum hiDRV_FIELD_MODE_E
{
    HI_DRV_FIELD_TOP = 0,
    HI_DRV_FIELD_BOTTOM,
    HI_DRV_FIELD_ALL,
    HI_DRV_FIELD_BUTT
} HI_DRV_FIELD_MODE_E;

/* source color space */
typedef enum hiDRV_COLOR_SPACE_E
{
    HI_DRV_CS_UNKNOWN = 0,
    HI_DRV_CS_DEFAULT,

    HI_DRV_CS_BT601_YUV_LIMITED,/* BT.601 */
    HI_DRV_CS_BT601_YUV_FULL,
    HI_DRV_CS_BT601_RGB_LIMITED,
    HI_DRV_CS_BT601_RGB_FULL,

    HI_DRV_CS_NTSC1953,

    /* These should be useful.  Assume 601 extents. */
    HI_DRV_CS_BT470_SYSTEM_M,
    HI_DRV_CS_BT470_SYSTEM_BG,

    HI_DRV_CS_BT709_YUV_LIMITED,/* BT.709 */
    HI_DRV_CS_BT709_YUV_FULL,
    HI_DRV_CS_BT709_RGB_LIMITED,
    HI_DRV_CS_BT709_RGB_FULL,

    HI_DRV_CS_REC709,      /* HD and modern captures. */

    HI_DRV_CS_SMPT170M, /* ITU-R 601 -- broadcast NTSC/PAL */
    HI_DRV_CS_SMPT240M, /* 1125-Line (US) HDTV */

    HI_DRV_CS_BT878,    /* broken BT878 extents
                           (601, luma range 16-253 instead of 16-235) */

    HI_DRV_CS_XVYCC,

    /* I know there will be cameras that send this.  So, this is
     * unspecified chromaticities and full 0-255 on each of the
     * Y'CbCr components
     */
    HI_DRV_CS_JPEG,
    HI_DRV_CS_BUTT
} HI_DRV_COLOR_SPACE_E;

enum
{
    YUV_SEMIPLANNER_420_UV  = 0,
    YUV_SEMIPLANNER_420_VU  = 1,
    YUV_PLANNER_420         = 2,
    YUV_PACKAGE_422_YUYV    = 3,
    YUV_PACKAGE_422_YVYU    = 4,
    YUV_PACKAGE_422_UYVY    = 5,
    YUV_SEMIPLANNER_422_UV  = 6, //没调通
    YUV_SEMIPLANNER_422_VU  = 7,//没调通
    YUV_UNKNOW_BUIT
};

enum
{
    V_U             = 0,
    U_V             = 1
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
/*private struct*/
typedef struct hiDRV_VIDEO_PRIVATE_S
{
    HI_BOOL bValid;
    HI_U32 u32LastFlag;                  /*最后一帧标记*/
    HI_DRV_COLOR_SPACE_E eColorSpace;    //?????????
    HI_U32 u32BufferID;                    //?????????????ID
    HI_U32 u32FrmCnt;
    HI_U32  u32PlayTime;                /* ????*/
    HI_DRV_FIELD_MODE_E  eOriginField;   /*?DEI???,????????????*/
    HI_RECT_S stOriginImageRect;         /* ?LBOX???,??LBOX?????? */
    HI_U32 u32Reserve[32];              //??????????????????,?VC1????
} HI_DRV_VIDEO_PRIVATE_S;
#endif

static int yuvfile_to_memory( FILE*  fpYUV,
                              HI_U8* yAddr,      /* only by plannar, semiplannar,
package */
                              HI_U8* cAddr,      /* only by plannar, semiplannar */
                              HI_U8* vAddr,      /* only by plannar */
                              int    yuvSample,  /* 420 422 444 */
                              int    yuvStore,   /* semiplannar, plannar, package */
                              int    w,
                              int    h,
                              int    ystride,     /* only by semiplannar, plannar,
package */
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
        for ( j = 0; j < hY; j++, pY += ystride )
        {
            if ( 1 != fread(pY, wY, 1,
                            fpYUV) ) { ERROR_READ; }
        }
        for ( j = 0; j < hC; j++, pU += cstride )
        {
            if ( 1 != fread(pU, wC, 1,
                            fpYUV) ) { ERROR_READ; }
        }
        for ( j = 0; j < hC; j++, pV += cstride )
        {
            if ( 1 != fread(pV, wC, 1,
                            fpYUV) ) { ERROR_READ; }
        }
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

        for ( j = 0; j < hY; j++, pY += ystride )
        {
            if ( 1 != fread(pY, wY, 1,
                            fpYUV) ) { ERROR_READ; }
        }
        for ( j = 0; j < hC; j++, pU += cstride )
        {
            if ( 1 != fread(pU, wC, 1,
                            fpYUV) ) { ERROR_READ; }
        }
        for ( j = 0; j < hC; j++, pV += cstride )
        {
            if ( 1 != fread(pV, wC, 1,
                            fpYUV) ) { ERROR_READ; }
        }

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

static int Read_Buffer_from_YUVFile (QUEUE_THREAD_ARG_S* pArgs, HI_U8* u32TmpData)
{
    int frame_size = 0;
    int offsetYC = 0;
    int offsetYCr = 0;
    HI_U8* yAddr = NULL;
    HI_U8* cAddr = NULL;
    HI_U8* vAddr = NULL;
    int yuvSample = 0;
    int yuvStore = 0;
    int ystride = 0;
    int cstride = 0;
    int sel = 0;

    switch (YUV_store_type)
    {
        case YUV_SEMIPLANNER_420_UV:
            ystride   = ALIGN_UP(pArgs->u32width, 16);
            cstride   = ystride;
            offsetYC  = ystride * pArgs->u32height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = U_V;
            frame_size = ystride * pArgs->u32height + cstride * pArgs->u32height / 2;
            break;
        case YUV_SEMIPLANNER_420_VU:
            ystride   = ALIGN_UP(pArgs->u32width, 16);
            cstride   = ystride;
            offsetYC  = ystride * pArgs->u32height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = V_U;
            frame_size = ystride * pArgs->u32height + cstride * pArgs->u32height / 2;
            break;
        case YUV_PLANNER_420:
            ystride   = ALIGN_UP(pArgs->u32width, 16);
            cstride   = ALIGN_UP(pArgs->u32width / 2, 16);
            offsetYC  = ystride * pArgs->u32height;
            offsetYCr = cstride * pArgs->u32height / 2;
            yuvSample = SAMPLE_VEDU_YUV420;
            yuvStore  = SAMPLE_VEDU_PLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = cAddr + offsetYCr;
            sel       = 0;
            frame_size = ystride * pArgs->u32height + cstride * pArgs->u32height;
            break;
        case YUV_SEMIPLANNER_422_UV:
            ystride   = ALIGN_UP(pArgs->u32width, 16);
            cstride   = ALIGN_UP(pArgs->u32width, 16);        //??
            offsetYC  = ystride * pArgs->u32height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = U_V;
            frame_size = ystride * pArgs->u32height + cstride * pArgs->u32height;
            break;
        case YUV_SEMIPLANNER_422_VU:
            ystride   = ALIGN_UP(pArgs->u32width, 16);
            cstride   = ALIGN_UP(pArgs->u32width, 16);        //??
            offsetYC  = ystride * pArgs->u32height;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_SEMIPLANNAR;
            yAddr     = u32TmpData;
            cAddr     = yAddr + offsetYC;
            vAddr     = NULL;
            sel       = V_U;
            frame_size = ystride * pArgs->u32height + cstride * pArgs->u32height;
            break;

        case YUV_PACKAGE_422_YUYV:
            ystride   = ALIGN_UP(pArgs->u32width * 2, 16);
            cstride   = 0;        //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_Y0UY1V;    //??
            frame_size = ystride * pArgs->u32height;
            break;
        case YUV_PACKAGE_422_YVYU:
            ystride   = ALIGN_UP(pArgs->u32width * 2, 16);
            cstride   = 0;        //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_Y0VY1U;    //??
            frame_size = ystride * pArgs->u32height;
            break;
        case YUV_PACKAGE_422_UYVY:
            ystride   = ALIGN_UP(pArgs->u32width * 2, 16);
            cstride   = 0;        //??
            offsetYC  = 0;//ystride * picHeight;
            offsetYCr = 0;
            yuvSample = SAMPLE_VEDU_YUV422;
            yuvStore  = SAMPLE_VEDU_PACKAGE;
            yAddr     = u32TmpData;
            cAddr     = NULL;
            vAddr     = NULL;
            sel       = SAMPLE_VEDU_UY0VY1;    //??
            frame_size = ystride * pArgs->u32height;
            break;
    }

    if (HI_SUCCESS != yuvfile_to_memory(pArgs->pFiletoEnc, yAddr, cAddr, vAddr,
                                        yuvSample, yuvStore, pArgs->u32width, pArgs->u32height, ystride, cstride, sel))
    {
        return 0;
    }

    return frame_size;
}

HI_U32 CalcPerFrmSize(HI_U32 Width, HI_U32 Height, HI_U32 YUVFormat)
{
    HI_U32 FrmBufSize = 0;
    if(YUV_PLANNER_420 == YUVFormat)
    {
        FrmBufSize = (ALIGN_UP(Width, 16) + ALIGN_UP(Width/2, 16) ) * Height;
    }
    else if (YUV_SEMIPLANNER_420_UV == YUVFormat || YUV_SEMIPLANNER_420_VU == YUVFormat)
    {
        FrmBufSize =  ALIGN_UP(Width, 16) * Height * 3 / 2;
    }
    else if (YUV_PACKAGE_422_YUYV == YUVFormat 
          || YUV_PACKAGE_422_YVYU == YUVFormat 
          || YUV_PACKAGE_422_UYVY == YUVFormat)
    {
        FrmBufSize =  ALIGN_UP(Width, 16) * Height * 2;
    }
    else if (YUV_SEMIPLANNER_422_UV == YUVFormat 
          || YUV_SEMIPLANNER_422_VU == YUVFormat)
    {
        FrmBufSize = ALIGN_UP(Width, 16) * Height * 2;
    }
    else
    {
        printf("not support yuv store type!!!\n");
    }   

    return FrmBufSize;
}

HI_S32 AllocFrameBuf(HI_U32 ChanId, HI_U32 Width, HI_U32 Height)
{
    HI_MMZ_BUF_S stBuf;
    HI_U32 perSize = 0;
    HI_S32 s32Ret;

    perSize = CalcPerFrmSize(Width, Height, YUV_store_type);
    if (perSize == 0)
    {
        return HI_FAILURE;
    }

    sprintf(stBuf.bufname, "%s", MMZ_name_table[ChanId]);
    stBuf.bufsize = perSize * QFRAME_NUM;
    
    s32Ret = HI_MMZ_Malloc(&stBuf);
    if (HI_FAILURE == s32Ret)
    {
        printf("~~~~~~ERROR: HI_MMZ_Malloc(%d) Failed!! Ret:%d\n", ChanId, s32Ret);
        
        return HI_FAILURE;
    }

    memcpy(&g_stFrmBuf[ChanId], &stBuf, sizeof(HI_MMZ_BUF_S));
    
    return HI_SUCCESS;
}

HI_S32 FreeFrameBuf(HI_U32 ChanId)
{
    HI_S32 s32Ret;
    
    s32Ret = HI_MMZ_Free(&g_stFrmBuf[ChanId]);
    if (HI_FAILURE == s32Ret)
    {
        printf("~~~~~~ERROR: HI_MMZ_free Failed!! Ret:%d\n", s32Ret);
        return HI_FAILURE;
    }  

    memset(&g_stFrmBuf[ChanId], 0, sizeof(HI_MMZ_BUF_S));

    return HI_SUCCESS;
}

void* SampleSendFrameRoutine(void* args)
{
    HI_S32 s32Ret;
    HI_U32 pts = 1;
    HI_U32 u32PhyAddr = 0x0;
    HI_U32 offset ;

    HI_UNF_VIDEO_FRAME_INFO_S stFrameinfo;
    HI_UNF_VIDEO_FRAME_INFO_S stFrameinfo_release;
    HI_U32 perSize = 0;
    HI_DRV_VIDEO_PRIVATE_S drv_private_info;

    QUEUE_THREAD_ARG_S* pArgs = (QUEUE_THREAD_ARG_S*)args;
    HI_U32 queue_id  = 0;
    HI_U32 readlen = 0;
    HI_U64 u32Vir2PhyOffset = 0;
    //====================================== MMZ申请内存
    HI_MMZ_BUF_S stBuf;

    //HI_U8 u32TmpData[1920*1088*3/2];
    HI_U8* u32TmpData = malloc(1920 * 1088 * 3);

    perSize = CalcPerFrmSize(pArgs->u32width, pArgs->u32height, YUV_store_type);
    if (perSize == 0)
    {
        return HI_NULL;
    }
    
    memcpy(&stBuf, &g_stFrmBuf[pArgs->u32ChanId], sizeof(HI_MMZ_BUF_S));
    
    u32Vir2PhyOffset = (HI_U64)((long)stBuf.user_viraddr - stBuf.phyaddr);
    //======================================

    for (queue_id = 0; queue_id < QFRAME_NUM; queue_id++)
    {
        memset(&stFrameinfo, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));
        memset(&stFrameinfo_release, 0, sizeof(HI_UNF_VIDEO_FRAME_INFO_S));

        //readlen = fread(u32TmpData, perSize, 1, pArgs->pFiletoEnc);
        readlen = Read_Buffer_from_YUVFile(pArgs, u32TmpData);
        if (readlen > 0)
        {
            u32PhyAddr = stBuf.phyaddr + queue_id * perSize;
            memcpy(stBuf.user_viraddr + queue_id * perSize, u32TmpData , perSize);
            stFrameinfo.u32Width                       = pArgs->u32width;
            stFrameinfo.u32Height                      = pArgs->u32height;
            stFrameinfo.bProgressive                   = 1;
            stFrameinfo.enFieldMode                    = HI_UNF_VIDEO_FIELD_ALL;
            stFrameinfo.enFramePackingType             = 0;
            if (YUV_SEMIPLANNER_420_UV == YUV_store_type || YUV_SEMIPLANNER_420_VU == YUV_store_type)
            {
                stFrameinfo.stVideoFrameAddr[0].u32YAddr   = u32PhyAddr;
                offset                                     = ALIGN_UP(pArgs->u32width, 16) * pArgs->u32height;
                stFrameinfo.stVideoFrameAddr[0].u32CAddr   = u32PhyAddr + offset;
                stFrameinfo.stVideoFrameAddr[0].u32YStride = ALIGN_UP(pArgs->u32width, 16);
                stFrameinfo.stVideoFrameAddr[0].u32CStride = ALIGN_UP(pArgs->u32width, 16);
                if (YUV_SEMIPLANNER_420_VU == YUV_store_type)
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_SEMIPLANAR_420;    //??
                }
                else
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_SEMIPLANAR_420_UV;    //??
                }
            }
            else if (YUV_PLANNER_420 == YUV_store_type)
            {
                stFrameinfo.stVideoFrameAddr[0].u32YAddr   = u32PhyAddr;
                offset                                     = ALIGN_UP(pArgs->u32width, 16) * pArgs->u32height;
                stFrameinfo.stVideoFrameAddr[0].u32CAddr   = u32PhyAddr + offset;
                offset += (ALIGN_UP(pArgs->u32width / 2, 16) * pArgs->u32height) / 2;
                stFrameinfo.stVideoFrameAddr[0].u32CrAddr = u32PhyAddr + offset;
                stFrameinfo.stVideoFrameAddr[0].u32YStride = ALIGN_UP(pArgs->u32width, 16);
                stFrameinfo.stVideoFrameAddr[0].u32CStride = ALIGN_UP(pArgs->u32width / 2, 16);
                stFrameinfo.stVideoFrameAddr[0].u32CrStride = ALIGN_UP(pArgs->u32width / 2, 16);
                stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_PLANAR_420;    //??
            }
            else if (YUV_PACKAGE_422_YUYV == YUV_store_type || YUV_PACKAGE_422_YVYU ==
                     YUV_store_type || YUV_PACKAGE_422_UYVY == YUV_store_type)
            {
                stFrameinfo.stVideoFrameAddr[0].u32YAddr   = u32PhyAddr;
                offset                                     = ALIGN_UP(pArgs->u32width * 2, 16) * pArgs->u32height;
                stFrameinfo.stVideoFrameAddr[0].u32CAddr   = 0;
                stFrameinfo.stVideoFrameAddr[0].u32YStride =  ALIGN_UP(pArgs->u32width * 2, 16);
                stFrameinfo.stVideoFrameAddr[0].u32CStride = 0;
                if (YUV_PACKAGE_422_YUYV == YUV_store_type)
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;    //??
                }
                else if (YUV_PACKAGE_422_YVYU == YUV_store_type)
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_PACKAGE_YVYU;    //??
                }
                else
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_PACKAGE_UYVY;    //??
                }
            }
            else if (YUV_SEMIPLANNER_422_UV == YUV_store_type ||  YUV_SEMIPLANNER_422_VU== YUV_store_type)
            {
                stFrameinfo.stVideoFrameAddr[0].u32YAddr   = u32PhyAddr;
                offset                                     = ALIGN_UP(pArgs->u32width, 16) * pArgs->u32height;
                stFrameinfo.stVideoFrameAddr[0].u32CAddr   = u32PhyAddr + offset;
                stFrameinfo.stVideoFrameAddr[0].u32YStride = ALIGN_UP(pArgs->u32width, 16);
                stFrameinfo.stVideoFrameAddr[0].u32CStride =
                ALIGN_UP(pArgs->u32width / 2, 16);
                if (YUV_SEMIPLANNER_422_VU == YUV_store_type)
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_SEMIPLANAR_422;    //??
                }
                else
                {
                    stFrameinfo.enVideoFormat                  = HI_UNF_FORMAT_YUV_SEMIPLANAR_422_1X2;    //??
                }
            }
            stFrameinfo.u32Pts                         = pts++;
            drv_private_info.u32FrmCnt                 = stFrameinfo.u32Pts;

            memcpy(stFrameinfo.u32Private, &drv_private_info, sizeof(HI_DRV_VIDEO_PRIVATE_S));
        }
        else
        {
            printf("read vid file error!\n");
            rewind(pArgs->pFiletoEnc);
            continue;
        }

        s32Ret = HI_UNF_VENC_QueueFrame(pArgs->hVenc, &stFrameinfo);
        if (s32Ret != HI_SUCCESS)
        {
            printf("the venc inter frame buffer is full(frame num =  %d)!\n", queue_id);
            break;
        }
    }

    while (!g_bSampleSendFrameStop)
    {
        do
        {
            s32Ret = HI_UNF_VENC_DequeueFrame(pArgs->hVenc, &stFrameinfo_release);
            usleep(5 * 1000);
        }
        while ((0 != s32Ret) && (!g_bSampleSendFrameStop));

#ifndef VENC_PROPERTY_TEST
        readlen = Read_Buffer_from_YUVFile(pArgs, u32TmpData);
        if (readlen > 0)
        {
            memcpy((HI_VOID*)(long)(stFrameinfo_release.stVideoFrameAddr[0].u32YAddr + u32Vir2PhyOffset),
                   u32TmpData , perSize);
            stFrameinfo_release.u32Pts                 = pts++;
            drv_private_info.u32FrmCnt                 = stFrameinfo_release.u32Pts;
            memcpy(stFrameinfo_release.u32Private, &drv_private_info, sizeof(HI_DRV_VIDEO_PRIVATE_S));
        }
        else
        {
            printf("read vid file error!\n");
            rewind(pArgs->pFiletoEnc);
            readlen = Read_Buffer_from_YUVFile(pArgs, u32TmpData);
            if (readlen > 0)
            {
                memcpy((HI_VOID*)(long)(stFrameinfo_release.stVideoFrameAddr[0].u32YAddr + u32Vir2PhyOffset),
                       u32TmpData , perSize);
                stFrameinfo_release.u32Pts                 = pts++;
                drv_private_info.u32FrmCnt                 = stFrameinfo_release.u32Pts;
                memcpy(stFrameinfo_release.u32Private, &drv_private_info, sizeof(HI_DRV_VIDEO_PRIVATE_S));
            }
            else
            {
                printf("ERROR :: miss one Framebuffer!!\n");
                continue;
            }
        }
#else
        stFrameinfo_release.u32Pts                 = pts++;
#endif
        s32Ret = HI_UNF_VENC_QueueFrame(pArgs->hVenc, &stFrameinfo_release);
        if (HI_SUCCESS != s32Ret)
        {
            printf("QueueFrame err!ret = %x\n", s32Ret);
            continue;
        }
    }

    free(u32TmpData);
    
    return NULL;
}

HI_S32 HI_ADP_VENC_Create(HI_UNF_VENC_CHN_ATTR_S* pstVeAttr, HI_HANDLE* phVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hVenc;

    if (phVenc == NULL)
    {
        printf("%s,%d,bad phVenc !\n", __func__, __LINE__);
    }

    HIAPI_RUN(HI_UNF_VENC_Create(&hVenc, pstVeAttr), ret);
    if (ret != HI_SUCCESS)
    {
        printf("HI_UNF_VENC_CreateChn failed!!\n");
        HI_UNF_VENC_DeInit();
        return ret;
    }

    *phVenc = hVenc;
    return ret;
}


HI_S32 HI_ADP_VENC_Start(HI_HANDLE hVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HIAPI_RUN(HI_UNF_VENC_Start(hVenc), ret);
    if (ret != HI_SUCCESS)
    {
        printf("HI_UNF_VENC_Start failed!!\n");
        //HI_UNF_VENC_DetachInput(hVenc, hVi);
        HI_UNF_VENC_Destroy(hVenc);
        HI_UNF_VENC_DeInit();
        return ret;
    }
    return ret;
}

HI_S32 HI_ADP_VENC_Thread_Create(SAMPLE_THREAD_INFO_S* input)
{
    HI_U32 i = 0;
    if (!input)
    {
        printf("%s,%d,bad input param !\n", __func__, __LINE__);
        return HI_FAILURE;
    }

    for (i = 0; i < input->RealChnNum; i++)
    {
        g_stSampleVencArgs[i].hVenc        = input->hVenc[i];
        g_stSampleVencArgs[i].pFileToSave  = input->pFileOutput[i];
        g_stSampleVencArgs[i].TimeOut      = input->u32TimeOut[i];

        if (g_JpegType[i] == 2)
        {
            memcpy(g_stSampleVencArgs[i].FileName, Output_Filename_table[i], sizeof(g_stSampleVencArgs[i].FileName));
        }
    }
    //g_stSampleVencArgs.RealChnNum = input->RealChnNum;
    g_bSampleVencStop = HI_FALSE;

    for (i = 0; i < input->RealChnNum; i++)
    {
        if (g_JpegType[i] == 1)   /*mjpeg*/
        {
            pthread_create(&g_threadSampleVenc[i], HI_NULL, SampleVencRoutine_mjpge, (void*)(&g_stSampleVencArgs[i]));
        }
        else if ((g_JpegType[i] == 0)||(g_JpegType[i] == 3)) /*h264/hevc*/
        {
            pthread_create(&g_threadSampleVenc[i], HI_NULL, SampleVencRoutine, (void*)(&g_stSampleVencArgs[i]));
        }
        else if (g_JpegType[i] == 2) /*jpeg*/
        {
            pthread_create(&g_threadSampleVenc[i], HI_NULL, SampleVencRoutine_jpge, (void*)(&g_stSampleVencArgs[i]));
        }

        usleep(2 * 1000);
    }

    for (i = 0; i < input->RealChnNum; i++)
    {
        g_stSampleSendArgs[i].hVenc       = input->hVenc[i];
        g_stSampleSendArgs[i].u32width    = input->u32PicWitdth[i];
        g_stSampleSendArgs[i].u32height   = input->u32PicHeight[i];
        g_stSampleSendArgs[i].pFiletoEnc  = input->pFileInput[i];

    }
    //g_stSampleSendArgs.RealChnNum = input->RealChnNum;
    g_bSampleSendFrameStop = HI_FALSE;
    for (i = 0; i < input->RealChnNum; i++)
    {
        g_stSampleSendArgs[i].u32ChanId = i;
        pthread_create(&g_threadSampleSendFrame[i], HI_NULL, SampleSendFrameRoutine, (void*)(&g_stSampleSendArgs[i]));
        usleep(2 * 1000);
    }
    return HI_SUCCESS;
}

HI_VOID HI_ADP_VENC_Thread_Stop(HI_U32 chnNum)
{
    HI_U32 i = 0;
    if (!g_bSampleVencStop)
    {
        g_bSampleVencStop = HI_TRUE;
        for (i = 0; i < chnNum; i++)
        {
            pthread_join(g_threadSampleVenc[i], NULL);
        }
    }


    if (!g_bSampleSendFrameStop)
    {
        g_bSampleSendFrameStop = HI_TRUE;
        for (i = 0; i < chnNum; i++)
        {
            pthread_join(g_threadSampleSendFrame[i], NULL);
        }
    }

    printf("Thread stop OK!!\n");

}


HI_S32 HI_ADP_VENC_Stop(HI_HANDLE hVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HIAPI_RUN(HI_UNF_VENC_Stop(hVenc), ret);
    return ret;
}

HI_S32 HI_ADP_VENC_Destroy(HI_HANDLE hVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HIAPI_RUN(HI_UNF_VENC_Destroy(hVenc), ret);
    return ret;
}


/*HI_S32 HI_ADP_VIVENC_DeInit(HI_HANDLE hVenc)
{
    HI_S32 ret = HI_SUCCESS;
    HIAPI_RUN(HI_UNF_VENC_Stop(hVenc), ret);

    HIAPI_RUN(HI_UNF_VENC_Destroy(hVenc), ret);

    //HIAPI_RUN(HI_UNF_VENC_DeInit(), ret);

    if (!g_bSampleSendFrameStop)
    {
        g_bSampleSendFrameStop = HI_TRUE;
        pthread_join(g_threadSampleSendFrame, NULL);
    }
    return ret;
}*/

#if 1
static HI_S32 process_current_command(HI_HANDLE hVenc, HI_S32 cmd)
{
    HI_S32 ret = 0;
    HI_BOOL flag = 0;
    HI_UNF_VENC_CHN_ATTR_S stAttr;
    ret = HI_UNF_VENC_GetAttr(hVenc, &stAttr);
    if ( HI_SUCCESS != ret )
    {
        printf("ERROR:VENC GetAttr FAILED!(%d)\n", __LINE__);
    }

    switch (cmd)
    {
        case 0:   /*Input FrameRate*/
        {
            HI_U32 inputFrmRate_temp;
            while ( !flag )
            {
                printf("\nCurrent input FrameRate -> %d,target FrameRate -> %d\n", stAttr.u32InputFrmRate, stAttr.u32TargetFrmRate);
                printf("Active Change the input FrameRate to -> ");
                fflush(stdin);
                scanf("%d", &inputFrmRate_temp);
                if (inputFrmRate_temp < stAttr.u32TargetFrmRate )
                {
                    printf("\nPlease input the input FrameRate again!(should > targetFrmRate(%d))\n", stAttr.u32TargetFrmRate);
                }
                else
                {
                    stAttr.u32InputFrmRate =  inputFrmRate_temp;
                    printf("\nThe new input FrameRate -> %d\n", stAttr.u32InputFrmRate);
                    flag = 1;
                }
            }
            break;
        }
        case 1:   /*Target FrameRate*/
        {
            HI_U32 targetFrmRate_temp;
            while ( !flag )
            {
                printf("\nCurrent input FrameRate -> %d,target FrameRate -> %d\n", stAttr.u32InputFrmRate, stAttr.u32TargetFrmRate);
                printf("Active Change the target FrameRate to -> ");
                fflush(stdin);
                scanf("%d", &targetFrmRate_temp);
                if (targetFrmRate_temp > stAttr.u32InputFrmRate)
                {
                    printf("\nPlease input the target FrameRate again!(should < inputFrmRate(%d))\n", stAttr.u32InputFrmRate);
                }
                else
                {
                    stAttr.u32TargetFrmRate =  targetFrmRate_temp;
                    printf("\nThe new target FrameRate -> %d\n", stAttr.u32TargetFrmRate);
                    flag = 1;
                }
            }
            break;
        }
        case 2:   /*Target BitRate*/
        {
            HI_U32 targetBitRate_temp;
            while ( !flag )
            {
                printf("\nCurrent Resolution Ratio  -> %d * %d\n", stAttr.u32Width, stAttr.u32Height);
                printf("Current target BitRate(unit:kbps) -> %d\n", stAttr.u32TargetBitRate / 1000);
                printf("Active Change the target BitRate(unit:kbps) to -> ");
                fflush(stdin);
                scanf("%d", &targetBitRate_temp);
                if ( (stAttr.u32Width >= 1920) && (targetBitRate_temp < 1000))
                {
                    printf("\nWARNING: the new target BitRate(%d) maybe too small!\n", targetBitRate_temp);
                    flag = 1;
                }
                else
                {
                    stAttr.u32TargetBitRate =  targetBitRate_temp * 1000;
                    printf("\nThe new target BitRate(unit:kbps) -> %d\n", targetBitRate_temp);
                    flag = 1;
                }
            }
            break;
        }
        case 3:   /*GOP*/
        {
            HI_U32 GOP_temp;
            printf("\nCurrent GOP  -> %d \n", stAttr.u32Gop);
            printf("Active Change the GOP to -> ");
            fflush(stdin);
            scanf("%d", &GOP_temp);

            stAttr.u32Gop =  GOP_temp;
            printf("\nThe new GOP -> %d\n", stAttr.u32Gop );

            break;
        }
        case 4:   /*MaxQP*/
        {
            HI_U32 MaxQP_temp;
            while ( !flag )
            {
                printf("\nCurrent MaxQP -> %d , MinQP -> %d\n", stAttr.u32MaxQp, stAttr.u32MinQp);
                printf("Active Change the MaxQP to -> ");
                fflush(stdin);
                scanf("%d", &MaxQP_temp);
                if ( MaxQP_temp > 51 || MaxQP_temp <= stAttr.u32MinQp)
                {
                    printf("\nPlease input the MaxQP again!(should < 51 && >MinQP(%d))\n", stAttr.u32MinQp);
                }
                else
                {
                    stAttr.u32MaxQp =  MaxQP_temp;
                    printf("\nThe new MaxQP -> %d\n", stAttr.u32MaxQp);
                    flag = 1;
                }
            }
            break;
        }
        case 5:   /*MinQP*/
        {
            HI_U32 MinQP_temp;
            while ( !flag )
            {
                printf("\nCurrent MaxQP -> %d , MinQP -> %d\n", stAttr.u32MaxQp, stAttr.u32MinQp);
                printf("Active Change the MinQP to -> ");
                fflush(stdin);
                scanf("%d", &MinQP_temp);
                if ( MinQP_temp > 51 || MinQP_temp >= stAttr.u32MaxQp)
                {
                    printf("\nPlease input the MinQP again!(should < 51 && < MaxQP(%d))\n", stAttr.u32MaxQp);
                }
                else
                {
                    stAttr.u32MinQp =  MinQP_temp;
                    printf("\nThe new MinQP -> %d\n", stAttr.u32MinQp);
                    flag = 1;
                }
            }
            break;
        }
        case 6:   /*channel Priority*/
        {
            HI_U32 Priority_temp;
            while ( !flag )
            {
                printf("\nCurrent channel Priority -> %d\n", stAttr.u8Priority);
                printf("Active Change the channel Priority to(should be 0~%d) -> ", VENC_MAX_CHN - 1);
                fflush(stdin);
                scanf("%d", &Priority_temp);
                if ( Priority_temp > VENC_MAX_CHN - 1)
                {
                    printf("\nPlease input the Priority again!(should be 0~%d)\n", VENC_MAX_CHN - 1);
                }
                else
                {
                    stAttr.u8Priority =  Priority_temp;
                    printf("\nThe new Priority of channel(0x%x) -> %d\n", hVenc, stAttr.u8Priority);
                    flag = 1;
                }
            }
            break;
        }
        default:
            printf("\nUnknow Cmd(%d)\n", cmd);
            break;
    }

    ret = HI_UNF_VENC_SetAttr(hVenc, &stAttr);
    if (HI_SUCCESS == ret)
    {
        printf("\nCmd(%d) change OK!!\n", cmd);
    }
    return ret;
}

#endif

static void loop_function(HI_S32 chnNum)
{
    HI_S32  cmd_error = 0;

    while (cmd_error == 0)
    {
        HI_S32  cmd_id = -1;
        HI_S32  channel_id = -1;
        HI_CHAR inputCmd;

        while ( (cmd_id > 7) || (cmd_id < 0) )
        {
            if (cmd_id > 7)
            {
                printf("\nPlease input the  correct Cmd again!\n\n");
            }
            printf("\n*************  input 'q' to exit!**************\n");
            printf("please choose the Channel ID(0~%d) to control :", chnNum - 1);
            fflush(stdin);

            do
            {
                scanf("%c", &inputCmd);
            } while (inputCmd == 10);

            if (('q' == inputCmd) || ('Q' == inputCmd) )
            {
                printf("test-case exit!!\n");
                //cmd_error = 1;
                return ;
            }

            if ((inputCmd >= '0') && (inputCmd <= '9'))
            {
                channel_id = inputCmd - 48;
            }
            else
            {
                printf("\nPlease input the  Channel ID again!\n\n");
                continue;
            }

            if ((channel_id >= chnNum) || (channel_id < 0))
            {
                printf("\nPlease input the  Channel ID again!\n\n");
                continue;
            }

            printf("*********************************************\n");
            printf("please choose the Cmd as follows :\n");
            printf("*********************************************\n");
            printf("0 --> active change the Input FrameRate\n");
            printf("1 --> active change the Target FrameRate\n");
            printf("2 --> active change the Target BitRate\n");
            printf("3 --> active change the GOP\n");
            printf("4 --> active change the MaxQP\n");
            printf("5 --> active change the MinQP\n");
            printf("6 --> active change the priority\n");
            printf("7 --> exit \n");
            fflush(stdin);
            scanf("%d", &cmd_id);
        }

        if (7 == cmd_id)
        {
            printf("test-case exit!!\n");
            cmd_error = 1;
            break;
        }

        cmd_error = process_current_command(g_hVenc[channel_id], cmd_id);
        cmd_id = -1;
    }
}

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 ret;
    HI_UNF_VENC_CHN_ATTR_S stVeAttr;
    //HI_HANDLE hVenc[VENC_MAX_CHN];
    HI_S32 i = 0;
    HI_CHAR FileName[64] = {""};
    HI_U32 chn_num = 0;
    SAMPLE_THREAD_INFO_S thread_arg;
    HI_U32 chn_width[VENC_MAX_CHN] = {0};
    HI_U32 chn_height[VENC_MAX_CHN] = {0};
    HI_U32 chn_priority[VENC_MAX_CHN] = {0};
    HI_UNF_VCODEC_TYPE_E VencType[VENC_MAX_CHN] = {0};

    HI_CHAR inputFileName[VENC_MAX_CHN][20] = {};
    if ( 2 == argc )
    {
        if (!strcmp("user-defined", argv[1]))
        {
            printf("input the num of VENC channel:");
            fflush(stdin);
            scanf("%d", &chn_num);
            if (chn_num > VENC_MAX_CHN)
            {
                printf("ERROR:: the num of VENC channel is invalid !(should < %d)", VENC_MAX_CHN);
                return 0;
            }
            thread_arg.RealChnNum = chn_num;
            for (i = 0; i < chn_num; i++ )
            {
                printf("input the channel of type[%d] (0->h264 1->mjpeg 2->jpeg 3->h265)    :", i);
                fflush(stdin);
                scanf("%d", &g_JpegType[i]);

                printf("\ninput the Input Filename of channel[%d]:", i);
                fflush(stdin);
                scanf("%s", &(inputFileName[i][0]));
                thread_arg.pFileInput[i]  = fopen(inputFileName[i], "rb");
                if (NULL == thread_arg.pFileInput[i])
                {
                    printf("ERROR:open file :%s failed!!\n", inputFileName[i]);               //liminqi
                    return 0;
                }

                strncpy(Output_Filename_table[i], inputFileName[i], (strlen(inputFileName[i]) - 4));

                if (g_JpegType[i] == 1)
                {
                    VencType[i] = HI_UNF_VCODEC_TYPE_JPEG;
                    snprintf(FileName, sizeof(FileName), "_chn%d.mjpeg", i);
                    strncat(Output_Filename_table[i], FileName, strlen(FileName));
                    Output_Filename_table[i][127] = '\0';
                }
                else if (g_JpegType[i] == 0)
                {
                    VencType[i] = HI_UNF_VCODEC_TYPE_H264;
                    snprintf(FileName, sizeof(FileName), "_chn%d.h264", i);
                    strncat(Output_Filename_table[i], FileName, strlen(FileName));
                    Output_Filename_table[i][127] = '\0';
                }
                else if (g_JpegType[i] == 2)
                {
                    VencType[i] = HI_UNF_VCODEC_TYPE_JPEG;
                    snprintf(FileName, sizeof(FileName), "_chn%d", i);
                    strncat(Output_Filename_table[i], FileName, strlen(FileName));
                    Output_Filename_table[i][127] = '\0';
                }
                else if (g_JpegType[i] == 3)
                {
                    VencType[i] = HI_UNF_VCODEC_TYPE_HEVC;
                    snprintf(FileName, sizeof(FileName), "_chn%d.h265", i);
                    strncat(Output_Filename_table[i], FileName, strlen(FileName));
                    Output_Filename_table[i][127] = '\0';
                }

                if (g_JpegType[i] != 2)
                {
                    thread_arg.pFileOutput[i]  = fopen(Output_Filename_table[i], "wb+");
                    if (NULL == thread_arg.pFileOutput[i])
                    {
                        printf("ERROR:open file :%s failed!!\n", Output_Filename_table[i]);               //liminqi
                        return 0;
                    }
                }

                printf("input the Picture Width of channel[%d]:", i);
                fflush(stdin);
                scanf("%d", &thread_arg.u32PicWitdth[i]);

                printf("input the Picture Height of channel[%d]:", i);
                fflush(stdin);
                scanf("%d", &thread_arg.u32PicHeight[i]);

                printf("\ninput the Encode Width of channel[%d]:", i);
                fflush(stdin);
                scanf("%d", &chn_width[i]);

                printf("input the Encode Height of channel[%d]:", i);
                fflush(stdin);
                scanf("%d", &chn_height[i]);

                printf("input the priority of channel[%d]:", i);
                fflush(stdin);
                scanf("%d", &chn_priority[i]);
            }
        }
        else
        {
            goto error_0;
        }
    }
    else if (argc == 5)
    {
        chn_num = 1;
        thread_arg.RealChnNum = chn_num;
        thread_arg.u32PicWitdth[0] = strtol(argv[2], NULL, 0);
        thread_arg.u32PicHeight[0] = strtol(argv[3], NULL, 0);
        chn_width[0]               = strtol(argv[2], NULL, 0);
        chn_height[0]              = strtol(argv[3], NULL, 0);
        chn_priority[0]            = 0;

        if(!strcmp(argv[4], "H264"))
        {
            VencType[0] = HI_UNF_VCODEC_TYPE_H264; 
            printf("H264 Type is success!\n");
        }
        else if (!strcmp(argv[4], "H265"))
        {
            VencType[0] = HI_UNF_VCODEC_TYPE_HEVC;
            printf("H265 Type is success!\n");
        }
        else 
        {
            printf("Please input the VencType H264 or H265!\n");

            return 0;
        }

        g_JpegType[0]              = 0;
        strncpy (Input_Filename_table[0], argv[1], 127);
        Input_Filename_table[0][127] = '\0';
        strncpy(Output_Filename_table[0], argv[1], (strlen(argv[1]) - 4));

        if (VencType[0] == HI_UNF_VCODEC_TYPE_H264)
        {
            strncat(Output_Filename_table[i], "_chn00.h264", strlen("_chn00.h264"));
        }
        else
        {
            strncat(Output_Filename_table[i], "_chn00.h265", strlen("_chn00.h265"));  //c00346155
        }

        Output_Filename_table[0][127] = '\0';
        thread_arg.pFileInput[0]   = fopen(Input_Filename_table[0], "rb");
        if (NULL == thread_arg.pFileInput[0])
        {
            printf("ERROR:open file :%s failed!!\n", inputFileName[0]);               //liminqi
            return 0;
        }
        thread_arg.pFileOutput[0]  = fopen(Output_Filename_table[0], "wb+");
        if (NULL == thread_arg.pFileOutput[0])
        {
            printf("ERROR:open file :%s failed!!\n", Output_Filename_table[0]);               //liminqi
            return 0;
        }
    }
    else
    {
        goto error_0;
    }
    printf("*********************************************\n");
    printf("please enter input YUV store/sample type\n");
    printf("*********************************************\n");
    printf("0 --> SP420_YCbCr\n");
    printf("1 --> SP420_YCrCb\n");
    printf("2 --> Planner 420\n");
    printf("3 --> Package422_YUYV\n");
    printf("4 --> Package422_YVYU\n");
    printf("5 --> Package422_UYVY\n");
    printf("6 --> SP422_YCbCr\n");
    printf("7 --> SP422_YCrCb\n");

    fflush(stdin);
    scanf("%d", &YUV_store_type);
    fflush(stdin);
    if (YUV_store_type >= YUV_UNKNOW_BUIT)
    {
        printf("Wrong option of YUV store/sample type! type(%d) not support!\n", YUV_store_type);
        return -1;
    }

    HI_SYS_Init();

    HIAPI_RUN_RETURN(HI_UNF_VENC_Init());

    for (i = 0; i < chn_num; i++)
    {
        HI_UNF_VENC_GetDefaultAttr(&stVeAttr);

        stVeAttr.u32Width   = chn_width[i];
        stVeAttr.u32Height  = chn_height[i];
        stVeAttr.u8Priority = chn_priority[i];

        stVeAttr.u32Gop          = 100;
        stVeAttr.enVencType      = VencType[i];
        stVeAttr.u32RotationAngle = 0;
        stVeAttr.bSlcSplitEn     = HI_FALSE;
        stVeAttr.bQuickEncode    = 0;

        if (stVeAttr.u32Width > 1280)
        {
            stVeAttr.enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_FULLHD;
            stVeAttr.u32StrmBufSize   = 1920 * 1080 * 2;
            stVeAttr.u32TargetBitRate = 5 * 1024 * 1024;
        }
        else if (stVeAttr.u32Width > 720)
        {
            stVeAttr.enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_720P;
            stVeAttr.u32StrmBufSize   = 1280 * 720 * 2;
            stVeAttr.u32TargetBitRate = 3 * 1024 * 1024;
        }
        else if (stVeAttr.u32Width > 352)
        {
            stVeAttr.enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_D1;
            stVeAttr.u32StrmBufSize   = 720 * 576 * 2;
            stVeAttr.u32TargetBitRate = 3 / 2 * 1024 * 1024;
        }
        else
        {
            stVeAttr.enCapLevel       = HI_UNF_VCODEC_CAP_LEVEL_CIF;
            stVeAttr.u32StrmBufSize   = 352 * 288 * 2;
            stVeAttr.u32TargetBitRate = 800 * 1024;
        }

        stVeAttr.u32InputFrmRate  = 25;
        stVeAttr.u32TargetFrmRate = 25;
        stVeAttr.u32MaxQp         = 48;
        stVeAttr.u32MinQp         = 16;
        stVeAttr.bQuickEncode     = HI_FALSE;

        if ((g_JpegType[i] == 2) || (g_JpegType[i] == 1))
        {
            stVeAttr.enVencType   = HI_UNF_VCODEC_TYPE_JPEG;
            stVeAttr.u32Qlevel    = 70;
        }
        HI_ADP_VENC_Create(&stVeAttr, &g_hVenc[i]);

        thread_arg.hVenc[i] = g_hVenc[i];
        thread_arg.u32TimeOut[i] = 0;
        if (HI_SUCCESS != AllocFrameBuf(i, stVeAttr.u32Width, stVeAttr.u32Height))
        {
            return -1;
        }
        
        HI_ADP_VENC_Start(g_hVenc[i]);
    }

    HI_ADP_VENC_Thread_Create(&thread_arg);

    sleep(1);

    /////////////////////////////////////////

    loop_function(chn_num);

    /////////////////////////////////////////  收尾工作

    HI_ADP_VENC_Thread_Stop(chn_num);

    for (i = 0; i < chn_num; i++)
    {
        HI_ADP_VENC_Stop(g_hVenc[i]);
        HI_ADP_VENC_Destroy(g_hVenc[i]);  
        if (g_JpegType[i] != 2)
        {
            fclose(thread_arg.pFileOutput[i]);
        }
        fclose(thread_arg.pFileInput[i]);

        FreeFrameBuf(i);
    }

    

    HIAPI_RUN(HI_UNF_VENC_DeInit(), ret);

    ret = HI_SYS_DeInit();                                            //liminqi
    if (HI_SUCCESS != ret)
    {
        printf("call HI_SYS_DeInit() failed.\n");
    }

    return ret;

error_0:
    printf("\n      Usage1 : %s filename.yuv  Picture_Width  Picture_Height  Encode_Type\n", argv[0]);
	printf("      Example: %s a_1280x720.yuv  1280  720  H265\n", argv[0]);
    printf("      Usage2: %s user-defined\n\n", argv[0]);
    return 0;

}
