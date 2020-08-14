/***********************************************************************************
 *              Copyright 2008 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:         hi_unf_api_ir.c
 * Description:      supply the api for userspace application
 * History:          NULL
 * Version           Date           Author     DefectNum    Description
 * 1.1               2009-04-13	    s00136582   NULL	    first draft
 ***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <semaphore.h>

#include "hi_drv_struct.h"
#include "drv_ir_ioctl.h"
#include "hi_drv_ir.h"
#include "hi_unf_ir.h"
#include "hi_osal.h"

static HI_S32 g_IrDevFd = -1;
//static HI_UNF_IR_PROTOCOL_E g_IrProtocol = -1;
static HI_CHAR g_szLastProtocolName[PROTOCOL_NAME_SZ];
static HI_U32 g_u32IrInitCount = 0;

static pthread_mutex_t   g_IrMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_IR_LOCK()  	 (void)pthread_mutex_lock(&g_IrMutex);
#define HI_IR_UNLOCK()  	 (void)pthread_mutex_unlock(&g_IrMutex);

#define CHECK_IR_OPEN()\
do{\
    HI_IR_LOCK();\
    if (g_IrDevFd < 0)\
    {\
        HI_ERR_IR("IR is not open.\n");\
        HI_IR_UNLOCK();\
        return HI_ERR_IR_NOT_INIT;\
    }\
    HI_IR_UNLOCK();\
}while(0)

/*************************************************************
Function:       HI_UNF_IR_Init
Description:    open ir device,and do the basical initialization
Calls:
Data Accessed:
Data Updated:   NA
Input:
Output:
Return:         HI_SUCCESS
                HI_ERR_IR_OPEN_ERR
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_Init(HI_VOID)
{
    HI_IR_LOCK();

    g_u32IrInitCount++;
    
    /* if had opened will return success*/
    if (g_IrDevFd > 0)
    {
        HI_IR_UNLOCK();
        return HI_SUCCESS;
    }

    g_IrDevFd = open("/dev/"UMAP_DEVNAME_IR, O_RDWR, 0);
    if (g_IrDevFd < 0)
    {
        HI_FATAL_IR("open IR err.\n");
        HI_IR_UNLOCK();
		return HI_ERR_IR_OPEN_ERR; 
    }

    HI_IR_UNLOCK();
    
    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_DeInit 
Description:    close ir device
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_CLOSE_ERR
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_DeInit(HI_VOID)
{   
    HI_S32 Ret;
    
    HI_IR_LOCK();

    if (g_IrDevFd < 0)
    {
        HI_IR_UNLOCK();
        return HI_SUCCESS;
    }

    g_u32IrInitCount--;

    if (0 == g_u32IrInitCount)
    {
        Ret = close(g_IrDevFd);

        if(HI_SUCCESS != Ret)
        {
            HI_FATAL_IR("Close IR err.\n");
            HI_IR_UNLOCK();
            return HI_ERR_IR_CLOSE_ERR;
        }

        g_IrDevFd = -1;
    }

    HI_IR_UNLOCK();
    
    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_Enable 
Description:    Enable ir device
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_ENABLE_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_Enable (HI_BOOL bEnable)
{
    HI_S32 Ret;
    
    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE))
    {
        HI_ERR_IR("para bEnable is invalid.\n");
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();

    Ret = ioctl(g_IrDevFd, CMD_IR_SET_ENABLE, bEnable);

    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_ENABLE_FAILED;
    }
    
    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_GetProtocol
Description:    get the type of ir protocol
Calls:          
Data Accessed:   
Data Updated:   NA
Input:              NA
Output:         penProtocol:    ir protocol of the key
Return:         HI_ERR_IR_UNSUPPORT
Others:         NA
*************************************************************/
/*lint -save -e715 */
HI_S32 HI_UNF_IR_GetProtocol(HI_UNF_IR_PROTOCOL_E *penProtocol)
{    
    return HI_ERR_IR_UNSUPPORT;
}
/*************************************************************
Function:       HI_UNF_IR_GetProtocolName
Description:    reserved
Calls:          
Data Accessed:   
Data Updated:   NA
Input:          pProtocolName, s32BufLen
Output:         pProtocolName
Return:         HI_ERR_IR_UNSUPPORT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_GetProtocolName(HI_CHAR *pProtocolName,HI_S32 s32BufLen)
{
    return HI_ERR_IR_UNSUPPORT;
}
/*************************************************************
Function:       HI_UNF_IR_GetValueWithProtocol 
Description:    get the value and status of key
Calls:          
Data Accessed:   
Data Updated:   NA
Input:          u32TimeoutMs: overtime value with unit of ms : 0 means no block while 0xFFFFFFFF means block forever
                s32NameSize: buffer size of protocol_name.
Output:         pu32PressStatus:    status of the key
                   0:  means press
                   1:  means hold
                   2:  means release
                pu64KeyId:          value of the key

                pszProtocolName: ir protocol name.

Return:         HI_ERR_IR_NULL_PTR
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_SET_BLOCKTIME_FAILED
                HI_ERR_IR_READ_FAILED
                HI_ERR_IR_NOT_INIT
                HI_SUCCESS
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_GetValueWithProtocol(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId, 
        HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs)
{    
    HI_S32 Ret;
    struct key_attr Irkey;

    if (!penPressStatus)
    {
        HI_ERR_IR("para penPressStatus is null.\n");
        return HI_ERR_IR_NULL_PTR;
    }

    if (!pu64KeyId)
    {
        HI_ERR_IR("para pu64KeyId is null.\n");
        return HI_ERR_IR_NULL_PTR;
    }

    if (pszProtocolName && (s32NameSize < PROTOCOL_NAME_SZ))
    {
        HI_ERR_IR("Invalid protocol buffer!\n");
        return HI_ERR_IR_INVALID_PARA;
    }
    CHECK_IR_OPEN();

    memset(&Irkey, 0, sizeof(Irkey));

    Ret = ioctl(g_IrDevFd, CMD_IR_SET_BLOCKTIME, u32TimeoutMs);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_SET_BLOCKTIME_FAILED;
    }

    Ret = read(g_IrDevFd, &Irkey, sizeof(struct key_attr));
    if (Ret == (HI_S32)sizeof(struct key_attr))
    {
        if (pszProtocolName)
        {
                memcpy(pszProtocolName, Irkey.protocol_name, (HI_U32)(s32NameSize - 1));
                pszProtocolName[s32NameSize - 1] = '\0';
        }

        HI_OSAL_Strncpy(g_szLastProtocolName, Irkey.protocol_name, sizeof(Irkey.protocol_name));
        g_szLastProtocolName[PROTOCOL_NAME_SZ - 1] = '\0';

        *penPressStatus = (HI_UNF_KEY_STATUS_E)Irkey.key_stat;

        *pu64KeyId = Irkey.lower;
        if (Irkey.upper)
        {
            HI_ERR_IR("This infrared code constains more than" \
                      " 64bits data!\n");
        }
    }
    else
    {
        return HI_ERR_IR_READ_FAILED;
    }
   
    return HI_SUCCESS;
}
/*************************************************************
Function:       HI_UNF_IR_SetFetchMode 
Description:    set key fetch mode or symbol mode.
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          mode: 0-> key mode. 1-> raw symbol mode.
Output:
Return:         HI_SUCCESS
                HI_ERR_IR_NOT_INIT
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_SET_FETCHMETHOD_FAILED
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_SetFetchMode(HI_S32 s32Mode)
{
    HI_S32 Ret;

    if ((s32Mode != 0) && (s32Mode != 1))
    {
        HI_ERR_IR("only 0 or 1 is allowed!\n");
        return HI_ERR_IR_INVALID_PARA;
    }
    CHECK_IR_OPEN();

    if ((Ret = ioctl(g_IrDevFd, CMD_IR_SET_FETCH_METHOD, s32Mode)) != HI_SUCCESS)
    {
        HI_ERR_IR("Fail to set symbol mode, ret = 0x%x!\n", Ret);
        return HI_ERR_IR_SET_FETCHMETHOD_FAILED;
    }
    return HI_SUCCESS;
}
/*************************************************************
Function:       HI_UNF_IR_GetSymbol 
Description:    get one raw symbols from ir module.
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          u32TimeoutMs: read timeout in ms.
Output:         pu64lower, pu64upper.
Return:         HI_SUCCESS
                HI_ERR_IR_NULL_PTR
                HI_ERR_IR_NOT_INIT
                HI_ERR_IR_SET_BLOCKTIME_FAILED
                HI_ERR_IR_READ_FAILED
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_GetSymbol(HI_U64 *pu64First, HI_U64* pu64Second, HI_U32 u32TimeoutMs)
{
    HI_S32 Ret;
    struct key_attr key;

    if (!pu64First || !pu64Second)
    {
        HI_ERR_IR("Invalid parameters!\n");
        return HI_ERR_IR_NULL_PTR;
    }

    CHECK_IR_OPEN();

    Ret = ioctl(g_IrDevFd, CMD_IR_SET_BLOCKTIME, u32TimeoutMs);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_SET_BLOCKTIME_FAILED;
    }

    Ret = read(g_IrDevFd, &key, sizeof(struct key_attr));
    if (Ret != (HI_S32)sizeof(struct key_attr))
    {
        return HI_ERR_IR_READ_FAILED;
    }

    *pu64First = key.lower;
    *pu64Second = key.upper;

    return HI_SUCCESS;
}
/*************************************************************
Function:       HI_UNF_IR_EnableKeyUp 
Description:    config whether report the state of key release
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_SET_KEYUP_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable)
{
    HI_S32 Ret;

	if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE))
    {
        HI_ERR_IR("para bEnable is invalid.\n");
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();
	    
    Ret = ioctl(g_IrDevFd, CMD_IR_ENABLE_KEYUP, bEnable);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_SET_KEYUP_FAILED;
    }
    
    return HI_SUCCESS;
}


/*************************************************************
Function:       HI_UNF_IR_IsRepKey 
Description:    config whether report repeat key 
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_SET_REPEAT_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable)
{
    HI_S32 Ret;

    if ((bEnable != HI_TRUE)
        && (bEnable != HI_FALSE))
    {
        HI_ERR_IR("para bEnable is invalid.\n");
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();

    Ret = ioctl(g_IrDevFd, CMD_IR_ENABLE_REPKEY, bEnable);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_SET_REPEAT_FAILED;
    }

    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_RepKeyTimeoutVal 
Description:    Set the reporting interval when you keep pressing button.
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          u32TimeoutMs  The minimum interval to report repeat key 
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_NOT_INIT
                HI_ERR_IR_SET_REPKEYTIMEOUT_FAILED
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs)
{
    HI_S32 Ret;

    CHECK_IR_OPEN();

    /* Remove 108 minimal time limit.
     * For some user require every key press event should
     * be reported.
     */
    if (u32TimeoutMs == 0)
    {
        HI_ERR_IR("prompt:u32TimeoutMs == 0, set to 108\n");
        u32TimeoutMs = 108;
    }

    if (u32TimeoutMs > 65536)
    {
        HI_ERR_IR("prompt:u32TimeoutMs > 65536, set to 65536\n");
        u32TimeoutMs = 65536;
    }
	
    Ret = ioctl(g_IrDevFd, CMD_IR_SET_REPKEY_TIMEOUT, u32TimeoutMs);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_SET_REPKEYTIMEOUT_FAILED;
    }

    return HI_SUCCESS;
}
/*************************************************************
Function:       HI_UNF_IR_SetCodeType 
Description:    reserved interface.
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          enIRCode
Output:         
Return:         HI_SUCCESS
                
Others:         NA
*************************************************************/

HI_S32 HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_E enIRCode)
{
    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_Reset 
Description:    Reset ir device
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_RESET_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_Reset(HI_VOID)
{
	HI_S32 Ret;

    CHECK_IR_OPEN();

    Ret = ioctl(g_IrDevFd, CMD_IR_RESET);
    if (Ret != HI_SUCCESS)
    {
        return HI_ERR_IR_RESET_FAILED;
    }

    return HI_SUCCESS;
}
/*************************************************************
Function:       HI_UNF_IR_EnableProtocol 
Description:    enable an infrared code specified by @prot_name
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          pszProtocolName: infrared code name.
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_ENABLE_PROT_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName)
{
    HI_S32 ret;

    if (!pszProtocolName)
    {
        return HI_ERR_IR_NULL_PTR;
    }

    if (!pszProtocolName[0])
    {
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();

    ret = ioctl(g_IrDevFd, CMD_IR_SET_PROT_ENABLE, pszProtocolName);
    if (ret)
    {
        return HI_ERR_IR_ENABLE_PROT_FAILED;
    }
    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_DisableProtocol 
Description:    disable a infrared code specified by @prot_name
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          pszProtocolName: infrared code name.
Output:         
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_DISABLE_PROT_FAILED
                HI_ERR_IR_NOT_INIT
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName)
{
    HI_S32 ret;

    if (!pszProtocolName)
    {
        return HI_ERR_IR_NULL_PTR;
    }

    if (!pszProtocolName[0])
    {
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();

    ret = ioctl (g_IrDevFd, CMD_IR_SET_PROT_DISABLE, pszProtocolName);
    if (ret)
    {
        return HI_ERR_IR_DISABLE_PROT_FAILED;
    }

    return HI_SUCCESS;
}

/*************************************************************
Function:       HI_UNF_IR_GetProtocolEnabled
Description:    get the enable status of an infrared code specified by @prot_name
Calls:            
Data Accessed:   
Data Updated:   NA
Input:          pszProtocolName: infrared code name.
Output:         pbEnabled: 1 means protocol effective, can receive keys.
Return:         HI_SUCCESS
                HI_ERR_IR_INVALID_PARA
                HI_ERR_IR_GET_PROTENABLE_FAILED
                HI_ERR_IR_NOT_INIT
                HI_FAILURE
Others:         NA
*************************************************************/
HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled)

{
    HI_S32 ret;
    union NameEnable_U
    {
        char szname[PROTOCOL_NAME_SZ];
        int enable;
    } uNameEnable;

    if (!pszProtocolName || !pbEnabled)
    {
        return HI_ERR_IR_NULL_PTR;
    }

    if (!pszProtocolName[0] || (strlen(pszProtocolName) >= PROTOCOL_NAME_SZ))
    {
        return HI_ERR_IR_INVALID_PARA;
    }

    CHECK_IR_OPEN();

    HI_OSAL_Strncpy(uNameEnable.szname, pszProtocolName, strlen(pszProtocolName));

    ret = ioctl(g_IrDevFd, CMD_IR_GET_PROT_ENABLED, uNameEnable.szname);
    if (ret)
    {
        return HI_ERR_IR_GET_PROTENABLE_FAILED;
    }

    if (0 == uNameEnable.enable)
    {
        *pbEnabled = HI_TRUE;
    } 
    else if (1 == uNameEnable.enable)
    {
        *pbEnabled = HI_FALSE;
    }
    else 
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
