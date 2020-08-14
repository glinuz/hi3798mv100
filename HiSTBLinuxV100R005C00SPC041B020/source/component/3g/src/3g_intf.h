/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : 3g_intf.h
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2013-08-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __3G_INTF_H__
#define __3G_INTF_H__

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/klog.h>
#include <syslog.h>
#include <string.h>
#include <libgen.h>
#include <errno.h>

#define __MODULE_NAME__      "g3_card"

#define G3_TRACE_LEVEL_FATAL    (0)
#define G3_TRACE_LEVEL_ERROR    (1)
#define G3_TRACE_LEVEL_WARN     (2)
#define G3_TRACE_LEVEL_INFO     (3)
#define G3_TRACE_LEVEL_DBG      (4)

#ifndef G3_TRACE_LEVEL
#  define G3_TRACE_LEVEL G3_TRACE_LEVEL_ERROR
//#  define G3_TRACE_LEVEL G3_TRACE_LEVEL_DBG
#endif

/*lint -emacro(506, G3_PRINTF)    ignore by y00183183, ecause for printf control  */
/*lint -emacro(774, G3_PRINTF)   ignore by y00183183, because for printf control  */
#define G3_PRINTF(level, fmt, args...) do{\
    if (level <= G3_TRACE_LEVEL) { \
        printf("%s:%s(%d)!!! BUG "fmt, \
        __MODULE_NAME__, __FILE__, __LINE__, ##args); \
    } \
} while (0)

#undef G3_DEBUG
#ifdef G3_DEBUG
#define G3_ASSERT(expr) \
    if (!(expr)) {                                          \
        printf( "Assertion failed! %s,%s,%s,line=%d,condition:(%s)\n", \
        #expr,__FILE__,__func__,__LINE__, #expr);              \
    }
#else
#define G3_ASSERT(expr) do {} while (0)
#endif

/*********************************************************************/
#define ESYS             -1
#define EREPLY           -2
#define ETIMEOUT         -3
#define ENOCARD          -4
#define EINVALID         -5
#define ENOMEMERY        -6

/*********************************************************************/

typedef enum g3_card_port_type_e
{
    G3_CARD_PORT_TYPE_USB,
    G3_CARD_PORT_TYPE_UART,
    G3_CARD_PORT_TYPE__BOTT,
} g3_card_port_type_e;

typedef struct g3_card {
    char inf_unit[4];
    char manufacturer[64]; /** 64 should be enough. */
    char product_name[16]; /** 16 should be enough. */
    char vendor_id[8];
    char product_id[8];
    char device[16];
    char pcui[16];
    g3_card_port_type_e etype;
    char operator_name[16];
    //int (*pre_init)(void);
}g3_card;

/** 3g card compatible list */
typedef struct g3_card_ids
{
    char manufacturer[64];                /** manufacturer name */
    char product_name[16];                /** product_name */
    char VID[8];                          /** vendor id */
    char PID[8];                          /** product id */
    char modem_num[4];                    /** modem port number */
    char pcui_num[4];                     /** pcui port number */
    int (*pre_init)(g3_card *pcard);    /** card private init function */
}g3_card_ids;

typedef struct g3_operator {
    char netstat[2];
    char long_operator[32];
    char short_operator[16];
    char numeric_operator[16];
    char rat[2];
}g3_operator;

typedef struct g3_current_operator {
    char netmode[2];
    char operatorformat[2];
    char operatorinfo[32];
    char rat[2];
}g3_current_operator;

typedef struct g3_pdp {
    char cid[4];
    char pdp_type[8];
    char apn[32];
    char pdp_ipaddr[32];
    char d_comp[2];
    char h_comp[2];
}g3_pdp;

typedef struct g3_card_flow {
    long lastdstime;
    long long lasttxflow;
    long long lastrxflow;
    long totaldstime;
    long long totaltxflow;
    long long totalrxflow;
}g3_card_flow;

typedef struct g3_card_sim_status {
    char status_code[16];
    int times;
    int puk_times;
    int pin_times;
    int puk2_times;
    int pin2_times;
} g3_card_sim_status;

#define MAX_CARD_NUM                      2
#if defined (ANDROID)
#define PPPD_CMD_PATH                     "/system/bin/pppd"
#define PPPD_PID_FILE_PREFIX              "/system/etc/ppp/ppp"
#define PPPD_DIALER_SCRIPT                "/data/ppp/huawei-ppp-on-dialer"
#else
#define PPPD_CMD_PATH                     "/usr/local/sbin/pppd"
#define PPPD_PID_FILE_PREFIX              "/tmp/ppp/ppp"
#define PPPD_DIALER_SCRIPT                "/tmp/ppp/huawei-ppp-on-dialer"
#endif

#define PPPD_PID_FILE_SUFFIX              ".pid"

#define MAX_PPPD_ARGS                     64
#define PPPD_OPTIONAL_ARG_START           20
#define PPPD_INTERFACE_NUMBER             "0"
#define PPP_INTERFACE_PATH                "/proc/net/dev"

/* COPS macro */

#define MAX_CMD_RETRIES                   3
#define MAX_CMD_TIMEOUT                   1
#define MAX_OPERATOR_NUM                  20


typedef enum g3_card_status_e {
    G3_CARD_STATUS_UNAVALIABLE,
    G3_CARD_STATUS_SIM_PIN,
    G3_CARD_STATUS_SIM_PUK,
    G3_CARD_STATUS_SIM_PIN2,
    G3_CARD_STATUS_SIM_PUK2,
    G3_CARD_STATUS_DISCONNECTED,
    G3_CARD_STATUS_CONNECTED,
    G3_CARD_STATUS_BOTT,
}g3_card_status_e;

typedef enum g3_card_mode_e {
    G3_CARD_MODE_AUTO = 2,
    G3_CARD_MODE_GSM_ONLY = 13,
    G3_CARD_MODE_UTRAN_ONLY = 14,
    G3_CARD_MODE_TDSCDMA_ONLY = 15,
    G3_CARD_MODE_NO_CHANGE = 16,
    G3_CARD_MODE_BOTT = 17,
} g3_card_mode_e;

typedef enum g3_card_acqorder_e {
    G3_CARD_ACQORDER_AUTO = 0,
    G3_CARD_ACQORDER_GSM_PRIOR_UTRAN,
    G3_CARD_ACQORDER_UTRAN_PRIOR_GSM,
    G3_CARD_ACQORDER_NO_CHANGE,
    G3_CARD_ACQORDER_BOTT,
} g3_card_acqorder_e;

typedef enum g3_card_cops_mode_e {
    G3_CARD_COPS_MODE_AUTO,
    G3_CARD_COPS_MODE_MANUAL,
    G3_CARD_COPS_MODE_DEREGISTER,
    G3_CARD_COPS_MODE_SET_OP_FORMAT,
    G3_CARD_COPS_MODE_MANUAL_AUTO,
    G3_CARD_COPS_MODE_BOTT,
} g3_card_cops_mode_e;

typedef enum g3_card_cops_op_format_e {
    G3_CARD_COPS_OP_FORMAT_LONG_STRING,
    G3_CARD_COPS_OP_FORMAT_SHORT_STRING,
    G3_CARD_COPS_OP_FORMAT_NUMERIC_NUMBER,
    G3_CARD_COPS_OP_FORMAT_BOTT,
} g3_card_cops_op_format_e;

/*
typedef enum g3_card_srvdomain_e {
    G3_CARD_SRVDOMAIN_CS_ONLY = 0,
    G3_CARD_SRVDOMAIN_PS_ONLY,
    G3_CARD_SRVDOMAIN_CS_PS,
    G3_CARD_SRVDOMAIN_ANY,
    G3_CARD_SRVDOMAIN_BOTT,
} g3_card_srvdomain_e;

typedef enum g3_card_roam_e {
    G3_CARD_ROAM_DISABLE = 0,
    G3_CARD_ROAM_ENABLE,
    G3_CARD_ROAM_NOCHANGE,
    G3_CARD_ROAM_BOTT,
} g3_card_roam_e;
*/

int g3_card_init(g3_card *card);
int g3_card_deinit(g3_card *card);
int g3_card_get_sim_status(g3_card *card, g3_card_status_e *status);
int g3_card_get_conn_status(g3_card *card, g3_card_status_e *status);
int g3_card_set_mode_acqorder(g3_card *card,
                  g3_card_mode_e mode,
                  g3_card_acqorder_e acqorder);
int g3_card_get_card_status(g3_card *card, int *status);
int g3_card_get_all_operators(g3_card *card,
                  g3_operator *operators,
                  int maxnum,
                  int *opnum);
int g3_card_register_operator(g3_card *card,
                  g3_card_cops_mode_e cops_mode,
                  g3_card_cops_op_format_e cops_format,
                  char *cops_op);
int g3_card_get_current_pdp(g3_card *card, g3_pdp *pdp);
int g3_card_set_pdp(g3_card *card, char *apn);
int g3_card_get_current_operator(g3_card *card,
                 g3_current_operator *current_operator);
int g3_card_get_quality(g3_card *card, int *issr, int *ber);
int g3_card_get_dataflow(g3_card *card, g3_card_flow *flow);
int g3_card_scan(g3_card *card, int maxnum, int *cardnum, char *port);
int g3_card_connect(g3_card *card,
            char *account,
            char *password,
            char *telephone,
            int  argvnum,
            char *argv[]);
int g3_card_disconnect(g3_card *card);

int g3_card_send_cmd(char *cmd, int len);
int g3_card_get_cmd_result(char *buf, int buflen, int timeout);
int g3_pcui_open(g3_card *card);
void g3_pcui_close(g3_card *card);

#endif /* __3G_INTF_H__ */

