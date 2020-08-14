#include "tee_internal_api.h"
/*lint -e749*/

enum rpmb_key_type {
	RPMB_KEY_UNKOWN = 0,
	RPMB_MANAGE_KEY,
	RPMB_DERIVE_KEY,
	RPMB_ACCESS_KEY
};

void tee_rpmb_get_key_type(uint32_t *key_type)
{
	if (key_type != NULL) {
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
#if (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650)
		*key_type = RPMB_DERIVE_KEY;
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250) || (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3660)
		*key_type = RPMB_ACCESS_KEY;
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3670)
		*key_type = RPMB_ACCESS_KEY;
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3635)
		*key_type = RPMB_MANAGE_KEY;
#else
		*key_type = RPMB_KEY_UNKOWN;
#endif
#elif (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
		*key_type = RPMB_MANAGE_KEY;
#else
		*key_type = RPMB_KEY_UNKOWN;
#endif
	}
}

enum rpmb_fs_meta_type {
	RPMB_FS_META_UNKOWN = 0,
	RPMB_FS_META,
	RPMB_FS_META_EX
};

void tee_rpmb_get_meta_type(uint32_t *meta_type)
{
	if (meta_type != NULL) {
#if (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_BALONG_V722_PLATFORM)
		*meta_type = RPMB_FS_META_UNKOWN;
#elif (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_KIRIN_PLATFORM)
#if (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3650)
		*meta_type = RPMB_FS_META;
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI6250)
		*meta_type = RPMB_FS_META;
#elif (TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3660 || TRUSTEDCORE_CHIP_CHOOSE == WITH_CHIP_HI3670)
		*meta_type = RPMB_FS_META_EX;
#else
		*meta_type = RPMB_FS_META_UNKOWN;
#endif
#elif (TRUSTEDCORE_PLATFORM_CHOOSE == WITH_DEVCHIP_PLATFORM)
		*meta_type = RPMB_FS_META_EX;
#else
		*meta_type = RPMB_FS_META_UNKOWN;
#endif
	}
}
