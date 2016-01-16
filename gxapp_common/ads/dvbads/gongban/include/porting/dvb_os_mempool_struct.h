#ifndef __DVB_OS_MEMPOOL_STRUCT_H__
#define __DVB_OS_MEMPOOL_STRUCT_H__
//#include "kernelcalls.h"
#include "dvb_ads_type.h"

#ifndef size_t
#define size_t unsigned int
#endif

//typedef struct cyg_mutex_t gx_mutex_t;

typedef struct _dvb_node {
	struct _dvb_node* next;
} dvb_node;


typedef struct _dvb_mempool {
	size_t            block_size;
	size_t            block_number;
	char *            buffer;
	char *            buffer_end;
	int               max_need_count;
	int               alloc_count;
	int               free_count;
	dvb_node          *chunk;
	//gx_mutex_t	  mutex;
	os_sem_t          mutex;//exchange mutex to sem
} dvb_mempool;
#endif  //__DVB_OS_MEMPOOL_STRUCT_H__
