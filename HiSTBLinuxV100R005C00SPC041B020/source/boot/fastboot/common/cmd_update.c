/******************************************************************************
*    Copyright (c) 2009-2012 by Hisi
*    All rights reserved.
* ***
*    Create by Czyong. 2012-12-14
*
******************************************************************************/

#include <config.h>
#include <common.h>
#include <command.h>
#include <pkg_update.h>

int do_update(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	void *addr;

	/* Get the address */
	if (argc < 2) {
		printf("Please input update image address\n");
		return 0;
	} else
		addr = (void *)simple_strtoul(argv[1], NULL, 16);

	ptn_update(addr);

	return 0;
}

U_BOOT_CMD(update, 2, 0, do_update,
	"update system image",
	"[addr]\n"
	"\taddr\t - update package start address\n"
	""
);
