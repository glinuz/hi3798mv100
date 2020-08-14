/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_disp_tran.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/17
  Description   :
  History       :
  1.Date        : 2013/1/31
    Author      : 
    Modification: Created file

*******************************************************************************/


#ifndef __MPI_DISP_TRAN_H__
#define __MPI_DISP_TRAN_H__

#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"

#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_drv_win.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

HI_S32 Transfer_DispType(HI_UNF_DISP_HDR_TYPE_E *pU, HI_DRV_DISP_OUT_TYPE_E *pM, HI_BOOL bu2m);
HI_S32 Transfer_DispID(HI_UNF_DISP_E *pU, HI_DRV_DISPLAY_E *pM, HI_BOOL bu2m);
HI_S32 Transfer_DispOffset(HI_UNF_DISP_OFFSET_S *pU, HI_DRV_DISP_OFFSET_S *pM, HI_BOOL bu2m);
HI_S32 Transfer_LayerID(HI_UNF_DISP_LAYER_E *pU, HI_DRV_DISP_LAYER_E *pM, HI_BOOL bu2m);
HI_S32 Transfer_EncFmt(HI_UNF_ENC_FMT_E *pU, HI_DRV_DISP_FMT_E *pM, HI_BOOL bu2m);
HI_S32 Transfer_AspectRatio(HI_UNF_DISP_ASPECT_RATIO_S *pU, HI_U32 *pH, HI_U32 *pV, HI_BOOL bu2m);
HI_S32 Transfer_Timing(HI_UNF_DISP_TIMING_S *pU, HI_DRV_DISP_TIMING_S *pM, HI_BOOL bu2m);
HI_S32 Transfer_BGColor(HI_UNF_DISP_BG_COLOR_S *pU, HI_DRV_DISP_COLOR_S *pM, HI_BOOL bu2m);


HI_S32 Transfer_VideoFormat(HI_UNF_VIDEO_FORMAT_E  *pU, HI_DRV_PIX_FORMAT_E *pM, HI_BOOL bu2m);
HI_S32 Transfe_ARConvert(HI_UNF_VO_ASPECT_CVRS_E  *pU, HI_DRV_ASP_RAT_MODE_E *pM, HI_BOOL bu2m);
HI_S32 Transfe_ZOrder(HI_LAYER_ZORDER_E *pU, HI_DRV_DISP_ZORDER_E *pM, HI_BOOL bu2m);
HI_S32 Transfe_SwitchMode(HI_UNF_WINDOW_FREEZE_MODE_E *pU, HI_DRV_WIN_SWITCH_E *pM, HI_BOOL bu2m);
HI_S32 Transfer_Disp3DMode(HI_UNF_DISP_3D_E *pU, HI_DRV_DISP_STEREO_MODE_E *pM, HI_BOOL bu2m);
HI_S32 Transfe_Rotate(HI_UNF_VO_ROTATION_E *pU, HI_DRV_ROT_ANGLE_E *pM, HI_BOOL bu2m);




HI_S32 Transfer_Frame(HI_UNF_VIDEO_FRAME_INFO_S  *pU, HI_DRV_VIDEO_FRAME_S *pM, HI_BOOL bu2m);
HI_S32 Transfer_BufferPool(HI_UNF_BUFFER_ATTR_S *pU, HI_DRV_VIDEO_BUFFER_POOL_S*pM, HI_BOOL bu2m);
HI_S32 Transfer_CastCfg(HI_UNF_DISP_CAST_ATTR_S  *pU, HI_DRV_DISP_CAST_CFG_S *pM, HI_BOOL bu2m);

HI_S32 Transfer_Intf(HI_UNF_DISP_INTF_S *pU, HI_DRV_DISP_INTF_S *pM, HI_BOOL bu2m);

HI_S32 Transfer_VbiCfg(HI_UNF_DISP_VBI_CFG_S *pU, HI_DRV_DISP_VBI_CFG_S *pM, HI_BOOL bu2m);
HI_S32 Transfer_VbiData(HI_UNF_DISP_VBI_DATA_S *pU, HI_DRV_DISP_VBI_DATA_S *pM, HI_BOOL bu2m);
HI_S32 Transfer_CgmsCfg(const HI_UNF_DISP_CGMS_CFG_S  *pU, HI_DRV_DISP_CGMSA_CFG_S *pM, HI_BOOL bu2m);

//HI_S32 Transfe_FrostMode(HI_UNF_WINDOW_FROST_E *pU, HI_DRV_WINDOW_FROST_E *pM, HI_BOOL bu2m);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

