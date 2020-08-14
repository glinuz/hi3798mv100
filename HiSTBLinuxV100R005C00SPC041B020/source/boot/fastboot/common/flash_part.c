/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/
#include <common.h>
#include <linux/types.h>
#include <linux/string.h>
#include <malloc.h>
#include <asm/errno.h>

#if 0
#define DBG_MSG(_fmt, arg...) \
	printf("%s(%d): " _fmt, __FILE__, __LINE__, ##arg);
#else
#define DBG_MSG(_fmt, arg...)
#endif

/* Largest string for a blockdev identifier */
#define BDEVNAME_SIZE                   32

struct cmdline_subpart {
	char name[BDEVNAME_SIZE]; /* partition name, such as 'rootfs' */
	__u64 from;
	__u64 size;
	struct cmdline_subpart *next_subpart;
};

struct cmdline_parts {
	char name[BDEVNAME_SIZE]; /* block device, such as 'mmcblk0' */
	__u64 end_addr;
	struct cmdline_subpart *subpart;
	struct cmdline_parts *next_parts;
};

static struct cmdline_parts *cmdline_parts_head = NULL;

static __u64 memparse(const char *ptr, char **retptr)
{
	char *endptr;   /* local pointer to end of parsed string */
	__u64 ret = simple_strtoull(ptr, &endptr, 0);

	switch (*endptr) {
	case 'G':
	case 'g':
		ret <<= 10;
	//lint -fallthrough
	case 'M':
	case 'm':
		ret <<= 10;
	//lint -fallthrough
	case 'K':
	case 'k':
		ret <<= 10;
		endptr++;
	//lint -fallthrough
	default:
		break;
	}

	if (retptr)
		*retptr = endptr;

	return ret;
}

static int parse_subpart(struct cmdline_parts *parts,
			 struct cmdline_subpart **subpart, char *cmdline)
{
	int ret = 0;
	struct cmdline_subpart *new_subpart;

	*subpart = NULL;

	new_subpart = malloc(sizeof(struct cmdline_subpart));
	if (!new_subpart) {
		DBG_MSG("Fail to malloc for new subpart\n");
		return -ENOMEM;
	}
	memset(new_subpart, 0, sizeof(struct cmdline_subpart));

	if (*cmdline == '-') {
		new_subpart->size = (__u64)(~0ULL);
		cmdline++;
	} else {
		new_subpart->size = memparse(cmdline, &cmdline);
		if (new_subpart->size < 4096) {
			DBG_MSG("cmdline partition size is invalid.");
			ret = -EINVAL;
			goto fail;
		}
	}

	if (*cmdline == '@') {
		cmdline++;
		new_subpart->from = memparse(cmdline, &cmdline);
		parts->end_addr = new_subpart->from + new_subpart->size;
	} else {
		new_subpart->from = parts->end_addr;
		parts->end_addr += new_subpart->size;
	}

	if (*cmdline == '(') {
		int length;
		char *next = strchr(++cmdline, ')');

		if (!next) {
			DBG_MSG("cmdline partition format is invalid.");
			ret = -EINVAL;
			goto fail;
		}

		length = min((unsigned int)(next - cmdline),
			       sizeof(new_subpart->name) - 1);
		strncpy(new_subpart->name, cmdline, length);
		new_subpart->name[length] = '\0';

		cmdline = ++next;
	} else
		new_subpart->name[0] = '\0';

	*subpart = new_subpart;
	return 0;
fail:
	free(new_subpart);
	return ret;
}

static void free_subpart(struct cmdline_parts *parts)
{
	struct cmdline_subpart *subpart;

	while (parts->subpart) {
		subpart = parts->subpart;
		parts->subpart = subpart->next_subpart;
		if (subpart)
			free(subpart);
	}
}

static void free_parts(struct cmdline_parts **parts)
{
	struct cmdline_parts *next_parts;

	while (*parts) {
		next_parts = (*parts)->next_parts;
		free_subpart(*parts);
		free(*parts);
		*parts = next_parts;
	}
}

static int parse_parts(struct cmdline_parts **parts, char *cmdline)
{
	int ret = -EINVAL;
	char *next;
	int length;
	struct cmdline_subpart **next_subpart;
	struct cmdline_parts *newparts = NULL;
	char buf[BDEVNAME_SIZE + 32 + 4];

	*parts = NULL;

	newparts = malloc(sizeof(struct cmdline_parts));
	if (!newparts) {
		DBG_MSG("Fail to malloc for new parts\n");
		return -ENOMEM;
	}

	memset(newparts, 0, sizeof(struct cmdline_parts));

	next = strchr(cmdline, ':');
	if (!next) {
		DBG_MSG("cmdline partition has not block device.");
		goto fail;
	}

	length = min((unsigned int)(next - cmdline), sizeof(newparts->name) - 1);
	strncpy(newparts->name, cmdline, length);
	newparts->name[length] = '\0';
	newparts->end_addr = 0;

	next_subpart = &newparts->subpart;

	while (next && *(++next)) {
		cmdline = next;
		next = strchr(cmdline, ',');

		length = (!next) ? (sizeof(buf) - 1) :
			min((unsigned int)(next - cmdline), sizeof(buf) - 1);

		strncpy(buf, cmdline, length);
		buf[length] = '\0';

		ret = parse_subpart(newparts, next_subpart, buf);
		if (ret)
			goto fail;

		next_subpart = &(*next_subpart)->next_subpart;
	}

	if (!newparts->subpart) {
		DBG_MSG("cmdline partition has not valid partition.");
		ret = -EINVAL;
		goto fail;
	}

	*parts = newparts;

	return 0;
fail:
	if (newparts) {
		free_subpart(newparts);
		free(newparts);
	}
	return ret;
}

static int parse_cmdline(struct cmdline_parts **parts, const char *cmdline)
{
	int ret;
	char *buf;
	char *pbuf;
	char *next;
	struct cmdline_parts **next_parts;

	*parts = NULL;

	next = pbuf = buf = strdup(cmdline);
	if (!buf) {
		DBG_MSG("Fail to dup cmdline, size=%d\n", strlen(cmdline));
		return -ENOMEM;
	}

	next_parts = parts;

	while (next && *pbuf) {
		next = strchr(pbuf, ';');
		if (next)
			*next = '\0';

		ret = parse_parts(next_parts, pbuf);
		if (ret)
			goto fail;

		if (next)
			pbuf = ++next;

		next_parts = &(*next_parts)->next_parts;
	}

	if (!*parts) {
		DBG_MSG("cmdline partition has not valid partition.");
		ret = -EINVAL;
		goto fail;
	}

	ret = 0;
done:
	free(buf);
	return ret;

fail:
	free_parts(parts);
	goto done;
}


static struct cmdline_parts *get_cmdline_parts(char *cmdline_string)
{
	struct cmdline_parts *cmdline_parts = NULL;

	if (!cmdline_string)
		goto fail;

	if (parse_cmdline(&cmdline_parts, cmdline_string) || !cmdline_parts)
		goto fail;

	return cmdline_parts;
fail:
	if (cmdline_parts)
		free_parts(&cmdline_parts);

	return NULL;
}

/* cmdline_string like: mmcblk0:... hi_sfc:..., or hinand:... */
int cmdline_parts_init(char *cmdline_string)
{
	int ret = -1;
	char *ptail = NULL;
	char *pcmdline = NULL;

	if (cmdline_parts_head) {
		ret = 0;
		goto done;
	}

	pcmdline = strdup(cmdline_string);
	if (!pcmdline) {
		ret = -ENOMEM;
		goto done;
	}

	if ((ptail = strchr(pcmdline, ' ')) &&
	    (unsigned int)(ptail - pcmdline) <= strlen(pcmdline))
		*ptail = '\0';

	cmdline_parts_head = get_cmdline_parts(pcmdline);
	if (!cmdline_parts_head) {
		DBG_MSG("Fail to get cmdline parts from: %s\n", pcmdline);
		ret = -ENODEV;
		goto done;
	}

	ret = 0;
done:
	if (pcmdline)
		free(pcmdline);
	return ret;
}

int flash_part_init(char *cmdline_string)
{
	if (!cmdline_string) {
		DBG_MSG("Invalid parameter!\n");
		return -1;
	}

	if (!cmdline_parts_head && (0 > cmdline_parts_init(cmdline_string)))
		goto fail;

	return 0;
fail:
	return -1;
}
/* 1 - find, 0 - no find */
int find_flash_part(char *cmdline_string,
		    const char *media_name,  /* hi_sfc, hinand */
		    char *ptn_name,
		    uint64_t *start,
		    uint64_t *length)
{
	int got = 0;
	struct cmdline_subpart *subpart = NULL;
	struct cmdline_parts *cmdline_parts = NULL;

	if (!media_name || !ptn_name || !start || !length)
		goto fail;

	if (!cmdline_parts_head) {
		DBG_MSG("Flash part not initialized!\n");
		goto fail;
	}


	got = 0;
	cmdline_parts = cmdline_parts_head;
	while (cmdline_parts && !got) {
		if (!strncmp(cmdline_parts->name, media_name,
			     sizeof(cmdline_parts->name))) {
			got = 1;
			break;
		}
		cmdline_parts = cmdline_parts->next_parts;
	}

	if (!got) {
		DBG_MSG("%s not found from: %s\n", media_name, cmdline_string);
		goto fail;
	}

	got = 0;
	subpart = cmdline_parts->subpart;
	while (subpart && !got) {
		if (!strncmp(subpart->name, ptn_name,
			     sizeof(subpart->name))) {
			got = 1;
			break;
		}
		subpart = subpart->next_subpart;
	}

	if (!got) {
		DBG_MSG("%s not found from: %s\n", ptn_name, cmdline_string);
		goto fail;
	}

	*start  = (uint64_t)(subpart->from);
	*length = (uint64_t)(subpart->size);
	DBG_MSG("Got partition %s: start=0x%llX,size=%llu\n",
		ptn_name, *start, *length);

	return 1;
fail:
	return 0;
}
