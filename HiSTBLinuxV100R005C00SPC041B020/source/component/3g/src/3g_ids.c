/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : 3g_ids.c
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2014-06-04
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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
#include "3g_intf.h"

/*********************************************************************/
static int g3_card_huawei_ec169_set_crm(g3_card *card)
{
    char *cmd_set_crm = "AT+CRM=1\r\n";
    char readbuf[128];
    int ret = -1;

    ret = g3_card_send_cmd(cmd_set_crm, strlen(cmd_set_crm));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);
    if (ret != 0)
        return ret;
    return 0;
}
/*********************************************************************/

static int g3_card_zte_adjust_modem(g3_card *card)
{
    char tmp[16];

    G3_ASSERT(card != NULL);

    g3_pcui_close(card);

    /* swap pcui and modem port  */
    strncpy(tmp, card->pcui, sizeof(card->pcui));
    strncpy(card->pcui, card->device, sizeof(card->pcui));
    strncpy(card->device, tmp, sizeof(card->pcui));

    return g3_pcui_open(card);
}

/*********************************************************************/
g3_card_ids g3_card_list[] = {
    /**----------------- HUAWEI ---------------*/
    {   /* HUAWEI: EC169 */
        .product_name = "EC169",
        .pre_init     = g3_card_huawei_ec169_set_crm,
    },

    /**----------------- ZTE      ---------------*/
    {   /* ZTE: MF190 */
        .manufacturer = "ZTE CO",
        .product_name = "MF190",
        .pre_init     = g3_card_zte_adjust_modem,
    },
    /* Add new model here... */

    /* Add new model end */
    {   /* END of list */
        .manufacturer = "",
        .product_name = "",
        .pre_init      = NULL,
    },
};

/******************************************************************************/

