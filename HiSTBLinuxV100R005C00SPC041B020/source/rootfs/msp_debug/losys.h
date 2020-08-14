/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : logsys.h
  Version       : V1.0
  Author        : Hisilicon multimedia software group
  Created       : 
  Description   : interfaces in this file are used to debug, only output debug information, not
  				  effect on function;
  				  CNcomment:本文件实现的调试接口是为模块故障定位使用，用来向外部输出调试信息，
				  CNcomment:不对模块本身的功能造成任何影响，可以在模块内部使用宏定义为空的方式
				  CNcomment:将此屏蔽。可以产生32个级别的调试信息输出设定，使用UDP端口作为命令
				  CNcomment:接收端口，调试信息的打印格式如printf函数，前面添加级别设置的宏定义
				  CNcomment:控制输出。本模块为单任务模块，如果需要多任务调用DebugOutString需要
				  CNcomment:添加同步调试信息最大值不应超过500个字节，否则将出现致命错误--越界 
  History       :
  1.Date        : 
    Author      : rxc
    Modification: 

******************************************************************************/
#ifndef _LOSYS_H
#define _LOSYS_H

//#define LOW_WINDOWS_ON        1     //WINDOWS switch
#define LOW_LINUX_ON            1     //LINUX switch

/*****************************************************************************************
*                            head file                                                     * 
*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

#ifdef LOW_LINUX_ON
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/time.h>
    #include <netdb.h>
    #include <sys/ioctl.h>
    #include <net/if.h>
    #include <sys/utsname.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <fcntl.h>

    #define SOCKET int
#endif

#ifdef LOW_WINDOWS_ON
   #include <Winsock2.h>
/*****************************************************************************************
*                    define error CONST                                                   * 
*****************************************************************************************/
   #define ENOTSOCK           95     /* Socket operation on non-socket */
   #define EDESTADDRREQ       96     /* Destination address required */
   #define EMSGSIZE           97     /* Message too long */
   #define EPROTOTYPE         98     /* Protocol wrong type for socket */
   #define ENOPROTOOPT        99     /* Protocol not available */
   #define EPROTONOSUPPORT   120     /* Protocol not supported */   //
   #define ESOCKTNOSUPPORT   121     /* Socket type not supported */
   #define EOPNOTSUPP        122     /* Operation not supported on transport endpoint */
   #define EAFNOSUPPORT      124     /* Address family not supported by protocol */
   #define EADDRINUSE        125     /* Address already in use */
   #define EADDRNOTAVAIL     126     /* Cannot assign requested address */
   #define ENETDOWN          127     /* Network is down */
   #define ENETUNREACH       128     /* Network is unreachable */
   #define ENETRESET         129     /* Network dropped connection because of reset */
   #define ECONNABORTED      130     /* Software caused connection abort */
   #define ECONNRESET        131     /* Connection reset by peer */
   #define ENOBUFS           132     /* No buffer space available */
   #define EISCONN           133     /* Transport endpoint is already connected */
   #define ENOTCONN          134     /* Transport endpoint is not connected */
   #define ESHUTDOWN         143     /* Cannot send after transport endpoint shutdown */
   #define ETIMEDOUT         145     /* Connection timed out */
   #define ECONNREFUSED      146     /* Connection refused */
   #define EHOSTUNREACH      148     /* No route to host */
   #define EALREADY          149     /* Operation already in progress */
   #define EINPROGRESS       150     /* Operation now in progress */
   #define EWOULDBLOCK    EAGAIN     /* Operation would block */
//error.h
   #define EINTR               4     /* Interrupted system call */
   #define EAGAIN             11     /* Try again */
   #define EACCES             13     /* Permission denied */
   #define EFAULT             14     /* Bad address */
   #define EINVAL             22     /* Invalid argument */
   #define EMFILE             24     /* Too many open files */
//just windows 
   #define ENOTINITIALISED   200     //WSANOTINITIALISED   socket
   
#if 0
    #define IP_ADD_MEMBERSHIP   5           /* add  an IP group membership      */
    #define IP_DROP_MEMBERSHIP  6           /* drop an IP group membership      */
	struct ip_mreq {
		struct in_addr  imr_multiaddr;	   /* IP multicast address of group */
		struct in_addr  imr_interface;     /* local IP address of interface */
	};
#endif
/*****************************************************************************************
*                            define other CONST                                           * 
*****************************************************************************************/
   #define MSG_WAITALL  0x10
   #define MSG_DONTWAIT 0x11    

/*****************************************************************************************
*                            type definition                                             *
*****************************************************************************************/
#endif

#ifndef NULL
//#define NULL (void*)0
#endif

#ifndef BOOL
//#define BOOL int
#endif

#ifndef TRUE
//#define TRUE  1
#endif

#ifndef FALSE
//#define FALSE 0
#endif

#ifndef UINT32
#define UINT32 unsigned int
#endif

#ifndef UINT16
#define UINT16 unsigned short
#endif

#ifndef UCHAR
#define UCHAR unsigned char
#endif

#ifndef INT32
#define INT32 int
#endif

#ifndef INT16
#define INT16 short
#endif

/*****************************************************************************************
*                             interface declare                                          *
*****************************************************************************************/	   
//socket function

/**************************************************************************
* Function Name:My_getsockerr
* Description:  get information about last wrong operation on socket
*				CNcomment:获取上次有关socket错误操作的具体信息,
* Parameters:no
* Return:error code
***************************************************************************/
int My_getsockerr(void);

/**************************************************************************
* Function Name:My_socket
* Description:create SOCKET
* Parameters:
*			af:	 address description, only AF_INET is supported now
*				 CNcomment:地址描述，目前仅支持AF_INET
*           type:
*				 socket type; TCP use SOCK_STREAM, UDP use SOCK_DGRAM;
*				 SOCK_RAW and SOCK_RDM SOCK_SEQPACKET are also supported but shielded hereon
*				 CNcomment:套接字的类型  
*   	         CNcomment:TCP套接字为SOCK_STREAM值  
*		         CNcomment:UDP套接字为SOCK_DGRAM值 
*           	 CNcomment:注：SOCK_RAW SOCK_RDM SOCK_SEQPACKET 也可支持，在此暂时屏蔽   
*           protocol:
*				 socket protocol, only zero is supported now
*				 CNcomment:套接字所用的协议,目前只能为0
* Return:   socket handle:success; -1:failure;
*			CNcomment:成功返回套接字描述符，失败返回-1
*           CNcomment:注：用户可以通过调用My_getsockerr函数来获得有关错误的具体信息
***************************************************************************/
int My_socket(int domain, int type, int protocol);

/**************************************************************************
* Function Name:My_setsockopt
* Description:  set socket option
*				CNcomment:设置SOCKET的选项
* Parameters:
*			s:
*				socket handle;
*				CNcomment:套接字描述符
*           level:  option level, only support SOL_SOCKET
*					CNcomment:选项的层次，暂时固定为SOL_SOCKET 
*   	    optname:
*					option name
*					SO_DEBUG	:record debug information is permit
*                   SO_BROADCAST:transport broadcast message is permit
*                   SO_REUSEADDR:reuse local address is permit
*                   SO_KEEPALIVE:keep link is permit
*                   SO_DONTROUTE:message can be send without passing router
*                   SO_LINGER	:close is delay if there is data
*                   SO_OOBINLINE:receive outter data in inner is permit
*                   SO_SNDBUF	:set the size of send buffer
*                   SO_RCVBUF	:set the size of receive buffer
*                   SO_ERROR	:permit to get or clean error on socket

*					CNcomment:选项的名称 
*                   CNcomment:SO_DEBUG ：允许记录调试信息
*                   CNcomment:SO_BROADCAST：允许传送广播消息
*                   CNcomment:SO_REUSEADDR：允许本地地址重用
*                   CNcomment:SO_KEEPALIVE：允许保持连接活跃
*                   CNcomment:SO_DONTROUTE：允许发出的消息不经过路由选择
*                   CNcomment:SO_LINGER：如果有数据存在，那么关闭被延时
*                   CNcomment:SO_OOBINLINE：允许在带内接收带外数据
*                   CNcomment:SO_SNDBUF：设置发送数据的缓冲大小
*                   CNcomment:SO_RCVBUF：设置接收数据的缓存大小
*                   CNcomment:SO_ERROR：在socket上获得和清除错误
*		    optval:
*					porint to option buffer
*					CNcomment:指向用于获取或设置的选项值的缓存空间 
*           optlen:
*					buffer size
*					CNcomment:缓存空间的长度
*			Note:you can consult setsocket description in windows or linux to get more information
*           CNcomment:注：参数具体含义请参阅WINDOWS或LINUX的setsockopt函数说明
* Return:
*			0:success; -1:failure;
*           CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息
***************************************************************************/
int My_setsockopt(int s, int level, int optname, void *optval, int optlen);

/**************************************************************************
* Function Name:My_connect
* Description:	create connect
* Parameters:	sockfd:socket;     						CNcomment:请求连接套接字的描述符
*           	serv_addr:socket address on server;		CNcomment:用户需要连接的服务器端的套接字的地址  
*   	    	addrlen:size of address;				CNcomment:地址的长度  
* Return:		0:success; -1:failure;
*          	 	CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息
***************************************************************************/
int My_connect(int sockfd,  const struct sockaddr *serv_addr, int addrlen);

/**************************************************************************
* Function Name:My_recv
* Description:	receive data from stream socket;		CNcomment:从流式套接字中接收数据
* Parameters:	
*				s:socket;								CNcomment:发送数据套接字的描述符
*           	buf:buffer;								CNcomment:指向接收数据的缓冲区的指针  
*   	    	len:buffer size;						CNcomment:缓冲区的大小   
*           	flags:control option;					CNcmoomnt:控制选项
*                  CNcomment:MSG_OOB：接收带宽数据
*                  CNcomment:MSG_PEEK：查看套接字的数据，而不将数据从套接字缓冲区中取走
*                  CNcomment:MSG_DONTWAIT：如果套接字缓冲区无足够空间，不阻塞等待
*                  CNcomment:MSG_WAITALL：如果套接字缓冲区无足够空间，阻塞等待
*                  CNcomment:注：存在 0：无任何控制
* Return:		success: received data number(byte);
*				-1:error;
*				CNcomment:成功：返回接收的字节数
*           	CNcomment:connect关闭，返回0    
*           	CNcomment:错误：返回-1
*           	CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息
***************************************************************************/
int My_recv(int s, void *buf, int len, int flags);

/**************************************************************************
* Function Name:My_recvfrom
* Description:	receive data from datagram socket;		CNcomment:从数据报套接字中接收数据
* Parameters:
*				s:socket:								CNcomment:发送数据套接字的描述符
*           	buf:receive buffer;						CNcomment:指向接收数据的缓冲区的指针  
*   	    	len:buffer size;						CNcomment:缓冲区的大小   
*           	flags:control option;					CNcomment:控制选项
*                  CNcomment:MSG_OOB：接收带宽数据
*                  CNcomment:MSG_PEEK：查看套接字的数据，而不将数据从套接字缓冲区中取走
*                  CNcomment:MSG_DONTWAIT：如果套接字缓冲区无足够空间，不阻塞等待
*                  CNcomment:MSG_WAITALL：如果套接字缓冲区无足够空间，阻塞等待
*                  CNcomment:注：存在 0：无任何控制
*           	from:address of send socket;			CNcomment:发送数据报套接字的地址
*           	fromlen:size of address
* Return:		success: received data number(byte);
*				-1:error;
*				CNcomment:成功：返回接收的字节数
*           	CNcomment:connect关闭，返回0    
*           	CNcomment:错误：返回-1
*           	CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息
***************************************************************************/
int My_recvfrom(int s, void *buf, int len, int flags, struct sockaddr *from, int *fromlen);

/**************************************************************************
* Function Name:My_closesocket
* Description:	close socket
* Parameters:	s:socket neet to close
*          		CNcomment:函数行为由setsockopt设置的选项决定
* Return:		0 :success    
*           	-1: failure  
*           	CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息 
***************************************************************************/
int My_closesocket(int s);

/**************************************************************************
* Function Name:My_sendto
* Description	send data to datagram socket;			CNcomment:向数据报套接字中发送数据
* Parameters:	
*				s:socket;								CNcomment:接收数据套接字的描述符
*           	buf:send buffer;						CNcomment:指向发送数据的缓冲区的指针  
*   	    	len:buffer size;						CNcomment:缓冲区的大小   
*           	flags:control option;					CNcomment:控制选项
*                  CNcomment:MSG_OOB：接收带宽数据
*                  CNcomment:MSG_PEEK：查看套接字的数据，而不将数据从套接字缓冲区中取走
*                  CNcomment:MSG_DONTWAIT：如果套接字缓冲区无足够空间，不阻塞等待
*                  CNcomment:注：存在 0：无任何控制
*           	to:address of receive socket;			CNcomment:接收数据报套接字的地址
*           	tolen:sizo of address
* Return:		success: send data number(byte);
*				-1:error;
*				CNcomment:成功：返回接收的字节数
*           	CNcomment:错误：返回-1
*           	CNcomment:注：用户可以通过调用　　　函数来获得有关错误的具体信息
***************************************************************************/
int My_sendto(int s, void *buf, int len, int flags, const struct sockaddr *to, int tolen);

/**************************************************************************
* Function Name:My_send
* Description:	send data to stream socket;				CNcomment:向流式套接字中发送数据
* Parameters:
*				s:socket;								CNcomment:接收数据套接字的描述符
*           	buf:send buffer;						CNcomment:指向发送数据的缓冲区的指针  
*   	    	len:buffer size;						CNcomment:缓冲区的大小   
*           	flags:control option;					CNcomment:控制选项
*                  CNcomment:MSG_OOB：接收带宽数据
*                  CNcomment:MSG_PEEK：查看套接字的数据，而不将数据从套接字缓冲区中取走
*                  CNcomment:MSG_DONTWAIT：如果套接字缓冲区无足够空间，不阻塞等待
*                  CNcomment:MSG_WAITALL：如果套接字缓冲区无足够空间，阻塞等待
*                  CNcomment:0：无任何控制
* Return:		success: send data number(byte);
*				-1:error;
*				CNcomment:成功：返回接收的字节数
*           	CNcomment:错误：返回-1
***************************************************************************/
int My_send(int s, void *buf, int len, int flags);


//string function
/**************************************************************************
* Function Name:My_strcasecmp
* Description:	compare two string, char is sensitivity;	CNcomment:比较指定范围的字符串大小,不区分字符串的大小写
* Parameters:	string1:string1
*           	string2:string2
* Return:		>0:string1>string2
*           	0 :string1=string2
*				<0:string1<string2
***************************************************************************/
int My_strcasecmp(const char *string1, const char *string2);

/**************************************************************************
* Function Name:My_strncasecmp
* Description:	compare two string, char is sensitivity;	CNcomment:比较指定范围的字符串大小,不区分字符串的大小写
* Parameters:	string1:string1
*           	string2:string2
*           	count:count of char to compare
* Return:		>0:string1>string2
*           	0 :string1=string2
*				<0:string1<string2
***************************************************************************/
int My_strncasecmp(const char *string1, const char *string2, size_t count);

//system time function
/**************************************************************************
* Function Name:My_getsystime
* Description:	get system time;		CNcomment:获得系统的时间值 
* Parameters:	no
* Return:		return the time(ms);	CNcomment:返回系统的时间值，单位毫秒
***************************************************************************/
unsigned int My_getsystime(void);

/**************************************************************************
* Function Name:My_gettimeofday
* Description:	get system time;			CNcommont:获得系统的当前时间值，以timeval结构返回
* Parameters:	timeofday:time structure:	CNcomment:获取的时间值
* Return:		0:success; oters:failure;
***************************************************************************/
int My_gettimeofday(struct timeval *timeofday);

/**************************************************************************
* Function Name:My_Sleep
* Description:	sleep;				CNcomment:休眠指定的时间单位 
* Parameters:	sleep time(ms);		CNcomment:指定时间单位，毫秒
* Return:no
***************************************************************************/
void My_Sleep(unsigned short uSleep);

/**************************************************************************
* Function Name:My_drand48
* Description:get a double precision random;	CNcomment:获取一个双精度型随机数
* Parameters:no
* Return:random
***************************************************************************/
double My_drand48(void);

/**************************************************************************
* Function Name:My_createMutex
* Description:	create a mutex;		CNcomment:创建一个互斥信号量
* Parameters:	no
* Return:		0: falure; others:mutex ID
***************************************************************************/
int My_createMutex(void);

/**************************************************************************
* Function Name:y_freeMutex
* Description:	free a mutex;	CNcomment:删除一个互斥信号量
* Parameters:	mutex ID
* Return:no
***************************************************************************/
void My_deleteMutex(int mutexID);

/**************************************************************************
* Function Name:My_enterMutex
* Description:get a mutex, enter protect area;		CNcomment:获取一个互斥信号，进入保护阶段
* Parameters:	mutex ID
* Return:no
***************************************************************************/
void My_enterMutex(int mutexID);

/**************************************************************************
* Function Name:My_leaveMutex
* Description:	release a mutex, leave protect area;	CNcomment:释放一个互斥信号，离开保护阶段
* Parameters:	mutex ID
* Return:no
***************************************************************************/
void My_leaveMutex(int mutexID);
/**************************************************************************
* Function Name:My_strstr
* Description:	find a string
*				CNcomment:查找字符串，对linux c函数进行封装，对输入参数做检验，防止非法访问
* Parameters:	haystack:target string pointer;    	CNcomment:指向目标字符串的指针
*           	needle:string will be search;		CNcomment:指向要查找的字符串的指针
* Return:		
*				NULL:not find; 
*				others:pointer point to the string find in target string
***************************************************************************/
char *My_strstr(const char *haystack,const char *needle);
/**************************************************************************
* Function Name:My_strchr
* Description:  search char
*				CNcomment:查找字符，对linux c函数进行封装，对输入参数做检验，防止非法访问
* Parameters:	s:target string pointer;    CNcomment:指向目标字符串的指针
*           	c:char will be search;		CNcomment:要查找的字符
* Return:		
*				NULL:not find; 
*				others:pointer point to the char find in target string
***************************************************************************/
char * My_strchr(const char *s, int c);
#endif

