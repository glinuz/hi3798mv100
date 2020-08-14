#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <securec.h>
/* #include "tee_socket_debug.h" */
#define LOG_NDEBUG 0

#ifdef LOG_TAG
#undef	LOG_TAG
#define LOG_TAG "teecd"
#endif

#include <cutils/log.h>
#define LOGE ALOGE
#define LOGV ALOGV
#include "agent_socket_buffer.h"

static TEEC_UUID m_uuid_reserved = {
	.timeLow = 0xFFFFFFFF,
	.timeMid = 0xFFFF,
	.timeHiAndVersion = 0xFFFF,
	.clockSeqAndNode = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};
static uint32_t m_ssid_reserved = 0xFFFFFFFF; /* concurrent opened session count in tee is 10 */

void print_uuid(TEEC_UUID *uuid)
{
	if (NULL == uuid) {
		return;
	}

	LOGV("UUID=%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x\n",
	     uuid->timeLow,
	     uuid->timeMid, uuid->timeHiAndVersion,
	     uuid->clockSeqAndNode[0], uuid->clockSeqAndNode[1],
	     uuid->clockSeqAndNode[2], uuid->clockSeqAndNode[3],
	     uuid->clockSeqAndNode[4], uuid->clockSeqAndNode[5],
	     uuid->clockSeqAndNode[6], uuid->clockSeqAndNode[7]);
}

void print_socket_buffer_entry(struct socket_buffer_entry_t *entry)
{
	struct socket_buffer_entry_t *p = entry;

	if (p == NULL) {
		LOGV("====>entry is null\n");
	}

	while (p != NULL) {
		LOGV("==>ssid=%d, len=%d, pos=%d, buffer=%s\n",
		     p->session_id,
		     p->length,
		     p->pos,
		     p->buffer);
		p = p->next;
	}
}

void print_socket_buffer_pool(struct socket_buffer_pool_t *pool)
{
	struct socket_buffer_pool_t *p = pool;

	LOGV("+++++++++++++++++++++++++++++\n");

	while (p) {
		LOGV("====>");
		print_uuid(&p->service_id);
		print_socket_buffer_entry(p->entry);
		p = p->next;
	}

	LOGV("-----------------------------\n");
}

struct socket_buffer_pool_t *socket_pool_init()
{
	struct socket_buffer_pool_t *pool;
	errno_t rc = EOK;

	pool = malloc(sizeof(struct socket_buffer_pool_t));
	if (!pool) {
		return NULL;
	}

	rc = memcpy_s(&pool->service_id, sizeof(pool->service_id),
		&m_uuid_reserved, sizeof(TEEC_UUID)); /*lint !e838 */
	if (EOK != rc) {
		free(pool);
		pool = NULL;
		(void)pool;
		return NULL;
	}

	pool->next  = NULL;
	pool->entry = NULL;

	pool->entry = malloc(sizeof(struct socket_buffer_entry_t));

	if (!pool->entry) {
		free(pool);
		pool = NULL;
		(void)pool;
		return NULL;
	}

	pool->entry->session_id = m_ssid_reserved;
	pool->entry->length = 0;
	pool->entry->pos = 0;
	pool->entry->buffer = NULL;
	pool->entry->next = NULL;

	return pool;
}

struct socket_buffer_pool_t *socket_pool_new(struct socket_buffer_pool_t *pool, TEEC_UUID uuid)
{
	struct socket_buffer_pool_t *p, *new_pool;
	errno_t rc = EOK;

	p = pool;
	if (NULL == p) {
		return NULL;
	}

	while (p->next) {
		p = p->next;
	}

	new_pool = socket_pool_init(); /*[false alarm]: free buffer in socket_clear_buffer */
	if (new_pool != NULL) {
		rc = memcpy_s(&new_pool->service_id, sizeof(new_pool->service_id),
				&uuid, sizeof(TEEC_UUID));
		if (EOK != rc) {
			socket_free_mem(&new_pool);
			return NULL;
		}

		p->next = new_pool;
		return new_pool;
	} else {
		return NULL;
	}
}

struct socket_buffer_pool_t *socket_pool_add(struct socket_buffer_pool_t *pool, TEEC_UUID uuid)
{
	struct socket_buffer_pool_t *p;
	bool found = false;

	p = pool;
	if (NULL == p) {
		return NULL;
	}

	while (p) {
		if (memcmp(&p->service_id, &uuid, sizeof(TEEC_UUID)) == 0) {
			found = true;
			break;
		}

		p = p->next;
	}

	if (found) {
		LOGV("have found uuid in pool\n");
		return p;
	} else {
		LOGV("not found uuid in pool, need insert new to pool\n");
		return socket_pool_new(pool, uuid);
	}
}

struct socket_buffer_entry_t *socket_entry_init()
{
	struct socket_buffer_entry_t *entry;

	entry = malloc(sizeof(struct socket_buffer_entry_t));
	if (!entry)
		return NULL;

	entry->session_id = 0;
	entry->length = 0;
	entry->pos = 0;
	entry->buffer = NULL;
	entry->next = NULL;
	return entry;
}

struct socket_buffer_entry_t *socket_entry_new(struct socket_buffer_pool_t *pool, uint32_t ssid)
{
	struct socket_buffer_entry_t *p, *new_entry;

	p = pool->entry;
	if (NULL == p) {
		return NULL;
	}

	while (p->next) {
		p = p->next;
	}

	new_entry = socket_entry_init();

	if (new_entry != NULL) {
		p->next = new_entry;
		new_entry->session_id = ssid;
		return new_entry;
	} else {
		return NULL;
	}
}

struct socket_buffer_entry_t *socket_entry_add(struct socket_buffer_pool_t *pool, uint32_t ssid)
{
	struct socket_buffer_entry_t *p;
	bool found = false;

	if (NULL == pool) {
		return NULL;
	}

	p = pool->entry;
	while (p) {
		if (p->session_id == ssid) {
			found = true;
			break;
		}

		p = p->next;
	}

	if (found) {
		LOGV("have found ssid in entry\n");
		return p;
	} else {
		return socket_entry_new(pool, ssid);
	}
}

int socket_store_buffer(struct socket_buffer_pool_t *pool,
			TEEC_UUID uuid, uint32_t ssid,
			uint8_t *buffer,
			uint32_t len_part, uint32_t len_whole)
{
	struct socket_buffer_pool_t *cur_pool;
	struct socket_buffer_entry_t *cur_entry;
	errno_t rc = EOK;

	if (NULL == pool || NULL == buffer || (0 == len_whole)) {
		LOGE("parameter is invalid\n");
		return -1;
	}

	cur_pool = socket_pool_add(pool, uuid); /*[false alarm]: free buffer in follow func */
	if (cur_pool == NULL) {
		LOGE("cant get uuid from pool\n");
		return -1;
	}

	cur_entry = socket_entry_add(cur_pool, ssid); /*[false alarm]: free buffer in follow func */
	if (cur_entry == NULL) {
		/* NOTICE: we dont free uuid-pool */
		LOGE("cant get ssid from uuid_pool\n");
		return -1;
	}

	if (cur_entry->buffer == NULL) {
		cur_entry->length = len_whole;
		cur_entry->buffer = malloc(cur_entry->length);

		if (cur_entry->buffer == NULL) {
			socket_clear_buffer(pool, uuid, ssid); /*  free the uuid-entry */
			LOGE("malloc size=%d failed\n", cur_entry->length);
			return -1;
		}
	}

	if ((cur_entry->pos + len_part) > cur_entry->length) {
		socket_clear_buffer(pool, uuid, ssid); /*  free the uuid-entry */
		LOGE("buffer is overflow\n");
		return -1;
	}

	rc = memcpy_s(cur_entry->buffer + cur_entry->pos, (cur_entry->length - cur_entry->pos),
			buffer, len_part); /*lint !e838 */
	if (EOK != rc) {
		socket_clear_buffer(pool, uuid, ssid); /*  free the uuid-entry */
		LOGE("memcpy_s failed  %d\n", rc);
		return -1;
	}

	cur_entry->pos += len_part;

	return 0;
}

int socket_clear_buffer(struct socket_buffer_pool_t *pool, TEEC_UUID uuid, uint32_t ssid)
{
	struct socket_buffer_pool_t *cur_pool;
	struct socket_buffer_entry_t *cur_entry, *pre_entry;
	bool found;

	if (NULL == pool) {
		return 0;
	}

	/* find uuid in pool */
	found = false;
	cur_pool = pool;

	while (cur_pool) {
		if (memcmp(&cur_pool->service_id, &uuid, sizeof(TEEC_UUID)) == 0) {
			found = true;
			break;
		}

		cur_pool = cur_pool->next;
	}

	if (!found || cur_pool == NULL) {
		return 0;
	}

	/* find ssid in uuid-pool */
	found = false;
	cur_entry = cur_pool->entry;
	pre_entry = cur_pool->entry;

	while (cur_entry) {
		if (cur_entry->session_id == ssid) {
			found = true;
			break;
		}

		pre_entry = cur_entry;
		cur_entry = cur_entry->next;
	}

	if (!found || cur_entry == NULL) {
		return 0;
	}

	/* delete the cur_entry from list */
	pre_entry->next = cur_entry->next;

	/* free cur_entry */
	if (cur_entry->buffer) {
		free(cur_entry->buffer);
		cur_entry->buffer = NULL;
	}
	free(cur_entry);
	cur_entry = NULL;
	(void)cur_entry;

	return 0;
	/*NOTICE: if uuid-pool is null, we dont free it*/
}

struct socket_buffer_entry_t *socket_get_entry(struct socket_buffer_pool_t *pool, TEEC_UUID uuid,
		uint32_t ssid)
{
	struct socket_buffer_pool_t *cur_pool;
	struct socket_buffer_entry_t *cur_entry;
	bool found;

	if (NULL == pool) {
		return NULL;
	}

	/* find uuid in pool */
	found = false;
	cur_pool = pool;

	while (cur_pool) {
		if (memcmp(&cur_pool->service_id, &uuid, sizeof(TEEC_UUID)) == 0) {
			found = true;
			break;
		}

		cur_pool = cur_pool->next;
	}

	if (!found || cur_pool == NULL) {
		return NULL;
	}

	/* find ssid in uuid-pool */
	found = false;
	cur_entry = cur_pool->entry;
	while (cur_entry) {
		if (cur_entry->session_id == ssid) {
			found = true;
			break;
		}

		cur_entry = cur_entry->next;
	}

	if (!found)
		return NULL;

	return cur_entry;
}

uint8_t *socket_get_whole_buffer(struct socket_buffer_pool_t *pool,
				 TEEC_UUID uuid, uint32_t ssid, uint32_t *len)
{
	struct socket_buffer_entry_t *entry;

	entry = socket_get_entry(pool, uuid, ssid);
	if (entry) {
		*len = entry->length;
		return entry->buffer;
	} else {
		return NULL;
	}
}

uint8_t *socket_get_part_buffer(struct socket_buffer_pool_t *pool,
				TEEC_UUID uuid, uint32_t ssid, uint32_t *len)
{
	struct socket_buffer_entry_t *entry;
	uint8_t *tmp;

	entry = socket_get_entry(pool, uuid, ssid);
	if (entry) {
		if (*len > entry->length - entry->pos)
			*len = entry->length - entry->pos;

		tmp = entry->buffer + entry->pos;
		entry->pos += *len;
		return tmp;
	} else {
		return NULL;
	}
}

int socket_update_buffer_length(struct socket_buffer_pool_t *pool,
				TEEC_UUID uuid, uint32_t ssid, uint32_t len)
{
	struct socket_buffer_entry_t *entry;

	entry = socket_get_entry(pool, uuid, ssid);
	if (entry) {
		entry->length = len;
		return 0;
	} else {
		return -1;
	}
}

int socket_free_mem(struct socket_buffer_pool_t **pool)
{
	struct socket_buffer_pool_t *cur_pool, *pre_pool;
	struct socket_buffer_entry_t *cur_entry, *pre_entry;
	bool last_entry, last_pool;

	if (NULL == pool || NULL == *pool) {
		return 0;
	}

	/* find uuid in pool */
	while (*pool) {
		/* find the last uuid-pool */
		pre_pool = *pool;
		cur_pool = *pool;

		while (cur_pool->next) {
			pre_pool = cur_pool;
			cur_pool = cur_pool->next;
		}

		/* is last pool? */
		last_pool = false;

		if (cur_pool == pre_pool) /* there is only one pool */
			last_pool = true;

		while (cur_pool->entry) {
			/* find the last uuid-entry */
			pre_entry = cur_pool->entry;
			cur_entry = cur_pool->entry;

			while (cur_entry->next) {
				pre_entry = cur_entry;
				cur_entry = cur_entry->next;
			}

			/* is last entry? */
			last_entry = false;

			if (cur_entry == pre_entry) /* there is only one entry */
				last_entry = true;

			/* delete the cur_entry from the list */
			if (!last_entry)
				pre_entry->next = NULL;
			else
				cur_pool->entry = NULL;

			/* free the entry */
			if (cur_entry->buffer) {
				free(cur_entry->buffer);
				cur_entry->buffer = NULL;
			}

			free(cur_entry);
			cur_entry = NULL;

		}

		/* delete cur_pool from list */
		if (!last_pool)
			pre_pool->next = NULL;
		else
			*pool = NULL;

		/* free the cur_pool */
		free(cur_pool);
		cur_pool = NULL;
	}

	return 0;
}
