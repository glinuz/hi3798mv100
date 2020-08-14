#include "tee_internal_api.h"

int ss_crypto_use_cbc()
{
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
	return 1;
#else
	return 0;
#endif
}

