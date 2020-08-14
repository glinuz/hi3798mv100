#ifndef __OMXVDEC_PTS_RECV_H__
#define __OMXVDEC_PTS_RECV_H__

/******************************* Include Files *******************************/

/* add include here */
#include "hi_type.h"
#include "omxvdec.h"

//#include "hi_unf_common.h"
//#include "hi_unf_avplay.h"
#include "vfmw.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

/*************************** Enum Definition ****************************/
/**Defines the source of frame rate.*/
/**CNcomment: 定义帧率来源类型的枚举 */
typedef enum hi_FRMRATE_TYPE_E
{
    HI_FRMRATE_TYPE_PTS,         /**<Use the frame rate calculates from PTS*//**<CNcomment: 采用PTS计算帧率 */
    HI_FRMRATE_TYPE_STREAM,      /**<Use the frame rate comes from stream*//**<CNcomment: 采用码流信息中的帧率 */
    HI_FRMRATE_TYPE_USER,        /**<Use the frame rate set by user*//**<CNcomment: 采用用户设置的帧率 */
    HI_FRMRATE_TYPE_USER_PTS,    /**<Use the frame rate set by user until the 2nd I frame comes, then use the frame rate calculates from PTS*//**<CNcomment: 第二个I帧来之前采用用户设置的帧率，之后根据PTS计算帧率 */
    HI_FRMRATE_TYPE_BUTT
}HI_FRMRATE_TYPE_E;

/*************************** Structure Definition ****************************/

typedef struct hi_VCODEC_FRMRATE_S
{
    HI_U32 u32fpsInteger;     /**<Integral part of the frame rate (in frame/s)*/ /**<CNcomment: 码流的帧率的整数部分, fps */
    HI_U32 u32fpsDecimal;     /**<Fractional part (calculated to three decimal places) of the frame rate (in frame/s)*/
                              /**<CNcomment: 码流的帧率的小数部分（保留3位）, fps */
}HI_VCODEC_FRMRATE_S;

/**Defines the parameter of frame rate.*/
/**CNcomment: 定义帧率属性参数的结构体 */
typedef struct hi_FRMRATE_PARAM_S
{
    HI_FRMRATE_TYPE_E      enFrmRateType;  /**<The source of frame rate*//**<CNcomment: 帧率来源类型 */
    HI_VCODEC_FRMRATE_S    stSetFrmRate;   /**<Setting frame rate*//**<CNcomment: 设置的帧率 */
}HI_FRMRATE_PARAM_S;

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
HI_S32 OMX_PTSREC_Init(HI_VOID);
HI_S32 OMX_PTSREC_DeInit(HI_VOID);
HI_S32 OMX_PTSREC_Alloc(HI_HANDLE hHandle);
HI_S32 OMX_PTSREC_Free(HI_HANDLE hHandle);
HI_S32 OMX_PTSREC_Start(HI_HANDLE hHandle);
HI_S32 OMX_PTSREC_Stop(HI_HANDLE hHandle);
HI_S32 OMX_PTSREC_Reset(HI_HANDLE hHandle);
HI_S32 OMX_PTSREC_SetFrmRate(HI_HANDLE hHandle, HI_FRMRATE_PARAM_S *pstFrmRate);
HI_S32 OMX_PTSREC_GetFrmRate(HI_HANDLE hHandle, HI_FRMRATE_PARAM_S *pstFrmRate);
HI_U32 OMX_PTSREC_CalcStamp(HI_HANDLE hHandle, IMAGE *pstImage);
HI_U32 OMX_PTSREC_GetInterPtsDelta(HI_HANDLE hHandle);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __PTS_RECV_H__ */
