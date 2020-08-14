#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sta_ui.h"

struct eloop_sock {
	HI_S32 sock;
	HI_VOID *eloop_data;
	HI_VOID *user_data;
	sta_ui_sock_handler handler;
};

struct eloop_timeout {
//	struct dl_list list;
//	struct os_time time;
	HI_VOID *eloop_data;
	HI_VOID *user_data;
//	eloop_timeout_handler handler;
};

struct eloop_signal {
	HI_S32 sig;
	HI_VOID *user_data;
//	eloop_signal_handler handler;
	HI_S32 signaled;
};

struct eloop_sock_table {
	HI_S32 count;
	struct eloop_sock *table;
	HI_S32 changed;
};

struct eloop_data {
    HI_S32 max_sock;

//	struct eloop_sock_table readers;
//	struct eloop_sock_table writers;
//	struct eloop_sock_table exceptions;

//	struct dl_list timeout;

//	HI_S32 signal_count;
//	struct eloop_signal *signals;
//	HI_S32 signaled;
//	HI_S32 pending_terminate;

//	HI_S32 terminate;
//	HI_S32 reader_table_changed;
    struct eloop_sock ui_cmd_sock;
};

static HI_VOID *edit_cb_ctx;
static HI_S32 (*edit_cmd_cb)(HI_VOID *ctx, HI_PCHAR cmd);
static HI_VOID (*edit_eof_cb)(HI_VOID *ctx);

#define CMD_BUF_SIZE 256
static HI_CHAR cmdbuf[CMD_BUF_SIZE];
static HI_S32 cmdbuf_pos = 0;
static HI_S32 cmdbuf_len = 0;
static struct eloop_data eloop;

HI_S32 sta_ui_register_sock(HI_S32 sock, eloop_sock_type type,
			sta_ui_sock_handler handler,
			HI_VOID *eloop_data, HI_VOID *user_data)
{
    struct eloop_sock *tmp;

    if (type == TYPE_UI_CMD)
        tmp = &eloop.ui_cmd_sock;
    else
        return -1;

    tmp->sock = sock;
    tmp->eloop_data = eloop_data;
    tmp->user_data = user_data;
    tmp->handler = handler;

    eloop.max_sock += 1;
 
    return 0;
}


HI_VOID sta_ui_unregister_sock(HI_S32 sock, eloop_sock_type type)
{
    struct eloop_sock *tmp;

    if (type == TYPE_UI_CMD)
        tmp = &eloop.ui_cmd_sock;
    else
        return;

    memset(tmp, 0, sizeof(struct eloop_sock));

    eloop.max_sock -= 1;
}

HI_VOID edit_redraw(HI_VOID)
{
    HI_CHAR tmp;
    
    cmdbuf[cmdbuf_len] = '\0';
    printf("\r> %s", cmdbuf);
    if (cmdbuf_pos != cmdbuf_len) {
        tmp = cmdbuf[cmdbuf_pos];
        cmdbuf[cmdbuf_pos] = '\0';
        printf("\r> %s", cmdbuf);
        cmdbuf[cmdbuf_pos] = tmp;
    }
    fflush(stdout);
}

static HI_VOID process_cmd(HI_VOID)
{
    if (cmdbuf_len == 0) {
        printf("\n> ");
        fflush(stdout);
        return;
    }

    printf("\n");
    cmdbuf[cmdbuf_len] = '\0';
    cmdbuf_pos = 0;
    cmdbuf_len = 0;
    if (edit_cmd_cb)
    {
        if (0 == edit_cmd_cb(edit_cb_ctx, cmdbuf))
        {
            printf("OK\n");
        }
        else
        {
            printf("ERROR\n");
        }
    }
    printf("> ");
    fflush(stdout);
}

static HI_VOID insert_char(HI_S32 c)
{
    if (cmdbuf_len >= (HI_S32) sizeof(cmdbuf) - 1)
        return;

    if (cmdbuf_len == cmdbuf_pos) {
        cmdbuf[cmdbuf_pos++] = c;
        cmdbuf_len++;
        putchar(c);
        fflush(stdout);
    } else {
        memmove(cmdbuf + cmdbuf_pos + 1, cmdbuf + cmdbuf_pos,
                   cmdbuf_len - cmdbuf_pos);
        cmdbuf[cmdbuf_pos++] = c;
        cmdbuf_len++;
        edit_redraw();
    }
}

static enum edit_key_code edit_read_key(HI_S32 sock)
{
    HI_S32 c;
    HI_UCHAR buf[1];
    HI_S32 res;

    res = read(sock, buf, 1);
    if (res < 0)
        perror("read");
    if (res <= 0)
        return EDIT_KEY_EOF;

    c = buf[0];

    switch (c) {
        case 10:
            return EDIT_KEY_CTRL_J;
        case 13: /* CR */
            return EDIT_KEY_ENTER;
        case 127:
            return EDIT_KEY_BACKSPACE;
        default:
            return c;
    }
}

static HI_VOID sta_ui_read_char(HI_S32 sock, HI_VOID *eloop_ctx, HI_VOID *sock_ctx)
{
    enum edit_key_code c;

    c = edit_read_key(sock);

    switch (c) {
        case EDIT_KEY_EOF:
            edit_eof_cb(edit_cb_ctx);
            break;
        case EDIT_KEY_ENTER:
        case EDIT_KEY_CTRL_J:
            process_cmd();
            break;
        default:
            if (c >= 32 && c <= 255)
                insert_char(c);
            break;
    }
}

HI_S32 sta_ui_init(HI_S32 (*cmd_cb)(HI_VOID *ctx, HI_PCHAR cmd),
                      HI_VOID (*eof_cb)(HI_VOID *ctx),
                      HI_VOID *ctx)
{
    edit_cb_ctx = ctx;
    edit_cmd_cb = cmd_cb;
    edit_eof_cb = eof_cb;
	
    sta_ui_register_sock(STDIN_FILENO, TYPE_UI_CMD, sta_ui_read_char, NULL, NULL);

    return 0;
}

HI_VOID sta_ui_deinit(HI_VOID)
{
    sta_ui_unregister_sock(STDIN_FILENO, TYPE_UI_CMD);
}

HI_VOID sta_ui_run(HI_VOID)
{
    fd_set rfds, wfds;
    HI_S32 res;

    while (1) {
        FD_ZERO(&rfds);
        FD_ZERO(&wfds);
        FD_SET(eloop.ui_cmd_sock.sock, &rfds);

        res = select(eloop.max_sock +1, &rfds, 0, 0, 0);
        if (res < 0) {
           perror("select");
           break;
        }

        if (FD_ISSET(eloop.ui_cmd_sock.sock, &rfds)) {
            eloop.ui_cmd_sock.handler(eloop.ui_cmd_sock.sock,
                    eloop.ui_cmd_sock.eloop_data,
                    eloop.ui_cmd_sock.user_data);
        }

    }
}

