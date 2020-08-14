#include <command.h>
#include "hi_unf_ir.h"
#include "ir.h"

#ifdef HI_MINIBOOT_SUPPORT
#include "app.h"
#endif

#ifndef HI_MINIBOOT_SUPPORT

static int get_name_from_cmd(int argc, char *argv[], int from, char *name, int sz)
{
	int i, left = sz, len;
	char *p = name;

	if (sz <= 1)
		return -1;

	memset(name, 0, sz);

	for (i = from; i < argc; i++) {
		len = strlen(argv[i]);
		if (len + 1 < left) {
			memcpy(p, argv[i], len);
			p += len ;
			if (i < argc - 1)
				*p++ = ' ';
			else 
				*p = '\0';
			left -= (len + 1);
		} else {
			HI_INFO_IR("protocol name too long!\n");
			return -1;
		}

	}
	return 0;
}

/*
 * IR command:
 * ir read @cnt @timeout: get RC key pressed, read @cnt keys
 *	and each key can wait @timeout ms 
 * ir start: open ir module
 * ir stop: close ir module
 * ir set up @0|1: set ir driver report up event or not.
 *	@0: do not report. @1: report up event.
 * ir set hold @0|1 : set ir driver report hold event or not.
 *	@0: do not report. @1: report hold.
 * ir set hi @time: set the HOLD event report interval time. unit ms
 * ir set dbg @on|off : turn on debug or turn off debug 
 * ir set info @on|off : turn on info or turn off info 
 * ir reset : reset ir module, which will clear buffers, restart ir IP.
 * ir enable @protocol_name: enable protocol specified by @protocol_name.
 * ir disable @protocol_name: disable protocol specified by @protocol_name.
 * ir status @protcol_name: display the enable/disable status of @protocol_name.
 */

int do_ir(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	char *cmd;
	char name[PROTOCOL_NAME_SZ];
	unsigned long val;
	if (argc < 2)
		goto usage;

	cmd = argv[1];

	if (!strncmp(cmd, "start", 5)) {
		HI_INFO_IR("Trying open ir module!\n");
		return HI_UNF_IR_Init();
	}
	else if (!strncmp(cmd, "stop", 4)) {
		HI_INFO_IR("Trying close ir module!\n");
		return HI_UNF_IR_DeInit();
	}
	else if (!strncmp(cmd, "reset", 5)) {
		HI_INFO_IR("Trying reset ir module!\n");
		return HI_UNF_IR_Reset();
	}
	else if (!strncmp(cmd, "set", 6)) {
		if (argc != 4)
			goto usage;

		if (!strncmp(argv[2], "up", 2)) {
			val = simple_strtoul(argv[3], NULL, 16);
			if (val !=0 && val != 1)
				goto usage;
			HI_INFO_IR("Trying set up event to %lu.\n",val);
			return HI_UNF_IR_EnableKeyUp(val);
		} 
		else if (!strncmp(argv[2], "hold", 4)) {
			val = simple_strtoul(argv[3], NULL, 16);
			if (val !=0 && val != 1)
				goto usage;
			HI_INFO_IR("Trying set hold event to %lu.\n",val);
			return HI_UNF_IR_EnableRepKey(val);
		}
		else if (!strncmp(argv[2], "hi", 2)) {
			val = simple_strtoul(argv[3], NULL, 16);
			HI_INFO_IR("Trying set hold interval to %lu ms\n", val);
			return HI_UNF_IR_SetRepKeyTimeoutAttr(val);
		}
#ifdef HIIR_DEBUG
		else if (!strncmp(argv[2], "dbg", 3)) {
			if (!strncmp(argv[3], "on", 2)) {
				HI_UNF_IR_SetDbgOn(1);
			}
			else  {
				HI_UNF_IR_SetDbgOn(0);
			}
			return 0;
		}
		else if (!strncmp(argv[2], "info", 4)) {
			if (!strncmp(argv[3], "on", 2)) {
				HI_UNF_IR_SetInfoOn(1);
			} else {
				HI_UNF_IR_SetInfoOn(0);
			}
			return 0;
		}
#endif
		else 
			goto usage;
	}
	else if (!strncmp(cmd, "disable", 7)){
		if (argc < 3)
			goto usage;

		if (get_name_from_cmd(argc, argv, 2, name, PROTOCOL_NAME_SZ)) 
			return -1;
		HI_INFO_IR("Trying disable %s\n", name);
		return HI_UNF_IR_DisableProtocol(name);
	}
	else if (!strncmp(cmd, "status", 6)){
		HI_BOOL enable;
		if (argc < 3)
			goto usage;

		if (get_name_from_cmd(argc, argv, 2, name, PROTOCOL_NAME_SZ)) 
			return -1;
		if (HI_UNF_IR_GetProtocolEnabled(name, &enable)) {
			HI_INFO_IR("Fail to get enable status of '%s'\n",name);
			return 1;
		}
		HI_INFO_IR("Get status of %s is %s\n", name, enable? "Enable" : "Disable");
		return 0;
	}
	else if (!strncmp(cmd, "enable", 6)) {
		if (argc < 3)
			goto usage;

		if (get_name_from_cmd(argc, argv, 2, name, PROTOCOL_NAME_SZ)) 
			return -1;
		HI_INFO_IR("Trying enable %s\n", name);
		return HI_UNF_IR_EnableProtocol(name);
	}
	else if (!strncmp(cmd, "read", 4)) {
		unsigned long cnt, timeout_ms;
		unsigned long i, received;
		int ret;
		enum KEY_STATUS status;
		HI_U64 key;
	
		if (argc != 4)
			goto usage;
		cnt = simple_strtoul(argv[2], NULL, 10);
		timeout_ms = simple_strtoul(argv[3], NULL, 10);
		for (i = 0, received = 0; i < cnt; i++) {
			ret = HI_UNF_IR_GetValueWithProtocol(&status, &key, &name[0], PROTOCOL_NAME_SZ, timeout_ms);
			if (!ret) {
				HI_INFO_IR("Received key: 0x%.08llx, %4s,\tprotocol: %s.\n",
						key, status == KEY_STAT_DOWN ? "DOWN" :
						(status == KEY_STAT_UP ? "UP" : "HOLD"),
						name);
				received ++;
			}
			if (tstc()) {
				if (getc()  == 0x3) {
					HI_INFO_IR("^C detected, abort!\n");
					return 0;
				}
			}
		}
		HI_INFO_IR("Total receive %lu keys!\n", received);
		return 0;
	}
usage:
	cmd_usage(cmdtp);
	return 1;
}

#ifdef HIIR_DEBUG
U_BOOT_CMD(ir, CONFIG_SYS_MAXARGS, 1, do_ir,
	"IR command:",
	"ir read @cnt @timeout: get RC key pressed, read @cnt keys.\n"\
	"     and each key can wait @timeout ms.\n"\
	"     press ctrl+c to exit read.\n"\
	"ir start: open ir module.\n"\
	"ir stop: close ir module.\n"\
	"ir set up @0|1: set ir driver report up event or not.\n"\
	"     @0: do not report. @1: report up event.\n"\
	"ir set hold @0|1 : set ir driver report hold event or not.\n"\
	"     @0: do not report. @1: report hold.\n"\
	"ir set hi @time: set the HOLD event report interval time. unit ms\n"\
	"ir set dbg @on: turn on|off debug message.\n"\
	"ir set info @on: turn on|off info message.\n"\
	"ir reset : reset ir module, which will clear buffers, restart ir IP.\n"\
	"ir enable @protocol_name: enable protocol specified by @protocol_name.\n"\
	"ir disable @protocol_name: disable protocol specified by @protocol_name.\n"\
	"ir status @protcol_name: display the enable/disable status of @protocol_name."
);
#else
U_BOOT_CMD(ir, CONFIG_SYS_MAXARGS, 1, do_ir,
	"IR command:",
	"ir read @cnt @timeout: get RC key pressed, read @cnt keys.\n"\
	"     and each key can wait @timeout ms.\n"\
	"ir start: open ir module.\n"\
	"ir stop: close ir module.\n"\
	"ir set up @0|1: set ir driver report up event or not.\n"\
	"     @0: do not report. @1: report up event.\n"\
	"ir set hold @0|1 : set ir driver report hold event or not.\n"\
	"     @0: do not report. @1: report hold.\n"\
	"ir set hi @time: set the HOLD event report interval time. unit ms\n"\
	"ir reset : reset ir module, which will clear buffers, restart ir IP.\n"\
	"ir enable @protocol_name: enable protocol specified by @protocol_name.\n"\
	"ir disable @protocol_name: disable protocol specified by @protocol_name.\n"\
	"ir status @protcol_name: display the enable/disable status of @protocol_name."
);
#endif
#endif
