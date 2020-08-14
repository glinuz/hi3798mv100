/*********************************************************************************
* copyright (C)2004, hw corp ltd.
* 
* File Name: losys.c
* Description:	The file supplies encapsulation of some system function to segregate from Operate
*				System. Including socket,standard C lib and System related function, and dubug outpt
*				,timer function, user can expend them.
*				CNcomment:本文件提供了一些常用的系统函数封装，用于完成这些函数同操作系统的隔离，
*            	CNcomment:主要包括Socket、标准C库中一些与操作系统相关的函数，以及调试信息输出和
*            	CNcomment:定时器函数，使用者可以根据需要自行扩充，目前支持Windows和Linux操作系统
*            	CNcomment:的隔离，可以扩展实现对其它操作系统的隔离
*            
* Version:	 V1.0
* Create:	 rxc
* Date:  
* History:	 
* 	Modify date:
*	Version:
* 	Author:
* 	Description:
*************************************************************************************/

#include "losys.h"

#ifdef LOW_WINDOWS_ON
int Win_getlasterr(void)
{
	int iErrNo;
	int iReturnErr;
	iErrNo = WSAGetLastError();
	switch(iErrNo)
	{
	    case WSAEINTR:
		{
            iReturnErr = EINTR;            //4
			break;
		}
		case WSAEACCES:         
		{
            iReturnErr = EACCES;          //13 
			break;
		}
		case WSAEFAULT:
		{
            iReturnErr = EFAULT;          //14         
			break;
		}
		case WSAEINVAL:
		{
            iReturnErr = EINVAL;          //22         
			break;
		}
		case WSAEMFILE: 
		{
			iReturnErr = EMFILE;          //24
			break;
		}
		case WSAENOTSOCK:
		{
            iReturnErr = ENOTSOCK;        //95
			break;
		}
		case WSAEDESTADDRREQ:     
		{
            iReturnErr = EDESTADDRREQ;    //96
			break;
		}
		case WSAEMSGSIZE:     
		{
            iReturnErr = EMSGSIZE;        //97 
			break;
		}
		case WSAEPROTOTYPE:
		{
			iReturnErr = EPROTOTYPE;      //98
			break;
		}
		case WSAENOPROTOOPT:
		{
            iReturnErr = ENOPROTOOPT;     //99
			break;
		}
		case WSAEPROTONOSUPPORT:
		{
			iReturnErr = EPROTONOSUPPORT; //120
			break;
		}
		case WSAESOCKTNOSUPPORT:
		{
            iReturnErr = ESOCKTNOSUPPORT; //121
			break;
		}
		case WSAEOPNOTSUPP:      
		{
            iReturnErr = EOPNOTSUPP;      //122
			break;
		}
        case WSAEAFNOSUPPORT:             
		{
			iReturnErr = EAFNOSUPPORT;    //124
			break;
		}
		case WSAEADDRINUSE:
		{
            iReturnErr = EADDRINUSE;      //125
			break;
		}
		case WSAEADDRNOTAVAIL:            
		{
            iReturnErr = EADDRNOTAVAIL;   //126
			break;
		}
		case WSAENETDOWN:
		{
			iReturnErr = ENETDOWN;        //127
			break;
		}
		case WSAENETUNREACH:             
		{
            iReturnErr = ENETUNREACH;     //128
			break;
		}
		case WSAENETRESET:
		{
            iReturnErr = ENETRESET;       //129
			break;
		}
		case WSAECONNABORTED:      
		{
            iReturnErr = ECONNABORTED;    //130
			break;
		}
	    case WSAECONNRESET:     
		{
            iReturnErr = ECONNRESET;      //131 
			break;
		}
		case WSAENOBUFS:
		{
			iReturnErr = ENOBUFS;         //132       
			break;
		}
		case WSAEISCONN:                 
		{
            iReturnErr = EISCONN;         //133
			break;
		}
		case WSAENOTCONN:
		{
            iReturnErr = ENOTCONN;        //134
			break;
		}
		case WSAESHUTDOWN:      
		{
            iReturnErr = ESHUTDOWN;       //143
			break;
		}
		case WSAETIMEDOUT:      
		{
            iReturnErr = ETIMEDOUT;       //145
			break;
		}
		case WSAECONNREFUSED:            
		{
            iReturnErr = ECONNREFUSED;    //146
			break;
		}
		case WSAEHOSTUNREACH:    
		{
            iReturnErr = EHOSTUNREACH;    //148 
			break;
		}
		case WSAEALREADY:                 
		{
            iReturnErr = EALREADY;        //149
			break;
		}
		case WSAEINPROGRESS:             
		{
			iReturnErr = EINPROGRESS;     //150    
			break;
		}
		case WSANOTINITIALISED:
		{
			iReturnErr = ENOTINITIALISED; //200
			break;
		}
        case WSAEWOULDBLOCK:    
		{
            iReturnErr = EWOULDBLOCK;    // EAGAIN  11
			break;
		}
		default:
		{
			iReturnErr = 0;
			break;
		}
	}
	return iReturnErr;
}
#endif

int My_getsockerr()
{
	int iLastErr = -1;
#ifdef LOW_WINDOWS_ON
	iLastErr = Win_getlasterr(); 
#endif
#ifdef LOW_LINUX_ON
	iLastErr = errno;
#endif
	return iLastErr;
}

int My_socket(int domain, int type, int protocol)
{	
#ifdef LOW_WINDOWS_ON
	if(domain != AF_INET)
	{
        WSASetLastError(WSAEAFNOSUPPORT);
		return -1;
	}
	if(type != SOCK_STREAM && type != SOCK_DGRAM)
	{
        WSASetLastError(WSAESOCKTNOSUPPORT);
		return -1;
	}
	if(protocol != 0)
	{
        WSASetLastError(WSAEPROTONOSUPPORT);
		return -1;
	}
	return (int)socket(domain, type, protocol);   //return type is SOCKET;
#endif

#ifdef LOW_LINUX_ON
	if(domain != AF_INET)
	{
        errno = EAFNOSUPPORT;
		return -1;
	}
	if(type != SOCK_STREAM && type != SOCK_DGRAM)
	{
        errno = EINVAL;
		return -1;
	}
	if(protocol != 0)
	{
        errno = ESOCKTNOSUPPORT;
		return -1;
	}
	return socket(domain, type, protocol);   //return type is int;
#else
	return 1;
#endif  
}

int My_setsockopt(int s, int level, int optname, void *optval, int optlen)
{
#ifdef LOW_WINDOWS_ON
	if (level != SOL_SOCKET)
	{
		if(level == IPPROTO_IP){
			if((optname == IP_ADD_MEMBERSHIP)||(optname == IP_DROP_MEMBERSHIP)){
				/*
				It's no need to set in windows OS,and enter or leave broadcast group is automatic.
				*/
				/*CNcomment:windows不需要设置会自动将加入和离开组播组*/
				return 0;
			}
		}
		WSASetLastError(WSAEINVAL);
		return -1;
	}
	if( optname != SO_DEBUG && optname != SO_BROADCAST && optname != SO_REUSEADDR && 
	    optname != SO_KEEPALIVE && optname != SO_DONTROUTE && optname != SO_LINGER &&
		optname != SO_OOBINLINE && optname != SO_SNDBUF && optname != SO_RCVBUF &&
        optname != SO_ERROR)
	{
		WSASetLastError(WSAENOPROTOOPT);
		return -1;
	}
	if(optval == NULL) 
	{
		WSASetLastError(WSAEFAULT);
        return -1;
	}
	return setsockopt((SOCKET)s, level, optname, (char*)optval, optlen);
#endif
#ifdef LOW_LINUX_ON
	if ((level != SOL_SOCKET)&&(level != IPPROTO_IP))
	{
		 errno = ENOPROTOOPT;
		 return -1;
	}
	if( optname != SO_DEBUG && optname != SO_BROADCAST && optname != SO_REUSEADDR && 
	    optname != SO_KEEPALIVE && optname != SO_DONTROUTE && optname != SO_LINGER &&
		optname != SO_OOBINLINE && optname != SO_SNDBUF && optname != SO_RCVBUF &&
        optname != SO_ERROR && optname != IP_ADD_MEMBERSHIP && optname != IP_DROP_MEMBERSHIP)
	{
		errno = ENOPROTOOPT;
		return -1;
	}
	if(optval == NULL) 
	{
		errno = EFAULT;
		return -1;
   	}
	return setsockopt(s, level, optname, optval, (socklen_t)optlen);
#else
	return -1;
#endif
}

int  My_connect(int sockfd, const struct sockaddr *serv_addr, int addrlen)
{
#ifdef LOW_WINDOWS_ON
	if (serv_addr == NULL)
	{
         WSASetLastError(WSAEFAULT);
		 return -1;
	}
	return connect((SOCKET)sockfd, serv_addr, addrlen);   
#endif
#ifdef LOW_LINUX_ON
	if (serv_addr == NULL)
	{
         errno = EFAULT;
		 return -1;
	}
	return connect(sockfd, serv_addr, (socklen_t)addrlen); 
#else
	return -1;
#endif  
}

int My_recv(int s, void *buf, int len, int flags)
{
#ifdef LOW_WINDOWS_ON
	int iCount = 0;
	int iLen   = 0;
	int iTime  = 0;
	int iResult;
	UINT32 uMaxFd;
	fd_set t_Readfd;
    struct timeval t_TimeDelay;
	if(s <= 0)
	{
		WSASetLastError(WSAENOTSOCK);
		return -1;
	}
	if(buf == NULL)
	{
		WSASetLastError(WSAEFAULT);
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		WSASetLastError(WSAEINVAL);
		//WSASetLastError(WSAEOPNOTSUPP);
		return -1;
	}
    uMaxFd = s;   
	FD_ZERO(&t_Readfd);
	FD_SET((SOCKET)s, &t_Readfd);
    if(flags == MSG_WAITALL)
	{
		iCount = len;
		while(iCount > 0)
		{
			iResult = select(uMaxFd + 1, &t_Readfd, NULL, NULL, NULL);
            if(iResult < 0)
			{
				return -1;
			}
            if(iResult > 0)
			{
				if(FD_ISSET(s, &t_Readfd)) 
				{
					iLen = recv((SOCKET)s, ((char*)buf) + (len-iCount), iCount, 0);
			        if(iLen < 0)
					{ 
						return iLen;
					}
			        if(iLen == 0)
					{
						return 0;
					}
		 			iCount -= iLen;
				}	
			}
		}
		return len;
	}
	if ( flags == MSG_DONTWAIT)
	{
		t_TimeDelay.tv_sec = 0;
	    t_TimeDelay.tv_usec = 0;
		
		iResult = select(uMaxFd + 1, &t_Readfd, NULL, NULL, &t_TimeDelay);
        if(iResult < 0)
		{
			return -1;
		}
		if(iResult == 0)  
		{
			WSASetLastError(WSAEWOULDBLOCK);
			return -1;
		}
		if(iResult > 0)
		{
			if(FD_ISSET((SOCKET)s, &t_Readfd)) 
			{
				return recv((SOCKET)s, (char*)buf, len, 0);
			}
		}

	}
	if (flags == MSG_OOB || flags == MSG_PEEK || flags == 0)
	{
		return recv((SOCKET)s, (char*)buf, len, flags);
	}
#endif


#ifdef LOW_LINUX_ON
	if(s <= 0)
	{
		errno = ENOTSOCK;
		return -1;
	}
	if(buf == NULL)
	{
		errno = EFAULT;
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		errno = EINVAL;
		return -1;
	}
	return recv(s, buf, (size_t) len, flags);
#else
	return -1;
#endif
}

int My_recvfrom(int s, void *buf, int len, int flags, struct sockaddr *from, int *fromlen)
{
#ifdef LOW_WINDOWS_ON
	int iResult;
	UINT32 uMaxFd;
	fd_set t_Readfd;
    struct timeval t_TimeDelay;
	if(s <= 0)
	{
		WSASetLastError(WSAENOTSOCK);
		return -1;
	}
	if(buf == NULL)
	{
		WSASetLastError(WSAEFAULT);
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		WSASetLastError(WSAEINVAL);
		//WSASetLastError(WSAEOPNOTSUPP);
		return -1;
	}
	
	uMaxFd = s;
	FD_ZERO(&t_Readfd);
	FD_SET((SOCKET)s, &t_Readfd);

	if(flags == MSG_WAITALL)
	{
		iResult = select(uMaxFd + 1, &t_Readfd, NULL, NULL, NULL);
        if(iResult < 0)
		{
			return -1;
		}
        if(iResult > 0)
		{
			if(FD_ISSET(s, &t_Readfd)) 
			{
				return recvfrom((SOCKET)s, (char*)buf, len, 0, from, fromlen);
			}
		}
	}

	if(flags == MSG_DONTWAIT)
	{   
		t_TimeDelay.tv_sec = 0;
	    t_TimeDelay.tv_usec = 0;
		
		iResult = select(uMaxFd + 1, &t_Readfd, NULL, NULL, &t_TimeDelay);
        if(iResult < 0)
		{
			return -1;
		}
		if(iResult == 0)   
		{
			WSASetLastError(WSAEWOULDBLOCK);
			return -1;
		}
		if(iResult > 0)
		{
			if(FD_ISSET((SOCKET)s, &t_Readfd)) 
			{
				return recvfrom((SOCKET)s, (char*)buf, len, 0, from, fromlen); 
			}
		}
	}
	
	if((flags == MSG_OOB) || (flags==MSG_PEEK) || (flags==0))
	{
		return recvfrom((SOCKET)s, (char*)buf, len, flags, from, fromlen);
	}
#endif

#ifdef LOW_LINUX_ON
	if(s <= 0)
	{
		errno = ENOTSOCK;
		return -1;
	}
	if(buf == NULL)
	{
		errno = EFAULT;
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		errno = EINVAL;
		return -1;
	}
	return recvfrom(s, buf, (size_t)len, flags, from, (socklen_t*)fromlen);
#else
	return -1;
#endif
}

int My_closesocket(int s)
{
#ifdef LOW_WINDOWS_ON
    return closesocket((SOCKET)s);
#endif
#ifdef LOW_LINUX_ON
    return close(s);
#else
	return -1;
#endif
}

int My_sendto(int s, void *buf, int len, int flags, const struct sockaddr *to, int tolen)
{
#ifdef LOW_WINDOWS_ON
	int iResult;
	UINT32 uMaxFd;
	fd_set t_Writefd;
    struct timeval t_TimeDelay;
	if(s <= 0)
	{
		WSASetLastError(WSAENOTSOCK);
		return -1;
	}
	if(buf == NULL)
	{
		WSASetLastError(WSAEFAULT);
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		WSASetLastError(WSAEINVAL);
		//WSASetLastError(WSAEOPNOTSUPP);
		return -1;
	}
	
	if(flags == MSG_DONTWAIT)
	{
		uMaxFd = s;
		FD_ZERO(&t_Writefd);
		FD_SET((SOCKET)s, &t_Writefd);

        t_TimeDelay.tv_sec = 0;
	    t_TimeDelay.tv_usec = 0;
		
		iResult = select(uMaxFd + 1, NULL, &t_Writefd, NULL, &t_TimeDelay);
        if(iResult < 0)
		{
			return -1;
		}
		if(iResult == 0)   
		{
			WSASetLastError(WSAEWOULDBLOCK);
			return -1;
		}
		if(iResult > 0)
		{
			if(FD_ISSET((SOCKET)s, &t_Writefd))
			{
				return sendto((SOCKET)s, (char*)buf, len, 0, to, tolen);
			}
		}
	}

	if((flags == MSG_DONTROUTE) || (flags == MSG_OOB) || (flags == 0))
	{
		return sendto((SOCKET)s, (char*)buf, len, flags, to, tolen);
	}
	
#endif

#ifdef LOW_LINUX_ON
	if(s <= 0)
	{
		errno = ENOTSOCK;
		return -1;
	}
	if(buf == NULL)
	{
		errno = EFAULT;
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		errno = EINVAL;
		return -1;
	}
	return sendto(s, buf, (size_t)len, flags, to, (socklen_t)tolen);
#else
	return -1;
#endif
}

int My_send(int s, void *buf, int len, int flags)
{
#ifdef LOW_WINDOWS_ON
	int iCount = 0;
	int iLen   = 0;       /*the number of byte once read from socket*//*CNcomment:每次从socket中读取的字节数*/
	int iTime  = 0; 
	int iResult;
	UINT32 uMaxFd;
	fd_set t_Writefd;
    struct timeval t_TimeDelay;
    if(s <= 0)
	{
		WSASetLastError(WSAENOTSOCK);
		return -1;
	}
	if(buf == NULL)
	{
		WSASetLastError(WSAEFAULT);
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		WSASetLastError(WSAEINVAL);
		//WSASetLastError(WSAEOPNOTSUPP);
		return -1;
	}

	uMaxFd = s;
	FD_ZERO(&t_Writefd);
	FD_SET((SOCKET)s, &t_Writefd);
	if(flags == MSG_WAITALL)
	{
	   	iCount = len;
		while(iCount > 0)
		{
			iResult = select(uMaxFd + 1, NULL, &t_Writefd, NULL, NULL);
            if(iResult < 0)
			{
				return -1;
			}
            if(iResult > 0)
			{
				if(FD_ISSET((SOCKET)s, &t_Writefd)) 
				{
					iLen = send((SOCKET)s, ((char*)buf) + (len-iCount), iCount, 0);
			        if(iLen < 0)
					{
						return iLen;
					}
			        iCount -= iLen;
				}
			}
		}
		return len;
	}

	if(flags == MSG_DONTWAIT)
	{
		t_TimeDelay.tv_sec = 0;
	    t_TimeDelay.tv_usec = 0;
		
		iResult = select(uMaxFd + 1, NULL, &t_Writefd, NULL, &t_TimeDelay);
        if(iResult < 0)
		{
			return -1;
		}
		if(iResult == 0)  
		{
		    WSASetLastError(WSAEWOULDBLOCK);
			return -1;
		}
		if(iResult > 0)
		{
			if(FD_ISSET((SOCKET)s, &t_Writefd)) 
			{
				return send((SOCKET)s, (char*)buf, len, 0);
			}
		}
	}
	if((flags == MSG_DONTROUTE) || (flags == MSG_OOB) || (flags == 0))
	{
		return send((SOCKET)s, (char*)buf, len, flags);
	}
	
#endif
#ifdef LOW_LINUX_ON
	if(s <= 0)
	{
		errno = ENOTSOCK;
		return -1;
	}
	if(buf == NULL)
	{
		errno = EFAULT;
		return -1;
	}
	if(flags != MSG_OOB && flags != MSG_WAITALL && flags != MSG_DONTWAIT && 
	   flags != MSG_PEEK && flags != 0)
	{
		errno = EINVAL;
		return -1;
	}
	return send(s, buf, (size_t)len, flags);
#else
	return -1;
#endif
}

//string Function
int My_strcasecmp(const char *string1, const char *string2)
{
#ifdef LOW_WINDOWS_ON
	return _stricmp(string1, string2);
#endif
#ifdef LOW_LINUX_ON
	return strcasecmp(string1, string2);
#else
	return -1;
#endif
}

int My_strncasecmp(const char *string1, const char *string2, size_t count)
{
#ifdef LOW_WINDOWS_ON
	return _strnicmp(string1, string2, count);
#endif
#ifdef LOW_LINUX_ON
	return strncasecmp(string1, string2, count);
#else
	return -1;
#endif
}

unsigned int My_getsystime(void)
{
	unsigned int milliseconds = 0;
#ifdef LOW_WINDOWS_ON  
    milliseconds = GetTickCount();
#endif

#ifdef LOW_LINUX_ON
	struct timeval t_CurTime;
	gettimeofday(&t_CurTime,NULL);
    milliseconds = (1000 * t_CurTime.tv_sec) + (t_CurTime.tv_usec/1000);
#endif
	return milliseconds;
}

void My_Sleep(unsigned short uSleep)
{
#ifdef LOW_WINDOWS_ON  
    Sleep(uSleep);
#endif
#ifdef LOW_LINUX_ON
	usleep(uSleep * 1000);
#endif
	return;
}

int My_gettimeofday(struct timeval *timeofday)
{
#ifdef LOW_WINDOWS_ON  
    UINT32 uiTime = GetTickCount();
	timeofday->tv_sec = uiTime / 1000;
	timeofday->tv_usec= (uiTime % 1000) * 1000;
	return 0;
#endif
#ifdef LOW_LINUX_ON
	return gettimeofday(timeofday,NULL);
#else
	return -1;
#endif
}

double My_drand48()
{
#ifdef LOW_WINDOWS_ON  
    double ddRand = rand() * 1.089645;
	return ddRand;
#endif
#ifdef LOW_LINUX_ON
	return My_getsystime()* 0.84284;//drand48();
#else
	return -1;
#endif

}

int My_createMutex(void)
{
	return 1;
	//not implement yet
}

void My_deleteMutex(int mutexID)
{
	//not implement yet
}

void My_enterMutex(int mutexID)
{
	//not implement yet
}

void My_leaveMutex(int mutexID)
{
	//not implement yet
}

char *My_strstr(const char *haystack,const char *needle)
{
    if (haystack == NULL)
    {
    	  return NULL;
    }

	return strstr(haystack, needle);
}

char * My_strchr(const char *s, int c)
{
    if (s == NULL)
    {
    	return NULL;
    }

	return strchr(s,c);
}

