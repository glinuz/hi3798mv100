/******************************************************************************

  Copyright (C), 2011-2021, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : tee_drv_advca_ioctl.h
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/

#ifndef __TEE_DRV_ADVCA_IOCTL_H__
#define __TEE_DRV_ADVCA_IOCTL_H__

#include "hi_type.h"

#define TEE_ADVCA_IOC_NA	    0U
#define TEE_ADVCA_IOC_W	    1U
#define TEE_ADVCA_IOC_R	    2U
#define TEE_ADVCA_IOC_RW	    3U

#define TEE_ADVCA_IOC(dir,type,nr,size) (((dir) << 30)|((size) << 16)|((type) << 8)|((nr) << 0))

#define TEE_ADVCA_IO(type,nr)           TEE_ADVCA_IOC(TEE_ADVCA_IOC_R,(type),(nr), 0)
#define TEE_ADVCA_IOR(type,nr,size)	    TEE_ADVCA_IOC(TEE_ADVCA_IOC_R,(type),(nr), sizeof(size))
#define TEE_ADVCA_IOW(type,nr,size)	    TEE_ADVCA_IOC(TEE_ADVCA_IOC_W,(type),(nr), sizeof(size))
#define TEE_ADVCA_IOWR(type,nr,size)	TEE_ADVCA_IOC(TEE_ADVCA_IOC_RW,(type),(nr),sizeof(size))

#define HI_ID_ADVCA 0x5D

HI_S32 TEE_ADVCA_Ioctl(HI_U32 cmd, HI_VOID *argp);


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct
{
    HI_TEE_ADVCA_OTP_FUSE_E enOtpFuseId;
    HI_TEE_ADVCA_OTP_ATTR_S stOtpAttr;
}TEE_ADVCA_OTP_ATTR_S;

typedef struct
{
    HI_TEE_ADVCA_KEYLADDER_TYPE_E enKeyLadderType;
    HI_TEE_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;
}TEE_ADVCA_KEYLADDER_ATTR_S;

typedef struct
{
    HI_TEE_ADVCA_VENDORID_E enVendorID;
    HI_TEE_ADVCA_CA_VENDOR_OPT_S stVendorOpt;
}TEE_ADVCA_VENDOR_OPT_S;

typedef struct
{
    HI_U32 u32MSBID;
    HI_U32 u32LSBID;
}TEE_ADVCA_DCAS_CHIPID_S;

typedef struct hiCA_DCAS_PARAM_S
{
    HI_U32                       level;
    HI_U32                       pDin[4];
    HI_U32                       pDout[4];
} TEE_ADVCA_DCAS_SESSIONKEY_S;

typedef struct
{
    HI_TEE_ADVCA_KEYLADDER_LEV_E ladder;
    HI_U32 u32KeyLen;
    HI_U32 pDin[8];                     /* 32bytes for gdrm */
} TEE_ADVCA_CRYPTPM_S;

#define    CMD_ADVCA_INIT                    TEE_ADVCA_IO(HI_ID_ADVCA,   0x1)
#define    CMD_ADVCA_DEINIT                  TEE_ADVCA_IO(HI_ID_ADVCA,   0x2)
#define    CMD_ADVCA_SET_OTP_FUSE            TEE_ADVCA_IOW(HI_ID_ADVCA,  0x3, TEE_ADVCA_OTP_ATTR_S)
#define    CMD_ADVCA_GET_OTP_FUSE            TEE_ADVCA_IOWR(HI_ID_ADVCA,  0x4, TEE_ADVCA_OTP_ATTR_S)
#define    CMD_ADVCA_SET_KEYLADDER_ATTR      TEE_ADVCA_IOWR(HI_ID_ADVCA, 0x5, TEE_ADVCA_KEYLADDER_ATTR_S)
#define    CMD_ADVCA_CA_VENDOR_OPT           TEE_ADVCA_IOWR(HI_ID_ADVCA, 0x6, TEE_ADVCA_VENDOR_OPT_S)
#define    CMD_ADVCA_DCAS_CLOSE              TEE_ADVCA_IO(HI_ID_ADVCA, 0x7)
#define    CMD_ADVCA_DCAS_OPEN               TEE_ADVCA_IOWR(HI_ID_ADVCA, 0x8, HI_TEE_ADVCA_ALG_TYPE_E)
#define    CMD_ADVCA_DCAS_GET_CHIPID         TEE_ADVCA_IOWR(HI_ID_ADVCA, 0x9, TEE_ADVCA_DCAS_CHIPID_S)
#define    CMD_ADVCA_DCAS_SET_SESSIONKEY     TEE_ADVCA_IOWR(HI_ID_ADVCA, 0x10, TEE_ADVCA_DCAS_SESSIONKEY_S)
#define    CMD_ADVCA_MISC_SET_SESSIONKEY     TEE_ADVCA_IOW(HI_ID_ADVCA,  0x11, TEE_ADVCA_CRYPTPM_S)
#define    CMD_ADVCA_MISC_SET_ALG            TEE_ADVCA_IOW(HI_ID_ADVCA,  0x12, HI_TEE_ADVCA_ALG_TYPE_E)
#define    CMD_ADVCA_MISC_SET_DSCMODE        TEE_ADVCA_IOW(HI_ID_ADVCA,  0x13, HI_TEE_ADVCA_SP_DSC_MODE_E)
#define    CMD_ADVCA_MISC_GET_LEVEL          TEE_ADVCA_IOR(HI_ID_ADVCA,  0x14, HI_TEE_ADVCA_KEYLADDER_LEV_E)
#define    CMD_ADVCA_SP_SET_SESSIONKEY       TEE_ADVCA_IOW(HI_ID_ADVCA,  0x15, TEE_ADVCA_CRYPTPM_S)
#define    CMD_ADVCA_SP_SET_ALG              TEE_ADVCA_IOW(HI_ID_ADVCA,  0x16, HI_TEE_ADVCA_ALG_TYPE_E)
#define    CMD_ADVCA_SP_SET_DSCMODE          TEE_ADVCA_IOW(HI_ID_ADVCA,  0x17, HI_TEE_ADVCA_SP_DSC_MODE_E)
#define    CMD_ADVCA_SP_GET_LEVEL            TEE_ADVCA_IOR(HI_ID_ADVCA,  0x18, HI_TEE_ADVCA_KEYLADDER_LEV_E)
#define    CMD_ADVCA_MISC_OPEN               TEE_ADVCA_IO(HI_ID_ADVCA,   0x19)
#define    CMD_ADVCA_MISC_CLOSE              TEE_ADVCA_IO(HI_ID_ADVCA,   0x1A)
#define    CMD_ADVCA_SP_OPEN                 TEE_ADVCA_IO(HI_ID_ADVCA,   0x1B)
#define    CMD_ADVCA_SP_CLOSE                TEE_ADVCA_IO(HI_ID_ADVCA,   0x1C)
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of #ifndef __TEE_DRV_CIPHER_IOCTL_H__*/
