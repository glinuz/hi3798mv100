/******************************************************************************

Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_verify_def.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __VMX_VERIFY_DEFH__
#define __VMX_VERIFY_DEFH__

#include "hi_type.h"
//#include "hi_debug.h"

#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#endif

#ifndef HI_ADVCA_FUNCTION_RELEASE
#define AUTHCOMMONDTEST
#endif

#define NAND_PAGE_SIZE          (0x2000)
#define RSA_2048_LEN            (0x100)
#define SIGNATURE_LEN           (0x100)                             
#define KEYAREALEN              (0x200)
#define CMD_LEN                 (0X200)
#if defined (CHIP_TYPE_hi3716mv310)
#define IMG_VERIFY_ADDRESS      (0X84000000)
#else
#define IMG_VERIFY_ADDRESS      (0X04000000)
#endif


#define CA_IMAGE_HEADER_AREA_LEN (0x2000)   /*8K*/
#define IMG_MAX_BLOCK_NUM       (5)

typedef struct hi_CAImgHead_S
{   
    HI_U8  u8MagicNumber[32];                    //Magic Number: "Hisilicon_ADVCA_ImgHead_MagicNum"
    HI_U8  u8HeaderVersion[8];                         //version: "V000 0003" 
    HI_U32 u32TotalLen;                          //Total length
    HI_U32 u32CodeOffset;                        //Image offset
    HI_U32 u32SignedImageLen;                    //Signed Image file size
    HI_U32 u32SignatureOffset;                   //Signed Image offset
    HI_U32 u32SignatureLen;                      //Signature length
    HI_U32 u32BlockNum;                          //Image block number
    HI_U32 u32BlockOffset[IMG_MAX_BLOCK_NUM];    //Each Block offset
    HI_U32 u32BlockLength[IMG_MAX_BLOCK_NUM];    //Each Block length
    HI_U32 u32SoftwareVerion;                    //Software version
    HI_U32 Reserverd[31];
    HI_U32 u32CRC32;                             //CRC32 value
} HI_CAImgHead_S;

typedef enum
{
    AUTH_SHA1,
    AUTH_SHA2,
    AUTH_BUTT
}AUTHALG_E;


typedef enum hiCA_KEY_GROUP_E
{
    HI_CA_KEY_GROUP_1 = 0,
    HI_CA_KEY_GROUP_2,
    HI_CA_KEY_GROUP_MAX,
}HI_CA_KEY_GROUP_E;

#endif/*__VMX_VERIFY_DEFH__*/
