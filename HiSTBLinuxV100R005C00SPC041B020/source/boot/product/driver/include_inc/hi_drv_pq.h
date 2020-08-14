#ifndef __HI_DRV_PQ_H__
#define __HI_DRV_PQ_H__


#include "drv_pq_alg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PQ_VERSION "1.0.2.0"
#define PQ_DEF_NAME         "pqparam"
#define MCE_DEF_PQPARAM_SIZE       (512*1024)

#if 0
#define HI_FATAL_PQ(format, arg...)    printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_ERR_PQ(format, arg...)      printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_WARN_PQ(format, arg...)     printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#define HI_INFO_PQ(format, arg...)     printf( "%s,%d: " format , __FUNCTION__, __LINE__, ## arg)
#else
#define HI_FATAL_PQ(format...)    HI_FATAL_PRINT(HI_ID_PQ, format)
#define HI_ERR_PQ(format...)      HI_ERR_PRINT(HI_ID_PQ, format)
#define HI_WARN_PQ(format...)     HI_WARN_PRINT(HI_ID_PQ, format)
#define HI_INFO_PQ(format...)     HI_INFO_PRINT(HI_ID_PQ, format)
#endif

typedef struct hi_PQ_TOP_OFST_TABLE_S
{
    HI_U32      u32FileHeaderOfst;
    HI_U32      u32DefaltOptOfst;
    HI_U32      u32CoefOfst;
} PQ_TOP_OFST_TABLE_S;

/////////////////////////////////////////////////define header///////////////////////////////////////////////////////////////
typedef struct hi_PQ_FILE_HEADER_S
{
    HI_U32          u32FileCheckSum;        /* 参数文件的校验和，File Header（除此变量外）和data的所有逐字节校验和，用于检验参数正确性和判断是否dirty */
    HI_U32          u32ParamSize;           /* 参数文件大小，包括File header和data */
    HI_CHAR         u8Version[STR_LEN_32];  /* 版本号，字符串表示 */
    HI_CHAR         u8Author[STR_LEN_32];   /* 参数调试者签名，字符串表示 */
    HI_CHAR         u8Desc[STR_LEN_1024];   /* 版本描述，字符串表示 */
    HI_CHAR         u8Time[STR_LEN_32];     /* 参数文件生成（烧写）时间，[0] ~ [5]：yy mm dd hh mm ss，[6]~[7]保留。该时间由PQ工具从PC上自动获取，无需用户输入 */
    /* 以下是参数文件生成时的Chip、SDK版本，当这个参数文件被下载到其它盒子时，接受参数的盒子根据这两个参数判断是否兼容 */
    HI_CHAR         u8ChipName[STR_LEN_32];
    HI_CHAR         u8SDKVersion[STR_LEN_80];
} PQ_FILE_HEADER_S;

typedef struct hi_PQ_OPT_OFST_TABLE_S
{
    HI_U32 u32OptCoefDefaultOfSt;
    HI_U32 u32OptCoefWorkOfSt;
} PQ_OPT_OFST_TABLE_S;

typedef struct hi_PQ_OPT_S
{
    PQ_OPT_OFST_TABLE_S stPqOptOfstTable;
    PQ_OPT_COEF_S       stDefaultPqOptCoef;
    PQ_OPT_COEF_S       stWorkPqOptCoef;
} PQ_OPT_S;

typedef struct hi_PQ_COEF_OFST_TABLE_S
{
    HI_U32      u32DeiOfst;
    HI_U32      u32FmdOfst;
    HI_U32      u32DnrOfst;
    HI_U32      u32SharpOfst;
    HI_U32      u32CscOfst;
    HI_U32      u32AccOfst;
    HI_U32      u32AcmOfst;
    HI_U32      u32GammaOfst;
    HI_U32      u32DitherOfst;
    HI_U32      u32stColorTempOfst;
} PQ_COEF_OFST_TABLE_S;

typedef struct hi_PQ_COEF_S
{
    PQ_COEF_OFST_TABLE_S        stCoefOfstTable;
    PQ_DEI_COEF_S               stDeiCoef;
    PQ_FMD_COEF_S               stFmdCoef;
    PQ_DNR_COEF_S               stDnrCoef;
    PQ_SHARP_COEF_S             stSharpCoef;
    PQ_CSC_COEF_S               stCscCoef;
    PQ_ACC_COEF_S               stAccCoef;
    PQ_ACM_COEF_S               stAcmCoef;
    PQ_GAMMA_COEF_S             stGammaCoef;
    PQ_DITHER_COEF_S            stDitherCoef;
    PQ_COLOR_TEMP_COEF_S        stColorTempCoef;
    PQ_GFX_DEFLICKER_S          stGfxCoef;
} PQ_COEF_S;

typedef struct hi_PQ_PARAM_S
{
    PQ_TOP_OFST_TABLE_S         stTopOfstTable;
    PQ_FILE_HEADER_S            stPQFileHeader;
    PQ_OPT_S                    stPqOption;
    PQ_COEF_S                   stPQCoef;
} PQ_PARAM_S;


HI_VOID HI_PQ_Init(HI_VOID);
HI_S32 HI_PQ_GetPQData(PQ_PARAM_S** pAddr);

HI_S32 HI_PQ_GetBrightness(HI_U32 enChan, HI_U32 *pu32Brightness);
HI_S32 HI_PQ_GetContrast(HI_U32 enChan, HI_U32 *pu32Contrast);
HI_S32 HI_PQ_GetHue(HI_U32 enChan, HI_U32 *pu32Hue);
HI_S32 HI_PQ_GetSaturation(HI_U32 enChan, HI_U32 *pu32Saturation);
#if 0
HI_S32 HI_PQ_SetBrightness(HI_U32 enChan, HI_U32 u32Brightness);
HI_S32 HI_PQ_SetContrast(HI_U32 enChan, HI_U32 u32Contrast);
HI_S32 HI_PQ_SetHue(HI_U32 enChan, HI_U32 u32Hue);
HI_S32 HI_PQ_SetSaturation(HI_U32 enChan, HI_U32 u32Saturation);
#endif

#ifdef __cplusplus
}
#endif

#endif


