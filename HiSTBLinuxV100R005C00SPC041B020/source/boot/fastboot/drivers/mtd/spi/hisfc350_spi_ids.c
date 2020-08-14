#include <config.h>

#ifdef CONFIG_ARCH_GODBOX
#  include "hisfc350_godbox_spi_ids.c"
#endif

#if defined(CONFIG_ARCH_S40) || defined(CONFIG_ARCH_S5) || defined(CONFIG_ARCH_HIFONE)
#  include "hisfc350_s40_spi_ids.c"
#endif
