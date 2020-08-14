#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
#include "kirin/include/platform_kirin.h"
#endif

#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_BALONG_V722_PLATFORM)
#include "balong/include/platform_balong.h"
#endif

#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
#include "devchip/dispatch/platform_disp.h"
#endif

#endif /*__PLATFORM_H__ */
