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
#include <stdio.h>
#include <command.h>
#include <cmd-words.h>
#include <env_set.h>

/*****************************************************************************/

static int do_env_show(struct cmd_t *cmd)
{
	env_dump();
	return 0;
}
CMD({&cw_printenv, _T("show system environment value."), do_env_show})
/*****************************************************************************/

static int do_env_set(struct cmd_t *cmd)
{
	int parm_index = param_first(cmd);
	char *key = param_str(cmd, parm_index++);
	char *value = param_str(cmd, parm_index++);

	return env_set(key, value);
}
CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_string, NULL},
    {&cw_string, 0, do_env_set})
/*****************************************************************************/

static int do_env_save(struct cmd_t *cmd)
{
	return env_save_to_flash();
}
CMD({&cw_saveenv, _T("save system environment value to flash."), do_env_save})
/*****************************************************************************/

static int do_env_clear(struct cmd_t *cmd)
{
	int parm_index = param_first(cmd);
	char *key = param_str(cmd, parm_index++);

	return env_clear(key);
}
CMD({&cw_clearenv, _T("clear one environment value."), NULL},
    {&cw_string, 0, do_env_clear})
/*****************************************************************************/

static int do_env_spc_set(struct cmd_t *cmd)
{
	int parm_index = param_first(cmd);
	const char *key = param_keyword(cmd, parm_index - 1);
	char *value = param_str(cmd, parm_index);
	return env_set(key, value);
}

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_keyipaddr, _T("set ip address"), NULL},
    {&cw_ipaddr, _T("ip address. such as '192.168.1.1'"), do_env_spc_set}) 

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_keynetmask, _T("set ip address subnet mask"), NULL},
    {&cw_ipaddr, _T("ip address subnet mask. such as '255.255.255.0'"), do_env_spc_set}) 

static struct cmd_word_t cw_bootcmd = {
	"bootcmd", CMDWORD_TYPE_KEYWORD, NULL,
};

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_bootcmd, _T("set boot command"), NULL},
    {&cw_string, _T("boot command"), do_env_spc_set}) 

static struct cmd_word_t cw_bootargs = {
	"bootargs", CMDWORD_TYPE_KEYWORD, NULL,
};

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_bootargs, _T("set boot args"), NULL},
    {&cw_string, _T("bootargs"), do_env_spc_set}) 

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_keymacaddr, _T("set eth mac address"), NULL},
    {&cw_macaddr, NULL, do_env_spc_set})

static struct cmd_word_t cw_worketh = {
	"worketh", CMDWORD_TYPE_KEYWORD, NULL,
};

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_worketh, _T("set tftp/ping ethernet device num"), NULL},
    {&cw_devnum, _T("ethernet device number, such as 0,1,..."), do_env_spc_set})

CMD({&cw_setenv, _T("set system environment value."), NULL},
    {&cw_keygatewayip, _T("set gateway ip address"), NULL},
    {&cw_ipaddr, _T("ip address. such as '192.168.1.1'"), do_env_spc_set}) 

