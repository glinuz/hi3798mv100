#ifndef __TEE_HDMI_H__
#define __TEE_HDMI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif 

#include "hi_type.h"
#include "hi_tee_hdmi.h"

/** 
\brief hdcp init, load 8051 program.\n
\param[in]  enHdmi  hdmi channel id. 
\retval HI_SUCCESS   success.  
*/
HI_S32 TEE_MPI_HDMI_HDCP22_Init(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdcp1.4 init.\n
\param[in]  enHdmi  hdmi channel id. 
\retval HI_SUCCESS   success.  
*/
HI_S32 TEE_MPI_HDMI_HDCP14_Init(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdmi tee status\n
\param[in]  enHdmi  hdmi channel id. 
\retval HI_SUCCESS  success.  
*/
HI_S32 TEE_MPI_HDMI_TEEProc(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdmi hdcp caps set \n
\param[in]  enHdmi       hdmi channel id. 
\param[in]  pstHdcpCaps  hdcp caps
\retval HI_SUCCESS   success.  
*/
HI_S32 TEE_MPI_HDMI_SetHDCPCaps(HI_TEE_HDMI_ID_E enHdmi, HI_TEE_HDMI_HDCP_CAPS_S *pstHdcpCaps);


/** 
\brief hdmi IOCTRL
\param[in]  enHdmi       hdmi channel id. 
\param[in]  pvData      Ioctrl data
\param[in]  u32Size      data size
\retval HI_SUCCESS   success.  
*/
HI_S32 TEE_MPI_HDMI_IOCTRL(HI_TEE_HDMI_ID_E enHdmi, HI_VOID *pvData,HI_U32 u32Size);
/** 
\brief hdmi init \n
\param[in]  enHdmi       hdmi channel id. 
\retval HI_SUCCESS       success.
*/
HI_S32 TEE_MPI_HDMI_Init(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdmi deinit \n
\param[in]  enHdmi       hdmi channel id. 
\retval HI_SUCCESS       success. 
*/
HI_S32 TEE_MPI_HDMI_DeInit(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdmi start \n
\param[in]  enHdmi       hdmi channel id. 
\retval HI_SUCCESS       success. 
*/
HI_S32 TEE_MPI_HDMI_Start(HI_TEE_HDMI_ID_E enHdmi);

/** 
\brief hdmi stop \n
\param[in]  enHdmi       hdmi channel id. 
\retval HI_SUCCESS       success.
*/
HI_S32 TEE_MPI_HDMI_Stop(HI_TEE_HDMI_ID_E enHdmi);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif






