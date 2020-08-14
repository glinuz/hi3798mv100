/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : openssl_server_test.c
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


extern HI_VOID OpenSSL_ClientTest(HI_OPENSSL_CTX_S *pSslCtx, char *pServerIpAddr);
extern HI_S32 OpenSSL_ServerTest(HI_OPENSSL_CTX_S *pSslCtx);

static int server_password_cb(char *buf, int num, int rwflag,void *userdata);

#define SERVER_PRIVATE_KEY_PASSWD "789456"

static void usage()
{
    printf("Usage: openssl_server_test\n");
    printf("Example:./openssl_server_test \n");
    return;
}
HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32      Ret;
	Hi_SSL_INIT_PARAMETERS InitParamers;
	HI_OPENSSL_CTX_S       SslCtx;	
	char RootCert[] = "./key/ca.crt";
    char ServerCert[] = "./key/server.crt";
    char ServerPrivatekey[] = "./key/server.key";
	
    if (argc > 1)
    {
        usage();
        return HI_FAILURE;
    }

    HI_SYS_Init();
    
    InitParamers.bIdentityAuth = HI_TRUE;
    InitParamers.rootCaCertPath = RootCert;

    InitParamers.certPath = ServerCert;
    InitParamers.privKeyPath = ServerPrivatekey;  
    InitParamers.eSSLVersion = SSL_VERSION_TLS_V1_2;
    InitParamers.funPasswdCb = server_password_cb;
    InitParamers.bIsServer = HI_TRUE;
    
    printf("Start open server side!!!!!!!!!\n");
    
    Ret = HI_OpenSSL_Init(&InitParamers, &SslCtx);
    if (Ret != HI_SUCCESS)
    {
        printf("Fail to call HI_OpenSSL_Init!!!!!!!!!\n");  
        goto SYS_DEINIT;
    }    

    OpenSSL_ServerTest(&SslCtx);    
    
    printf("SSL TEST END!!!!!!!!!\n");

SYS_DEINIT:
    
    HI_OpenSSL_UnInit(&SslCtx);
    HI_SYS_DeInit();     
    return Ret;
}

/*If the private key is encrypted, the call function will be called*/
static int server_password_cb(char *buf, int num, int rwflag,void *userdata)
{
    int len;
    
    //printf("%s: Enter this function  \n", __FUNCTION__);
    //printf("num= %d rwflag=%d\n",num,rwflag);
    //printf("userdata= %s\n",(char*)userdata);

    strncpy(buf, SERVER_PRIVATE_KEY_PASSWD, num);
    buf[num - 1] = '\0';

    len = strlen(SERVER_PRIVATE_KEY_PASSWD);
    return len;
}


