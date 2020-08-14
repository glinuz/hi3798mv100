/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tee_hdmi_hal_hdcp14.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/08/13
  Description   :
  History       :
  Date          : 2016/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __TEE_HDMI_HAL_HDCP14_H__
#define __TEE_HDMI_HAL_HDCP14_H__

#define HDCP14_KSV_SIZE_5BYTES          5
#define HDCP14_Ri_SIZE_2BYTES           2
#define HDCP14_Vi_SIZE_20BYTES          20


HI_S32 HAL_HDMI_Hdcp14R0Cmp(HI_U8 *pu8Data);

HI_S32 HAL_HDMI_Hdcp14BksvSet(HI_U8 *pu8Data);

HI_S32 HAL_HDMI_Hdcp14BksvListSet(HI_U8 *pu8Data,HI_U32 u32Size);

HI_S32 HAL_HDMI_Hdcp14EncrpytionEanble(HI_BOOL bEnable);

HI_S32 HAL_HDMI_Hdcp14RepeaterEnable(HI_BOOL bEnable);
 
HI_S32 HAL_HDMI_Hdcp14ViCmp(HI_U8 *pu8Data);



#endif /* __TEE_HDMI_HAL_HDCP14_H__ */


