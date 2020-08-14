/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_cipher_intf.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef HI_MINIBOOT_SUPPORT
#include "linux/mtd/compat.h"
#include <common.h>
#else
#include "string.h"
#include "stdio.h"
#include "delay.h"
#endif

#include "hi_type.h"
#include "drv_cipher_intf.h"
#include "drv_cipher.h"
#include "drv_rsa.h"
#include "drv_cipher_define.h"
#include "hi_unf_cipher.h"
#include "malloc.h"

extern HI_S32 HI_CIPHER_EncryptEx(HI_HANDLE hCipherHandle, HI_U8 *input, HI_U32 datalen, HI_U8 *output);

#define HI_ID_CIPHER 0x4D

#define HI_HANDLE_MAKEHANDLE(mod, privatedata, chnid)  (HI_HANDLE)( (((mod)& 0xffff) << 16) | ((((privatedata)& 0xff) << 8) ) | (((chnid) & 0xff)) )

#define HI_HANDLE_GET_MODID(handle)    (((handle) >> 16) & 0xffff)
#define HI_HANDLE_GET_PriDATA(handle)  (((handle) >> 8) & 0xff)
#define HI_HANDLE_GET_CHNID(handle)    (((handle)) & 0xff)

typedef struct hiCIPHER_OSR_CHN_S
{
    HI_BOOL g_bSoftChnOpen; /* mark soft channel open or not*/
    HI_BOOL g_bDataDone;    /* mark the data done or not */

    HI_UNF_CIPHER_DATA_S *pstDataPkg;
    HI_U32                u32DataPkgNum;
}CIPHER_OSR_CHN_S;

static CIPHER_OSR_CHN_S g_stCipherOsrChn[CIPHER_SOFT_CHAN_NUM];
static HI_HANDLE  g_cipherChn = 0;          /* cipher handle */
static HI_U32 g_softChnId = 0;                  /* soft channel ID */

static HI_S32 g_HashDevFd = -1;
static HI_S32 g_CipherDevFd = -1;
CIPHER_HASH_DATA_S g_stCipherHashData;

static HI_U8 g_u8CbcMac[16];

#define CIPHER_CheckHandle(softChnId) do{\
        if (HI_FALSE == g_stCipherOsrChn[softChnId].g_bSoftChnOpen)\
        {\
            HI_ERR_CIPHER("invalid chn %d, open=%d.\n", softChnId, g_stCipherOsrChn[softChnId].g_bSoftChnOpen);\
            return HI_ERR_CIPHER_INVALID_HANDLE;\
        }\
    }while(0)

static HI_S32 cipher_waitStat(HI_U32 chnId)
{
    HI_U32 cnt;
    HI_U32 cipherStat;

    /* wait for Cipher calc done */
    cnt = 0;
    // corform one value when test, about 500
    while (cnt < 10000)
    {
        DRV_Cipher_Stat(chnId, &cipherStat);
        if (CIPHER_PKGx1_CHAN == chnId)
        {
            if (cipherStat == 0)
            {
                break;
            }
        }
        else
        {
            if (cipherStat == 1)
            {
                break;
            }
        }
        udelay(10);
        cnt++;
    }

    if (cnt >= 10000)
    {
        HI_ERR_CIPHER("cipher_waitStat handle too long \n");
        return HI_FAILURE;      /* overtime */
    }

    DRV_CipherDataDoneMultiPkg(chnId);
    return HI_SUCCESS;        /* success */
}

static HI_VOID DRV_CIPHER_UserCommCallBack(HI_U32 arg)
{
    //HI_ERR_CIPHER("arg=%#x.\n", arg);

    //g_stCipherOsrChn[arg].g_bDataDone = HI_TRUE;

    return ;
}

/* New unf interface: HI_UNF_CIHPER_Init */
HI_S32 Cipher_Init(HI_VOID)
{
    HI_U32 i;
    HI_S32 ret;
    
    if (g_CipherDevFd >= 0)
    {
        g_CipherDevFd++;
        return HI_SUCCESS;
    }

    ret = DRV_Cipher_Init();
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
        g_stCipherOsrChn[i].g_bDataDone = HI_FALSE;
        g_stCipherOsrChn[i].pstDataPkg = NULL;
    }

    g_CipherDevFd = 0;
    
    return HI_SUCCESS;
}

/* New unf interface: HI_UNF_CIHPER_DeInit */
HI_VOID Cipher_Exit(HI_VOID)
{
    HI_U32 i;

    
    if (g_CipherDevFd > 0)
    {
        g_CipherDevFd--;
        return;
    }
	
	if (g_CipherDevFd < 0)
    {
        g_CipherDevFd = -1;
        return;
    }
    
    for (i = 0; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        DRV_Cipher_CloseChn(i);
        g_stCipherOsrChn[i].g_bSoftChnOpen = HI_FALSE;
		if (g_stCipherOsrChn[i].pstDataPkg)
	    {
		    free(g_stCipherOsrChn[i].pstDataPkg);
		    g_stCipherOsrChn[i].pstDataPkg = NULL;
	    }
    }
    
    DRV_Cipher_DeInit();
    g_CipherDevFd = -1;

    return ;
}

/* New unf interface: HI_UNF_CIHPER_CreateHandle */
HI_S32 Cipher_CreateHandle(HI_HANDLE* phCipher, const HI_UNF_CIPHER_ATTS_S *pstCipherAttr)
{
    HI_U32 i;
    HI_S32         Ret = 0;

    if (NULL == phCipher)
    {
        HI_ERR_CIPHER("para phCipher is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }
    
    if ( HI_UNF_CIPHER_TYPE_NORMAL != pstCipherAttr->enCipherType )
    {
		if( HI_UNF_CIPHER_TYPE_COPY_AVOID != pstCipherAttr->enCipherType )
    	{
            Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
            HI_ERR_CIPHER("Invalid Cipher Type!\n");
            return Ret;
        }

        if (0 == g_stCipherOsrChn[0].g_bSoftChnOpen)
        {
            i = 0;
        }
        else
        {
            i = CIPHER_INVALID_CHN;
        }
    }
    else
    {
        for(i = CIPHER_PKGxN_CHAN_MIN; i < CIPHER_SOFT_CHAN_NUM; i++)
        {
            if (0 == g_stCipherOsrChn[i].g_bSoftChnOpen)
            {
                break;
            }
        }
    }
    
    for(i = CIPHER_PKGxN_CHAN_MIN; i < CIPHER_SOFT_CHAN_NUM; i++)
    {
        if (0 == g_stCipherOsrChn[i].g_bSoftChnOpen)
        {
            break;
        }
    }

    if (i >= CIPHER_SOFT_CHAN_NUM)
    {
        Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
        HI_ERR_CIPHER("No more cipher chan left.\n");
        return HI_FAILURE;
    }
    else /* get a free chn */
    {
        g_stCipherOsrChn[i].pstDataPkg = (HI_UNF_CIPHER_DATA_S *)malloc(sizeof(HI_UNF_CIPHER_DATA_S) * CIPHER_MAX_LIST_NUM);
        if (NULL == g_stCipherOsrChn[i].pstDataPkg)
        {
            Ret = HI_ERR_CIPHER_FAILED_GETHANDLE;
            HI_ERR_CIPHER("can NOT malloc memory for cipher.\n");
            return HI_FAILURE;
        }
            
        g_softChnId = i;
        g_stCipherOsrChn[g_softChnId].g_bSoftChnOpen = HI_TRUE;
    }


    g_cipherChn = HI_HANDLE_MAKEHANDLE(HI_ID_CIPHER, 0, g_softChnId);

    Ret = DRV_Cipher_OpenChn(g_softChnId);
    if (HI_SUCCESS != Ret)
    {
        free(g_stCipherOsrChn[i].pstDataPkg);
        g_stCipherOsrChn[i].pstDataPkg = NULL;
        return HI_FAILURE;
    }

    *phCipher = g_cipherChn;
    
    Ret = HI_SUCCESS;
    
    return Ret;
}

/* New unf interface: HI_UNF_CIHPER_DestroyHandle */
HI_S32 Cipher_DestroyHandle(HI_HANDLE hCipher)
{
    HI_S32         Ret = 0;

    g_cipherChn = hCipher;

    g_softChnId = HI_HANDLE_GET_CHNID(g_cipherChn);
    if (HI_FALSE == g_stCipherOsrChn[g_softChnId].g_bSoftChnOpen)
    {
        Ret = HI_SUCCESS; /* success on re-Destroy */
        return Ret;
    }

	if (g_stCipherOsrChn[g_softChnId].pstDataPkg)
	{
		free(g_stCipherOsrChn[g_softChnId].pstDataPkg);
		g_stCipherOsrChn[g_softChnId].pstDataPkg = NULL;
    }

    g_stCipherOsrChn[g_softChnId].g_bSoftChnOpen = HI_FALSE;
    Ret = DRV_Cipher_CloseChn(g_softChnId);
    
    return Ret;
}

/* New unf interface: HI_UNF_CIHPER_ConfigHandle */
HI_S32 Cipher_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl)
{
    HI_U32 Ret;
    CIPHER_Config_CTRL  configdata;

    if (NULL == pstCtrl)
    {
        HI_ERR_CIPHER("para pstCtrl is null.\n");
        return HI_ERR_CIPHER_INVALID_POINT;
    }

    memcpy(&configdata.CIpstCtrl, pstCtrl, sizeof(HI_UNF_CIPHER_CTRL_S));
    configdata.CIHandle=hCipher;

    if(configdata.CIpstCtrl.enWorkMode >= HI_UNF_CIPHER_WORK_MODE_BUTT)
    {
        HI_ERR_CIPHER("para setCIPHER wokemode is invalid.\n");
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    g_softChnId = HI_HANDLE_GET_CHNID(configdata.CIHandle);
    CIPHER_CheckHandle(g_softChnId);

    Ret = DRV_Cipher_ConfigChn(g_softChnId, &configdata.CIpstCtrl, DRV_CIPHER_UserCommCallBack);
            
    return Ret;
}

/* New unf interface: HI_UNF_CIHPER_Encrypt */
HI_S32 Cipher_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret;
    HI_U32 chnid;
    CIPHER_DATA_S CIdata;
    HI_DRV_CIPHER_TASK_S pCITask;

    chnid = HI_HANDLE_GET_CHNID(hCipher);

    if (0 == chnid || u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    g_softChnId = HI_HANDLE_GET_CHNID(CIdata.CIHandle);
    CIPHER_CheckHandle(g_softChnId);

    pCITask.stData2Process.u32src = CIdata.ScrPhyAddr;
    pCITask.stData2Process.u32dest = CIdata.DestPhyAddr;
    pCITask.stData2Process.u32length = CIdata.ByteLength;
    pCITask.stData2Process.bDecrypt = HI_FALSE;
    pCITask.u32CallBackArg = g_softChnId;

    g_stCipherOsrChn[g_softChnId].g_bDataDone = HI_FALSE;

    Ret = DRV_Cipher_CreatTask(g_softChnId,&pCITask, NULL, NULL);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    //HI_INFO_CIPHER("\nencrypt wait channel %d queue event\n", softChnId);

    Ret = cipher_waitStat(chnid);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Cipher_Encrypt  timeout\n");
        return HI_FAILURE;
    }

    Ret = HI_SUCCESS;

    return Ret;
}

/* New unf interface: HI_UNF_CIHPER_Decrypt */
HI_S32 Cipher_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_U32 Ret;
    HI_U32 chnid;
    CIPHER_DATA_S CIdata;
    HI_DRV_CIPHER_TASK_S       pCITask;

    chnid = HI_HANDLE_GET_CHNID(hCipher);

    if (0 == chnid || u32ByteLength < HI_UNF_CIPHER_MIN_CRYPT_LEN)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    CIdata.ScrPhyAddr=u32SrcPhyAddr;
    CIdata.DestPhyAddr=u32DestPhyAddr;
    CIdata.ByteLength=u32ByteLength;
    CIdata.CIHandle=hCipher;

    g_softChnId = HI_HANDLE_GET_CHNID(CIdata.CIHandle);
    CIPHER_CheckHandle(g_softChnId);
    pCITask.stData2Process.u32src=CIdata.ScrPhyAddr;
    pCITask.stData2Process.u32dest=CIdata.DestPhyAddr;
    pCITask.stData2Process.u32length=CIdata.ByteLength;
    pCITask.stData2Process.bDecrypt=HI_TRUE;

    pCITask.u32CallBackArg=g_softChnId;

    g_stCipherOsrChn[g_softChnId].g_bDataDone = HI_FALSE;

    Ret = DRV_Cipher_CreatTask(g_softChnId,&pCITask,NULL,NULL);
    if (HI_SUCCESS != Ret)
    {
        return Ret;
    }

    Ret = cipher_waitStat(chnid);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CIPHER("Cipher_Encrypt  timeout\n");
        return HI_FAILURE;
    }


    Ret = HI_SUCCESS;

    return Ret;
}

HI_S32 Cipher_GetRandomNumber(HI_U32 *pu32RandomNumber)
{
    return HAL_Cipher_GetRandomNumber(pu32RandomNumber);
}

//#ifndef HI_MINIBOOT_SUPPORT
/* New unf interface: HI_UNF_CIHPER_HashInit */
HI_S32 Cipher_HashInit(HI_UNF_CIPHER_HASH_ATTS_S *pstHashAttr, HI_HANDLE *pHashHandle)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_CIPHER_CTRL_S stCipherCtrl;
    HI_UNF_CIPHER_ATTS_S stCipherAtts;
    HI_HANDLE hCipherHandle;

    if( (NULL== pstHashAttr) || (NULL == pHashHandle) )
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        return HI_FAILURE;
    }

    if(pstHashAttr->eShaType >= HI_UNF_CIPHER_HASH_TYPE_BUTT)
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        return HI_FAILURE;
    }

    if(g_HashDevFd > 0)
    {
        HI_ERR_CIPHER("Hash module is busy!\n");
        return HI_FAILURE;       
    }

    g_HashDevFd = 1;
    
    if(pstHashAttr->eShaType == HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC)
    {
        memset((HI_U8 *)&g_stCipherHashData, 0, sizeof(g_stCipherHashData));
        g_stCipherHashData.enShaType = pstHashAttr->eShaType;
        
        memset(&stCipherAtts, 0, sizeof(HI_UNF_CIPHER_ATTS_S));
        stCipherAtts.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
        Ret = Cipher_CreateHandle(&hCipherHandle, &stCipherAtts);
        if (Ret != HI_SUCCESS)
        {
           HI_ERR_CIPHER("Fail to create Handle.\n");
           return HI_FAILURE;   
        }
        memset(stCipherCtrl.u32IV, 0, sizeof(stCipherCtrl.u32IV));
        stCipherCtrl.bKeyByCA = HI_TRUE;
        stCipherCtrl.enAlg = HI_UNF_CIPHER_ALG_AES;
        stCipherCtrl.enBitWidth = HI_UNF_CIPHER_BIT_WIDTH_128BIT;
        stCipherCtrl.enWorkMode = HI_UNF_CIPHER_WORK_MODE_CBC;
        stCipherCtrl.enKeyLen = HI_UNF_CIPHER_KEY_AES_128BIT;
        stCipherCtrl.stChangeFlags.bit1IV = 1;
        stCipherCtrl.enCaType = HI_UNF_CIPHER_CA_TYPE_IRDETO_HCA;
        Ret = Cipher_ConfigHandle(hCipherHandle, &stCipherCtrl);
        if (Ret != HI_SUCCESS)
        {
           HI_ERR_CIPHER("Fail to create Handle.\n");
           return HI_FAILURE;   
        }
        g_stCipherHashData.hHandle = hCipherHandle;
    }
    else
    {
        memset((HI_U8 *)&g_stCipherHashData, 0, sizeof(g_stCipherHashData));
        g_stCipherHashData.enShaType = pstHashAttr->eShaType;
        g_stCipherHashData.u32TotalDataLen = pstHashAttr->u32TotalDataLen;
        g_stCipherHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
        if( ( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pstHashAttr->eShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pstHashAttr->eShaType ))
        {
            memcpy(g_stCipherHashData.u8HMACKey, pstHashAttr->u8HMACKey, 16);
            g_stCipherHashData.enHMACKeyFrom = HI_CIPHER_HMAC_KEY_FROM_CPU;
        }
    
        Ret = DRV_Cipher_CalcHashInit(&g_stCipherHashData);
        if(Ret != HI_SUCCESS)
        {
            g_HashDevFd = -1;
            return Ret;
        }
    }
    *pHashHandle = (HI_U32)&g_stCipherHashData;

    return HI_SUCCESS;
}

/* New unf interface: HI_UNF_CIHPER_HashUpdate */	
HI_S32 Cipher_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen)
{
    HI_S32 Ret = HI_SUCCESS;
    CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S *)hHashHandle;
    static HI_U32 u32MmzPhyAddr = 0;
    static HI_BOOL bIsDstMmzInit = HI_FALSE;

    if(pu8InputData == NULL || u32InputDataLen == 0)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        g_HashDevFd = -1;
        return HI_FAILURE;
    }
    
    if(pstCipherHashData->enShaType == HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC)
    {
	    if(!bIsDstMmzInit)
	    {
	        #if 0
	        Ret = HI_MEM_Alloc(&u32MmzPhyAddr, MAX_DATA_LEN);
	        if (Ret != HI_SUCCESS)
	        {
	            HI_ERR_CIPHER("malloc input memory failed!\n");
	            return HI_FAILURE;
	        }
            #else
            u32MmzPhyAddr = (HI_U32)devmemalign(16, MAX_DATA_LEN);
            if (u32MmzPhyAddr == 0)
	        {
	            HI_ERR_CIPHER("malloc input memory failed!\n");
	            return HI_FAILURE;
	        }
            #endif
		    memset((void *)u32MmzPhyAddr, 0, MAX_DATA_LEN);

	        bIsDstMmzInit = HI_TRUE;
	    }

        Ret = HI_CIPHER_EncryptEx(pstCipherHashData->hHandle, pu8InputData, u32InputDataLen, (HI_U8 *)u32MmzPhyAddr);
        memcpy(g_u8CbcMac, (HI_U8 *)(u32MmzPhyAddr + u32InputDataLen - 16), 16);
    }
    else
    {
        pstCipherHashData->u32InputDataLen = u32InputDataLen;
        pstCipherHashData->pu8InputData = pu8InputData;
        Ret = DRV_Cipher_CalcHashUpdate(pstCipherHashData);
    }
    
    if(Ret != HI_SUCCESS)
    {
        g_HashDevFd = -1;
        return Ret;
    }

    return HI_SUCCESS;
}

/* New unf interface: HI_UNF_CIHPER_HashFinal */		
HI_S32 Cipher_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash)
{
    HI_S32 Ret = HI_SUCCESS;
    CIPHER_HASH_DATA_S *pstCipherHashData = (CIPHER_HASH_DATA_S *)hHashHandle;

    if(pu8OutputHash == NULL)
    {
        HI_ERR_CIPHER("Invalid parameter!\n");
        g_HashDevFd = -1;
        return HI_FAILURE;
    }
    
    if(pstCipherHashData->enShaType == HI_UNF_CIPHER_HASH_TYPE_IRDETO_CBCMAC)
    {
        memcpy(pu8OutputHash, g_u8CbcMac, 16);
        Cipher_DestroyHandle(pstCipherHashData->hHandle);
    }
    else
    {
        pstCipherHashData->pu8Output = pu8OutputHash;
        Ret = DRV_Cipher_CalcHashFinal(pstCipherHashData);
    }
    
    if(Ret != HI_SUCCESS)
    {
        g_HashDevFd = -1;
        return Ret;
    }
    g_HashDevFd = -1;

    return HI_SUCCESS;
}

HI_S32 Cipher_AuthCbcMac(HI_U8 *pu8RefCbcMac, HI_U32 u32AppLen)
{
	return DRV_Cipher_AuthCbcMac(pu8RefCbcMac, u32AppLen);	
}


HI_S32 Cipher_CalcRsa(CIPHER_RSA_DATA_S *pCipherRsaData)
{
#ifdef RSA_HARDWARE_SUPPORT
    return DRV_CIPHER_CalcRsa(pCipherRsaData);
#else
    return DRV_CIPHER_CalcRsa_SW(pCipherRsaData);
#endif
}

//#endif
