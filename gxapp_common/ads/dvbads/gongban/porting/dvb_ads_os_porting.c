#include "dvb_ads_function_porting.h"
#include "pthread.h"
#include "app_common_porting_ca_os.h"
#include "dvb_ads_gx_mempool.h"

void dvb_mempool_destroy(dvb_mempool* pool);
//---------------------------sem--------------------------------------
u8 app_dvb_ads_sem_create(os_sem_t *p_sem,int mutex)
{
	return 	app_porting_ca_os_sem_create((handle_t*)p_sem,mutex);
}
u8 app_dvb_ads_sem_destroy(os_sem_t sem)
{
	return  app_porting_ca_os_sem_delete((handle_t)sem);
}

u8 app_dvb_ads_sem_take(os_sem_t sem)
{
	return app_porting_ca_os_sem_wait((handle_t)sem);
}

u8 app_dvb_ads_sem_post(os_sem_t sem)
{
	return app_porting_ca_os_sem_signal((handle_t)sem);
}
//--------------------------mempool-------------------------------

RET_CODE dvb_ads_mempool_create(dvb_mempool* mem,u32 block_size,u32 block_number,u8* buffer)
{
	mem = dvb_mempool_create((size_t)block_size,(size_t)block_number,(char*)buffer);
	if(NULL != mem)
	{
		return SUCCESS;
	}
	return ERR_FAILURE;
}

void* dvb_ads_mempool_alloc(dvb_mempool* pool,u32 size)
{
	//DVB_ADS_DEBUG(DVBAD,MEM_ERR,"Alloc mempool:%u ...!",(unsigned int)size);
	void *ptr = NULL;
	ptr    =   	app_porting_ca_os_malloc(size);
	
	DVB_ADS_DEBUG(DVBAD,INFO,"Alloc mempool:0x%x ...!",(unsigned int)ptr);
	return ptr;
	//return dvb_mempool_alloc(pool, (size_t)size);
}

RET_CODE dvb_ads_mempool_free(dvb_mempool *pool,void* buff,u32 size)
{
	
	if(NULL != buff)
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"The address is going to be free is 0x%x",(unsigned int)buff);
//		app_porting_ca_os_free(buff);
		GxCore_Free(buff);
		buff = NULL;		
		DVB_ADS_DEBUG(DVBAD,INFO,"The address after being freed is 0x%x",(unsigned int)buff);
		if(NULL != buff)
		{
			DVB_ADS_DEBUG(DVBAD,INFO,"free mem failed!!!");
			return ERR_FAILURE;
		}
		DVB_ADS_DEBUG(DVBAD,INFO,"free mem success!!!");
		return SUCCESS;
	}
	else
	{
		DVB_ADS_DEBUG(DVBAD,INFO,"free mem success!!!");
		return SUCCESS;
	}
	
	#if 0
	dvb_mempool_free(pool,(void*)buff,(size_t)size);
	if(NULL == buff)
	{
		return SUCCESS;
	}
	DVB_ADS_DEBUG(DVBAD,MEM_ERR,"Free mempool failed!");

	return ERR_FAILURE;
	#endif
}

RET_CODE dvb_ads_mempool_destroy(dvb_mempool *pool)
{
	dvb_mempool_destroy(pool);
	if(NULL == pool)
	{
		return SUCCESS;
	}
	DVB_ADS_DEBUG(DVBAD,MEM_ERR,"Destroy mempool failed!");
	return ERR_FAILURE;
}
//--------------------------thread-------------------------------------
int32_t dvb_ads_thread_create(char* p_taskname,
                     void (*p_taskproc)(void *p_param),
                     void  *p_param,
                     u32    nprio,
                     u32   *pstk,
                     u32    nstksize)
{
	int32_t ret = 0;
	ret = app_porting_ca_os_create_task(p_taskname,(handle_t*)pstk,p_taskproc,p_param,nstksize,nprio);
	
#if 0
	return GxCore_ThreadCreate((char*)p_taskname,(handle_t*)pstk,p_taskproc,p_param,nstksize,nprio);
#endif
	return ret;
}

void dvb_ads_thread_destroy(void)
{
	pthread_exit((void *)NULL);	//no need to pass out the return code of thread!!!
}


#if 0
extern int32_t GxCore_ThreadCreate    (const char *thread_name,
				      handle_t *thread_id,
                      void(*entry_func)(void *),
				      void *arg,
                      uint32_t stack_size,
                      uint32_t priority);
#endif
