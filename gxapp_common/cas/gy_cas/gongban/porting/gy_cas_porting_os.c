
/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_os.c
* Author    : 	bingyu
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  bingyu 	 			creation
*****************************************************************************/

#include <gxtype.h>
#include "gxmsg.h"
#include "gxapp_sys_config.h"
#include "app_common_play.h"
#include "app_common_table_pmt.h"
#include "app_common_porting_stb_api.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_os.h"

#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"



/*********************************************************************************************
** 	函数名称:GYCA_OSPTaskCreate
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	创建任务。
**	输入参数：	name[]，	4个字节的任务名称。
**	 				stackSize，	任务所使用的堆栈的大小。
**	 				entryPoint，任务的入口地址。
**	 				priority，	任务的优先级。
**	 				arg1，		传递给任务的第一个参数。
**	 				taskId，	任务的ID。	
**	输出参数：	无。
**	返 回 值：	SUCCESS：   成功；
**	 				FAILURE?   发生错误
**	其他说明：
**********************************************************************************************/
GYS32 GYCA_OSPTaskCreate
(
	char name[],
	GYU32 stackSize,
	void (*entryPoint)(void),
	GYS32 priority, 
	GYU32 arg1,
	GYU32 arg2,
	GYU32 * taskId
)
{
	int err;

	err = GxCore_ThreadCreate(name,(handle_t *)taskId,(void*)entryPoint,&arg1,(uint32_t)stackSize*20,/*(uint32_t)priority*/GXOS_DEFAULT_PRIORITY);
	CAS_Dbg("GYCA_OSPTaskCreate,name[%s],taskId[%x],stackSize[0x%x],priority[%d]\n",name,*taskId,stackSize,priority);
	if (err !=GXCORE_SUCCESS)
	{
		CA_FAIL("\n  GYCA_OSPTaskCreate  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPTaskTemporarySleep
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	暂停任务一段指定的时间。
**	输入参数：	milliSecsToWait，	终止任务的时间。（以毫秒为单位。）
**	输出参数：	无。
**	返 回 值：	SUCCESS：       	成功；
**	 				FAILURE：       	发生错误
**	其他说明：
**********************************************************************************************/
GYS32 GYCA_OSPTaskTemporarySleep(GYU32 milliSecsToWait)
{
	int err;

	err= app_porting_ca_os_sleep(milliSecsToWait);
	
	if (err != 0)
	{
		CA_FAIL("\n  GYCA_OSPTaskTemporarySleep  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPSemaphoreCreate
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	创建信号量。
**	输入参数：	name[],				4个字节的信号量名称。
**	 				initialTokenCount	初始化时信号量的数量。
**	 				taskWaitMode，		取得该信号量的方式。为以下两种之一:
**	 				GYCA_WAIT_FIFO， 按先等待者先得的方式取得。
** 					GYCA_WAIT_PRIO， 按优先级高者先得的方式取得。
**	 				semaphoreId，		该信号量的ID
**	输出参数：	无。
**	返 回 值：	SUCCESS：       	成功；
**	 				FAILURE：       	发生错误
**	其他说明：目前CA模块只用到了GYCA_WAIT_FIFO模式，如果无法实现
**	 				GYCA_WAIT_PRIO则可以暂时不实现。
**********************************************************************************************/
GYS32 GYCA_OSPSemaphoreCreate
(
	char name[4], 
	GYU32 initialTokenCount,
	GYU32 taskWaitMode,
	GYU32 *semaphoreId
)
{	
	int err;
	
	err = GxCore_SemCreate((handle_t *)semaphoreId, initialTokenCount);

	if (err !=GXCORE_SUCCESS)
	{
		CA_FAIL("\n  GYCA_OSPSemaphoreCreate  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCAS_GYCA_OSPSemaphoreGetToken
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	何时使用：	创建任务。
**	功能描述：	取得信号量。
**	输入参数：	semaphoreId,	等待信号量的方式，为以下两种之一:
**	 				GYCA_TASK_WAIT，等待信号量直到超时。
** 					GYCA_TASK_NOWAIT，不管信号量存在与否立即返回。
**	 				milliSecsToWait,等待的时间。如果waitMode为GYCA_TASK_NOWAIT，则该参数无效。
**					如果为GYCA_TASK_WAIT，则代表时间。
**	输出参数：	无。
**	返 回 值：	SUCCESS：       成功；
**	 				FAILURE：       发生错误
**	其他说明：注意区别这里的等待模式和创建信号量的等待模式之间的区别。
**					创建信号量的是指同时有多个任务在等待同一个信号量时谁优先取
**  					得信号量。到取得信号量为止还是不做任何的等待，不管是否可以
**					取得信号量，立即返回。
**********************************************************************************************/
GYS32 GYCA_OSPSemaphoreGetToken (	
	GYU32 semaphoreId,
 	GYU32 waitMode,
	GYU32 milliSecsToWait
)
{
	int err=GYFAILURE;
	
	if(waitMode==GYCA_TASK_WAIT)
	{
	    err= GxCore_SemTimedWait(semaphoreId,milliSecsToWait);
	}
	else if(waitMode==GYCA_TASK_NOWAIT)
	{
		err =GxCore_SemTimedWait(semaphoreId,1);
	}

	if (err !=GXCORE_SUCCESS)
	{
//		CA_FAIL("\n  GYCA_OSPSemaphoreGetToken  Err : semaphoreId =%d waitMode=%d err=%d \n",semaphoreId,waitMode,err);
		return GYFAILURE;
	}

	return GYSUCCESS;
	
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPSemaphoreReturnToken
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	增加一个信号量
**	输入参数：	semaphoreId,	该信号量的ID
**	输出参数：	无。
**	返 回 值：	SUCCESS：       成功；
**	 				FAILURE：       发生错误
**	其他说明：              
**********************************************************************************************/
GYS32 GYCA_OSPSemaphoreReturnToken(	GYU32 semaphoreId )
{
	int err;

	err = GxCore_SemPost(semaphoreId);
	
	if (err !=GXCORE_SUCCESS)
	{
		CA_FAIL("\n  GYCA_OSPSemaphoreReturnToken  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPMalloc
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	分配内存。
**	输入参数：	numberOfBytes，	分配的内存空间的大小（以字节为单位）。
**	输出参数：	无。
**	返 回 值：	如果为NULL，则表示分配不到内存,其他的均为成功分配。
**	其他说明：              
**********************************************************************************************/
void * GYCA_OSPMalloc(GYU32 numberOfBytes)
{
	return (void*)app_porting_ca_os_malloc(numberOfBytes);
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPFree
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	释放掉分配的内存空间。
**	输入参数：	ptrToSegment,	要释放的内存空间指针。
**	输出参数：	无。
**	返 回 值：	SUCCESS：       成功释放；
**	 				FAILURE：       释放不成功，可能是因为所传递进来的指针无效。
**	其他说明：              
**********************************************************************************************/
GYS32 GYCA_OSPFree(void *ptrToSegment)
{
	app_porting_ca_os_free(ptrToSegment);

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPQueueCreate
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	创建一个消息队列。
**	输入参数：	name[],	4个字节的队列名称。
**	 				MaxQueueLength，消息队列中可以存放的消息的数量。当消息队列中该
**                                     	数量达到该数量时，再往该消息队列发消息将会失败。
**	 				taskWaitMode，	取得消息队列中消息的方式。为以下两种之一:
**	 					GYCA_WAIT_FIFO， 按先等待者先得的方式取得。
** 						GYCA_WAIT_PRIO， 按优先级高者先得的方式取得。
**	 				queueId，消息队列的ID。
**	输出参数：	无。
**	返 回 值：	SUCCESS：       	成功；
**	 				FAILURE：       	发生错误
**	其他说明：              
**********************************************************************************************/
GYS32 GYCA_OSPQueueCreate
(
	char name[],
	GYU32 maxQueueLength,
	GYU32 taskWaitMode,
	GYU32 *queueId
)
{
	int err;
	
	err = GxCore_QueueCreate((handle_t *)queueId, maxQueueLength, sizeof(GYCA_QUEUE_MESSAGE));

	if (err !=GXCORE_SUCCESS)
	{
		CA_FAIL("\n  GYCA_OSPQueueCreate  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称:GYCA_OSPQueueGetMessage
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	从消息队列获取消息。
**	输入参数：	queueId，	所要取得的消息的ID。
**	 				waitMode，	等待消息的方式，为以下两种之一:
**	 					GYCA_TASK_WAIT，等待消息直到超时。
** 						GYCA_TASK_NOWAIT，不管消息存在与否立即返回。
**	 				milliSecsToWait，	等待的时间。如果waitMode为GYCA_TASK_NOWAIT，则该
**		 				参数无效。如果为GYCA_TASK_WAIT，则代表时间。
**	输出参数：	messsage，	为消息的格式。参照QUEUE_MESSAGE。
**	返 回 值：	SUCCESS： 成功；
**	 				FAILURE：   发生错误
**	其他说明：              
**********************************************************************************************/
GYS32 GYCA_OSPQueueGetMessage
(
	GYU32 queueId,
	GYCA_QUEUE_MESSAGE *message ,
	GYU32 waitMode,
	GYU32 milliSecsToWait
)
{
	int err = 0;
	uint32_t size_copied;

	if(waitMode==GYCA_TASK_WAIT)
	{
		err = GxCore_QueueGet(queueId, (void*)message, sizeof(GYCA_QUEUE_MESSAGE),&size_copied,milliSecsToWait);
	}
	else if(waitMode==GYCA_TASK_NOWAIT)
	{
		err = GxCore_QueueGet(queueId, (void*)message, sizeof(GYCA_QUEUE_MESSAGE),&size_copied,1);
	}
	

	if ((err !=GXCORE_SUCCESS)||(size_copied!=sizeof(GYCA_QUEUE_MESSAGE)))
	{
	//	CA_FAIL("\n  GYCA_OSPQueueGetMessage  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}

/*********************************************************************************************
** 	函数名称: GYCA_OSPQueueSendMessage
**********************************************************************************************
**	提 供 者： 	机顶盒。
**	使 用 者：	 CA模块。
**	功能描述：	发送消息到消息队列。
**	输入参数：	queueId，消息队列ID。
**	 				message，要发送的消息。其格式见QUEUE_MESSAGE结构。
**	输出参数：	无。
**	返 回 值：	SUCCESS：       	成功；
**	 				FAILURE：       	发生错误
**	其他说明：              
**********************************************************************************************/
GYS32  GYCA_OSPQueueSendMessage(GYU32 queueId,GYCA_QUEUE_MESSAGE * message)
{
	int err;

	if((message==NULL)||queueId==0)
	{
		CA_FAIL("\n  GYCA_OSPQueueSendMessage  Err : message=%p queueId=%x  \n",message,queueId);
		return GYFAILURE;
	}
	
	err =GxCore_QueuePut(queueId, message, sizeof(GYCA_QUEUE_MESSAGE),1000);
	
	if (err !=GXCORE_SUCCESS)
	{
		CA_FAIL("\n  GYCA_OSPQueueSendMessage  Err : err=%d \n",err);
		return GYFAILURE;
	}

	return GYSUCCESS;
}







