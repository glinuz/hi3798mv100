/******************************************************************************

  Copyright (C), 2016-2026, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : openssl_server.c
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

#define TEST_SERVER_PORT 5000
#define MAX_BUF 1024

HI_S32 server_socket_select(int fd) 
{
    int rc = -1;
    fd_set rfds;
    struct timeval tv;
    int timeoutInterval = 200; /* 200msec */

    while (1) 
    {        
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        tv.tv_sec = 0; 
        tv.tv_usec = timeoutInterval * 1000; /* in micro-secs */

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



HI_S32 OpenSSL_ServerTest(HI_OPENSSL_CTX_S *pSslCtx)
{
    int ret;
    HI_S32 server_fd;
    HI_S32 new_fd; 
    HI_S32 len;    
    char   rev_buffer[MAX_BUF + 1];
    char   snd_buffer[MAX_BUF + 1];

    struct sockaddr_in localAddr;
    int reuse = 1;
    struct sockaddr_in remote_addr;
    int addrLen;
    HI_OPENSSL_HANDLE sslServer = HI_NULL;


    sample_printf("%s: creat socket \n", __FUNCTION__);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( server_fd < 0) 
    {
        /* Socket Create Error */
        perror("Socket Open Err \n");
        return -EINVAL;
    }
    
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0 ) 
    {
        sample_printf("REUSE socket error\n");
        return HI_FAILURE;
    }

    localAddr.sin_family = AF_INET;    
    localAddr.sin_port = htons(TEST_SERVER_PORT);
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    sample_printf("%s: bind local address to socket \n", __FUNCTION__);
    if (bind(server_fd, (struct sockaddr *) &localAddr, sizeof(localAddr))) 
    {
        sample_printf("Socket bind error");
        return HI_FAILURE;
    }

    sample_printf("%s: start listen \n", __FUNCTION__);
    if (listen(server_fd, 4)) 
    {
        sample_printf("Socket listen error");
        return HI_FAILURE;
    }  

    sample_printf( "%s: Waiting for connection... \n", __FUNCTION__);
    
    if (server_socket_select(server_fd))
    {
       sample_printf("Fail to Waiting for connection");
       return HI_FAILURE;
    }

    addrLen = sizeof(remote_addr);
    if ((new_fd = accept(server_fd, (struct sockaddr *)&remote_addr, (socklen_t *)&addrLen)) == -1)
    {
       sample_printf("Socket accept error \n");
       return HI_FAILURE; 
    }

    sample_printf("one connection accepted from %ulx:%d \n", remote_addr.sin_addr.s_addr, ntohs(remote_addr.sin_port));
    
    ret = HI_OpenSSL_SessionOpen(new_fd, pSslCtx, &sslServer);
    if (ret != HI_SUCCESS)
    {
       sample_printf("%s: Fail to open ssl session \n", __FUNCTION__);
       goto _ERR_EXIT_;
    }

    memset(snd_buffer, 0x0, sizeof(rev_buffer));
    strncpy(snd_buffer, "Server is OK!!!\n", sizeof(snd_buffer));
    len = HI_OpenSSL_SocketWrite(sslServer, snd_buffer, strlen(snd_buffer));
    if (len < 0)
    {
        sample_printf ("%s:Fail to send first confirm data\n",__FUNCTION__);
        goto _ERR_EXIT_;
    }
    sample_printf("=========================Server=====================================\n");   
    while(1)
    {
       if (server_socket_select(new_fd))
       {
         usleep(2*1000);
         continue;
       }       
       
       sample_printf("Waiting for receiving data from Client......\n");
       memset(rev_buffer, 0x0, sizeof(rev_buffer));
       len = HI_OpenSSL_SocketRead(sslServer, rev_buffer, MAX_BUF);
       if (len < 0 )
       {
          sample_printf ("%s:Fail to read data\n",__FUNCTION__);
          usleep(2*1000);
          continue;
       }
       sample_printf("Total %d bytes are received from client:\n", len);
       sample_printf("%s\n", rev_buffer);
       
       sample_printf("Received OK!!!!!\n");

       sample_printf("--------------------------------------------\n");
       sample_printf("Send ACK data to Client......\n");
       memset(snd_buffer, 0x0, sizeof(snd_buffer));
       strncpy(snd_buffer, rev_buffer, sizeof(snd_buffer));
       printf("%s\n",snd_buffer);
       len = HI_OpenSSL_SocketWrite(sslServer, snd_buffer, strlen(snd_buffer));
       if (len < 0)
       {
           sample_printf ("%s:Fail to send first confirm data\n",__FUNCTION__);
           usleep(2*1000);
           continue;
       }
       sample_printf("Send OK!!!!!\n");       
       sample_printf("=========================Server=====================================\n");   
    }


_ERR_EXIT_:

    if (sslServer != HI_NULL)
    {
        HI_OpenSSL_SessionClose(sslServer);
    }   

    close(new_fd);

    close(server_fd);
    
    return HI_SUCCESS;
}


