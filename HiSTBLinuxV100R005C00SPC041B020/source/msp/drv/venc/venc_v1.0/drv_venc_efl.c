#include "drv_venc_efl.h"
#include "drv_omxvenc_efl.h"
#include "drv_venc_osal.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"

#include "hal_venc.h"
#include "hi_drv_log.h"
#include "hi_osal.h"
//#include "drv_vi_ext.h"
//#include "drv_vo_ext.h"

#include "drv_vpss_ext.h"
#include "drv_vdec_ext.h"
#include "hi_drv_vpss.h"

#include "hi_drv_reg.h"
#include "hi_reg_common.h"

#include "hi_drv_stat.h"
#include "hi_drv_sys.h"

#ifdef __VENC_SUPPORT_JPGE__
#include "drv_jpge_ext.h"
#endif

#ifdef __VENC_DPT_ONLY__
#include "hi_math.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static HI_U32 g_idr_pic_id = 0;

//#define __VENC_FREQ_DYNAMIC__

#define SceneChangeLimit 50000
#define SceneChangeLimit2 100000


extern HI_VOID VENC_DRV_BoardInit(HI_VOID);
extern HI_VOID VENC_DRV_BoardDeinit(HI_VOID);

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define Smooth_printk printk
#else
#define Smooth_printk(format, arg...)
#endif

#if (defined __VENC_98CV200_CONFIG__)
#ifdef HI_SMMU_SUPPORT
HI_U32 VeduEfl_SmmuToPhyAddr(HI_U32 PageTabBaseAddr , HI_U32 SmmuAddr  );
#endif
#endif

enum
{
    VENC_YUV_420	= 0,
    VENC_YUV_422	= 1,
    VENC_YUV_444    = 2,
    VENC_YUV_NONE   = 3
};

enum
{
    VENC_V_U        = 0,
    VENC_U_V    	= 1
};


enum
{
    VENC_STORE_SEMIPLANNAR	= 0,
    VENC_STORE_PACKAGE   	= 1,
    VENC_STORE_PLANNAR      = 2
};

enum
{
    VENC_PACKAGE_UY0VY1  	= 0b10001101,
    VENC_PACKAGE_Y0UY1V	    = 0b11011000,
    VENC_PACKAGE_Y0VY1U     = 0b01111000
};

enum
{
    VENC_SEMIPLANNAR_420_UV  = 0,
    VENC_SEMIPLANNAR_420_VU  = 1,
    VENC_PLANNAR_420         = 2,
    VENC_PLANNAR_422         = 3,
    VENC_PACKAGE_422_YUYV    = 4,
    VENC_PACKAGE_422_UYVY    = 5,
    VENC_PACKAGE_422_YVYU    = 6,
    VENC_SEMIPLANNAR_422     = 7,
    VENC_UNKNOW              = 8

};

enum
{
    JPGE_STORE_SEMIPLANNAR	= 0,
    JPGE_STORE_PLANNAR   	= 1,
    JPGE_STORE_PACKAGE      = 2
};

/*enum {
	JPGE_ROTATION_0  = 0,
	JPGE_ROTATION_90  = 1,
	JPGE_ROTATION_270 = 2,
	JPGE_ROTATION_180 = 3
};*/

enum
{
    VEDU_CAP_LEVEL_QCIF = 0, /**<The resolution of the picture to be decoded is less than or equal to 176x144.*/ /**<CNcomment: 解码的图像大小不超过176*144 */
    VEDU_CAP_LEVEL_CIF  = 1,      /**<The resolution of the picture to be decoded less than or equal to 352x288.*/ /**<CNcomment: 解码的图像大小不超过352*288 */
    VEDU_CAP_LEVEL_D1   = 2,       /**<The resolution of the picture to be decoded less than or equal to 720x576.*/ /**<CNcomment: 解码的图像大小不超过720*576 */
    VEDU_CAP_LEVEL_720P = 3,     /**<The resolution of the picture to be decoded is less than or equal to 1280x720.*/ /**<CNcomment: 解码的图像大小不超过1280*720 */
    VEDU_CAP_LEVEL_1080P = 4,   /**<The resolution of the picture to be decoded is less than or equal to 1920x1080.*/ /**<CNcomment: 解码的图像大小不超过1920*1080 */
    VEDU_CAP_LEVEL_BUTT
} ;

enum
{
    VENC_FREQUENCY_200M  	= 0,
    VENC_FREQUENCY_150M  	= 2,
    VENC_FREQUENCY_100M     = 3
};

#ifdef __VENC_DRV_DBG__

HI_U32 TmpTime_dbg[100][7] = {{0, 0, 0, 0, 0, 0, 0}};
HI_U32 QueueToVpss_ID = 0;
HI_U32 GetImgFmVpss_ID = 0;
HI_U32 Isr_ID = 0;
HI_U32 PutMsg_EBD_ID = 0;
HI_U32 PutMsg_FBD_ID = 0;
HI_U32 GetMsg_EBD_ID = 0;
HI_U32 GetMsg_FBD_ID = 0;
HI_BOOL flage_dbg = 1;

#endif

static const HI_U8 ZigZagScan[64] =
{
    0,  1,  5,  6, 14, 15, 27, 28,
    2,  4,  7, 13, 16, 26, 29, 42,
    3,  8, 12, 17, 25, 30, 41, 43,
    9, 11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63
};

static const HI_U8 Jpge_Yqt[64] =
{
    16, 11, 10, 16,  24,  40,  51,  61,
    12, 12, 14, 19,  26,  58,  60,  55,
    14, 13, 16, 24,  40,  57,  69,  56,
    14, 17, 22, 29,  51,  87,  80,  62,
    18, 22, 37, 56,  68, 109, 103,  77,
    24, 35, 55, 64,  81, 104, 113,  92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103,  99
};
static const HI_U8 Jpge_Cqt[64] =
{
    17, 18, 24, 47, 99, 99, 99, 99,
    18, 21, 26, 66, 99, 99, 99, 99,
    24, 26, 56, 99, 99, 99, 99, 99,
    47, 66, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99
};


static const HI_U8 Jpge_JfifHdr[698] =  /* 2(SOI)+18(APP0)+207(DQT)+19(SOF)+432(DHT)+6(DRI)+14(SOS) */
{
    0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,
    0xff, 0xdb, 0x00, 0x43, 0x00, 0x10, 0x0b, 0x0c, 0x0e, 0x0c, 0x0a, 0x10, 0x0e, 0x0d, 0x0e, 0x12, 0x11, 0x10, 0x13, 0x18,
    0x28, 0x1a, 0x18, 0x16, 0x16, 0x18, 0x31, 0x23, 0x25, 0x1d, 0x28, 0x3a, 0x33, 0x3d, 0x3c, 0x39, 0x33, 0x38, 0x37, 0x40,
    0x48, 0x5c, 0x4e, 0x40, 0x44, 0x57, 0x45, 0x37, 0x38, 0x50, 0x6d, 0x51, 0x57, 0x5f, 0x62, 0x67, 0x68, 0x67, 0x3e, 0x4d,
    0x71, 0x79, 0x70, 0x64, 0x78, 0x5c, 0x65, 0x67, 0x63, 0xff, 0xdb, 0x00, 0x43, 0x01, 0x11, 0x12, 0x12, 0x18, 0x15, 0x18,
    0x2f, 0x1a, 0x1a, 0x2f, 0x63, 0x42, 0x38, 0x42, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0xff, 0xdb,
    0x00, 0x43, 0x02, 0x11, 0x12, 0x12, 0x18, 0x15, 0x18, 0x2f, 0x1a, 0x1a, 0x2f, 0x63, 0x42, 0x38, 0x42, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63,
    0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0x63, 0xff, 0xc0, 0x00, 0x11, 0x08, 0x01, 0x20, 0x01, 0x60, 0x03, 0x01, 0x22, 0x00,
    0x02, 0x11, 0x01, 0x03, 0x11, 0x02, 0xff, 0xc4, 0x00, 0x1f, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff,
    0xc4, 0x00, 0x1f, 0x01, 0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0xff, 0xc4, 0x00, 0xb5, 0x10, 0x00, 0x02, 0x01,
    0x03, 0x03, 0x02, 0x04, 0x03, 0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05,
    0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1,
    0xc1, 0x15, 0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x53, 0x54,
    0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2,
    0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4,
    0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5,
    0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xc4, 0x00, 0xb5, 0x11,
    0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03,
    0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
    0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25, 0xf1, 0x17,
    0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x73, 0x74,
    0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
    0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xff, 0xdd,
    0x00, 0x04, 0x00, 0x64, 0xff, 0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00
};

/*******************************************************************/

VeduEfl_ChnCtx_S VeduChnCtx[MAX_VEDU_CHN];

VeduEfl_IpCtx_S VeduIpCtx;

extern OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];

extern VEDU_OSAL_EVENT g_VencWait_Stream[VENC_MAX_CHN_NUM];
extern VPSS_EXPORT_FUNC_S* pVpssFunc;
wait_queue_head_t gqueue;
volatile HI_U32 gwait;

extern volatile HI_U32 gencodeframe;
extern wait_queue_head_t gEncodeFrame;

extern spinlock_t g_SendFrame_Lock[VENC_MAX_CHN_NUM];     /*lock the destroy and send frame*/
VEDU_OSAL_EVENT g_VENC_Event;
VEDU_OSAL_EVENT g_VENC_Event_stream;
//extern HI_S32  HI_DRV_VPSS_GetPortFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);           //add
//extern HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);

extern VENC_BUFFER_S g_stKernelOmxmmz[VENC_MAX_CHN_NUM][OMX_OUTPUT_PORT_BUFFER_MAX];
extern HI_U32 g_map_count;
extern HI_U32 g_ummap_count;

#ifdef __VENC_SUPPORT_JPGE__
extern JPGE_EXPORT_FUNC_S* pJpgeFunc;
#endif

extern HI_S8 PriorityTab[2][MAX_VEDU_CHN];                //add by l00228308

#define D_VENC_GET_CHN(u32VeChn, hVencChn) \
    do {\
        if (hVencChn == NULL)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hVEncHandle == hVencChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_CHN_BY_UHND(u32VeChn, hVencUsrChn) \
    do {\
        if (hVencUsrChn == HI_INVALID_HANDLE)\
        {\
            u32VeChn = VENC_MAX_CHN_NUM;\
            break;\
        }\
        u32VeChn = 0; \
        while (u32VeChn < VENC_MAX_CHN_NUM)\
        {   \
            if (g_stVencChn[u32VeChn].hUsrHandle == hVencUsrChn)\
            { \
                break; \
            } \
            u32VeChn++; \
        } \
    } while (0)

#define D_VENC_GET_CHN_FORM_VPSS(hVPSS, hVencChn) \
    do {\
        HI_U32 i = 0; \
        for (i = 0;i < MAX_VEDU_CHN;i++)\
        {   \
            if (g_stVencChn[i].hVPSS == hVPSS)\
            { \
                hVencChn = g_stVencChn[i].hVEncHandle;\
                break; \
            } \
        } \
    } while (0)

#define D_VENC_CHECK_ALL_EQUAL(wh,wt,rh,rt,flag)\
    do{  \
        if( (wh == wt) && (rh == rt) && (wt == rh))\
            flag = HI_TRUE; \
        else               \
            flag = HI_FALSE; \
    } while (0)

#define D_VENC_GET_CHN_FROM_TAB(s32ChnID,TabNO)\
    do{  \
        s32ChnID = PriorityTab[0][TabNO];\
    } while (0)

#define D_VENC_CHECK_ZERO(x, def) \
    do {\
        if ((x) == 0)\
        {\
            HI_ERR_VENC("Error: Division by zero in kernel\n"); \
            x = (def);\
        }\
    } while (0)
//////////add by ckf77439

#define D_VENC_RC_ABS(x,y) (((x) > (y)) ? ((x) - (y)):((y) - (x)))

#define D_VENC_RC_UPDATA(data,curdata) \
    do{\
        *(data + 5) = *(data + 4);\
        *(data + 4) = *(data + 3);\
        *(data + 3) = *(data + 2);\
        *(data + 2) = *(data + 1);\
        *(data + 1) = *data;\
        *data = curdata;\
    }while(0)

#define D_VENC_RC_MAX(x,y) ((x)>(y) ? (x) : (y))
#define D_VENC_RC_MIN(x,y) ((x)>(y) ? (y) : (x))


#define D_VENC_RC_MIN3(x,y,z) (((x) < (y)) ? D_VENC_RC_MIN(x, z) : D_VENC_RC_MIN(y, z))
#define D_VENC_RC_MAX3(x,y,z) (((x) > (y)) ? D_VENC_RC_MAX(x, z) : D_VENC_RC_MAX(y, z))
#define D_VENC_RC_MEDIAN(x,y,z) (((x) + (y) + (z) - D_VENC_RC_MAX3(x, y, z)) - D_VENC_RC_MIN3(x, y, z))

HI_S32 D_VENC_RC_CLIP3(HI_S32 x, HI_S32 y, HI_S32 z)
{
    if (x < y) {x = y;}

    if (x > z) {x = z;}

    return x;
}
void Updata_StreamBuffer(int data[30], int data2)
{
    int i;

    for (i = 29; i > 0; i--)
    {
        data[i] = data[i - 1];
    }

    data[0] = data2;
}
HI_S32 VEDU_DRV_EflRcInitQp(HI_S32 bits, HI_S32 w, HI_S32 h)
{
    HI_S32 Qp;
    HI_S32 BitsPerPoint;
    BitsPerPoint = 100 * bits * 2 / (w * h * 3);

    if (BitsPerPoint > 170) {Qp = 7;}
    else if ((BitsPerPoint > 120) && (BitsPerPoint <= 170)) {Qp = 15;}
    else if (BitsPerPoint > 80 && BitsPerPoint <= 120) {Qp = 20;}
    else if (BitsPerPoint > 40 && BitsPerPoint <= 80) {Qp = 25;}
    else if (BitsPerPoint > 15 && BitsPerPoint <= 40) {Qp = 30;}
    else if (BitsPerPoint > 5 && BitsPerPoint <= 15) {Qp = 38;}  // for test!
    else if (BitsPerPoint > 2 && BitsPerPoint <= 5) {Qp = 40;}
    else {Qp = 43;}

    return Qp;
}


HI_S32 VENC_DRV_EflRcAverage(HI_S32* pData, HI_S32 n)
{
    HI_S32 num = n;
    HI_S32 ave;
    HI_S32 i, sum = 0;

    for (i = 0; i < n; i ++)
    {
        if (*pData == 0)
        {
            num --;
        }

        sum += *pData;
        pData ++;
    }

    if (num == 0) {ave = 0;}
    else
    {
        ave = sum / num;
    }

    return ave;
}


HI_VOID VENC_DRV_EflSceChaDetect(VeduEfl_Rc_S* pRc, HI_S32 w, HI_S32 h, HI_S32 IntraPic)
{
    HI_S32 i;
    HI_S32 DiffData = 0;
    HI_S32 AveY = 0;
    HI_S32 TotalY = 0;//sum of values of all the pixels in one frame
    HI_S32* pCur;
    HI_S32* pPre;
    pRc->SceneChangeFlag = 0;
    pCur = pRc->CurZone;
    pPre = pRc->PreZone;

    for (i = 0; i < 16; i ++)
    {
        DiffData += D_VENC_RC_ABS(*(pCur + i) , *(pPre + i)); //取绝对值
        TotalY = TotalY + (*(pCur + i) * (i * 16 + 8));
    }

    AveY = TotalY / w / h; //average pixel value in current frame

    if ((DiffData > 5 * pRc->AveDiffZone)
        && (DiffData > SceneChangeLimit)
        && (D_VENC_RC_ABS(pRc->PreAveY , AveY) > 5))
    {
        pRc->SceneChangeFlag = 2;

        if (DiffData > SceneChangeLimit2 && DiffData > 5 * pRc->DiffZone[0] )
        {
            pRc->SceneChangeFlag = 1;
            DiffData = pRc->AveDiffZone;
        }
    }

    pRc->PreAveY = VENC_DRV_EflRcAverage(pRc->AverageY, 6);
    D_VENC_RC_UPDATA(pRc->AverageY, AveY);
    D_VENC_RC_UPDATA(pRc->DiffZone, DiffData);
    pRc->AveDiffZone = VENC_DRV_EflRcAverage(pRc->DiffZone, 6);

    while ( i --)
    {
        *pPre ++ = *pCur ++;
    }

}


HI_VOID VENC_DRV_EflRcCalIFrmQp(VeduEfl_Rc_S* pRc, HI_S32 w, HI_S32 h, HI_S32 times, HI_S32 GopLength, int FrameIMBRatio)
{
    HI_S32 TimesDelta = 0;

    if (pRc->StillFrameNum >= 30)
    {
        TimesDelta = 4;

        if (pRc->StillFrameNum2 >= 30)
        {
            TimesDelta = 6;
        }
    }

    times = times + TimesDelta;
    times = times + pRc->DeltaTimeOfP;
    times = D_VENC_RC_MAX(times, pRc->MinTimeOfP);
    times = D_VENC_RC_MIN(times, pRc->MaxTimeOfP);

    if (GopLength <= 1000)
    {
        pRc->TargetBits = pRc->GopBits / (times + GopLength - 1) * times;
    }
    else
    {
        pRc->TargetBits = times * pRc->AveFrameBits;
    }

    if (pRc->ITotalBits[0])
    {
        pRc->TargetBits = D_VENC_RC_MIN(pRc->TargetBits, pRc->ITotalBits[0] * 15 / 10);
    }


    if (pRc->SenChaNum[0] > 0)
    {
        pRc->CurQp = pRc->PreQp;
    }
    else
    {
        if (pRc->TargetBits > pRc->ITotalBits[0])
        {
            pRc->CurQp = -6 * (pRc->TargetBits - pRc->ITotalBits[0]) / pRc->ITotalBits[0] + pRc->IPreQp[0];
        }
        else
        {
            pRc->CurQp = -6 * (pRc->TargetBits - pRc->ITotalBits[0]) / pRc->TargetBits + pRc->IPreQp[0] + 1;
        }

        pRc->CurQp = D_VENC_RC_CLIP3(pRc->CurQp, pRc->IPreQp[0] - 6, pRc->IPreQp[0] + 8);

    }

    pRc->CurQp = D_VENC_RC_MAX(pRc->CurQp, pRc->PreQp - 6);

    if (pRc->SceneChangeFlag)
    {
        pRc->CurQp = D_VENC_RC_MAX(pRc->CurQp, pRc->PreQp + 3);
    }

    //pRc->CurQp = D_VENC_RC_MAX(pRc->CurQp, pRc->PreQp-3);
}

HI_VOID VENC_DRV_EflRcProcGop(VeduEfl_Rc_S* pRc, HI_S32 FrameIMBRatio)
{
    if ((pRc->TotalBitsLeft - pRc->GopBitsLeft + pRc->TotalTargetBitsUsedInGop - pRc->TotalBitsUsedInGop + pRc->GopBits * 4 / 10 > 0)
        && (pRc->TotalTargetBitsUsedInGop * 14 / 10 - pRc->TotalBitsUsedInGop > 0)
        && (pRc->StillMoveNum < 12) && (pRc->AveMeanQp <= 46)
        && ((pRc->ConsAvePBits - pRc->AvePBits * pRc->AveFrameMinusAveP / 10) < 0))
    {
        if ((FrameIMBRatio > 15) && (pRc->PreMeanQp >= 40))
        {
            pRc->StillToMove = 6;
        }
        else
        {
            pRc->StillToMove --;
        }

        if ((pRc->StillToMove >= 1) && (FrameIMBRatio > 15 || pRc->PreMeanQp >= 43))
        {
            pRc->TargetBits = ((100 + FrameIMBRatio) * D_VENC_RC_MAX(pRc->TargetBits, pRc->AveFrameBits) + 50) / 100;
            pRc->TargetBits = D_VENC_RC_MAX(pRc->TargetBits, 12 * pRc->PreTargetBits / 10);
        }

        if ((pRc->StillToMove >= 1) && (pRc->PreMeanQp <= 38))
        {
            pRc->TargetBits = pRc->TargetBits * 9 / 10;
        }

        if (FrameIMBRatio > 30)
        {
            pRc->CurQp = pRc->CurQp - 1;
        }

        if (pRc->CurQp > 43)
        {
            pRc->CurQp = pRc->CurQp - 1;
        }
    }
    else
    {
        pRc->StillToMove --;
    }
}

HI_VOID VENC_DRV_EflRcProcQp(VeduEfl_Rc_S* pRc, VeduEfl_EncPara_S* pEncPara)
{
    HI_S32 ratio, ratio2;
    HI_S32 PrePicBits = pEncPara->PicBits;
    
    if (100 * PrePicBits < 105 * pRc->PreTargetBits)
    {
        if (100 * PrePicBits > 90 * pRc->PreTargetBits)
        {
            pRc->CurQp = pRc->PreQp;
        }
        else
        {
            ratio = (pRc->PreTargetBits - PrePicBits) * 100 / pRc->PreTargetBits;

            if (ratio >= 40)
            {
                pRc->CurQp = pRc->PreQp - 3;
            }
            else if ((ratio > 20) && (ratio < 40))
            {
                pRc->CurQp = pRc->PreQp - 2;
            }
            else
            {
                pRc->CurQp = pRc->PreQp - 1;
            }

            pRc->CurQp = D_VENC_RC_MIN(pRc->PreMeanQp, pRc->CurQp);
        }
    }
    else
    {
        ratio2 = (PrePicBits - pRc->PreTargetBits) * 100 / pRc->PreTargetBits;
        ratio2 = D_VENC_RC_MIN(ratio2, 200);
        pRc->CurQp = pRc->PreQp + (ratio2 * 5) / 100 + 1;
        pRc->CurQp = D_VENC_RC_MIN(pRc->CurQp, 51);
    }

    if ((pRc->PreMeanQp - pRc->PreQp) > 5)
    {
        pRc->CurQp = pRc->CurQp + 1;
    }

    if (pEncPara->stBitsFifo.Sum * 100 > pEncPara->BitRate * 110)
    {
        pRc->TargetBits = pRc->ConsAvePBits * 9 / 10;
    }
    else if (pEncPara->stBitsFifo.Sum * 100 < pEncPara->BitRate * 90)
    {
        pRc->TargetBits = pRc->ConsAvePBits * 11 / 10;
    }
    else
    {
        pRc->TargetBits = pRc->ConsAvePBits;
    }
}


HI_VOID VENC_DRV_EflRcCalPFrmQp(VeduEfl_Rc_S* pRc, VeduEfl_EncPara_S* pEncPara, HI_S32 FrameIMBRatio)
{
    HI_S32 Gop = pEncPara->Gop;    

    VENC_DRV_EflRcProcQp(pRc, pEncPara);

    if (Gop <= 1000)
    {
        VENC_DRV_EflRcProcGop(pRc, FrameIMBRatio);
    }

    if ((pRc->CurQp < pRc->PreQp) && (pRc->PreQp > pRc->PPreQp[0]))
    {
        if ((pRc->PreQp > 0) && (pRc->PPreQp[0] > 0))
        {
            pRc->CurQp = (pRc->CurQp + pRc->PreQp - 1) / 2;
        }
    }

    if ((pRc->CurQp > pRc->PreQp) && (pRc->PreQp < pRc->PPreQp[0]))
    {
        if ((pRc->PreQp > 0) && (pRc->PPreQp[0] > 0))
        {
            pRc->CurQp = (pRc->PreQp + pRc->PPreQp[0] + 1) / 2;
        }
    }

    if ((Gop > 10) && (pRc->FrmNumInGop > Gop - 10))
    {
        if (pRc->FrmNumInGop % 2 == 0)
        {
            pRc->CurQp = pRc->PreQp + 1;
            pRc->TargetBits = pRc->PreTargetBits * 95 / 100;
        }
        else
        {
            pRc->CurQp = pRc->PreQp;
            pRc->TargetBits = pRc->PreTargetBits;
        }
    }
}

/////////////add end

static HI_VOID H264e_PutTrailingBits(tBitStream* pBS)
{
    VENC_DRV_BsPutBits31(pBS, 1, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0, 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_VOID MP4e_PutTrailingBits(tBitStream* pBS)
{
    VENC_DRV_BsPutBits31(pBS, 0, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0xFF >> (pBS->totalBits & 7), 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_U32 H264e_MakeSPS(HI_U8* pSPSBuf, const VeduEfl_H264e_SPS_S* pSPS)
{
    HI_U32 code, TotalMb, profile_idc, level_idc, direct_8x8_interence_flag;
    int bits;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32*)pSPSBuf);

    TotalMb = pSPS->FrameWidthInMb * pSPS->FrameHeightInMb;

    if (TotalMb <= 99)
    {
        level_idc = 10;
    }
    else if (TotalMb <= 396)
    {
        level_idc = 20;
    }
    else if (TotalMb <= 792)
    {
        level_idc = 21;
    }
    else if (TotalMb <= 1620)
    {
        level_idc = 30;
    }
    else if (TotalMb <= 3600)
    {
        level_idc = 31;
    }
    else if (TotalMb <= 5120)
    {
        level_idc = 32;
    }
    else if (TotalMb <= 8192)
    {
        level_idc = 41;
    }
    else
    {
        level_idc = 0;
    }

    if (TotalMb < 1620)
    {
        direct_8x8_interence_flag = 0;
    }
    else
    {
        direct_8x8_interence_flag = 1;
    }


    //profile_idc = 66;
    profile_idc = pSPS->ProfileIDC;

    VENC_DRV_BsPutBits32(&BS, 1, 32);

    VENC_DRV_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    VENC_DRV_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    VENC_DRV_BsPutBits31(&BS, 7, 5); // nal_unit_type

    VENC_DRV_BsPutBits31(&BS, profile_idc, 8);
    VENC_DRV_BsPutBits31(&BS, 0x00, 8);
    VENC_DRV_BsPutBits31(&BS, level_idc, 8);

    VENC_DRV_BsPutBits31(&BS, 1, 1); // ue, sps_id = 0

    if (100 == pSPS->ProfileIDC)  //just for high profile
    {
        VENC_DRV_BsPutBits31(&BS, 0x2, 3);
        VENC_DRV_BsPutBits31(&BS, 0xC, 4);
    }

    VENC_DRV_BsPutBits31(&BS, 1, 1); // ue, log2_max_frame_num_minus4 = 0

    VENC_DRV_BsPutBits31(&BS, 3, 3); // ue, pic_order_cnt_type = 2
    VENC_DRV_BsPutBits31(&BS, 2, 3); // ue, num_ref_frames = 1 (or 2)
    VENC_DRV_BsPutBits31(&BS, 0, 1); // u1, gaps_in_frame_num_value_allowed_flag

    ue_vlc(bits, code, (pSPS->FrameWidthInMb - 1));
    VENC_DRV_BsPutBits31(&BS, code, bits);
    ue_vlc(bits, code, (pSPS->FrameHeightInMb - 1));
    VENC_DRV_BsPutBits31(&BS, code, bits);

    VENC_DRV_BsPutBits31(&BS, 1, 1); // u1, frame_mbs_only_flag = 1 (or 0)

    if (0)                // !pSPS->FrameMbsOnlyFlag
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1); // mb_adaptive_frame_field_flag = 0
        VENC_DRV_BsPutBits31(&BS, 1, 1); // direct_8x8_inference_flag
    }
    else
    {
        VENC_DRV_BsPutBits31(&BS, direct_8x8_interence_flag, 1); // direct_8x8_inference_flag
    }

    {
        int bFrameCropping = ((pSPS->FrameCropLeft | pSPS->FrameCropRight |
                               pSPS->FrameCropTop | pSPS->FrameCropBottom) != 0);

        VENC_DRV_BsPutBits31(&BS, bFrameCropping, 1);

        if (bFrameCropping)
        {
            ue_vlc(bits, code, pSPS->FrameCropLeft);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropRight);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropTop);
            VENC_DRV_BsPutBits31(&BS, code, bits);
            ue_vlc(bits, code, pSPS->FrameCropBottom);
            VENC_DRV_BsPutBits31(&BS, code, bits);
        }
    }

    VENC_DRV_BsPutBits31(&BS, 0, 1); // vui_parameters_present_flag
    H264e_PutTrailingBits(&BS);

    HI_INFO_VENC("VENC h264 profile_id:%d,level_id:%d,num_ref_frames: %d\n", profile_idc, level_idc, 1);
    return (HI_U32)BS.totalBits;
}

static HI_U32 H264e_MakePPS(HI_U8* pPPSBuf, const VeduEfl_H264e_PPS_S* pPPS)
{
    HI_U32 code;
    int bits;
    HI_U32 b = pPPS->H264CabacEn ? 1 : 0;

    tBitStream BS;

#ifdef __VENC_S40V200_CONFIG__
    HI_U8 zz_scan_table[64] =
    {
        0,  1,  8, 16,  9,  2,  3, 10,
        17, 24, 32, 25, 18, 11,  4,  5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13,  6,  7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63
    };
#endif

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32*)pPPSBuf);

    VENC_DRV_BsPutBits32(&BS, 1, 32);

    VENC_DRV_BsPutBits31(&BS, 0, 1); // forbidden_zero_bit
    VENC_DRV_BsPutBits31(&BS, 3, 2); // nal_ref_idc
    VENC_DRV_BsPutBits31(&BS, 8, 5); // nal_unit_type

    VENC_DRV_BsPutBits31(&BS, 1, 1); // pps_id = 0
    VENC_DRV_BsPutBits31(&BS, 1, 1); // sps_id = 0

    VENC_DRV_BsPutBits31(&BS, b, 1); // entropy_coding_mode_flag = 0
    VENC_DRV_BsPutBits31(&BS, 0, 1); // pic_order_present_flag
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_slice_groups_minus1
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_l0_active_minus1 = 0 (or 1)
    VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_l1_active_minus1 = 0
    VENC_DRV_BsPutBits31(&BS, 0, 3); // weighted_pred_flag & weighted_bipred_idc
    VENC_DRV_BsPutBits31(&BS, 3, 2); // pic_init_qp_minus26 & pic_init_qs_minus26

    se_vlc(bits, code, pPPS->ChrQpOffset); // chroma_qp_index_offset
    VENC_DRV_BsPutBits31(&BS, code, bits);

    VENC_DRV_BsPutBits31(&BS, 1, 1);                // deblocking_filter_control_present_flag
    VENC_DRV_BsPutBits31(&BS, pPPS->ConstIntra, 1); // constrained_intra_pred_flag

    VENC_DRV_BsPutBits31(&BS, 0, 1);                // redundant_pic_cnt_present_flag

    if (pPPS->H264HpEn)
    {

        int i, j;
#ifdef __VENC_S40V200_CONFIG__
        VENC_DRV_BsPutBits31(&BS, 1, 1); // transform_8x8_mode_flag
#else
        VENC_DRV_BsPutBits31(&BS, 0, 1); // transform_8x8_mode_flag
#endif
        VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_matrix_present_flag

        for (i = 0; i < 6; i++)
        {
            VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag

            se_vlc(bits, code, 8);
            VENC_DRV_BsPutBits31(&BS, code, bits); /* all be 16 */

            for (j = 0; j < 15; j++)
            {
                VENC_DRV_BsPutBits31(&BS, 1, 1);
            }
        }

#ifdef __VENC_S40V200_CONFIG__

        for (i = 0; i < 2; i++)
        {
            int lastScale, currScale, deltaScale;
            HI_S32* pList = pPPS->pScale8x8;

            if (i == 1) { pList = pPPS->pScale8x8 + 64; }

            VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag

            for (lastScale = 8, j = 0; j < 64; j++)
            {
                currScale  = (int)(pList[zz_scan_table[j]]);
                deltaScale = currScale - lastScale;

                if     (deltaScale < -128) { deltaScale += 256; }
                else if (deltaScale >  127) { deltaScale -= 256; }

                se_vlc(bits, code, deltaScale);
                VENC_DRV_BsPutBits31(&BS, code, bits);
                lastScale = currScale;
            }
        }

#endif
        se_vlc(bits, code, pPPS->ChrQpOffset);
        VENC_DRV_BsPutBits31(&BS, code, bits);
    }

    H264e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_U32 H264e_MakeSlcHdr(HI_U32* pHdrBuf, HI_U32* pReorderBuf, HI_U32* pMarkBuf,
                               const VeduEfl_H264e_SlcHdr_S* pSlcHdr)
{
    HI_U32 code   = 0;
    HI_U32 buf[8] = {0};
    int bits, i, bitPara;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    ue_vlc(bits, code, pSlcHdr->slice_type);
    VENC_DRV_BsPutBits31(&BS, code, bits);                                        // slice_type, 0(P) or 2(I)

    VENC_DRV_BsPutBits31(&BS, 1, 1);                      // pic_parameter_set_id
    VENC_DRV_BsPutBits31(&BS, pSlcHdr->frame_num & 0xF, 4); // frame number

    if (pSlcHdr->slice_type == 2) // all I Picture be IDR
    {
        ue_vlc(bits, code, g_idr_pic_id & 0xF);
        VENC_DRV_BsPutBits31(&BS, code, bits);
        g_idr_pic_id++;
    }
    else if (pSlcHdr->NumRefIndex ==  0)
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1); // num_ref_idx_active_override_flag
    }
    else
    {
        VENC_DRV_BsPutBits31(&BS, 1, 1); // num_ref_idx_active_override_flag
        ue_vlc(bits, code, pSlcHdr->NumRefIndex);
        VENC_DRV_BsPutBits31(&BS, code, bits);
    }

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);

        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    bitPara = BS.totalBits;

    /****** RefPicListReordering() ************************************/

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits31(&BS, 0, 1);/* ref_pic_list_reordering_flag_l0 = 0 ("0") */

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pReorderBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);

        if (bits < 32)
        {
            pReorderBuf[i] >>= (32 - bits);
        }
    }

    bitPara |= BS.totalBits << 8;

    /****** DecRefPicMarking() *****************************************/

    VENC_DRV_BsOpenBitStream(&BS, buf);

    if (pSlcHdr->slice_type == 2)
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* no_output_of_prior_pics_flag */
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* long_term_reference_flag     */
    }
    else
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);/* adaptive_ref_pic_marking_mode_flag */
    }

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pMarkBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);

        if (bits < 32)
        {
            pMarkBuf[i] >>= (32 - bits);
        }
    }

    bitPara |= BS.totalBits << 16;
    return (HI_U32)bitPara;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_U32 H263e_MakePicHdr(HI_U32* pHdrBuf, const VeduEfl_H263e_PicHdr_S* pPicHdr)
{
    HI_U32 buf[8] = {0};
    int bits, i;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits31(&BS, 0x20, 22);             // start code
    VENC_DRV_BsPutBits31(&BS, pPicHdr->TR & 0xFF, 8); // temporal ref
    VENC_DRV_BsPutBits31(&BS, 0x10, 5);              // PTYPE bit 1-5
    VENC_DRV_BsPutBits31(&BS, 7, 3);                 // PTYPE bit 6-8

    /* PLUSPTYPE */
    VENC_DRV_BsPutBits31(&BS, 1, 3);                 // UFEP
    VENC_DRV_BsPutBits31(&BS, pPicHdr->SrcFmt, 3);   // OPPTYPE: Source Format
    VENC_DRV_BsPutBits31(&BS, 0, 11);                // OPPTYPE: C-PCF ~ MQ
    VENC_DRV_BsPutBits31(&BS, 8, 4);                 // OPPTYPE: Reserved
    VENC_DRV_BsPutBits31(&BS, pPicHdr->Pframe, 3);   // MPPTYPE: Pic Type - 0:I, 1:P
    VENC_DRV_BsPutBits31(&BS, 0, 3);                 // MPPTYPE: RPR, RRU, Rounding
    VENC_DRV_BsPutBits31(&BS, 1, 3);                 // MPPTYPE: Reserved

    VENC_DRV_BsPutBits31(&BS, 0, 1);                 // CPM

    if (pPicHdr->SrcFmt == 6)
    {
        VENC_DRV_BsPutBits31(&BS, 2, 4);
        VENC_DRV_BsPutBits31(&BS, pPicHdr->Wframe / 4 - 1, 9);
        VENC_DRV_BsPutBits31(&BS, 1, 1);
        VENC_DRV_BsPutBits31(&BS, pPicHdr->Hframe / 4, 9);
    }

    VENC_DRV_BsPutBits31(&BS, pPicHdr->PicQP, 5);
    VENC_DRV_BsPutBits31(&BS, 0, 1); // PEI

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);

        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    return (HI_U32)BS.totalBits;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_U32 MP4e_MakeVOL(HI_U8* pVOLBuf, const VeduEfl_MP4e_VOL_S* pVOL)
{
    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32*)pVOLBuf);

    VENC_DRV_BsPutBits32(&BS, 0x0101, 32);  // video_object_start_code
    VENC_DRV_BsPutBits32(&BS, 0x0120, 32);  // video_object_layer_start_code
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // random_accessible_vol
    VENC_DRV_BsPutBits31(&BS, 1, 8);        // video_object_type_indication: simple

    VENC_DRV_BsPutBits31(&BS, 0, 1);        // is_object_layer_identifier: 0
    VENC_DRV_BsPutBits31(&BS, 1, 4);        // aspect_ration_info: 0001

    VENC_DRV_BsPutBits31(&BS, 1, 1);        // vol_control_para: 1
    VENC_DRV_BsPutBits31(&BS, 1, 2);        // chroma_format: 01
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // low_dalay
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // vbv_para

    VENC_DRV_BsPutBits31(&BS, 0, 2);        // vol_shape: 00
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit
    VENC_DRV_BsPutBits31(&BS, 25, 16);      //?vop_time_increment_resolution
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // fixed_vop_rate
    //VENC_DRV_BsPutBits31(&BS, 1001, 15);      //?fixed_vop_time_increment
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // marker_bit

    VENC_DRV_BsPutBits31(&BS, pVOL->Wframe, 13);
    VENC_DRV_BsPutBits31(&BS, 1, 1);
    VENC_DRV_BsPutBits31(&BS, pVOL->Hframe, 13);
    VENC_DRV_BsPutBits31(&BS, 1, 1);
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // interlaced
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // obmc_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // sprite_enable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // not_8_bit

    VENC_DRV_BsPutBits31(&BS, 0, 1);        // quant_type
    VENC_DRV_BsPutBits31(&BS, 1, 1);        // complexity_estimation_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // resync_marker_disable
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // data_partitioned
    VENC_DRV_BsPutBits31(&BS, 0, 1);        // scalabilitu

    MP4e_PutTrailingBits(&BS);
    return (HI_U32)BS.totalBits;
}

static HI_U32 MP4e_MakeVopHdr(HI_U32* pHdrBuf, const VeduEfl_MP4e_VopHdr_S* pVopHdr)
{
    HI_U32 buf[8] = {0};
    int bits, i;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, buf);

    VENC_DRV_BsPutBits32(&BS, 0x01B6, 32);           // start code
    VENC_DRV_BsPutBits31(&BS, pVopHdr->Pframe, 2);   // vop_coding_type - 0:I, 1:p
    VENC_DRV_BsPutBits31(&BS, 0, 1);                 //?modulo_time_base
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // marker_bit
    VENC_DRV_BsPutBits31(&BS, 0, 5);                 //?vop_time_increment
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // marker_bit
    VENC_DRV_BsPutBits31(&BS, 1, 1);                 // vop_coded

    if (pVopHdr->Pframe)
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);
    }                                     // vop_rounding_type

    VENC_DRV_BsPutBits31(&BS, 0, 3);                 // intra_dc_vlc_thr = 000
    VENC_DRV_BsPutBits31(&BS, pVopHdr->PicQP, 5);

    if (pVopHdr->Pframe)
    {
        VENC_DRV_BsPutBits31(&BS, pVopHdr->Fcode, 3);
    }

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pHdrBuf[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);

        if (bits < 32)
        {
            pHdrBuf[i] >>= (32 - bits);
        }
    }

    return (HI_U32)BS.totalBits;
}

#ifdef __VENC_SUPPORT_JPGE__
static HI_VOID Jpge_MakeQTable(HI_S32 s32Q, HI_U8* pLumaQt, HI_U8* pChromaQt)
{
    HI_S32 i = 0;
    HI_S32 lq = 0;
    HI_S32 cq = 0;
    HI_S32 factor = s32Q;
    HI_S32 q = s32Q;

    if (q < 1)
    {
        factor = 1;
    }

    if (q > 99)
    {
        factor = 99;
    }

    if (q < 50)
    {
        q = 5000 / factor;
    }
    else
    {
        q = 200 - factor * 2;
    }

    /* Calculate the new quantizer */
    for (i = 0; i < 64; i++)
    {
        lq = (Jpge_Yqt[i] * q + 50) / 100;
        cq = (Jpge_Cqt[i] * q + 50) / 100;

        /* Limit the quantizers to 1 <= q <= 255 */
        if (lq < 1) { lq = 1; }
        else if (lq > 255) { lq = 255; }

        pLumaQt[i] = lq;

        if (cq < 1) { cq = 1; }
        else if (cq > 255) { cq = 255; }

        pChromaQt[i] = cq;
    }
}

static HI_VOID JPGE_MakeJFIF(VeduEfl_EncPara_S* EncHandle)
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    /* Make JFIF header bitstream */
    HI_U32 w = pEncPara->PicWidth, t, i;
    HI_U32 h = pEncPara->PicHeight;
    VeduEfl_NaluHdr_S NaluHdr;
    HI_U32 JfifHdrLen = pEncPara->SlcSplitEn ? 698 : 698 - 6;
    VALG_CRCL_BUF_S* pstStrBuf = &pEncPara->stCycBuf;

    HI_U8  dri[] = {0xff, 0xdd, 0x00, 0x04, 0x00, 0x64};
    HI_U8  sos[] = {0xff, 0xda, 0x00, 0x0c, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3f, 0x00};

    for ( i = 0; i < 698; i++ )
    { pEncPara->pJfifHdr[i] = Jpge_JfifHdr[i]; }

    /* 420 422 or 444 */
    if     ( pEncPara->YuvSampleType == JPGE_YUV420 ) { pEncPara->pJfifHdr[238] = 0x22; }
    else if ( pEncPara->YuvSampleType == JPGE_YUV422 ) { pEncPara->pJfifHdr[238] = 0x21; }
    else if ( pEncPara->YuvSampleType == JPGE_YUV444 ) { pEncPara->pJfifHdr[238] = 0x11; }

    if ( pEncPara->RotationAngle == JPGE_ROTATION_90 ||
         pEncPara->RotationAngle == JPGE_ROTATION_270 )
    {
        if (pEncPara->YuvSampleType == JPGE_YUV422) /* 90 or 270 @ 422 */
        { pEncPara->pJfifHdr[238] = 0x22; }       /* change 422 to 420 */

        t = w;
        w = h;
        h = t;
    }

    /* img size */
    pEncPara->pJfifHdr[232] = h >> 8;
    pEncPara->pJfifHdr[233] = h & 0xFF;
    pEncPara->pJfifHdr[234] = w >> 8;
    pEncPara->pJfifHdr[235] = w & 0xFF;

    /* DRI & SOS */
    t = 678;

    if (pEncPara->SlcSplitEn)
    {
        dri[4] = pEncPara->SplitSize >> 8;
        dri[5] = pEncPara->SplitSize & 0xFF;

        for ( i = 0; i < 6; i++, t++ )
        { pEncPara->pJfifHdr[t] = dri[i]; }
    }

    for ( i = 0; i < 14; i++, t++ )
    { pEncPara->pJfifHdr[t] = sos[i]; }

    /* DQT */
    Jpge_MakeQTable( pEncPara->QLevel, pEncPara->pJpgeYqt, pEncPara->pJpgeCqt );

    for ( i = 0; i < 64; i++ )
    {
        t = ZigZagScan[i];
        pEncPara->pJfifHdr[t +  25] = pEncPara->pJpgeYqt[i];
        pEncPara->pJfifHdr[t +  94] = pEncPara->pJpgeCqt[i];
        pEncPara->pJfifHdr[t + 163] = pEncPara->pJpgeCqt[i];
    }


    /*write the heard of JFIF*/
    {
        NaluHdr.PacketLen  = 64 + D_VENC_ALIGN_UP(JfifHdrLen , 64);
        NaluHdr.InvldByte  = D_VENC_ALIGN_UP(JfifHdrLen , 64) - JfifHdrLen;
        NaluHdr.bLastSlice = 0;
        NaluHdr.PTS0       = pEncPara->PTS0;
        NaluHdr.PTS1       = pEncPara->PTS1;

        VENC_DRV_BufWrite( pstStrBuf, &NaluHdr, 64 );
        VENC_DRV_BufWrite( pstStrBuf, pEncPara->pJfifHdr , D_VENC_ALIGN_UP(JfifHdrLen , 64));

        VENC_DRV_BufUpdateWp( pstStrBuf );
    }

    *(pEncPara->StrmBufRpVirAddr) = pstStrBuf->u32RdTail;
    *(pEncPara->StrmBufWpVirAddr) = pstStrBuf->u32WrHead;

    return;
}

static HI_VOID JPGE_RemoveJFIF(VeduEfl_EncPara_S* EncHandle)
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32 JfifHdrLen = pEncPara->SlcSplitEn ? 698 : 698 - 6;
    VALG_CRCL_BUF_S* pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 u32Len = 64 + D_VENC_ALIGN_UP(JfifHdrLen , 64);

    if (pEncPara->stCycBuf.u32WrHead < u32Len)   /*turn back*/
    {
        pEncPara->stCycBuf.u32WrHead =   pEncPara->stCycBuf.u32BufLen + pEncPara->stCycBuf.u32WrHead - u32Len;
    }
    else
    {
        pEncPara->stCycBuf.u32WrHead -= u32Len;
    }

    VENC_DRV_BufUpdateWp( pstStrBuf );
    *(pEncPara->StrmBufRpVirAddr) = pstStrBuf->u32RdTail;
    *(pEncPara->StrmBufWpVirAddr) = pstStrBuf->u32WrHead;

    return;
}

#endif

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : flag :0 -> YUVStoreType; 1 -> YUVSampleType; 2 -> PackageSel
******************************************************************************/
#ifdef __VENC_S40V200_CONFIG__  //FOR S40V200
static void Venc_SetRegDefault( VeduEfl_EncPara_S*  pEncPara )
{
    typedef struct
    {
        int  rWnd[2];
        int  thresh[2];
        int  rectMod[6];
        int  range[6][4];

    } intSearchIn;

    int i;
    intSearchIn* pIsr;

    HI_U8 Quant8_intra_default[64] =
    {
        6, 10, 13, 16, 18, 23, 25, 27,
        10, 11, 16, 18, 23, 25, 27, 29,
        13, 16, 18, 23, 25, 27, 29, 31,
        16, 18, 23, 25, 27, 29, 31, 33,
        18, 23, 25, 27, 29, 31, 33, 36,
        23, 25, 27, 29, 31, 33, 36, 38,
        25, 27, 29, 31, 33, 36, 38, 40,
        27, 29, 31, 33, 36, 38, 40, 42
    };

    HI_U8 Quant8_inter_default[64] =
    {
        9, 13, 15, 17, 19, 21, 22, 24,
        13, 13, 17, 19, 21, 22, 24, 25,
        15, 17, 19, 21, 22, 24, 25, 27,
        17, 19, 21, 22, 24, 25, 27, 28,
        19, 21, 22, 24, 25, 27, 28, 30,
        21, 22, 24, 25, 27, 28, 30, 32,
        22, 24, 25, 27, 28, 30, 32, 33,
        24, 25, 27, 28, 30, 32, 33, 35
    };
    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] =
    {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
        86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
        870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095
    };

    static intSearchIn isrD1 =
    {
        { 5, 2 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr720p =
    {
        { 5, 1 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr1080p =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isrWidth0 =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };

    static intSearchIn isrMpeg4 =
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },

        {   { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0},
            {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}
        }
    };

    if     (pEncPara->PicWidth >  2048 ) { pIsr = &isrWidth0; }
    else if (pEncPara->PicWidth >  1280 ) { pIsr = &isr1080p; }
    else if (pEncPara->PicWidth >   720 ) { pIsr = &isr720p; }
    else                                 { pIsr = &isrD1; }

    if (pEncPara->Protocol == VEDU_MPEG4) { pIsr = &isrMpeg4; }

    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 0;

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = pEncPara->H264HpEn ? 1 : 0;
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;           //可提高性能，配置成1

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 0;          //使能补搜索，配置成0可以提高性能          //change by tyx
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = pEncPara->H264HpEn ? 0 : 1;
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 0;//1;
    pEncPara->intpLowpowEn     = 0;//1;
    pEncPara->fracLowpowEn     = 0;//1;
    pEncPara->OutStdNum        = 1;

    for (i = 0; i < 4; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }

    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;
    pEncPara->MctfStrength0    = 0;
    pEncPara->MctfStrength1    = 0;

    pEncPara->MctfStillEn      = 0;
    pEncPara->MctfMovEn        = 0;
    pEncPara->mctfStillMvThr   = 1;
    pEncPara->mctfRealMvThr    = 2;
    pEncPara->MctfStillCostThr = 0x300;
    pEncPara->MctfLog2mctf     = 2;
    pEncPara->MctfLumaDiffThr  = 0xa;
    pEncPara->MctfChrDiffThr   = 0x5;
    pEncPara->MctfChrDeltaThr  = 0xf;
    pEncPara->MctfStiMadiThr1  = 0x4;
    pEncPara->MctfStiMadiThr2  = 0xa;
    pEncPara->MctfMovMadiThr   = 0xa;
    pEncPara->MctfMovMad1_m    = 8;
    pEncPara->MctfMovMad1_n    = 8;
    pEncPara->MctfMovMad2_m    = 8;
    pEncPara->MctfMovMad2_n    = 8;
    pEncPara->MctfStiMad1_m    = 7;
    pEncPara->MctfStiMad1_n    = 9;
    pEncPara->MctfStiMad2_m    = 8;
    pEncPara->MctfStiMad2_n    = 8;

    pEncPara->StartQpType      = 0;

    //pEncPara->RcQpDelta        = 4;  //no use
    pEncPara->RcMadpDelta      = -8;

    for (i = 0; i < 12; i++)    { pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i]; }

    for (i = 0; i < 40; i++)    { pEncPara->ModLambda[i]     = ModLambda[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i]      = Quant8_intra_default[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i + 64]   = Quant8_inter_default[i]; }

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 2;
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 2;
    pEncPara->TimeOut          = 0;
    pEncPara->PtBitsEn         = 1;         //avoid to skip the stream
    pEncPara->PtBits           = 500000 * 8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;

    for (i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i * 5;
        pEncPara->RoiCfg.StartY [i] = i * 5;
    }

    for (i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en      [i] = 0;
        pEncPara->OsdCfg.osd_global_en[i] = 0;
        pEncPara->OsdCfg.osd_absqp_en[i] = 0;
        pEncPara->OsdCfg.osd_qp      [i] = 0;
        pEncPara->OsdCfg.osd_x       [i] = i * 20;
        pEncPara->OsdCfg.osd_y       [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en [i] = 0;
        pEncPara->OsdCfg.osd_invs_w      = 1;
        pEncPara->OsdCfg.osd_invs_h      = 1;
        pEncPara->OsdCfg.osd_invs_thr    = 127;
    }
}

#elif defined(__VENC_3716CV200_CONFIG__)
static void Venc_SetRegDefault( VeduEfl_EncPara_S*  pEncPara )
{
    typedef struct
    {
        int  rWnd[2];
        int  thresh[2];
        int  rectMod[6];
        int  range[6][4];

    } intSearchIn;

    int i;
    HI_U8 Quant8_intra_default[64] =
    {
        6, 10, 13, 16, 18, 23, 25, 27,
        10, 11, 16, 18, 23, 25, 27, 29,
        13, 16, 18, 23, 25, 27, 29, 31,
        16, 18, 23, 25, 27, 29, 31, 33,
        18, 23, 25, 27, 29, 31, 33, 36,
        23, 25, 27, 29, 31, 33, 36, 38,
        25, 27, 29, 31, 33, 36, 38, 40,
        27, 29, 31, 33, 36, 38, 40, 42
    };

    HI_U8 Quant8_inter_default[64] =
    {
        9, 13, 15, 17, 19, 21, 22, 24,
        13, 13, 17, 19, 21, 22, 24, 25,
        15, 17, 19, 21, 22, 24, 25, 27,
        17, 19, 21, 22, 24, 25, 27, 28,
        19, 21, 22, 24, 25, 27, 28, 30,
        21, 22, 24, 25, 27, 28, 30, 32,
        22, 24, 25, 27, 28, 30, 32, 33,
        24, 25, 27, 28, 30, 32, 33, 35
    };
    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] =
    {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
        86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
        870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095
    };

    static intSearchIn isrD1 =
    {
        { 5, 2 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr720p =
    {
        { 5, 1 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr1080p =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isrWidth0 =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };

    static intSearchIn isrMpeg4 =
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },

        {   { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0},
            {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}
        }
    };


    intSearchIn* pIsr;

    if     (pEncPara->PicWidth >  2048 ) { pIsr = &isrWidth0; }
    else if (pEncPara->PicWidth >  1280 ) { pIsr = &isr1080p; }
    else if (pEncPara->PicWidth >   720 ) { pIsr = &isr720p; }
    else                                 { pIsr = &isrD1; }

    if (pEncPara->Protocol == VEDU_MPEG4) { pIsr = &isrMpeg4; }

    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 0;

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = 0;//pEncPara->H264HpEn ? 1 : 0;  // don't support 1
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 0;          //使能补搜索，配置成0可以提高性能          //change by tyx
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = 1;//pEncPara->H264HpEn ? 0 : 1; //don't suppore 0
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 1;
    pEncPara->intpLowpowEn     = 1;
    pEncPara->fracLowpowEn     = 1;
    pEncPara->OutStdNum        = 1;

    for (i = 0; i < 4; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }

    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;

    pEncPara->MctfStillEn             =  0     ;
    pEncPara->MctfSmlmovEn            =  0     ;
    pEncPara->MctfBigmovEn            =  0     ;
    pEncPara->mctfStillMvThr          =  4     ; //        m_picOptions.mctf_still_mv_thr
    pEncPara->mctfRealMvThr           =  4     ; //        m_picOptions.mctf_real_mv_thr
    pEncPara->MctfStillCostThr        =  2000  ; //        m_picOptions.mctf_still_cost_thr
    pEncPara->MctfStiLumaOrialpha     =  0     ; //        m_picOptions.mctf_sti_luma_ori_alpha
    pEncPara->MctfSmlmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov0_luma_ori_alpha
    pEncPara->MctfBigmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov1_luma_ori_alpha
    pEncPara->MctfChrOrialpha         =  8     ; //        m_picOptions.mctf_chr_ori_alpha
    pEncPara->mctfStiLumaDiffThr0     =  4     ; //        m_picOptions.mctf_sti_luma_diff_thr1
    pEncPara->mctfStiLumaDiffThr1     =  8     ; //        m_picOptions.mctf_sti_luma_diff_thr2
    pEncPara->mctfStiLumaDiffThr2     =  16    ; //        m_picOptions.mctf_sti_luma_diff_thr3
    pEncPara->mctfSmlmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov0_luma_diff_thr1
    pEncPara->mctfSmlmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov0_luma_diff_thr2
    pEncPara->mctfSmlmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov0_luma_diff_thr3
    pEncPara->mctfBigmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov1_luma_diff_thr1
    pEncPara->mctfBigmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov1_luma_diff_thr2
    pEncPara->mctfBigmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov1_luma_diff_thr3
    pEncPara->mctfStiLumaDiffK0       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k1
    pEncPara->mctfStiLumaDiffK1       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k2
    pEncPara->mctfStiLumaDiffK2       =  32    ; //        m_picOptions.mctf_sti_luma_diff_k3
    pEncPara->mctfSmlmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k1
    pEncPara->mctfSmlmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k2
    pEncPara->mctfSmlmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov0_luma_diff_k3
    pEncPara->mctfBigmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k1
    pEncPara->mctfBigmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k2
    pEncPara->mctfBigmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov1_luma_diff_k3
    pEncPara->mctfChrDiffThr0         =  4     ; //        m_picOptions.mctf_chr_diff_thr1
    pEncPara->mctfChrDiffThr1         =  8     ; //        m_picOptions.mctf_chr_diff_thr2
    pEncPara->mctfChrDiffThr2         =  16    ; //        m_picOptions.mctf_chr_diff_thr3
    pEncPara->mctfChrDiffK0           =  16    ; //        m_picOptions.mctf_chr_diff_k1
    pEncPara->mctfChrDiffK1           =  16    ; //        m_picOptions.mctf_chr_diff_k2
    pEncPara->mctfChrDiffK2           =  16    ; //        m_picOptions.mctf_chr_diff_k3
    pEncPara->mctfOriRatio            =  4     ; //        m_picOptions.mctf_ori_ratio
    pEncPara->mctfStiMaxRatio         =  4     ; //        m_picOptions.mctf_sti_max_ratio
    pEncPara->mctfSmlmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov0_max_ratio
    pEncPara->mctfBigmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov1_max_ratio
    pEncPara->mctfStiMadiThr0         =  3     ; //        m_picOptions.mctf_sti_madi_thr1
    pEncPara->mctfStiMadiThr1         =  10    ; //        m_picOptions.mctf_sti_madi_thr2
    pEncPara->mctfStiMadiThr2         =  20    ; //        m_picOptions.mctf_sti_madi_thr3
    pEncPara->mctfSmlmovMadiThr0      =  6     ; //        m_picOptions.mctf_mov0_madi_thr1
    pEncPara->mctfSmlmovMadiThr1      =  16    ; //        m_picOptions.mctf_mov0_madi_thr2
    pEncPara->mctfSmlmovMadiThr2      =  30    ; //        m_picOptions.mctf_mov0_madi_thr3
    pEncPara->mctfBigmovMadiThr0      =  15    ; //        m_picOptions.mctf_mov1_madi_thr1
    pEncPara->mctfBigmovMadiThr1      =  40    ; //        m_picOptions.mctf_mov1_madi_thr2
    pEncPara->mctfBigmovMadiThr2      =  60    ; //        m_picOptions.mctf_mov1_madi_thr3
    pEncPara->mctfStiMadiK0           =  0     ; //        m_picOptions.mctf_sti_madi_k1
    pEncPara->mctfStiMadiK1           =  16    ; //        m_picOptions.mctf_sti_madi_k2
    pEncPara->mctfStiMadiK2           =  16    ; //        m_picOptions.mctf_sti_madi_k3
    pEncPara->mctfSmlmovMadiK0        =  0     ; //        m_picOptions.mctf_mov0_madi_k1
    pEncPara->mctfSmlmovMadiK1        =  10    ; //        m_picOptions.mctf_mov0_madi_k2
    pEncPara->mctfSmlmovMadiK2        =  16    ; //        m_picOptions.mctf_mov0_madi_k3
    pEncPara->mctfBigmovMadiK0        =  0     ; //        m_picOptions.mctf_mov1_madi_k1
    pEncPara->mctfBigmovMadiK1        =  10    ; //        m_picOptions.mctf_mov1_madi_k2
    pEncPara->mctfBigmovMadiK2        =  16    ; //        m_picOptions.mctf_mov1_madi_k3

    pEncPara->StartQpType      = 0;

    pEncPara->RcQpDelta        = 4;
    pEncPara->RcMadpDelta      = -8;

    for (i = 0; i < 12; i++)    { pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i]; }

    for (i = 0; i < 40; i++)    { pEncPara->ModLambda[i]     = ModLambda[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i]      = Quant8_intra_default[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i + 64]   = Quant8_inter_default[i]; }

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 2;  /*open the frame level and MB level ClkGate*/
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 3;
    pEncPara->TimeOut          = 10000000;
    pEncPara->PtBitsEn         = 1;
    pEncPara->PtBits           = 500000 * 8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;

    /* add for VBR */
    pEncPara->MaxIPdelta       = 3;
    pEncPara->MaxPPdelta       = 1;

    for (i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.Keep   [i] = 0;
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i * 5;
        pEncPara->RoiCfg.StartY [i] = i * 5;
    }

    for (i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en       [i] = 0;
        pEncPara->OsdCfg.osd_global_en[i] = 0;
        pEncPara->OsdCfg.osd_absqp_en [i] = 0;
        pEncPara->OsdCfg.osd_qp       [i] = 0;
        pEncPara->OsdCfg.osd_x        [i] = i * 20;
        pEncPara->OsdCfg.osd_y        [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en  [i] = 0;
        pEncPara->OsdCfg.osd_invs_w       = 1;
        pEncPara->OsdCfg.osd_invs_h       = 1;
        pEncPara->OsdCfg.osd_invs_thr     = 127;
    }
}

#elif (defined (__VENC_S5V100_CONFIG__) || defined(__VENC_98M_CONFIG__))
static void Venc_SetRegDefault( VeduEfl_EncPara_S*  pEncPara )
{
    typedef struct
    {
        int  rWnd[2];
        int  thresh[2];
        int  rectMod[6];
        int  range[6][4];

    } intSearchIn;

    int i;

    HI_U8 Quant8_intra_default[64] =
    {
        6, 10, 13, 16, 18, 23, 25, 27,
        10, 11, 16, 18, 23, 25, 27, 29,
        13, 16, 18, 23, 25, 27, 29, 31,
        16, 18, 23, 25, 27, 29, 31, 33,
        18, 23, 25, 27, 29, 31, 33, 36,
        23, 25, 27, 29, 31, 33, 36, 38,
        25, 27, 29, 31, 33, 36, 38, 40,
        27, 29, 31, 33, 36, 38, 40, 42
    };

    HI_U8 Quant8_inter_default[64] =
    {
        9, 13, 15, 17, 19, 21, 22, 24,
        13, 13, 17, 19, 21, 22, 24, 25,
        15, 17, 19, 21, 22, 24, 25, 27,
        17, 19, 21, 22, 24, 25, 27, 28,
        19, 21, 22, 24, 25, 27, 28, 30,
        21, 22, 24, 25, 27, 28, 30, 32,
        22, 24, 25, 27, 28, 30, 32, 33,
        24, 25, 27, 28, 30, 32, 33, 35
    };

    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] =
    {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
        86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
        870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095
    };

#ifdef __VENC_S5V200L_EXT_CONFIG__
    static intSearchIn isrD1 =
    {
        { 5, 1 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr720p =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
#else
    static intSearchIn isrD1 =
    {
        { 5, 2 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr720p =
    {
        { 5, 1 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
#endif

    static intSearchIn isr1080p =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0,  0, 0, 0}
        }
    };
    static intSearchIn isrWidth0 =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0,  0, 0, 0}
        }
    };

    static intSearchIn isrMpeg4 =
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },

        {   { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0},
            {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}
        }
    };


    intSearchIn* pIsr;

    if     (pEncPara->PicWidth >  2048 ) { pIsr = &isrWidth0; }
    else if (pEncPara->PicWidth >  1280 ) { pIsr = &isr1080p; }
    else if (pEncPara->PicWidth >   720 ) { pIsr = &isr720p; }
    else                                 { pIsr = &isrD1; }

    if (pEncPara->Protocol == VEDU_MPEG4) { pIsr = &isrMpeg4; }

    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 0;    //cabac 字节填充使能信号 don't support 1

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = 0;//pEncPara->H264HpEn ? 1 : 0;  // don't support 1
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 1;          //CV200 OPEN this
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = 1;//pEncPara->H264HpEn ? 0 : 1; //don't suppore 0
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 1;
    pEncPara->intpLowpowEn     = 1;
    pEncPara->fracLowpowEn     = 1;
    pEncPara->OutStdNum        = 1;

    for (i = 0; i < 4; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }

    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;

    pEncPara->MctfStillEn             =  0     ;
    pEncPara->MctfSmlmovEn            =  0     ;
    pEncPara->MctfBigmovEn            =  0     ;
    pEncPara->mctfStillMvThr          =  4     ; //        m_picOptions.mctf_still_mv_thr
    pEncPara->mctfRealMvThr           =  4     ; //        m_picOptions.mctf_real_mv_thr
    pEncPara->MctfStillCostThr        =  2000  ; //        m_picOptions.mctf_still_cost_thr
    pEncPara->MctfStiLumaOrialpha     =  0     ; //        m_picOptions.mctf_sti_luma_ori_alpha
    pEncPara->MctfSmlmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov0_luma_ori_alpha
    pEncPara->MctfBigmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov1_luma_ori_alpha
    pEncPara->MctfChrOrialpha         =  8     ; //        m_picOptions.mctf_chr_ori_alpha
    pEncPara->mctfStiLumaDiffThr0     =  4     ; //        m_picOptions.mctf_sti_luma_diff_thr1
    pEncPara->mctfStiLumaDiffThr1     =  8     ; //        m_picOptions.mctf_sti_luma_diff_thr2
    pEncPara->mctfStiLumaDiffThr2     =  16    ; //        m_picOptions.mctf_sti_luma_diff_thr3
    pEncPara->mctfSmlmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov0_luma_diff_thr1
    pEncPara->mctfSmlmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov0_luma_diff_thr2
    pEncPara->mctfSmlmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov0_luma_diff_thr3
    pEncPara->mctfBigmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov1_luma_diff_thr1
    pEncPara->mctfBigmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov1_luma_diff_thr2
    pEncPara->mctfBigmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov1_luma_diff_thr3
    pEncPara->mctfStiLumaDiffK0       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k1
    pEncPara->mctfStiLumaDiffK1       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k2
    pEncPara->mctfStiLumaDiffK2       =  32    ; //        m_picOptions.mctf_sti_luma_diff_k3
    pEncPara->mctfSmlmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k1
    pEncPara->mctfSmlmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k2
    pEncPara->mctfSmlmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov0_luma_diff_k3
    pEncPara->mctfBigmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k1
    pEncPara->mctfBigmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k2
    pEncPara->mctfBigmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov1_luma_diff_k3
    pEncPara->mctfChrDiffThr0         =  4     ; //        m_picOptions.mctf_chr_diff_thr1
    pEncPara->mctfChrDiffThr1         =  8     ; //        m_picOptions.mctf_chr_diff_thr2
    pEncPara->mctfChrDiffThr2         =  16    ; //        m_picOptions.mctf_chr_diff_thr3
    pEncPara->mctfChrDiffK0           =  16    ; //        m_picOptions.mctf_chr_diff_k1
    pEncPara->mctfChrDiffK1           =  16    ; //        m_picOptions.mctf_chr_diff_k2
    pEncPara->mctfChrDiffK2           =  16    ; //        m_picOptions.mctf_chr_diff_k3
    pEncPara->mctfOriRatio            =  4     ; //        m_picOptions.mctf_ori_ratio
    pEncPara->mctfStiMaxRatio         =  4     ; //        m_picOptions.mctf_sti_max_ratio
    pEncPara->mctfSmlmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov0_max_ratio
    pEncPara->mctfBigmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov1_max_ratio
    pEncPara->mctfStiMadiThr0         =  3     ; //        m_picOptions.mctf_sti_madi_thr1
    pEncPara->mctfStiMadiThr1         =  10    ; //        m_picOptions.mctf_sti_madi_thr2
    pEncPara->mctfStiMadiThr2         =  20    ; //        m_picOptions.mctf_sti_madi_thr3
    pEncPara->mctfSmlmovMadiThr0      =  6     ; //        m_picOptions.mctf_mov0_madi_thr1
    pEncPara->mctfSmlmovMadiThr1      =  16    ; //        m_picOptions.mctf_mov0_madi_thr2
    pEncPara->mctfSmlmovMadiThr2      =  30    ; //        m_picOptions.mctf_mov0_madi_thr3
    pEncPara->mctfBigmovMadiThr0      =  15    ; //        m_picOptions.mctf_mov1_madi_thr1
    pEncPara->mctfBigmovMadiThr1      =  40    ; //        m_picOptions.mctf_mov1_madi_thr2
    pEncPara->mctfBigmovMadiThr2      =  60    ; //        m_picOptions.mctf_mov1_madi_thr3
    pEncPara->mctfStiMadiK0           =  0     ; //        m_picOptions.mctf_sti_madi_k1
    pEncPara->mctfStiMadiK1           =  16    ; //        m_picOptions.mctf_sti_madi_k2
    pEncPara->mctfStiMadiK2           =  16    ; //        m_picOptions.mctf_sti_madi_k3
    pEncPara->mctfSmlmovMadiK0        =  0     ; //        m_picOptions.mctf_mov0_madi_k1
    pEncPara->mctfSmlmovMadiK1        =  10    ; //        m_picOptions.mctf_mov0_madi_k2
    pEncPara->mctfSmlmovMadiK2        =  16    ; //        m_picOptions.mctf_mov0_madi_k3
    pEncPara->mctfBigmovMadiK0        =  0     ; //        m_picOptions.mctf_mov1_madi_k1
    pEncPara->mctfBigmovMadiK1        =  10    ; //        m_picOptions.mctf_mov1_madi_k2
    pEncPara->mctfBigmovMadiK2        =  16    ; //        m_picOptions.mctf_mov1_madi_k3


    pEncPara->csc_mode      =  0        ;
#if 1     //601->709 配置
    pEncPara->csc00         =  1024     ;
    pEncPara->csc01         =  -120     ;
    pEncPara->csc02         =  -212     ;
    pEncPara->csc10         =  0        ;
    pEncPara->csc11         =  1044     ;
    pEncPara->csc12         =  116      ;
    pEncPara->csc20         =  0        ;
    pEncPara->csc21         =  76       ;
    pEncPara->csc22         =  1052     ;
    pEncPara->cscin0        =  -128     ;
    pEncPara->cscin1        =  -128     ;
    pEncPara->cscin2        =  -16      ;
    pEncPara->cscout0       =   128     ;
    pEncPara->cscout1       =   128     ;
    pEncPara->cscout2       =   16      ;
#else
    pEncPara->csc00         =  1024     ;
    pEncPara->csc01         =  102      ;
    pEncPara->csc02         =  196      ;
    pEncPara->csc10         =  0        ;
    pEncPara->csc11         =  1014     ;
    pEncPara->csc12         =  -113     ;
    pEncPara->csc20         =  0        ;
    pEncPara->csc21         =  -74      ;
    pEncPara->csc22         =  1007     ;
    pEncPara->cscin0        =  -128     ;
    pEncPara->cscin1        =  -128     ;
    pEncPara->cscin2        =  -16      ;
    pEncPara->cscout0       =   128     ;
    pEncPara->cscout1       =   128     ;
    pEncPara->cscout2       =   16      ;
#endif

    pEncPara->StartQpType      = 0;

    pEncPara->RcQpDelta        = 4;
    pEncPara->RcMadpDelta      = -8;

    for (i = 0; i < 12; i++)    { pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i]; }

    for (i = 0; i < 40; i++)    { pEncPara->ModLambda[i]     = ModLambda[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i]      = Quant8_intra_default[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i + 64]   = Quant8_inter_default[i]; }

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 2;  /*open the frame level and MB level ClkGate*/
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 3;
    pEncPara->TimeOut          = 10000000;
    pEncPara->PtBitsEn         = 1;
    pEncPara->PtBits           = 500000 * 8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;

    /* add for VBR */
    pEncPara->MaxIPdelta       = 3;
    pEncPara->MaxPPdelta       = 1;

    for (i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.Keep   [i] = 0;
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i * 5;
        pEncPara->RoiCfg.StartY [i] = i * 5;
    }

    for (i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en       [i] = 0;
        pEncPara->OsdCfg.osd_global_en[i] = 0;
        pEncPara->OsdCfg.osd_absqp_en [i] = 0;
        pEncPara->OsdCfg.osd_qp       [i] = 0;
        pEncPara->OsdCfg.osd_x        [i] = i * 20;
        pEncPara->OsdCfg.osd_y        [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en  [i] = 0;
        pEncPara->OsdCfg.osd_invs_w       = 1;
        pEncPara->OsdCfg.osd_invs_h       = 1;
        pEncPara->OsdCfg.osd_invs_thr     = 127;
    }
}

#elif (defined (__VENC_98CV200_CONFIG__))

static void Venc_SetRegDefault( VeduEfl_EncPara_S*  pEncPara )
{
    typedef struct
    {
        int  rWnd[2];
        int  thresh[2];
        int  rectMod[6];
        int  range[6][4];

    } intSearchIn;

    int i;

    HI_U8 Quant8_intra_default[64] =
    {
        6, 10, 13, 16, 18, 23, 25, 27,
        10, 11, 16, 18, 23, 25, 27, 29,
        13, 16, 18, 23, 25, 27, 29, 31,
        16, 18, 23, 25, 27, 29, 31, 33,
        18, 23, 25, 27, 29, 31, 33, 36,
        23, 25, 27, 29, 31, 33, 36, 38,
        25, 27, 29, 31, 33, 36, 38, 40,
        27, 29, 31, 33, 36, 38, 40, 42
    };

    HI_U8 Quant8_inter_default[64] =
    {
        9, 13, 15, 17, 19, 21, 22, 24,
        13, 13, 17, 19, 21, 22, 24, 25,
        15, 17, 19, 21, 22, 24, 25, 27,
        17, 19, 21, 22, 24, 25, 27, 28,
        19, 21, 22, 24, 25, 27, 28, 30,
        21, 22, 24, 25, 27, 28, 30, 32,
        22, 24, 25, 27, 28, 30, 32, 33,
        24, 25, 27, 28, 30, 32, 33, 35
    };

    int  RcQpDeltaThr[12] = {7, 7, 7, 9, 11, 14, 18, 25, 255, 255, 255, 255};
    int  ModLambda[40] =
    {
        1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
        9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
        86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
        870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095
    };

    static intSearchIn isrCif =
    {
        { 3, 2 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };

    static intSearchIn isrD1 =
    {
        { 5, 2 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr720p =
    {
        { 5, 1 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0, 0, 0, 0}
        }
    };
    static intSearchIn isr1080p =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0,  0, 0, 0}
        }
    };
    static intSearchIn isrWidth0 =
    {
        { 5, 0 },  { 0, 1500 },  { 1, 1, 1, 1, 0, 0 },

        {   { 2, 2, 0, 0}, { 8, 8, 0, 0}, { 13, 13, 1, 1},
            { 4, 4, 0, 0}, {  0,  0, 0, 0}, { 0,  0, 0, 0}
        }
    };

    static intSearchIn isrMpeg4 =
    {
        { 1, 0 },  { 0,  0 },  { 1, 0, 0, 0, 0, 0 },

        {   { 32, 16, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0},
            {  0,  0, 0, 0}, { 0, 0, 0, 0}, { 0, 0, 0, 0}
        }
    };


    intSearchIn* pIsr;

    if     (pEncPara->PicWidth >  2048 ) { pIsr = &isrWidth0; }
    else if (pEncPara->PicWidth >  1280 ) { pIsr = &isr1080p; }
    else if (pEncPara->PicWidth >   720 ) { pIsr = &isr720p; }
    else if (pEncPara->PicWidth >   352)  { pIsr = &isrD1; }
    else {pIsr = &isrCif;}

    if (pEncPara->Protocol == VEDU_MPEG4) { pIsr = &isrMpeg4; }

    pEncPara->bMorePPS         = 0;
    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 0;    //cabac 字节填充使能信号 don't support 1

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->IPCMPredEn       = 1;
    pEncPara->Intra4x4PredEn   = 1;
    pEncPara->Intra8x8PredEn   = 0;//pEncPara->H264HpEn ? 1 : 0;  // don't support 1
    pEncPara->Intra16x16PredEn = 1;
    pEncPara->I4ReducedModeEn  = 0;

    pEncPara->Inter8x8PredEn   = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter8x16PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x8PredEn  = pEncPara->Protocol == VEDU_H264 ? 1 : 0;
    pEncPara->Inter16x16PredEn = 1;
    pEncPara->PskipEn          = 1;          //CV200 OPEN this
    pEncPara->ExtedgeEn        = 1;
    pEncPara->TransMode        = 1;//pEncPara->H264HpEn ? 0 : 1; //don't suppore 0
    pEncPara->NumRefIndex      = 0;

    pEncPara->PixClipEn        = 0;
    pEncPara->LumaClipMax      = 235;
    pEncPara->LumaClipMin      = 16;
    pEncPara->ChrClipMax       = 240;
    pEncPara->ChrClipMin       = 16;

    pEncPara->HSWSize          = pIsr->rWnd[0];
    pEncPara->VSWSize          = pIsr->rWnd[1];
    pEncPara->fracRealMvThr    = 15;
    pEncPara->IntraLowpowEn    = 1;
    pEncPara->intpLowpowEn     = 1;
    pEncPara->fracLowpowEn     = 1;

    pEncPara->wOutStdNum      = 1;
    pEncPara->rOutStdNum       = 1;

    for (i = 0; i < 4; i++)
    {
        pEncPara->RectMod   [i]    = pIsr->rectMod[i];
        pEncPara->RectWidth [i]    = pIsr->range  [i][0];
        pEncPara->RectHeight[i]    = pIsr->range  [i][1];
        pEncPara->RectHstep [i]    = pIsr->range  [i][2];
        pEncPara->RectVstep [i]    = pIsr->range  [i][3];
    }

    pEncPara->StartThr1        = pIsr->thresh[0];
    pEncPara->StartThr2        = pIsr->thresh[1];
    pEncPara->IntraThr         = 4096;
    pEncPara->LmdaOff16        = 0;
    pEncPara->LmdaOff1608      = 0;
    pEncPara->LmdaOff0816      = 0;
    pEncPara->LmdaOff8         = 0;
    pEncPara->CrefldBur8En     = 1;
    pEncPara->MdDelta          = -1024;

    pEncPara->MctfStillEn             =  0     ;
    pEncPara->MctfSmlmovEn            =  0     ;
    pEncPara->MctfBigmovEn            =  0     ;
    pEncPara->mctfStillMvThr          =  4     ; //        m_picOptions.mctf_still_mv_thr
    pEncPara->mctfRealMvThr           =  4     ; //        m_picOptions.mctf_real_mv_thr
    pEncPara->MctfStillCostThr        =  2000  ; //        m_picOptions.mctf_still_cost_thr
    pEncPara->MctfStiLumaOrialpha     =  0     ; //        m_picOptions.mctf_sti_luma_ori_alpha
    pEncPara->MctfSmlmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov0_luma_ori_alpha
    pEncPara->MctfBigmovLumaOrialpha  =  0     ; //        m_picOptions.mctf_mov1_luma_ori_alpha
    pEncPara->MctfChrOrialpha         =  8     ; //        m_picOptions.mctf_chr_ori_alpha
    pEncPara->mctfStiLumaDiffThr0     =  4     ; //        m_picOptions.mctf_sti_luma_diff_thr1
    pEncPara->mctfStiLumaDiffThr1     =  8     ; //        m_picOptions.mctf_sti_luma_diff_thr2
    pEncPara->mctfStiLumaDiffThr2     =  16    ; //        m_picOptions.mctf_sti_luma_diff_thr3
    pEncPara->mctfSmlmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov0_luma_diff_thr1
    pEncPara->mctfSmlmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov0_luma_diff_thr2
    pEncPara->mctfSmlmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov0_luma_diff_thr3
    pEncPara->mctfBigmovLumaDiffThr0  =  4     ; //        m_picOptions.mctf_mov1_luma_diff_thr1
    pEncPara->mctfBigmovLumaDiffThr1  =  8     ; //        m_picOptions.mctf_mov1_luma_diff_thr2
    pEncPara->mctfBigmovLumaDiffThr2  =  16    ; //        m_picOptions.mctf_mov1_luma_diff_thr3
    pEncPara->mctfStiLumaDiffK0       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k1
    pEncPara->mctfStiLumaDiffK1       =  16    ; //        m_picOptions.mctf_sti_luma_diff_k2
    pEncPara->mctfStiLumaDiffK2       =  32    ; //        m_picOptions.mctf_sti_luma_diff_k3
    pEncPara->mctfSmlmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k1
    pEncPara->mctfSmlmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov0_luma_diff_k2
    pEncPara->mctfSmlmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov0_luma_diff_k3
    pEncPara->mctfBigmovLumaDiffK0    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k1
    pEncPara->mctfBigmovLumaDiffK1    =  16    ; //        m_picOptions.mctf_mov1_luma_diff_k2
    pEncPara->mctfBigmovLumaDiffK2    =  32    ; //        m_picOptions.mctf_mov1_luma_diff_k3
    pEncPara->mctfChrDiffThr0         =  4     ; //        m_picOptions.mctf_chr_diff_thr1
    pEncPara->mctfChrDiffThr1         =  8     ; //        m_picOptions.mctf_chr_diff_thr2
    pEncPara->mctfChrDiffThr2         =  16    ; //        m_picOptions.mctf_chr_diff_thr3
    pEncPara->mctfChrDiffK0           =  16    ; //        m_picOptions.mctf_chr_diff_k1
    pEncPara->mctfChrDiffK1           =  16    ; //        m_picOptions.mctf_chr_diff_k2
    pEncPara->mctfChrDiffK2           =  16    ; //        m_picOptions.mctf_chr_diff_k3
    pEncPara->mctfOriRatio            =  4     ; //        m_picOptions.mctf_ori_ratio
    pEncPara->mctfStiMaxRatio         =  4     ; //        m_picOptions.mctf_sti_max_ratio
    pEncPara->mctfSmlmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov0_max_ratio
    pEncPara->mctfBigmovMaxRatio      =  4     ; //        m_picOptions.mctf_mov1_max_ratio
    pEncPara->mctfStiMadiThr0         =  3     ; //        m_picOptions.mctf_sti_madi_thr1
    pEncPara->mctfStiMadiThr1         =  10    ; //        m_picOptions.mctf_sti_madi_thr2
    pEncPara->mctfStiMadiThr2         =  20    ; //        m_picOptions.mctf_sti_madi_thr3
    pEncPara->mctfSmlmovMadiThr0      =  6     ; //        m_picOptions.mctf_mov0_madi_thr1
    pEncPara->mctfSmlmovMadiThr1      =  16    ; //        m_picOptions.mctf_mov0_madi_thr2
    pEncPara->mctfSmlmovMadiThr2      =  30    ; //        m_picOptions.mctf_mov0_madi_thr3
    pEncPara->mctfBigmovMadiThr0      =  15    ; //        m_picOptions.mctf_mov1_madi_thr1
    pEncPara->mctfBigmovMadiThr1      =  40    ; //        m_picOptions.mctf_mov1_madi_thr2
    pEncPara->mctfBigmovMadiThr2      =  60    ; //        m_picOptions.mctf_mov1_madi_thr3
    pEncPara->mctfStiMadiK0           =  0     ; //        m_picOptions.mctf_sti_madi_k1
    pEncPara->mctfStiMadiK1           =  16    ; //        m_picOptions.mctf_sti_madi_k2
    pEncPara->mctfStiMadiK2           =  16    ; //        m_picOptions.mctf_sti_madi_k3
    pEncPara->mctfSmlmovMadiK0        =  0     ; //        m_picOptions.mctf_mov0_madi_k1
    pEncPara->mctfSmlmovMadiK1        =  10    ; //        m_picOptions.mctf_mov0_madi_k2
    pEncPara->mctfSmlmovMadiK2        =  16    ; //        m_picOptions.mctf_mov0_madi_k3
    pEncPara->mctfBigmovMadiK0        =  0     ; //        m_picOptions.mctf_mov1_madi_k1
    pEncPara->mctfBigmovMadiK1        =  10    ; //        m_picOptions.mctf_mov1_madi_k2
    pEncPara->mctfBigmovMadiK2        =  16    ; //        m_picOptions.mctf_mov1_madi_k3


    pEncPara->csc_mode      =  0        ;
#if 1     //601->709 配置
    pEncPara->csc00         =  1024     ;
    pEncPara->csc01         =  -120     ;
    pEncPara->csc02         =  -212     ;
    pEncPara->csc10         =  0        ;
    pEncPara->csc11         =  1044     ;
    pEncPara->csc12         =  116      ;
    pEncPara->csc20         =  0        ;
    pEncPara->csc21         =  76       ;
    pEncPara->csc22         =  1052     ;
    pEncPara->cscin0        =  -128     ;
    pEncPara->cscin1        =  -128     ;
    pEncPara->cscin2        =  -16      ;
    pEncPara->cscout0       =   128     ;
    pEncPara->cscout1       =   128     ;
    pEncPara->cscout2       =   16      ;
#else
    pEncPara->csc00         =  1024     ;
    pEncPara->csc01         =  102      ;
    pEncPara->csc02         =  196      ;
    pEncPara->csc10         =  0        ;
    pEncPara->csc11         =  1014     ;
    pEncPara->csc12         =  -113     ;
    pEncPara->csc20         =  0        ;
    pEncPara->csc21         =  -74      ;
    pEncPara->csc22         =  1007     ;
    pEncPara->cscin0        =  -128     ;
    pEncPara->cscin1        =  -128     ;
    pEncPara->cscin2        =  -16      ;
    pEncPara->cscout0       =   128     ;
    pEncPara->cscout1       =   128     ;
    pEncPara->cscout2       =   16      ;
#endif

    ////////// add for 98cv200 mmu config
    pEncPara->ch00_rrmax = 0;
    pEncPara->ch01_rrmax = 0;
    pEncPara->ch02_rrmax = 0;
    pEncPara->ch03_rrmax = 0;
    pEncPara->ch04_rrmax = 0;
    pEncPara->ch05_rrmax = 0;
    pEncPara->ch06_rrmax = 0;
    pEncPara->ch07_rrmax = 0;
    pEncPara->ch08_rrmax = 0;
    pEncPara->ch09_rrmax = 0;
    pEncPara->ch10_rrmax = 0;
    pEncPara->ch11_rrmax = 0;
    pEncPara->ch12_rrmax = 0;

    pEncPara->ch00_wrmax = 0;
    pEncPara->ch01_wrmax = 0;
    pEncPara->ch02_wrmax = 0;
    pEncPara->ch03_wrmax = 0;
    pEncPara->ch04_wrmax = 0;
    pEncPara->ch05_wrmax = 0;
    pEncPara->ch06_wrmax = 0;
    pEncPara->ch07_wrmax = 0;
    pEncPara->ch08_wrmax = 0;
    pEncPara->ch09_wrmax = 0;
    pEncPara->ch10_wrmax = 0;
    pEncPara->ch11_wrmax = 0;
    pEncPara->ch12_wrmax = 0;
    pEncPara->ch13_wrmax = 0;
    pEncPara->ch14_wrmax = 0;

    pEncPara->wtmax      = 0;
    pEncPara->rtmax      = 0;

    pEncPara->secure_en  = 0;

#ifdef HI_SMMU_SUPPORT
    pEncPara->glb_bypass = 0;
    pEncPara->int_en        = 1;    // smmu中断使能

    pEncPara->vcpi_srcy_bypass = 0;
    pEncPara->vcpi_srcc_bypass = 0;
    pEncPara->vcpi_srcv_bypass = 0;
    pEncPara->vcpi_refy_bypass = 0;
    pEncPara->vcpi_refc_bypass = 0;
    pEncPara->vcpi_rcny_bypass = 0;
    pEncPara->vcpi_rcnc_bypass = 0;
    pEncPara->vcpi_strm_bypass = 0;
#else
    pEncPara->glb_bypass = 1;
    pEncPara->int_en        = 0;    // smmu中断使能

    pEncPara->vcpi_srcy_bypass = 1;
    pEncPara->vcpi_srcc_bypass = 1;
    pEncPara->vcpi_srcv_bypass = 1;
    pEncPara->vcpi_refy_bypass = 1;
    pEncPara->vcpi_refc_bypass = 1;
    pEncPara->vcpi_rcny_bypass = 1;
    pEncPara->vcpi_rcnc_bypass = 1;
    pEncPara->vcpi_strm_bypass = 1;
#endif
    /////////  add end
    pEncPara->StartQpType      = 0;

    pEncPara->RcQpDelta        = 4;
    pEncPara->RcMadpDelta      = -8;

    for (i = 0; i < 12; i++)    { pEncPara->RcQpDeltaThr[i]  = RcQpDeltaThr[i]; }

    for (i = 0; i < 40; i++)    { pEncPara->ModLambda[i]     = ModLambda[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i]      = Quant8_intra_default[i]; }

    for (i = 0; i < 64; i++)    { pEncPara->Scale8x8[i + 64]   = Quant8_inter_default[i]; }

    pEncPara->RegLockEn        = 0;
    pEncPara->ClkGateEn        = 2;  /*open the frame level and MB level ClkGate*/
    pEncPara->MemClkGateEn     = 1;
    pEncPara->TimeOutEn        = 3;
    pEncPara->TimeOut          = 10000000;
    pEncPara->PtBitsEn         = 1;
    pEncPara->PtBits           = 500000 * 8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;

    /* add for VBR */
    pEncPara->MaxIPdelta       = 3;
    pEncPara->MaxPPdelta       = 1;

    for (i = 0; i < 8; i++)
    {
        pEncPara->RoiCfg.Enable [i] = 0;
        pEncPara->RoiCfg.Keep   [i] = 0;
        pEncPara->RoiCfg.AbsQpEn[i] = 1;
        pEncPara->RoiCfg.Qp     [i] = 26;
        pEncPara->RoiCfg.Width  [i] = 7;
        pEncPara->RoiCfg.Height [i] = 7;
        pEncPara->RoiCfg.StartX [i] = i * 5;
        pEncPara->RoiCfg.StartY [i] = i * 5;
    }

    for (i = 0; i < 8; i++)
    {
        pEncPara->OsdCfg.osd_en       [i] = 0;
        pEncPara->OsdCfg.osd_global_en[i] = 0;
        pEncPara->OsdCfg.osd_absqp_en [i] = 0;
        pEncPara->OsdCfg.osd_qp       [i] = 0;
        pEncPara->OsdCfg.osd_x        [i] = i * 20;
        pEncPara->OsdCfg.osd_y        [i] = i * 20;
        pEncPara->OsdCfg.osd_invs_en  [i] = 0;
        pEncPara->OsdCfg.osd_invs_w       = 1;
        pEncPara->OsdCfg.osd_invs_h       = 1;
        pEncPara->OsdCfg.osd_invs_thr     = 127;
    }
}


#endif

static HI_U32 Convert_PIX_Format_YUVStoreType(HI_DRV_PIX_FORMAT_E oldFormat)
{
    HI_U32 Ret = HI_SUCCESS;

    switch (oldFormat)
    {
        case HI_DRV_PIX_FMT_NV61_2X1:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_NV80:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_NV61:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_NV42:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;

        case HI_DRV_PIX_FMT_UYVY:
            Ret = VENC_STORE_PACKAGE;
            break;

        case HI_DRV_PIX_FMT_YUYV:
            Ret = VENC_STORE_PACKAGE;
            break;

        case HI_DRV_PIX_FMT_YVYU :
            Ret = VENC_STORE_PACKAGE;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV411:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            Ret = VENC_STORE_PLANNAR;
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            Ret = VENC_STORE_PLANNAR;
            break;

        default:
            Ret = VENC_STORE_SEMIPLANNAR;
            break;
    }

    return Ret;
}

static HI_U32 Convert_PIX_Format_YUVSampleType(HI_DRV_PIX_FORMAT_E oldFormat)
{
    HI_U32 Ret = HI_SUCCESS;

    switch (oldFormat)
    {
        case HI_DRV_PIX_FMT_NV61_2X1:
            Ret = VENC_YUV_422;
            break;

        case HI_DRV_PIX_FMT_NV21:
        case HI_DRV_PIX_FMT_NV12:
            Ret = VENC_YUV_420;
            break;

        case HI_DRV_PIX_FMT_NV80:     //400
            Ret = VENC_YUV_NONE;
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            Ret = VENC_YUV_NONE;
            break;

        case HI_DRV_PIX_FMT_NV61:
        case HI_DRV_PIX_FMT_NV16:
            Ret = VENC_YUV_422;
            break;

        case HI_DRV_PIX_FMT_NV42:
            Ret = VENC_YUV_444;
            break;

        case HI_DRV_PIX_FMT_UYVY:
        case HI_DRV_PIX_FMT_YUYV:
        case HI_DRV_PIX_FMT_YVYU:
            Ret = VENC_YUV_422;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            Ret = VENC_YUV_NONE;
            break;

        case HI_DRV_PIX_FMT_YUV411:
            Ret = VENC_YUV_NONE;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            Ret = VENC_YUV_420;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            Ret = VENC_YUV_422;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            Ret = VENC_YUV_422;
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            Ret = VENC_YUV_444;
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            Ret = VENC_YUV_NONE;
            break;

        default:
            Ret = VENC_YUV_NONE;
            break;
    }

    return Ret;
}

static HI_U32 Convert_PIX_Format_PackageSel(HI_DRV_PIX_FORMAT_E oldFormat)
{
    HI_U32 Ret = HI_SUCCESS;

    switch (oldFormat)
    {
        case HI_DRV_PIX_FMT_NV61_2X1:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_NV21:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_NV12:
            Ret = VENC_U_V;
            break;

        case HI_DRV_PIX_FMT_NV80:     //400
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_NV12_411:
            Ret = VENC_U_V;
            break;

        case HI_DRV_PIX_FMT_NV61:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_NV16:
            Ret = VENC_U_V;
            break;

        case HI_DRV_PIX_FMT_NV42:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_UYVY:
            Ret = VENC_PACKAGE_UY0VY1;
            break;

        case HI_DRV_PIX_FMT_YUYV:
            Ret = VENC_PACKAGE_Y0UY1V;
            break;

        case HI_DRV_PIX_FMT_YVYU :
            Ret = VENC_PACKAGE_Y0VY1U;
            break;

        case HI_DRV_PIX_FMT_YUV400:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_YUV411:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            Ret = VENC_U_V;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_YUV_444:
            Ret = VENC_V_U;
            break;

        case HI_DRV_PIX_FMT_YUV410p:
            Ret = VENC_V_U;
            break;

        default:
            Ret = VENC_YUV_NONE;
            break;
    }

    return Ret;
}

static HI_U32 Convert_PIX_Format_FrameInfo(HI_DRV_PIX_FORMAT_E oldFormat)
{
    HI_U32 Ret = HI_SUCCESS;

    switch (oldFormat)
    {
        case HI_DRV_PIX_FMT_NV21:
            Ret = VENC_SEMIPLANNAR_420_VU;
            break;

        case HI_DRV_PIX_FMT_NV12:
            Ret = VENC_SEMIPLANNAR_420_UV;
            break;

        case HI_DRV_PIX_FMT_UYVY:
            Ret = VENC_PACKAGE_422_UYVY;
            break;

        case HI_DRV_PIX_FMT_YUYV:
            Ret = VENC_PACKAGE_422_YUYV;
            break;

        case HI_DRV_PIX_FMT_YVYU :
            Ret = VENC_PACKAGE_422_YVYU;
            break;

        case HI_DRV_PIX_FMT_YUV420p:
            Ret = VENC_PLANNAR_420;
            break;

        case HI_DRV_PIX_FMT_YUV422_1X2:
            Ret = VENC_PLANNAR_422;
            break;

        case HI_DRV_PIX_FMT_YUV422_2X1:
            Ret = VENC_PLANNAR_422;
            break;

        default:
            Ret = VENC_UNKNOW;
            break;
    }

    return Ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : flag :0 -> YUVStoreType; 1 -> YUVSampleType; 2 -> PackageSel
******************************************************************************/
static HI_U32 Convert_PIX_Format(HI_DRV_PIX_FORMAT_E oldFormat,HI_U32 flag)
{
   HI_U32 Ret = HI_SUCCESS;
   if(0 == flag) /*YUVStoreType*/
   {
        Ret = Convert_PIX_Format_YUVStoreType(oldFormat);
   }
   else if (1 == flag) /*YUVSampleType*/
   {
        Ret = Convert_PIX_Format_YUVSampleType(oldFormat);
   }
   else if(2 == flag) /*PackageSel*/
   {
        Ret = Convert_PIX_Format_PackageSel(oldFormat);
   }
   else if (3 == flag)   /*for proc FrameInfo*/
   {
     Ret = Convert_PIX_Format_FrameInfo(oldFormat);
   }
   else
   {
      Ret = HI_FALSE;
   }
   
   return Ret;
}

static HI_S32 QuickEncode_Process(VeduEfl_EncPara_S* EncHandle, HI_HANDLE GetImgHhd)         //成功取帧返回 HI_SUCCESS,连一次都取不成功返回HI_FAILURE
{
    HI_BOOL bLastFrame = HI_FALSE;
    HI_DRV_VIDEO_FRAME_S stImage_temp;
    VeduEfl_EncPara_S* pEncPara;
    HI_U32 u32VeChn;
    pEncPara = EncHandle;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);

    pEncPara->stStat.GetFrameNumTry++;

    if ( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(GetImgHhd, &pEncPara->stImage))
    {
        pEncPara->stStat.GetFrameNumOK++;

        if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum++;
        }

        while (!bLastFrame)
        {
            pEncPara->stStat.GetFrameNumTry++;

            if ( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(GetImgHhd, &stImage_temp))
            {
                pEncPara->stStat.GetFrameNumOK++;

                if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
                {
                    pEncPara->stStat.VpssImgBufNum++;
                }

                pEncPara->stStat.PutFrameNumTry++;
                (*pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
                pEncPara->stStat.PutFrameNumOK++;

                if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
                {
                    pEncPara->stStat.VpssImgBufNum--;

                }

                pEncPara->stStat.QuickEncodeSkip++;
                pEncPara->stImage = stImage_temp;
            }
            else
            {
                bLastFrame = HI_TRUE;
            }
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflToVPSSGetImge(VPSS_HANDLE hVPSS, HI_DRV_VIDEO_FRAME_S* pstImageInfo)  //给VPSS的回调,主动取帧
{
    HI_S32 Ret = HI_FAILURE;
    VeduEfl_EncPara_S*  pEncPara = NULL;

    D_VENC_GET_CHN_FORM_VPSS(hVPSS, pEncPara);

    if (NULL == pEncPara)
    { return HI_FAILURE; }

    if (pEncPara->stSrcInfo_toVPSS.pfGetImage)
    {
        Ret = (pEncPara->stSrcInfo_toVPSS.pfGetImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
    }

    return Ret;
}

HI_S32 VENC_DRV_EflToVPSSRelImge(VPSS_HANDLE hVPSS, HI_DRV_VIDEO_FRAME_S* pstImageInfo)  //给VPSS的回调，主动释放帧
{
    HI_S32 Ret = HI_FAILURE;

    VeduEfl_EncPara_S*  pEncPara = NULL;
    D_VENC_GET_CHN_FORM_VPSS(hVPSS, pEncPara);

    if (NULL == pEncPara)
    { return HI_FAILURE; }

    if (pEncPara->stSrcInfo_toVPSS.pfPutImage)
    {
        Ret = (pEncPara->stSrcInfo_toVPSS.pfPutImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
    }

    return Ret;
}

HI_BOOL VENC_DRV_EflJudgeVPSS( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S* pstFrameInfo)   //调用在获得帧信息后
{
    HI_S32 Ret;
    HI_U32 u32ChnID;
    //HI_BOOL flag = HI_FALSE;
    VeduEfl_EncPara_S* pEncPara;

    D_VENC_GET_CHN(u32ChnID, EncHandle);
    D_VENC_CHECK_CHN(u32ChnID);

    pEncPara = EncHandle;

#if 0
    flag |= (pEncPara->PicHeight != D_VENC_ALIGN_UP(pstFrameInfo->u32Height , 16) );
    flag |= (pEncPara->PicWidth  != D_VENC_ALIGN_UP(pstFrameInfo->u32Width , 16)  );
    flag |= (pstFrameInfo->ePixFormat != HI_DRV_PIX_FMT_NV21 ) && (pstFrameInfo->ePixFormat != HI_DRV_PIX_FMT_NV12); /*目前只支持的源格式*/

    flag = 1;   //全过VPSS

    if (HI_TRUE == flag)  /* need VPSS to Process the Frame from now on*/
    {
        g_stVencChn[u32ChnID].bNeedVPSS = HI_TRUE;
        pEncPara->stSrcInfo.handle       = (HI_HANDLE)g_stVencChn[u32ChnID].hPort[0];
        pEncPara->stSrcInfo.pfGetImage   = pVpssFunc->pfnVpssGetPortFrame;
        pEncPara->stSrcInfo.pfPutImage   = pVpssFunc->pfnVpssRelPortFrame;
        Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS, VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
        (pVpssFunc->pfnVpssEnablePort)(g_stVencChn[u32ChnID].hPort[0], HI_TRUE);
    }
    else
    {
        g_stVencChn[u32ChnID].bNeedVPSS = HI_FALSE;
    }

#else
    g_stVencChn[u32ChnID].bNeedVPSS  = HI_TRUE;
    pEncPara->stSrcInfo.handle       = (HI_HANDLE)g_stVencChn[u32ChnID].hPort[0];
    pEncPara->stSrcInfo.pfGetImage   = pVpssFunc->pfnVpssGetPortFrame;
    pEncPara->stSrcInfo.pfPutImage   = pVpssFunc->pfnVpssRelPortFrame;
    Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS, VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("pfnVpssSetSourceMode VPSS_SOURCE_MODE_USERACTIVE ERROR!\n");
    }

    (pVpssFunc->pfnVpssEnablePort)(g_stVencChn[u32ChnID].hPort[0], HI_TRUE);
#endif
    pEncPara->bNeverEnc = HI_FALSE;
    return g_stVencChn[u32ChnID].bNeedVPSS;
}


HI_VOID VENC_DRV_EflWakeUpThread( HI_VOID)
{
    VENC_DRV_OsalGiveEvent(&g_VENC_Event);
    return ;
}

HI_VOID VENC_DRV_EflSortPriority(HI_VOID)
{
    HI_U32 i, j;

    for ( i = 0; i < MAX_VEDU_CHN - 1; i++)
    {
        for (j =  MAX_VEDU_CHN - 1; j > i; j--)
        {
            if (PriorityTab[1][j] > PriorityTab[1][j - 1])
            {
                HI_U32 temp0 = PriorityTab[0][j];
                HI_U32 temp1 = PriorityTab[1][j];
                PriorityTab[0][j]   = PriorityTab[0][j - 1];
                PriorityTab[1][j]   = PriorityTab[1][j - 1];
                PriorityTab[0][j - 1] = temp0;
                PriorityTab[1][j - 1] = temp1;
            }
        }
    }
}

HI_S32 VENC_DRV_EflSortPriority_2(HI_S8 priority)
{
    HI_U32 i;
    HI_U32 cnt = 0;
    HI_U32 id  = MAX_VEDU_CHN - 1;
    HI_BOOL bFind = 0;

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((priority == PriorityTab[1][i]) && (INVAILD_CHN_FLAG != PriorityTab[0][i]))
        {
            if (!bFind)
            {
                id = i;
                bFind = 1;
            }

            cnt++;
        }
    }

    if (!bFind)
    {
        HI_WARN_VENC("can't fine the channel match with priority(%d)\n", priority);
        return HI_FAILURE;
    }

    if (1 == cnt || (id >= MAX_VEDU_CHN - 1))
    {
        return HI_SUCCESS;
    }

    for (i = 0; (i < (cnt - 1)) && (id < (MAX_VEDU_CHN - 1)); i++, id++)
    {
        HI_U32 temp0 = PriorityTab[0][id];
        HI_U32 temp1 = PriorityTab[1][id];
        PriorityTab[0][id]   = PriorityTab[0][id + 1];
        PriorityTab[1][id]   = PriorityTab[1][id + 1];
        PriorityTab[0][id + 1] = temp0;
        PriorityTab[1][id + 1] = temp1;
    }

    return HI_SUCCESS;
}


HI_S32 VENC_DRV_EflCfgRegVenc( VeduEfl_EncPara_S* EncHandle )
{
    //open the clock before start the channel!    l00228308
    VENC_DRV_BoardInit();
    //add end

    VENC_HAL_CfgReg( EncHandle );
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflReadRegVenc( VeduEfl_EncPara_S* EncHandle )
{
    VENC_HAL_ReadReg( EncHandle );
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflStartOneFrameVenc( VeduEfl_EncPara_S* EncHandle, VeduEfl_EncIn_S* pEncIn )
{
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;

    if (pEncPara->Gop <= 1000)
    {
        VENC_DRV_EflRcOpenOneFrm( EncHandle );
    }
    else
    {
        VENC_DRV_EflRcOpenOneFrm_2( EncHandle );
        //VENC_DRV_EflRcOpenOneFrm( EncHandle );
    }

    /* Make slice header bitstream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SlcHdr_S SlcHdr;

        pEncPara->H264FrmNum = pEncPara->IntraPic ? 0 : pEncPara->H264FrmNum + 1;
        SlcHdr.frame_num  = pEncPara->H264FrmNum;
        SlcHdr.slice_type = pEncPara->IntraPic ? 2 : 0;
        SlcHdr.NumRefIndex = pEncPara->NumRefIndex;
        pEncPara->SlcHdrBits = H264e_MakeSlcHdr(pEncPara->SlcHdrStream,
                                                pEncPara->ReorderStream,
                                                pEncPara->MarkingStream, &SlcHdr);
    }
    else if (pEncPara->Protocol == VEDU_H263)
    {
        VeduEfl_H263e_PicHdr_S PicHdr;

        PicHdr.Pframe = !pEncPara->IntraPic;
        PicHdr.TR = pEncPara->LastTR & 0xFF; /*CurrTR after RC*/
        PicHdr.SrcFmt = pEncPara->H263SrcFmt;
        PicHdr.Wframe = pEncPara->PicWidth;
        PicHdr.Hframe = pEncPara->PicHeight;
        PicHdr.PicQP = pEncPara->StartQp;

        pEncPara->SlcHdrBits = H263e_MakePicHdr(pEncPara->SlcHdrStream, &PicHdr);
    }
    else if (pEncPara->Protocol == VEDU_MPEG4)
    {
        VeduEfl_MP4e_VopHdr_S VopHdr;

        VopHdr.Pframe = !pEncPara->IntraPic;
        VopHdr.PicQP = pEncPara->StartQp;
        VopHdr.Fcode = 2;

        pEncPara->SlcHdrBits = MP4e_MakeVopHdr(pEncPara->SlcHdrStream, &VopHdr);
    }

    pEncPara->SrcYAddr   = pEncIn->BusViY;
    pEncPara->SrcCAddr   = pEncIn->BusViC;
    pEncPara->SrcVAddr   = pEncIn->BusViV;
    pEncPara->SStrideY = pEncIn->ViYStride;
    pEncPara->SStrideC = pEncIn->ViCStride;

    if (pEncPara->LowDlyMod)
    {
        pEncPara->tunlcellAddr = pEncIn->TunlCellAddr;

        if (pEncPara->PicWidth >= 1920)  //D1
        {
            pEncPara->tunlReadIntvl = 3;
        }
        else if (pEncPara->PicWidth >= 720)
        {
            pEncPara->tunlReadIntvl = 2;
        }
        else
        {
            pEncPara->tunlReadIntvl = 1;
        }
    }

    pEncPara->PTS0 = pEncIn->PTS0;
    pEncPara->PTS1 = pEncIn->PTS1;

    pEncPara->RcnIdx = !pEncPara->RcnIdx;
    pEncPara->RStrideY = pEncIn->RStrideY;
    pEncPara->RStrideC = pEncIn->RStrideC;

#if (defined __VENC_98CV200_CONFIG__)
#ifdef HI_SMMU_SUPPORT
    pEncPara->vcpi_srcy_bypass = 0;
    pEncPara->vcpi_srcc_bypass = 0;
    pEncPara->vcpi_srcv_bypass = 0;
#else
    pEncPara->vcpi_srcy_bypass = 1;
    pEncPara->vcpi_srcc_bypass = 1;
    pEncPara->vcpi_srcv_bypass = 1;
#endif
#endif

    if (pEncPara->IntraPic)
    {
        if ((pEncPara->PrependSpsPpsEnable == HI_TRUE) || (pEncPara->stStat.GetFrameNumOK == 1))
        {
            VeduEfl_NaluHdr_S NaluHdr;

            NaluHdr.PacketLen  = 128;
            NaluHdr.InvldByte  = 64 - pEncPara->SpsBits / 8;
            NaluHdr.bLastSlice = 0;
            NaluHdr.Type = 7;

            NaluHdr.PTS0 = pEncPara->PTS0;
            NaluHdr.PTS1 = pEncPara->PTS1;

            if ((pEncPara->Protocol == VEDU_H264) || (pEncPara->Protocol == VEDU_MPEG4))
            {
                VENC_DRV_BufWrite( pstStrBuf, &NaluHdr, 64 );
                VENC_DRV_BufWrite( pstStrBuf, pEncPara->SpsStream, 64 );
            }

            NaluHdr.InvldByte = 64 - pEncPara->PpsBits / 8;
            NaluHdr.Type = 8;

            if (pEncPara->Protocol == VEDU_H264)
            {
                VENC_DRV_BufWrite( pstStrBuf, &NaluHdr, 64 );
                VENC_DRV_BufWrite( pstStrBuf, pEncPara->PpsStream, 64 );
            }

            VENC_DRV_BufUpdateWp( pstStrBuf );

        }
    }

    *(pEncPara->StrmBufRpVirAddr) = pstStrBuf->u32RdTail;
    *(pEncPara->StrmBufWpVirAddr) = pstStrBuf->u32WrHead;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflEndOneFrameVenc( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    if (!pEncPara->LowDlyMod)  // not use low delay mode
    {
        if (!pEncPara->VencBufFull && !pEncPara->VencPbitOverflow)
        {
            /* Read Wp which be changed by HW */
            wrptr = *(pEncPara->StrmBufWpVirAddr);

            VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
            VENC_DRV_BufUpdateWp( pstStrBuf );
        }
        else
        {
            pEncPara->stStat.TooFewBufferSkip++;
        }
    }
    else
    {
        wrptr = *(pEncPara->StrmBufWpVirAddr);
        VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
        VENC_DRV_BufUpdateWp( pstStrBuf );
    }

    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    if (pEncPara->Gop <= 1000)
    {
        VENC_DRV_EflRcCloseOneFrm( EncHandle );
    }
    else
    {
        VENC_DRV_EflRcCloseOneFrm_2( EncHandle );
        //VENC_DRV_EflRcCloseOneFrm( EncHandle );
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflEndOneFrameJpge( VeduEfl_EncPara_S* EncHandle)
{
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    if (!pEncPara->VencBufFull && !pEncPara->VencPbitOverflow)
    {
        /* Read Wp which be changed by HW */
        wrptr = *(pEncPara->StrmBufWpVirAddr);
        VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
        VENC_DRV_BufUpdateWp( pstStrBuf );
    }
    else
    {
        pEncPara->stStat.TooFewBufferSkip++;
    }

    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);
    return HI_SUCCESS;
}
HI_S32 VENC_DRV_EflEndOneSliceVenc( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);
    //if (!pEncPara->VencBufFull || !pEncPara->VencPbitOverflow)
    //{
    /* Read Wp which be changed by HW */
    wrptr = *(pEncPara->StrmBufWpVirAddr);

    VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
    VENC_DRV_BufUpdateWp( pstStrBuf );
    //}

    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    //VENC_DRV_EflRcCloseOneFrm( EncHandle );

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_S32 VENC_DRV_EflChkChnCfg( VeduEfl_EncCfg_S* pEncCfg )
{
    HI_S32 CfgErr = 0;

    CfgErr |= (pEncCfg->FrameWidth < VEDU_MIN_ENC_WIDTH) | (pEncCfg->FrameWidth > VEDU_MAX_ENC_WIDTH);
    CfgErr |= (pEncCfg->FrameHeight < VEDU_MIN_ENC_HEIGHT) | (pEncCfg->FrameHeight > VEDU_MAX_ENC_HEIGHT);
    CfgErr |= (pEncCfg->Protocol > VEDU_JPGE);
    CfgErr |= (pEncCfg->CapLevel > VEDU_CAP_LEVEL_1080P);

    if (VEDU_H264 == pEncCfg->Protocol)
    {
        CfgErr |= ((pEncCfg->Profile > VEDU_H264_HIGH_PROFILE) || (pEncCfg->Profile == VEDU_H264_EXTENDED_PROFILE)  );
    }

    if (CfgErr)
    {
        HI_ERR_VENC("Check channel cfg failed!\n");

        return HI_FAILURE;
    }
    else
    {
        return HI_SUCCESS;
    }
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflFlushStrmHeader( VeduEfl_EncPara_S*  pEncPara )
{
    D_VENC_CHECK_PTR(pEncPara);
    Venc_SetRegDefault(pEncPara);

    /* make sps & pps & VOL stream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SPS_S sps;
        VeduEfl_H264e_PPS_S pps;

        switch (pEncPara->H264HpEn)
        {
            case VEDU_H264_BASELINE_PROFILE:
                sps.ProfileIDC = 66;
                break;

            case VEDU_H264_MAIN_PROFILE:
                sps.ProfileIDC = 77;
                break;

            case VEDU_H264_HIGH_PROFILE:
                sps.ProfileIDC = 100;
                break;

            default:
                sps.ProfileIDC = 100;
                break;
        }

        sps.FrameWidthInMb  = (pEncPara->PicWidth + 15) >> 4;
        sps.FrameHeightInMb = (pEncPara->PicHeight + 15) >> 4;
        sps.FrameCropLeft = 0;
        sps.FrameCropTop    = 0;
        sps.FrameCropRight  = (sps.FrameWidthInMb * 16 - pEncPara->PicWidth) >> 1;
        sps.FrameCropBottom = (sps.FrameHeightInMb * 16 - pEncPara->PicHeight) >> 1;

        pps.ChrQpOffset = pEncPara->ChrQpOffset;
        pps.ConstIntra  = pEncPara->ConstIntra;
        pps.H264HpEn    = (pEncPara->H264HpEn == VEDU_H264_BASELINE_PROFILE) ? 0 : 1;  //pEncPara->H264HpEn;
        pps.H264CabacEn = pEncPara->H264CabacEn;
        pps.pScale8x8   = pEncPara->Scale8x8;

        pEncPara->SpsBits = H264e_MakeSPS(pEncPara->SpsStream, &sps);
        pEncPara->PpsBits = H264e_MakePPS(pEncPara->PpsStream, &pps);
    }
    else if (pEncPara->Protocol == VEDU_H263)
    {
        int w = pEncPara->PicWidth, srcFmt;
        int h = pEncPara->PicHeight;

        if ((w == 128) && (h == 96))
        {
            srcFmt = 1;
        }
        else if ((w == 176) && (h == 144))
        {
            srcFmt = 2;
        }
        else if ((w == 352) && (h == 288))
        {
            srcFmt = 3;
        }
        else if ((w == 704) && (h == 576))
        {
            srcFmt = 4;
        }
        else if ((w == 1408) && (h == 1152))
        {
            srcFmt = 5;
        }
        else
        {
            srcFmt = 6;
        }

        pEncPara->H263SrcFmt = srcFmt;
    }
    else if (pEncPara->Protocol == VEDU_MPEG4)
    {
        VeduEfl_MP4e_VOL_S vol;

        vol.Wframe = pEncPara->PicWidth;
        vol.Hframe = pEncPara->PicHeight;

        pEncPara->SpsBits = MP4e_MakeVOL(pEncPara->SpsStream, &vol);
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflGetLumaSize(VeduEfl_EncCfg_S *pEncCfg, HI_U32 *pLumaSize)
{
    HI_U32 LumaSize = 0;

    switch (pEncCfg->CapLevel)
    {
        case VEDU_CAP_LEVEL_QCIF:
            LumaSize = 144 * D_VENC_ALIGN_UP(176 , VEDU_MMZ_ALIGN);
            break;

        case VEDU_CAP_LEVEL_CIF:
            LumaSize = 288 * D_VENC_ALIGN_UP(352 , VEDU_MMZ_ALIGN);
            break;

        case VEDU_CAP_LEVEL_D1:
            LumaSize = 576 * D_VENC_ALIGN_UP(720 , VEDU_MMZ_ALIGN);
            break;

        case VEDU_CAP_LEVEL_720P:
            LumaSize = 720 * D_VENC_ALIGN_UP(1280 , VEDU_MMZ_ALIGN);
            break;

        case VEDU_CAP_LEVEL_1080P:
            LumaSize = 1088 * D_VENC_ALIGN_UP(1920 , VEDU_MMZ_ALIGN);
            break;

        default:
            return HI_ERR_VENC_INVALID_PARA;
            break;
    }

    *pLumaSize = LumaSize;

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflAllocStreamBuf(VeduEfl_EncPara_S* pEncPara, VeduEfl_EncCfg_S* pEncCfg, HI_U32 BitBufSize, HI_U32 RcnBufSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (pEncCfg->Protocol == VEDU_JPGE)
    {
        MMZ_BUFFER_S stMMZBuf;
        memset(&stMMZBuf, 0 , sizeof(MMZ_BUFFER_S));
        s32Ret = HI_DRV_MMZ_AllocAndMap("VENC_JpegBuf", HI_NULL, BitBufSize, 64, &stMMZBuf);
        if (HI_SUCCESS == s32Ret)
        {
            pEncPara->StreamMMZBuf.u32StartPhyAddr = stMMZBuf.u32StartPhyAddr;
            pEncPara->StreamMMZBuf.pu8StartVirAddr = stMMZBuf.pu8StartVirAddr;
            pEncPara->StreamMMZBuf.u32Size         = stMMZBuf.u32Size;
        }
        else
        {
            HI_ERR_VENC("HI_DRV_VENC_AllocAndMap failed\n");
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = HI_DRV_VENC_AllocAndMap("VENC_SteamBuf", HI_NULL, BitBufSize + RcnBufSize, 64, &pEncPara->StreamMMZBuf);

        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_VENC("HI_DRV_VENC_AllocAndMap failed\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_VOID VENC_DRV_EflInitCfg(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncCfg_S *pEncCfg)
{
    HI_U32 WidthInMb  = (pEncCfg->FrameWidth + 15) >> 4;
    HI_U32 HeightInMb = (pEncCfg->FrameHeight + 15) >> 4;

    /* get channel para */
    pEncPara->Protocol  = pEncCfg->Protocol;
    pEncPara->PicWidth  = pEncCfg->FrameWidth;//WidthInMb  << 4;
    pEncPara->PicHeight = pEncCfg->FrameHeight;//HeightInMb << 4;

    pEncPara->RotationAngle = pEncCfg->RotationAngle;//VEDU_ROTATION_0;

    switch (pEncCfg->RotationAngle)
    {
        case 0:
            pEncPara->RotationAngle = VEDU_ROTATION_0;
            break;

        case 90:
            pEncPara->RotationAngle = VEDU_ROTATION_90;
            break;

        case 270:
            pEncPara->RotationAngle = VEDU_ROTATION_270;
            break;

        case 180:
            pEncPara->RotationAngle = VEDU_ROTATION_180;
            break;

        default:
            pEncPara->RotationAngle = VEDU_ROTATION_0;
            break;
    }

    pEncPara->SlcSplitEn = pEncCfg->SlcSplitEn;
    pEncPara->SplitSize  = pEncCfg->SplitSize;
    pEncPara->QuickEncode = pEncCfg->QuickEncode;
    pEncPara->QLevel     = pEncCfg->QLevel;

    pEncPara->Priority   = pEncCfg->Priority;
    pEncPara->Gop        = pEncCfg->Gop;
    pEncPara->WaitingIsr = 0;
    pEncPara->WaitingIsr_jpge = 0;
    pEncPara->pRegBase   = VeduIpCtx.pRegBase;
    pEncPara->OMXChn     = pEncCfg->bOMXChn;
    pEncPara->ICatchEnable = pEncCfg->bAutoRequestIfrm;
    pEncPara->PrependSpsPpsEnable = pEncCfg->bPrependSpsPps;
    pEncPara->AutoSkipFrameEn = pEncCfg->bRCSkipFrmEn;
    pEncPara->DriftRcSkipThr  = pEncCfg->DriftRCThr;
    //pEncPara->YuvStoreType  = pEncCfg->YuvStoreType;                  //
    //pEncPara->YuvSampleType = VEDU_YUV420;                            //
    //pEncPara->PackageSel = pEncCfg->PackageSel;

    /* init RC para */
    pEncPara->IntraPic = 1;

    /* other */
    pEncPara->bNeverEnc   = HI_TRUE;
    pEncPara->SlcSplitMod = 0;   // 1;                                          //just choose the mb line Mode
    pEncPara->NumRefIndex = 0;

    if (pEncCfg->bOMXChn)
    {
#ifdef __VENC_3716CV200_CONFIG__
        pEncPara->WithoutVPSS = 0;
#else
        pEncPara->WithoutVPSS = 1;
#endif
    }
    else
    {
#if (defined(__VENC_98M_CONFIG__) || defined(__VENC_S5V100_CONFIG__) || defined(__VENC_98CV200_CONFIG__))
        pEncPara->WithoutVPSS = 1;
#else
        pEncPara->WithoutVPSS = 0;
#endif
    }

    if (pEncPara->Protocol == VEDU_H264)
    {
        pEncPara->H264HpEn    = pEncCfg->Profile;
        pEncPara->H264CabacEn = (pEncCfg->Profile == VEDU_H264_BASELINE_PROFILE) ? 0 : 1;
    }

    /* make sps & pps & VOL stream */
    VENC_DRV_EflFlushStrmHeader(pEncPara);

    /* init RC para */
    pEncPara->IntraPic = 1;
    //pEncPara->StartQp = pEncPara->Protocol == VEDU_H264 ? 24 : 5;
    ///////////add by ckf77439

    pEncPara->stRc.MinTimeOfP = 3;
    pEncPara->stRc.MaxTimeOfP = 5;
    pEncPara->stRc.DeltaTimeOfP = 0;
    pEncPara->stRc.AveFrameMinusAveP = 0;     //15; change by tyx
    pEncPara->stRc.StillToMoveDelay = 8;
    pEncPara->stRc.IQpDelta = 2;
    pEncPara->stRc.PQpDelta = 2;
    pEncPara->stRc.MbNum = WidthInMb * HeightInMb;

    ////////////add end

    /* init stat info */
    pEncPara->stStat.GetFrameNumTry  = 0;
    pEncPara->stStat.PutFrameNumTry  = 0;
    pEncPara->stStat.GetStreamNumTry = 0;
    pEncPara->stStat.PutStreamNumTry = 0;
    pEncPara->stStat.GetFrameNumOK  = 0;
    pEncPara->stStat.PutFrameNumOK  = 0;
    pEncPara->stStat.GetStreamNumOK = 0;
    pEncPara->stStat.PutStreamNumOK = 0;
    pEncPara->stStat.BufFullNum = 0;
    pEncPara->stStat.QuickEncodeSkip = 0;
    pEncPara->stStat.FrmRcCtrlSkip   = 0;
    pEncPara->stStat.TooFewBufferSkip = 0;
    pEncPara->stStat.TooManyBitsSkip = 0;
    pEncPara->stStat.TooFewBufferWaitCnt = 0;
    pEncPara->stStat.TooFewBufferQueFailCnt = 0;
    pEncPara->stStat.QueueFrmByAttachCnt = 0;
    pEncPara->stStat.ErrCfgSkip      = 0;
    pEncPara->stStat.SamePTSSkip     = 0;
    pEncPara->stStat.StreamTotalByte = 0;

    pEncPara->stStat.u32RealSendInputRrmRate = 0;
    pEncPara->stStat.u32RealSendOutputFrmRate = 0;

    pEncPara->stStat.QueueNum        = 0;
    pEncPara->stStat.DequeueNum      = 0;
    pEncPara->stStat.VpssImgBufNum   = 0;
    pEncPara->stStat.VpssQueueBufNum = 0;
    pEncPara->stStat.StreamQueueNum  = 0;
    pEncPara->stStat.MsgQueueNum     = 0;
    pEncPara->stStat.UsedStreamBuf   = 0;

    pEncPara->stStat.u32TotalEncodeNum = 0;
    pEncPara->stStat.u32TotalPicBits   = 0;

    /* init src info */
    pEncPara->stSrcInfo.pfGetImage = VENC_DRV_EflGetImage;
    pEncPara->stSrcInfo.pfPutImage = VENC_DRV_EflPutImage;

    pEncPara->stSrcInfo.handle = HI_INVALID_HANDLE;
}

HI_VOID VENC_DRV_EflFreeStreamBuf(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncCfg_S *pEncCfg)
{
    if (VEDU_JPGE == pEncCfg->Protocol)
    {
        MMZ_BUFFER_S stMMZBuf;
        stMMZBuf.pu8StartVirAddr = pEncPara->StreamMMZBuf.pu8StartVirAddr;
        stMMZBuf.u32StartPhyAddr = pEncPara->StreamMMZBuf.u32StartPhyAddr;
        stMMZBuf.u32Size         = pEncPara->StreamMMZBuf.u32Size;
        HI_DRV_MMZ_UnmapAndRelease(&stMMZBuf);
    }
    else
    {
        HI_DRV_VENC_UnmapAndRelease(&pEncPara->StreamMMZBuf);
    }
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     : EncHandle
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflCreateVenc( VeduEfl_EncPara_S** pEncHandle, VeduEfl_EncCfg_S* pEncCfg )
{
    VeduEfl_EncPara_S*  pEncPara;
    HI_U32 LumaSize;
    HI_U32 BusRcnBuf, RcnBufSize; /* 16 aligned, = 2.0 frame mb-yuv */
    HI_U32 BusBitBuf, BitBufSize;
    HI_VOID* pVirBit = HI_NULL;
#ifdef HI_SMMU_SUPPORT
    HI_U32 u32ptaddr;
    HI_U32 u32err_rdaddr;
    HI_U32 u32err_wraddr;
#endif

    /* check channel config */
    if (HI_FAILURE == VENC_DRV_EflChkChnCfg( pEncCfg ))
    {
        return HI_FAILURE;
    }

    if (HI_SUCCESS != VENC_DRV_EflGetLumaSize(pEncCfg, &LumaSize))
    {
        return HI_ERR_VENC_INVALID_PARA;
    }

    RcnBufSize = LumaSize * 3;

    /* malloc encoder parameter & reconstruction frames & bitstream */

    //    pEncPara = (VeduEfl_EncPara_S *) VENC_DRV_OsalMemMalloc( sizeof(VeduEfl_EncPara_S));
    pEncPara = (VeduEfl_EncPara_S*)HI_KMALLOC(HI_ID_VENC, sizeof(VeduEfl_EncPara_S), GFP_KERNEL);

    if (HI_NULL == pEncPara)
    {
        HI_ERR_VENC("HI_KMALLOC failed, size = %d\n", sizeof(VeduEfl_EncPara_S));
        goto error_0;
    }

    memset(pEncPara, 0, sizeof(VeduEfl_EncPara_S));

#ifdef HI_SMMU_SUPPORT
    HI_DRV_SMMU_GetPageTableAddr(&u32ptaddr, &u32err_rdaddr, &u32err_wraddr);
    pEncPara->u32SmmuPageBaseAddr = u32ptaddr;
    pEncPara->u32SmmuErrReadAddr  = u32err_rdaddr;
    pEncPara->u32SmmuErrWriteAddr = u32err_wraddr;
#endif

    if (pEncCfg->SlcSplitEn)
    {
        BitBufSize = pEncCfg->streamBufSize + D_VENC_ALIGN_UP((LumaSize * 3 / 2 / 4), VEDU_MMZ_ALIGN);
    }
    else
    {
        BitBufSize = pEncCfg->streamBufSize + D_VENC_ALIGN_UP((LumaSize * 3 / 2), VEDU_MMZ_ALIGN);
    }

    if (VENC_DRV_EflAllocStreamBuf(pEncPara, pEncCfg, BitBufSize, RcnBufSize) != HI_SUCCESS)
    {
        goto error_1;
    }

    BusBitBuf = pEncPara->StreamMMZBuf.u32StartPhyAddr;
    pVirBit = (HI_VOID*)(pEncPara->StreamMMZBuf.pu8StartVirAddr);
    
    BusRcnBuf = BusBitBuf + BitBufSize;

    /* creat stream buffer lock */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &pEncPara->pStrmBufLock ))
    {
        goto error_2;
    }

    /* ArrangeChnBuf -> rcn & bits */
    pEncPara->RcnYAddr[0] = BusRcnBuf;
    pEncPara->RcnCAddr[0] = BusRcnBuf + LumaSize;
    pEncPara->RcnYAddr[1] = BusRcnBuf + LumaSize * 3 / 2;
    pEncPara->RcnCAddr[1] = BusRcnBuf + LumaSize * 5 / 2;

#ifdef HI_SMMU_SUPPORT

    if (VEDU_JPGE == pEncCfg->Protocol)
    {
        pEncPara->StrmBufRpAddr = BusBitBuf;
        pEncPara->StrmBufWpAddr = BusBitBuf  + 16; /* 16-byte aligned */
    }
    else
    {
        /*vedu wp,rp should be phyAddr*/
        pEncPara->StrmBufRpAddr = VeduEfl_SmmuToPhyAddr(pEncPara->u32SmmuPageBaseAddr, BusBitBuf);
        pEncPara->StrmBufWpAddr = VeduEfl_SmmuToPhyAddr(pEncPara->u32SmmuPageBaseAddr, BusBitBuf  + 16); /* 16-byte aligned */
    }

#else
    pEncPara->StrmBufRpAddr = BusBitBuf;
    pEncPara->StrmBufWpAddr = BusBitBuf  + 16; /* 16-byte aligned */
#endif
    pEncPara->StrmBufAddr   = BusBitBuf  + VEDU_MMZ_ALIGN;
    pEncPara->StrmBufSize   = pEncCfg->streamBufSize - VEDU_MMZ_ALIGN;
    pEncPara->StrmBufRpVirAddr = (HI_U32*)pEncPara->StreamMMZBuf.pu8StartVirAddr;
    pEncPara->StrmBufWpVirAddr = (HI_U32*)((HI_U8*)pEncPara->StrmBufRpVirAddr + 16);

    //    pVirBit = VENC_DRV_OsalBufMap( BusBitBuf, BitBufSize );

    //    pEncPara->Vir2BusOffset = (HI_U32)pVirBit - BusBitBuf;
    pEncPara->Vir2BusOffset = pEncPara->StreamMMZBuf.u32Size;

    /* init cycle buffer for stream */
    if (HI_FAILURE == VENC_DRV_BufInit(&pEncPara->stCycBuf, (HI_U8*)pVirBit + VEDU_MMZ_ALIGN , pEncPara->StrmBufSize, 64))
    {
        goto error_3;
    }

    /************************************* add ******************************************/

    if (pEncCfg->bOMXChn)
    {
        pEncPara->FrameQueue_OMX = VENC_DRV_MngQueueInit(MAX_VEDU_QUEUE_NUM, HI_TRUE);

        if (HI_NULL == pEncPara->FrameQueue_OMX)
        {
            HI_ERR_VENC("failed to init FrameQueue, size = %d\n", sizeof(queue_info_s));
            goto error_3;
        }

        pEncPara->StreamQueue_OMX = VENC_DRV_MngQueueInit(MAX_VEDU_STRM_QUEUE_NUM, HI_TRUE);

        if (HI_NULL == pEncPara->StreamQueue_OMX)
        {
            HI_ERR_VENC("failed to init StreamQueue, size = %d\n", sizeof(queue_info_s));
            goto error_5;
        }

        pEncPara->MsgQueue_OMX = VENC_DRV_MngQueueInit(MSG_QUEUE_NUM, HI_TRUE);

        if (HI_NULL == pEncPara->MsgQueue_OMX)
        {
            HI_ERR_VENC("failed to init MsgQueue, size = %d\n", sizeof(queue_info_s));
            goto error_6;
        }
    }
    else
    {
        pEncPara->FrameQueue = VENC_DRV_MngQueueInit(MAX_VEDU_QUEUE_NUM, HI_FALSE);

        if (HI_NULL == pEncPara->FrameQueue)
        {
            HI_ERR_VENC("failed to init FrameQueue, size = %d\n", sizeof(queue_info_s));
            goto error_3;
        }

        pEncPara->FrameDequeue = VENC_DRV_MngQueueInit(MAX_VEDU_QUEUE_NUM * 2, HI_FALSE);

        if (HI_NULL == pEncPara->FrameDequeue)
        {
            HI_ERR_VENC("failed to init FrameDequeue, size = %d\n", sizeof(queue_info_s));
            goto error_5;
        }
    }

    /*************************************************************************************************/

    VENC_DRV_EflInitCfg(pEncPara, pEncCfg);

    /* get return val */
    *pEncHandle = pEncPara;

    return HI_SUCCESS;

error_6:

    if (pEncCfg->bOMXChn)
    {
        VENC_DRV_MngQueueDeinit(pEncPara->StreamQueue_OMX);
    }

error_5:

    if (pEncCfg->bOMXChn)
    {
        VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue_OMX);
    }
    else
    {
        VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue);
    }

error_3:
    VENC_DRV_OsalLockDestroy( pEncPara->pStrmBufLock );
error_2:

    VENC_DRV_EflFreeStreamBuf(pEncPara, pEncCfg);

error_1:
    HI_KFREE(HI_ID_VENC, pEncPara);
error_0:
    return HI_FAILURE;
}

HI_S32 VENC_DRV_EflDestroyVenc( VeduEfl_EncPara_S* EncHandle )
{
    VENC_BUFFER_S sMBufVenc_temp  = {0};
    HI_U32 i = 0;
    HI_U32 ChanId = 0;
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    if (pEncPara == NULL)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    D_VENC_GET_CHN(ChanId, EncHandle);
    D_VENC_CHECK_CHN(ChanId);

    VENC_DRV_OsalLockDestroy( pEncPara->pStrmBufLock );

    if (VEDU_JPGE == pEncPara->Protocol)
    {
        MMZ_BUFFER_S stMMZBuf;
        stMMZBuf.pu8StartVirAddr = pEncPara->StreamMMZBuf.pu8StartVirAddr;
        stMMZBuf.u32StartPhyAddr = pEncPara->StreamMMZBuf.u32StartPhyAddr;
        stMMZBuf.u32Size         = pEncPara->StreamMMZBuf.u32Size;
        HI_DRV_MMZ_UnmapAndRelease(&stMMZBuf);
    }
    else
    {
        HI_DRV_VENC_UnmapAndRelease(&pEncPara->StreamMMZBuf);
    }

    if (pEncPara->OMXChn)
    {
        for (i = 0; i < OMX_OUTPUT_PORT_BUFFER_MAX; i++)
        {
            if ( 0 != g_stKernelOmxmmz[ChanId][i].pu8StartVirAddr)
            {
                sMBufVenc_temp.pu8StartVirAddr = g_stKernelOmxmmz[ChanId][i].pu8StartVirAddr;
                HI_DRV_VENC_Unmap(&sMBufVenc_temp);
            }
        }

        if ( VENC_DRV_MngQueueDeinit(pEncPara->StreamQueue_OMX))
        {
            HI_ERR_VENC("HI_KFREE failed to free StreamQueue_OMX, size = %d\n", sizeof(queue_info_s));
            return HI_FAILURE;
        }

        if ( VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue_OMX))
        {
            HI_ERR_VENC("HI_KFREE failed to free FrameQueue_OMX, size = %d\n", sizeof(queue_info_s));
            return HI_FAILURE;
        }

        if ( VENC_DRV_MngQueueDeinit(pEncPara->MsgQueue_OMX))
        {
            HI_ERR_VENC("HI_KFREE failed to free MsgQueue_OMX, size = %d\n", sizeof(queue_info_s));
            return HI_FAILURE;
        }

    }
    else
    {
        if ( VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue))
        {
            HI_ERR_VENC("HI_KFREE failed to free FrameQueue, size = %d\n", sizeof(queue_info_s));
            return HI_FAILURE;
        }

        if ( VENC_DRV_MngQueueDeinit(pEncPara->FrameDequeue))
        {
            HI_ERR_VENC("HI_KFREE failed to free FrameDequeue, size = %d\n", sizeof(queue_info_s));
            return HI_FAILURE;
        }
    }

    HI_KFREE(HI_ID_VENC, pEncPara);

    /*    VENC_DRV_OsalBufUnmap((HI_VOID*)(pEncPara->StrmBufAddr + pEncPara->Vir2BusOffset));
        VENC_DRV_OsalBufFree ( pEncPara->StrmBufAddr );
        VENC_DRV_OsalMemFree ( pEncPara );
     */
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflAttachInput( VeduEfl_EncPara_S* EncHandle, VeduEfl_SrcInfo_S* pSrcInfo )
{
    HI_U32 u32ChnID = 0;
    VeduEfl_EncPara_S*  pEncPara;
    D_VENC_GET_CHN(u32ChnID, EncHandle);
    D_VENC_CHECK_CHN(u32ChnID);

    pEncPara = EncHandle;

    if (pEncPara->OMXChn)
    {
        if (!pSrcInfo->pfGetImage_OMX)
        {
            return HI_FAILURE;
        }
    }
    else
    {
        if ((!pSrcInfo->pfGetImage) || (!pSrcInfo->pfPutImage) || (!pSrcInfo->pfChangeInfo))
        {
            return HI_FAILURE;
        }
    }

    pEncPara->stSrcInfo = *pSrcInfo;
    pEncPara->stSrcInfo_toVPSS = *pSrcInfo;

    g_stVencChn[u32ChnID].stSrcInfo = *pSrcInfo;
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflDetachInput( VeduEfl_EncPara_S* EncHandle, VeduEfl_SrcInfo_S* pSrcInfo )
{
    HI_U32 u32ChnID = 0;
    VeduEfl_EncPara_S*  pEncPara;
    D_VENC_GET_CHN(u32ChnID, EncHandle);
    D_VENC_CHECK_CHN(u32ChnID);
    pEncPara = EncHandle;
    pEncPara->stSrcInfo = *pSrcInfo;
    pEncPara->stSrcInfo_toVPSS = *pSrcInfo;
    g_stVencChn[u32ChnID].stSrcInfo = *pSrcInfo;
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_BOOL g_flag_I_insert = 0;
HI_VOID VENC_DRV_EflRequestIframe( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    pEncPara->InterFrmCnt = pEncPara->Gop - 1;

    g_flag_I_insert = 1;
    pEncPara->RcStart = 1;
    
    return;
}

HI_S32 VENC_DRV_EflRcGetAttr( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    pRcAttr->BitRate = pEncPara->BitRate;
    pRcAttr->OutFrmRate = pEncPara->VoFrmRate;
    pRcAttr->InFrmRate = pEncPara->ViFrmRate;

    pRcAttr->MaxQp  = pEncPara->MaxQp;
    pRcAttr->MinQp  = pEncPara->MinQp;
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflAllocBuf2Jpge( VeduEfl_EncPara_S* EncHandle, HI_U32 CapLevel)
{
    HI_S32 s32Ret = 0;
    HI_U32 u32Size = 0;
    VeduEfl_EncPara_S*  pEncPara = (VeduEfl_EncPara_S*)EncHandle;

    if (pEncPara == NULL)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    switch (CapLevel)
    {
        case VEDU_CAP_LEVEL_QCIF:
            u32Size = 176 * 144 * 3 / 2;
            break;

        case VEDU_CAP_LEVEL_CIF:
            u32Size = 352 * 288 * 3 / 2;
            break;

        case VEDU_CAP_LEVEL_D1:
            u32Size = 720 * 576 * 3 / 2;
            break;

        case VEDU_CAP_LEVEL_720P:
            u32Size = 1280 * 720 * 3 / 2;
            break;

        case VEDU_CAP_LEVEL_1080P:
            u32Size = 1920 * 1088 * 3 / 2;
            break;

        default:
            return HI_ERR_VENC_INVALID_PARA;
    }

    s32Ret = HI_DRV_VENC_AllocAndMap("Venc_JpegOutBuf", HI_NULL, u32Size, 64, &pEncPara->JpgMMZBuf);

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("HI_DRV_VENC_AllocAndMap failed\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflFreeBuf2Jpge( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    if (pEncPara == NULL)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }


    HI_DRV_VENC_UnmapAndRelease(&pEncPara->JpgMMZBuf);


    return HI_SUCCESS;
}

#ifndef __VEDU_NEW_RC_ALG__
HI_S32 VENC_DRV_EflRcAttrInit( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32 i;

    if ((pRcAttr->InFrmRate > VEDU_MAX_INPUT_FRAME_RATE) || (pRcAttr->InFrmRate < pRcAttr->OutFrmRate))
    {
        return HI_FAILURE;
    }

    if (pRcAttr->OutFrmRate == 0)
    {
        return HI_FAILURE;
    }

    pEncPara->BitRate   = pRcAttr->BitRate;
    pEncPara->VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->MaxQp     = pRcAttr->MaxQp;
    pEncPara->MinQp     = pRcAttr->MinQp;

    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*initialize frame rate control parameter*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                       pEncPara->BitRate / pEncPara->ViFrmRate);

    pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;
    pEncPara->TrCount = pEncPara->ViFrmRate;

    for (i = 0; i < pEncPara->ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->VoFrmRate;

        if (pEncPara->TrCount > pEncPara->ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    /*initialize re-start parameter*/
    pEncPara->RcStart = 1;

    return HI_SUCCESS;
}

#else

HI_S32 VENC_DRV_EflRcAttrInit( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32 i;

    if ((pRcAttr->InFrmRate > VEDU_MAX_INPUT_FRAME_RATE) || (pRcAttr->InFrmRate < pRcAttr->OutFrmRate))
    {
        return HI_FAILURE;
    }

    if (pRcAttr->OutFrmRate == 0)
    {
        return HI_FAILURE;
    }

    pEncPara->BitRate   = pRcAttr->BitRate;
    pEncPara->VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->MaxQp     = pRcAttr->MaxQp;
    pEncPara->MinQp     = pRcAttr->MinQp;

    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*initialize frame rate control parameter*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                       pEncPara->BitRate / pEncPara->ViFrmRate);

    pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;
    pEncPara->TrCount = pEncPara->ViFrmRate;

    for (i = 0; i < pEncPara->ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->VoFrmRate;

        if (pEncPara->TrCount > pEncPara->ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    /*initialize re-start parameter*/
    pEncPara->RcStart = 1;

    return HI_SUCCESS;
}

#endif

HI_S32 VENC_DRV_EflRcFrmRateCtrl( VeduEfl_EncPara_S* EncHandle, HI_U32 TR )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32  DiffTR = (TR - pEncPara->LastTR);

    HI_U32 i;

    if (pEncPara->RcStart)
    {
        pEncPara->TrCount  = pEncPara->ViFrmRate;
        pEncPara->TrCount += pEncPara->VoFrmRate;
        pEncPara->LastTR   = TR;
        pEncPara->IntraPic = 1;
        pEncPara->FrmCnt   = 0;

        if (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < (64 * 8))
        {
            pEncPara->stStat.TooFewBufferSkip++;
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
            return HI_FAILURE;
        }
    }
    else
    {
        /* don't run too many loop */
        if (DiffTR < VEDU_TR_STEP)
        {
            return HI_FAILURE;
        }
        else if (DiffTR > 0x1f)
        {
            DiffTR = 0x1f;
        }

        /* LOST frames into fifo */
        for (i = 0; i < DiffTR - VEDU_TR_STEP; i += VEDU_TR_STEP)
        {
            pEncPara->TrCount += pEncPara->VoFrmRate;
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
        }

        /* this frame */
        pEncPara->TrCount += pEncPara->VoFrmRate;
        pEncPara->LastTR = TR;

        /* don't care too many Lost frames */
        if (pEncPara->TrCount > pEncPara->VoFrmRate + pEncPara->ViFrmRate)
        {
            pEncPara->TrCount = pEncPara->VoFrmRate + pEncPara->ViFrmRate;
        }

        /* skip this frame */
        if ((pEncPara->TrCount <= pEncPara->ViFrmRate)      /* time to skip */
            || (pEncPara->stBitsFifo.Sum > (pEncPara->BitRate / 100 * (pEncPara->DriftRcSkipThr + 100))) /* too many bits */ //RC_Threshold = 20%
            || (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < 64 * 8))/* too few buffer */
        {
            if (pEncPara->TrCount <= pEncPara->ViFrmRate)
            {
                pEncPara->stStat.FrmRcCtrlSkip++;
            }
            else if (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < 64 * 8)
            {
                pEncPara->stStat.TooFewBufferSkip++;
            }
            else
            {
                if ((!pEncPara->OMXChn) && (pEncPara->AutoSkipFrameEn))
                {
                    pEncPara->stStat.TooManyBitsSkip++;
                }
                else
                {
                    goto OMX_SKIP;
                }

            }

            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
            return HI_FAILURE;
        }

    OMX_SKIP:

        /* intra or inter based gop */
        if (pEncPara->InterFrmCnt >= pEncPara->Gop - 1)
        {
            pEncPara->IntraPic = 1;
            pEncPara->FrmCnt   = 0;
            pEncPara->stRc.FrmNumInGop = 0;
        }
        else
        {
            pEncPara->IntraPic = 0;
            pEncPara->stRc.FrmNumInGop = pEncPara->InterFrmCnt + 1;
        }
    }

    return HI_SUCCESS;
}

//#ifndef __VEDU_NEW_RC_ALG__
HI_S32 VENC_DRV_EflRcOpenOneFrm_2( VeduEfl_EncPara_S* EncHandle )     //old version
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    #define VEDU_CLIP3(x, y, z) (((x) < (y)) ? (y) : (((x) > (z)) ? (z) : (x)))

        /* TargetBits */
        if ( pEncPara->stBitsFifo.Sum > pEncPara->BitRate )
        {
            pEncPara->TargetBits = pEncPara->MeanBit -
                                   (pEncPara->stBitsFifo.Sum - pEncPara->BitRate) / pEncPara->VoFrmRate;
        }
        else
        {
            pEncPara->TargetBits = pEncPara->MeanBit +
                                   (pEncPara->BitRate - pEncPara->stBitsFifo.Sum) / pEncPara->VoFrmRate;
        }

        if ( pEncPara->IntraPic )
        {
            /* StartQp */
            if (pEncPara->Gop > 5)
            {
                pEncPara->StartQp -= 0;
            }
            else if (pEncPara->PicBits > pEncPara->TargetBits)
            {
                if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 8)
                { pEncPara->StartQp++; }
            }
            else
            {
                if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 8)
                { pEncPara->StartQp--; }
            }

            if (g_flag_I_insert) /*if request I frame, qp+2,so I frame bits down*/
            {
                pEncPara->StartQp += 2;
                g_flag_I_insert = 0;
            }
        }
        else
        {
            /* StartQp */
            if (pEncPara->PicBits > pEncPara->TargetBits)
            {
                if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 8)
                { pEncPara->StartQp++; }

                if (pEncPara->Protocol == VEDU_H264)
                    if (pEncPara->PicBits - pEncPara->TargetBits > pEncPara->TargetBits / 4)
                    { pEncPara->StartQp++; }
            }
            else
            {
                if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 8)
                { pEncPara->StartQp--; }

                if (pEncPara->Protocol == VEDU_H264)
                    if (pEncPara->TargetBits - pEncPara->PicBits > pEncPara->TargetBits / 4)
                    { pEncPara->StartQp--; }
            }

            /* VBR */
            if ( pEncPara->VBRflag )
            {
                pEncPara->MinQp += pEncPara->PicLevel * (pEncPara->Protocol == VEDU_H264 ? 2 : 1);
            }
        }

    if (pEncPara->Protocol == VEDU_H264)
    { pEncPara->StartQp = VEDU_CLIP3(pEncPara->StartQp, pEncPara->MinQp, pEncPara->MaxQp); } //40
    else
    { pEncPara->StartQp = VEDU_CLIP3(pEncPara->StartQp, pEncPara->MinQp, pEncPara->MaxQp); }

#if 1

    if (pEncPara->IntraPic)   /*control I frame QP*/
    {
        pEncPara->StartQp = VEDU_CLIP3(pEncPara->StartQp, 23, 35);
    }

#endif

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcCloseOneFrm_2( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    if (pEncPara->VencBufFull || pEncPara->VencPbitOverflow) /* Buffer Full -> LOST */
    {
        pEncPara->H264FrmNum--;
        pEncPara->RcnIdx = !pEncPara->RcnIdx;

        if (!pEncPara->RcStart)
        {
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0 );
        }

        pEncPara->stStat.BufFullNum++;

        return HI_FAILURE;
    }
    else
    {
        pEncPara->RcStart  = 0;
        pEncPara->TrCount -= pEncPara->ViFrmRate;

        if (pEncPara->IntraPic)
        {
            pEncPara->InterFrmCnt = 0;
        }
        else
        {
            pEncPara->InterFrmCnt++;
        }

        VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, pEncPara->PicBits );
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflRcSetAttr( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    VeduEfl_Rc_S stRc_old;
    HI_U32 i;

    if ((pRcAttr->InFrmRate > VEDU_MAX_INPUT_FRAME_RATE) || (pRcAttr->InFrmRate < pRcAttr->OutFrmRate))
    {
        return HI_FAILURE;
    }

    if (pRcAttr->OutFrmRate == 0)
    {
        return HI_FAILURE;
    }

    memcpy(&stRc_old, &(pEncPara->stRc), sizeof(stRc_old));

    /* updata the RC structure*/
    pEncPara->stRc.AveFrameBits   = pRcAttr->BitRate / pRcAttr->OutFrmRate;

    if (pEncPara->Gop == pRcAttr->Gop)
    {
        pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AveFrameBits * (pEncPara->Gop - pEncPara->stRc.FrmNumInGop - 1);
    }
    else if (pRcAttr->Gop <= (pEncPara->stRc.FrmNumInGop + 1))
    {
        VENC_DRV_EflRequestIframe(EncHandle);
        goto init_process;
    }
    else
    {
        pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AveFrameBits * (pRcAttr->Gop - pEncPara->stRc.FrmNumInGop - 1);
    }

    pEncPara->stRc.TotalBitsLeft = pEncPara->stRc.GopBitsLeft;

init_process:

    /*initialize  parameter*/
    pEncPara->BitRate   = pRcAttr->BitRate;
    pEncPara->VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->TrCount   = pEncPara->ViFrmRate;
    pEncPara->Gop       = pRcAttr->Gop;
    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    if (pEncPara->Gop <= 1000)
    {
        pEncPara->stRc.ConsAvePBits = pEncPara->stRc.GopBitsLeft / (pEncPara->Gop - pEncPara->stRc.FrmNumInGop);
    }
    else
    {
        pEncPara->stRc.ConsAvePBits = pEncPara->stRc.AveFrameBits;
    }

    /*updata the RC strmBuffer*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                       pEncPara->BitRate / pEncPara->ViFrmRate);

    pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;

    for (i = 0; i < pEncPara->ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->VoFrmRate;

        if (pEncPara->TrCount > pEncPara->ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    return HI_SUCCESS;
}

#if 0
static HI_VOID VENC_DRV_EflIntraFrmRateChange( VeduEfl_EncPara_S* pEncPara, HI_U32 newViFrmRate, HI_UNF_VENC_CHN_ATTR_S stSrcUserCfg)
{
    HI_U32 i;
    pEncPara->ViFrmRate = newViFrmRate;

    if (stSrcUserCfg.u32TargetFrmRate > pEncPara->ViFrmRate)
    {
        pEncPara->VoFrmRate = pEncPara->ViFrmRate;

    }
    else
    {
        pEncPara->VoFrmRate = stSrcUserCfg.u32TargetFrmRate;
    }

    /* updata the RC structure*/
    pEncPara->stRc.AveFrameBits   = pEncPara->BitRate / pEncPara->VoFrmRate;

    pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AveFrameBits * (pEncPara->Gop - pEncPara->stRc.FrmNumInGop - 1);

    pEncPara->stRc.TotalBitsLeft += pEncPara->stRc.GopBitsLeft;

    /*initialize  parameter*/
    pEncPara->TrCount   = pEncPara->ViFrmRate;

    /*updata the RC strmBuffer*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                       pEncPara->BitRate / pEncPara->ViFrmRate);

    pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;

    for (i = 0; i < pEncPara->ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->VoFrmRate;

        if (pEncPara->TrCount > pEncPara->ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    return;
}
#endif
#if 1
/******************************************************************************
Function   :
Description: 获取尽量可靠的输入帧率 与对应的输出帧率 并做相应的处理
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_VOID VENC_DRV_EflGetFrmRate( VeduEfl_EncPara_S* pEncPara, HI_UNF_VENC_CHN_ATTR_S stSrcUserCfg)
{
    HI_U32 diff1 = 0;
    HI_U32 newViFrmRate = 0;
    HI_U32 i = 0;

    HI_U32 max1, max2;

    if (0/*!pEncPara->AutoSkipFrameEn*/)
    {
        newViFrmRate = pEncPara->LastFrmRate[0] ;
    }
    else
    {
        if (pEncPara->LastFrmRate[5] == 0)  /*前6 帧还没有满，一直相信最大的那个帧率*/
        {
            max1 = D_VENC_RC_MAX3(pEncPara->LastFrmRate[0], pEncPara->LastFrmRate[1], pEncPara->LastFrmRate[2]);
            max2 = D_VENC_RC_MAX3(pEncPara->LastFrmRate[3], pEncPara->LastFrmRate[4], pEncPara->LastFrmRate[5]);
            newViFrmRate = D_VENC_RC_MAX(max1, max2);
        }
        else
        {
            if (pEncPara->bFrmRateSataError)
            {
                newViFrmRate = pEncPara->ViFrmRate;
            }
            else if (pEncPara->LastSecInputFrmRate[1] != 0) /*前2s帧率都统计出来了*/
            {
                diff1 = D_VENC_RC_ABS(pEncPara->LastSecInputFrmRate[0], pEncPara->LastSecInputFrmRate[1]);

                if ( diff1 <= 3)
                {
                    newViFrmRate = D_VENC_RC_MAX(pEncPara->LastSecInputFrmRate[0], pEncPara->LastSecInputFrmRate[1]);
                }
                else
                {
                    newViFrmRate = pEncPara->LastSecInputFrmRate[0];
                }
            }
            else if (pEncPara->LastSecInputFrmRate[0] != 0)  /*前1s帧率已经统计出来*/
            {
                newViFrmRate = pEncPara->LastSecInputFrmRate[0];
            }
            else  /*第一s的帧率还没有统计出来*/
            {
                max1 = D_VENC_RC_MAX3(pEncPara->LastFrmRate[0], pEncPara->LastFrmRate[1], pEncPara->LastFrmRate[2]);
                max2 = D_VENC_RC_MAX3(pEncPara->LastFrmRate[3], pEncPara->LastFrmRate[4], pEncPara->LastFrmRate[5]);
                newViFrmRate = D_VENC_RC_MAX(max1, max2);
            }
        }

        if (pEncPara->LastSecFrameCnt >= (1 + newViFrmRate))
        {
            if ( pEncPara->LastFrmRate[0] > newViFrmRate )
            {
                newViFrmRate = pEncPara->LastFrmRate[0] + (pEncPara->LastFrmRate[0] - newViFrmRate) + 6;
                pEncPara->bFrmRateSataError = 1;
            }
            else if (newViFrmRate < pEncPara->LastSecFrameCnt)
            {
                newViFrmRate = pEncPara->LastSecFrameCnt + 10;
                pEncPara->bFrmRateSataError = 1;
            }

        }
    }

    if ((newViFrmRate != pEncPara->ViFrmRate) && (newViFrmRate <= FrmRateCntBufNum))
    {
        if (newViFrmRate >= FrmRateCntBufNum)
        {
            newViFrmRate = FrmRateCntBufNum;   // fifo buffer range
        }

        pEncPara->ViFrmRate = newViFrmRate;

        if (stSrcUserCfg.u32TargetFrmRate > pEncPara->ViFrmRate)
        {
            pEncPara->VoFrmRate = pEncPara->ViFrmRate;

        }
        else
        {
            pEncPara->VoFrmRate = stSrcUserCfg.u32TargetFrmRate;
        }

        /* updata the RC structure*/
        if (pEncPara->bFrmRateSataError)    //error!
        {
            pEncPara->stRc.AveFrameBits   = pEncPara->BitRate / (pEncPara->VoFrmRate + 8);

            pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AveFrameBits * (pEncPara->Gop - pEncPara->stRc.FrmNumInGop - 1);

            pEncPara->stRc.TotalBitsLeft += pEncPara->stRc.GopBitsLeft;
        }
        else
        {
            pEncPara->stRc.AveFrameBits   = pEncPara->BitRate / pEncPara->VoFrmRate;

            pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AveFrameBits * (pEncPara->Gop - pEncPara->stRc.FrmNumInGop - 1);

            pEncPara->stRc.TotalBitsLeft += pEncPara->stRc.GopBitsLeft;
        }


        /*initialize  parameter*/
        pEncPara->TrCount   = pEncPara->ViFrmRate;

        /*updata the RC strmBuffer*/
        VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->ViFrmRate,
                           pEncPara->BitRate / pEncPara->ViFrmRate);

        pEncPara->MeanBit = pEncPara->BitRate / pEncPara->VoFrmRate;

        for (i = 0; i < pEncPara->ViFrmRate; i++)
        {
            pEncPara->TrCount += pEncPara->VoFrmRate;

            if (pEncPara->TrCount > pEncPara->ViFrmRate)
            {
                pEncPara->TrCount -= pEncPara->ViFrmRate;
                VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
            }
            else
            {
                VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
            }
        }
    }

    return;
}

HI_VOID VENC_DRV_EflRcOfFirstFrm(VeduEfl_EncPara_S* pEncPara)
{
    HI_S32 TimeOfPFrame = 0;
    
    pEncPara->stRc.AveFrameBits =  pEncPara->BitRate  / pEncPara->VoFrmRate;

    if (pEncPara->Gop <= 1000)
    {
        pEncPara->stRc.GopBits = pEncPara->stRc.AveFrameBits * pEncPara->Gop;
    }

    if (g_flag_I_insert)
    {
        pEncPara->stRc.FrmNumSeq = 0;
    }
    else
    {
        pEncPara->stRc.FrmNumSeq++;
    }

    D_VENC_RC_UPDATA(pEncPara->stRc.SenChaNum, pEncPara->stRc.CurSenChaNum);
    pEncPara->stRc.CurSenChaNum = 0;

    if (pEncPara->Gop <= 1000)
    {
        pEncPara->stRc.GopBitsLeft = pEncPara->stRc.GopBits;
        pEncPara->stRc.TotalBitsLeft = pEncPara->stRc.GopBits;
    }

    pEncPara->stRc.TotalTargetBitsUsedInGop = 0;
    pEncPara->stRc.TotalBitsUsedInGop = 0;

    TimeOfPFrame = 5;

    pEncPara->stRc.StillFrameNum++;
    pEncPara->stRc.StillFrameNum2++;

    TimeOfPFrame = TimeOfPFrame + pEncPara->stRc.DeltaTimeOfP;
    TimeOfPFrame = D_VENC_RC_MAX(TimeOfPFrame , pEncPara->stRc.MinTimeOfP);
    TimeOfPFrame = D_VENC_RC_MIN(TimeOfPFrame , pEncPara->stRc.MaxTimeOfP);

    if (pEncPara->Gop <= 1000)
    {
        pEncPara->stRc.TargetBits = pEncPara->stRc.GopBits / (TimeOfPFrame + pEncPara->Gop - 1) * TimeOfPFrame;
    }
    else
    {
        pEncPara->stRc.TargetBits = pEncPara->stRc.AveFrameBits;
    }

    pEncPara->stRc.CurQp = VEDU_DRV_EflRcInitQp(pEncPara->stRc.TargetBits, pEncPara->PicWidth, pEncPara->PicHeight);
    pEncPara->stRc.InitialQp = pEncPara->stRc.CurQp;
}

HI_VOID VENC_DRV_EflRcBitsInit(VeduEfl_EncPara_S* pEncPara)
{
    if (pEncPara->stRc.FrmNumInGop == 0) //I Frame
    {
        pEncPara->stRc.AveFrameBits =  pEncPara->BitRate  / pEncPara->VoFrmRate;

        if (pEncPara->Gop <= 1000)
        {
            pEncPara->stRc.GopBits = pEncPara->stRc.AveFrameBits * pEncPara->Gop;
            D_VENC_RC_UPDATA(pEncPara->stRc.SenChaNum, pEncPara->stRc.CurSenChaNum);
            pEncPara->stRc.CurSenChaNum = 0;
            pEncPara->stRc.PreGopBitsLeft = pEncPara->stRc.GopBitsLeft;
            pEncPara->stRc.GopBitsLeft = pEncPara->stRc.GopBits;
            pEncPara->stRc.TotalBitsLeft += pEncPara->stRc.GopBits;
        }

        pEncPara->stRc.TotalTargetBitsUsedInGop = 0;
        pEncPara->stRc.TotalBitsUsedInGop = 0;
    }
    else
    {
        if (pEncPara->stRc.FrmNumInGop == 1) //first P Frame in Gop
        {
            pEncPara->stRc.NumIMBCurFrm = pEncPara->stRc.AveOfIMB; //GOP中第一个P帧，前一帧IMB比例更新为I帧前六帧的平均值

            if (pEncPara->Gop <= 1000)
            {
                pEncPara->stRc.ConsAvePBits = pEncPara->stRc.GopBitsLeft / (pEncPara->Gop - pEncPara->stRc.FrmNumInGop);
            }
            else
            {
                pEncPara->stRc.ConsAvePBits = pEncPara->stRc.AveFrameBits;
            }
        }

        if (pEncPara->Gop <= 1000)
        {
            pEncPara->stRc.AvePBits = pEncPara->stRc.GopBitsLeft / (pEncPara->Gop - pEncPara->stRc.FrmNumInGop);
        }
        else
        {
            pEncPara->stRc.AvePBits = pEncPara->stRc.AveFrameBits;
        }

        pEncPara->stRc.AvePBits = D_VENC_RC_MAX(pEncPara->stRc.AvePBits, 5 * pEncPara->stRc.AveFrameBits / 10);

        if (pEncPara->VBRflag)
        {
            pEncPara->MinQp += pEncPara->PicLevel * (pEncPara->Protocol == VEDU_H264 ? 2 : 1);
        }
    }
}

HI_VOID VENC_DRV_EflRcOfIntraFrm(VeduEfl_EncPara_S* pEncPara, HI_S32 FrameIMBRatio, HI_S32 IMBRatio)
{
    HI_S32 TimeOfPFrame = 0;
    
    if (pEncPara->stRc.IPreQp[0] == 0)
    {
        TimeOfPFrame = 5;
    }
    else
    {
        TimeOfPFrame = 9 - (12 * IMBRatio + 50) / 100;
    }

    if (pEncPara->stRc.SenChaNum[0] > 1)
    {
        TimeOfPFrame = TimeOfPFrame - pEncPara->stRc.SenChaNum[0];
        TimeOfPFrame = D_VENC_RC_MAX(2, TimeOfPFrame);
    }

    if (pEncPara->stRc.PreMeanQp > 40)
    {
        TimeOfPFrame = TimeOfPFrame - (pEncPara->stRc.AveMeanQp - 40) / 2;
    }

    if (pEncPara->stRc.StillMoveNum > 12)
    {
        TimeOfPFrame = TimeOfPFrame - 2;
    }

    VENC_DRV_EflRcCalIFrmQp(&pEncPara->stRc, pEncPara->PicWidth, pEncPara->PicHeight, TimeOfPFrame, pEncPara->Gop, FrameIMBRatio);
}

HI_VOID VENC_DRV_EflRcOfPFrm(VeduEfl_EncPara_S* pEncPara, HI_S32 FrameIMBRatio)
{
    if (pEncPara->stRc.PreType == 0) //not first P frame
    {
        VENC_DRV_EflRcCalPFrmQp(&pEncPara->stRc, pEncPara, FrameIMBRatio);
    }
    else
    {
        if (pEncPara->stRc.PPreQp[0] == 0) //first P frame in the sequence
        {
            pEncPara->stRc.CurQp = pEncPara->stRc.PreQp + 3;
            pEncPara->stRc.TargetBits = pEncPara->stRc.ConsAvePBits;

        }
        else//first P Frame in Gop
        {
            pEncPara->stRc.CurQp = D_VENC_RC_MAX(pEncPara->stRc.PPreQp[0], pEncPara->stRc.PreQp + 0);
            //pEncPara->stRc.TargetBits = D_VENC_RC_MAX(pEncPara->stRc.AvePBits, pEncPara->stRc.PTotalBits[0]);
            pEncPara->stRc.TargetBits = pEncPara->stRc.ConsAvePBits;
        }
    }

    if (pEncPara->stRc.SceneChangeFlag == 2)
    {
        pEncPara->stRc.CurQp = pEncPara->stRc.CurQp + 2;
    }
}

HI_VOID VENC_DRV_EflRcSetStillNum(VeduEfl_EncPara_S* pEncPara, HI_S32 FrameIMBRatio)
{
    if (FrameIMBRatio >= 8)
    {
        pEncPara->stRc.StillMoveNum ++;
        pEncPara->stRc.StillMoveNum = D_VENC_RC_MIN(pEncPara->stRc.StillMoveNum, 20);
    }
    else
    {
        pEncPara->stRc.StillMoveNum --;

        if (FrameIMBRatio <= 1)
        {
            pEncPara->stRc.StillMoveNum --;
        }

        if (FrameIMBRatio == 0)
        {
            pEncPara->stRc.StillMoveNum --;
        }

        pEncPara->stRc.StillMoveNum = D_VENC_RC_MAX(pEncPara->stRc.StillMoveNum, 0);
    }

    D_VENC_RC_UPDATA(pEncPara->stRc.MeanQp, pEncPara->stRc.PreMeanQp);
    pEncPara->stRc.AveMeanQp = VENC_DRV_EflRcAverage(pEncPara->stRc.MeanQp, 6);

    if (pEncPara->stRc.AveMeanQp < 35)
    {
        pEncPara->stRc.StillFrameNum ++;
        pEncPara->stRc.StillFrameNum = D_VENC_RC_MIN(pEncPara->stRc.StillFrameNum, 40);

        if (pEncPara->stRc.AveMeanQp < 30)
        {
            pEncPara->stRc.StillFrameNum2 ++;
            pEncPara->stRc.StillFrameNum2 = D_VENC_RC_MIN(pEncPara->stRc.StillFrameNum2, 40);
        }
        else
        {
            pEncPara->stRc.StillFrameNum2 = pEncPara->stRc.StillFrameNum2 - 5;
            pEncPara->stRc.StillFrameNum2 = D_VENC_RC_MAX(pEncPara->stRc.StillFrameNum2, 0);
        }
    }
    else
    {
        pEncPara->stRc.StillFrameNum = pEncPara->stRc.StillFrameNum - 5;
        pEncPara->stRc.StillFrameNum = D_VENC_RC_MAX(pEncPara->stRc.StillFrameNum, 0);
    }
}

HI_S32 VENC_DRV_EflRcOpenOneFrm( VeduEfl_EncPara_S* EncHandle)
{
    HI_S32 IMBRatio = 0;//ratio of IMB in previous six frames
    //HI_S32 AveImbRatioSeq;//average ratio of IMB in sequence
    HI_S32 FrameIMBRatio = 0;
    HI_S32 TempQp;
    VeduEfl_EncPara_S* pEncPara = EncHandle;

    if (pEncPara->RcStart)
    {
        VENC_DRV_EflRcOfFirstFrm(pEncPara);
    }
    else
    {
        VENC_DRV_EflRcBitsInit(pEncPara);        

        if (pEncPara->stRc.PreSceneChangeFlag == 1)
        {
            pEncPara->stRc.NumIMBCurFrm = pEncPara->stRc.AveOfIMB;
        }

        D_VENC_RC_UPDATA(pEncPara->stRc.NumIMB, pEncPara->stRc.NumIMBCurFrm);

#ifdef SceneChangeDetect
        VENC_DRV_EflSceChaDetect(&pEncPara->stRc, pEncPara->PicWidth, pEncPara->PicHeight, pEncPara->IntraPic);
#endif

        pEncPara->stRc.AveOfIMB = VENC_DRV_EflRcAverage(pEncPara->stRc.NumIMB, 6);
        IMBRatio = pEncPara->stRc.AveOfIMB * 100 / pEncPara->stRc.MbNum;

        FrameIMBRatio = pEncPara->stRc.NumIMB[0] * 100 / pEncPara->stRc.MbNum;
        pEncPara->stRc.ImbRatioSeq += FrameIMBRatio;

        VENC_DRV_EflRcSetStillNum(pEncPara, FrameIMBRatio);

        //AveImbRatioSeq = pEncPara->stRc.ImbRatioSeq / D_VENC_RC_MAX(pEncPara->stRc.FrmNumSeq, 1);

        if (pEncPara->stRc.FrmNumSeq == 1000)
        {
            pEncPara->stRc.ImbRatioSeq = 0;
            pEncPara->stRc.FrmNumSeq = 0;
        }

        pEncPara->stRc.FrmNumSeq ++;

        //    #ifdef SceneChangeDetect
        if ((pEncPara->stRc.SceneChangeFlag == 1) && (!pEncPara->IntraPic))
        {
            pEncPara->stRc.CurSenChaNum ++;

            if (pEncPara->stRc.PreSceneChangeFlag == 1)
            {
                pEncPara->stRc.CurQp = D_VENC_RC_MAX(pEncPara->stRc.PreQp, pEncPara->stRc.InitialQp) + 3;
            }
            else
            {
                TempQp = pEncPara->stRc.FrmNumInGop / 5 + 2 ;
                TempQp = D_VENC_RC_MIN(TempQp, 6);

                if (D_VENC_RC_ABS(pEncPara->stRc.PreAveY , pEncPara->stRc.AverageY[0]) > 20)
                {
                    pEncPara->stRc.CurQp = D_VENC_RC_MAX(pEncPara->stRc.PreQp + TempQp, pEncPara->stRc.InitialQp);
                }
                else
                {
                    pEncPara->stRc.CurQp = D_VENC_RC_MAX((pEncPara->stRc.InitialQp + pEncPara->stRc.PreQp) / 2 , pEncPara->stRc.PreQp)
                                           + TempQp;
                }

                if (pEncPara->stRc.CurSenChaNum > 1) //若场景切换次数大于1，增大QP 以控制码率
                {
                    if (pEncPara->stRc.CurSenChaNum == 2)
                    {
                        pEncPara->stRc.CurQp += 2;
                    }

                    if (pEncPara->stRc.CurSenChaNum > 2)
                    {
                        pEncPara->stRc.CurQp += 3;
                    }
                }
            }

            pEncPara->stRc.CurQp = D_VENC_RC_MAX( pEncPara->stRc.CurQp, pEncPara->stRc.PreQp - 3 );
            /*********使其线性变化***********/
            pEncPara->stRc.TargetBits =  (pEncPara->stRc.FrmNumInGop - 2) * (pEncPara->stRc.AvePBits - pEncPara->stRc.ITotalBits[0]) / (pEncPara->Gop - 1)
                                         + pEncPara->stRc.ITotalBits[0];
        }
        //      #endif
        else if (pEncPara->IntraPic)
        {
            VENC_DRV_EflRcOfIntraFrm(pEncPara, FrameIMBRatio, IMBRatio);
        }
        else   // p_Frame
        {
            VENC_DRV_EflRcOfPFrm(pEncPara, FrameIMBRatio);
        }
    }

    if (pEncPara->bFrmRateSataError)  //BitRate should be down!
    {
        pEncPara->stRc.CurQp++;
        pEncPara->TargetBits -=  pEncPara->MeanBit / pEncPara->VoFrmRate;
    }
    else if (pEncPara->stBitsFifo.Sum * 100 > pEncPara->BitRate * 105)
    {
        pEncPara->stRc.CurQp++;
    }
    else if (pEncPara->stBitsFifo.Sum * 100 < pEncPara->BitRate * 95)
    {
        pEncPara->stRc.CurQp--;
    }

    if (!pEncPara->IntraPic && pEncPara->stRc.PreType == 0) //
    {
        pEncPara->stRc.CurQp = D_VENC_RC_CLIP3(pEncPara->stRc.CurQp, pEncPara->stRc.PreQp - 3, pEncPara->stRc.PreQp + 3);
    }

    if (!pEncPara->IntraPic && pEncPara->stRc.PreType == 1)
    {
        pEncPara->stRc.CurQp = D_VENC_RC_CLIP3(pEncPara->stRc.CurQp, pEncPara->stRc.PreQp - 3, pEncPara->stRc.PreQp + 3);
    }

    /*if the I frame is insert form user control the QP with [23,35]*/
    if (g_flag_I_insert)
    {
        pEncPara->stRc.CurQp += 1;
        pEncPara->stRc.CurQp = D_VENC_RC_CLIP3(pEncPara->stRc.CurQp, 23, 35);
        g_flag_I_insert = 0;
    }

    pEncPara->stRc.CurQp = D_VENC_RC_CLIP3(pEncPara->stRc.CurQp, pEncPara->MinQp, pEncPara->MaxQp);   //QS

    pEncPara->StartQp = pEncPara->stRc.CurQp;
    pEncPara->TargetBits = pEncPara->stRc.TargetBits;


    return HI_SUCCESS;

}
//////////////change end
#endif

HI_S32 VENC_DRV_EflRcCloseOneFrm( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    if (pEncPara->VencBufFull || pEncPara->VencPbitOverflow) /* Buffer Full -> LOST */
    {
        pEncPara->H264FrmNum--;
        pEncPara->RcnIdx = !pEncPara->RcnIdx;

        if (!pEncPara->RcStart)
        {
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0 );
        }

        pEncPara->stStat.BufFullNum++;

        return HI_FAILURE;
    }
    else
    {
        pEncPara->RcStart  = 0;
        pEncPara->TrCount -= pEncPara->ViFrmRate;

        if (pEncPara->IntraPic)
        {
            pEncPara->InterFrmCnt = 0;
        }
        else
        {
            pEncPara->InterFrmCnt++;
        }

        VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, pEncPara->PicBits );

        ////////////add by ckf77439

        pEncPara->stRc.GopBitsLeft -= pEncPara->PicBits;

        if (pEncPara->IntraPic)
        {
            D_VENC_RC_UPDATA(pEncPara->stRc.ITotalBits, pEncPara->PicBits);
            D_VENC_RC_UPDATA(pEncPara->stRc.IPreQp, pEncPara->stRc.CurQp);
        }
        else
        {
            D_VENC_RC_UPDATA(pEncPara->stRc.PTotalBits, pEncPara->PicBits);
            D_VENC_RC_UPDATA(pEncPara->stRc.PPreQp, pEncPara->stRc.CurQp);
        }

        pEncPara->stRc.PreQp = pEncPara->stRc.CurQp;
        pEncPara->stRc.PreMeanQp = pEncPara->MeanQP;

        pEncPara->stRc.PreType = pEncPara->IntraPic;
        pEncPara->stRc.PreTargetBits = pEncPara->stRc.TargetBits;

        pEncPara->stRc.TotalBitsUsedInGop += pEncPara->PicBits;
        pEncPara->stRc.TotalTargetBitsUsedInGop += pEncPara->stRc.TargetBits;

        pEncPara->stRc.PreSceneChangeFlag = pEncPara->stRc.SceneChangeFlag;
        pEncPara->stRc.TotalBitsLeft -=  pEncPara->PicBits;


        ////////////add end
    }

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflSetResolution( VeduEfl_EncPara_S* EncHandle, HI_U32 FrameWidth, HI_U32 FrameHeight )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    VeduEfl_RcAttr_S RcAttr;
    HI_U32 WidthInMb  = (FrameWidth + 15) >> 4;
    HI_U32 HeightInMb = (FrameHeight + 15) >> 4;
    HI_U32 LumaSize = (WidthInMb * HeightInMb) << 8;

    /* check config */
    if (LumaSize > pEncPara->RcnCAddr[0] - pEncPara->RcnYAddr[0])
    {
        return HI_FAILURE;
    }

    VENC_DRV_EflRcGetAttr( EncHandle, &RcAttr );

    if (HI_SUCCESS != VENC_DRV_EflRcAttrInit( EncHandle, &RcAttr ))
    {
        HI_ERR_VENC("config venc Rate Control Attribute err!.\n");
        return HI_FAILURE;
    }

    //pEncPara->H264HpEn    = 1;                                          //打开High profile
    //pEncPara->H264CabacEn = 1;                                          //打开cabac 编码
    pEncPara->SlcSplitMod = 1;
    pEncPara->NumRefIndex = 0;
    pEncPara->PicWidth  = FrameWidth;
    pEncPara->PicHeight = FrameHeight;

    VENC_DRV_EflFlushStrmHeader( pEncPara );
    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/

HI_S32 VENC_DRV_EflStartVenc( VeduEfl_EncPara_S* EncHandle )
{
    HI_U32 i, j;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* pEncPara = NULL;
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == NULL)
        {
            VeduChnCtx[i].EncHandle = EncHandle;
            pEncPara = EncHandle;
            pEncPara->pRegBase = VeduIpCtx.pRegBase;
            break;
        }
    }

    if ((i >= VENC_MAX_CHN_NUM) || (pEncPara == NULL))
    {
        VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    for (j = 0; j < MAX_VEDU_CHN; j++)
    {
        if ( INVAILD_CHN_FLAG == PriorityTab[0][j])
        {
            PriorityTab[0][j] = i;
            PriorityTab[1][j] = pEncPara->Priority;
            VENC_DRV_EflSortPriority();
            break;
        }
    }

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    pEncPara->bNeverEnc = HI_TRUE;
    pEncPara->bFirstNal2Send = HI_TRUE;

    return HI_SUCCESS;
}


HI_VOID VENC_DRV_EflStopVenc_SortPri( VeduEfl_EncPara_S* EncHandle, HI_U32 *pChanId)
{
    HI_U32 i, j;
    
    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == EncHandle)
        {
            VeduChnCtx[i].EncHandle = NULL;
            break;
        }
    }

    for (j = 0; (i < MAX_VEDU_CHN) && (j < MAX_VEDU_CHN); j++)
    {
        if ( i == PriorityTab[0][j])
        {
            PriorityTab[0][j] = INVAILD_CHN_FLAG;
            PriorityTab[1][j] = 0;
            VENC_DRV_EflSortPriority();
            break;
        }
    }

    *pChanId = i;
}

HI_S32 VENC_DRV_EflStopVenc( VeduEfl_EncPara_S* EncHandle )
{
    HI_U32 i, ChanId;
    HI_U32 timeCnt = 0;

    VEDU_LOCK_FLAG flag;
    HI_U32 u32VeChn;
    VeduEfl_EncPara_S* pEncPara = NULL;

    D_VENC_GET_CHN(u32VeChn, EncHandle);
    D_VENC_CHECK_CHN(u32VeChn);
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    VENC_DRV_EflStopVenc_SortPri( EncHandle, &ChanId);

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (ChanId == MAX_VEDU_CHN)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    /* wait finish last frame */
    while ((((VeduEfl_EncPara_S*)EncHandle)->WaitingIsr) && (timeCnt < 1000))
    {
        msleep(1);
        timeCnt++;
        HI_WARN_VENC("wait the VEDU isr!!! timeCnt = %d\n", timeCnt);
    }

    if (timeCnt == 1000)
    {
        HI_ERR_VENC("wait the VEDU isr time out!!Force to stop the channel\n");
    }

#ifdef __VENC_SUPPORT_JPGE__

    /* wait finish last frame */
    while (((VeduEfl_EncPara_S*)EncHandle)->WaitingIsr_jpge)
    {
        msleep(1);
    }

#endif

    VENC_DRV_OsalGiveEvent(&g_VencWait_Stream[u32VeChn]);

    /* rfresh the queue of the Img */
    pEncPara = EncHandle;

    if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)
    {
        if (!pEncPara->OMXChn)  /* for venc */
        {
            if (pEncPara->WithoutVPSS)
            {
                VENC_DRV_MngQueueRefresh(pEncPara->FrameQueue);
                VENC_DRV_MngQueueRefresh(pEncPara->FrameDequeue);
            }
            else
            {
                VENC_DRV_Refresh_FromVPSS(EncHandle);
            }
        }
        else                    /* for omxvenc */
        {
            //frush the buffer  by lmq
            VENC_DRV_EflFlushPort_OMX(EncHandle, ALL_PORT, HI_TRUE);

        }
    }
    else
    {
        VENC_DRV_EflRlsAllFrame( EncHandle);
    }

    pEncPara->bFrmRateSataError = 0;

    for (i = 0; i < 6; i++)
    {
        pEncPara->LastFrmRate[i] = 0;
    }

    for (i = 0; i < 2; i++)
    {
        pEncPara->LastSecInputFrmRate[i] = 0;
    }

    /*recycle the stream buffer*/
    //pEncPara->stCycBuf.u32RdHead = 0;
    //pEncPara->stCycBuf.u32RdTail = 0;

    if (g_stVencChn[u32VeChn].hJPGE == HI_INVALID_HANDLE)
    {
        pEncPara->stCycBuf.u32WrHead = pEncPara->stCycBuf.u32RdHead;
        pEncPara->stCycBuf.u32WrTail = pEncPara->stCycBuf.u32RdHead;
    }
    else   //jpeg
    {
        VeduEfl_NaluHdr_S* pNaluHdr;
        HI_U32    u32NewWrHead;
        VALG_CRCL_BUF_S stCycBuf_1;
        memcpy(&stCycBuf_1, &pEncPara->stCycBuf, sizeof(VALG_CRCL_BUF_S));

        pNaluHdr = (VeduEfl_NaluHdr_S*)(stCycBuf_1.pBase + stCycBuf_1.u32RdHead);

        if (pNaluHdr->bLastSlice == 1)    /*当前包是图像数据*/
        {
            if (HI_SUCCESS == VENC_DRV_BufGetNextRp(&stCycBuf_1, pNaluHdr->PacketLen))
            {
                u32NewWrHead = stCycBuf_1.u32RdHead;
            }
            else   /*下一包已没数据*/
            {
                u32NewWrHead = stCycBuf_1.u32WrHead;
            }
        }
        else        /*当前包不是有效数据:或者是头，或者没有数据*/
        {
            u32NewWrHead = stCycBuf_1.u32RdHead;
        }

        pEncPara->stCycBuf.u32WrHead = u32NewWrHead;
        pEncPara->stCycBuf.u32WrTail = u32NewWrHead;
    }


    if (g_stVencChn[u32VeChn].bOMXChn)
    {
        VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_STOP_DONE, HI_SUCCESS, NULL);
        pEncPara->stStat.MsgQueueNum++;
    }

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_VOID VENC_DRV_DbgLowdelayEvent(HI_S32 ChanId, HI_LD_Event_ID_E eventId, HI_U32 u32FrameId, HI_HANDLE hSrcHd)
{
    HI_LD_Event_S evt;
    HI_U32 TmpTime = 0;
    HI_DRV_SYS_GetTimeStampMs(&TmpTime);

    evt.evt_id = eventId;
    evt.frame = u32FrameId;
    evt.handle = hSrcHd;
    evt.time = TmpTime;
#if 0

    if (1 == g_LowDelayCalcStat[ChanId])
    {
        HI_DRV_LD_Notify_Event(&evt);
    }

#else
    HI_DRV_LD_Notify_Event(&evt);
#endif
}
static HI_S32 VENC_DRV_EflCheckImgCfg(const HI_DRV_VIDEO_FRAME_S* pstPreImage, HI_U32 yuvStoreType)
{
    HI_BOOL flag = 0;

    flag |= (pstPreImage->u32Width > VEDU_MAX_ENC_WIDTH) || (pstPreImage->u32Width < VEDU_MIN_ENC_WIDTH);
    flag |= (pstPreImage->u32Height > VEDU_MAX_ENC_HEIGHT) || (pstPreImage->u32Height < VEDU_MIN_ENC_HEIGHT);
    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Y) || (pstPreImage->stBufAddr[0].u32PhyAddr_Y % 16);
    flag |= (!pstPreImage->stBufAddr[0].u32Stride_Y ) || (pstPreImage->stBufAddr[0].u32Stride_Y  % 16);

    if (VENC_STORE_PLANNAR == yuvStoreType)
    {
        flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C % 16);
        flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Cr) || (pstPreImage->stBufAddr[0].u32PhyAddr_Cr % 16);
        flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  % 16);
        flag |= (!pstPreImage->stBufAddr[0].u32Stride_Cr)  || (pstPreImage->stBufAddr[0].u32Stride_Cr % 16);

        flag |= (pstPreImage->stBufAddr[0].u32Stride_Cr != pstPreImage->stBufAddr[0].u32Stride_C);
    }
    else if (VENC_STORE_SEMIPLANNAR == yuvStoreType)
    {
        flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C % 16);
        flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  % 16);
    }

    if (HI_TRUE == flag)
    {
        return HI_FAILURE;
    }
    else { return HI_SUCCESS; }
}

static HI_S32 VENC_DRV_EflQueryChn_X_Resize(HI_U32 u32ChnID, VeduEfl_EncPara_S* pEncPara)
{
    /* configured the resolving power dynamically */
    if ((pEncPara->stImage.u32Width != pEncPara->PicWidth) || (pEncPara->stImage.u32Height != pEncPara->PicHeight))
    {
        if ((pEncPara->stImage.u32Width == g_stVencChn[u32ChnID].stChnUserCfg.u32Width)
            && (pEncPara->stImage.u32Height == g_stVencChn[u32ChnID].stChnUserCfg.u32Height))
        {
            if (HI_INVALID_HANDLE == g_stVencChn[u32ChnID].hJPGE)   /*h264*/
            {
                HI_S32 s32Ret = HI_FAILURE;
                
                VENC_DRV_EflRequestIframe(g_stVencChn[u32ChnID].hVEncHandle);
                s32Ret = VENC_DRV_EflSetResolution(g_stVencChn[u32ChnID].hVEncHandle, pEncPara->stImage.u32Width, pEncPara->stImage.u32Height);

                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_VENC("VeduEfl_SetResolution err:%#x.\n", s32Ret);
                    pEncPara->stStat.ErrCfgSkip++;
                    pEncPara->stStat.PutFrameNumTry++;
                    (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
                    pEncPara->stStat.PutFrameNumOK++;

                    if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
                    {
                        pEncPara->stStat.VpssImgBufNum--;
                    }

                    pEncPara->PTS0 = pEncPara->stImage.u32Pts;
                    return HI_FAILURE;
                }
            }
            else   /* JPEG */
            {
                pEncPara->PicWidth  = pEncPara->stImage.u32Width;
                pEncPara->PicHeight = pEncPara->stImage.u32Height;
            }
        }
        else
        {
            HI_ERR_VENC("ERR:Different resolution between the frame Info and the Encoder Cfg!Encode: %dX%d,FrameInfo: %dX%d\n",
                        g_stVencChn[u32ChnID].stChnUserCfg.u32Width, g_stVencChn[u32ChnID].stChnUserCfg.u32Height,
                        pEncPara->stImage.u32Width, pEncPara->stImage.u32Height);
            pEncPara->stStat.ErrCfgSkip++;
            pEncPara->stStat.PutFrameNumTry++;
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
            pEncPara->stStat.PutFrameNumOK++;

            if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
            {
                pEncPara->stStat.VpssImgBufNum--;
            }

            pEncPara->PTS0 = pEncPara->stImage.u32Pts;
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}


static HI_VOID VENC_DRV_EflQueryChn_X_Stride(VeduEfl_EncIn_S* pEncIn, VeduEfl_EncPara_S* pEncPara)
{
    if (pEncPara->StoreFmt == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
    }

    if ((VENC_STORE_SEMIPLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C * 2;
    }
    else if ((VENC_STORE_PACKAGE == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else if ((VENC_STORE_PLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_420 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
}

static HI_S32 VENC_DRV_EflQueryChn_X(HI_U32 u32ChnID, VeduEfl_EncIn_S* pEncIn )    //for venc
{
    HI_HANDLE hHd;
    VeduEfl_EncPara_S* pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32ChnID].hVEncHandle;

    if (g_stVencChn[u32ChnID].bNeedVPSS)
    {
        hHd = g_stVencChn[u32ChnID].hPort[0];
    }
    else if (g_stVencChn[u32ChnID].bFrameBufMng)
    {
        hHd = g_stVencChn[u32ChnID].hUsrHandle;
    }
    else
    {
        hHd = pEncPara->stSrcInfo.handle;
    }

    if (pEncPara->QuickEncode)
    {
        if ( HI_FAILURE == QuickEncode_Process(g_stVencChn[u32ChnID].hVEncHandle, hHd))
        { return HI_FAILURE; }
    }
    else
    {
        pEncPara->stStat.GetFrameNumTry++;

        if (!pEncPara->stSrcInfo.pfGetImage)
        {
            HI_ERR_VENC(" pfGetImage is NULL!!\n");
            return HI_FAILURE;
        }

        if (HI_SUCCESS != (pEncPara->stSrcInfo.pfGetImage)(hHd, &(pEncPara->stImage)))
        { return HI_FAILURE; }

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)
        {
            pEncPara->stStat.VpssImgBufNum++;
        }

        pEncPara->stStat.GetFrameNumOK++;
    }

    /* don't re-get */
    if (pEncPara->PTS0 == pEncPara->stImage.u32Pts)
    {
        pEncPara->stStat.PutFrameNumTry++;              //add
        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &(pEncPara->stImage));
        pEncPara->stStat.PutFrameNumOK++;               //add

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum--;
        }

        pEncPara->stStat.SamePTSSkip++;
        return HI_FAILURE;
    }

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)|(defined __VENC_98CV200_CONFIG__))             //for lowdelay 
    if (pEncPara->stImage.u32TunnelPhyAddr)      //待明确!!
    {
        pEncPara->LowDlyMod = HI_TRUE;
        pEncIn->TunlCellAddr = pEncPara->stImage.u32TunnelPhyAddr;
    }
    else
    {
        pEncPara->LowDlyMod = HI_FALSE;
        pEncIn->TunlCellAddr = 0;
    }
#endif

    /* video encoder does frame rate control by two value: input frame rate and target frame rate */
    /* input frame rate is calculated by timer mechanism accurately */
    /* target frame rate is input by user and can be changed dynamiclly */
    if (HI_ID_BUTT != g_stVencChn[u32ChnID].enSrcModId)
    {
        D_VENC_RC_UPDATA(pEncPara->LastFrmRate, pEncPara->stImage.u32FrameRate / 1000);
        pEncPara->LastSecFrameCnt++;
        VENC_DRV_EflGetFrmRate(pEncPara, g_stVencChn[u32ChnID].stChnUserCfg);
    }
    else
    {
        pEncPara->ViFrmRate = g_stVencChn[u32ChnID].stChnUserCfg.u32InputFrmRate;
        pEncPara->VoFrmRate = g_stVencChn[u32ChnID].stChnUserCfg.u32TargetFrmRate;
    }

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->VoFrmRate;

    if (HI_SUCCESS != VENC_DRV_EflQueryChn_X_Resize(u32ChnID, pEncPara))
    {
        return HI_FAILURE;
    }

    /* skip - frame rate ctrl */
    if (HI_SUCCESS
        != VENC_DRV_EflRcFrmRateCtrl( g_stVencChn[u32ChnID].hVEncHandle, pEncPara->stStat.PutFrameNumOK/*pImagePriv->u32FrmCnt*/))
    {
        pEncPara->stStat.PutFrameNumTry++;
        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
        pEncPara->stStat.PutFrameNumOK++;

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum--;
        }

        pEncPara->PTS0 = pEncPara->stImage.u32Pts;
        return HI_FAILURE;
    }

    pEncPara->YuvStoreType  = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 0);
    pEncPara->YuvSampleType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 1); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->PackageSel    = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 2); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->StoreFmt      = pEncPara->YuvStoreType;
    pEncPara->stStat.u32FrameType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 3);

    /* check the picture resolving power ,stride ,addr info first*/
    if ( HI_SUCCESS != VENC_DRV_EflCheckImgCfg(&pEncPara->stImage, pEncPara->YuvStoreType) )
    {
        HI_ERR_VENC("stImg cfg erro!!\n");
        pEncPara->stStat.ErrCfgSkip++;
        pEncPara->stStat.PutFrameNumTry++;
        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);
        pEncPara->stStat.PutFrameNumOK++;

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum--;
        }

        pEncPara->PTS0 = pEncPara->stImage.u32Pts;
        return HI_FAILURE;
    }

    pEncIn->BusViY = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    pEncIn->BusViC = pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;
    if (pEncPara->YuvStoreType == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //?????????????,planer?????;
    }

    VENC_DRV_EflQueryChn_X_Stride(pEncIn, pEncPara);

#ifdef __VENC_DPT_ONLY__
    pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);//HI_SYS_GET_STRIDE(pEncPara->stImage.u32Width);//D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , VEDU_MMZ_ALIGN);   //16
#else
    pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);
#endif
    pEncIn->RStrideC  = pEncIn->RStrideY;

    pEncIn->PTS0 = pEncPara->stImage.u32Pts;
    pEncIn->PTS1 = 0;
    return HI_SUCCESS;
}


static HI_S32 VENC_DRV_EflQueryChn_Y1(HI_U32 u32ChnID, VeduEfl_EncIn_S* pEncIn)   //for omxvenc without VPSS
{
    //HI_HANDLE hHd;
    //HI_S32 s32Ret = 0;
    VeduEfl_EncPara_S* pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32ChnID].hVEncHandle;

    pEncPara->stStat.GetFrameNumTry++;

    if (!pEncPara->stSrcInfo.pfGetImage_OMX)
    { return HI_FAILURE; }

    if (HI_SUCCESS != (pEncPara->stSrcInfo.pfGetImage_OMX)(pEncPara->stSrcInfo.handle, &(pEncPara->stImage_OMX)))
    { return HI_FAILURE; }

    pEncPara->stStat.GetFrameNumOK++;

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->VoFrmRate;

    /* video encoder does frame rate control by two value: input frame rate and target frame rate */
    /* input frame rate is calculated by timer mechanism accurately */
    /* target frame rate is input by user and can be changed dynamiclly */

    /* skip - frame rate ctrl */
    if (HI_SUCCESS
        != VENC_DRV_EflRcFrmRateCtrl( g_stVencChn[u32ChnID].hVEncHandle, pEncPara->stStat.PutFrameNumOK))
    {
        pEncPara->stStat.PutFrameNumTry++;

        VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_INPUT_DONE, HI_SUCCESS , &(pEncPara->stImage_OMX));
        pEncPara->stStat.MsgQueueNum++;
        pEncPara->stStat.PutFrameNumOK++;

        pEncPara->PTS0 = pEncPara->stImage_OMX.timestamp0;
        pEncPara->PTS1 = pEncPara->stImage_OMX.timestamp1;
        return HI_FAILURE;
    }


    pEncPara->YuvStoreType  = pEncPara->stImage_OMX.store_type;//VENC_STORE_SEMIPLANNAR;
    pEncPara->YuvSampleType = pEncPara->stImage_OMX.sample_type;//VENC_YUV_420;//Convert_PIX_Format(pEncPara->stImage.ePixFormat,1);
    pEncPara->PackageSel    = pEncPara->stImage_OMX.package_sel;//VENC_V_U;
    pEncPara->StoreFmt      = pEncPara->YuvStoreType;

    if (pEncPara->stImage_OMX.MetaDateFlag)   //storeMetaData case
    {
        pEncIn->BusViY = pEncPara->stImage_OMX.stMetaData.bufferaddr_Phy + pEncPara->stImage_OMX.offset; //pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    }
    else
    {
        pEncIn->BusViY = pEncPara->stImage_OMX.bufferaddr_Phy + pEncPara->stImage_OMX.offset; //pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    }

    pEncIn->BusViC = pEncIn->BusViY + pEncPara->stImage_OMX.offset_YC;//pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;

    if (pEncPara->StoreFmt == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncIn->BusViY + pEncPara->stImage_OMX.offset_YCr;//pEncPara->stImage_OMX.strideC*pEncPara->stImage_OMX.picHeight /2;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
    }

    if ((VENC_STORE_SEMIPLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType)) /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC * 2;
        pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_422;
    }
    else if ((VENC_STORE_PACKAGE == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;

        //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
        switch (pEncPara->PackageSel)
        {
            case VENC_PACKAGE_UY0VY1:
                pEncPara->stStat.u32FrameType = VENC_PACKAGE_422_UYVY;
                break;

            case VENC_PACKAGE_Y0UY1V:
                pEncPara->stStat.u32FrameType = VENC_PACKAGE_422_YUYV;
                break;

            case VENC_PACKAGE_Y0VY1U:
                pEncPara->stStat.u32FrameType = VENC_PACKAGE_422_YVYU;
                break;

            default:
                pEncPara->stStat.u32FrameType = VENC_UNKNOW;
                break;
        }
    }
    else if ((VENC_STORE_PLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_420 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC;
        pEncPara->stStat.u32FrameType = VENC_PLANNAR_420;
    }
    else
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC;

        if ((VENC_STORE_PLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))
        {
            pEncPara->stStat.u32FrameType = VENC_PLANNAR_422;
        }
        else if ((VENC_STORE_SEMIPLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_420 == pEncPara->YuvSampleType))
        {
            if (pEncPara->PackageSel == 0)
            {
                pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_420_VU;
            }
            else
            {
                pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_420_UV;
            }
        }

    }

    pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->PicWidth, 16);
    pEncIn->RStrideC  = pEncIn->RStrideY;

    pEncIn->PTS0 = pEncPara->stImage_OMX.timestamp0;
    pEncIn->PTS1 = pEncPara->stImage_OMX.timestamp1;
    return 0;

}

static HI_S32 VENC_DRV_EflQueryChn_Y2_Resize(HI_U32 u32ChnID, VeduEfl_EncPara_S* pEncPara)
{
    HI_S32 s32Ret = HI_FAILURE;
    
    /* configured the resolving power dynamically */
    if ((pEncPara->stImage.u32Width != pEncPara->PicWidth) || (pEncPara->stImage.u32Height != pEncPara->PicHeight))
    {
        if ((pEncPara->stImage.u32Width == g_stVencChn[u32ChnID].stChnUserCfg.u32Width)
            && (pEncPara->stImage.u32Height == g_stVencChn[u32ChnID].stChnUserCfg.u32Height))
        {
            VENC_DRV_EflRequestIframe(g_stVencChn[u32ChnID].hVEncHandle);
            s32Ret = VENC_DRV_EflSetResolution(g_stVencChn[u32ChnID].hVEncHandle, pEncPara->stImage.u32Width, pEncPara->stImage.u32Height);

            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_VENC("VeduEfl_SetResolution err:%#x.\n", s32Ret);
                pEncPara->stStat.ErrCfgSkip++;

                /** release frame **/
                pEncPara->stStat.PutFrameNumTry++;
                (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

                if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
                {
                    pEncPara->stStat.VpssImgBufNum--;
                }

                pEncPara->stStat.PutFrameNumOK++;
                return HI_FAILURE;
            }
        }
        else
        {
            HI_ERR_VENC("ERR:Different resolution between the frame Info and the Encoder Cfg!Encode: %dX%d,FrameInfo: %dX%d\n",
                        g_stVencChn[u32ChnID].stChnUserCfg.u32Width, g_stVencChn[u32ChnID].stChnUserCfg.u32Height,
                        pEncPara->stImage.u32Width, pEncPara->stImage.u32Height);
            pEncPara->stStat.ErrCfgSkip++;

            /** release frame **/
            pEncPara->stStat.PutFrameNumTry++;
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

            if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
            {
                pEncPara->stStat.VpssImgBufNum--;
            }

            pEncPara->stStat.PutFrameNumOK++;
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 VENC_DRV_EflQueryChn_Y2(HI_U32 u32ChnID, VeduEfl_EncIn_S* pEncIn )    //for omxvenc with VPSS
{
    HI_HANDLE hHd;
    //HI_DRV_VIDEO_FRAME_S    stVpssFrame;
    //HI_DRV_VIDEO_PRIVATE_S* pstFramePriv;
    VeduEfl_EncPara_S* pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32ChnID].hVEncHandle;

    if (g_stVencChn[u32ChnID].bNeedVPSS)
    {
        hHd = g_stVencChn[u32ChnID].hPort[0];
    }
    else if (g_stVencChn[u32ChnID].bFrameBufMng)
    {
        hHd = g_stVencChn[u32ChnID].hUsrHandle;
    }
    else
    {
        hHd = pEncPara->stSrcInfo.handle;
    }


    pEncPara->stStat.GetFrameNumTry++;

    if (!pEncPara->stSrcInfo.pfGetImage)
    { return HI_FAILURE; }

    if (HI_SUCCESS != (pEncPara->stSrcInfo.pfGetImage)(hHd, &(pEncPara->stImage)))
    { return HI_FAILURE; }

#ifdef __VENC_DRV_DBG__

    if (GetImgFmVpss_ID < 100)
    {
        HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[GetImgFmVpss_ID][1]);
        GetImgFmVpss_ID++;
    }

#endif

    if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)
    {
        pEncPara->stStat.VpssImgBufNum++;
    }

    pEncPara->stStat.GetFrameNumOK++;

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) | (defined __VENC_98CV200_CONFIG__))            //for lowdelay 
    if (pEncPara->stImage.u32TunnelPhyAddr)      //待明确!!
    {
        pEncPara->LowDlyMod = HI_TRUE;
        pEncIn->TunlCellAddr = pEncPara->stImage.u32TunnelPhyAddr;
    }
    else
    {
        pEncPara->LowDlyMod = HI_FALSE;
        pEncIn->TunlCellAddr = 0;
    }
#endif

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->VoFrmRate;

    /* configured the resolving power dynamically */
    if (HI_SUCCESS != VENC_DRV_EflQueryChn_Y2_Resize(u32ChnID, pEncPara))
    {
        return HI_FAILURE;
    }
    
    /* skip - frame rate ctrl */
    if (HI_SUCCESS
        != VENC_DRV_EflRcFrmRateCtrl( g_stVencChn[u32ChnID].hVEncHandle, pEncPara->stStat.PutFrameNumOK/*pImagePriv->u32FrmCnt*/))
    {

        /** release frame **/
        pEncPara->stStat.PutFrameNumTry++;
        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum--;
        }

        pEncPara->stStat.PutFrameNumOK++;

        return HI_FAILURE;
    }

    pEncPara->YuvStoreType  = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 0);
    pEncPara->YuvSampleType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 1); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->PackageSel    = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 2); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->StoreFmt      = pEncPara->YuvStoreType;
    pEncPara->stStat.u32FrameType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 3);

    /* check the picture resolving power ,stride ,addr info first*/
    if ( HI_SUCCESS != VENC_DRV_EflCheckImgCfg(&pEncPara->stImage, pEncPara->YuvStoreType) )
    {
        HI_ERR_VENC("stImg cfg erro!!\n");
        pEncPara->stStat.ErrCfgSkip++;

        /** release frame **/
        pEncPara->stStat.PutFrameNumTry++;
        (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

        if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
        {
            pEncPara->stStat.VpssImgBufNum--;
        }

        pEncPara->stStat.PutFrameNumOK++;

        return HI_FAILURE;
    }

    pEncIn->BusViY = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    pEncIn->BusViC = pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;

    if (pEncPara->StoreFmt == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
        //pEncIn->BusViV    = pEncIn->BusViC + pEncPara->stImage_OMX.strideC * pEncPara->PicHeight / 2;
    }

    if ((VENC_STORE_SEMIPLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C * 2;
    }
    else if ((VENC_STORE_PACKAGE == pEncPara->YuvStoreType) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else if ((VENC_STORE_PLANNAR == pEncPara->YuvStoreType) && (VENC_YUV_420 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }

    pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stImage.u32Width , 16);
    pEncIn->RStrideC  = pEncIn->RStrideY;
    pEncIn->PTS0 = (HI_U32)(pEncPara->stImage.s64OmxPts & 0x00000000ffffffffLL);
    pEncIn->PTS1 = (HI_U32)((pEncPara->stImage.s64OmxPts & 0xffffffff00000000LL) >> 32);

    return HI_SUCCESS;
}

//HI_UNF_VIDEO_FRAME_INFO_S stPreImage;
//static HI_DRV_VIDEO_FRAME_S s_stVPSSImage;
static VeduEfl_EncPara_S* VENC_DRV_EflQueryChn_H264( VeduEfl_EncIn_S* pEncIn )
{
    HI_U32 u32StartQueryNo = 0;
    HI_S32 s32StartChnID   = 0;     /*this ID correspond to VeduChnCtx(class:VeduEfl_ChnCtx_S) */
    HI_U32 u32ChnID = 0;            /*this ID correspond to g_stVencChn(class:OPTM_VENC_CHN_S)*/
    VeduEfl_EncPara_S* pEncPara = HI_NULL;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* hHd_ret = HI_NULL;
    HI_U32 u32StreamThr = 0;

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);

    for (u32StartQueryNo = 0; u32StartQueryNo < MAX_VEDU_CHN; u32StartQueryNo++)
    {
        /*if (MAX_VEDU_CHN == u32StartQueryNo)
        {
            u32StartQueryNo = 0;
        }*/
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID, u32StartQueryNo);

        if ( INVAILD_CHN_FLAG == s32StartChnID )
        {
            continue;
        }

        D_VENC_GET_CHN(u32ChnID, VeduChnCtx[s32StartChnID].EncHandle);

        if (u32ChnID >= VENC_MAX_CHN_NUM)
        {
            VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
            return HI_NULL;
        }

        pEncPara = (VeduEfl_EncPara_S*) VeduChnCtx[s32StartChnID].EncHandle;

        if ( HI_INVALID_HANDLE == pEncPara->stSrcInfo.handle )
        {
            pEncPara->stSrcInfo.handle = g_stVencChn[u32ChnID].hUsrHandle;//VeduChnCtx[s32StartChnID].EncHandle;
        }

        /* this QueryChn just for H264*/
        if (HI_INVALID_HANDLE != g_stVencChn[u32ChnID].hJPGE)
        {
            continue;
        }

        /*check stream buffer first*/
        u32StreamThr = D_VENC_RC_MIN((pEncPara->StrmBufSize / 10), ((pEncPara->BitRate / pEncPara->VoFrmRate) * 2));

        if (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < u32StreamThr)
        {
            pEncPara->stStat.TooFewBufferWaitCnt++;
            continue;
        }

        if (g_stVencChn[u32ChnID].bOMXChn)
        {
            if (g_stVencChn[u32ChnID].bWithoutVPSS) /*without VPSS*/
            {
                if (HI_SUCCESS != VENC_DRV_EflQueryChn_Y1(u32ChnID, pEncIn))
                { continue; }
            }
            else
            {
                if (HI_SUCCESS != VENC_DRV_EflQueryChn_Y2(u32ChnID, pEncIn))
                { continue; }
            }
        }
        else
        {
            if (HI_SUCCESS != VENC_DRV_EflQueryChn_X(u32ChnID, pEncIn))
            { continue; }
        }

        VENC_DRV_DbgLowdelayEvent(u32ChnID, EVENT_VENC_FRM_IN, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);

        pEncPara->WaitingIsr = 1;
        break; /* find channel:s32StartChnID  to enc */
    }

    if (MAX_VEDU_CHN != u32StartQueryNo)
    {
        VENC_DRV_EflSortPriority_2(PriorityTab[1][u32StartQueryNo]);
    }

    if ((s32StartChnID >= 0) && (s32StartChnID < MAX_VEDU_CHN))
    {
        hHd_ret = VeduChnCtx[s32StartChnID].EncHandle;
    }

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        return HI_NULL;
    }
    else
    {
        if (g_stVencChn[u32ChnID].stProcWrite.bSaveYUVFileRun && pEncPara && (g_stVencChn[u32ChnID].enSrcModId < HI_ID_BUTT))
        {
            VENC_DRV_DbgWriteYUV(&pEncPara->stImage, g_stVencChn[u32ChnID].stProcWrite.YUVFileName);
        }

        return hHd_ret;
    }
}

#ifdef __VENC_SUPPORT_JPGE__
static VeduEfl_EncPara_S* VENC_DRV_EflQueryChn_JPEG( VeduEfl_EncIn_S* pEncIn )
{
    HI_U32 u32StartQueryNo = 0;
    HI_S32 s32StartChnID   = 0;     /*this ID correspond to VeduChnCtx(class:VeduEfl_ChnCtx_S) */
    HI_U32 u32ChnID = 0;            /*this ID correspond to g_stVencChn(class:OPTM_VENC_CHN_S)*/
    VeduEfl_EncPara_S* pEncPara = HI_NULL;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* hHd_ret = HI_NULL;
    //HI_S32 i;


    /* start from last query channel */
    /*if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        u32StartQueryNo = 0;
    }*/

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);

    for (u32StartQueryNo = 0; u32StartQueryNo < MAX_VEDU_CHN; u32StartQueryNo++)
    {
        /*if (MAX_VEDU_CHN == u32StartQueryNo)
        {
            u32StartQueryNo = 0;
        }*/
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID, u32StartQueryNo);

        if ( INVAILD_CHN_FLAG == s32StartChnID )
        {
            continue;
        }

        D_VENC_GET_CHN(u32ChnID, VeduChnCtx[s32StartChnID].EncHandle);

        if (u32ChnID >= VENC_MAX_CHN_NUM)
        {
            VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
            return HI_NULL;
        }

        pEncPara = (VeduEfl_EncPara_S*) VeduChnCtx[s32StartChnID].EncHandle;

        if ( HI_INVALID_HANDLE == pEncPara->stSrcInfo.handle )
        {
            pEncPara->stSrcInfo.handle = g_stVencChn[u32ChnID].hUsrHandle;//VeduChnCtx[s32StartChnID].EncHandle;
        }

        /* this QueryChn just for jpeg*/
        if (HI_INVALID_HANDLE == g_stVencChn[u32ChnID].hJPGE)
        {
            continue;
        }

        if (g_stVencChn[u32ChnID].bOMXChn)
        {
            if (g_stVencChn[u32ChnID].bWithoutVPSS) /*without VPSS*/
            {
                if (HI_SUCCESS != VENC_DRV_EflQueryChn_Y1(u32ChnID, pEncIn))
                { continue; }
            }
            else
            {
                if (HI_SUCCESS != VENC_DRV_EflQueryChn_Y2(u32ChnID, pEncIn))
                { continue; }
            }
        }
        else
        {
            if (HI_SUCCESS != VENC_DRV_EflQueryChn_X(u32ChnID, pEncIn))
            { continue; }
        }

        VENC_DRV_DbgLowdelayEvent(u32ChnID, EVENT_VENC_FRM_IN, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);

        pEncPara->WaitingIsr_jpge = 1;
        break; /* find channel:s32StartChnID  to enc */
    }

    if (MAX_VEDU_CHN != u32StartQueryNo)
    {
        VENC_DRV_EflSortPriority_2(PriorityTab[1][u32StartQueryNo]);
    }

    if ((s32StartChnID >= 0) && (s32StartChnID < MAX_VEDU_CHN))
    {
        hHd_ret = VeduChnCtx[s32StartChnID].EncHandle;
    }

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        return HI_NULL;
    }
    else
    {
        if (g_stVencChn[u32ChnID].stProcWrite.bSaveYUVFileRun && pEncPara && (g_stVencChn[u32ChnID].enSrcModId < HI_ID_BUTT))
        {
            VENC_DRV_DbgWriteYUV(&pEncPara->stImage, g_stVencChn[u32ChnID].stProcWrite.YUVFileName);
        }

        if (pEncIn->TunlCellAddr != 0)
        {
            HI_ERR_VENC("Warning!!!!! JPEG encoder not support lowdelay!!!The output maybe error!\n");
        }

        return hHd_ret;
    }
}
#endif

#ifdef __VENC_FREQ_DYNAMIC__
static HI_S32 VENC_DRV_EflCurrentCap(HI_S32* pClkSel)
{
    HI_U32 u32ChnID = 0;
    HI_U32 u32TotalMB = 0, u32MBNum = 0;
    HI_U32 WidthInMb = 0;
    HI_U32 HeightInMb = 0;

    for (u32ChnID = 0; u32ChnID < MAX_VEDU_CHN; u32ChnID++)
    {
        if (g_stVencChn[u32ChnID].bEnable)
        {
            WidthInMb  = (g_stVencChn[u32ChnID].stChnUserCfg.u32Width + 15) >> 4;
            HeightInMb = (g_stVencChn[u32ChnID].stChnUserCfg.u32Height + 15) >> 4;
            u32MBNum = WidthInMb * HeightInMb * g_stVencChn[u32ChnID].stChnUserCfg.u32TargetFrmRate;
            u32TotalMB += u32MBNum;
        }
    }

    if (u32TotalMB < 125000) /* 0.5s = 400*(MB)*(1/100MHZ)   MB=125000*/
    {
        *pClkSel = VENC_FREQUENCY_100M;
    }
    else if (u32TotalMB < 187500)
    {
        *pClkSel = VENC_FREQUENCY_150M;
    }
    else
    {
        *pClkSel = VENC_FREQUENCY_200M;
    }

    return HI_SUCCESS;
}
#endif

static HI_BOOL bFirstSlc = HI_TRUE;

#ifdef HI_SMMU_SUPPORT
static HI_VOID Venc_Smmu_ISR( VeduEfl_EncPara_S* pEncPara )
{
    VENC_HAL_ReadReg_Smmu(pEncPara);

    if (pEncPara->intns_tlbmiss_stat)
    {
        HI_WARN_VENC("%s,%d, SMMU Table miss error!\n", __func__, __LINE__);
        pEncPara->intns_tlbmiss_stat = 0;
    }

    if (pEncPara->intns_ptw_trans_stat)
    {
        HI_WARN_VENC("%s,%d, SMMU Table pre get error!\n", __func__, __LINE__);
        pEncPara->intns_ptw_trans_stat = 0;
    }

    if (pEncPara->intns_tlbinvalid_stat)
    {
        HI_WARN_VENC("%s,%d, SMMU Table invalid error!\n", __func__, __LINE__);
        pEncPara->intns_tlbinvalid_stat = 0;
    }

    return;

}
#endif

static HI_VOID Venc_ISR_TimeOut(VeduEfl_EncPara_S* pEncPara, S_VEDU_REGS_TYPE* pAllReg, HI_U32 u32VeChn)
{
    U_PERI_CRG35 unTmpValue;
    
    HI_ERR_VENC("VENC timeout interrupt!! lowdelay= %d\n", pEncPara->LowDlyMod);

    pAllReg->VEDU_INTCLR.u32 = 0x08000000;           //清中断

    /* reset VEDU*/
    unTmpValue.bits.venc_srst_req = 1;
    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

    /* open vedu clock */
#ifdef __VENC_98CV200_CONFIG__
    unTmpValue.bits.venc_core_cken = 1;
#else
    unTmpValue.bits.venc_cken = 1;
#endif

#ifndef __VENC_S5V200L_EXT_CONFIG__
    /* config vedu clock frequency: 200Mhz */
    unTmpValue.bits.venc_clk_sel  = 0;
#endif
    /* cancel reset */
    unTmpValue.bits.venc_srst_req = 0;
    g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

    pEncPara->WaitingIsr = 0;
    VeduIpCtx.IpFree = 1;
    /* release image encoded */
    {
        //HI_U32 regread;
        pEncPara->stStat.PutFrameNumTry++;

        if ((pEncPara->OMXChn) && (HI_FALSE == g_stVencChn[u32VeChn].bNeedVPSS))
        {
            VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_INPUT_DONE, HI_SUCCESS, &pEncPara->stImage_OMX);
            pEncPara->stStat.MsgQueueNum++;
        }
        else
        {
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

            if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
            {
                pEncPara->stStat.VpssImgBufNum--;
            }
        }

        pEncPara->stStat.PutFrameNumOK++;


        //S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;
        //regread = pAllReg->VEDU_TIMER;

        if ((!pEncPara->LowDlyMod) || (!pEncPara->SlcSplitEn))
        {
            VENC_DRV_DbgLowdelayEvent(u32VeChn, EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);
        }

        bFirstSlc = HI_TRUE;
    }
}

static HI_VOID Venc_ISR_ReleaseImage(VeduEfl_EncPara_S* pEncPara, HI_U32 u32VeChn)
{
    /* release image encoded */
    {
        //HI_U32 regread;
        pEncPara->stStat.PutFrameNumTry++;

        if ((pEncPara->OMXChn) && (HI_FALSE == g_stVencChn[u32VeChn].bNeedVPSS))
        {
            VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_INPUT_DONE, HI_SUCCESS, &pEncPara->stImage_OMX);
            pEncPara->stStat.MsgQueueNum++;
#ifdef __VENC_DRV_DBG__

            if (PutMsg_EBD_ID < 100)
            {
                HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[PutMsg_EBD_ID][3]);
                PutMsg_EBD_ID++;
            }

#endif
        }
        else
        {
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

            if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
            {
                pEncPara->stStat.VpssImgBufNum--;
            }
        }

        pEncPara->stStat.PutFrameNumOK++;


        pEncPara->WaitingIsr = 0;

        //S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->pRegBase;
        //regread = pAllReg->VEDU_TIMER;

        if ((!pEncPara->LowDlyMod) || (!pEncPara->SlcSplitEn))
        {
            VENC_DRV_DbgLowdelayEvent(u32VeChn, EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);
        }

        bFirstSlc = HI_TRUE;
    }
}

static HI_VOID Venc_ISR( HI_VOID )
{
    VeduEfl_EncPara_S* EncHandle;
    VeduEfl_EncIn_S EncIn;
    //VEDU_LOCK_FLAG  flag;
    HI_U32 u32VeChn;
    VeduEfl_EncPara_S* pEncPara;
    S_VEDU_REGS_TYPE* pAllReg;
    HI_U32 ImbRatio = 0;
    HI_U32 ImbRatioAvg = 0;
    HI_U32 Mbtotal = 0;

    D_VENC_GET_CHN(u32VeChn, VeduIpCtx.CurrHandle);

    if (u32VeChn >= MAX_VEDU_CHN)
    {
        return;
    }

    pEncPara = (VeduEfl_EncPara_S*) VeduIpCtx.CurrHandle;
    pAllReg  = (S_VEDU_REGS_TYPE*)pEncPara->pRegBase;

#if (defined HI_SMMU_SUPPORT) && (defined __VENC_98CV200_CONFIG__)
    Venc_Smmu_ISR(pEncPara);
#endif

    if (pAllReg->VEDU_RAWINT.bits.VeduTimeout)			   //超时软复位
    {
        Venc_ISR_TimeOut(pEncPara, pAllReg, u32VeChn);
    }

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)| (defined __VENC_98CV200_CONFIG__))

    if (pAllReg->VEDU_RAWINT.bits.VeduSliceInt)			   //低延时模式下slice级中断处理
    {
        if (bFirstSlc)
        {
            VENC_DRV_DbgLowdelayEvent(u32VeChn, EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);
            bFirstSlc = HI_FALSE;
        }

        VENC_DRV_EflEndOneSliceVenc( VeduIpCtx.CurrHandle );
        pAllReg->VEDU_INTCLR.u32 = 0x400;           //清中断
    }

#endif


    if ( pAllReg->VEDU_RAWINT.bits.VencEndOfPic )        //帧级中断
    {
#ifdef __VENC_DRV_DBG__

        if (Isr_ID < 100)
        {
            HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[Isr_ID][2]);
            Isr_ID++;
        }

#endif
        /* release image encoded */
        Venc_ISR_ReleaseImage(pEncPara, u32VeChn);

        gencodeframe = 1;
        wake_up(&gEncodeFrame);

        /* postprocess frame encoded */
        VENC_DRV_EflReadRegVenc    ( VeduIpCtx.CurrHandle );
        pAllReg->VEDU_INTCLR.u32 = 0xFFFFFFFF;//0xFFFFFBFF;
        VENC_DRV_EflEndOneFrameVenc( VeduIpCtx.CurrHandle );
        //////////////////////// add by w00283384
        Mbtotal =  pEncPara->PicWidth * pEncPara->PicHeight / (16 * 16);

        if (Mbtotal != 0)
        {
            ImbRatio = pEncPara->stRc.NumIMBCurFrm * 1000 / Mbtotal;
        }

        if (pEncPara->ICatchEnable)
        {
            pEncPara->FrmCnt++;
#if 1

            if (pEncPara->FrmCnt > 30)
            {
                HI_U32 diff = 0, i = 0;
                ImbRatioAvg =  VENC_DRV_EflRcAverage(pEncPara->stRc.IMBRatioCurScen, 6);
                ImbRatioAvg = D_VENC_RC_MAX(ImbRatioAvg, 1);

                diff = D_VENC_RC_ABS(ImbRatioAvg, ImbRatio);

                if ((ImbRatio > ImbRatioAvg) && ((diff * 100 / ImbRatioAvg) > 200) && (ImbRatio > 500) )
                {
                    pEncPara->InterFrmCnt = pEncPara->Gop - 1;
                    pEncPara->ICatchOn = 1;
                    g_flag_I_insert = 1;
                    pEncPara->RcStart = 1;

                    for (i = 0; i < 6; i++)
                    {
                        pEncPara->stRc.IMBRatioCurScen[i] = 0;
                    }
                }
                else
                {
                    D_VENC_RC_UPDATA(pEncPara->stRc.IMBRatioCurScen, ImbRatio);
                }
            }
            else
            {
                if (pEncPara->FrmCnt != 1)   //if it is I frame not update IMBRatioCurScen.
                {
                    D_VENC_RC_UPDATA(pEncPara->stRc.IMBRatioCurScen, ImbRatio);
                }
            }
#endif
        }

#ifdef __VENC_FREQ_DYNAMIC__
        /* change the frequency activity */
        {
            HI_S32 s32ClkSel = 0;
            VENC_DRV_EflCurrentCap(&s32ClkSel);

            if (VeduIpCtx.CurrFreq != s32ClkSel)
            {
                U_PERI_CRG35 unTmpValue;

                /* reset */
                //unTmpValue.bits.venc_srst_req = 1;
                //g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

                //msleep(1);

                /* close vedu clock */
                unTmpValue.bits.venc_cken     = 0;
                g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

#ifndef __VENC_S5V200L_EXT_CONFIG__
                /* config vedu clock frequency*/
                unTmpValue.bits.venc_clk_sel  = s32ClkSel;
#endif
                unTmpValue.bits.venc_cken     = 1;
                g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

                /* cancel reset */
                //unTmpValue.bits.venc_srst_req = 0;
                //g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;

                VeduIpCtx.CurrFreq = s32ClkSel;
            }
        }
#endif

        if ((!g_stVencChn[u32VeChn].stProcWrite.bSaveYUVFileRun) || (g_stVencChn[u32VeChn].enSrcModId == HI_ID_BUTT))
        {
            /* next frame to encode */
            //VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
            EncHandle = VENC_DRV_EflQueryChn_H264( &EncIn );

            //VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
            if ( EncHandle != NULL )
            {

                VeduIpCtx.IpFree = 0;
                VeduIpCtx.CurrHandle = EncHandle;

                VENC_DRV_EflStartOneFrameVenc( EncHandle, &EncIn );
                VENC_DRV_EflCfgRegVenc       ( EncHandle );
                //g_start = OSAL_GetTimeInMs();

            }
            else
            {
                VeduIpCtx.IpFree = 1;
                VENC_DRV_BoardDeinit();
            }
        }
        else
        {
            VeduIpCtx.IpFree = 1;
            VENC_DRV_BoardDeinit();
        }

        VENC_DRV_OsalGiveEvent(&g_VencWait_Stream[u32VeChn]);

        if (g_stVencChn[u32VeChn].bOMXChn)
        {
            VENC_DRV_OsalGiveEvent(&g_VENC_Event_stream);
        }
    }
}

HI_S32 VENC_DRV_EflEncodeFrame(HI_VOID)
{
    VeduEfl_EncPara_S* EncHandle;
    VeduEfl_EncIn_S EncIn;
    //VEDU_LOCK_FLAG flag;

    if (HI_NULL == VeduChnCtx[0].EncHandle)
    {
        return 0;
    }

    if (!VeduIpCtx.StopTask)
    {
        if (VeduIpCtx.IpFree)
        {
            /* if ipfree, don't irqlock */
            //VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
            EncHandle = VENC_DRV_EflQueryChn_H264( &EncIn );
            //VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

            if (EncHandle != NULL)
            {
                VeduIpCtx.IpFree = 0;
                VeduIpCtx.CurrHandle = EncHandle;

                VENC_DRV_EflStartOneFrameVenc( EncHandle, &EncIn );
                VENC_DRV_EflCfgRegVenc       ( EncHandle );
            }
        }
    }

    return 0;
}

static HI_S32 VENC_DRV_EflTask_JPEG(HI_U32 u32ChnID, VeduEfl_EncPara_S* pEncPara)
{
#ifdef __VENC_SUPPORT_JPGE__
    HI_S32 s32Ret = 0;
    VeduEfl_EncIn_S EncIn;
    Venc2Jpge_EncIn_S JpgeEncIn;
    HI_BOOL  bJpgeBufFull = HI_FALSE;

    {
        memset(&EncIn, 0, sizeof(VeduEfl_EncIn_S));
        memset(&JpgeEncIn, 0, sizeof(Venc2Jpge_EncIn_S));

        pEncPara = VENC_DRV_EflQueryChn_JPEG( &EncIn );

        if (pEncPara != NULL)
        {
            D_VENC_GET_CHN(u32ChnID, pEncPara);

            if (u32ChnID >= VENC_MAX_CHN_NUM)
            {
                return HI_FAILURE;
            }

            if ((HI_INVALID_HANDLE != g_stVencChn[u32ChnID].hJPGE) && (pJpgeFunc != HI_NULL))  /*JPGE CHANNEL*/
            {

                pEncPara->PTS0 = EncIn.PTS0;
                pEncPara->PTS1 = EncIn.PTS1;

                JPGE_MakeJFIF(pEncPara);

                JpgeEncIn.BusViY = EncIn.BusViY;
                JpgeEncIn.BusViC = EncIn.BusViC;
                JpgeEncIn.BusViV = EncIn.BusViV;
                JpgeEncIn.ViYStride = EncIn.ViYStride;
                JpgeEncIn.ViCStride = EncIn.ViCStride;

                JpgeEncIn.StrmBufAddr   = pEncPara->StrmBufAddr;
                JpgeEncIn.StrmBufRpAddr = pEncPara->StrmBufRpAddr;
                JpgeEncIn.StrmBufWpAddr = pEncPara->StrmBufWpAddr;
                JpgeEncIn.StrmBufSize = pEncPara->StrmBufSize;
                JpgeEncIn.Vir2BusOffset = pEncPara->Vir2BusOffset;
                JpgeEncIn.pJpgeYqt      = pEncPara->pJpgeYqt;
                JpgeEncIn.pJpgeCqt      = pEncPara->pJpgeCqt;
                JpgeEncIn.FrameHeight   = pEncPara->PicHeight;
                JpgeEncIn.FrameWidth    = pEncPara->PicWidth;
                JpgeEncIn.YuvSampleType = pEncPara->YuvSampleType;
                JpgeEncIn.PTS0          = EncIn.PTS0;

                switch (pEncPara->YuvStoreType)
                {
                    case VENC_STORE_SEMIPLANNAR:
                        JpgeEncIn.YuvStoreType = JPGE_STORE_SEMIPLANNAR;
                        break;

                    case VENC_STORE_PACKAGE:
                        JpgeEncIn.YuvStoreType = JPGE_STORE_PACKAGE;
                        break;

                    case VENC_STORE_PLANNAR:
                        JpgeEncIn.YuvStoreType = JPGE_STORE_PLANNAR;
                        break;

                    default:
                        JpgeEncIn.YuvStoreType = JPGE_STORE_SEMIPLANNAR;
                        break;
                }

                JpgeEncIn.PackageSel = pEncPara->PackageSel;
                JpgeEncIn.RotationAngle = pEncPara->RotationAngle;

                s32Ret = pJpgeFunc->pfnJpgeEncodeFrame(g_stVencChn[u32ChnID].hJPGE, &JpgeEncIn, &bJpgeBufFull);

                if (s32Ret != HI_SUCCESS)
                {
                    JPGE_RemoveJFIF(pEncPara);
                    pEncPara->stStat.ErrCfgSkip++;
                }
                else if (bJpgeBufFull)
                {
                    /*remove the heard of JFIF before*/
                    JPGE_RemoveJFIF(pEncPara);
                    pEncPara->stStat.TooFewBufferSkip++;
                }
                else
                {
                    VENC_DRV_EflEndOneFrameJpge(pEncPara);
                }

                pEncPara->WaitingIsr_jpge = 0;

                /*release the Img*/
                pEncPara->stStat.PutFrameNumTry++;
                (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

                if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)
                {
                    pEncPara->stStat.VpssImgBufNum--;

                }

                pEncPara->RcStart  = 0;
                pEncPara->TrCount -= pEncPara->ViFrmRate;
                pEncPara->stStat.PutFrameNumOK++;
            }
        }
    }
#endif

    return HI_SUCCESS;
}

static HI_VOID VENC_DRV_EflTask( HI_VOID )
{
    HI_S32 s32Ret = 0;
    VeduEfl_EncIn_S EncIn;

    VeduEfl_EncPara_S* pEncPara = HI_NULL;
    HI_U32 i = 0;
    HI_U32 u32ChnID = 0;
    HI_BOOL bTmpValue = HI_FALSE;
    //HI_BOOL bVoAttach = HI_FALSE;
    //HI_BOOL bQueueMode = HI_TRUE;
    VeduIpCtx.TaskRunning = 1;

    /* 初始化等待队列头*/
    VENC_DRV_OsalInitEvent(&g_VENC_Event, 0);

    /* wait for venc start */
    while (!VeduIpCtx.StopTask)
    {
        for (i = 0; i < MAX_VEDU_CHN; i++)
        {
            bTmpValue |= g_stVencChn[i].bEnable;
        }

        if (HI_FALSE == bTmpValue)
        {
            msleep(10);
        }
        else
        {
            break;
        }
    }

    /* find valid venc handle */
    for (i = 0; i < VENC_MAX_CHN_NUM; i++)
    {
        pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[i].hVEncHandle;

        if ((HI_NULL == pEncPara))
        {
            continue;
        }
    }

    while (!VeduIpCtx.StopTask)
    {
        /* vedu_ip for h264*/
        if (VeduIpCtx.IpFree)
        {
            /* if ipfree, don't irqlock */
            // VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
            memset(&EncIn, 0, sizeof(VeduEfl_EncIn_S));
            pEncPara = VENC_DRV_EflQueryChn_H264( &EncIn );
            //VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

            if (pEncPara != NULL)
            {
                D_VENC_GET_CHN(u32ChnID, pEncPara);

                if (u32ChnID >= VENC_MAX_CHN_NUM)
                {
                    continue;
                }

                if (HI_INVALID_HANDLE == g_stVencChn[u32ChnID].hJPGE)  /*JPGE CHANNEL*/
                {
                    VeduIpCtx.IpFree = 0;
                    VeduIpCtx.CurrHandle = pEncPara;

                    VENC_DRV_EflStartOneFrameVenc( pEncPara, &EncIn );
                    VENC_DRV_EflCfgRegVenc( pEncPara );
                }

            }
        }

        if (HI_SUCCESS != VENC_DRV_EflTask_JPEG(u32ChnID, pEncPara))
        {
            continue;
        }
            
        /* just for omx_venc channel which with vpss */

        for (i = 0; i < VENC_MAX_CHN_NUM; i++)
        {
            if (!g_stVencChn[i].bWithoutVPSS)
            {
                if ((g_stVencChn[i].bOMXChn) && g_stVencChn[i].bEnable && (g_stVencChn[i].hVEncHandle != NULL))
                {

                    HI_DRV_VIDEO_PRIVATE_S* pstFramePriv = HI_NULL;
                    pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[i].hVEncHandle;

                    do
                    {
                        s32Ret = (pVpssFunc->pfnVpssGetImage)(g_stVencChn[i].hVPSS, &pEncPara->stVpssFrame);

                        if (s32Ret == HI_SUCCESS)
                        {
                            pEncPara->stStat.VpssQueueBufNum--;
                            pstFramePriv = (HI_DRV_VIDEO_PRIVATE_S*)pEncPara->stVpssFrame.u32Priv;
                            memcpy(&pEncPara->stImage_OMX, &pstFramePriv->u32Reserve, sizeof(venc_user_buf));
                            VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_INPUT_DONE, HI_SUCCESS , &(pEncPara->stImage_OMX));
                            pEncPara->stStat.MsgQueueNum++;

#ifdef __VENC_DRV_DBG__

                            if (PutMsg_EBD_ID < 100)
                            {
                                HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[PutMsg_EBD_ID][3]);
                                PutMsg_EBD_ID++;
                            }

#endif
                        }
                    }
                    while ((!VeduIpCtx.StopTask) && (!s32Ret));
                }

                s32Ret = HI_SUCCESS;
            }
        }

        /* if ipfree, sleep */
        s32Ret = VENC_DRV_OsalWaitEvent(&g_VENC_Event, msecs_to_jiffies(30));

        if (0 != s32Ret)
        {
            HI_INFO_VENC("wait time out!\n");
        }
    }

    VeduIpCtx.TaskRunning = 0;
    return;
}

static VeduEfl_EncPara_S* VENC_DRV_EflQueryChn_Stream(venc_msginfo* pMsgInfo, HI_U32* pu32ChnID)
{
    HI_U32 u32StartQueryNo = 0;
    HI_S32 s32StartChnID   = 0;     /*this ID correspond to VeduChnCtx(class:VeduEfl_ChnCtx_S) */
    HI_U32 u32ChnID = 0;            /*this ID correspond to g_stVencChn(class:OPTM_VENC_CHN_S)*/
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* pEncPara = NULL;
    VeduEfl_EncPara_S* hVencHandle = HI_NULL;
    queue_data_s Queue_Data;

    VENC_DRV_OsalLock(VeduIpCtx.pChnLock, &flag);

    for (u32StartQueryNo = 0; u32StartQueryNo < MAX_VEDU_CHN; u32StartQueryNo++)
    {
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID, u32StartQueryNo);

        if ( INVAILD_CHN_FLAG == s32StartChnID )
        { continue; }

        D_VENC_GET_CHN(u32ChnID, VeduChnCtx[s32StartChnID].EncHandle);

        if (u32ChnID >= VENC_MAX_CHN_NUM)
        {
            VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
            return HI_NULL;
        }

        if (!g_stVencChn[u32ChnID].bOMXChn)
        { continue; }

        hVencHandle = VeduChnCtx[s32StartChnID].EncHandle;
        pEncPara    = hVencHandle;
        *pu32ChnID  =  u32ChnID;

        pEncPara->stStat.GetStreamNumTry++;

        if ( VENC_DRV_EflGetStreamLen(hVencHandle) > 0 )   /*fine one channel have stream*/
        {
            if ( VENC_DRV_MngQueueEmpty(pEncPara->StreamQueue_OMX))
            { continue; }

            if ( VENC_DRV_MngDequeue(pEncPara->StreamQueue_OMX, &Queue_Data))
            { continue; }

            pEncPara->StreamQueue_OMX->bStrmLock = 1;   //mutex with the stop processing add by l00228308
            pEncPara->stStat.StreamQueueNum--;

            if (Queue_Data.bToOMX)
            {
                memcpy(pMsgInfo, &(Queue_Data.unFrmInfo.msg_info_omx), sizeof(venc_msginfo));
            }
            else
            {
                HI_ERR_VENC("Err:output stream queue not match with Omx Component!\n");
                continue;
            }

            pEncPara->stStat.GetStreamNumOK++;
            break; /* find channel:s32StartChnID  have buffer to fill */
        }
    }

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);

    if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        return HI_NULL;
    }
    else
    {
        return hVencHandle;
    }
}

static HI_VOID VENC_DRV_EflTask_Stream_Wait( HI_VOID )
{
    HI_U32 i = 0;
    HI_BOOL bTmpValue = HI_FALSE;

    /* wait for venc start */
    while (!VeduIpCtx.StopTask)
    {
        for (i = 0; i < MAX_VEDU_CHN; i++)
        {
            bTmpValue |= (g_stVencChn[i].bEnable && g_stVencChn[i].bOMXChn);
        }

        if (HI_FALSE == bTmpValue)
        {
            msleep(10);
        }
        else
        {
            break;
        }
    }
}

static HI_VOID VENC_DRV_EflTask_Stream( HI_VOID )
{
    VeduEfl_EncPara_S* EncHandle;
    VeduEfl_NALU_S Nalu = {{NULL, NULL}};
    VeduEfl_EncPara_S* pEncPara = HI_NULL;
    HI_S32 s32Ret = 0, s32Ret2 = 0;
    venc_user_buf StreamBuf;
    venc_msginfo msg_info;
    HI_U32 ChanID = 0;
    HI_VOID* venc_stream_addr = NULL;

    /* wait for venc start */
    VENC_DRV_EflTask_Stream_Wait();
    
    //VENC_DRV_OsalInitEvent(&g_VENC_Event_stream, 0);

    while (!VeduIpCtx.StopTask)
    {
        /* if ipfree, don't irqlock */
        EncHandle = VENC_DRV_EflQueryChn_Stream(&msg_info, &ChanID);

        if (EncHandle != NULL)
        {
            pEncPara = EncHandle;

            memcpy(&StreamBuf, &(msg_info.buf), sizeof(venc_user_buf));
            venc_stream_addr = (HI_VOID*)(StreamBuf.bufferaddr_Phy + StreamBuf.vir2phy_offset);

            do
            {
                if ( VENC_DRV_EflGetStreamLen(EncHandle) > 0)
                {
                    s32Ret = VENC_DRV_EflGetBitStream( EncHandle, &Nalu );

                    if (Nalu.SlcLen[0] > 0)
                    {
                        memcpy(venc_stream_addr, Nalu.pVirtAddr[0], Nalu.SlcLen[0]);
                        StreamBuf.data_len += Nalu.SlcLen[0];
                        venc_stream_addr += Nalu.SlcLen[0];
                    }

                    if (Nalu.SlcLen[1] > 0)
                    {
                        memcpy(venc_stream_addr, Nalu.pVirtAddr[1], Nalu.SlcLen[1]);
                        StreamBuf.data_len += Nalu.SlcLen[1];
                        venc_stream_addr += Nalu.SlcLen[1];
                    }

                    StreamBuf.timestamp0 =  Nalu.PTS0;
                    StreamBuf.timestamp1 =  Nalu.PTS1;
                    s32Ret |= VENC_DRV_EflSkpBitStream(EncHandle, &Nalu);
                }
                else
                {
                    //msleep(5);
                    s32Ret2 = VENC_DRV_OsalWaitEvent(&g_VENC_Event_stream, msecs_to_jiffies(10));

                    if (0 != s32Ret2)
                    {
                        HI_INFO_VENC("wait stream time out! %d\n", __LINE__);
                    }
                }

#ifdef __OMXVENC_ONE_IN_ONE_OUT__
            }
            while ((s32Ret) || (Nalu.NaluType == 7) ||  (Nalu.NaluType == 8));

#else
            }

            while ((!s32Ret) && (Nalu.NaluType == 7));   //((s32Ret) || (Nalu.NaluType == 7) ||  (Nalu.NaluType == 8)); //   //not support split h264

#endif

            StreamBuf.flags = 0;

            switch ( Nalu.NaluType )
            {
                case 5:  /*IDR*/
#ifdef __OMXVENC_ONE_IN_ONE_OUT__
                    if (pEncPara->bFirstNal2Send)
                    {
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
                        pEncPara->bFirstNal2Send = HI_FALSE;
                    }
                    else
#endif
                    {
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
                    }

                    break;

                case 1:  /*P*/
                    StreamBuf.flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
                    break;
#if 1

                case 8:  /* PPS */
                    if (pEncPara->bFirstNal2Send)
                    {
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                        pEncPara->bFirstNal2Send = HI_FALSE;
                    }
                    else
                    {
                        StreamBuf.flags |=  OMXVENC_BUFFERFLAG_EXTRADATA;
                    }

                    break;
#endif

                default:
                    HI_ERR_VENC("Nalu.NaluType(%d) is invalid\n");
                    break;
            }

            VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_OUTPUT_DONE, s32Ret , &StreamBuf);
            pEncPara->StreamQueue_OMX->bStrmLock = 0;
            pEncPara->stStat.MsgQueueNum++;

#ifdef __VENC_DRV_DBG__

            if (PutMsg_FBD_ID < 100)
            {
                HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[PutMsg_FBD_ID][4]);
                PutMsg_FBD_ID++;
            }

#endif
        }
        else
        {
            /* if channel not ready, sleep */
            //msleep(10);
            s32Ret = VENC_DRV_OsalWaitEvent(&g_VENC_Event_stream, msecs_to_jiffies(30));

            if (0 != s32Ret)
            {
                HI_INFO_VENC("wait stream time out! %d\n", __LINE__);
            }
        }
    }
    return;
}

/******************************************************************************
Function   :
Description: IP-VEDU & IP-JPGE Open & Close
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflOpenVedu( HI_VOID )
{
    HI_U32 i;

    /* creat channel control mutex */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &VeduIpCtx.pChnLock ))
    {
        return HI_FAILURE;
    }

    /* map reg_base_addr to virtual address */

    VeduIpCtx.pRegBase = (HI_U32*)ioremap( VEDU_REG_BASE_ADDR, D_VENC_ALIGN_UP(sizeof(S_VEDU_REGS_TYPE), 256) );

    if (HI_NULL == VeduIpCtx.pRegBase)
    {
        VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );
        return HI_FAILURE;
    }

    /* set ip free status */
    VeduIpCtx.IpFree = 1;

    /* set ip free status */
    VeduIpCtx.CurrFreq = VENC_FREQUENCY_200M;

    /* clear channel status */
    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        VeduChnCtx[i].EncHandle = NULL;
    }

    /* init IRQ */
    VENC_HAL_DisableAllInt((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_HAL_ClrAllInt    ((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));

    if (HI_FAILURE == VENC_DRV_OsalIrqInit(VEDU_IRQ_ID, Venc_ISR))
    {
        return HI_FAILURE;
    }

    /* creat thread to manage channel */
    VeduIpCtx.StopTask = 0;
    VeduIpCtx.TaskRunning = 0;

    /* for test compare*/
    g_idr_pic_id = 0;

    init_waitqueue_head(&gqueue);
    gwait = 0;

    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask_Frame, "HI_VENC_FrameTask", VENC_DRV_EflTask );
    msleep(1);
    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask_Stream, "HI_VENC_StreamTask", VENC_DRV_EflTask_Stream);
    return HI_SUCCESS;
}

HI_VOID VENC_DRV_EflCloseVedu( HI_VOID )
{
    HI_U32 RecycleCnt = 0;

    VeduIpCtx.StopTask = 1;

    while ((VeduIpCtx.TaskRunning) && (RecycleCnt < 50))
    {
        RecycleCnt++;
        
        msleep(1);
    }

    if (RecycleCnt == 50)
    {
        HI_INFO_VENC("Vedu close timeout! force to close Vedu\n");
    }

    VENC_DRV_OsalDeleteTask(VeduIpCtx.pTask_Frame);
    VENC_DRV_OsalDeleteTask(VeduIpCtx.pTask_Stream);
    VENC_HAL_DisableAllInt((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_HAL_ClrAllInt    ((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_DRV_OsalIrqFree( VEDU_IRQ_ID );

    //    VENC_DRV_OsalUnmapRegisterAddr( VeduIpCtx.pRegBase );
    iounmap(VeduIpCtx.pRegBase);

    /* for test compare*/
    g_idr_pic_id = 0;

    VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );

    return;
}

HI_VOID VENC_DRV_EflSuspendVedu( HI_VOID )
{
    VeduIpCtx.StopTask = 1;

    while (VeduIpCtx.TaskRunning)
    {
        msleep(1);
    }

    VENC_DRV_OsalDeleteTask(VeduIpCtx.pTask_Frame);
    VENC_DRV_OsalDeleteTask(VeduIpCtx.pTask_Stream);
    VENC_HAL_DisableAllInt((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_HAL_ClrAllInt    ((S_VEDU_REGS_TYPE*)(VeduIpCtx.pRegBase));
    VENC_DRV_OsalIrqFree( VEDU_IRQ_ID );

    //    VENC_DRV_OsalUnmapRegisterAddr( VeduIpCtx.pRegBase );
    iounmap(VeduIpCtx.pRegBase);

    //VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );

    return;
}

/*****************************************************************************
 Prototype    : VENC_DRV_Resume
 Description  : VENC resume function
 Input        : None
 Output       : None
 Return Value : None
 Others       : Delete initialization of global value compared with VeduEfl_OpenVedu
*****************************************************************************/
HI_S32 VENC_DRV_EflResumeVedu(HI_VOID)
{
    /* init IRQ */
    if (HI_FAILURE == VENC_DRV_OsalIrqInit(VEDU_IRQ_ID, Venc_ISR))
    {
        return HI_FAILURE;
    }

    /* creat channel control mutex */
    /*if (HI_FAILURE == VENC_DRV_OsalLockCreate( &VeduIpCtx.pChnLock ))
    {
        return HI_FAILURE;
    }*/

    /* map reg_base_addr to virtual address */

    //    VeduIpCtx.pRegBase = (HI_U32 *)VENC_DRV_OsalMapRegisterAddr(VEDU_REG_BASE_ADDR, 0x8000);
    VeduIpCtx.pRegBase = ioremap( VEDU_REG_BASE_ADDR, D_VENC_ALIGN_UP(sizeof(S_VEDU_REGS_TYPE), 256) );
    /* creat thread to manage channel */
    VeduIpCtx.StopTask = 0;
    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask_Frame, "HI_VENC_FrameTask", VENC_DRV_EflTask );
    msleep(1);
    VENC_DRV_OsalCreateTask( &VeduIpCtx.pTask_Stream, "HI_VENC_StreamTask", VENC_DRV_EflTask_Stream);

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description: check if bs len > 0, before call VeduEfl_GetBitStream
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_U32 VENC_DRV_EflGetStreamLen( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32 u32BsLen;

    u32BsLen = VENC_DRV_BufGetDataLen( &(pEncPara->stCycBuf));

    return u32BsLen;
}

/******************************************************************************
Function   :
Description: Get One Nalu address & length(include 64-byte packet header)
Calls      :
Input      :
Output     :
Return     :
Others     : Read Head pointer be changed.
******************************************************************************/
HI_S32 VENC_DRV_EflGetBitStream_X( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu )
{
    VeduEfl_NaluHdr_S*  pNaluHdr;
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;
    VALG_CB_RDINFO_S stRdInfo;
    HI_U32 bit_offset = 0;
    HI_U32 bit_offset1 = 0;
    //pEncPara->stStat.GetStreamNumTry++;    //change by l00228308

    if (VENC_DRV_BufIsVaild(pstStrBuf) != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    if (0 == VENC_DRV_BufGetDataLen(pstStrBuf))    /* no bs rdy   */
    {
        return HI_FAILURE;
    }

    pNaluHdr = (VeduEfl_NaluHdr_S*) VENC_DRV_BufGetRdHead( pstStrBuf );  /* parse the head   */

    /******* get addr and len, updata readhead *******/
    if (HI_SUCCESS != VENC_DRV_BufRead( pstStrBuf, pNaluHdr->PacketLen, &stRdInfo ))
    {
        return HI_FAILURE;
    }

    pNalu->pVirtAddr[0] = stRdInfo.pSrc  [0];
    pNalu->pVirtAddr[1] = stRdInfo.pSrc  [1];
    pNalu->SlcLen   [0] = stRdInfo.u32Len[0];
    pNalu->SlcLen   [1] = stRdInfo.u32Len[1];
    pNalu->InvldByte    = pNaluHdr->InvldByte;

    if (pNalu->SlcLen[1] > 0)
    {
        pNalu->SlcLen[1] -= pNaluHdr->InvldByte;
    }
    else
    {
        pNalu->SlcLen[0] -= pNaluHdr->InvldByte;
    }

    pNalu->PTS0 = pNaluHdr->PTS0;
    pNalu->PTS1 = pNaluHdr->PTS1;
    pNalu->bFrameEnd = pNaluHdr->bLastSlice;
    pNalu->NaluType = pNaluHdr->Type;

    /* add by j35383, discard nal header of 64 byte */
    pNalu->SlcLen   [0] -= 64;
    pNalu->pVirtAddr[0] = (HI_VOID*)(pNalu->pVirtAddr[0] + 64);
    bit_offset = (HI_U32)((HI_U8*)pNalu->pVirtAddr[0] - (HI_U8*)pEncPara->StreamMMZBuf.pu8StartVirAddr);
    bit_offset1 = (HI_U32)((HI_U8*)pNalu->pVirtAddr[1] - (HI_U8*)pEncPara->StreamMMZBuf.pu8StartVirAddr);
    //pNalu->PhyAddr[0] = (HI_U32)pNalu->pVirtAddr[0] - pEncPara->Vir2BusOffset;
    //pNalu->PhyAddr[1] = (HI_U32)pNalu->pVirtAddr[1] - pEncPara->Vir2BusOffset;

    pNalu->PhyAddr[0] = pEncPara->StreamMMZBuf.u32StartPhyAddr + bit_offset;
    pNalu->PhyAddr[1] = pEncPara->StreamMMZBuf.u32StartPhyAddr + bit_offset1;

    //pEncPara->stStat.GetStreamNumOK++;
    pEncPara->stStat.StreamTotalByte += pNalu->SlcLen   [0];
    pEncPara->stStat.StreamTotalByte += pNalu->SlcLen   [1];

    if (pNalu->SlcLen[1] > 0)
    {
        memcpy((pNalu->pVirtAddr[0] + pNalu->SlcLen[0]), pNalu->pVirtAddr[1], pNalu->SlcLen[1]);
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflGetBitStream( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu )
{
    VEDU_LOCK_FLAG flag;
    HI_S32 ret;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflGetBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    return ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     : Read Tail pointer be changed
******************************************************************************/
HI_S32 VENC_DRV_EflSkpBitStream_X( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu )
{
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S*    pstStrBuf = &pEncPara->stCycBuf;

    HI_U32 u32InputLen;


    if (VENC_DRV_BufIsVaild(pstStrBuf) != HI_SUCCESS)
    {
        return HI_FAILURE;
    }

    u32InputLen = pNalu->SlcLen[0] + pNalu->SlcLen[1] + 64; /* add by j35383 (+64) */

    /*if (pEncPara->SlcSplitEn)
    {
        u32InputLen = u32InputLen & 63 ? (u32InputLen | 63) + 1 : u32InputLen;
    }
    else
    {
        u32InputLen = u32InputLen & 63 ? (u32InputLen | 63) + 1 : u32InputLen + 64;
    }*/


    //u32InputLen += pNalu->InvldByte;
    u32InputLen = u32InputLen & 63 ? (u32InputLen | 63) + 1 : u32InputLen;

    /******* check start addr *******/  /* add by j35383 (-64) */
    if ((HI_VOID*)((HI_U8*)pNalu->pVirtAddr[0] - 64) != VENC_DRV_BufGetRdTail(pstStrBuf))
    {
        return HI_FAILURE;
    }

    /******* update Read index *******/
    if (HI_FAILURE == VENC_DRV_BufUpdateRp(pstStrBuf, u32InputLen))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflSkpBitStream( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu )
{
    VEDU_LOCK_FLAG flag;
    HI_S32 ret;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflSkpBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    return ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflQueryStatInfo( VeduEfl_EncPara_S* EncHandle, VeduEfl_StatInfo_S* pStatInfo )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    if ((pEncPara == NULL) || (pStatInfo == NULL))
    {
        return HI_FAILURE;
    }

    pEncPara->stStat.UsedStreamBuf = VENC_DRV_EflGetStreamLen(EncHandle);
    *pStatInfo = pEncPara->stStat;

    return HI_SUCCESS;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflQueueFrame( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame)
{
    VeduEfl_EncPara_S*  pEncPara = NULL;
    HI_S32 s32Ret;
    queue_data_s  Queue_data;
    HI_U32 i;

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((g_stVencChn[i].hVEncHandle == EncHandle) && (g_stVencChn[i].hVEncHandle != NULL))
        {
            pEncPara  = EncHandle;
            break;
        }
    }

    if ((i == MAX_VEDU_CHN) || (pEncPara == NULL))
    {
        HI_ERR_VENC(" bad handle!!\n");

        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    Queue_data.bToOMX = 0;
    memcpy(&(Queue_data.unFrmInfo.queue_info), pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
    s32Ret = VENC_DRV_MngQueue(pEncPara->FrameQueue, &Queue_data, 0, 0);

    if (HI_SUCCESS == s32Ret)
    {
        pEncPara->stStat.QueueNum++;
        VENC_DRV_EflWakeUpThread();

    }

    return (s32Ret == HI_SUCCESS) ? HI_SUCCESS : HI_ERR_VENC_BUF_EMPTY;
}

HI_S32 VENC_DRV_EflQFrameByAttach( HI_U32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S*  pstFrame)
{
    HI_S32 s32ChIndx = 0;
    HI_S32 s32Ret = 0;
    unsigned long flags;
    VeduEfl_EncPara_S*  pEncPara;
    HI_U32 u32StreamThr = 0;

    while (s32ChIndx < VENC_MAX_CHN_NUM)
    {
        if (g_stVencChn[s32ChIndx].hUsrHandle == EncUsrHandle)
        {
            break;
        }

        s32ChIndx++;
    }

    if (VENC_MAX_CHN_NUM  == s32ChIndx)
    {
        HI_ERR_VENC(" bad handle!!\n");

        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    pEncPara = g_stVencChn[s32ChIndx].hVEncHandle;

    if(pEncPara == NULL)
    {
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    if (g_stVencChn[s32ChIndx].enSrcModId >= HI_ID_BUTT)
    {
        HI_ERR_VENC("cur srcMod(0x%x) error\n", g_stVencChn[s32ChIndx].enSrcModId);
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    pEncPara->stStat.QueueFrmByAttachCnt++;

    u32StreamThr = D_VENC_RC_MIN((pEncPara->StrmBufSize / 10), ((pEncPara->BitRate / pEncPara->VoFrmRate) * 2));

    if (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < u32StreamThr)
    {
        pEncPara->stStat.TooFewBufferQueFailCnt++;

        return HI_ERR_VENC_BUF_EMPTY;
    }

    spin_lock_irqsave(&g_SendFrame_Lock[s32ChIndx], flags);
    s32Ret = VENC_DRV_EflQueueFrame(pEncPara, pstFrame);
    spin_unlock_irqrestore(&g_SendFrame_Lock[s32ChIndx], flags);

    return s32Ret;
}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32  VENC_DRV_EflDequeueFrame( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    VeduEfl_EncPara_S*  pEncPara = NULL;
    HI_S32 s32Ret;
    HI_U32 i;

    queue_data_s  Queue_data;

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((g_stVencChn[i].hVEncHandle == EncHandle) && (g_stVencChn[i].hVEncHandle != NULL))
        {
            pEncPara  = EncHandle;
            break;
        }
    }

    if ((i == MAX_VEDU_CHN) || (pEncPara == NULL))
    {
        HI_ERR_VENC(" bad handle!!\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    s32Ret = VENC_DRV_MngDequeue(pEncPara->FrameDequeue, &Queue_data);

    if (Queue_data.bToOMX || s32Ret)
    {
        HI_INFO_VENC("NOT FOR OMX FREAM!!\n");
        return s32Ret;
    }
    else
    {
        memcpy(pstFrame, &(Queue_data.unFrmInfo.queue_info), sizeof(HI_DRV_VIDEO_FRAME_S));
    }

    pEncPara->stStat.DequeueNum--;
    return s32Ret;
}


HI_S32 VENC_DRV_EflRlsAllFrame( VeduEfl_EncPara_S* EncHandle)
{
    VEDU_LOCK_FLAG flag;
    //HI_DRV_VIDEO_FRAME_S stFrame ;
    VeduEfl_EncPara_S*  pEncPara = NULL;
    HI_S32 s32Ret = 0;
    HI_U32 i;
    queue_data_s  Queue_data;

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if ((g_stVencChn[i].hVEncHandle == EncHandle) && (g_stVencChn[i].hVEncHandle != NULL))
        {
            pEncPara  = EncHandle;
            break;
        }
    }

    if ((i == MAX_VEDU_CHN) || ((pEncPara == NULL)))
    {
        HI_ERR_VENC(" bad handle!!\n");
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);

    while ( (!VENC_DRV_MngQueueEmpty(pEncPara->FrameQueue)) && (!s32Ret) )
    {
        s32Ret = VENC_DRV_MngDequeue(pEncPara->FrameQueue, &Queue_data);

        if (!s32Ret && !Queue_data.bToOMX)
        {
            (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &(Queue_data.unFrmInfo.queue_info));

            if (g_stVencChn[i].enSrcModId >= HI_ID_BUTT)
            {
                pEncPara->stStat.VpssImgBufNum--;
            }
            else
            {
                pEncPara->stStat.QueueNum--;
            }
        }
    }

    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag);
    return s32Ret;
}

HI_S32 VENC_DRV_Refresh_FromVPSS( VeduEfl_EncPara_S* EncHandle )
{
    HI_DRV_VIDEO_FRAME_S    stVpssFrame;
    HI_U32  u32Count = 0;
    VeduEfl_EncPara_S*  pEncPara;

    HI_U32 u32ChnID;
    HI_S32 s32Ret = 0;
    HI_BOOL bVPSSAllDone = HI_FALSE;
    D_VENC_GET_CHN(u32ChnID, EncHandle);
    D_VENC_CHECK_CHN(u32ChnID);

    pEncPara  = EncHandle;

    do
    {
        if (HI_SUCCESS == (pVpssFunc->pfnVpssGetPortFrame(g_stVencChn[u32ChnID].hPort[0], &stVpssFrame)))
        {
            pVpssFunc->pfnVpssRelPortFrame(g_stVencChn[u32ChnID].hPort[0], &stVpssFrame);
        }

        msleep(5);

        s32Ret = pVpssFunc->pfnVpssSendCommand(g_stVencChn[u32ChnID].hVPSS, HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE, &bVPSSAllDone);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_VENC("%s, %d, pfnVpssSendCommand HI_DRV_VPSS_USER_COMMAND_CHECKALLDONE error\n", __func__, __LINE__);
        }
        

        u32Count++;

        if (u32Count == 50)
        {
            HI_ERR_VENC("VPSS check all done failed!\n");
        }
    }
    while ((!VeduIpCtx.StopTask) && (!bVPSSAllDone) && (u32Count < 50));

    return HI_SUCCESS;
}
/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflGetImage(HI_S32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_U32 u32ChnID;
    HI_S32 s32Ret;
    VeduEfl_EncPara_S* pEncPara;
    queue_data_s  Queue_data;
    D_VENC_GET_CHN_BY_UHND(u32ChnID, EncUsrHandle);
    D_VENC_CHECK_CHN(u32ChnID);
    pEncPara = g_stVencChn[u32ChnID].hVEncHandle;
    s32Ret = VENC_DRV_MngDequeue(pEncPara->FrameQueue, &Queue_data);

    if (Queue_data.bToOMX || s32Ret)
    {
        HI_INFO_VENC("NOT FOR OMX FREAM!!\n");
        return s32Ret;
    }
    else
    {
        memcpy(pstFrame, &(Queue_data.unFrmInfo.queue_info), sizeof(HI_DRV_VIDEO_FRAME_S));
    }

    pEncPara->stStat.QueueNum--;
    return s32Ret;
}

HI_S32 VENC_DRV_EflGetImage_OMX(HI_S32 EncUsrHandle, venc_user_buf* pstFrame)
{
    HI_U32 u32ChnID;
    HI_S32 s32Ret;
    VeduEfl_EncPara_S*  pEncPara;
    queue_data_s  Queue_data;

    D_VENC_GET_CHN_BY_UHND(u32ChnID, EncUsrHandle);
    D_VENC_CHECK_CHN(u32ChnID);
    pEncPara = g_stVencChn[u32ChnID].hVEncHandle;

    if ( VENC_DRV_MngQueueEmpty(pEncPara->FrameQueue_OMX))
    {
        return HI_FAILURE;
    }

    s32Ret = VENC_DRV_MngDequeue(pEncPara->FrameQueue_OMX, &Queue_data);

    if ( !Queue_data.bToOMX || s32Ret)
    {
        HI_INFO_VENC("err:not match for OMX!! ret = %d\n", s32Ret);
        return s32Ret;
    }
    else
    {
        memcpy(pstFrame, &(Queue_data.unFrmInfo.msg_info_omx.buf), sizeof(venc_user_buf));
    }

    pEncPara->stStat.QueueNum--;
    return HI_SUCCESS;

}

/******************************************************************************
Function   :
Description:
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
HI_S32 VENC_DRV_EflPutImage(HI_S32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S* pstFrame)
{
    HI_U32 u32ChnID;
    HI_S32 s32Ret;
    VeduEfl_EncPara_S* pEncPara;
    queue_data_s  Queue_data;
    D_VENC_GET_CHN_BY_UHND(u32ChnID, EncUsrHandle);
    D_VENC_CHECK_CHN(u32ChnID);

    pEncPara = g_stVencChn[u32ChnID].hVEncHandle;

    Queue_data.bToOMX = 0;
    memcpy(&(Queue_data.unFrmInfo.queue_info), pstFrame, sizeof(HI_DRV_VIDEO_FRAME_S));
    s32Ret = VENC_DRV_MngQueue(pEncPara->FrameDequeue, &Queue_data, 0, 0);

    if (!s32Ret)
    {
        pEncPara->stStat.DequeueNum++;
    }

    return s32Ret;

}

#if (defined __VENC_98CV200_CONFIG__)
#ifdef HI_SMMU_SUPPORT
HI_U32 VeduEfl_SmmuToPhyAddr(HI_U32 PageTabBaseAddr , HI_U32 SmmuAddr  )
{
    //HI_U32 PageTabBaseAddr = 0;
    HI_U32 PageIndex = 0;
    HI_U32 PageOffset = 0;
    HI_U32 PhyAddr = 0;
    HI_U32*  pPage;
    HI_U32 Hige20Bit, Low12Bit;
    //PageTabBaseAddr = pAllReg->SMMU_CB_TTBR;

    HI_INFO_VENC("%s,%d,PageTabBaseAddr = 0x%x,smmuAddr = 0x%x\n", __func__, __LINE__, PageTabBaseAddr, SmmuAddr);

    if (PageTabBaseAddr == 0)
    {
        HI_ERR_VENC("%s,%d, pageTabBaseAddr = %d Error!\n", __func__, __LINE__, PageTabBaseAddr);
        return -1;
    }

    PageOffset = SmmuAddr / 4096 * 4;

    PageIndex = PageTabBaseAddr + PageOffset;
    HI_INFO_VENC("%s,%d,PageIndex = 0x%x\n", __func__, __LINE__, PageIndex);

    pPage = (HI_U32*)phys_to_virt(PageIndex); //(HI_U32 *)ioremap(PageIndex, 4);

    Hige20Bit = ((*pPage) & 0xfffff000);
    HI_INFO_VENC("%s,%d,Hige20Bit = 0x%x(0x%x)\n", __func__, __LINE__, Hige20Bit, *pPage);

    Low12Bit = SmmuAddr & 0xfff;
    HI_INFO_VENC("%s,%d,Low12Bit = 0x%x\n", __func__, __LINE__, Low12Bit);

    PhyAddr = Hige20Bit + Low12Bit;

    HI_INFO_VENC("\n%s,%d,~ PhyAddr = 0x%x\n", __func__, __LINE__, PhyAddr);
    return PhyAddr;
}
#endif
#endif


/////////////////////////////////////////////////////////////////debug
HI_S32 VENC_DRV_DbgWriteYUV(HI_DRV_VIDEO_FRAME_S* pstFrame, HI_CHAR* pFileName)
{
    char str[64] = {0};
    unsigned char* ptr;
    struct file* fp;

    HI_U8* pu8Udata;
    HI_U8* pu8Vdata;
    HI_U8* pu8Ydata;
    HI_S8  s_VencSavePath[64];
    HI_U32 i, j;
#ifdef HI_SMMU_SUPPORT
    VENC_BUFFER_S stMBuf;
    HI_S32 Ret;

    memset(&stMBuf, 0, sizeof(VENC_BUFFER_S));
#endif
    HI_DRV_LOG_GetStorePath(s_VencSavePath, 64);

    pu8Udata = HI_VMALLOC(HI_ID_VENC, pstFrame->u32Width * pstFrame->u32Height / 2 / 2);

    if (pu8Udata == HI_NULL)
    {
        return HI_FAILURE;
    }

    pu8Vdata = HI_VMALLOC(HI_ID_VENC, pstFrame->u32Width * pstFrame->u32Height / 2 / 2);

    if (pu8Vdata == HI_NULL)
    {
        HI_VFREE(HI_ID_VENC, pu8Udata);
        return HI_FAILURE;
    }

    pu8Ydata = HI_VMALLOC(HI_ID_VENC, pstFrame->stBufAddr[0].u32Stride_Y);

    if (pu8Ydata == HI_NULL)
    {
        HI_VFREE(HI_ID_VENC, pu8Udata);
        HI_VFREE(HI_ID_VENC, pu8Vdata);
        return HI_FAILURE;
    }

#ifdef HI_SMMU_SUPPORT
    stMBuf.u32StartPhyAddr = pstFrame->stBufAddr[0].u32PhyAddr_Y;
    Ret = HI_DRV_VENC_Map(&stMBuf);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_VENC("SMMU map is failure!\n");
        HI_VFREE(HI_ID_VENC, pu8Udata);
        HI_VFREE(HI_ID_VENC, pu8Vdata);
        HI_VFREE(HI_ID_VENC, pu8Ydata);

        return HI_FAILURE;
    }
    else
    {
        ptr = stMBuf.pu8StartVirAddr;
    }
#else
    ptr = (unsigned char*)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
#endif

    if (!ptr)
    {
        HI_ERR_VENC("address is not valid!\n");
    }
    else
    {
        HI_OSAL_Snprintf(str, 64, "%s/%s", s_VencSavePath, pFileName);

        fp = VENC_DRV_OsalFopen(str, O_RDWR | O_CREAT | O_APPEND, 0);

        if (fp == HI_NULL)
        {
            HI_ERR_VENC("open file '%s' fail!\n", str);
            HI_VFREE(HI_ID_VENC, pu8Udata);
            HI_VFREE(HI_ID_VENC, pu8Vdata);
            HI_VFREE(HI_ID_VENC, pu8Ydata);
        #ifdef HI_SMMU_SUPPORT
            HI_DRV_VENC_Unmap(&stMBuf);
        #endif

            return HI_FAILURE;
        }

        /*写 Y 数据*/
        for (i = 0; i < pstFrame->u32Height; i++)
        {
            memcpy(pu8Ydata, ptr, sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);

            if (pstFrame->u32Width != VENC_DRV_OsalFwrite(pu8Ydata, pstFrame->u32Width, fp))
            {
                HI_ERR_VENC("fwrite fail!\n");
            }

            ptr += pstFrame->stBufAddr[0].u32Stride_Y;
        }

#ifdef HI_SMMU_SUPPORT
        ptr = stMBuf.pu8StartVirAddr + (pstFrame->stBufAddr[0].u32PhyAddr_C - pstFrame->stBufAddr[0].u32PhyAddr_Y);
#else
        ptr = (unsigned char*)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);
#endif

        /* U V 数据 转存*/
        for (i = 0; i < pstFrame->u32Height / 2; i++)
        {
            for (j = 0; j < pstFrame->u32Width / 2; j++)
            {
                if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
                {
                    pu8Vdata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j];
                    pu8Udata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j + 1];
                }
                else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
                {
                    pu8Udata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j];
                    pu8Vdata[i * pstFrame->u32Width / 2 + j] = ptr[2 * j + 1];
                }
                else
                {
                    HI_ERR_VENC("other pix formet= %d\n", pstFrame->ePixFormat);
                }
            }

            ptr += pstFrame->stBufAddr[0].u32Stride_C;
        }

        /*写 U */
        VENC_DRV_OsalFwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 / 2, fp);

        /*写 V */
        VENC_DRV_OsalFwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 / 2, fp);

        VENC_DRV_OsalFclose(fp);
        HI_INFO_VENC("2d image has been saved to '%s' W=%d H=%d Format=%d \n",
                     str, pstFrame->u32Width, pstFrame->u32Height, pstFrame->ePixFormat);
    }

    HI_VFREE(HI_ID_VENC, pu8Udata);
    HI_VFREE(HI_ID_VENC, pu8Vdata);
    HI_VFREE(HI_ID_VENC, pu8Ydata);
#ifdef HI_SMMU_SUPPORT
    HI_DRV_VENC_Unmap(&stMBuf);
#endif
    return HI_SUCCESS;
}

/////////////////////////////////////////////////////////////////debug end
/*************************************************************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
