/*
 * =====================================================================================
 *
 *	Filename:  ads_debug.c
 *
 *	Description:  
 *
 *	Version:  1.0
 *	Created:  2015年06月29日 11时06分28秒
 *	Revision:  none
 *	Compiler:  gcc
 *
 *	Author:  SY Zhang (SY), zsy8311091@163.com
 *	Company:  hangzhou thinewtec technology co.,ltd
 *
 * =====================================================================================
 */

#ifndef __DVB_ADS_DEBUG_H__
#define __DVB_ADS_DEBUG_H__


enum
{
	MAIN,
	HANDLE,
	DVBAD,
	MAX_MODULE_CNT
};

#define DISABLE     0   /*disable debug out put*/
#define EMERG	    1   /* system is unusable                   */
#define ERR	    	2   /* error conditions                     */
#define WARNING	    3   /* warning conditions                   */
#define NOTICE	    4   /* normal but significant condition     */
#define INFO	    5   /* informational 			*/
#define MEM_ERR     6   /* mem alloc error 			*/

#define DVB_DEBUG

#ifdef DVB_DEBUG

#define DVB_ADS_DEBUG(module,level,...)		do{			\
		printf("\n---[fun:%s]---[line:%d]---",__FUNCTION__,__LINE__);	\
		printf("[module:%d]---[level:%d]---",module,level);	\
		printf(__VA_ARGS__);\
	}while(0)
#define DVB_ADS_ASSERT(condition)		do{			\
		if(condition != TRUE){					\
			printf("\n---[fun:%s]---[line:%d]---",__FUNCTION__,__LINE__);\
			printf("[condition : %s ]\n",#condition);\
			printf("\n");	\
		}	\
	}while(0)
	
#define DVB_ADS_CHECK_RET(module,ret)     do{	\
		if(ret < 0){			\
			DVB_ADS_DEBUG(module,ERR,"RET value err :%d\n",(int)ret);\
		}	\
	}while(0)

#define DVB_ADS_DUMP(str, ptr, size)                                                                    \
		do {																		\
			int i;																	\
			printf("\t\%s  len=%d\n\t", (str), (size)); 	\
			if (size != 0) {								\
				for (i = 0; i < (size); i++) {				\
			printf("0x%02x,", (ptr)[i]);			\
				}											\
				printf("\n\t\n");							\
			}												\
		} while (0)

#define DVB_ADS_PRINTF(...)					printf(__VA_ARGS__)	

#else

#define DVB_ADS_DEBUG(module,level,fmt,...)
#define DVB_ADS_ASSERT(condition)
#define DVB_ADS_CHECK_RET(modlule,ret)
#define DVB_ADS_DUMP(str,ptr,size)
#define DVB_ADS_PRINTF(...)

#endif
#endif //__DVB_ADS_DEBUG_H__
