#ifndef __WLAN_STA_H__
#define __WLAN_STA_H__

#include <sys/socket.h>
#include <sys/un.h>

#include "hi_wlan_sta.h"

#define WPA_SUPPLICANT_8
#define STRING_REPLY_SIZE    4096  /** the max size of command reply */

/** struct of socket information */
typedef struct hiHI_WPA_SOCKET_S {
    HI_S32 s;
    struct sockaddr_un local;
    struct sockaddr_un remote;
} HI_WPA_SOCKET_S;

/** struct of global data */
typedef struct hiHI_STA_DATA_S {
    HI_S32 device_id;
    HI_WPA_SOCKET_S ctrl_s;
    HI_WPA_SOCKET_S mon_s;
} HI_STA_DATA_S;

/* sta_init
 * brief: Init the sta module
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_init(HI_VOID);

/* sta_deinit
 * brief: Deinit the sta module
 * return  VOID
 */
HI_VOID sta_deinit(HI_VOID);

/* sta_load_driver
 * brief: Find WiFi device then load it's driver
 * return  0    successfull
 *         -1   cann't find supported device
 *         -2   failed to load driver
 */
HI_S32 sta_load_driver(HI_VOID);

/* sta_unload_driver
 * brief: Unload WiFi driver
 * param   VOID
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    failed to unload driver
 */
HI_S32 sta_unload_driver(HI_VOID);

/* sta_run_supplicant
 * brief: start wpa_supplicant process
 * param   ifname    wlan interface name
 * param   driver    interface of wpa_supplicant and driver, wext, nl80211 etc.
 * param   config_file    directory of wpa_supplicant.conf
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_run_supplicant(HI_CHAR *ifname, const HI_CHAR *driver,
                            const HI_CHAR *config_file);

/* sta_run_supplicant
 * brief: stop wpa_supplicant process
 * param   VOID
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_kill_supplicant(HI_VOID);

/* sta_wpa_request
 * brief: send command to wpa_supplicant
 * param   cmd     command string that send to wpa_supplicant
 * param   cbuf    buffer wpa_supplicant returned
 * param   size    size of buffer returned
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_wpa_request(HI_CHAR *cmd, HI_CHAR *cbuf, HI_S32 *size);

/* sta_parse_scan_result
 * brief: parse a line of scan results read from wpa_supplicant
 * param   line     string of a AP
 * param[out]   ap  AP's information     
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_parse_scan_result(HI_CHAR *line, HI_WLAN_STA_ACCESSPOINT_S *ap);

/* sta_receive_event_thread
 * brief: thread proc that read wpa_supplicant events
 * param   args     data pass to thread
 * return  VOID
 */
HI_VOID *sta_receive_event_thread(HI_VOID *args);

/* sta_connect_supplicant
 * brief: connect to wpa_supplicant
 * param   ifname    wlan interface name
 * param   ctrl_iface_dir    directory of control interface
 * return  HI_SUCCESS    successfull
 *         HI_FAILURE    fail
 */
HI_S32 sta_connect_supplicant(const HI_CHAR *ifname, const HI_CHAR *ctrl_iface_dir);

/* sta_disconnect_supplicant
 * brief: close connection to wpa_supplicant
 * param   VOID     
 * return  VOID
 */
HI_VOID sta_disconnect_supplicant(HI_VOID);

#endif /* __WLAN_STA_H__ */
