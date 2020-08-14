/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
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

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <command.h>
#include <cmd-words.h>

struct cmd_word_t cw_show = {
	"show", CMDWORD_TYPE_KEYWORD, _T("show system information.")
};

struct cmd_word_t cw_test = {
	"test", CMDWORD_TYPE_KEYWORD, _T("do some test.")
};

struct cmd_word_t cw_reset = {
	"reset", CMDWORD_TYPE_KEYWORD, _T("reset system, modules, ...")
};

struct cmd_word_t cw_reboot = {
	"reboot", CMDWORD_TYPE_KEYWORD, _T("reboot system.")
};

struct cmd_word_t cw_go = {
	"go", CMDWORD_TYPE_KEYWORD, _T("run image.")
};

struct cmd_word_t cw_boot = {
	"bootm", CMDWORD_TYPE_KEYWORD, _T("load image.")
};

struct cmd_word_t cw_nand = {
	"nand", CMDWORD_TYPE_KEYWORD, _T("NAND sub-system")
};

struct cmd_word_t cw_usb = {
	"usb", CMDWORD_TYPE_KEYWORD, _T("USB sub-system")
};

struct cmd_word_t cw_fat = {
	"fat", CMDWORD_TYPE_KEYWORD, _T("FAT sub-system")
};

struct cmd_word_t cw_mmc = {
	"mmc", CMDWORD_TYPE_KEYWORD, _T("MMC sub-system")
};

struct cmd_word_t cw_printenv = {
	"printenv", CMDWORD_TYPE_KEYWORD, _T("set environment")
};

struct cmd_word_t cw_setenv = {
	"setenv", CMDWORD_TYPE_KEYWORD, _T("set environment")
};

struct cmd_word_t cw_saveenv = {
	"saveenv", CMDWORD_TYPE_KEYWORD, _T("save environment to flash")
};

struct cmd_word_t cw_clearenv = {
	"clearenv", CMDWORD_TYPE_KEYWORD, _T("clear environment")
};

struct cmd_word_t cw_read = {
	"read", CMDWORD_TYPE_KEYWORD, _T("read operation")
};

struct cmd_word_t cw_read_yaffs = {
	"read.yaffs", CMDWORD_TYPE_KEYWORD, _T("read operation")
};

struct cmd_word_t cw_write = {
	"write", CMDWORD_TYPE_KEYWORD, _T("write operation")
};

struct cmd_word_t cw_write_yaffs = {
	"write.yaffs", CMDWORD_TYPE_KEYWORD, _T("write operation")
};

struct cmd_word_t cw_save = {
	"save", CMDWORD_TYPE_KEYWORD, _T("save operation")
};

struct cmd_word_t cw_erase = {
	"erase", CMDWORD_TYPE_KEYWORD, _T("erase operation")
};

struct cmd_word_t cw_clear = {
	"clear", CMDWORD_TYPE_KEYWORD, _T("clear operation")
};

struct cmd_word_t cw_scrub = {
	"scrub", CMDWORD_TYPE_KEYWORD, _T("scrub operation")
};

struct cmd_word_t cw_dump = {
	"dump", CMDWORD_TYPE_KEYWORD, _T("dump operation")
};

struct cmd_word_t cw_mark = {
	"mark", CMDWORD_TYPE_KEYWORD, _T("mark operation")
};

struct cmd_word_t cw_bad = {
	"bad", CMDWORD_TYPE_KEYWORD, _T("bad block")
};

struct cmd_word_t cw_start = {
	"start", CMDWORD_TYPE_KEYWORD, _T("init usb")
};

struct cmd_word_t cw_cmp = {
	"cmp", CMDWORD_TYPE_KEYWORD, _T("compare operation")
};

struct cmd_word_t cw_set = {
	"set", CMDWORD_TYPE_KEYWORD, _T("set operation")
};

struct cmd_word_t cw_byte = {
	"byte", CMDWORD_TYPE_KEYWORD, _T("read/write 8bit")
};

struct cmd_word_t cw_long = {
	"long", CMDWORD_TYPE_KEYWORD, _T("read/write 32bit")
};

struct cmd_word_t cw_help = {
	"help", CMDWORD_TYPE_KEYWORD, _T("show help information")
};

struct cmd_word_t cw_timer = {
	"timer", CMDWORD_TYPE_KEYWORD, _T("timer related operation.")
};

struct cmd_word_t cw_md = {
	"md", CMDWORD_TYPE_KEYWORD, _T("memory display sub-system.")
};

struct cmd_word_t cw_mw = {
	"mw", CMDWORD_TYPE_KEYWORD, _T("memory write sub-system.")
};

struct cmd_word_t cw_mm = {
	"mm", CMDWORD_TYPE_KEYWORD, _T("memory sub-system.")
};

struct cmd_word_t cw_system = {
	"system", CMDWORD_TYPE_KEYWORD, _T("current operation system.")
};

struct cmd_word_t cw_spiflash = {
	"spi", CMDWORD_TYPE_KEYWORD, _T("SPI flash sub-system.")
};

struct cmd_word_t cw_sf = {
	"sf", CMDWORD_TYPE_KEYWORD, _T("SPI flash sub-system.")
};

struct cmd_word_t cw_probe = {
	"probe", CMDWORD_TYPE_KEYWORD, _T("Init spiflash.")
};

struct cmd_word_t cw_mii = {
	"mii", CMDWORD_TYPE_KEYWORD, _T("ethernet MII sub-system")
};

struct cmd_word_t cw_oob = {
	"oob", CMDWORD_TYPE_KEYWORD, _T("nand oob area")
};

struct cmd_word_t cw_withoob = {
	"withoob", CMDWORD_TYPE_KEYWORD, _T("operation include oob area")
};

struct cmd_word_t cw_withbb = {
	"withbb", CMDWORD_TYPE_KEYWORD, _T("operation don't skip bad block.")
};

struct cmd_word_t cw_skipempty = {
	"skipempty", CMDWORD_TYPE_KEYWORD, _T("operation skip empty page.")
};

struct cmd_word_t cw_ubi = {
	"ubi", CMDWORD_TYPE_KEYWORD, _T("ubifs image.")
};

struct cmd_word_t cw_getinfo = {
	"getinfo", CMDWORD_TYPE_KEYWORD, _T("getinfo.")
};

struct cmd_word_t cw_bootmode = {
	"bootmode", CMDWORD_TYPE_KEYWORD, _T("boot media")
};

struct cmd_word_t cw_version = {
	"version", CMDWORD_TYPE_KEYWORD, _T("boot version.")
};

struct cmd_word_t cw_tftp = {
	"tftp", CMDWORD_TYPE_KEYWORD, _T("net sub-system.")
};

struct cmd_word_t cw_ping = {
	"ping", CMDWORD_TYPE_KEYWORD, _T("net sub-system.")
};

struct cmd_word_t cw_keyipaddr = {
	"ipaddr", CMDWORD_TYPE_KEYWORD, _T("ip address")
};

struct cmd_word_t cw_keynetmask = {
	"netmask", CMDWORD_TYPE_KEYWORD, _T("ip address subnet mask")
};

struct cmd_word_t cw_keymacaddr = {
	"ethaddr", CMDWORD_TYPE_KEYWORD, _T("eth mac address")
};

struct cmd_word_t cw_keygatewayip = {
	"serverip", CMDWORD_TYPE_KEYWORD, _T("gateway ip address")
};

struct cmd_word_t cw_devnum = {
	"<devnum>", CMDWORD_TYPE_SIZE, _T("devnum, such as: 0 or 1.")
};

struct cmd_word_t cw_offset = {
	"<offset>", CMDWORD_TYPE_SIZE, _T("offset, such as: 0x1000 or 4096 or 4k.")
};

struct cmd_word_t cw_length = {
	"<length>", CMDWORD_TYPE_SIZE, _T("length, such as: 0x1000 or 4096 or 4k.")
};

struct cmd_word_t cw_size = {
	"<size>", CMDWORD_TYPE_SIZE, _T("size, such as: 0x1000 or 4096 or 4k.")
};

struct cmd_word_t cw_addr = {
	"<address>", CMDWORD_TYPE_SIZE, _T("address, such as: 0x1000 or 4096 or 4k.")
};

struct cmd_word_t cw_number = {
	"<number>", CMDWORD_TYPE_SIZE, _T("number, such 1, 2, 3, ...")
};

struct cmd_word_t cw_data = {
	"<data>", CMDWORD_TYPE_SIZE, _T("data, such as: 12,0x12345678,...")
};

struct cmd_word_t cw_string = {
	"<string>", CMDWORD_TYPE_STRING, _T("string, such as: 'hello world'")
};

struct cmd_word_t cw_ipaddr = {
	"<ipaddr>", CMDWORD_TYPE_IPADDR, _T("ip address, such as '192.168.1.1', '255.255.255.0'")
};

struct cmd_word_t cw_macaddr = {
	"<ethaddr>", CMDWORD_TYPE_MAC, _T("eth mac address, such as '00:43:59:5c:00:01'")
};
