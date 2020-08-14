#ifndef __FV_FACE_H__
#define __FV_FACE_H__

#include "fv_config.h"

int fv_face_on(void);

void fv_face_off(void);


int HI_FV_DrawText(enum fv_item_list item_type, char *buf, unsigned int sz);
int fv_get_win_handle(unsigned int *Win);
int fv_draw_vol(unsigned int vol);
void fv_destroy_win_handle(void);

#endif
