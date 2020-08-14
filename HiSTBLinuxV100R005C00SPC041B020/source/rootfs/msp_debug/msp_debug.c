#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "hi_common.h"
#include "hi_debug.h"
#include "mpi_log.h"
#include "losys.h"

#if 1
/*********************************************************************************
*                          CONST  definition                                              *
**********************************************************************************/
#define DEBUG_BUF_SIZE    1536        /*debug buffer size*//*CNcomment:存储模块调试信息的缓冲区尺寸*/
#define DEBUG_TIMEOUT     60000       /*timeout(ms)*//*CNcomment:保持连接的最大时间间隔，单位毫秒*/

//command type
#define DEBUG_CONN       0x55aa       /*estabish link*//*CNcomment:建立连接*/
#define DEBUG_ON         0x55bb       /*open debug level*//*CNcomment:打开指定的调试级别*/
#define DEBUG_OFF        0x55cc       /*close debug level*//*CNcomment:关闭指定的调试级别*/
#define DEBUG_DISC       0x55ee       /*disconnect link*//*CNcomment:断开连接*/
#define DEBUG_TEST       0x55ff       /*detect the validity of link*//*CNcomment:检测连接有效性*/
#define DEBUG_GET_MODULE_INFO 0x66aa
#define DEBUG_PUT_MODULE_INFO 0X66bb

//Error code
#define DEBUG_INIT_PARAMETER_ERROR    -1   /*input error when init*//*CNcomment:模块初始化时，输入参数有误*/
#define DEBUG_INIT_SOCKCREATE_ERROR   -2   /*create socket error when init*//*CNcomment:模块初始化时，创建socket有误*/
#define DEBUG_INIT_SOCKBIND_ERROR     -3   /*bind socket error when init*//*CNcomment:模块初始化时，绑定socket有误*/
#define DEBUG_CREATE_MUTEX_ERROR      -4   /*create mutex error when init*//*CNcomment:模块初始化时，创建MUTEX错误*/
/*********************************************************************************
*                          data type                                              *
**********************************************************************************/
/*debug command information structure*//*CNcomment:调试命令信息结构体*/
typedef struct
{
    unsigned int   cmdType;           /*command type*//*CNcomment:命令类型*/
    unsigned int   cmdPara;           /*command parameter*//*CNcomment:命令附加参数*/
}T_Debug_Cmd;

typedef struct
{
    T_Debug_Cmd cmd;
    char        privData[0];
}T_Debug_Cmd_Ext;

#define REVERSE_US(x)   (((x) & 0xff) << 8 | (((x) >> 8) & 0xff))
#define REVERSE_UI(x)   (((x) & 0xff) << 24 | (((x) >> 8) & 0xff) << 16 | (((x)>> 16) & 0xff) << 8 | (((x) >> 24) & 0xff))

void DebugProcessCmd(void);
int DedbugOutIsConnect(void);
/*********************************************************************************
*                             global varible                                       *
**********************************************************************************/
static int      g_iDebugSocket = -2;            //socket
int             g_bConnect = 0;                 /*connect flag*//*CNcomment:调试连接标识*/
unsigned int    g_iDebugOutLevel = 0;           /*debug level*//*CNcomment:调试级别*/
unsigned short  g_usUdpPort;                    /*udp port*//*CNcomment:本地使用的UDP端口，用于表示底层连接*/
static HI_U8    g_acBuf[DEBUG_BUF_SIZE + 1];    /*record buffer*//*CNcomment:记录接收与发送临时信息的缓存*/
struct          sockaddr_in g_tAddr;            /*ip address structure*//*CNcomment:通讯端IP地址存储结构*/
unsigned int    g_uMarkTime;                    /*time of last receive info*//*CNcomment:接收上次信息的时间，单位毫秒*/
int             g_iMutex;                       /*mutex id*//*CNcomment:互斥量ID*/

/*********************************************************************************
* Function Name:InitDebugOut
* Description:inital debug model;//CNcomment:初始化调试信息模块的信息：
*                                       //CNcomment:主要是：创建绑定socket
*                                       //CNcomment:初始化调试信息的级别，所有内部输出级别设定为关闭
* Parameters:no
* Return Value:0-success, other-failure; //CNcomment:成功0，else失败，见头文件定义
**********************************************************************************/
int InitSyncDebugNet(void)
{
    int iResult;
    struct sockaddr_in t_srcAddr;
    memset(&t_srcAddr, 0 ,sizeof(t_srcAddr));
    unsigned short udpPort ;

    udpPort = 10020 ;

    if(udpPort <= 1024)
    {
        return DEBUG_INIT_PARAMETER_ERROR;
    }

    g_usUdpPort = udpPort;
    g_iDebugSocket = My_socket(AF_INET, SOCK_DGRAM, 0);
    if(g_iDebugSocket < 0)
    {
        printf( " dbg socket create err.\n");
        return DEBUG_INIT_SOCKCREATE_ERROR;
    }
    else
        printf( "InitSyncDebugNet: dbg socket create ok,  %d .\n" , g_iDebugSocket );

    t_srcAddr.sin_family = AF_INET;
    t_srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    t_srcAddr.sin_port = htons(udpPort);

    iResult = bind(g_iDebugSocket, (struct sockaddr*)&t_srcAddr, sizeof(t_srcAddr));
    if(iResult < 0)
    {
        My_closesocket(g_iDebugSocket);
        printf( "InitSyncDebugNet: dbg sock bind err , %d.\n" , g_iDebugSocket );
        g_iDebugSocket = -1;
        return DEBUG_INIT_SOCKBIND_ERROR;
    }

    g_iMutex = My_createMutex();
    if(g_iMutex == 0){
        printf("FATAL ERROR-DEBUG:Can not Create Mutex at InitDebug!\n");
        return DEBUG_CREATE_MUTEX_ERROR;
    }
    g_iDebugOutLevel = 0;           
    g_bConnect = 0;
    return 0;
}

void *debug_thread_proc(void *arg)
{
    int loop = 0;


    printf("debug_thread_proc start , pid %d\n" , getpid() );

    while(1)
    {
        DebugProcessCmd();

        usleep(100000);

        loop ++ ;

        // if( 0 == loop % 10 ) DBG_MARK_STEP_DISP();

    }

    return NULL;
}

int InitSyncDebug( int waitLink )
{
    pthread_t  debugout_thread;
    int re_creat_th;

    /* Init debug out mod */
    if (InitSyncDebugNet() != 0)
        printf("ERR MCPROC: -------InitDebugOut faile!--------\n");
    else
        printf("InitDebugOut ok.\n ");

    printf("Start Debug Out Thread.\n");
    re_creat_th = pthread_create(&debugout_thread, NULL, debug_thread_proc , NULL);
    if(0 != re_creat_th)
    {
        printf("ERROR:Create debugout thread fail, error code=%u\n",re_creat_th);
        return -1;
    }

    while(waitLink)
    {
        if( DedbugOutIsConnect() )
            break;

        sleep(1);
        //printf("Waitting for debug connect.\n ");
    }

    return 0;
}


/**************************************************************************
* Function Name:SetDebugLevel
* Description:set debug level and pos
***************************************************************************/
HI_S32 SetDebugLevel(HI_U32 u32Pos, HI_U32 u32Level)
{
    HI_U32 u32ModID    = u32Level & 0x0000ffff;
    HI_U32 u32LogLevel = ( u32Level & 0x000f0000) >> 16;

    HI_MPI_LogPrintPosSet((HI_MOD_ID_E)u32ModID, (LOG_OUTPUT_POS_E)u32Pos);
    HI_SYS_SetLogLevel((HI_MOD_ID_E)u32ModID, (HI_LOG_LEVEL_E)u32LogLevel);

    return HI_SUCCESS;
}

/**************************************************************************
* Function Name:SetDebugLevelForce
* Description:set debug pos with a force level
***************************************************************************/
HI_S32 SetDebugLevelForce(HI_U32 u32Pos, HI_U32 u32Cmd, HI_U32 u32ForceLevel)
{
    HI_U32 u32ModID    = u32Cmd & 0x0000ffff;

    HI_MPI_LogPrintPosSet((HI_MOD_ID_E)u32ModID, (LOG_OUTPUT_POS_E)u32Pos);
    HI_SYS_SetLogLevel((HI_MOD_ID_E)u32ModID, (HI_LOG_LEVEL_E)u32ForceLevel);

    return HI_SUCCESS;

}

#define UNUSED(x) ((x)=(x))
#define MAX_LINE_SIZE (128)
static char line_buf[MAX_LINE_SIZE];

#define STRIP_INVALID_HEAD(nr) do{\
    int index = 0;\
    char *pu8Ret = NULL;\
    for(;index < nr; index++) \
    {\
        pu8Ret = fgets(line_buf, sizeof(line_buf), stream);\
        UNUSED(pu8Ret);\
    }\
}while(0)

#define TAIL_TAG "----------"
#define STRIP_INVALID_TAIL() { break; }

/*
 * get current board software module info from /proc/msp/module
 */
int get_modules_info(char * buff, unsigned int size)
{
        FILE *stream;
#define MAX_ITEM_SIZE (32)
    char s1[MAX_ITEM_SIZE], s2[MAX_ITEM_SIZE], s3[MAX_ITEM_SIZE], s4[MAX_ITEM_SIZE], s5[MAX_ITEM_SIZE], s6[MAX_ITEM_SIZE];

    if (!buff || 0 == size)
        return -1;

    memset(buff, 0, size);

    /* for normal modules */
    stream = fopen("/proc/msp/module", "r");
    if (NULL == stream)
        return -1;

    /* strip first invalid three line */
    STRIP_INVALID_HEAD(3);

        do{
                if (NULL != fgets(line_buf, sizeof(line_buf), stream))
                {
                        if (!strstr(line_buf, TAIL_TAG))
                        {
                sscanf(line_buf, "%s %s %s %s %s %s", s1, s2 ,s3 ,s4, s5, s6);

                /* MODULE_name,MODULE_id,LOG_level; */
                snprintf(line_buf, MAX_LINE_SIZE, "%s,%s,%s;", s2, s3, s5);
                strncat(buff, line_buf, size - strlen(buff) - 1);
                        }
                        else
                        {
                                STRIP_INVALID_TAIL();
                        }
                }
                else
                        STRIP_INVALID_TAIL();
        }while(1);

    fclose(stream);	

    /* for special modules */
    strncat(buff, "VSYNC,0x00000043,ERROR;", size - strlen(buff) - 1);
    strncat(buff, "ASYNC,0x00000044,ERROR;", size - strlen(buff) - 1);

    return 0;
}

/*
 * send modules info to pc tools
 */
int send_modules_info()
{
#define MAX_MODULES_INFO_BUF_SIZE (4*1024)
    char * modules_info ;
    int ret;

    modules_info = malloc(MAX_MODULES_INFO_BUF_SIZE);
    if(!modules_info)
    {
        printf("malloc memory for modules info failed.\n");
        return -1;
    }

    if (!get_modules_info(modules_info, MAX_MODULES_INFO_BUF_SIZE))
    {
        T_Debug_Cmd_Ext *p_send_cmd;
        unsigned int modules_info_len = strlen(modules_info) + 1;
        unsigned int send_cmd_size    = sizeof(T_Debug_Cmd_Ext) + sizeof(char) * modules_info_len;

        p_send_cmd = malloc( send_cmd_size );  
        if (p_send_cmd)
        {
            p_send_cmd->cmd.cmdType = DEBUG_PUT_MODULE_INFO;
            p_send_cmd->cmd.cmdPara = modules_info_len;
            memcpy(p_send_cmd->privData, modules_info, modules_info_len);

            My_sendto(g_iDebugSocket, p_send_cmd, send_cmd_size, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));		

            free(p_send_cmd);
        }
        else
        {
            printf("malloc memory for send cmd failed.\n");
            ret = -1;
            goto out;
        }
    }
    else
    {
        printf("get modules info failed.\n");
        ret = -1;
    }

    ret = 0;

out:
    free(modules_info);

    return ret;
}

/**************************************************************************
* Function Name:DebugProcessCmd
* Description:deal with the command from outter; 
*           //CNcomment:用于接收处理外部控制命令，具有不可重入性。
*           //CNcomment:通过Select监听实现，非阻塞方式，内部实现定时检测功能
*           //CNcomment:当远端长时间（60S）没有连接指示时断开连接
* Parameters:no
* Return Value:no
***************************************************************************/
void DebugProcessCmd(void)
{
    static int iResult;
    static unsigned int  uCurrTime;
    static int iAddrLen = sizeof(struct sockaddr_in);
    static T_Debug_Cmd  *ptCmd;
    static int iCmdLen = sizeof(T_Debug_Cmd);
    static int uMaxFd;
    static struct timeval t_TimeDelay;
    static fd_set t_Readfd;
    static char ucBuf[54];
    static struct sockaddr_in tAddr;
    t_TimeDelay.tv_sec  = 0;
    t_TimeDelay.tv_usec = 0;
    uMaxFd = g_iDebugSocket;


    if( g_iDebugSocket < 0 )
    {
        printf( " Dbg sock handle err , %d .\n" , g_iDebugSocket );
        return;
    }

    while(1)
    {
        FD_ZERO(&t_Readfd);

        FD_SET((SOCKET)g_iDebugSocket, &t_Readfd);

        iResult = select(uMaxFd + 1, &t_Readfd, NULL, NULL, &t_TimeDelay);
        if(iResult <= 0)
        {
            if(g_bConnect)
            {
                uCurrTime = My_getsystime();
                if((unsigned int)(uCurrTime - g_uMarkTime) > DEBUG_TIMEOUT)
                {
                    memset(&g_tAddr, 0, sizeof(g_tAddr));
                    g_uMarkTime = 0;
                    g_bConnect  = 0;
                    g_iDebugOutLevel = 0;
                    sprintf(ucBuf, "Debuger Time out, Port=%u, Disconnected!\n", g_usUdpPort);
                    My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                }
            }
            return;
        }
        else
        {
            if(FD_ISSET(g_iDebugSocket, &t_Readfd))
            {
                /*distill ip and port information*/
                //CNcomment:提取远端IP地址和端口等信息
                My_recvfrom(g_iDebugSocket, ucBuf, iCmdLen, 0, (struct sockaddr*)&tAddr, &iAddrLen);
                g_uMarkTime = My_getsystime();
            }
            ptCmd = (T_Debug_Cmd*)ucBuf;
            ptCmd->cmdType = ntohl(ptCmd->cmdType);
            ptCmd->cmdPara = ntohl(ptCmd->cmdPara);

            switch (ptCmd->cmdType){
            /*inital debug level and estabish link*/
            //CNcomment:建立连接，初始化调试信息级别，附加参数无意义
            case DEBUG_CONN:
            {
                g_iDebugOutLevel = 0;
                g_bConnect = 1;
                memcpy(&g_tAddr, &tAddr, sizeof(struct sockaddr_in));
                /*
                           if(g_bVodSleep)
                           {
                                printf(ucBuf, "Debuger Connect Succeed ON SLEEP, Port=%u, Connected!\n", g_usUdpPort);
                           }
                           else
                          */
                {
                    sprintf(ucBuf, "Debuger Connect Succeed, Port=%u, Connected!\n", g_usUdpPort);
                }
                My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                printf("DebugCONN cmd recv\n");
                break;
            }

            /*open appoint level*/
            //CNcomment:打开指定的调试级别，附加参数为调试级别控制字，与模块内部的调试级别控制字执行或操作
            case DEBUG_ON:
            {
                if(g_bConnect > 0)
                {
                    g_iDebugOutLevel = ptCmd->cmdPara;

                    SetDebugLevel(LOG_OUTPUT_NETWORK, g_iDebugOutLevel);
                }
                else
                {
                    sprintf(ucBuf, "Debuger SetOnLevel Failed, Port=%u, Disconnected!\n", g_usUdpPort);
                    My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                }
                printf("DebugON cmd recv,Level=0x%x,cmd=0x%x\n",g_iDebugOutLevel,ptCmd->cmdPara);
                break;
            }

            /*close appoint level*/
            //CNcomment:关闭指定的调试级别，附加参数为调试级别控制字，与模块内部的调试级别控制字执行与操作
            case DEBUG_OFF:
            {
                if(g_bConnect > 0)
                {
                    g_iDebugOutLevel = ptCmd->cmdPara;
                     /*set debuglevel error when debug off*/
                    SetDebugLevelForce(LOG_OUTPUT_SERIAL, g_iDebugOutLevel, HI_LOG_LEVEL_ERROR);
                }
                else
                {
                    sprintf(ucBuf, "Debuger SetOffLevel Failed, Port=%u, Disconnected!\n", g_usUdpPort);
                    My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                }
                printf("DebugOFF cmd recv,Level=0x%x,cmd=0x%x\n",g_iDebugOutLevel,ptCmd->cmdPara);
                break;
            }

            /*disconnect link, clean address information and debug level*/
            //CNcomment:断开连接，清空地址信息和调试级别
            case DEBUG_DISC:
            {
                if(g_bConnect > 0)
                {
                    memset(&g_tAddr, 0, sizeof(g_tAddr));
                    g_iDebugOutLevel = 0;
                    g_uMarkTime = 0;
                    g_bConnect = 0;
                    SetDebugLevel(LOG_OUTPUT_SERIAL, g_iDebugOutLevel);
                }
                printf("DebugDISC cmd recv\n");
                break;
            }

            case DEBUG_GET_MODULE_INFO:
            {
                if (send_modules_info())
                {
                    printf("send modules info to pc tool failed.\n");

                    sprintf(ucBuf, "send modules info to pc tool failed!\n");
                    My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                }
                break;
            }

            /*detect the validity of link*/
            //CNcomment:检测连接有效性，远端每10s发送一次。
            case DEBUG_TEST:
            {
                // printf("DebugTEST cmd recv\n");
                break;
            }

            default:
            {
                printf("Debug Unknown cmd=%d recv\n",ptCmd->cmdType);
                sprintf(ucBuf, "Debuger Process Command, Port=%u, Wrong Command!\n", g_usUdpPort);
                My_sendto(g_iDebugSocket, ucBuf, strlen(ucBuf) + 1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                break;
            }

            } // end of switch
        }
    }// end of while

}


/**************************************************************************
* Function Name:CloseDebugOut
* Description:close debug model;//CNcomment:关闭调试模块，关闭和清除相关信息
* Parameters:no
* Return Value:no
***************************************************************************/
void CloseDebugOut(void)
{
    printf(" CloseDebugOut .\n" );

    return;
    if(g_iDebugSocket != -1)
    {
        My_closesocket(g_iDebugSocket);
        g_iDebugSocket = -1;
    }
    My_deleteMutex(g_iMutex);
    g_bConnect = 0;
    g_uMarkTime = 0;
    g_iDebugOutLevel = 0;

    return;
}

int DedbugOutIsConnect(void)
{
    return g_bConnect ;
}
#endif


int main(int argc, char *argv[])
{
    HI_U32 ReadLen;

    HI_SYS_Init();


    InitSyncDebug(1);

    while(1)
    {
        if (HI_SUCCESS == HI_MPI_LogRead(g_acBuf, DEBUG_BUF_SIZE, &ReadLen))
        {
            HI_U8  tmp[DEBUG_BUF_SIZE];
            HI_U32 i, total = 0;

            while (total < ReadLen)
            {
                for (i=0;i<DEBUG_BUF_SIZE;i++)
                {
                    /*read over src, quit*/
                    if (total+i > DEBUG_BUF_SIZE)
                        break;

                    tmp[i] = g_acBuf[total+i];

                    /*msg end, then send this msg*/
                    if (g_acBuf[total+i] == '\n')
                    {
                        break;
                    }
                }

                /*find a msg, send it*/
                if (i < DEBUG_BUF_SIZE)
                {
                    My_sendto(g_iDebugSocket, tmp, i+1, 0, (struct sockaddr*)&g_tAddr, sizeof(g_tAddr));
                    total += i+1;
                }
                /*can not find, discard the DEBUG_BUF_SIZE bytes*/
                else
                {
                    //printf("msg overflow\n");
                    total += DEBUG_BUF_SIZE;
                }
            }
        }
    }

    HI_SYS_DeInit();
    return 0;
}





