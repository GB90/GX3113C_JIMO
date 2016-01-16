/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_pop.h"
#include "app_cd_cas_3.0_api_updating.h"
#include "app_common_epg.h"

	
u_int8_t g_byProgress = 0;
u_int8_t g_byMark = 0;
extern 	void win_cd_cas30_cardupdating_message_show(u_int8_t byProgress,u_int8_t byMark);
int32_t app_cd_cas30_gxmsg_ca_on_event_cardupdating(GxMsgProperty0_OnEvent* event)
{
	int32_t BarTmp=0;
	u_int32_t config = 0;
	struct tm LocalTime;
	uint8_t buf[50] = {0};


	cd_cas30_card_update_st* card_update = NULL;
	if (NULL == event)
		return 0;
	
	card_update = (cd_cas30_card_update_st*)(event->buf);
	
	if (NULL == card_update)
		return 0;
		
	BarTmp = card_update->byProgress;
	g_byProgress = card_update->byProgress;
	g_byMark = card_update->byMark;

	if (card_update->byProgress > 100)
	{
		/*进度值大于100，关闭或隐藏进度条显示*/
		win_cd_cas30_cardupdating_message_show(g_byProgress,g_byMark);
	}
	else
	{
		BarTmp = card_update->byProgress ;
		win_cd_cas30_cardupdating_message_show(g_byProgress,g_byMark);
		if ((100 == card_update->byProgress)&&(CDCA_SCALE_PATCHING == card_update->byMark))
		{	
			config = 2; /*升级成功*/
			GxBus_ConfigSetInt(CARD_UPDATE_FLAG,config);
			app_epg_get_local_time(&LocalTime,TRUE);
			sprintf((void*)buf,
			"%04d-%02d-%02d %02d:%02d",
			LocalTime.tm_year,
			LocalTime.tm_mon,
			LocalTime.tm_mday,
			LocalTime.tm_hour,
			LocalTime.tm_min);
			GxBus_ConfigSet(CARD_UPDATE_TIME, (const char*)buf);
			g_byMark = 0;
		}
		else
		{
			/*记录升级过程中的状态
			* 升级过程中，卡通讯用低速模式
			*
			*/
			config = 1;
			GxBus_ConfigSetInt(CARD_UPDATE_FLAG,config);
		}
	}
	
	return 1;
}





