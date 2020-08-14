/***********************************************************************************
 *              Copyright 2008 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:         unf_ir_lirc.c
 * Description:      supply the api for userspace application
 * History:          NULL
 * Version           Date           Author     DefectNum    Description
 * 1.1               2013-11-26	    h00183514   NULL	    first draft
 ***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <limits.h>

#include "hi_drv_struct.h"
#include "drv_ir_ioctl.h"
#include "hi_drv_ir.h"
#include "hi_unf_ir.h"


#define LIRC_MAX_NAME_LEN       32
#define LIRC_DEFAULT_REPTIME    108   /* unit:ms */
#define LIRC_DEFAULT_BLOCKTIME  6000  /* unit:ms */
#define IR_MAX_BUF              100
#define LIRC_DEV_NAME           "/dev/lircd"
#define	INVALID_KEY_ID          -1

typedef struct hiLIRC_KEY_S
{
	HI_U64                  KeyId;
	HI_U32                  KeyRptCnt;
	HI_CHAR                 KeyName[LIRC_MAX_NAME_LEN];
	HI_CHAR                 ProcotolName[LIRC_MAX_NAME_LEN];
	HI_UNF_IR_PROTOCOL_E    ProtocolType;
	HI_UNF_KEY_STATUS_E     KeyStatus;
	HI_U32                  GotTime;
}LIRC_KEY_S;

typedef struct hiLIRC_S
{
    pthread_t               IrThread;
    HI_BOOL                 bThreadRun;
    HI_BOOL                 bEnable;
    LIRC_KEY_S              stIrBuf[IR_MAX_BUF];
    HI_U32                  u32BufHead;
    HI_U32                  u32Buftail;
    HI_U32                  u32RepTime;
    HI_U32                  u32BlockTime;
    HI_BOOL                 bRepFlag;
    HI_BOOL                 bUpFlag;
    HI_BOOL                 bBlockFlag;
    HI_CHAR                 LastProtocolName[LIRC_MAX_NAME_LEN];
}LIRC_S;


static LIRC_S               g_Lirc;
static HI_S32               g_IrDevFd = -1;
static pthread_mutex_t      g_IrMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_IR_LOCK()  	        (void)pthread_mutex_lock(&g_IrMutex);
#define HI_IR_UNLOCK()  	    (void)pthread_mutex_unlock(&g_IrMutex);

#define IR_BUF_TAIL		        g_Lirc.stIrBuf[g_Lirc.u32Buftail]
#define IR_BUF_LAST		        g_Lirc.stIrBuf[(g_Lirc.u32BufHead == 0) ? (IR_MAX_BUF - 1) : (g_Lirc.u32BufHead - 1)]

#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK           0x00004000
#endif

#define CHECK_IR_OPEN()\
do{\
    HI_IR_LOCK();\
    if (g_IrDevFd < 0)\
    {\
        HI_ERR_IR("IR is not open.\n");\
        HI_IR_UNLOCK();\
        return HI_ERR_IR_NOT_INIT;\
    }\
    HI_IR_UNLOCK();\
}while(0)


static HI_VOID IrTransProtocol(HI_CHAR *pProtocolName, HI_UNF_IR_PROTOCOL_E *penProtocol)
{
	if (strncmp(pProtocolName, "nec_simple", sizeof("nec_simple")))
	{
	    *penProtocol = HI_UNF_IR_NEC;
	}
	else if(strncmp(pProtocolName, "rc5", sizeof("rc5")))
	{
	    *penProtocol = HI_UNF_IR_RC5;
	}
	else if(strncmp(pProtocolName, "rc6", sizeof("rc6")))
	{
	    *penProtocol = HI_UNF_IR_RC6_MODE0;
	}
	else
	{
	    *penProtocol = HI_UNF_IR_USER_DEFINED;
	}

}

/* Message: KeyId KeyRptCnt KeyName Protocol */
static HI_S32 IrParseMessage(HI_CHAR *pMessage, LIRC_KEY_S *pLircKey)
{
	HI_S32 Ret;

	Ret = sscanf(pMessage,"%llx %x %32s %32s", &pLircKey->KeyId, &pLircKey->KeyRptCnt,
	            pLircKey->KeyName, pLircKey->ProcotolName);
	if (Ret < 4)
	{
	    return HI_FAILURE;
	}
	
	IrTransProtocol(pLircKey->ProcotolName, &pLircKey->ProtocolType);
    
	/* key up */
	if (strstr(pLircKey->KeyName,"__up"))
	{
	    pLircKey->KeyStatus = HI_UNF_KEY_STATUS_UP;
	}
	/* key hold */
	else if (pLircKey->KeyRptCnt > 0)
	{
	    pLircKey->KeyStatus = HI_UNF_KEY_STATUS_HOLD;
	}
	/* key down */
	else
	{
	    pLircKey->KeyStatus = HI_UNF_KEY_STATUS_DOWN;
	}

	return HI_SUCCESS;
}

static HI_VOID *IrRecvThread(HI_VOID *args)
{
    HI_S32              Ret;
	HI_S32              fd;
	HI_S32              i;
	HI_CHAR             buf[128] = {0};
	struct sockaddr_un  addr;
	LIRC_KEY_S          CurKey;
	HI_CHAR             *pToken = HI_NULL;
	fd_set              fds;    
	struct timeval      timeout = {0};
	HI_U32 Stime,Etime;

	fd = socket(AF_UNIX, (HI_U32)SOCK_STREAM | (HI_U32)SOCK_NONBLOCK, 0);
	if (fd == -1)
	{
		HI_ERR_IR("Create socket error\n");
		return HI_NULL;
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, LIRC_DEV_NAME, strlen(LIRC_DEV_NAME));
		
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		close(fd);
		HI_ERR_IR("Connect error\n");
		return HI_NULL;
	}

	while(g_Lirc.bThreadRun)
	{
		if (HI_FALSE == g_Lirc.bEnable)
		{
			usleep(10 * 1000);
			continue;
		}

		FD_ZERO(&fds);
		FD_SET(fd, &fds);
		timeout.tv_sec = 0;
		timeout.tv_usec = 300000;
		Ret = select(fd + 1, &fds, NULL, NULL, &timeout);
		if(Ret == -1)
		{
			break;    /*error*/
		}
		else if(Ret == 0)
		{
			continue;  /*timeout*/
		}
		else if(!FD_ISSET(fd, &fds))
		{
			continue;   /*can't reach here*/
		}

		i = read(fd, buf, 128);
		if(-1 == i)
		{
			HI_ERR_IR("Read error\n");
			break;
		}

		if(!i)
		{
			break;
		}

		buf[i] = '\0';
		
        /* parse buffer to get key_id rep_cnt key_name and protocol_name */
		pToken = strtok(buf, "\n");
		
		while(HI_NULL != pToken)
		{			
			Ret = IrParseMessage(pToken, &CurKey);
			if (HI_SUCCESS != Ret)
			{
			    HI_ERR_IR("IrParseMessage err, Ret=%#x\n", Ret);
				break;
			}
	
			if (HI_UNF_KEY_STATUS_UP == CurKey.KeyStatus)
			{
		    	/* discard key up*/
			    if (HI_FALSE == g_Lirc.bUpFlag)
			    {
			    	pToken = strtok(HI_NULL,"\n");
			    	continue;
			    }
			}
			else if (HI_UNF_KEY_STATUS_HOLD == CurKey.KeyStatus)
			{
				 HI_SYS_GetTimeStampMs(&Etime);
		    	/* discard key repeat or repeat too frequently*/
			    if ((HI_FALSE == g_Lirc.bRepFlag) || (Etime - Stime < g_Lirc.u32RepTime))
			    {
			    	pToken = strtok(HI_NULL,"\n");
					continue;
			    }
				HI_SYS_GetTimeStampMs(&Stime);
			}
			else
			{
				HI_SYS_GetTimeStampMs(&Stime);
			}
			/* insert key */
			if (g_Lirc.u32Buftail == (g_Lirc.u32BufHead + 1) % IR_MAX_BUF)
			{
			    HI_ERR_IR("IR buffer is full\n");
			    pToken = strtok(HI_NULL,"\n");
			    continue;
			}
			else
			{
				g_Lirc.stIrBuf[g_Lirc.u32BufHead] = CurKey;
				g_Lirc.u32BufHead = (g_Lirc.u32BufHead + 1) % IR_MAX_BUF;
			}
			
			pToken = strtok(HI_NULL,"\n");
		}
	};

    close(fd);
    
	return HI_NULL;
    
}

HI_S32 HI_UNF_IR_Init(HI_VOID)
{
    HI_S32          Ret;
    HI_U32          i;
    
    HI_IR_LOCK();

    if (g_IrDevFd > 0)
    {
        HI_IR_UNLOCK();
        return HI_SUCCESS;
    }

    g_Lirc.bThreadRun = HI_TRUE;
    g_Lirc.bEnable = HI_FALSE;
    g_Lirc.bBlockFlag = HI_TRUE;
    g_Lirc.bRepFlag = HI_TRUE;
    g_Lirc.bUpFlag = HI_FALSE;

    for (i = 0; i < IR_MAX_BUF; i++)
    {
        memset(&(g_Lirc.stIrBuf[i]), 0x0, sizeof(LIRC_KEY_S));
    }

    memset(g_Lirc.LastProtocolName, 0x0, sizeof(g_Lirc.LastProtocolName));

    g_Lirc.u32BufHead = 0;
    g_Lirc.u32Buftail = 0;

	IR_BUF_LAST.KeyId = (HI_U64)INVALID_KEY_ID;
	IR_BUF_LAST.KeyStatus = HI_UNF_KEY_STATUS_UP;
	
	g_Lirc.u32RepTime = LIRC_DEFAULT_REPTIME;
    g_Lirc.u32BlockTime = LIRC_DEFAULT_BLOCKTIME;

    Ret = pthread_create(&g_Lirc.IrThread, HI_NULL, IrRecvThread, HI_NULL);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_IR("create thread err, Ret=%#x\n", Ret);
        HI_IR_UNLOCK();
        return HI_ERR_IR_OPEN_ERR;
    }

    g_IrDevFd = 1;

	HI_SYS_Init();

    HI_IR_UNLOCK();
    return HI_SUCCESS;    

}

HI_S32 HI_UNF_IR_DeInit(HI_VOID)
{      
    HI_IR_LOCK();

    if (g_IrDevFd < 0)
    {
        HI_IR_UNLOCK();
        return HI_SUCCESS;
    }

    g_Lirc.bThreadRun = HI_FALSE;

    pthread_join(g_Lirc.IrThread, NULL);

    g_IrDevFd = -1;

    HI_IR_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_Enable (HI_BOOL bEnable)
{
    CHECK_IR_OPEN();

    if (bEnable)
    {
        g_Lirc.u32BufHead = 0;
        g_Lirc.u32Buftail = 0;
        g_Lirc.bEnable = HI_TRUE;
    }
    else
    {
        g_Lirc.bEnable = HI_FALSE;
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_GetProtocol(HI_UNF_IR_PROTOCOL_E *penProtocol)
{    
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_GetProtocolName(HI_CHAR *pProtocolName,HI_S32 s32BufLen)
{
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_GetValueWithProtocol(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId, 
                                      HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs)
{    
    HI_S32      i;

    CHECK_IR_OPEN();

    if (HI_NULL == penPressStatus)
    {
        HI_ERR_IR("null point(penPressStatus) \n");
        return HI_ERR_IR_NULL_PTR;
    }

    if (HI_NULL == pu64KeyId)
    {
        HI_ERR_IR("null point(pu64KeyId)\n");
        return HI_ERR_IR_NULL_PTR;
    }

    if (0 == u32TimeoutMs)
    {
        g_Lirc.bBlockFlag = HI_FALSE;
    }
    else
    {
        g_Lirc.u32BlockTime = u32TimeoutMs/10;
        g_Lirc.bBlockFlag = HI_TRUE;
    }

    if (g_Lirc.u32BufHead == g_Lirc.u32Buftail)
	{
		if (g_Lirc.bBlockFlag == HI_FALSE)
		{
			return HI_ERR_IR_READ_FAILED;
		}
		else
		{
			for (i = (HI_S32)g_Lirc.u32BlockTime; i>0; i--)
			{
				if (g_Lirc.u32BufHead != g_Lirc.u32Buftail)
				{
					break;
				}
				else
				{
					usleep(10*1000);
				}
			}
			
			if (i <= 0)
			{
				return HI_ERR_IR_READ_FAILED;
			}
		}
	}

    *penPressStatus = IR_BUF_TAIL.KeyStatus;
    *pu64KeyId      = IR_BUF_TAIL.KeyId;

	strncpy(g_Lirc.LastProtocolName,IR_BUF_TAIL.ProcotolName,LIRC_MAX_NAME_LEN);

	g_Lirc.LastProtocolName[LIRC_MAX_NAME_LEN - 1] = '\0';
	
    if (HI_NULL != pszProtocolName) 
    {
        if (s32NameSize <= (HI_S32)LIRC_MAX_NAME_LEN)
        {
            strncpy(pszProtocolName, IR_BUF_TAIL.ProcotolName, (HI_U32)s32NameSize);
        }
        else
        {
            strncpy(pszProtocolName, IR_BUF_TAIL.ProcotolName, LIRC_MAX_NAME_LEN); 
        }
    } 
    
    g_Lirc.u32Buftail = (g_Lirc.u32Buftail + 1) % IR_MAX_BUF;    

    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_SetFetchMode(HI_S32 s32Mode)
{
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_GetSymbol(HI_U64 *pu64First, HI_U64* pu64Second, HI_U32 u32TimeoutMs)
{
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable)
{
    CHECK_IR_OPEN();

	g_Lirc.bUpFlag = bEnable;
	
	return HI_SUCCESS;
}


HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable)
{
    CHECK_IR_OPEN();

    g_Lirc.bRepFlag = bEnable;
    
    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs)
{
    CHECK_IR_OPEN();

    if (u32TimeoutMs < 108)
    {
        HI_ERR_IR("u32TimeoutMs is invalid\n");
		return HI_ERR_IR_INVALID_PARA;
    }

    g_Lirc.u32RepTime = u32TimeoutMs;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_E enIRCode)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_Reset(HI_VOID)
{
    CHECK_IR_OPEN();

	g_Lirc.u32BufHead = 0;
	g_Lirc.u32Buftail = 0;
	
	IR_BUF_LAST.KeyId = (HI_U64)INVALID_KEY_ID;
	IR_BUF_LAST.KeyStatus = HI_UNF_KEY_STATUS_UP;

    return HI_SUCCESS;
}

HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName)
{
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName)
{
    return HI_ERR_IR_UNSUPPORT;
}

HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled)

{
    return HI_ERR_IR_UNSUPPORT;
}

