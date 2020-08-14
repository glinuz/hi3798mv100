#ifndef __AUTHDEFINE_H__
#define __AUTHDEFINE_H__

#include "hi_type.h"

#define HASH256_LEN				(0x20)			// 256bit, 32Byte
#define CAIMG_HEAD_BLOCK_SIZE   (0x2000)       // 8K
#define RSA_2048_LEN            (0x100)         //256
#define SIGN_BLOCK_SIZE         (0x200000)      //2MB
#define CAIMGHEAD_MAGICNUMBER   "Hisilicon_ADVCA_ImgHead_MagicNum"

#define SAMPLE_PRINTF       printf
#define AUTH_OK                 0
#define AUTH_FAIL               -1

typedef enum
{
    AUTH_SHA1,
    AUTH_SHA2,
    AUTH_BUTT
}AUTHALG_SHA_E;

typedef struct
{
    unsigned char u8MagicNumber[32];             //Magic Number: "Hisilicon_ADVCA_ImgHead_MagicNum"
    unsigned char u8Version[8];                  //version: "V000 0003" 
    unsigned int  u32CreateDay;                  //yyyymmdd
    unsigned int  u32CreateTime;                 //hhmmss
    unsigned int  u32HeadLength;                 //The following data size
    unsigned int  u32SignedDataLength;           //signed data length
    unsigned int  u32IsYaffsImage;               //Yaffsr image need to specail read-out, No use
    unsigned int  u32IsConfigNandBlock;          //Yaffsr image need to specail read-out, No use
    unsigned int  u32NandBlockType;              //Yaffsr image need to specail read-out, No use
    unsigned int  u32IsNeedEncrypt;              //if "1", code need to be encrypted. 
    unsigned int  u32IsEncrypted;                //if "1", code has encrypted. 
    unsigned int  u32HashType;                   //if "0", u8Sha save sha1 of code, if "1",  u8Sha save sha256 of code
    unsigned char u8Sha[32];                     //SHA value
    unsigned int  u32SignatureLen;               //Actural Signature length
    unsigned char u8Signature[256];              //Max length:0x100
    unsigned char OrignalImagePath[256];         //Max length:
    unsigned char RSAPrivateKeyPath[256];        //Max length:0x100
    unsigned int  u32CurrentsectionID;            //begin with 0
	unsigned int  u32SectionSize;                 //section size 
    unsigned int  u32TotalsectionID;              //Max section ID > 1 
    unsigned int  CRC32;                         //CRC32 value
} HI_CASignImageTail_S;

typedef struct
{
    unsigned char *pImageData;
    unsigned int ImageDataLength;
    HI_CASignImageTail_S *pCASignTail;
} HI_CA_NEW_SignParam_S;


/** 
\brief verify partition  CNcomment: 分区校验 CNend
\param[in]  verifyPartition the name of partition need to verify.    CNcomment: 需要校验的分区名 CNend
\param[in]  signPartition  the name of partition which store the signautre data.  CNcomment: 保存签名数据的分区名 CNend
\param[out]  offset  the offset of the singature data in partition which store the signature data .  CNcomment:签名数据分区中签名数据的偏移位置 CNend
*/
HI_S32 CA_common_authPartition(HI_CHAR *verifyPartition, HI_CHAR *signPartition, HI_U32 offset);
/** 
\brief Init software Sha1 or Sha2 calculate  CNcomment: 软hash计算 CNend
\param[in]  HashType the type of HASH.    CNcomment: hash计算类型 CNend
\param[out]  handle  the handle of hash.  CNcomment:计算hash的句柄 CNend
*/
HI_S32 CA_common_InitHASH(HI_U8 HashType, HI_HANDLE **handle);  

/** 
\brief update software Sha1 or Sha2 calculate process CNcomment: 软hash计算 CNend
\param[in]  handle  the handle of hash. CNcomment:计算hash的句柄 CNend
\param[in]  StartAddr the point of input data.    CNcomment:输入数据地址 CNend
\param[in]  length  the length of input data. CNcomment:输入数据长度 CNend
\param[out]  HashType  the type of hash. CNcomment:hash计算类型 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\return ::HI_FAILURE Failure.   CNcomment:HI_FAILURE 失败 CNend
*/
HI_VOID CA_common_UpdateHASH(HI_HANDLE *handle, HI_U8 *StartAddr, HI_U32 length);  

/** 
\brief Finish software Sha1 or Sha2 calculate CNcomment: 软hash计算 CNend
\param[out]  handle  the handle of hash. CNcomment:计算hash的句柄 CNend
\param[in]  StartAddr the point of input data.    CNcomment:输入数据地址 CNend
\param[in]  length  the length of input data. CNcomment:输入数据长度 CNend
\param[in]  HashType  the type of hash. CNcomment:hash计算类型 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\return ::HI_FAILURE Failure.   CNcomment:HI_FAILURE 失败 CNend
*/
HI_VOID CA_common_FinishHASH(HI_HANDLE *handle, HI_U8 *Hash); 

/** 
\brief Authentic data CNcomment: 校验结果 CNend
\param[in]  pCASignTail the point of signature structure.    CNcomment:签名数据结构体地址 CNend
\param[in]  length  the length of input data. CNcomment: 输入数据长度 CNend
\param[in]  Hash  the result of hash. CNcomment: hash计算结果 CNend
\retval ::HI_SUCCESS Success CNcomment:HI_SUCCESS 成功 CNend
\return ::HI_FAILURE Failure.   CNcomment:HI_FAILURE 失败 CNend
*/
HI_S32 CA_common_DataAuthenticateEx(HI_U8 HashType, HI_U8 *Hash, HI_CASignImageTail_S *pCASignTail);

/*
#define CA_printf(p) \
    do { \
        FILE *file = fopen("/dev/ttyAMA0", "w"); \
        fprintf(file, "p");\
        fclose(file);\
    } while(0)  */


//#define CA_printf(p) 	printf(p)
#define CA_CheckPointer(p) \
    do {  \
        if (NULL == p)\
        {\
            printf("pointer parameter is NULL.\n"); \
            return -1; \
        } \
    } while (0)

#define CA_ASSERT(api, ret) \
    do{ \
        HI_S32 l_ret = api; \
        if (l_ret != HI_SUCCESS) \
        { \
            printf("run failed"); \
            return -1;\
        } \
        else\
        {\
        /*printf("sample %s: run %s ok.\n", __FUNCTION__, #api);}*/   \
        }\
        ret = l_ret;\
    } while(0)
    
#endif /*__AUTHDEFINE_H__*/

