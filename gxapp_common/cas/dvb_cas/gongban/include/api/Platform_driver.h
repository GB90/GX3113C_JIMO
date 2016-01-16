#ifndef _IOCARD___H_
#define _IOCARD___H_


#ifndef HAVE_SYSDB_RET_TYPE
typedef enum {
	SYSDB_RET_DEFRAG = 1,
	SYSDB_RET_SUCCESS = 0,
	SYSDB_RET_FAIL = -1,
	SYSDB_RET_FULL = -2,
	SYSDB_RET_PARAM_ERROR = -3,
	SYSDB_RET_SPACE_NOT_FOUND = -4,
}SYSDB_RET_TYPE;
#endif

int PlatfromInit(void);

unsigned int PlatfromInitFlashAddr(unsigned int need_size);

SYSDB_RET_TYPE PlatformErasermFlash(unsigned int addr);

SYSDB_RET_TYPE PlatformErasermFlashMiniSector(unsigned int addr,unsigned int size);

SYSDB_RET_TYPE PlatformReadFlash(unsigned int addr,unsigned char * buff, unsigned int size);

SYSDB_RET_TYPE PlatformWriteFlash(unsigned int addr,unsigned char * buff, unsigned int size);

SYSDB_RET_TYPE PlatformMemcpyFlash2Ram(void* dest,unsigned int addr, unsigned int size);

int PlatformSendApduToCard( unsigned char *buff,unsigned int size);

int PlatformGetApduFromCard( unsigned char *out_buff, unsigned int* size);


int CardSendApduToPlatform( unsigned char *buff,unsigned int size);


int CardGetApduFromPlatform( unsigned char *out_buff, unsigned int* size);

void CardCleanSendFifo(void);

void CardPushByteToSendFifo(unsigned char byte);

void CardSendApduFifoToPlatform(void);

void PlatformPrintf(const char* fmt,...);

typedef void(*code_entry)(void*);
void PlatformTaskCreate(code_entry func);

int PlatformGetUid(void);

int PlatformGetSpid(void);

/* used to init ca modul */
void DVBCACardInit(void);
#endif

