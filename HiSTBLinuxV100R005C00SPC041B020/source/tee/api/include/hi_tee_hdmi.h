/******************************************************************************

  Copyright (C), 2001-2025, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_tee_hdmi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016-07-12
  Description   :
  History       :
  1.Date        : 2016-07-12
    Author      : q00352704
    Modification: Created file

*******************************************************************************/
#ifndef __HI_TEE_HDMI_H__
#define __HI_TEE_HDMI_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/*************************** Structure Definition ****************************/
/** \addtogroup      HDMI */
/** @{ */  /** <!-- [HDMI] */


/**HDMI interface ID *//**CNcomment:HDMI接口ID  */
typedef enum hiTEE_HDMI_ID_E
{
    HI_TEE_HDMI_ID_0 = 0,    /**<HDMI ID 0 */
    HI_TEE_HDMI_ID_BUTT
} HI_TEE_HDMI_ID_E; 

/**HDMI HDCP status *//**CNcomment:HDMI HDCP状态 */
typedef enum hiTEE_HDMI_HDCP_STATUS_E
{
    HI_TEE_HDMI_HDCP_NONE = 0,  	 /**<no hdcp is working     */
    HI_TEE_HDMI_HDCP_14_SUCC,        /**<hdcp1.4 auth success 	*/
	HI_TEE_HDMI_HDCP_14_FAIL,        /**<hdcp1.4 auth fail 	 	*/
    HI_TEE_HDMI_HDCP_22_SUCC,        /**<hdcp2.2 auth success	*/
	HI_TEE_HDMI_HDCP_22_FAIL,        /**<hdcp2.2 auth fail		*/
    HI_TEE_HDMI_HDCP_BUTT
} HI_TEE_HDMI_HDCP_STATUS_E;

/**HDMI HDCP monitoring level *//**CNcomment:HDMI HDCP监控等级*/
typedef enum hiTEE_HDMI_HDCP_LEVLEL_E
{
    HI_TEE_HDMI_HDCP_LEVLE_NONE = 0,        /**<no hdcp monitoring */
    HI_TEE_HDMI_HDCP_LEVLE_1,               /**<hdcp monitoring level 1 (hdcp1.4 or hdcp2.2) */
    HI_TEE_HDMI_HDCP_LEVLE_2,               /**<hdcp monitoring level 2 (only hdcp2.2) */
    HI_TEE_HDMI_HDCP_LEVLE_BUTT
} HI_TEE_HDMI_HDCP_LEVLEL_E; 

/**HDMI HDCP capability *//**CNcomment:HDMI HDCP 能力集*/
typedef struct hiTEE_HDMI_HDCP_CAPS_S
{
    HI_BOOL bHdcp14Support;     /**<support HDCP1.4 */
    HI_BOOL bHdcp22Support;     /**<support HDCP2.2 */
} HI_TEE_HDMI_HDCP_CAPS_S;

/**
\brief Create handle for setting hdcp strategy \n
\param[out]  *phHDMI  DRM HDMI control handle
\param[in]  enHdmi   hdmi channel id.
\retval HI_SUCCESS   success.
\retval HI_FAILURE   failure.
*/
HI_S32 HI_TEE_HDMI_CreateHandle(HI_HANDLE* phHDMI, HI_TEE_HDMI_ID_E enHdmi);

/**
\brief Destroy handle for setting hdcp strategy\n
\param[in]  phHDMI DRM HDMI control handle
\retval HI_SUCCESS   success.
*/
HI_S32 HI_TEE_HDMI_DestoryHandle(HI_HANDLE hHDMI);

/** 
\brief get hdcp status \n
\param[in]  hHDMI  		    HDMI control handle
\param[out]  penHdcpStatus   pointer to hdcp status
\retval HI_SUCCESS          success.
\retval HI_FAILURE          failed.
*/
HI_S32 HI_TEE_HDMI_GetHDCPStatus(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_STATUS_E *penHdcpStatus);

/** 
\brief set hdcp strategy \n
\param[in]  hHDMI    HDMI control handle
\param[in]  enHdcp   set the monitoring level
\retval HI_SUCCESS   success.  
\retval HI_FAILURE   failed.
*/
HI_S32 HI_TEE_HDMI_SetHDCPStrategy(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_LEVLEL_E enHdcp);

/**
\brief get hdcp strategy from drm, it is called by drm,it can be call more time\n
\param[in]  hHDMI    HDMI control handle
\param[out] *penHdcp  the monitoring level
\retval HI_SUCCESS   success.
*/
HI_S32 HI_TEE_HDMI_GetHDCPStrategy(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_LEVLEL_E *penHdcp);

/** 
\brief get HDCP capability \n
\param[in]  hHDMI         HDMI control handle
\param[out]  pstHdcpCaps   hdcp caps
                          bHdcp14Support: support HDCP1.4 or not
                          bHdcp22Support: support HDCP2.2 or not
\retval HI_SUCCESS        success. 
\retval HI_FAILURE        failed.
*/
HI_S32 HI_TEE_HDMI_GetHDCPCaps(HI_HANDLE hHDMI, HI_TEE_HDMI_HDCP_CAPS_S *pstHdcpCaps);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_TEE_HDMI_H__ */

