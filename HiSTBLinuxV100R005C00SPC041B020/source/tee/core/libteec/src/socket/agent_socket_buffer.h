#ifndef __AGENT_SOCKET_BUFFER_H_
#define __AGENT_SOCKET_BUFFER_H_

/* #include "tee_socket_debug.h" */
#ifndef TEE_SOCKET_DEBUG_ON_SERVER
#include "tee_client_api.h"
#else
typedef unsigned int     uint32_t;
typedef unsigned short    uint16_t;
typedef unsigned char     uint8_t;
typedef uint8_t bool;

#define true    1
#define false   0
#ifndef NULL
	#define  NULL   ((void *) 0)
#endif

typedef struct {
	uint32_t timeLow;
	uint16_t timeMid;
	uint16_t timeHiAndVersion;
	uint8_t clockSeqAndNode[8];
} TEEC_UUID;
#endif

struct socket_buffer_entry_t {
	struct socket_buffer_entry_t *next;
	uint32_t session_id;
	uint32_t length;
	uint32_t pos;
	uint8_t *buffer;
};

struct socket_buffer_pool_t {
	struct socket_buffer_pool_t *next;
	struct socket_buffer_entry_t *entry;
	TEEC_UUID service_id;
};

struct socket_buffer_pool_t *socket_pool_init();

struct socket_buffer_pool_t *socket_pool_new(struct socket_buffer_pool_t *pool, TEEC_UUID uuid);

struct socket_buffer_pool_t *socket_pool_add(struct socket_buffer_pool_t *pool, TEEC_UUID uuid);

struct socket_buffer_entry_t *socket_entry_init();

struct socket_buffer_entry_t *socket_entry_new(struct socket_buffer_pool_t *pool, uint32_t ssid);

struct socket_buffer_entry_t *socket_entry_add(struct socket_buffer_pool_t *pool, uint32_t ssid);

int socket_store_buffer(struct socket_buffer_pool_t *pool,
			TEEC_UUID uuid, uint32_t ssid,
			uint8_t *buffer,
			uint32_t len_part, uint32_t len_whole);

int socket_clear_buffer(struct socket_buffer_pool_t *pool,
			TEEC_UUID uuid, uint32_t ssid);

uint8_t *socket_get_whole_buffer(struct socket_buffer_pool_t *pool,
				 TEEC_UUID uuid, uint32_t ssid, uint32_t *len);

uint8_t *socket_get_part_buffer(struct socket_buffer_pool_t *pool,
				TEEC_UUID uuid, uint32_t ssid, uint32_t *len);

int socket_update_buffer_length(struct socket_buffer_pool_t *pool,
				TEEC_UUID uuid, uint32_t ssid, uint32_t len);

int socket_free_mem(struct socket_buffer_pool_t **pool);

void print_uuid(TEEC_UUID *uuid);
void print_socket_buffer_entry(struct socket_buffer_entry_t *entry);
void print_socket_buffer_pool(struct socket_buffer_pool_t *pool);


#endif /* __AGENT_SOCKET_BUFFER_H_ */

