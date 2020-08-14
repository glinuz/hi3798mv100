/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : hi_mpi_pq.h
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Description   :

******************************************************************************/

#ifndef __HI_MPI_PQ_H__
#define __HI_MPI_PQ_H__

#include "hi_type.h"
#include "hi_unf_pq.h"
#include "hi_drv_pq.h"
#include "hi_mpi_disp.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MPI_PQ_CHECK_NULL_PTR(ptr) \
    do{\
        if (HI_NULL == ptr){\
            HI_ERR_PQ("ptr is NULL!\n");\
            return HI_ERR_PQ_NULL_PTR;\
        }\
    }while(0)


/*ACM table结构*/
typedef struct hiMPI_COLOR_PARAM_S
{
    HI_U16 u16Stretch;
    HI_U16 u16ClipRange;
    HI_U16 u16HueGain;         /*表示对Hue的增益，范围0-1023*/
    HI_U16 u16SatGain;         /*表示对Saturation的增益*/
    HI_U16 u16LumaGain;        /*表示对Luma的增益*/
    HI_U16 u16CbCrThre;        /*CbCr的门限值，若低于门限值就不做Color处理*/
    HI_S16 as16Luma[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM]; /*Luma查找表*/
    HI_S16 as16Hue[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];  /*Hue查找表*/
    HI_S16 as16Sat[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];  /*Saturation查找表*/

} MPI_COLOR_PARAM_S;

/*ACM 曲线存储结构*/
typedef struct hiMPI_ACM_PARAM_S
{
    HI_S16 as16Luma[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM]; /*Luma查找表*/
    HI_S16 as16Hue[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];  /*Hue查找表*/
    HI_S16 as16Sat[ACM_Y_NUM][ACM_S_NUM][ACM_H_NUM];  /*Saturation查找表*/
} MPI_ACM_PARAM_S;


typedef enum hiMPI_PQ_IMAGE_TYPE_E
{
    HI_MPI_PQ_IMAGE_GRAPH = 0,          /**<graph image */
    HI_MPI_PQ_IMAGE_VIDEO,              /**<video image */
    HI_MPI_PQ_IMAGE_BUTT
} HI_MPI_PQ_IMAGE_TYPE_E;

/**
 \PQ初始化
 \attention \n

 \param[in] pszPath:PQ配置文件路径
 \param[out]

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_Init(HI_CHAR* pszPath);

/**
 \PQ去初始化
 \attention \n

 \param[in]
 \param[out]

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_DeInit(HI_VOID);

/**
 \brief 获取亮度
 \attention \n
无

 \param[in] pu32Brightness：亮度值,有效范围: 0~255;
 \param[out]

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetBrightness(HI_DRV_DISPLAY_E enChan, HI_U32* pu32Brightness);

/**
 \brief 设置亮度
 \attention \n
无

 \param[in] u32Brightness：亮度值,有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetBrightness(HI_DRV_DISPLAY_E enChan, HI_U32 u32Brightness);

/**
 \brief 获取对比度
 \attention \n
无

 \param[in]
 \param[out] pu32Contrast：对比度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetContrast(HI_DRV_DISPLAY_E enChan, HI_U32* pu32Contrast);

/**
 \brief 设置对比度
 \attention \n
无

 \param[in] u32Contrast：对比度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetContrast(HI_DRV_DISPLAY_E enChan, HI_U32 u32Contrast);

/**
 \brief 获取色调
 \attention \n
无

 \param[in]
 \param[out] pu32Hue：色调, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetHue(HI_DRV_DISPLAY_E enChan, HI_U32* pu32Hue);

/**
 \brief 设置色调
 \attention \n
无

 \param[in] u32Hue：色调, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetHue(HI_DRV_DISPLAY_E enChan, HI_U32 u32Hue);

/**
 \brief 获取饱和度
 \attention \n
无

 \param[out] pu32Saturation：饱和度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetSaturation(HI_DRV_DISPLAY_E enChan, HI_U32* pu32Saturation);

/**
 \brief 设置饱和度
 \attention \n
无

 \param[in] u32Saturation：饱和度,有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSaturation(HI_DRV_DISPLAY_E enChan, HI_U32 u32Saturation);

/**
 \brief 获取降噪强度
 \attention \n
无

 \param[out] pu32TNRLevel: 降噪等级, 有效范围: 0~255


 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetTNR(HI_U32* pu32TNRLevel);
HI_S32 HI_MPI_PQ_GetSNR(HI_U32* pu32SNRLevel);

/**
 \brief 设置降噪强度
 \attention \n
无

 \param[in] u32TNRLevel: 降噪等级, 有效范围: 0~255

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNR(HI_U32 u32TNRLevel);
HI_S32 HI_MPI_PQ_SetSNR(HI_U32 u32SNRLevel);


/**
 \brief 获取SR演示类型
 \attention \n
无

 \param[out] *penType


 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetSRMode(HI_PQ_SR_DEMO_E* penType);

/**
 \brief 设置SR演示类型
 \attention \n
无

 \param[in] enType

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSRMode(HI_PQ_SR_DEMO_E enType);

/**
 \brief 获取清晰度
 \attention \n
无

 \param[out] pu32Sharpness：清晰度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetSharpness(HI_U32* pu32Sharpness);

/**
 \brief 设置清晰度
 \attention \n
无

 \param[in] u32Sharpness：清晰度, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSharpness(HI_U32 u32Sharpness);

/**
 \brief 获取块降噪De-blocking强度
 \attention \n
无

 \param[out] pu32DBlevel：降噪等级, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetDeBlocking(HI_U32* pu32DBlevel);

/**
 \brief 设置块降噪De-blocking强度
 \attention \n
无

 \param[in] u32DBlevel:降噪等级, 有效范围: 0~255;

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetDeBlocking(HI_U32 u32DBlevel);

HI_S32 HI_MPI_PQ_SetDeMosquito(HI_U32 u32DMlevel);

HI_S32 HI_MPI_PQ_GetDeMosquito(HI_U32* pu32DMlevel);


/**
 \brief 获取颜色增强
 \attention \n
无

 \param[out] pu32ColorGainLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetColorGain(HI_U32* pu32ColorGainLevel);

/**
 \brief 设置颜色增强
 \attention \n
无

 \param[in] enColorGainLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetColorGain(HI_U32 u32ColorGainLevel);

/**
 \brief 获取肤色增强
 \attention \n
  无

 \param[out] penFleshToneLevel

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetFleshTone(HI_UNF_PQ_FLESHTONE_E* penFleshToneLevel);

/**
 \brief 设置颜色增强类型
 \attention \n
  无

 \param[in] enSixBaseColor

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetColorEnhanceMode(HI_PQ_COLOR_SPEC_MODE_E enColorMode);

/**
 \brief 获取颜色增强类型
 \attention \n
  无

 \param[in] enSixBaseColor

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetColorEnhanceMode(HI_UNF_PQ_COLOR_SPEC_MODE_E* penColorMode);


/**
 \brief 设置肤色增强
 \attention \n
  无

 \param[in] enFleshToneLevel，参考HI_COLOR_GAIN_E

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetFleshTone( HI_UNF_PQ_FLESHTONE_E enFleshToneLevel);

/**
 \brief 获取六基色类型
 \attention \n
  无

 \param[out] pstSixBaseColor

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetSixBaseColor(HI_UNF_PQ_SIX_BASE_S* pstSixBaseColor);

/**
 \brief 设置六基色类型
 \attention \n
  无

 \param[in] stSixBaseColor

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSixBaseColor(HI_PQ_SIX_BASE_COLOR_S* pstSixBaseColor);

/**
 \brief 设置PQ模块开关
 \attention \n
  无

 \param[in] enFlags
 \param[in] u32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetPQModule( HI_UNF_PQ_MODULE_E enFlags, HI_U32 u32OnOff);

/**
 \brief 获取PQ模块开关
 \attention \n
  无

 \param[in] enFlags
 \param[in] pu32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetPQModule( HI_UNF_PQ_MODULE_E enFlags, HI_U32* pu32OnOff);

/**
 \brief 设置卖场模式开关
 \attention \n
无

 \param[in] enFlags
 \param[in] u32OnOff

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetDemo( HI_UNF_PQ_DEMO_E enFlags, HI_U32 u32OnOff);

/**
 \brief 设置卖场模式显示方式
 \attention \n
无

 \param[in] enChan
 \param[in] enMode

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetDemoMode( HI_DRV_DISPLAY_E enChan, HI_PQ_DEMO_MODE_E enMode);

/**
 \brief 获取卖场模式显示方式
 \attention \n
无

 \param[in] enChan
 \param[in] penMode

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetDemoMode( HI_DRV_DISPLAY_E enChan, HI_PQ_DEMO_MODE_E* penMode);



/**
 \brief 设置PQ寄存器
 \attention \n
无

 \param[in] u32RegAddr
 \param[in] u32Data

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_WritePQRegister(HI_U32 u32RegAddr, HI_U32 u32Data);

/**
 \brief 获取PQ寄存器
 \attention \n
无

 \param[in] u32RegAddr
 \param[in] *pu32Value

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_ReadPQRegister(HI_U32 u32RegAddr, HI_U32* pu32Value);

/**
 \brief 获取ACM曲线
 \attention \n
  无

 \param[in] *pstGammaTable

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetAcmTable( MPI_ACM_PARAM_S* pstColorTable);

/**
 \brief 设置ACM曲线
 \attention \n
  无

 \param[in] *pstGammaTable

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetAcmTable(HI_U32 u32RegAddr, MPI_ACM_PARAM_S* pstColorTable);

/**
 \brief 获取DCI曲线
 \attention \n
  无

 \param[in] *pstDCITable

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetDciTable( HI_PQ_DCI_WGT_S* pstDCITable);

/**
 \brief 设置DCI曲线
 \attention \n
  无

 \param[in] *pstDCITable

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetDciTable( HI_PQ_DCI_WGT_S* pstDCITable);

/**
 \brief 获取DCI直方图
 \attention \n
  无

 \param[in] *pstDCIHistgram

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_GetHistgram( HI_PQ_DCI_HISTGRAM_S* pstDCIHistgram);

/**
 \brief 获取TNR的亮度PixMean-to-Ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRLumaPixMean2Ratio( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的亮度PixMean-to-Ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRLumaPixMean2Ratio( HI_PQ_TNR_S* pstTnrData);


/**
 \brief 获取TNR的色度PixMean-to-Ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRChromPixMean2Ratio( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的色度PixMean-to-Ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRChromPixMean2Ratio( HI_PQ_TNR_S* pstTnrData);


/**
 \brief 获取TNR的亮度MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRLumaMotionMapping( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的亮度MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRLumaMotionMapping( HI_PQ_TNR_S* pstTnrData);


/**
 \brief 获取TNR的色度MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRChromMotionMapping( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的色度MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRChromMotionMapping( HI_PQ_TNR_S* pstTnrData);


/**
 \brief 获取TNR的亮度FINAL MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRLumaFinalMotionMapping( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的亮度FINAL MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRLumaFinalMotionMapping( HI_PQ_TNR_S* pstTnrData);


/**
 \brief 获取TNR的色度FINAL MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetTNRChromFinalMotionMapping( HI_PQ_TNR_S* pstTnrData);

/**
 \brief 设置TNR的色度FINAL MotionMapping曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetTNRChromFinalMotionMapping( HI_PQ_TNR_S* pstTnrData);

HI_S32 HI_MPI_PQ_GetTNRFMotionMapping(HI_PQ_TNR_FMOTION_S* pstTnrFMotion);

HI_S32 HI_MPI_PQ_SetTNRFMotionMapping(HI_PQ_TNR_FMOTION_S* pstTnrFMotion);


/**
 \brief 获取SNR的pixmean-ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetSNRPixmean2Ratio( HI_PQ_SNR_PIXMEAN_2_RATIO_S* pstSnrData);

/**
 \brief 设置SNR的pixmean-ratio曲线
 \attention \n
  无

 \param[in] *pstTnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSNRPixmean2Ratio( HI_PQ_SNR_PIXMEAN_2_RATIO_S* pstSnrData);

/**
 \brief 获取SNR的pixdiff-edgestr曲线
 \attention \n
  无

 \param[in] *pstSnrData

 \retval ::HI_SUCCESS

 */
HI_S32 HI_MPI_PQ_GetSNRPixdiff2Edgestr( HI_PQ_SNR_PIXDIFF_2_EDGESTR_S* pstSnrData);

/**
 \brief 设置SNR的pixdiff-edgestr曲线
 \attention \n
  无

 \param[in] *pstSnrData

 \retval ::HI_SUCCESS

 */

HI_S32 HI_MPI_PQ_SetSNRPixdiff2Edgestr( HI_PQ_SNR_PIXDIFF_2_EDGESTR_S* pstSnrData);
HI_S32 HI_MPI_PQ_GetDciLevel(HI_U32* pu32DCIlevel);
HI_S32 HI_MPI_PQ_SetDciLevel( HI_U32 u32DCIlevel);
HI_S32 HI_MPI_PQ_SetBasicImageParam(HI_MPI_PQ_IMAGE_TYPE_E enType, HI_DRV_DISPLAY_E enChan, HI_PQ_IMAGE_PARAM_S stParam);
HI_S32 HI_MPI_PQ_GetBasicImageParam(HI_MPI_PQ_IMAGE_TYPE_E enType, HI_DRV_DISPLAY_E enChan, HI_PQ_IMAGE_PARAM_S* pstParam);
HI_S32 HI_MPI_PQ_SetDefaultParam(HI_BOOL bDefault);
HI_S32 HI_MPI_PQ_GetDemoCoordinate(HI_U32* pu32X);
HI_S32 HI_MPI_PQ_SetDemoCoordinate(HI_U32 u32X);
HI_S32 HI_MPI_PQ_GetHdrSaturation(HI_U32* pu32Saturation);
HI_S32 HI_MPI_PQ_SetHdrSaturation(HI_U32 u32Saturation);
HI_S32 HI_MPI_PQ_SetImageMode(HI_UNF_PQ_IMAGE_MODE_E enImageMode);
HI_S32 HI_MPI_PQ_GetImageMode(HI_UNF_PQ_IMAGE_MODE_E *penImageMode);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* End of #ifndef __HI_MPI_PQ_H__ */



