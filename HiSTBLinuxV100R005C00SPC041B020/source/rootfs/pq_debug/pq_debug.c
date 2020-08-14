/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : pq_debug.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2013/12/13
  Description   : PQ Debug socket server functions .
  History       :
    Date        : 2013/12/13
    Author      : p00203646
    Modification: Created.
******************************************************************************/

#include "pq_comm_def.h"
#include "drv_pq_define.h"
#include "hi_mpi_mem.h"
#include <arpa/inet.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/*-----------------------------------------------------------------------------*/
/* Extern Function Prototype                                                   */
/*-----------------------------------------------------------------------------*/
/* PQ Debug模块初始化 */
extern HI_S32        PQ_DRV_Init(HI_VOID);

/* PQ Debug模块去初始化 */
extern HI_S32        PQ_DRV_Deinit(HI_VOID);
/* PQ Debug 解析命令 */
extern HI_S32 PQ_Parse_ProcessCmd(HI_S32 client_sockfd, HI_U8* pu8Buf);

/* 接收数据*/
extern HI_S32 PQ_Parse_RecvData(HI_S32 client_sockfd, HI_U8* pu8Buf, HI_U32 u32Count, HI_S32* ps32ReadSize);

/* 发送错误标志*/
extern HI_S32 PQ_Parse_SendErrorFlag(HI_S32 client_sockfd, HI_U8 error);

/*-----------------------------------------------------------------------------*/
/* Local Types Declarations                                                    */
/*-----------------------------------------------------------------------------*/
static HI_S32 sg_s32ServerSockfd;
static HI_S32 sg_netprocess_flag = 1;
static HI_S32 sg_s32port = 4321;
static HI_CHAR  sg_sVersion[MAXLEN] = PQ_VERSION;
static HI_CHAR  sg_sMagicNum[MAXLEN] = PQ_MAGIC_NUM;
static HI_CHAR  sg_sChipName[MAXLEN] = PQ_CHIP_NAME;


/*-----------------------------------------------------------------------------*/
/* Local Function Declarations                                                    */
/*-----------------------------------------------------------------------------*/
HI_S32 PQ_DBG_Getcfg(const HI_CHAR* sFileName);
HI_S32 PQ_DBG_ServerInit(HI_VOID);
HI_S32 PQ_DBG_CheckMagic(HI_U8* au8MagicNum);
HI_S32 PQ_DBG_CheckChip(HI_U8* au8Chip);
HI_S32 PQ_DBG_CheckVersion(HI_U8* pu8Version);
HI_S32 PQ_DBG_CheckData(HI_S32 client_sockfd, HI_U8* pu8Buf);
HI_S32 PQ_DBG_ServerStart(HI_VOID);
static HI_S32 DBG_GetIniItem(const HI_CHAR* sFileName, const HI_CHAR* sSec, const HI_CHAR* sItem, HI_CHAR* sVal);
static HI_S32 DBG_IsEmpty(const HI_CHAR* str, HI_S32 len);
static HI_VOID DBG_Destroy(HI_S32 sig);
static HI_VOID DBG_TrimStr( HI_CHAR* s );
static HI_VOID* DBG_NetProcess(HI_VOID* args);
static HI_S32 DBG_CreateThread(HI_S32 s32Fd);


/*-----------------------------------------------------------------------------*/
/* Local Function Protype                                                      */
/*-----------------------------------------------------------------------------*/

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 s32Ret;

    /*1. pq 设备初始化*/
    s32Ret = PQ_DRV_Init();

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("HI_PQ_DBG_Init failed\n");
        exit(0);
    }

    /*2. 读取配置文件*/
    s32Ret = PQ_DBG_ServerInit();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("init server failed!\n");
        return HI_FAILURE;
    }

    /*3. 入参检测*/
    if (argc > 1)
    {
        if (strncmp(argv[1], "-v", strlen("-v")) == 0)
        {
            printf("Version:%s\n",  sg_sVersion);
            exit(0);
        }
        if (strncmp(argv[1], "-info", strlen("-info")) == 0)
        {
            printf("Port: %d\n", sg_s32port);
            printf("Version: %s\n",  sg_sVersion);
            printf("ChipName: %s\n",  sg_sChipName);
            printf("MagicNum: %s\n",  sg_sMagicNum);
            exit(0);
        }
        else if (strncmp(argv[1], "-test", strlen("-test")) == 0)
        {
            exit(0);
        }
    }

    /*4. 启动server*/
    s32Ret = PQ_DBG_ServerStart();
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("start server failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 PQ_DBG_Getcfg(const HI_CHAR* sFileName)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_CHAR sTmp[256];

    //get port
    memset(sTmp, 0, sizeof(sTmp));
    s32Ret = DBG_GetIniItem(sFileName, "default", "port", sTmp);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("get port failed!\n");
        return HI_FAILURE;
    }
    sg_s32port = atoi((HI_CHAR*)sTmp);
    HI_INFO_PQ("get port [%s]\n", sTmp);

    //get version
    memset(sTmp, 0, sizeof(sTmp));
    s32Ret = DBG_GetIniItem(sFileName, "default", "version", sTmp);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("get version failed!\n");
        return HI_FAILURE;
    }
    memset(sg_sVersion, 0, sizeof(sg_sVersion));
    strncpy(sg_sVersion, sTmp, sizeof(sg_sVersion) - 1);
    HI_INFO_PQ("get version [%s]\n", sg_sVersion);

    //get magicnum
    memset(sTmp, 0, sizeof(sTmp));
    s32Ret = DBG_GetIniItem(sFileName, "default", "magicnum", sTmp);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("get magicnum failed!\n");
        return HI_FAILURE;
    }
    memset(sg_sMagicNum, 0, sizeof(sg_sMagicNum));
    strncpy(sg_sMagicNum, sTmp, sizeof(sg_sMagicNum) - 1);
    HI_INFO_PQ("get magicnum [%s]\n", sg_sMagicNum);

    //get chip name
    memset(sTmp, 0, sizeof(sTmp));
    s32Ret = DBG_GetIniItem(sFileName, "default", "chipname", sTmp);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("get chipname failed!\n");
        return HI_FAILURE;
    }
    memset(sg_sChipName, 0, sizeof(sg_sChipName));
    strncpy(sg_sChipName, sTmp, sizeof(sg_sChipName) - 1);
    HI_INFO_PQ("get chipname [%s]\n", sg_sChipName);

    return HI_SUCCESS;
}

HI_S32 PQ_DBG_ServerInit(HI_VOID)
{
#if 0
    HI_S32 s32Ret;

    /*读取配置文件*/
    s32Ret = PQ_DBG_Getcfg(PQ_CFG_FILE);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PRINT( HI_ID_PQ,  "get cfg file [%s] failed!\n", PQ_CFG_FILE);
        return HI_FAILURE;
    }
#endif
    signal(SIGINT, DBG_Destroy);
    signal(SIGTERM, DBG_Destroy);
    signal(SIGPIPE, SIG_IGN);
    sg_netprocess_flag = 1;

    return HI_SUCCESS;
}


HI_S32 PQ_DBG_CheckMagic(HI_U8* au8MagicNum)
{
    CHECK_PTR_REINT(au8MagicNum);

    if (0 != strncmp((char*)au8MagicNum, sg_sMagicNum, strlen(sg_sMagicNum)))
    {
        HI_ERR_PQ("check magic number[%s] error!\n", (char*)au8MagicNum);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_DBG_CheckChip(HI_U8* au8Chip)
{
    CHECK_PTR_REINT(au8Chip);

    if (0 != strncmp((char*)au8Chip, sg_sChipName, strlen(sg_sChipName)))
    {
        HI_ERR_PQ("check chip[%s] error!\n", (char*)au8Chip);
        return HI_FAILURE;
    }

    if (strlen((char*)au8Chip) != strlen(sg_sChipName))
    {
        HI_ERR_PQ("check chip[%s] error!\n", (char*)au8Chip);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_DBG_CheckVersion(HI_U8* pu8Version)
{
    CHECK_PTR_REINT(pu8Version);

    HI_S32  s32Len = strlen(sg_sVersion);
    HI_CHAR chVersion[s32Len + 1] ;

    memset(chVersion, 0, sizeof(chVersion));
    memcpy(chVersion, sg_sVersion, s32Len);

    if ((pu8Version[1] == '.') \
        && (pu8Version[3] == '.') \
        && (pu8Version[5] == '.'))
    {
        if ( (pu8Version[0] > chVersion[0]) \
             || (pu8Version[0] == chVersion[0] && pu8Version[2] > chVersion[2]) \
             || (pu8Version[0] == chVersion[0] && pu8Version[2] == chVersion[2] && pu8Version[4] > chVersion[4])\
             || (pu8Version[0] == chVersion[0] && pu8Version[2] == chVersion[2] && pu8Version[4] == chVersion[4] && pu8Version[6] > chVersion[6]))
        {
            HI_ERR_PQ("check version[%s] error!\n", (char*)pu8Version);
            return HI_FAILURE;
        }

        if ( (pu8Version[0] > '9' || pu8Version[0] < '0') \
             || (pu8Version[2] > '9' || pu8Version[2] < '0') \
             || (pu8Version[4] > '9' || pu8Version[4] < '0') \
             || (pu8Version[6] > '9' || pu8Version[6] < '0') )
        {
            HI_ERR_PQ("check version[%s] error!\n", (char*)pu8Version);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_PQ("check version[%s] error!\n", (char*)pu8Version);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 PQ_DBG_CheckData(HI_S32 client_sockfd, HI_U8* pu8Buf)
{
    HI_S32 s32Ret;
	
    if (HI_NULL == pu8Buf)
    {
        return HI_FAILURE;
    }

    /*1.check magic data*/
    if (HI_SUCCESS != PQ_DBG_CheckMagic(((HI_MDBG_MSG_S*)pu8Buf)->au8MagicNum))
    {
        s32Ret = PQ_Parse_SendErrorFlag(client_sockfd, HI_MAGIC_ERROR);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("PQ_Parse_SendErrorFlag error\n");
        }
		
        return HI_FAILURE;
    }

    /*2.check chip version*/
    if (HI_SUCCESS != PQ_DBG_CheckChip(((HI_MDBG_MSG_S*)pu8Buf)->au32ChipVersion))
    {
        s32Ret = PQ_Parse_SendErrorFlag(client_sockfd, HI_CHIP_ERROR);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("PQ_Parse_SendErrorFlag error\n");
        }
		
        return HI_FAILURE;
    }
	
    /*3.check sdk version*/
    if (HI_SUCCESS != PQ_DBG_CheckVersion(((HI_MDBG_MSG_S*)pu8Buf)->au32PQVersion))
    {
        s32Ret = PQ_Parse_SendErrorFlag(client_sockfd, HI_VERSION_ERROR);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("PQ_Parse_SendErrorFlag error\n");
        }
		
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32  PQ_DBG_ServerStart(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 client_len;
    HI_S32 sg_s32NetDevFd;
    struct sockaddr_in addr_serv, addr_client;
    sg_s32ServerSockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sg_s32ServerSockfd < 0)
    {
        HI_ERR_PQ("socket fail");
        return HI_FAILURE;
    }
    else
    {
        HI_INFO_PQ("socket sucessful!\n");
    }

    memset(&addr_serv, 0, sizeof(addr_serv));
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(sg_s32port);
    addr_serv.sin_addr.s_addr =  htonl(INADDR_ANY);
    client_len = sizeof(struct sockaddr_in);
    if (bind(sg_s32ServerSockfd, (struct sockaddr*)&addr_serv, sizeof(struct sockaddr_in)) < 0)
    {
        HI_ERR_PQ("bind fail\n");
    }
    else
    {
        HI_INFO_PQ("bind sucessful!\n");
    }

    if (listen(sg_s32ServerSockfd, BACKLOG) < 0)
    {
        HI_ERR_PQ("listen fail\n");
    }
    else
    {
        HI_INFO_PQ("listen sucessful!\n");
    }

    while (sg_netprocess_flag == 1)
    {
        sg_s32NetDevFd = accept(sg_s32ServerSockfd, (struct sockaddr*)&addr_client, (socklen_t*)&client_len);

        if (sg_s32NetDevFd < 0)
        {
            HI_ERR_PQ("accept fail\n");
            continue;
        }
        HI_INFO_PQ("accept a new client,ip:%s\n", inet_ntoa(addr_client.sin_addr));

        s32Ret = DBG_CreateThread(sg_s32NetDevFd);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("HI_UNF_MDBG_CreateThread error!\n");
            continue;
        }
    }
    return HI_SUCCESS;
}

static HI_VOID* DBG_NetProcess(HI_VOID* args)
{
    HI_S32 client_sockfd = 0;
    HI_U8* pPara = NULL;
    HI_S32 s32Ret = HI_FAILURE;

    if (args == NULL)
    {
        return NULL;
    }

    client_sockfd = *(HI_S32*)args;

    pPara = (HI_U8*)HI_MALLOC(HI_ID_PQ, CMDBUF_SIZE);
    if (NULL == pPara)
    {
        HI_ERR_PQ("MALLOC failed\n");
        return NULL;
    }
    memset(pPara, 0, CMDBUF_SIZE);

    while (1)
    {
        HI_S32 readesize = 0;

        /*1.接收数据*/
        s32Ret = PQ_Parse_RecvData(client_sockfd, pPara, CMDBUF_SIZE, &readesize);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("PQ_Parse_RecvData  failed!\n");
            break;
        }
        if (readesize <= 0)
        {
            HI_ERR_PQ("check  failed! [%i]\n", s32Ret);
            PQ_Parse_SendErrorFlag(client_sockfd, 0xE0);
            break;
        }

        /*2.校验数据*/
        s32Ret = PQ_DBG_CheckData(client_sockfd, pPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PQ("check  failed! [%i]\n", s32Ret);
            break;
        }

        /*3.解析数据*/
        s32Ret = PQ_Parse_ProcessCmd(client_sockfd, (HI_U8*)pPara);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PQ("Parser  failed!\n");
            PQ_Parse_SendErrorFlag(client_sockfd, HI_INVALID_ADDRESS_TYPE);
        }

        break;
    }

    HI_FREE(HI_ID_PQ, pPara);
    pPara = HI_NULL;
    if (client_sockfd > 0)
    {
        close(client_sockfd);
        client_sockfd = 0;
    }

    HI_INFO_PQ("connection close!\n");
    return HI_NULL;
}


static HI_S32 DBG_CreateThread(HI_S32 s32Fd)
{
    pthread_t thread;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32* pTmpFd = &s32Fd;

    pthread_attr_t child_thread_attr;
    pthread_attr_init(&child_thread_attr);
    pthread_attr_setdetachstate(&child_thread_attr, PTHREAD_CREATE_DETACHED);
    s32Ret = pthread_create(&thread, &child_thread_attr, DBG_NetProcess, (void*)pTmpFd ); //创建子线程
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PQ("pthread_create error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID DBG_Destroy(HI_S32 sig)
{
    sg_netprocess_flag = -1;

    shutdown(sg_s32ServerSockfd, 2);
    close(sg_s32ServerSockfd);

    HI_INFO_PQ("PQ_DRV_Deinit!\n");
    (HI_VOID)PQ_DRV_Deinit();

    exit(0);
}

static HI_S32 DBG_IsEmpty(const HI_CHAR* str, HI_S32 len)
{
    int i;
    if (*str == '\0')
    { return 0; }
    for (i = 0; i < len; i++)
    {
        if ((*(str + i) != ' ') && (*(str + i) != '\0') && (*(str + i) != '\t'))
        { return 1; }
    }
    return 0;
}


static HI_VOID DBG_TrimStr( HI_CHAR* s )
{
    if ( s == NULL )
    { return; }

    int  len = strlen( s );
    char* p = s;

    if ( strlen( p ) == 0 )
    { return; }

    while ( 1 )
    {
        if (( p[ len - 1 ] != 0x20 ) &&
            ( p[ len - 1 ] != '\t' ) &&
            ( p[ len - 1 ] != '\n' ) )
        {
            p[ len ] = 0x00;
            break;
        }
        len--;
    }

    while ( 1 )
    {
        if (( *p != 0x20 ) && ( *p != '\t' ))
        { break; }
        p++;
        len--;
    }

    strncpy(s, p, len + 1);
}


static HI_S32 DBG_GetIniItem(const HI_CHAR* sFileName, const HI_CHAR* sSec, const HI_CHAR* sItem, HI_CHAR* sVal)
{
    FILE* fp;
    HI_CHAR cLineBuf[2048], *cValBuf;
    HI_CHAR sTmp[2048];
    int iFindSec = 0;
    int i = 0;
    int rtnval;
    int iEndFlg = 0;

    if (strlen(sSec) > (200 - 3)) { return 1; }
    if ((fp = fopen(sFileName, "r")) == NULL)
    {
        HI_ERR_PQ("open file failed! [%s]\n", sFileName);
        return -1;
    }

    strncpy(sTmp, "[", sizeof(sTmp));
    strncat(sTmp, sSec, sizeof(sTmp));
    strncat(sTmp, "]", sizeof(sTmp));

    while (1)
    {
        if (iEndFlg)
        { break; }
        if (feof(fp))
        { iEndFlg = 1; }

        rtnval = fgetc(fp);
        if ((rtnval == EOF) || (rtnval == '\n') || feof(fp))
        {
            if (rtnval == EOF || feof(fp))
            { iEndFlg = 1; }

            if (i >= 1800 - 1)
            {
                while (!feof(fp) || rtnval == '\n')
                { rtnval = fgetc(fp); }
            }
            cLineBuf[i++] = 0x00;
            i = 0;
            if ((cValBuf = strchr(cLineBuf, 0x0d)) != NULL)
            { *cValBuf = '\0'; }

            if ((cValBuf = strchr(cLineBuf, '#')) != NULL)
            { *cValBuf = '\0'; }

            if (!DBG_IsEmpty(cLineBuf, strlen(cLineBuf)))
            { continue; }

            if (0 == strncmp(sTmp, cLineBuf, sizeof(sTmp)) )
            {
                iFindSec = 1;
                continue;
            }
            if (iFindSec == 0) { continue; }

            if (cLineBuf[0] == '[') { break; }

            if ((cValBuf = strchr(cLineBuf, '=')) == NULL) { continue; }
            *cValBuf = '\0';
            cValBuf++;
            DBG_TrimStr(cLineBuf);
            if (!strcmp(cLineBuf, sItem))
            {
                DBG_TrimStr(cValBuf);
                strncpy(sVal, cValBuf, 256);
                fclose(fp);
                return 0;
            }
        }
        else
        {
            cLineBuf[i++] = rtnval;
        }
    }

    fclose(fp);
    HI_ERR_PQ("not found: file=%s, section=[%s], Item=[%s]\n", sFileName, sSec, sItem);

    return HI_FAILURE;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
