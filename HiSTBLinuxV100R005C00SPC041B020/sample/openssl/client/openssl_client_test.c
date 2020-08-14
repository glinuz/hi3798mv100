/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : openssl_client_test.c
  Version       : Initial Draft
  Author        : Hisilicon security software group
  Created       : 2016/04/13
  Description   :
  History       :
  1.Date        : 2016/04/13
    Author      : security team
    Modification: Created file
 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "openssl/ssl.h"
#include "openssl/err.h"

#include "hi_type.h"
#include "hi_common.h"
#include "openssl_adp.h"


static int client_password_cb(char *buf, int num, int rwflag,void *userdata);

extern HI_VOID OpenSSL_ClientTest(HI_OPENSSL_CTX_S *pSslCtx, char *pServerIpAddr);
extern HI_S32 OpenSSL_ServerTest(HI_OPENSSL_CTX_S *pSslCtx);

#define CLIENT_PRIVATE_KEY_PASSWD "789456"


static void usage()
{
    printf("Usage: openssl_test flag [server_ip_addr]\n"      
       "       ipaddress: e.g. 10.67.217.86\n");
    printf("Example :./openssl_test 10.67.217.86 \n");
    return;
}
HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32      Ret;
    //HI_CHAR   InputCmd[32];
    char        *pIpAddr = HI_NULL;
	Hi_SSL_INIT_PARAMETERS InitParamers;
	HI_OPENSSL_CTX_S       SslCtx;	
	char RootCert[] = "./key/ca.crt";
    char ClientCert[] = "./key/client.crt";
    char ClientPrivatekey[] = "./key/client.key";
	
    if (argc < 2 || HI_NULL == (char *)argv[1])
    {
        usage();
        return HI_FAILURE;
    }
  
    pIpAddr = (char *)argv[1];
 
    HI_SYS_Init();
    
    InitParamers.bIdentityAuth = HI_TRUE;
    InitParamers.rootCaCertPath = RootCert;

    InitParamers.certPath = ClientCert;
    InitParamers.privKeyPath = ClientPrivatekey;  
    InitParamers.eSSLVersion = SSL_VERSION_TLS_V1_2;
    InitParamers.funPasswdCb = client_password_cb;
    InitParamers.bIsServer = HI_FALSE;

    printf("Start open Client side!!!!!!!!!\n");
    
    Ret = HI_OpenSSL_Init(&InitParamers, &SslCtx);
    if (Ret != HI_SUCCESS)
    {
        printf("Fail to call HI_OpenSSL_Init!!!!!!!!!\n");  
        goto SYS_DEINIT;
    }
    

    OpenSSL_ClientTest(&SslCtx, pIpAddr);
    
    printf("SSL TEST END!!!!!!!!!\n");

SYS_DEINIT:
    
    HI_OpenSSL_UnInit(&SslCtx);
    HI_SYS_DeInit();     
    return Ret;
}

/*If the private key is encrypted, the call function will be called*/
static int client_password_cb(char *buf, int num, int rwflag,void *userdata)
{
    int len;
    
    //printf("%s: Enter this function  \n", __FUNCTION__);
    //printf("num= %d rwflag=%d\n",num,rwflag);
    //printf("userdata= %s\n",(char*)userdata);

    strncpy(buf, CLIENT_PRIVATE_KEY_PASSWD, num);
    buf[num - 1] = '\0';

    len = strlen(CLIENT_PRIVATE_KEY_PASSWD);
    return len;
}