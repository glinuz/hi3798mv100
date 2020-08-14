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

#include <stdio.h>
#include <stddef.h>
#include <stddef.h>
#include <cmd-words.h>
#include <command.h>
#include <string.h>
#include <net/eth_phy.h>
#include <debug.h>

/*****************************************************************************/

static int do_send(struct cmd_t *cmd)
{
	int revlen;
	uint32 sz_buf;
	int param_index = param_first(cmd);
	uint32 devnum = param_int32(cmd, param_index++);
	struct net_dev_t *netdev = ethdev(devnum);
	char buf[1600] = {
		0x00, 0x0e, 0x7f, 0xae, 0x84, 0x51,
		0x00, 0x43, 0x59, 0x00, 0x00, 0x01,
		0x08, 0x00,
		0x45, 0x00, 0x00, 0x3c, 0x00, 0x02, 0x00, 0x00,
		0xff, 0x01, 0x38, 0x6a, 0xc0, 0xa8, 0x01, 0x03,
		0xc0, 0xa8, 0x01, 0x01, 0x08, 0x00, 0x57, 0x4c,
		0xaf, 0xaf, 0x00, 0x03, 0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
		0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
		0x1c, 0x1d, 0x1e, 0x1f, 0x00, 0x00, 0x00, 0x00,
	};

	if (!netdev) {
		printf("no found this dev:%d\n", devnum);
		return 0;
	}

	if (ethdev_init(netdev)) {
		printf("%s: init fail.\n", netdev->name);
		return 0;
	}

	netdev->linkup = -1;
	phy_link_status(netdev);
	if (netdev->linkup != TRUE)
		return 0;

	sz_buf = 64;
	printf("send package: length(%d)\n", sz_buf);
	dump_hex(0, buf, sz_buf, 1);
	netdev_send(netdev, buf, sz_buf);

	revlen = netdev_recv(netdev, buf, sizeof(buf));
	printf("recv package: length(%d)\n", revlen);
	if (revlen > 0)
		dump_hex(0, buf, sz_buf, 1);

	return 0;
}
/*****************************************************************************/

struct cmd_word_t cw_send = {
	"send", CMDWORD_TYPE_KEYWORD, _T("test send raw data.")
};

CMD({&cw_send, NULL},
    {&cw_devnum, 0, do_send});
