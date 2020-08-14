/******************************************************************************

Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : openssl_adp.h
Version       : Initial
Author        : Hisilicon security software group
Created       : 2016-4-13
Last Modified :
Description   : OpenSSL Adapte API declaration
Function List :
History       :
* Version   Date         Author     DefectNum    Description
* main\1    2016-4-13                NULL      Create this file.
******************************************************************************/

#ifndef _OPENSSL_ADP_H_
#define _OPENSSL_ADP_H_

#include "hi_type.h"
#include "openssl/ssl.h"


//#define USE_SSL_V2
//#define USE_SSL_V3
//#define USE_TLS_V1
//#define USE_TLS_V1_1
#define USE_TLS_V1_2

typedef enum hiSSL_VERSION_TYPE_E
{
   SSL_VERSION_SSL_V2 = 1,
   SSL_VERSION_SSL_V3,
   SSL_VERSION_TLS_V1,
   SSL_VERSION_TLS_V1_1,
   SSL_VERSION_TLS_V1_2,
}Hi_SSL_VERSION_TYPE_E;

typedef int (*PEM_PASSWORD_CB)(char *buf, int size, int rwflag, void *userdata);


typedef struct hiSSL_INIT_PARAMETERS
{
    HI_CHAR *rootCaCertPath;             /* path for root CA cert */
    HI_BOOL  bIdentityAuth;              /* Do identity Authentication */
    HI_CHAR *certPath;                   /* path for certificate */
    HI_CHAR *privKeyPath;                /* path for private keys */
    PEM_PASSWORD_CB       funPasswdCb;  /* For crypto-text private key. set to NULL if no password */
    Hi_SSL_VERSION_TYPE_E eSSLVersion;   /*SSL Version Type*/
    HI_BOOL  bIsServer;                  /*Is Server*/
} Hi_SSL_INIT_PARAMETERS;


typedef struct hiOPENSSL_CTX_S
{
    HI_VOID     *ctx;
    HI_VOID     *meth; 
    HI_BOOL     bIsServer;     /*Is Server*/
} HI_OPENSSL_CTX_S;


typedef HI_VOID* HI_OPENSSL_HANDLE;

HI_S32 HI_OpenSSL_Init(Hi_SSL_INIT_PARAMETERS *pInitParamers, HI_OPENSSL_CTX_S *pSslCtx);

HI_VOID HI_OpenSSL_UnInit(HI_OPENSSL_CTX_S *pSslCtx);

HI_S32 HI_OpenSSL_SessionOpen(HI_S32 sd, HI_OPENSSL_CTX_S *pSslCtx, HI_OPENSSL_HANDLE *pSSLHandle); 

HI_S32 HI_OpenSSL_SessionClose(HI_OPENSSL_HANDLE sslHandle);

HI_S32 HI_OpenSSL_SocketRead(HI_OPENSSL_HANDLE sslHandle, HI_CHAR *DataBuf, HI_S32 Datalen);

HI_S32 HI_OpenSSL_SocketWrite(HI_OPENSSL_HANDLE sslHandle, HI_CHAR *DataBuf, HI_S32 DataLen);

HI_S32 HI_OpenSSL_SessionOpenEx(HI_S32 sd, HI_OPENSSL_HANDLE SrcSslHandle, HI_OPENSSL_HANDLE *pNewSSLHandle); 

HI_S32 HI_OpenSSL_SessionSuspend(HI_OPENSSL_HANDLE sslHandle);

#endif
