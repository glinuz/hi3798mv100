/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : download_ip.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Restructure this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <curl.h>

#include "hi_osal.h"
#include "com_osal.h"
#include "com_debug.h"
#include "download_ip.h"
#include "loader_dbinfo.h"

#define IPDL_LEN_IPADDR             (20)
#define IPDL_LEN_URL                (256)
#define IPDL_LEN_CURLOPT            (32)
#define IPDL_LEN_DEVICENAME         (16)
#define IPDL_LEN_COMMAND            (128)

#define IPDL_NUM_ETHDEVICE          (2)
#define IPDL_SIZE_BUFFER            (64*1024)

#define IPDL_DEFAULT_IPPROT_TFTP    (69)
#define IPDL_DEFAULT_IPPROT_FTP     (21)
#define IPDL_DEFAULT_IPPROT_HTTP    (80)

typedef enum tagIPDL_LINKSTATUS_E
{
    IPDL_LINKSTATUS_OFF = 0,
    IPDL_LINKSTATUS_ON,
    IPDL_LINKSTATUS_BUFF

} IPDL_LINKSTATUS_E;


HI_LOADER_IPPROT_E  s_enProtType    = HI_LOADER_IPPROT_BUTT;

static HI_U8*       s_pu8Buffer     = HI_NULL_PTR;              /**< TFTP缓存大小 */
static HI_U32       s_u32DataLen    = 0;                        /**< TFTP缓存中已下载数据的长度 */
static HI_U32       s_u32ReadPos    = 0;                        /**< TFTP缓存中已读取的数据长度 */
static HI_U32       s_u32TotalDL    = 0;                        /**< TFTP已下载数据总大小 */

static CURL*        s_hEasyHandle   = HI_NULL_PTR;
static CURLM*       s_hMultiHandle  = HI_NULL_PTR;

static HI_BOOL      s_bInited = HI_FALSE;

static HI_VOID ip2string (HI_U32 u32IP, HI_CHAR* pcDotIp)
{
    u32IP = ntohl (u32IP);

    HI_OSAL_Snprintf(pcDotIp,
                     IPDL_LEN_IPADDR,
                     "%d.%d.%d.%d",
                     (HI_S32)((u32IP >> 24) & 0xff),
                     (HI_S32)((u32IP >> 16) & 0xff),
                     (HI_S32)((u32IP >> 8) & 0xff),
                     (HI_S32)((u32IP >> 0) & 0xff));
}

static HI_S32 net_device_down(HI_U32 u32DevNo)
{
    HI_S8 acCommand[IPDL_LEN_COMMAND] = {0};

    HI_OSAL_Snprintf(acCommand, sizeof(acCommand), "ifconfig eth%d down", u32DevNo);
    system(acCommand);

    return HI_SUCCESS;
}

static HI_S32 net_GetLinkStatus(HI_CHAR* pcEthDevName, IPDL_LINKSTATUS_E* ptrLinkStatus)
{
    HI_S32 sockfd = 0;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 retryTimes = 0;
    struct ifreq ifr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        COM_ERROR("create socket failed\n");
        return HI_FAILURE;
    }

#define MAX_RETRY_NETLINK_STATUS_TIMES (10)  /* 10 * 1s */

    while (retryTimes++ < MAX_RETRY_NETLINK_STATUS_TIMES)
    {
        COM_MemSet(&ifr, 0x00, sizeof(ifr));
        HI_OSAL_Strncpy(ifr.ifr_name, pcEthDevName, sizeof(ifr.ifr_name));
        ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = '\0';

        s32Ret = ioctl(sockfd, SIOCGIFFLAGS, (HI_CHAR*)&ifr);
        if (-1 == s32Ret)
        {
            break;
        }

        if ((ifr.ifr_flags & IFF_RUNNING) == 0)
        {
            *ptrLinkStatus = IPDL_LINKSTATUS_OFF;
        }
        else
        {
            *ptrLinkStatus = IPDL_LINKSTATUS_ON;
            s32Ret = 0;
            break;
        }

        /* delay 1s */
        COM_Delayms(1000);
    }

    close(sockfd);
    return -1 == s32Ret ? HI_FAILURE : HI_SUCCESS;
}

static HI_S32 InitStaticIP(HI_CHAR* pcEthDevName, HI_U32 u32Ipaddr, HI_U32 u32SubnetMask, HI_U32 u32Gateway)
{
    HI_S8 acCommand[IPDL_LEN_COMMAND] = {0};
    HI_CHAR acIP[IPDL_LEN_IPADDR] = {0};
    HI_CHAR acMask[IPDL_LEN_IPADDR] = {0};
    HI_CHAR acGateWay[IPDL_LEN_IPADDR] = {0};
    HI_S32 s32Status = HI_FAILURE;


    ip2string(u32Ipaddr, acIP);
    ip2string(u32SubnetMask, acMask);
    ip2string(u32Gateway, acGateWay);

    /*Config ip*/
    HI_OSAL_Snprintf(acCommand, sizeof(acCommand),  "ifconfig %s %s netmask %s", pcEthDevName, acIP, acMask);
    COM_DEBUG("Command:%s\n", acCommand);

    s32Status = system(acCommand);

    /*lint -save -e69 -e155*/
    if ( !WIFEXITED(s32Status)
         || HI_SUCCESS != (WEXITSTATUS(s32Status)))
    {
        return HI_FAILURE;
    }

    /*lint -restore*/

    /* Config gateway */
    HI_OSAL_Snprintf(acCommand, sizeof(acCommand), "route add default gw %s", acGateWay);
    COM_DEBUG("Command:%s\n", acCommand);

    s32Status = system(acCommand);

    /*lint -save -e69 -e155*/
    if ( !WIFEXITED(s32Status)
         || HI_SUCCESS != (WEXITSTATUS(s32Status)))
    {
        return HI_FAILURE;
    }
    /*lint -restore*/


    return HI_SUCCESS;
}

/*DHCPC*/
static HI_S32 InitDynamicIP(HI_CHAR* pcEthDevName)
{
    HI_S32 s32Status = 0;
    pid_t  child_pid = 0;
    HI_CHAR string[6][32] = {"/sbin/udhcpc", "-i", "-n", "-q", "-t", "10"};
    HI_CHAR* argv_list[8] = {NULL};

    argv_list[0] = string[0];
    argv_list[1] = string[1];
    argv_list[2] = pcEthDevName;
    argv_list[3] = string[2];
    argv_list[4] = string[3];
    argv_list[5] = string[4];
    argv_list[6] = string[5];

    COM_DEBUG("EthDevName:%s\n", argv_list[2]);
    child_pid = fork();
    if ( 0 == child_pid )
    {
        execvp((const HI_CHAR*)argv_list[0], (HI_CHAR * const*)argv_list);
        COM_ERROR("on error occurred in execvp!\r\n");
        abort();
    }

    if (-1 == child_pid )
    {
        return HI_FAILURE;
    }

    waitpid(child_pid, &s32Status, 0);
    /*lint -save -e69 -e155*/
    if (WIFEXITED(s32Status))
    {
        return (WEXITSTATUS(s32Status));
    }
    /*lint -restore*/

    return HI_FAILURE;
}

static HI_U8* net_ltrim(HI_U8* pcString)
{
    HI_U8* pc = NULL;

    if (NULL == pcString)
    {
        return NULL;
    }

    for (pc = pcString; (((0x20 == *pc) || (0x09 == *pc))); pc++)
    {
        continue;
    }

    return pc;
}

static HI_U8* net_rtrim(HI_U8* pcString)
{
    HI_U8* pc = NULL;

    if (NULL == pcString)
    {
        return NULL;
    }

    for (pc = pcString + COM_StrLen((HI_CHAR*)pcString) - 1;
         (0 == *pc) || (0x20 == *pc || 0x09 == *pc);
         pc--)
    {
        continue;
    }

    *(pc + 1) = 0;
    return pcString;
}

static HI_S32 get_url(HI_U8  u8ProtoType,
                      HI_CHAR* pcFileName,
                      HI_CHAR* pcHost,
                      HI_U16 u16Port,
                      HI_U8* pu8User,
                      HI_U8* pu8Pwd,
                      HI_S8* url,
                      HI_U32 urllen)
{
    switch (u8ProtoType)
    {
        case HI_LOADER_IPPROT_TFTP:
        {
            if (0 == u16Port)
            {
                u16Port = IPDL_DEFAULT_IPPROT_TFTP;
            }

            HI_OSAL_Snprintf(url, urllen, "tftp://%s:%d/%s",
                             pcHost, u16Port, pcFileName);
            break;
        }

        case HI_LOADER_IPPROT_FTP:
        {
            if (0 == u16Port)
            {
                u16Port = IPDL_DEFAULT_IPPROT_FTP;
            }

            pu8User = net_ltrim(pu8User);
            pu8User = net_rtrim(pu8User);

            if ((HI_NULL_PTR != pu8User)
                && (0 != pu8User[0]))
            {
                pu8Pwd = net_ltrim(pu8Pwd);
                pu8Pwd = net_rtrim(pu8Pwd);

                if ((HI_NULL_PTR != pu8Pwd)
                    && (0 != pu8Pwd[0]))
                {
                    HI_OSAL_Snprintf(url, urllen, "ftp://%s:%s@%s:%d/%s",
                                     pu8User, pu8Pwd, pcHost, u16Port, pcFileName);
                }
                else
                {
                    HI_OSAL_Snprintf(url, urllen, "ftp://%s@%s:%d/%s",
                                     pu8User, pcHost, u16Port, pcFileName);
                }
            }
            else
            {
                HI_OSAL_Snprintf(url, urllen, "ftp://%s:%d/%s", pcHost, u16Port, pcFileName);
            }

            break;
        }

        case HI_LOADER_IPPROT_HTTP:
        {
            if (0 == u16Port)
            {
                u16Port = IPDL_DEFAULT_IPPROT_HTTP;
            }

            HI_OSAL_Snprintf(url, urllen, "http://%s:%d/%s",
                             pcHost, u16Port, pcFileName);
            break;
        }

        default:
        {
            COM_PrintErrCode(COM_ERR_UNSUPPORTED);
            return COM_ERR_UNSUPPORTED;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 InitIP(HI_LOADER_PARAM_IP_S* pstIpParam)
{
    HI_U32  u32Loop = 0;
    HI_CHAR acDevName[IPDL_LEN_DEVICENAME] = {0};
    IPDL_LINKSTATUS_E enLinkStatus = IPDL_LINKSTATUS_OFF;
    HI_S32 s32Ret = HI_FAILURE;

    for (u32Loop = 0; u32Loop < IPDL_NUM_ETHDEVICE; u32Loop++ )
    {
        HI_OSAL_Snprintf(acDevName, sizeof(acDevName), "eth%d", u32Loop);

        switch (pstIpParam->enIPCfgType )
        {
            case HI_LOADER_IPCFG_STATIC:
            {
                s32Ret = InitStaticIP(acDevName,
                                      pstIpParam->ipOurIp,
                                      pstIpParam->ipSubnetMask,
                                      pstIpParam->ipGateway);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_PrintFuncErr(InitStaticIP, s32Ret);
                    continue;
                }

                break;

            }

            case HI_LOADER_IPCFG_DHCP:
            {
                s32Ret = InitDynamicIP(acDevName);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_PrintFuncErr(InitDynamicIP, s32Ret);
                    continue;
                }

                break;
            }

            default:
            {
                COM_PrintErrCode(COM_ERR_UNSUPPORTED);
                return COM_ERR_UNSUPPORTED;
            }
        }

        s32Ret = net_GetLinkStatus(acDevName, &enLinkStatus);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(net_GetLinkStatus, s32Ret);
            net_device_down(u32Loop);
            continue;
        }

        if (IPDL_LINKSTATUS_ON == enLinkStatus )
        {
            COM_DBG_FuncExit();
            return HI_SUCCESS;
        }

        net_device_down(u32Loop);
    }

    COM_PrintErrCode(COM_ERR_NETWORK);
    return COM_ERR_NETWORK;
}

static size_t WriteFunction(HI_CHAR* ptr, size_t size, size_t nmemb, HI_VOID* stream)
{
    HI_U32 u32Size = 0;

    u32Size = (HI_U32)(size * nmemb);

    if (HI_NULL_PTR == s_pu8Buffer)
    {
        COM_PrintErrCode(CURLE_WRITE_ERROR);
        return CURLE_WRITE_ERROR;
    }

    COM_MemCpy(s_pu8Buffer + s_u32DataLen, ptr, u32Size);
    s_u32DataLen += u32Size;

    return u32Size;
}

static size_t TftpWriteFunction(HI_CHAR* ptr, size_t size, size_t nmemb, HI_VOID* stream)
{
    HI_U32 u32Size = 0;

    u32Size = (HI_U32)(size * nmemb);

    if (IPDL_SIZE_BUFFER < u32Size)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return CURLE_WRITE_ERROR;
    }

    COM_MemCpy(s_pu8Buffer, ptr, u32Size);
    s_u32TotalDL += u32Size;

    s_u32DataLen = u32Size;
    s_u32ReadPos = 0;

    return u32Size;
}

static HI_S32 ErrorConvert(CURLcode code)
{
    switch (code)
    {
        case CURLE_COULDNT_CONNECT:
        {
            return COM_ERR_CONNECT_FAIL;
        }

        case CURLE_OPERATION_TIMEDOUT:
        {
            return COM_ERR_DOWNLOAD_TIMEOUT;
        }

        case CURLE_REMOTE_FILE_NOT_FOUND:
        {
            return COM_ERR_IMAGE_NOEXIST;
        }

        default:
        {
            return COM_ERR_NETWORK;
        }
    }
}

static HI_VOID ShowIPParam(HI_LOADER_PARAM_IP_S* pstIpParam)
{
    HI_CHAR acAddrBuf[IPDL_LEN_IPADDR] = {0};

    COM_DEBUG("*************** ip upgrade parameter *******************\n");
    COM_DEBUG("protocol_type(0:TFTP, 1:FTP, 2:HTTP)    :  %d \n", pstIpParam->enProtType);
    COM_DEBUG("ip_get_way(0:static, 1:dynamic)         :  %d \n", pstIpParam->enIPCfgType);

    if (HI_LOADER_IPCFG_STATIC == pstIpParam->enIPCfgType)
    {
        ip2string(pstIpParam->ipOurIp, acAddrBuf);
        COM_DEBUG("local_ip                                :  %s \n", acAddrBuf);
        ip2string(pstIpParam->ipGateway, acAddrBuf);
        COM_DEBUG("gateway_ip                              :  %s \n", acAddrBuf);
        ip2string(pstIpParam->ipSubnetMask, acAddrBuf);
        COM_DEBUG("netmask                                 :  %s \n", acAddrBuf);
    }

    ip2string(pstIpParam->ipServer, acAddrBuf);
    COM_DEBUG("server_ip                               :  %s \n", acAddrBuf);
    COM_DEBUG("server_port                             :  %d \n", pstIpParam->ipServerPort);
    COM_DEBUG("********************** end *****************************\n");

    return;
}

static HI_S32 ipdlTFTPGetData(HI_VOID* pBuffer, HI_U32 u32GetSize, HI_U32* pu32RetSize)
{
    HI_U32 u32ReadCount = 0;
    HI_U32 u32LeftCount = 0;
    HI_S32 s32Handles = 0;
    HI_S32 s32MsgCount = 0;;
    CURLMsg* pstMsg = HI_NULL_PTR;
    CURLMcode mCode = CURLM_OK;

    COM_DBG_FuncEnter();

    u32LeftCount = s_u32DataLen - s_u32ReadPos;

    if (0 < u32LeftCount)
    {
        if (u32GetSize <= u32LeftCount)
        {
            COM_MemCpy(pBuffer, s_pu8Buffer + s_u32ReadPos, u32GetSize);
            s_u32ReadPos += u32GetSize;

            *pu32RetSize = u32GetSize;
            return HI_SUCCESS;
        }

        COM_MemCpy(pBuffer, s_pu8Buffer + s_u32ReadPos, u32LeftCount);
        u32ReadCount = u32LeftCount;
        s_u32ReadPos = s_u32DataLen;
    }

    while (u32GetSize > u32ReadCount)
    {
        mCode = curl_multi_perform(s_hMultiHandle, &s32Handles);
        if (CURLM_OK != mCode)
        {
            if (CURLM_CALL_MULTI_PERFORM != mCode)
            {
                COM_PrintFuncErr(curl_multi_perform, mCode);
                return COM_ERR_NETWORK;
            }
        }

        if (s_u32ReadPos != s_u32DataLen)
        {
            if (u32GetSize - u32ReadCount <= s_u32DataLen)
            {
                COM_MemCpy(pBuffer + u32ReadCount, s_pu8Buffer, u32GetSize - u32ReadCount);
                s_u32ReadPos += u32GetSize - u32ReadCount;

                *pu32RetSize = u32GetSize;
                return HI_SUCCESS;
            }
            else
            {
                COM_MemCpy(pBuffer + u32ReadCount, s_pu8Buffer, s_u32DataLen);
                u32ReadCount += s_u32DataLen;
                s_u32ReadPos  = s_u32DataLen;
            }
        }

        if (0 == s32Handles)
        {
            pstMsg = curl_multi_info_read(s_hMultiHandle, &s32MsgCount);
            if (HI_NULL_PTR != pstMsg)
            {
                if ((CURLMSG_DONE == pstMsg->msg)
                    && (CURLE_OK == pstMsg->data.result))
                {
                    *pu32RetSize = u32ReadCount;

                    COM_DBG_FuncExit();
                    return HI_SUCCESS;
                }

                COM_PrintErrCode(pstMsg->data.result);
                return ErrorConvert(pstMsg->data.result);
            }

            COM_PrintErrCode(COM_ERR_NETWORK);
            return COM_ERR_NETWORK;
        }
    }

    COM_PrintErrCode(COM_ERR_PARAM);
    return COM_ERR_PARAM;
}

static HI_S32 ipdlGeneralGetData(HI_VOID* pBuffer, HI_U32 u32GetSize, HI_U32* pu32RetSize)
{
    HI_S8 as8Range[IPDL_LEN_CURLOPT] = {0};
    CURLcode eCode = CURLM_OK;

    COM_DBG_FuncEnter();

    static HI_U32 u32Offset = 0;

    s_pu8Buffer = (HI_U8*)pBuffer;
    s_u32DataLen = 0;

    HI_OSAL_Snprintf(as8Range, sizeof(as8Range), "%d-%d", u32Offset, u32Offset + u32GetSize - 1);

    eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_RANGE, as8Range);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_easy_setopt, eCode);
        return ErrorConvert(eCode);
    }

    eCode = curl_easy_perform(s_hEasyHandle);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_easy_perform, eCode);
        return ErrorConvert(eCode);
    }

    u32Offset   += s_u32DataLen;
    *pu32RetSize = s_u32DataLen;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_IP_Init(HI_LOADER_PARAM_S* pstLoaderParam)
{
    HI_S8 as8URL[IPDL_LEN_URL] = {0};
    HI_CHAR acAddrBuf[IPDL_LEN_IPADDR] = {0};
    HI_LOADER_PARAM_IP_S* pstIpParam = HI_NULL_PTR;

    CURLcode eCode = CURLE_OK;
    CURLMcode mCode = CURLM_OK;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    if (HI_TRUE == s_bInited)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    pstIpParam = &pstLoaderParam->unParam.stIPParam;

    ShowIPParam(pstIpParam);

    s32Ret = InitIP(pstIpParam);
    if ( HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(InitIP, s32Ret);
        return s32Ret;
    }

    ip2string(pstIpParam->ipServer, acAddrBuf);

    s32Ret = get_url(pstIpParam->enProtType,
                     pstIpParam->as8FileName,
                     acAddrBuf,
                     pstIpParam->ipServerPort,
                     (HI_U8*)pstIpParam->as8UserName,
                     (HI_U8*)pstIpParam->as8Password,
                     as8URL,
                     sizeof(as8URL));

    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(get_url, mCode);
        return s32Ret;
    }

    eCode = curl_global_init(CURL_GLOBAL_ALL);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_global_init, eCode);
        return COM_ERR_IPCONFIG;
    }

    s_hEasyHandle = curl_easy_init();
    if (HI_NULL_PTR == s_hEasyHandle)
    {
        COM_PrintFuncErr(curl_easy_init, COM_ERR_IPCONFIG);
        return COM_ERR_IPCONFIG;
    }

    eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_URL, as8URL);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_easy_setopt, eCode);
        return COM_ERR_IPCONFIG;
    }

    eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_HEADER, 0L);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_easy_setopt, eCode);
        return COM_ERR_IPCONFIG;
    }

    eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_CONNECTTIMEOUT, 10L);
    if (CURLE_OK != eCode)
    {
        COM_PrintFuncErr(curl_easy_setopt, eCode);
        return COM_ERR_IPCONFIG;
    }

    switch (pstIpParam->enProtType)
    {
        case HI_LOADER_IPPROT_TFTP:
        {
            s_pu8Buffer = COM_MemMalloc(IPDL_SIZE_BUFFER);
            if (HI_NULL_PTR == s_pu8Buffer)
            {
                COM_PrintErrCode(COM_ERR_MEM);
                return COM_ERR_MEM;
            }

            s_hMultiHandle = curl_multi_init();
            if (HI_NULL_PTR == s_hMultiHandle)
            {
                COM_PrintFuncErr(curl_multi_init, COM_ERR_IPCONFIG);
                COM_MemFreeSetNull(s_pu8Buffer);
                return COM_ERR_IPCONFIG;
            }

            mCode = curl_multi_add_handle(s_hMultiHandle, s_hEasyHandle);
            if (CURLM_OK != mCode)
            {
                COM_PrintFuncErr(curl_multi_add_handle, mCode);
                COM_MemFreeSetNull(s_pu8Buffer);
                return COM_ERR_IPCONFIG;
            }

            eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_WRITEFUNCTION, TftpWriteFunction);
            if (CURLE_OK != eCode)
            {
                COM_PrintFuncErr(curl_easy_setopt, eCode);
                COM_MemFreeSetNull(s_pu8Buffer);
                return COM_ERR_IPCONFIG;
            }

            break;
        }

        case HI_LOADER_IPPROT_FTP:
        case HI_LOADER_IPPROT_HTTP:
        {
            eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_WRITEFUNCTION, WriteFunction);
            if (CURLE_OK != eCode)
            {
                COM_PrintFuncErr(curl_easy_setopt, eCode);
                return COM_ERR_IPCONFIG;
            }

            eCode = curl_easy_setopt(s_hEasyHandle, CURLOPT_TIMEOUT, 10L);
            if (CURLE_OK != eCode)
            {
                COM_PrintFuncErr(curl_easy_setopt, eCode);
                return COM_ERR_IPCONFIG;
            }

            break;
        }

        default:
        {
            COM_PrintErrCode(COM_ERR_UNSUPPORTED);
            return COM_ERR_UNSUPPORTED;
        }
    }

    s_enProtType    = pstIpParam->enProtType;
    s_bInited       = HI_TRUE;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_IP_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    curl_easy_cleanup(s_hEasyHandle);
    curl_global_cleanup();

    s_enProtType = HI_LOADER_IPPROT_BUTT;

    if (HI_LOADER_IPPROT_TFTP == s_enProtType)
    {
        curl_multi_remove_handle(s_hMultiHandle, s_hEasyHandle);
        curl_multi_cleanup(s_hMultiHandle);
        COM_MemFree(s_pu8Buffer);

        s_hMultiHandle  = HI_NULL_PTR;
        s_u32ReadPos    = 0;

    }

    s_enProtType    = HI_LOADER_IPPROT_BUTT;
    s_hEasyHandle   = HI_NULL_PTR;

    s_u32DataLen    = 0;
    s_u32TotalDL    = 0;
    s_pu8Buffer     = HI_NULL_PTR;

    s_bInited = HI_FALSE;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_IP_Getdata(HI_VOID* pBuffer, HI_U32 u32GetSize, HI_U32* pu32RetSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32RetSize);
    COM_CHECK_PARAM(0 == u32GetSize);

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    if (HI_LOADER_IPPROT_TFTP == s_enProtType)
    {
        s32Ret = ipdlTFTPGetData(pBuffer, u32GetSize, pu32RetSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ipdlTFTPGetData, s32Ret);
            return s32Ret;
        }
    }
    else if ((HI_LOADER_IPPROT_FTP == s_enProtType)
             || (HI_LOADER_IPPROT_HTTP == s_enProtType))
    {
        s32Ret = ipdlGeneralGetData(pBuffer, u32GetSize, pu32RetSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ipdlGeneralGetData, s32Ret);
            return s32Ret;
        }
    }
    else
    {
        COM_PrintErrCode(COM_ERR_UNSUPPORTED);
        return COM_ERR_UNSUPPORTED;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_IP_GetFileSize(HI_U32* pu32FileSize)
{
    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu32FileSize);

    if (HI_TRUE != s_bInited)
    {
        COM_PrintErrCode(COM_ERR_UNINITED);
        return COM_ERR_UNINITED;
    }

    COM_PrintErrCode(COM_ERR_UNSUPPORTED);
    return COM_ERR_UNSUPPORTED;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

