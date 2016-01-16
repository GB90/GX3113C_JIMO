#include "Platform_driver.h"
#include <gxcore.h>
#include "gx_nvram.h"

#define DVBCA_PLATFORM_QUEUE_DEEPTH			5
#define DVBCA_PLATFORM_QUEUE_DATA_SIZE		512
#define DVBCA_PLATFORM_FLASH_SIZE			(64*1024)
#define DVBCA_PLATFORM_FILE_NAME			"tnt_card_info.dat"

typedef struct {
	unsigned char card_send_fifo[512];
	unsigned int card_send_fifo_valid;

	int queue_card2app;
	int queue_app2card;
	int thread;
	int nvram;
}CASCAMPlatformControlClass;

static CASCAMPlatformControlClass thiz_control;

extern int get_card_id(char *buffer, unsigned int bytes);
extern int get_provider_id(char *buffer, unsigned int bytes);

/*virtul slot from card to app*/


int PlatformSendApduToCard( unsigned char *buff,unsigned int size)
{
	int ret;
	ret = GxCore_QueuePut(thiz_control.queue_app2card,buff,size,1000);
	if (ret != 0)
	{
		printf("%s():%d  ret[%d]\n",__FUNCTION__,__LINE__,ret);
		return ret;
	}
	return size;
}

int PlatformGetApduFromCard( unsigned char *out_buff, unsigned int* size)
{
	int ret;
	ret = GxCore_QueueGet(thiz_control.queue_card2app,out_buff,DVBCA_PLATFORM_QUEUE_DATA_SIZE,size,3000);
	if (ret != 0)
	{
		printf("%s():%d  ret[%d]\n",__FUNCTION__,__LINE__,ret);
		*size = 0;
	}
	return *size;
}


static int _CardSendApduToPlatform( unsigned char *buff,unsigned int size)
{
	int ret =GxCore_QueuePut(thiz_control.queue_card2app,buff,size,1000);
	if (ret != 0)
	{
		printf("%s():%d  ret[%d]\n",__FUNCTION__,__LINE__,ret);
		return ret;
	}
	return size;
}

// for cas lib
int PlatfromInit(void)
{
	int ret;
	ret = GxCore_QueueCreate(&thiz_control.queue_card2app, DVBCA_PLATFORM_QUEUE_DEEPTH, DVBCA_PLATFORM_QUEUE_DATA_SIZE);
	if (ret != 0)
		return -1;
	ret = GxCore_QueueCreate(&thiz_control.queue_app2card, DVBCA_PLATFORM_QUEUE_DEEPTH, DVBCA_PLATFORM_QUEUE_DATA_SIZE);
	if (ret != 0)
		return -1;

	thiz_control.nvram = GxNvram_Open(DVBCA_PLATFORM_FILE_NAME, DVBCA_PLATFORM_FLASH_SIZE);
	return 0;
}


int CardGetApduFromPlatform( unsigned char *out_buff, unsigned int* size)
{
	int ret = GxCore_QueueGet(thiz_control.queue_app2card, out_buff, DVBCA_PLATFORM_QUEUE_DATA_SIZE, size, 3000);
	if (ret != 0)
	{
		*size = 0;
	}
	return *size;
}

void CardPushByteToSendFifo(unsigned char byte)
{
	thiz_control.card_send_fifo[thiz_control.card_send_fifo_valid] = byte;
	thiz_control.card_send_fifo_valid ++;
}

void CardSendApduFifoToPlatform(void)
{
	_CardSendApduToPlatform(thiz_control.card_send_fifo, thiz_control.card_send_fifo_valid);
	thiz_control.card_send_fifo_valid = 0;
}

/* flash operate functions */

SYSDB_RET_TYPE PlatformErasermFlashMiniSector(unsigned int addr,unsigned int size)
{
	unsigned char * pBuff = GxCore_Mallocz(size);
	unsigned int ret;
	if (pBuff == NULL)
		return SYSDB_RET_FAIL;
	memset(pBuff,0xFF,size);
	ret = GxNvram_Write(thiz_control.nvram,addr,pBuff,size);
	if (ret == size)
		return SYSDB_RET_SUCCESS;
	return SYSDB_RET_FAIL;
}

SYSDB_RET_TYPE PlatformReadFlash(unsigned int addr,unsigned char * buff, unsigned int size)
{
	unsigned int ret;
	ret = GxNvram_Read(thiz_control.nvram,addr,buff,size);
	if (ret == size)
		return SYSDB_RET_SUCCESS;
	return SYSDB_RET_FAIL;
}

SYSDB_RET_TYPE PlatformWriteFlash(unsigned int addr,unsigned char * buff, unsigned int size)
{
	unsigned int ret;
	ret = GxNvram_Write(thiz_control.nvram,addr,buff,size);
	if (ret == size)
		return SYSDB_RET_SUCCESS;
	return SYSDB_RET_FAIL;
}

SYSDB_RET_TYPE PlatformMemcpyFlash2Ram(void* dest,unsigned int addr, unsigned int size)
{
	return PlatformReadFlash(addr, dest, size);
}



//#define TEST_ID
int PlatformGetUid(void)
{
#ifdef TEST_ID
	return 80000030;
#else
	char buf[18] = {0};
	if (get_card_id(buf,8) == 0)
		return atoi(buf);
	return 80000000;
#endif

}

int PlatformGetSpid(void)
{
#ifdef TEST_ID
	return 1190;
#else
	char buf[18] = {0};
	if (get_provider_id(buf,16) == 0)
		return atoi(buf);
	return 1190;
#endif

}

void PlatformPrintf(const char* fmt,...)
{
#if 1
	va_list args;
	int i;
	char buf[256];

	if(NULL == fmt)
		return ;

	va_start(args, fmt);
	i = vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	printf("%s", buf);
#endif
}

void PlatformTaskCreate(code_entry func)
{
	GxCore_ThreadCreate("dvbca_card", &thiz_control.thread, func, NULL, 16*1024, 10);
}



