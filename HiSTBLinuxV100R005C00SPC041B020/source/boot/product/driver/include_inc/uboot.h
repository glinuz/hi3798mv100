#include <config.h>
#ifdef HI_MINIBOOT_SUPPORT
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <flash.h>
#include <hi_flash.h>
#include <malloc.h>
#include <string.h>
#include <delay.h>
#else
#include <common.h>
#include <asm/types.h>
#include <linux/types.h>
#include <part.h>
#include <flash_part.h>
#include <nand_logif.h>
#include <mmc.h>
#include <emmc_logif.h>
#include <spiflash_logif.h>
#include <mmc.h>
#include <hi_flash.h>
#include <usb_if.h>
#include <malloc.h>
#include <asm/string.h>
#include <nand.h>
#endif
