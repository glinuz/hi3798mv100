/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : openssl_client.c
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

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "hi_type.h"
#include "openssl_adp.h"

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define sample_printf(pszFormat...)
#else
#define sample_printf printf
#endif

#define TEST_CLIENT_PORT 5000

#define TEST_SERVER_PORT 5000



HI_S32 client_socket_select(int fd) 
{
    int rc = -1;
    fd_set rfds;
    struct timeval tv;
    int timeoutInterval = 200; 

    while (1) 
    {        
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        tv.tv_sec = 0; 
        tv.tv_usec = timeoutInterval * 1000; 

        rc = select(fd +1, &rfds, NULL, NULL, &tv);
        if (rc < 0) 
        {
            if (errno == EINTR) 
            {
                sample_printf("%s: select is interrupted, re-try\n", __FUNCTION__);
                continue;
            }
            sample_printf("%s: ERROR: select(): rc=%d, errno=%d", __FUNCTION__, rc, errno);
            return HI_FAILURE;
        }

        if (rc == 0 || !FD_ISSET(fd, &rfds)) 
        {
            continue;
        }
        
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

#define MAX_BUF 1024

HI_VOID OpenSSL_ClientTest(HI_OPENSSL_CTX_S *pSslCtx, char *pServerIpAddr)
{
    int ret;
    int socket_fd = -1;
    int len ;
    int reuse = 1;
    struct sockaddr_in localAddr;
    struct sockaddr_in serverAddr;    
    HI_OPENSSL_HANDLE sslHandle = HI_NULL;
    char buffer[MAX_BUF + 1];

    sample_printf("%s: creat socket \n", __FUNCTION__);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        sample_printf("%s: socket() error, ", __FUNCTION__ );
        goto _ERR_EXIT_;
    }

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0 ) 
    {
        sample_printf("REUSE socket error\n");
        goto _ERR_EXIT_;
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(TEST_CLIENT_PORT);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    sample_printf("%s: bind local address to socket \n", __FUNCTION__);
    if (bind(socket_fd, (struct sockaddr *)&localAddr, sizeof(localAddr))) 
    {
        sample_printf("%s: bind() error \n", __FUNCTION__);
        goto _ERR_EXIT_;
    }

    /*Init server address and port information*/
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TEST_SERVER_PORT);
    sample_printf("%s:Server ip addr is %s \n", __FUNCTION__,pServerIpAddr);
    if (inet_aton(pServerIpAddr, &serverAddr.sin_addr) == 0)
    {
        sample_printf("%s: inet_aton call error", __FUNCTION__);
        goto _ERR_EXIT_;;
    }
    
    sample_printf("%s: start to connect server\n", __FUNCTION__);
    /* Connect to server */
    if (connect(socket_fd, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) 
    {
        sample_printf("%s: TCP connect server Error \n", __FUNCTION__);
        goto _ERR_EXIT_;
    }
  
    sample_printf("%s: succeed in connection to server\n", __FUNCTION__);  
    
    ret = HI_OpenSSL_SessionOpen(socket_fd, pSslCtx, &sslHandle);
    if (ret != HI_SUCCESS)
    {
       sample_printf("%s: Fail to open ssl session \n", __FUNCTION__);
       goto _ERR_EXIT_;
    }

    if (client_socket_select(socket_fd))
    {
       printf ("%s:Fail to waiting data 1\n",__FUNCTION__);
       goto _ERR_EXIT_;
    }          
     
    memset(buffer, 0x0, sizeof(buffer));
    len = HI_OpenSSL_SocketRead(sslHandle, buffer, MAX_BUF);
    if (len < 0 )
    {
      printf ("%s:Fail to read data 1 \n",__FUNCTION__);
      goto _ERR_EXIT_;
    }
    sample_printf("%s\n", buffer);
    sample_printf("=========================Client=====================================\n");
    while(1)
    {
       
       sample_printf("Please input data to be sent to server:\n");      
       memset(buffer, 0x0, sizeof(buffer));
       scanf("%1024s", buffer);

       len = HI_OpenSSL_SocketWrite(sslHandle, buffer, strlen(buffer));
       if (len < 0)
       {
           printf ("%s:Fail to send data\n",__FUNCTION__);
           usleep(2*1000);
           continue;
       }
       sample_printf("Send OK!!!!!\n");

       sample_printf("--------------------------------------------\n");
       sample_printf("Waiting for receiving ACK data from Server......\n");
       if (client_socket_select(socket_fd))
       {
           usleep(2*1000);
           continue;
       }          
     
       memset(buffer, 0x0, sizeof(buffer));
       len = HI_OpenSSL_SocketRead(sslHandle, buffer, MAX_BUF);
       if (len < 0 )
       {
          printf ("%s:Fail to read data\n",__FUNCTION__);
          usleep(2*1000);
          continue;
       }
 
       printf("Total %d bytes are received:\n", len);
       printf("%s\n", buffer);
       sample_printf("=========================Client=====================================\n");
    }

_ERR_EXIT_:

    if (sslHandle != HI_NULL)
    {
        HI_OpenSSL_SessionClose(sslHandle);
    }
    
    if (socket_fd >= 0)
    {
        close(socket_fd);
    }
    
    return;
}


