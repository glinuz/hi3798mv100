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

#include <stddef.h>
#include <compile.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <command.h>

#include "parse.h"
#include "history.h"
#include "intf.h"
#include "linepro.h"

/*****************************************************************************/

struct input_t {
	char cmdline[MAX_CMD_LENGTH];
	int last;
	int pos; /* current cursor position  */
	char *prompt; /* prompt */
};

#define BACKSPACE                     '\b'

/*****************************************************************************/

#define INPUT_POS_IS_LAST(_input) ((_input)->pos == (_input)->last)

#define INPUT_LAST_CHAR(_input) ((_input)->cmdline[(_input)->pos-1])

static void line_edit_init(struct input_t *input)
{
	input->prompt = CONFIG_PROMPT;
	input->pos = input->last = 0;
	input->cmdline[0] = 0;
}

static void line_edit_show(struct input_t *input)
{
	cl_puts(input->prompt);

	if (input->last) {
		input->cmdline[input->last] = 0;
		cl_puts(input->cmdline);
	}
}

static void line_edit_move(struct input_t *input, int pos)
{
	int num;
	char *ptr;

	if (!pos)
		return;

	num = input->pos;
	ptr = &input->cmdline[num];

	input->pos += pos;
	if (input->pos < 0)
		input->pos = 0;
	else if (input->pos > input->last)
		input->pos = input->last;

	num = (input->pos - num);
	if (num > 0) {
		while (num-- > 0)
			cl_putc(*ptr++);
	} else if (num < 0) {
		while (num++ < 0)
			cl_putc(BACKSPACE);
	}
}

static void line_edit_insert(struct input_t *input, char *str, int length)
{
	int pos;

	if (length + input->last + 1 > sizeof(input->cmdline))
		length = sizeof(input->cmdline) - input->last - 1;

	if (!length)
		return;

	for (pos = input->last; pos >= input->pos; pos--)
		input->cmdline[pos + length] = input->cmdline[pos];

	strncpy(input->cmdline + input->pos, str, length);

	input->last += length;
	for (pos = input->pos; pos < input->last; pos++)
		cl_putc(input->cmdline[pos]);

	input->pos += length;
	for (pos = input->pos; pos < input->last; pos++)
		cl_putc(BACKSPACE);
}

static void line_edit_remove(struct input_t *input, int length)
{
	int pos;
	int num = length + input->pos;

	if (num > input->last)
		length = input->last - input->pos;
	else if (num < 0)
		length = -input->pos;

	if (!length)
		return;

	if (length < 0) {
		length = -length;
		input->pos -= length;
		for (num = length; num > 0; num--)
			cl_putc(BACKSPACE);
	}

	for (pos = input->pos; pos < input->last - length; pos++) {
		input->cmdline[pos] = input->cmdline[pos + length];
		cl_putc(input->cmdline[pos]);
	}
	input->cmdline[pos] = 0;
	
	for (num = length; num > 0; num--)
		cl_putc(' ');

	num = input->last - input->pos;
	while (num-- > 0)
		cl_putc(BACKSPACE);

	input->last -= length;
}

static void line_edit_clean(struct input_t *input)
{
	line_edit_remove(input, input->last);
	line_edit_remove(input, -input->last);
}

static char *get_last_cmd(char *cmdline, int length)
{
	int size;
	char *head = NULL;
	char *next = cmdline - 1;

	if (!*cmdline)
		return NULL;

	do {
		++next;
		size = cmdline + length - next;
		if (size <= 0)
			return head;
		head = get_next_cmd(&next, (unsigned int)size);
	} while (*next);

	return head;
}
/*****************************************************************************/

static struct input_t __line_input;
static struct input_t *pinput = &__line_input;

int get_line(struct cmd_t *cmd, char *buf, int buflen)
{
	int chr;
	int pos = 0;
	char *ptr;

	line_edit_init(pinput);
	line_edit_show(pinput);

	do {
		chr = cl_getc();
		switch(chr) {
		case KEY_CTRL_C:
			line_edit_clean(pinput);
			break;

		case KEY_LEFT:
			line_edit_move(pinput, -1);
			break;

		case KEY_RIGHT:
			line_edit_move(pinput, 1);
			break;

		case KEY_BACKSPACE:
			line_edit_remove(pinput, -1);
			break;

		case KEY_DELETE:
			line_edit_remove(pinput, 1);
			break;

		case KEY_UP:
			pos--;

		case KEY_DOWN:
			if (chr == KEY_DOWN)
				pos++;

			ptr = hist_load(pos);
			if (ptr) {
				line_edit_clean(pinput);
				line_edit_insert(pinput, ptr, strlen(ptr));
			}
			break;

		default:
			if (isprint(chr)) {
				char str[1];
				str[0] = (char)chr;
				line_edit_insert(pinput, str, 1);
			}
			break;

		case '\t':
			if (!pinput->last || !INPUT_POS_IS_LAST(pinput))
				break;

			chr = (INPUT_LAST_CHAR(pinput) == ' ' ? '?' : chr);

		case '?':
			ptr = get_last_cmd(pinput->cmdline,
					   sizeof(pinput->cmdline));
			if (!ptr) {
				cmd->cmdline[0] = '\0';
			} else {
				strncpy(cmd->cmdline, ptr, sizeof(cmd->cmdline));
			}

			if (chr == '?') {
				cl_puts ("\r\n");
				show_cmd_help(cmd);
				line_edit_show(pinput);
			} else {
				char keyword[24];
				int len = sizeof(keyword);
				int ret = fill_cmd_word(cmd, keyword, &len);

				if (ret && ret != -EEXIST) {
					cl_puts ("\r\n");
					line_edit_show(pinput);
					break;
				}

				if (ret == -EEXIST)
					line_edit_show(pinput);

				line_edit_insert(pinput, keyword, len);
			}
			break;

		case '\r':
		case '\n':
			if (pinput->last) {
				strncpy(buf, pinput->cmdline, buflen);
				buf[buflen-1] = '\0';
				hist_save(pinput->cmdline,
					  sizeof(pinput->cmdline));
			} else {
				buf[0] = '\0';
			}
			cl_puts ("\r\n");
			return pinput->last;
		}
	} while (1);

	return 0;
}

char *get_next_cmd(char **cmdline, unsigned int length)
{
	int dem = 0;
	char *head, *tail;

	if (!cmdline || !*cmdline || !**cmdline || !length)
		return NULL;

	tail = head = *cmdline;

	while (length-- > 0 && *tail && (*tail != ';' || dem)) {
		if (*tail == '\'' || *tail == '\"') {
			if (!dem)
				dem = *tail;
			else if (*tail == dem)
				dem = 0;
		}
		tail++;
	}
	*cmdline = tail;
	return head;
}
