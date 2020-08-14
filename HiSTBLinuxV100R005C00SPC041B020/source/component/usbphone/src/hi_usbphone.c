/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_unf_usbphone.c
 * Description:
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    2011-08-04   f00172091      NULL         Create this file.
 ***********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <termios.h>

#include "hi_usbphone.h"
#include "hi_module.h"
#include "hi_debug.h"


#define HI_USB_PHONE_FATAL(fmt...) \
            HI_FATAL_PRINT(HI_ID_ALSA, fmt)

#define HI_USB_PHONE_ERR(fmt...) \
            HI_ERR_PRINT(HI_ID_ALSA, fmt)

#define HI_USB_PHONE_WARN(fmt...) \
            HI_WARN_PRINT(HI_ID_ALSA, fmt)

#define HI_USB_PHONE_INFO(fmt...) \
            HI_INFO_PRINT(HI_ID_ALSA, fmt)

#define USB_PHONE_DEV_FILE_HEAD "/dev/ttyUSB"
#define USB_PHONE_RECV_HEAD_LEN 16

/*define received usbphone command*/ /*CNcomment:定义接收到的USB PHONE命令*/
typedef enum
{
    USB_PHONE_RECV_BS_REG = 0,   /*usbphone seat register state*/ /*CNcomment:座机注册状态*/
    USB_PHONE_RECV_OFF_HOOK,     /*hook off*/ /*CNcomment:摘机*/
    USB_PHONE_RECV_ON_HOOK,      /*hook on*/ /*CNcomment:挂机*/
    USB_PHONE_RECV_KEY,          /*key*/ /*CNcomment:按键*/
    USB_PHONE_RECV_HOLD,         /*call hold, reserved*/ /*CNcomment:通话保持，保留*/  
    USB_PHONE_RECV_READY,        /*usbphone seat initialization finish*/ /*CNcomment:座机初始化结束*/  
    USB_PHONE_RECV_VER,          /*version number*/ /*CNcomment:版本号*/  
    USB_PHONE_RECV_ERROR,        /*command execute result ERROR*/ /*CNcomment:命令执行结果ERROR*/  
    USB_PHONE_RECV_OK,           /*command execute result OK*/ /*CNcomment:命令执行结果OK*/  
    USB_PHONE_RECV_STATE,        /*usbphone state*/ /*CNcomment:话机状态*/  
    USB_PHONE_RECV_BUTT
}USB_PHONE_RECV_CMD_E;

/*define type of command sending to usbphone*/ /*CNcomment:定义发送给USB PHONE的命令类型 */
typedef enum
{
    USB_PHONE_CMD_NONE = 0,             /**/
        
    USB_PHONE_CMD_RING_STOP,            /*request stop ring and return wait status*/ /*CNcomment:请求BS停止响铃，并返回待机状态*/
    USB_PHONE_CMD_RING_START,           /*request start ring*/ /*CNcomment:请求BS开始响铃*/
    USB_PHONE_CMD_BUSY,                 /*request busy status of HS line, reserved*/ /*CNcomment:请求BS告知HS线路忙, 保留*/
    USB_PHONE_CMD_HANDSHAKE,            /*handshake command*/ /*CNcomment:GW握手命令*/
    
    USB_PHONE_CMD_BUSY_TONE,            /*request play with busy voice*/ /*CNcomment:GW请求BS播放忙音*/
    USB_PHONE_CMD_HOWLER_TONE,          /*request play with over voice*/ /*CNcomment:GW请求BS播放催挂音*/
    USB_PHONE_CMD_STOP_TONE,            /*request switch play to over voice*/ /*CNcomment:GW请求BS停止音频播放，并通知HS被动挂机*/
    USB_PHONE_CMD_STOP_TONE_ONLY,       /*request stop play*/ /*CNcomment:GW请求BS停止音频播放 */
    USB_PHONE_CMD_DISP_CALLID,          /*inform number of called*/ /*CNcomment:GW通知BS来电号码 */

    USB_PHONE_CMD_TIME,                 /*inform switch to current time, reserved*/ /*CNcomment:GW通知BS统一到当前时间，保留*/
    USB_PHONE_CMD_LINE_STATE,           /*inform line state, if not allowed, will failure and play, reserved*/ /*CNcomment:GW告知BS取线是否允许，若不允许，HS摘机将失败并提示音，保留*/
    USB_PHONE_CMD_RESET,                /*reset command*/ /*CNcomment:控制BS复位重启*/
    USB_PHONE_CMD_CFG_DIALTONE,         /*initial setting, set whether produce dialling tone*/ /*CNcomment:BS的初始化设置，用于设置HS的拨号时是否产生拨号音*/
    USB_PHONE_CMD_CFG_CONTONE,          /*initial setting, set option group of hook off*/ /*CNcomment:BS的初始化设置，用于设置摘机通音选项*/
    USB_PHONE_CMD_WIFI_TEST,            /*WIFI command for interference test*/ /*CNcomment:WIFI干扰测试命令*/
    USB_PHONE_CMD_GET_BS_VER,           /**/ /*CNcomment:GW请求BS停止音频播放*/
    
    USB_PHONE_CMD_BUTT               
}USB_PHONE_CMD_E;

/*define usbphone command execute result*/ /*CNcomment:定义USB PHONE命令执行结果 */
typedef enum
{
    USB_PHONE_CMD_RET_NONE = 0,     /*not return result*/ /*CNcomment:未返回结果*/
    USB_PHONE_CMD_RET_OK,           /**< OK*/
    USB_PHONE_CMD_RET_ERR,          /**< ERROR*/
    USB_PHONE_CMD_RET_BUTT
}USB_PHONE_CMD_RESULT_E;

typedef struct
{
    pthread_t   tThread;
    pthread_mutex_t  tMutex;
    HI_U32   u32RunFlag;
}USB_PHONE_THREAD_STRU;

typedef struct
{
    USB_PHONE_CMD_E enCurCmd;
    USB_PHONE_CMD_RESULT_E  enResult;
}USB_PHONE_SEND_CMD_STRU;

typedef struct
{
    HI_CHAR szHead[USB_PHONE_RECV_HEAD_LEN];
    USB_PHONE_RECV_CMD_E recv;
    HI_S32 (*pfunc)(HI_CHAR *szValue, HI_CHAR **pszNext);
}USB_PHONE_RECV_CMD_STRU;

typedef struct
{
    HI_USB_PHONE_EVENT_CB_FN pfnEventCB;
    HI_U32  args;
}USB_PHONE_EVENT_CB_S;

static USB_PHONE_THREAD_STRU  stThreadRead;

static HI_S32 g_UsbPhoneDevFd = -1;
static pthread_mutex_t   g_UsbPhoneMutex;

#define HI_USB_PHONE_LOCK()     (void)pthread_mutex_lock(&g_UsbPhoneMutex)
#define HI_USB_PHONE_UNLOCK()   (void)pthread_mutex_unlock(&g_UsbPhoneMutex)

static HI_BOOL g_bInited = HI_FALSE;
static HI_S32 g_s32State = HI_USB_PHONE_STATE_BUTT;
static HI_BOOL g_bVerFlag = HI_FALSE;
static HI_CHAR g_szBSVer[HI_USB_PHONE_BUF_SIZE];
static USB_PHONE_EVENT_CB_S g_stEventCB;
static USB_PHONE_SEND_CMD_STRU g_stSendCmd;
static HI_CHAR g_szDevFile[32];

#define CHECK_USB_PHONE_OPEN()\
do{\
    HI_USB_PHONE_LOCK();\
    if(HI_FALSE == g_bInited)\
    {\
        HI_USB_PHONE_UNLOCK();\
        HI_USB_PHONE_ERR("USB_PHONE is not inited.\n");\
        return HI_ERR_USB_PHONE_NOT_INIT;\
    }\
    if (g_UsbPhoneDevFd < 0)\
    {\
        HI_USB_PHONE_UNLOCK();\
        HI_USB_PHONE_ERR("USB_PHONE is not open.\n");\
        return HI_ERR_USB_PHONE_NOT_OPENED;\
    }\
    if(HI_USB_PHONE_STATE_REG == g_s32State)\
    {\
        HI_USB_PHONE_UNLOCK();\
        HI_USB_PHONE_ERR("USB_PHONE is in register state.\n");\
        return HI_ERR_USB_PHONE_INVALID_OPT;\
    }\
    HI_USB_PHONE_UNLOCK();\
}while(0)

static HI_S32 usb_phone_set_serial_speed(HI_S32 fd)
{
	HI_S32 ret = 0;
	struct  termios opt;

	tcgetattr(fd, &opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&opt,B115200);
	cfsetospeed(&opt,B115200);

	ret = tcsetattr(fd,TCSAFLUSH,&opt);
	if(ret <0) {
		HI_USB_PHONE_ERR("set serial port speed failure.\n");
	}
    else{
	    HI_USB_PHONE_INFO("set serial port speed success.\n");
    }
	tcflush(fd,TCIOFLUSH);
    
	return ret;
}

static HI_S32 usb_phone_set_serial_rowmode(HI_S32 fd)
{
	HI_S32 ret = 0;
	struct termios options; 

	tcgetattr(fd, &options);

	tcflush(fd, TCIOFLUSH);
    options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;

	tcflush(fd,TCIFLUSH); 
	options.c_cc[VTIME] = 10;
	options.c_cc[VMIN] = 0;

	cfmakeraw(&options);

	ret = tcsetattr(fd,TCSAFLUSH,&options);
	if (ret < 0) { 
		HI_USB_PHONE_ERR("set serial port attribute failure.\n");
	} 

	return ret;
}

static HI_S32 usb_phone_open_device(HI_CHAR *pszDev)
{
    /*g_UsbPhoneDevFd should be checked outside*/
    
    if(HI_NULL == pszDev)
    {
        HI_USB_PHONE_ERR("null pointer.\n");
		return HI_ERR_USB_PHONE_NULL_PTR; 
    }

    if(strncmp(pszDev,USB_PHONE_DEV_FILE_HEAD,strlen(USB_PHONE_DEV_FILE_HEAD)) != 0)
    {
        HI_USB_PHONE_ERR("Invalid device file %s.\n",pszDev);
        return HI_ERR_USB_PHONE_INVALID_PARA;
    }

//    g_UsbPhoneDevFd = open(pszDev,O_RDWR | O_NOCTTY);
    g_UsbPhoneDevFd = open(pszDev,O_RDWR | O_NOCTTY | O_NDELAY);
    if (g_UsbPhoneDevFd < 0)
    {
        HI_USB_PHONE_ERR("open %s failed.\n",pszDev);
		return HI_ERR_USB_PHONE_OPEN_ERR; 
    }
    fcntl(g_UsbPhoneDevFd, F_SETFL, 0);
    usb_phone_set_serial_speed(g_UsbPhoneDevFd);
    usb_phone_set_serial_rowmode(g_UsbPhoneDevFd);
    
    return HI_SUCCESS;
}

static HI_S32 usb_phone_close_device(HI_VOID)
{
    if (g_UsbPhoneDevFd < 0)
    {
        return HI_SUCCESS;
    }
    
    close(g_UsbPhoneDevFd);
    g_UsbPhoneDevFd = -1;
    return HI_SUCCESS;
}

static HI_S32 usb_phone_cmd2str(HI_CHAR *pSrc, HI_CHAR *pDest,HI_S32 *pLen)
{
	HI_S32 num = HI_USB_PHONE_BUF_SIZE;
	
    if((NULL == pSrc)||(NULL == pDest))
        return HI_FAILURE;
    
    *pLen = 0;
    while(num > 0) //for fix MOTO
    {
        if(*pSrc == 0)
            break;
        
        if((*pSrc == 0x0d)||(*pSrc == 0x0a))
        {
            pSrc++;
            continue;
        }
        *pDest++ = *pSrc++;
        (*pLen)++;
		num--;
    }
    return HI_SUCCESS;
}

static HI_S32 usb_phone_send_cmd(USB_PHONE_CMD_E enCmd, HI_U32 Args)
{
    HI_S32  ret = HI_SUCCESS;
    HI_CHAR szWriteBuf[HI_USB_PHONE_BUF_SIZE];
    HI_U32  len = 0;
    
    memset(szWriteBuf,0,HI_USB_PHONE_BUF_SIZE);
    switch(enCmd)
    {
        case USB_PHONE_CMD_RING_STOP:
            sprintf(szWriteBuf,"AT+TPLR=0\r\n");
            break;
        case USB_PHONE_CMD_RING_START:
            sprintf(szWriteBuf,"AT+TPLR=1\r\n");
            break;
        case USB_PHONE_CMD_BUSY:
            sprintf(szWriteBuf,"AT+TPLR=2\r\n");
            break;
        case USB_PHONE_CMD_HANDSHAKE:
            if(Args == 0)
                sprintf(szWriteBuf,"AT+TPLR=8\r\n");
            else
                sprintf(szWriteBuf,"AT+TPLR=9\r\n");
            break;
        case USB_PHONE_CMD_BUSY_TONE:
            sprintf(szWriteBuf,"AT+TPSN=1\r\n");
            break;
        case USB_PHONE_CMD_HOWLER_TONE:
            sprintf(szWriteBuf,"AT+TPSN=2\r\n");
            break;
        case USB_PHONE_CMD_STOP_TONE:
            sprintf(szWriteBuf,"AT+TPSN=0\r\n");
            break;
        case USB_PHONE_CMD_STOP_TONE_ONLY:
            sprintf(szWriteBuf,"AT+TPSN=9\r\n");
            break;
        case USB_PHONE_CMD_DISP_CALLID:
            sprintf(szWriteBuf,"AT+TPCI=%s\r\n",(HI_CHAR *)Args);
            break;
        case USB_PHONE_CMD_TIME:
            sprintf(szWriteBuf,"AT+TPTS=%s\r\n",(HI_CHAR *)Args);
            break;
        case USB_PHONE_CMD_LINE_STATE:
            sprintf(szWriteBuf,"AT+TPLN=%d\r\n",Args);
            break;
        case USB_PHONE_CMD_RESET:
            sprintf(szWriteBuf,"AT+TPRS\r\n");
            break;
        case USB_PHONE_CMD_CFG_DIALTONE:
            sprintf(szWriteBuf,"AT+TPPD=%d\r\n",Args);
            break;
        case USB_PHONE_CMD_CFG_CONTONE:
            sprintf(szWriteBuf,"AT+TPCT=%d\r\n",Args);
            break;
        case USB_PHONE_CMD_WIFI_TEST:
            sprintf(szWriteBuf,"AT+TPWC=%d\r\n",Args);
            break;
        case USB_PHONE_CMD_GET_BS_VER:
            sprintf(szWriteBuf,"AT+TPVR\r\n");
            break;
        default:
            break;
    }

    len = strlen(szWriteBuf);
    if((len > 0)&&(len < (HI_USB_PHONE_BUF_SIZE-1)))
    {
        ret = write(g_UsbPhoneDevFd,szWriteBuf,len);
        if((HI_U32)ret != len)
        {
            HI_USB_PHONE_ERR("\nwrite error, len %d, ret %d , cmd %s\n",len,ret,szWriteBuf);
            return HI_ERR_USB_PHONE_SEND_ERR;
        }
        
        HI_USB_PHONE_INFO("\nwrite ok, len %d, ret %d , cmd %s\n",len,ret,szWriteBuf);
        
        g_stSendCmd.enCurCmd = enCmd;
        g_stSendCmd.enResult = USB_PHONE_CMD_RET_NONE;
        usleep(10*1000);
    }
    
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_bs_reg(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    HI_CHAR *p = szValue;
    HI_S32 state;

    while(HI_NULL != p)
    {
        if(0 == *p)
            break;
       
        if((*p == '0')||(*p == '1'))
        {
            state = *p - '0';
            if(state == 0)
            {
                g_s32State = HI_USB_PHONE_STATE_IDLE;
            }
            else
            {
                g_s32State = HI_USB_PHONE_STATE_REG;
            }
            *pszNext = p + 1;
            if(HI_NULL != g_stEventCB.pfnEventCB)
            {
                (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_BS_REG, state, g_stEventCB.args);
            }
            return HI_SUCCESS;
        }
        p++;
    }

    HI_USB_PHONE_INFO("Invalid BS register state, %s\n",szValue);
    return HI_ERR_USB_PHONE_RECEIVE_ERR;
}

static HI_S32 usb_phone_recv_cmd_off_hook(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    *pszNext = szValue;
    g_s32State = HI_USB_PHONE_STATE_TALK;

    if(HI_NULL != g_stEventCB.pfnEventCB)
    {
        (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_OFF_HOOK, 0, g_stEventCB.args);
    }
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_on_hook(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    *pszNext = szValue;
    g_s32State = HI_USB_PHONE_STATE_IDLE;
    
    if(HI_NULL != g_stEventCB.pfnEventCB)
    {
        (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_ON_HOOK, 0, g_stEventCB.args);
    }
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_key(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    HI_CHAR *p = szValue;
    HI_CHAR key;
    
    while(HI_NULL != p)
    {
        if(0 == *p)
            break;
       
        if(((*p >= '0')&&(*p <= '9')) ||(*p == '*')
            ||(*p == '#')||(*p == 'R')||(*p == 'P'))
        {
            key = *p;
            *pszNext = p + 1;
            if(HI_NULL != g_stEventCB.pfnEventCB)
            {
                (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_KEY, key, g_stEventCB.args);
            }
            return HI_SUCCESS;
        }
        p++;
    }

    HI_USB_PHONE_INFO("Invalid key, %s\n",szValue);
    return HI_ERR_USB_PHONE_RECEIVE_ERR;
}

static HI_S32 usb_phone_recv_cmd_hold(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    HI_CHAR *p = szValue;
    HI_S32 state;

    while(HI_NULL != p)
    {
        if(0 == *p)
            break;
       
        if((*p == '0')||(*p == '1'))
        {
            state = *p - '0';
            *pszNext = p + 1;
            if(HI_NULL != g_stEventCB.pfnEventCB)
            {
                (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_HOLD, state, g_stEventCB.args);
            }
            return HI_SUCCESS;
        }
        p++;
    }

    HI_USB_PHONE_INFO("Invalid hold state, %s\n",szValue);
    return HI_ERR_USB_PHONE_RECEIVE_ERR;
}

static HI_S32 usb_phone_recv_cmd_ready(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    HI_S32  ret = HI_SUCCESS;
    ret = usb_phone_send_cmd(USB_PHONE_CMD_HANDSHAKE, 1);
    *pszNext = szValue;
    return ret;
}

static HI_S32 usb_phone_recv_cmd_version(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    sprintf(g_szBSVer,"%s",szValue);
    g_bVerFlag = HI_TRUE;
    if(USB_PHONE_CMD_GET_BS_VER == g_stSendCmd.enCurCmd)
        g_stSendCmd.enResult = USB_PHONE_CMD_RET_OK;
    *pszNext = szValue + sizeof(szValue);
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_error(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    g_stSendCmd.enResult = USB_PHONE_CMD_RET_ERR;
    *pszNext = szValue;
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_ok(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    g_stSendCmd.enResult = USB_PHONE_CMD_RET_OK;
    *pszNext = szValue;
    return HI_SUCCESS;
}

static HI_S32 usb_phone_recv_cmd_state(HI_CHAR *szValue, HI_CHAR **pszNext)
{
    HI_U32 state = 0;   //0--idle 1--talk 2--register 3--ring
    HI_CHAR *p = szValue;

    while(HI_NULL != p)
    {
        if(0 == *p)
            break;
       
        if((*p >= '0')&&(*p <= '3'))
        {
            state = (HI_U32)(HI_S32)(*p - '0');
            switch(state)
            {
                case 0:
                    g_s32State = HI_USB_PHONE_STATE_IDLE;
                    break;
                case 1:
                    g_s32State = HI_USB_PHONE_STATE_TALK;
                    if(HI_NULL != g_stEventCB.pfnEventCB)
                    {
                        (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_OFF_HOOK, 0, g_stEventCB.args);
                    }
                    break;
                case 2:
                    g_s32State = HI_USB_PHONE_STATE_REG;
                    if(HI_NULL != g_stEventCB.pfnEventCB)
                    {
                        (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_BS_REG, 1, g_stEventCB.args);
                    }
                    break;
                case 3:
                    g_s32State = HI_USB_PHONE_STATE_RING;
                    break;
                default:
                    break;
            }
            *pszNext = p + 1;
            return HI_SUCCESS;
        }
        p++;
    }

    HI_USB_PHONE_INFO("Invalid line state, %s\n",szValue);
    return HI_ERR_USB_PHONE_RECEIVE_ERR;
}

static const USB_PHONE_RECV_CMD_STRU stRecvCmd[USB_PHONE_RECV_BUTT]={
    {"+PTRG:",  USB_PHONE_RECV_BS_REG,    usb_phone_recv_cmd_bs_reg},
    {"+PTHW",   USB_PHONE_RECV_OFF_HOOK,  usb_phone_recv_cmd_off_hook},
    {"+PTHS",   USB_PHONE_RECV_ON_HOOK,   usb_phone_recv_cmd_on_hook},
    {"+PTKB:",  USB_PHONE_RECV_KEY,       usb_phone_recv_cmd_key},
    {"+PTHD:",  USB_PHONE_RECV_HOLD,      usb_phone_recv_cmd_hold},
    {"+PTIT",   USB_PHONE_RECV_READY,     usb_phone_recv_cmd_ready},
    {"+VERS:",  USB_PHONE_RECV_VER,       usb_phone_recv_cmd_version},
    {"ERROR",   USB_PHONE_RECV_ERROR,     usb_phone_recv_cmd_error},
    {"OK",      USB_PHONE_RECV_OK,        usb_phone_recv_cmd_ok},
    {"+PTLS:",  USB_PHONE_RECV_STATE,     usb_phone_recv_cmd_state}
};

static HI_VOID *usb_phone_read(HI_VOID *arg)
{
    HI_S32 ret = 0;
    HI_S32 len = 0;
    HI_CHAR szReadBuf[HI_USB_PHONE_BUF_SIZE];
    HI_CHAR szRecvCmdStr[HI_USB_PHONE_BUF_SIZE];
    HI_CHAR *pszCmd = HI_NULL;

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    HI_USB_PHONE_INFO("usb phone read thread start.\n");
    
    while(stThreadRead.u32RunFlag)
    {
        HI_USB_PHONE_LOCK();
        if(g_UsbPhoneDevFd < 0)
        {
            HI_USB_PHONE_UNLOCK();
            usleep(10*1000);
            continue;
        }
        HI_USB_PHONE_UNLOCK();
        
        memset(szReadBuf,0,HI_USB_PHONE_BUF_SIZE);
        ret = read(g_UsbPhoneDevFd,szReadBuf,HI_USB_PHONE_BUF_SIZE);
        HI_USB_PHONE_LOCK();
        if(g_UsbPhoneDevFd < 0)
        {
            HI_USB_PHONE_UNLOCK();
            continue;
        }
        else if(ret > 0)
        {

            memset(szRecvCmdStr,0,HI_USB_PHONE_BUF_SIZE);
            usb_phone_cmd2str(szReadBuf,szRecvCmdStr,&len);
            HI_USB_PHONE_INFO("read %d/%d/%d bytes, %s\n",ret,len,strlen(szRecvCmdStr),szRecvCmdStr); 
            
            pszCmd = szRecvCmdStr;
            while(pszCmd < (szRecvCmdStr + len))
            {
                USB_PHONE_RECV_CMD_STRU *pstRecvCmd = HI_NULL;
                HI_U32  i;
                    
                for(i=0; i<USB_PHONE_RECV_BUTT; i++)
                {
                    pstRecvCmd = (USB_PHONE_RECV_CMD_STRU *)&stRecvCmd[i];
                    if(strncmp(pszCmd,pstRecvCmd->szHead,strlen(pstRecvCmd->szHead)) == 0)
                        break;
                }
                if(i >= USB_PHONE_RECV_BUTT)
                    break;

                if(HI_NULL == pstRecvCmd->pfunc)
                    break;
                
                ret = pstRecvCmd->pfunc(pszCmd + strlen(pstRecvCmd->szHead), &pszCmd);
                if(ret != HI_SUCCESS)
                {
                    break;
                }
//                HI_USB_PHONE_INFO("processed cmd len %d.\n",pszCmd-szRecvCmdStr);
            }

            if(g_s32State == HI_USB_PHONE_STATE_BUTT)
            {
                (HI_VOID)usb_phone_send_cmd(USB_PHONE_CMD_HANDSHAKE, 1);
            }
        }
        else
        {
            usb_phone_close_device();
            g_s32State = HI_USB_PHONE_STATE_BUTT;
            if(HI_NULL != g_stEventCB.pfnEventCB)
            {
                (HI_VOID)g_stEventCB.pfnEventCB(HI_USB_PHONE_EVENT_DISCON, 0, g_stEventCB.args);
            }
            HI_USB_PHONE_ERR("read error %d.\n",ret);
        }
        HI_USB_PHONE_UNLOCK();
    }     

    HI_USB_PHONE_INFO("usb phone read thread end.\n");
    pthread_exit(NULL);
}


static HI_S32 usb_phone_thread_init(void)
{
    HI_S32  ret;

    HI_USB_PHONE_INFO("Thread initial start.\n");

    memset(&stThreadRead, 0, sizeof(USB_PHONE_THREAD_STRU));
    ret = pthread_mutex_init(&stThreadRead.tMutex,NULL);
    if(ret != HI_SUCCESS)
    {
        HI_USB_PHONE_ERR("Initial thread mutex err.\n");
        goto Tread_Init_Failure;
    }
    stThreadRead.u32RunFlag = 1;
    ret = pthread_create(&stThreadRead.tThread, HI_NULL, usb_phone_read, HI_NULL);
    if(ret != HI_SUCCESS)
    {
        pthread_mutex_destroy(&stThreadRead.tMutex);
        HI_USB_PHONE_ERR("create read pthread err.\n");
        goto Tread_Init_Failure;
    }
    HI_USB_PHONE_INFO("Thread initial success.\n");
    return HI_SUCCESS;

Tread_Init_Failure:    
    HI_USB_PHONE_ERR("Thread initial failure.\n");
    return HI_FAILURE;
}

static HI_S32 usb_phone_thread_deinit(void)
{

    HI_USB_PHONE_INFO("Thread deinitial start.\n");

    stThreadRead.u32RunFlag = 0;
    pthread_cancel(stThreadRead.tThread);
    pthread_join(stThreadRead.tThread, NULL);
    pthread_mutex_destroy(&stThreadRead.tMutex);
    memset(&stThreadRead,0,sizeof(USB_PHONE_THREAD_STRU));
    
    HI_USB_PHONE_INFO("Thread deinitial end.\n");

    return HI_SUCCESS;
}

HI_S32 HI_USB_PHONE_Init(HI_VOID)
{
    HI_S32 ret;
    HI_USB_PHONE_LOCK();
    
    if(HI_TRUE == g_bInited)
    {
        HI_USB_PHONE_UNLOCK();
        return HI_SUCCESS;
    }
    
    g_bVerFlag = HI_FALSE;
    g_s32State = HI_USB_PHONE_STATE_BUTT;
    memset(g_szBSVer, 0, HI_USB_PHONE_BUF_SIZE);
    memset(&g_stEventCB, 0, sizeof(USB_PHONE_EVENT_CB_S));
    memset(&g_stSendCmd, 0, sizeof(USB_PHONE_SEND_CMD_STRU));
    memset(g_szDevFile,0,sizeof(g_szDevFile));
    ret = usb_phone_thread_init();
    if(HI_SUCCESS != ret)
    {
        HI_USB_PHONE_UNLOCK();
        return ret;
    }
    
    g_bInited = HI_TRUE;
    HI_USB_PHONE_UNLOCK();

    return HI_SUCCESS;
}

    
HI_S32 HI_USB_PHONE_DeInit(HI_VOID)
{
    HI_USB_PHONE_LOCK();

    if(HI_FALSE == g_bInited)
    {
        HI_USB_PHONE_UNLOCK();
        return HI_SUCCESS;
    }

    usb_phone_thread_deinit();

    usb_phone_close_device();

    g_bVerFlag = HI_FALSE;
    g_s32State = HI_USB_PHONE_STATE_BUTT;
    memset(g_szBSVer, 0, HI_USB_PHONE_BUF_SIZE);
    memset(&g_stEventCB, 0, sizeof(USB_PHONE_EVENT_CB_S));
    memset(&g_stSendCmd, 0, sizeof(USB_PHONE_SEND_CMD_STRU));
    g_bInited = HI_FALSE;
    HI_USB_PHONE_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_USB_PHONE_Open(HI_CHAR *pszDev)
{
    HI_S32  Ret;
        
    if(HI_NULL == pszDev)
    {
        HI_USB_PHONE_ERR("Null pointer.\n");
        return HI_ERR_USB_PHONE_NULL_PTR;
    }
    
    HI_USB_PHONE_LOCK();

    if(HI_FALSE == g_bInited)
    {
        HI_USB_PHONE_UNLOCK();
        HI_USB_PHONE_ERR("USB PHONE is not inited.\n");
        return HI_ERR_USB_PHONE_NOT_INIT;
    }

    if(g_UsbPhoneDevFd >= 0)
    {
        HI_USB_PHONE_UNLOCK();
        HI_USB_PHONE_INFO("USB PHONE is opened.\n");
        return HI_SUCCESS;
    }
        
    Ret = usb_phone_open_device(pszDev);
    if(HI_SUCCESS != Ret)
    {
        HI_USB_PHONE_UNLOCK();
        return Ret;
    }
    strcpy(g_szDevFile,pszDev);

    g_s32State = HI_USB_PHONE_STATE_BUTT;
    
    usleep(20*1000);
    Ret = usb_phone_send_cmd(USB_PHONE_CMD_HANDSHAKE, 1);
    HI_USB_PHONE_UNLOCK();
    {
        HI_U32  i;

        i = 0;
        while( i < 50)
        {
            HI_USB_PHONE_LOCK();
            if(g_s32State != HI_USB_PHONE_STATE_BUTT)
            {
//                HI_USB_PHONE_INFO("USB Phone open success, state is %d, wait %d ms\n",g_s32State,i*10+10);
                HI_USB_PHONE_UNLOCK();
                return HI_SUCCESS; 
            }
            HI_USB_PHONE_UNLOCK();
            usleep(10*1000);
            i++;
        }
        
        HI_USB_PHONE_LOCK();
        usb_phone_send_cmd(USB_PHONE_CMD_HANDSHAKE, 0);
        usb_phone_close_device();
        Ret = HI_ERR_USB_PHONE_OPEN_ERR;
        HI_USB_PHONE_UNLOCK();
    }
        
    return Ret;
}

HI_S32 HI_USB_PHONE_Close(HI_VOID)
{

    HI_USB_PHONE_LOCK();
    
    if(HI_FALSE == g_bInited)
    {
        HI_USB_PHONE_UNLOCK();
        HI_USB_PHONE_ERR("USB PHONE is not inited.\n");
        return HI_ERR_USB_PHONE_NOT_INIT;
    }
    
    if(g_UsbPhoneDevFd < 0)
    {
        HI_USB_PHONE_UNLOCK();
        HI_USB_PHONE_INFO("USB PHONE is closed.\n");
        return HI_SUCCESS;
    }

    usb_phone_send_cmd(USB_PHONE_CMD_HANDSHAKE, 0);
    
    usleep(30*1000); 
    
    usb_phone_close_device();
    g_s32State = HI_USB_PHONE_STATE_BUTT;
    
    HI_USB_PHONE_UNLOCK();
        
    return HI_SUCCESS;
}


HI_S32 HI_USB_PHONE_Reset(HI_VOID)
{
//    HI_S32  Ret;
        
//    CHECK_USB_PHONE_OPEN();
   
//    Ret = usb_phone_send_cmd(USB_PHONE_CMD_RESET, 0);
        
//    return Ret;
    return HI_ERR_USB_PHONE_INVALID_OPT;
}


HI_S32 HI_USB_PHONE_Ring(HI_BOOL bEnable)
{
    HI_S32  Ret = HI_ERR_USB_PHONE_INVALID_OPT;
        
    CHECK_USB_PHONE_OPEN();

    HI_USB_PHONE_LOCK();

    if(bEnable)
    {
        if(g_s32State == HI_USB_PHONE_STATE_IDLE)
        {
            g_s32State = HI_USB_PHONE_STATE_RING;
            Ret = usb_phone_send_cmd(USB_PHONE_CMD_RING_START, 0);
        }
    }
    else
    {
        if(g_s32State == HI_USB_PHONE_STATE_RING)
        {
            g_s32State = HI_USB_PHONE_STATE_IDLE;
            Ret = usb_phone_send_cmd(USB_PHONE_CMD_RING_STOP, 0);
        }
    }

    HI_USB_PHONE_UNLOCK();
        
    return Ret;
}

HI_S32 HI_USB_PHONE_PlayTone(HI_USB_PHONE_TONE_TYPE_E enToneType)
{
    HI_S32  Ret;
        
    CHECK_USB_PHONE_OPEN();
    
    if(enToneType >= HI_USB_PHONE_TONE_BUTT)
        return HI_ERR_USB_PHONE_INVALID_PARA;

    HI_USB_PHONE_LOCK();

    if(g_s32State != HI_USB_PHONE_STATE_TALK)
    {
        HI_USB_PHONE_UNLOCK();
        return HI_ERR_USB_PHONE_INVALID_OPT;
    }
    
    if(HI_USB_PHONE_BUSY_TONE == enToneType)
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_BUSY_TONE, 0);
    else if(HI_USB_PHONE_HOWLER_TONE == enToneType)
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_HOWLER_TONE, 0);
    else
        Ret = HI_ERR_USB_PHONE_INVALID_PARA;

    HI_USB_PHONE_UNLOCK();
        
    return Ret;
}

HI_S32 HI_USB_PHONE_StopTone(HI_BOOL bOnHook)
{
    HI_S32  Ret;
        
    CHECK_USB_PHONE_OPEN();
    
    HI_USB_PHONE_LOCK();

    if(g_s32State != HI_USB_PHONE_STATE_TALK)
    {
        HI_USB_PHONE_UNLOCK();
        return HI_ERR_USB_PHONE_INVALID_OPT;
    }
    
    if(bOnHook)
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_STOP_TONE, 0);
    else
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_STOP_TONE_ONLY, 0);
    
    HI_USB_PHONE_UNLOCK();
        
    return Ret;
}

/*Attention: Display call in ID should after ring on 300~600ms*/
HI_S32 HI_USB_PHONE_DisplayCallID(HI_CHAR *pszCallID)
{
    HI_S32  Ret;
    HI_CHAR szTemp[HI_USB_PHONE_BUF_SIZE];
    HI_U32  len = 0;
    HI_CHAR *pSrc;
    HI_CHAR *pDest;
        
    CHECK_USB_PHONE_OPEN();

    if(HI_NULL == pszCallID)
    {
        HI_USB_PHONE_ERR("Null pointer.\n");
        return HI_ERR_USB_PHONE_NULL_PTR;
    }

    HI_USB_PHONE_LOCK();

    memset(szTemp,0,HI_USB_PHONE_BUF_SIZE);
    pSrc = pszCallID;
    pDest = szTemp;
    len = 0;
    while(len < (HI_USB_PHONE_BUF_SIZE-10-1))
    {
        if(0 == *pSrc)
        {
            break;
        }
        else if(((*pSrc >= '0')&&(*pSrc <= '9'))||(*pSrc == 'O')||(*pSrc == 'P'))
        {
            *pDest++ = *pSrc++;
            len++;
        }
        else
        {
            pSrc++;
        }
    }

    if(len == 0)
    {
        HI_USB_PHONE_UNLOCK();
        HI_USB_PHONE_ERR("Invalid call id, %s\n",pszCallID);
        return HI_ERR_USB_PHONE_INVALID_PARA;
    }

    Ret = usb_phone_send_cmd(USB_PHONE_CMD_DISP_CALLID, (HI_U32)szTemp);

    HI_USB_PHONE_UNLOCK();

    return Ret;
}

HI_S32 HI_USB_PHONE_Config_DialTone(HI_BOOL bEnable)
{
    HI_S32  Ret;
        
    CHECK_USB_PHONE_OPEN();

    HI_USB_PHONE_LOCK();

    if(bEnable)
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_CFG_DIALTONE, 1);
    else
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_CFG_DIALTONE, 0);

    HI_USB_PHONE_UNLOCK();

    return Ret;
}


HI_S32 HI_USB_PHONE_Config_ConTone(HI_BOOL bEnable)
{
    HI_S32  Ret;
        
    CHECK_USB_PHONE_OPEN();

    HI_USB_PHONE_LOCK();

    if(bEnable)
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_CFG_CONTONE, 1);
    else
        Ret = usb_phone_send_cmd(USB_PHONE_CMD_CFG_CONTONE, 0);

    HI_USB_PHONE_UNLOCK();
   
    return Ret;
}

HI_S32 HI_USB_PHONE_GetBSVersion(HI_CHAR **pszBSVersion)
{
    HI_S32  Ret;
    HI_U32  i = 0;
        
    CHECK_USB_PHONE_OPEN();
    if(HI_NULL == pszBSVersion)
    {
        HI_USB_PHONE_ERR("Null pointer.\n");
        return HI_ERR_USB_PHONE_NULL_PTR;
    }

    *pszBSVersion = g_szBSVer;
    i = 0;
    while( i < 100)
    {
        HI_USB_PHONE_LOCK();
        if(HI_TRUE == g_bVerFlag)
        {
            HI_USB_PHONE_UNLOCK();
            return HI_SUCCESS; 
        }
        if((i%10)==0)
        {
            Ret = usb_phone_send_cmd(USB_PHONE_CMD_GET_BS_VER, 0);
            if(HI_SUCCESS != Ret)
            {
                HI_USB_PHONE_UNLOCK();
                return Ret; 
            }
        }
        HI_USB_PHONE_UNLOCK();
        usleep(10*1000);
        i++;
    }

    HI_USB_PHONE_ERR("Get USB Phone BS version failure.\n");
    return HI_ERR_USB_PHONE_RECEIVE_ERR;
}

HI_S32 HI_USB_PHONE_GetState(HI_USB_PHONE_STATE_E *penState)
{
    if(HI_NULL == penState)
    {
        HI_USB_PHONE_ERR("Null pointer.\n");
        return HI_ERR_USB_PHONE_NULL_PTR;
    }
    HI_USB_PHONE_LOCK();
    *penState = g_s32State;
    HI_USB_PHONE_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_USB_PHONE_WIFI_Test(HI_USB_PHONE_WIFI_MODE_E enWifiMode)
{
    HI_S32  Ret;
        
    CHECK_USB_PHONE_OPEN();

    if(enWifiMode >= HI_USB_PHONE_WIFI_MODE_BUTT)
    {
        HI_USB_PHONE_ERR("Invalie Wifi mode %d.\n",enWifiMode);
        return HI_ERR_USB_PHONE_INVALID_PARA;
    }

    HI_USB_PHONE_LOCK();

    Ret = usb_phone_send_cmd(USB_PHONE_CMD_WIFI_TEST, enWifiMode);

    HI_USB_PHONE_UNLOCK();
    
    return Ret;
}

HI_S32 HI_USB_PHONE_RegisterEvent(HI_USB_PHONE_EVENT_CB_FN pfnEventCB, HI_U32 u32Args)
{
    HI_USB_PHONE_LOCK();
    if(HI_FALSE == g_bInited)
    {
        HI_USB_PHONE_ERR("USB PHONE is not inited.\n");
        return HI_ERR_USB_PHONE_NOT_INIT;
    }

    g_stEventCB.pfnEventCB = pfnEventCB;
    g_stEventCB.args = u32Args;

    HI_USB_PHONE_UNLOCK();
    return HI_SUCCESS;
}


