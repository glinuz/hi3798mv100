#ifndef __FV_ADAPTER_H__
#define __FV_ADAPTER_H__

int fv_is_first_enter(int *first);
void fv_clear_first_enter(void);
void fv_factory_reset(void);
#define MAX_VERSION_LEN	(64)
void fv_get_sdk_version(char *buf, int *sz);
void fv_get_app_version(char *buf, int *sz);
void fv_get_plat_version(char *buf, int *sz);
void fv_get_sn(char *buf, int *sz);

#endif

