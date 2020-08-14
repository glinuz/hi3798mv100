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

#ifndef CMDWORDSH
#define CMDWORDSH
/******************************************************************************/

#include <command.h>

extern struct cmd_word_t cw_show;
extern struct cmd_word_t cw_go;
extern struct cmd_word_t cw_reset;
extern struct cmd_word_t cw_reboot;
extern struct cmd_word_t cw_boot;

extern struct cmd_word_t cw_nand;
extern struct cmd_word_t cw_usb;
extern struct cmd_word_t cw_fat;
extern struct cmd_word_t cw_spiflash;
extern struct cmd_word_t cw_sf;
extern struct cmd_word_t cw_probe;
extern struct cmd_word_t cw_mmc;
extern struct cmd_word_t cw_timer;
extern struct cmd_word_t cw_md;
extern struct cmd_word_t cw_mw;
extern struct cmd_word_t cw_mm;
extern struct cmd_word_t cw_system;
extern struct cmd_word_t cw_mii;
extern struct cmd_word_t cw_setenv;
extern struct cmd_word_t cw_printenv;
extern struct cmd_word_t cw_saveenv;
extern struct cmd_word_t cw_clearenv;
extern struct cmd_word_t cw_test;

extern struct cmd_word_t cw_read;
extern struct cmd_word_t cw_read_yaffs;
extern struct cmd_word_t cw_write;
extern struct cmd_word_t cw_write_yaffs;
extern struct cmd_word_t cw_save;
extern struct cmd_word_t cw_erase;
extern struct cmd_word_t cw_clear;
extern struct cmd_word_t cw_scrub;
extern struct cmd_word_t cw_dump;
extern struct cmd_word_t cw_set;
extern struct cmd_word_t cw_cmp;
extern struct cmd_word_t cw_mark;

extern struct cmd_word_t cw_byte;
extern struct cmd_word_t cw_long;
extern struct cmd_word_t cw_devnum;
extern struct cmd_word_t cw_offset;
extern struct cmd_word_t cw_length;
extern struct cmd_word_t cw_size;
extern struct cmd_word_t cw_addr;
extern struct cmd_word_t cw_data;
extern struct cmd_word_t cw_number;
extern struct cmd_word_t cw_ipaddr;
extern struct cmd_word_t cw_macaddr;

extern struct cmd_word_t cw_help;
extern struct cmd_word_t cw_string;

extern struct cmd_word_t cw_oob;
extern struct cmd_word_t cw_withoob;
extern struct cmd_word_t cw_withbb;
extern struct cmd_word_t cw_skipempty;
extern struct cmd_word_t cw_ubi;
extern struct cmd_word_t cw_bad;
extern struct cmd_word_t cw_start;

extern struct cmd_word_t cw_getinfo;
extern struct cmd_word_t cw_bootmode;
extern struct cmd_word_t cw_version;

extern struct cmd_word_t cw_tftp; 
extern struct cmd_word_t cw_ping;
extern struct cmd_word_t cw_keyipaddr;
extern struct cmd_word_t cw_keynetmask;
extern struct cmd_word_t cw_keymacaddr;
extern struct cmd_word_t cw_keygatewayip;

/******************************************************************************/
#endif /* CMDWORDSH */
