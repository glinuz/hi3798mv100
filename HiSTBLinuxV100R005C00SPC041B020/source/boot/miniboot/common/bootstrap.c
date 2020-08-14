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
#include <malloc.h>
#include <string.h>
#include <debug.h>
#include <delay.h>

#define XHEAD             0xAB
#define XCMD              0xCD
#define ACK               0xAA  /* ACK VALUE */
#define NAK               0x55  /* NAK VALUE */
#define BOOTUP            0x20  /* secure CA download */
#define START_FRAME_LEN   5
#define MAX_BUFF_SIZE     1024

static unsigned short crc16_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

static uint16 calc_crc16(unsigned char *packet, int length)
{
	uint16 crc16 = 0;
	int i;

	for (i = 0; i < length; i++) {
		crc16 = ((crc16 << 8) | packet[i])
			^ crc16_table[(crc16 >> 8) & 0xFF];
	}

	for (i = 0; i < 2; i++)
		crc16 = ((crc16 << 8) | 0) ^ crc16_table[(crc16 >> 8) & 0xFF];

	return crc16;
}

static int recv_byte(void)
{
	if(tstchar())
		return getchar();
	return -1;
}

static char recv_data(void)
{
	int ret = -1;
	while(ret == -1)
		ret = recv_byte();
	return (char)ret;
}

static void loop_cmd(void)
{
	int i = 0;
	int cr = 0;
	int ret = -1;
	uint32 head_frame_len = 0;
	uint32 cmd_len = 0;
	uint8 send_buf[20] = {0};
	uint16 cksum;
	char *recv_buf;

	recv_buf = malloc(MAX_BUFF_SIZE);
	if (!recv_buf) {
		pr_error("allocate memory fail.\n");
		return;
	}

	while (1) {
retry:
		cr = recv_byte();
		if (cr == -1)
			goto retry;

		if (XHEAD == cr) {
			head_frame_len = START_FRAME_LEN;
			recv_buf[0] = (char)cr;

			/* RECV: head frame */
			for (i=0; i < (head_frame_len - 1); i++)
				recv_buf[i+1] = recv_data();

			/* crc check */
			cksum = calc_crc16((unsigned char*)recv_buf, 3);
			if (cksum == ((recv_buf[3] << 8) | recv_buf[4])) {
				cmd_len = ((recv_buf[1] << 8) | recv_buf[2]) + 3;
				/* SEND: ack */
				send_buf[0] = ACK;
			} else {
				/* init */
				cmd_len = 0;
				/* SEND: nak */
				send_buf[0] = NAK;
			}
			putchar(send_buf[0]);
		} else if (XCMD == cr) {
			recv_buf[0] = (char)cr;

			/* RECV: cmd data */
			for (i = 0; i < (cmd_len - 1); i++)
				recv_buf[i+1] = recv_data();

			/* crc check */
			cksum = calc_crc16((unsigned char*)recv_buf, cmd_len - 2);
			if (cksum == ((recv_buf[cmd_len-2] << 8) | recv_buf[cmd_len-1])) {
				/* SEND: ack wait result */
				send_buf[0] = ACK;
				putchar(send_buf[0]);
			} else {
				memset(recv_buf, 0, sizeof(recv_buf));
				/* SEND: nak */
				send_buf[0] = NAK;
				putchar(send_buf[0]);
				goto retry;
			}

			/* clean crc */
			recv_buf[cmd_len-1] = 0;
			recv_buf[cmd_len-2] = 0;

			/* cmd process */
			ret = run_cmd((recv_buf + 1), strlen(recv_buf + 1));
			if (ret < 0)
				/* SEND: end flag */
 				pr_tool("[EOT](ERROR)\n");
 			else
				/* SEND: end flag */
				pr_tool("[EOT](OK)\n");
		} else {
			/* flush fifo */
		}
	}
}

#if defined(CONFIG_BOOTROM_SUPPORT) && defined (CONFIG_BOOTROM_CA_SUPPORT)
#  error CONFIG_BOOTROM_SUPPORT and CONFIG_BOOTROM_CA_SUPPORT should NOT be defined at the same time.
#endif

#ifdef CONFIG_BOOTROM_SUPPORT

void start_bootstrap(int (*handle)(void))
{
	if (CONFIG_START_MAGIC == (*(volatile uint32 *)(REG_START_FLAG))) {
		printf("begin to download boot.\n");

		/* clear flag */
		*(volatile uint32*)(REG_START_FLAG) = 0;
		pr_tool("start download process.\n");
		loop_cmd();
	}

	if (handle)
		handle();
}
#endif /* CONFIG_BOOTROM_SUPPORT */

#ifdef CONFIG_BOOTROM_CA_SUPPORT

void start_bootstrap(int (*handle)(void))
{
	int i = 0;
	int j = 0;
	int cr = 0;
	unsigned char send_buf[20] = {0};

	printf("begin to download boot.\n");

	for (i=0; i < 5;i++) {
		/* SEND: CA to PC, wait PC ACK */
		send_buf[0] = BOOTUP;
		for (j = 0; j < 5; j++)
			putchar(send_buf[0]);

		for (j = 0; j < 100; j++) {
			cr = recv_byte();
			if (cr == ACK) {
				/* goto down_load */
				pr_tool("start download process.\n");
				loop_cmd();
			}
			udelay(1000);
		}
		if (i == 4) {
			putchar('\n');
			return;
		}
	}

	if (handle)
		handle();
}
#endif /* CONFIG_BOOTROM_CA_SUPPORT */
