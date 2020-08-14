#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "fv_adapter.h"
#include "hi_flash.h"

int fv_is_first_enter(int *first)
{
	/* FIXME */
	*first = 0;
	return 0;
}

void fv_clear_first_enter(void)
{
	/* FIXME */
	return;
}

void fv_factory_reset(void)
{
	/* FIXME */
	return;
}

#ifdef CFG_HI_SDK_VERSION
#define MKSTR(exp) # exp
#define MKMARCOTOSTR(exp) MKSTR(exp)

static const HI_CHAR SDK_VERSION[]  = "SDK:["MKMARCOTOSTR(CFG_HI_SDK_VERSION) "]";
#else
static const HI_CHAR SDK_VERSION[]  = "SDK:[""N/A""]";
#endif

#ifndef APP_VERSION
#define APP_VERSION     "APP V0.1"
#endif

#ifndef PLATFORM_VERSION
#define PLATFORM_VERSION	"PLATFORM V0.1"
#endif

#ifndef PRODUCT_SN
#define PRODUCT_SN	"00.00.00.00.01"
#endif

void fv_get_sdk_version(char *buf, int*sz)
{
	int len;

	if (!buf || !sz)
		return;

	len = strlen(SDK_VERSION) + 1 > *sz ? *sz: strlen(SDK_VERSION);
	strncpy(buf, SDK_VERSION, len);
	buf[len]= '\0';

	*sz = len;
}

void fv_get_app_version(char *buf, int *sz)
{
	int len;

	if (!buf || !sz)
		return;

	len = strlen(APP_VERSION) + 1 > *sz ? *sz : strlen(APP_VERSION);
	strncpy(buf, APP_VERSION, len);
	buf[len]= '\0';

	*sz = len;
}

void fv_get_plat_version(char *buf, int *sz)
{
	int len;

	if (!buf || !sz)
		return;

	len = strlen(PLATFORM_VERSION) + 1  > *sz ? *sz : strlen(PLATFORM_VERSION);
	strncpy(buf, PLATFORM_VERSION, len);
	buf[len]= '\0';

	*sz = len;
}

void fv_get_sn(char *buf, int *sz)
{
	int len;

	if (!buf || !sz)
		return;

	len = strlen(PRODUCT_SN) + 1  > *sz ? *sz : strlen(PRODUCT_SN);
	strncpy(buf, PRODUCT_SN, len);
	buf[len]= '\0';

	*sz = len;

}
