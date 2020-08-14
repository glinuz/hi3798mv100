/******************************************************************************

  Copyright (C), 2011-2014, HiSilicon Technologies Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_pdm.h
  Version       : Initial Draft
  Author        : HiSilicon multimedia software group
  Created       : 2011-xx-xx
  Description   : hi_unf_pdm.h header file
  History       :
  1.Date        : 2011/xx/xx
    Author      : 
    Modification: This file is created.

******************************************************************************/

#ifndef __HI_UNF_PDM_H__
#define __HI_UNF_PDM_H__

#include "hi_unf_common.h"
#include "hi_unf_mce.h"
#include "hi_go_surface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  /* End of #ifdef __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      PDM */
/** @{ */  /** <!-- [PDM] */

/**Defines the base config parameter*/
/** CNcomment:基本配置参数结构体*/
typedef struct hiUNF_PDM_DISP_PARAM_S
{
	HI_UNF_DISP_E				enSrcDisp;			/**<source display*//**<CNcomment: 同源display枚举*/
    HI_UNF_ENC_FMT_E            enFormat;           /**<Format*//**<CNcomment: 制式*/
    HI_U32                      u32Brightness;      /**<Brightness*//**<CNcomment: 亮度*/ 
    HI_U32                      u32Contrast;        /**<Contrast*//**<CNcomment: 对比度*/ 
    HI_U32                      u32Saturation;      /**<Saturation*//**<CNcomment: 饱和度*/
    HI_U32                      u32HuePlus;         /**<HuePlus*//**<CNcomment: 色度*/
    HI_BOOL                     bGammaEnable;       /**<Is Gamma enable*//**<CNcomment: 是否使能gamma*/   
    HI_UNF_DISP_BG_COLOR_S      stBgColor;          /**<Background Color*//**<CNcomment: 背景色*/
    HI_UNF_DISP_INTF_S          stIntf[HI_UNF_DISP_INTF_TYPE_BUTT];     /**<configuration of display interface*//**<CNcomment: 显示接口配置*/
    HI_UNF_DISP_TIMING_S        stDispTiming;       /**<Display timeing*//**<CNcomment: 显示时序*/
	
    HIGO_PF_E                   enPixelFormat;      /**<Pixel format*//**<CNcomment: 像素格式*/
    HI_UNF_DISP_OFFSET_S        stDispOffset;		/**<Display margin info*//**<CNcomment: 显示空白区域*/
    HI_U32                      u32VirtScreenWidth;	/**<Virtual screen width*//**<CNcomment: 虚拟屏幕宽*/
    HI_U32                      u32VirtScreenHeight;	/**<Virtual screen height*//**<CNcomment: 虚拟屏幕高*/
	
    HI_UNF_DISP_ASPECT_RATIO_S  stAspectRatio;      /**<The device aspect ratio*//**<CNcomment: 设备宽高比*/
    HI_UNF_HDMI_VIDEO_MODE_E    enVidOutMode;       /**<HDMI output vedio mode*//**<CNcomment:HDMI输出视频模式*/
    HI_UNF_HDMI_DEEP_COLOR_E    enDeepColorMode;    /**<Deep Color output mode*//**<CNcomment:DeepColor输出模式*/
}HI_UNF_PDM_DISP_PARAM_S;

typedef struct hiUNF_PDM_SOUND_PARAM_S
{
    HI_U32                    u32PortNum;  /**<Outport number attached sound*/ /**<CNcomment:绑定到Sound设备的输出端口数*/
    HI_UNF_SND_OUTPORT_S stOutport[HI_UNF_SND_OUTPUTPORT_MAX];  /**<Outports attached sound*/ /**<CNcomment:绑定到Sound设备的输出端口*/
    HI_U32               au32Volume[HI_UNF_SND_OUTPUTPORT_MAX];
}HI_UNF_PDM_SOUND_PARAM_S;

typedef enum hiUNF_PDM_BASEPARAM_TYPE_E
{
    HI_UNF_PDM_BASEPARAM_DISP0 = 0,     /*HI_UNF_PDM_DISP_PARAM_S * */
    HI_UNF_PDM_BASEPARAM_DISP1,         /*HI_UNF_PDM_DISP_PARAM_S * */
    HI_UNF_PDM_BASEPARAM_SOUND0 = 10,
    HI_UNF_PDM_BASEPARAM_SOUND1,
    HI_UNF_PDM_BASEPARAM_SOUND2,
    HI_UNF_PDM_BASEPARAM_HDMI = 20,

    HI_UNF_PDM_BASEPARAM_BUTT = 0xFFFF,
}HI_UNF_PDM_BASEPARAM_TYPE_E;

typedef struct hiUNF_PDM_HDMI_PARAM_S
{
	HI_U8    *pu8EDID;
	HI_U32   *pu32EDIDLen;
}HI_UNF_PDM_HDMI_PARAM_S;

/** @} */  /*! <!-- Structure Definition end */

/******************************* API declaration *****************************/
/** \addtogroup      PDM */
/** @{ */  /** <!-- [PDM] */
/** 
\brief Obtains the basic configuration information.  CNcomment:获取BASE配置区信息 CNend
\attention \n
N/A
\param[in]  pstBaseParam   Pointer to the basic parameters.  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData);


/** 
\brief Modifies the basic configuration information.  CNcomment:更新BASE配置区信息 CNend
\attention \n
N/A
\param[in]  pstBaseParam Pointer to the basic parameters.  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdateBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData);

/** 
\brief Obtains the basic configuration information from the flash memory.  
CNcomment:获取LOGO配置区信息 CNend
\attention \n
N/A
\param[in]  pstLogoParam   Pointer to the logo parameter  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam);

/** 
\brief Modifies the configuration of the logo parameter.  CNcomment:更新LOGO配置区信息 CNend
\attention \n
Update logo parameter, you must invoke this function befor update data of logo
\CNcomment:更新logo参数，在更新logo内容前必须先调用此函数 CNend
\param[in]  pstLogoParam   Pointer to the logo parameter  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdateLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam);


/** 
\brief Obtains the logo data from the flash memory. CNcomment:获取LOGO内容区信息 CNend
\attention \n
N/A
\param[in]  pu8Content Pointer to the logo data that is written from the flash memory CNcomment:内容buf指针 CNend
\param[in]  u32Size  Size of the logo data to be written  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetLogoContent(HI_U8 *pu8Content, HI_U32 u32Size);


/** 
\brief Modifies the logo data.  CNcomment:更新LOGO内容区信息 CNend
\attention \n
Update data in logo partition, You must invoke HI_UNF_PDM_UpdateLogoParam to update logo parameter before invoke this function
\CNcomment:更新logo内容，在此函数之前必须先调用HI_UNF_PDM_UpdateLogoParam函数更新logo参数 CNend
\param[in]  pu8Content   Pointer to the logo data that is read from the flash memory  CNcomment:内容buf指针 CNend
\param[in]  u32Size   Size of the logo data to be read  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdateLogoContent(HI_U8 *pu8Content, HI_U32 u32Size);

/** 
\brief Obtains the playing configuration information.  CNcomment:获取PLAY配置区信息 CNend
\attention \n
N/A
\param[in]  pstPlayParam   Playing configuration information  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetPlayParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

/** 
\brief   Modifies the playing configuration information.  CNcomment:更新PLAY配置区信息 CNend
\attention \n 
Update play parameter, you must invoke this function befor update data of play
\CNcomment:更新play参数，在更新play内容前必须先调用此函数 CNend
\param[in]  pstPlayParam  Playing configuration information  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdatePlayParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

/** 
\brief Obtains the data being playing from the flash memory.  CNcomment:获取PLAY内容区信息 CNend
\attention \n
N/A
\param[in]  pu8Content Pointer to the data that is read from the flash memory  CNcomment:内容buf指针 CNend
\param[in]  u32Size  Size of the data being played that is read from the flash memory  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetPlayContent(HI_U8 *pu8Content, HI_U32 u32Size);


/** 
\brief Updates the playing data part. CNcomment:更新PLAY内容区信息 CNend
\attention \n
Update data in play partition, You must invoke HI_UNF_PDM_UpdatePlayParam to update plat parameter before invoke this function
\CNcomment:更新play内容，在此函数之前必须先调用HI_UNF_PDM_UpdatePlayParam函数更新play参数 CNend
\param[in]  pu8Content  Pointer to the data that is written to the flash memroy  CNcomment:内容buf指针 CNend
\param[in]  u32Size  Size of the data being played  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdatePlayContent(HI_U8 *pu8Content, HI_U32 u32Size);

/** 
\brief Obtains the playing configuration information from backup partition.  CNcomment:从备份分区获取PLAY配置区信息 CNend
\attention \n
N/A
\param[in]  pstPlayParam   Playing configuration information  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetPlayBakParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

/** 
\brief   Modifies the playing configuration information to backup partition.  CNcomment:更新PLAY配置区信息 CNend
\attention \n 
Update play parameter, you must invoke this function befor update data of play
\CNcomment:更新play参数，在更新play内容前必须先调用此函数 CNend
\param[in]  pstPlayParam  Playing configuration information  CNcomment:参数指针 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdatePlayBakParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam);

/** 
\brief Obtains the playing content from backup partition.  CNcomment:从备份分区获取PLAY内容区信息 CNend
\attention \n
N/A
\param[in]  pu8Content Pointer to the data that is read from the flash memory  CNcomment:内容buf指针 CNend
\param[in]  u32Size  Size of the data being played that is read from the flash memory  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_GetPlayBakContent(HI_U8 *pu8Content, HI_U32 u32Size);


/** 
\brief Updates the playing content to backup partition. CNcomment:更新PLAY内容到备份分区 CNend
\attention \n
Update data in play partition, You must invoke HI_UNF_PDM_UpdatePlayBakParam to update plat parameter before invoke this function
\CNcomment:更新play内容，在此函数之前必须先调用HI_UNF_PDM_UpdatePlayBakParam函数更新play参数 CNend
\param[in]  pu8Content  Pointer to the data that is written to the flash memroy  CNcomment:内容buf指针 CNend
\param[in]  u32Size  Size of the data being played  CNcomment:内容长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_WRITE      Write flash pation error. CNcomment:写Flash分区失败 CNend
\retval :: HI_ERR_PDM_MTD_ERASE      Erase flash pation error. CNcomment:擦除Flash分区失败 CNend
\retval :: HI_ERR_PDM_INVALID_OPT      The operation is invalid. CNcomment:无效操作 CNend
\see \n
N/A
*/
HI_S32  HI_UNF_PDM_UpdatePlayBakContent(HI_U8 *pu8Content, HI_U32 u32Size);


/** 
\brief Obtains the device cfg info.  CNcomment:获取设备相关的配置信息 CNend
\attention \n
N/A
\param[in]  pstrCfgName Pointer to the cfg name,for exampe,
"MAC" to get the MAC address,"SN" to get the SN number.
Other configure items can be refered from the Readme file and BurnConfig.ini in HiproInfo tool directory. 
CNcomment:传入的配置项名,例如"MAC"为获取MAC地址,"SN"为获取SN号,
其他配置项请参考HiproInfo 工具目录的Readme文件及配置文件 CNend
\param[in]  pstrCfgValue Pointer to the cfg output buffer  CNcomment:输出
buffer的地址，输出buffer用以保存读取到的值 CNend
\param[in]  u32Size  Size of the output buffer  CNcomment:输出buffer长度 CNend
\retval ::HI_SUCCESS       Success. CNcomment:成功 CNend
\retval :: HI_ERR_PDM_PTR_NULL      Pointer is null. CNcomment:空指针 CNend
\retval :: HI_ERR_PDM_GET_MTDINFO_ERR      Get flash pation infomation error. 
CNcomment:获取Flash分区信息失败 CNend
\retval :: HI_ERR_PDM_MTD_OPEN      Open flash pation error. CNcomment:打开
Flash分区失败 CNend
\retval :: HI_ERR_PDM_MEM_ALLC      Alloc memory error. CNcomment
:分配内存失败 CNend
\retval :: HI_ERR_PDM_MTD_READ      Read flash pation error. CNcomment:读取
Flash分区失败 CNend
\retval :: HI_ERR_PDM_GET_DEVINFO_ERR      Parse device info failed. CNcomment
:解析device info配置失败  CNend
\see \n
N/A
*/
HI_S32 HI_UNF_PDM_GetDeviceInfo(HI_CHAR * pstrCfgName, HI_CHAR * pstrCfgValue, HI_U32 u32Size);

/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif




