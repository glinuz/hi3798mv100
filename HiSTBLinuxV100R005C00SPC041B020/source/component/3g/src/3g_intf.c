/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : 3g_intf.c
Version       : Initial Draft
Author        : Hisilicon OSDRV group
Created       : 2013-08-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "3g_intf.h"
#include <net/if.h>

/******************************************************************************/
static int fd_pcui = 0;
static int error_format_saved = 0;
static struct termios tio_saved;
extern g3_card_ids g3_card_list[];
#if defined (ANDROID)
#define LOG_TAG "3g_intf"
#include <cutils/log.h>
#define HI_ERR_3G(fmt...)        SLOGI(fmt)
#else
#define HI_ERR_3G(fmt...)
#endif


/******************************************************************************/

static void squeeze(char s[], char c)
{
    int i,j;
    for (i = 0, j = 0; s[i] != '\0'; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}

/******************************************************************************/
static int set_pcui(int speed, int bits, char event, int stop)
{

    struct termios newtio;
    int ret;

    G3_ASSERT((speed == 2400)
          || (speed == 4800)
          || (speed == 9600)
          || (speed == 115200));

    G3_ASSERT((bits == 7)
          || (bits == 8));

    G3_ASSERT((event == 'O')
          || (event == 'E')
          || (event == 'N'));

    G3_ASSERT((stop == 1)
          || (stop == 2));

    ret = tcgetattr(fd_pcui, &tio_saved);
    if (ret != 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "save pcui attr failed!(%s)\n", strerror(errno));
        return ret;
    }

    bzero(&newtio, sizeof(newtio) );
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch (bits) {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch (event) {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch (speed) {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }

    if (stop == 1)
        newtio.c_cflag &= ~CSTOPB;
    else if (stop == 2)
        newtio.c_cflag |= CSTOPB;

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd_pcui, TCIOFLUSH);
    ret = tcsetattr(fd_pcui, TCSANOW, &newtio);
    if (ret != 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "set pcui failed!(%s)\n", strerror(errno));
        return ret;
    }

    return 0;
}
/******************************************************************************/

int g3_pcui_open(g3_card *card)
{
    int ret;
    int speed = 115200;
    int bits = 8;
    char events = 'N';
    int stop = 1;
    char *pcui;

    G3_ASSERT((card != NULL) && (card->pcui));

    pcui = card->pcui;

    if (fd_pcui <= 0) {
        fd_pcui= open(pcui, O_RDWR | O_NOCTTY | O_NDELAY);
        if (fd_pcui < 0) {
            HI_ERR_3G("open pcui(%s) failed!(%s)\n",
                  pcui, strerror(errno));
            return ESYS;
        }
        ret = set_pcui(speed, bits, events, stop);
        if (ret < 0) {
            close(fd_pcui);
            fd_pcui = -1;
            HI_ERR_3G("set_pcui failed,ret=%d\n", ret);
            return ESYS;
        }
    }

    return 0;
}
/*********************************************************************/

void g3_pcui_close(g3_card *card)
{
    close(fd_pcui);
    fd_pcui = -1;
    HI_ERR_3G("close fd_pcui success\n");
}
/*********************************************************************/

static int pcui_write(int fd, const void *buf, int count)
{
    int n;

    do {
        n = write(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}
/*********************************************************************/

static int pcui_read(int fd, void *buf, int count)
{
    int n;

    do {
        n = read(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}

/******************************************************************************/

int g3_card_send_cmd(char *cmd, int len)
{
    int ret;

    G3_ASSERT((cmd != NULL) && (len > 0));

    if (fd_pcui <= 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR, "fd_pcui invalid!\n");
        return EBADFD;
    }

    ret = pcui_write(fd_pcui, cmd, len);
    if(ret != len) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "send command(%s) failed!\n", cmd);
        close(fd_pcui);
        fd_pcui = -1;
        return ESYS;
    }

    return 0;
}

/******************************************************************************/
int g3_card_get_cmd_result(char *buf, int buflen, int timeout)
{
    int ret = 0;
    int retry = 0;

    G3_ASSERT((buf != NULL) && (buflen > 0));

    tcflush(fd_pcui, TCOFLUSH);
    if (fd_pcui <= 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "pcui is not open, fd_pcui=%d\n",
              fd_pcui);
        return EBADFD;
    }

    memset(buf, 0, buflen);

    while(retry <= timeout) {
        ret = pcui_read(fd_pcui, buf, buflen - 1);
        buf[buflen - 1] = '\0';

        if ((ret <= 0)
           || (strstr(buf, "ERROR") == NULL
           && strstr(buf, "\r\nOK\r\n") == NULL)) {
            sleep(1);
            retry ++;
        } else
            break;

    }

    if ((timeout > 0) && (retry > timeout)) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "get command results failed: %s\n", buf);
        return ETIMEOUT;
    }
    tcflush(fd_pcui, TCIOFLUSH);

    return 0;
}

/******************************************************************************/

static int g3_card_get_manufacturer(g3_card *card)
{
    char *cmd_get_manu = "AT+CGMI\r\n";
    char *str;
    char *strend;
    char *stopstr;
    char readbuf[2048] = {0};
    char *tmpbuf;
    int ret;
    int len = 0;
    char error_code[8] = {0};
    int error;

    G3_ASSERT(card != NULL);

    ret = g3_card_send_cmd(cmd_get_manu, strlen(cmd_get_manu));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    if (ret != 0)
        return ret;

    tmpbuf = readbuf;
    if ((str = strstr(tmpbuf, "\r\n\r\nOK\r\n")) != NULL) {

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "buf=%s\n", tmpbuf);

        len = str - tmpbuf;
        tmpbuf[len] = '\0';
        str = strstr(tmpbuf, "\r\n");
        if (str) {
            str = str + strlen("\r\n");
            if (NULL != strstr(str, "+GMI: "))
                str += strlen("+GMI: ");
            snprintf(card->manufacturer,
                 sizeof(card->manufacturer), "%s", str);
            return 0;
        }

    } else if ((str = strstr(tmpbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:") - 1;
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "get manufacturer name reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

static int g3_card_get_product_name(g3_card *card)
{
    char *cmd_get_product = "AT+CGMM\r\n";
    char *str;
    char *strend;
    char *stopstr;
    char readbuf[2048] = {0};
    char *tmpbuf;
    int ret;
    int len = 0;
    char error_code[8] = {0};
    int error;

    G3_ASSERT(card != NULL);

    ret = g3_card_send_cmd(cmd_get_product, strlen(cmd_get_product));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    if (ret != 0)
        return ret;

    tmpbuf = readbuf;

    if ((str = strstr(tmpbuf, "\r\n\r\nOK\r\n")) != NULL) {

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "buf=%s\n", tmpbuf);

        len = str - tmpbuf;
        tmpbuf[len] = '\0';
        str = strstr(tmpbuf, "\r\n");
        if (str) {
            str = str + strlen("\r\n");
            snprintf(card->product_name,
                 sizeof(card->product_name),
                 "%s", str);
            return 0;
        }

    } else if ((str = strstr(tmpbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get product name reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/
#if 0
int g3_card_get_pcui(g3_card *card)
{
    int ret;
    int speed = 115200;
    int bits = 8;
    char events = 'N';
    int stop = 1;
    char *modem = "/dev/ttyUSB0";
    char *cmd_get_pcui = "AT^GETPORTMODE\r\n";
    char *str;
    char *stopstr;
    char buf[256] = {0};
    char error_code[8] = {0};
    int fd_mdm;
    int len;
    int error;

    G3_ASSERT(card != NULL);

    fd_mdm= open(modem, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd_mdm < 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "open modem(%s) failed!(%s)\n",
              modem, strerror(errno));
        return ESYS;
    }
    ret = set_pcui(speed, bits, events, stop);
    if (ret < 0) {
        close(fd_mdm);
        return ESYS;
    }

    /* get pcui */
    ret = g3_card_send_cmd(cmd_get_pcui, strlen(cmd_get_pcui));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(buf, sizeof(buf), 1);
    if (ret != 0)
        return ret;

    if (strstr(buf, "\r\n^PORTMODE: ") != NULL &&
       (strstr(buf, "\r\nOK\r\n") != NULL)) {
        str = strstr(buf, "\r\n^PORTMODE: ") +
                 strlen("\r\n^PORTMODE: ");

        len = strstr(buf, "\r\nOK\r\n") - str;
        str[len] = '\0';
        str = strstr(str, "PCUI:");
        if (str) {
            str = str + strlen("PCUI:");
            if (strstr(str, ",")) {
                len = strstr(str, ",") -str;
                str[len] = '\0';
            }
            snprintf(card->pcui,
                 sizeof(card->pcui), "/dev/ttyUSB%s", str);
            return 0;
        }
    } else if (strstr(buf, "+CME ERROR:")) {
        str = strstr(buf, "+CME ERROR:") + strlen("+CME ERROR:");
        if (strstr(str, "\r\n") != NULL) {
            len = strstr(str, "\r\n") - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get pcui reply:%s\n", buf);
    return EREPLY;

}

#endif
/******************************************************************************/

static int g3_card_set_echo(g3_card *card, int echo_mode)
{
    char *cmd_echo;
    int ret;
    char readbuf[16] = {0};

    G3_ASSERT((card != NULL) && ((echo_mode == 0) || (echo_mode == 1)));

    switch (echo_mode) {
        case 0:
            cmd_echo = "ATE0\r\n";
            break;
        case 1:
        default:
            cmd_echo = "ATE1\r\n";
            break;
    }

    /* disable echo */
    ret = g3_card_send_cmd(cmd_echo, strlen(cmd_echo));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    if (ret != 0)
        return ret;

    if (strstr(readbuf, "\r\nOK\r\n") != NULL)
        return 0;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "set echo reply:%s\n", readbuf);

    return EREPLY;
}
/*********************************************************************/

static int g3_card_get_error_format(g3_card *card, int *error_mode)
{
    char *cmd_get_error_format = "AT+CMEE?\r\n";
    char readbuf[32];
    int len;
    char *str;
    char *strend;
    char *stopstr = NULL;
    int ret;

    G3_ASSERT((card != NULL) && (error_mode != NULL));
    memset(readbuf, 0, sizeof(readbuf));

    ret = g3_card_send_cmd(cmd_get_error_format,
                   strlen(cmd_get_error_format));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    /* AT+CMEE return have bug, not return here!!! */

    str = strstr(readbuf, "CMEE:");
    if (str) {
        str += strlen("CMEE:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            *error_mode = strtol(str, &stopstr, 10);
            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                  "error_mode=%d\n", *error_mode);
            return 0;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get error format reply:%s\n", readbuf);

    return EREPLY;
}

static int g3_card_set_error_format(g3_card *card, int error_mode)
{
    char *cmd_set_error_format;
    char readbuf[16];
    int ret;

    G3_ASSERT((card != NULL)
         && ((error_mode == 0)
         || (error_mode == 1)
         || (error_mode == 2)));

    switch(error_mode) {
    case 0:
        cmd_set_error_format = "AT+CMEE=0\r\n";
        break;
    case 1:
        cmd_set_error_format = "AT+CMEE=1\r\n";
        break;
    case 2:
    default:
        cmd_set_error_format = "AT+CMEE=2\r\n";
        break;
    }

    ret = g3_card_send_cmd(cmd_set_error_format,
                   strlen(cmd_set_error_format));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);

    if (ret != 0)
        return ret;

    if (strstr(readbuf, "\r\nOK\r\n") != NULL)
        return 0;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "set error format reply:%s\n", readbuf);

    return EREPLY;
}


static int g3_card_init_pcui(g3_card *card)
{
    int ret;

    G3_ASSERT(card != NULL);

    ret = g3_pcui_open(card);
    if (ret != 0)
        return ret;

    /* disable echo */
    ret = g3_card_set_echo(card, 0);
    if (ret != 0)
        goto ERR;

    /* save old error format. error format invalid after power down */
    ret = g3_card_get_error_format(card, &error_format_saved);
    if (ret < 0)
        goto ERR;

    /* set error format:
     * 0: "ERROR";
     * 1: +CME ERROR:<err>result code, <err> is numeric number
     * 2: +CME ERROR:<err>result code, <err> is string format
     */
    if (1 != error_format_saved) {
        ret = g3_card_set_error_format(card, 1);
        if (ret != 0)
            goto ERR;
    }

    return 0;

ERR:
    g3_pcui_close(card);
    return ret;
}
/******************************************************************************/

int g3_card_init(g3_card *card)
{
    int ret;
    g3_card_ids *cardtmp;

    G3_ASSERT(card != NULL);

    ret = g3_card_init_pcui(card);
    if (ret != 0)
        return ret;

    ret = g3_card_get_manufacturer(card);
    if (ret != 0)
        goto ERR;

    ret = g3_card_get_product_name(card);
    if (ret != 0)
        goto ERR;

    for (cardtmp = g3_card_list; cardtmp->product_name[0]; cardtmp++) {
        if ((strncmp(card->product_name, cardtmp->product_name,
            strlen(cardtmp->product_name)) == 0)
            || ((0 != strlen(cardtmp->manufacturer))
                && (NULL != strstr(card->manufacturer, cardtmp->manufacturer)))) {
            if (cardtmp->pre_init) {
                ret =cardtmp->pre_init(card);
                if ( ret < 0) {
                    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "pre_init failed\n");
                    goto ERR;
                }
            }
            break;
        }
    }

    printf("3g card information:\n"
        "   Manufacturer: %s\n"
        "   Product:      %s\n"
        "   pcui:   %s\n"
        "   device:   %s\n",
        card->manufacturer,
        card->product_name,
        card->pcui,
        card->device);

    return 0;

ERR:
    g3_pcui_close(card);
    return ret;
}

/******************************************************************************/
#if 0
int g3_card_get_all_pdp(g3_card *card)
{
    char *cmd_get_all_pdp = "AT+CGDCONT=?\r\n";
    char readbuf[1024] = {0};
    int ret;

    G3_ASSERT(card);

    ret = g3_card_send_cmd(cmd_get_all_pdp,
                   strlen(cmd_get_all_pdp));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);

    if (ret != 0)
        return ret;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get all pdp reply1:%s\n", readbuf);

    return 0;

}
#endif

/******************************************************************************/

static int cmd_parse_current_pdp(char *readbuf, g3_pdp *pdp)
{
    char pdp_buf[6][32] = {{0}};
    char error_code[8] = {0};
    char *str_op;
    char *strend;
    char *stopstr;
    char *outer_ptr = NULL;
    char *str;
    char *delim;
    int ix = 0;
    int len = 0;
    int error;

    G3_ASSERT((readbuf != NULL) && (pdp != NULL));

    if ((str = strstr(readbuf, "\r\n+CGDCONT: ")) != NULL &&
       ((strend = strstr(readbuf, "\r\nOK\r\n")) != NULL)) {
        readbuf = str + strlen("\r\n+CGDCONT: ");
        len = strend - readbuf;
        readbuf[len] = '\0';

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "readbuf=%s\n", readbuf);

        delim = ",";
        str_op = strtok_r(readbuf, delim, &outer_ptr);
        while (str_op && ix < 6) {
            snprintf(pdp_buf[ix],
                 sizeof(pdp_buf[ix]), "%s", str_op);

            str_op = strtok_r(NULL, delim, &outer_ptr);
            squeeze(pdp_buf[ix], '"');

            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                  "pdp_buf[%d]=%s\n",
                  ix, pdp_buf[ix]);

            ix ++;
        }
        if (ix == 6) {
            snprintf(pdp->cid,
                sizeof(pdp->cid),
                "%s", pdp_buf[0]);
            snprintf(pdp->pdp_type,
                sizeof(pdp->pdp_type),
                "%s", pdp_buf[1]);
            snprintf(pdp->apn,
                sizeof(pdp->apn),
                "%s", pdp_buf[2]);
            snprintf(pdp->pdp_ipaddr,
                sizeof(pdp->pdp_ipaddr),
                "%s", pdp_buf[3]);
            snprintf(pdp->d_comp,
                sizeof(pdp->d_comp),
                "%s", pdp_buf[4]);
            snprintf(pdp->h_comp,
                sizeof(pdp->h_comp),
                "%s", pdp_buf[5]);


            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                 "current pdp:\n"
                 "  cid:             %s\n"
                 "  pdp_type:        %s\n"
                 "  apn:             %s\n"
                 "  pdp_ipaddr:      %s\n"
                 "  d_comp:          %s\n"
                 "  h_comp:          %s\n"
                 "++++ get current pdp successfully!+++\n",
                 pdp->cid,
                 pdp->pdp_type,
                 pdp->apn,
                 pdp->pdp_ipaddr,
                 pdp->d_comp,
                 pdp->h_comp);

                 return 0;
        }
    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "get current pdp reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

int g3_card_get_current_pdp(g3_card *card, g3_pdp *pdp)
{
    char *cmd_get_pdp = "AT+CGDCONT?\r\n";
    char readbuf[128] = {0};
    int ret;

    G3_ASSERT(card);

    ret = g3_card_send_cmd(cmd_get_pdp,
                   strlen(cmd_get_pdp));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);
    if (ret != 0)
        return ret;

    ret = cmd_parse_current_pdp(readbuf, pdp);
    return ret;

}

/******************************************************************************/

int g3_card_set_pdp(g3_card *card, char *apn)
{
    char *cmd_set_pdp_prefix = "AT+CGDCONT=";
    char cmd_set_pdp[64] = {0};
    char *cid = "1";
    char *pdp_type = "\"IP\"";
    char *pdp_ipaddr = "\"\"";
    char *d_comp = "0";
    char *h_comp = "0";
    char readbuf[128] = {0};
    int ret;

    G3_ASSERT(card);
    snprintf(cmd_set_pdp,
         sizeof(cmd_set_pdp), "%s%s,%s,\"%s\",%s,%s,%s\r\n",
         cmd_set_pdp_prefix, cid, pdp_type,
         apn, pdp_ipaddr, d_comp, h_comp);

    G3_PRINTF(G3_TRACE_LEVEL_DBG, "cmd_set_pdp:%s\n", cmd_set_pdp);

    ret = g3_card_send_cmd(cmd_set_pdp,
                   strlen(cmd_set_pdp));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);
    if (ret != 0)
        return ret;

    if (strstr(readbuf, "\r\nOK\r\n") != NULL)
        return 0;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "set pdp reply:%s\n", readbuf);

    return EREPLY;
}


/******************************************************************************/

int g3_card_set_mode_acqorder(g3_card *card,
                  g3_card_mode_e mode,
                  g3_card_acqorder_e acqorder)
{
    int ret;
    char *cmd_prefix = "AT^SYSCFG=";
    char cmd_set_mode_acqorder[32] = {0};
    char readbuf[16] = {0};

    /* roam support:
     * 0: not support
     * 1: support
     * 2: no change
     */
    int roam = 2;

    /* srvdomain support:
     * 0: CS only
     * 1: PS only
     * 2: CS and PS
     * 3: ANY
     * 4: no change
     */
    int srvdomain = 4;


    G3_ASSERT((card != NULL)
           && (mode < G3_CARD_MODE_BOTT)
           && (acqorder < G3_CARD_ACQORDER_BOTT));

    snprintf(cmd_set_mode_acqorder,
         sizeof(cmd_set_mode_acqorder), "%s%d,%d,%x,%d,%d\r\n",
         cmd_prefix, mode, acqorder, 0x3fffffff, roam, srvdomain);

    G3_PRINTF(G3_TRACE_LEVEL_DBG,
          "cmd_set_mode_acqorder=%s\n", cmd_set_mode_acqorder);

    ret = g3_card_send_cmd(cmd_set_mode_acqorder,
                   strlen(cmd_set_mode_acqorder));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);
    if (ret != 0)
        return ret;

    if (strstr(readbuf, "\r\nOK\r\n") != NULL)
        return 0;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "set mode and acqorder reply:%s\n", readbuf);

    return EREPLY;
}

/******************************************************************************/

static int cmd_parse_sim_status(char *readbuf,
                g3_card_status_e *status)
{
    char error_code[8] = {0};
    g3_card_sim_status sim_status;
    char *str;
    char *strend;
    char *strstop;
    unsigned int len;
    int error;

    G3_ASSERT((readbuf != NULL) && (status != NULL));

    G3_PRINTF(G3_TRACE_LEVEL_DBG, "get sim status reply:%s\n", readbuf);

    if ((str = strstr(readbuf, "\r\n+CPIN:")) != NULL) {

        str += strlen("\r\n+CPIN:");

        if ((strend = strstr(str, "\r\nOK\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            G3_PRINTF(G3_TRACE_LEVEL_DBG, "str=%s\n", str);

            snprintf(sim_status.status_code,
                sizeof(sim_status.status_code),
                "%s", str);

            if (strncmp(sim_status.status_code, "READY", 5) == 0)
                *status = G3_CARD_STATUS_DISCONNECTED;
            else if (strncmp(sim_status.status_code, "SIM PIN", 7) == 0)
                *status = G3_CARD_STATUS_SIM_PIN;
            else if (strncmp(sim_status.status_code, "SIM PUK", 7) == 0)
                *status = G3_CARD_STATUS_SIM_PUK;
            else if (strncmp(sim_status.status_code, "SIM PIN2", 8) == 0)
                *status = G3_CARD_STATUS_SIM_PIN2;
            else if (strncmp(sim_status.status_code, "SIM PUK2", 8) == 0)
                *status = G3_CARD_STATUS_SIM_PUK2;
            else
                *status = G3_CARD_STATUS_UNAVALIABLE;
            return 0;
        }
    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &strstop, 10);
            G3_PRINTF(G3_TRACE_LEVEL_DBG, "ERROR: %d\n", error);
            *status = G3_CARD_STATUS_UNAVALIABLE;
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get sim status reply:%s\n", readbuf);

    return EREPLY;
}

/******************************************************************************/

int g3_card_get_sim_status(g3_card *card, g3_card_status_e *status)
{
    char *cmd_get_status = "AT+CPIN?\r\n";
    char readbuf[64] = {0};
    int ret;

    G3_ASSERT((status != NULL) && (status != NULL));

    /* send get status cmd */
    ret = g3_card_send_cmd(cmd_get_status, strlen(cmd_get_status));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 3);
    if (ret != 0)
        return ret;

    ret = cmd_parse_sim_status(readbuf, status);
    return ret;
}

/******************************************************************************/

int g3_card_get_conn_status(g3_card *card, g3_card_status_e *status)
{
    FILE *fp;
    char buf[128] = {0};
    char netif[8] = {0};
    char *str;

    G3_ASSERT((card != NULL) && (status != NULL));

#ifdef PPPD_INTERFACE_NUMBER
    snprintf(netif, sizeof(netif), "ppp%s", PPPD_INTERFACE_NUMBER);
#else
    snprintf(netif, sizeof(netif), "%s", "ppp0");
#endif
    fp = fopen(PPP_INTERFACE_PATH, "r+");
    if (fp == NULL) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR, "open(%s) failed!(%s)\n",
              PPP_INTERFACE_PATH, strerror(errno));
        return ESYS;
    }

    do {
        str = fgets(buf, sizeof(buf), fp);
        if (str && strstr(buf, netif) != NULL)
            break;
    } while (str != NULL);

    if (str)
        *status = G3_CARD_STATUS_CONNECTED;
    else
        *status = G3_CARD_STATUS_DISCONNECTED;

    G3_PRINTF(G3_TRACE_LEVEL_DBG,
          "card connect status=%d\n", *status);

    fclose(fp);
    return 0;
}

/******************************************************************************/

static int cmd_parse_all_operators(char *readbuf,
                g3_operator *operator,
                int maxnum,
                int *opnum)
{
    char single_operator[5][16] = {{0}};
    char error_code[8] = {0};
    char *str;
    char *strend;
    char *str_op;
    char *buf_op;
    char *stopstr;
    char *outer_ptr = NULL;
    char *inner_ptr = NULL;
    int oper_num = 0;
    int len;
    int error;
    int ix = 0;

    G3_ASSERT((readbuf != NULL)
          && (operator != NULL)
          && (maxnum > 0)
          && (maxnum <= MAX_OPERATOR_NUM));

    if ((str = strstr(readbuf, "\r\n+COPS: ")) != NULL) {
        str += strlen("\r\n+COPS: ");
        if ((strend = strstr(str, "\r\nOK\r\n")) != NULL) {
            if ((str_op = strstr(str, ",,")) != NULL) {
                strend = str_op;
            }

            len = strend - str;
            str[len] = '\0';
            char *delim_op = "()";
            str_op = strtok_r(str, delim_op, &outer_ptr);
            char *single_op;

            while (str_op && oper_num < maxnum) {
                G3_PRINTF(G3_TRACE_LEVEL_DBG,
                      "str_op=%s\n", str_op);
                /* skip "," between "(),()" */
                if (strncmp(str_op, ",", 1) == 0) {
                    str_op = strtok_r(NULL, delim_op,
                             &outer_ptr);
                    continue;
                }
                ix = 0;
                buf_op = str_op;
                char *delim_single_op = ",";

                single_op = strtok_r(buf_op,
                             delim_single_op,
                             &inner_ptr);
                while (single_op && ix < 5) {
                    snprintf(single_operator[ix],
                         sizeof(single_operator[ix]),
                         "%s", single_op);
                    single_op = strtok_r(NULL,
                               delim_single_op,
                               &inner_ptr);
                    squeeze(single_operator[ix], '"');
                    G3_PRINTF(G3_TRACE_LEVEL_DBG,
                          "single_operator[%d]=%s\n",
                          ix, single_operator[ix]);
                    ix ++;
                }
                snprintf(operator[oper_num].netstat,
                    sizeof(operator->netstat),
                    "%s", single_operator[0]);
                snprintf(operator[oper_num].long_operator,
                    sizeof(operator->long_operator),
                    "%s", single_operator[1]);
                snprintf(operator[oper_num].short_operator,
                    sizeof(operator->short_operator),
                    "%s", single_operator[2]);
                snprintf(operator[oper_num].numeric_operator,
                    sizeof(operator->numeric_operator),
                    "%s", single_operator[3]);
                snprintf(operator[oper_num].rat,
                    sizeof(operator->rat),
                    "%s", single_operator[4]);
                str_op = strtok_r(NULL, delim_op, &outer_ptr);
                oper_num ++;
            }

            for (ix = 0; ix < oper_num; ix ++) {
                G3_PRINTF(G3_TRACE_LEVEL_DBG,
                     "find %d operator(s):\n"
                     "operator[%d]:\n"
                     "  netstat:           %s\n"
                     "  long_operator:     %s\n"
                     "  short_operator:    %s\n"
                     "  numeric_operator:  %s\n"
                     "  rat:               %s\n"
                     "++get all operators successfully!++\n",
                     oper_num, ix,
                     operator[ix].netstat,
                     operator[ix].long_operator,
                     operator[ix].short_operator,
                     operator[ix].numeric_operator,
                     operator[ix].rat);
            }
            *opnum = oper_num;
        }
    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }
    if (*opnum > 0)
        return 0;

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "get all operators reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

int g3_card_get_all_operators(g3_card *card,
                  g3_operator *operator,
                  int maxnum,
                  int *opnum)
{
    char *cmd = "AT+COPS=?\r\n";
    char readbuf[1024] = {0};
    int ret = -1;

    G3_ASSERT((card != NULL)
          && (operator != NULL)
          && (maxnum > 0)
          && (maxnum <= MAX_OPERATOR_NUM));

    ret = g3_card_send_cmd(cmd, strlen(cmd));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 20);
    if (ret != 0)
        return ret;

    ret = cmd_parse_all_operators(readbuf, operator, maxnum, opnum);
    return ret;
}

/******************************************************************************/

int g3_card_register_operator(g3_card *card,
                  g3_card_cops_mode_e cops_mode,
                  g3_card_cops_op_format_e cops_format,
                  char *cops_op)
{
    char *cmd_prefix = "AT+COPS=";
    char cmd_register_operator[64] = {0};
    char readbuf[32] = {0};
    char error_code[8] = {0};
    char *str;
    char *strend;
    char *stopstr;
    int ret;
    int error;
    int len;

    G3_ASSERT((card != NULL)
           && (cops_mode < G3_CARD_COPS_MODE_BOTT)
           && (cops_format < G3_CARD_COPS_OP_FORMAT_BOTT)
           && (cops_op != NULL));

    switch (cops_mode) {
        case G3_CARD_COPS_MODE_AUTO:
            snprintf(cmd_register_operator,
                 sizeof(cmd_register_operator), "%s%d\r\n",
                 cmd_prefix, cops_mode);
            break;
        default:
            snprintf(cmd_register_operator,
                 sizeof(cmd_register_operator),
                 "%s%d,%d,%s\r\n",
                 cmd_prefix, cops_mode,
                 cops_format, cops_op);
            break;
    }

    ret = g3_card_send_cmd(cmd_register_operator,
                   strlen(cmd_register_operator));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 5);
    if (ret != 0)
        return ret;

    G3_PRINTF(G3_TRACE_LEVEL_DBG, "register operator reply:%s\n", readbuf);

    if (NULL != (strstr(readbuf, "\r\nOK\r\n")))
        return 0;
    else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "register operator reply:%s\n", readbuf);

    return EREPLY;
}

/******************************************************************************/

static int cmd_parse_current_operators(char *readbuf,
                g3_current_operator *current_operator)
{
    char operator_buf[4][16] = {{0}};
    char error_code[8] = {0};
    char *str_op;
    char *strend;
    char *stopstr;
    char *outer_ptr = NULL;
    char *str;
    char *delim;
    int ix = 0;
    int len = 0;
    int error;

    G3_ASSERT((readbuf != NULL) && (current_operator != NULL));

    if ((str = strstr(readbuf, "\r\n+COPS: ")) != NULL &&
       ((strend = strstr(readbuf, "\r\nOK\r\n")) != NULL)) {
        readbuf = str + strlen("\r\n+COPS: ");
        len = strend - readbuf;
        readbuf[len] = '\0';

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "buf=%s\n", readbuf);

        delim = ",";
        str_op = strtok_r(readbuf, delim, &outer_ptr);
        while (str_op && ix < 4) {
            snprintf(operator_buf[ix],
                 sizeof(operator_buf[ix]),
                 "%s", str_op);

            str_op = strtok_r(NULL, delim, &outer_ptr);
            squeeze(operator_buf[ix], '"');

            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                  "operator_buf[%d]=%s\n",
                  ix, operator_buf[ix]);

            ix ++;
        }
        if (ix == 4) {
            snprintf(current_operator->netmode,
                sizeof(current_operator->netmode),
                "%s", operator_buf[0]);
            snprintf(current_operator->operatorformat,
                sizeof(current_operator->operatorformat),
                "%s", operator_buf[1]);
            snprintf(current_operator->operatorinfo,
                sizeof(current_operator->operatorinfo),
                "%s", operator_buf[2]);
            snprintf(current_operator->rat,
                sizeof(current_operator->rat),
                "%s", operator_buf[3]);


            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                 "current operator:\n"
                 "  netmode:         %s\n"
                 "  operatorformat:  %s\n"
                 "  operatorinfo:    %s\n"
                 "  rat:             %s\n"
                 "++++ get current operator successfully!+++\n",
                 current_operator->netmode,
                 current_operator->operatorformat,
                 current_operator->operatorinfo,
                 current_operator->rat);

                 return 0;
        }
    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "get current operator reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

int g3_card_get_current_operator(g3_card *card,
             g3_current_operator *current_operator)
{
    char *cmd = "AT+COPS?\r\n";
    char readbuf[128] = {0};
    int ret = -1;

    G3_ASSERT((card != NULL) && (current_operator != NULL));

    ret = g3_card_send_cmd(cmd, strlen(cmd));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 5);
    if (ret != 0)
        return ret;

    ret = cmd_parse_current_operators(readbuf, current_operator);
    return ret;
}

/******************************************************************************/

int g3_card_get_quality(g3_card *card, int *rssi, int *ber)
{

    char readbuf[64] = {0};
    char rssi_ber[2][8] = {{0}};
    char error_code[8];
    char *str;
    char *strend;
    char *delim;
    char *stopstr = NULL;
    char *outer_ptr = NULL;
    char *cmd = "AT+CSQ\r\n";
    int ret = -1;
    int error;
    int len;
    int ix = 0;

    G3_ASSERT((card != NULL) && (rssi != NULL) && (ber != NULL));

    ret = g3_card_send_cmd(cmd, strlen(cmd));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    if (ret != 0)
        return ret;

    if ((str = strstr(readbuf, "\r\n+CSQ:")) != NULL &&
       ((strend = strstr(readbuf, "\r\nOK\r\n")) != NULL)) {
        str += strlen("\r\n+CSQ:");

        len = strend - str;
        str[len] = '\0';

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "buf=%s\n", str);

        delim = ",";
        str = strtok_r(str, delim, &outer_ptr);
        while (str && ix < 2) {
            snprintf(rssi_ber[ix],
                 sizeof(rssi_ber[ix]), "%s", str);

            G3_PRINTF(G3_TRACE_LEVEL_DBG, "rssi_ber[%d]=%s\n",
                  ix, rssi_ber[ix]);

            ix ++;
            str = strtok_r(NULL, delim, &outer_ptr);
        }
        if (ix == 2) {
            *rssi = strtol(rssi_ber[0], &stopstr, 10);
            *ber = strtol(rssi_ber[1], &stopstr, 10);

            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                 "get quality:\n"
                 "  rss      %d\n"
                 "  ber:     %d\n"
                 "+++++ get quality successfully! ++\n",
                 *rssi, *ber);

            return 0;
        }

    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get quality reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

static int cmd_parse_get_dataflow(char *readbuf, g3_card_flow *flow)
{
    char dataflowbuf[6][18];
    char error_code[8];
    char *stopstr;
    char *strend;
    char *outer_ptr = NULL;
    char *str;
    char *delim;
    int error;
    int len;
    int ix = 0;

    G3_ASSERT((readbuf != NULL) && (flow != NULL));

    if ((str = strstr(readbuf, "\r\n^DSFLOWQRY:")) != NULL &&
       ((strend = strstr(readbuf, "\r\n\r\nOK\r\n")) != NULL)) {
        readbuf = str + strlen("\r\n^DSFLOWQRY:");
        len = strend - readbuf;
        readbuf[len] = '\0';

        G3_PRINTF(G3_TRACE_LEVEL_DBG, "buf=%s\n", readbuf);

        delim = ",";
        str = strtok_r(readbuf, delim, &outer_ptr);
        while (str && ix < 6) {
            snprintf(dataflowbuf[ix], sizeof(dataflowbuf[ix]), "%s", str);

            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                  "dataflowbuf[%d]=%s\n", ix, dataflowbuf[ix]);

            str = strtok_r(NULL, delim, &outer_ptr);
            ix ++;
        }
        if (ix < 6) {
            G3_PRINTF(G3_TRACE_LEVEL_ERROR, "get all operators cmd "
                  "failed:(%s)\n", readbuf);
        }

        if (ix == 6) {
            flow->lastdstime = strtol(dataflowbuf[0], &stopstr, 16);
            flow->lasttxflow = strtol(dataflowbuf[1], &stopstr, 16);
            flow->lastrxflow = strtol(dataflowbuf[2], &stopstr, 16);
            flow->totaltxflow = strtol(dataflowbuf[3], &stopstr, 16);
            flow->totaldstime = strtol(dataflowbuf[4], &stopstr, 16);
            flow->totalrxflow = strtol(dataflowbuf[5], &stopstr, 16);

            G3_PRINTF(G3_TRACE_LEVEL_DBG,
                 "get dataflow:\n"
                 "  lastdstime:          %lx\n"
                 "  lasttxflow:          %llx\n"
                 "  lastrxflow:          %llx\n"
                 "  totaltxflow:         %llx\n"
                 "  totaldstime:         %lx\n"
                 "  totalrxflow:         %llx\n"
                 "+++++ get dataflow successfully! +++++++\n",
                 flow->lastdstime,
                 flow->lasttxflow,
                 flow->lastrxflow,
                 flow->totaltxflow,
                 flow->totaldstime,
                 flow->totalrxflow);

            return 0;
        }
    } else if ((str = strstr(readbuf, "+CME ERROR:")) != NULL) {
        str += strlen("+CME ERROR:");
        if ((strend = strstr(str, "\r\n")) != NULL) {
            len = strend - str;
            str[len] = '\0';
            snprintf(error_code, sizeof(error_code), "%s", str);
            error = strtol(error_code, &stopstr, 10);
            return error;
        }
    }

    G3_PRINTF(G3_TRACE_LEVEL_ERROR,
          "get data flow reply:%s\n", readbuf);
    return EREPLY;
}

/******************************************************************************/

int g3_card_get_dataflow(g3_card *card, g3_card_flow *flow)
{
    char readbuf[128];
    char *cmd = "AT^DSFLOWQRY\r\n";
    int ret = -1;

    G3_ASSERT((card != NULL) && (flow != NULL));

    ret = g3_card_send_cmd(cmd, strlen(cmd));
    if (ret != 0)
        return ret;

    ret = g3_card_get_cmd_result(readbuf, sizeof(readbuf), 1);
    if (ret != 0)
        return ret;

    ret = cmd_parse_get_dataflow(readbuf, flow);
    return ret;
}

/******************************************************************************/
static int g3_modem_get_result(int fd, char *buf, int buflen, int timeout)
{
    int ret = 0;
    int retry = 0;

    tcflush(fd, TCOFLUSH);

    memset(buf, 0, buflen);

    while(retry <= timeout) {
        ret = pcui_read(fd, buf, buflen - 1);
        buf[buflen - 1] = '\0';
        if ((ret <= 0)
           || (strstr(buf, "ERROR") == NULL
           && strstr(buf, "\r\nOK\r\n") == NULL)) {
            usleep(50000);
            retry ++;
        } else
            break;

    }

    if ((timeout > 0) && (retry > timeout)) {
        return ETIMEOUT;
    }
    tcflush(fd, TCIOFLUSH);

    return 0;
}
/******************************************************************************/

static int g3_modem_scan(const char *port_name)
{
    int ret;
    int fd;
    struct termios newtio, tio_saved;
    char file_name[16];
    char *at_cmd = "AT\r\n";
    char rcv_buf[1024];

    snprintf(file_name, sizeof(file_name), "/dev/%s", port_name);

    /* 1. open */
    fd = open(file_name, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        HI_ERR_3G("open %s failed!(%s)\n", file_name, strerror(errno));
        return ESYS;
    }

    /* 2. backup */
    ret = tcgetattr(fd, &tio_saved);
    if (ret) {
        HI_ERR_3G("save attr failed!(%s)\n", strerror(errno));
        close(fd);
        fd = -1;
        return ESYS;
    }

    /* 3. new attr */
    bzero(&newtio, sizeof(newtio) );
    newtio.c_cflag |= CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_cflag &= ~PARENB;
    cfsetispeed(&newtio, B115200);
    cfsetospeed(&newtio, B115200);
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;

    tcflush(fd, TCIOFLUSH);
    ret = tcsetattr(fd, TCSANOW, &newtio);
    if (ret) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "set attr failed!(%s)\n", strerror(errno));
        tcsetattr(fd, TCSANOW, &tio_saved);
        close(fd);
        fd = -1;
        return ESYS;
    }

    /* 4. send AT */
    ret = pcui_write(fd, at_cmd, strlen(at_cmd));
    if(ret != (int)strlen(at_cmd)) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "send command(%s) failed!\n", at_cmd);
        tcsetattr(fd, TCSANOW, &tio_saved);
        close(fd);
        fd = -1;
        return ESYS;
    }

    /* 5. get result */
    ret = g3_modem_get_result(fd, rcv_buf, sizeof(rcv_buf), 1);
    if (ret) {
        tcsetattr(fd, TCSANOW, &tio_saved);
        close(fd);
        return ESYS;
    }

    if (NULL == strstr(rcv_buf, "\r\nOK\r\n")) {
        tcsetattr(fd, TCSANOW, &tio_saved);
        close(fd);
        return ESYS;
    }

    tcsetattr(fd, TCSANOW, &tio_saved);
    close(fd);

    HI_ERR_3G("g3_modem_scan, [%s] can use!\n", file_name);
    return 0;
}
/******************************************************************************/

static int fileselect(const struct dirent *e)
{
    return !strncmp(e->d_name, "ttyUSB", 6);
}
/******************************************************************************/

static int filesort(const struct dirent **e1, const struct dirent **e2)
{
    const char *p1, *p2;
    int diff;

    for (p1=(*e1)->d_name, p2=(*e2)->d_name; *p1 || *p2; p1++, p2++) {
        if (!isdigit(*p1) || !isdigit(*p2)) {
            diff = *p1 - *p2;
            if (diff)
                return (int)diff;
        } else {
            return atoi(p1) - atoi(p2);
        }
    }
    return 0;
}
/******************************************************************************/

static int g3_card_scan_usb(g3_card *card, int maxnum, int *cardnum)
{
    int ret = -1;
    int ix, j = 0, num = 0;
    char *modem[2] = {card->device, card->pcui};
    struct dirent **namelist;

    G3_ASSERT((card != NULL) && (maxnum <= MAX_CARD_NUM));

    num = scandir("/dev", &namelist, fileselect, filesort);
    if (num < 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "scan ttyUSB* failed!(%s)\n", strerror(errno));
        return ESYS;
    }

    /* test AT */
    for (ix = 0; (ix < num) && (j < 2); ix++) {
        ret = g3_modem_scan(namelist[ix]->d_name);

        if (0 == ret)
            snprintf(modem[j++], sizeof(card->device), "/dev/%s", namelist[ix]->d_name);

        free(namelist[ix]);
    }
    free(namelist);

    if (j < 2) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR, "find no card!\n");
        return ENOCARD;
    }
    *cardnum = 1;

    HI_ERR_3G("find 1 g3 card, modem:%s, pcui:%s.\n", card->device, card->pcui);
    return 0;
}
/******************************************************************************/

static int g3_card_scan_uart(g3_card *card, int maxnum, int *cardnum, char *port)
{
    G3_ASSERT((card != NULL) && (port != NULL) && (maxnum <= MAX_CARD_NUM));

    /* open uart */
    snprintf(card->device, sizeof(card->device), "%s", port);
    snprintf(card->pcui, sizeof(card->pcui), "%s", port);

    *cardnum = 1;

    return 0;
}
/******************************************************************************/

int g3_card_scan(g3_card *card, int maxnum, int *cardnum, char *port)
{
    /* re-assign 0 to fd_pcui, avoid init fail after init deinit  many times. */
    fd_pcui = 0;

    if (card->etype == G3_CARD_PORT_TYPE_UART)
        return g3_card_scan_uart(card, maxnum, cardnum, port);
    else
        return g3_card_scan_usb(card, maxnum, cardnum);
}
/******************************************************************************/

static int generic_dialer_script(char *dialer_script, char *telephone) {

    int ret = -1;
    int fd;
    char buf[1024] = {0};
    char *filepath;
    char *dup_dialer_script;
    mode_t mode;

    G3_ASSERT((dialer_script != NULL) && (telephone != NULL));

    char *dialer_cript_cmd = "#!/bin/sh\n"
        "#\n"
        "# This is part 2 of the ppp-on script. "
        "It will perform the connection\n"
        "# protocol for the desired conn  ection.\n"
        "# Modify: jiangyanfeng@gmail.com\n"
        "#\n"
        "TELEPHONE=%s\n"
#if defined (ANDROID)
        "/system/bin/busybox chat -v                                \\\n"
#else
        "/usr/local/sbin/chat -v                                \\\n"
#endif
        "TIMEOUT         40                                     \\\n"
        "ABORT           '\\nBUSY\\r'                           \\\n"
        "ABORT           '\\nNO ANSWER\\r'                      \\\n"
        "ABORT           '\\nNO CARRIER\\r'                     \\\n"
        "ABORT           '\\nRINGING\\r\\n\\r\\nRINGING\\r'     \\\n"
        "ABORT           '\\nUsername/Password Incorrect\\r'    \\\n"
        "SAY             \"Beginning...\\n\"                    \\\n"
        "''              AT                                     \\\n"
        "''              ATH0                                   \\\n"
        "SAY             \"Dialing up...$TELEPHONE\\n\"         \\\n"
        "OK              ATD$TELEPHONE                         \\\n"
        "CONNECT         \\c                                    \\\n"
        "SAY             \"Logging...\\n\"                      \\\n";

    dup_dialer_script = strdup(dialer_script);

    filepath = dirname(dup_dialer_script);
    if (NULL == filepath) {
            G3_PRINTF(G3_TRACE_LEVEL_ERROR,
                  "dirname failed!(%s)\n", strerror(errno));
            free(dup_dialer_script);
            return ESYS;
    }

    if (access(filepath, 0) < 0) {
        mode = S_IRUSR | S_IWUSR;
        ret = mkdir(filepath, mode);
        if (ret < 0) {
            G3_PRINTF(G3_TRACE_LEVEL_ERROR,
                  "mkdir failed!(%s)\n", strerror(errno));
            free(dup_dialer_script);
            return ESYS;
        }
    }

    mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP
        | S_IXGRP | S_IROTH | S_IXOTH;
    fd = open(dialer_script, O_RDWR | O_CREAT | O_TRUNC, mode);
    if (fd < 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "open %s failed!(%s)\n",
              dialer_script, strerror(errno));
        free(dup_dialer_script);
        return ESYS;
    }
    snprintf(buf, sizeof(buf), dialer_cript_cmd, telephone);
    ret = write(fd, buf, strlen(buf));
    close(fd);
    if (ret < 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "write %s failed!(%s)\n",
              dialer_script, strerror(errno));
        free(dup_dialer_script);
        return ESYS;
    }

    return 0;
}

/******************************************************************************/
#if defined (ANDROID)
static int dirfilter(const struct dirent *e)
{
    if(!e || !e->d_name)
		return 0;
	if(*(e->d_name) >= '0' && *(e->d_name) <= '9')
		return 1;
	return 0;
}

static long detect_process(char *name)
{
	struct dirent **dirlist;
	int i;
	char process[256] = {0};
	FILE *fp = NULL;
	char *pget = NULL;
	char readbuf[1024] = {0};
	char pid[32] = {0};
#define PROCESS_FOUND  0x1234
	int found = 0;

	if(!name) {
		HI_ERR_3G("detect_process invalid param");
		return -1;
	}

	int ndir = scandir("/proc", &dirlist, dirfilter, alphasort);

	for(i=0; i<ndir; i++) {
		snprintf(process, sizeof(process), "/proc/%s/stat", dirlist[i]->d_name);
		free(dirlist[i]);

		/* must finish the loop to free dirlist[i] */
		if(found == PROCESS_FOUND)
		    continue;

		fp = fopen(process, "r");
		if(!fp) {
			HI_ERR_3G("fopen %s failed\n", process);
			fp = NULL;
			continue;
		}

		while((pget = fgets(readbuf, sizeof(readbuf), fp)) != NULL) {
			if(sscanf(readbuf, "%[^ ] (%[^)]", pid, process) == 2) {
				if(!strcmp(name, process)) {
					HI_ERR_3G("%s is alive, pid :%s\n", name , pid);
					found = PROCESS_FOUND;
				}
			}
		}

		if(fp) {
			fclose(fp);
			fp = NULL;
		}
	}
	free(dirlist);
	if(found == PROCESS_FOUND) 
		return strtol(pid, NULL, 10);

	HI_ERR_3G("process %s not found\n", name);
	return -1;
}
#endif

int g3_card_connect(g3_card *card,
            char *account,
            char *password,
            char *telephone,
            int  argvnum,
            char *argv[])
{
    int ret = -1;
    int status;
    int ix = 0;
    int jx = 0;
    char *cmd = PPPD_CMD_PATH;
    char *device = card->device;
    char buf[128];
    char buf_netdev[128];
    pid_t pid = 0;
    int inet_sock;
    struct ifreq ifr;

    char *str;
    FILE *fp;

    G3_ASSERT(card != NULL);

    if (card->etype == G3_CARD_PORT_TYPE_UART) { /* avoid reconnect fail */
        close(fd_pcui);
        fd_pcui= open(card->pcui, O_RDWR | O_NOCTTY | O_NDELAY);
        set_pcui(115200, 8, 'N', 1);
    }

    static char *pppd_argv[MAX_PPPD_ARGS] = {
        "pppd",
        "debug",
        "crtscts",
        "modem",
#define ARG_TTY_DEV                  4
        "/dev/ttyUSB0",
        "115200",
        "noauth",
        "remotenumber",
#define TELEPHONE                    8
        "*99#",
        "usepeerdns",
        "noipdefault",
        "defaultroute",
        "name",
#define ARG_ACCOUNT                  13
        "",
        "password",
#define ARG_PASSWORD                 15
        "",
        "unit",
#define ARG_UNIT_NUMBER              17
        "0",
        "connect",
#define ARG_DIALER_SCRIPT            19
#if defined (ANDROID)
        "\"sh /data/ppp/huawei-ppp-on-dialer\"",
#else
        "\"sh /tmp/ppp/huawei-ppp-on-dialer\"",
#endif
#define ARG_OPTIONAL                 PPPD_OPTIONAL_ARG_START
    };

    if (device) /*lint !e774 ignore by y00183183, just for sure device is always valid */
        pppd_argv[ARG_TTY_DEV]  = device;
    else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "ttydev is not specified, use default ttydev(%s)\n",
            pppd_argv[ARG_TTY_DEV]);
    if (telephone)
        pppd_argv[TELEPHONE]    = telephone;
    else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "telephone is not specified, "
              "use default telephone(%s)\n",
            pppd_argv[TELEPHONE]);
    if (account)
        pppd_argv[ARG_ACCOUNT]  = account;
    else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "account is not specified, "
              "use default account(%s)\n",
            pppd_argv[ARG_ACCOUNT]);
    if (password)
        pppd_argv[ARG_PASSWORD] = password;
    else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "password is not specified, "
              "use default password(%s)\n",
            pppd_argv[ARG_PASSWORD]);
#ifdef PPPD_INTERFACE_NUMBER
        pppd_argv[ARG_UNIT_NUMBER] = PPPD_INTERFACE_NUMBER;
#else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "pppd interface number is not specified, "
              "use default interface number(%s)\n",
            pppd_argv[ARG_UNIT_NUMBER]);
#endif
#ifdef PPPD_DIALER_SCRIPT
        snprintf(buf, sizeof(buf), "sh %s", PPPD_DIALER_SCRIPT);
        pppd_argv[ARG_DIALER_SCRIPT] = buf;
#else
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "dialer_script is not specified, "
              "use default dialer_script(%s)\n",
            pppd_argv[ARG_DIALER_SCRIPT]);
#endif

    /* Check if 3g card is already connected */
    fp = fopen(PPP_INTERFACE_PATH, "r+");
    if (fp == NULL) {
        HI_ERR_3G(
              "open(%s) failed!(%s)\n",
              PPP_INTERFACE_PATH, strerror(errno));
        return ESYS;
    }
    do {
        str = fgets(buf_netdev, sizeof(buf_netdev), fp);
        HI_ERR_3G(
              "fgets(%s):\n(%s)\n",
              PPP_INTERFACE_PATH, str);
        if (str && (strstr(str, "ppp0") != NULL)) {
            HI_ERR_3G(
                  "WARNING: "
                  "3g card is already connected!\n");
            fclose(fp);
            return 0;
        }
    } while (str != NULL);

    fclose(fp);

    for (ix = 0; ix < argvnum; ix++ )
        pppd_argv[ix + ARG_OPTIONAL] = argv[ix];

    if (ix > MAX_PPPD_ARGS - ARG_OPTIONAL - 1) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR, "too many args(%d) for pppd\n",
            ix + ARG_OPTIONAL);
        return EINVAL;
    }
    pppd_argv[ix + ARG_OPTIONAL] = NULL;

    for (jx = 0; jx < ix + ARG_OPTIONAL && pppd_argv[jx]; jx ++)
        HI_ERR_3G(
              "pppd_argv[%d]=%s\n", jx, pppd_argv[jx]);

    ret = generic_dialer_script(PPPD_DIALER_SCRIPT, pppd_argv[TELEPHONE]);
    if ((ret) < 0) {
        HI_ERR_3G(
              "generic_dialer_script %s failed\n",
              pppd_argv[ARG_DIALER_SCRIPT]);
        return ret;
    }

    pid = fork();
    if (pid < 0) {
        HI_ERR_3G(
              "fork failed!(%s)\n", strerror(errno));
        return ESYS;
    }

    if (!pid) {
        execv(cmd, pppd_argv);
        HI_ERR_3G(
              "execv cmd %s failed!(%s)\n", cmd, strerror(errno));
        exit(0);
    }

    if (waitpid(pid, &status, 0) < 0) {
        HI_ERR_3G(
              "waitpid(%d) failed!(%s)\n", pid, strerror(errno));
        return ESYS;
    }

    /* check pppd is already running */
    ix = 0;
    while (ix < 10) {
        fp = fopen(PPP_INTERFACE_PATH, "r+");
        if (fp == NULL) {
            HI_ERR_3G(
                  "open(%s) failed!(%s)\n",
                  PPP_INTERFACE_PATH, strerror(errno));
            return ESYS;
        }

        do {
            str = fgets(buf_netdev, sizeof(buf_netdev), fp);
            if (str) {
                HI_ERR_3G(
                      "fgets(%s):\n(%s)\n",
                      PPP_INTERFACE_PATH, str);
                if (strstr(str, "ppp0") != NULL)
                    break;
            }
        } while (str != NULL);

        if (str == NULL) {

            fclose(fp);
            sleep(2);

        } else
            break;
        ix ++;

    }
    if (ix >= 10) {
        HI_ERR_3G("check ppp0 failed!\n");
        return ESYS;
    }

    fclose(fp);
    snprintf(card->inf_unit, sizeof(card->inf_unit),
        "%s", pppd_argv[ARG_UNIT_NUMBER]);

    /* If ppp0 is allocated ip address, then connect success. */
#if defined (ANDROID)
#define G3_WAIT_IP_TIMEOUT 13
#else
#define G3_WAIT_IP_TIMEOUT 15
#endif
    ix = 0;
    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (inet_sock > 0) {
        snprintf(ifr.ifr_name, sizeof("ppp0"), "ppp0");
#if defined (ANDROID)
        while (ix < G3_WAIT_IP_TIMEOUT && detect_process("pppd") > 0) {
#else
         while (ix < G3_WAIT_IP_TIMEOUT ) {
#endif
            if(0 == ioctl(inet_sock, SIOCGIFADDR, &ifr))
                break;
            sleep(2);
            ix ++;
        }

        close(inet_sock);
    }

    if (ix >= G3_WAIT_IP_TIMEOUT) {
        HI_ERR_3G("check IP failed!\n");
        return ESYS;
    }

    return 0;
}

/******************************************************************************/

int g3_card_disconnect(g3_card *card)
{
    system("busybox killall pppd");
    HI_ERR_3G("busybox killall pppd");
    return 0;
#if 0
    char pppd_pid_path[32] = {0};
    char ppp_pid_buf[32] = {0};
    char cmd_kill[30] ={0};
    char *stopstr;
    FILE *fp;
    char *tmp;
    char buf[128] = {0};
    int pid = 0;
    int fd = 0;

    G3_ASSERT(card != NULL);

    snprintf(pppd_pid_path, sizeof(pppd_pid_path),
        "%s", PPPD_PID_FILE_PREFIX);

    snprintf(pppd_pid_path + strlen(pppd_pid_path),
        sizeof(pppd_pid_path) - strlen(pppd_pid_path),
        "%s", card->inf_unit);
    snprintf(pppd_pid_path + strlen(pppd_pid_path) ,
        sizeof(pppd_pid_path) - strlen(pppd_pid_path),
        "%s", PPPD_PID_FILE_SUFFIX);

    fp = fopen(PPP_INTERFACE_PATH, "r+");
    if (fp == NULL) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "open(%s) failed!(%s)\n",
              PPP_INTERFACE_PATH, strerror(errno));
        return ESYS;
    }
    do {
        tmp = fgets(buf, sizeof(buf), fp);
        G3_PRINTF(G3_TRACE_LEVEL_DBG,
              "fgets(%s):\n(%s)\n",
              PPP_INTERFACE_PATH, tmp);
        if (tmp && (strstr(tmp, "ppp0") != NULL)) {
            fd = open(pppd_pid_path, O_RDONLY);
            if (fd < 0) {
                G3_PRINTF(G3_TRACE_LEVEL_ERROR,
                      "open %s failed!(%s)\n",
                      pppd_pid_path, strerror(errno));
                fclose(fp);
                return ESYS;
            }
            break;
        }
    } while (tmp != NULL);

    if (tmp == NULL) {
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
              "WARNING: get ppp0.pid failed. "
              "3g card is already disconnected!\n");
        fclose(fp);
        return 0;
    }

    if (read(fd, ppp_pid_buf, sizeof(ppp_pid_buf) - 1) == 0) {
        G3_PRINTF(G3_TRACE_LEVEL_ERROR,
              "read %s failed!(%s)\n",
              pppd_pid_path, strerror(errno));
        close(fd);
        fclose(fp);
        return ESYS;
    }
    pid = strtol(ppp_pid_buf, &stopstr, 0);

    snprintf(cmd_kill, sizeof(cmd_kill), "kill %d", pid);
    system(cmd_kill);
    close(fd);
    fclose(fp);
    return 0;
#endif
}

/******************************************************************************/

int g3_card_deinit(g3_card *card)
{
    G3_ASSERT(card != NULL);

    if (fd_pcui <= 0) {
        G3_PRINTF(G3_TRACE_LEVEL_WARN,
             "fd_pcui invalid, card may be deinit already!\n");
        return 0;
    }

    g3_pcui_close(card);

    return 0;
}

