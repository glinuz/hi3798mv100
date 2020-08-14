#include "drv_venc_efl.h"
#include "drv_omxvenc_efl.h"
#include "drv_venc_osal.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "drv_venc.h"

#include "hal_venc.h"
#include "hi_drv_log.h"
#include "hi_osal.h"
//#include "drv_vi_ext.h"
//#include "drv_vo_ext.h"
#include "drv_venc_osal_ext.h"
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

extern Venc_Osal_Func_Ptr venc_Osal_Func_Ptr_S;

static HI_U32 g_idr_pic_id = 0;

//#define __VENC_FREQ_DYNAMIC__


extern HI_VOID VENC_DRV_BoardInit(HI_VOID);
extern HI_VOID VENC_DRV_BoardDeinit(HI_VOID);

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define Smooth_printk printk
#else
#define Smooth_printk(format, arg...)
#endif

#if ( defined(__VENC_98CV200_CONFIG__) || defined(__VENC_98MV200_CONFIG__) )  
#ifdef HI_SMMU_SUPPORT
HI_U32 VeduEfl_SmmuToPhyAddr(HI_U32 PageTabBaseAddr ,HI_U32 SmmuAddr  );
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

#if ( defined(__VENC_98MV200_CONFIG__) )
static const HI_U32  g_h264LambdaSsd[40] =
{
    1,    1,    1,    2,    2,    3,    3,    4,    5,    7,
    9,   11,   14,   17,   22,   27,   34,   43,   54,   69,
    86,  109,  137,  173,  218,  274,  345,  435,  548,  691,
    870, 1097, 1382, 1741, 2193, 2763, 3482, 4095, 4095, 4095
};

static const HI_U32 g_h264LambdaSad[40] =
{
    1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 6,
    6, 7, 8, 9, 10, 11, 13, 14, 16, 18, 20, 23, 25, 29, 32, 36,
    40, 45, 51, 57, 64, 72, 81, 91
};

static const HI_U32  g_h265LambdaSsd[40] =
{
    18, 22, 28, 36, 45, 57, 72, 91, 115, 145, 
    183, 231, 291, 367, 463, 583, 735, 926, 1167, 1470, 
    1853, 2334, 2941, 3706, 4669, 5883, 7412, 9338, 11766, 14824, 
    18677, 23532, 29649, 37355, 47065, 59298, 74711, 94130, 118596, 131071
};

static const HI_U32 g_h265LambdaSad[40] =
{
    193, 216, 243, 273, 306, 344, 386, 433, 487, 546, 
    613, 688, 773, 867, 974, 1093, 1227, 1377, 1546, 1735, 
    1948, 2186, 2454, 2755, 3092, 3471, 3896, 4373, 4908, 5510, 
    6184, 6942, 7792, 8746, 9817, 11020, 12369, 13884, 15584, 17493
};

static const unsigned char QpTable[96] = 
{
	16, 16, 16, 16,    16, 16, 16, 16,    16, 16, 16, 16,    16, 16, 16, 16,
	16, 16, 16, 16,    16, 16, 16, 16,    16, 16, 16, 16,    16, 16, 16, 16,
	16, 16, 16, 16,    16, 16, 18, 20,    21, 22, 23, 24,    25, 25, 26, 27, 
	28, 28, 29, 29,    30, 30, 30, 31,    31, 32, 32, 33,    33, 33, 34, 34,
	34, 34, 35, 35,    35, 36, 36, 36,    36, 36, 37, 37,    37, 37, 38, 38,
	38, 38, 38, 39,    39, 39, 39, 39,    39, 40, 40, 40,    40, 41, 41, 41,  
};
static const int TabLnx[64] = 
{
       0, -1216, -972, -830, -729, -651, -587, -533,
    -486,  -445, -408, -374, -344, -316, -290, -265,
    -243,  -221, -201, -182, -164, -147, -131, -115,
    -100,   -86,  -72,  -59,  -46,  -34,  -22,  -11,
       0,    10,   21,   31,   41,   50,   60,   69,
      78,    86,   95,  103,  111,  119,  127,  134,
     142,   149,  156,  163,  170,  177,  183,  190,
     196,   202,  208,  214,  220,  226,  232,  237,
};
static const int MaxIPropDeltaQP[52] = 
{
    10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 
     9,  9,  8,  8,  7,  7,  6,  6,  5,  5,  5,  4,  4,  4,
     3,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  1,  1,  1,
     1,  1,  1,  0,  0,  0,  0,  0,  0,  0,
};
#endif
/*******************************************************************/

VeduEfl_ChnCtx_S VeduChnCtx[MAX_VEDU_CHN];

VeduEfl_IpCtx_S VeduIpCtx;

extern OPTM_VENC_CHN_S g_stVencChn[VENC_MAX_CHN_NUM];

extern VEDU_OSAL_EVENT g_VencWait_Stream[VENC_MAX_CHN_NUM];
extern VPSS_EXPORT_FUNC_S *pVpssFunc;
wait_queue_head_t gqueue;
volatile HI_U32 gwait;

extern volatile HI_U32 gencodeframe;
extern wait_queue_head_t gEncodeFrame;

extern spinlock_t g_SendFrame_Lock[VENC_MAX_CHN_NUM];     /*lock the destroy and send frame*/
VEDU_OSAL_EVENT g_VENC_Event;
VEDU_OSAL_EVENT g_VENC_Event_stream;
//extern HI_S32  HI_DRV_VPSS_GetPortFrame(HI_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);           //add
//extern HI_S32  HI_DRV_VPSS_RelPortFrame(VPSS_HANDLE hPort, HI_DRV_VIDEO_FRAME_S *pstVpssFrame);

static HI_VOID VENC_DRV_GetLastFrmInfo_OMX(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncIn_S *pEncIn);
extern VENC_BUFFER_S g_stKernelOmxmmz[VENC_MAX_CHN_NUM][OMX_OUTPUT_PORT_BUFFER_MAX];
extern HI_U32 g_map_count;
extern HI_U32 g_ummap_count;

#ifdef __VENC_SUPPORT_JPGE__
extern JPGE_EXPORT_FUNC_S *pJpgeFunc;
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

#define D_VENC_RC_UPDATE(data,curdata) \
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

HI_S32 VEDU_DRV_RCProcessOfRecoding(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncIn_S *EncIn);
HI_S32 VENC_DRV_GetEncodePara(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncIn_S *pEncIn);



void Update_StreamBuffer(int data[30],int data2)
{
    int i;
	for(i = 29; i > 0;i--)
	{
	    data[i] = data[i-1];
	}
	data[0] = data2;
}

static HI_VOID H264e_PutTrailingBits(tBitStream *pBS)
{
    VENC_DRV_BsPutBits31(pBS, 1, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0, 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_VOID MP4e_PutTrailingBits(tBitStream *pBS)
{
    VENC_DRV_BsPutBits31(pBS, 0, 1);

    if (pBS->totalBits & 7)
    {
        VENC_DRV_BsPutBits31(pBS, 0xFF >> (pBS->totalBits & 7), 8 - (pBS->totalBits & 7));
    }

    *pBS->pBuff++ = (pBS->bBigEndian ? pBS->tU32b : REV32(pBS->tU32b));
}

static HI_U32 H264e_MakeSPS(HI_U8 *pSPSBuf, const VeduEfl_H264e_SPS_S *pSPS)
{
    HI_U32 code, TotalMb, profile_idc, level_idc,direct_8x8_interence_flag;
    int bits;

    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pSPSBuf);

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

   if(100 == pSPS->ProfileIDC)   //just for high profile
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

	HI_INFO_VENC("VENC h264 profile_id:%d,level_id:%d,num_ref_frames: %d\n",profile_idc,level_idc,1);
    return (HI_U32)BS.totalBits;
}

static HI_U32 H264e_MakePPS(HI_U8 *pPPSBuf, const VeduEfl_H264e_PPS_S *pPPS)
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

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pPPSBuf);

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
 #if ( defined(__VENC_S40V200_CONFIG__) || defined(__VENC_98MV200_CONFIG__) ) //#ifdef __VENC_S40V200_CONFIG__
      VENC_DRV_BsPutBits31(&BS, 1, 1); // transform_8x8_mode_flag
 #else
      VENC_DRV_BsPutBits31(&BS, 0, 1); // transform_8x8_mode_flag
 #endif
 #if ( defined(__VENC_98MV200_CONFIG__) )
      VENC_DRV_BsPutBits31(&BS, 0, 1); // pic_scaling_matrix_present_flag
      i = 0, j = 0;
 #else
      VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_matrix_present_flag
      
      for(i = 0; i < 6; i++)
      {
        VENC_DRV_BsPutBits31(&BS, 1, 1); // pic_scaling_list_present_flag
        
        se_vlc(bits, code, 8);  
		VENC_DRV_BsPutBits31(&BS, code, bits); /* all be 16 */
        for(j = 0; j < 15; j++)
		{
		    VENC_DRV_BsPutBits31(&BS, 1, 1);
        }
      }
 #endif
      
#ifdef __VENC_S40V200_CONFIG__
      for(i = 0; i < 2; i++)
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
                               const VeduEfl_H264e_SlcHdr_S*pSlcHdr)
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
    else if(pSlcHdr->NumRefIndex ==  0)
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

/*获取VPS语法信息*/
static HI_U32 H265e_MakeVPS(HI_U8 *pVPSBuf, const VeduEfl_H265e_VPS_S *pVPS)
{
    HI_U32 code;
    int    bits;

    int i = 0;
    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pVPSBuf);

    VENC_DRV_BsPutBits32(&BS, 1, 32);
    VENC_DRV_BsPutBits31(&BS, 0x4001, 16);

    VENC_DRV_BsPutBits31(&BS, 0, 4);           //video_parameter_set_id = 0  u(4)
    VENC_DRV_BsPutBits31(&BS, 3, 2);           //vps_reserved_three_2bits = 3  u(2)
    VENC_DRV_BsPutBits31(&BS, 0, 6);           //vps_max_layers_minus1 = 0   u(6)
    VENC_DRV_BsPutBits31(&BS, 0, 3);           //vps_max_sub_layers_minus1 = 0或1  u(3)   开大小P，取值为1；否则取值为0
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //vps_temporal_id_nesting_flag = 1 u(1)
    VENC_DRV_BsPutBits31(&BS, 0xffff, 16);     //vps_reserved_ffff_16bits = 0xffff u(16)

    {   ////////profile_tier_level
        VENC_DRV_BsPutBits31(&BS, 0, 2);       //general_profile_space=0  u(2)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_tier_flag=0  u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 5);       //general_profile_idc =1   u(5)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_profile_compatibility_flag[0]  u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 1);       //general_profile_compatibility_flag[1]  u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 1);       //general_profile_compatibility_flag[2]  u(1)

        for(i = 3; i<32;i++)
        {
            VENC_DRV_BsPutBits31(&BS, 0, 1);   //general_profile_compatibility_flag[][j]  u(1)
        }

        VENC_DRV_BsPutBits31(&BS, pVPS->general_progressive_source_flag, 1);       //general_progressive_source_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, pVPS->general_interlaced_source_flag, 1);       //general_interlaced_source_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_non_packed_constraint_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_frame_only_constraint_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 16);      //general_reserved_zero_44bits[0..15] = 0  u(16)
        VENC_DRV_BsPutBits31(&BS, 0, 16);      //general_reserved_zero_44bits[16..31] = 0  u(16)
        VENC_DRV_BsPutBits31(&BS, 0, 12);      //general_reserved_zero_44bits[32..43] = 0  u(12)
        VENC_DRV_BsPutBits31(&BS, 0, 8);       //general_level_idc=0    u(8)
    }

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //vps_sub_layer_ordering_info_present_flag = 1  u(1)

    for( i = 0; i <= 0; i++ )
    {
        ue_vlc(bits, code, 1);
        VENC_DRV_BsPutBits31(&BS, code, bits); //vps_max_dec_pic_buffering_minus1[ j ]  ue(v), 1

        ue_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //vps_max_num_reorder_pics[ j ]  ue(v),0
     
        ue_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //vps_max_latency_increase_plus1[ j ]  ue(v),0        
    }


    VENC_DRV_BsPutBits31(&BS, 0, 6);           //vps_max_layer_id=0  u(6)

    ue_vlc(bits, code, 0);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //vps_num_layer_sets_minus1=0  ue(v)

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //vps_timing_info_present_flag = 0  u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //vps_extension_flag=0    u(1)   

    H264e_PutTrailingBits(&BS);
    
    return (HI_U32)BS.totalBits;
}

/*获取SPS语法信息*/
static HI_U32 H265e_MakeSPS(HI_U8 *pSPSBuf, const VeduEfl_H265e_SPS_S *pSPS)
{
    HI_U32 code;
    int    bits, i;

    tBitStream BS;
    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pSPSBuf);

    VENC_DRV_BsPutBits32(&BS, 1, 32);
    VENC_DRV_BsPutBits31(&BS, 0x4201, 16);

    VENC_DRV_BsPutBits31(&BS, 0, 4);           // video_parameter_set_id  u(4)
    VENC_DRV_BsPutBits31(&BS, 0, 3);           // sps_max_sub_layers_minus1=0  u(3)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // sps_temporal_id_nesting_flag = 1  u(1)

    {   //profile_tier_level
        VENC_DRV_BsPutBits31(&BS, 0, 2);       //general_profile_space  u(2)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_tier_flag=0  u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 5);       //general_profile_idc=1   u(5)

        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_profile_compatibility_flag[][0] =1u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 1);       //general_profile_compatibility_flag[][1] =1u(1)
        VENC_DRV_BsPutBits31(&BS, 1, 1);       //general_profile_compatibility_flag[][2] =1u(1)
        for ( i = 3; i < 32; i++ )
        {
            VENC_DRV_BsPutBits31(&BS, 0, 1);   //general_profile_compatibility_flag[][j]=0  u(1)
        }
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_progressive_source_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_interlaced_source_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_non_packed_constraint_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //general_frame_only_constraint_flag = 0  u(1)
        VENC_DRV_BsPutBits31(&BS, 0, 16);      //general_reserved_zero_44bits[0..15] = 0  u(16)
        VENC_DRV_BsPutBits31(&BS, 0, 16);      //general_reserved_zero_44bits[16..31] = 0  u(16)
        VENC_DRV_BsPutBits31(&BS, 0, 12);      //general_reserved_zero_44bits[32..43] = 0  u(12)
        VENC_DRV_BsPutBits31(&BS, 0, 8);       //general_level_idc    u(8)
    }   
   

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //seq_parameter_set_id   ue(v) //sps_id = 0
    
    ue_vlc(bits, code, 1);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //chroma_format_idc = 1  ue(v)

    ue_vlc(bits, code, pSPS->uiPicWidthInLumaSamples);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //pic_width_in_luma_samples   ue(v)

    ue_vlc(bits, code, pSPS->uiPicHeightInLumaSamples);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //pic_height_in_luma_samples  ue(v)

    VENC_DRV_BsPutBits31(&BS, pSPS->bPicCroppingFlag, 1); 

    if( 1 == pSPS->bPicCroppingFlag )
    {
        ue_vlc(bits, code, pSPS->uiPicCropLeftOffset);
        VENC_DRV_BsPutBits31(&BS, code, bits); 

        ue_vlc(bits, code, pSPS->uiPicCropRightOffset);
        VENC_DRV_BsPutBits31(&BS, code, bits); 

        ue_vlc(bits, code, pSPS->uiPicCropTopOffset);
        VENC_DRV_BsPutBits31(&BS, code, bits); 

        ue_vlc(bits, code, pSPS->uiPicCropBottomOffset);
        VENC_DRV_BsPutBits31(&BS, code, bits);   
    }

    VENC_DRV_BsPutBits31(&BS, 1, 1);           // bit_depth_luma_minus8 = 0    ue(v)  
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // bit_depth_chroma_minus8 = 0   ue(v)

    ue_vlc(bits, code, 12);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //log2_max_pic_order_cnt_lsb_minus4 =12   ue(v)

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //sps_sub_layer_ordering_info_present_flag = 1   u(1)
    
    {   //for( i = 0; i <= pSPS->uiSPSMaxSubLayersMinus1; i++ )
        ue_vlc(bits, code, 1);
        VENC_DRV_BsPutBits31(&BS, code, bits); //sps_max_dec_pic_buffering_minus1[ i ] = 1    ue(v)

        ue_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //sps_max_num_reorder_pics[ i ]=0      ue(v),

        VENC_DRV_BsPutBits31(&BS, 1, 1);       //sps_max_latency_increase_plus1[ i ]=0     ue(v)
    }

    VENC_DRV_BsPutBits31(&BS, 1, 1);           // log2_min_luma_coding_block_size_minus3=0     ue(v)

    ue_vlc(bits, code, 3);
    VENC_DRV_BsPutBits31(&BS, code, bits);     // log2_diff_max_min_luma_coding_block_size = 3   ue(v)
     
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //log2_min_transform_block_size_minus2 =0    ue(v)

    ue_vlc(bits, code, 2);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //log2_diff_max_min_transform_block_size=2   ue(v)

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //max_transform_hierarchy_depth_inter =0   ue(v)  
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //max_transform_hierarchy_depth_intra =0   ue(v)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //scaling_list_enable_flag=0         u(1)

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //amp_enabled_flag =0        u(1)

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //sample_adaptive_offset_enabled_flag =0   u(1)
  
    VENC_DRV_BsPutBits31(&BS, pSPS->bPcmEnable, 1); 
    if (1 == pSPS->bPcmEnable)
    {
        VENC_DRV_BsPutBits31(&BS, 7, 4);       //pcm_sample_bit_depth_luma_minus1=7    u(4)
        VENC_DRV_BsPutBits31(&BS, 7, 4);       //pcm_sample_bit_depth_chroma_minus1=7  u(4)

        ue_vlc(bits, code, (pSPS->iPcmLog2MinSize - 3));
        VENC_DRV_BsPutBits31(&BS, code, bits); //log2_min_pcm_luma_coding_block_size_minus3    ue(v)

        ue_vlc(bits, code, (pSPS->iPcmLog2MaxSize - pSPS->iPcmLog2MinSize));
        VENC_DRV_BsPutBits31(&BS, code, bits); //log2_diff_max_min_pcm_luma_coding_block_size    ue(v)

        VENC_DRV_BsPutBits31(&BS, 0, 1);       //pcm_loop_filter_disabled_flag    u(1)
    }

    ue_vlc(bits, code, 1);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //num_short_term_ref_pic_sets = 1   ue(v)

    {   /*short_term_ref_pic_set*/ 
        ue_vlc(bits, code, 1);
        VENC_DRV_BsPutBits31(&BS, code, bits); //num_Negative_pics = 1

        ue_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //num_positive_pics = 0  ue(v)

        ue_vlc(bits, code,  0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //delta_poc_s0_minus1 = 0        ue(v)

        VENC_DRV_BsPutBits31(&BS, 1, 1);       //used_by_curr_pic_s0_flag = 1   u(1) 
    }

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //long_term_ref_pics_present_flag = 0     u(1)
    
    VENC_DRV_BsPutBits31(&BS, pSPS->sps_temporal_mvp_enable_flag, 1);  //sps_temporal_mvp_enable_flag =1           u(1)
    VENC_DRV_BsPutBits31(&BS, pSPS->bStrongIntraSmoothingFlag, 1);     //strong_intra_smoothing_enable_flag=1      u(1)

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //vui_parameters_present_flag =0        u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //sps_extension_flag = 0                u(1)

    H264e_PutTrailingBits(&BS);

    return (HI_U32)BS.totalBits;
}

/*获取PPS语法信息*/

static HI_U32 H265e_MakePPS(HI_U8 *pPPSBuf, const VeduEfl_H265e_PPS_S *pPPS)
{
    HI_U32 code;
    int    bits;

    tBitStream BS;
    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pPPSBuf);

    VENC_DRV_BsPutBits32(&BS, 1, 32);
    VENC_DRV_BsPutBits31(&BS, 0x4401, 16);

    VENC_DRV_BsPutBits31(&BS, 1, 1);           // pic_parameter_set_id=0   ue(v)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // seq_parameter_set_id=0   ue(v)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           // dependent_slice_segments_enabled_flag=0        u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           // output_flag_present_flag=0        u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 3);           // num_extra_slice_header_bits = 0     u(3)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           // sign_data_hiding_flag = 0   u(1)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // cabac_init_present_flag=0    u(1)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // num_ref_idx_l0_default_active_minus1 =0     ue(v)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           // num_ref_idx_l1_default_active_minus1=0      ue(v)

    se_vlc(bits, code, 0);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //pic_init_qp_minus26=0                       se(v)

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //constrained_intra_pred_flag =0              u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //transform_skip_enabled_flag=0               u(1)

    VENC_DRV_BsPutBits31(&BS, pPPS->bCUQpDeltaEnable, bits);    //cu_qp_delta_enabled_flag               u(1)
    if( 1 == pPPS->bCUQpDeltaEnable )
    {
        ue_vlc(bits, code, 2);
        VENC_DRV_BsPutBits31(&BS, code, bits); //diff_cu_qp_delta_depth                ue(v)
    }

    se_vlc(bits, code, pPPS->iCbQpOffset);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //pic_cb_qp_offset                    se(v)

    se_vlc(bits, code, pPPS->iCrQpOffset);
    VENC_DRV_BsPutBits31(&BS, code, bits);     //pic_cr_qp_offset                    se(v)

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //pic_slice_chroma_qp_offsets_present_flag = 0   u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //weighted_pred_flag = 0        u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //weighted_bipred_flag = 0        u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //transquant_bypass_enable_flag   u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //tiles_enabled_flag = 0             u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //entropy_coding_sync_enabled_flag    u(1)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //loop_filter_across_slices_enabled_flag    u(1)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //deblocking_filter_control_present_flag = 1   u(1) 
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //deblocking_filter_override_enabled_flag   u(1)
    VENC_DRV_BsPutBits31(&BS, pPPS->bPicDisableDBFilter, 1);        //pic_disable_deblocking_filter_flag   u(1)
    if( 0 == pPPS->bPicDisableDBFilter )
    {
        se_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //pps_beta_offset_div2    se(v)

        se_vlc(bits, code, 0);
        VENC_DRV_BsPutBits31(&BS, code, bits); //pps_tc_offset_div2       se(v)
    }

    VENC_DRV_BsPutBits31(&BS, 0, 1);           //pic_scaling_list_data_present_flag    u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //lists_modification_present_flag = 0   u(1)
    VENC_DRV_BsPutBits31(&BS, 1, 1);           //log2_parallel_merge_level_minus2=0    ue(v)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //slice_segment_header_extension_present_flag=0     u(1)
    VENC_DRV_BsPutBits31(&BS, 0, 1);           //pps_extension_flag    u(1)

    H264e_PutTrailingBits(&BS);

    return (HI_U32)BS.totalBits;
}

static HI_VOID H265e_MakeSliceHead_DBSAO(VeduEfl_EncPara_S *pEncPara, const VeduEfl_H265e_SlcHdr_S *pSlcHdr, tBitStream *pBS)
{
    HI_U32 code, bSAOEnabled, bDBFEnabled;
    int    bits;

    if ( 1 == pSlcHdr->bDBFilterCtlPresent )
    {
        if ( 1 == pSlcHdr->bDBFilterOverrideEnabled )
        {
            VENC_DRV_BsPutBits31(pBS, pSlcHdr->bDFOverrideFlag, 1);        //deblocking_filter_override_flag        u(1)
        }
        if ( 1 == pSlcHdr->bDFOverrideFlag )
        {
            VENC_DRV_BsPutBits31(pBS, pSlcHdr->bSliceHeaderDisableDF, 1);  //slice_disable_deblocking_filter_flag     u(1)
            if( 0 == pSlcHdr->bSliceHeaderDisableDF )
            {
                se_vlc(bits, code, 0);
                VENC_DRV_BsPutBits31(pBS, code, bits);                     //beta_offset_div2        se(v)

                se_vlc(bits, code, 0);
                VENC_DRV_BsPutBits31(pBS, code, bits);                     //tc_offset_div2          se(v)
            }
        }
    }
    bSAOEnabled = (0 == pSlcHdr->bSAOEnabledFlag) ? 0 : (pSlcHdr->bSliceSaoLuma || pSlcHdr->bSliceSaoChroma);
    bDBFEnabled = (0 == pSlcHdr->bSliceHeaderDisableDF) ? 1 : 0;

    if( 1 == bSAOEnabled || 1 == bDBFEnabled ||pSlcHdr->bSliceHeaderDisableDF == 0 )
    {
        VENC_DRV_BsPutBits31(pBS, pSlcHdr->bSliceLFAcrossSlice, 1);        //slice_loop_filter_across_slices_enabled_flag     u(1)
    }

    return;
}

static HI_U32 H265e_MakeSliceHead(VeduEfl_EncPara_S* EncHandle, const VeduEfl_H265e_SlcHdr_S *pSlcHdr)
{
    HI_U32 code, buf[8];
    int    bits, i;
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;

    tBitStream BS;

    // part1
    VENC_DRV_BsOpenBitStream(&BS, buf);

    if( 2 == pSlcHdr->eSliceType )  //H265E_I_SLICE
    {
        VENC_DRV_BsPutBits31(&BS, 0, 1);       //no_output_of_prior_pics_flag = 0    u(1)
    }

    VENC_DRV_BsPutBits31(&BS, 1, 1);           //slice_pic_parameter_set_id=0         ue(v)
    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));
  
    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pEncPara->stHal.slchdr_part1 = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32) { pEncPara->stHal.slchdr_part1 >>= (32 - bits); }
    }
    pEncPara->stHal.slchdr_size_part1 = BS.totalBits;

    // part2
    VENC_DRV_BsOpenBitStream(&BS, buf);

    ue_vlc(bits, code, pSlcHdr->eSliceType);
    VENC_DRV_BsPutBits31(&BS, code, bits);                     //slice_type    ue(v)
  
    if ( 2 != pSlcHdr->eSliceType )
    {
        VENC_DRV_BsPutBits31(&BS, pSlcHdr->uiPocLsb, 16);      //pic_order_cnt_lsb         u(16)
        VENC_DRV_BsPutBits31(&BS, 1, 1);                       //short_term_ref_pic_set_sps_flag=1     u(1)

        if ( pSlcHdr->bSpsTemporalMvpEnableFlag )
        {
            VENC_DRV_BsPutBits31(&BS, 1, 1);                   //enable_temporal_mvp_flag=1        u(1)
        }
    }

    VENC_DRV_BsPutBits31(&BS, pSlcHdr->bSliceSaoLuma, 1);      //slice_sao_luma_flag                  u(1)
    VENC_DRV_BsPutBits31(&BS, pSlcHdr->bSliceSaoChroma, 1);    //slice_sao_chroma_flag                u(1)

    if ( pSlcHdr->eSliceType != 2 )
    {
        VENC_DRV_BsPutBits31(&BS, 1, 1);                       //num_ref_idx_active_override_flag = 1    u(1)

        ue_vlc(bits, code,  pSlcHdr->num_ref_idx_l0_active_minus1);
        VENC_DRV_BsPutBits31(&BS, code, bits);                 //num_ref_idx_l0_active_minus1
        
        if( pSlcHdr->eSliceType == 0)
        {
            VENC_DRV_BsPutBits31(&BS, 1, 1);                   //num_ref_idx_l1_active_minus1
        }

        if( pSlcHdr->eSliceType == 0)
        {
            VENC_DRV_BsPutBits31(&BS, 0, 1);                   //mvd_l1_zero_flag
        }

        VENC_DRV_BsPutBits31(&BS, 0, 1);                       //cabac_init_flag

        if (pSlcHdr->bSpsTemporalMvpEnableFlag)
        {
            if( pSlcHdr->eSliceType == 0)
            {
                VENC_DRV_BsPutBits31(&BS, pSlcHdr->collocated_from_l0_flag, 1);
            }

            if( ((pSlcHdr->collocated_from_l0_flag ==1) &&(pSlcHdr->num_ref_idx_l0_active_minus1>0))
            || ((pSlcHdr->collocated_from_l0_flag ==0)  &&(pSlcHdr->num_ref_idx_l0_active_minus1>0)))
            {
                ue_vlc(bits, code, 0);
                VENC_DRV_BsPutBits31(&BS, code, bits); 
            }
        }

        ue_vlc(bits, code, pSlcHdr->five_minus_max_num_merge_cand);
        VENC_DRV_BsPutBits31(&BS, code, bits);                             //five_minus_max_num_merge_cand=3         ue(v)

    }

    se_vlc(bits, code, pSlcHdr->iSliceQPDelta);
    VENC_DRV_BsPutBits31(&BS, code, bits);                                 //slice_qp_delta     se(v)

    H265e_MakeSliceHead_DBSAO(pEncPara, pSlcHdr, &BS);

    *BS.pBuff++ = (BS.bBigEndian ? BS.tU32b : REV32(BS.tU32b));

    bits = BS.totalBits;

    for (i = 0; bits > 0; i++, bits -= 32)
    {
        pEncPara->stHal.CABAC_SLCHDR_PART2_SEG[i] = BS.bBigEndian ? buf[i] : REV32(buf[i]);
        if (bits < 32) { pEncPara->stHal.CABAC_SLCHDR_PART2_SEG[i] >>= (32 - bits); }
    }
    pEncPara->stHal.slchdr_size_part2 = BS.totalBits;
   
    return (HI_U32)BS.totalBits;
}

static HI_U32 CheckPreventionThreeByte(HI_U8 *pcStream, HI_U32 iNum ,HI_U8 *tempStream)
{
    HI_U32 iInsertBytes  = 0;
    HI_U32 i = 0, j = 0;
    HI_U32 inum = iNum/8;

    if ( inum < 5 )
    {
        return iInsertBytes;
       //return;
    }
    for ( i=0; i<6; i++,j++ )
    {
        tempStream[j]     = pcStream[i];
        
    }
    for ( i=6,j = 6; i<=inum-3; i++ )
    {
        tempStream[j]     = pcStream[i];
        tempStream[j + 1] = pcStream[i + 1];

        if ( ( 0 == pcStream[i] ) && ( 0 == pcStream[i+1] ) )
        {
            if ( ( 0 == pcStream[i+2] ) || ( 1 == pcStream[i+2] ) || ( 2 == pcStream[i+2] ) || ( 3 == pcStream[i+2] ))
            {
                tempStream[j + 2] = 0x03;
                iInsertBytes++;
                

                if ( (i == (inum - 3)) || (i == (inum - 4)) )
                {
                    tempStream[j + 3] = pcStream[i + 2];
                    tempStream[j + 4] = pcStream[i + 3];
                    i += 1;
                }
                else
                {
                    i += 1;
                    j += 3;
                }
            }
            else
            {
                tempStream[j + 2] = pcStream[i + 2];
                j++;
            }
        }
        else
        {
            tempStream[j + 2] = pcStream[i + 2];
            j++;
        }
    }

    return iInsertBytes;
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
static HI_U32 MP4e_MakeVOL(HI_U8 *pVOLBuf, const VeduEfl_MP4e_VOL_S *pVOL)
{
    tBitStream BS;

    VENC_DRV_BsOpenBitStream(&BS, (HI_U32 *)pVOLBuf);

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

#ifdef __VENC_SUPPORT_JPGE__
static HI_VOID Jpge_MakeQTable(HI_S32 s32Q, HI_U8* pLumaQt, HI_U8 *pChromaQt)
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
    HI_U32 w = pEncPara->stRc.PicWidth, t, i;
    HI_U32 h = pEncPara->stRc.PicHeight;
    VeduEfl_NaluHdr_S NaluHdr;
    HI_U32 JfifHdrLen = pEncPara->stHal.SlcSplitEn ? 698 : 698 - 6;
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

    if (pEncPara->stHal.SlcSplitEn)
    {
        dri[4] = pEncPara->stHal.SplitSize >> 8;
        dri[5] = pEncPara->stHal.SplitSize & 0xFF;

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
        NaluHdr.PTS0       = pEncPara->stHal.PTS0;
        NaluHdr.PTS1       = pEncPara->stHal.PTS1;

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
    HI_U32 JfifHdrLen = pEncPara->stHal.SlcSplitEn ? 698 : 698 - 6;
    VALG_CRCL_BUF_S *pstStrBuf = &pEncPara->stCycBuf;
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
#if ( defined(__VENC_98MV200_CONFIG__) )

static HI_S32 VeduEfl_PME_SetDefaultAttr  (VeduEfl_PmeAttr_S *pPmeAttr )
{
  pPmeAttr->l0_psw_adapt_en = 1 ; 
  pPmeAttr->l0_psw_thr0     = 20 ; 
  pPmeAttr->l0_psw_thr1     = 50 ; 
  pPmeAttr->l0_psw_thr2     = 150 ; 
  pPmeAttr->tr_weightx_2    = 12 ;   
  pPmeAttr->tr_weightx_1    = 8 ;   
  pPmeAttr->tr_weightx_0    = 4 ;   
  pPmeAttr->tr_weighty_2    = 12 ;   
  pPmeAttr->tr_weighty_1    = 8 ;   
  pPmeAttr->tr_weighty_0    = 4 ;   
  pPmeAttr->sr_weight_2     = 2 ;    
  pPmeAttr->sr_weight_1     = 4 ;    
  pPmeAttr->sr_weight_0     = 6 ;
  pPmeAttr->skin_num        = 0xc8 ;  
  pPmeAttr->skin_v_min_thr  = 0x87 ; 
  pPmeAttr->skin_v_max_thr  = 0xa0 ;
  pPmeAttr->skin_u_min_thr  = 0x64 ; 
  pPmeAttr->skin_u_max_thr  = 0x7f ;
  pPmeAttr->still_scene_thr = 0 ;   
  pPmeAttr->high_edge_cnt   = 0x6 ;   
  pPmeAttr->high_edge_thr   = 0x28 ;   

  pPmeAttr->move_sad_thr   = 0 ;
  pPmeAttr->move_scene_thr = 0 ;
  pPmeAttr->interstrongedge_madi_thr     = 0 ;
  pPmeAttr->interdiff_max_min_madi_times = 0 ;
  pPmeAttr->interdiff_max_min_madi_abs   = 0 ;

  pPmeAttr->cost_lamda_en = 0 ;
  pPmeAttr->new_cost_en   = 0 ;
  pPmeAttr->cost_lamda1   = 0 ;
  pPmeAttr->cost_lamda0   = 0 ;
  
  pPmeAttr->l0_win0_width  = 0x8;
  pPmeAttr->l0_win0_height = 0x8;
  pPmeAttr->l0_win1_width  = 0x8;
  pPmeAttr->l0_win1_height = 0x8;
  pPmeAttr->l0_win2_width  = 16;
  pPmeAttr->l0_win2_height = 16;
  pPmeAttr->l0_win3_width  = 24;
  pPmeAttr->l0_win3_height = 24;

  pPmeAttr->l0_cost_offset = 0 ;
  
  pPmeAttr->pme_safe_line       = 0;
  pPmeAttr->pme_safe_line_val   = 0;

  pPmeAttr->pme_iblk_refresh_start_num  = 0;
  pPmeAttr->pme_iblk_refresh_num        = 0;
  pPmeAttr->pme_high_luma_thr           = 0;
  pPmeAttr->pme_inter_first             = 0;

  pPmeAttr->low_luma_thr        = 0;
  pPmeAttr->low_luma_madi_thr   = 0;
  pPmeAttr->madi_dif_thr        = 0;
  pPmeAttr->cur_madi_dif_thr    = 0;
  pPmeAttr->smooth_madi_thr     = 0;
  pPmeAttr->min_sad_thr_gain    = 0;
  pPmeAttr->min_sad_thr_offset  = 0;

  pPmeAttr->pme_intra_lowpow_en  = 1;
  pPmeAttr->pme_intra32_madi_thr = 20;
  pPmeAttr->pme_intra16_madi_thr = 10;
  pPmeAttr->pme_iblk_pre_cost_thr_h264 = 767;

  return HI_SUCCESS;
}

static HI_S32 VeduEfl_QPG_SetDefaultAttr  (VeduEfl_QpgAttr_S *pQpgAttr )
{
    int i; 

    pQpgAttr->qp_delta = 2 ;    
    pQpgAttr->cu_qp_delta_thresh[0]  = 0x2 ;   
    pQpgAttr->cu_qp_delta_thresh[1]  = 0x2 ;   
    pQpgAttr->cu_qp_delta_thresh[2]  = 0x6 ;   
    pQpgAttr->cu_qp_delta_thresh[3]  = 0x6 ;   
    pQpgAttr->cu_qp_delta_thresh[4]  = 0xa ;   
    pQpgAttr->cu_qp_delta_thresh[5]  = 0xa ;   
    pQpgAttr->cu_qp_delta_thresh[6]  = 0xff;   
    pQpgAttr->cu_qp_delta_thresh[7]  = 0xff;   

    pQpgAttr->cu_qp_delta_thresh[8]  = 0xff;   
    pQpgAttr->cu_qp_delta_thresh[9]  = 0xff;   
    pQpgAttr->cu_qp_delta_thresh[10] = 0xff; 
    pQpgAttr->cu_qp_delta_thresh[11] = 0xff; 
    pQpgAttr->cu_qp_delta_thresh[12] = 0xff;
    pQpgAttr->cu_qp_delta_thresh[13] = 0xff;   
    pQpgAttr->cu_qp_delta_thresh[14] = 0xff; 
    pQpgAttr->cu_qp_delta_thresh[15] = 0xff; 

    for(i = 0; i < 16; i++)
    {
        pQpgAttr->qp_delta_level[i] = 1;
    }

    pQpgAttr->qp_madi_switch_thr  = 0x8;

    pQpgAttr->strong_edge_max_qp = 0x33;
    pQpgAttr->large_move_max_qp  = 0x33; 
    pQpgAttr->move_edge_max_qp   = 0x33;   
    pQpgAttr->skin_max_qp        = 0x33;
    pQpgAttr->intra_det_max_qp   = 0x33;

    pQpgAttr->strong_edge_move_qp_delta = 3; 
    pQpgAttr->large_move_qp_delta       = 3; 
    pQpgAttr->strong_edge_qp_delta      = 3; 
    pQpgAttr->skin_qp_delta             = 3;   
    pQpgAttr->intra_det_qp_delta        = 3;

    pQpgAttr->h264_smooth_qp_delta  = 4;
    pQpgAttr->h264_smooth_qp_delta1 = 8;
    pQpgAttr->h264_smooth_max_qp    = 51;  
    pQpgAttr->cu32_delta_low        = 0 ; 
    pQpgAttr->cu32_delta_high       = 0 ; 

    pQpgAttr->rc_cu_qp_en  = 1 ; 
    pQpgAttr->rc_row_qp_en = 1 ;

    pQpgAttr->rc_cu_madi_en       = 1 ;
    pQpgAttr->move_scene_en       = 0 ;
    pQpgAttr->strong_edge_move_en = 0 ; 
    pQpgAttr->intra_det_qp_en     = 0 ;
    pQpgAttr->rd_smooth_mb_en     = 0;
    pQpgAttr->rd_min_sad_flag_en  = 0;
    pQpgAttr->wr_min_sad_flag_en  = 0;
    pQpgAttr->prev_min_sad_en     = 0;

    return HI_SUCCESS;
}

static HI_S32 VeduEfl_IMGIMPROVE_SetDefaultAttr  (VeduEfl_ImgImproveAttr_S *pImgImproveAttr )
{
    pImgImproveAttr->highedge_en    = 0;
    pImgImproveAttr->skin_en        = 0;
    pImgImproveAttr->static_en      = 0;
    pImgImproveAttr->img_improve_en = 0;

    pImgImproveAttr->norm32_tr1_denois_max_num      = 0 ;
    pImgImproveAttr->norm32_coeff_protect_num       = 0 ;
    pImgImproveAttr->norm16_tr1_denois_max_num      = 0 ;
    pImgImproveAttr->norm16_coeff_protect_num       = 0 ;
    pImgImproveAttr->skin32_tr1_denois_max_num      = 0 ;
    pImgImproveAttr->skin32_coeff_protect_num       = 0 ;
    pImgImproveAttr->skin16_tr1_denois_max_num      = 0 ;
    pImgImproveAttr->skin16_coeff_protect_num       = 0 ;
    pImgImproveAttr->static32_tr1_denois_max_num    = 0 ;
    pImgImproveAttr->static32_coeff_protect_num     = 0 ;
    pImgImproveAttr->static16_tr1_denois_max_num    = 0 ;
    pImgImproveAttr->static16_coeff_protect_num     = 0 ;
    pImgImproveAttr->hedge32_tr1_denois_max_num     = 0 ;
    pImgImproveAttr->hedge32_coeff_protect_num      = 0 ;
    pImgImproveAttr->hedge16_tr1_denois_max_num     = 0 ;
    pImgImproveAttr->hedge16_coeff_protect_num      = 0 ;
    pImgImproveAttr->norm_intra_cu32_rdcost_offset  = 0 ;
    pImgImproveAttr->norm_intra_cu16_rdcost_offset  = 0 ;
    pImgImproveAttr->norm_intra_cu8_rdcost_offset   = 0 ;
    pImgImproveAttr->norm_intra_cu4_rdcost_offset   = 0 ;
    pImgImproveAttr->norm_mrg_cu64_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_mrg_cu32_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_mrg_cu16_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_mrg_cu8_rdcost_offset     = 0 ;
    pImgImproveAttr->norm_fme_cu64_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_fme_cu32_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_fme_cu16_rdcost_offset    = 0 ;
    pImgImproveAttr->norm_fme_cu8_rdcost_offset     = 0 ;
    pImgImproveAttr->skin_intra_cu32_rdcost_offset  = 0 ;
    pImgImproveAttr->skin_intra_cu16_rdcost_offset  = 0 ;
    pImgImproveAttr->skin_intra_cu8_rdcost_offset   = 0 ;
    pImgImproveAttr->skin_intra_cu4_rdcost_offset   = 0 ;
    pImgImproveAttr->skin_mrg_cu64_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_mrg_cu32_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_mrg_cu16_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_mrg_cu8_rdcost_offset     = 0 ;
    pImgImproveAttr->skin_fme_cu64_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_fme_cu32_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_fme_cu16_rdcost_offset    = 0 ;
    pImgImproveAttr->skin_fme_cu8_rdcost_offset     = 0 ;
    pImgImproveAttr->hedge_intra_cu32_rdcost_offset = 0 ; 
    pImgImproveAttr->hedge_intra_cu16_rdcost_offset = 0 ; 
    pImgImproveAttr->hedge_intra_cu8_rdcost_offset  = 0 ;   
    pImgImproveAttr->hedge_intra_cu4_rdcost_offset  = 0 ;   
    pImgImproveAttr->hedge_mrg_cu64_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_mrg_cu32_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_mrg_cu16_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_mrg_cu8_rdcost_offset    = 0 ;    
    pImgImproveAttr->hedge_fme_cu64_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_fme_cu32_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_fme_cu16_rdcost_offset   = 0 ;  
    pImgImproveAttr->hedge_fme_cu8_rdcost_offset    = 0 ;   
    pImgImproveAttr->roundingMechanism              = 0 ;
    pImgImproveAttr->roundingDegreeThresh           = 0 ;
    pImgImproveAttr->roundingForceZeroResidThresh   = 0 ;
    pImgImproveAttr->roundingAC32sum                = 0 ;
    pImgImproveAttr->roundingAC16sum                = 0 ;
    pImgImproveAttr->roundingLowFreqACBlk32         = 0 ;
    pImgImproveAttr->roundingLowFreqACBlk16         = 0 ;

    pImgImproveAttr->roundingSkinMechanism              = 0 ;
    pImgImproveAttr->roundingSkinDegreeThresh           = 0 ;
    pImgImproveAttr->roundingSkinForceZeroResidThresh   = 0 ;
    pImgImproveAttr->roundingSkinAC32sum                = 0 ;
    pImgImproveAttr->roundingSkinAC16sum                = 0 ;
    pImgImproveAttr->roundingSkinLowFreqACBlk32         = 0 ;
    pImgImproveAttr->roundingSkinLowFreqACBlk16         = 0 ; 

    pImgImproveAttr->roundingStillMechanism             = 0 ;
    pImgImproveAttr->roundingStillDegreeThresh          = 0 ;
    pImgImproveAttr->roundingStillForceZeroResidThresh  = 0 ;
    pImgImproveAttr->roundingStillAC32sum               = 0 ;
    pImgImproveAttr->roundingStillAC16sum               = 0 ;
    pImgImproveAttr->roundingStillLowFreqACBlk32        = 0 ;
    pImgImproveAttr->roundingStillLowFreqACBlk16        = 0 ; 

    pImgImproveAttr->roundingEdgeMechanism              = 0 ;
    pImgImproveAttr->roundingEdgeDegreeThresh           = 0 ;
    pImgImproveAttr->roundingEdgeForceZeroResidThresh   = 0 ;
    pImgImproveAttr->roundingEdgeAC32sum                = 0 ;
    pImgImproveAttr->roundingEdgeAC16sum                = 0 ;
    pImgImproveAttr->roundingEdgeLowFreqACBlk32         = 0 ;
    pImgImproveAttr->roundingEdgeLowFreqACBlk16         = 0 ; 

    pImgImproveAttr->norm_sel_cu8_rd_offset     = 0 ;
    pImgImproveAttr->skin_sel_cu8_rd_offset     = 0 ;
    pImgImproveAttr->hedge_sel_cu8_rd_offset    = 0 ;
    pImgImproveAttr->strmov_sel_cu8_rd_offset   = 0 ;
    pImgImproveAttr->sobelstr_sel_cu8_rd_offset = 0 ;
    pImgImproveAttr->sobeltex_sel_cu8_rd_offset = 0 ;

    pImgImproveAttr->skin_inter_cu_rdcost_offset  = 0 ;
    pImgImproveAttr->hedge_inter_cu_rdcost_offset = 0 ;
    pImgImproveAttr->skin_layer_rdcost_offset     = 0 ;

    pImgImproveAttr->sobelstr_inter_cu_rdcost_offset = 0 ;
    pImgImproveAttr->sobeltex_inter_cu_rdcost_offset = 0 ;
    pImgImproveAttr->hedge_layer_rdcost_offset       = 0 ;

    pImgImproveAttr->strmov_inter_cu_rdcost_offset  = 0 ;
    pImgImproveAttr->norm_inter_cu_rdcost_offset    = 0 ;
    pImgImproveAttr->strmov_layer_rdcost_offset     = 0 ;

    pImgImproveAttr->sobelstr_layer_rdcost_offset   = 0 ;
    pImgImproveAttr->sobeltex_layer_rdcost_offset   = 0 ;
    pImgImproveAttr->norm_layer_rdcost_offset       = 0 ;

    pImgImproveAttr->sel_intra4_8_abs_offset    = 0 ;
    pImgImproveAttr->sel_intra16_abs_offset     = 0 ;
    pImgImproveAttr->sel_inter8_16_abs_offset   = 0 ;
    pImgImproveAttr->sel_inter32_abs_offset     = 0 ;
    pImgImproveAttr->sel_layer32_abs_offset     = 0 ;
  
    return HI_SUCCESS;
}
#endif

static void Venc_SetRegDefault( VeduEfl_EncPara_S  *pEncPara )
{
    int  i;

    pEncPara->stHal.slchdr_part1 = 0;

    for(i = 0;i< 8;i++)
    {
        pEncPara->stHal.CABAC_SLCHDR_PART2_SEG[i] = 0;
    }
    
    VeduEfl_IMGIMPROVE_SetDefaultAttr(&(pEncPara->stHal.ImgImproveCfg));
    VeduEfl_QPG_SetDefaultAttr       (&(pEncPara->stHal.QpgCfg));
    VeduEfl_PME_SetDefaultAttr       (&(pEncPara->stHal.PmeCfg));

    if(VEDU_H264 == pEncPara->Protocol)
    {
        for (i =  0; i < 40; i++)    { pEncPara->stHal.QpgCfg.lambda[i] = g_h264LambdaSsd[i   ]; }
        for (i = 40; i < 80; i++)    { pEncPara->stHal.QpgCfg.lambda[i] = g_h264LambdaSad[i-40]; }
    }
    else
    {
        for (i =  0; i < 40; i++)    { pEncPara->stHal.QpgCfg.lambda[i] = g_h265LambdaSsd[i   ]; }
        for (i = 40; i < 80; i++)    { pEncPara->stHal.QpgCfg.lambda[i] = g_h265LambdaSad[i-40]; }
    }

    pEncPara->ChrQpOffset      = 0;
    pEncPara->ConstIntra       = 0;
    pEncPara->CabacInitIdc     = 0;
    pEncPara->CabacStuffEn     = 0;    //cabac 字节填充使能信号 don't support 1

    pEncPara->DblkIdc          = 0;
    pEncPara->DblkAlpha        = 0;
    pEncPara->DblkBeta         = 0;

    pEncPara->NumRefIndex      = 0;

    pEncPara->stHal.wOutStdNum       = 15;
    pEncPara->stHal.rOutStdNum       = 15;

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
    pEncPara->stHal.glb_bypass = 0;  
    pEncPara->stHal.int_en        = 1;    // smmu中断使能
    
    pEncPara->stHal.vcpi_srcv_bypass = 0;
    pEncPara->stHal.vcpi_srcy_bypass = 0; 
    pEncPara->stHal.vcpi_refc_bypass = 0;
    pEncPara->stHal.vcpi_refy_bypass = 0; 
    pEncPara->vcpi_srcc_bypass = 0; 

    pEncPara->stHal.vcpi_strm_bypass = 0;
    pEncPara->vcpi_rcny_bypass = 0; 
    pEncPara->vcpi_rcnc_bypass = 0; 
     

////added by c00346155 
    pEncPara->stHal.vcpi_srcu_bypass = 0;          
    pEncPara->stHal.vcpi_pmeinfold1_bypass = 0;
    pEncPara->stHal.vcpi_pmeinfold0_bypass = 0;
    pEncPara->stHal.vcpi_refch_bypass        = 0;
    pEncPara->stHal.vcpi_refyh_bypass        = 0;
    pEncPara->stHal.vcpi_pmeld_bypass       = 0;
    pEncPara->stHal.vcpi_nbild_bypass         = 0;

    pEncPara->stHal.vcpi_pmeinfost_bypass  = 0;
    pEncPara->stHal.vcpi_recc_bypass         = 0;
    pEncPara->stHal.vcpi_recy_bypass         = 0;
    pEncPara->stHal.vcpi_recch_bypass        = 0;
    pEncPara->stHal.vcpi_recyh_bypass        = 0;
    pEncPara->stHal.vcpi_pmest_bypass        = 0;
    pEncPara->stHal.vcpi_nbist_bypass          = 0;
////add end

#else
    pEncPara->stHal.glb_bypass = 1;  
    pEncPara->stHal.int_en        = 0;    // smmu中断使能

    pEncPara->stHal.vcpi_srcy_bypass = 1; 
    pEncPara->vcpi_srcc_bypass = 1; 
    pEncPara->stHal.vcpi_srcv_bypass = 1; 
    pEncPara->stHal.vcpi_refy_bypass = 1; 
    pEncPara->stHal.vcpi_refc_bypass = 1; 
    pEncPara->vcpi_rcny_bypass = 1; 
    pEncPara->vcpi_rcnc_bypass = 1; 
    pEncPara->stHal.vcpi_strm_bypass = 1; 
#endif
    
    pEncPara->RegLockEn        = 0;
    pEncPara->stHal.ClkGateEn        = 2;  /*open the frame level and MB level ClkGate*/
    pEncPara->stHal.MemClkGateEn     = 1;
    pEncPara->stHal.TimeOutEn        = 3;
    pEncPara->stHal.TimeOut          = 20000000;
    pEncPara->stHal.PtBitsEn         = 1;
    pEncPara->stHal.PtBits           = 500000*8;

    pEncPara->IMbNum           = 0;
    pEncPara->StartMb          = 0;

    /* add for VBR */
    pEncPara->MaxIPdelta       = 3;
    pEncPara->MaxPPdelta       = 1; 

    for(i = 0; i < 8; i++)
    {
        pEncPara->stHal.RoiCfg.Enable [i] = 0;
        pEncPara->stHal.RoiCfg.Keep   [i] = 0;        
        pEncPara->stHal.RoiCfg.AbsQpEn[i] = 1;
        pEncPara->stHal.RoiCfg.Qp     [i] = 26;
        pEncPara->stHal.RoiCfg.Width  [i] = 7;
        pEncPara->stHal.RoiCfg.Height [i] = 7;
        pEncPara->stHal.RoiCfg.StartX [i] = i*5;
        pEncPara->stHal.RoiCfg.StartY [i] = i*5;
    }
}

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
    VeduEfl_EncPara_S *pEncPara;
	HI_U32 u32VeChn;
    pEncPara = EncHandle;
	
    D_VENC_GET_CHN(u32VeChn, EncHandle);
	D_VENC_CHECK_CHN(u32VeChn);

    pEncPara->stStat.GetFrameNumTry++;
    if( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(GetImgHhd, &pEncPara->stImage))
    {
        pEncPara->stStat.GetFrameNumOK++;
		if (g_stVencChn[u32VeChn].enSrcModId >= HI_ID_BUTT)   /* venc don't attach src*/
		{
		    pEncPara->stStat.VpssImgBufNum++;
		}
        while(!bLastFrame)
        {
           pEncPara->stStat.GetFrameNumTry++;
           if( HI_SUCCESS == (pEncPara->stSrcInfo.pfGetImage)(GetImgHhd, &stImage_temp))
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

HI_S32 VENC_DRV_EflToVPSSGetImge(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImageInfo)   //给VPSS的回调,主动取帧
{
    HI_S32 Ret = HI_FAILURE;
    VeduEfl_EncPara_S*  pEncPara = NULL;
    
    D_VENC_GET_CHN_FORM_VPSS(hVPSS, pEncPara);
    if(NULL == pEncPara)
        return HI_FAILURE;
    if (pEncPara->stSrcInfo_toVPSS.pfGetImage)
	{
	    Ret =(pEncPara->stSrcInfo_toVPSS.pfGetImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
	}
    return Ret;
}

HI_S32 VENC_DRV_EflToVPSSRelImge(VPSS_HANDLE hVPSS,HI_DRV_VIDEO_FRAME_S *pstImageInfo)   //给VPSS的回调，主动释放帧
{
    HI_S32 Ret = HI_FAILURE;
    
    VeduEfl_EncPara_S*  pEncPara = NULL;
    D_VENC_GET_CHN_FORM_VPSS(hVPSS, pEncPara);
    if(NULL == pEncPara)
        return HI_FAILURE;
	if (pEncPara->stSrcInfo_toVPSS.pfPutImage)
	{
       Ret =(pEncPara->stSrcInfo_toVPSS.pfPutImage)(pEncPara->stSrcInfo_toVPSS.handle, pstImageInfo);
	}
    return Ret;
}

HI_BOOL VENC_DRV_EflJudgeVPSS( VeduEfl_EncPara_S* EncHandle, HI_DRV_VIDEO_FRAME_S* pstFrameInfo)   //调用在获得帧信息后
{
    HI_S32 Ret;
    HI_U32 u32ChnID;
    //HI_BOOL flag = HI_FALSE;
    VeduEfl_EncPara_S *pEncPara;
    
    D_VENC_GET_CHN(u32ChnID,EncHandle);
	D_VENC_CHECK_CHN(u32ChnID);

    pEncPara = EncHandle;

#if 0
    flag |= (pEncPara->PicHeight != D_VENC_ALIGN_UP(pstFrameInfo->u32Height , 16) );
    flag |= (pEncPara->PicWidth  != D_VENC_ALIGN_UP(pstFrameInfo->u32Width , 16)  );
    flag |= (pstFrameInfo->ePixFormat !=HI_DRV_PIX_FMT_NV21 )&&(pstFrameInfo->ePixFormat != HI_DRV_PIX_FMT_NV12);   /*目前只支持的源格式*/

	flag = 1;   //全过VPSS
    if (HI_TRUE == flag)  /* need VPSS to Process the Frame from now on*/
    {
        g_stVencChn[u32ChnID].bNeedVPSS = HI_TRUE;    
        pEncPara->stSrcInfo.handle       = (HI_HANDLE)g_stVencChn[u32ChnID].hPort[0];
        pEncPara->stSrcInfo.pfGetImage   = pVpssFunc->pfnVpssGetPortFrame;
        pEncPara->stSrcInfo.pfPutImage   = pVpssFunc->pfnVpssRelPortFrame;
        Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS,VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
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
    Ret = (pVpssFunc->pfnVpssSetSourceMode)(g_stVencChn[u32ChnID].hVPSS,VPSS_SOURCE_MODE_USERACTIVE, HI_NULL);
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
   HI_U32 i,j;
   for( i = 0; i < MAX_VEDU_CHN - 1; i++)
   {
      for(j =  MAX_VEDU_CHN - 1; j > i; j--)
      {
          if(PriorityTab[1][j]>PriorityTab[1][j-1])
          {
             HI_U32 temp0 = PriorityTab[0][j];
             HI_U32 temp1 = PriorityTab[1][j];
             PriorityTab[0][j]   = PriorityTab[0][j-1];
             PriorityTab[1][j]   = PriorityTab[1][j-1];
             PriorityTab[0][j-1] = temp0;
             PriorityTab[1][j-1] = temp1;
          }
      }
   }
}

HI_S32 VENC_DRV_EflSortPriority_2(HI_S8 priority)
{
   HI_U32 i;
   HI_U32 cnt = 0;
   HI_U32 id  = MAX_VEDU_CHN-1;
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
      HI_WARN_VENC("can't fine the channel match with priority(%d)\n",priority);
      return HI_FAILURE;
   }

   if (1 == cnt || (id>=MAX_VEDU_CHN-1))
   {
      return HI_SUCCESS;
   }
   
   for(i = 0; (i<(cnt-1)) && (id<(MAX_VEDU_CHN-1)); i++,id++)
   {
       HI_U32 temp0 = PriorityTab[0][id];
       HI_U32 temp1 = PriorityTab[1][id];
       PriorityTab[0][id]   = PriorityTab[0][id+1];
       PriorityTab[1][id]   = PriorityTab[1][id+1];
       PriorityTab[0][id+1] = temp0;
       PriorityTab[1][id+1] = temp1;     
   }

   return HI_SUCCESS;
}


HI_S32 VENC_DRV_EflCfgRegVenc( VeduEfl_EncPara_S* EncHandle )
{
   //open the clock before start the channel!    l00228308
    VENC_DRV_BoardInit();
   //add end
    EncHandle->stHal.Protocol = EncHandle->Protocol;
    if(EncHandle->RegConfigMode)
    {
        VENC_HAL_DDRCfgReg(&EncHandle->stHal, &EncHandle->stRc);
    }
    else
    {
        VENC_HAL_CfgReg(&EncHandle->stHal, &EncHandle->stRc);
    }
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflReadRegVenc( VeduEfl_EncPara_S* EncHandle )
{
    VENC_HAL_ReadReg( &EncHandle->stHal, &EncHandle->stRc, &EncHandle->stStat);

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflStartOneFrameVenc(VeduEfl_EncPara_S *EncHandle, VeduEfl_EncIn_S *pEncIn )
{
    VeduEfl_EncPara_S *pEncPara  = EncHandle;
    VALG_CRCL_BUF_S *pstStrBuf = &pEncPara->stCycBuf;

    VENC_DRV_RcOpenOneFrm(&pEncPara->stRc);

    /* Make slice header bitstream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SlcHdr_S SlcHdr;

        pEncPara->stRc.H264FrmNum = pEncPara->stRc.IsIntraPic ? 0 : pEncPara->stRc.H264FrmNum  + 1;
        SlcHdr.frame_num  = pEncPara->stRc.H264FrmNum ;
        SlcHdr.slice_type = pEncPara->stRc.IsIntraPic ? 2 : 0;
        SlcHdr.NumRefIndex= pEncPara->NumRefIndex;
        pEncPara->stHal.SlcHdrBits = H264e_MakeSlcHdr(pEncPara->stHal.SlcHdrStream,
                                                pEncPara->stHal.ReorderStream,
                                                pEncPara->stHal.MarkingStream, &SlcHdr);
    }
    else if(pEncPara->Protocol == VEDU_H265)
    {
        VeduEfl_H265e_SlcHdr_S  SlcHdr;
        
        pEncPara->stRc.H264FrmNum  = pEncPara->stRc.IsIntraPic ? 0 : pEncPara->stRc.H264FrmNum  + 1;
        SlcHdr.frame_num     = pEncPara->stRc.H264FrmNum ;
        SlcHdr.eSliceType    = pEncPara->stRc.IsIntraPic ? 2 : 1;
        
        SlcHdr.uiPocLsb = SlcHdr.frame_num ;

        SlcHdr.bSpsTemporalMvpEnableFlag    = pEncPara->stHal.BaseCfg.tmv_en;
        SlcHdr.bSAOEnabledFlag              = pEncPara->stHal.BaseCfg.vcpi_sao_luma || pEncPara->stHal.BaseCfg.vcpi_sao_chroma;
        SlcHdr.bSliceSaoLuma                = pEncPara->stHal.BaseCfg.vcpi_sao_luma;
        SlcHdr.bSliceSaoChroma              = pEncPara->stHal.BaseCfg.vcpi_sao_chroma;
        
        SlcHdr.iSliceQPDelta = pEncPara->stRc.StartQP - 26;
        SlcHdr.bDBFilterCtlPresent = 1;
        SlcHdr.bDBFilterOverrideEnabled = 1;
        
        SlcHdr.bFirstSliceInPic = 1;
        SlcHdr.bSliceHeaderDisableDF = 1;

        SlcHdr.bSliceHeaderDisableDF = 0;
        SlcHdr.bDFOverrideFlag = 1;

        SlcHdr.bSliceLFAcrossSlice = 1;        

        SlcHdr.five_minus_max_num_merge_cand = 5 - pEncPara->stHal.BaseCfg.max_num_mergecand;
        SlcHdr.num_ref_idx_l0_active_minus1  = 0;
        SlcHdr.collocated_from_l0_flag       = 1;
        
        pEncPara->stHal.SlcHdrBits = H265e_MakeSliceHead(EncHandle,&SlcHdr);
    }

    pEncPara->stHal.SrcYAddr   = pEncIn->BusViY;
    pEncPara->stHal.SrcCAddr   = pEncIn->BusViC;
    pEncPara->stHal.SrcVAddr   = pEncIn->BusViV;
    pEncPara->stHal.SStrideY = pEncIn->ViYStride;
    pEncPara->stHal.SStrideC = pEncIn->ViCStride;

    if (pEncPara->stRc.LowDlyMod)
    {
        pEncPara->stHal.tunlcellAddr = pEncIn->TunlCellAddr;

        if (pEncPara->stRc.PicWidth >= 1920)  //D1
        {
            pEncPara->stHal.tunlReadIntvl = 3;
        }
        else if (pEncPara->stRc.PicWidth >= 720)
        {
            pEncPara->stHal.tunlReadIntvl = 2;
        }
        else
        {
            pEncPara->stHal.tunlReadIntvl = 1;
        }
    }

    pEncPara->stHal.PTS0 = pEncIn->PTS0;
    pEncPara->stHal.PTS1 = pEncIn->PTS1;

    if (pEncPara->Protocol == VEDU_H265)
    {
        // pmv_poc
        if (pEncPara->stRc.H264FrmNum % pEncPara->stRc.Gop == 0)
        {
            pEncPara->stHal.BaseCfg.pmv_poc[0] = 0;
            pEncPara->stHal.BaseCfg.pmv_poc[1] = 0;
        }
        else
        {
            pEncPara->stHal.BaseCfg.pmv_poc[0] = pEncPara->stRc.H264FrmNum;
            pEncPara->stHal.BaseCfg.pmv_poc[1] = pEncPara->stRc.H264FrmNum - 1;
        }

        pEncPara->stHal.BaseCfg.pmv_poc[3] = pEncPara->stHal.BaseCfg.pmv_poc[1] ;

        if (pEncPara->stRc.H264FrmNum % pEncPara->stRc.Gop > 1)
        {
            pEncPara->stHal.BaseCfg.pmv_poc[4] = pEncPara->stHal.BaseCfg.pmv_poc[3] - 1;
        }
    }

    pEncPara->stRc.PmeInfoIdx = (pEncPara->stRc.PmeInfoIdx + 1) % 3;
    pEncPara->stRc.RcnIdx = !pEncPara->stRc.RcnIdx;
    pEncPara->stHal.RStrideY = pEncIn->RStrideY;
    pEncPara->stHal.RStrideC = pEncIn->RStrideC;

#if ( defined(__VENC_98CV200_CONFIG__) || defined(__VENC_98MV200_CONFIG__) )  
#ifdef HI_SMMU_SUPPORT
    pEncPara->stHal.vcpi_srcy_bypass = 0; 
    pEncPara->vcpi_srcc_bypass = 0; 
    pEncPara->stHal.vcpi_srcv_bypass = 0;  
#else		
    pEncPara->stHal.vcpi_srcy_bypass = 1; 
    pEncPara->vcpi_srcc_bypass = 1; 
    pEncPara->stHal.vcpi_srcv_bypass = 1; 
#endif
#endif

    if ((pEncPara->stRc.IsIntraPic ) && (!pEncPara->stRc.Recode)) //当前帧进行重编无需再编码头信息
    {
        if ((pEncPara->PrependSpsPpsEnable == HI_TRUE) || (pEncPara->stStat.GetFrameNumOK == 1))
        {
            VeduEfl_NaluHdr_S NaluHdr;
    
            if(pEncPara->Protocol != VEDU_H265)
            {
                NaluHdr.PacketLen  = 128;
                NaluHdr.InvldByte  = 64 - pEncPara->SpsBits / 8;
                NaluHdr.bLastSlice = 0;
                NaluHdr.Type = 7;
        
                NaluHdr.PTS0 = pEncPara->stHal.PTS0;
                NaluHdr.PTS1 = pEncPara->stHal.PTS1;
        
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
            }
            else
            {
                // vps
                NaluHdr.PacketLen  = 128;
                NaluHdr.InvldByte  = 64 - pEncPara->VpsBits / 8;
                NaluHdr.bLastSlice = 0;
                NaluHdr.Type       = 32;
                NaluHdr.bBotField  = 0;
                NaluHdr.bField     = 0;

                NaluHdr.PTS0  = pEncPara->stHal.PTS0;
                NaluHdr.PTS1  = pEncPara->stHal.PTS1;

                VENC_DRV_BufWrite( pstStrBuf, &NaluHdr           , 64 );
                VENC_DRV_BufWrite( pstStrBuf, pEncPara->VpsStream, 64 );

                // sps
                NaluHdr.InvldByte  = 64 - pEncPara->SpsBits / 8;
                NaluHdr.Type       = 33;

                VENC_DRV_BufWrite( pstStrBuf, &NaluHdr           , 64 );
                VENC_DRV_BufWrite( pstStrBuf, pEncPara->SpsStream, 64);

                // pps
                NaluHdr.InvldByte = 64 - pEncPara->PpsBits / 8;
                NaluHdr.Type      = 34;

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

HI_S32 VENC_DRV_EflEndOneFrameVenc(VeduEfl_EncPara_S *EncHandle)
{
    VeduEfl_EncPara_S *pEncPara  = EncHandle;
    VALG_CRCL_BUF_S *pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    if (!pEncPara->stRc.LowDlyMod)  // not use low delay mode
    {
        if (!pEncPara->stHal.VencBufFull && !pEncPara->stHal.VencPbitOverflow)
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

    pEncPara->stRc.VencBufFull = pEncPara->stHal.VencBufFull;
    pEncPara->stRc.VencPbitOverflow = pEncPara->stHal.VencPbitOverflow;
    VENC_DRV_RcCloseOneFrm(&pEncPara->stRc, &pEncPara->stBitsFifo, &pEncPara->TrCount, &pEncPara->InterFrmCnt);

    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflEndOneFrameJpge( VeduEfl_EncPara_S *EncHandle)
{
    VeduEfl_EncPara_S *pEncPara  = EncHandle;
    VALG_CRCL_BUF_S *pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S*)EncHandle)->pStrmBufLock, &flag);

    if (!pEncPara->stHal.VencBufFull && !pEncPara->stHal.VencPbitOverflow)
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
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;
    HI_U32 wrptr;
    VEDU_LOCK_FLAG flag;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    //if (!pEncPara->VencBufFull || !pEncPara->VencPbitOverflow)
    //{
        /* Read Wp which be changed by HW */
    wrptr = *(pEncPara->StrmBufWpVirAddr);

        VENC_DRV_BufWrite   ( pstStrBuf, NULL, wrptr );
        VENC_DRV_BufUpdateWp( pstStrBuf );
    //}

    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

    //VENC_DRV_RcCloseOneFrm( EncHandle );

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
static HI_S32 VENC_DRV_EflChkChnCfg( VeduEfl_EncCfg_S *pEncCfg )
{
    HI_S32 CfgErr = 0;

    CfgErr |= (pEncCfg->FrameWidth < VEDU_MIN_ENC_WIDTH) | (pEncCfg->FrameWidth > VEDU_MAX_ENC_WIDTH);
    CfgErr |= (pEncCfg->FrameHeight < VEDU_MIN_ENC_HEIGHT) | (pEncCfg->FrameHeight > VEDU_MAX_ENC_HEIGHT);
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
HI_S32 VENC_DRV_EflFlushStrmHeader( VeduEfl_EncPara_S  *pEncPara )
{
    D_VENC_CHECK_PTR(pEncPara);
	Venc_SetRegDefault(pEncPara);
	
    /* make sps & pps & VOL stream */
    if (pEncPara->Protocol == VEDU_H264)
    {
        VeduEfl_H264e_SPS_S sps;
        VeduEfl_H264e_PPS_S pps;

        switch(pEncPara->H264HpEn)
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
	
        sps.FrameWidthInMb  = (pEncPara->stRc.PicWidth + 15) >> 4;
        sps.FrameHeightInMb = (pEncPara->stRc.PicHeight+ 15) >> 4;
        sps.FrameCropLeft = 0;
        sps.FrameCropTop    = 0;
        sps.FrameCropRight  = (sps.FrameWidthInMb * 16 - pEncPara->stRc.PicWidth) >> 1;
        sps.FrameCropBottom = (sps.FrameHeightInMb * 16 - pEncPara->stRc.PicHeight) >> 1;

        pps.ChrQpOffset = pEncPara->ChrQpOffset;
        pps.ConstIntra  = pEncPara->ConstIntra;
#if (defined __VENC_98MV200_CONFIG__)
        pps.H264HpEn    = (pEncPara->H264HpEn == VEDU_H264_HIGH_PROFILE)? 1 : 0;
#else
        pps.H264HpEn    = (pEncPara->H264HpEn == VEDU_H264_BASELINE_PROFILE)? 0 : 1;   //pEncPara->H264HpEn;
#endif
        pps.H264CabacEn = pEncPara->H264CabacEn;
        pps.pScale8x8   = pEncPara->Scale8x8;

        pEncPara->SpsBits = H264e_MakeSPS(pEncPara->SpsStream, &sps);
        pEncPara->PpsBits = H264e_MakePPS(pEncPara->PpsStream, &pps);
    }
    else if (pEncPara->Protocol == VEDU_H263)
    {
        int w = pEncPara->stRc.PicWidth, srcFmt;
        int h = pEncPara->stRc.PicHeight;

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

        vol.Wframe = pEncPara->stRc.PicWidth;
        vol.Hframe = pEncPara->stRc.PicHeight;

        pEncPara->SpsBits = MP4e_MakeVOL(pEncPara->SpsStream, &vol);
    }
    else if (pEncPara->Protocol == VEDU_H265)
    {   
        VeduEfl_H265e_VPS_S vps;
        VeduEfl_H265e_SPS_S sps;
        VeduEfl_H265e_PPS_S pps;
        HI_U32 insert;

        vps.general_interlaced_source_flag = !pEncPara->IsProgressive;
        vps.general_progressive_source_flag = pEncPara->IsProgressive;
        sps.uiPicWidthInLumaSamples  = ((pEncPara->stRc.PicWidth  + 7) >> 3) << 3;
        sps.uiPicHeightInLumaSamples = ((pEncPara->stRc.PicHeight + 7) >> 3) << 3;
        sps.uiPicCropLeftOffset = 0 ;
        sps.uiPicCropRightOffset = (sps.uiPicWidthInLumaSamples  - pEncPara->stRc.PicWidth) >> 1;
        sps.uiPicCropTopOffset = 0 ;
        sps.uiPicCropBottomOffset = (sps.uiPicHeightInLumaSamples - pEncPara->stRc.PicHeight) >> 1;
        sps.bPicCroppingFlag  = sps.uiPicCropLeftOffset  || sps.uiPicCropTopOffset |sps.uiPicCropRightOffset || sps.uiPicCropBottomOffset;

        sps.bSAOEnabledFlag = 0;
        sps.sps_temporal_mvp_enable_flag = pEncPara->stHal.BaseCfg.tmv_en;
        sps.bPcmEnable = pEncPara->stHal.BaseCfg.vcpi_ipcm_en;
        sps.iPcmLog2MaxSize = 3 ;
        sps.iPcmLog2MinSize = 3 ;

        sps.bStrongIntraSmoothingFlag = 0;
        sps.ProfileIDC = 100;

        pps.bCUQpDeltaEnable = 1;
        pps.iCbQpOffset = 0;
        pps.iCrQpOffset = 0;
        
        pps.bPicDisableDBFilter = 1;

        pEncPara->VpsBits = H265e_MakeVPS(pEncPara->VpsStream, &vps);
        pEncPara->SpsBits = H265e_MakeSPS(pEncPara->SpsStream, &sps);
        pEncPara->PpsBits = H265e_MakePPS(pEncPara->PpsStream, &pps);

        memcpy(pEncPara->tempVpsStream, pEncPara->VpsStream, sizeof(pEncPara->tempVpsStream));
        memcpy(pEncPara->tempSpsStream, pEncPara->SpsStream, sizeof(pEncPara->tempSpsStream));
        memcpy(pEncPara->tempPpsStream, pEncPara->PpsStream, sizeof(pEncPara->tempPpsStream));
        memset(pEncPara->VpsStream, 0, sizeof(pEncPara->VpsStream));
        memset(pEncPara->SpsStream, 0, sizeof(pEncPara->SpsStream));
        memset(pEncPara->PpsStream, 0, sizeof(pEncPara->PpsStream));

        insert = CheckPreventionThreeByte(pEncPara->tempVpsStream, pEncPara->VpsBits, pEncPara->VpsStream);
        pEncPara->VpsBits = pEncPara->VpsBits + insert*8;
        insert = CheckPreventionThreeByte(pEncPara->tempSpsStream, pEncPara->SpsBits, pEncPara->SpsStream);
        pEncPara->SpsBits = pEncPara->SpsBits + insert*8;
        insert = CheckPreventionThreeByte(pEncPara->tempPpsStream, pEncPara->PpsBits, pEncPara->PpsStream);
        pEncPara->PpsBits = pEncPara->PpsBits + insert*8;
    }

    return HI_SUCCESS;
}

HI_VOID VENC_DRV_EflGetMMZBufAddr(VENC_BUFFER_S *pVencMMZBuf, MMZ_BUFFER_S *pMMZBuf)
{
    pVencMMZBuf->pu8StartVirAddr  = pMMZBuf->pu8StartVirAddr;
    pVencMMZBuf->u32StartPhyAddr  = pMMZBuf->u32StartPhyAddr;
    pVencMMZBuf->u32Size          = pMMZBuf->u32Size;
}

HI_VOID VENC_DRV_EflReleaseMMZBuf(VENC_BUFFER_S *pVencMMZBuf)
{
    MMZ_BUFFER_S MMZBuf;
    memset(&MMZBuf, 0 , sizeof(MMZ_BUFFER_S));
    MMZBuf.pu8StartVirAddr = pVencMMZBuf->pu8StartVirAddr;
    MMZBuf.u32StartPhyAddr = pVencMMZBuf->u32StartPhyAddr;
    MMZBuf.u32Size         = pVencMMZBuf->u32Size;        
    HI_DRV_MMZ_UnmapAndRelease(&MMZBuf);
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

static HI_U32 VENC_DRV_EflGetBitBufSize(VeduEfl_EncCfg_S *pEncCfg, HI_U32 LumaSize)
{
    HI_U32 BitBufSize = 0;
    
    if (pEncCfg->SlcSplitEn)
    {
        BitBufSize = pEncCfg->streamBufSize + D_VENC_ALIGN_UP((LumaSize * 3 / 2 / 4), VEDU_MMZ_ALIGN);
    }
    else
    {
        BitBufSize = pEncCfg->streamBufSize + D_VENC_ALIGN_UP((LumaSize * 3 / 2), VEDU_MMZ_ALIGN);
    }

    return BitBufSize;
}

HI_S32 VENC_DRV_EflAllocStreamBuf(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncCfg_S *pEncCfg, HI_U32 BitBufSize, HI_U32 RcnBufSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (pEncCfg->Protocol == VEDU_JPGE)
    {
        MMZ_BUFFER_S stMMZBuf;
        memset(&stMMZBuf, 0 , sizeof(MMZ_BUFFER_S));

        s32Ret = HI_DRV_MMZ_AllocAndMap("VENC_JpegBuf", HI_NULL, BitBufSize, 64, &stMMZBuf);

        if (HI_SUCCESS == s32Ret)
        {
            VENC_DRV_EflGetMMZBufAddr(&pEncPara->StreamMMZBuf, &stMMZBuf);
        }
        else
        {
            HI_ERR_VENC("JPGE HI_DRV_VENC_AllocAndMap failed\n");

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


HI_U32 VENC_DRV_EflGetFrameStride(VeduEfl_EncCfg_S *pEncCfg)
{
    HI_U32 Stride = 0;

    if (pEncCfg->Protocol == VEDU_H264)
    {
        Stride = (pEncCfg->FrameWidth + 255) / 256 * 32;
    }
    else
    {
        Stride = (pEncCfg->FrameWidth + 63) / 64 * 32;
    }

    return Stride;
}

HI_VOID VENC_DRV_EflInitCfg(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncCfg_S *pEncCfg)
{
    /* get channel para */
    pEncPara->Protocol  = pEncCfg->Protocol;
    pEncPara->IsProgressive = 1;
    pEncPara->stRc.PicWidth  = pEncCfg->FrameWidth;//WidthInMb  << 4;
    pEncPara->stRc.PicHeight = pEncCfg->FrameHeight;//HeightInMb << 4;

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

    pEncPara->stHal.SlcSplitEn = pEncCfg->SlcSplitEn;
    pEncPara->stHal.SplitSize  = pEncCfg->SplitSize;
    pEncPara->QuickEncode = pEncCfg->QuickEncode;
    pEncPara->QLevel     = pEncCfg->QLevel;

    pEncPara->Priority   = pEncCfg->Priority;
    pEncPara->stRc.Gop        = pEncCfg->Gop;
    pEncPara->WaitingIsr = 0;
    pEncPara->WaitingIsr_jpge = 0;
    pEncPara->stHal.pRegBase   = VeduIpCtx.pRegBase;
    pEncPara->OMXChn     = pEncCfg->bOMXChn;
    pEncPara->ICatchEnable = pEncCfg->bAutoRequestIfrm;
    pEncPara->PrependSpsPpsEnable = pEncCfg->bPrependSpsPps;
    pEncPara->AutoSkipFrameEn = pEncCfg->bRCSkipFrmEn;
    pEncPara->DriftRcSkipThr  = pEncCfg->DriftRCThr;
    //pEncPara->YuvStoreType  = pEncCfg->YuvStoreType;                  //
    //pEncPara->YuvSampleType = VEDU_YUV420;                            //
    //pEncPara->PackageSel = pEncCfg->PackageSel;

    /* init RC para */
    pEncPara->stRc.IsIntraPic = 1;

    /* other */
    pEncPara->bNeverEnc   = HI_TRUE;
    pEncPara->stHal.SlcSplitMod = 0;   // 1;                                          //just choose the mb line Mode
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
#if (defined(__VENC_98M_CONFIG__) || defined(__VENC_S5V100_CONFIG__) || defined(__VENC_98CV200_CONFIG__) || defined(__VENC_98MV200_CONFIG__))
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

#ifdef __VENC_98MV200_CONFIG__

    if (pEncPara->Protocol == VEDU_H264)
    {
        pEncPara->stHal.BaseCfg.vcpi_entropy_mode = !pEncPara->H264CabacEn;
        pEncPara->stHal.BaseCfg.vcpi_trans_mode   = (pEncPara->H264HpEn == VEDU_H264_HIGH_PROFILE ? 0 : 1);
        pEncPara->stHal.BaseCfg.vcpi_intra_cu_en  = (pEncPara->H264HpEn == VEDU_H264_HIGH_PROFILE ? 7 : 5);
    }
    else
    {
        pEncPara->stHal.BaseCfg.vcpi_entropy_mode = 0;
        pEncPara->stHal.BaseCfg.vcpi_trans_mode   = 0;
        pEncPara->stHal.BaseCfg.vcpi_intra_cu_en  = 7;
    }

    pEncPara->stHal.BaseCfg.vcpi_ipcm_en      = 1 ;

    pEncPara->stHal.BaseCfg.vcpi_sao_chroma   = 1 ;
    pEncPara->stHal.BaseCfg.vcpi_sao_luma     = 1 ;

    pEncPara->stHal.BaseCfg.max_num_mergecand = 2 ;
    pEncPara->stHal.BaseCfg.tmv_en            = 1 ;
#endif

    /* make sps & pps & VOL stream */
    VENC_DRV_EflFlushStrmHeader(pEncPara);

    /* init RC para */
    pEncPara->stRc.IsIntraPic = 1;
    //pEncPara->StartQp = pEncPara->Protocol == VEDU_H264 ? 24 : 5;
    ///////////add by ckf77439
    pEncPara->stRc.MinTimeOfP = 3;
    pEncPara->stRc.MaxTimeOfP = 6;
    pEncPara->stRc.DeltaTimeOfP = 0;
    pEncPara->stRc.IQpDelta = 2;
    pEncPara->stRc.PQpDelta = 2;
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
    if (pEncCfg->Protocol == VEDU_JPGE)
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
HI_S32 VENC_DRV_EflCreateVenc( VeduEfl_EncPara_S **pEncHandle, VeduEfl_EncCfg_S *pEncCfg )
{
    HI_S32 s32Ret = HI_FAILURE;
    VeduEfl_EncPara_S *pEncPara;
    HI_U32 LumaSize;

    HI_U32 BusRcnBuf, RcnBufSize; /* 16 aligned, = 2.0 frame mb-yuv */
    HI_U32 BusBitBuf, BitBufSize;
    HI_VOID *pVirBit = HI_NULL;
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

    if (VENC_DRV_EflGetLumaSize(pEncCfg, &LumaSize) != HI_SUCCESS)
    {
        return HI_ERR_VENC_INVALID_PARA;
    }

    RcnBufSize = LumaSize * 3;
#if (defined __VENC_98MV200_CONFIG__)
    RcnBufSize = RcnBufSize + 0x600000; // todo.....6M
#endif

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
    pEncPara->stHal.u32SmmuPageBaseAddr = u32ptaddr;
    pEncPara->stHal.u32SmmuErrReadAddr  = u32err_rdaddr;
    pEncPara->stHal.u32SmmuErrWriteAddr = u32err_wraddr;
    //printk("%s,%d, u32SmmuPageBaseAddr =0x%x,u32SmmuErrReadAddr = 0x%x,u32SmmuErrWriteAddr = 0x%x\n",__func__,__LINE__,pEncPara->u32SmmuPageBaseAddr,pEncPara->u32SmmuErrReadAddr,pEncPara->u32SmmuErrWriteAddr);
#endif

    BitBufSize = VENC_DRV_EflGetBitBufSize(pEncCfg, LumaSize);

    if (VENC_DRV_EflAllocStreamBuf(pEncPara, pEncCfg, BitBufSize, RcnBufSize) != HI_SUCCESS)
    {
        goto error_1;
    }

    BusBitBuf = pEncPara->StreamMMZBuf.u32StartPhyAddr;
    pVirBit = (HI_VOID*)(pEncPara->StreamMMZBuf.pu8StartVirAddr);

    BusRcnBuf = BusBitBuf + BitBufSize;

    pEncPara->RegConfigMode = 1;

    if ((pEncPara->RegConfigMode) && (pEncCfg->Protocol != VEDU_JPGE))
    {
        MMZ_BUFFER_S DdrRegCfgMMZBuf;
        HI_U32 DdrRegCfgBufSize = 4096;
        memset(&DdrRegCfgMMZBuf, 0 , sizeof(MMZ_BUFFER_S));
        s32Ret = HI_DRV_MMZ_AllocAndMap("VENC_DdrRegCfgBuf", HI_NULL, DdrRegCfgBufSize, 64, &DdrRegCfgMMZBuf);

        if (HI_SUCCESS == s32Ret)
        {
            VENC_DRV_EflGetMMZBufAddr(&(pEncPara->stHal.DdrRegCfgBuf), &DdrRegCfgMMZBuf);
            memset(pEncPara->stHal.DdrRegCfgBuf.pu8StartVirAddr, 0 , DdrRegCfgBufSize);
        }
        else
        {
            goto error_2;
        }
    }


    /* creat stream buffer lock */
    if (HI_FAILURE == VENC_DRV_OsalLockCreate( &pEncPara->pStrmBufLock ))
    {
        goto error_3;
    }

    /* ArrangeChnBuf -> rcn & bits */
    pEncPara->stHal.RcnYAddr[0] = BusRcnBuf;
    pEncPara->stHal.RcnCAddr[0] = BusRcnBuf + LumaSize;
    pEncPara->stHal.RcnYAddr[1] = BusRcnBuf + LumaSize * 3 / 2;
    pEncPara->stHal.RcnCAddr[1] = BusRcnBuf + LumaSize * 5 / 2;
#if (defined __VENC_98MV200_CONFIG__)
    {
        HI_U32 j = BusRcnBuf + LumaSize * 3;
        pEncPara->stHal.RcnYAddr_head[0] = j;
        j += 0x08000;   //(width+63)/64* (height+63)/64*2*16
        pEncPara->stHal.RcnYAddr_head[1] = j;
        j += 0x08000;
        pEncPara->stHal.RcnCAddr_head[0] = j;
        j += 0x04000;   //(width+63)/64* (height/2+63)/64*2*16
        pEncPara->stHal.RcnCAddr_head[1] = j;
        j += 0x04000;

        pEncPara->stHal.TmvAddr      [0] = j;
        j += 0x100000;  //H265: (width+63)/64* (height+63)/64*8*16 ;H264:((width+15)/16* (height+15)/16+7)/8*8*1601425.83333333333333333333
        pEncPara->stHal.TmvAddr      [1] = j;
        j += 0x100000;
        pEncPara->stHal.PmeAddr      [0] = j;
        j += 0x100000;  //(width+63)/4* (height+63)/4
        pEncPara->stHal.PmeAddr      [1] = j;
        j += 0x100000;
        pEncPara->stHal.PmeInfoAddr  [0] = j;
        j += 0x20000;   //(((width+15)/16))+127)/128*(height+15)/16
        pEncPara->stHal.PmeInfoAddr  [1] = j;
        j += 0x20000;
        pEncPara->stHal.PmeInfoAddr  [2] = j;
        j += 0x20000;

        pEncPara->stHal.RHStrideY = VENC_DRV_EflGetFrameStride(pEncCfg);
        pEncPara->stHal.RHStrideC = VENC_DRV_EflGetFrameStride(pEncCfg);
    }
#endif

#ifdef HI_SMMU_SUPPORT

    /*wp,rp should be phyAddr*/
    if (pEncCfg->Protocol == VEDU_JPGE)
    {
        pEncPara->stHal.StrmBufRpAddr = BusBitBuf;
        pEncPara->stHal.StrmBufWpAddr = BusBitBuf  + 16; /* 16-byte aligned */
    }
    else
    {
        pEncPara->stHal.StrmBufRpAddr = VeduEfl_SmmuToPhyAddr(pEncPara->stHal.u32SmmuPageBaseAddr, BusBitBuf);
        pEncPara->stHal.StrmBufWpAddr = VeduEfl_SmmuToPhyAddr(pEncPara->stHal.u32SmmuPageBaseAddr, BusBitBuf  + 16); /* 16-byte aligned */
    }

#else
    pEncPara->stHal.StrmBufRpAddr = BusBitBuf;
    pEncPara->stHal.StrmBufWpAddr = BusBitBuf  + 16; /* 16-byte aligned */
#endif
    pEncPara->stHal.StrmBufAddr   = BusBitBuf  + VEDU_MMZ_ALIGN;
    pEncPara->stHal.StrmBufSize   = pEncCfg->streamBufSize - VEDU_MMZ_ALIGN;
    pEncPara->StrmBufRpVirAddr = (HI_U32*)pEncPara->StreamMMZBuf.pu8StartVirAddr;
    pEncPara->StrmBufWpVirAddr = (HI_U32*)((HI_U8*)pEncPara->StrmBufRpVirAddr + 16);

    //    pVirBit = VENC_DRV_OsalBufMap( BusBitBuf, BitBufSize );

    //    pEncPara->Vir2BusOffset = (HI_U32)pVirBit - BusBitBuf;
    pEncPara->Vir2BusOffset = pEncPara->StreamMMZBuf.u32Size;

    /* init cycle buffer for stream */
    if (HI_FAILURE == VENC_DRV_BufInit(&pEncPara->stCycBuf, (HI_U8*)pVirBit + VEDU_MMZ_ALIGN , pEncPara->stHal.StrmBufSize, 64))
    {
        goto error_4;
    }

    /************************************* add ******************************************/

    if (pEncCfg->bOMXChn)
    {
        pEncPara->FrameQueue_OMX = VENC_DRV_MngQueueInit(MAX_VEDU_QUEUE_NUM, HI_TRUE);

        if (HI_NULL == pEncPara->FrameQueue_OMX)
        {
            HI_ERR_VENC("failed to init FrameQueue, size = %d\n", sizeof(queue_info_s));
            goto error_4;
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
            goto error_4;
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

error_4:
    VENC_DRV_OsalLockDestroy(pEncPara->pStrmBufLock);
error_3:
    VENC_DRV_EflReleaseMMZBuf(&pEncPara->stHal.DdrRegCfgBuf);
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

    VENC_DRV_OsalLockDestroy( pEncPara->pStrmBufLock );

    if (pEncPara->Protocol == VEDU_JPGE)
    {
        VENC_DRV_EflReleaseMMZBuf(&pEncPara->StreamMMZBuf);
    }
    else
    {
        HI_DRV_VENC_UnmapAndRelease(&pEncPara->StreamMMZBuf);
    }

    if (pEncPara->RegConfigMode)
    {
        VENC_DRV_EflReleaseMMZBuf(&pEncPara->stHal.DdrRegCfgBuf);
    }


    if (pEncPara->OMXChn)
    {
	    for(i = 0; i < OMX_OUTPUT_PORT_BUFFER_MAX;i++)
	    {
            if ( 0 != g_stKernelOmxmmz[ChanId][i].pu8StartVirAddr)
	        {
               sMBufVenc_temp.pu8StartVirAddr = g_stKernelOmxmmz[ChanId][i].pu8StartVirAddr;
	           HI_DRV_VENC_Unmap(&sMBufVenc_temp);  
	        }
	    }

	    if( VENC_DRV_MngQueueDeinit(pEncPara->StreamQueue_OMX))
		{
			HI_ERR_VENC("HI_KFREE failed to free StreamQueue_OMX, size = %d\n", sizeof(queue_info_s));
			return HI_FAILURE;
		}

	    if( VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue_OMX))
		{
			HI_ERR_VENC("HI_KFREE failed to free FrameQueue_OMX, size = %d\n", sizeof(queue_info_s));
			return HI_FAILURE;
		}

		if( VENC_DRV_MngQueueDeinit(pEncPara->MsgQueue_OMX))
		{
			HI_ERR_VENC("HI_KFREE failed to free MsgQueue_OMX, size = %d\n", sizeof(queue_info_s));
			return HI_FAILURE;
		}
				
    }
	else
	{
        if( VENC_DRV_MngQueueDeinit(pEncPara->FrameQueue))
		{
			HI_ERR_VENC("HI_KFREE failed to free FrameQueue, size = %d\n", sizeof(queue_info_s));
			return HI_FAILURE;
		}
        if( VENC_DRV_MngQueueDeinit(pEncPara->FrameDequeue))
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
	VeduEfl_EncPara_S  *pEncPara;
    D_VENC_GET_CHN(u32ChnID,EncHandle); 
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
	VeduEfl_EncPara_S  *pEncPara;
	D_VENC_GET_CHN(u32ChnID,EncHandle); 
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
HI_VOID VENC_DRV_EflRequestIframe( VeduEfl_EncPara_S* EncHandle )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    pEncPara->InterFrmCnt = pEncPara->stRc.Gop - 1;

    pEncPara->stRc.IFrmInsertFlag = 1;
    pEncPara->stRc.RcStart = 1;
    
    return;
}

HI_S32 VENC_DRV_EflRcGetAttr( VeduEfl_EncPara_S* EncHandle, VeduEfl_RcAttr_S* pRcAttr )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;

    pRcAttr->BitRate = pEncPara->stRc.BitRate;
    pRcAttr->OutFrmRate = pEncPara->stRc.VoFrmRate;
    pRcAttr->InFrmRate = pEncPara->stRc.ViFrmRate;
    
    pRcAttr->MaxQp  = pEncPara->stRc.MaxQp;
    pRcAttr->MinQp  = pEncPara->stRc.MinQp;
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflAllocBuf2Jpge( VeduEfl_EncPara_S* EncHandle, HI_U32 CapLevel)
{
    HI_S32 s32Ret = 0;
	HI_U32 u32Size = 0;
    VeduEfl_EncPara_S  *pEncPara = (VeduEfl_EncPara_S *)EncHandle;
    if (pEncPara == NULL)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
	
    switch (CapLevel)
    {
        case VEDU_CAP_LEVEL_QCIF:
			u32Size = 176*144*3/2;
			break;
		case VEDU_CAP_LEVEL_CIF:
			u32Size = 352*288*3/2;
			break;
		case VEDU_CAP_LEVEL_D1:
			u32Size = 720*576*3/2;
			break;
		case VEDU_CAP_LEVEL_720P:
			u32Size = 1280*720*3/2;
			break;
		case VEDU_CAP_LEVEL_1080P:
			u32Size = 1920*1088*3/2;
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

    pEncPara->stRc.BitRate   = pRcAttr->BitRate;
    pEncPara->stRc.VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->stRc.ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->stRc.MaxQp     = pRcAttr->MaxQp;
    pEncPara->stRc.MinQp     = pRcAttr->MinQp;

    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*initialize frame rate control parameter*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->stRc.ViFrmRate,
                       pEncPara->stRc.BitRate / pEncPara->stRc.ViFrmRate);

    pEncPara->MeanBit = pEncPara->stRc.BitRate / pEncPara->stRc.VoFrmRate;
    pEncPara->TrCount = pEncPara->stRc.ViFrmRate;

    for (i = 0; i < pEncPara->stRc.ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->stRc.VoFrmRate;

        if (pEncPara->TrCount > pEncPara->stRc.ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->stRc.ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }

    /*initialize re-start parameter*/
    pEncPara->stRc.RcStart = 1;

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

    pEncPara->stRc.BitRate   = pRcAttr->BitRate;
    pEncPara->stRc.VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->stRc.ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->stRc.MaxQp     = pRcAttr->MaxQp;
    pEncPara->stRc.MinQp     = pRcAttr->MinQp;

    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*initialize frame rate control parameter*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->stRc.ViFrmRate,
                       pEncPara->stRc.BitRate / pEncPara->stRc.ViFrmRate);

    pEncPara->MeanBit = pEncPara->stRc.BitRate / pEncPara->stRc.VoFrmRate;
    pEncPara->TrCount = pEncPara->stRc.ViFrmRate;

    for (i = 0; i < pEncPara->stRc.ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->stRc.VoFrmRate;

        if (pEncPara->TrCount > pEncPara->stRc.ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->stRc.ViFrmRate;
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, pEncPara->MeanBit);
        }
        else
        {
            VENC_DRV_FifoWriteInit( &pEncPara->stBitsFifo, 0);
        }
    }
	
    /*initialize re-start parameter*/
    pEncPara->stRc.RcStart = 1;

    return HI_SUCCESS;
}

#endif

HI_S32 VENC_DRV_EflRcFrmRateCtrl( VeduEfl_EncPara_S* EncHandle, HI_U32 TR )
{
    VeduEfl_EncPara_S*  pEncPara = EncHandle;
    HI_U32  DiffTR = (TR - pEncPara->LastTR);

    HI_U32 i;
    if (pEncPara->stRc.RcStart)
    {
        pEncPara->TrCount  = pEncPara->stRc.ViFrmRate;
        pEncPara->TrCount += pEncPara->stRc.VoFrmRate;
        pEncPara->LastTR   = TR;
        pEncPara->stRc.IsIntraPic = 1;
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
            pEncPara->TrCount += pEncPara->stRc.VoFrmRate;
            VENC_DRV_FifoWrite( &pEncPara->stBitsFifo, 0);
        }
		
        /* this frame */
        pEncPara->TrCount += pEncPara->stRc.VoFrmRate;
        pEncPara->LastTR = TR;

        /* don't care too many Lost frames */
        if (pEncPara->TrCount > pEncPara->stRc.VoFrmRate + pEncPara->stRc.ViFrmRate)
        {
            pEncPara->TrCount = pEncPara->stRc.VoFrmRate + pEncPara->stRc.ViFrmRate;
        }

        /* skip this frame */
        if ((pEncPara->TrCount <= pEncPara->stRc.ViFrmRate)      /* time to skip */
            || (pEncPara->stBitsFifo.Sum > (pEncPara->stRc.BitRate / 100 * (pEncPara->DriftRcSkipThr + 100))) /* too many bits */ //RC_Threshold = 20%
            || (VENC_DRV_BufGetFreeLen( &pEncPara->stCycBuf ) < 64 * 8))/* too few buffer */ 
        {
            if (pEncPara->TrCount <= pEncPara->stRc.ViFrmRate)
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
        if (pEncPara->InterFrmCnt >= pEncPara->stRc.Gop - 1)
        {
            pEncPara->stRc.IsIntraPic = 1;
			pEncPara->FrmCnt   = 0;
			pEncPara->stRc.FrmNumInGop = 0;
        }
        else
        {
            pEncPara->stRc.IsIntraPic = 0;
			pEncPara->stRc.FrmNumInGop = pEncPara->InterFrmCnt + 1;
        }
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

    memcpy(&stRc_old,&(pEncPara->stRc),sizeof(stRc_old));
	
    /* updata the RC structure*/
    pEncPara->stRc.AverageFrameBits = pRcAttr->BitRate / pRcAttr->OutFrmRate;
	pEncPara->stRc.InstBitsLen = pRcAttr->OutFrmRate;

    if (pRcAttr->Gop <= (pEncPara->stRc.FrmNumInGop+1))
	{
         VENC_DRV_EflRequestIframe(EncHandle);
		 goto init_process;
	}

init_process:

    /*initialize  parameter*/
    pEncPara->stRc.BitRate   = pRcAttr->BitRate;
    pEncPara->stRc.VoFrmRate = pRcAttr->OutFrmRate;
    pEncPara->stRc.ViFrmRate = pRcAttr->InFrmRate;
    pEncPara->TrCount   = pEncPara->stRc.ViFrmRate;
    pEncPara->stRc.Gop  = pRcAttr->Gop;
    pEncPara->VBRflag  = 0;
    pEncPara->PicLevel = 0;

    /*updata the RC strmBuffer*/
    VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->stRc.ViFrmRate,
                       pEncPara->stRc.BitRate / pEncPara->stRc.ViFrmRate);

    pEncPara->MeanBit = pEncPara->stRc.BitRate / pEncPara->stRc.VoFrmRate;

    for (i = 0; i < pEncPara->stRc.ViFrmRate; i++)
    {
        pEncPara->TrCount += pEncPara->stRc.VoFrmRate;

        if (pEncPara->TrCount > pEncPara->stRc.ViFrmRate)
        {
            pEncPara->TrCount -= pEncPara->stRc.ViFrmRate;
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
static HI_VOID VENC_DRV_EflIntraFrmRateChange( VeduEfl_EncPara_S *pEncPara,HI_U32 newViFrmRate,HI_UNF_VENC_CHN_ATTR_S stSrcUserCfg)
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
    pEncPara->stRc.AverageFrameBits   = pEncPara->BitRate / pEncPara->VoFrmRate;

	pEncPara->stRc.GopBitsLeft    = pEncPara->stRc.AverageFrameBits * (pEncPara->Gop - pEncPara->stRc.FrmNumInGop -1);

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

/******************************************************************************
Function   :
Description: 获取尽量可靠的输入帧率 与对应的输出帧率 并做相应的处理
Calls      :
Input      :
Output     :
Return     :
Others     :
******************************************************************************/
static HI_VOID VENC_DRV_EflGetFrmRate( VeduEfl_EncPara_S *pEncPara,HI_UNF_VENC_CHN_ATTR_S stSrcUserCfg)
{
    HI_U32 diff1 = 0; 
	HI_U32 newViFrmRate = 0;
	HI_U32 i = 0;

    HI_U32 max1,max2;

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
                newViFrmRate = pEncPara->stRc.ViFrmRate;
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
	
    if ((newViFrmRate != pEncPara->stRc.ViFrmRate) && (newViFrmRate <= FrmRateCntBufNum))
    {
        if (newViFrmRate >= FrmRateCntBufNum)
        {
            newViFrmRate = FrmRateCntBufNum;   // fifo buffer range
        }

        pEncPara->stRc.ViFrmRate = newViFrmRate;

        if (stSrcUserCfg.u32TargetFrmRate > pEncPara->stRc.ViFrmRate)
        {
            pEncPara->stRc.VoFrmRate = pEncPara->stRc.ViFrmRate;

        }
        else
        {
            pEncPara->stRc.VoFrmRate = stSrcUserCfg.u32TargetFrmRate;
        }

        /* updata the RC structure*/
        if (pEncPara->bFrmRateSataError)    //error!
        {
            pEncPara->stRc.AverageFrameBits   = pEncPara->stRc.BitRate / (pEncPara->stRc.VoFrmRate + 8);
        }
        else
        {
            pEncPara->stRc.AverageFrameBits   = pEncPara->stRc.BitRate / pEncPara->stRc.VoFrmRate;
        }


        /*initialize  parameter*/
        pEncPara->TrCount   = pEncPara->stRc.ViFrmRate;

        /*updata the RC strmBuffer*/
        VENC_DRV_FifoInit( &pEncPara->stBitsFifo, pEncPara->BitsFifo, pEncPara->stRc.ViFrmRate,
                           pEncPara->stRc.BitRate / pEncPara->stRc.ViFrmRate);

        pEncPara->MeanBit = pEncPara->stRc.BitRate / pEncPara->stRc.VoFrmRate;

        for (i = 0; i < pEncPara->stRc.ViFrmRate; i++)
        {
            pEncPara->TrCount += pEncPara->stRc.VoFrmRate;

            if (pEncPara->TrCount > pEncPara->stRc.ViFrmRate)
            {
                pEncPara->TrCount -= pEncPara->stRc.ViFrmRate;
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
    if (LumaSize > pEncPara->stHal.RcnCAddr[0] - pEncPara->stHal.RcnYAddr[0])
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
    pEncPara->stHal.SlcSplitMod = 1;                                  
    pEncPara->NumRefIndex = 0;
    pEncPara->stRc.PicWidth  = FrameWidth;
    pEncPara->stRc.PicHeight = FrameHeight;

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
    HI_U32 i,j;
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S *pEncPara = NULL;
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == NULL)
        {
            VeduChnCtx[i].EncHandle = EncHandle;
            pEncPara = EncHandle;
            pEncPara->stHal.pRegBase = VeduIpCtx.pRegBase;
            break;
        }
    }
    if ((i >= VENC_MAX_CHN_NUM) || (pEncPara == NULL))
	{  
	    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }  
    for (j = 0;j < MAX_VEDU_CHN; j++)
    {
        if( INVAILD_CHN_FLAG ==PriorityTab[0][j])
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

static HI_VOID VENC_DRV_EflInitLastFrmRate(HI_U32 *LastFrmRate, HI_U32 *LastSecInputFrmRate)
{
    HI_U32 i;
    
    for(i = 0; i < 6; i++)
    {
        LastFrmRate[i] = 0;
    }
    for(i = 0; i < 2; i++)
    {
        LastSecInputFrmRate[i] = 0;
    }

    return;
}

HI_S32 VENC_DRV_EflStopVenc( VeduEfl_EncPara_S* EncHandle )
{
    HI_U32 i,j;   
    HI_U32 timeCnt = 0;
  
    VEDU_LOCK_FLAG flag;
	HI_U32 u32VeChn;
	VeduEfl_EncPara_S *pEncPara = NULL;
	
	D_VENC_GET_CHN(u32VeChn, EncHandle);
	D_VENC_CHECK_CHN(u32VeChn);
    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag );

    for (i = 0; i < MAX_VEDU_CHN; i++)
    {
        if (VeduChnCtx[i].EncHandle == EncHandle)
        {
            VeduChnCtx[i].EncHandle = NULL;
            break;
        }
    }
    for (j = 0;(i < MAX_VEDU_CHN)&&(j < MAX_VEDU_CHN); j++)
    {
        if( i ==PriorityTab[0][j])
        {
           PriorityTab[0][j] = INVAILD_CHN_FLAG;
           PriorityTab[1][j] = 0;
           VENC_DRV_EflSortPriority();
           break;
        }
    }
    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );

    if (i == MAX_VEDU_CHN)
    {
        return HI_ERR_VENC_CHN_NOT_EXIST;
    }
    /* wait finish last frame */   
    while ((((VeduEfl_EncPara_S *)EncHandle)->WaitingIsr) && (timeCnt < 1000))
    {
        msleep(1);
        timeCnt++;
        HI_WARN_VENC("wait the VEDU isr!!! timeCnt = %d\n",timeCnt);
    }

    if (timeCnt == 1000)
    {
        HI_ERR_VENC("wait the VEDU isr time out!!Force to stop the channel\n");
    }
#ifdef __VENC_SUPPORT_JPGE__
    /* wait finish last frame */
    while (((VeduEfl_EncPara_S *)EncHandle)->WaitingIsr_jpge)
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
           VENC_DRV_EflFlushPort_OMX(EncHandle,ALL_PORT,HI_TRUE);
 
	   }
	}
	else
	{
	   VENC_DRV_EflRlsAllFrame( EncHandle);
	}
	
    pEncPara->bFrmRateSataError = 0;
    VENC_DRV_EflInitLastFrmRate(pEncPara->LastFrmRate, pEncPara->LastSecInputFrmRate);

	if (g_stVencChn[u32VeChn].hJPGE == HI_INVALID_HANDLE)
	{
	    pEncPara->stCycBuf.u32WrHead = pEncPara->stCycBuf.u32RdHead;
	    pEncPara->stCycBuf.u32WrTail = pEncPara->stCycBuf.u32RdHead;
	}
	else   //jpeg
	{
	    VeduEfl_NaluHdr_S *pNaluHdr;
		HI_U32    u32NewWrHead;
		VALG_CRCL_BUF_S stCycBuf_1;
		memcpy(&stCycBuf_1,&pEncPara->stCycBuf,sizeof(VALG_CRCL_BUF_S));

        pNaluHdr = (VeduEfl_NaluHdr_S*)(stCycBuf_1.pBase + stCycBuf_1.u32RdHead);
		if (pNaluHdr->bLastSlice == 1)    /*当前包是图像数据*/
		{
		     if (HI_SUCCESS == VENC_DRV_BufGetNextRp(&stCycBuf_1,pNaluHdr->PacketLen))
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
HI_VOID VENC_DRV_DbgLowdelayEvent(HI_S32 ChanId,HI_LD_Event_ID_E eventId, HI_U32 u32FrameId,HI_HANDLE hSrcHd)
{
    HI_LD_Event_S evt;
    HI_U32 TmpTime = 0;    
	HI_DRV_SYS_GetTimeStampMs(&TmpTime);
    
    evt.evt_id = eventId;
    evt.frame = u32FrameId;
    evt.handle = hSrcHd;
    evt.time = TmpTime; 
#if 0	
    if(1==g_LowDelayCalcStat[ChanId])
    {    
        HI_DRV_LD_Notify_Event(&evt);
    }
#else
     HI_DRV_LD_Notify_Event(&evt);
#endif
}
static HI_S32 VENC_DRV_EflCheckImgCfg(const HI_DRV_VIDEO_FRAME_S *pstPreImage,HI_U32 yuvStoreType)
{
    HI_BOOL flag = 0;

	flag |= (pstPreImage->u32Width > VEDU_MAX_ENC_WIDTH)||(pstPreImage->u32Width < VEDU_MIN_ENC_WIDTH);
	flag |= (pstPreImage->u32Height> VEDU_MAX_ENC_HEIGHT)||(pstPreImage->u32Height< VEDU_MIN_ENC_HEIGHT);
    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Y) || (pstPreImage->stBufAddr[0].u32PhyAddr_Y % 16);
	flag |= (!pstPreImage->stBufAddr[0].u32Stride_Y ) || (pstPreImage->stBufAddr[0].u32Stride_Y  % 16);
	if (VENC_STORE_PLANNAR == yuvStoreType)
	{
	    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C %16);
	    flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_Cr) || (pstPreImage->stBufAddr[0].u32PhyAddr_Cr%16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  %16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_Cr)  || (pstPreImage->stBufAddr[0].u32Stride_Cr %16);

		flag |= (pstPreImage->stBufAddr[0].u32Stride_Cr != pstPreImage->stBufAddr[0].u32Stride_C);
    }
	else if (VENC_STORE_SEMIPLANNAR == yuvStoreType)
    {
        flag |= (!pstPreImage->stBufAddr[0].u32PhyAddr_C)  || (pstPreImage->stBufAddr[0].u32PhyAddr_C %16);
		flag |= (!pstPreImage->stBufAddr[0].u32Stride_C )  || (pstPreImage->stBufAddr[0].u32Stride_C  %16);
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
    if ((pEncPara->stImage.u32Width != pEncPara->stRc.PicWidth) || (pEncPara->stImage.u32Height != pEncPara->stRc.PicHeight))
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

                    pEncPara->stHal.PTS0 = pEncPara->stImage.u32Pts;
                    return HI_FAILURE;
                }
            }
            else   /* JPEG */
            {
                pEncPara->stRc.PicWidth  = pEncPara->stImage.u32Width;
                pEncPara->stRc.PicHeight = pEncPara->stImage.u32Height;
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

            pEncPara->stHal.PTS0 = pEncPara->stImage.u32Pts;
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID VENC_DRV_EflQueryChn_X_Stride(VeduEfl_EncIn_S* pEncIn, VeduEfl_EncPara_S* pEncPara)
{
    if (pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR)
    {
       pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
    }

    if ((pEncPara->stHal.YuvStoreType == VENC_STORE_SEMIPLANNAR) && (VENC_YUV_422 == pEncPara->YuvSampleType))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C * 2;   
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PACKAGE) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
	else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR) && (VENC_YUV_420 == pEncPara->YuvSampleType))  
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

HI_VOID VENC_DRV_EflUpdateScanType(VeduEfl_EncPara_S *pEncPara)
{
    if ((pEncPara->Protocol == VEDU_H265) && (pEncPara->IsProgressive != pEncPara->stImage.bProgressive))
    {
        VeduEfl_H265e_VPS_S vps;
        pEncPara->IsProgressive = pEncPara->stImage.bProgressive;
        vps.general_interlaced_source_flag  = !pEncPara->IsProgressive;
        vps.general_progressive_source_flag =  pEncPara->IsProgressive;
        pEncPara->VpsBits = H265e_MakeVPS(pEncPara->VpsStream, &vps);
    }
}

static HI_S32 VENC_DRV_EflQueryChn_X(HI_U32 u32ChnID, VeduEfl_EncIn_S *pEncIn )    //for venc
{
    HI_HANDLE hHd;
	//HI_S32 s32Ret = 0;
    VeduEfl_EncPara_S *pEncPara = (VeduEfl_EncPara_S *)g_stVencChn[u32ChnID].hVEncHandle;

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
	
    if(pEncPara->QuickEncode)
    {
        if ( HI_FAILURE == QuickEncode_Process(g_stVencChn[u32ChnID].hVEncHandle,hHd))
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
   if (pEncPara->stHal.PTS0 == pEncPara->stImage.u32Pts)
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

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)|(defined __VENC_98CV200_CONFIG__)|(defined __VENC_98MV200_CONFIG__))             //for lowdelay 
    if (pEncPara->stImage.u32TunnelPhyAddr)
    {
        pEncPara->stRc.LowDlyMod = HI_TRUE;
        pEncIn->TunlCellAddr = pEncPara->stImage.u32TunnelPhyAddr;
    }
    else
    {
        pEncPara->stRc.LowDlyMod = HI_FALSE;
        pEncIn->TunlCellAddr = 0;
    }
#endif

    /* video encoder does frame rate control by two value: input frame rate and target frame rate */
    /* input frame rate is calculated by timer mechanism accurately */
    /* target frame rate is input by user and can be changed dynamiclly */
    if (HI_ID_BUTT != g_stVencChn[u32ChnID].enSrcModId)
    {
       D_VENC_RC_UPDATE(pEncPara->LastFrmRate, pEncPara->stImage.u32FrameRate/1000);
       pEncPara->LastSecFrameCnt++;
       VENC_DRV_EflGetFrmRate(pEncPara,g_stVencChn[u32ChnID].stChnUserCfg);
    }
    else
    {
        pEncPara->stRc.ViFrmRate = g_stVencChn[u32ChnID].stChnUserCfg.u32InputFrmRate;
        pEncPara->stRc.VoFrmRate = g_stVencChn[u32ChnID].stChnUserCfg.u32TargetFrmRate;
    }

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->stRc.ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->stRc.VoFrmRate;

    VENC_DRV_EflUpdateScanType(pEncPara);
    /* configured the resolving power dynamically */
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

        pEncPara->stHal.PTS0 = pEncPara->stImage.u32Pts;
        return HI_FAILURE;
    }

    pEncPara->stHal.YuvStoreType  = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 0);
    pEncPara->YuvSampleType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 1); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->stHal.PackageSel    = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 2); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
	pEncPara->stStat.u32FrameType = Convert_PIX_Format(pEncPara->stImage.ePixFormat,3);

    /* check the picture resolving power ,stride ,addr info first*/
    if ( HI_SUCCESS != VENC_DRV_EflCheckImgCfg(&pEncPara->stImage, pEncPara->stHal.YuvStoreType) )
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
        pEncPara->stHal.PTS0 = pEncPara->stImage.u32Pts;
        return HI_FAILURE;         
    }

    pEncIn->BusViY = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    pEncIn->BusViC = pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;

    if (pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
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


static HI_S32 VENC_DRV_EflQueryChn_Y1(HI_U32 u32ChnID, VeduEfl_EncIn_S *pEncIn)   //for omxvenc without VPSS
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

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->stRc.ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->stRc.VoFrmRate;

    /* video encoder does frame rate control by two value: input frame rate and target frame rate */
    /* input frame rate is calculated by timer mechanism accurately */
    /* target frame rate is input by user and can be changed dynamiclly */

    /* skip - frame rate ctrl */
    if (HI_SUCCESS
        != VENC_DRV_EflRcFrmRateCtrl( g_stVencChn[u32ChnID].hVEncHandle,pEncPara->stStat.PutFrameNumOK))
    {
        pEncPara->stStat.PutFrameNumTry++;

        VENC_DRV_EflPutMsg_OMX(pEncPara->MsgQueue_OMX, VENC_MSG_RESP_INPUT_DONE, HI_SUCCESS , &(pEncPara->stImage_OMX));
        pEncPara->stStat.MsgQueueNum++;
        pEncPara->stStat.PutFrameNumOK++;

        pEncPara->stHal.PTS0 = pEncPara->stImage_OMX.timestamp0;
        pEncPara->stHal.PTS1 = pEncPara->stImage_OMX.timestamp1;
        return HI_FAILURE;
    }

    pEncPara->stHal.YuvStoreType  = pEncPara->stImage_OMX.store_type;//VENC_STORE_SEMIPLANNAR;
    pEncPara->YuvSampleType = pEncPara->stImage_OMX.sample_type;//VENC_YUV_420;//Convert_PIX_Format(pEncPara->stImage.ePixFormat,1);
    pEncPara->stHal.PackageSel    = pEncPara->stImage_OMX.package_sel;//VENC_V_U;

    VENC_DRV_GetLastFrmInfo_OMX(pEncPara, pEncIn);
    return 0;
}

static HI_VOID VENC_DRV_EflGetViStride(VeduEfl_EncIn_S *pEncIn, VeduEfl_EncPara_S *pEncPara)
{
    if ((pEncPara->stHal.YuvStoreType == VENC_STORE_SEMIPLANNAR) && (VENC_YUV_422 == pEncPara->YuvSampleType))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C * 2;
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PACKAGE) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        //pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR) && (VENC_YUV_420 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }
    else
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C;
    }

    return;
}

static HI_S32 VENC_DRV_EflQueryChn_Y2(HI_U32 u32ChnID, VeduEfl_EncIn_S *pEncIn )    //for omxvenc with VPSS
{
    HI_HANDLE hHd;
    HI_S32 s32Ret = 0;
    //HI_DRV_VIDEO_FRAME_S    stVpssFrame;
    //HI_DRV_VIDEO_PRIVATE_S *pstFramePriv;
    VeduEfl_EncPara_S *pEncPara = (VeduEfl_EncPara_S*)g_stVencChn[u32ChnID].hVEncHandle;

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


#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__) | (defined __VENC_98CV200_CONFIG__)|(defined __VENC_98MV200_CONFIG__))            //for lowdelay 

    if (pEncPara->stImage.u32TunnelPhyAddr)      //待明确!!
    {
        pEncPara->stRc.LowDlyMod = HI_TRUE;
        pEncIn->TunlCellAddr = pEncPara->stImage.u32TunnelPhyAddr;
    }
    else
    {
        pEncPara->stRc.LowDlyMod = HI_FALSE;
        pEncIn->TunlCellAddr = 0;
    }

#endif

    pEncPara->stStat.u32RealSendInputRrmRate  = pEncPara->stRc.ViFrmRate;
    pEncPara->stStat.u32RealSendOutputFrmRate = pEncPara->stRc.VoFrmRate;

    /* configured the resolving power dynamically */
    if ((pEncPara->stImage.u32Width != pEncPara->stRc.PicWidth) || (pEncPara->stImage.u32Height != pEncPara->stRc.PicHeight))
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

    pEncPara->stHal.YuvStoreType  = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 0);
    pEncPara->YuvSampleType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 1); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->stHal.PackageSel    = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 2); //SampleOrPackageSelTab[pEncPara->stImage.enVideoFormat];
    pEncPara->stStat.u32FrameType = Convert_PIX_Format(pEncPara->stImage.ePixFormat, 3);

    /* check the picture resolving power ,stride ,addr info first*/
    if ( HI_SUCCESS != VENC_DRV_EflCheckImgCfg(&pEncPara->stImage, pEncPara->stHal.YuvStoreType))
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

    if (pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
        //pEncIn->BusViV    = pEncIn->BusViC + pEncPara->stImage_OMX.strideC * pEncPara->PicHeight / 2;
    }

    VENC_DRV_EflGetViStride(pEncIn, pEncPara);

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
    VeduEfl_EncPara_S *pEncPara = HI_NULL;
	VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* hHd_ret = HI_NULL;
	//HI_S32 i;
	
    /* start from last query channel */
    /*if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        u32StartQueryNo = 0;
    }*/

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
    for (u32StartQueryNo = 0;u32StartQueryNo < MAX_VEDU_CHN;u32StartQueryNo++)
    {
	    /*if (MAX_VEDU_CHN == u32StartQueryNo)
	    {
	        u32StartQueryNo = 0;
	    }*/
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID,u32StartQueryNo); 
        if ( INVAILD_CHN_FLAG == s32StartChnID )
        {
            continue;
        }
		D_VENC_GET_CHN(u32ChnID,VeduChnCtx[s32StartChnID].EncHandle); 
		if (u32ChnID >= VENC_MAX_CHN_NUM)
		{   
		    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag ); 
            return HI_NULL;  
        } 
        pEncPara = (VeduEfl_EncPara_S *) VeduChnCtx[s32StartChnID].EncHandle;
		
        if( HI_INVALID_HANDLE == pEncPara->stSrcInfo.handle )
        {
            pEncPara->stSrcInfo.handle = g_stVencChn[u32ChnID].hUsrHandle;//VeduChnCtx[s32StartChnID].EncHandle;
        }

       /* this QueryChn just for H264*/
	   if (HI_INVALID_HANDLE != g_stVencChn[u32ChnID].hJPGE)
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
  
       VENC_DRV_DbgLowdelayEvent(u32ChnID,EVENT_VENC_FRM_IN,pEncPara->stImage.u32FrameIndex,pEncPara->stImage.hTunnelSrc);
	   
        pEncPara->WaitingIsr = 1;
        break; /* find channel:s32StartChnID  to enc */
    }

	if (MAX_VEDU_CHN != u32StartQueryNo)
	{
	   VENC_DRV_EflSortPriority_2(PriorityTab[1][u32StartQueryNo]);
	}

    if ((s32StartChnID >= 0)&&(s32StartChnID < MAX_VEDU_CHN))
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
    VeduEfl_EncPara_S *pEncPara = HI_NULL;
	VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S* hHd_ret = HI_NULL;
	//HI_S32 i;
	
	
    /* start from last query channel */
    /*if (MAX_VEDU_CHN == u32StartQueryNo)
    {
        u32StartQueryNo = 0;
    }*/

    VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
	
    for (u32StartQueryNo = 0;u32StartQueryNo < MAX_VEDU_CHN;u32StartQueryNo++)
    {
	    /*if (MAX_VEDU_CHN == u32StartQueryNo)
	    {
	        u32StartQueryNo = 0;
	    }*/
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID,u32StartQueryNo); 
        if ( INVAILD_CHN_FLAG == s32StartChnID )
        {
            continue;
        }
		D_VENC_GET_CHN(u32ChnID,VeduChnCtx[s32StartChnID].EncHandle); 
		if (u32ChnID >= VENC_MAX_CHN_NUM)
		{   
		    VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag ); 
            return HI_NULL;  
        } 
        pEncPara = (VeduEfl_EncPara_S *) VeduChnCtx[s32StartChnID].EncHandle;
        if( HI_INVALID_HANDLE == pEncPara->stSrcInfo.handle )
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
  
       VENC_DRV_DbgLowdelayEvent(u32ChnID,EVENT_VENC_FRM_IN,pEncPara->stImage.u32FrameIndex,pEncPara->stImage.hTunnelSrc);
	   
        pEncPara->WaitingIsr_jpge = 1;
        break; /* find channel:s32StartChnID  to enc */
    }

	if (MAX_VEDU_CHN != u32StartQueryNo)
	{
	   VENC_DRV_EflSortPriority_2(PriorityTab[1][u32StartQueryNo]);
	}

    if ((s32StartChnID >= 0)&&(s32StartChnID < MAX_VEDU_CHN))
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
static HI_S32 VENC_DRV_EflCurrentCap(HI_S32 *pClkSel)
{
    HI_U32 u32ChnID = 0;
	HI_U32 u32TotalMB = 0,u32MBNum = 0;
	HI_U32 WidthInMb = 0;
    HI_U32 HeightInMb= 0;
	for (u32ChnID = 0; u32ChnID < MAX_VEDU_CHN; u32ChnID++)
	{
	   if (g_stVencChn[u32ChnID].bEnable) 
	   {
	       WidthInMb  = (g_stVencChn[u32ChnID].stChnUserCfg.u32Width + 15) >> 4;
		   HeightInMb = (g_stVencChn[u32ChnID].stChnUserCfg.u32Height+ 15) >> 4;
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
static HI_VOID Venc_Smmu_ISR( VeduEfl_EncPara_S *pEncPara )
{
    S_VEDU_REGS_TYPE *pAllReg = (S_VEDU_REGS_TYPE *)pEncPara->stHal.pRegBase;

    VENC_HAL_ReadReg_Smmu(&pEncPara->stHal);

    if (pEncPara->stHal.intns_tlbmiss_stat)
    {
        HI_ERR_VENC("%s,%d, SMMU Table miss error!\n", __func__, __LINE__);
        pEncPara->stHal.intns_tlbmiss_stat = 0;
    }

    if (pEncPara->stHal.intns_ptw_trans_stat)
    {
        HI_ERR_VENC("%s,%d, SMMU Table pre get error!\n", __func__, __LINE__);
        pEncPara->stHal.intns_ptw_trans_stat = 0;
    }
    /*for not safe mode*/
    if (pEncPara->stHal.intns_tlbinvalid_rd_stat)
    {
        HI_ERR_VENC("SMMU_READ_ERR MODULE : %s NSSTATE : %#x RdAddr:%#x\n", "HI_MOD_VENC", pEncPara->stHal.intns_tlbinvalid_rd_stat, pAllReg->VEDU_MMU_REGS.SMMU_FAULT_ADDR_RD_NS);
        pEncPara->stHal.intns_tlbinvalid_rd_stat = 0;
    }

    if (pEncPara->stHal.intns_tlbinvalid_wr_stat)
    {
        HI_ERR_VENC("SMMU_WRITE_ERR MODULE : %s NSSTATE : %#x WrAddr:%#x\n", "HI_MOD_VENC", pEncPara->stHal.intns_tlbinvalid_wr_stat, pAllReg->VEDU_MMU_REGS.SMMU_FAULT_ADDR_WR_NS);
        pEncPara->stHal.intns_tlbinvalid_wr_stat = 0;
    }
    /*for safe mode*/
    if (pEncPara->stHal.ints_tlbinvalid_rd_stat)    
    {
        HI_ERR_VENC("SMMU_READ_ERR MODULE : %s NSSTATE : %#x RdAddr:%#x\n", "HI_MOD_VENC", pEncPara->stHal.ints_tlbinvalid_rd_stat, pAllReg->VEDU_MMU_REGS.SMMU_FAULT_ADDR_RD_S);
        pEncPara->stHal.ints_tlbinvalid_rd_stat = 0;
    }

    if (pEncPara->stHal.ints_tlbinvalid_wr_stat)
    {
        HI_ERR_VENC("SMMU_WRITE_ERR MODULE : %s NSSTATE : %#x WrAddr:%#x\n", "HI_MOD_VENC", pEncPara->stHal.ints_tlbinvalid_wr_stat, pAllReg->VEDU_MMU_REGS.SMMU_FAULT_ADDR_WR_S);
        pEncPara->stHal.ints_tlbinvalid_wr_stat = 0;
    }

    return;
}
#endif


static HI_VOID VENC_DRV_RcIFrmInsertForLargeImb(VeduEfl_EncPara_S *pEncPara)
{
    HI_U32 ImbRatio = 0;
    HI_U32 ImbRatioAvg = 0;
    HI_U32 Mbtotal = 0;

    Mbtotal =  pEncPara->stRc.PicWidth * pEncPara->stRc.PicHeight / (16 * 16);
    
    if (Mbtotal != 0)
    {
        ImbRatio = pEncPara->stRc.NumIMBCurFrm * 1000 / Mbtotal;
    }

    if (pEncPara->ICatchEnable)
    {
        pEncPara->FrmCnt++;

        if (pEncPara->FrmCnt > 30)
        {
            HI_U32 diff = 0, i = 0;
            ImbRatioAvg =  VENC_DRV_EflRcAverage(pEncPara->stRc.IMBRatioCurScen, 6);
            ImbRatioAvg = D_VENC_RC_MAX(ImbRatioAvg, 1);

            diff = D_VENC_RC_ABS(ImbRatioAvg, ImbRatio);

            if ((ImbRatio > ImbRatioAvg) && ((diff * 100 / ImbRatioAvg) > 200) && (ImbRatio > 500) )
            {
                pEncPara->InterFrmCnt = pEncPara->stRc.Gop - 1;
                pEncPara->ICatchOn = 1;
                pEncPara->stRc.IFrmInsertFlag = 1;
                pEncPara->stRc.RcStart = 1;

                for (i = 0; i < 6; i++)
                {
                    pEncPara->stRc.IMBRatioCurScen[i] = 0;
                }
            }
            else
            {
                D_VENC_RC_UPDATE(pEncPara->stRc.IMBRatioCurScen, ImbRatio);
            }
        }
        else
        {
            if (pEncPara->FrmCnt != 1)   //if it is I frame not update IMBRatioCurScen.
            {
                D_VENC_RC_UPDATE(pEncPara->stRc.IMBRatioCurScen, ImbRatio);
            }
        }
    }

    return;
}

static HI_VOID VENC_DRV_EncNextFrm(HI_U32 u32VeChn, VeduEfl_EncIn_S *pEncIn)
{
    VeduEfl_EncPara_S* EncHandle;

    if ((!g_stVencChn[u32VeChn].stProcWrite.bSaveYUVFileRun) || (g_stVencChn[u32VeChn].enSrcModId == HI_ID_BUTT))
    {

        /* next frame to encode */
        //VENC_DRV_OsalLock( VeduIpCtx.pChnLock, &flag);
        EncHandle = VENC_DRV_EflQueryChn_H264( pEncIn );

        //VENC_DRV_OsalUnlock( VeduIpCtx.pChnLock, &flag );
        if ( EncHandle != NULL )
        {

            VeduIpCtx.IpFree = 0;
            VeduIpCtx.CurrHandle = EncHandle;

            VENC_DRV_EflStartOneFrameVenc( EncHandle, pEncIn );
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

    return;
}

static HI_VOID Venc_ISR( HI_VOID )
{
    VeduEfl_EncIn_S EncIn;
    //VEDU_LOCK_FLAG  flag;
    HI_U32 u32VeChn;
	VeduEfl_EncPara_S *pEncPara;
	S_VEDU_REGS_TYPE *pAllReg;
	U_PERI_CRG35 unTmpValue;
	
    D_VENC_GET_CHN(u32VeChn, VeduIpCtx.CurrHandle);
	if (u32VeChn >= MAX_VEDU_CHN)
	{
	   return;
	}

    pEncPara = (VeduEfl_EncPara_S *) VeduIpCtx.CurrHandle;
    pAllReg  = (S_VEDU_REGS_TYPE *)pEncPara->stHal.pRegBase;

#if (defined HI_SMMU_SUPPORT) && ((defined __VENC_98CV200_CONFIG__)|(defined __VENC_98MV200_CONFIG__))
    Venc_Smmu_ISR(pEncPara);
#endif

#if (defined __VENC_98MV200_CONFIG__)
    if (pAllReg->VEDU_VCPI_REGS.VCPI_RAWINT.bits.vcpi_rint_vedu_timeout) //超时软复位
#else
	if (pAllReg->VEDU_RAWINT.bits.VeduTimeout)			   //超时软复位
#endif
    {
        HI_ERR_VENC("VENC timeout interrupt!! lowdelay= %d\n",pEncPara->stRc.LowDlyMod);
	
#if (defined __VENC_98MV200_CONFIG__)
        pAllReg->VEDU_VCPI_REGS.VCPI_INTCLR.u32 = 0x08000000;  //清中断
#else
		pAllReg->VEDU_INTCLR.u32 = 0x08000000;           //清中断 
#endif

		/* reset VEDU*/
		unTmpValue.bits.venc_srst_req = 1;
		g_pstRegCrg->PERI_CRG35.u32 = unTmpValue.u32;
		
	    /* open vedu clock */
#if ( defined(__VENC_98CV200_CONFIG__) || defined(__VENC_98MV200_CONFIG__) )
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

			if ((!pEncPara->stRc.LowDlyMod) || (!pEncPara->stHal.SlcSplitEn))
			{
			   VENC_DRV_DbgLowdelayEvent(u32VeChn,EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex,pEncPara->stImage.hTunnelSrc);  
			}
			bFirstSlc = HI_TRUE;
	    }
				
    } 

#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)| (defined __VENC_98CV200_CONFIG__) | (defined __VENC_98MV200_CONFIG__)) 
#if (defined __VENC_98MV200_CONFIG__)
	if (pAllReg->VEDU_VCPI_REGS.VCPI_RAWINT.bits.vcpi_rint_vedu_slice_end) //低延时模式下slice级中断处理
#else
	if (pAllReg->VEDU_RAWINT.bits.VeduSliceInt)			   //低延时模式下slice级中断处理
#endif
    {
		 if (bFirstSlc)
		 {
		    VENC_DRV_DbgLowdelayEvent(u32VeChn,EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex,pEncPara->stImage.hTunnelSrc);
			bFirstSlc = HI_FALSE;
		 }
		 
		 VENC_DRV_EflEndOneSliceVenc( VeduIpCtx.CurrHandle );
#if (defined __VENC_98MV200_CONFIG__)
        pAllReg->VEDU_VCPI_REGS.VCPI_INTCLR.u32 = 0x400;  //清中断 	 
#else
		 pAllReg->VEDU_INTCLR.u32 = 0x400;           //清中断 	 
#endif
    } 
#endif


#if (defined __VENC_98MV200_CONFIG__)
    if ( pAllReg->VEDU_VCPI_REGS.VCPI_RAWINT.bits.vcpi_rint_ve_eop ) //帧级中断
#else
    if ( pAllReg->VEDU_RAWINT.bits.VencEndOfPic )        //帧级中断
#endif
    {
         //printk("%s,%d, Frm Isr OK\n\n",__func__,__LINE__);
#ifdef __VENC_DRV_DBG__   
   if (Isr_ID < 100)
   {
       HI_DRV_SYS_GetTimeStampMs(&TmpTime_dbg[Isr_ID][2]);
	   Isr_ID++;
   }
#endif
	    /* release image encoded */
	    {
        /* postprocess frame encoded */
	    VENC_DRV_EflReadRegVenc    ( VeduIpCtx.CurrHandle );

#if (defined __VENC_98MV200_CONFIG__)
        pAllReg->VEDU_VCPI_REGS.VCPI_INTCLR.u32 = 0xFFFFFFFF;
#else
		pAllReg->VEDU_INTCLR.u32 = 0xFFFFFFFF;//0xFFFFFBFF;
#endif

 /* add for oversized frame recoding */
        pEncPara->stStat.RateControlRatio = VENC_DRV_CalRCRatio(pEncPara->stRc.InstBitsSum, pEncPara->stRc.BitRate);
        VEDU_DRV_RCUpdateInfo(&pEncPara->stRc);
#if RC_RECODE_USE
        if (VEDU_DRV_RCProcessOfRecoding(pEncPara, &EncIn) == HI_SUCCESS)
        {
            return;
        }
#endif
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

            if ((!pEncPara->stRc.LowDlyMod) || (!pEncPara->stHal.SlcSplitEn))
            {
                VENC_DRV_DbgLowdelayEvent(u32VeChn, EVENT_VENC_FRM_OUT, pEncPara->stImage.u32FrameIndex, pEncPara->stImage.hTunnelSrc);
            }

            bFirstSlc = HI_TRUE;
        }

        gencodeframe = 1;
        wake_up(&gEncodeFrame);

        VENC_DRV_EflEndOneFrameVenc( VeduIpCtx.CurrHandle );

        VENC_DRV_RcIFrmInsertForLargeImb(pEncPara);

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

        VENC_DRV_EncNextFrm(u32VeChn, &EncIn);

        VENC_DRV_OsalGiveEvent(&g_VencWait_Stream[u32VeChn]);

        if (g_stVencChn[u32VeChn].bOMXChn)
        {
            VENC_DRV_OsalGiveEvent(&g_VENC_Event_stream);
        }
    }
}

HI_S32 VEDU_DRV_RCProcessOfRecoding(VeduEfl_EncPara_S *pEncPara, VeduEfl_EncIn_S *EncIn)
{
	HI_S32 s32Ret = HI_FAILURE;

    if ((VEDU_DRV_RCIsNeedRecoding(&pEncPara->stRc) != HI_SUCCESS)
		||(VEDU_DRV_RCStartQPChange(&pEncPara->stRc) != HI_SUCCESS)
		||(VENC_DRV_GetEncodePara(pEncPara, EncIn) != HI_SUCCESS))
    {
		pEncPara->stRc.Recode = 0;
        s32Ret = HI_FAILURE;
    }
    else
    {
		pEncPara->stRc.H264FrmNum--;
		pEncPara->stRc.RcnIdx = !pEncPara->stRc.RcnIdx;
		pEncPara->stRc.PmeInfoIdx = pEncPara->stRc.PmeInfoIdx == 0 ? 2 : pEncPara->stRc.PmeInfoIdx - 1;
        VeduIpCtx.IpFree = 0;
        VENC_DRV_EflStartOneFrameVenc(pEncPara, EncIn);
        VENC_DRV_EflCfgRegVenc(pEncPara);
        s32Ret = HI_SUCCESS;
	}

    return s32Ret;
}

static HI_VOID VENC_DRV_GetLastFrmInfo(VeduEfl_EncPara_S* pEncPara, HI_U32 OmxPrivFlag,VeduEfl_EncIn_S* pEncIn)
{
#if ((defined __VENC_3716CV200_CONFIG__)|(defined __VENC_S5V100_CONFIG__)|(defined __VENC_98M_CONFIG__)|(defined __VENC_98CV200_CONFIG__)|(defined __VENC_98MV200_CONFIG__))             //for lowdelay
    if (pEncPara->stImage.u32TunnelPhyAddr)      //待明确!!
    {
        pEncPara->stRc.LowDlyMod = HI_TRUE;
        pEncIn->TunlCellAddr = pEncPara->stImage.u32TunnelPhyAddr;
    }
    else
    {
        pEncPara->stRc.LowDlyMod = HI_FALSE;
        pEncIn->TunlCellAddr = 0;
    }
#endif

    pEncIn->BusViY = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    pEncIn->BusViC = pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;

    if (pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncPara->stImage.stBufAddr[0].u32PhyAddr_Cr;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
    }

    if ((pEncPara->stHal.YuvStoreType == VENC_STORE_SEMIPLANNAR) && (pEncPara->YuvSampleType == VENC_YUV_422))  /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
        pEncIn->ViCStride = pEncPara->stImage.stBufAddr[0].u32Stride_C * 2;
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PACKAGE) && (pEncPara->YuvSampleType == VENC_YUV_422))
    {
        pEncIn->ViYStride = pEncPara->stImage.stBufAddr[0].u32Stride_Y;
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR) && (pEncPara->YuvSampleType == VENC_YUV_420))
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

    if (OmxPrivFlag)
    {
        pEncIn->PTS0 = (HI_U32)(pEncPara->stImage.s64OmxPts & 0x00000000ffffffffLL);
        pEncIn->PTS1 = (HI_U32)((pEncPara->stImage.s64OmxPts & 0xffffffff00000000LL) >> 32);        
    }
    else
    {
        pEncIn->PTS0 = pEncPara->stImage.u32Pts;
        pEncIn->PTS1 = 0;        
    }

    return ;
}

static HI_VOID VENC_DRV_GetLastFrmInfo_OMX(VeduEfl_EncPara_S* pEncPara, VeduEfl_EncIn_S* pEncIn)
{
    pEncPara->stRc.LowDlyMod = HI_FALSE;
    pEncIn->TunlCellAddr = 0;

    if (pEncPara->stImage_OMX.MetaDateFlag)   //storeMetaData case
    {
        pEncIn->BusViY = pEncPara->stImage_OMX.stMetaData.bufferaddr_Phy + pEncPara->stImage_OMX.offset; //pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    }
    else
    {
        pEncIn->BusViY = pEncPara->stImage_OMX.bufferaddr_Phy + pEncPara->stImage_OMX.offset; //pEncPara->stImage.stBufAddr[0].u32PhyAddr_Y;
    }
    
    pEncIn->BusViC = pEncIn->BusViY + pEncPara->stImage_OMX.offset_YC;//pEncPara->stImage.stBufAddr[0].u32PhyAddr_C;

    if (pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR)
    {
        pEncIn->BusViV = pEncIn->BusViY + pEncPara->stImage_OMX.offset_YCr;//pEncPara->stImage_OMX.strideC*pEncPara->stImage_OMX.picHeight /2;//pEncPara->stImage.u32CAddr;      //目前帧信息结构体缺少该结构,planer格式时需要；
    }

    if ((pEncPara->stHal.YuvStoreType == VENC_STORE_SEMIPLANNAR) && (VENC_YUV_422 == pEncPara->YuvSampleType)) /*==强制把SEMIPLANAR_422 当semiplaner 420编码*/
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC * 2;
        pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_422;
    }
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PACKAGE) && (VENC_YUV_422 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;

        switch (pEncPara->stHal.PackageSel)
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
    else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR) && (VENC_YUV_420 == pEncPara->YuvSampleType))
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC;
        pEncPara->stStat.u32FrameType = VENC_PLANNAR_420;
    }
    else
    {
        pEncIn->ViYStride = pEncPara->stImage_OMX.strideY;
        pEncIn->ViCStride = pEncPara->stImage_OMX.strideC;

        if ((pEncPara->stHal.YuvStoreType == VENC_STORE_PLANNAR) && (VENC_YUV_422 == pEncPara->YuvSampleType))
        {
            pEncPara->stStat.u32FrameType = VENC_PLANNAR_422;
        }
        else if ((pEncPara->stHal.YuvStoreType == VENC_STORE_SEMIPLANNAR) && (VENC_YUV_420 == pEncPara->YuvSampleType))
        {
            if (pEncPara->stHal.PackageSel == 0)
            {
                pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_420_VU;
            }
            else
            {
                pEncPara->stStat.u32FrameType = VENC_SEMIPLANNAR_420_UV;
            }
        }

    }

    pEncIn->RStrideY  = D_VENC_ALIGN_UP(pEncPara->stRc.PicWidth, 16);
    pEncIn->RStrideC  = pEncIn->RStrideY;

    pEncIn->PTS0 = pEncPara->stImage_OMX.timestamp0;
    pEncIn->PTS1 = pEncPara->stImage_OMX.timestamp1;

    return;
}

HI_S32 VENC_DRV_GetEncodePara(VeduEfl_EncPara_S* pEncPara, VeduEfl_EncIn_S* pEncIn)
{
    if (pEncPara->OMXChn)
    {
        if (pEncPara->WithoutVPSS) /*without VPSS*/
        {
            VENC_DRV_GetLastFrmInfo_OMX(pEncPara, pEncIn);
        }
        else
        {
            VENC_DRV_GetLastFrmInfo(pEncPara, HI_TRUE, pEncIn);
        }
    }
    else
    {
        VENC_DRV_GetLastFrmInfo(pEncPara, HI_FALSE, pEncIn);
    }   

    return HI_SUCCESS;
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

static HI_VOID VENC_DRV_OMX_GetImgFromVPSS(VeduEfl_EncPara_S *pEncPara)
{
    HI_U32 i = 0;
    HI_S32 s32Ret = 0;

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

}

static HI_VOID VENC_DRV_EflTask( HI_VOID )
{
	HI_S32 s32Ret = 0;
    VeduEfl_EncIn_S EncIn;

#ifdef __VENC_SUPPORT_JPGE__	
	Venc2Jpge_EncIn_S JpgeEncIn;
    HI_BOOL  bJpgeBufFull = HI_FALSE;
#endif
	
    VeduEfl_EncPara_S *pEncPara = HI_NULL;
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
        pEncPara = (VeduEfl_EncPara_S *)g_stVencChn[i].hVEncHandle;
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
                    D_VENC_GET_CHN(u32ChnID,pEncPara); 
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

#ifdef __VENC_SUPPORT_JPGE__
        {
            pEncPara = VENC_DRV_EflQueryChn_JPEG(&EncIn);

            if (pEncPara != NULL)
            {
                D_VENC_GET_CHN(u32ChnID, pEncPara);

                if (u32ChnID >= VENC_MAX_CHN_NUM)
                {
                    continue;
                }

                if (HI_INVALID_HANDLE != g_stVencChn[u32ChnID].hJPGE)  /*JPGE CHANNEL*/
                {
                    pEncPara->stHal.PTS0 = EncIn.PTS0;
                    pEncPara->stHal.PTS1 = EncIn.PTS1;

                    JPGE_MakeJFIF(pEncPara);

                    JpgeEncIn.BusViY = EncIn.BusViY;
                    JpgeEncIn.BusViC = EncIn.BusViC;
                    JpgeEncIn.BusViV = EncIn.BusViV;
                    JpgeEncIn.ViYStride = EncIn.ViYStride;
                    JpgeEncIn.ViCStride = EncIn.ViCStride;

                    JpgeEncIn.StrmBufAddr   = pEncPara->stHal.StrmBufAddr;
                    JpgeEncIn.StrmBufRpAddr = pEncPara->stHal.StrmBufRpAddr;
                    JpgeEncIn.StrmBufWpAddr = pEncPara->stHal.StrmBufWpAddr;
                    JpgeEncIn.StrmBufSize = pEncPara->stHal.StrmBufSize;
                    JpgeEncIn.Vir2BusOffset = pEncPara->Vir2BusOffset;
                    JpgeEncIn.pJpgeYqt      = pEncPara->pJpgeYqt;
                    JpgeEncIn.pJpgeCqt      = pEncPara->pJpgeCqt;
                    JpgeEncIn.FrameHeight   = pEncPara->stRc.PicHeight;
                    JpgeEncIn.FrameWidth    = pEncPara->stRc.PicWidth;
                    JpgeEncIn.YuvSampleType = pEncPara->YuvSampleType;
                    JpgeEncIn.PTS0          = EncIn.PTS0;

                    switch (pEncPara->stHal.YuvStoreType)
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

                    JpgeEncIn.PackageSel = pEncPara->stHal.PackageSel;
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

                    /*release the Img*/
                    pEncPara->stStat.PutFrameNumTry++;
                    (pEncPara->stSrcInfo.pfPutImage)(pEncPara->stSrcInfo.handle, &pEncPara->stImage);

                    if (g_stVencChn[u32ChnID].enSrcModId >= HI_ID_BUTT)
                    {
                        pEncPara->stStat.VpssImgBufNum--;

                    }

                    pEncPara->stRc.RcStart  = 0;
                    pEncPara->TrCount -= pEncPara->stRc.ViFrmRate;
                    pEncPara->stStat.PutFrameNumOK++;

                    //VeduIpCtx.IpFree = 1;  /*NEXT*/

                    pEncPara->WaitingIsr_jpge = 0;
                }
            }

        }
#endif
            /* just for omx_venc channel which with vpss */
            VENC_DRV_OMX_GetImgFromVPSS(pEncPara);

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

static VeduEfl_EncPara_S* VENC_DRV_EflQueryChn_Stream(venc_msginfo* pMsgInfo)
{
    HI_U32 u32StartQueryNo = 0;
    HI_S32 s32StartChnID   = 0;     /*this ID correspond to VeduChnCtx(class:VeduEfl_ChnCtx_S) */
	HI_U32 u32ChnID = 0;            /*this ID correspond to g_stVencChn(class:OPTM_VENC_CHN_S)*/
    VEDU_LOCK_FLAG flag;
    VeduEfl_EncPara_S *pEncPara = NULL;
    VeduEfl_EncPara_S* hVencHandle = HI_NULL;
	queue_data_s Queue_Data;

	VENC_DRV_OsalLock(VeduIpCtx.pChnLock, &flag);
    for (u32StartQueryNo = 0; u32StartQueryNo < MAX_VEDU_CHN; u32StartQueryNo++)
    {      
        D_VENC_GET_CHN_FROM_TAB(s32StartChnID,u32StartQueryNo);	
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

		pEncPara->stStat.GetStreamNumTry++;
        if( VENC_DRV_EflGetStreamLen(hVencHandle) > 0 )    /*fine one channel have stream*/
        {
            if( VENC_DRV_MngQueueEmpty(pEncPara->StreamQueue_OMX))
            { continue; }
			
			if( VENC_DRV_MngDequeue(pEncPara->StreamQueue_OMX, &Queue_Data))
            { continue; }
 
			pEncPara->StreamQueue_OMX->bStrmLock = 1;   //mutex with the stop processing add by l00228308
            pEncPara->stStat.StreamQueueNum--;
			if (Queue_Data.bToOMX)
			{
			    memcpy(pMsgInfo,&(Queue_Data.unFrmInfo.msg_info_omx),sizeof(venc_msginfo));
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


static HI_S32 VENC_DRV_OMXOutputCheck(HI_U32 Protocol, HI_U32 NaluType, HI_U32 PackageFlag)
{
    HI_S32 ret = HI_FAILURE;

    switch (PackageFlag)
    {
        case OMX_PACKAGE_ONEINONEOUT:
            if (Protocol == VEDU_H265)
            {
                if ((NaluType != H265_NALU_TYPE_VPS) 
                    && (NaluType != H265_NALU_TYPE_SPS)
                    && (NaluType != H265_NALU_TYPE_PPS))
                {
                    ret = HI_SUCCESS;
                }                
            }
            else 
            {
                /*H264*/
                if ((NaluType != H264_NALU_TYPE_SPS) && (NaluType != H264_NALU_TYPE_PPS))
                {
                    ret = HI_SUCCESS;
                }
            }
            break;  
            
        case OMX_PACKAGE_SPLIT:
            if (Protocol == VEDU_H265)
            {
                if ((NaluType != H265_NALU_TYPE_VPS) 
                    && (NaluType != H265_NALU_TYPE_SPS))
                {
                    ret = HI_SUCCESS;
                }                
            }
            else 
            {
                /*H264*/
                if ((NaluType != H264_NALU_TYPE_SPS))
                {
                    ret = HI_SUCCESS;
                }
            }            
            break;   
            
        case OMX_PACKAGE_ALONE:
            ret = HI_SUCCESS;
            break;
            
        default:
            HI_ERR_VENC("not support this PackageFlag = %d\n",PackageFlag);
            ret = HI_SUCCESS;
            break;
    }

    return ret;
}

static HI_VOID H264_GetStrmBufFlag(HI_U32 NaluType, HI_BOOL *pFirstNal2Send, venc_user_buf *pStreamBuf)
{
    switch (NaluType)
    {
        case H264_NALU_TYPE_IDR:
#ifdef __OMXVENC_ONE_IN_ONE_OUT__
            if (*pFirstNal2Send)
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
                *pFirstNal2Send = HI_FALSE;
            }
            else
#endif
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
            }
            break;

        case H264_NALU_TYPE_P:
            pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
            break;

        case H264_NALU_TYPE_PPS:
            if (*pFirstNal2Send)
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                *pFirstNal2Send = HI_FALSE;
            }
            else
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_EXTRADATA;
            }
            break;

        default:
            HI_ERR_VENC("Nalu.NaluType(%d) is invalid\n",NaluType);
            break;
    }        
}

static HI_VOID H265_GetStrmBufFlag(HI_U32 NaluType, HI_BOOL *pFirstNal2Send, venc_user_buf *pStreamBuf)
{
    switch (NaluType)
    {
        case H265_NALU_TYPE_IDR:
#ifdef __OMXVENC_ONE_IN_ONE_OUT__
            if (*pFirstNal2Send)
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
                *pFirstNal2Send = HI_FALSE;
            }
            else
#endif
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_SYNCFRAME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
            }
            break;

        case H265_NALU_TYPE_P:
            pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_ENDOFFRAME;
            break;

        case H265_NALU_TYPE_PPS:
            if (*pFirstNal2Send)
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_STARTTIME;
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_CODECCONFIG;
                *pFirstNal2Send = HI_FALSE;
            }
            else
            {
                pStreamBuf->flags |=  OMXVENC_BUFFERFLAG_EXTRADATA;
            }
            break;

        default:
            HI_ERR_VENC("Nalu.NaluType(%d) is invalid\n",NaluType);
            break;
    }        
}

static HI_VOID VENC_DRV_OMXGetStrmBufFlag(HI_U32 Protocol, HI_U32 NaluType, HI_BOOL *pFirstNal2Send, venc_user_buf *pStreamBuf)
{
    if (Protocol == VEDU_H264)
    {
        H264_GetStrmBufFlag(NaluType, pFirstNal2Send, pStreamBuf);
    }
    else
    {
        H265_GetStrmBufFlag(NaluType, pFirstNal2Send, pStreamBuf);
    }
}

static HI_VOID VENC_DRV_EflTask_Stream( HI_VOID )
{
    VeduEfl_EncPara_S* EncHandle;
    VeduEfl_NALU_S Nalu ={{NULL,NULL}};
    VeduEfl_EncPara_S *pEncPara = HI_NULL;
    HI_U32 i = 0;
    HI_S32 s32Ret = 0,s32Ret2 = 0;
    HI_BOOL bTmpValue = HI_FALSE;
    venc_user_buf StreamBuf;
    venc_msginfo msg_info;
    HI_VOID* venc_stream_addr = NULL;
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

	//VENC_DRV_OsalInitEvent(&g_VENC_Event_stream, 0);
	
    while (!VeduIpCtx.StopTask)
    {
        /* if ipfree, don't irqlock */
        EncHandle = VENC_DRV_EflQueryChn_Stream(&msg_info);
        if (EncHandle != NULL)
        {   
            pEncPara = EncHandle;
          
           memcpy(&StreamBuf,&(msg_info.buf),sizeof(venc_user_buf));
           venc_stream_addr = (HI_VOID*)(StreamBuf.bufferaddr_Phy+StreamBuf.vir2phy_offset);
            do
            {
                if ( VENC_DRV_EflGetStreamLen(EncHandle) > 0)
                {
                    memset(&Nalu, 0, sizeof(VeduEfl_NALU_S));
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
            while ((!s32Ret) 
                && (VENC_DRV_OMXOutputCheck(pEncPara->Protocol, Nalu.NaluType, OMX_PACKAGE_ONEINONEOUT) == HI_FAILURE));
#else
            }
            while ((!s32Ret) 
                && (VENC_DRV_OMXOutputCheck(pEncPara->Protocol, Nalu.NaluType, OMX_PACKAGE_SPLIT) == HI_FAILURE));  
#endif

            VENC_DRV_OMXGetStrmBufFlag(pEncPara->Protocol, Nalu.NaluType, &pEncPara->bFirstNal2Send, &StreamBuf);

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
               HI_INFO_VENC("wait stream time out! %d\n",__LINE__);
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

    //VENC_DRV_OsalLockDestroy( VeduIpCtx.pChnLock );

    return ;
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
    VeduIpCtx.pRegBase = ioremap( VEDU_REG_BASE_ADDR, D_VENC_ALIGN_UP(sizeof(S_VEDU_REGS_TYPE),256) );

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
    VeduEfl_NaluHdr_S  *pNaluHdr;
    VeduEfl_EncPara_S*  pEncPara  = EncHandle;
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;
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
    pNalu->bFrameEnd = (pNaluHdr->bLastSlice) & 1;
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
        memcpy((pNalu->pVirtAddr[0]+pNalu->SlcLen[0]),pNalu->pVirtAddr[1],pNalu->SlcLen[1]);
    }
    return HI_SUCCESS;
}

HI_S32 VENC_DRV_EflGetBitStream( VeduEfl_EncPara_S* EncHandle, VeduEfl_NALU_S* pNalu )
{
    VEDU_LOCK_FLAG flag;
    HI_S32 ret;

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflGetBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

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
    VALG_CRCL_BUF_S    *pstStrBuf = &pEncPara->stCycBuf;

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

    VENC_DRV_OsalLock  (((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);
    ret = VENC_DRV_EflSkpBitStream_X( EncHandle, pNalu );
    VENC_DRV_OsalUnlock(((VeduEfl_EncPara_S *)EncHandle)->pStrmBufLock, &flag);

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
    VeduEfl_EncPara_S  *pEncPara = NULL; 
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
    memcpy(&(Queue_data.unFrmInfo.queue_info),pstFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
    s32Ret = VENC_DRV_MngQueue(pEncPara->FrameQueue,&Queue_data,0,0);
    if (HI_SUCCESS == s32Ret)
    {
       pEncPara->stStat.QueueNum++;
       VENC_DRV_EflWakeUpThread();
	   
    }
    return s32Ret;
}

HI_S32 VENC_DRV_EflQFrameByAttach( HI_U32 EncUsrHandle, HI_DRV_VIDEO_FRAME_S *pstFrame)
{
    HI_S32 s32ChIndx = 0;
    HI_S32 s32Ret = 0;
    unsigned long flags;

    while (s32ChIndx < VENC_MAX_CHN_NUM)
    {
        if (g_stVencChn[s32ChIndx].hUsrHandle == EncUsrHandle)
        {
            break;
        }

        s32ChIndx++;
    }

    if (g_stVencChn[s32ChIndx].hVEncHandle == NULL)
    {
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    if (VENC_MAX_CHN_NUM == s32ChIndx)
    {
        HI_ERR_VENC(" bad handle!!\n");

        return HI_ERR_VENC_CHN_NOT_EXIST;
    }

    if (g_stVencChn[s32ChIndx].enSrcModId >= HI_ID_BUTT)
    {
        return HI_ERR_VENC_CHN_NO_ATTACH;
    }

    spin_lock_irqsave(&g_SendFrame_Lock[s32ChIndx], flags);
    s32Ret = VENC_DRV_EflQueueFrame(g_stVencChn[s32ChIndx].hVEncHandle, pstFrame);
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
    VeduEfl_EncPara_S  *pEncPara = NULL;
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
	   memcpy(pstFrame,&(Queue_data.unFrmInfo.queue_info),sizeof(HI_DRV_VIDEO_FRAME_S));
	}
	pEncPara->stStat.DequeueNum--;
	return s32Ret;	
}


HI_S32 VENC_DRV_EflRlsAllFrame( VeduEfl_EncPara_S* EncHandle)
{
    VEDU_LOCK_FLAG flag;
    //HI_DRV_VIDEO_FRAME_S stFrame ;
    VeduEfl_EncPara_S  *pEncPara = NULL;
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
      VeduEfl_EncPara_S  *pEncPara;
  	
      HI_U32 u32ChnID;
	  HI_S32 s32Ret = 0;
      HI_BOOL bVPSSAllDone = HI_FALSE;
      D_VENC_GET_CHN(u32ChnID,EncHandle);
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
    VeduEfl_EncPara_S *pEncPara;
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
	   memcpy(pstFrame,&(Queue_data.unFrmInfo.queue_info),sizeof(HI_DRV_VIDEO_FRAME_S));
	}	
	pEncPara->stStat.QueueNum--;
	return s32Ret;
}

HI_S32 VENC_DRV_EflGetImage_OMX(HI_S32 EncUsrHandle, venc_user_buf* pstFrame)
{
    HI_U32 u32ChnID;
    HI_S32 s32Ret;
    VeduEfl_EncPara_S  *pEncPara;
	queue_data_s  Queue_data;
    
    D_VENC_GET_CHN_BY_UHND(u32ChnID, EncUsrHandle);
	D_VENC_CHECK_CHN(u32ChnID);
    pEncPara = g_stVencChn[u32ChnID].hVEncHandle;

    if( VENC_DRV_MngQueueEmpty(pEncPara->FrameQueue_OMX))
    {
       return HI_FAILURE;
    }
	s32Ret = VENC_DRV_MngDequeue(pEncPara->FrameQueue_OMX, &Queue_data);
    if( !Queue_data.bToOMX || s32Ret)
    {
	   HI_INFO_VENC("err:not match for OMX!! ret = %d\n",s32Ret);
       return s32Ret;
    }
	else
	{
	   memcpy(pstFrame,&(Queue_data.unFrmInfo.msg_info_omx.buf),sizeof(venc_user_buf));
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
    VeduEfl_EncPara_S *pEncPara;
	queue_data_s  Queue_data; 
    D_VENC_GET_CHN_BY_UHND(u32ChnID, EncUsrHandle);
	D_VENC_CHECK_CHN(u32ChnID);
	
    pEncPara = g_stVencChn[u32ChnID].hVEncHandle;

	Queue_data.bToOMX = 0;
	memcpy(&(Queue_data.unFrmInfo.queue_info),pstFrame,sizeof(HI_DRV_VIDEO_FRAME_S));
	s32Ret = VENC_DRV_MngQueue(pEncPara->FrameDequeue, &Queue_data,0,0);
	if (!s32Ret)
	{
	   pEncPara->stStat.DequeueNum++;
	}
	return s32Ret;

}

#if ( defined(__VENC_98CV200_CONFIG__) || defined(__VENC_98MV200_CONFIG__) )  
#ifdef HI_SMMU_SUPPORT
HI_U32 VeduEfl_SmmuToPhyAddr(HI_U32 PageTabBaseAddr ,HI_U32 SmmuAddr  )
{
   //HI_U32 PageTabBaseAddr = 0;
   HI_U32 PageIndex = 0;
   HI_U32 PageOffset = 0;
   HI_U32 PhyAddr = 0;
   HI_U32  *pPage;
   HI_U32 Hige20Bit,Low12Bit;
   //PageTabBaseAddr = pAllReg->SMMU_CB_TTBR;

   HI_INFO_VENC("%s,%d,PageTabBaseAddr = 0x%x,smmuAddr = 0x%x\n",__func__,__LINE__,PageTabBaseAddr,SmmuAddr);

   if (PageTabBaseAddr == 0)
   {
        HI_ERR_VENC("%s,%d, pageTabBaseAddr = %d Error!\n",__func__,__LINE__,PageTabBaseAddr);
		return -1;
   }

   PageOffset = SmmuAddr / 4096 * 4;//one phypage has 4k bytes memory, its length is 4 byte

   PageIndex = PageTabBaseAddr + PageOffset;
   HI_INFO_VENC("%s,%d,PageIndex = 0x%x\n",__func__,__LINE__,PageIndex);
   
   pPage = (HI_U32 *)phys_to_virt(PageIndex);//(HI_U32 *)ioremap(PageIndex, 4);

   Hige20Bit = ((*pPage) & 0xfffff000);
   HI_INFO_VENC("%s,%d,Hige20Bit = 0x%x(0x%x)\n",__func__,__LINE__,Hige20Bit,*pPage);
   
   Low12Bit = SmmuAddr & 0xfff;
   HI_INFO_VENC("%s,%d,Low12Bit = 0x%x\n",__func__,__LINE__,Low12Bit);

   PhyAddr = Hige20Bit + Low12Bit;
  
   HI_INFO_VENC("\n%s,%d,~ PhyAddr = 0x%x\n",__func__,__LINE__,PhyAddr);
   return PhyAddr;
}
#endif
#endif


/////////////////////////////////////////////////////////////////debug
HI_S32 VENC_DRV_DbgWriteYUV(HI_DRV_VIDEO_FRAME_S *pstFrame,HI_CHAR *pFileName)
{
	char str[64] = {0};
	unsigned char *ptr;
	struct file *fp;

    HI_U8 *pu8Udata;
    HI_U8 *pu8Vdata;
    HI_U8 *pu8Ydata;
    HI_S8  s_VencSavePath[64];
    HI_U32 i,j;
#ifdef HI_SMMU_SUPPORT
    VENC_BUFFER_S stMBuf;
    HI_S32 Ret;

    memset(&stMBuf, 0, sizeof(VENC_BUFFER_S));
#endif
    HI_DRV_LOG_GetStorePath(s_VencSavePath, 64);
	
    pu8Udata = HI_VMALLOC(HI_ID_VENC,pstFrame->u32Width * pstFrame->u32Height / 2 /2);
	if (pu8Udata == HI_NULL)
    {
        return HI_FAILURE;
    }
    pu8Vdata = HI_VMALLOC(HI_ID_VENC,pstFrame->u32Width * pstFrame->u32Height / 2 /2);
    if (pu8Vdata == HI_NULL)
    {
        HI_VFREE(HI_ID_VENC,pu8Udata);
        return HI_FAILURE;
    }	
    pu8Ydata = HI_VMALLOC(HI_ID_VENC,pstFrame->stBufAddr[0].u32Stride_Y);
    if (pu8Ydata == HI_NULL)
    {
        HI_VFREE(HI_ID_VENC,pu8Udata);
        HI_VFREE(HI_ID_VENC,pu8Vdata);
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
    ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_Y);
#endif
	if (!ptr)
	{
        HI_ERR_VENC("address is not valid!\n");
	}
	else
	{   
	    HI_OSAL_Snprintf(str, 64, "%s/%s", s_VencSavePath,pFileName);

        fp = VENC_DRV_OsalFopen(str, O_RDWR | O_CREAT|O_APPEND, 0);
        if (fp == HI_NULL)
        {
            HI_ERR_VENC("open file '%s' fail!\n", str);
            HI_VFREE(HI_ID_VENC,pu8Udata);
            HI_VFREE(HI_ID_VENC,pu8Vdata);
            HI_VFREE(HI_ID_VENC,pu8Ydata);
        #ifdef HI_SMMU_SUPPORT
            HI_DRV_VENC_Unmap(&stMBuf);
        #endif
            return HI_FAILURE;
        }

        /*写 Y 数据*/
        for (i=0; i<pstFrame->u32Height; i++)
        {
            memcpy(pu8Ydata,ptr,sizeof(HI_U8)*pstFrame->stBufAddr[0].u32Stride_Y);
            if(pstFrame->u32Width != VENC_DRV_OsalFwrite(pu8Ydata,pstFrame->u32Width, fp))
      	    {
                HI_ERR_VENC("fwrite fail!\n");
            }
            ptr += pstFrame->stBufAddr[0].u32Stride_Y;
        }

#ifdef HI_SMMU_SUPPORT
        ptr = stMBuf.pu8StartVirAddr + (pstFrame->stBufAddr[0].u32PhyAddr_C - pstFrame->stBufAddr[0].u32PhyAddr_Y);
#else
        ptr = (unsigned char *)phys_to_virt(pstFrame->stBufAddr[0].u32PhyAddr_C);		
#endif
		
        /* U V 数据 转存*/
        for (i=0; i<pstFrame->u32Height/2; i++)
        {
            for (j=0; j<pstFrame->u32Width/2; j++)
            {
                if(pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV21)
                {
                    pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                    pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                }
                else if (pstFrame->ePixFormat == HI_DRV_PIX_FMT_NV12)
                {
                    pu8Udata[i*pstFrame->u32Width/2+j] = ptr[2*j];
                    pu8Vdata[i*pstFrame->u32Width/2+j] = ptr[2*j+1];
                }
                else
                {
                    HI_ERR_VENC("other pix formet= %d\n",pstFrame->ePixFormat);
                }
            }
            ptr += pstFrame->stBufAddr[0].u32Stride_C;
        }
        /*写 U */
        VENC_DRV_OsalFwrite(pu8Udata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);

        /*写 V */
        VENC_DRV_OsalFwrite(pu8Vdata, pstFrame->u32Width * pstFrame->u32Height / 2 /2, fp);

        VENC_DRV_OsalFclose(fp);
        HI_INFO_VENC("2d image has been saved to '%s' W=%d H=%d Format=%d \n", 
                    str,pstFrame->u32Width,pstFrame->u32Height,pstFrame->ePixFormat);
    }

    HI_VFREE(HI_ID_VENC,pu8Udata);
    HI_VFREE(HI_ID_VENC,pu8Vdata);
    HI_VFREE(HI_ID_VENC,pu8Ydata);
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
