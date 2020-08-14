/*****************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : sample_sta.c
  Version       : Initial Draft
  Author        : Hisilicon sdk software group
  Created       :
  Last Modified :
  Description   : Demo of wifi STA interfaces usage.
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <spawn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "hi_wlan_sta.h"
#include "sta_ui.h"

#define VERSION         "1.0.4"
#define IFNAMSIZ        16
#define MAX_AP_NUM      32

const HI_PCHAR sample_version = "sample_sta v" VERSION "\n";

typedef enum hiHI_WLAN_STA_STATE_E
{
    STA_STATE_DISABLED,
    STA_STATE_ENABLED,
    STA_STATE_SCANNING,
    STA_STATE_CONNECTING,
    STA_STATE_CONNECTED,
    STA_STATE_BUTT,
} HI_WLAN_STA_STATE_E;

typedef enum hiHI_WLAN_MSG_E
{
    /* commands */
    STA_CMD_ENABLE,        // start WiFi STA
    STA_CMD_DISABLE,       // stop WiFi
    STA_CMD_GET_STATUS,    // get current connection status
    STA_CMD_SCAN,          // start to scan
    STA_CMD_GET_SCANRESULTS,  // get scan results
    STA_CMD_CONNECT,       // connect to AP
    STA_CMD_DISCONNECT,    // disconnect to AP
    STA_CMD_MACADDRESS,    // get local WiFi MAC address
    STA_CMD_QUIT,          // quit sample_sta
    /* events */
    STA_EVT_SCANRESULTS_AVAILABLE,  // scan is done, results is ready
    STA_EVT_DISCONNECTED,  // connection has been disconnected
    STA_EVT_CONNECTING,    // try to connect to AP
    STA_EVT_CONNECTED,     // connect successfully
    STA_EVT_SUPP_STOPPED,  // wpa_supplicant exited
    STA_EVT_DRIVER_STOPPED,  // driver stopped
    WLAN_MSG_BUTT,
} HI_WLAN_MSG_E;

typedef union hiHI_WLAN_MSG_DATA_U
{
    HI_WLAN_STA_CONFIG_S config;
    HI_CHAR bssid[BSSID_LEN+1];
}HI_WLAN_MSG_DATA_U;

/* command/event information */
typedef struct hiHI_WLAN_MESSAGE_S
{
    HI_WLAN_MSG_E what;
    HI_WLAN_MSG_DATA_U obj;
} HI_WLAN_MESSAGE_S;

struct sNode{
    HI_WLAN_MESSAGE_S message;
    struct sNode *next;
};

struct sQueue{
    struct sNode *front;
    struct sNode *rear;
};

typedef struct hiHI_WLAN_DATA_S
{
    HI_CHAR ifname[IFNAMSIZ+1];
    HI_CHAR mac[BSSID_LEN+1];
    /* save AP list of scan */
    HI_WLAN_STA_ACCESSPOINT_S aplist[MAX_AP_NUM];
    HI_U32  ap_num;
    /* configuration of network connected or connecting */
    HI_WLAN_STA_CONFIG_S config;
    /* connection status */
    HI_WLAN_STA_CONN_STATUS_S connection;
    /* current state of WiFi */
    HI_WLAN_STA_STATE_E state;
    /* UI thread */
    pthread_t uiThread;
    /* mutex */
    pthread_mutex_t mut;
    pthread_cond_t cond;
    struct sQueue cmd_queue;
    /* used for power save, if true, need turn on after wakeup */
    HI_BOOL persist_on;
} HI_WLAN_DATA_S;

HI_VOID wlan_stop_udhcpc(HI_VOID);

static HI_WLAN_DATA_S *pData;
static const HI_CHAR DEV_WIFI_DIR[] = "/dev/wifi";
static const HI_CHAR PID_FILE[] = "/dev/wifi/sample.pid";

static HI_VOID usage(HI_VOID)
{
    printf("\nUsage:\n");
    printf("  enable      start wifi station, load driver and start supplicant\n");
    printf("  disable     stop wifi station, stop supplicant and unload driver\n");
    printf("  scan        scan for APs\n");
    printf("  connect     connect to AP, for example: connect HISILICON\n");
    printf("  disconnect  disconnect to network\n");
    printf("  status      request current connection status\n");
    printf("  mac         request local wifi mac address\n");
    printf("  quit        quit sample_sta\n");
    printf("  q           the same as 'quit'\n");
    printf("  help        show this message\n");
}

static HI_VOID wlan_cleanup(HI_VOID)
{
    if (pData->uiThread)
        pthread_cancel(pData->uiThread);

    if (pData->state != STA_STATE_DISABLED) {
        wlan_stop_udhcpc();
        HI_WLAN_STA_Stop(pData->ifname);
        HI_WLAN_STA_Close(pData->ifname);
    }

    if (pData)
        free(pData);

    HI_WLAN_STA_DeInit();

    unlink(PID_FILE);
}

static HI_VOID wlan_terminate(HI_S32 sig)
{
    wlan_cleanup();
    exit(0);
}

static HI_VOID wlan_print_status(HI_WLAN_DATA_S *pAd)
{
    HI_S32 i;
    HI_CHAR state[13] = {0};

    printf("\n==============================================================\n");
    printf("Scan List:\n");
    printf("  ID / SSID / MAC Address / Channel / Security / Signal Level\n");
    for (i = 0; i < pAd->ap_num; i++) {
        HI_CHAR sec[13];
		memset(sec, 0, sizeof(sec));
        switch (pAd->aplist[i].security) {
            case HI_WLAN_SECURITY_OPEN:
                strcpy(sec, "OPEN");
                break;
            case HI_WLAN_SECURITY_WEP:
                strcpy(sec, "WEP");
                break;
            case HI_WLAN_SECURITY_WPA_WPA2_PSK:
                strcpy(sec, "WPA_WPA2_PSK");
                break;
            case HI_WLAN_SECURITY_WPA_WPA2_EAP:
                strcpy(sec, "WPA_WPA2_EAP");
                break;
            case HI_WLAN_SECURITY_WAPI_PSK:
                strcpy(sec, "WAPI_PSK");
                break;
            case HI_WLAN_SECURITY_WAPI_CERT:
                strcpy(sec, "WAPI_CERT");
                break;
            default:
                strcpy(sec, "UNKOWN");
                break;
        }
        printf("  %02d  %s  %s  %d  %s  %d\n", i, pAd->aplist[i].ssid,
                        pAd->aplist[i].bssid,
                        pAd->aplist[i].channel,
                        sec,
                        pAd->aplist[i].level);
    }
    printf("\n");
    printf("Connection:\n");
    printf("  Status / AP\n");
    if (pAd->connection.state == HI_WLAN_STA_CONN_STATUS_DISCONNECTED)
        strcpy(state, "DISCONNECTED");
    else if (pAd->connection.state == HI_WLAN_STA_CONN_STATUS_CONNECTING)
        strcpy(state, "CONNECTING");
    else if (pAd->connection.state == HI_WLAN_STA_CONN_STATUS_CONNECTED)
        strcpy(state, "CONNECTED");
    else
        strcpy(state, "UNKOWN");
    printf("  %s   %s\n", state,
           	pAd->connection.state == HI_WLAN_STA_CONN_STATUS_DISCONNECTED ?
           	"NONE" : pAd->connection.ap.ssid);
    printf("==============================================================\n");
    printf("> ");
    fflush(stdout);
}

/* insert a element to queue's end */
HI_S32 wlan_enqueue(struct sQueue *pQueue, HI_WLAN_MESSAGE_S *element)
{
    struct sNode *pNew;

    if (pQueue == NULL || element == NULL)
        return -1;

    /* Create a new node */
    pNew = malloc(sizeof(struct sNode));
    if(pNew == NULL){
        return -1;
    }

    pNew->message = *element;
    pNew->next = NULL;

    if(pQueue->rear == NULL){
        /* queue is empty, set front and rear points to new node */
        pQueue->front = pQueue->rear = pNew;
    }else{
        /* queue is not empty, set rear points to the new node */
        pQueue->rear = pQueue->rear->next = pNew;
    }

    return HI_SUCCESS;
}

/* read a element from queue's front, then free the buffer */
HI_S32 wlan_dequeue(struct sQueue *pQueue, HI_WLAN_MESSAGE_S *element)
{
    struct sNode *p;

    if (pQueue == NULL || element == NULL)
        return HI_FAILURE;

    if(pQueue->front == NULL){
        return HI_FAILURE;
    }

    *element = pQueue->front->message;
    p = pQueue->front;
    pQueue->front = p->next;
    /* if the queue is empty, set rear = NULL */
    if(pQueue->front == NULL){
        pQueue->rear = NULL;
    }
    free(p);

    return HI_SUCCESS;
}

/* receive events from interface layer, then send to main loop */
static HI_VOID wlan_event_receiver(HI_WLAN_STA_EVENT_E event, HI_VOID *pPrivateData)
{
    HI_WLAN_MESSAGE_S message;

    memset(&message, 0, sizeof(HI_WLAN_MESSAGE_S));
    message.what = WLAN_MSG_BUTT;

    switch (event) {
        case HI_WLAN_STA_EVENT_DISCONNECTED:
            message.what = STA_EVT_DISCONNECTED;
        break;

        case HI_WLAN_STA_EVENT_SCAN_RESULTS_AVAILABLE:
            message.what = STA_EVT_SCANRESULTS_AVAILABLE;
        break;

        case HI_WLAN_STA_EVENT_CONNECTING:
            message.what = STA_EVT_CONNECTING;
            strcpy(message.obj.bssid, pPrivateData);
        break;

        case HI_WLAN_STA_EVENT_CONNECTED:
            message.what = STA_EVT_CONNECTED;
            strcpy(message.obj.bssid, pPrivateData);
        break;

        case HI_WLAN_STA_EVENT_SUPP_STOPPED:
            message.what = STA_EVT_SUPP_STOPPED;
        break;

        case HI_WLAN_STA_EVENT_DRIVER_STOPPED:
            message.what = STA_EVT_DRIVER_STOPPED;
        break;

        default:
        break;
    }
    if (message.what != WLAN_MSG_BUTT) {
        pthread_mutex_lock(&pData->mut);
        if (wlan_enqueue(&pData->cmd_queue, &message) == HI_SUCCESS)
            pthread_cond_signal(&pData->cond);  // notify the main loop
        pthread_mutex_unlock(&pData->mut);
    }
}

/* process user input */
static HI_S32 wlan_ui_cmd_cb(HI_VOID *ctx, HI_CHAR *cmd)
{
    HI_S32 ret = HI_FAILURE;
    HI_WLAN_DATA_S *pAd = (HI_WLAN_DATA_S *)ctx;
    HI_WLAN_MESSAGE_S message;

    memset(&message, 0, sizeof(HI_WLAN_MESSAGE_S));
    if (strcmp(cmd, "enable") == 0) {
        if (pAd->state == STA_STATE_DISABLED) {
            message.what = STA_CMD_ENABLE;
            ret = HI_SUCCESS;
        }
    } else if (strcmp(cmd, "disable") == 0) {
        message.what = STA_CMD_DISABLE;
        ret = HI_SUCCESS;
    } else if (strcmp(cmd, "scan") == 0) {
        if (pAd->state != STA_STATE_DISABLED) {
            message.what = STA_CMD_SCAN;
            ret = HI_SUCCESS;
    	}
    } else if (strncmp(cmd, "connect", 7) == 0) {
        if (pAd->state != STA_STATE_DISABLED) {
            HI_CHAR input[256];

            message.what = STA_CMD_CONNECT;

            cmd += 7;
            /* eat blanks before SSID */
            while (*cmd != '\0') {
                if (*cmd != ' ')
                    break;

                cmd++;
            }
            /* SSID len is between 1 - 32(ASCII)/128(GB2312)  */
            if (strlen(cmd) == 0 || strlen(cmd) > 128) {
                printf("SAMPLE_STA: SSID length is wrong!\n");
                goto cmd_out;
            }
            strcpy(message.obj.config.ssid, cmd);

            memset(input, 0, sizeof(input));
            printf("Please input security(OPEN/WEP/WPA_WPA2_PSK): ");
            scanf("%s", input);
            if (strcmp(input, "OPEN") == 0)
                message.obj.config.security = HI_WLAN_SECURITY_OPEN;
            else if (strcmp(input, "WEP") == 0)
                message.obj.config.security = HI_WLAN_SECURITY_WEP;
            else if (strcmp(input, "WPA_WPA2_PSK") == 0)
                message.obj.config.security = HI_WLAN_SECURITY_WPA_WPA2_PSK;
            else {
                printf("SAMPLE_STA: security (%s) is not supported!\n", input);
                goto cmd_out;
            }

            if (message.obj.config.security == HI_WLAN_SECURITY_WEP
                 || message.obj.config.security == HI_WLAN_SECURITY_WPA_WPA2_PSK) {
                memset(input, 0, sizeof(input));
                printf("Please input password: ");
                scanf("%s", input);
                strcpy(message.obj.config.psswd, input);
            }

            /* set hidden_ssid */
            memset(input, 0, sizeof(input));
            printf("Hidden SSID?(0-no, 1-yes): ");
            scanf("%s", input);
            if (strcmp(input, "1") == 0)
                message.obj.config.hidden_ssid = HI_TRUE;
            else
                message.obj.config.hidden_ssid = HI_FALSE;

            ret = HI_SUCCESS;
        }
    } else if (strcmp(cmd, "disconnect") == 0) {
        if (pAd->state != STA_STATE_DISABLED) {
            message.what = STA_CMD_DISCONNECT;
            ret = HI_SUCCESS;
    	}
    } else if (strcmp(cmd, "status") == 0) {
        if (pAd->state != STA_STATE_DISABLED) {
            message.what = STA_CMD_GET_STATUS;
            ret = HI_SUCCESS;
    	}
    } else if (strcmp(cmd, "mac") == 0) {
        if (pAd->state != STA_STATE_DISABLED) {
            message.what = STA_CMD_MACADDRESS;
            ret = HI_SUCCESS;
    	}
    } else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0) {
        message.what = STA_CMD_QUIT;
        ret = HI_SUCCESS;
    } else if (strcmp(cmd, "help") == 0) {
        usage();
        return HI_SUCCESS;
    } else {
        printf("SAMPLE_STA: command not supported\n");
        ret = HI_FAILURE;
    }

cmd_out:
    if (ret == HI_SUCCESS) {
        pthread_mutex_lock(&pAd->mut);
        if (wlan_enqueue(&pAd->cmd_queue, &message) == HI_SUCCESS)
            pthread_cond_signal(&pAd->cond);  // notify the main loop
        pthread_mutex_unlock(&pAd->mut);
    }

    return ret;
}

static HI_VOID wlan_ui_eof_cb(HI_VOID *ctx)
{
    // Do nothing
    printf("SAMPLE_STA: eof\n");
}

static HI_VOID *wlan_ui_thread_proc(HI_VOID *args)
{
    HI_S32 ret;
    HI_WLAN_DATA_S *pAd = (HI_WLAN_DATA_S *)args;

    ret = sta_ui_init(wlan_ui_cmd_cb, wlan_ui_eof_cb, pAd);
    if (ret) {
        printf("SAMPLE_STA: UI init failed\n");
        return NULL;
    }

    /* UI loop */
    sta_ui_run();

    sta_ui_deinit();

    return NULL;
}

static HI_S32 wlan_start_udhcpc(const HI_CHAR *ifname)
{
    HI_S32 ret;
    HI_S32 pid = 0;
//    HI_S32 status = 0;
    char param[128];
    char *spawn_env[] = {NULL};
    char *spawn_args[] = {"udhcpc", NULL, NULL, NULL};

    sprintf(param, "%s", ifname);
    spawn_args[1] = "-i";
    spawn_args[2] = param;

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return HI_FAILURE;
    }
/*
    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        return -1;
    }
*/
    return HI_SUCCESS;
}

HI_VOID wlan_stop_udhcpc(HI_VOID)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"killall", NULL, NULL};
    
    spawn_args[1] = "udhcpc";

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        perror("waitpid");
    }
}

HI_VOID wlan_kill_process(HI_CHAR *process)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"killall", NULL, NULL};
    
    spawn_args[1] = process;

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        perror("waitpid");
    }
}

/* handle SIGPWR from sample_pmoc. Before sleep, sample_pmoc sends a SIGPWR,
 * when received the SIGPWR, turn off WiFi. After wakeup, sample_pmoc sends
 * a SIGPWR again, when received the SIGPWR, turn on WiFi.
 * Notes: only used for sample, please use socktes for real life.
 */
static HI_VOID wlan_power(HI_S32 sig)
{
    printf("SAMPLE_STA: Received SIGPWR\n");
    if(pData->persist_on) {
        if (HI_WLAN_STA_Open(pData->ifname) == HI_SUCCESS) {
            if (HI_WLAN_STA_Start(pData->ifname, wlan_event_receiver) == HI_SUCCESS) {
                pData->state = STA_STATE_ENABLED;
            } else {
                printf("SAMPLE_STA: HI_WLAN_STA_Start fail\n");
                HI_WLAN_STA_Close(pData->ifname);
            }
        } else
            printf("SAMPLE_STA: HI_WLAN_STA_Open fail\n");

        // TODO: scan and connect to saved AP

        wlan_print_status(pData);

        pData->persist_on = HI_FALSE;
    } else {
        if (pData->state != STA_STATE_DISABLED) {
            wlan_stop_udhcpc();
            HI_WLAN_STA_Stop(pData->ifname);
            HI_WLAN_STA_Close(pData->ifname);

            // TODO: if connected, save the AP's information

            memset(&pData->connection, 0, sizeof(HI_WLAN_STA_CONN_STATUS_S));
            pData->state = STA_STATE_DISABLED;
            pData->persist_on = HI_TRUE;
        }
    }
}

HI_VOID wlan_create_pid_file(const HI_CHAR *pid_file)
{
    HI_S32 fd;
    HI_CHAR fbuf[100];

    /* ensure /dev/wifi exist */
    if (access(DEV_WIFI_DIR, F_OK) < 0) {
        if(mkdir(DEV_WIFI_DIR, 0666) != 0) {
            printf("SAMPLE_STA: Create '%s' fail\n", DEV_WIFI_DIR);
            return;
        }
        chmod(DEV_WIFI_DIR, 0666);
    }

    /* create pid file, if exist, delete it firstly */
    if (access(pid_file, F_OK) == 0)
        unlink(pid_file);

    fd = open(pid_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if(fd < 0) {
        printf("SAMPLE_STA: Cann't create file '%s'\n", PID_FILE);
        return;
    }

    /* write pid into pid file */
    memset(fbuf, 0, sizeof(fbuf));
    sprintf(fbuf, "%d", getpid());

    if (write(fd, fbuf, strlen(fbuf)) < 0) {
        printf("SAMPLE_STA: Cann't write pid to '%s'\n", PID_FILE);
    }
    close(fd);

    if (chmod(pid_file, 0666) < 0) {
        printf("SAMPLE_STA: Failed to change '%s' to 0666\n", PID_FILE);
        unlink(pid_file);
    }
}

HI_S32 main(HI_S32 argc, HI_CHAR *argv[])
{
    HI_S32 ret;
    HI_BOOL terminate = HI_FALSE;

    printf("%s\n", sample_version);
    usage();

    wlan_create_pid_file(PID_FILE);

    signal(SIGINT, wlan_terminate);
    signal(SIGTERM, wlan_terminate);
    signal(SIGPWR, wlan_power);

    pData = (HI_WLAN_DATA_S *)malloc(sizeof(HI_WLAN_DATA_S));
    if (pData == NULL)
        return -1;
    memset(pData, 0, sizeof(HI_WLAN_DATA_S));

    pthread_mutex_init(&pData->mut, NULL);
    pthread_cond_init(&pData->cond, NULL);
    pData->cmd_queue.front = pData->cmd_queue.rear = NULL;

    pData->state = STA_STATE_DISABLED;
	
    /* create UI thread, receives commands that user inputed */
    ret = pthread_create(&pData->uiThread, NULL, wlan_ui_thread_proc, pData);
    if (HI_SUCCESS != ret)
    {
        printf("SAMPLE_STA: create UI thread failed\n");
        goto out;
    }

    ret = HI_WLAN_STA_Init();
    if (ret != HI_SUCCESS) {
        printf("SAMPLE_STA: Init failed\n");
        return -1;
    }

    wlan_print_status(pData);

    /* main loop */
    while(!terminate) {
        HI_WLAN_MESSAGE_S message;
        HI_S32 i;

        /* dequeue cmd_queue, if no command/event, wait */
        pthread_mutex_lock(&pData->mut);
        while (wlan_dequeue(&pData->cmd_queue, &message) != HI_SUCCESS) {
            pthread_cond_wait(&pData->cond, &pData->mut);
        }
        pthread_mutex_unlock(&pData->mut);

        /* a command or event arrives, process it */
        switch (message.what) {
            case STA_CMD_ENABLE:
                ret = HI_WLAN_STA_Open(pData->ifname);
                if (ret == HI_SUCCESS) {
                    ret = HI_WLAN_STA_Start(pData->ifname, wlan_event_receiver);
                    if (ret == HI_SUCCESS) {
                        pData->state = STA_STATE_ENABLED;
                        wlan_print_status(pData);
                    } else {
                        printf("SAMPLE_STA: HI_WLAN_STA_Start fail (%d)\n", ret);
                        HI_WLAN_STA_Close(pData->ifname);
                    }
                } else
                    printf("SAMPLE_STA: HI_WLAN_STA_Open fail (%d)\n", ret);
            break;

            case STA_CMD_DISABLE:
                ret = HI_WLAN_STA_Stop(pData->ifname);
                if (ret == HI_SUCCESS) {
                    ret = HI_WLAN_STA_Close(pData->ifname);
                    if (ret == HI_SUCCESS) {
                        memset(pData, 0, sizeof(HI_WLAN_DATA_S));
                        pData->state = STA_STATE_DISABLED;
                        wlan_stop_udhcpc();
                    }
                }
                wlan_print_status(pData);
            break;

            case STA_CMD_SCAN:
                ret = HI_WLAN_STA_StartScan(pData->ifname);
                if (ret == HI_SUCCESS)
                    pData->state = STA_STATE_SCANNING;
                else
                    printf("SAMPLE_STA: HI_WLAN_STA_StartScan fail (%d)\n", ret);
            break;

            case STA_CMD_CONNECT:
                pData->config = message.obj.config;

                ret = HI_WLAN_STA_Connect(pData->ifname, &pData->config);
                if (ret == HI_SUCCESS)
                    pData->state = STA_STATE_CONNECTING;
                else
                    printf("SAMPLE_STA: HI_WLAN_STA_Connect fail (%d)\n", ret);
            break;

            case STA_CMD_DISCONNECT:
                ret = HI_WLAN_STA_Disconnect(pData->ifname);
                if (ret == HI_SUCCESS)
                    pData->connection.state = HI_WLAN_STA_CONN_STATUS_DISCONNECTED;
                else
                    printf("SAMPLE_STA: HI_WLAN_STA_Disconnect fail (%d)\n", ret);
            break;

            case STA_CMD_GET_STATUS:
                ret = HI_WLAN_STA_GetConnectionStatus(pData->ifname, &pData->connection);
                if (ret == HI_SUCCESS)
                    wlan_print_status(pData);
                else
                    printf("SAMPLE_STA: HI_WLAN_STA_GetConnectionStatus fail (%d)\n", ret);
            break;

            case STA_CMD_MACADDRESS:
                ret = HI_WLAN_STA_GetMacAddress(pData->ifname, pData->mac);
                if (ret == HI_SUCCESS) {
                    printf("MAC: %s\n", pData->mac);
                    printf("> ");
                    fflush(stdout);
                } else
                    printf("SAMPLE_STA: HI_WLAN_STA_GetMacAddress fail (%d)\n", ret);
            break;

            case STA_CMD_QUIT:
                terminate = HI_TRUE;
            break;

            case STA_EVT_DISCONNECTED:
                pData->connection.state = HI_WLAN_STA_CONN_STATUS_DISCONNECTED;
                wlan_stop_udhcpc();
                wlan_print_status(pData);
            break;

            case STA_EVT_SCANRESULTS_AVAILABLE:
            {
                HI_U32 num = sizeof(pData->aplist)/sizeof(HI_WLAN_STA_ACCESSPOINT_S);
                memset(pData->aplist, 0, sizeof(pData->aplist));
                ret = HI_WLAN_STA_GetScanResults(pData->ifname, pData->aplist, &num);
                if (ret == HI_SUCCESS) {
                    pData->ap_num = num;
                    wlan_print_status(pData);
                } else
                    printf("SAMPLE_STA: HI_WLAN_STA_GetScanResults fail (%d)\n", ret);
            }
            break;

            case STA_EVT_CONNECTING:
                memset(&pData->connection, 0, sizeof(pData->connection));
                pData->connection.state = HI_WLAN_STA_CONN_STATUS_CONNECTING;
                strcpy(pData->connection.ap.bssid, message.obj.bssid);
                for (i = 0; i < pData->ap_num; i++) {
                    if (strcmp(pData->connection.ap.bssid, pData->aplist[i].bssid) == 0)
                        strcpy(pData->connection.ap.ssid, pData->aplist[i].ssid);
                }
                wlan_print_status(pData);
            break;

            case STA_EVT_CONNECTED:
                memset(&pData->connection, 0, sizeof(pData->connection));
                pData->connection.state = HI_WLAN_STA_CONN_STATUS_CONNECTED;
                strcpy(pData->connection.ap.bssid, message.obj.bssid);
                for (i = 0; i < pData->ap_num; i++) {
                    if (strcmp(pData->connection.ap.bssid, pData->aplist[i].bssid) == 0)
                        strcpy(pData->connection.ap.ssid, pData->aplist[i].ssid);
                }
                wlan_print_status(pData);
                wlan_start_udhcpc(pData->ifname);
            break;

            case STA_EVT_SUPP_STOPPED:
            case STA_EVT_DRIVER_STOPPED:
                printf("SAMPLE_STA: Something wrong, disabling WiFi...\n");
                ret = HI_WLAN_STA_Stop(pData->ifname);
                if (ret == HI_SUCCESS) {
                    ret = HI_WLAN_STA_Close(pData->ifname);
                    if (ret == HI_SUCCESS) {
                        memset(pData, 0, sizeof(HI_WLAN_DATA_S));
                        pData->state = STA_STATE_DISABLED;
                    }
                }
                wlan_print_status(pData);
            break;

            default:
            break;
        }
    }

out:
    wlan_cleanup();
    return ret;
}
