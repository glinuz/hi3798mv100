#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <spawn.h>
#include <sys/wait.h>
#include <dirent.h>

#include "wlan_util.h"

#define DRIVER_MODULE_LEN_MAX    256

static const HI_CHAR USB_DIR[] = "/sys/bus/usb/devices";
static const HI_CHAR MODULE_FILE[] = "/proc/modules";

static hi_wifi_device_info dev[] = {
    {WIFI_ID_RALINK_RT3070, WIFI_RALINK_RT3070},
    {WIFI_ID_RALINK_RT5370, WIFI_RALINK_RT5370},
    {WIFI_ID_RALINK_RT5372, WIFI_RALINK_RT5372},
    {WIFI_ID_RALINK_RT5572, WIFI_RALINK_RT5572},
    {WIFI_ID_RALINK_MT7601U, WIFI_RALINK_MT7601U},
    {WIFI_ID_ATHEROS_AR9271, WIFI_ATHEROS_AR9271},
    {WIFI_ID_ATHEROS_AR9374, WIFI_ATHEROS_AR9374},
    {WIFI_ID_REALTEK_RTL8188SU, WIFI_REALTEK_RTL8188SU},
    {WIFI_ID_REALTEK_RTL8192SU, WIFI_REALTEK_RTL8192SU},
    {WIFI_ID_REALTEK_RTL8188CUS, WIFI_REALTEK_RTL8188CUS},
    {WIFI_ID_REALTEK_RTL8192CU, WIFI_REALTEK_RTL8192CU},
    {WIFI_ID_REALTEK_RTL8188EUS, WIFI_REALTEK_RTL8188EUS},
    {WIFI_ID_REALTEK_RTL8188ETV, WIFI_REALTEK_RTL8188ETV},
    {WIFI_ID_REALTEK_RTL8192DU_VC, WIFI_REALTEK_RTL8192DU},
    {WIFI_ID_REALTEK_RTL8192DU_VS, WIFI_REALTEK_RTL8192DU},
    {WIFI_ID_REALTEK_RTL8192EU, WIFI_REALTEK_RTL8192EU},
    {WIFI_ID_REALTEK_RTL8812AU, WIFI_REALTEK_RTL8812AU},
    {WIFI_ID_REALTEK_RTL8812AU_VS, WIFI_REALTEK_RTL8812AU},
    {WIFI_ID_ATHEROS_QCA1021G, WIFI_ATHEROS_QCA1021G},
    {WIFI_ID_ATHEROS_QCA1021X, WIFI_ATHEROS_QCA1021X},
    {WIFI_ID_BROADCOM_BCM43236, WIFI_BROADCOM_BCM43236},
    {WIFI_ID_REALTEK_RTL8723BU, WIFI_REALTEK_RTL8723BU},
    {WIFI_ID_REALTEK_RTL8821AU, WIFI_REALTEK_RTL8821AU},
    {WIFI_ID_MEDIATEK_MT7632TU, WIFI_MEDIATEK_MT7632TU},
    {WIFI_ID_MEDIATEK_MT7662TU, WIFI_MEDIATEK_MT7662TU},
    {WIFI_ID_MEDIATEK_MT7662TE, WIFI_MEDIATEK_MT7662TE},
    {WIFI_ID_REALTEK_RTL8822BU, WIFI_REALTEK_RTL8822BU}
};

HI_S32 wlan_util_get_wifi_device(HI_VOID)
{
    HI_S32 ret = -1;
    DIR *dir;
    struct dirent *next;
    FILE *fp = NULL;

    dir = opendir(USB_DIR);
    if (!dir) {
        return -1;
    }

    while ((next = readdir(dir)) != NULL) {
        HI_CHAR line[MAX_LEN_OF_LINE];
        HI_CHAR uevent_file[256] = {0};

        /* read uevent file, uevent's data like below:
         * MAJOR=189
         * MINOR=4
         * DEVNAME=bus/usb/001/005
         * DEVTYPE=usb_device
         * DRIVER=usb
         * DEVICE=/proc/bus/usb/001/005
         * PRODUCT=bda/8176/200
         * TYPE=0/0/0
         * BUSNUM=001
         * DEVNUM=005
         */
        snprintf(uevent_file, sizeof(uevent_file), "%s/%s/uevent", USB_DIR, next->d_name);
   
        fp = fopen(uevent_file, "r");
        if (fp == NULL) {
            continue;
        }

        while (fgets(line, sizeof(line), fp)) {
            HI_CHAR *pos = NULL;
            HI_S32 product_vid;
            HI_S32 product_did;
            HI_S32 product_bcddev;
            HI_CHAR device_id[32];

            pos = strstr(line, "PRODUCT=");
            if(pos != NULL) {
                HI_S32 i;

                if (sscanf(pos + 8, "%x/%x/%x", &product_vid, &product_did,
                             &product_bcddev) <= 0)
                    continue;
                snprintf(device_id, sizeof(device_id), "%04x:%04x", product_vid, product_did);

                for (i = 0; i < sizeof(dev)/sizeof(hi_wifi_device_info); i++) {
                    if(strcmp(device_id, dev[i].usb_id) == 0) {
                        ret = dev[i].id;
                        break;
                    }
                }
            }
            if (ret != -1)
                break;
        }
        fclose(fp);
        if (ret != -1)
            break;
    }

    closedir(dir);

    return ret;
}

HI_S32 wlan_util_insmod_module(HI_CHAR *module, HI_CHAR *module_tag,
                                 HI_CHAR *param)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"insmod", module, param, NULL};
    FILE *proc;
    HI_CHAR line[DRIVER_MODULE_LEN_MAX+10];

    /* if module is loaded, return ok */
    if ((proc = fopen(MODULE_FILE, "r")) == NULL) {
        DBGPRINT(("Could not open %s\n", MODULE_FILE));
        return -1;
    }

    while ((fgets(line, sizeof(line), proc)) != NULL) {
        if (strncmp(line, module_tag, strlen(module_tag)) == 0) {
            fclose(proc);
            return 0;
        }
    }

    fclose(proc);

    /* insmod module */
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

HI_S32 wlan_util_rmmod_module(HI_CHAR *module)
{
    HI_S32 ret;
    HI_S32 pid = 0;
    HI_S32 status = 0;
    HI_CHAR *spawn_env[] = {NULL};
    HI_CHAR *spawn_args[] = {"rmmod", module, NULL, NULL};
    
    /* rmmod module */
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

HI_S32 wlan_util_get_interface(HI_CHAR *ifname, const HI_CHAR *dev_file)
{
    HI_S32 ret = HI_FAILURE;
    HI_CHAR buff[1024];
    FILE *fh;
    HI_CHAR *begin, *end;

    if (ifname == NULL || dev_file == NULL)
        return HI_FAILURE;

    fh = fopen(dev_file, "r");
    if(fh != NULL) {
        /* Eat 2 lines of header */
        fgets(buff, sizeof(buff), fh);
        fgets(buff, sizeof(buff), fh);

        /* Read each device line */
        while(fgets(buff, sizeof(buff), fh)) {
            /* Skip empty or almost empty lines. It seems that in some
             * cases fgets return a line with only a newline. */
            if((buff[0] == '\0') || (buff[1] == '\0'))
                continue;

            begin = buff;
            while(*begin == ' ')
                begin++;

            end = strstr(begin, ": ");
            /* Not found ??? To big ??? */
            if((end == NULL) || (((end - begin) + 1) > (IFNAMSIZ + 1)))
                continue;
            if (strncmp(begin, "wlan", 4) != 0)
                continue;

            /* Copy */
            memcpy(ifname, begin, (end - begin));
            ifname[end - begin] = '\0';
            ret = HI_SUCCESS;
            break;
        }

    }

    return ret;
}

HI_S32 wlan_util_frequency_to_channel(HI_S32 freq)
{
    HI_S32 chn;

    if (freq == 2484)
        chn = 14;
    else if (freq < 2484)
        chn = (freq - 2407)/5;
    else if (freq >= 4910 && freq <= 4980)
        chn = (freq - 4000)/5;
    else
        chn = (freq - 5000)/5;

    return chn;
}

HI_S32 wlan_util_read_line(HI_CHAR *buf, HI_CHAR *line)
{
    HI_S32 i = 0;

    if (line == NULL || buf == NULL)
        return 0;

    while (*buf != '\0') {
        i++;
        if (*buf == '\n')
            break;

        *line++ = *buf++;
    }
    *line = '\0';

    return i;
}

