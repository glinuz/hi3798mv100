#ifndef __DRV_VDP_INTF__
#define __DEV_VDP_INTF__

#include "hi_tee_vdp.h"

#if 0
#define HI_PRINT uart_printf_func
#else
#define HI_PRINT
#endif
#define HI_ERR_PRINT uart_printf_func

int vdp_drv_Ioctl(int cmd ,void *arg);
#endif
