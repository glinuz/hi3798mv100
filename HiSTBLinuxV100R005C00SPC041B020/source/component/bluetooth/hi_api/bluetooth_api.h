#ifndef  _BLUETOOTH_API_H
#define _BLUETOOTH_API_H

#include "bluetooth/bluetooth.h"
#include "hi_bluetooth.h"
//#define __DEBUG

#define pinfo( format, arg...) \
	HI_BT_INFO(format , ## arg)
#define perror( format, arg...) \
	HI_BT_ERR(format , ## arg)

#ifdef 	__DEBUG
#define pdbg( format, arg...) \
	HI_BT_ERR(format , ## arg)
#else
#define pdbg( format, arg...)  
#endif


struct bt_device{
	char dev_name[48];
	int status;
	bdaddr_t bdaddr;
	short reserved;
	int	paired;
	unsigned int dev_class;
};

#define HI_BLUETOOTH_DEV_LIST_MAX    10

int hci_read_bd_addr(int dd, bdaddr_t *bdaddr, int to);
int get_adapter_bdaddr(const char * hci_name,bdaddr_t *bdaddr);
int get_adapter_updown_status(const char *hci_name,int *updown_status);
int enable_adapter(const char* hci_name, int enable);
int adapter_scan_devices(const char *hci_name,struct bt_device *pdevice,int max);
int pair_with_device(const char *hci_name,struct bt_device *pdev,char * passkey);
int get_paired_devices(const char *hci_name,struct bt_device *pdev,int max);
int get_adpater_property_uint(const char * hci_name,char * property,unsigned int *value);
int remove_paried_device(const char * hci_name,struct bt_device *pdev );
int get_device_conn_status(const char *hci_name,struct bt_device *pdev,unsigned int *status);
int connect_with_device(const char *hci_name,struct bt_device *pdev,unsigned char connect);
int set_trust_device(const char *hci_name,struct bt_device *pdev);
int get_headset_conn_status(const char *hci_name,struct bt_device *pdev,unsigned int *status);
int headset_ring(const char *hci_name,struct bt_device *pdev,unsigned char ring);
int get_connected_headset(const char *hci_name,struct bt_device *pdev);

#endif //end of _BLUETOOTH_API_H
