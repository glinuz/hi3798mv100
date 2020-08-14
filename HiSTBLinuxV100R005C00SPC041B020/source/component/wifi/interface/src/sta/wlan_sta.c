#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <spawn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <poll.h>
#include <pthread.h>

#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include "wlan_sta.h"
#include "wlan_util.h"

static HI_STA_DATA_S *sta = NULL;

HI_S32 sta_init(HI_VOID)
{
    sta = malloc(sizeof(HI_STA_DATA_S));
    if (sta == NULL)
        return HI_FAILURE;

    memset(sta, 0, sizeof(HI_STA_DATA_S));
    sta->device_id = -1;

    return HI_SUCCESS;
}

HI_VOID sta_deinit(HI_VOID)
{
    if (sta) {
        free(sta);
        sta = NULL;
    }
}

HI_S32 sta_load_driver(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;

    sta->device_id = wlan_util_get_wifi_device();
    if (sta->device_id < 0) {
        DBGPRINT(("WiFi: Cann't find supported device\n"));
        return HI_WLAN_DEVICE_NOT_FOUND;
    }
    DBGPRINT(("WiFi: Find device %d\n", sta->device_id));

    if (WIFI_RALINK_MT7601U == sta->device_id) {
         if (wlan_util_insmod_module("/kmod/mtprealloc7601Usta.ko",
                                           "mtprealloc7601Usta ", NULL)) {
            return HI_WLAN_LOAD_DRIVER_FAIL;
        }
    }
    if (WIFI_MEDIATEK_MT7632TU == sta->device_id || WIFI_MEDIATEK_MT7662TU == sta->device_id) {
         if (wlan_util_insmod_module("/kmod/mtprealloc76x2.ko",
                                           "mtprealloc76x2 ", NULL)) {
            return HI_WLAN_LOAD_DRIVER_FAIL;
        }
    }

    /* insmod driver */
    switch(sta->device_id) {
        case WIFI_RALINK_RT3070:
        case WIFI_RALINK_RT5370:
        case WIFI_RALINK_RT5372:
        case WIFI_RALINK_RT5572:
            if (wlan_util_insmod_module("/kmod/rtutil5572sta.ko", "rtutil5572sta ", NULL)
              || wlan_util_insmod_module("/kmod/rt5572sta.ko", "rt5572sta ", NULL)
              || wlan_util_insmod_module("/kmod/rtnet5572sta.ko", "rtnet5572sta", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_RALINK_MT7601U:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/mt7601Usta.ko", "mt7601Usta ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_MEDIATEK_MT7632TU:
        case WIFI_MEDIATEK_MT7662TU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/mt7662u_sta.ko", "mt7662u_sta ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_MEDIATEK_MT7662TE:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/mt7662e_sta.ko", "mt7662e_sta ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_ATHEROS_AR9271:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/mac80211.ko", "mac80211 ", NULL)
              || wlan_util_insmod_module("/kmod/ath.ko", "ath ", NULL)
              || wlan_util_insmod_module("/kmod/ath9k_hw.ko", "ath9k_hw ", NULL)
              || wlan_util_insmod_module("/kmod/ath9k_common.ko", "ath9k_common ", NULL)
              || wlan_util_insmod_module("/kmod/ath9k_htc.ko", "ath9k_htc ", NULL)
              || wlan_util_insmod_module("/kmod/ath9k.ko", "ath9k ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_ATHEROS_AR9374:
        case WIFI_ATHEROS_QCA1021G:
        case WIFI_ATHEROS_QCA1021X:
            if (wlan_util_insmod_module("/kmod/cfg80211_ath6k.ko", "cfg80211_ath6k ", NULL)
              || wlan_util_insmod_module("/kmod/ath6kl_usb.ko", "ath6kl_usb ",
                                           "reg_domain=0x809c")) {
                sleep(2);
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            }
            break;
        case WIFI_REALTEK_RTL8188SU:
        case WIFI_REALTEK_RTL8192SU:
            if (wlan_util_insmod_module("/kmod/rtl8712u.ko", "rtl8712u ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8188CUS:
        case WIFI_REALTEK_RTL8192CU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8192cu.ko", "rtl8192cu ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8188EUS:
        case WIFI_REALTEK_RTL8188ETV:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8188eu.ko", "rtl8188eu ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8192DU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8192du.ko", "rtl8192du ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8192EU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8192eu.ko", "rtl8192eu ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8812AU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8812au.ko", "rtl8812au ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_BROADCOM_BCM43236:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/bcmdhd.ko", "bcmdhd ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            else
                sleep(2);
            break;
        case WIFI_REALTEK_RTL8723BU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8723bu.ko", "rtl8723bu ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8821AU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8821au.ko", "rtl8821au ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_REALTEK_RTL8822BU:
            if (wlan_util_insmod_module("/kmod/cfg80211.ko", "cfg80211 ", NULL)
              || wlan_util_insmod_module("/kmod/rtl8822bu.ko", "rtl8822bu ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        default:
            DBGPRINT(("WiFi: device %d is not supported, "
                        "cann't load driver\n", sta->device_id));
            ret = HI_WLAN_DEVICE_NOT_FOUND;
            break;
    }

    if (ret == HI_WLAN_LOAD_DRIVER_FAIL)
        DBGPRINT(("WiFi: Load driver fail\n"));

    return ret;
}

HI_S32 sta_unload_driver(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    DBGPRINT(("WiFi: Unloading driver\n"));
    /* rmmod driver */
    switch(sta->device_id) {
        case WIFI_RALINK_RT3070:
        case WIFI_RALINK_RT5370:
        case WIFI_RALINK_RT5372:
        case WIFI_RALINK_RT5572:
            if ((wlan_util_rmmod_module("rtnet5572sta") == 0)
              && (wlan_util_rmmod_module("rt5572sta") == 0)
              && (wlan_util_rmmod_module("rtutil5572sta") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_RALINK_MT7601U:
            if ((wlan_util_rmmod_module("mt7601Usta") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_MEDIATEK_MT7632TU:
        case WIFI_MEDIATEK_MT7662TU:
            if ((wlan_util_rmmod_module("mt7662u_sta") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_MEDIATEK_MT7662TE:
            if ((wlan_util_rmmod_module("mt7662e_sta") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_ATHEROS_AR9271:
            if ((wlan_util_rmmod_module("ath9k") == 0)
              && (wlan_util_rmmod_module("ath9k_htc") == 0)
              && (wlan_util_rmmod_module("ath9k_common") == 0)
              && (wlan_util_rmmod_module("ath9k_hw") == 0)
              && (wlan_util_rmmod_module("ath") == 0)
              && (wlan_util_rmmod_module("mac80211") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_ATHEROS_AR9374:
        case WIFI_ATHEROS_QCA1021G:
        case WIFI_ATHEROS_QCA1021X:
            if ((wlan_util_rmmod_module("ath6kl_usb") == 0)
              && (wlan_util_rmmod_module("cfg80211_ath6k") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8188SU:
        case WIFI_REALTEK_RTL8192SU:
            if (wlan_util_rmmod_module("rtl8712u") == 0)
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8188CUS:
        case WIFI_REALTEK_RTL8192CU:
            if ((wlan_util_rmmod_module("rtl8192cu") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8188EUS:
        case WIFI_REALTEK_RTL8188ETV:
            if ((wlan_util_rmmod_module("rtl8188eu") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8192DU:
            if ((wlan_util_rmmod_module("rtl8192du") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8192EU:
            if ((wlan_util_rmmod_module("rtl8192eu") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8812AU:
            if ((wlan_util_rmmod_module("rtl8812au") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_BROADCOM_BCM43236:
            if ((wlan_util_rmmod_module("bcmdhd") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8723BU:
            if ((wlan_util_rmmod_module("rtl8723bu") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8821AU:
            if ((wlan_util_rmmod_module("rtl8821au") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_REALTEK_RTL8822BU:
            if ((wlan_util_rmmod_module("rtl8822bu") == 0)
              && (wlan_util_rmmod_module("cfg80211") == 0))
                ret = HI_SUCCESS;
            break;
        default:
            DBGPRINT(("WiFi: device %d is not supported, "
                        "cann't unload driver\n", sta->device_id));
            break;
    }

    if (ret == HI_SUCCESS)
        sta->device_id = -1;

    return ret;
}

HI_S32 sta_run_supplicant(HI_CHAR *ifname, const HI_CHAR *driver,
                            const HI_CHAR *config_file)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR param[3][128];
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"wpa_supplicant", NULL, NULL, NULL,
    	                      NULL, NULL};
    
    snprintf(param[0], sizeof(param[0]), "-i%s", ifname);
    if (sta->device_id == WIFI_RALINK_RT3070 || sta->device_id == WIFI_RALINK_RT5370
      || sta->device_id == WIFI_RALINK_RT5372 || sta->device_id == WIFI_RALINK_RT5572
      || sta->device_id == WIFI_ATHEROS_AR9271)
        snprintf(param[1], sizeof(param[1]), "-D%s", "wext");
    else
        snprintf(param[1], sizeof(param[1]), "-D%s", driver);
    snprintf(param[2], sizeof(param[2]), "-c%s", config_file);
    spawn_args[1] = param[0];
    spawn_args[2] = param[1];
    spawn_args[3] = param[2];
    spawn_args[4] = "-B";

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return HI_FAILURE;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 sta_kill_supplicant(HI_VOID)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"killall", NULL, NULL};
    
    spawn_args[1] = "wpa_supplicant";

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return HI_FAILURE;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        perror("waitpid");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

static
HI_S32 sta_send_command(HI_WPA_SOCKET_S *wpa_s, const HI_CHAR *cmd,
                          HI_CHAR *reply, HI_S32 *reply_len)
{
    HI_S32 ret, i = 0;

    if (wpa_s == NULL || cmd == NULL)
        return HI_FAILURE;

    /* Send command to wpa_supplicant, if failed, try 50 times */
send_again:
    ret = send(wpa_s->s, cmd, strlen(cmd), 0);
    if (ret < 0) {
        if (errno == EAGAIN || errno == EBUSY || errno == EWOULDBLOCK) {
            i++;
            if (i >= 50)
                return HI_FAILURE;

            usleep(100000);
            goto send_again;
        } else
            return HI_FAILURE;
    }

    if (reply == NULL || reply_len == NULL)
        return HI_SUCCESS;

    /* Receive command's reply */
    for (i = 0; i < 100; i++) {
        fd_set rfd;
        struct timeval tv;

        FD_ZERO(&rfd);
        FD_SET(wpa_s->s, &rfd);
        tv.tv_sec = 10;
        tv.tv_usec = 0;

        ret = select(wpa_s->s + 1, &rfd, NULL, NULL, &tv);
        if (ret < 0)
            return HI_FAILURE;

        if (FD_ISSET(wpa_s->s, &rfd)) {
            ret = recv(wpa_s->s, reply, *reply_len, 0);
            if (ret < 0)
                return HI_FAILURE;

            if (ret > 0 && reply[0] == '<')
                continue;

            *reply_len = ret;
            break;
        } else {
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

HI_S32 sta_wpa_request(HI_CHAR *cmd, HI_CHAR *cbuf, HI_S32 *size)
{
    HI_CHAR reply[STRING_REPLY_SIZE] = {0};
    HI_S32 reply_len;
    HI_S32 ret;

    if (cmd == NULL)
        return HI_FAILURE;

    reply_len = sizeof(reply);
    ret = sta_send_command(&sta->ctrl_s, cmd, reply, &reply_len);
    if (ret == HI_FAILURE || strncmp(reply, "FAIL", 4) == 0) {
        DBGPRINT(("WiFi: '%s' command fail!\n", cmd));
        return HI_FAILURE;
    }

    if (cbuf != NULL && size != NULL) {
        if (*size < reply_len)
            reply_len = *size;

        strncpy(cbuf, reply, reply_len);
        *size = reply_len;
    }

    return HI_SUCCESS;
}

static
HI_VOID sta_scan_result_split(HI_CHAR *src, HI_CHAR sym, HI_CHAR *ss[])
{
    *ss++ = src;
    while (*src) {
        if (*src == sym) {
            *src++ = '\0';
            *ss++ = src;
        } else
            src++;
    }
}

static
HI_WLAN_SECURITY_E sta_parse_security(HI_CHAR *capability)
{
    HI_WLAN_SECURITY_E sec;
    
    if (capability == NULL)
        return HI_WLAN_SECURITY_BUTT;
        
    if (strstr(capability, "WEP"))
        sec = HI_WLAN_SECURITY_WEP;
    else if (strstr(capability, "WPA-PSK") || strstr(capability, "WPA2-PSK"))
        sec = HI_WLAN_SECURITY_WPA_WPA2_PSK;
    else if (strstr(capability, "WPA-EAP") || strstr(capability, "WPA2-EAP"))
        sec = HI_WLAN_SECURITY_WPA_WPA2_EAP;
    else
        sec = HI_WLAN_SECURITY_OPEN;
        
    return sec;
}

HI_S32 sta_parse_scan_result(HI_CHAR *line, HI_WLAN_STA_ACCESSPOINT_S *ap)
{
#define MAX_RSSI    256  // 0 dBm
#define MIN_RSSI    156  // -100 dBm
    HI_S32 ret = 0;
    HI_CHAR *result[5];
    HI_S32 rssi;

    if (line == NULL || ap == NULL)
        return -1;

    sta_scan_result_split(line, '\t', result);

    if (result[0] != NULL)
        strncpy(ap->bssid, result[0], sizeof(ap->bssid));
    ap->channel = wlan_util_frequency_to_channel(atoi(result[1]));
    rssi = atoi(result[2]);
#ifdef WPA_SUPPLICANT_8
    rssi += 256;
#endif
    if (rssi < MIN_RSSI)
        rssi = MIN_RSSI;
    if (rssi > MAX_RSSI)
        rssi = MAX_RSSI;
    ap->level = (rssi - MIN_RSSI) * 100 / (MAX_RSSI - MIN_RSSI);
    ap->security = sta_parse_security(result[3]);

    if (result[4] == NULL || strlen(result[4]) > MAX_SSID_LEN)
        return -1;
    strncpy(ap->ssid, result[4], sizeof(ap->ssid));
    if (ap->ssid[0] == '\0')
        return -1;

    return ret;	
}

static
HI_S32 sta_read_wpa_event(HI_WPA_SOCKET_S *wpa_s, HI_CHAR *event, HI_S32 *size)
{
    HI_S32 ret = -1;
    struct pollfd rfds[1];
	
    if (wpa_s == NULL) {
        return -1;
    }
	
    if (event == NULL || size == NULL)
        return -1;

    memset(rfds, 0, 1 * sizeof(struct pollfd));
    rfds[0].fd = wpa_s->s;
    rfds[0].events |= POLLIN;
    
    ret = poll(rfds, 1, -1);
    if (ret < 0) {
        return ret;
    }

    if (rfds[0].revents & POLLIN) {
        HI_CHAR buf[256];
        size_t len = sizeof(buf) - 1;
        ret = recv(wpa_s->s, buf, len, 0);
        if (ret >= 0) {
            len = ret;
            buf[len] = '\0';
            if (len+1 > *size)
                memcpy(event, buf, *size);
            else {
                memcpy(event, buf, len+1);
                *size = len+1;
            }
        } else {
            return -1;
        }
    }
    return 0;
}

HI_VOID *sta_receive_event_thread(HI_VOID *args)
{
#define EVENT_BUF_SIZE    2048
    HI_S32 ret;
    HI_CHAR buf[EVENT_BUF_SIZE] = {0};
    HI_S32 size;
    HI_WLAN_STA_EVENT_E event;
    HI_CHAR bssid[BSSID_LEN+1];
    HI_CHAR *pos;

    HI_WLAN_STA_Event_CallBack event_cb = (HI_WLAN_STA_Event_CallBack)args;

    while (1) {
        memset(buf, 0, sizeof(buf));
        size = sizeof(buf);
        ret = sta_read_wpa_event(&sta->mon_s, buf, &size);
        if (ret)
            continue;

        DBGPRINT(("WiFi: WPA Event \"%s\"\n", buf));
        if (strstr(buf, "CTRL-EVENT-SCAN-RESULTS"))
            event = HI_WLAN_STA_EVENT_SCAN_RESULTS_AVAILABLE;
        else if (strstr(buf, "CTRL-EVENT-CONNECTED")) {
            /* CTRL-EVENT-CONNECTED - Connection to ec:88:8f:92:be:70 
             * completed (auth) [id=0 id_str=]
             */
            event = HI_WLAN_STA_EVENT_CONNECTED;
            memset(bssid, 0, sizeof(bssid));
            pos = strstr(buf, "Connection to ");
            if (pos)
                strncpy(bssid, pos + 14, BSSID_LEN);
        } else if (strstr(buf, "CTRL-EVENT-DISCONNECTED"))
            event = HI_WLAN_STA_EVENT_DISCONNECTED;
        else if (strstr(buf, "CTRL-EVENT-ASSOC-REJECT"))
            event = HI_WLAN_STA_EVENT_DISCONNECTED;
        else if (strstr(buf, "Trying to associate with ")) {
            pos = strstr(buf, "Trying to associate with ");
            event = HI_WLAN_STA_EVENT_CONNECTING;
            memset(bssid, 0, sizeof(bssid));
            if (*pos)
                strncpy(bssid, pos + 25, BSSID_LEN);
        } else if (strstr(buf, "CTRL-EVENT-TERMINATING")) {
            event = HI_WLAN_STA_EVENT_SUPP_STOPPED;
        } else if (strstr(buf, "CTRL-EVENT-DRIVER-STATE STOPPED"))
            event = HI_WLAN_STA_EVENT_DRIVER_STOPPED;
        else
            continue;
        
        if (event_cb)
            event_cb(event, bssid);
    }
}

HI_S32 sta_connect_supplicant(const HI_CHAR *ifname, const HI_CHAR *ctrl_iface_dir)
{
    HI_CHAR *ctrl_file = NULL;
    HI_S32 len, ret;
    static HI_S32 count = 0;
    HI_S32 flags;
    HI_CHAR reply[STRING_REPLY_SIZE] = {0};
    HI_S32 reply_len = sizeof(reply);

    if (ifname == NULL || ctrl_iface_dir == NULL)
        return HI_FAILURE;

    if (access(ctrl_iface_dir, F_OK) < 0) {
        return HI_FAILURE;
    }

    len = strlen(ctrl_iface_dir) + strlen(ifname) + 2;
    ctrl_file = malloc(len);
    if (ctrl_file == NULL)
        return HI_FAILURE;

    ret = snprintf(ctrl_file, len, "%s/%s", ctrl_iface_dir, ifname);
    if (ret < 0 || ret >= len)
        goto fail;

    /* Open socket to send command to wpa_supplicant */
    sta->ctrl_s.s = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sta->ctrl_s.s < 0)
        goto fail;

    sta->ctrl_s.local.sun_family = AF_UNIX;
    ret = snprintf(sta->ctrl_s.local.sun_path, sizeof(sta->ctrl_s.local.sun_path),
                     "%s/wpa_%d_%d", ctrl_iface_dir, (int) getpid(), count++);
    if (ret < 0 || ret >= sizeof(sta->ctrl_s.local.sun_path))
        goto fail;

    if (bind(sta->ctrl_s.s, (struct sockaddr *) &sta->ctrl_s.local,
               sizeof(sta->ctrl_s.local)) < 0)
        goto fail;

    sta->ctrl_s.remote.sun_family = AF_UNIX;
    strncpy(sta->ctrl_s.remote.sun_path, ctrl_file,
              sizeof(sta->ctrl_s.remote.sun_path));
    if (connect(sta->ctrl_s.s, (struct sockaddr *) &sta->ctrl_s.remote,
		    sizeof(sta->ctrl_s.remote)) < 0)
        goto fail;

    flags = fcntl(sta->ctrl_s.s, F_GETFL);
    if (flags >= 0) {
        flags |= O_NONBLOCK;
        if (fcntl(sta->ctrl_s.s, F_SETFL, flags) < 0) {
            DBGPRINT(("WiFi: fcntl fail\n"));
        }
    }

    /* Open monitor socket to receive wpa_supplicant's event */
    sta->mon_s.s = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sta->mon_s.s < 0)
        goto fail;

    sta->mon_s.local.sun_family = AF_UNIX;
    ret = snprintf(sta->mon_s.local.sun_path, sizeof(sta->mon_s.local.sun_path),
                     "%s/wpa_%d_%d", ctrl_iface_dir, (int) getpid(), count++);
    if (ret < 0 || ret >= sizeof(sta->mon_s.local.sun_path))
        goto fail;

    if (bind(sta->mon_s.s, (struct sockaddr *) &sta->mon_s.local,
                  sizeof(sta->mon_s.local)) < 0)
        goto fail;

    sta->mon_s.remote.sun_family = AF_UNIX;
    strncpy(sta->mon_s.remote.sun_path, ctrl_file,
              sizeof(sta->mon_s.remote.sun_path));
    if (connect(sta->mon_s.s, (struct sockaddr *) &sta->mon_s.remote,
                  sizeof(sta->mon_s.remote)) < 0)
        goto fail;

    flags = fcntl(sta->mon_s.s, F_GETFL);
    if (flags >= 0) {
        flags |= O_NONBLOCK;
        if (fcntl(sta->mon_s.s, F_SETFL, flags) < 0) {
            DBGPRINT(("WiFi: fcntl fail\n"));
        }
    }

    /* Attach mon_s socket to wpa_supplicant */
    ret = sta_send_command(&sta->mon_s, "ATTACH", reply, &reply_len);
    if (ret == HI_FAILURE || strncmp(reply, "OK", 2) != 0) {
        DBGPRINT(("WiFi: Attach to wpa_supplicant fail\n"));
        goto fail;
    }

    free(ctrl_file);

    return HI_SUCCESS;

fail:
    if (ctrl_file)
        free(ctrl_file);

    unlink(sta->ctrl_s.local.sun_path);
    if (sta->ctrl_s.s > 0)
        close(sta->ctrl_s.s);

    unlink(sta->mon_s.local.sun_path);
    if (sta->mon_s.s > 0)
        close(sta->mon_s.s);

    memset(&sta->ctrl_s, 0, sizeof(HI_WPA_SOCKET_S));
    memset(&sta->mon_s, 0, sizeof(HI_WPA_SOCKET_S));

    return HI_FAILURE;
}

HI_VOID sta_disconnect_supplicant(HI_VOID)
{
    sta_send_command(&sta->mon_s, "DETACH", NULL, NULL);

    unlink(sta->ctrl_s.local.sun_path);
    if (sta->ctrl_s.s > 0)
        close(sta->ctrl_s.s);

    unlink(sta->mon_s.local.sun_path);
    if (sta->mon_s.s > 0)
        close(sta->mon_s.s);

    memset(&sta->ctrl_s, 0, sizeof(HI_WPA_SOCKET_S));
    memset(&sta->mon_s, 0, sizeof(HI_WPA_SOCKET_S));
}

