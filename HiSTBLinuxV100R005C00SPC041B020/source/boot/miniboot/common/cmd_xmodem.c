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
#include <xmodem.h>

static int do_loadx(struct cmd_t *cmd)
{
	int ret;
	int param_index = param_first(cmd);
	uint32 addr = param_int32(cmd, param_index++);
	uint32 len = param_int32(cmd, param_index++);

	printf("Send data using the xmodem protocol from your terminal emulator now...\n");

	/*
	 * the following should be changed for your environment:
	 * 0x30000 is the download address,
	 * 65536 is the maximum size to be written at this address
	 */
	ret = xmodem_receive((unsigned char *)addr, len);
	if (ret < 0)
		printf("Xmodem receive error: status: %d\n", ret);
	else
		printf("Xmodem successfully received %d bytes\n", ret);

	return 0;
}

static struct cmd_word_t cw_loadx = {
	"loadx", CMDWORD_TYPE_KEYWORD, _T("load file from host over serial line(xmodem).")
};
CMD({&cw_loadx, NULL},
    {&cw_addr, 0, NULL},
    {&cw_length, 0, do_loadx})

static int do_uploadx(struct cmd_t *cmd)
{
	int ret;
	int param_index = param_first(cmd);
	uint32 addr = param_int32(cmd, param_index++);
	uint32 len = param_int32(cmd, param_index++);

	printf ("Send data using the xmodem protocol from your terminal emulator now...\n");

	/*
	 * the following should be changed for your environment:
	 * 0x30000 is the download address,
	 * 65536 is the maximum size to be written at this address
	 */
	ret = xmodem_transmit((unsigned char *)addr, len);
	if (ret < 0)
		printf ("Xmodem receive error: ret: %d\n", ret);
	else
		printf ("Xmodem successfully received %d bytes\n", ret);

	return 0;
}

static struct cmd_word_t cw_uploadx = {
	"uploadx", CMDWORD_TYPE_KEYWORD, _T("upload file to host over serial line(xmodem).")
};
CMD({&cw_uploadx, NULL},
    {&cw_addr, 0, NULL},
    {&cw_length, 0, do_uploadx})
