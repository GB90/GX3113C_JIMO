/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	yg_cas_porting_demux.c
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
	
#include "gyca_def.h"
#include "gyca_pub.h"
#include "gyca_osp.h"
#include "gyca_interface.h"
#include "gyca_porting.h"


typedef struct 
{
	int 		ReqId;
	unsigned short int pid;
	char* 	        Section;
	size_t 			Size;
}CAS_SecitonData_Mess_t;


void GYCA_NotifyTable
(
	handle_t		Filter, 
	const uint8_t* 	Section,
	size_t 			Size
)
{
	uint16_t			filterID;
	uint16_t			i;
	int16_t 		   pid;
	uint16_t			section_length;
	bool bFlag = FALSE;
	uint8_t byReqID = 0;
	uint8_t*			data = (uint8_t*)Section;
	int 				len = Size;
	ca_filter_t* sfilter = NULL;
	ca_filter_t  temp_sfilter = {0};
	int32_t ret;
	int j=0;

		//ASSERT(Section != NULL);
	ret = GxDemux_FilterGetPID(Filter,&pid);

	while(len > 0) 
	{
		if(app_porting_ca_section_filter_crc32_check(data))
		{
			break;
		}
				bFlag = FALSE;
				section_length = ((data[1] & 0x0F) << 8) + data[2] + 3;
		if(j == 0)
		{
			for (filterID = 0; filterID< MAX_FILTER_COUNT; filterID++)
			{
				  sfilter = app_porting_ca_demux_get_by_filter_id(filterID,&temp_sfilter);
				if (( 0 !=temp_sfilter.usedStatus)&&(pid == temp_sfilter.pid)
					&&(0 !=temp_sfilter.channelhandle))
				{
					if ((0x10 == pid)||(Filter == temp_sfilter.handle))
						{
							/*NIT表或相应FILTER数据*/
							for (i = 0; i<temp_sfilter.filterLen; i++)
							{
								if ((data[i]&temp_sfilter.mask[i]) != (temp_sfilter.match[i]&temp_sfilter.mask[i]))
									{
										//CAS_Dbg("data[%d]=0x%x cafilter[filterID].match[%d]=0x%x \n",
										//	i,data[i],i,sfilter->match[i]);
										return;
									}
							}

							//sfilter->nms = 0;
							byReqID = temp_sfilter.byReqID;
							bFlag = TRUE;
							break;
						}
				}
			}

			if (FALSE == bFlag)
			{
				CA_FAIL(" FALSE == bFlag\n");
				return;
			}

			//GxDemux_FilterDisable(Filter);
			if(byReqID == 3)
			{
				//CAS_Dbg("*********GYCA_TableReceived pid=0x%x byReqID=%d,len=%d**********\n",pid,byReqID,section_length);
				//CAS_DUMP("data===",data,10);
			}
			GYCA_TableReceived(byReqID,TRUE,pid,data,section_length);
		}
		j ++;
		data += section_length;
		len -= section_length;
	 }
		return;

		
}

void GYCA_NotifyTableTimeout
(
	handle_t		Filter, 
	const uint8_t* 	Section,
	size_t 			Size
)
{
		
	uint8_t filterId;
	int16_t pid;
	ca_filter_t* filter = NULL;
	ca_filter_t  temp_sfilter = {0};
	int32_t ret;

		//ASSERT(Section != NULL);
	ret = GxDemux_FilterGetPID(Filter,&pid);

	for (filterId= 0; filterId< MAX_FILTER_COUNT; filterId++)
	{
		filter = app_porting_ca_demux_get_by_filter_id(filterId,&temp_sfilter);
		if (( 0 != temp_sfilter.usedStatus)&&(0 != temp_sfilter.handle)
			&&(0 != temp_sfilter.channelhandle)&&(Filter == temp_sfilter.handle)&&(pid == temp_sfilter.pid))
			{	
			//	pid = filter->pid;
				if((temp_sfilter.byReqID == 1)||(temp_sfilter.byReqID == 2))
				{
					//CAS_Dbg("app_desai_cas50_filter_timeout_notify filterId=%d	pid = %d filter.byReqID = %d\n",
					//	filterId,pid,filter->byReqID);
				}
				GYCA_TableReceived(temp_sfilter.byReqID,FALSE, pid,NULL, 0);			
			}
	}
	return;

}


static GYU8 GYCA_StopFilter(GYU8 enDataID, GYU16 usPID)
{
	uint32_t filterId = 0;
	bool releaseChannel = TRUE;
	ca_filter_t* sfilter = NULL;
	ca_filter_t  temp_sfilter = {0};

	/*
	* 查找对应channel id , filter id
	*/
	
	for (filterId = 0; filterId< MAX_FILTER_COUNT;filterId++)
	{
		sfilter = app_porting_ca_demux_get_by_filter_id(filterId,&temp_sfilter);
		if (( 0 != temp_sfilter.usedStatus)&&(0 != temp_sfilter.handle)&&(0 != temp_sfilter.channelhandle)
			&&(usPID == temp_sfilter.pid)&&(enDataID == temp_sfilter.byReqID))
		{
			if (0x10 == usPID)
			{
				releaseChannel = FALSE;
			}
			else
			{
				releaseChannel = TRUE;
			}

			  app_porting_ca_demux_release_filter( filterId, releaseChannel);
			  return TRUE;

		}
	}

	return TRUE;
}



/***********************************************************************************************
** 	函数名称:GYCA_TableStart
************************************************************************************************
**	提 供 者：		机顶盒。
**	使 用 者：		CA模块。
**	功能描述：	 	启动过滤器去接收CA的数据。
**	输入参数：	 	bRequestID，	为以下的几个在值之一。
** 						GYCAS_STB_FILTER_0，GYCAS_STB_FILTER_1，GYCAS_STB_FILTER_2，
** 						GYCAS_STB_FILTER_3，GYCAS_STB_FILTER_4；
**		 				pbFilterMatch，	filter的数据。
**		 				pbFilterMask，	filter的mask。
**		 				bLen，		filter的长度。
**		 				wPid，		通道的PID值。
**		 				bWaitTime，	通道过期时间，即从多长时间内通道无法接收到与
**	输出参数：	 	无。
**	返 回 值：	 	SUCCESS：成功， FAILURE： 失败。
**	其他说明：		filter match与filter mask的关系：每个filter match的位对应有一个
**						filter mask的位。如果相应的filter mask的位为1，则表示该
**						filter match位需要匹配。如：filter match中的一个字节为：
**						0x5a，	filter mask 中相应的一个字节为：0x55，则过滤器中
**						该filter match字节起作用的仅仅为第0、2、4、6位，即须匹配
**						0、0、1、1。另外，filter match和filter mask不包含table中的第1、
**						第2字节（从0开始计数），因为这两个字节在DVB中的定义
**						比较特殊。详细看13818中Table的格式。
***************************************************************************************************/
GYS32 GYCA_TableStart
(
	GYU8 bRequestID,
  	const GYU8* pbFilterMatch,
  	const GYU8* pbFilterMask,
  	GYU8 bLen,
  	GYU16 wPid,
  	GYU8 bWaitTime
)
{
	ca_filter_t filter = {0};
	bool repeatFlag = TRUE;
	uint8_t 	match[18] = {0,};
	uint8_t 	mask[18] = {0,};
	uint16_t	 i=0;
	uint8_t    bRet = 0;
	static handle_t  mutex_id = 0;
	if(mutex_id == 0)
	{
		GxCore_MutexCreate(&mutex_id);
	}
	if(bRequestID == 3)
	{
		//CAS_Dbg("GYCA_TableStart bRequestID=0x%x,wPid=0x%x bLen=0x%x,bWaitTime=%d\n",
		//bRequestID,wPid,bLen,bWaitTime);
	}
	if(wPid == 0x1fff)
	{
		CA_FAIL("Wrong PID 0x%x!!\n",wPid); 			
	}
	if(pbFilterMatch == NULL || pbFilterMask == NULL )
	{
		CA_FAIL("DSCA_SetFilter pbyFilter=0x%x	pbyMask=0x%x\n",(unsigned int)pbFilterMatch,(unsigned int)pbFilterMask); 			
		return 0;
	}
	if(bLen > 16)
	{
		CA_FAIL("Filter depth is too long %d!!\n",bLen); 			
		return 0;
	}

/*	if (0x80 ==( 0x80&byReqID))
	{
		repeatFlag = FALSE;
	}*/

//		CAS_Dbg("DSCA_SetFilter enDataID=0x%x,usPID=0x%x ucFilterLen=0x%x,ucWaitSeconds=%d\n",
//		enDataID,usPID,ucFilterLen,ucWaitSeconds);

//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyFilter", pbyFilter , byLen);
//	CAS_DUMP("BYSTBCA_SetStreamGuardFilter pbyMask", pbyMask , byLen);

	GxCore_MutexLock(mutex_id);
	for (i = 0; i< bLen; i++)
	{
		if (0 == i)
		{
			match[i]	 = pbFilterMatch[i];
			mask[i] = pbFilterMask[i];
		}
		else
		{
			match[i+2]	 = pbFilterMatch[i];
			mask[i+2] = pbFilterMask[i];
		}
	}
	if(wPid == 0x21)
	{
		filter.crcFlag = TRUE;
	}
	GYCA_StopFilter(bRequestID, wPid);
	
	if (1 == bRequestID)
	{
		//repeatFlag = FALSE;
	}

	filter.byReqID = bRequestID;
//	filter.crcFlag = FALSE;
	filter.crcFlag = TRUE;
	filter.filterLen = bLen+2;
	if(filter.filterLen <= 3)
	{
		filter.filterLen = 4;
	}
	memcpy(filter.match,match,bLen+2);
	memcpy(filter.mask,mask,bLen+2);
	filter.nWaitSeconds = bWaitTime;
	filter.pid = wPid;
	filter.equalFlag = TRUE;
	filter.repeatFlag = repeatFlag;

	filter.Callback = GYCA_NotifyTable;
	filter.timeOutCallback = GYCA_NotifyTableTimeout;
	bRet = app_porting_ca_demux_start_filter(&filter);
	if(bRet)
	{
		//CAS_Dbg("%s.%d>>exit\n",__FUNCTION__,__LINE__);
	}
	else
	{
		CAS_Dbg("%s.%d>>exit\n",__FUNCTION__,__LINE__);
	}
	
	GxCore_MutexUnlock(mutex_id);
	return bRet;

}




/***********************************************************************************************
** 	函数名称:GYCA_SetCW
************************************************************************************************
**	提 供 者：		机顶盒。
**	使 用 者：		CA模块。
**	功能描述：	 	设置解扰通道的控制字。
**	输入参数：
** 						wEcmPid，		控制字的所在的ecm包的PID值。
**						szOddKey，		奇控制字数据的指针。
** 						szEvenKey，		偶控制字数据的指针。
 **						bKeyLen，		控制字长度。
** 						bReservedFlag,	保留。
**	输出参数：		无。
** 	返 回 值：	 	无。
**	其他说明：
*************************************************************************************************/
void GYCA_SetCW 
(  
	GYU16 wEcmPid,
 	const GYU8* szOddKey,
	const GYU8* szEvenKey,
	GYU8 bKeyLen,
	GYU8 bReservedFlag
)
{

	if((wEcmPid==0)||(bKeyLen==0)||(szOddKey==NULL)||(szEvenKey==NULL))
		return;
	
	app_porting_ca_descramble_set_cw(wEcmPid,
							  szOddKey,
							  szEvenKey,
							  bKeyLen,
							  0);	
}
/**/
