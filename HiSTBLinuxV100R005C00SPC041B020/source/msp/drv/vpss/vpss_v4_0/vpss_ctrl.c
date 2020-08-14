#include "vpss_ctrl.h"
#include "vpss_common.h"
#include "hi_drv_proc.h"
#ifdef VPSS_HAL_WITH_CBB
#include "vpss_cbb_common.h"
#include "vpss_cbb_component.h"
#include "vpss_cbb_intf.h"
#include "vpss_cbb_hal.h"
#include "vpss_cbb_alg.h"
#endif
#ifdef VPSS_SUPPORT_PROC_V2
#include "vpss_cbb_reg.h"
#include "vpss_cbb_common.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef VPSS_SUPPORT_PROC_V2
#define COLOR_NORMAL            "\033[0m"
#define COLOR_BLACK             "\033[30m"
#define COLOR_GRAY              "\033[1;30m"
#define COLOR_RED               "\033[1;31m"
#define COLOR_GREEN             "\033[1;32m"
#define COLOR_YELLOW            "\033[1;33m"
#define COLOR_BLUE              "\033[1;34m"
#define COLOR_MAGENTA           "\033[1;35m"
#define COLOR_AQUA              "\033[1;36m"
#define COLOR_WHITE             "\033[1;37m"
#define BLINK                   "\033[5m"
#define VPSS_PROC_GAP "------------------------------------------------------------------------------------------------------------------"
#define VPSS_PROC_LEVEL1 "\n├─"
#define VPSS_PROC_LEVEL2 "\n│  ├─"
#define VPSS_PROC_LEVEL3 "\n│  │  ├─"
#define VPSS_PROC_LEVEL4 "\n│  │  │  ├─"
#define VPSS_PROC_LEVEL5 "\n│  │  │  │  "
#define VPSS_PROC_PRINT_ENUM(val, max, valstr) (HI_CHAR*)(((val) < (max)) ? valstr[(val)] : (HI_U8*)"Invalid")
#define VPSS_PROC_PRINT_YN(cdt) ((HI_TRUE == (cdt)) ? "Y" : "N")
#define VPSS_PROC_PRINT_YN_COL(cdt) ((HI_TRUE == (cdt)) ? COLOR_RED"Y"COLOR_NORMAL : "N")
#endif
#if DEF_VPSS_STATIC
HI_U32 g_u32LogicStart = 0;
HI_U32 g_u32LogicEnd = 0;

HI_U32 g_u32LogicTime[15];
HI_U32 g_u32LogicCycle[2][15];
HI_U32 g_u32Pos = 0;
#endif
HI_U8 *g_pAlgModeString[4] = {
    "off",
    "on",
    "auto",
    "butt",
};
HI_U8 *g_pInstState[3] = {
    "stop",
    "working",
    "butt",
};
HI_U8 *g_pProgDetectString[4] = {
    "P",
    "I",
    "auto",
    "butt",
};
HI_U8 *g_pRotationString[5] = {
    "00",
    "90",
    "180",
    "270",
    "butt",
};
HI_U8 *g_pDeiString[9] = {
    "off",
    "auto",
    "2 field",
    "3 field",
    "4 field",
    "5 field",
    "6 field",
    "7 field",
    "butt",
};

HI_U8 *g_pSrcMutualString[3] = {
    "src active",
    "vpss active",
    "butt",
};

HI_U8 *g_pPixString[14] = {
    "YCbCr420",
    "YCrCb420",
    "YCbCr411",
    "YCbCr422",
    "YCrCb422",
    "YCbCr422_2X1",
    "YCrCb422_2X1",
    "YCbCr420_cmp",
    "YCrCb420_cmp",
    "YCbCr422_cmp",
    "YCrCb422_cmp",
    "YCbCr422_2X1_cmp",
    "YCrCb422_2X1_cmp",
    "butt",
};

HI_U8 *g_pAspString[8] = {
    "Full",
    "LBOX",
    "PANSCAN",
    "COMBINED",
    "FULL_H",
    "FULL_V",
    "CUSTOMER",
    "butt",
};

HI_U8 *g_pSrcModuleString[10] = {
    "Vdec",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Unknow",
    "Vi",
    "Venc",
    "Unknow"
};

HI_U8 *g_pBufTypeString[3] = {
    "vpss",
    "usr",
    "unknow",
};
HI_U8 *g_pRotateString[5] = {
    "Rotation_00",
    "Rotation_90",
    "Rotation_180",
    "Rotation_270",
    "Rotation_butt",
};
HI_U8 *g_pCscString[20] = {
    "UNKNOWN",
    "DEFAULT",
    "BT601_YUV",
    "BT601_YUV",
    "BT601_RGB",
    "BT601_RGB",
    "NTSC1953",
    "BT470_SYSTEM_M",
    "BT470_SYSTEM_BG",
    "BT709_YUV",
    "BT709_YUV",
    "BT709_RGB",
    "BT709_RGB",
    "REC709",
    "SMPT170M",
    "SMPT240M",
    "BT878",
    "XVYCC",
    "JPEG",
    "BUTT",
};
HI_U8 *g_pOutBitWidthString[] = {
    "8BIT",
    "10BIT",
    "12BIT",
    "10BIT_CTS"
    "BUTT",
};
#ifdef VPSS_SUPPORT_PROC_V2
#define NUM_2_STRING(num) (#num)
HI_U8 *g_pcPixelFormatString[] =
{
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB332  ),  /*  8  RGB-3-3-2     */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB444  ),     /* 16  xxxxrrrr ggggbbbb */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB555  ),     /* 16  RGB-5-5-5     */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB565  ),     /* 16  RGB-5-6-5     */
    NUM_2_STRING(HI_DRV_PIX_FMT_BGR565  ),    /* 16  RGB-5-6-5     */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB555X ),    /* 16  RGB-5-5-5 BE  */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB565X ),    /* 16  RGB-5-6-5 BE  */
    NUM_2_STRING(HI_DRV_PIX_FMT_BGR666  ),    /* 18  BGR-6-6-6   */
    NUM_2_STRING(HI_DRV_PIX_FMT_BGR24   ),    /* 24  BGR-8-8-8     */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB24   ),    /* 24  RGB-8-8-8     */
    NUM_2_STRING(HI_DRV_PIX_FMT_BGR32   ),    /* 32  BGR-8-8-8-8   */
    NUM_2_STRING(HI_DRV_PIX_FMT_RGB32   ),    /* 32  RGB-8-8-8-8   */
    NUM_2_STRING(HI_DRV_PIX_FMT_CLUT_1BPP),
    NUM_2_STRING(HI_DRV_PIX_FMT_CLUT_2BPP),
    NUM_2_STRING(HI_DRV_PIX_FMT_CLUT_4BPP),
    NUM_2_STRING(HI_DRV_PIX_FMT_CLUT_8BPP),
    NUM_2_STRING(HI_DRV_PIX_FMT_ACLUT_44),
    NUM_2_STRING(HI_DRV_PIX_FMT_ACLUT_88),
    NUM_2_STRING(HI_DRV_PIX_FMT_ARGB4444),
    NUM_2_STRING(HI_DRV_PIX_FMT_ABGR4444),
    NUM_2_STRING(HI_DRV_PIX_FMT_RGBA4444),
    NUM_2_STRING(HI_DRV_PIX_FMT_ARGB1555),
    NUM_2_STRING(HI_DRV_PIX_FMT_ABGR1555),
    NUM_2_STRING(HI_DRV_PIX_FMT_RGBA5551),
    NUM_2_STRING(HI_DRV_PIX_FMT_ARGB8565),
    NUM_2_STRING(HI_DRV_PIX_FMT_ABGR8565),
    NUM_2_STRING(HI_DRV_PIX_FMT_RGBA5658),
    NUM_2_STRING(HI_DRV_PIX_FMT_ARGB6666),
    NUM_2_STRING(HI_DRV_PIX_FMT_RGBA6666),
    NUM_2_STRING(HI_DRV_PIX_FMT_ARGB8888),
    NUM_2_STRING(HI_DRV_PIX_FMT_ABGR8888),
    NUM_2_STRING(HI_DRV_PIX_FMT_RGBA8888),
    NUM_2_STRING(HI_DRV_PIX_FMT_AYUV8888),
    NUM_2_STRING(HI_DRV_PIX_FMT_YUVA8888),
    NUM_2_STRING(HI_DRV_PIX_FMT_GREY    ),     /*  8  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y4      ),     /*  4  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y6      ),     /*  6  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y10     ),     /* 10  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y12     ),     /* 12  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y16     ),     /* 16  Greyscale     */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y10BPACK    ),    /* 10  Greyscale bit-packed */
    NUM_2_STRING(HI_DRV_PIX_FMT_PAL8    ),     /*  8  8-bit palette */
    NUM_2_STRING(HI_DRV_PIX_FMT_YVU410  ),     /*  9  YVU 4:1:0     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YVU420  ),     /* 12  YVU 4:2:0     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUYV    ),     /* 16  YUV 4:2:2     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YYUV    ),     /* 16  YUV 4:2:2     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YVYU    ),     /* 16  YVU 4:2:2 */
    NUM_2_STRING(HI_DRV_PIX_FMT_UYVY    ),     /* 16  YUV 4:2:2     */
    NUM_2_STRING(HI_DRV_PIX_FMT_VYUY    ),     /* 16  YUV 4:2:2     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV422P ),     /* 16  YVU422 planar */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV411P ),     /* 16  YVU411 planar */
    NUM_2_STRING(HI_DRV_PIX_FMT_Y41P    ),     /* 12  YUV 4:1:1     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV444  ),     /* 16  xxxxyyyy uuuuvvvv */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV555  ),     /* 16  YUV-5-5-5     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV565  ),     /* 16  YUV-5-6-5     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV32   ),     /* 32  YUV-8-8-8-8   */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV410  ),     /*  9  YUV 4:1:0     */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV420  ),     /* 12  YUV 4:2:0     */
    NUM_2_STRING(HI_DRV_PIX_FMT_HI240   ),     /*  8  8-bit color   */
    NUM_2_STRING(HI_DRV_PIX_FMT_HM12    ),     /*  8  YUV 4:2:0 16x16 macroblocks */
    NUM_2_STRING(HI_DRV_PIX_FMT_M420    ),     /* 12  YUV 4:2:0 2 lines y), 1 line uv interleaved */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV08    ),      /*08  Y/CbCr 4:0:0 @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV80    ),      /*08  Y/CrCb 4:0:0 @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV12    ),     /* 12  Y/CbCr 4:2:0  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV21    ),     /* 12  Y/CrCb 4:2:0  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV12_411),     /* 12  Y/CbCr 4:1:1  @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV16    ),     /* 16  Y/CbCr 4:2:2  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV61    ),     /* 16  Y/CrCb 4:2:2  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV16_2X1),     /* 16  Y/CbCr 4:2:2 2X1 @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV61_2X1),     /* 16  Y/CrCb 4:2:2  2X1 @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV24    ),     /* 24  Y/CbCr 4:4:4  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV42    ),     /* 24  Y/CrCb 4:4:4  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV42_RGB),     /* 24  G/RB 4:4:4  */
    NUM_2_STRING(HI_DRV__PIX_FMT_NV12M  ),     /* 12  Y/CbCr 4:2:0  */
    NUM_2_STRING(HI_DRV__PIX_FMT_NV12MT ),     /* 12  Y/CbCr 4:2:0 64x32 macroblocks */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV420M ),     /* 12  YUV420 planar */
    NUM_2_STRING(HI_DRV_PIX_FMT_SBGGR8  ),     /*  8  BGBG.. GRGR.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGBRG8  ),     /*  8  GBGB.. RGRG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGRBG8  ),     /*  8  GRGR.. BGBG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SRGGB8  ),     /*  8  RGRG.. GBGB.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SBGGR10 ),     /* 10  BGBG.. GRGR.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGBRG10 ),     /* 10  GBGB.. RGRG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGRBG10 ),     /* 10  GRGR.. BGBG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SRGGB10 ),     /* 10  RGRG.. GBGB.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SBGGR12 ),     /* 12  BGBG.. GRGR.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGBRG12 ),     /* 12  GBGB.. RGRG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SGRBG12 ),     /* 12  GRGR.. BGBG.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_SRGGB12 ),     /* 12  RGRG.. GBGB.. */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV08_CMP),      /*08   Y/CbCr 4:0:0 compressed @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV80_CMP),      /*08   Y/CrCb 4:0:0 compressed @*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV12_CMP ),     /* 12  Y/CbCr 4:2:0 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV21_CMP ),     /* 12  Y/CrCb 4:2:0 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV16_CMP ),     /* 16  Y/CbCr 4:2:2 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV61_CMP ),     /* 16  Y/CrCb 4:2:2 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV16_2X1_CMP),     /* 16  Y/CbCr 4:2:2   2X1@*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV61_2X1_CMP),     /* 16  Y/CrCb 4:2:2  2X1@*/
    NUM_2_STRING(HI_DRV_PIX_FMT_NV24_CMP ),     /* 24  Y/CbCr 4:4:4 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV42_CMP ),     /* 24  Y/CrCb 4:4:4 compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV12_TILE),     /* 12 tile  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV21_TILE),     /* 21 tile  */
    NUM_2_STRING(HI_DRV_PIX_FMT_YUV400_TILE),     /* 21 tile  */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV12_TILE_CMP),    /* 12 tile compressed */
    NUM_2_STRING(HI_DRV_PIX_FMT_NV21_TILE_CMP),    /* 21 tile compressed */
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV400  ),    /*08  YUV400 planar @*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV410p  ),    /*10  YUV410 planar @*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV420p ),    /*12  YUV420 planar @*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV411  ),    /*12  YUV411  planar @*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV422_1X2),  /*16  YUV422  planar 1X2 @*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV422_2X1),  /*16  YUV422  planar 2X1@*/
	NUM_2_STRING(HI_DRV_PIX_FMT_YUV_444 ),    /*24  YUV444  planar @*/
};
HI_U8 *g_pcFrameTypeString[] =
{
    NUM_2_STRING(SDR),          /*Normal SDR frame.*/
    NUM_2_STRING(DOLBY_BL),     /*Dolby BL frame.*/
    NUM_2_STRING(DOLBY_EL),     /*Dolby EL frame.*/
    NUM_2_STRING(HDR10),        /*HDR10 frame.*/
    NUM_2_STRING(HLG),          /*HLG frame.*/
    NUM_2_STRING(SLF),          /*SLF frame.*/
    NUM_2_STRING(TECHNICOLOR),  /*TECHNICOLOR frame.*/
};
HI_U8 *g_pc3DTypeString[] =
{
    NUM_2_STRING(NOT_STEREO),
    NUM_2_STRING(SBS),
    NUM_2_STRING(TAB),
    NUM_2_STRING(FPK),
    NUM_2_STRING(TILE),
};
HI_U8 *g_pcProDetectString[] =
{
    NUM_2_STRING(PRODETECT_PROGRESSIVE),
    NUM_2_STRING(PRODETECT_INTERLACE),
    NUM_2_STRING(PRODETECT_AUTO),
};
HI_U8 *g_pcProColorSpaceString[] =
{
    NUM_2_STRING(UNKNOWN),
    NUM_2_STRING(DEFAULT),
    NUM_2_STRING(BT601_YUV_LIMITED), /* BT.601 */
    NUM_2_STRING(BT601_YUV_FULL),
    NUM_2_STRING(BT601_RGB_LIMITED),
    NUM_2_STRING(BT601_RGB_FULL),
    NUM_2_STRING(NTSC1953),
    NUM_2_STRING(BT470_SYSTEM_M),
    NUM_2_STRING(BT470_SYSTEM_BG),
    NUM_2_STRING(BT709_YUV_LIMITED), /* BT.709 */
    NUM_2_STRING(BT709_YUV_FULL),
    NUM_2_STRING(BT709_RGB_LIMITED),
    NUM_2_STRING(BT709_RGB_FULL),
    NUM_2_STRING(BT2020_YUV_LIMITED), /* ::::Current Used:::: BT.2020 */
    NUM_2_STRING(BT2020_YUV_FULL),
    NUM_2_STRING(BT2020_RGB_LIMITED),
    NUM_2_STRING(BT2020_RGB_FULL),    /* ::::Current Used:::: */
    NUM_2_STRING(REC709),       /* HD and modern captures. */
    NUM_2_STRING(SMPT170M),  /* ITU-R 601 -- broadcast NTSC/PAL */
    NUM_2_STRING(SMPT240M),  /* 1125-Line (US) HDTV */
    NUM_2_STRING(BT878),     /* broken BT878 extents (601),  luma range 16-253 instead of 16-235) */
    NUM_2_STRING(XVYCC),
    NUM_2_STRING(JPEG),
};
#ifdef VPSS_HAL_WITH_CBB
HI_U8 *g_pcHalNodeString[] =
{
    NUM_2_STRING(2D_FRAME),             //2D逐行
    NUM_2_STRING(2D_FRAME_VIRTUAL ),    //2D逐行虚拟端口), 转码使用
    NUM_2_STRING(2D_DEI),               //2D隔行),  hd
    NUM_2_STRING(2D_DEI_VIRTUAL),       //2D隔行),  hd虚拟端口), 转码使用
    NUM_2_STRING(2D_FIELD),             //2D逐行), 单场读入
    NUM_2_STRING(2D_FIELD_VIRTUAL),     //2D隔行), 单场读入虚拟端口), 转码使用
    NUM_2_STRING(2D_SD_FIRST),          //sd),  隔行),  tnr),  dei
    NUM_2_STRING(2D_SD_SECOND),         //sd),  隔行),  snr),  vhd0 funcs
    NUM_2_STRING(2D_SD_VIRTUAL),        //sd),  隔行虚拟端口), 转码使用
    NUM_2_STRING(2D_CTS10),             //只有2D时打开
    NUM_2_STRING(2D_CTS10_VIRTUAL ),    //CTS10), 虚拟端口
    NUM_2_STRING(2D_UHD),
    NUM_2_STRING(3D_FRAME_L),           //3D左眼
    NUM_2_STRING(3D_FRAME_R),           //3D右眼
    NUM_2_STRING(ROTATION_Y),           //Y), C翻转
    NUM_2_STRING(ROTATION_Y_VIRTUA5),
    NUM_2_STRING(ROTATION_C),           //Y), C翻转
    NUM_2_STRING(ROTATION_C_VIRTUA7),
    NUM_2_STRING(HDR_DETILE_NOALG),
    NUM_2_STRING(2D_H265_STEP1_INTERLACE),
    NUM_2_STRING(2D_H265_STEP2_DEI),
	NUM_2_STRING(2D_DEI_LAST_2FIELD),
    NUM_2_STRING(2D_DEI_LAST_2FIELD_VIRTUAL),
};
#else
HI_U8 *g_pcHalNodeString[] =
{
    NUM_2_STRING(2D_FRAME),
    NUM_2_STRING(2D_5Field),
    NUM_2_STRING(2D_3Field),
    NUM_2_STRING(2D_Field),
    NUM_2_STRING(3D_FRAME_R),
    NUM_2_STRING(PZME),
    NUM_2_STRING(UHD),
    NUM_2_STRING(UHD_HIGH_SPEED),
    NUM_2_STRING(UHD_SPLIT_L),
    NUM_2_STRING(UHD_SPLIT_R),
    NUM_2_STRING(UHD_HALF),
    NUM_2_STRING(3DDET),
    NUM_2_STRING(ZME_2L),
    NUM_2_STRING(ROTATION_Y),
    NUM_2_STRING(ROTATION_C),
    NUM_2_STRING(HDR_DETILE_NOALG),
    NUM_2_STRING(2D_DETILE_STEP1),
    NUM_2_STRING(2D_DETILE_STEP2),
    NUM_2_STRING(2D_H265_STEP1_INTERLACE),
    NUM_2_STRING(2D_H265_STEP2_DEI),
};
#endif
#endif
static VPSS_CTRL_S g_stVpssCtrl[VPSS_IP_BUTT] =
{
    {
        .bIPVaild = HI_FALSE,
        .enIP     = VPSS_IP_0,
        .u32VpssIrqNum = VPSS0_IRQ_NUM,
        .pVpssIntService = VPSS0_CTRL_IntService,
        .isr_name = "VPSS0_ISR",
        .s32IsVPSSOpen = 0,
    },

    {
        .bIPVaild = HI_TRUE,
        .enIP     = VPSS_IP_1,
        .u32VpssIrqNum = VPSS1_IRQ_NUM,
        .pVpssIntService = VPSS1_CTRL_IntService,
        .isr_name = "VPSS1_ISR",
        .s32IsVPSSOpen = 0,
    }
};

HI_VOID VPSS_CTRL_InitInstList(VPSS_IP_E enIp)
{
    HI_U32 u32Count;
    VPSS_INST_CTRL_S *pstInsList;

    pstInsList = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
    rwlock_init(&(pstInsList->stListLock));
    pstInsList->u32Target = 0;
    pstInsList->u32InstanceNum = 0;

    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count ++)
    {
        pstInsList->pstInstPool[u32Count] = HI_NULL;
    }

}

HI_S32 VPSS_CTRL_RegistISR(VPSS_IP_E enIp)
{
    if (request_irq(g_stVpssCtrl[enIp].u32VpssIrqNum, g_stVpssCtrl[enIp].pVpssIntService,
                        IRQF_DISABLED, g_stVpssCtrl[enIp].isr_name, &(g_stVpssCtrl[enIp].hVpssIRQ)))
    {
        VPSS_FATAL("VPSS%d registe IRQ failed!\n",(HI_U32)enIp);
        return HI_FAILURE;
    }
    else
    {
		if (HI_SUCCESS != HI_DRV_SYS_SetIrqAffinity(HI_ID_VPSS, g_stVpssCtrl[enIp].u32VpssIrqNum))
        {
            VPSS_ERROR("Set Vpss Irq Affinity Failed.\n");
        }

        return HI_SUCCESS;
    }
}

HI_S32 VPSS_CTRL_UnRegistISR(VPSS_IP_E enIp)
{
    free_irq(g_stVpssCtrl[enIp].u32VpssIrqNum, &(g_stVpssCtrl[enIp].hVpssIRQ));
    return HI_SUCCESS;
}

HI_U32 VPSS_CTRL_GetDistributeIP(VPSS_IP_E *penVpssIp)
{
    HI_U32 u32MinInstCount = VPSS_INSTANCE_MAX_NUMB;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 i;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        if((HI_TRUE == g_stVpssCtrl[i].bIPVaild) && (u32MinInstCount > g_stVpssCtrl[i].stInstCtrlInfo.u32InstanceNum))
        {
            u32MinInstCount = g_stVpssCtrl[i].stInstCtrlInfo.u32InstanceNum;
            *penVpssIp = i;
            s32Ret = HI_SUCCESS;
        }
    }

    return s32Ret;
}


HI_S32 VPSS_CTRL_SyncDistributeIP(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_IPMODE_E enIpmode)
{
    HI_U32 i;
    VPSS_IP_E enDstVpssIp;
    VPSS_IP_E enSrcVpssIp;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    unsigned long u32LockFlag;
    VPSS_INSTANCE_S *pstChgInst = HI_NULL;

    //step1:get instance real ip
    //step2:change instance to new ip
    //step2.1:if the new ip has instance ,change it together
    if(HI_DRV_VPSS_IPMODE_BUTT <= enIpmode)
    {
        VPSS_FATAL("Invalid Ipmode %d.",enIpmode);
        return HI_FAILURE;
    }

    if((HI_FALSE == g_stVpssCtrl[VPSS_IP_0].bIPVaild)
        || (HI_FALSE == g_stVpssCtrl[VPSS_IP_1].bIPVaild))
    {
        VPSS_ERROR("Can not change IP %d,single ctrl",enIpmode);
        return HI_FAILURE;
    }

    if(HI_DRV_VPSS_IPMODE_IP0 == enIpmode)
    {
        enDstVpssIp = VPSS_IP_0;
    }
    else
    {
        enDstVpssIp = VPSS_IP_1;
    }

    pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);
    if(enDstVpssIp == pstInstance->CtrlID)
    {
        return HI_SUCCESS;
    }

    enSrcVpssIp = pstInstance->CtrlID;

    //we need exchange instance to keep balance
    if(0 < pstInstCtrlInfo->u32InstanceNum)
	{
		pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);

		read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(HI_NULL != pstInstCtrlInfo->pstInstPool[i])
			{
				pstChgInst = pstInstCtrlInfo->pstInstPool[i];
				pstInstCtrlInfo->pstInstPool[i] = pstInstance;
				pstInstance->CtrlID = enDstVpssIp;
				break;
			}
		}

		read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		if (HI_NULL == pstChgInst)
		{
			VPSS_ERROR("Can't get DstInst\n");
			return HI_FAILURE;
		}

		pstInstCtrlInfo = &(g_stVpssCtrl[enSrcVpssIp].stInstCtrlInfo);

		read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

		for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(pstInstance == pstInstCtrlInfo->pstInstPool[i])
			{
				pstInstCtrlInfo->pstInstPool[i] = pstChgInst;
				pstChgInst->CtrlID = enSrcVpssIp;
				break;
			}
		}
		read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	}
    else
    {
        pstInstCtrlInfo = &(g_stVpssCtrl[enSrcVpssIp].stInstCtrlInfo);

        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
        {
            if(pstInstance == pstInstCtrlInfo->pstInstPool[i])
            {
                pstInstCtrlInfo->pstInstPool[i] = HI_NULL;
                pstInstCtrlInfo->u32InstanceNum--;
                break;
            }
        }

        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        pstInstCtrlInfo = &(g_stVpssCtrl[enDstVpssIp].stInstCtrlInfo);

        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
        {
            if(HI_NULL == pstInstCtrlInfo->pstInstPool[i])
            {
                pstInstCtrlInfo->pstInstPool[i] = pstInstance;
                pstInstance->CtrlID = enDstVpssIp;
                pstInstCtrlInfo->u32InstanceNum++;
                break;
            }
        }

        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    }

    return HI_SUCCESS;
}

VPSS_INSTANCE_S *VPSS_CTRL_GetServiceInstance(VPSS_IP_E enIp)
{
    HI_S32 s32LockRet;
    HI_S32 s32CheckRet;
    HI_U32 u32CycleTime;
    HI_U32 u32CurPos;
    unsigned long  u32LockFlag;
    VPSS_INST_CTRL_S  *pstInstCtrlInfo;
    VPSS_INSTANCE_S *pstInstance;

    pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
    u32CycleTime = 0;
    u32CurPos = pstInstCtrlInfo->u32Target;

    while(u32CycleTime < VPSS_INSTANCE_MAX_NUMB)
    {
        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        pstInstance = pstInstCtrlInfo->pstInstPool[u32CurPos];
        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

        if(pstInstance == HI_NULL)
        {
            u32CurPos = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
            u32CycleTime++;
            continue;
        }

        s32LockRet = HI_SUCCESS;//VPSS_OSAL_TryLock(&(pstInstance->stInstLock));
        //s32LockRet = VPSS_OSAL_TryLock(&(pstInstance->stInstLock));
        if (s32LockRet == HI_SUCCESS)
		{
			pstInstance->u32CheckCnt++;
			if (jiffies - pstInstance->ulLastCheckTime > HZ)
			{
				pstInstance->u32CheckRate = pstInstance->u32CheckCnt;
				pstInstance->u32CheckSucRate = pstInstance->u32CheckSucCnt;
#ifdef VPSS_SUPPORT_PROC_V2
				pstInstance->u32BufRate =  pstInstance->u32BufCnt - pstInstance->u32LastBufCnt;
				pstInstance->u32BufSucRate = pstInstance->u32BufSucCnt - pstInstance->u32BufLastSucCnt;
#else
				pstInstance->u32BufRate = pstInstance->u32BufCnt;
				pstInstance->u32BufSucRate = pstInstance->u32BufSucCnt;
#endif
				pstInstance->u32SrcRate = pstInstance->u32SrcCnt;
				pstInstance->u32SrcSucRate = pstInstance->u32SrcSucCnt;
				pstInstance->u32CheckCnt = 0;
				pstInstance->u32CheckSucCnt = 0;
#ifdef VPSS_SUPPORT_PROC_V2
				pstInstance->u32LastBufCnt = pstInstance->u32BufCnt;
				pstInstance->u32BufLastSucCnt = pstInstance->u32BufSucCnt;
#else
				pstInstance->u32BufCnt = 0;
				pstInstance->u32BufSucCnt = 0;
#endif
				pstInstance->u32SrcCnt = 0;
				pstInstance->u32SrcSucCnt = 0;

				pstInstance->u32ImgRate
					= pstInstance->u32ImgCnt - pstInstance->u32ImgLastCnt;
				pstInstance->u32ImgSucRate
					= pstInstance->u32ImgSucCnt - pstInstance->u32ImgLastSucCnt;
				pstInstance->u32ImgLastCnt = pstInstance->u32ImgCnt;
				pstInstance->u32ImgLastSucCnt = pstInstance->u32ImgSucCnt;
				pstInstance->ulLastCheckTime = jiffies;
			}

			if (pstInstance->enState == INSTANCE_STATE_WORING)
			{
				(HI_VOID)VPSS_INST_SyncUsrCfg(pstInstance);

				VPSS_INST_SyncTvpCfg(pstInstance);

				s32CheckRet = VPSS_INST_CheckInstAvailable(pstInstance);
				if(s32CheckRet == HI_SUCCESS)
				{
                    (HI_VOID)VPSS_INST_RevisePortOutBitWidth(pstInstance);
#if !defined(CHIP_TYPE_hi3798cv200_a) && !defined(CHIP_TYPE_hi3798cv200)
                    VPSS_INST_RevisePortOutRect(pstInstance);
#endif
					pstInstCtrlInfo->u32Target = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
					pstInstance->u32CheckSucCnt++;
#ifdef VPSS_SUPPORT_PROC_V2
					pstInstance->aulCreatTime[pstInstance->u32TimeStampIndex] = jiffies;
#endif
					//VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
					return pstInstance;
				}
				else
				{
					//VPSS_OSAL_UpLock(&(pstInstance->stInstLock));
				}
			}
		}

        u32CurPos = (u32CurPos + 1) % VPSS_INSTANCE_MAX_NUMB;
        u32CycleTime++;
    }

    return HI_NULL;
}

HI_S32 VPSS_CTRL_FixTask(VPSS_IP_E enIp, HI_DRV_BUF_ADDR_E enLR, VPSS_TASK_S *pstTask)
{
    HI_S32 i;
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstCur;
    VPSS_FB_NODE_S *pstFrmNode = HI_NULL;
    VPSS_IN_INTF_S stInIntf;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->pstPqCfg = &pstInst->stPqRegData;
#ifdef VPSS_HAL_WITH_CBB
    pstHalInfo->enSceneMode = pstInst->stUsrInstCfg.enSceneMode;
    pstHalInfo->s32InstanceID = pstInst->ID;
#endif
    s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

	if (HI_DRV_BUF_ADDR_MAX <= enLR)
	{
		VPSS_ERROR("Invalid Para enLR %d\n",enLR);
		return HI_FAILURE;
	}

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

    /* INInfo */
    s32Ret = stInIntf.pfnGetProcessImage(&pstInst->stInEntity, &pstCur);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
        return HI_FAILURE;
    }

#if !defined(CHIP_TYPE_hi3798cv200_a) && !defined(CHIP_TYPE_hi3798cv200)
	if ((pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_DEI)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_SD_FIRST)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_SD_SECOND)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI))
#else
	if ((pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_DETILE_STEP1)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE)
		|| (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_H265_STEP2_DEI))
#endif
    {
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_PQ_WBC_INFO_S stVpssWbcInfo;
        HI_PQ_CFG_INFO_S stVpssCfgInfo;

        stVpssWbcInfo.u32HandleNo = pstInst->ID;
        stVpssWbcInfo.u32Width = pstCur->u32Width;
        stVpssWbcInfo.u32Height = pstCur->u32Height;
		stVpssWbcInfo.bProgressive = pstCur->bProgressive;
        stVpssWbcInfo.pstVPSSWbcReg = &(pstInst->stPqWbcReg);

        stVpssWbcInfo.s32FieldOrder = !pstCur->bTopFieldFirst;

    stVpssWbcInfo.stOriCoordinate.s32X = 0;
    stVpssWbcInfo.stOriCoordinate.s32Y = 0;
    stVpssWbcInfo.stOriCoordinate.s32Width = 0;
    stVpssWbcInfo.stOriCoordinate.s32Height = 0;
    stVpssWbcInfo.stCropCoordinate = stVpssWbcInfo.stOriCoordinate;

        if(pstCur->enFieldMode == HI_DRV_FIELD_TOP)
        {
            stVpssWbcInfo.s32FieldMode = 0;
        }
        else if(pstCur->enFieldMode == HI_DRV_FIELD_BOTTOM)
        {
            stVpssWbcInfo.s32FieldMode = 1;
        }
        else
        {
            VPSS_ERROR("Dei Error.\n");
            stVpssWbcInfo.s32FieldMode = 0;
        }
        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstCur->u32Priv;

        stVpssWbcInfo.u32FrameRate = pstCur->u32FrameRate;
        stVpssWbcInfo.bPreInfo = HI_FALSE;

        stVpssWbcInfo.stVdecInfo.IsProgressiveFrm = pstPriv->ePictureMode;
        stVpssWbcInfo.stVdecInfo.IsProgressiveSeq = pstPriv->eSampleType;
        stVpssWbcInfo.stVdecInfo.RealFrmRate = 2500;
        {
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_DRV_DNR_INFO_S *pstDnrInfo;

        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstCur->u32Priv;
        pstDnrInfo = &pstPriv->stDNRInfo;
        stVpssWbcInfo.bCropBeforeDB = HI_FALSE;
        stVpssWbcInfo.u32SingleModeFlag = 1;
        stVpssWbcInfo.u32Protocol = 1;
        stVpssWbcInfo.u32ImageAvgQP = pstDnrInfo->u32ImageAvgQP;
        stVpssWbcInfo.u32VdhInfoAddr= pstDnrInfo->u32VdhInfoAddr;
        stVpssWbcInfo.u32VdhInfoStride= pstDnrInfo->u32VdhInfoStride;
        stVpssWbcInfo.u32ImageStruct= 0;//pstDnrInfo->u32Size;
        }
        DRV_PQ_SetAlgCalcCfg(&stVpssWbcInfo, &stVpssCfgInfo);

        /* open set filed order of alg detection d00241380 */
        if (stVpssCfgInfo.s32FieldOrder != 2)
        {
            if(pstInst->stInEntity.stStreamInfo.u32RealTopFirst == HI_FALSE)
            {
                if(pstCur->enFieldMode == HI_DRV_FIELD_TOP
                    && pstCur->enBufValidMode != HI_DRV_FIELD_ALL)
                {
                    HI_BOOL bTopFirst;
                    bTopFirst = !stVpssCfgInfo.s32FieldOrder;
                    stInIntf.pfnGetInfo(&pstInst->stInEntity,
                            VPSS_IN_INFO_CORRECT_FIELD,
                            HI_DRV_BUF_ADDR_MAX,
                            &bTopFirst);
                }
            }
            else if(pstInst->stInEntity.stStreamInfo.u32RealTopFirst == HI_TRUE)
            {
				if(pstCur->enFieldMode == HI_DRV_FIELD_BOTTOM
                    && pstCur->enBufValidMode != HI_DRV_FIELD_ALL)

                {
                    HI_BOOL bTopFirst;
                    bTopFirst = !stVpssCfgInfo.s32FieldOrder;
                    stInIntf.pfnGetInfo(&pstInst->stInEntity,
                            VPSS_IN_INFO_CORRECT_FIELD,
                            HI_DRV_BUF_ADDR_MAX,
                            &bTopFirst);
                }
            }
            else
            {

            }
        }
    }
	else
	{
        HI_PQ_WBC_INFO_S stVpssWbcInfo;
        HI_PQ_CFG_INFO_S stVpssCfgInfo;

		memset(&stVpssWbcInfo,0,sizeof(HI_PQ_WBC_INFO_S));
		memset(&stVpssCfgInfo,0,sizeof(HI_PQ_CFG_INFO_S));

        stVpssWbcInfo.u32HandleNo = pstInst->ID;
        stVpssWbcInfo.u32Width = pstCur->u32Width;
        stVpssWbcInfo.u32Height = pstCur->u32Height;
		stVpssWbcInfo.bProgressive = pstCur->bProgressive;
        stVpssWbcInfo.pstVPSSWbcReg = &(pstInst->stPqWbcReg);

        DRV_PQ_SetAlgCalcCfg(&stVpssWbcInfo, &stVpssCfgInfo);
	}

#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_R
        || pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L)
    {
        VPSS_INST_SetHalFrameInfo(pstCur,
                                  &pstHalInfo->stInInfo,
                                  HI_DRV_BUF_ADDR_LEFT);
    }
    else
#endif
    {
        VPSS_INST_SetHalFrameInfo(pstCur,
                                  &pstHalInfo->stInInfo,
                                  enLR);

        pstHalInfo->stInInfo.u32TunnelAddr = pstCur->u32TunnelPhyAddr;
    }

#if !defined(HI_NVR_SUPPORT)
	{
	    HI_S32			s32SrcId;
		s32SrcId = (pstInst->hDst & 0x00ff0000) >>16;
		//vi donot need rwzb, vi cannot give viraddr which rwzb needs.
	    if ((s32SrcId != HI_ID_VI) && (pstHalInfo->stInInfo.u32Width <= VPSS_FHD_WIDTH)
		    && (!pstHalInfo->stInInfo.bSecure))
	    {
	        VPSS_RWZB_IMG_S stRwzbImage;

			/*RWZB*/
			if(pstHalInfo->stInInfo.enFieldMode == HI_DRV_FIELD_BOTTOM
				|| pstHalInfo->stInInfo.bProgressive == HI_TRUE)
			{
				VPSS_RWZB_GetRwzbData(&(pstInst->stRwzbInfo), &pstHalInfo->stRwzbInfo);
			}

			stRwzbImage.bProgressive = pstHalInfo->stInInfo.bProgressive;
			stRwzbImage.enFieldMode = pstHalInfo->stInInfo.enFieldMode;
			stRwzbImage.u32Height = pstHalInfo->stInInfo.u32Height;
			stRwzbImage.u32Width = pstHalInfo->stInInfo.u32Width;

			VPSS_RWZB_GetRwzbInfo(&(pstInst->stRwzbInfo), &pstHalInfo->stRwzbInfo, &stRwzbImage);
	    }
	    else
	    {
			 pstHalInfo->stRwzbInfo.u32EnRwzb = HI_FALSE;
	    }
    }
#endif

	/*****************************DEI or TNR cfg*******************************/
	VPSS_CHECK_NULL(stInIntf.pfnSetInfo);

	stInIntf.pfnSetInfo(&(pstInst->stInEntity), VPSS_SET_INFO_INPUT_CFG, enLR, pstHalInfo);

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
        if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L
            || pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_R)
        {
            pstFrmNode = pstTask->pstFrmNode[i*2];
        }
        else
#endif
        {
            pstFrmNode = pstTask->pstFrmNode[i*2 + enLR];
        }

#ifdef ZME_2L_TEST
#ifndef VPSS_HAL_WITH_CBB
		if( HI_TRUE == pstTask->pstInstance->stPort[i].bNeedRotate)
		{
			pstFrmNode = pstTask->pstFrmNodeRoBuf[i];
		}
		else if( HI_TRUE == pstTask->pstInstance->stPort[i].bNeedZME2L)
#else
		if( HI_TRUE == pstTask->pstInstance->stPort[i].bNeedZME2L)
#endif
		{
			pstFrmNode = pstTask->pstFrmNodeZME1L;
		}
#endif

#ifdef VPSS_HAL_WITH_CBB
        VPSS_INST_GetRotate(pstInst,i,&pstHalInfo->astPortInfo[i],pstCur);
#endif
        if (pstFrmNode!= HI_NULL)
        {
            pstHalInfo->astPortInfo[i].bEnable = HI_TRUE;
			pstHalInfo->astPortInfo[i].bConfig = HI_FALSE;
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
            pstHalInfo->astPortInfo[i].bCmpLoss = VPSS_ISLOSS_CMP;
#endif

#ifdef VPSS_SUPPORT_OUT_TUNNEL
			pstHalInfo->astPortInfo[i].bOutLowDelay = pstTask->bOutLowDelay;
#endif
			if ((pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV16_2X1
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV12_TILE_CMP)
					&& (pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV21_CMP
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV21
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV61_2X1))
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_TRUE;
			}
			else if((pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV61_2X1
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE
						|| pstCur->ePixFormat == HI_DRV_PIX_FMT_NV21_TILE_CMP)
					&& (pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV12
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV12_CMP
						|| pstInst->stPort[i].eFormat == HI_DRV_PIX_FMT_NV16_2X1))
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_TRUE;
			}
			else if (pstInst->stPort[i].bUVInver) //dbg cmd
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_TRUE;
			}
			else
			{
				pstHalInfo->astPortInfo[i].stOutInfo.bUVInver = HI_FALSE;
			}

#ifdef ZME_2L_TEST
			VPSS_INST_SetOutFrameInfo(pstInst, i,
                    &pstFrmNode->stBuffer, &pstFrmNode->stOutFrame, enLR);
#ifdef VPSS_SUPPORT_OUT_TUNNEL
			if (pstHalInfo->astPortInfo[i].bOutLowDelay)
			{
				pstFrmNode->stOutFrame.u32TunnelPhyAddr = VPSS_HAL_TunnelOut_GetBufAddr(enIp, pstHalInfo, i);
			}
			else
			{
				pstFrmNode->stOutFrame.u32TunnelPhyAddr = HI_NULL;
			}
#endif
#else
            //如果开启了旋转，使用旋转BUFFER
#ifndef VPSS_HAL_WITH_CBB
            if ((HI_DRV_VPSS_ROTATION_90 == pstInst->stPort[i].enRotation)
                || (HI_DRV_VPSS_ROTATION_270 == pstInst->stPort[i].enRotation))
#else
           	if (0)
#endif
            {
                VPSS_INST_SetOutFrameInfo(pstInst, i,
                    &g_stVpssCtrl[enIp].stRoBuf[i], &pstFrmNode->stOutFrame, enLR);
            }
            else
            {
                VPSS_INST_SetOutFrameInfo(pstInst, i,
                    &pstFrmNode->stBuffer, &pstFrmNode->stOutFrame, enLR);
            }
#endif

            VPSS_INST_SetHalFrameInfo(&pstFrmNode->stOutFrame,
                &pstHalInfo->astPortInfo[i].stOutInfo, enLR);

#if !defined(CHIP_TYPE_hi3798cv200_a) && !defined(CHIP_TYPE_hi3798cv200)
			/* when vpss is working at :10bit src input , 8bit output with ZME,CROP eanble.
			  Cfg the input src as 8bit, because ZME,CROP do not support 10bit */
			if ((HI_DRV_PIXEL_BITWIDTH_10BIT == pstHalInfo->stInInfo.enBitWidth)
				&& (HI_DRV_PIXEL_BITWIDTH_8BIT == pstHalInfo->astPortInfo[i].stOutInfo.enBitWidth))
			{
				pstHalInfo->stInInfo.enBitWidth = HI_DRV_PIXEL_BITWIDTH_8BIT;
			}
#endif
            VPSS_INST_GetInCrop(pstInst, i, &pstHalInfo->astPortInfo[i].stInCropRect);
            if (HI_DRV_FT_TAB == pstCur->eFrmType)
            {
                if (VPSS_HAL_NODE_3D_FRAME_R == pstHalInfo->enNodeType)
                {
                    pstHalInfo->astPortInfo[i].stInCropRect.s32Y
                        += (pstCur->u32Height / 2);
                    pstHalInfo->astPortInfo[i].stInCropRect.s32Y
                        &= VPSS_HEIGHT_ALIGN;
                }
                pstHalInfo->astPortInfo[i].stInCropRect.s32Height
                        -= (pstCur->u32Height / 2);
                pstHalInfo->astPortInfo[i].stInCropRect.s32Height
                     &= VPSS_HEIGHT_ALIGN;
            }
            else if (HI_DRV_FT_SBS == pstCur->eFrmType)
            {
                if (VPSS_HAL_NODE_3D_FRAME_R == pstHalInfo->enNodeType)
                {
                    pstHalInfo->astPortInfo[i].stInCropRect.s32X
                        += (pstCur->u32Width / 2);
                    pstHalInfo->astPortInfo[i].stInCropRect.s32X
                        &= VPSS_WIDTH_ALIGN;
                }
                pstHalInfo->astPortInfo[i].stInCropRect.s32Width
                    -= (pstCur->u32Width / 2);
                pstHalInfo->astPortInfo[i].stInCropRect.s32Width
                     &= VPSS_WIDTH_ALIGN;
            }
            else
            {
                /*do nothing!!*/
            }

            VPSS_INST_GetVideoRect(pstInst, i, &pstHalInfo->astPortInfo[i].stInCropRect,
                &pstHalInfo->astPortInfo[i].stVideoRect,
                &pstHalInfo->astPortInfo[i].stOutCropRect);

            VPSS_DBG_INFO("Incrop W H %d %d\n",
                    pstHalInfo->astPortInfo[i].stInCropRect.s32Width,
                    pstHalInfo->astPortInfo[i].stInCropRect.s32Height);
            VPSS_DBG_INFO("VRect W H %d %d\n",
                    pstHalInfo->astPortInfo[i].stVideoRect.s32Width,
                    pstHalInfo->astPortInfo[i].stVideoRect.s32Height);
            VPSS_DBG_INFO("ORect W H %d %d\n",
                    pstHalInfo->astPortInfo[i].stOutCropRect.s32Width,
                    pstHalInfo->astPortInfo[i].stOutCropRect.s32Height);

            VPSS_SAFE_MEMCPY(&(pstFrmNode->stOutFrame.stLbxInfo),
                    &(pstHalInfo->astPortInfo[i].stVideoRect),
                    sizeof(HI_RECT_S));
#ifndef VPSS_HAL_WITH_CBB
            VPSS_INST_GetRotate(pstInst,i,&pstHalInfo->astPortInfo[i],pstCur);
#endif
        }
        else
        {
            pstHalInfo->astPortInfo[i].bEnable = HI_FALSE;
        }
    }

    return HI_SUCCESS;

}

HI_BOOL VPSS_CTRL_VirtualPortLowQulity(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_BOOL bStart = HI_FALSE;
	HI_U32 u32Cnt;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
	VPSS_HAL_PORT_INFO_S *pstPortInfo;
	VPSS_PORT_S			*pstPortCfg;
    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;
	for(u32Cnt = 0 ; u32Cnt < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Cnt ++)
	{
		pstPortInfo = &(pstHalInfo->astPortInfo[u32Cnt]);
		pstPortCfg  = &(pstInst->stPort[u32Cnt]);
		if (pstPortInfo->bEnable == HI_TRUE
			&& pstPortInfo->bConfig == HI_FALSE)
		{
			if ((pstPortCfg->u32MaxFrameRate < 50)
			    && (HI_FALSE == pstHalInfo->stInInfo.bProgressive))
			{
				bStart = HI_TRUE;
			}
		}
	}
	return bStart;
}


#ifndef VPSS_HAL_WITH_CBB
HI_BOOL VPSS_CTRL_CheckVirtualStart(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_BOOL bStart = HI_FALSE;

	HI_U32 u32Cnt;

    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
	VPSS_HAL_PORT_INFO_S *pstPortInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

	for(u32Cnt = 0 ; u32Cnt < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Cnt ++)
	{
		pstPortInfo = &(pstHalInfo->astPortInfo[u32Cnt]);

		if (pstPortInfo->bEnable == HI_TRUE
			&& pstPortInfo->bConfig == HI_FALSE)
		{
			bStart = HI_TRUE;
		}
	}

	return bStart;
}

HI_S32 VPSS_CTRL_Start2DTask_First3Field(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_S32 s32Ret = HI_FAILURE;
	VPSS_HAL_INFO_S *pstHalInfo;
	VPSS_INSTANCE_S* pstInst;

	pstHalInfo = &pstTask->stVpssHalInfo;
	pstInst = pstTask->pstInstance;

	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_DETILE_STEP1;
	s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
	HI_ASSERT(s32Ret != HI_FAILURE);
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_DETILE_STEP1] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_DETILE_STEP1);

	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_DETILE_STEP2;
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_DETILE_STEP2] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_DETILE_STEP2);

	return HI_SUCCESS;
}


HI_S32 VPSS_CTRL_GetFieldIdx(VPSS_TASK_S *pstTask)
{
	VPSS_IN_ENTITY_S *pstEntity = &(pstTask->pstInstance->stInEntity);
	HI_DRV_BUF_ADDR_E enLR = HI_DRV_BUF_ADDR_LEFT;
	HI_S32 idx;
	HI_U32 u32FieldIdx;
	HI_DRV_VIDEO_FRAME_S *pstRef[4] = {HI_NULL,HI_NULL,HI_NULL,HI_NULL};
	VPSS_IN_INTF_S stInIntf;
	HI_S32 s32Ret;

	s32Ret = VPSS_IN_GetIntf(pstEntity, &stInIntf);
	if (HI_SUCCESS != s32Ret)
	{
		VPSS_ERROR("Get Interface Failed\n");
		return HI_FAILURE;
	}

	stInIntf.pfnGetInfo( pstEntity,
						VPSS_IN_INFO_REF,
						enLR,
						(HI_VOID*)pstRef);

	u32FieldIdx = 0;
	for (idx=2; idx >= 0; idx--) //field 0,1,2
	{
		if (pstRef[idx] != HI_NULL)
		{
			u32FieldIdx++;
		}
	}

	return u32FieldIdx;
}

HI_BOOL VPSS_CTRL_Check2DDetileStart(VPSS_TASK_S *pstTask)
{
	VPSS_IN_ENTITY_S *pstEntity = &(pstTask->pstInstance->stInEntity);
	HI_S32 s32FieldIdx;
	if (pstEntity->stStreamInfo.u32StreamProg)
	{
		return HI_FALSE;
	}

	s32FieldIdx = VPSS_CTRL_GetFieldIdx(pstTask);
	if ((0 <= s32FieldIdx) && (s32FieldIdx < 3))//field 0,1,2
	{
		pstTask->stVpssHalInfo.u32DetileFieldIdx = s32FieldIdx;
		return HI_TRUE;
	}

    return HI_FALSE;
}
#endif
HI_S32 VPSS_CTRL_Start2DTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
#ifdef VPSS_HAL_WITH_CBB
	VPSS_FB_NODE_S *pstOutputInfo;
	HI_U32 PortId;
#endif

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->enNodeType = VPSS_INST_Check2DNodeType(pstInst);

    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

#ifdef VPSS_HAL_WITH_CBB
	for (PortId = 0; PortId < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; PortId++)
	{
		pstOutputInfo = pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT];

		if (pstInst->stPort[PortId].bNeedRotate)
		{
		   //set rotation output frame info
		   if (HI_NULL != pstOutputInfo)
           {
               VPSS_INST_SetRotationOutFrameInfo(pstInst, PortId,
                                      &pstOutputInfo->stBuffer, &pstOutputInfo->stOutFrame, HI_DRV_BUF_ADDR_LEFT);
           }
		   else
		   {
               VPSS_FATAL("pstOutputInfo is NULL! PortId:%d\n", PortId);
              // return HI_FAILURE;
		   }
		}
	}

	if (VPSS_CTRL_VirtualPortLowQulity(enIp, pstTask))
	{
		VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_LOWQUALITY, pstInst->abNodeVaild);
	}
	else if ((VPSS_HAL_NODE_2D_FRAME == pstHalInfo->enNodeType) || (VPSS_HAL_NODE_2D_UHD== pstHalInfo->enNodeType))
	{
		VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FRAME, pstInst->abNodeVaild);
	}
	else if ((VPSS_HAL_NODE_2D_DEI == pstHalInfo->enNodeType)
			|| (VPSS_HAL_NODE_2D_SD_FIRST == pstHalInfo->enNodeType)
			|| (VPSS_HAL_NODE_2D_FIELD == pstHalInfo->enNodeType)
			|| (VPSS_HAL_NODE_2D_SD_SECOND == pstHalInfo->enNodeType))
	{
		VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD, pstInst->abNodeVaild);
	}
	else if (VPSS_HAL_NODE_2D_DEI_LAST_2FIELD == pstHalInfo->enNodeType)
	{
        VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_LAST2FIELD, pstInst->abNodeVaild);
	}
	else if (VPSS_HAL_NODE_HDR_DETILE_NOALG == pstHalInfo->enNodeType)
	{
		VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_HDR, pstInst->abNodeVaild);
	}
#else
    if (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_Field)
    {
        pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_FIELD] = HI_TRUE;
        VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL]
				= HI_TRUE;
			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL);
		}
    }
    else if(pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_FRAME
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_HIGH_SPEED
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD
			|| pstHalInfo->enNodeType == VPSS_HAL_NODE_HDR_DETILE_NOALG)
    {
		pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D] = HI_TRUE;

		VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			if ((VPSS_CTRL_VirtualPortLowQulity(enIp, pstTask))
				&& (pstHalInfo->enNodeType == VPSS_HAL_NODE_2D_5Field))
			{
				pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_Field;
				pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL] = HI_TRUE;
				VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL);
			}
			else
			{
				pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_VIRTUAL]	= HI_TRUE;
			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_VIRTUAL);
			}
		}
    }
	else if (pstHalInfo->enNodeType == VPSS_HAL_NODE_UHD_SPLIT_L)
	{
		pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_SPLIT_L] = HI_TRUE;

		VPSS_HAL_SetNodeInfo(enIp,pstHalInfo,VPSS_HAL_TASK_NODE_SPLIT_L);

		if (VPSS_CTRL_CheckVirtualStart(enIp,pstTask))
		{
			pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_FRAME;

			pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_VIRTUAL]
				= HI_TRUE;

			VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_VIRTUAL);
		}
	}

	else
	{
		VPSS_ERROR("Invalid Node Type %d\n",pstHalInfo->enNodeType);
	}
#endif

    return HI_SUCCESS;
}

HI_BOOL VPSS_CTRL_Check2DStart(VPSS_TASK_S *pstTask)
{
    HI_U32 i;

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if (pstTask->pstFrmNode[i*2] != HI_NULL)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}
#ifndef VPSS_HAL_WITH_CBB
HI_BOOL VPSS_CTRL_CheckRotateStart(VPSS_TASK_S *pstTask, HI_U32 PortId)
{
	VPSS_PORT_S *pstPort;
	pstPort = &(pstTask->pstInstance->stPort[PortId]);

    if((HI_DRV_VPSS_ROTATION_DISABLE == pstPort->enRotation)
        ||(HI_DRV_VPSS_ROTATION_180 == pstPort->enRotation)
        || (HI_FALSE == pstPort->bEnble))
    {
        return HI_FALSE;
    }

    return HI_TRUE;
}

HI_BOOL VPSS_CTRL_CheckZME1LStart(VPSS_TASK_S *pstTask, HI_U32 PortId)
{
	VPSS_PORT_S *pstPort;
	pstPort = &(pstTask->pstInstance->stPort[PortId]);

    if(( HI_TRUE == pstTask->bZME2lTaskNeed[PortId]) && (HI_TRUE == pstPort->bEnble))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

#ifdef ZME_2L_TEST
/******************************************************************************
brief      : CNcomment :  check rotate and ZME_2L task
		   :  本函数功能:
			  1  根据输入图像信息(解码图像)检测与判定是否需要做二级缩放及旋转操作。并记录每个port相关标志。
			  2  如果需要做二级缩放(旋转)，则分配所需要的缓存图像buffer。
attention  :
param[in]  : enIp      :  vpss ID
			 pstTask   :
retval     : HI_S32
see        :
author	   :f00241306
******************************************************************************/

HI_U32 VPSS_CTRL_Zme2lAndRotateCfg(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_U32	i = 0, u32DstW = 0, u32DstH =0 , u32RotateNeed = 0;

	HI_S32 s32Ret				 = HI_FAILURE;
    VPSS_INSTANCE_S     *pstInst = HI_NULL;
	HI_DRV_VIDEO_FRAME_S *pstCur = HI_NULL;
    VPSS_IN_INTF_S stInIntf;

    HI_U32 u32BufSize			 = 0;
    HI_U32 u32BufStride			 = 0;
	HI_DRV_VIDEO_FRAME_S *pstFrm = HI_NULL;
	VPSS_BUFFER_S        *pstBuf = HI_NULL;

	VPSS_PORT_S			 *pstPort[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER] = {HI_NULL, HI_NULL, HI_NULL};
	HI_PQ_ZME_WIN_S      stZmeWin;

    HI_DRV_VIDEO_PRIVATE_S *pPriInfo;
	HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;


    pstInst = pstTask->pstInstance;
	memset(pstTask->bZME2lTaskNeed, 0, sizeof(HI_BOOL)*DEF_HI_DRV_VPSS_PORT_MAX_NUMBER);

	s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);
    s32Ret = stInIntf.pfnGetProcessImage(&pstInst->stInEntity, &pstCur);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
        return HI_FAILURE;
    }

	for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
		pstPort[i]	= &pstInst->stPort[i];

		if( HI_TRUE == pstPort[i]->bEnble && VPSS_INVALID_HANDLE != pstPort[i]->s32PortId )
		{
			 /*	 如果需要做旋转操作，则记录当前port旋转标志 并 申请相应的缓存buffer*/
			if (VPSS_CTRL_CheckRotateStart(pstTask, i))
			{
				pstPort[i]->bNeedRotate = HI_TRUE;
				u32RotateNeed++;
				if ( HI_NULL == pstTask->pstFrmNodeRoBuf[i])  //只分配一次缓存帧存
				{
					pstTask->pstFrmNodeRoBuf[i] = (VPSS_FB_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_FB_NODE_S));
				}

				if ( HI_NULL != pstTask->pstFrmNodeRoBuf[i])
				{
					pstFrm = &pstTask->pstFrmNodeRoBuf[i]->stOutFrame;
					pstBuf = &pstTask->pstFrmNodeRoBuf[i]->stBuffer;

					if (pstPort[i]->s32OutputWidth != 0 && pstPort[i]->s32OutputHeight != 0)
					{
						pstFrm->u32Width  = pstPort[i]->s32OutputHeight;
						pstFrm->u32Height = pstPort[i]->s32OutputWidth;
					}
					else
					{
                        pPriInfo = (HI_DRV_VIDEO_PRIVATE_S*) &pstCur->u32Priv[0];
                        pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pPriInfo->u32Reserve[0]);
                        if ((HI_UNF_VCODEC_TYPE_HEVC == pstVdecPriv->entype)
                            && (pstCur->enFieldMode != HI_DRV_FIELD_ALL)
                            && (pstCur->enBufValidMode == HI_DRV_FIELD_ALL))
                        {
                            pstFrm->u32Width = pstCur->u32Height * 2;
    						pstFrm->u32Height = pstCur->u32Width;
                        }
                        else
                        {
                            pstFrm->u32Width = pstCur->u32Height;
                            pstFrm->u32Height = pstCur->u32Width;
                        }
					}

					VPSS_OSAL_CalBufSize(&u32BufSize,
								&u32BufStride,
								pstFrm->u32Height,
								pstFrm->u32Width,
								pstPort[i]->eFormat,
								pstPort[i]->enOutBitWidth);

					if ((pstBuf->stMemBuf.u32Size != u32BufSize)
						|| (pstBuf->u32Stride != u32BufStride))
					{
						if (pstBuf->stMemBuf.u32Size != 0)
						{
							VPSS_OSAL_FreeMem(&(pstBuf->stMemBuf));
						}

						if (!pstInst->bSecure)
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,u32BufSize,
							HI_NULL,
							"VPSS_RoBuf",
							&(pstBuf->stMemBuf));
						}
						else
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,u32BufSize,
							HI_NULL,
							"VPSS_RoBuf",
							&(pstBuf->stMemBuf));
						}
						if (s32Ret != HI_SUCCESS)
						{
							VPSS_FATAL("Alloc RoBuf Failed\n");
						}

						pstBuf->u32Stride = u32BufStride;
					}
				}
			}

            else
            {
				if (HI_NULL != pstTask->pstFrmNodeRoBuf[i])
				{
					//释放旋转内存，节省SDK空间占用
					if(0 != pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf.u32Size)
					{
						VPSS_OSAL_FreeMem(&(pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf));
						pstTask->pstFrmNodeRoBuf[i]->stBuffer.stMemBuf.u32Size = 0;
						pstTask->pstFrmNodeRoBuf[i]->stBuffer.u32Stride = 0;
					}

					VPSS_VFREE(pstTask->pstFrmNodeRoBuf[i]);
					pstTask->pstFrmNodeRoBuf[i] = HI_NULL;
				}
            }
		}
	}

	if( u32RotateNeed > 0)  //如果有旋转任务，则不再做二级缩放
	{
		return HI_SUCCESS;
	}

	stZmeWin.stPort0Win.u32Width = pstPort[0]->s32OutputWidth;
	stZmeWin.stPort0Win.u32Height= pstPort[0]->s32OutputHeight;

	stZmeWin.stPort1Win.u32Width = pstPort[1]->s32OutputWidth;
	stZmeWin.stPort1Win.u32Height= pstPort[1]->s32OutputHeight;

	if(  0 != u32DstW && 0 != u32DstH)  //只分配一次缓存帧存

	{
		if (HI_NULL == pstTask->pstFrmNodeZME1L) //只分配一次缓存帧存
		{
			pstTask->pstFrmNodeZME1L = (VPSS_FB_NODE_S*)VPSS_VMALLOC(sizeof(VPSS_FB_NODE_S));
		}

		if ( HI_NULL != pstTask->pstFrmNodeZME1L)
		{
			pstFrm = &pstTask->pstFrmNodeZME1L->stOutFrame;
			pstBuf = &pstTask->pstFrmNodeZME1L->stBuffer;

			pstFrm->u32Width	= u32DstW;
			pstFrm->u32Height	= u32DstH;

			VPSS_OSAL_CalBufSize(&u32BufSize,
								&u32BufStride,
								pstFrm->u32Height,
								pstFrm->u32Width,
								HI_DRV_PIX_FMT_NV21,
								HI_DRV_PIXEL_BITWIDTH_10BIT);

			if ((pstBuf->stMemBuf.u32Size != u32BufSize)
				|| (pstBuf->u32Stride != u32BufStride))
			{
				if (pstBuf->stMemBuf.u32Size != 0)
				{
					VPSS_OSAL_FreeMem(&(pstBuf->stMemBuf));
				}
                if (!pstInst->bSecure)
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_NORMAL,u32BufSize,
							"VPSS_ZME1LBuf",
							HI_NULL,
							&(pstBuf->stMemBuf));
						}
						else
						{
							s32Ret = VPSS_OSAL_AllocateMem(VPSS_MEM_FLAG_SECURE,u32BufSize,
							"VPSS_ZME1LBuf",
							HI_NULL,
							&(pstBuf->stMemBuf));
						}
				if (s32Ret != HI_SUCCESS)
				{
					VPSS_FATAL("Alloc ZME1LBuf Failed\n");
				}

				pstBuf->u32Stride = u32BufStride;
			}
		}
	}
    else
    {
		if ( HI_NULL != pstTask->pstFrmNodeZME1L)
		{
			//释放二级缩放内存，节省SDK空间占用
			if(0 != pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size)
			{
				VPSS_OSAL_FreeMem(&(pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf));
				pstTask->pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size = 0;
				pstTask->pstFrmNodeZME1L->stBuffer.u32Stride = 0;

				VPSS_VFREE(pstTask->pstFrmNodeZME1L);
				pstTask->pstFrmNodeZME1L = HI_NULL;
			}
		}
    }

	return s32Ret;
}


HI_S32 VPSS_CTRL_StartZME2LTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask, HI_U32 PortId)
{
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstInputFrame;
    VPSS_FB_NODE_S *pstOutputInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    //set ZME_2L input frame info
    pstInputFrame = &(pstTask->pstFrmNodeZME1L->stOutFrame);
    VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);

    //set ZME_2L output frame info
    pstOutputInfo = pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT];
	VPSS_INST_SetOutFrameInfo(pstInst, PortId, &pstOutputInfo->stBuffer, &pstOutputInfo->stOutFrame, HI_DRV_BUF_ADDR_LEFT);
    VPSS_INST_SetHalFrameInfo(&pstOutputInfo->stOutFrame, &pstHalInfo->astPortInfo[PortId].stOutInfo, HI_DRV_BUF_ADDR_LEFT);

	VPSS_INST_GetInCrop(pstInst, PortId, &pstHalInfo->astPortInfo[PortId].stInCropRect);

    VPSS_INST_GetVideoRect(pstInst, PortId, &pstHalInfo->astPortInfo[PortId].stInCropRect,
        &pstHalInfo->astPortInfo[PortId].stVideoRect,
        &pstHalInfo->astPortInfo[PortId].stOutCropRect);

	pstHalInfo->enNodeType = VPSS_HAL_NODE_ZME_2L;
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_ZME_L2 + PortId] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_ZME_L2 + PortId );
	return HI_SUCCESS;
}
#endif

HI_S32 VPSS_CTRL_StartRotateTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask, HI_U32 PortId)
{
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;
    HI_DRV_VIDEO_FRAME_S *pstInputFrame;
    VPSS_FB_NODE_S *pstOutputInfo;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

#ifdef ZME_2L_TEST
	//set rotation input frame info
	pstInputFrame = &(pstTask->pstFrmNodeRoBuf[PortId]->stOutFrame);
	VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);
#else
    //set rotation input frame info
    pstInputFrame = &(pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT]->stOutFrame);
    VPSS_INST_SetHalFrameInfo(pstInputFrame, &pstHalInfo->stInInfo, HI_DRV_BUF_ADDR_LEFT);

#endif

	//set rotation output frame info
    pstOutputInfo = pstTask->pstFrmNode[PortId*2+HI_DRV_BUF_ADDR_LEFT];
    VPSS_INST_SetRotationOutFrameInfo(pstInst, PortId,
                    &pstOutputInfo->stBuffer, &pstOutputInfo->stOutFrame, HI_DRV_BUF_ADDR_LEFT);

	if ((HI_DRV_VPSS_ROTATION_90 == pstInst->stPort[PortId].enRotation)
			|| (HI_DRV_VPSS_ROTATION_270 == pstInst->stPort[PortId].enRotation))
	{
		pstOutputInfo->stOutFrame.stLbxInfo.s32X =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Y;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Y =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32X;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Width =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Height;
		pstOutputInfo->stOutFrame.stLbxInfo.s32Height =
			pstHalInfo->astPortInfo[PortId].stVideoRect.s32Width;
	}
	else
	{
		memcpy(&(pstOutputInfo->stOutFrame.stLbxInfo),
				&(pstHalInfo->astPortInfo[PortId].stVideoRect),
				sizeof(HI_RECT_S));
	}

    VPSS_INST_SetHalFrameInfo(&pstOutputInfo->stOutFrame,
        &pstHalInfo->astPortInfo[PortId].stOutInfo, HI_DRV_BUF_ADDR_LEFT);

    //rotation Y
    pstHalInfo->enNodeType = VPSS_HAL_NODE_ROTATION_Y;
    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_RO_Y + PortId * 2] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_RO_Y + PortId * 2);

    //rotation C
    pstHalInfo->enNodeType = VPSS_HAL_NODE_ROTATION_C;
    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_P0_RO_C + PortId * 2] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_P0_RO_C + PortId * 2);

    return HI_SUCCESS;
}

#endif
HI_S32 VPSS_CTRL_Start3DTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    VPSS_HAL_INFO_S *pstHalInfo;
    VPSS_INSTANCE_S* pstInst;

    pstHalInfo = &pstTask->stVpssHalInfo;
    pstInst = pstTask->pstInstance;

    pstHalInfo->enNodeType = VPSS_INST_Check3DNodeType(pstInst);

    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_RIGHT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

#ifndef VPSS_HAL_WITH_CBB
    pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_3D_R] = HI_TRUE;
    VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_3D_R);
#else
  	VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_3D, pstInst->abNodeVaild);
#endif
    return HI_SUCCESS;
}

HI_BOOL VPSS_CTRL_Check3DStart(VPSS_TASK_S *pstTask)
{
    HI_U32 i;

    for(i=0; i<DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if (pstTask->pstFrmNode[i*2 + 1] != HI_NULL)
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}

HI_S32 VPSS_CTRL_GetOutBufferRect(HI_RECT_S stOriRect,HI_RECT_S *pstRevisedRect)
{
	HI_U32 u32OriW = 0,u32OriH = 0;
	HI_U32 u32DstW = 0,u32DstH = 0;
	HI_U32 u32RatioW;
	HI_U32 u32RatioH;
	HI_U32 u32TmpH;
	HI_U32 u32TmpW;
	HI_U32 u32WidthLevel;
	HI_U32 u32HeightLevel;

	u32WidthLevel = 3840;
	u32HeightLevel = 2160;

	u32OriW = stOriRect.s32Width;
	u32OriH = stOriRect.s32Height;

	if (u32OriW < u32WidthLevel && u32OriH < u32HeightLevel)
	{
		u32DstW = u32OriW;
		u32DstH = u32OriH;
	}
	else if (u32OriW >= u32WidthLevel && u32OriH >= u32HeightLevel)
	{
		u32DstW = u32WidthLevel;
		u32DstH = u32HeightLevel;
	}
	else
	{
		u32RatioW = u32OriW*2048/u32WidthLevel;
		u32RatioH = u32OriH*2048/u32HeightLevel;

		if (u32RatioW > u32RatioH)
		{
			u32TmpW = u32OriW*2048/u32RatioW;
			u32TmpH = u32OriH*2048/u32RatioW;
		}
		else
		{
			u32TmpW = u32OriW*2048/u32RatioH;
			u32TmpH = u32OriH*2048/u32RatioH;
		}

		VPSS_PIXLE_DOWNALIGN_W(u32TmpW, HI_FALSE);
		VPSS_PIXLE_DOWNALIGN_H(u32TmpH, HI_FALSE);

		u32DstW = u32TmpW;
		u32DstH = u32TmpH;
	}

	pstRevisedRect->s32Width = u32DstW;
	pstRevisedRect->s32Height = u32DstH;

	return HI_SUCCESS;
}

#ifdef VPSS_SUPPORT_OUT_TUNNEL
HI_BOOL VPSS_CTRL_CheckLowDelay(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_BOOL bLowDelayTask = HI_FALSE;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_IN_INTF_S stInIntf = {0};
    HI_DRV_VIDEO_FRAME_S *pstImage;
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    HI_U32 u32EnablePort = 0;
    HI_BOOL bRotation = HI_FALSE;

    if (pstTask->pstInstance)
    {
		pstInstance = pstTask->pstInstance;

        (HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

        VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

        (HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
                            &pstImage);

        for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
        {
            pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
            if (pstPort->bEnble)
            {
                u32EnablePort++;

                if (pstPort->enRotation != HI_DRV_VPSS_ROTATION_DISABLE)
                {
                    bRotation = HI_TRUE;
                }

                if (pstPort->bTunnelEnable)
                {
                    bLowDelayTask = HI_TRUE;
                }
            }
        }

        if (pstImage->u32Width > 1920)
        {
            bLowDelayTask = HI_FALSE;
        }

        if (pstImage->bSecure)
        {
            bLowDelayTask = HI_FALSE;
        }

        if (pstImage->eFrmType != HI_DRV_FT_NOT_STEREO)
        {
            bLowDelayTask = HI_FALSE;
        }

        if (u32EnablePort > 1 || bRotation == HI_TRUE)
        {
            bLowDelayTask = HI_FALSE;
        }
    }
    else
    {
        bLowDelayTask = HI_FALSE;
    }

    return bLowDelayTask;
}
#endif

HI_S32 VPSS_CTRL_CreateTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_U32 u32StoreH;
    HI_U32 u32StoreW;
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    HI_DRV_VPSS_BUFLIST_CFG_S*  pstBufListCfg;
    VPSS_IN_INTF_S stInIntf = {0};
    HI_DRV_VIDEO_FRAME_S *pstImage;
    HI_U32 u32ExtBufSize;
    HI_DRV_VIDEO_PRIVATE_S *pstPriv = HI_NULL;

    /*
        Traversal instance list to find a Available inst
        available means two requirement:
        1.one undo image
        2.at least two writting space
     */
    pstTask->pstInstance = VPSS_CTRL_GetServiceInstance(enIp);

    if (HI_NULL == pstTask->pstInstance)
    {
        return HI_FAILURE;
    }

    /*
        get the image info to  inform user out buffer size
    */
    (HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

    VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

    (HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
                        &pstImage);

    pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
    /***********config out buffer************************/
    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstBufListCfg = &(pstPort->stFrmInfo.stBufListCfg);

        if (pstPort->s32PortId != VPSS_INVALID_HANDLE &&
            pstPort->bEnble == HI_TRUE)
        {
            if (pstPort->s32OutputHeight == 0 && pstPort->s32OutputWidth == 0)
            {
                HI_RECT_S stInRect;
                VPSS_INST_GetInCrop(pstTask->pstInstance, u32Count, &stInRect);
                u32StoreW = (HI_U32)stInRect.s32Width;
                u32StoreH = (HI_U32)stInRect.s32Height;

                //u32StoreH = pstStreamInfo->u32StreamH;
                //u32StoreW = pstStreamInfo->u32StreamW;
            }
            else
			{
				if(pstBufListCfg->eBufType != HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
				{
					u32StoreH = pstPort->s32OutputHeight;
					u32StoreW = pstPort->s32OutputWidth;
				}
				else
				{
					HI_RECT_S stInRect;
					HI_RECT_S stBufferRect;
					VPSS_INST_GetInCrop(pstTask->pstInstance, u32Count, &stInRect);

					VPSS_CTRL_GetOutBufferRect(stInRect,&stBufferRect);

					u32StoreW = (HI_U32)stBufferRect.s32Width;
					u32StoreH = (HI_U32)stBufferRect.s32Height;
				}
			}

            if (VPSS_HDR_TYPE_NEED_TRANS_METADATA(pstImage->enSrcFrameType))
            {
                u32ExtBufSize = 4096;//pstImage->unHDRInfo.stDolbyInfo.stMetadata.u32Length;
            }
            else
            {
                u32ExtBufSize = 0;
            }

			if (pstPriv->u32LastFlag == DEF_HI_DRV_VPSS_LAST_FRAME_FLAG)
			{
				pstPort->bCurDropped = HI_FALSE;
			}
			else
			{
				pstPort->bCurDropped = VPSS_INST_CheckIsDropped(pstTask->pstInstance,
						pstPort->u32MaxFrameRate,
						pstPort->u32OutCount);
			}
			if (pstPort->bCurDropped == HI_FALSE)
    		{
                /*2D image -> 1 outFrame 1 buffer*/
                if (pstImage->eFrmType == HI_DRV_FT_NOT_STEREO
                    || pstPort->b3Dsupport == HI_FALSE)
                {
                    pstTask->pstFrmNode[u32Count * 2] =
                        VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
                                               u32StoreH, u32StoreW,
                                               pstPort->eFormat,
    										   pstPort->enOutBitWidth,
    										   pstTask->pstInstance->bSecure,
    										   u32ExtBufSize);
                    pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
    			}
    			/*3D image -> 1 outFrame 2 buffer*/
    			else
    			{
    				pstTask->pstFrmNode[u32Count * 2] =
    					VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
    							u32StoreH, u32StoreW,
    							pstPort->eFormat,
    							pstPort->enOutBitWidth,
    							pstTask->pstInstance->bSecure,
    							u32ExtBufSize);
    				pstTask->pstFrmNode[u32Count * 2 + 1] =
    					VPSS_FB_GetEmptyFrmBuf(&(pstPort->stFrmInfo),
    							u32StoreH, u32StoreW,
    							pstPort->eFormat,
    							pstPort->enOutBitWidth,
    							pstTask->pstInstance->bSecure,
    							u32ExtBufSize);
    				if (pstTask->pstFrmNode[u32Count * 2] == HI_NULL
    						|| pstTask->pstFrmNode[u32Count * 2 + 1] == HI_NULL)
    				{

    					if (pstTask->pstFrmNode[u32Count * 2] != HI_NULL)
    					{
    						VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    								pstTask->pstFrmNode[u32Count * 2],
    								VPSS_FB_TYPE_NORMAL);
    					}

    					if (pstTask->pstFrmNode[u32Count * 2 + 1] != HI_NULL)
    					{
    						VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    								pstTask->pstFrmNode[u32Count * 2 + 1],
    								VPSS_FB_TYPE_NORMAL);
    					}
    					pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
    					pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
    				}
    			}


#if 1
    			if(pstBufListCfg->eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
    			{
    				/*************************/

    				if(pstPort->eFormat == HI_DRV_PIX_FMT_NV12
    						|| pstPort->eFormat == HI_DRV_PIX_FMT_NV21)
    				{
    					pstBufListCfg->u32BufStride = (u32StoreW + 0xf) & 0xfffffff0 ;
    					pstBufListCfg->u32BufSize =
    						pstBufListCfg->u32BufStride * u32StoreH * 3 / 2;
    				}
    				else if(pstPort->eFormat == HI_DRV_PIX_FMT_NV16_2X1
    						|| pstPort->eFormat == HI_DRV_PIX_FMT_NV61_2X1)
    				{
    					pstBufListCfg->u32BufStride =  (u32StoreW + 0xf) & 0xfffffff0 ;
    					pstBufListCfg->u32BufSize =
    						pstBufListCfg->u32BufStride * u32StoreH * 2;
    				}
    				else
    				{
    					VPSS_FATAL("Port %x OutFormat%d isn't supported.\n",
    							pstPort->s32PortId, pstPort->eFormat);
    				}
    				/*************************/
    				if(pstTask->pstFrmNode[u32Count*2] != HI_NULL)
    				{
    					s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
    							&(pstTask->pstFrmNode[u32Count*2]->stBuffer),u32StoreH,u32StoreW);
    					if (s32Ret != HI_SUCCESS)
    					{
    						VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    								pstTask->pstFrmNode[u32Count*2],
    								VPSS_FB_TYPE_NORMAL);
    						if (pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
    						{
    							VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    									pstTask->pstFrmNode[u32Count*2+1],
    									VPSS_FB_TYPE_NORMAL);
    						}
    						VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
    						return HI_FAILURE;
    					}
    				}

    				if(pstTask->pstFrmNode[u32Count*2+1] != HI_NULL)
    				{
    					s32Ret = VPSS_INST_GetFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,pstBufListCfg,
    							&(pstTask->pstFrmNode[u32Count*2+1]->stBuffer),u32StoreH,u32StoreW);
    					if(s32Ret != HI_SUCCESS)
    					{
    						VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    								pstTask->pstFrmNode[u32Count*2],
    								VPSS_FB_TYPE_NORMAL);
    						VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
    								pstTask->pstFrmNode[u32Count*2+1],
    								VPSS_FB_TYPE_NORMAL);
    						VPSS_OSAL_UpLock(&(pstTask->pstInstance->stInstLock));
    						return HI_FAILURE;
    					}
    				}
    			}
    			else
    			{

    			}
#endif
    		}
	        else
	        {
	            pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
	            pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
	        }
    	}
		else
		{
			pstTask->pstFrmNode[u32Count * 2] = HI_NULL;
			pstTask->pstFrmNode[u32Count * 2 + 1] = HI_NULL;
		}
	}

    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER * 2; u32Count++)
    {
        if (pstTask->pstFrmNode[u32Count] != HI_NULL)
        {
            s32Ret = HI_SUCCESS;
            break;
        }
    }

#if 0
	for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);

		if (pstPort->bEnble == HI_TRUE
				&& pstPort->bCurDropped == HI_FALSE
				&& pstTask->pstFrmNode[u32Count*2] == HI_NULL)
		{
            s32Ret = HI_FAILURE;
            break;
		}
	}
#endif

	if (s32Ret == HI_SUCCESS)
	{
		for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
		{
			pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
			if (pstPort->bEnble == HI_TRUE)
			{
				pstPort->u32OutCount++;
			}
		}
	}

    return s32Ret;

}


HI_BOOL VPSS_CTRL_Check2DH265InterlaceStart(VPSS_TASK_S *pstTask)
{
	VPSS_IN_INTF_S stInIntf = {0};
	HI_DRV_VIDEO_FRAME_S *pstImage;
	HI_DRV_VIDEO_PRIVATE_S *pPriInfo;
	HI_VDEC_PRIV_FRAMEINFO_S *pstVdecPriv;
	VPSS_CHECK_NULL(pstTask->pstInstance);

	(HI_VOID)VPSS_IN_GetIntf(&(pstTask->pstInstance->stInEntity), &stInIntf);

	VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);

	(HI_VOID)stInIntf.pfnGetProcessImage(&(pstTask->pstInstance->stInEntity),
						&pstImage);

	pPriInfo = (HI_DRV_VIDEO_PRIVATE_S *)&(pstImage->u32Priv[0]);
	pstVdecPriv = (HI_VDEC_PRIV_FRAMEINFO_S *)&(pPriInfo->u32Reserve[0]);
	if ((HI_UNF_VCODEC_TYPE_HEVC == pstVdecPriv->entype)
			&& (!pstImage->bProgressive)
			&& pstImage->enBufValidMode == HI_DRV_FIELD_ALL)
	{
		VPSS_DBG_INFO("Start H265Interlace task...\n");
		return HI_TRUE;
	}
	else
	{
        pstTask->stVpssHalInfo.pstH265RefList = HI_NULL;
		VPSS_HAL_H265RefListDeInit(&pstTask->pstInstance->stH265RefList);
		return HI_FALSE;
	}
}
HI_S32 VPSS_CTRL_Start2DTask_H265(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
	HI_S32 s32Ret = HI_FAILURE;
	VPSS_HAL_INFO_S *pstHalInfo;
	VPSS_INSTANCE_S* pstInst;
	pstHalInfo = &pstTask->stVpssHalInfo;
	pstInst = pstTask->pstInstance;
	pstHalInfo->pstH265RefList = &pstInst->stH265RefList;
	if ((pstInst->stInEntity.stStreamInfo.u32IsNewImage) || (!pstHalInfo->pstH265RefList->bRefListValid))
	{
		pstHalInfo->pstH265RefList = &pstInst->stH265RefList;
		VPSS_HAL_H265RefListInit(pstHalInfo->pstH265RefList,
								pstInst->stInEntity.stStreamInfo.u32StreamW,
									pstInst->stInEntity.stStreamInfo.u32StreamH,
										pstInst->stPort[0].eFormat,
										pstInst->bSecure);
	}
#ifdef VPSS_HAL_WITH_CBB
    pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE;
    s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
    HI_ASSERT(s32Ret != HI_FAILURE);

    VPSS_HAL_SetTaskInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_H265_DEI, pstInst->abNodeVaild);

#else
	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE;
	s32Ret = VPSS_CTRL_FixTask(enIp, HI_DRV_BUF_ADDR_LEFT, pstTask);
	HI_ASSERT(s32Ret != HI_FAILURE);
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_H265_STEP1_INTERLACE] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_H265_STEP1_INTERLACE);
	pstHalInfo->enNodeType = VPSS_HAL_NODE_2D_H265_STEP2_DEI;
	pstInst->abNodeVaild[VPSS_HAL_TASK_NODE_2D_H265_STEP2_DEI] = HI_TRUE;
	VPSS_HAL_SetNodeInfo(enIp, pstHalInfo, VPSS_HAL_TASK_NODE_2D_H265_STEP2_DEI);
#endif
	return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_StartTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 i;
    HI_BOOL bStart2D = HI_TRUE;
    HI_BOOL bStart3D = HI_FALSE;
    HI_S32 s32Ret;

    /* 所有节点都置为HI_FALSE */
#ifdef VPSS_HAL_WITH_CBB
    for(i=0; i<VPSS_HAL_NODE_BUTT;i++)
    {
        pstTask->pstInstance->abNodeVaild[i] = HI_FALSE;
    }
#else
	for(i=0; i<VPSS_HAL_TASK_NODE_BUTT;i++)
    {
        pstTask->pstInstance->abNodeVaild[i] = HI_FALSE;
    }
#endif

#ifdef VPSS_SUPPORT_PROC_V2
    pstTask->pstInstance->u32LogicTimeoutCnt++;
#endif
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
#ifdef ZME_2L_TEST
    s32Ret = VPSS_CTRL_Zme2lAndRotateCfg(enIp, pstTask);
	if (HI_SUCCESS != s32Ret)
	{
		VPSS_ERROR("Check ZME_2L Failed\n");
		return s32Ret;
	}
#endif
#endif

	if (VPSS_CTRL_Check2DH265InterlaceStart(pstTask))
	{
		s32Ret = VPSS_CTRL_Start2DTask_H265(enIp, pstTask);
		if (HI_SUCCESS != s32Ret)
		{
			VPSS_ERROR("Start VPSS_CTRL_Start2DTask_H265 Task Failed\n");
			return s32Ret;
		}
	}
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
	else if (VPSS_CTRL_Check2DDetileStart(pstTask))
	{
		s32Ret = VPSS_CTRL_Start2DTask_First3Field(enIp, pstTask);
		if (HI_SUCCESS != s32Ret)
		{
			VPSS_ERROR("Start VPSS_CTRL_Start2DTask_First3Field Task Failed\n");
			return s32Ret;
		}
	}
#endif
	else
	{
		bStart2D = VPSS_CTRL_Check2DStart(pstTask);
		if(bStart2D)
		{
			s32Ret = VPSS_CTRL_Start2DTask(enIp, pstTask);
			if (HI_SUCCESS != s32Ret)
			{
			VPSS_ERROR("Start 2D Task Failed\n");
			return s32Ret;
			}
		}
	}
#if defined(CHIP_TYPE_hi3798cv200_a) || defined(CHIP_TYPE_hi3798cv200)
#ifdef ZME_2L_TEST
	//ZME_2L
	for(i = 0; i < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
	{
		if( VPSS_CTRL_CheckRotateStart(pstTask,i) )
		{
			VPSS_CTRL_StartRotateTask(enIp, pstTask, i);
		}
		else  if( VPSS_CTRL_CheckZME1LStart(pstTask,i) )
		{
			VPSS_CTRL_StartZME2LTask(enIp, pstTask, i);
		}
		else
		{
		}
	}
#else
    //rotate
    for(i = 0; i < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; i++)
    {
        if(VPSS_CTRL_CheckRotateStart(pstTask,i))
        {
            VPSS_CTRL_StartRotateTask(enIp, pstTask, i);
        }
        else
        {
            //释放旋转内存，节省SDK空间占用
            if(0 != g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf.u32Size)
            {
				VPSS_OSAL_FreeMem(&(g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf));
                g_stVpssCtrl[enIp].stRoBuf[i].stMemBuf.u32Size = 0;
            }
        }
    }
#endif
#endif
    bStart3D = VPSS_CTRL_Check3DStart(pstTask);
    if(bStart3D)
    {
        (HI_VOID)VPSS_CTRL_Start3DTask(enIp, pstTask);
    }

#if DEF_VPSS_LOGIC_WORK
    s32Ret = VPSS_HAL_StartLogic(enIp, pstTask->pstInstance->abNodeVaild);
#else
	s32Ret = HI_SUCCESS;
#endif

#ifdef VPSS_SUPPORT_PROC_V2
    pstTask->pstInstance->aulLogicStartTime[pstTask->pstInstance->u32TimeStampIndex] = jiffies;
#endif
#if DEF_VPSS_STATIC
	g_u32LogicStart = jiffies;
#endif

    return s32Ret;
}


HI_S32 VPSS_CTRL_GetRwzbData(VPSS_IP_E enIP,VPSS_TASK_S *pstTask)
{
    #if 1
    VPSS_INSTANCE_S* pstInstance;
    HI_U32 u32Count;
    VPSS_RWZB_S *pstRwzb;

    pstInstance = pstTask->pstInstance;
    pstRwzb = &(pstInstance->stRwzbInfo);

    for(u32Count = 0; u32Count < 6 ; u32Count ++)
	{
#if defined(CHIP_TYPE_hi3798mv100_a) || defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
		VPSS_HAL_GetDetPixel(enIP,u32Count,&(pstRwzb->u8RwzbData[u32Count][0]));
#endif

#if defined(CHIP_TYPE_hi3798cv200_a)||defined(CHIP_TYPE_hi3798cv200)||defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
    /*pu8stt_w_vir_addr not malloc when 4K*/
    if (pstTask->stVpssHalInfo.stInInfo.u32Width <= VPSS_FHD_WIDTH)
		VPSS_HAL_GetDetPixel((HI_U32 *)pstTask->stVpssHalInfo.pu8stt_w_vir_addr,u32Count,&(pstRwzb->u8RwzbData[u32Count][0]));
#endif
#if defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
		VPSS_HAL_GetDetPixel(&(pstRwzb->u8RwzbData[u32Count][0]), &(pstTask->stVpssHalInfo.stRwzbInfo.u8Data[u32Count][0]));
#endif
	}
    #endif

    return HI_SUCCESS;
}


HI_S32 VPSS_CTRL_UpdatePqWbcData(VPSS_INSTANCE_S *pstInstance,PQ_VPSS_WBC_REG_S *pstLocalWbcReg)
{
    HI_PQ_WBC_INFO_S stPqWbcInfo;

    HI_DRV_VIDEO_FRAME_S *pstCur;

    VPSS_IN_INTF_S stInIntf;
#if !defined(CHIP_TYPE_hi3798mv200) && !defined(CHIP_TYPE_hi3798mv200_a)
    HI_U32 u32BaseRegPhyAddr;
    HI_U8* pu8BaseRegVirAddr;
#endif
    HI_S32 s32Ret;

	if ( HI_NULL == pstLocalWbcReg)
	{
    //    VPSS_ERROR("Get pstLocalWbcReg is null!!!\n");
        return HI_FAILURE;
	}

    VPSS_SAFE_MEMCPY(&pstInstance->stPqWbcReg,
            pstLocalWbcReg,
            sizeof(PQ_VPSS_WBC_REG_S));

    s32Ret = VPSS_IN_GetIntf(&(pstInstance->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    (HI_VOID)stInIntf.pfnGetProcessImage(&pstInstance->stInEntity, &pstCur);

    stPqWbcInfo.u32Width = pstCur->u32Width;
    stPqWbcInfo.u32Height = pstCur->u32Height;
	stPqWbcInfo.bProgressive = pstCur->bProgressive;
    stPqWbcInfo.u32HandleNo = pstInstance->ID ;
    stPqWbcInfo.pstVPSSWbcReg = &pstInstance->stPqWbcReg;
    stPqWbcInfo.stOriCoordinate.s32X = 0;
    stPqWbcInfo.stOriCoordinate.s32Y = 0;
    stPqWbcInfo.stOriCoordinate.s32Width = 0;
    stPqWbcInfo.stOriCoordinate.s32Height = 0;
    stPqWbcInfo.stCropCoordinate = stPqWbcInfo.stOriCoordinate;
#ifdef VPSS_SUPPORT_ALG_MCNR
    if (VPSS_ALG_IS_MCNR_ENABLE(pstCur->u32Width, pstCur->u32Height))
    {
        stPqWbcInfo.bMcnrEnableFlag = HI_TRUE;
    }
    else
    {
        stPqWbcInfo.bMcnrEnableFlag = HI_FALSE;
    }

    if ((pstInstance->stRwzbInfo.stDetInfo.isRWZB > 0)
        || VPSS_IS_TILE_FMT(pstCur->ePixFormat))
    {
        stPqWbcInfo.bMcnrEnableFlag = HI_FALSE;
    }
#endif
	if (pstCur->bProgressive == HI_FALSE)
	{
		VPSS_ST_RGME_CFG_S stStRgmeCfg;
		stInIntf.pfnGetInfo(&(pstInstance->stInEntity), VPSS_IN_INFO_RGME, HI_DRV_BUF_ADDR_LEFT, &stStRgmeCfg);

		stPqWbcInfo.pRGMV = (HI_VOID *)(stStRgmeCfg.pu8Nx2WriteAddr);

		stPqWbcInfo.stride = stStRgmeCfg.u32Stride;
        stPqWbcInfo.u32DeiEnFlag = HI_TRUE;
	}
	else
	{
#ifdef VPSS_SUPPORT_ALG_MCNR
        //逐行 mctnr 开的时候，也要配置 pRGMV
        if (HI_TRUE == stPqWbcInfo.bMcnrEnableFlag)
        {
            VPSS_ST_RGME_CFG_S stStRgmeCfg;
            stInIntf.pfnGetInfo(&(pstInstance->stInEntity), VPSS_IN_INFO_RGME, HI_DRV_BUF_ADDR_LEFT, &stStRgmeCfg);
            stPqWbcInfo.pRGMV = (HI_VOID *)(stStRgmeCfg.pu8Nx2WriteAddr);
            stPqWbcInfo.stride = stStRgmeCfg.u32Stride;
        }
        else
        {
            stPqWbcInfo.pRGMV = HI_NULL;
            stPqWbcInfo.stride = 0;
        }
#endif
        stPqWbcInfo.u32DeiEnFlag = HI_FALSE;
	}

    {
        HI_DRV_VIDEO_PRIVATE_S *pstPriv;
        HI_DRV_DNR_INFO_S *pstDnrInfo;

        pstPriv = (HI_DRV_VIDEO_PRIVATE_S *)pstCur->u32Priv;
        pstDnrInfo = &pstPriv->stDNRInfo;
        stPqWbcInfo.bCropBeforeDB = HI_FALSE;
        stPqWbcInfo.u32SingleModeFlag = 1;
        stPqWbcInfo.u32Protocol = 1;
        stPqWbcInfo.u32ImageAvgQP = pstDnrInfo->u32ImageAvgQP;
        stPqWbcInfo.u32VdhInfoAddr= pstDnrInfo->u32VdhInfoAddr;
        stPqWbcInfo.u32VdhInfoStride= pstDnrInfo->u32VdhInfoStride;
        stPqWbcInfo.u32ImageStruct= 0;//pstDnrInfo->u32Size;
    }
#if !defined(CHIP_TYPE_hi3798mv200) && !defined(CHIP_TYPE_hi3798mv200_a)
    (HI_VOID)VPSS_HAL_GetBaseRegAddr(VPSS_IP_0,
                &u32BaseRegPhyAddr,
                &pu8BaseRegVirAddr);
#endif
#ifndef HI_TEE_SUPPORT
    DRV_PQ_GetWbcInfo(&stPqWbcInfo);
#endif

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
	{
		HI_U32 u32RegSize;
		VPSS_REG_S *pstPhyReg;

		pstPhyReg = (VPSS_REG_S *)pu8BaseRegVirAddr;

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_DIEGLBMTNBIN, VPSS_DIEMTNSUM4);

		VPSS_SAFE_MEMCPY((HI_VOID*)&(pstInstance->stPqRegData.VPSS_DIEGLBMTNBIN[0]),
				(HI_VOID*)&(pstPhyReg->VPSS_DIEGLBMTNBIN[0]),
				u32RegSize);

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_PDICHD, VPSS_PDICHD);

		VPSS_SAFE_MEMCPY((HI_VOID*)&(pstInstance->stPqRegData.VPSS_PDICHD),
				(HI_VOID*)&(pstPhyReg->VPSS_PDICHD),
				u32RegSize);

		u32RegSize = VPSS_REG_SIZE_CALC(VPSS_PDFRMITDIFF, VPSS_PDLASICNT341);

		VPSS_SAFE_MEMCPY((HI_VOID*)&(pstInstance->stPqRegData.VPSS_PDFRMITDIFF),
				(HI_VOID*)&(pstPhyReg->VPSS_PDFRMITDIFF),
				u32RegSize);
	}
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_StorePrivData(VPSS_BUFFER_S *pstVpssBuf,HI_DRV_VIDEO_FRAME_S *pstPrivFrmData)
{
	HI_DRV_VIDEO_FRAME_S *pstPrivFrame;
	VPSS_MEM_S *pstPrivDataBuf;

#ifdef HI_VPSS_SMMU_SUPPORT
#else
	HI_S32 s32Ret;
    MMZ_BUFFER_S stMMZBuf;
#endif

	pstPrivDataBuf = &(pstVpssBuf->stPrivDataBuf);

#ifdef HI_VPSS_SMMU_SUPPORT
    pstPrivFrame = (HI_DRV_VIDEO_FRAME_S *)pstPrivDataBuf->pu8StartVirAddr;

    VPSS_SAFE_MEMCPY(pstPrivFrame,pstPrivFrmData,sizeof(HI_DRV_VIDEO_FRAME_S));
#else
    stMMZBuf.u32StartPhyAddr= pstPrivDataBuf->u32StartPhyAddr;
    stMMZBuf.u32Size = pstPrivDataBuf->u32Size;
	s32Ret = HI_DRV_MMZ_Map(&stMMZBuf);
    if (HI_SUCCESS == s32Ret)
	{
	    pstPrivFrame = (HI_DRV_VIDEO_FRAME_S *)stMMZBuf.pu8StartVirAddr;

	    VPSS_SAFE_MEMCPY(pstPrivFrame,pstPrivFrmData,sizeof(HI_DRV_VIDEO_FRAME_S));
	}
	else
	{
        VPSS_ERROR("Get PrivDataBuf Failed,PhyAddr=%#x\n",
            pstPrivDataBuf->u32StartPhyAddr);
	}
#endif

	return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_CompleteTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S* pstPort;
    VPSS_INSTANCE_S* pstInstance;
    VPSS_FB_NODE_S* pstLeftFbNode;
    VPSS_FB_NODE_S* pstRightFbNode;
    HI_BOOL bDropped;
    HI_DRV_VIDEO_FRAME_S *pstTmpFrame = HI_NULL;
    HI_DRV_VIDEO_PRIVATE_S* pstPriv;
    HI_DRV_VPSS_BUFFER_TYPE_E ePortType = HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE;
    HI_DRV_VIDEO_FRAME_S *pstCur;
    VPSS_IN_INTF_S stInIntf;
    HI_S32 s32Ret = HI_FAILURE;


    pstInstance = pstTask->pstInstance;

#ifdef VPSS_SUPPORT_PROC_V2
    pstInstance->u32LogicTimeoutCnt--;
    pstInstance->aulLogicEndTime[pstInstance->u32TimeStampIndex] = jiffies;
#endif
    s32Ret = VPSS_IN_GetIntf(&(pstInstance->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    //correct src info ,because rotation change it

    /*step 0.0:get  info from pq module*/
	for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
#ifdef VPSS_HAL_WITH_CBB
        if ((HI_TRUE == pstPort->bEnble) && pstPort->bNeedRotate)
#else
		if ((HI_TRUE == pstPort->bEnble) &&
            ((HI_DRV_VPSS_ROTATION_90 == pstPort->enRotation)
            || (HI_DRV_VPSS_ROTATION_270 == pstPort->enRotation)))
#endif
        {
            VPSS_CHECK_NULL(stInIntf.pfnGetProcessImage);
            s32Ret = stInIntf.pfnGetProcessImage(&pstInstance->stInEntity, &pstCur);
            if (HI_SUCCESS != s32Ret)
            {
                VPSS_FATAL("VPSS_SRC_GetProcessImage failed!\n");
                return HI_FAILURE;
            }

            pstTask->stVpssHalInfo.stInInfo.u32Width = pstCur->u32Width;
            pstTask->stVpssHalInfo.stInInfo.u32Height = pstCur->u32Height;
            pstTask->stVpssHalInfo.stInInfo.bProgressive = pstCur->bProgressive;
            pstTask->stVpssHalInfo.stInInfo.enFieldMode = pstCur->enFieldMode;
            break;
        }
	}


#ifdef VPSS_HAL_WITH_CBB
	VPSS_CTRL_UpdatePqWbcData(pstTask->pstInstance,(PQ_VPSS_WBC_REG_S *)pstTask->stVpssHalInfo.stSttAddr.pu8stt_w_vir_addr);
#else
	VPSS_CTRL_UpdatePqWbcData(pstTask->pstInstance,(PQ_VPSS_WBC_REG_S *)pstTask->stVpssHalInfo.pu8stt_w_vir_addr);
#endif

#if !defined(HI_NVR_SUPPORT)
	VPSS_CTRL_GetRwzbData(enIp,pstTask);
#endif
    /*step 1.0 :release done image*/
    VPSS_INST_CompleteImage(pstInstance);
    pstTmpFrame = VPSS_VMALLOC(sizeof(HI_DRV_VIDEO_FRAME_S));
    if (HI_NULL == pstTmpFrame)
    {
        return HI_FAILURE;
    }

    /*step 2:add outframe to outPort list*/
    for (u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        if (pstPort->s32PortId == VPSS_INVALID_HANDLE)
        {
            continue;
        }
        pstLeftFbNode = pstTask->pstFrmNode[u32Count * 2];
        pstRightFbNode = pstTask->pstFrmNode[u32Count * 2 + 1];

        bDropped = HI_FALSE;
        if (pstLeftFbNode != HI_NULL || pstRightFbNode != HI_NULL)
        {
			//Drop 2first frame for interlace stream
            if(HI_FALSE == pstInstance->stInEntity.stStreamInfo.u32StreamProg)
            {
                if((2 == pstInstance->stInEntity.stWbcInfo[0].u32CompleteCount)
                    || (3 == pstInstance->stInEntity.stWbcInfo[0].u32CompleteCount))
                {
                    bDropped = HI_TRUE;
                }
            }
        }

        if (pstLeftFbNode != HI_NULL )
        {
            VPSS_SAFE_MEMCPY(pstTmpFrame,
                   &(pstLeftFbNode->stOutFrame),
                   sizeof(HI_DRV_VIDEO_FRAME_S));

#if 0
#ifdef VPSS_SUPPORT_OUT_TUNNEL
			(HI_VOID)HI_DRV_SYS_GetTimeStampMs(&(pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_OUT));

			if (pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_IN + 20 <
						pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_OUT)
			{
				VPSS_FATAL("time: %d !!!!!!!!!!!!\n",
								(pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_OUT -
								pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_IN));
			}
			else
			{
				VPSS_FATAL("time: %d\n",
								(pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_OUT -
								pstLeftFbNode->stOutFrame.stLowdelayStat.u32VPSS_IN));
			}
#endif
#endif
            if (pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                /*Revise the Port Type to HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE
                    *in order to decide whether report newframe
                    */
                ePortType = HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE;

				if (pstInstance->bStorePrivData)
				{
#ifdef VPSS_SUPPORT_OUT_TUNNEL
					memset(&(pstLeftFbNode->stOutFrame.stLowdelayStat), 0x0, sizeof(HI_DRV_LOWDELAY_STAT_INFO_S));
					// record report done time for overlay lowdelay
					(HI_VOID)HI_DRV_SYS_GetTimeStampMs(&(pstLeftFbNode->stOutFrame.stLowdelayStat.u32OmxReportDoneTime));
#endif
					(HI_VOID)VPSS_CTRL_StorePrivData(&(pstLeftFbNode->stBuffer),
							&(pstLeftFbNode->stOutFrame));
				}

                if (HI_FALSE == bDropped)
                {
                    if (u32Count == 0)
                    {
                        HI_LD_Event_S evt;
                        HI_U32 TmpTime = 0;
                        HI_DRV_SYS_GetTimeStampMs(&TmpTime);
                        evt.evt_id = EVENT_VPSS_FRM_OUT;
                        evt.frame = pstLeftFbNode->stOutFrame.u32FrameIndex;
                        evt.handle = pstLeftFbNode->stOutFrame.hTunnelSrc;
                        evt.time = TmpTime;
                        HI_DRV_LD_Notify_Event(&evt);
                    }

                    VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                                           pstPort->s32PortId,
                                           &(pstLeftFbNode->stOutFrame));
                }
                else
                {
                    VPSS_INST_RelFrmBuffer(pstTask->pstInstance, pstPort->s32PortId,
                                           &(pstPort->stFrmInfo.stBufListCfg),
                                           &(pstLeftFbNode->stBuffer.stMemBuf));
                }

                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                       pstTask->pstFrmNode[u32Count],
                                       VPSS_FB_TYPE_NORMAL);
            }
            else
            {
                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S*) & (pstLeftFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    if ((pstInstance->stInEntity.stStreamInfo.u32InRate > pstPort->u32MaxFrameRate)
                        &&(pstPort->u32MaxFrameRate != 0))
                    {
                        pstLeftFbNode->stOutFrame.u32FrameRate
                            = pstInstance->stInEntity.stStreamInfo.u32InRate / 2 * 1000;
                    }

                    if (u32Count == 0)
                    {
                        HI_LD_Event_S evt;
                        HI_U32 TmpTime = 0;
                        HI_DRV_SYS_GetTimeStampMs(&TmpTime);
                        evt.evt_id = EVENT_VPSS_FRM_OUT;
                        evt.frame = pstLeftFbNode->stOutFrame.u32FrameIndex;
                        evt.handle = pstLeftFbNode->stOutFrame.hTunnelSrc;
                        evt.time = TmpTime;
                        HI_DRV_LD_Notify_Event(&evt);
                    }
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo), pstLeftFbNode);
                }
                else
                {

                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                           pstLeftFbNode,
                                           VPSS_FB_TYPE_NORMAL);
                }
            }
        }

        if (pstRightFbNode != HI_NULL)
        {
            VPSS_SAFE_MEMCPY(&(pstRightFbNode->stOutFrame.stBufAddr[0]),
                   &(pstTmpFrame->stBufAddr[0]),
                   sizeof(HI_DRV_VID_FRAME_ADDR_S));
            if (pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                if (HI_FALSE == bDropped)
                {
                    VPSS_INST_ReportNewFrm(pstTask->pstInstance,
                                           pstPort->s32PortId,
                                           &(pstRightFbNode->stOutFrame));
                }
                else
                {
                    VPSS_INST_RelFrmBuffer(pstTask->pstInstance, pstPort->s32PortId,
                                           &(pstPort->stFrmInfo.stBufListCfg),
                                           &(pstRightFbNode->stBuffer.stMemBuf));
                }

                VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                       pstTask->pstFrmNode[u32Count],
                                       VPSS_FB_TYPE_NORMAL);
            }
            else
            {

                if (HI_FALSE == bDropped)
                {
                    pstPriv = (HI_DRV_VIDEO_PRIVATE_S*) & (pstRightFbNode->stOutFrame.u32Priv[0]);
                    pstPriv->u32FrmCnt = pstPort->u32OutCount;
                    VPSS_FB_AddFulFrmBuf(&(pstPort->stFrmInfo), pstRightFbNode);
                }
                else
                {
                    VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                                           pstRightFbNode,
                                           VPSS_FB_TYPE_NORMAL);
                }
            }
        }
    }
    if (ePortType == HI_DRV_VPSS_BUF_VPSS_ALLOC_MANAGE)
    {
        if (pstInstance->pfUserCallBack != HI_NULL)
        {
            pstInstance->pfUserCallBack(pstInstance->hDst, VPSS_EVENT_NEW_FRAME, HI_NULL);
        }
        else
        {
            VPSS_FATAL("Can't report VPSS_EVENT_NEW_FRAME,pfUserCallBack is NULL");
        }

        VPSS_INST_ReportCompleteEvent(pstInstance);
    }

	VPSS_VFREE(pstTmpFrame);
#ifdef VPSS_SUPPORT_PROC_V2
    pstInstance->aulFinishTime[pstInstance->u32TimeStampIndex] = jiffies;
    pstInstance->u32TimeStampIndex = (pstInstance->u32TimeStampIndex + 1) % VPSS_PROC_TIMESTAMP_MAXCNT;
#endif
    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_ClearTask(VPSS_IP_E enIp, VPSS_TASK_S *pstTask)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    VPSS_FB_NODE_S *pstFbNode;
    VPSS_IN_INTF_S stInIntf;
    VPSS_INSTANCE_S *pstInst;

    HI_S32 s32Ret = HI_FAILURE;

    pstInst = pstTask->pstInstance;

#ifdef VPSS_SUPPORT_PROC_V2
    pstInst->aulLogicEndTime[pstInst->u32TimeStampIndex] = 0xFFFFFFFF;
    pstInst->aulFinishTime[pstInst->u32TimeStampIndex] = 0xFFFFFFFF;
    pstInst->u32TimeStampIndex = (pstInst->u32TimeStampIndex + 1) % VPSS_PROC_TIMESTAMP_MAXCNT;
#endif

    s32Ret = VPSS_IN_GetIntf(&(pstInst->stInEntity), &stInIntf);
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_ERROR("Get Interface Failed\n");
        return HI_FAILURE;
    }

    //step1:release src buffer
    s32Ret = stInIntf.pfnCompleteImage(&(pstInst->stInEntity));
    if (HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("pfnCompleteImage failed!\n");
        return HI_FAILURE;
    }

    //step2:release port buffer
    for(u32Count = 0;u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER;u32Count++)
    {
        pstPort = &((pstTask->pstInstance)->stPort[u32Count]);
        pstFbNode = pstTask->pstFrmNode[u32Count*2];
        if(pstFbNode != HI_NULL)
        {
            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMemBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                pstFbNode,
                VPSS_FB_TYPE_NORMAL);
        }

        pstFbNode = pstTask->pstFrmNode[u32Count*2 + 1];
        if(pstFbNode != HI_NULL)
        {

            if(pstPort->stFrmInfo.stBufListCfg.eBufType == HI_DRV_VPSS_BUF_USER_ALLOC_MANAGE)
            {
                VPSS_INST_RelFrmBuffer(pstTask->pstInstance,pstPort->s32PortId,
                        &(pstPort->stFrmInfo.stBufListCfg),
                        &(pstTask->pstFrmNode[u32Count]->stBuffer.stMemBuf));
            }
            VPSS_FB_AddEmptyFrmBuf(&(pstPort->stFrmInfo),
                            pstFbNode,
                            VPSS_FB_TYPE_NORMAL);
        }
    }

#ifndef VPSS_HAL_WITH_CBB
    //step3:reset vpss logic
    VPSS_REG_ReSetCRG(pstTask->pstInstance->CtrlID);
#endif
    return HI_SUCCESS;

}

HI_S32 VPSS_CTRL_ThreadProc(HI_VOID* pArg)
{
	VPSS_IP_E enIp;
	HI_S32 s32CreateRet   = HI_FAILURE;
	HI_S32 s32StartRet    = HI_FAILURE;
	HI_S32 s32WaitRet     = HI_FAILURE;

	HI_U32 u32NowTime = 0;
	VPSS_CTRL_S *pstVpssCtrl = HI_NULL;

	enIp = (VPSS_IP_E)pArg;
	pstVpssCtrl = &(g_stVpssCtrl[enIp]);

	pstVpssCtrl->s32ThreadPos = 0;

	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stTaskNext), EVENT_UNDO, EVENT_UNDO);
	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stNewTask), EVENT_UNDO, EVENT_UNDO);
#ifdef VPSS_SUPPORT_OUT_TUNNEL
	VPSS_OSAL_InitEvent(&(pstVpssCtrl->stTaskLowDelay), EVENT_UNDO, EVENT_UNDO);
#endif
	pstVpssCtrl->stTask.u32LastTotal = 0;
	pstVpssCtrl->stTask.u32SuccessTotal = 0;
	pstVpssCtrl->stTask.u32Create = 0;
	pstVpssCtrl->stTask.u32Fail = 0;
	pstVpssCtrl->stTask.u32TimeOut= 0;

	while(!kthread_should_stop())
	{
		HI_S32 s32Ret = HI_FAILURE;
		pstVpssCtrl->stTask.stState = TASK_STATE_READY;

		if(pstVpssCtrl->u32ThreadSleep == 1)
		{
			goto VpssThreadIdle;
		}

		pstVpssCtrl->s32ThreadPos = 1;

		pstVpssCtrl->stTask.u32Create++;

		s32CreateRet =  VPSS_CTRL_CreateTask(enIp, &(pstVpssCtrl->stTask));
		/* create success  running -> waitting */
		if(s32CreateRet == HI_SUCCESS)
		{
			VPSS_INFO("...............CreateTask\n");
			pstVpssCtrl->s32ThreadPos = 2;

			VPSS_HAL_SetClockEn(enIp, HI_TRUE);
#ifdef VPSS_SUPPORT_OUT_TUNNEL
			if (VPSS_CTRL_CheckLowDelay(enIp, &(pstVpssCtrl->stTask)))
			{
				pstVpssCtrl->stTask.bOutLowDelay = HI_TRUE;
			}
			else
			{
				pstVpssCtrl->stTask.bOutLowDelay = HI_FALSE;
			}
			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stTaskLowDelay), EVENT_UNDO, EVENT_UNDO);
#endif
			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stTaskNext), EVENT_UNDO, EVENT_UNDO);

			s32StartRet = VPSS_CTRL_StartTask(enIp, &(pstVpssCtrl->stTask));
			if (s32StartRet == HI_SUCCESS)
			{
				VPSS_INFO("...............StartTask\n");
				/*
				   start logic running, waitting for irq to wakeup thread
				   */
				pstVpssCtrl->stTask.stState = TASK_STATE_WAIT;
#if DEF_VPSS_LOGIC_WORK
#ifdef VPSS_SUPPORT_OUT_TUNNEL
				if (pstVpssCtrl->stTask.bOutLowDelay)
				{
					s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskLowDelay), HZ);
				}
				else
				{
					s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskNext), HZ);
				}
#else
				s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskNext), HZ);
#endif
#else
                msleep(10);
				s32WaitRet = HI_SUCCESS;
#endif
                if (s32WaitRet == HI_SUCCESS)
				{
					pstVpssCtrl->s32ThreadPos = 3;

					VPSS_CTRL_CompleteTask(enIp, &(pstVpssCtrl->stTask));

					if(jiffies - u32NowTime >= HZ)
					{
						u32NowTime = jiffies;
						pstVpssCtrl->stTask.u32SucRate = pstVpssCtrl->stTask.u32SuccessTotal
							- pstVpssCtrl->stTask.u32LastTotal;
						pstVpssCtrl->stTask.u32LastTotal = pstVpssCtrl->stTask.u32SuccessTotal;
					}

					pstVpssCtrl->stTask.u32SuccessTotal ++;
#ifdef VPSS_SUPPORT_OUT_TUNNEL
					if (pstVpssCtrl->stTask.bOutLowDelay)
					{
						s32WaitRet = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stTaskNext), HZ);
						if (s32WaitRet != HI_SUCCESS)
						{
							VPSS_FATAL("...............Wait LowDelay Faild\n");
						}
					}
#endif
				}
				else
				{
					VPSS_FATAL("...............Wait OutTime Faild\n");
					pstVpssCtrl->s32ThreadPos = 4;
					pstVpssCtrl->stTask.u32TimeOut++;
#ifdef VPSS_HAL_WITH_CBB
					VPSS_HAL_DumpReg(enIp, VPSS_HAL_NODE_2D_FRAME, HI_TRUE);
#else
                    VPSS_HAL_DumpReg(enIp, VPSS_HAL_TASK_NODE_2D);
#endif
					VPSS_CTRL_ClearTask(enIp, &(pstVpssCtrl->stTask));
			        VPSS_HAL_SetClockEn(enIp, HI_FALSE);
				}
			}
			else
			{
				/* 创建Task成功之后，start必须成功 */
				VPSS_FATAL("...............StartTask Faild\n");
				s32StartRet = HI_FAILURE;

				VPSS_CTRL_ClearTask(enIp, &(pstVpssCtrl->stTask));

				goto  VpssThreadIdle;
			}
		}
		else/*create failed or start failed running -> idle*/
		{
			pstVpssCtrl->stTask.u32Fail++;

VpssThreadIdle:
			pstVpssCtrl->s32ThreadPos = 5;
			pstVpssCtrl->stTask.stState = TASK_STATE_IDLE;

			VPSS_HAL_SetClockEn(enIp, HI_FALSE);

			s32Ret = VPSS_OSAL_WaitEvent(&(pstVpssCtrl->stNewTask),HZ/100);
			if(s32Ret == HI_SUCCESS)
			{
				VPSS_INFO("WakeUpThread Success.\n");
			}

			VPSS_OSAL_ResetEvent(&(pstVpssCtrl->stNewTask), EVENT_UNDO, EVENT_UNDO);
#ifdef ZME_2L_TEST
			/*当上层出现stop操作时，释放缓存的buffer*/
			if( HI_NULL != pstVpssCtrl->stTask.pstFrmNodeZME1L)
			{
				//释放旋转内存，节省SDK空间占用
				if(0 != pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size)
				{
					VPSS_OSAL_FreeMem(&(pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf));
					pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.stMemBuf.u32Size = 0;
					pstVpssCtrl->stTask.pstFrmNodeZME1L->stBuffer.u32Stride = 0;

					VPSS_VFREE(pstVpssCtrl->stTask.pstFrmNodeZME1L);
					pstVpssCtrl->stTask.pstFrmNodeZME1L = HI_NULL;
				}
			}
#endif
		}

	}

	pstVpssCtrl->s32ThreadPos = 6;

	VPSS_INFO("s32ThreadPos = %d...\n",pstVpssCtrl->s32ThreadPos);

	return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_CreateThread(VPSS_IP_E enIp)
{
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#else
    struct sched_param param;
#endif
    g_stVpssCtrl[enIp].u32ThreadKilled = 0;
    g_stVpssCtrl[enIp].u32ThreadSleep = 0;
    g_stVpssCtrl[enIp].hThread =
        kthread_create(VPSS_CTRL_ThreadProc, (HI_VOID *)enIp, "HI_VPSS_Process");

	if( IS_ERR(g_stVpssCtrl[enIp].hThread) )
    {
        VPSS_FATAL("Can not create thread.\n");
        return HI_FAILURE;
    }

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
#else
    param.sched_priority = 99;
    sched_setscheduler(g_stVpssCtrl[enIp].hThread, SCHED_RR, &param);

#ifndef HI_TEE_SUPPORT
	//kthread_bind(g_stVpssCtrl[enIp].hThread, 3);
	//kthread_bind(g_stVpssCtrl[enIp].hThread, 0);
#endif
#endif

#ifdef HI_FPGA_SUPPORT
/* TO avoid VPSS's reg r/w operation collision with VDH. (they are both on FPGA4) */
	if (num_online_cpus() > 2)
	{
		if (cpu_online(VPSS_CPU_INDEX))
		{
			kthread_bind(g_stVpssCtrl[enIp].hThread, VPSS_CPU_INDEX);

			//VPSS_FATAL("bind vpss thread to cpu %d!\n", VPSS_CPU_INDEX);
		}
		else
		{
			VPSS_FATAL("kthread_bind fail, cpu %d is not online!\n", VPSS_CPU_INDEX);
		}
	}
#endif
    wake_up_process(g_stVpssCtrl[enIp].hThread);

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DestoryThread(VPSS_IP_E enIp)
{
    HI_S32 s32Ret;

    s32Ret = kthread_stop(g_stVpssCtrl[enIp].hThread);

    if (s32Ret != HI_SUCCESS)
    {
        VPSS_FATAL("Destory Thread Error.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_CreateInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    DRV_PROC_ITEM_S  *pProcItem;

    HI_OSAL_Snprintf(ProcName, 20, "vpss%02x", hVPSS);

    pProcItem = HI_DRV_PROC_AddModule(ProcName, HI_NULL, HI_NULL);

    if (!pProcItem)
    {
        VPSS_FATAL("Vpss add proc failed!\n");
        return HI_FAILURE;
    }

    pProcItem->data  = (HI_VOID*)(unsigned long)hVPSS;
    pProcItem->read  = VPSS_CTRL_ProcRead;
    pProcItem->write = VPSS_CTRL_ProcWrite;

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_DestoryInstProc(VPSS_HANDLE hVPSS)
{
    HI_CHAR           ProcName[20];
    HI_OSAL_Snprintf(ProcName, 20, "vpss%02x", (HI_U32)(hVPSS));
    HI_DRV_PROC_RemoveModule(ProcName);
    return HI_SUCCESS;
}

HI_U32 VPSS_CTRL_MallocInstanceId(HI_VOID)
{
    HI_U32 i;
    HI_U32 u32InstCount;
    HI_BOOL bFindVpss;
    HI_U32 u32VpssId;
    VPSS_IP_E enVpssIp;
    unsigned long  u32LockFlag;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;

    u32InstCount = 0;

    for(u32VpssId = 0; u32VpssId < VPSS_IP_BUTT * VPSS_INSTANCE_MAX_NUMB; u32VpssId++)
    {
        bFindVpss = HI_FALSE;

        for(enVpssIp = VPSS_IP_0; enVpssIp < VPSS_IP_BUTT; enVpssIp++)
        {
            pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIp].stInstCtrlInfo);
            read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
            for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
            {
                if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
                {
                    if(u32VpssId == pstInstCtrlInfo->pstInstPool[i]->ID)
                    {
                        bFindVpss = HI_TRUE;
                        u32InstCount++;
                        break;
                    }
                }
            }
            read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        }

        if(!bFindVpss)
        {
            break;
        }
    }

    if(u32InstCount == 0)
    {
        return 0;
    }
    else
    {
        return u32VpssId;
    }
}

VPSS_HANDLE VPSS_CTRL_AddInstance(VPSS_INSTANCE_S *pstInstance)
{
    HI_U32 u32VpssId = 0;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    HI_U32 u32Count;
    unsigned long  u32LockFlag;
    VPSS_IP_E enVpssIp;


    enVpssIp = pstInstance->CtrlID;

    if (enVpssIp >= VPSS_IP_BUTT)
    {
        VPSS_FATAL("Invalid enVpssIp:%d\n", enVpssIp);
        return VPSS_INVALID_HANDLE;
    }

    pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIp].stInstCtrlInfo);
    //u32VpssId = VPSS_CTRL_MallocInstanceId();
    write_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count++)
    {
        if (pstInstCtrlInfo->pstInstPool[u32Count] == HI_NULL)
        {
            pstInstCtrlInfo->pstInstPool[u32Count] = pstInstance;
            u32VpssId = u32Count;
            break;
        }
    }
    write_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    if (u32Count == VPSS_INSTANCE_MAX_NUMB)
    {
        VPSS_FATAL("Instance Number is Max.\n");

        return VPSS_INVALID_HANDLE;
    }
    else
    {
        pstInstance->ID = u32VpssId;
        pstInstCtrlInfo->u32InstanceNum++;
        return pstInstance->ID;
    }
}

HI_S32 VPSS_CTRL_DelInstance(VPSS_INSTANCE_S* pstInstance)
{
    HI_U32 i;
    VPSS_IP_E enVpssIP;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    unsigned long  u32LockFlag;

    VPSS_CHECK_NULL(pstInstance);

    enVpssIP = pstInstance->CtrlID;
    pstInstCtrlInfo = &(g_stVpssCtrl[enVpssIP].stInstCtrlInfo);

    write_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
    for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
    {
        if(pstInstCtrlInfo->pstInstPool[i] == pstInstance)
        {
            pstInstCtrlInfo->pstInstPool[i] = HI_NULL;
            break;
        }
    }
    write_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    return HI_SUCCESS;
}



HI_S32 VPSS_CTRL_Init(HI_VOID)
{
	HI_S32 s32Ret;
	HI_U32 i,j;

	g_stVpssCtrl[0].bIPVaild = HI_TRUE;
	g_stVpssCtrl[1].bIPVaild = HI_FALSE;


	for(i = 0; i < VPSS_IP_BUTT; i++)
	{
		if(HI_TRUE != g_stVpssCtrl[i].bIPVaild)
		{
			continue;
		}

		if(0 == g_stVpssCtrl[i].s32IsVPSSOpen)
		{
			s32Ret = VPSS_CTRL_RegistISR((VPSS_IP_E)i);

			if(HI_SUCCESS != s32Ret)
			{
				goto VPSS_IP_DEL_INT;
			}

			VPSS_CTRL_InitInstList((VPSS_IP_E)i);

			s32Ret = VPSS_HAL_Init((VPSS_IP_E)i);
			if(HI_SUCCESS != s32Ret)
			{
				goto VPSS_IP_UnRegist_IRQ;
			}

			s32Ret = VPSS_CTRL_CreateThread((VPSS_IP_E)i);
			if (s32Ret != HI_SUCCESS)
			{
				VPSS_FATAL("VPSS_CTRL_CreateThread Failed\n");
				goto VPSS_IP_HAL_DEL_INIT;
			}
		}

		g_stVpssCtrl[i].s32IsVPSSOpen++;

	}

	return HI_SUCCESS;
#ifdef VPSS_HAL_WITH_CBB
VPSS_IP_HAL_DEL_INIT:
	(HI_VOID)VPSS_HAL_DeInit((VPSS_IP_E)i);
VPSS_IP_UnRegist_IRQ:
	(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)i);

VPSS_IP_DEL_INT:
	for(j = 0; j < i; j++)
	{
		if(HI_TRUE == g_stVpssCtrl[j].bIPVaild)
		{
			(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)j);
			(HI_VOID)VPSS_HAL_DeInit((VPSS_IP_E)j);
			(HI_VOID)VPSS_CTRL_DestoryThread((VPSS_IP_E)j);
		}
	}
#else

VPSS_IP_HAL_DEL_INIT:
		(HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)i);
VPSS_IP_UnRegist_IRQ:
		(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)i);

VPSS_IP_DEL_INT:
		for(j = 0; j < i; j++)
		{
			if(HI_TRUE == g_stVpssCtrl[j].bIPVaild)
			{
				(HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)j);
				(HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)j);
				(HI_VOID)VPSS_CTRL_DestoryThread((VPSS_IP_E)j);
			}
		}
#endif
	return HI_FAILURE;
}

HI_S32 VPSS_CTRL_DelInit(HI_VOID)
{
    HI_U32 i;
    HI_U32 u32Count;
    VPSS_BUFFER_S *pstVpssBuf;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        if(HI_TRUE != g_stVpssCtrl[i].bIPVaild)
        {
            continue;
        }

        if(1 > g_stVpssCtrl[i].s32IsVPSSOpen)
        {
            VPSS_FATAL("CTRL_DelInit Error,vpss hasn't initted.\n");
            return HI_FAILURE;
        }

        g_stVpssCtrl[i].s32IsVPSSOpen--;
        if(1 == g_stVpssCtrl[i].s32IsVPSSOpen)
        {
        for(u32Count = 0; u32Count < VPSS_INSTANCE_MAX_NUMB; u32Count ++)
        {
            if (HI_NULL != g_stVpssCtrl[i].stInstCtrlInfo.pstInstPool[u32Count])
            {
                VPSS_FATAL("CTRL_DelInit Error,destroy instance first.\n");
                g_stVpssCtrl[i].s32IsVPSSOpen++;
                return HI_FAILURE;
                }
            }
            //add for low power
            VPSS_HAL_SetClockEn(i, HI_FALSE);

            //release rotation buffer

            for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count ++)
            {
            #ifdef ZME_2L_TEST
				if ( g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count] != HI_NULL)
				{
					pstVpssBuf = &(g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count]->stBuffer);
					if (pstVpssBuf->stMemBuf.u32Size != 0)
					{
						(HI_VOID)VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
						pstVpssBuf->u32Stride = 0;
						pstVpssBuf->stMemBuf.u32Size = 0;

						VPSS_VFREE(g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count]);
						g_stVpssCtrl[i].stTask.pstFrmNodeRoBuf[u32Count] = HI_NULL;
					}

				}

				if ( g_stVpssCtrl[i].stTask.pstFrmNodeZME1L != HI_NULL)
				{
					pstVpssBuf = &(g_stVpssCtrl[i].stTask.pstFrmNodeZME1L->stBuffer);
					if (pstVpssBuf->stMemBuf.u32Size != 0)
					{
						(HI_VOID)VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
						pstVpssBuf->u32Stride = 0;
						pstVpssBuf->stMemBuf.u32Size = 0;

						VPSS_VFREE(g_stVpssCtrl[i].stTask.pstFrmNodeZME1L);
						g_stVpssCtrl[i].stTask.pstFrmNodeZME1L = HI_NULL;
					}
				}
			#else
                pstVpssBuf = &(g_stVpssCtrl[i].stRoBuf[u32Count]);
                if (pstVpssBuf->stMemBuf.u32Size != 0)
                {
					VPSS_OSAL_FreeMem(&(pstVpssBuf->stMemBuf));
					pstVpssBuf->u32Stride = 0;
					pstVpssBuf->stMemBuf.u32Size = 0;
                }
			#endif
            }
        }

        if(0 == g_stVpssCtrl[i].s32IsVPSSOpen)
        {
            (HI_VOID)VPSS_CTRL_UnRegistISR((VPSS_IP_E)i);
            (HI_VOID)VPSS_CTRL_DestoryThread((VPSS_IP_E)i);
#ifdef VPSS_HAL_WITH_CBB
            (HI_VOID)VPSS_HAL_DeInit((VPSS_IP_E)i);
#else
            (HI_VOID)VPSS_HAL_DelInit((VPSS_IP_E)i);
#endif
        }
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_SetMceFlag(HI_BOOL bInMCE)
{
    HI_U32 i;

    for(i = 0; i < VPSS_IP_BUTT; i++)
    {
        g_stVpssCtrl[i].bInMCE = bInMCE;
    }

    return HI_SUCCESS;
}

VPSS_HANDLE VPSS_CTRL_CreateInstance(HI_DRV_VPSS_CFG_S *pstVpssCfg)
{
    HI_S32 s32Ret;
    HI_S32 s32InstHandle = VPSS_INVALID_HANDLE;
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    s32Ret = VPSS_CTRL_GetDistributeIP(&enVpssIp);
    if(HI_SUCCESS != s32Ret)
    {
        VPSS_FATAL("vpss ctrl isn't enable\n");
        return s32InstHandle;
    }

    if (1 >= g_stVpssCtrl[enVpssIp].s32IsVPSSOpen)
    {
        VPSS_FATAL("vpss ctrl isn't opened\n");
        return s32InstHandle;
    }

    pstInstance = (VPSS_INSTANCE_S*)VPSS_VMALLOC(sizeof(VPSS_INSTANCE_S));
    if (pstInstance != HI_NULL)
    {
        if (pstVpssCfg)
        {
            s32Ret = VPSS_INST_Init(pstInstance,pstVpssCfg);
        }
        else
        {
            HI_DRV_VPSS_CFG_S stDefCfg;
            VPSS_INST_GetDefInstCfg(&stDefCfg);
            s32Ret = VPSS_INST_Init(pstInstance,&stDefCfg);
        }
        if(HI_SUCCESS != s32Ret)
        {
            VPSS_VFREE(pstInstance);
            return s32InstHandle;
        }

        pstInstance->CtrlID = enVpssIp;
#ifdef VPSS_SUPPORT_PROC_V2
        pstInstance->u32LogicTimeoutCnt = 0;
#endif
        s32InstHandle = VPSS_CTRL_AddInstance(pstInstance);
        if (s32InstHandle != VPSS_INVALID_HANDLE)
        {
            if (g_stVpssCtrl[enVpssIp].bInMCE == HI_FALSE)
            {
                (HI_VOID)VPSS_CTRL_CreateInstProc(pstInstance->ID);
            }
        }
        else
        {
            VPSS_VFREE(pstInstance);
        }

        return s32InstHandle;
    }
    else
    {
        VPSS_FATAL("vmalloc instance node failed \n");
        s32Ret = HI_FAILURE;
        return s32InstHandle;
    }

}

HI_S32 VPSS_CTRL_DestoryInstance(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);
    if (pstInstance == HI_NULL)
    {
        VPSS_FATAL("hVPSS(%d) is Not be Vaild Or Created\n", hVPSS);
        return HI_FAILURE;
    }

    if (pstInstance->enState != INSTANCE_STATE_STOP)
    {
        VPSS_FATAL("Instance %#x is still working, please stop first\n");
        return HI_FAILURE;
    }

    /*
      *  when deletting instance
      *  must get lock first to ensure that it isn't being served
     */
    enVpssIp = pstInstance->CtrlID;
    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 1;

    while(g_stVpssCtrl[enVpssIp].s32ThreadPos != 5)
    {
        msleep(10);
    }

    if (g_stVpssCtrl[enVpssIp].bInMCE == HI_FALSE)
    {
        VPSS_CTRL_DestoryInstProc(hVPSS);
    }

    g_stVpssCtrl[enVpssIp].stInstCtrlInfo.u32InstanceNum--;
	VPSS_HAL_H265RefListDeInit(&pstInstance->stH265RefList);
    VPSS_CTRL_DelInstance(pstInstance);
    (HI_VOID)VPSS_INST_DelInit(pstInstance);
    VPSS_VFREE(pstInstance);

    pstInstance = HI_NULL;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 0;
    return HI_SUCCESS;

}
HI_S32 VPSS_CTRL_GetInstanceNumber(VPSS_IP_E enIp,HI_U32 *pu32Number)
{
	HI_U32 i;
	VPSS_INST_CTRL_S *pstInstCtrlInfo;
	unsigned long u32LockFlag;
	HI_U32 u32Cnt = 0;

	pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);
	read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
	for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
	{
		if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
		{
			u32Cnt ++;
		}
	}
	read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	*pu32Number = u32Cnt;

	return HI_SUCCESS;
}

VPSS_INSTANCE_S *VPSS_CTRL_GetOnlyInstance(VPSS_IP_E enIp)
{
	HI_U32 u32InstNumber;
	VPSS_INST_CTRL_S *pstInstCtrlInfo;
	VPSS_INSTANCE_S *pstRetPtr = HI_NULL;
    unsigned long u32LockFlag;
    HI_U32 i;

	VPSS_CTRL_GetInstanceNumber(VPSS_IP_0,&u32InstNumber);

	if (u32InstNumber != 1)
	{
		return HI_NULL;
	}

	pstInstCtrlInfo = &(g_stVpssCtrl[enIp].stInstCtrlInfo);

	read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
	{
		if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
		{
			pstRetPtr = pstInstCtrlInfo->pstInstPool[i];
		}
	}

	read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

	return pstRetPtr;
}

VPSS_INSTANCE_S* VPSS_CTRL_GetInstance(VPSS_HANDLE hVPSS)
{
    HI_U32 i;
    HI_U32 u32IpPos;
    VPSS_INST_CTRL_S *pstInstCtrlInfo;
    VPSS_INSTANCE_S *pstRetPtr = HI_NULL;
    unsigned long u32LockFlag;


    if ((hVPSS < 0) || (hVPSS >= VPSS_INSTANCE_MAX_NUMB * VPSS_IP_BUTT))
    {
        VPSS_FATAL("Invalid VPSS HANDLE %x.\n",hVPSS);
        return HI_NULL;
    }

    for(u32IpPos = VPSS_IP_0; u32IpPos < VPSS_IP_BUTT; u32IpPos++)
    {
        pstInstCtrlInfo = &(g_stVpssCtrl[u32IpPos].stInstCtrlInfo);
        read_lock_irqsave(&(pstInstCtrlInfo->stListLock),u32LockFlag);
        for(i = 0; i < VPSS_INSTANCE_MAX_NUMB; i++)
		{
			if(pstInstCtrlInfo->pstInstPool[i] != HI_NULL )
			{
				if(hVPSS == pstInstCtrlInfo->pstInstPool[i]->ID)
				{
					pstRetPtr = pstInstCtrlInfo->pstInstPool[i];
					break;
				}
			}
		}
        read_unlock_irqrestore(&(pstInstCtrlInfo->stListLock),u32LockFlag);

    }

    return pstRetPtr;
}

HI_S32 VPSS_CTRL_WakeUpThread(HI_VOID)
{
    if (g_stVpssCtrl[VPSS_IP_0].s32IsVPSSOpen >= 1)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stNewTask), 1, 0);
    }

    if (g_stVpssCtrl[VPSS_IP_1].s32IsVPSSOpen >= 1)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stNewTask), 1, 0);
    }

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Pause(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;
	HI_U32 u32SleepNumb = 0;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    VPSS_CHECK_NULL(pstInstance);

    enVpssIp = pstInstance->CtrlID;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 1;

    while(g_stVpssCtrl[enVpssIp].s32ThreadPos != 5 && u32SleepNumb < 200)
    {
        msleep(10);

		u32SleepNumb ++;
    }

	if (u32SleepNumb >= 200)
	{
		VPSS_ERROR("thread sleep failed pos %d\n",g_stVpssCtrl[enVpssIp].s32ThreadPos);
	}

    return HI_SUCCESS;
}

HI_S32 VPSS_CTRL_Resume(VPSS_HANDLE hVPSS)
{
    VPSS_IP_E enVpssIp = VPSS_IP_BUTT;
    VPSS_INSTANCE_S* pstInstance;

    pstInstance = VPSS_CTRL_GetInstance(hVPSS);

    VPSS_CHECK_NULL(pstInstance);

    enVpssIp = pstInstance->CtrlID;

    g_stVpssCtrl[enVpssIp].u32ThreadSleep = 0;

    VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[enVpssIp].stNewTask), 1, 0);

    return HI_SUCCESS;
}
#ifndef VPSS_HAL_WITH_CBB
irqreturn_t VPSS0_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32State = 0;

    (HI_VOID)VPSS_HAL_GetIntState(VPSS_IP_0,&u32State);

#if 0
    if(u32State & 0x80)
    {
        VPSS_FATAL("IRQ DCMP ERR    state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x80);
    }
#endif

    if(u32State & 0x4)
    {
        VPSS_FATAL("IRQ BUS ERR  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x4);
    }

    if(u32State & 0x2)
    {
        VPSS_FATAL("TIMEOUT  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x2);
    }

    if(u32State & 0x70)//   0xf ---> 0xff open tunl mask and dcmp err mask
    {
        VPSS_FATAL(" Tunnel = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x70);
    }
#ifdef VPSS_SUPPORT_OUT_TUNNEL
    if (u32State & 0x10)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskLowDelay),EVENT_DONE,EVENT_UNDO);
    }
#endif
    if(u32State & 0x1)
    {
        //VPSS_FATAL("NODE  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x1);

#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[0][g_u32Pos]));
#endif
    }

    if(u32State & 0x88)
    {
    #if 1
        VPSS_HAL_ClearIntState(VPSS_IP_0,0x88);

#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[1][g_u32Pos]));

		g_u32LogicEnd = jiffies;

		g_u32LogicTime[g_u32Pos] = g_u32LogicEnd - g_u32LogicStart;

		g_u32Pos = (g_u32Pos+1)%15;
#endif
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskNext),EVENT_DONE,EVENT_UNDO);

        #endif
    }
    return IRQ_HANDLED;
}

irqreturn_t VPSS1_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
    HI_U32 u32State = 0;

    (HI_VOID)VPSS_HAL_GetIntState(VPSS_IP_1,&u32State);

    if(u32State & 0x80)
    {
        VPSS_FATAL("IRQ DCMP ERR    state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x80);
    }

    if(u32State & 0x4)
    {
        VPSS_FATAL("IRQ BUS ERR  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x4);
    }

    if(u32State & 0x2)
    {
        VPSS_FATAL("TIMEOUT  state = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x2);
    }

    if(u32State & 0x70)//   0xf ---> 0xff open tunl mask and dcmp err mask
    {
        VPSS_FATAL(" Tunnel = %x \n", u32State);
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x70);
    }
#ifdef VPSS_SUPPORT_OUT_TUNNEL
    if (u32State & 0x10)
    {
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stTaskLowDelay),EVENT_DONE,EVENT_UNDO);
    }
#endif

    if(u32State & 0x1)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x1);
    }

    if(u32State & 0x8)
    {
        VPSS_HAL_ClearIntState(VPSS_IP_1,0x8);
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stTaskNext),EVENT_DONE,EVENT_UNDO);
    }

    return IRQ_HANDLED;
}
#else  //98mv200
irqreturn_t VPSS0_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
	HI_BOOL abISRStat[CBB_ISR_BUTT] = {0};
	HI_BOOL abISRStatMask[CBB_ISR_BUTT] = {0};
	CBB_REG_ADDR_S stRegAddr = {0};
	HI_U32 u32IntState;
	HI_U32 u32IntStateSMMU;

	stRegAddr.pu8BaseVirAddr = stHalCtx[VPSS_IP_0].pu8BaseRegVir;
    VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_GET_ISR_STAT, abISRStat, HI_NULL);

	VPSS_SAFE_MEMCPY(abISRStatMask, abISRStat, sizeof(abISRStatMask));
	VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_CLR_ISR_STAT, abISRStatMask, HI_NULL);

	u32IntState = ((abISRStat[CBB_ISR_NODE_COMPLETE]	<< CBB_ISR_NODE_COMPLETE)
	 |(abISRStat[CBB_ISR_LIST_COMPLETE] << CBB_ISR_LIST_COMPLETE)
	 |(abISRStat[CBB_ISR_BUS_READ_ERR]  << CBB_ISR_BUS_READ_ERR	)
	 |(abISRStat[CBB_ISR_BUS_WRITE_ERR] << CBB_ISR_BUS_WRITE_ERR)
	 |(abISRStat[CBB_ISR_DCMP_ERR] 	 	<< CBB_ISR_DCMP_ERR		)
	 |(abISRStat[CBB_ISR_VHD0_TUNNEL]	<< CBB_ISR_VHD0_TUNNEL )
	 );

	//VPSS_FATAL("VPSS IP0 int (0x%X) ...\n", u32IntState);

	if (abISRStat[CBB_ISR_SMMU_S_TLBMISS] || abISRStat[CBB_ISR_SMMU_S_PTW_TRANS]
		|| abISRStat[CBB_ISR_SMMU_S_TLBINVALID_RD] || abISRStat[CBB_ISR_SMMU_S_TLBINVALID_WR])
	{
		u32IntStateSMMU = (abISRStat[CBB_ISR_SMMU_S_TLBMISS] << (CBB_ISR_SMMU_S_TLBMISS - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_PTW_TRANS] << (CBB_ISR_SMMU_S_PTW_TRANS - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_TLBINVALID_RD] << (CBB_ISR_SMMU_S_TLBINVALID_RD - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_TLBINVALID_WR] << (CBB_ISR_SMMU_S_TLBINVALID_WR - CBB_ISR_SMMU_S_MIN));

		VPSS_FATAL("IRQ SMMU-S ERR state = %x \n", u32IntStateSMMU);
	}

 	if (abISRStat[CBB_ISR_SMMU_NS_TLBMISS] || abISRStat[CBB_ISR_SMMU_NS_PTW_TRANS]
		|| abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_RD] || abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_WR])
	{
		u32IntStateSMMU = (abISRStat[CBB_ISR_SMMU_NS_TLBMISS] << (CBB_ISR_SMMU_NS_TLBMISS - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_PTW_TRANS] << (CBB_ISR_SMMU_NS_PTW_TRANS - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_RD] << (CBB_ISR_SMMU_NS_TLBINVALID_RD - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_WR] << (CBB_ISR_SMMU_NS_TLBINVALID_WR - CBB_ISR_SMMU_NS_MIN));

		VPSS_FATAL("IRQ SMMU-NS ERR  state = %x \n", u32IntStateSMMU);
	}

    if (abISRStat[CBB_ISR_BUS_WRITE_ERR] || abISRStat[CBB_ISR_BUS_READ_ERR])
    {
        VPSS_FATAL("IRQ BUS W/R ERR  state = %x \n", u32IntState);
    }

    if (abISRStat[CBB_ISR_TIMEOUT])
    {
        VPSS_FATAL("IRQ TIMEOUT state = %x \n", u32IntState);
    }

    if (abISRStat[CBB_ISR_DCMP_ERR])
    {
        VPSS_FATAL("IRQ  DCMP ERR state = %x \n", u32IntState);
    }
#ifdef VPSS_SUPPORT_OUT_TUNNEL
    if (abISRStat[CBB_ISR_VHD0_TUNNEL])
    {
		VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskLowDelay),EVENT_DONE,EVENT_UNDO);
    }
#endif
#if DEF_VPSS_STATIC
    if (abISRStat[CBB_ISR_NODE_COMPLETE])
    {
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[0][g_u32Pos]));
    }
#endif

    if (abISRStat[CBB_ISR_LIST_COMPLETE])
    {
#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_0,&(g_u32LogicCycle[1][g_u32Pos]));
		g_u32LogicEnd = jiffies;
		g_u32LogicTime[g_u32Pos] = g_u32LogicEnd - g_u32LogicStart;
		g_u32Pos = (g_u32Pos+1)%15;
#endif
        VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_0].stTaskNext),EVENT_DONE,EVENT_UNDO);
    }

    return IRQ_HANDLED;
}

irqreturn_t VPSS1_CTRL_IntService(HI_S32 irq, HI_VOID *dev_id)
{
	HI_BOOL abISRStat[CBB_ISR_BUTT] = {0};
	HI_BOOL abISRStatMask[CBB_ISR_BUTT] = {0};
	CBB_REG_ADDR_S stRegAddr = {0};
	HI_U32 u32IntState;
	HI_U32 u32IntStateSMMU;

	stRegAddr.pu8BaseVirAddr = stHalCtx[VPSS_IP_1].pu8BaseRegVir;
	VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_GET_ISR_STAT, abISRStat, HI_NULL);

	VPSS_SAFE_MEMCPY(abISRStatMask, abISRStat, sizeof(abISRStatMask));
	VPSS_CBB_BASE_Operating(stRegAddr, CBB_BASEOPT_CLR_ISR_STAT, abISRStatMask, HI_NULL);

	u32IntState = ((abISRStat[CBB_ISR_NODE_COMPLETE]	<< CBB_ISR_NODE_COMPLETE)
	 |(abISRStat[CBB_ISR_LIST_COMPLETE] << CBB_ISR_LIST_COMPLETE)
	 |(abISRStat[CBB_ISR_BUS_READ_ERR]	<< CBB_ISR_BUS_READ_ERR )
	 |(abISRStat[CBB_ISR_BUS_WRITE_ERR] << CBB_ISR_BUS_WRITE_ERR)
	 |(abISRStat[CBB_ISR_DCMP_ERR]		<< CBB_ISR_DCMP_ERR 	)
	 |(abISRStat[CBB_ISR_VHD0_TUNNEL]	<< CBB_ISR_VHD0_TUNNEL )
	 );

	//VPSS_FATAL("VPSS IP1 int (0x%X) ...\n", u32IntState);

	if (abISRStat[CBB_ISR_SMMU_S_TLBMISS] || abISRStat[CBB_ISR_SMMU_S_PTW_TRANS]
		|| abISRStat[CBB_ISR_SMMU_S_TLBINVALID_RD] || abISRStat[CBB_ISR_SMMU_S_TLBINVALID_WR])
	{
		u32IntStateSMMU = (abISRStat[CBB_ISR_SMMU_S_TLBMISS] << (CBB_ISR_SMMU_S_TLBMISS - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_PTW_TRANS] << (CBB_ISR_SMMU_S_PTW_TRANS - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_TLBINVALID_RD] << (CBB_ISR_SMMU_S_TLBINVALID_RD - CBB_ISR_SMMU_S_MIN))
						+ (abISRStat[CBB_ISR_SMMU_S_TLBINVALID_WR] << (CBB_ISR_SMMU_S_TLBINVALID_WR - CBB_ISR_SMMU_S_MIN));

		VPSS_FATAL("IRQ SMMU-S ERR state = %x \n", u32IntStateSMMU);
	}

	if (abISRStat[CBB_ISR_SMMU_NS_TLBMISS] || abISRStat[CBB_ISR_SMMU_NS_PTW_TRANS]
		|| abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_RD] || abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_WR])
	{
		u32IntStateSMMU = (abISRStat[CBB_ISR_SMMU_NS_TLBMISS] << (CBB_ISR_SMMU_NS_TLBMISS - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_PTW_TRANS] << (CBB_ISR_SMMU_NS_PTW_TRANS - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_RD] << (CBB_ISR_SMMU_NS_TLBINVALID_RD - CBB_ISR_SMMU_NS_MIN))
								+ (abISRStat[CBB_ISR_SMMU_NS_TLBINVALID_WR] << (CBB_ISR_SMMU_NS_TLBINVALID_WR - CBB_ISR_SMMU_NS_MIN));

		VPSS_FATAL("IRQ SMMU-NS ERR  state = %x \n", u32IntStateSMMU);
	}

	if (abISRStat[CBB_ISR_BUS_WRITE_ERR])
	{
		VPSS_FATAL("IRQ BUS ERR  state = %x \n", u32IntState);
	}


	if (abISRStat[CBB_ISR_TIMEOUT])
	{
		VPSS_FATAL("TIMEOUT  state = %x \n", u32IntState);
	}


	if (abISRStat[CBB_ISR_DCMP_ERR] || abISRStat[CBB_ISR_BUS_READ_ERR])
	{
		VPSS_FATAL(" Tunnel = %x \n", u32IntState);
	}

	if (abISRStat[CBB_ISR_NODE_COMPLETE])
	{
#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_1,&(g_u32LogicCycle[0][g_u32Pos]));
#endif
	}

	if (abISRStat[CBB_ISR_LIST_COMPLETE])
	{
#if DEF_VPSS_STATIC
		VPSS_HAL_GetCycleCnt(VPSS_IP_1,&(g_u32LogicCycle[1][g_u32Pos]));
		g_u32LogicEnd = jiffies;
		g_u32LogicTime[g_u32Pos] = g_u32LogicEnd - g_u32LogicStart;
		g_u32Pos = (g_u32Pos+1)%15;
#endif
		VPSS_OSAL_GiveEvent(&(g_stVpssCtrl[VPSS_IP_1].stTaskNext),EVENT_DONE,EVENT_UNDO);
	}

	return IRQ_HANDLED;
}


#endif

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3798cv200_a)|| defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#ifdef VPSS_SUPPORT_PROC_V2
HI_VOID VPSS_CTRL_ProcPrintWBCList(struct seq_file *p, VPSS_IN_ENTITY_S* pstInEntity)
{
    PROC_PRINT(p, VPSS_PROC_LEVEL2 "ALG WBC List : ");
    if (pstInEntity->stWbcInfo[0].bInit)
    {
        PROC_PRINT(p, "wbc ");
    }
    if (pstInEntity->stSttWbc[0].bInit)
    {
        PROC_PRINT(p, "stt-wbc ");
    }
    if (pstInEntity->stDieStInfo[0].bInit)
    {
        PROC_PRINT(p, "dei-st ");
    }
    if (pstInEntity->stNrMadInfo[0].bInit)
    {
        PROC_PRINT(p, "nr-mad ");
    }
    if (pstInEntity->stMcdeiInfo[0].stRgmeInfo.bInit)
    {
        PROC_PRINT(p, "rgme ");
    }
    if (pstInEntity->stMcdeiInfo[0].stRgmeWbcInfo.bInit)
    {
        PROC_PRINT(p, "rgme-wbc ");
    }
    if (pstInEntity->stMcdeiInfo[0].stBlendWbcInfo.bInit)
    {
        PROC_PRINT(p, "blend-wbc ");
    }
    if (pstInEntity->stMcdeiInfo[0].stBlkmvInfo.bInit)
    {
        PROC_PRINT(p, "blkmv ");
    }
    if (pstInEntity->stMcdeiInfo[0].stPrjhInfo.bInit)
    {
        PROC_PRINT(p, "prjh ");
    }
    if (pstInEntity->stMcdeiInfo[0].stPrjhInfo.bInit)
    {
        PROC_PRINT(p, "prjv ");
    }
}
HI_VOID VPSS_CTRL_ProcPrintStreamInfo(struct seq_file *p, VPSS_IN_STREAM_INFO_S* pstStreamInfo)
{
    PROC_PRINT(p, VPSS_PROC_LEVEL3"Src Image Info"
                        VPSS_PROC_LEVEL4"Progress       : %s"
                        VPSS_PROC_LEVEL4"FieldMode      : %s"
                        VPSS_PROC_LEVEL4"TopFirst       : %s"
                        VPSS_PROC_LEVEL4"BufValid       : %s"
                        VPSS_PROC_LEVEL4"Security       : %s"
                        VPSS_PROC_LEVEL4"Width*Heigth   : %d * %d"
                        VPSS_PROC_LEVEL4"AspectRatio    : %d / %d"
                        VPSS_PROC_LEVEL4"FrameType      : %s(%d)"
                        VPSS_PROC_LEVEL4"PixelFormat    : %s(%d)"
                        VPSS_PROC_LEVEL4"BitWidth       : %s(%d)"
                        VPSS_PROC_LEVEL4"3DType         : %s(%d)"
                        VPSS_PROC_LEVEL4"H265iIn2Frm    : %s"
              ,
              VPSS_PROC_PRINT_YN(pstStreamInfo->u32StreamProg),
              (HI_DRV_FIELD_ALL == pstStreamInfo->enFieldMode) ? "All" :
                            ((HI_DRV_FIELD_TOP == pstStreamInfo->enFieldMode) ? "Top" : "Bot"),
              VPSS_PROC_PRINT_YN(pstStreamInfo->bTopFieldFirst),
              (HI_DRV_FIELD_ALL == pstStreamInfo->enBufValidMode) ? "All" : "T/B",
              VPSS_PROC_PRINT_YN_COL(pstStreamInfo->bSecure),
              pstStreamInfo->u32StreamW, pstStreamInfo->u32StreamH,
              pstStreamInfo->u32StreamAspW, pstStreamInfo->u32StreamAspH,
              VPSS_PROC_PRINT_ENUM(pstStreamInfo->enSrcFrameType, HI_DRV_VIDEO_FRAME_TYPE_BUTT, g_pcFrameTypeString), pstStreamInfo->enSrcFrameType,
              VPSS_PROC_PRINT_ENUM(pstStreamInfo->ePixFormat, HI_DRV_PIX_BUTT, g_pcPixelFormatString), pstStreamInfo->ePixFormat,
              VPSS_PROC_PRINT_ENUM(pstStreamInfo->enBitWidth, HI_DRV_PIXEL_BITWIDTH_BUTT, g_pOutBitWidthString),
              pstStreamInfo->enBitWidth,
              VPSS_PROC_PRINT_ENUM(pstStreamInfo->eFrmType, HI_DRV_FT_BUTT, g_pc3DTypeString), pstStreamInfo->eFrmType,
              VPSS_PROC_PRINT_YN(pstStreamInfo->bH265Interlace)
        );
}
HI_VOID VPSS_CTRL_ProcPrintHalInfo(struct seq_file *p, S_VPSS_REGS_TYPE *pstNodeReg)
{
    PROC_PRINT(p,
                    VPSS_PROC_LEVEL5"Image   field:%u fieldmode:%u topfirst:%u in_format:%u  in_tunl_en:%u security:%u"
                    VPSS_PROC_LEVEL5"Next2   tile :%u dcmp:%u bitw:%u W*H:%u*%u   yaddr:%08x ystride:%u  yheadaddr:%08x headstride:%u"
                    VPSS_PROC_LEVEL5"Ref     tile :%u bitw:%u         W*H:%u*%u   yaddr:%08x ystride:%u"
                    VPSS_PROC_LEVEL5"Cur                                        yaddr:%08x ystride:%u"
                    VPSS_PROC_LEVEL5"Next1                                      yaddr:%08x ystride:%u"
                    VPSS_PROC_LEVEL5"Rfr     enable :%u dither_en:%u bitw:%u    yaddr:%08x ystride:%u"
                    VPSS_PROC_LEVEL5"Alg     tnr_en :%u snr_en:%u dei_en:%u mcdi_en:%u rgme_en:%u ifmd_en:%u igbm_en:%u db_en:%u blk_det_en:%u"
                    VPSS_PROC_LEVEL5"VHD0    4pc_en :%u bitw:%u   format:%u  rota_en:%u flip_en:%u cts_en:%u  tunl_en:%u vfir_en:%u vfirmode:%u"
                    VPSS_PROC_LEVEL5"        crop_en:%u xywh: %u,%u,%u,%u"
                    VPSS_PROC_LEVEL5"        zme_en :%u   wh: %u,%u"
                    VPSS_PROC_LEVEL5"        lbx_en :%u xywh: %u,%u,%u,%u"
                    VPSS_PROC_LEVEL5"        vhd0_en:%u   wh: %u,%u"
                    ,
                    pstNodeReg->VPSS_CTRL.bits.bfield,
                    pstNodeReg->VPSS_CTRL.bits.bfield_mode,
                    !pstNodeReg->VPSS_CTRL.bits.bfield_first,
                    pstNodeReg->VPSS_IN_CTRL.bits.in_format,
                    pstNodeReg->VPSS_NX2_TUNL_CTRL.bits.nx2_tunl_en,
                    pstNodeReg->VPSS_CTRL.bits.prot,
                    pstNodeReg->VPSS_NXT2_CTRL.bits.nxt2_tile_format,
                    pstNodeReg->VPSS_NXT2_CTRL.bits.nxt2_dcmp_en,
                    pstNodeReg->VPSS_IN_CTRL.bits.in_pix_bitw,
                    pstNodeReg->VPSS_IMGSIZE.bits.imgwidth  + 1,
                    pstNodeReg->VPSS_IMGSIZE.bits.imgheight + 1,
                    pstNodeReg->VPSS_NXT2YADDR.u32,
                    pstNodeReg->VPSS_NXT2STRIDE.bits.nxt2y_stride,
                    pstNodeReg->VPSS_NX2Y_HEAD_ADDR.u32,
                    pstNodeReg->VPSS_NX2_HEAD_STRIDE.bits.nx2_head_stride,
                    pstNodeReg->VPSS_REF_CTRL.bits.ref_tile_format,
                    pstNodeReg->VPSS_IN_CTRL.bits.ref_nxt_pix_bitw,
                    pstNodeReg->VPSS_IMGSIZE.bits.imgwidth  + 1,
                    pstNodeReg->VPSS_IMGSIZE.bits.imgheight + 1,
                    pstNodeReg->VPSS_REFYADDR.u32,
                    pstNodeReg->VPSS_REFSTRIDE.bits.refy_stride,
                    pstNodeReg->VPSS_CURYADDR.u32,
                    pstNodeReg->VPSS_CURSTRIDE.bits.cury_stride,
                    pstNodeReg->VPSS_NXT1YADDR.u32,
                    pstNodeReg->VPSS_NXT1STRIDE.bits.nxt1y_stride,
                    pstNodeReg->VPSS_CTRL.bits.rfr_en,
                    pstNodeReg->VPSS_RFRCTRL.bits.rfr_dither_en,
                    pstNodeReg->VPSS_IN_CTRL.bits.rfr_pix_bitw,
                    pstNodeReg->VPSS_RFRYADDR.u32,
                    pstNodeReg->VPSS_RFRSTRIDE.bits.rfry_stride,
                    pstNodeReg->VPSS_CTRL.bits.tnr_en,
                    pstNodeReg->VPSS_CTRL.bits.snr_en,
                    pstNodeReg->VPSS_CTRL.bits.dei_en,
                    pstNodeReg->VPSS_CTRL.bits.mcdi_en,
                    pstNodeReg->VPSS_CTRL.bits.rgme_en,
                    pstNodeReg->VPSS_CTRL.bits.ifmd_en,
                    pstNodeReg->VPSS_CTRL.bits.igbm_en,
                    pstNodeReg->VPSS_DB_CTRL.bits.db_en,
                    pstNodeReg->VPSS_CTRL.bits.blk_det_en,
                    pstNodeReg->VPSS_CTRL.bits.four_pix_en,
                    pstNodeReg->VPSS_VHD0CTRL.bits.vhd0_pix_bitw,
                    pstNodeReg->VPSS_VHD0CTRL.bits.vhd0_format,
                    pstNodeReg->VPSS_CTRL.bits.rotate_en,
                    pstNodeReg->VPSS_VHD0CTRL.bits.vhd0_flip,
                    pstNodeReg->VPSS_IN_CTRL.bits.cts_en,
                    pstNodeReg->VPSS_VHD0_TUNL_CTRL.bits.vhd0_tunl_en,
                    pstNodeReg->VPSS_CTRL3.bits.vhd0_pre_vfir_en,
                    pstNodeReg->VPSS_VPZME_CFG0.bits.vhd0_pre_vfir_mode,
                    pstNodeReg->VPSS_CTRL3.bits.vhd0_crop_en,
                    pstNodeReg->VPSS_VHD0CROP_POS.bits.vhd0_crop_x,
                    pstNodeReg->VPSS_VHD0CROP_POS.bits.vhd0_crop_y,
                    pstNodeReg->VPSS_VHD0CROP_SIZE.bits.vhd0_crop_width + 1,
                    pstNodeReg->VPSS_VHD0CROP_SIZE.bits.vhd0_crop_height + 1,
                    pstNodeReg->VPSS_CTRL3.bits.zme_vhd0_en,
                    pstNodeReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_ow+1,
                    pstNodeReg->VPSS_VHD0_ZMEORESO.bits.vhd0_zme_oh+1,
                    pstNodeReg->VPSS_CTRL3.bits.vhd0_lba_en,
                    pstNodeReg->VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_xfpos,
                    pstNodeReg->VPSS_VHD0LBA_VFPOS.bits.vhd0_lba_yfpos,
                    pstNodeReg->VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_width  + 1,
                    pstNodeReg->VPSS_VHD0LBA_DSIZE.bits.vhd0_lba_height + 1,
                    pstNodeReg->VPSS_CTRL.bits.vhd0_en,
                    pstNodeReg->VPSS_VHD0SIZE.bits.vhd0_width + 1,
                    pstNodeReg->VPSS_VHD0SIZE.bits.vhd0_height + 1
             );
}
HI_VOID VPSS_CTRL_ProcPrintPortUserCfg(struct seq_file *p, VPSS_INSTANCE_S* pstInstance)
{
    HI_U32 u32Count;
    VPSS_PORT_S *pstPort;
    HI_DRV_VPSS_PORT_CFG_S *pstUsrPortCfg;
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
       pstPort = &(pstInstance->stPort[u32Count]);
       PROC_PRINT(p,  VPSS_PROC_LEVEL3 "Port%d Config Info", u32Count);
       if (pstPort->bEnble)
       {
           pstUsrPortCfg = &(pstInstance->stUsrPortCfg[u32Count]);
           PROC_PRINT(p,
                           VPSS_PROC_LEVEL4"Bypass         : %s"
                           VPSS_PROC_LEVEL4"3DSupport      : %s"
                           VPSS_PROC_LEVEL4"Width          : %d"
                           VPSS_PROC_LEVEL4"Height         : %d"
                           VPSS_PROC_LEVEL4"BitWidth       : %s"
                           VPSS_PROC_LEVEL4"PixelFormat    : %s(%d)"
                           VPSS_PROC_LEVEL4"Rotation       : %s"
                           VPSS_PROC_LEVEL4"FlipV          : %s"
                           VPSS_PROC_LEVEL4"FlipH          : %s"
                           VPSS_PROC_LEVEL4"MaxFrameRate   : %d"
                           ,
                       VPSS_PROC_PRINT_YN(pstUsrPortCfg->bPassThrough),
                       VPSS_PROC_PRINT_YN(pstUsrPortCfg->b3Dsupport),
                       pstUsrPortCfg->s32OutputWidth,
                       pstUsrPortCfg->s32OutputHeight,
                       VPSS_PROC_PRINT_ENUM(pstUsrPortCfg->enOutBitWidth, HI_DRV_PIXEL_BITWIDTH_BUTT, g_pOutBitWidthString),
                       VPSS_PROC_PRINT_ENUM(pstUsrPortCfg->eFormat, HI_DRV_PIX_BUTT, g_pcPixelFormatString),
                       pstUsrPortCfg->eFormat,
                       VPSS_PROC_PRINT_ENUM(pstUsrPortCfg->enRotation, HI_DRV_VPSS_ROTATION_BUTT, g_pRotateString),
                       VPSS_PROC_PRINT_YN(pstUsrPortCfg->bVertFlip),
                       VPSS_PROC_PRINT_YN(pstUsrPortCfg->bHoriFlip),
                       pstUsrPortCfg->u32MaxFrameRate
                    );
            PROC_PRINT(p,
                           VPSS_PROC_LEVEL4"UseCrop        : %s"
                           VPSS_PROC_LEVEL4"Crop(LTRB)     : %d, %d, %d, %d"
                           VPSS_PROC_LEVEL4"AspectMode     : %s(%d)"
                           VPSS_PROC_LEVEL4"InRect(XYWH)   : %d, %d, %d, %d"
                           VPSS_PROC_LEVEL4"VideoRect(XYWH): %d, %d, %d, %d"
                           VPSS_PROC_LEVEL4"Screen(XYWH)   : %d, %d, %d, %d"
                           VPSS_PROC_LEVEL4"CustomAR(W/H)  : %d / %d"
                           VPSS_PROC_LEVEL4"DispPixAR(W/H) : %d / %d"
                           ,
                           VPSS_PROC_PRINT_YN(pstUsrPortCfg->bUseCropRect),
                           pstUsrPortCfg->stCropRect.u32LeftOffset,
                           pstUsrPortCfg->stCropRect.u32TopOffset,
                           pstUsrPortCfg->stCropRect.u32RightOffset,
                           pstUsrPortCfg->stCropRect.u32BottomOffset,
                           VPSS_PROC_PRINT_ENUM(pstUsrPortCfg->eAspMode, HI_DRV_ASP_RAT_MODE_BUTT, g_pAspString),
                           pstUsrPortCfg->eAspMode,
                           pstUsrPortCfg->stInRect.s32X, pstUsrPortCfg->stInRect.s32Y,
                           pstUsrPortCfg->stInRect.s32Width, pstUsrPortCfg->stInRect.s32Height,
                           pstUsrPortCfg->stVideoRect.s32X, pstUsrPortCfg->stVideoRect.s32Y,
                           pstUsrPortCfg->stVideoRect.s32Width, pstUsrPortCfg->stVideoRect.s32Height,
                           pstUsrPortCfg->stCustmAR.u32ARw, pstUsrPortCfg->stCustmAR.u32ARh,
                           pstUsrPortCfg->stDispPixAR.u32ARw, pstUsrPortCfg->stDispPixAR.u32ARh,
                           pstUsrPortCfg->stScreen.s32X, pstUsrPortCfg->stScreen.s32Y,
                           pstUsrPortCfg->stScreen.s32Width, pstUsrPortCfg->stScreen.s32Height
                    );
       }
       else
       {
           PROC_PRINT(p, VPSS_PROC_LEVEL4"Port is disable.");
       }
    }
}
HI_VOID VPSS_CTRL_ProcPrintTimeStamp(struct seq_file *p, VPSS_INSTANCE_S* pstInstance)
{
    HI_U32 u32Count;
    for(u32Count = 0; u32Count < VPSS_PROC_TIMESTAMP_MAXCNT; u32Count++)
    {
        PROC_PRINT(p,
                    VPSS_PROC_LEVEL4 "Task%d"
                    VPSS_PROC_LEVEL4 "InstanceCreat        : %lu"
                    VPSS_PROC_LEVEL4 "LogicStart           : %lu(+%lu)"
                    VPSS_PROC_LEVEL4 "LogicFinish          : %lu(+%lu)"
                    VPSS_PROC_LEVEL4 "InstanceFinish       : %lu(+%lu)"
                    ,
                    u32Count,
                    pstInstance->aulCreatTime[u32Count],
                    pstInstance->aulLogicStartTime[u32Count],
                    pstInstance->aulLogicStartTime[u32Count] - pstInstance->aulCreatTime[u32Count],
                    pstInstance->aulLogicEndTime[u32Count],
                    pstInstance->aulLogicEndTime[u32Count] - pstInstance->aulLogicStartTime[u32Count],
                    pstInstance->aulFinishTime[u32Count],
                    pstInstance->aulFinishTime[u32Count] - pstInstance->aulLogicEndTime[u32Count]
               );
    }
}
HI_S32 VPSS_CTRL_ProcPrint_V2(struct seq_file *p, HI_VOID *v)
{
    HI_CHAR asNodeState[VPSS_HAL_NODE_BUTT] = {0};
    VPSS_INSTANCE_S* pstInstance;
    VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_IN_ENTITY_S* pstInEntity;
    VPSS_IN_IMAGELIST_STATE_S stImgListState;
    HI_DRV_VPSS_CFG_S *pstUsrInstCfg;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    VPSS_DBG_S  *pstDbgCtrl;
    HI_S32 s32SrcModuleID;
    HI_U32 u32Count;
    HI_CHAR *pcSourceName;
    S_VPSS_REGS_TYPE *pstNodeReg;
    HI_BOOL bInputBusy      = HI_FALSE;
    HI_BOOL bInputNormal    = HI_FALSE;
    HI_BOOL bInputIdle      = HI_FALSE;
    HI_BOOL bOutputBlock    = HI_FALSE;
    HI_BOOL bOutputNormal   = HI_FALSE;
    HI_BOOL bOutputIdle     = HI_FALSE;


    pProcItem = p->private;
    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)((unsigned long)pProcItem->data));
    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)((unsigned long)pProcItem->data));
        return HI_FAILURE;
    }
    pstInEntity = &(pstInstance->stInEntity);
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);
    pstUsrInstCfg = &(pstInstance->stUsrInstCfg);
    pstDbgCtrl    = &(pstInstance->stDbgCtrl);
    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");
            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));
    }
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);
    }
    s32SrcModuleID = (pstInstance->hDst & 0x00ff0000) >>16;
    pcSourceName = ((s32SrcModuleID >= HI_ID_VFMW) && (s32SrcModuleID <= HI_ID_VENC)) ?
     g_pSrcModuleString[s32SrcModuleID - HI_ID_VFMW] : (s32SrcModuleID == 0 ? g_pSrcModuleString[0]: g_pSrcModuleString[9]);
    PROC_PRINT(p, VPSS_PROC_GAP);
    PROC_PRINT(p,   VPSS_PROC_LEVEL1
                                    "VPSS : %d"
                                    " |State : %s"
                                    " |Bypass : %s"
                                    " |QuickOut : %s"
                                    " |Source : %s"
                                    " |ProcLevel : %d"
                    ,
                    pstInstance->ID,
                    g_pInstState[pstInstance->enState],
                    VPSS_PROC_PRINT_YN_COL(pstInstance->stInEntity.stTransFbInfo.bNeedTrans),
                    g_pAlgModeString[pstInstance->bAlwaysFlushSrc],
                    pcSourceName,
                    pstDbgCtrl->stInstDbg.eProcPrintLevel
    );
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL3)
    {
        VPSS_CTRL_ProcPrintWBCList(p, pstInEntity);
    }
    PROC_PRINT(p, (VPSS_PROC_LEVEL2 "Input Info"));
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL1)
    {
        VPSS_CTRL_ProcPrintStreamInfo(p, pstStreamInfo);
    }
    if (pstInEntity->u32ScenceChgCnt > 1)
    {
        PROC_PRINT(p, VPSS_PROC_LEVEL4      "ScenceChange   : "COLOR_RED"%d"COLOR_NORMAL, pstInEntity->u32ScenceChgCnt);
    }
    else
    {
        PROC_PRINT(p, VPSS_PROC_LEVEL4      "ScenceChange   : %d", pstInEntity->u32ScenceChgCnt);
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL3)
    {
        PROC_PRINT(p,   VPSS_PROC_LEVEL3 "Instance Config Info"
                                VPSS_PROC_LEVEL4"QuickOut       : %s"
                                VPSS_PROC_LEVEL4"Secure         : %s"
                                VPSS_PROC_LEVEL4"ProgRevise     : %s"
                                VPSS_PROC_LEVEL4"ProgInfo       : %s"
                                VPSS_PROC_LEVEL4"SrcColorSpace  : %s"
                        ,
                        VPSS_PROC_PRINT_YN(pstUsrInstCfg->bAlwaysFlushSrc),
                        VPSS_PROC_PRINT_YN(pstUsrInstCfg->bSecure),
                        VPSS_PROC_PRINT_YN(pstUsrInstCfg->bProgRevise),
                        VPSS_PROC_PRINT_ENUM(pstUsrInstCfg->enProgInfo, HI_DRV_VPSS_PRODETECT_BUTT, g_pcProDetectString),
                        VPSS_PROC_PRINT_ENUM(pstUsrInstCfg->enSrcCS, HI_DRV_CS_BUTT, g_pcProColorSpaceString)
                   );
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL2)
    {
        PROC_PRINT(p,   VPSS_PROC_LEVEL3 "Instance RWZB Info"
                                        VPSS_PROC_LEVEL4"RWZB           : %s"
                                        VPSS_PROC_LEVEL4"Fidelity       : %d"
                                        ,
                                        VPSS_PROC_PRINT_YN_COL(pstInstance->stRwzbInfo.u32Rwzb>0),
                                        pstInstance->stRwzbInfo.u32Rwzb
                      );
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL2)
    {
        VPSS_CTRL_ProcPrintPortUserCfg(p, pstInstance);
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL3)
    {
        PROC_PRINT(p, VPSS_PROC_LEVEL2 "Statistic Info");
        PROC_PRINT(p, VPSS_PROC_LEVEL3 "TimeStamp");
        VPSS_CTRL_ProcPrintTimeStamp(p, pstInstance);
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL1)
    {
        if (pstInstance->u32LogicTimeoutCnt > 1)
        {
            PROC_PRINT(p, VPSS_PROC_LEVEL3 "Logic Timeout : " COLOR_RED"%d"COLOR_NORMAL, pstInstance->u32LogicTimeoutCnt);
        }
        else
        {
            PROC_PRINT(p, VPSS_PROC_LEVEL3 "Logic Timeout : %d", 0);
        }
    }

    if (pstInstance->u32BufSucCnt*100 > pstInstance->u32BufCnt*95)
    {
        bOutputIdle = HI_TRUE;
    }
    else if (pstInstance->u32BufSucCnt*100 > pstInstance->u32BufCnt*50)
    {
        bOutputNormal = HI_TRUE;
    }
    else
    {
        bOutputBlock = HI_TRUE;
    }

    if (stImgListState.u32GetUsrSuccess*100 < stImgListState.u32GetUsrTotal*50)
    {
        bInputIdle = HI_TRUE;
    }
    else if ((stImgListState.u32GetUsrSuccess*100 > stImgListState.u32GetUsrTotal*98)
            && (HI_FALSE == bOutputBlock))
    {
        bInputBusy = HI_TRUE;
    }
    else
    {
        bInputNormal = HI_TRUE;
    }

    PROC_PRINT(p, VPSS_PROC_LEVEL3 "Input");
    PROC_PRINT(p,
                    VPSS_PROC_LEVEL4 "ProcessFrame  (try/ok) : %d/%d\tHz"
                    VPSS_PROC_LEVEL4 "ProcessField  (try/ok) : %d/%d\tHz"
                    VPSS_PROC_LEVEL4 "InputFrame    (try/ok) : %d/%d\t%s"
                    VPSS_PROC_LEVEL4 "SrcList put/proc/rel   : %d/%d/%d"
                    ,
                    pstInstance->u32ImgRate, pstInstance->u32ImgSucRate,
                    pstInstance->u32CheckRate, pstInstance->u32CheckSucRate,
                    stImgListState.u32GetUsrTotal, stImgListState.u32GetUsrSuccess,
                    ((bInputNormal) ? "Normal" : ((bInputIdle) ? COLOR_GREEN"Idle"COLOR_NORMAL : COLOR_RED"Busy"COLOR_NORMAL)),
                    pstInEntity->stSrc.u32PutSrcCount,
                    pstInEntity->stSrc.u32CompleteSrcCount,
                    pstInEntity->stSrc.u32ReleaseSrcCount
               );
    PROC_PRINT(p, VPSS_PROC_LEVEL3 "Output");
    PROC_PRINT(p,
                    VPSS_PROC_LEVEL4 "GetOutbuf (try/ok)   : %d/%d\tHz"
                    VPSS_PROC_LEVEL4 "GetOutbuf (try/ok)   : %d/%d\t%s"
                    ,
                    pstInstance->u32BufRate, pstInstance->u32BufSucRate,
                    pstInstance->u32BufCnt, pstInstance->u32BufSucCnt,
                    ((bOutputNormal) ? "Normal" : ((bOutputIdle) ? COLOR_GREEN"Idle"COLOR_NORMAL : COLOR_RED"Block"COLOR_NORMAL))
              );
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL3)
    {
        for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
        {
            pstPort = &(pstInstance->stPort[u32Count]);
            PROC_PRINT(p,  VPSS_PROC_LEVEL3 "Port%d FBState ", u32Count);
            if (pstPort->bEnble)
            {
                PROC_PRINT(p,
                        VPSS_PROC_LEVEL4 "BufManager           : %s"
                        VPSS_PROC_LEVEL4 "BufNumber            : %d(L) + %d(R)"
                        VPSS_PROC_LEVEL4 "BufFul               : %d"
                        VPSS_PROC_LEVEL4 "BufEmpty             : %d"
                        VPSS_PROC_LEVEL4 "Acquire              : %d\tHz"
                        VPSS_PROC_LEVEL4 "Acquire(try/ok)      : %d/%d"
                        VPSS_PROC_LEVEL4 "Release(try/ok)      : %d/%d"
                        ,
                        g_pBufTypeString[pstPortPrc[u32Count]->stBufListCfg.eBufType],
                        pstPortPrc[u32Count]->stBufListCfg.u32BufNumber,
                        pstPortPrc[u32Count]->stFbPrc.u32ExtListNumb,
                        pstPortPrc[u32Count]->stFbPrc.u32FulListNumb,
                        pstPortPrc[u32Count]->stFbPrc.u32EmptyListNumb,
                        pstPortPrc[u32Count]->stFbPrc.u32GetHZ,
                        pstPortPrc[u32Count]->stFbPrc.u32GetTotal, pstPortPrc[u32Count]->stFbPrc.u32GetSuccess,
                        pstPortPrc[u32Count]->stFbPrc.u32RelTotal, pstPortPrc[u32Count]->stFbPrc.u32RelSuccess
                );
            }
            else
            {
                PROC_PRINT(p, VPSS_PROC_LEVEL4"Port is disable.");
            }
        }
    }
    if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL1)
    {
        PROC_PRINT(p,  VPSS_PROC_LEVEL2 "Hal Info");
        PROC_PRINT(p,  VPSS_PROC_LEVEL3 "Node Enable List");
        for(u32Count = 0; u32Count < VPSS_HAL_NODE_BUTT; u32Count++)
        {
            asNodeState[u32Count] = (pstInstance->abNodeVaild[u32Count] == HI_TRUE) ? '1':'0';
            if (pstInstance->abNodeVaild[u32Count])
            {
                PROC_PRINT(p,  VPSS_PROC_LEVEL4 "%s(%d)",
                                VPSS_PROC_PRINT_ENUM(u32Count, VPSS_HAL_NODE_BUTT, g_pcHalNodeString),
                                u32Count
                           );
                if (pstDbgCtrl->stInstDbg.eProcPrintLevel >= PROC_PRINT_LEVEL3)
                {
                    pstNodeReg = HI_NULL;
                    if ((HI_SUCCESS == VPSS_HAL_GetNodeAddr(VPSS_IP_0, u32Count, (HI_U8**)&pstNodeReg))
                            && (HI_NULL != pstNodeReg))
                    {
                        VPSS_CTRL_ProcPrintHalInfo(p, pstNodeReg);
                    }
                    else
                    {
                        PROC_PRINT(p, VPSS_PROC_LEVEL5 "Get node info fail.");
                    }
                }
            }
        }
    }
    PROC_PRINT(p, "\n"VPSS_PROC_GAP"\n");
READFREE:
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
    return HI_SUCCESS;
}
#endif
HI_S32 VPSS_CTRL_ProcPrint_V1(struct seq_file *p, HI_VOID *v)
{
    VPSS_INSTANCE_S* pstInstance;
    VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_IN_ENTITY_S* pstInEntity;
    VPSS_IN_IMAGELIST_STATE_S stImgListState;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
	HI_U8 *pProcPortFmt[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    HI_S32 s32SrcModuleID;
    HI_U32 u32Count;
    pProcItem = p->private;

    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)((unsigned long)(pProcItem->data)));

    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(unsigned long)((VPSS_HANDLE*)pProcItem->data));
        return HI_FAILURE;
    }
    pstInEntity = &(pstInstance->stInEntity);
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);

    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");

            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));

    }

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);

		if ((pstPortPrc[u32Count]->eFormat >= HI_DRV_PIX_FMT_NV12_CMP) &&
		  ((pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12_CMP) <= 5))
		{
			pProcPortFmt[u32Count] = g_pPixString[pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12_CMP + 7] ;
		}
		else if ((pstPortPrc[u32Count]->eFormat >= HI_DRV_PIX_FMT_NV12) &&
		            ((pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6 ))
		{
			pProcPortFmt[u32Count] = g_pPixString[pstPortPrc[u32Count]->eFormat - HI_DRV_PIX_FMT_NV12];
		}
		else
		{
			pProcPortFmt[u32Count] = g_pPixString[13];
		}
    }

    s32SrcModuleID = (pstInstance->hDst & 0x00ff0000) >>16;

#if DEF_VPSS_STATIC

	PROC_PRINT(p,
	"-----Logic Process Jiffies----|" "Node 0 Cycle     Node 1 Cycle\n"
	);

	for (u32Count = 0; u32Count < 15; u32Count++)
	{
		PROC_PRINT(p,
		"%d                    |%d              %d\n",
		g_u32LogicTime[u32Count],
		g_u32LogicCycle[0][u32Count],
		g_u32LogicCycle[1][u32Count]);
	}
#else
    PROC_PRINT(p,
        "--------VPSS%04x---------------|"   "------------------------PortInfo------------------------|\n"
        "ID               :0x%-8x   |"       "ID               :0x%-8x  |0x%-8x  |0x%-8x  |\n"
        "State            :%-10s   |"        "State            :%-3s         |%-3s         |%-3s         |\n"
        "Priority         :%-10d   |"        "PixelFormat      :%-12s|%-12s|%-12s|\n"
        "QuickOutPut      :%-10s   |"        "Resolution       :%4d*%-4d   |%4d*%-4d   |%4d*%-4d   |\n"
        "SourceID         :%-6s(%02x)   |"   "ColorSpace       :%-12s|%-12s|%-12s|\n"
        "Version          :%-10s   |"        "DispPixelAR(W/H) :%2d/%-2d       |%2d/%-2d       |%2d/%-2d       |\n"
        "                               |"   "Aspect Mode      :%-12s|%-12s|%-12s|\n"
        "                               |"   "Support3DStream  :%-12s|%-12s|%-12s|\n"
        "                               |"   "MaxFrameRate     :%-5d       |%-5d       |%-5d       |\n"
        "-------- Algorithm-------------|"   "*LowDelay        :%-12s|%-12s|%-12s|\n"
        "P/I Setting   :%-10s      |"        "HorizonFlip      :%-12s|%-12s|%-12s|\n"
        "Deinterlace   :%-10s      |"        "VerticalFlip     :%-12s|%-12s|%-12s|\n"
        "Sharpness     :%-10s      |"        "Rotation         :%-12s|%-12s|%-12s|\n"
#ifdef ZME_2L_TEST
		"*ProgRevise   :%-10s      |"		 "*ZME1LResolution :%4d*%-4d   |%4d*%-4d   |%4d*%-4d   |\n"
#else
        "*ProgRevise   :%-10s      |"        "                              |            |            |\n"
#endif
        "                               |"   "OutBitWidth      :%-12s|%-12s|%-12s|\n"
        "--------Detect Info------------|"   "                              |            |            |\n"
        "TopFirst(Src):%6s(%-6s)   |"        "                              |            |            |\n"
        "InRate(Src)  :%6d(%-6d)   |"        "                              |            |            |\n"
		"*Trans       :%8s         |"        "                              |            |            |\n"
        "Progressive/Interlace(Src):%-1s(%-1s)|"        "                              |            |            |\n",
        /* attribute */
        pstInstance->ID,
        pstInstance->ID,
                                                pstPortPrc[0]->s32PortId,
                                                pstPortPrc[1]->s32PortId,
                                                pstPortPrc[2]->s32PortId,
        g_pInstState[pstInstance->enState],
                                                g_pAlgModeString[pstInstance->stPort[0].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[1].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[2].bEnble],

        0,
												pProcPortFmt[0],
												pProcPortFmt[1],
												pProcPortFmt[2],

        g_pAlgModeString[pstInstance->bAlwaysFlushSrc],
                                                pstPortPrc[0]->s32OutputWidth,
                                                pstPortPrc[0]->s32OutputHeight,
                                                pstPortPrc[1]->s32OutputWidth,
                                                pstPortPrc[1]->s32OutputHeight,
                                                pstPortPrc[2]->s32OutputWidth,
                                                pstPortPrc[2]->s32OutputHeight,

        (s32SrcModuleID >= HI_ID_VFMW && s32SrcModuleID <= HI_ID_VENC)?
         g_pSrcModuleString[s32SrcModuleID - HI_ID_VFMW]:
        (s32SrcModuleID == 0?g_pSrcModuleString[0]:
         g_pSrcModuleString[9]),
         (pstInstance->hDst & 0x000000ff),
                                                g_pCscString[pstPortPrc[0]->eDstCS],
                                                g_pCscString[pstPortPrc[1]->eDstCS],
                                                g_pCscString[pstPortPrc[2]->eDstCS],
         DEF_SDK_VERSIO_LOG,
                                                pstPortPrc[0]->stDispPixAR.u32ARw,
                                                pstPortPrc[0]->stDispPixAR.u32ARh,
                                                pstPortPrc[1]->stDispPixAR.u32ARw,
                                                pstPortPrc[1]->stDispPixAR.u32ARh,
												pstPortPrc[2]->stDispPixAR.u32ARw,
                                                pstPortPrc[2]->stDispPixAR.u32ARh,

                                                g_pAspString[pstPortPrc[0]->eAspMode],
                                                g_pAspString[pstPortPrc[1]->eAspMode],
                                                g_pAspString[pstPortPrc[2]->eAspMode],

                                                g_pAlgModeString[pstPortPrc[0]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[1]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[2]->b3Dsupport],

                                                pstPortPrc[0]->u32MaxFrameRate,
                                                pstPortPrc[1]->u32MaxFrameRate,
                                                pstPortPrc[2]->u32MaxFrameRate,
        /*alg config*/                             g_pAlgModeString[pstPortPrc[0]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[1]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[2]->bTunnelEnable],

        g_pProgDetectString[pstInEntity->enProgInfo],
                                                g_pAlgModeString[pstPortPrc[0]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bHoriFlip],
        g_pDeiString[pstInstance->stProcCtrl.eDEI],
                                                g_pAlgModeString[pstPortPrc[0]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bVertFlip],
        g_pAlgModeString[pstInstance->stProcCtrl.eSharpness],
                                                g_pRotationString[pstPortPrc[0]->enRotation],
                                                g_pRotationString[pstPortPrc[1]->enRotation],
                                                g_pRotationString[pstPortPrc[2]->enRotation],
#ifdef ZME_2L_TEST
		g_pAlgModeString[pstInEntity->bProgRevise],
												pstInstance->stPort[0].u32ZME1LWidth,
												pstInstance->stPort[0].u32ZME1LHeight,
												pstInstance->stPort[1].u32ZME1LWidth,
												pstInstance->stPort[1].u32ZME1LHeight,
												pstInstance->stPort[2].u32ZME1LWidth,
												pstInstance->stPort[2].u32ZME1LHeight,
#else
        g_pAlgModeString[pstInEntity->bProgRevise],
#endif
		(pstInstance->stPort[0].enOutBitWidth < HI_DRV_PIXEL_BITWIDTH_BUTT) ?
						(HI_CHAR*)g_pOutBitWidthString[pstInstance->stPort[0].enOutBitWidth] : "UNKONWN",
		(pstInstance->stPort[1].enOutBitWidth < HI_DRV_PIXEL_BITWIDTH_BUTT) ?
						(HI_CHAR*)g_pOutBitWidthString[pstInstance->stPort[1].enOutBitWidth] : "UNKONWN",
		(pstInstance->stPort[2].enOutBitWidth < HI_DRV_PIXEL_BITWIDTH_BUTT) ?
						(HI_CHAR*)g_pOutBitWidthString[pstInstance->stPort[2].enOutBitWidth] : "UNKONWN",

        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        ((pstStreamInfo->u32RealTopFirst == 0)?"Bottom":"Top"):"NA",
        (pstStreamInfo->u32StreamTopFirst == 0)?"Bottom":"Top",

        pstStreamInfo->u32InRate*1000,pstStreamInfo->u32StreamInRate,
		(pstInstance->stInEntity.stTransFbInfo.bNeedTrans == HI_TRUE)? "ON":"OFF",


        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        "I":"P",
        (pstStreamInfo->u32StreamTopFirst == 0)?"I":"P"
        );
    #endif
    #if 1
    PROC_PRINT(p,
    "-----SourceFrameList Info------|"  "--------------------OutFrameList Info-------------------|\n"
    "      (source to vpss)         |"  "                     (vpss to sink)                     |\n"
    "*Mutual Mode  :%-11s     |"        "BufManager       :%-10s  |%-10s  |%-10s  |\n"
    "                               |"  "BufNumber        :%-2d+%-2d       |%-2d+%-2d       |%-2d+%-2d       |\n"
    "GetSrcImgHZ(Try/OK)  :%3d/%-3d  |" "BufFul           :%-2d          |%-2d          |%-2d          |\n"
    "GetOutBufHZ(Try/OK)  :%3d/%-3d  |" "BufEmpty         :%-2d          |%-2d          |%-2d          |\n"
    "ProcessHZ(Try/OK)    :%3d/%-3d  |" "AcquireHZ        :%-10d  |%-10d  |%-10d  |\n"
    "Acquire(Try/OK):               |"  "Acquire(Try/OK):              |            |            |\n"
    " %10d/%-10d         |"             " %10d/%-10d%10d/%-10d%10d/%-10d\n"
    "Release(Try/OK):               |"  "Release(Try/OK):              |            |            |\n"
    " %10d/%-10d         |"             " %10d/%-10d%10d/%-10d%10d/%-10d\n",

    g_pSrcMutualString[pstInstance->eSrcImgMode],
                                        g_pBufTypeString[pstPortPrc[0]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[1]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[2]->stBufListCfg.eBufType],

    pstPortPrc[0]->stBufListCfg.u32BufNumber,
    pstPortPrc[0]->stFbPrc.u32ExtListNumb,
    pstPortPrc[1]->stBufListCfg.u32BufNumber,
    pstPortPrc[1]->stFbPrc.u32ExtListNumb,
    pstPortPrc[2]->stBufListCfg.u32BufNumber,
    pstPortPrc[2]->stFbPrc.u32ExtListNumb,


    pstInstance->u32ImgRate,
    pstInstance->u32ImgSucRate,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb,
    pstInstance->u32BufRate,
    pstInstance->u32BufSucRate,
                                        pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stFbPrc.u32EmptyListNumb,
    pstInstance->u32CheckRate,
    pstInstance->u32CheckSucRate,
                                        pstPortPrc[0]->stFbPrc.u32GetHZ,
                                        pstPortPrc[1]->stFbPrc.u32GetHZ,
                                        pstPortPrc[2]->stFbPrc.u32GetHZ,


    stImgListState.u32GetUsrTotal,      stImgListState.u32GetUsrSuccess,
    pstPortPrc[0]->stFbPrc.u32GetTotal, pstPortPrc[0]->stFbPrc.u32GetSuccess,
    pstPortPrc[1]->stFbPrc.u32GetTotal, pstPortPrc[1]->stFbPrc.u32GetSuccess,
    pstPortPrc[2]->stFbPrc.u32GetTotal, pstPortPrc[2]->stFbPrc.u32GetSuccess,
    stImgListState.u32RelUsrTotal,      stImgListState.u32RelUsrSuccess,
    pstPortPrc[0]->stFbPrc.u32RelTotal, pstPortPrc[0]->stFbPrc.u32RelSuccess,
    pstPortPrc[1]->stFbPrc.u32RelTotal, pstPortPrc[1]->stFbPrc.u32RelSuccess,
    pstPortPrc[2]->stFbPrc.u32RelTotal, pstPortPrc[2]->stFbPrc.u32RelSuccess
    );


    #endif




READFREE:
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
    return HI_SUCCESS;

}

HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
#ifdef VPSS_SUPPORT_PROC_V2
    VPSS_INSTANCE_S* pstInstance;
    DRV_PROC_ITEM_S *pProcItem;
    pProcItem = p->private;
    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)((unsigned long)pProcItem->data));
    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)((unsigned long)pProcItem->data));
        return HI_FAILURE;
    }
    if (PROC_PRINT_LEVEL_ORI == pstInstance->stDbgCtrl.stInstDbg.eProcPrintLevel)
    {
        return VPSS_CTRL_ProcPrint_V1(p, v);
    }
    else
    {
        return VPSS_CTRL_ProcPrint_V2(p, v);
    }
#else
    return VPSS_CTRL_ProcPrint_V1(p, v);
#endif
#else
    return HI_SUCCESS;
#endif
}
#elif defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
HI_S32 VPSS_CTRL_ProcRead(struct seq_file *p, HI_VOID *v)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
	HI_CHAR asNodeState[VPSS_HAL_TASK_NODE_BUTT] = {0};
    VPSS_INSTANCE_S* pstInstance;
	VPSS_IN_STREAM_INFO_S* pstStreamInfo;
    VPSS_IN_ENTITY_S* pstInEntity;
    VPSS_IN_IMAGELIST_STATE_S stImgListState;
    DRV_PROC_ITEM_S *pProcItem;
    VPSS_PORT_PRC_S *pstPortPrc[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    VPSS_PORT_S *pstPort;
    HI_S32 s32SrcModuleID;
    HI_U32 u32Count;
    pProcItem = p->private;

    pstInstance = VPSS_CTRL_GetInstance((VPSS_HANDLE)pProcItem->data));

    if(!pstInstance)
    {
        VPSS_FATAL("Can't get instance %x proc!\n",(VPSS_HANDLE)pProcItem->data);
        return HI_FAILURE;
    }

	pstInEntity = &(pstInstance->stInEntity);
    pstStreamInfo = &(pstInstance->stInEntity.stStreamInfo);

    VPSS_INST_GetSrcListState(pstInstance, &stImgListState);

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPortPrc[u32Count] = VPSS_VMALLOC(sizeof(VPSS_PORT_PRC_S));
        if (pstPortPrc[u32Count] == HI_NULL)
        {
            VPSS_FATAL("Vmalloc Proc space Failed.\n");

            goto READFREE;
        }
        memset(pstPortPrc[u32Count],0,sizeof(VPSS_PORT_PRC_S));

    }

    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        pstPort = &(pstInstance->stPort[u32Count]);
        VPSS_INST_GetPortPrc(pstInstance,pstPort->s32PortId,pstPortPrc[u32Count]);
    }

    s32SrcModuleID = (pstInstance->hDst & 0x00ff0000) >>16;

	for(u32Count = 0; u32Count < VPSS_HAL_TASK_NODE_BUTT; u32Count++)
	{
		asNodeState[u32Count] = (pstInstance->abNodeVaild[u32Count] == HI_TRUE) ? '1':'0';
	}

    PROC_PRINT(p,
        "--------VPSS%04x---------------|"   "------------------------PortInfo--------------------------|\n"
        "ID               :0x%-8d   |"      "ID               :0x%-8d  |0x%-8d  |0x%-8d    |\n"
        "State/Pause      :%-10s   |"         "State            :%-3s         |%-3s         |%-3s           |\n"
        "SourceFormat     :%-10s   |"        "PixelFormat      :%-12s|%-12s|%-12s  |\n"
        "QuickOutPut      :%-10s   |"        "OutResolution    :%4d*%-4d   |%4d*%-4d   |%4d*%-4d     |\n"
        "SourceID         :%-6s(%02x)   |"   "ColorSpace       :%-12s|%-12s|%-12s  |\n"
        "Version          :%-10s   |"        "DispPixelAR(W/H) :%2d/%-2d       |%2d/%-2d       |%2d/%-2d         |\n"
        "                               |"   "Aspect Mode      :%-12s|%-12s|%-12s  |\n"
        "                               |"   "Support3DStream  :%-12s|%-12s|%-12s  |\n"
        "                               |"   "MaxFrameRate     :%-5d       |%-5d       |%-5d         |\n"
        "-------- Algorithm-------------|"   "*LowDelay        :%-12s|%-12s|%-12s  |\n"
        "P/I Setting   :%-10s      |"        "HorizonFlip      :%-12s|%-12s|%-12s  |\n"
        "Deinterlace   :%-10s      |"        "VerticalFlip     :%-12s|%-12s|%-12s  |\n"
        "Sharpness     :%-10s      |"        "Rotation         :%-12s|%-12s|%-12s  |\n"
		"*Trans        :%-10s      |"        "Incrop           :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
        "*ProgRevise   :%-10s      |"        "Usercrop         :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
        "                               |"   "VideoRect        :%d,%-d,%d,%-d|%d,%-d,%d,%-d|%d,%-d,%d,%-d  |\n"
        "--------Detect Info------------|"   "InResolution     :%4d*%-4d   |%4d*%-4d   |%4d*%-4d     |\n"
        "TopFirst(Src):%6s(%-6s)   |"        "UsercropEn       :%-12s|%-12s|%-12s  |\n"
        "InRate(Src)  :%6d(%-6d)   |"        "										  |\n"
        "Progressive/Interlace(Src):%-1s(%-1s)|""KeyFrameEn       :%-12s|%-12s|%-12s  |\n",

        /* attribute */
        pstInstance->ID,
        pstInstance->ID,
                                                pstPortPrc[0]->s32PortId,
                                                pstPortPrc[1]->s32PortId,
                                                pstPortPrc[2]->s32PortId,
        g_pInstState[pstInstance->enState],
												g_pAlgModeString[pstInstance->stPort[0].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[1].bEnble],
                                                g_pAlgModeString[pstInstance->stPort[2].bEnble],

        ((pstStreamInfo->ePixFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
        g_pPixString[pstStreamInfo->ePixFormat - HI_DRV_PIX_FMT_NV12]:
        g_pPixString[7],
                                                ((pstPortPrc[0]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[0]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],
                                                ((pstPortPrc[1]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[1]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],
                                                ((pstPortPrc[2]->eFormat - HI_DRV_PIX_FMT_NV12) <= 6)?
                                                g_pPixString[pstPortPrc[2]->eFormat - HI_DRV_PIX_FMT_NV12]:
                                                g_pPixString[7],

        g_pAlgModeString[pstInstance->bAlwaysFlushSrc],
                                                (pstPortPrc[0]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[0]->s32OutputWidth,
                                                (pstPortPrc[0]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[0]->s32OutputHeight,
                                                (pstPortPrc[1]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[1]->s32OutputWidth,
                                                (pstPortPrc[1]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[1]->s32OutputHeight,
                                                (pstPortPrc[2]->s32OutputWidth == 0) ? pstStreamInfo->u32StreamW:pstPortPrc[2]->s32OutputWidth,
                                                (pstPortPrc[2]->s32OutputHeight == 0) ? pstStreamInfo->u32StreamH:pstPortPrc[2]->s32OutputHeight,


        (s32SrcModuleID >= HI_ID_VFMW && s32SrcModuleID <= HI_ID_VENC)?
         g_pSrcModuleString[s32SrcModuleID - HI_ID_VFMW]:
        (s32SrcModuleID == 0?g_pSrcModuleString[0]:
         g_pSrcModuleString[9]),
         (pstInstance->hDst & 0x000000ff),
                                                g_pCscString[pstPortPrc[0]->eDstCS],
                                                g_pCscString[pstPortPrc[1]->eDstCS],
                                                g_pCscString[pstPortPrc[2]->eDstCS],
         DEF_SDK_VERSIO_LOG,
                                                pstPortPrc[0]->stDispPixAR.u32ARw,
                                                pstPortPrc[0]->stDispPixAR.u32ARh,
                                                pstPortPrc[1]->stDispPixAR.u32ARw,
                                                pstPortPrc[1]->stDispPixAR.u32ARh,
                                                pstPortPrc[2]->stDispPixAR.u32ARw,
                                                pstPortPrc[2]->stDispPixAR.u32ARh,

                                                g_pAspString[pstPortPrc[0]->eAspMode],
                                                g_pAspString[pstPortPrc[1]->eAspMode],
                                                g_pAspString[pstPortPrc[2]->eAspMode],

                                                g_pAlgModeString[pstPortPrc[0]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[1]->b3Dsupport],
                                                g_pAlgModeString[pstPortPrc[2]->b3Dsupport],
//         asNodeState,
                                                pstPortPrc[0]->u32MaxFrameRate,
                                                pstPortPrc[1]->u32MaxFrameRate,
                                                pstPortPrc[2]->u32MaxFrameRate,
        /*alg config*/                             g_pAlgModeString[pstPortPrc[0]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[1]->bTunnelEnable],
                                                g_pAlgModeString[pstPortPrc[2]->bTunnelEnable],

        g_pProgDetectString[pstInEntity->enProgInfo],
                                                g_pAlgModeString[pstPortPrc[0]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bHoriFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bHoriFlip],
        g_pDeiString[pstInstance->stProcCtrl.eDEI],
                                                g_pAlgModeString[pstPortPrc[0]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[1]->bVertFlip],
                                                g_pAlgModeString[pstPortPrc[2]->bVertFlip],
        g_pAlgModeString[pstInstance->stProcCtrl.eSharpness],
                                                g_pRotationString[pstPortPrc[0]->enRotation],
                                                g_pRotationString[pstPortPrc[1]->enRotation],
                                                g_pRotationString[pstPortPrc[2]->enRotation],
		(pstInstance->stInEntity.stTransFbInfo.bNeedTrans == HI_TRUE)? "ON":"OFF",

                                                pstInstance->stPort[0].stInRect.s32X,pstInstance->stPort[0].stInRect.s32Y,
                                                pstInstance->stPort[0].stInRect.s32Width,pstInstance->stPort[0].stInRect.s32Height,
                                                pstInstance->stPort[1].stInRect.s32X,pstInstance->stPort[1].stInRect.s32Y,
                                                pstInstance->stPort[1].stInRect.s32Width,pstInstance->stPort[1].stInRect.s32Height,
                                                pstInstance->stPort[2].stInRect.s32X,pstInstance->stPort[2].stInRect.s32Y,
                                                pstInstance->stPort[2].stInRect.s32Width,pstInstance->stPort[2].stInRect.s32Height,

		 g_pAlgModeString[pstInEntity->bProgRevise],
                                                pstInstance->stPort[0].stCropRect.u32LeftOffset,pstInstance->stPort[0].stCropRect.u32TopOffset,
                                                pstInstance->stPort[0].stCropRect.u32RightOffset,pstInstance->stPort[0].stCropRect.u32BottomOffset,
                                                pstInstance->stPort[1].stCropRect.u32LeftOffset,pstInstance->stPort[1].stCropRect.u32TopOffset,
                                                pstInstance->stPort[1].stCropRect.u32RightOffset,pstInstance->stPort[1].stCropRect.u32BottomOffset,
                                                pstInstance->stPort[2].stCropRect.u32LeftOffset,pstInstance->stPort[2].stCropRect.u32TopOffset,
                                                pstInstance->stPort[2].stCropRect.u32RightOffset,pstInstance->stPort[2].stCropRect.u32BottomOffset,
                                                pstInstance->stPort[0].stVideoRect.s32X,pstInstance->stPort[0].stVideoRect.s32Y,
                                                pstInstance->stPort[0].stVideoRect.s32Width,pstInstance->stPort[0].stVideoRect.s32Height,
                                                pstInstance->stPort[1].stVideoRect.s32X,pstInstance->stPort[1].stVideoRect.s32Y,
                                                pstInstance->stPort[1].stVideoRect.s32Width,pstInstance->stPort[1].stVideoRect.s32Height,
                                                pstInstance->stPort[2].stVideoRect.s32X,pstInstance->stPort[2].stVideoRect.s32Y,
                                                pstInstance->stPort[2].stVideoRect.s32Width,pstInstance->stPort[2].stVideoRect.s32Height,

                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,
                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,
                                                pstStreamInfo->u32StreamW,pstStreamInfo->u32StreamH,

        (pstStreamInfo->u32RealTopFirst == 0 || pstStreamInfo->u32RealTopFirst == 1)?
        ((pstStreamInfo->u32RealTopFirst == 0)?"Bottom":"Top"):"NA",
        (pstStreamInfo->u32StreamTopFirst == 0)?"Bottom":"Top",

                                                g_pAlgModeString[pstInstance->stPort[0].bUseCropRect],
                                                g_pAlgModeString[pstInstance->stPort[1].bUseCropRect],
                                                g_pAlgModeString[pstInstance->stPort[2].bUseCropRect],
        pstStreamInfo->u32InRate*1000,pstStreamInfo->u32StreamInRate,

        (pstStreamInfo->u32StreamProg == 0)?
        "I":"P",
        (pstStreamInfo->u32StreamTopFirst == 0)?"I":"P" ,
                                                g_pAlgModeString[pstInstance->stPort[0].bOnlyKeyFrame],
                                                g_pAlgModeString[pstInstance->stPort[1].bOnlyKeyFrame],
                                                g_pAlgModeString[pstInstance->stPort[2].bOnlyKeyFrame]
        );


    PROC_PRINT(p,
    "-----SourceFrameList Info------|"  "--------------------OutFrameList Info---------------------|\n"
    "      (source to vpss)         |"  "                     (vpss to sink)                       |\n"
    "*Mutual Mode  :%-11s     |"        "BufManager       :%-10s  |%-10s   |%-10s   |\n"
    "SrcChangeCnt  :%-11d     |"  "BufNumber        :%-2d+%-2d       |%-2d+%-2d        |%-2d+%-2d        |\n"
    "GetSrcImgHZ(Try/OK)  :%3d/%-3d  |" "BufFul           :%-2d          |%-2d           |%-2d           |\n"
    "GetOutBufHZ(Try/OK)  :%3d/%-3d  |" "BufEmpty         :%-2d          |%-2d           |%-2d           |\n"
    "ProcessHZ(Try/OK)    :%3d/%-3d  |" "AcquireHZ        :%-10d  |%-10d   |%-10d   |\n"
    "SrcList(PUT/COMPLETE/RELEASE): |"  "Acquire(Try/OK):              |             |             |\n"
    " %6d/%-6d/%-6d          |"         "              %6d/%-6d   |%6d/%-6d|%6d/%-6d|\n"
    "WbcList(COMPLETE[L/R]):        |"  "Release(Try/OK):              |             |             |\n"
    " %6d/%-6d                 |"       "              %6d/%-6d   |%6d/%-6d|%6d/%-6d|\n"
    "StWbcList(COMPLETE[L/R]):      |"  "OutRate          :%-10d  |%-10d   |%-10d   |\n"
    " %6d/%-6d                 |"       "Releasing        :%-10d  |%-10d   |%-10d   |\n"
    "StDieList(COMPLETE[L/R]):      |"  "Waiting          :%-10d  |%-10d   |%-10d   |\n"
    " %6d/%-6d                 |"       "Working          :%-10d  |%-10d   |%-10d   |\n"
    "StNrlList(COMPLETE[L/R]):      |\n"
    " %6d/%-6d                 |\n",

    g_pSrcMutualString[pstInstance->eSrcImgMode],
                                        g_pBufTypeString[pstPortPrc[0]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[1]->stBufListCfg.eBufType],
                                        g_pBufTypeString[pstPortPrc[2]->stBufListCfg.eBufType],

    pstInstance->u32ScenceChgCnt,
    pstPortPrc[0]->stBufListCfg.u32BufNumber,
    pstPortPrc[0]->stFbPrc.u32ExtListNumb,
    pstPortPrc[1]->stBufListCfg.u32BufNumber,
    pstPortPrc[1]->stFbPrc.u32ExtListNumb,
    pstPortPrc[2]->stBufListCfg.u32BufNumber,
    pstPortPrc[2]->stFbPrc.u32ExtListNumb,
    pstInstance->u32ImgRate,
    pstInstance->u32ImgSucRate,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb,
    pstInstance->u32BufRate,
    pstInstance->u32BufSucRate,
                                        pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stFbPrc.u32EmptyListNumb,
    pstInstance->u32CheckRate,
    pstInstance->u32CheckSucRate,
                                        pstPortPrc[0]->stFbPrc.u32GetHZ,
                                        pstPortPrc[1]->stFbPrc.u32GetHZ,
                                        pstPortPrc[2]->stFbPrc.u32GetHZ,

    pstInEntity->pstSrc->u32PutSrcCount,pstInEntity->pstSrc->u32CompleteSrcCount,pstInEntity->pstSrc->u32ReleaseSrcCount,
                                        pstPortPrc[0]->stFbPrc.u32GetTotal, pstPortPrc[0]->stFbPrc.u32GetSuccess,
                                        pstPortPrc[1]->stFbPrc.u32GetTotal, pstPortPrc[1]->stFbPrc.u32GetSuccess,
                                        pstPortPrc[2]->stFbPrc.u32GetTotal, pstPortPrc[2]->stFbPrc.u32GetSuccess,

    pstInEntity->pstWbcInfo[0]->u32CompleteCount,pstInEntity->pstWbcInfo[1]->u32CompleteCount,
                                        pstPortPrc[0]->stFbPrc.u32RelTotal, pstPortPrc[0]->stFbPrc.u32RelSuccess,
                                        pstPortPrc[1]->stFbPrc.u32RelTotal, pstPortPrc[1]->stFbPrc.u32RelSuccess,
                                        pstPortPrc[2]->stFbPrc.u32RelTotal, pstPortPrc[2]->stFbPrc.u32RelSuccess,

                                        pstPortPrc[0]->stFbPrc.u32OutRate,
                                        pstPortPrc[1]->stFbPrc.u32OutRate,
                                        pstPortPrc[2]->stFbPrc.u32OutRate,
    pstInEntity->pstSttWbc[0]->u32Cnt,pstInEntity->pstSttWbc[1]->u32Cnt,
                                        pstPortPrc[0]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[1]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[2]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb - pstPortPrc[0]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb - pstPortPrc[1]->stFbPrc.u32WaitSinkRlsNumb,
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb - pstPortPrc[2]->stFbPrc.u32WaitSinkRlsNumb,
    pstInEntity->pstDieStInfo[0]->u32Cnt,pstInEntity->pstDieStInfo[1]->u32Cnt,
                                        pstPortPrc[0]->stBufListCfg.u32BufNumber + pstPortPrc[0]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[0]->stFbPrc.u32FulListNumb - pstPortPrc[0]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[1]->stBufListCfg.u32BufNumber + pstPortPrc[1]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[1]->stFbPrc.u32FulListNumb - pstPortPrc[1]->stFbPrc.u32EmptyListNumb,
                                        pstPortPrc[2]->stBufListCfg.u32BufNumber + pstPortPrc[2]->stFbPrc.u32ExtListNumb -
                                        pstPortPrc[2]->stFbPrc.u32FulListNumb - pstPortPrc[2]->stFbPrc.u32EmptyListNumb,

    pstInEntity->pstNrMadInfo[0]->u32Cnt,pstInEntity->pstNrMadInfo[1]->u32Cnt
    );





READFREE:
    for(u32Count = 0; u32Count < DEF_HI_DRV_VPSS_PORT_MAX_NUMBER; u32Count++)
    {
        if (pstPortPrc[u32Count] != HI_NULL)
            VPSS_VFREE(pstPortPrc[u32Count]);
    }
#endif
    return HI_SUCCESS;

}

#endif


HI_S32 VPSS_CTRL_ProcWrite(struct file * file,
    const char __user * buf, size_t count, loff_t *ppos)
{
#ifndef HI_ADVCA_FUNCTION_RELEASE
    struct seq_file   *s = file->private_data;
    DRV_PROC_ITEM_S  *pProcItem = s->private;
    VPSS_HANDLE hVpss;
    HI_CHAR  chCmd[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg1[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg2[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg3[DEF_FILE_NAMELENGTH] = {0};
    HI_CHAR  chArg4[DEF_FILE_NAMELENGTH] = {0};
    HI_S32   s32Ret;
    VPSS_INSTANCE_S *pstInstance;
    VPSS_DBG_INST_S *pstInstDbg;

    hVpss = (VPSS_HANDLE)((unsigned long)pProcItem->data);
    pstInstance = VPSS_CTRL_GetInstance(hVpss);
    if (pstInstance == HI_NULL)
    {
        VPSS_FATAL("Can't Get Debug Instance.\n");
        return count;
    }

    if(count >= DEF_FILE_NAMELENGTH)
    {
        VPSS_FATAL("Error:Echo too long.\n");
        return (-1);
    }

    if(copy_from_user(chCmd,buf,count))
    {
        VPSS_FATAL("copy from user failed\n");
        return (-1);
    }

    pstInstDbg = &(pstInstance->stDbgCtrl.stInstDbg);

    VPSS_OSAL_GetProcArg(chCmd, chArg1, 1);
    VPSS_OSAL_GetProcArg(chCmd, chArg2, 2);
    VPSS_OSAL_GetProcArg(chCmd, chArg3, 3);
    VPSS_OSAL_GetProcArg(chCmd, chArg4, 4);

    if (chArg1[0] == 'h' && chArg1[1] == 'e' && chArg1[2] == 'l' && chArg1[3] == 'p')
    {
        HI_DRV_PROC_EchoHelper("-------------------VPSS debug options--------------------      \n"
                               "you can perform VPSS debug with such command                  \n"
                               "echo [arg1] [arg2] [arg3] > /proc/msp/vpssXX\n                  \n\n"
                               "debug action                    arg1        arg2                    arg3\n"
                               "-------------                   ----------  ---------------------   -----------\n"
                               " save one yuv                   saveyuv     src/port0/port1/port2\n");
        HI_DRV_PROC_EchoHelper(" print frameinfo                printinfo   src/port0/port1/port2\n"
                               " turn off info                  none        src/port0/port1/port2\n");

        HI_DRV_PROC_EchoHelper(" set progress/interlace on/off  i2p         src                     on/off\n"
                               " set vpssbypass                 setbypass   on/off\n"
                               " set uvinvert                   uvinvert    port0/port1/port2       on/off \n"
                               " set cmpon                      cmpon       port0/port1/port2\n");

		HI_DRV_PROC_EchoHelper(" set cmpoff                     cmpoff      port0/port1/port2\n"
                               " set rate                       setrate     on/off                  framerate\n"
                               " set out bitwidth               setbitwidth port0/port1/port2       8bit/10bit/12bit/disable\n"
                               " set rotation                   setrota     port0/port1/port2       0/90/180/270/disable\n"
                               " set src image w h              setsrcwh    port0/port1/port2/src   width  heigth\n"
#ifdef VPSS_SUPPORT_PROC_V2
                               " set proc level                 proc        0/1/2/3\n"
#endif
                               " save yuv in file with index    saveyuvidx  src/port0/port1/port2   index  count\n"
                               " save yuv in one file           saveyuvfile src/port0/port1/port2   index  count\n"

        );
    }
    else
    {
        VPSS_DBG_CMD_S stDbgCmd;

        if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_YUV,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_YUV;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_YUVIDX,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_YUV_IDX;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_YUVFILE,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_YUV_FILE;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_REVISEIMGWH,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_REVISE_SRC_WH;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_DBG,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_INFO_FRM;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_NONE,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_INFO_NONE;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_STREAM,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_W_STREAM;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_LOW,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_LOW;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_MIDDLE,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_MID;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_HIGH,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_HIGH;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_UHD_USR,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UHD_USR;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SET_UV,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_UV_INVERT;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_CMP_ON,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_CMP_ON;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_CMP_OFF,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_CMP_OFF;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SET_I2P,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.enDbgType = DBG_SET_I2P;
        }
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETROTATION,DEF_FILE_NAMELENGTH))
		{
			stDbgCmd.enDbgType = DBG_SET_RATATION;
		}
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETOUTBITWIDTH,DEF_FILE_NAMELENGTH))
		{
			stDbgCmd.enDbgType = DBG_SET_OUTBITWIDTH;
		}
		else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETBYPASS,DEF_FILE_NAMELENGTH))
        {
            if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->unInfo.bits.vpssbypass = 1;
            }
            else
            {
                pstInstDbg->unInfo.bits.vpssbypass = 0;
            }

            return count;
        }
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETRATE,DEF_FILE_NAMELENGTH))
        {
            if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->unInfo.bits.frameEn = 1;
            }
            else
            {
                pstInstDbg->unInfo.bits.frameEn = 0;
            }

            if(strlen(chArg3) == 0)
            {
                pstInstDbg->unInfo.bits.framerate = pstInstance->stInEntity.stStreamInfo.u32InRate;
            }
            else
            {
                pstInstDbg->unInfo.bits.framerate = (HI_U32)(simple_strtol(chArg3, NULL, 10));
            }

            return count;

        }
#ifdef VPSS_SUPPORT_PROC_V2
        else if (!HI_OSAL_Strncmp(chArg1,DEF_DBG_CMD_SETPROCLEVEL,DEF_FILE_NAMELENGTH))
        {
            if(!HI_OSAL_Strncmp(chArg2,DEF_DBG_PROCLEVEL_1,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->eProcPrintLevel = PROC_PRINT_LEVEL1;
            }
            else if(!HI_OSAL_Strncmp(chArg2,DEF_DBG_PROCLEVEL_2,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->eProcPrintLevel = PROC_PRINT_LEVEL2;
            }
            else if(!HI_OSAL_Strncmp(chArg2,DEF_DBG_PROCLEVEL_3,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->eProcPrintLevel = PROC_PRINT_LEVEL3;
            }
            else //(!HI_OSAL_Strncmp(chArg2,DEF_DBG_PROCLEVEL_ORI,DEF_FILE_NAMELENGTH))
            {
                pstInstDbg->eProcPrintLevel = PROC_PRINT_LEVEL_ORI;
            }
            return count;
        }
#endif
        else
        {
            VPSS_FATAL("Cmd Can't Support\n");
            goto PROC_EXIT;
        }

        s32Ret = HI_SUCCESS;
        if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_SRC,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_SRC_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_0,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT0_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_1,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT1_ID;
        }
        else if (!HI_OSAL_Strncmp(chArg2,DEF_DBG_PORT_2,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgPart = DEF_DBG_PORT2_ID;
        }
        else
        {
            VPSS_FATAL("Invalid para2 %s\n",chArg2);
            goto PROC_EXIT;
        }

        s32Ret = HI_SUCCESS;
        if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_ON,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_TRUE;
        }
        else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_OFF,DEF_FILE_NAMELENGTH))
        {
            stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_FALSE;
        }
        else
        {
        	if (stDbgCmd.enDbgType == DBG_SET_RATATION)
        	{
				if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_ROTATION_90,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_VPSS_ROTATION_90;
				}
				else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_ROTATION_180,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_VPSS_ROTATION_180;
				}
				else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_ROTATION_270,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_VPSS_ROTATION_270;
				}
				else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_ROTATION_0,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_VPSS_ROTATION_DISABLE;
				}
				else
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_VPSS_ROTATION_BUTT;
				}
			}

			if (stDbgCmd.enDbgType == DBG_SET_OUTBITWIDTH)
			{
				if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_OUTWIDTH_8BIT,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_PIXEL_BITWIDTH_8BIT;
				}
				else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_OUTWIDTH_10BIT,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_PIXEL_BITWIDTH_10BIT;
				}
				else if (!HI_OSAL_Strncmp(chArg3,DEF_DBG_SET_OUTWIDTH_12BIT,DEF_FILE_NAMELENGTH))
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_PIXEL_BITWIDTH_12BIT;
				}
				else
				{
					stDbgCmd.hDbgValue = (VPSS_HANDLE)HI_DRV_PIXEL_BITWIDTH_BUTT;
				}
			}
            if ((stDbgCmd.enDbgType == DBG_W_YUV_IDX) || (stDbgCmd.enDbgType == DBG_W_YUV_FILE))
            {
                stDbgCmd.hDbgValue = simple_strtoul(chArg3, NULL, 10);
                stDbgCmd.hDbgValue = stDbgCmd.hDbgValue << 16;
                stDbgCmd.hDbgValue = stDbgCmd.hDbgValue + (simple_strtoul(chArg4, NULL, 10) & 0xFFFF);
            }

            if (stDbgCmd.enDbgType == DBG_REVISE_SRC_WH)
            {
                stDbgCmd.hDbgValue = simple_strtoul(chArg3, NULL, 10);
                stDbgCmd.hDbgValue = stDbgCmd.hDbgValue << 16;
                stDbgCmd.hDbgValue = stDbgCmd.hDbgValue + (simple_strtoul(chArg4, NULL, 10) & 0xFFFF);
            }
        }

#if DEF_VPSS_DEBUG
        VPSS_DBG_SendDbgCmd(&(pstInstance->stDbgCtrl), &stDbgCmd);
#endif
    }
PROC_EXIT:

#endif //#ifndef HI_ADVCA_FUNCTION_RELEASE
    return count;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

