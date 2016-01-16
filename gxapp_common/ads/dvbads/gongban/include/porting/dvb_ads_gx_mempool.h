#ifndef __GX_PLAYER_MEMPOOL_H__
#define __GX_PLAYER_MEMPOOL_H__

#include "dvb_os_mempool_struct.h"

dvb_mempool *dvb_mempool_create(size_t block_size, size_t block_number, char* buffer);
void *dvb_mempool_alloc(dvb_mempool* pool, size_t size);
void dvb_mempool_free(dvb_mempool* pool, void *buffer, size_t size);
void dvb_mempool_destroy(dvb_mempool* pool);

#endif

