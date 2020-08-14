/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cx_verify_api.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2016-01-15
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "run.h"
#include "app.h"
#else
#include "exports.h"
#endif
#include "cx_debug.h"
#include "cx_adp.h"
#include "cx_flash.h"
#include "cx_verify_api.h"


HI_U8 CX_CAIMGHEAD_MAGICNUMBER[32] = {0};

extern unsigned long DVB_CRC32(unsigned char *buf, int length);


static HI_VOID printf_hex(HI_U8 *pu8Buf, HI_U32 u32Length)
{
    HI_U32 i;

    for(i = 0; i < u32Length; i++)
    {
        if(i % 16 == 0 && i != 0)
        {
            HI_SIMPLEINFO_CA("\n");
        }
        HI_SIMPLEINFO_CA("0x%02X, ", pu8Buf[i]);
    }
    HI_SIMPLEINFO_CA("\n");

    return;
}

static HI_VOID GenerateMagicNum(HI_U8 u8ImgHeaderMagic[32])
{
    u8ImgHeaderMagic[0] = 'H';
    u8ImgHeaderMagic[1] = 'i';
    u8ImgHeaderMagic[2] = 's';
    u8ImgHeaderMagic[3] = 'i';
    u8ImgHeaderMagic[4] = 'l';
    u8ImgHeaderMagic[5] = 'i';
    u8ImgHeaderMagic[6] = 'c';
    u8ImgHeaderMagic[7] = 'o';
    u8ImgHeaderMagic[8] = 'n';
    u8ImgHeaderMagic[9] = '_';
    u8ImgHeaderMagic[10] = 'A';
    u8ImgHeaderMagic[11] = 'D';
    u8ImgHeaderMagic[12] = 'V';
    u8ImgHeaderMagic[13] = 'C';
    u8ImgHeaderMagic[14] = 'A';
    u8ImgHeaderMagic[15] = '_';
    u8ImgHeaderMagic[16] = 'I';
    u8ImgHeaderMagic[17] = 'm';
    u8ImgHeaderMagic[18] = 'g';
    u8ImgHeaderMagic[19] = 'H';
    u8ImgHeaderMagic[20] = 'e';
    u8ImgHeaderMagic[21] = 'a';
    u8ImgHeaderMagic[22] = 'd';
    u8ImgHeaderMagic[23] = '_';
    u8ImgHeaderMagic[24] = 'M';
    u8ImgHeaderMagic[25] = 'a';
    u8ImgHeaderMagic[26] = 'g';
    u8ImgHeaderMagic[27] = 'i';
    u8ImgHeaderMagic[28] = 'c';
    u8ImgHeaderMagic[29] = 'N';
    u8ImgHeaderMagic[30] = 'u';
    u8ImgHeaderMagic[31] = 'm';

    return;
}

HI_S32 HI_CX_VerifyInit(HI_VOID)
{
    /*Generate the magic number*/
    GenerateMagicNum(CX_CAIMGHEAD_MAGICNUMBER);

    return HI_SUCCESS;
}

#if (HI_ADVCA_SIGN_VERSION == 0003)
static HI_S32 CX_VerifyImageHeader(HI_CA_AuthInfo_S *pstAuthInfo, HI_U8 *pu8ImgHeaderBuf, HI_U32 u32ImageMaxLen, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U8 u8Signature[SIGNATURE_LEN] = {0};
    HI_U8* pImageSignature = NULL;
    HI_U32 u32CRC32Value = 0;
    HI_CAImgHead_S* pstImgInfo;

    CA_CheckPointer(pstAuthInfo);
    CA_CheckPointer(pu8ImgHeaderBuf);

    pstImgInfo = (HI_CAImgHead_S*)pu8ImgHeaderBuf;

    ret = HI_CX_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, pu8ImgHeaderBuf,CA_IMAGE_HEADER_AREA_LEN,pu8ImgHeaderBuf);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to decrypt header image\n", __FUNCTION__);
        return HI_FAILURE
    }
    
    udelay(100000);//wait fo decrypt end,this code is essential
    
    ret = HI_CX_ADP_VerifyImage((HI_U32)pu8ImgHeaderBuf,CA_IMAGE_HEADER_AREA_LEN,u8Signature, pstAuthInfo->eAuthAlg);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to verify header image\n", __FUNCTION__);
        return HI_FAILURE
    }

    if (CX_CAIMGHEAD_MAGICNUMBER[0] != 'H')
    {
        GenerateMagicNum(CX_CAIMGHEAD_MAGICNUMBER);
    }
    
    pstImgInfo = pu8ImgHeaderBuf;
    if(0 != memcmp(pstImgInfo->u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(pstImgInfo->u8MagicNumber)))
    {
        HI_INFO_CA("Magic number err, get Img information failed\n");
        return HI_FAILURE;
    }    

    HI_INFO_CA("ImgInfo.u8MagicNumber:%s\n", pstImgInfo->u8MagicNumber);
    printf_hex(pstImgInfo->u8MagicNumber, sizeof(pstImgInfo->u8MagicNumber));
    HI_INFO_CA("Magic number check success\n");

    *pIsEncrypt = HI_TRUE;

    /*Check if the total length is valid*/
    if ((pstImgInfo->u32TotalLen > u32ImageMaxLen) || (pstImgInfo->u32TotalLen < CA_IMAGE_HEADER_AREA_LEN))
    {
        HI_ERR_CA("The total length of image invalid!\n");
        return HI_FAILURE;
    }

    /*Check if the other lengthes of the header are valid*/
    if ((pstImgInfo->u32CodeOffset != CA_IMAGE_HEADER_AREA_LEN) || (pstImgInfo->u32SignedImageLen == 0) || (pstImgInfo->u32SignedImageLen > pstImgInfo->u32TotalLen) ||
            (pstImgInfo->u32SignatureOffset == 0) || (pstImgInfo->u32SignatureOffset > pstImgInfo->u32TotalLen))
    {
        HI_ERR_CA("The header data of image invalid!\n");
        return HI_FAILURE;
    }

    //Clacluate CRC32!
    u32CRC32Value = DVB_CRC32((HI_U8 *)pstImgInfo, (sizeof(HI_CAImgHead_S) - 4));
    if(u32CRC32Value != pstImgInfo->u32CRC32)
    {
        HI_ERR_CA("Fail to check CRC, CRC32Value:%x != pstImgInfo->CRC32:%x\n", u32CRC32Value, pstImgInfo->u32CRC32);
        return HI_FAILURE;
    }

    return ret;
}
#else
static HI_S32  CX_VerifyImageHeader(HI_CA_AuthInfo_S *pstAuthInfo, HI_U8 *pu8ImgHeaderBuf, HI_U32 u32ImageMaxLen, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U32 u32CRC32Value = 0;
    HI_CAImgHead_S* pstImgInfo;

    CA_CheckPointer(pstAuthInfo);
    CA_CheckPointer(pu8ImgHeaderBuf);

    if (CX_CAIMGHEAD_MAGICNUMBER[0] != 'H')
    {
        GenerateMagicNum(CX_CAIMGHEAD_MAGICNUMBER);
    }

    pstImgInfo = (HI_CAImgHead_S*)pu8ImgHeaderBuf;    
    if(!memcmp(pstImgInfo->u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(pstImgInfo->u8MagicNumber)))
    {
        HI_INFO_CA("Img has not be encrypted\n");
        *pIsEncrypt = HI_FALSE;
    }
    else
    {
        CA_ASSERT(HI_CX_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, pu8ImgHeaderBuf,CA_IMAGE_HEADER_AREA_LEN,pu8ImgHeaderBuf),ret);
        udelay(100000);//wait fo decrypt end,this code is essential
        if(0 != memcmp(pstImgInfo->u8MagicNumber,CX_CAIMGHEAD_MAGICNUMBER,sizeof(pstImgInfo->u8MagicNumber)))
        {
            HI_INFO_CA("Magic number err, get Img information failed\n");
            return HI_FAILURE;
        }
        *pIsEncrypt = HI_TRUE;
    }

    HI_INFO_CA("ImgInfo.u8MagicNumber:\n");
    printf_hex(pstImgInfo->u8MagicNumber, sizeof(pstImgInfo->u8MagicNumber));
    HI_INFO_CA("Magic number check success\n");

    /*Check if the total length is valid*/
    if ((pstImgInfo->u32TotalLen > u32ImageMaxLen) || (pstImgInfo->u32TotalLen < CA_IMAGE_HEADER_AREA_LEN))
    {
        HI_ERR_CA("The total length of image invalid!\n");
        return HI_FAILURE;
    }

    /*Check if the other lengthes of the header are valid*/
    if ((pstImgInfo->u32CodeOffset != CA_IMAGE_HEADER_AREA_LEN) || (pstImgInfo->u32SignedImageLen == 0) || (pstImgInfo->u32SignedImageLen > pstImgInfo->u32TotalLen) ||
            (pstImgInfo->u32SignatureOffset == 0) || (pstImgInfo->u32SignatureOffset > pstImgInfo->u32TotalLen))
    {
        HI_ERR_CA("The header data of image invalid!\n");
        return HI_FAILURE;
    }

    //Clacluate CRC32!
    u32CRC32Value = DVB_CRC32((HI_U8 *)pstImgInfo, (sizeof(HI_CAImgHead_S) - 4));
    if(u32CRC32Value != pstImgInfo->u32CRC32)
    {
        HI_ERR_CA("Fail to check CRC, CRC32Value:%x != pstImgInfo->CRC32:%x\n", u32CRC32Value, pstImgInfo->u32CRC32);
        return HI_FAILURE;
    }

    return ret;
}
#endif

HI_S32 HI_CX_GetFlashImgInfoByName(HI_CHAR* pPartionName, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U8 pu8ImgHeaderBuf[CA_IMAGE_HEADER_AREA_LEN] = {0};
    HI_Flash_InterInfo_S flashInfo;

    CA_CheckPointer(pPartionName);
    CA_CheckPointer(pstAuthInfo);
    
    CA_ASSERT(CA_flash_read(pPartionName, 0ULL, CA_IMAGE_HEADER_AREA_LEN,(HI_U8*)pu8ImgHeaderBuf, &flashInfo),ret);

    ret = CX_VerifyImageHeader(pstAuthInfo, pu8ImgHeaderBuf, flashInfo.PartSize, pIsEncrypt);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to verify image header\n", __FUNCTION__);
        return HI_FAILURE;
    }
    
    memcpy(pstImgInfo, pu8ImgHeaderBuf,sizeof(HI_CAImgHead_S));
    return HI_SUCCESS;
}


HI_S32 HI_CX_GetFlashImgInfoByAddr(HI_U32 PartionAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_CAImgHead_S* pstImgInfo, HI_BOOL *pIsEncrypt)
{
    HI_S32 ret = 0;
    HI_U8 pu8ImgHeaderBuf[CA_IMAGE_HEADER_AREA_LEN] = {0};
    HI_Flash_InterInfo_S flashInfo;    

    CA_CheckPointer(pstImgInfo);
    CA_CheckPointer(pstAuthInfo);
    
    CA_ASSERT(CA_flash_read_addr(PartionAddr,CA_IMAGE_HEADER_AREA_LEN,(HI_U8*)pu8ImgHeaderBuf, &flashInfo),ret);
    
    ret = CX_VerifyImageHeader(pstAuthInfo, pu8ImgHeaderBuf, flashInfo.PartSize, pIsEncrypt);
    if (ret != HI_SUCCESS)
    {
        HI_INFO_CA("%s: Fail to verify image header\n", __FUNCTION__);
        return HI_FAILURE;
    }
    
    memcpy(pstImgInfo, pu8ImgHeaderBuf,sizeof(HI_CAImgHead_S));
    return HI_SUCCESS;

}


HI_S32 HI_CX_VerifyFlashPartitionByName(HI_CHAR* pPartitionName,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo, HI_U32 *pu32CodeAddr)
{
    HI_S32 ret = 0;
    HI_CAImgHead_S ImgInfo;
    HI_U8 u8Signature[SIGNATURE_LEN] = {0};
    HI_U8* pImageSignature = NULL;
    HI_BOOL bEncrypt = HI_TRUE;

    CA_CheckPointer(pPartitionName);
    CA_CheckPointer(pstAuthInfo);
    
    memset(&ImgInfo,0,sizeof(ImgInfo));

    CA_ASSERT(HI_CX_GetFlashImgInfoByName(pPartitionName, pstAuthInfo, &ImgInfo, &bEncrypt),ret);   
    CA_ASSERT(CA_flash_read(pPartitionName,0ULL,ImgInfo.u32TotalLen,(HI_U8*)u32RamAddr, HI_NULL),ret);

    if (HI_TRUE == bEncrypt)
    {
        CA_ASSERT(HI_CX_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, (HI_U8 *)u32RamAddr,ImgInfo.u32TotalLen,(HI_U8 *)u32RamAddr),ret);
    }
    else
    {
        HI_ERR_CA("%s is not encrypted, it is not allowed ,reset.......\n",pPartitionName);
        return -1;
    }

    pImageSignature = (HI_U8*)(u32RamAddr + ImgInfo.u32SignatureOffset );
    memcpy(u8Signature,pImageSignature,SIGNATURE_LEN);

    CA_ASSERT(HI_CX_ADP_VerifyImage(u32RamAddr,ImgInfo.u32SignedImageLen,u8Signature, pstAuthInfo->eAuthAlg),ret);
    if(pu32CodeAddr)
    {
        *pu32CodeAddr = u32RamAddr + ImgInfo.u32CodeOffset ;
    }

    HI_INFO_CA("%s: %s successed\n", __FUNCTION__, pPartitionName);
 
    return ret;
}


HI_S32 HI_CX_VerifyFlashPartitionByAddr(HI_U32 PartitionAddr,HI_U32 u32RamAddr, HI_CA_AuthInfo_S *pstAuthInfo,HI_U32 *pu32CodeAddr)
{
    HI_S32 ret = 0;
    HI_CAImgHead_S ImgInfo;
    HI_U8 u8Signature[SIGNATURE_LEN] = {0};
    HI_U8* pImageSignature = NULL;
    HI_BOOL bEncrypt = HI_TRUE;

    CA_CheckPointer(pstAuthInfo);
    memset(&ImgInfo,0,sizeof(ImgInfo));

    CA_ASSERT(HI_CX_GetFlashImgInfoByAddr(PartitionAddr, pstAuthInfo, &ImgInfo, &bEncrypt),ret);
    CA_ASSERT(CA_flash_read_addr(PartitionAddr,ImgInfo.u32TotalLen,(HI_U8*)u32RamAddr, HI_NULL),ret);

    if (HI_TRUE == bEncrypt)
    {
        CA_ASSERT(HI_CX_ADP_DataDecrypt(&pstAuthInfo->stCryptoInfo, (HI_U8 *)u32RamAddr,ImgInfo.u32TotalLen,(HI_U8 *)u32RamAddr),ret);
    }
    else
    {
        HI_ERR_CA("Image is not encrypted, it is not allowed ,reset.......\n");
        return -1;
    }

    pImageSignature = (HI_U8*)(u32RamAddr + ImgInfo.u32SignatureOffset );
    memcpy(u8Signature,pImageSignature,SIGNATURE_LEN);

    CA_ASSERT(HI_CX_ADP_VerifyImage(u32RamAddr,ImgInfo.u32SignedImageLen,u8Signature, pstAuthInfo->eAuthAlg),ret);
    
    if(pu32CodeAddr)
    {
        *pu32CodeAddr = u32RamAddr + ImgInfo.u32CodeOffset ;
    }

    HI_INFO_CA("%s:  0x%x  successed\n",__FUNCTION__, PartitionAddr);    

    return ret;
}


