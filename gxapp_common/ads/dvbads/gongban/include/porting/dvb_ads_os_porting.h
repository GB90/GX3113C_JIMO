#ifndef __DVB_ADS_OS_PORTING_H__
#define __DVB_ADS_OS_PORTING_H__

#include "dvb_ads_function_porting.h"
#include "dvb_ads_gx_mempool.h"

u8 app_dvb_ads_sem_create(os_sem_t *p_sem,int mutex);

u8 app_dvb_ads_sem_destroy(os_sem_t sem);

u8 app_dvb_ads_sem_take(os_sem_t sem);

u8 app_dvb_ads_sem_post(os_sem_t sem);


RET_CODE dvb_ads_mempool_create(dvb_mempool* mem,u32 block_size,u32 block_number,u8*buffer);
void* dvb_ads_mempool_alloc(dvb_mempool* pool,u32 size);
RET_CODE dvb_ads_mempool_free(dvb_mempool *pool,void* buff,u32 size);
RET_CODE dvb_ads_mempool_destroy(dvb_mempool *pool);


int32_t dvb_ads_thread_create(char* p_taskname,
                     void (*p_taskproc)(void *p_param),
                     void  *p_param,
                     u32    nprio,
                     u32   *pstk,
                     u32    nstksize);

void dvb_ads_thread_destroy(void);

#endif //__DVB_ADS_OS_PORTING_H__
