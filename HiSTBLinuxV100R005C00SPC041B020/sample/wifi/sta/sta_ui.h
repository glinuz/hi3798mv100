#ifndef __STA_UI_H__
#define __STA_UI_H__

#include "hi_type.h"

typedef enum {
	TYPE_UI_CMD = 0,
	TYPE_WPA_EVENT
} eloop_sock_type;

enum edit_key_code {
	EDIT_KEY_NONE = 256,
	EDIT_KEY_ENTER,
	EDIT_KEY_BACKSPACE,
	EDIT_KEY_CTRL_J,
	EDIT_KEY_EOF
};

typedef HI_VOID (*sta_ui_sock_handler)(HI_S32 sock, HI_VOID *eloop_ctx, HI_VOID *sock_ctx);

HI_S32 sta_ui_register_sock(HI_S32 sock, eloop_sock_type type, 
               sta_ui_sock_handler handler,
               HI_VOID *eloop_data, HI_VOID *user_data);

HI_VOID sta_ui_unregister_sock(HI_S32 sock, eloop_sock_type type);

HI_S32 sta_ui_init(HI_S32 (*cmd_cb)(HI_VOID *ctx, HI_PCHAR cmd),
                      HI_VOID (*eof_cb)(HI_VOID *ctx),
                      HI_VOID *ctx);
                      
HI_VOID sta_ui_deinit(HI_VOID);

HI_VOID sta_ui_run(HI_VOID);

#endif // __STA_UI_H__
