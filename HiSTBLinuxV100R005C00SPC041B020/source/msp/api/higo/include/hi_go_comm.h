/**
 \file
 \brief Describes the header file that contains the common contents of the HiGo.CNcomment:HiGo公共内容头文件 CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \version 1.0
 \author s37678
 \date 2008-5-16
 */

#ifndef __HI_GO_COMM_H__
#define __HI_GO_COMM_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

/***************************** Macro Definition ******************************/
/*************************** Structure Definition ****************************/
/** \addtogroup      HIGO_COMMON */
/** @{ */  /**<!—[HIGO_COMMON] */

/**Maximum number of colors in the palette*//** CNcomment: 调色板最大颜色数 */
#define MAX_PALETTE_COLOR_SIZE 256

/**Color value*//** CNcomment: 颜色值 */
typedef HI_U32 HI_COLOR;

/**Palette*//** CNcomment: 调色板 */
typedef HI_COLOR HI_PALETTE[MAX_PALETTE_COLOR_SIZE];

typedef enum 
{
    HIGO_IMGTYPE_JPEG = 0, /**<.jpeg picture*//**<CNcomment: JPEG格式图片*/
    HIGO_IMGTYPE_GIF,      /**<.gif picture*//**<CNcomment: GIF格式图片*/
    HIGO_IMGTYPE_BMP,       /**<.bmp picture*//**<CNcomment: BMP格式图片 */
    HIGO_IMGTYPE_PNG,      /**<.png picture*//**<CNcomment: PNG格式图片 */
    HIGO_IMGTYPE_RLE,      /**<.rle picture*//**<CNcomment: RLE格式图片 */
    HIGO_IMGTPYE_BUTT
} HIGO_IMGTYPE_E;


/**Rectangle*//** CNcomment: 矩形 */
typedef struct
{
    HI_S32 x, y;

    HI_S32 w, h;
} HI_RECT;

typedef HI_RECT HI_POINT;

/**Region*//** CNcomment: 区域 */
typedef struct
{
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
} HI_REGION;

/**Mode of adjusting the window z-order*//**CNcomment:Z序调整方式*/
typedef enum
{
    HIGO_ZORDER_MOVETOP = 0,  /**<Move to the top*//**<CNcomment:移到最顶部*/
    HIGO_ZORDER_MOVEUP,       /**<Move upwards*//**<CNcomment:向上移*/
    HIGO_ZORDER_MOVEBOTTOM,   /**<Move to the bottom*//**<CNcomment:移到最底部*/
    HIGO_ZORDER_MOVEDOWN,     /**<Move downwards*//**<CNcomment:向下移*/
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;

#define HIGO_INVALID_HANDLE 0x0

/**Stream position*//** CNcomment: 流式位置*/


/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** \addtogroup      HIGO_COMMON */
/** @{ */  /** <!—[HIGO_COMMON] */


/** 
\brief Initializes the HiGo.CNcomment:HiGo初始化 CNend
\attention \n
Before using the HiGo, you must call this application programming interface (API) to initialize the HiGo. The HiGo 
includes the graphic device (Gdev) module, decoder, Winc module, bit block transfer (Bliter) module, and surface 
module.
CNcomment:使用HiGo功能必须先调用该接口，完成HiGo的初始化，主要包括gdev, decoder, winc, blit, surface模块 CNend
\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Deinit
*/
HI_S32	HI_GO_Init(HI_VOID);

/**
\brief Deinitializes the HiGo.CNcomment: HiGo去初始化 CNend
\attention \n
If the HiGo is not used, you need to call this API to release resources.CNcomment: 不再使用HiGo功能时需要调用该接口，释放资源 CNend
\param N/A.CNcomment: 无 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Init
*/
HI_S32	HI_GO_Deinit(HI_VOID);

/** 
\brief Initializes the extended library of the HiGo.CNcomment:HiGo 扩展库初始化 CNend
\attention \n
If the extended library of the HiGo is not used, you need to call this API to release resources, especially the 
resources of the cursor and text modules.
CNcomment:主要是CURSOR和TEXT模块。CNend
\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_DeInitExt
*/
HI_S32 HI_GO_InitExt(HI_VOID);

/**
\brief Deinitializes the extended library of the HiGo.CNcomment: HiGo扩展库去初始化 CNend
\attention \n
If the HiGo is not used, you need to call this API to release resources.CNcomment: 不再使用HiGo扩展库功能时需要调用该接口，释放资源，主要是CURSOR和TEXT模块。CNend
\param N/A.CNcomment: 无 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_InitExt
*/
HI_S32 HI_GO_DeInitExt(HI_VOID);

/** 
\brief Obtains the version information.CNcomment:获取版本信息 CNend
\attention \n
N/A.CNcomment:无 CNend
\param[out] ppVersion Output address of the version information string. The value cannot be empty.CNcomment: 版本信息字符串输出地址，不可为空 CNend
\param[out] ppBuildTime Build Output address of the time string. The value cannot be empty.CNcomment:Build时间字符串输出地址，不可为空 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

\see \n
N/A.CNcomment:无 CNend
*/
HI_S32 HI_GO_GetVersion(HI_CHAR ** ppVersion, HI_CHAR **ppBuildTime);

/**
\brief Converts the data on a surface into a .bmp picture for output.CNcomment: 将Surface中的数据转换成BMP格式图象输出。CNend
\attention \n
The output file is [year]-[date]-[hour]-[min]-[second]-[ms].bmp.CNcomment: 输出文件名为[year]-[date]-[hour]-[min]-[second]-[ms].bmp CNend
The output picture must be a 16-bit bitmap.CNcomment: 输出图片固定为16位图。CNend
It is recommended that you call HI_GO_EncodeToFile rather than HI_GO_Surface2Bmp.CNcomment: 建议使用HI_GO_EncodeToFile接口来代替此接口 CNend

\param[in] Surface Data to be captured.CNcomment: 需要进行截屏的数据。CNend
\param[in] pRect Pointer to a rectangle. If this parameter is not set, it indicates the entire surface.CNcomment: 区域指针,为空表示整个surface。CNend


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVSURFACEPF
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NOMEM

*/
HI_S32	HI_GO_Surface2Bmp(HI_HANDLE Surface, const HI_RECT *pRect);

/** 
\brief Enables the memory management module.CNcomment:使能内存管理模块 CNend
\attention \n
The memory management module is disabled by default. Before using the memory statistics function, you must call this 
API to enable the memory management module.
CNcomment:默认情况处于非使能状态，只有该接口打开后才能使用内存统计功能 CNend

\param[in] bEnable Whether to enable the memory statistics function.CNcomment:是否开启内存统计功能 CNend

\retval ::HI_SUCCESS

*/
HI_S32 HI_GO_EnableMemMng(HI_BOOL bEnable);



/** 
\brief Obtains the enable status of the memory management module.CNcomment:获取内存管理模块使能状态 CNend
\attention \n
\param[out] pbEnable Whether to obtain the enable status of the memory statistics function.CNcomment:获取内存统计功能是否开启 CNend

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

*/
HI_S32 HI_GO_GetMemMngStatus(HI_BOOL *pbEnable);

/** 
\brief Outputs the general information about the system memory.CNcomment:输出系统内存的总体信息 CNend
\attention \n
\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS
*/
HI_S32 HI_GO_SysMemQuene(HI_VOID);

/** 
\brief Outputs the general information about the media memory zone (MMZ).CNcomment: 输出MMZ内存的总体信息。CNend
\attention \n
\param N/A.CNcomment:无 CNend

\retval ::HI_SUCCESS

*/
HI_S32 HI_GO_MMZMemQuene(HI_VOID);

/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /* __HI_GO_COMM_H__ */
