#include "string.h"
#include "dvb_os_mempool_struct.h"
#include "dvb_ads_debug.h"
#include "app_common_porting_ca_os.h"
#include "dvb_ads_drv.h"


extern RET_CODE dvb_ads_mempool_create(dvb_mempool* mem,u32 block_size,u32 block_number,u8*buffer);
extern void* dvb_ads_mempool_alloc(dvb_mempool* pool,u32 size);
extern RET_CODE dvb_ads_mempool_free(dvb_mempool *pool,void* buff,u32 size);
extern RET_CODE dvb_ads_mempool_destroy(dvb_mempool *pool);
extern int32_t  GxCore_ThreadDelay        (uint32_t millisecond);


dvb_mempool* dvb_mempool_create(size_t block_size, size_t block_number, char* buffer)
{
	block_size += 4;
	int i;
	int mem_size = block_size * block_number;
	dvb_mempool* pool;

	dvb_node* chunk;

	if(buffer == NULL){
		pool = (dvb_mempool*)app_porting_ca_os_malloc(sizeof(dvb_mempool) + mem_size);
		memset(pool,0,sizeof(dvb_mempool)+mem_size);
		if(!pool)
			return NULL;

		pool->buffer = (char*)pool + sizeof(dvb_mempool);
	}
	else{
		pool = (dvb_mempool*)app_porting_ca_os_malloc(sizeof(dvb_mempool));
		memset(pool,0,sizeof(dvb_mempool));
		if(!pool)
			return NULL;

		pool->buffer = buffer;
	}

	pool->block_number   = block_number;
	pool->block_size     = block_size;
	pool->buffer_end     = pool->buffer + mem_size;
	pool->max_need_count = 0;
	pool->alloc_count    = 0;
	pool->free_count     = block_number;
	DVB_ADS_PRINTF("\n");
	DVB_ADS_DEBUG(DVBAD,INFO,"Info of the mempool alloced:");
	DVB_ADS_DEBUG(DVBAD,INFO,"\tPool Start Addr:			\t0x%x",(unsigned int)pool);
	DVB_ADS_DEBUG(DVBAD,INFO,"\tPool manage size:			\t%d",sizeof(dvb_mempool));
	DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock Start Addr:			\t0x%x",(unsigned int)pool->buffer);
	DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock End Addr:			\t0x%x",(unsigned int)pool->buffer_end);
	DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock size:				\t%d",block_size);
	DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock number:			\t%d",block_number);
	DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock free/used(blocks):	\t\t%d/%d",pool->free_count,pool->alloc_count);
	DVB_ADS_PRINTF("\n");
	
	chunk = (dvb_node* )pool->buffer;
	pool->chunk = chunk;

	for (i = 0; i < block_number-1; i++) {
		chunk->next =(dvb_node*)((char*) chunk + block_size);
		chunk = chunk->next;
	}

	chunk->next = NULL;

	app_dvb_ads_sem_create(&pool->mutex,0);

	return pool;

}

void* dvb_mempool_alloc(dvb_mempool *pool, size_t size)
{
	return app_porting_ca_os_malloc(size);
		
	#if 0	
	char *mem = NULL;
	if (size <= 0)
	{
		return NULL;
	}
	else if(size > (DVB_POOL_BLOCK_SIZE - 4))
	{
		mem = (char*)app_porting_ca_os_malloc(size);
		mem[size + 3] = 0xa1;
		DVB_ADS_DEBUG(DVBAD,INFO,"size > block_size,alloc out of the mempool!!!");
	}
	else if((NULL != pool) && (size <= (DVB_POOL_BLOCK_SIZE - 4)))
	{
		app_dvb_ads_sem_take(pool->mutex,10);
		if ((pool->chunk != NULL) && (pool->free_count > 0))
		{
			mem = (char*)pool->chunk;
			pool->chunk = pool->chunk->next;
			pool->free_count--;
			pool->alloc_count++;
			DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock free/used(blocks):	 %d/%d",pool->free_count,pool->alloc_count);
			mem[size + 3] = 0xa0;
		}
		else
		{
			DVB_ADS_PRINTF("Run out of the blocks of the mempoo,Alloc out of the mempool!!!");
			mem = app_porting_ca_os_malloc(size + 4);
			mem[size + 3] = 0xa1;
		}
	}
	else
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"ERR!!!");
	}
	if (mem == NULL)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"Alloc failed!!!");
		return NULL;
	}
	memset(mem, 0xbb, size);
	mem[size + 0] = 0xa0;
	mem[size + 1] = 0xa0;
	mem[size + 2] = 0xa0;
	app_dvb_ads_sem_post(pool->mutex);
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloc succeed!");
	return mem;
#endif
}

//static int a = 1;
void dvb_mempool_free(dvb_mempool* pool, void *buffer, size_t size)
{
	if(NULL != buffer)
		app_porting_ca_os_free(buffer);
	return;

#if 0
	dvb_node* chunk = (dvb_node *)buffer;
	char *mem = (char*)buffer;
	if (mem[size + 0] != 0xa0 || mem[size + 1] != 0xa0 || mem[size + 2] != 0xa0 ) {
			DVB_ADS_PRINTF("[BUG]: %s: buffer %p overflow. fall into endless loop\n", __func__, buffer);
			while (a) 
				GxCore_ThreadDelay(1000);
	}
	else if(size > DVB_POOL_BLOCK_SIZE-4)
	{
		if (mem[size + 3] == 0xa1)
		{
			app_porting_ca_os_free(buffer);
			memset(buffer+size,0,4);
			return;
		}
		else
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"The flag IN POOL 0xa1 was destroyed!!!");
			while (a)
				GxCore_ThreadDelay(1000);
		}
	}
	else if(pool && size <= DVB_POOL_BLOCK_SIZE-4)
	{
		app_dvb_ads_sem_take(pool->mutex,10);
		if ((char*)buffer >= pool->buffer && (char*)buffer < pool->buffer_end) {
			if (mem[size + 3] == 0xa0)
			{
				chunk->next = pool->chunk;
				pool->chunk = chunk;
				pool->free_count++;
				pool->alloc_count--;
				DVB_ADS_DEBUG(DVBAD,INFO,"\tBlock free/used(blocks)	 :  %d/%d",pool->free_count,pool->alloc_count);
			}
			else
			{
				DVB_ADS_DEBUG(DVBAD,INFO,"The flag IN POOL 0xa0 was destroyed!!!");
				while (a)
					GxCore_ThreadDelay(1000);
			}
		}
		app_dvb_ads_sem_post(pool->mutex);
		return;
	}
	else
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"ERR");
		return;
	}
		app_porting_ca_os_free(buffer);
#endif
}


void dvb_mempool_destroy(dvb_mempool* pool)
{
	if(pool){
		app_dvb_ads_sem_destroy(pool->mutex);
		app_porting_ca_os_free(pool);
	}
}
