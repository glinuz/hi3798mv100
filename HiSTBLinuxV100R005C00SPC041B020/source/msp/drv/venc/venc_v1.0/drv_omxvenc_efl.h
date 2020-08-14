#ifndef __DRV_OMXVENC_EFL_H__
#define __DRV_OMXVENC_EFL_H__

#include "drv_venc_efl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define INPUT_PORT   0
#define OUTPUT_PORT  1
#define ALL_PORT    0xFFFFFFFF


HI_S32 VENC_DRV_EflQueueFrame_OMX( HI_U32 EncHandle, venc_user_buf*  pstFrame);
HI_S32 VENC_DRV_EflPutMsg_OMX(queue_info_s* queue, HI_U32 MsgID, HI_U32 Status, void* MsgData);
HI_S32 VENC_DRV_EflGetMsg_OMX(queue_info_s* queue, venc_msginfo* pmsg_info);

HI_S32 VENC_DRV_EflMMapToKernel_OMX(HI_U32 ChanId, HI_MMZ_BUF_S* pMMZbuf);
HI_S32 VENC_DRV_EflUMMapToKernel_OMX(HI_U32 ChanId, HI_MMZ_BUF_S* pMMZbuf);
HI_S32 VENC_DRV_EflFlushPort_OMX(VeduEfl_EncPara_S* EncHandle, HI_U32 u32PortIndex, HI_BOOL bIntra);
/*************************************************************************************/
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif //__DRV_OMXVENC_EFL_H__
