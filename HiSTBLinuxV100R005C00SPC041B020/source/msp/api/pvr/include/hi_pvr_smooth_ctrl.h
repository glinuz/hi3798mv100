/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_smooth_ctrl.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/20
  Description   :
  History       :
  1.Date        : 2013/05/20
    Author      : l00165842
                  z00111416
    Modification: Created file

******************************************************************************/

#ifndef __HI_PVR_SMOOTH_CTRL_H__
#define __HI_PVR_SMOOTH_CTRL_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32  HI_PVR_SmoothCheckStatus(HI_PVR_SMOOTH_PARA_S *pFBAttr,HI_UNF_PVR_PLAY_SPEED_E enCurSpeed);
HI_S32  HI_PVR_SmoothEntry(HI_PVR_SMOOTH_PARA_S *pFBAttr,HI_UNF_PVR_PLAY_SPEED_E enCurSpeed);
HI_VOID HI_PVR_SmoothExit( HI_PVR_SMOOTH_PARA_S *pFBAttr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif /* End of #ifdef __HI_PVR_H__ */

