/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_cipher_define.h
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef __DRV_CIPHER_DEFINE_H__
#define __DRV_CIPHER_DEFINE_H__

#include "hi_type.h"
#include "hi_unf_cipher.h"

#define __CIPHER_DEBUG_VERSION__ /* when release cipher SW, mask it */

#if defined (CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
#define RSA_HARDWARE_SUPPORT
#endif

#define HI_PRINT_HEX(name, str, len) \
{\
    HI_U32 _i = 0;\
    HI_U32 _val; \
    HI_U8 *_str = (HI_U8*)str;\
    printf("[%s]:\n", name);\
    for ( _i = 0 ; _i < (len); _i+=4 )\
    {\
        _val  = *((_str)+_i+3);\
        _val |= *((_str)+_i+2) << 8;\
        _val |= *((_str)+_i+1) << 16;\
        _val |= *((_str)+_i+0) << 24;\
        if( (_i % 16 == 0) && (_i != 0)) printf("\n");\
        printf("%x ", _val);\
    }\
    printf("\n");\
}

#define CIPHER_WAIT_IDLE_TIMES 1000
#define HASH_MAX_DURATION      60000

//#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,14)
#define  SA_INTERRUPT     IRQF_DISABLED
#define  SA_SHIRQ         IRQF_SHARED
//#endif


#define TASK_QUEUE_IDLE 			(0)
#define TASK_QUEUE_BUSY 			(1)

#define MEM_LENGTH_ERROR    		(1)
#define AHB_BUS_ERROR       		(2)

#define CIPHER_MAX_LIST_NUM  		(128)

#define CI_IV_SIZE       			(16)  /* max 128bit */
#define CI_KEY_SIZE      			(32)  /* max 256bit */


#define CI_BUF_LIST_FLAG_IVSET_BIT  (21)
#define CI_BUF_LIST_FLAG_EOL_BIT    (22)
#define CIPHER_MAX_RSA_KEY_LEN 512

typedef struct hiCI_BUF_LIST_ENTRY_S
{
    HI_U32  u32DataAddr;    /* Phy Addr */
    HI_U32  u32Flags;       /* bit21: iv_set_flag, bit22: end_of_list_flag */
    HI_U32  U32DataLen;     /* bit0~bit19, total 20 bit */
    HI_U32  u32IVStartAddr; /* if iv_set_flag == 1, this is the IV addr */
} CI_BUF_LIST_ENTRY_S;

/*************************** Structure Definition ****************************/
/** \addtogroup      CIPHER */
/** @{ */  /** <!-- [CIPHER] */

/** min length of encrypt, unit: byte */
/** CNcomment: ??С?ӽ??ܳ??ȣ???λbyte */
#define HI_UNF_CIPHER_MIN_CRYPT_LEN      8

/** max length of encrypt, unit: byte */
/** CNcomment: ?????ӽ??ܳ??ȣ???λbyte */
#define HI_UNF_CIPHER_MAX_CRYPT_LEN      0xfffff

/** Cipher data */
/** CNcomment:?ӽ??????? */
typedef struct hiHI_UNF_CIPHER_DATA_S
{
    HI_U32 u32SrcPhyAddr;     /**< phy address of the original data */   /**< CNcomment:Դ??????????ַ */
    HI_U32 u32DestPhyAddr;    /**< phy address of the purpose data */    /**< CNcomment:Ŀ????????????ַ */
    HI_U32 u32ByteLength;     /**< cigher data length*/                 /**< CNcomment:?ӽ??????ݳ??? */
} HI_UNF_CIPHER_DATA_S;



typedef enum
{
    HI_CIPHER_HMAC_KEY_FROM_CA  = 0,
    HI_CIPHER_HMAC_KEY_FROM_CPU = 1,
}CIPHER_HMAC_KEY_FROM_E;

typedef struct hiCIPHER_HASH_DATA_S
{
    HI_HANDLE hHandle;
    HI_UNF_CIPHER_HASH_TYPE_E enShaType;
    HI_U32 u32TotalDataLen;
    HI_U8  *pu8InputData;
    HI_U32 u32InputDataLen;
    /* max padding data (64bytes) | fix data length(8bytes) */
    HI_U8  u8Padding[64+8];
    HI_U32 u32PaddingLen;
    HI_U8  u8HMACKey[16];
    CIPHER_HMAC_KEY_FROM_E enHMACKeyFrom;
    HI_U8  *pu8Output;
}CIPHER_HASH_DATA_S;

/***/
typedef struct hiCIPHER_DATA_INFO_S
{
    HI_U32  u32src;          /**< */
    HI_U32  u32dest;         /**< */
    HI_U32  u32length;       /**< */
    HI_BOOL bDecrypt;        /**< */
    HI_U32  u32DataPkg[4];   /**< */
}HI_DRV_CIPHER_DATA_INFO_S;

typedef struct hiCIPHER_TASK_S
{
    HI_DRV_CIPHER_DATA_INFO_S         	stData2Process;
    HI_U32                      u32CallBackArg;
}HI_DRV_CIPHER_TASK_S;

typedef struct
{    
    HI_U8  *pu8Input;    
    HI_U8  *pu8Output;   
    HI_U32  u32DataLen;    
    HI_U8  *pu8N;    
    HI_U8  *pu8K;    
    HI_U16 u16NLen;    
    HI_U16 u16KLen;
}CIPHER_RSA_DATA_S;

/***************************** Macro Definition ******************************/
/** \addtogroup      H_3_5 */
/** @{ */  /** <!-- ?E[ECS error code] */

#define  HI_ERR_CIPHER_NOT_INIT                     (HI_S32)(0x804D0001)
#define  HI_ERR_CIPHER_INVALID_HANDLE               (HI_S32)(0x804D0002)
#define  HI_ERR_CIPHER_INVALID_POINT                (HI_S32)(0x804D0003)
#define  HI_ERR_CIPHER_INVALID_PARA                 (HI_S32)(0x804D0004)
#define  HI_ERR_CIPHER_FAILED_INIT                  (HI_S32)(0x804D0005)
#define  HI_ERR_CIPHER_FAILED_GETHANDLE             (HI_S32)(0x804D0006)
#define  HI_ERR_CIPHER_FAILED_RELEASEHANDLE         (HI_S32)(0x804D0007)
#define  HI_ERR_CIPHER_FAILED_CONFIGAES             (HI_S32)(0x804D0008)
#define  HI_ERR_CIPHER_FAILED_CONFIGDES             (HI_S32)(0x804D0009)
#define  HI_ERR_CIPHER_FAILED_ENCRYPT               (HI_S32)(0x804D000A)
#define  HI_ERR_CIPHER_FAILED_DECRYPT               (HI_S32)(0x804D000B)
#define  HI_ERR_CIPHER_BUSY                         (HI_S32)(0x804D000C)

/** @} */  /*! <!-- Macro Definition end */

/* device parameter */
typedef struct  hiCIPHER_DATA_S
{
    HI_HANDLE CIHandle;
    HI_U32 ScrPhyAddr;
    HI_U32 DestPhyAddr;
    HI_U32 ByteLength;

}CIPHER_DATA_S;

typedef struct hiCIPHER_Config_CTRL
{
    HI_HANDLE CIHandle;
    HI_UNF_CIPHER_CTRL_S CIpstCtrl;

}CIPHER_Config_CTRL;


typedef struct hiCIPHER_PROC_ITEM_S
{
    HI_U32    u32Resv;

}CIPHER_PROC_ITEM_S;
#endif


