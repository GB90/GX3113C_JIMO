#ifndef __DVB_ADS_TYPE_H__
#define __DVB_ADS_TYPE_H__

//typedef unsigned char		CHAR;
typedef signed int		BOOL;
typedef void*			dvb_ads_handle_t;
typedef signed int 		os_sem_t;
//typedef signed int 		handle_t;

typedef unsigned long long	u64;
typedef signed long long	s64;
typedef unsigned long		u32;
typedef signed long			s32;
typedef unsigned short		u16;
typedef signed short		s16;
typedef unsigned char		u8;
typedef signed char			s8;

#ifndef FALSE
#define FLASE 			(0)
#endif

#ifndef TRUE
#define TRUE 			(1)
#endif

#define RET_CODE 		s32

#define SUCCESS 		((s32)0)
#define ERR_FAILURE 	((s32)-1) 
#define ERR_TIMEOUT 	((s32)-2)  
#define ERR_PARAM 		((s32)-3) 
#define ERR_STATUS 		((s32)-4)  
#define ERR_BUSY 		((s32)-5)
#define ERR_NO_MEM 		((s32)-6)
#define ERR_NO_RSRC 	((s32)-7)
#define ERR_HARDWARE 	((s32)-8) 
#define ERR_NOFEATURE 	((s32)-9)  

#define KBYTES			(1024)                                                        
#define MBYTES 			(1024*1024)

#endif //__DVB_ADS_TYPE_H__
