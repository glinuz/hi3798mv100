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

#include "hi_type.h"
#include "hi_common.h"


#ifdef HI_ADVCA_FUNCTION_RELEASE
#define sample_printf(pszFormat...)
#else
#define sample_printf printf
#endif


#define SERVER_ADDR "192.168.1.2"
#define TEST_IPC_PORT 6000
#define MAX_BUF 1024

static HI_S32 client_socket_select(int fd) 
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

static int IPC_ClientConnect()
{
    int rc, i;
    struct sockaddr_in sock_addr;
    int socket_fd;
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
    {
        printf("%s:Fail to create socket\n",__FUNCTION__);
        return HI_FAILURE;
    }

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    
    inet_aton(SERVER_ADDR, &sock_addr.sin_addr);

    sock_addr.sin_port = htons(TEST_IPC_PORT);

    /*Note: probably server not build up, So client shall wait*/
    for (i=0; i<100; i++)
    {
        rc = connect(socket_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
        if (rc == 0) 
        {
            break;
        }

        usleep(100*1000);
    }

    if (i == 100)
    {
        printf("%s:Fail to connect socket\n",__FUNCTION__);
        close(socket_fd);
        return HI_FAILURE;
    }

    return socket_fd;
}

static int IPC_ClientGetTotalProgramNum(int socket_fd, HI_U32 *pu32TotalNumber)
{
   int len ;
   char buffer[512];
   HI_U32 *pu32Buf;
   
    memset(buffer, 0x0, sizeof(buffer));

    if (client_socket_select(socket_fd))
    {
       sample_printf("%s:Fail to waiting data 1\n",__FUNCTION__);
       return HI_FAILURE;
    }   
    
    len = read(socket_fd, buffer, MAX_BUF);
    if (len < 0 )
    {
      printf ("%s:Fail to read data 1 \n",__FUNCTION__);
      return HI_FAILURE;
    }
    
    pu32Buf = (HI_U32 *)buffer;
    sample_printf("%s: Total program number is %d\n", __FUNCTION__, *pu32Buf);

    if (pu32TotalNumber != HI_NULL)
    {
       *pu32TotalNumber = *pu32Buf;   
    }

    return HI_SUCCESS;
}

HI_S32 IPC_ClientTest()
{
    int socket_fd;
    int len ;
    HI_U32 u32TotalNumber=0;
    HI_U32 u32PlayNumber = 1;
    HI_U32 *pu32Buf;
    char buffer[MAX_BUF + 1];

    sample_printf("%s: Start connect server \n", __FUNCTION__);
    socket_fd = IPC_ClientConnect();
    if (socket_fd == HI_FAILURE)
    {
       sample_printf("%s:Fail to connect server\n",__FUNCTION__);
       return HI_FAILURE;
    }
    
    if (HI_SUCCESS != IPC_ClientGetTotalProgramNum(socket_fd, &u32TotalNumber))
    {
       sample_printf("Fail to get total program numbers\n");
       goto __ERROR_EXIT__;
    }
     
    
    sample_printf("********************Client********************\n");
    while(1)
    {       
       sample_printf("Client ask server to play program %d\n", u32PlayNumber);      
       memset(buffer, 0x0, sizeof(buffer));
       pu32Buf = (HI_U32 *)buffer;
       *pu32Buf = u32PlayNumber;       
       len = write(socket_fd, buffer, sizeof(u32PlayNumber));
       if (len < 0)
       {
           printf ("%s:Fail to send data\n",__FUNCTION__);
           usleep(2*1000);
           continue;
       }

       if (client_socket_select(socket_fd))
       {
           usleep(2*1000);
           continue;
       }          
     
       memset(buffer, 0x0, sizeof(buffer));
       len = read(socket_fd, buffer, MAX_BUF);
       if (len < 0 )
       {
          printf ("%s:Fail to read data\n",__FUNCTION__);
          usleep(2*1000);
          continue;
       }
 
       printf("%s\n", buffer);
       sample_printf("=========================Client=====================================\n");

       usleep(3*1000*1000);

       u32PlayNumber++;

       if (u32PlayNumber > u32TotalNumber)
       {
          u32PlayNumber = 1;
       }       
    }

__ERROR_EXIT__:
    if (socket_fd >= 0)
    {
        close(socket_fd);
    }  

    return HI_SUCCESS;
}


static void usage()
{
    printf("Usage: ./ipc_client_test\n");
    printf("Example :./ipc_client_test \n");
    return;
}
HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32      Ret;
	
    if (argc > 1)
    {
        usage();
        return HI_FAILURE;
    }
 
    IPC_ClientTest();
    
    //HI_SYS_DeInit();     
    return Ret;
}

