/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Yan Jiantao
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <stdio.h>
#include <command.h>
#include <cmd-words.h>
#include <cpuinfo.h>

/*****************************************************************************/

static int do_get_bootmode(struct cmd_t *cmd)
{
	switch (get_bootmedia(NULL, NULL)) {
		default:
		case BOOT_MEDIA_UNKNOWN:
			pr_tool("Boot from unknown device, please check your hardware config.\n");
			return -1;

		case BOOT_MEDIA_NAND:
			pr_tool("nand\n");
			break;

		case BOOT_MEDIA_SPIFLASH:
			pr_tool("spi\n");
			break;

		case BOOT_MEDIA_SPI_NAND:
			pr_tool("spi-nand\n");
			break;

		case BOOT_MEDIA_EMMC:
			pr_tool("emmc\n");
			break;

		case BOOT_MEDIA_DDR:
			pr_tool("ddr\n");
			break;
	}
	return 0;
}
CMD({&cw_getinfo, NULL},
    {&cw_bootmode, _T("show boot media type."), do_get_bootmode})

static int do_get_version(struct cmd_t *cmd)
{
	pr_tool("miniboot version: %x.%x.%x\n",
		VERSION, PATCHLEVEL, SUBLEVEL);
	return 0;
}
CMD({&cw_getinfo, NULL},
    {&cw_version, _T("show boot version."), do_get_version})
