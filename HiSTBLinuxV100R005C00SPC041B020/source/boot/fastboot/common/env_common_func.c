#include <common.h>
#include <config.h>
#include <environment.h>
#include <asm/arch/platform.h>

#ifdef CONFIG_ENV_IS_IN_NAND
extern struct env_common_func_t nand_env_cmn_func;
#endif /* CONFIG_ENV_IS_IN_NAND */

#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
extern struct env_common_func_t sf_env_cmn_func;
#endif /* CONFIG_ENV_IS_IN_SPI_FLASH */

#ifdef CONFIG_ENV_IS_IN_EMMC
extern struct env_common_func_t emmc_env_cmn_func;
extern struct env_common_func_t sd_env_cmn_func;
#endif /* CONFIG_ENV_IS_IN_EMMC_FLASH */

env_t *env_ptr = 0;

static struct env_common_func_t *env_cmn_func =
#if defined(CONFIG_FORCE_ENV_IN_NAND)
	&nand_env_cmn_func;
#elif defined(CONFIG_FORCE_ENV_IN_SPI)
	&sf_env_cmn_func;
#elif defined(CONFIG_FORCE_ENV_IN_EMMC)
	&emmc_env_cmn_func;
#else
	NULL;
#endif

unsigned char env_get_char_spec(int index)
{
	return env_cmn_func ? env_cmn_func->env_get_char_spec(index) : -1;
}

int saveenv(void)
{
	return env_cmn_func ? env_cmn_func->saveenv() : -1;
}

int env_relocate_spec(unsigned int offset)
{
	if (env_cmn_func)
		return env_cmn_func->env_relocate_spec(offset);
	printf("Not found evn relocate function.\n");
	return 0;
}

int env_init(void)
{
	if (env_cmn_func == NULL) {
		switch (get_bootmedia(NULL, NULL)) {
		case BOOT_MEDIA_NAND:
		case BOOT_MEDIA_SPI_NAND:
#ifdef CONFIG_ENV_IS_IN_NAND
			env_cmn_func = &nand_env_cmn_func;
#else
			puts("Nand or SPI Nand is not supported by this boot\n");
#endif /* CONFIG_ENV_IS_IN_NAND */
			break;
		case BOOT_MEDIA_SPIFLASH:
#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
			env_cmn_func = &sf_env_cmn_func;
#else
			puts("SPI Nor Flash is not supported by this boot\n");
#endif /* CONFIG_ENV_IS_IN_SPI_FLASH */
			break;
		case BOOT_MEDIA_EMMC:
#ifdef CONFIG_ENV_IS_IN_EMMC
			env_cmn_func = &emmc_env_cmn_func;
#else
			puts("eMMC is not supported by this boot\n");
#endif /* CONFIG_ENV_IS_IN_EMMC */
			break;
		case BOOT_MEDIA_SD:
#ifdef CONFIG_ENV_IS_IN_EMMC
			env_cmn_func = &sd_env_cmn_func;
#else
			puts("fSD/eSD is not supported by this boot\n");
#endif /* CONFIG_ENV_IS_IN_EMMC */
			break;
		}
	}

	if (env_cmn_func)
		return env_cmn_func->env_init();

	return -1;
}

char *env_get_media(int *media)
{
	if (env_cmn_func) {
		if (media)
			(*media) = env_cmn_func->env_media;
		return env_cmn_func->env_name_spec;
	}

	if (media)
		(*media) = BOOT_MEDIA_UNKNOWN;
	return "not save";
}
