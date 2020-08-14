#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "wlan_sta.h"
#include "wlan_util.h"

static const HI_CHAR DRIVER[] = "nl80211";
static const HI_CHAR SUPP_CONFIG_FILE[] = "/etc/Wireless/wpa_supplicant.conf";
static const HI_CHAR DEV_WIFI_DIR[] = "/dev/wifi";
static const HI_CHAR CTRL_IFACE_DIR[] = "/dev/wifi/wpa_supplicant";

static pthread_t sta_eThread = 0;
static HI_WLAN_STA_Event_CallBack sta_event_cb = NULL;

HI_S32 HI_WLAN_STA_Init(HI_VOID)
{
    return sta_init();
}

HI_VOID HI_WLAN_STA_DeInit(HI_VOID)
{
    sta_deinit();
}

HI_S32 HI_WLAN_STA_Open(HI_CHAR *ifname)
{
    HI_S32 ret;
    HI_CHAR iface[IFNAMSIZ + 1];
    HI_S32 count;

    if (ifname == NULL)
        return HI_WLAN_INVALID_PARAMETER;
#if 0
    /* firstly, read /proc/net/wireless, if there is a wlan interace, return 
     * it */
    ret = wlan_util_get_interface(iface, PROC_NET_WIRELESS);
    if (ret == HI_SUCCESS) {
        strncpy(ifname, iface, sizeof(ifname));
        return HI_SUCCESS;
    }
#endif
    /* read usb ID, find supported WiFi device, then load it's driver */
    ret = sta_load_driver();
    if (ret != HI_SUCCESS)
        return ret;
    DBGPRINT(("WiFi: Driver loaded successfully\n"));

    /* when driver startup, a new wlan network interface will be
     * created, wait 5s for the interface created successfully */
    for (count = 0; count < 50; count++) {
        ret = wlan_util_get_interface(iface, PROC_NET_WIRELESS);
        if (!ret) {
            DBGPRINT(("WiFi: Get interface '%s'\n", iface));
            strncpy(ifname, iface, sizeof(iface));
            return HI_SUCCESS;
        }
        ret = wlan_util_get_interface(iface, PROC_NET_DEV);
        if (!ret) {
            DBGPRINT(("WiFi: Get interface '%s'\n", iface));
            strncpy(ifname, iface, sizeof(iface));
            return HI_SUCCESS;
        }
        usleep(100000);
    }
    DBGPRINT(("WiFi: Failed to get interface, driver initialized fail!\n"));
    sta_unload_driver();

    return HI_FAILURE;
}

HI_S32 HI_WLAN_STA_Close(HI_CHAR *ifname)
{
    struct ifreq ifr;
    HI_S32 s;

    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    /* configure WiFi interface down, wait 200ms for it down*/
    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

    if((s = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
        if(ioctl(s, SIOCGIFFLAGS, &ifr) >= 0) {
            ifr.ifr_flags = (ifr.ifr_flags & (~IFF_UP));
            ioctl(s, SIOCSIFFLAGS, &ifr);
        }
        close(s);
    }
    usleep(200000);

    /* unload WiFi driver */
    return sta_unload_driver();
}

HI_S32 HI_WLAN_STA_Start(HI_CHAR *ifname, HI_WLAN_STA_Event_CallBack pfnEventCb)
{
    HI_S32 ret;
    HI_S32 i;

    if (ifname == NULL || *ifname == '\0' || pfnEventCb == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    /* ensure /dev/wifi exist */
    if (access(DEV_WIFI_DIR, F_OK) < 0) {
        if(mkdir(DEV_WIFI_DIR, 0666) != 0) {
            DBGPRINT(("WiFi: Create '%s' fail\n", DEV_WIFI_DIR));
            return HI_FAILURE;
        }
        chmod(DEV_WIFI_DIR, 0666);
    }

    /* start wpa_supplicant daemon */
    ret = sta_run_supplicant(ifname, DRIVER, SUPP_CONFIG_FILE);
    if (ret == HI_FAILURE) {
        DBGPRINT(("WiFi: start wpa_supplicant fail\n"));
        return HI_WLAN_START_SUPPLICANT_FAIL;
    }

    DBGPRINT(("WiFi: wpa_supplicant is running, connect to it\n"));

    /* connect to wpa_supplicant, try 50 times, if failed, kill wpa_supplicant */
    for (i = 0; ; i++) {
        if (i == 50) {
            DBGPRINT(("WiFi: Connect to wpa_supplicant timeout, "
                         "stop wpa_supplicant and return fail\n"));
            sta_kill_supplicant();
            return HI_WLAN_CONNECT_TO_SUPPLICANT_FAIL;
        }

        if (sta_connect_supplicant(ifname, CTRL_IFACE_DIR) == HI_SUCCESS) {
            DBGPRINT(("WiFi: Connected to wpa_supplicant\n"));
            break;
        }
        usleep(100000);
    }

    if (sta_event_cb == NULL)
        sta_event_cb = pfnEventCb;

    /* create thread to receive wpa_supplicant events */
    if (sta_eThread == 0) {
        ret = pthread_create(&sta_eThread, NULL, sta_receive_event_thread,
                               sta_event_cb);
        if (HI_SUCCESS != ret)
        {
            DBGPRINT(("WiFi: Cann't create monitor thread, "
                         "stop wpa_supplicant and return fail\n"));
            sta_disconnect_supplicant();
            sta_kill_supplicant();
            return HI_FAILURE;
        }
    }

    DBGPRINT(("WiFi: STA started\n"));
    return HI_SUCCESS;
}

HI_S32 HI_WLAN_STA_Stop(HI_CHAR *ifname)
{
    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    if (sta_eThread) {
        pthread_cancel(sta_eThread);
        sta_eThread = 0;
    }
    sta_event_cb = NULL;

    DBGPRINT(("WiFi: Stop wpa_supplicant\n"));
    /* stop wpa_supplicant */
    sta_disconnect_supplicant();
    if (sta_kill_supplicant() != HI_SUCCESS) {
        DBGPRINT(("WiFi: Kill wpa_supplicant fail\n"));
        return HI_FAILURE;
    }

    DBGPRINT(("WiFi: STA stopped\n"));
    return HI_SUCCESS;
}


HI_S32 HI_WLAN_STA_StartScan(HI_CHAR *ifname)
{
    HI_S32 ret;

    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    /* send "SCAN" to wpa_supplicant */
    ret = sta_wpa_request("SCAN", NULL, 0);
    if (ret != HI_SUCCESS)
        ret = HI_WLAN_SEND_COMMAND_FAIL;

    return ret;
}

HI_S32 HI_WLAN_STA_GetScanResults(HI_CHAR *ifname,
                     HI_WLAN_STA_ACCESSPOINT_S *pstApList, HI_U32 *pstApNum)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CHAR reply[STRING_REPLY_SIZE] = {0};
    HI_S32 size, len = sizeof(reply);
    HI_CHAR *pos = reply;
    HI_WLAN_STA_ACCESSPOINT_S ap;
    HI_CHAR line[256];
    HI_S32 i = 0;

    if (ifname == NULL || *ifname == '\0' || pstApList == NULL || pstApNum == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    /* Request stirng of scan results from wpa_supplicant */
    ret = sta_wpa_request("SCAN_RESULTS", reply, &len);
    if (ret != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;

    DBGPRINT(("%s\n", reply));

    /* The first line is "bssid / frequency / signal level / flags / ssid"
     * ignore this line */
    size = wlan_util_read_line(pos, line);
    pos += size;

    /* parse scan results, one line a AP */
    while (wlan_util_read_line(pos, line)) {
        size = wlan_util_read_line(pos, line);
        pos += size;
        if (!sta_parse_scan_result(line, &ap))
            pstApList[i++] = ap;
        if (i >= *pstApNum)
            break;
    }
    *pstApNum = i;

    return ret;
}

HI_S32 HI_WLAN_STA_Connect(HI_CHAR *ifname, HI_WLAN_STA_CONFIG_S *pstStaCfg)
{
    HI_S32 ret;
    HI_CHAR reply[256] = {0};
    HI_S32 len = sizeof(reply);
    HI_S32 netId;
    HI_CHAR cmd[256];
    HI_WLAN_STA_CONN_STATUS_S con;

    if (ifname == NULL || *ifname == '\0' || pstStaCfg == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    /* check connection, if connected, disconnect to AP firstly */
    ret = HI_WLAN_STA_GetConnectionStatus(ifname, &con);
    if (ret == HI_SUCCESS) {
        if (con.state == HI_WLAN_STA_CONN_STATUS_CONNECTED) {
            HI_WLAN_STA_Disconnect(ifname);
        }
    }

    /* add_network to wpa_supplicant, and remove all other networks */
    if (sta_wpa_request("REMOVE_NETWORK all", NULL, 0) != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;

    if (sta_wpa_request("ADD_NETWORK", reply, &len) != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;
    netId = atoi(reply);

    /* set network variables into wpa_supplicant, include ssid, bssid,
     * security, password etc.
     * when send "SELECT_NETWORK", connect start
     */
#ifdef WPA_SUPPLICANT_8
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid P\"%s\"", netId, pstStaCfg->ssid);
#else
    snprintf(cmd, sizeof(cmd), "SET_NETWORK %d ssid \"%s\"", netId, pstStaCfg->ssid);
#endif
    if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;

    if (pstStaCfg->bssid[0] != '\0') {
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d bssid %s", netId, pstStaCfg->bssid);
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
    }

    if (pstStaCfg->security == HI_WLAN_SECURITY_OPEN
    	  || pstStaCfg->security == HI_WLAN_SECURITY_WEP) {
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt NONE", netId);
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
    }

    if (pstStaCfg->security == HI_WLAN_SECURITY_WEP) {
        HI_S32 pwd_len = strlen(pstStaCfg->psswd);

        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d auth_alg OPEN SHARED", netId);
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;

        if(pwd_len == 5 || pwd_len == 13)
            snprintf(cmd, sizeof(cmd), "SET_NETWORK %d wep_key0 \"%s\"", netId, pstStaCfg->psswd);
        else if(pwd_len == 10 || pwd_len == 26)
            snprintf(cmd, sizeof(cmd), "SET_NETWORK %d wep_key0 %s", netId, pstStaCfg->psswd);
        else
            return HI_WLAN_INVALID_PARAMETER;

        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
    }

    if (pstStaCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK) {
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d proto %s", netId, "WPA RSN");
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d key_mgmt %s", netId, "WPA-PSK");
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d pairwise %s", netId, "CCMP TKIP");
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d group %s", netId, "CCMP TKIP");
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
        if (strlen(pstStaCfg->psswd) == 64)
            /* password is hexadecimal */
            snprintf(cmd, sizeof(cmd), "SET_NETWORK %d psk %s", netId, pstStaCfg->psswd);
        else
            snprintf(cmd, sizeof(cmd), "SET_NETWORK %d psk \"%s\"", netId, pstStaCfg->psswd);
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
    }

    if (pstStaCfg->hidden_ssid == HI_TRUE) {
        snprintf(cmd, sizeof(cmd), "SET_NETWORK %d scan_ssid 1", netId);
        if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
            return HI_WLAN_SEND_COMMAND_FAIL;
    }

    snprintf(cmd, sizeof(cmd), "SELECT_NETWORK %d", netId);
    if (sta_wpa_request(cmd, NULL, 0) != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;

    return HI_SUCCESS;
}

HI_S32 HI_WLAN_STA_Disconnect(HI_CHAR *ifname)
{
    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    if (sta_wpa_request("DISCONNECT", NULL, 0) != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;
        
    return HI_SUCCESS;
}

HI_S32 HI_WLAN_STA_GetConnectionStatus(HI_CHAR *ifname,
                              HI_WLAN_STA_CONN_STATUS_S *pstConnStatus)
{
    HI_S32 ret;
    HI_CHAR reply[STRING_REPLY_SIZE] = {0};
    HI_S32 len = sizeof(reply);
    HI_CHAR *pos;

    if (ifname == NULL || *ifname == '\0' || pstConnStatus == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    /* Get string of connection status from wpa_supplicant */
    ret = sta_wpa_request("STATUS", reply, &len);
    if (ret != HI_SUCCESS)
        return HI_WLAN_SEND_COMMAND_FAIL;

    /* Parse string of status, string likes bellow: 
     *   bssid=ec:23:3d:81:84:95
     *   ssid=SSID-HISI
     *   id=0
     *   mode=station
     *   pairwise_cipher=WEP-40
     *   group_cipher=WEP-40
     *   key_mgmt=NONE
     *   wpa_state=COMPLETED
     * while wpa_state=COMPLETED means connected
     */

    if (!(pos = strstr(reply, "wpa_state=")))
        return HI_FAILURE;

    memset(pstConnStatus, 0, sizeof(HI_WLAN_STA_CONN_STATUS_S));
    pos += 10;
    if (strncmp(pos, "COMPLETED", 9) == 0) {
        HI_S32 len = 0;
        HI_CHAR line[128];
        pos = reply;
        HI_S32 i;

        pstConnStatus->state = HI_WLAN_STA_CONN_STATUS_CONNECTED;

        while (wlan_util_read_line(pos, line)) {
            len = wlan_util_read_line(pos, line);
            if (strncmp(line, "ssid=", 5) == 0) {
                for (i = 0; i < len-5; i++)
                    pstConnStatus->ap.ssid[i] = line[i+5];
            } else if (strncmp(line, "bssid=", 6) == 0) {
                for (i = 0; i < len-6; i++)
                    pstConnStatus->ap.bssid[i] = line[i+6];
            } else if (strncmp(line, "key_mgmt=", 9) == 0) {
                if (strncmp(line+9, "NONE", 4) == 0)
                    pstConnStatus->ap.security = HI_WLAN_SECURITY_OPEN;
                else if (strncmp(line+9, "WEP", 3) == 0)
                    pstConnStatus->ap.security = HI_WLAN_SECURITY_WEP;
                else if (strncmp(line+9, "WPA-PSK", 7) == 0)
                    pstConnStatus->ap.security = HI_WLAN_SECURITY_WPA_WPA2_PSK;
            }
            pos += len;
        }
    } else
        pstConnStatus->state = HI_WLAN_STA_CONN_STATUS_DISCONNECTED;

    return ret;
}

HI_S32 HI_WLAN_STA_GetMacAddress(HI_CHAR *ifname, HI_CHAR *pstMac)
{    
    HI_S32 ret = HI_FAILURE;
    HI_S32 s;
    struct ifreq ifr;

    if (ifname == NULL || *ifname == '\0' || pstMac == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s <= 0)
        return HI_FAILURE;

    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

    if(ioctl(s, SIOCGIFHWADDR, &ifr) >= 0) {
        snprintf(pstMac, sizeof(ifr.ifr_hwaddr.sa_data), MACSTR, MAC2STR(ifr.ifr_hwaddr.sa_data));
        ret = HI_SUCCESS;
    }

    close(s);
    return ret;
}
