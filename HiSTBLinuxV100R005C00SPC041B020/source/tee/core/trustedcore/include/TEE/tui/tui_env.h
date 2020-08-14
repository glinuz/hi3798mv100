#ifndef __UI_ENV_H__
#define __UI_ENV_H__

#include "tui_obj.h"
#include "tui_api.h"

enum ui_dialog_button_t {
	DB_TIMEOUT = -21,
	DB_BAD_BUTTON = -1,
	DB_CANCEL = (1 << 0),
	DB_OK = (1 << 1),
};
enum edit_action_t {
	EDIT_ACTION_INSERT,
	EDIT_ACTION_BACKSPACE,
	EDIT_ACTION_DELETE
};

struct ui_internal_event_t {
	struct ui_event_t event;
	struct ui_internal_event_t *next;
};

struct ui_obj_set_t {
	struct ui_obj_t **objs;
	int32_t         obj_count;
	int32_t         to_free;
	int32_t         dx, dy;
	int32_t         looping;       /* unused for IM set. */
};

struct ui_env_t {
	/* layer priority: IM > SI > app */
	struct win_info     win;
	struct ui_obj_set_t app_ui;
	struct ui_label_t  *SI;
	int32_t             to_free_SI;
	struct ui_obj_set_t IM_ui;
	int32_t             IM_type;
	/* runtime attributes: */
	int32_t             SI_showing;
	int32_t             IM_showing;
	int32_t             result;           /* for dialog */
	struct ui_obj_t    *focused_obj;      /* one obj in app_ui. */
	volatile struct ui_obj_t    *pressed_obj;      /* one obj in app_ui or IM_ui. */
	struct ui_internal_event_t  *internal_events;
	struct caret_info_t caret_info;
	int32_t font_type;
};
#define WIN_INFO_GIVEN(w)     ((w).screen_width>0 && (w).screen_height>0)
#define TOPLEVEL_OBJ(pp_uis)  ((pp_uis)[0])
extern int32_t keyboard_flag;
EXPORTED int
env_init(struct ui_env_t *env);

EXPORTED int
env_release(struct ui_env_t *env);

EXPORTED void
env_attach_ui(struct ui_env_t *env, struct ui_obj_t **uis, int32_t obj_count, int32_t free_when_done);

EXPORTED void
env_attach_SI(struct ui_env_t *env, struct ui_label_t *si, int32_t free_si_when_done);

EXPORTED void
env_attach_IM(struct ui_env_t *env, struct ui_obj_t **im_uis, int32_t im_obj_count);

EXPORTED void
env_attach_IM_keyboard(struct ui_env_t *env, struct ui_obj_t **im_uis, int32_t im_obj_count);

EXPORTED void
env_show_app_ui(struct ui_env_t *env);

EXPORTED void
env_show_SI(struct ui_env_t *env);

EXPORTED void
env_show_IM(struct ui_env_t *env);

EXPORTED void
env_hide_IM(struct ui_env_t *env);

EXPORTED int env_showup(struct ui_env_t *env);
EXPORTED void env_invalidate(struct ui_env_t *env);
EXPORTED void env_redraw_obj(struct ui_env_t *env, struct ui_obj_t *obj);
EXPORTED void env_set_focus(struct ui_env_t *env, uint32_t idx, uint32_t focused);
EXPORTED void env_event_loop_quit(struct ui_env_t *env);
EXPORTED void env_event_invalidate(struct ui_env_t *env);
EXPORTED void env_event_send_key(struct ui_env_t *env, int32_t key_val);
EXPORTED void env_event_loop(struct ui_env_t *env, int *type);
void env_keyboard_loop(struct ui_env_t *env, int *type,  struct screen_init_info  *keyboard);
int32_t env_status_init(struct ui_env_t *env);
int32_t on_im_button_click(void *e, struct ui_obj_t *clicked_obj);
int init_keyboard(struct ui_env_t *env, int type);

#endif // __UI_ENV_H__
