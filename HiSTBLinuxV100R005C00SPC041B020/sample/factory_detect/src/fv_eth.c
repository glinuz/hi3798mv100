#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <net/if.h> 
#include <dirent.h>
#include "hi_type.h"
#include "fv_config.h"
#include "fv_face.h"

#define MAX_DEV_NAME_LEN 16
#define PHY_BMSR 0x01
#define PHY_BMSR_LS 0x0004
#define PROCNET_DEV_PATH "/proc/net/dev"
#define LINK_ON "Link On"
#define LINK_OFF "Link Off"
#define FV_WIFI_SECURITY_NONE "NONE"
#define FV_WIFI_SECURITY_WEP "WEP"
#define FV_WIFI_SECURITY_WPAPSK "WPAPSK"

#define MAX_DEV_NR	4
int ip_set[MAX_DEV_NR];
int ip_link[MAX_DEV_NR] = {1,1,1,1};
int next_if;
typedef struct eth_devinfo
{
	HI_CHAR mac_addrs[18];
	HI_CHAR ip_addrs[INET_ADDRSTRLEN];
	HI_CHAR net_mask[INET_ADDRSTRLEN];
	HI_S32 link_status;
	HI_CHAR wifi_chip_type[48];
}FV_ETH_DEVINFO;

typedef struct eth_devname
{
	HI_CHAR name[MAX_DEV_NAME_LEN];
	struct eth_devname *next;
}HI_ETH_DEVNAME;


/* This data structure is used for all the MII ioctl's */
struct mii_data 
{
	HI_U16   phy_id;
	HI_U16   reg_num;
	HI_U16   val_in;
	HI_U16   val_out;
};

static HI_ETH_DEVNAME *fv_eth_get_devname(void)
{
	FILE *fd = NULL;   
	HI_CHAR tmp = 0;
	HI_CHAR buf_tmp[512] = {0};
	HI_CHAR *buf;
	HI_CHAR *name;
	HI_U32 invaild_line = 0;
	HI_ETH_DEVNAME *name_head, *name_tmp = NULL;

	if(NULL == (fd = fopen(PROCNET_DEV_PATH, "r")))
	{
		printf("Open %s faile.\n", PROCNET_DEV_PATH);         
		return (HI_ETH_DEVNAME *)NULL;
	}

	while((tmp = (HI_CHAR)fgetc(fd)))
	{
		if (0xa == tmp)
			invaild_line++;
		if (3 == invaild_line)
			break;
	}

	while (fgets(buf_tmp, sizeof(buf_tmp), fd))
	{      
		buf = buf_tmp;
		name_head = (HI_ETH_DEVNAME *)malloc(sizeof(HI_ETH_DEVNAME));
		if (NULL == name_head)
		{
			goto err_out;
			break;
		}

		name = name_head->name;

		while (isspace(*buf))
			buf++;

		while (*buf)
		{
			if (isspace(*buf) ||(*buf == ':'))
				break;
			else		
				*name++ = *buf++;
		}

		*name++ = '\0';
		name_head->next = name_tmp;
		name_tmp          = name_head;

	}    
	fclose(fd);
	return name_tmp;

err_out:
	name_head = name_tmp;
	while(NULL != name_head)
	{
		name_tmp = name_head->next;
		name_head->next = NULL;
		free(name_head);
		name_head = NULL;
		name_head = name_tmp;
	}

	fclose(fd);
	return (HI_ETH_DEVNAME *)NULL;
}
static void fv_wifi_get_devinfo(HI_U32* dev_id, HI_CHAR* mode, HI_U32 size)
{
	DIR* dir;
	struct dirent *next;
	char  buf[1024];
	char *p;
	FILE *f;
	int sz;
	int vendor_id = 0, device_id = 0, other;
	char wireless_mode[16]= {0};
#define DEVICE_ROOT_PATH        "/sys/bus/usb/devices/"
#define UEVENT_FILE_NAME        "uevent"
#define PRODUCT_FILE_NAME       "product"

	if (!dev_id || !mode)
		goto out;
	*dev_id = 0;
//	memset(mode, 0, size);

	dir = opendir(DEVICE_ROOT_PATH);
	if (!dir) {
		perror("Fail to open usb devices dir!");
		goto out;
	}

	while ((next = readdir(dir))!= NULL) {
		// skip usb1/usb2/.. and ./../ dir.
		if (strstr(next->d_name, "usb")|| strlen(next->d_name) <3)
			continue;
		// skip cat hub
		snprintf(buf, sizeof(buf), "cat %s/%s/%s | grep DRIVER", DEVICE_ROOT_PATH, next->d_name,UEVENT_FILE_NAME);
		f = popen(buf, "r");
		if (!f)
			continue;

		memset(buf, 0, sizeof(buf));
		sz = fread(buf, sizeof(char), sizeof(buf), f);
		if (!sz || strstr(buf, "hub")) {
			pclose(f);
			continue;
		}
		pclose(f);
		//skip none wireless device.
		snprintf(buf, sizeof(buf), "cat %s/%s/%s | grep 802\\.", DEVICE_ROOT_PATH, next->d_name,PRODUCT_FILE_NAME);
		f = popen(buf, "r");
		if (!f)
			continue;
		sz = fread(wireless_mode, sizeof(char), sizeof(wireless_mode), f);
		if (!sz) {
			pclose(f);
			continue;
		}
		//skip WLAN... string and '\r\n'.
		p = strcasestr(wireless_mode, "WLAN");
		*p = '\0';
		pclose(f);
		//get vendorid and device id
		snprintf(buf, sizeof(buf), "cat %s/%s/%s | grep PRODUCT", DEVICE_ROOT_PATH, next->d_name,UEVENT_FILE_NAME);
		f = popen(buf, "r");
		if (!f)
			continue;
		memset(buf, 0, sizeof(buf));
		sz = fread(buf,sizeof(char), sizeof(buf), f);
		if (!sz) {
			pclose(f);
			continue;
		}
		sscanf(buf, "PRODUCT=%x/%x/%x", &vendor_id,&device_id, &other);
		pclose(f);
		goto ok;
	}
ok:
	*dev_id = device_id;
	memcpy(mode, wireless_mode, strlen(wireless_mode) > size - 1 ? strlen(wireless_mode) : size - 1);
	mode[size - 1] = '\0';
out:
	return;
}

static HI_S32 fv_eth_get_devinfo (const HI_CHAR *dev_name, FV_ETH_DEVINFO *devinfo)
{
	struct ifreq ifr;
	struct sockaddr_in sin;
	struct mii_data mii;
	HI_S32 sock_fd = -1;
	HI_CHAR shell_cmd[256] = {0};

	if(!strncmp("eth", dev_name, 3) && !ip_set[next_if] && ip_link[next_if])
	{
		snprintf(shell_cmd, sizeof(shell_cmd), "udhcpc -n -A 0 -q -i %s", dev_name);
		system(shell_cmd);
		usleep(100000);
	}
	else
	{
		snprintf(shell_cmd, sizeof(shell_cmd), "ifconfig %s up", dev_name);
		system(shell_cmd);
		usleep(100000);
	}
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) 
	{
		printf("error: no inet socket available\n\r");
		return -1;
	}

	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, dev_name, sizeof(ifr.ifr_name)-1);

	memset(&sin, 0, sizeof(struct sockaddr_in));
	ifr.ifr_addr.sa_family = AF_INET;

	/* get mac address */
	(void)ioctl(sock_fd, SIOCGIFHWADDR, &ifr); 

	snprintf(devinfo->mac_addrs,   
			sizeof(devinfo->mac_addrs),
			"%02x:%02x:%02x:%02x:%02x:%02x", 
			(ifr.ifr_hwaddr.sa_data)[0] & 0xff,
			(ifr.ifr_hwaddr.sa_data)[1] & 0xff,
			(ifr.ifr_hwaddr.sa_data)[2] & 0xff,
			(ifr.ifr_hwaddr.sa_data)[3] & 0xff,
			(ifr.ifr_hwaddr.sa_data)[4] & 0xff,
			(ifr.ifr_hwaddr.sa_data)[5] & 0xff);

	/* get ip address */
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, dev_name, sizeof(ifr.ifr_name)-1);
	(void)ioctl(sock_fd, SIOCGIFADDR, &ifr); 
	memcpy(&sin, &ifr.ifr_addr, sizeof(struct sockaddr_in));
	(void)inet_ntop(AF_INET, &sin.sin_addr, devinfo->ip_addrs, INET_ADDRSTRLEN);

	/* get net mask */
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, dev_name, sizeof(ifr.ifr_name)-1);
	(void)ioctl(sock_fd, SIOCGIFNETMASK, &ifr);
	memcpy(&sin, &ifr.ifr_addr, sizeof(struct sockaddr_in));
	(void)inet_ntop(AF_INET, &sin.sin_addr, devinfo->net_mask, INET_ADDRSTRLEN);

	if(!strncmp("eth", dev_name, 3))
	{
		/* get link status */
		memset(&ifr, 0, sizeof(struct ifreq));
		strncpy(ifr.ifr_name, dev_name, sizeof(ifr.ifr_name)-1);
		memset(&mii, 0, sizeof(struct mii_data));
		mii.reg_num = PHY_BMSR;
		memcpy(&ifr.ifr_data, &mii, sizeof(struct mii_data));

		ioctl(sock_fd, SIOCGMIIPHY, &ifr);
		memcpy(&mii, &ifr.ifr_data, sizeof(struct mii_data));

		devinfo->link_status = ((mii.val_out & PHY_BMSR_LS) != 0) ? 1 : 0;
	}
	else
	{
		HI_U32 product_id = {0};
		HI_CHAR wifi_mode[16] = {0};
		fv_wifi_get_devinfo(&product_id, wifi_mode, 16);
		//printf("%s->%d, product_id:0x%x, wifi_mode:%s.\n",__func__,__LINE__,product_id,wifi_mode);
		snprintf(devinfo->wifi_chip_type, sizeof(devinfo->wifi_chip_type), "ID=%x MODE=%s",  product_id, wifi_mode);
	}

	close(sock_fd);
	return 0;
}

static void fv_eth_get_result(HI_CHAR *mac, HI_CHAR *ip, HI_CHAR *link, FV_ETH_DEVINFO *devinfo)
{
	if (!strncmp(devinfo->mac_addrs, "00:00:00:00:00:00", sizeof("00:00:00:00:00:00")))
		strncpy(mac, FV_DEV_NA, sizeof(FV_DEV_NA));
	else
		strncpy(mac, devinfo->mac_addrs, sizeof(devinfo->mac_addrs));

	if (!strncmp(devinfo->ip_addrs, "0.0.0.0", sizeof("0.0.0.0")))
		strncpy(ip, FV_DEV_NA, sizeof(FV_DEV_NA));
	else {
		ip_set[next_if] = 1;
		strncpy(ip, devinfo->ip_addrs, sizeof(devinfo->ip_addrs));
	}

#ifdef FV_ETH_NETMASK
	strncat(ip, " ", sizeof(" "));
	if (!strncmp(devinfo->net_mask, "0.0.0.0", sizeof("0.0.0.0")))
		strncat(ip, FV_DEV_NA, sizeof(FV_DEV_NA));
	else
		strncat(ip, devinfo->net_mask, sizeof(devinfo->net_mask));
#endif

	if(devinfo->link_status) {
		ip_link[next_if] = 1;
		strncpy(link, LINK_ON, sizeof(LINK_ON));
	} else {
		ip_link[next_if] = 0;
		ip_set[next_if] = 0;
		strncpy(link, LINK_OFF, sizeof(LINK_OFF));
	}
}

void fv_eth_detect(void)
{
	HI_ETH_DEVNAME *dev_name, *name_tmp = NULL;
	FV_ETH_DEVINFO devinfo;
	HI_S32  wired_text[MAX_WIRED_NET_NR][3] = {{FV_ITEM_ETH0_IP_TXT, FV_ITEM_ETH0_MAC_TXT, FV_ITEM_ETH0_LINK_TXT},
		{FV_ITEM_ETH1_IP_TXT, FV_ITEM_ETH1_MAC_TXT, FV_ITEM_ETH1_LINK_TXT}};
	HI_S32  wireless_text[MAX_WIRELESS_NET_NR][3] = {{FV_ITEM_WIRELESS_MODEL_TXT, FV_ITEM_WIRELESS_MAC_TXT, FV_ITEM_WIRELESS_IP_TXT}};
	HI_CHAR mac_addrs[18] = {0};
	HI_CHAR ip_addrs[32] = {0};
	HI_CHAR link_status[16] = {0};
	HI_S32 wired_index = 0;
	HI_S32 wireless_index = 0;

	next_if = 0;
	dev_name = fv_eth_get_devname();
	name_tmp = dev_name;

	while(NULL != name_tmp)
	{
		memset(&devinfo, 0, sizeof(FV_ETH_DEVINFO));
		if (!strncmp("eth", name_tmp->name, strlen("eth")))
		{
			(void)fv_eth_get_devinfo(name_tmp->name, &devinfo);
			fv_eth_get_result(mac_addrs, ip_addrs, link_status, &devinfo);
			(void)HI_FV_DrawText(wired_text[wired_index][0], ip_addrs, sizeof(ip_addrs));
			(void)HI_FV_DrawText(wired_text[wired_index][1], mac_addrs, sizeof(mac_addrs));
			(void)HI_FV_DrawText(wired_text[wired_index][2], link_status, sizeof(link_status));
			wired_index++;
		}
		else
		{
			(void)fv_eth_get_devinfo(name_tmp->name, &devinfo);
			fv_eth_get_result(mac_addrs, ip_addrs, link_status, &devinfo);
			(void)HI_FV_DrawText(wireless_text[wireless_index][0], devinfo.wifi_chip_type, sizeof(devinfo.wifi_chip_type));
			(void)HI_FV_DrawText(wireless_text[wireless_index][1], mac_addrs, sizeof(mac_addrs));
			(void)HI_FV_DrawText(wireless_text[wireless_index][2], ip_addrs, sizeof(ip_addrs));
			wireless_index++;
		}

		name_tmp = name_tmp->next;
		next_if++;
		if (next_if >= MAX_DEV_NR)
			break;
	}

	if(MAX_WIRED_NET_NR > wired_index)
	{
		switch(wired_index)
		{
			case 0:
				(void)HI_FV_DrawText(wired_text[wired_index][0], FV_DEV_NA, sizeof(FV_DEV_NA));
				(void)HI_FV_DrawText(wired_text[wired_index][1], FV_DEV_NA, sizeof(FV_DEV_NA));
				(void)HI_FV_DrawText(wired_text[wired_index][2], FV_DEV_NA, sizeof(FV_DEV_NA));
			case 1:
				(void)HI_FV_DrawText(wired_text[wired_index][0], FV_DEV_NA, sizeof(FV_DEV_NA));
				(void)HI_FV_DrawText(wired_text[wired_index][1], FV_DEV_NA, sizeof(FV_DEV_NA));
				(void)HI_FV_DrawText(wired_text[wired_index][2], FV_DEV_NA, sizeof(FV_DEV_NA));
				break;
			default:
				break;
		};
	}

	if (MAX_WIRELESS_NET_NR > wireless_index)
	{
		(void)HI_FV_DrawText(wireless_text[wireless_index][0], FV_DEV_NA, sizeof(FV_DEV_NA));
		(void)HI_FV_DrawText(wireless_text[wireless_index][1], FV_DEV_NA, sizeof(FV_DEV_NA));
		(void)HI_FV_DrawText(wireless_text[wireless_index][2], FV_DEV_NA, sizeof(FV_DEV_NA));
	}

	while(NULL != dev_name)    
	{
		name_tmp = dev_name->next;
		dev_name->next = NULL;
		free(dev_name);
		dev_name = NULL;
		dev_name = name_tmp;
	}
}

