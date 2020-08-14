#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <spawn.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/wireless.h>

#include "hi_wlan_ap.h"
#include "wlan_util.h"

static const HI_CHAR DEV_WIFI_DIR[] = "/dev/wifi";
static const HI_CHAR HOSTAPD_CONFIG_DIR[] = "/dev/wifi/hostapd";
static const HI_CHAR HOSTAPD_CONFIG_FILE[] = "/dev/wifi/hostapd/hostapd.conf";
static const HI_CHAR ENTROPY_FILE[] = "/dev/wifi/hostapd/entropy.bin";

static HI_S32 device_id = -1;

/* For Broadcom's WiFi, after loading driver, before startup SoftAP, must
 * set SoftAP's firmware path to /sys/module/bcmdhd/paramters/firmware_path.
 * The name of firmware file should contain '_apsta'. */
static HI_S32 ap_update_firmware_path(HI_S32 device)
{
    HI_S32 fd;
    HI_CHAR *fbuf = NULL;

    fd = open("/sys/module/bcmdhd/parameters/firmware_path", O_TRUNC | O_WRONLY);
    if (fd < 0)
        return -1;

    if (device == WIFI_BROADCOM_BCM43236)
        asprintf(&fbuf, "/lib/firmware/brcm/bcm43236-firmware.bin_apsta.trx");

    if (write(fd, fbuf, strlen(fbuf)) < 0) {
        close(fd);
        return -1;
    }

    close(fd);
    free(fbuf);
    return 0;
}

static HI_S32 ap_load_driver(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;

    device_id = wlan_util_get_wifi_device();
    if (device_id < 0) {
        DBGPRINT(("WiFi: Cann't find supported device\n"));
        return HI_WLAN_DEVICE_NOT_FOUND;
    }
    DBGPRINT(("WiFi: Find device %d\n", device_id));

    if (WIFI_MEDIATEK_MT7632TU == device_id || WIFI_MEDIATEK_MT7662TU == device_id) {
         if (wlan_util_insmod_module("/kmod/mtprealloc76x2.ko",
                                           "mtprealloc76x2 ", NULL)) {
            return HI_WLAN_LOAD_DRIVER_FAIL;
        }
    }

    /* insmod driver */
    switch(device_id) {
        case WIFI_RALINK_RT3070:
        case WIFI_RALINK_RT5370:
        case WIFI_RALINK_RT5372:
            if (wlan_util_insmod_module("/kmod/rtutil5370ap.ko", "rtutil5370ap ", NULL)
              || wlan_util_insmod_module("/kmod/rt5370ap.ko", "rt5370ap ", NULL)
              || wlan_util_insmod_module("/kmod/rtnet5370ap.ko", "rtnet5370ap ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_RALINK_RT5572:
            if (wlan_util_insmod_module("/kmod/rtutil5572ap.ko", "rtutil5572ap ", NULL)
              || wlan_util_insmod_module("/kmod/rt5572ap.ko", "rt5572ap ", NULL)
              || wlan_util_insmod_module("/kmod/rtnet5572ap.ko", "rtnet5572ap ", NULL))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
            break;
        case WIFI_RALINK_MT7601U:
            if (wlan_util_insmod_module("/kmod/mt7601Uap.ko", "mt7601Uap ", NULL))
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
                                           "reg_domain=0x809c"))
                ret = HI_WLAN_LOAD_DRIVER_FAIL;
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
            else {
                sleep(2);
                ap_update_firmware_path(device_id);
            }
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
                        "cann't load driver\n", device_id));
            ret = HI_WLAN_DEVICE_NOT_FOUND;
            break;
    }

    if (ret == HI_WLAN_LOAD_DRIVER_FAIL)
        DBGPRINT(("WiFi: Load driver fail\n"));

    return ret;
}

static HI_S32 ap_unload_driver(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;

    DBGPRINT(("WiFi: Unloading driver\n"));
    /* rmmod driver */
    switch(device_id) {
        case WIFI_RALINK_RT3070:
        case WIFI_RALINK_RT5370:
        case WIFI_RALINK_RT5372:
            if ((wlan_util_rmmod_module("rtnet5370ap") == 0)
              && (wlan_util_rmmod_module("rt5370ap") == 0)
              && (wlan_util_rmmod_module("rtutil5370ap") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_RALINK_RT5572:
            if ((wlan_util_rmmod_module("rtnet5572ap") == 0)
              && (wlan_util_rmmod_module("rt5572ap") == 0)
              && (wlan_util_rmmod_module("rtutil5572ap") == 0))
                ret = HI_SUCCESS;
            break;
        case WIFI_RALINK_MT7601U:
            if (wlan_util_rmmod_module("mt7601Uap") == 0)
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
              && (wlan_util_rmmod_module("cfg80211_ath6k") == 0)) {
                sleep(2);
                ret = HI_SUCCESS;
            }
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
                        "cann't unload driver\n", device_id));
            break;
    }

    if (ret == HI_SUCCESS)
        device_id = -1;

    return ret;
}

/* configure softap by sending private ioctls to driver directly */
static HI_S32 ap_config_with_iwpriv_cmd(HI_S32 s, HI_CHAR *ifname,
                           HI_WLAN_AP_CONFIG_S *pstApCfg)
{
    char tBuf[4096];
    struct iwreq wrq;
    struct iw_priv_args *priv_ptr;
    HI_S32 i, j;
    HI_S32 cmd = 0, sub_cmd = 0;
    HI_CHAR mBuf[256];

    /* get all private commands that driver supported */
    strncpy(wrq.ifr_name, ifname, sizeof(wrq.ifr_name));
    wrq.u.data.pointer = tBuf;
    wrq.u.data.length = sizeof(tBuf) / sizeof(struct iw_priv_args);
    wrq.u.data.flags = 0;
    if (ioctl(s, SIOCGIWPRIV, &wrq) < 0) {
        return HI_FAILURE;
    }

    /* if driver don't support 'set' command, return failure */
    priv_ptr = (struct iw_priv_args *)wrq.u.data.pointer;
    for(i = 0; i < wrq.u.data.length; i++) {
        if (strcmp(priv_ptr[i].name, "set") == 0) {
            cmd = priv_ptr[i].cmd;
            break;
        }
    }
    if (i == wrq.u.data.length) {
        return HI_FAILURE;
    }

    /* get the 'set' command's ID */
    if (cmd < SIOCDEVPRIVATE) {
        for(j = 0; j < i; j++) {
            if ((priv_ptr[j].set_args == priv_ptr[i].set_args)
                && (priv_ptr[j].get_args == priv_ptr[i].get_args)
                && (priv_ptr[j].name[0] == '\0'))
                break;
        }
        if (j == i) {
            return HI_FAILURE;
        }
        sub_cmd = cmd;
        cmd = priv_ptr[j].cmd;
    }

    /* configure AP, order should be as follow
     *   0. WirelessMode
     *   1. Channel
     *   2. AuthMode
     *   3. EncrypType
     * for WPAPSK/WPA2PSK:
     *   4. SSID (must after AuthMode and before Password)
     *   5. Password
     * for WEP:
     *   4. Password
     *   5. SSID (must set lastly)
     */
    strncpy(wrq.ifr_name, ifname, sizeof(wrq.ifr_name));
    wrq.u.data.pointer = mBuf;
    wrq.u.data.flags = sub_cmd;

    /* configure WirelessMode */
    if (device_id == WIFI_RALINK_RT5572)
        snprintf(mBuf, sizeof(mBuf), "WirelessMode=5");
    else
        snprintf(mBuf, sizeof(mBuf), "WirelessMode=9");
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure Channel */
    snprintf(mBuf, sizeof(mBuf), "Channel=%d", pstApCfg->channel);
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure AuthMode */
    if (pstApCfg->security == HI_WLAN_SECURITY_OPEN)
        snprintf(mBuf, sizeof(mBuf), "AuthMode=OPEN");
    else if(pstApCfg->security == HI_WLAN_SECURITY_WEP)
        snprintf(mBuf, sizeof(mBuf), "AuthMode=WEPAUTO");
    else if (pstApCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK)
        snprintf(mBuf, sizeof(mBuf), "AuthMode=WPA2PSK");
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure EncrypType */
    if (pstApCfg->security == HI_WLAN_SECURITY_OPEN)
        snprintf(mBuf, sizeof(mBuf), "EncrypType=NONE");
    else if (pstApCfg->security == HI_WLAN_SECURITY_WEP)
        snprintf(mBuf, sizeof(mBuf), "EncrypType=WEP");
    else if (pstApCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK)
        snprintf(mBuf, sizeof(mBuf), "EncrypType=AES");
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure password of WEP */
    if (pstApCfg->security == HI_WLAN_SECURITY_WEP) {
        snprintf(mBuf, sizeof(mBuf), "DefaultKeyID=1");
        wrq.u.data.length = strlen(mBuf) + 1;
        if(ioctl(s, cmd, &wrq) < 0)
            return HI_FAILURE;

        snprintf(mBuf, sizeof(mBuf), "Key1=%s", pstApCfg->psswd);
        wrq.u.data.length = strlen(mBuf) + 1;
        if(ioctl(s, cmd, &wrq) < 0)
            return HI_FAILURE;
    }

    /* configure hide SSID */
    snprintf(mBuf, sizeof(mBuf), "HideSSID=%d", pstApCfg->hidden_ssid ? 1 : 0);
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure SSID */
    snprintf(mBuf, sizeof(mBuf), "SSID=%s", pstApCfg->ssid);
    wrq.u.data.length = strlen(mBuf) + 1;
    if(ioctl(s, cmd, &wrq) < 0)
        return HI_FAILURE;

    /* configure password of WPAPSK/WPA2PSK */
    if (pstApCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK) {
        snprintf(mBuf, sizeof(mBuf), "WPAPSK=%s", pstApCfg->psswd);
        wrq.u.data.length = strlen(mBuf) + 1;
        if(ioctl(s, cmd, &wrq) < 0)
            return HI_FAILURE;

        if (device_id == WIFI_RALINK_MT7601U) {
            /* for MT7601U, configure SSID again */
            snprintf(mBuf, sizeof(mBuf), "SSID=%s", pstApCfg->ssid);
            wrq.u.data.length = strlen(mBuf) + 1;
            if(ioctl(s, cmd, &wrq) < 0)
                return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 ap_start_hostapd(const HI_CHAR *config_file, const HI_CHAR *entropy_file)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    char param[2][4352];
    char *spawn_env[] = {NULL};
    char *spawn_args[] = {"hostapd", NULL, NULL, NULL, NULL};

    snprintf(param[0], sizeof(param[0]), "%s", config_file);
    snprintf(param[1], sizeof(param[1]), "-e%s", entropy_file);
    spawn_args[1] = "-B";
    spawn_args[2] = param[1];
    spawn_args[3] = param[0];

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return -1;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        return -1;
    }

    return 0;
}

static HI_S32 ap_kill_hostapd(HI_VOID)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"killall", NULL, NULL};

    spawn_args[1] = "hostapd";

    ret = posix_spawnp(&pid, spawn_args[0], NULL, NULL,
                   spawn_args, spawn_env);
    if (ret != 0) {
        return -1;
    }

    ret = waitpid(pid, &status, 0);
    if (ret == -1) {
        perror("waitpid");
        return -1;
    }

    return 0;
}

static HI_S32 ap_update_hostapd_config_file(HI_CHAR *ifname, 
            HI_WLAN_AP_CONFIG_S *pstApCfg, const HI_CHAR *config_file)
{
    HI_S32 ret = 0;
    HI_S32 fd;
    HI_CHAR *fbuf = NULL;
    HI_CHAR *wbuf = NULL;
    DIR *dir;
    HI_S32 broadcast_ssid = 0;
    HI_CHAR hw_mode;
    HI_CHAR ht40[32];

    /* ensure /dev/wifi exist */
    dir = opendir(DEV_WIFI_DIR);
    if (!dir) {
        if (mkdir(DEV_WIFI_DIR, 0666) < 0) {
            DBGPRINT(("WiFi: Create '%s' fail\n", DEV_WIFI_DIR));
            return -1;
        }
    }
    closedir(dir);

    /* ensure hostapd configure file directory exist */
    dir = opendir(HOSTAPD_CONFIG_DIR);
    if (!dir) {
        if (mkdir(HOSTAPD_CONFIG_DIR, 0666) < 0) {
            DBGPRINT(("WiFi: Create '%s' fail\n", HOSTAPD_CONFIG_DIR));
            return -1;
        }
    }
    closedir(dir);

    /* open configure file, if not exist, create it */
    fd = open(config_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if(fd < 0) {
        DBGPRINT(("WiFi: Cann't open configure file '%s'\n", config_file));
        return -1;
    }

    /* set broadcast ssid */
    if (pstApCfg->hidden_ssid == HI_TRUE) {
        DBGPRINT(("WiFi: Enable hidden SSID\n"));
        if (device_id == WIFI_ATHEROS_AR9374
          || device_id == WIFI_ATHEROS_QCA1021G
          || device_id == WIFI_ATHEROS_QCA1021X)
            broadcast_ssid = 2;     // clear SSID (ASCII 0)
        else
            broadcast_ssid = 1;     // send empty (length 0) SSID
    }

    /* set HT40 capability */
    memset(ht40, 0, sizeof(ht40));
    if (pstApCfg->channel >= 36) {
        HI_U32 i;
        HI_U32 ht40plus[] = {36, 44, 52, 60, 100, 108, 116, 124,
                             132, 149, 157};
        HI_U32 ht40minus[] = {40, 48, 56, 64, 104, 112, 120, 128,
                              136, 153, 161};

        hw_mode = 'a';

        for (i = 0; i < sizeof(ht40plus)/sizeof(ht40plus[0]); i++)
            if (pstApCfg->channel == ht40plus[i]) {
                strncpy(ht40,  "[SHORT-GI-40][HT40+]", sizeof(ht40));
                break;
            }

        for (i = 0; i < sizeof(ht40minus)/sizeof(ht40minus[0]); i++)
            if (pstApCfg->channel == ht40minus[i]) {
                strncpy(ht40, "[SHORT-GI-40][HT40-]", sizeof(ht40));
                break;
            }
    } else {
        hw_mode = 'g';
        if (pstApCfg->channel > 7)
            strncpy(ht40, "[SHORT-GI-40][HT40-]", sizeof(ht40));
        else
            strncpy(ht40, "[SHORT-GI-40][HT40+]", sizeof(ht40));
    }

    /* Broadcom's WiFi doesn't support 40M bandwidth in 2.4G, and cann't be set
     * HT40 capability in 5G
     */
    if (device_id == WIFI_BROADCOM_BCM43236)
        memset(ht40, 0, sizeof(ht40));

    /* set common paramters */
    if (pstApCfg->security == HI_WLAN_SECURITY_WEP)
        /* WEP is not supported in 802.11n */
        asprintf(&wbuf, "interface=%s\n"
                    "driver=%s\n"
                    "ctrl_interface=/dev/wifi/hostapd\n"
                    "ssid=%s\n"
                    "channel=%d\n"
                    "ignore_broadcast_ssid=%d\n"
                    "hw_mode=%c\n",
                    ifname, "nl80211", pstApCfg->ssid, pstApCfg->channel,
                    broadcast_ssid, hw_mode);
    else
        asprintf(&wbuf, "interface=%s\n"
                    "driver=%s\n"
                    "ctrl_interface=/dev/wifi/hostapd\n"
                    "ssid=%s\n"
                    "channel=%d\n"
                    "ignore_broadcast_ssid=%d\n"
                    "hw_mode=%c\n"
                    "ieee80211n=1\n"
                    "ht_capab=[SHORT-GI-20]%s\n",
                    ifname, "nl80211", pstApCfg->ssid, pstApCfg->channel,
                    broadcast_ssid, hw_mode,
                    ht40);

    /* set auth mode */
    if (pstApCfg->security == HI_WLAN_SECURITY_WEP) {
        HI_S32 pwd_len = strlen(pstApCfg->psswd);

        if (pwd_len == 5 || pwd_len == 13)
            asprintf(&fbuf, "%swep_default_key=0\n"
                            "wep_key0=\"%s\"\n", wbuf, pstApCfg->psswd);
        else
            asprintf(&fbuf, "%swep_default_key=0\n"
                            "wep_key0=%s\n", wbuf, pstApCfg->psswd);
    } else if (pstApCfg->security == HI_WLAN_SECURITY_WPA_WPA2_PSK)
        if (device_id == WIFI_BROADCOM_BCM43236)
            asprintf(&fbuf, "%swpa=2\n"
                            "auth_algs=3"
                            "wpa_key_mgmt=WPA-PSK\n"
                            "wpa_pairwise=TKIP CCMP\n"
                            "rsn_pairwise=TKIP CCMP\n"
                            "wpa_passphrase=%s\n", wbuf, pstApCfg->psswd);
        else
            asprintf(&fbuf, "%swpa=3\n"
                            "wpa_key_mgmt=WPA-PSK\n"
                            "wpa_pairwise=TKIP CCMP\n"
//                            "rsn_pairwise=CCMP\n"
                            "wpa_passphrase=%s\n", wbuf, pstApCfg->psswd);
    else
        asprintf(&fbuf, "%s", wbuf);

    if (write(fd, fbuf, strlen(fbuf)) < 0) {
        DBGPRINT(("WiFi: Cann't write configuration to '%s'\n", config_file));
        ret = -1;
    }
    close(fd);
    free(wbuf);
    free(fbuf);

    if (chmod(config_file, 0666) < 0) {
        DBGPRINT(("WiFi: Failed to change '%s' to 0666\n", config_file));
        unlink(config_file);
        ret = -1;
    }

    return ret;
}

int ensure_entropy_file_exists(const HI_CHAR *entropy_file)
{
    HI_S32 ret = 0;
    HI_S32 fd;
    char *fbuf = NULL;
    static unsigned char dummy_key[21] = { 0x02, 0x11, 0xbe, 0x33, 0x43, 0x35,
                                           0x68, 0x47, 0x84, 0x99, 0xa9, 0x2b,
                                           0x1c, 0xd3, 0xee, 0xff, 0xf1, 0xe2,
                                           0xf3, 0xf4, 0xf5 };

    if(0 == access(entropy_file, R_OK | W_OK | F_OK)) {
        DBGPRINT(("WiFi: entropy file already exists"));
        return 0;
    }

    fd = open(entropy_file, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if(fd < 0) {
        DBGPRINT(("WiFi: Cann't open entropy file '%s'\n", entropy_file));
        return -1;
    }
    asprintf(&fbuf, "%s", dummy_key);

    if (write(fd, fbuf, strlen(fbuf)) < 0) {
        DBGPRINT(("WiFi: Cann't write configuration to '%s'\n", entropy_file));
        ret = -1;
    }
    close(fd);
    free(fbuf);

    if (chmod(entropy_file, 0666) < 0) {
        DBGPRINT(("WiFi: Failed to change '%s' to 0666\n", entropy_file));
        unlink(entropy_file);
        ret = -1;
    }

    return ret;
}

HI_S32 HI_WLAN_AP_Init(HI_VOID)
{
    // TODO
    return HI_SUCCESS;
}

HI_VOID HI_WLAN_AP_DeInit(HI_VOID)
{
    // TODO
}

HI_S32 HI_WLAN_AP_Open(HI_CHAR *ifname)
{
    HI_S32 ret = HI_FAILURE;
    char iface[IFNAMSIZ + 1];
    HI_S32 count;

    if (ifname == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    ret = ap_load_driver();
    if (ret != HI_SUCCESS)
        return ret;
    DBGPRINT(("WiFi: Driver loaded successfully\n"));

    /* when driver startup, a new wireless network interface will be
     * created, wait 5s for the interface created successfully */
    for (count = 0; count < 50; count++) {
            ret = wlan_util_get_interface(iface, PROC_NET_DEV);
        if (ret != HI_SUCCESS)
            ret = wlan_util_get_interface(iface, PROC_NET_WIRELESS);

        if (ret == HI_SUCCESS) {
            DBGPRINT(("WiFi: Get interface '%s'\n", iface));
            strncpy(ifname, iface, sizeof(iface));
            return HI_SUCCESS;
        }
        usleep(100000);
    }
    DBGPRINT(("WiFi: Failed to get interface, driver initialized fail!\n"));
    ap_unload_driver();

    return HI_FAILURE;
}

HI_S32 HI_WLAN_AP_Close(HI_CHAR *ifname)
{
    HI_S32 ret;

    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    /* unload WiFi driver */
    ret = ap_unload_driver();

    return ret;
}

HI_S32 HI_WLAN_AP_Start(HI_CHAR *ifname, HI_WLAN_AP_CONFIG_S *pstApCfg)
{
    HI_S32 ret = HI_FAILURE;

    if (ifname == NULL || *ifname == '\0' || pstApCfg == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    /* check configures */
    if (strlen(pstApCfg->ssid) == 0)
        return HI_WLAN_INVALID_PARAMETER;

    /* channel, 11bg: 1 - 14, 11a: 36 - 165 */
    if (pstApCfg->channel < 1 || pstApCfg->channel > 165)
        return HI_WLAN_INVALID_PARAMETER;

    if (pstApCfg->security < HI_WLAN_SECURITY_OPEN
      || pstApCfg->security >= HI_WLAN_SECURITY_BUTT)
        return HI_WLAN_INVALID_PARAMETER;

    if (device_id == WIFI_RALINK_RT3070 || device_id == WIFI_RALINK_RT5370
      || device_id == WIFI_RALINK_RT5372 || device_id == WIFI_RALINK_RT5572
      || device_id == WIFI_RALINK_MT7601U) {
        struct ifreq ifr;
        HI_S32 s;

        /* For MTK WiFi, don't use hostapd, driver reads RT2870AP.dat and
         * configures WiFi to AP mode while intialize. After initialization
         * complete, configure WiFi interface up will startup AP. Then 
         * reconfigure AP by private commands.
         */
        memset(&ifr, 0, sizeof(struct ifreq));
        strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));

        if((s = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
            if(ioctl(s, SIOCGIFFLAGS, &ifr) >= 0) {
                ifr.ifr_flags = (ifr.ifr_flags | IFF_UP);
                if (ioctl(s, SIOCSIFFLAGS, &ifr) >= 0) {
                    ret = ap_config_with_iwpriv_cmd(s, ifname, pstApCfg);
                }
            }
            close(s);
        }

    } else {
        /* startup AP by hostapd. hostapd will configure WiFi to AP mode, then
         * start it */
        ret = ap_update_hostapd_config_file(ifname, pstApCfg, HOSTAPD_CONFIG_FILE);
        if (HI_FAILURE == ret) {
            DBGPRINT(("WiFi: update hostapd config file fail\n"));
            return ret;
        }

        ret = ensure_entropy_file_exists(ENTROPY_FILE);
        if (HI_FAILURE == ret) {
            DBGPRINT(("WiFi: creat entropy file fail\n"));
        }

        ret = ap_start_hostapd(HOSTAPD_CONFIG_FILE, ENTROPY_FILE);
        if (0 != ret) {
            DBGPRINT(("WiFi: start hostapd fail\n"));
            return HI_WLAN_START_HOSTAPD_FAIL;
        }
    }

    DBGPRINT(("WiFi: SoftAP started\n"));

    return HI_SUCCESS;
}

HI_S32 HI_WLAN_AP_Stop(HI_CHAR *ifname)
{
    struct ifreq ifr;
    HI_S32 s;

    if (ifname == NULL || *ifname == '\0')
        return HI_WLAN_INVALID_PARAMETER;

    /* configure WiFi interface down */
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

    if (device_id == WIFI_RALINK_RT3070 || device_id == WIFI_RALINK_RT5370
      || device_id == WIFI_RALINK_RT5372 || device_id == WIFI_RALINK_RT5572
      || device_id == WIFI_RALINK_MT7601U) {
        goto stopped;
    }

    if (ap_kill_hostapd()) {
        DBGPRINT(("WiFi: Kill hostapd fail\n"));
        return HI_FAILURE;
    }

stopped:
    DBGPRINT(("WiFi: SoftAP stopped\n"));
    return HI_SUCCESS;
}

HI_S32 HI_WLAN_AP_SetSoftap(HI_CHAR *ifname, HI_WLAN_AP_CONFIG_S *pstApCfg)
{
    HI_S32 ret;
    HI_CHAR iface[IFNAMSIZ + 1];

    if (ifname == NULL || *ifname == '\0' || pstApCfg == NULL)
        return HI_WLAN_INVALID_PARAMETER;

    DBGPRINT(("WiFi: Must stop SoftAP and close WiFi before setting it\n"));
    /* to set AP, we must restart it */
    ret = HI_WLAN_AP_Stop(ifname);
    if (ret != HI_SUCCESS) {
        DBGPRINT(("WiFi: Stop SoftAP fail\n"));
        return HI_FAILURE;
    }

    ret = HI_WLAN_AP_Close(ifname);
    if (ret != HI_SUCCESS) {
        DBGPRINT(("WiFi: Close WiFi fail\n"));
        return HI_FAILURE;
    }

    /* wait for driver deinitialization */
    sleep(1);
    DBGPRINT(("WiFi: Closed, then restart it\n"));

    memset(iface, 0, sizeof(iface));
    ret = HI_WLAN_AP_Open(iface);
    if (ret != HI_SUCCESS) {
        DBGPRINT(("WiFi: Open SoftAP fail\n"));
        return HI_FAILURE;
    }
    if (strcmp(iface, ifname) != 0) {
        DBGPRINT(("WiFi: Fail, new interface is '%s', expect for '%s'\n",
                      ifname, iface));
        return HI_FAILURE;
    }

    return HI_WLAN_AP_Start(ifname, pstApCfg);
}

HI_S32 HI_WLAN_AP_GetMacAddress(HI_CHAR *ifname, HI_CHAR *pstMac)
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
