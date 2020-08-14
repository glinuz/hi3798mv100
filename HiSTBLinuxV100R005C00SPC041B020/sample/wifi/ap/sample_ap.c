/*****************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : sample_ap.c
  Version       : Initial Draft
  Author        : Hisilicon sdk software group
  Created       :
  Last Modified :
  Description   : Demo of wifi SoftAP interfaces usage.
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "hi_wlan_ap.h"

#define VERSION         "1.0.1"
#define SIN_ADDR(x)	(((struct sockaddr_in *) (&(x)))->sin_addr.s_addr)
/* SET_SA_FAMILY - set the sa_family field of a struct sockaddr */
#define SET_SA_FAMILY(addr, family)			\
    memset ((char *) &(addr), '\0', sizeof(addr));	\
    addr.sa_family = (family);

const HI_PCHAR sample_version = "sample_ap v" VERSION "\n";

HI_VOID wlan_ap_stop_udhcpd(HI_VOID);

static HI_CHAR ifname[17];
static HI_WLAN_AP_CONFIG_S *pCfg = NULL;
static HI_BOOL ap_enabled = HI_FALSE;
static HI_BOOL persist_on = HI_FALSE;

static const HI_CHAR DEV_WIFI_DIR[] = "/dev/wifi";
static const HI_CHAR PID_FILE[] = "/dev/wifi/sample.pid";
static const HI_CHAR IPADDR[] = "192.168.49.1";
static const HI_CHAR DNSADDR[] = "192.168.49.1";
static const HI_CHAR NETMASK[] = "255.255.255.1";
static const HI_CHAR UDHCPD_CONFIG_FILE[] = "/etc/udhcpd.conf";

static HI_VOID usage(HI_VOID)
{
    printf("\nUsage:\n");
    printf("  enable      start wifi softap\n");
    printf("  disable     stop wifi softap\n");
    printf("  set         set softap\n");
    printf("                SSID: length <= 32\n");
    printf("                channel: channel number, for example: 1\n");
    printf("                security: OPEN/WEP/WPAPSK\n");
    printf("                password: length <= 63\n");
    printf("  mac         get local wifi MAC address\n");
    printf("  quit        quit sample_ap\n");
    printf("  q           the same as quit\n");
    printf("  help        show this message\n");
}

static HI_VOID wlan_cleanup(HI_VOID)
{
    if (ap_enabled) {
        wlan_ap_stop_udhcpd();
        HI_WLAN_AP_Stop(ifname);
        HI_WLAN_AP_Close(ifname);
    }

    if (pCfg)
        free(pCfg);

    HI_WLAN_AP_DeInit();

    unlink(PID_FILE);
}

static HI_VOID wlan_terminate(HI_S32 sig)
{
    wlan_cleanup();
    exit(0);
}

HI_S32 wlan_ap_set_ip(HI_CHAR *ifname, const HI_CHAR *ip)
{    
    HI_S32 ret = HI_FAILURE;
    HI_S32 s;
    struct ifreq ifr;

    if (ifname == NULL || *ifname == '\0' || ip == NULL)
        return HI_FAILURE;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s <= 0)
        return HI_FAILURE;

    memset(&ifr, 0, sizeof(struct ifreq));
    SET_SA_FAMILY(ifr.ifr_addr,    AF_INET);
    SET_SA_FAMILY(ifr.ifr_dstaddr, AF_INET);
    SET_SA_FAMILY(ifr.ifr_netmask, AF_INET);
    strcpy(ifr.ifr_name, ifname);
    SIN_ADDR(ifr.ifr_addr) = inet_addr(ip);
    if(ioctl(s, SIOCSIFADDR, &ifr) >= 0) {
        ret = HI_SUCCESS;
    }

    close(s);
    return ret;
}

static HI_S32 wlan_ap_start_udhcpd(const HI_CHAR *config_file)
{
    HI_S32 ret;
    HI_S32 pid = 0;
//    HI_S32 status = 0;
    char param[128];
    char *spawn_env[] = {NULL};
    char *spawn_args[] = {"udhcpd", NULL, NULL, NULL};

    sprintf(param, "%s", config_file);
    spawn_args[1] = "-fS";
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

HI_VOID wlan_ap_stop_udhcpd(HI_VOID)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"killall", NULL, NULL};
    
    spawn_args[1] = "udhcpd";

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
    HI_S32 ret;

    printf("SAMPLE_AP: Received SIGPWR\n");
    if(persist_on) {
        if (HI_WLAN_AP_Open(ifname) == HI_SUCCESS) {
            if (HI_WLAN_AP_Start(ifname, pCfg) == HI_SUCCESS) {
                ap_enabled = HI_TRUE;
                ret = wlan_ap_set_ip(ifname, IPADDR);
                if (ret)
                    printf("SAMPLE_AP: set local IP failed\n");

                ret = wlan_ap_start_udhcpd(UDHCPD_CONFIG_FILE);
                if (ret)
                    printf("SAMPLE_AP: start udhcpd failed\n");
            } else {
                printf("SAMPLE_AP: HI_WLAN_AP_Start fail\n");
                HI_WLAN_AP_Close(ifname);
            }
        } else
            printf("SAMPLE_AP: HI_WLAN_AP_Open fail\n");

        persist_on = HI_FALSE;
    } else {
        if (ap_enabled) {
            wlan_ap_stop_udhcpd();
            HI_WLAN_AP_Stop(ifname);
            HI_WLAN_AP_Close(ifname);

            ap_enabled = HI_FALSE;
            persist_on = HI_TRUE;
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
    HI_CHAR cmd[16];

    printf("%s\n", sample_version);
    usage();

    wlan_create_pid_file(PID_FILE);

    signal(SIGINT, wlan_terminate);
    signal(SIGTERM, wlan_terminate);
    signal(SIGPWR, wlan_power);

    pCfg = (HI_WLAN_AP_CONFIG_S *)malloc(sizeof(HI_WLAN_AP_CONFIG_S));
    if (pCfg == NULL)
        return -1;

    memset(pCfg, 0, sizeof(HI_WLAN_AP_CONFIG_S));
    strcpy(pCfg->ssid, "HisiAP");
    pCfg->channel = 6;
    pCfg->security = HI_WLAN_SECURITY_OPEN;
    pCfg->hidden_ssid = HI_FALSE;

    ret = HI_WLAN_AP_Init();
    if (ret) {
        printf("SAMPLE_AP: initialize AP failed (%d)\n", ret);
        goto out;
    }

    while (!terminate) {
        memset(cmd, 0, sizeof(cmd));
        printf("> ");
        fflush(stdout);
        scanf("%s", cmd);
        if (strcmp(cmd, "enable") == 0) {
            if (ap_enabled)
                continue;

            ret = HI_WLAN_AP_Open(ifname);
            if (ret != HI_SUCCESS) {
                printf("SAMPLE_AP: open AP failed (%d)\n", ret);
                continue;
            }

            ret = HI_WLAN_AP_Start(ifname, pCfg);
            if (ret) {
                printf("SAMPLE_AP: start AP failed (%d)\n", ret);
                HI_WLAN_AP_Close(ifname);
                continue;
            }

            ap_enabled = HI_TRUE;
            ret = wlan_ap_set_ip(ifname, IPADDR);
            if (ret)
                printf("SAMPLE_AP: set local IP failed\n");
		
            ret = wlan_ap_start_udhcpd(UDHCPD_CONFIG_FILE);
            if (ret)
                printf("SAMPLE_AP: start udhcpd failed\n");
        } else if (strcmp(cmd, "disable") == 0) {
            if (!ap_enabled)
                continue;

            ret = HI_WLAN_AP_Stop(ifname);
            if (ret)
                printf("SAMPLE_AP: stop AP failed (%d)\n", ret);

            ret = HI_WLAN_AP_Close(ifname);
            if (ret != HI_SUCCESS)
                printf("SAMPLE_AP: close AP failed (%d)\n", ret);

            ap_enabled = HI_FALSE;
            wlan_ap_stop_udhcpd();
        } else if (strncmp(cmd, "set", 3) == 0) {
             HI_CHAR input[128];
             /* set SSID */
             memset(input, 0, sizeof(input));
             printf("Please input SSID: ");
             scanf("%s", input);
             if (strlen(input) > MAX_SSID_LEN) {
                 printf("SAMPLE_AP: error(length > 32)\n");
                 continue;
             }
             strcpy(pCfg->ssid, input);

             /* set channel */
             memset(input, 0, sizeof(input));
             printf("Please input channel: ");
             scanf("%s", input);
             pCfg->channel = atoi(input);

             /* set security: OPEN/WEP/WPAPSK */
             memset(input, 0, sizeof(input));
             printf("Please input security(OPEN/WEP/WPA_WPA2_PSK): ");
             scanf("%s", input);
             if (strcmp(input, "OPEN") == 0)
                 pCfg->security = HI_WLAN_SECURITY_OPEN;
             else if (strcmp(input, "WEP") == 0) {
                 pCfg->security = HI_WLAN_SECURITY_WEP;
             } else if (strcmp(input, "WPA_WPA2_PSK") == 0) {
                 pCfg->security = HI_WLAN_SECURITY_WPA_WPA2_PSK;
             } else {
                 printf("SAMPLE_AP: security (%s) not supported\n", input);
                 continue;
             }

             /* set password */
             if (pCfg->security == HI_WLAN_SECURITY_WEP
                   || pCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK) {
                 memset(input, 0, sizeof(input));
                 printf("Please input password: ");
                 scanf("%s", input);
                 if (strlen(input) > MAX_PSSWD_LEN) {
                     printf("SAMPLE_AP: error(length > 64)\n");
                     continue;
                 }
                 strcpy(pCfg->psswd, input);
             }

             /* set hide SSID */
             memset(input, 0, sizeof(input));
             printf("Enable hidden SSID?(0-disable, 1-enable): ");
             scanf("%s", input);
             if (strcmp(input, "1") == 0)
                 pCfg->hidden_ssid = HI_TRUE;
             else
                 pCfg->hidden_ssid = HI_FALSE;

             /* restart AP */
             wlan_ap_stop_udhcpd();

             ret = HI_WLAN_AP_SetSoftap(ifname, pCfg);
             if (ret) {
                 printf("SAMPLE_AP: set AP failed (%d)\n", ret);
                 continue;
             }

             ret = wlan_ap_set_ip(ifname, IPADDR);
             if (ret)
                 printf("SAMPLE_AP: set local IP failed\n");
		
             ret = wlan_ap_start_udhcpd(UDHCPD_CONFIG_FILE);
             if (ret)
                 printf("SAMPLE_AP: start udhcpd failed\n");
        } else if (strcmp(cmd, "mac") == 0) {
            HI_CHAR mac[17];

            ret = HI_WLAN_AP_GetMacAddress(ifname, mac);
            if (ret) {
                  printf("SAMPLE_AP: get MAC failed (%d)\n", ret);
                 continue;
            }
            printf("MAC: %s\n", mac);
        } else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "q") == 0)
            terminate = HI_TRUE;
        else if (strcmp(cmd, "help") == 0)
            usage();
        else
            printf("SAMPLE_AP: Command not supported\n");
    }

out:
    wlan_cleanup();
    return ret;
}
