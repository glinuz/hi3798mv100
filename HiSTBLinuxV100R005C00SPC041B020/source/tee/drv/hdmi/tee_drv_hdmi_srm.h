/******************************************************************************
 Copyright (C), 2015-2025, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : tee_drv_hdmi_srm.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2015/12/19
Last Modified :
Description   : HDMI SRM H FILE
Function List :
History       :
******************************************************************************/
#ifndef __TEE_DRV_HDMI_SRM_H__
#define __TEE_DRV_HDMI_SRM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define __HDMI_COMPATIBILITY_TEST_DEMO__

#define HDMI_SRM_MAX_DEVID      128     /**<Max number of Device IDs*/

/******************************************************************************/
/* Structure data                                                             */
/******************************************************************************/
typedef struct
{
    HI_U8  u8SrmId;
    HI_U8  u8Hdcp2xIndicator;
    HI_U8  u8SrmGenerationNum;
    HI_U16 u16SrmVerion;
    HI_U32 u32Length;

}HDMI_SRM_HEADER_S;


typedef struct
{
    HI_U8  u8DevId[5];

}HDMI_SRM_DEVID_S;

typedef struct
{
    HI_U8  u8Msg[330];

}HDMI_SRM_MSG_S;


typedef struct
{
    HI_U8  u8DDCLLCSignature[384];

}HDMI_SRM_SIGN_S;

typedef enum 
{
    HDMI_SRM_UNKNOW,
    HDMI_SRM_RAW_DATA_INVALID,
    HDMI_SRM_RAW_PARSE_SUCCESS,
    HDMI_SRM_RSA_VER_FAILED,
    HDMI_SRM_RSA_VER_SUCCESS,
    HDMI_SRM_BUTT
}HDMI_SRM_STATUS_E;

typedef struct
{
    HDMI_SRM_HEADER_S stSrmHeader;
    HI_U16            u16DevNum;
    HDMI_SRM_DEVID_S  stDevId[HDMI_SRM_MAX_DEVID];
    HDMI_SRM_SIGN_S   stSignature;
    HI_U16            u16MsgLen;
    HDMI_SRM_MSG_S    stMsg;
    HDMI_SRM_STATUS_E enStatus;
}HDMI_SRM_CTX_S;

/******************************************************************************/
/* Pubic interface                                                            */
/******************************************************************************/
HI_VOID HDMI_SRM_Init(void);

HI_S32 HDMI_SRM_Check(HI_U8 *pSrm, HI_U32 u32Len, HDMI_SRM_CTX_S *pstSrmCtx);

HI_S32 HDMI_SRM_IsInRevoInfor(HDMI_SRM_CTX_S *pstSrmCtx, HI_U8 *pu8RecvID, HI_U32 u32Num);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TEE_DRV_HDMI_SRM_H__ */
/*------------------------------------END-------------------------------------*/

