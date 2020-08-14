#ifndef __TUI_H
#define __TUI_H

#define TEE_TUI_AGENT_ID    0x54554944	/* TUID */

/* poll event type from normal to secure */
enum tui_poll_type {
	TUI_POLL_CFG_OK,
	TUI_POLL_CFG_FAIL,
	TUI_POLL_TP,
	TUI_POLL_TICK,
	TUI_POLL_DELAYED_WORK,
	TUI_POLL_PAUSE_TUI,
	TUI_POLL_RESUME_TUI,
/* For some reasons, we need a method to terminate TUI from no-secure
 * OS, for example the TUI CA maybe killed.
 */
	TUI_POLL_CANCEL,
	TUI_POLL_HANDLE_TUI,	/*for tui to handle event*/
	TUI_POLL_NAVI_H_TO_S,	/*for navigator hide and show*/
	TUI_POLL_NAVI_S_TO_H,
	TUI_POLL_SHS_0_TO_1,	/*for single hand mode switch*/
	TUI_POLL_SHS_0_TO_2,
	TUI_POLL_SHS_1_TO_0,
	TUI_POLL_SHS_2_TO_0,
	TUI_POLL_ROTATION_0,	/*for retation switch*/
	TUI_POLL_ROTATION_90,
	TUI_POLL_ROTATION_180,
	TUI_POLL_ROTATION_270,
	TUI_POLL_KEYBOARDTYPE_0,
	TUI_POLL_KEYBOARDTYPE_3,
	TUI_POLL_SEMITRANS,
	TUI_POLL_CURSOR,
	TUI_POLL_GETFP,
	TUI_POLL_MAX			/*Do Not add type behine this one*/
};

typedef int (*tui_drv_init) (void *pdata, int secure);

#define TUI_DRV_NAME_MAX	32

#ifdef CONFIG_TEE_TUI
extern int ts_tui_report_input(void *finger_data);
extern int tui_fp_notify(void);
int __init init_tui(struct device *dev);
void tui_exit(void);
int tui_send_event(int event);
int register_tui_driver(tui_drv_init fun, const char *name,
					void *pdata, int priority);
void unregister_tui_driver(const char *name);
/*
 * TUI has different state that can recieve given types of message,
 * there are 3 APIs to send message.
 * send_tui_msg_config:send message to TUI in config state only.
 */
int send_tui_msg_config(int type, int val, void *data);
void tui_poweroff_work_start(void);
int tui_get_keyCode(void);

void set_tui_attach_device(unsigned int id);
unsigned int tui_attach_device(void);
#else
static inline int init_tui(struct device *dev)
{
	return 0;
}

static inline void tui_exit(void)
{
}

static inline int register_tui_driver(tui_drv_init fun,
		const char *name, void *pdata, int priority)
{
	return 0;
}

static inline void unregister_tui_driver(const char *name)
{
}

static inline int send_tui_msg_config(int type, int val, void *data)
{
	return 0;
}

static inline void set_tui_attach_device(unsigned int id)
{
}

static inline unsigned int tui_attach_device(void)
{
	return 0;
}

#endif

#endif
