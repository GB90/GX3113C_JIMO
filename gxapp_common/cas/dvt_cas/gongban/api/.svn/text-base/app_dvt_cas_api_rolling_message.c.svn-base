/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_rolling_message.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.11.29		  zhouhm 	 			creation
*****************************************************************************/
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_pop.h"
#include "app_dvt_cas_api_rolling_message.h"
#include "gui_core.h"

GxTime stoptime={0};
GxTime starttime={0};
int32_t sOsdLoopCount = 0;
extern uint8_t app_win_is_full_window(void);

extern void dvt_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
static dvt_cas_rolling_message_st ca_rolling_msg;
int8_t app_dvt_cas_rolling_message_init(void)
{
	memset(&ca_rolling_msg,0,sizeof(dvt_cas_rolling_message_st));
	return 0;
}

int32_t app_dvt_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	dvt_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (dvt_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;

	memcpy(&ca_rolling_msg,rolling_msg,sizeof(dvt_cas_rolling_message_st));

	dvt_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg.content);
	ca_rolling_msg.roll_status=FALSE;
	sOsdLoopCount = 0;
	memset(&stoptime,0,sizeof(GxTime));
	memset(&starttime,0,sizeof(GxTime));

			
	return 1;
}

int32_t app_dvt_cas_rolling_message_exec(void)
{

	if (TRUE == ca_rolling_msg.wStatus)
		{
			/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
			if (FALSE == ca_rolling_msg.roll_status)
				{
				
					if (1 == app_win_is_full_window())
						{
							printf(" roll 1 ca_rolling_msg.content=%s\n",ca_rolling_msg.content);
							dvt_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg.content);
							ca_rolling_msg.roll_status = TRUE;	
							GxCore_GetTickTime(&starttime);						
						}
				}
			else
				{
					if (1 != app_win_is_full_window())
						{
							dvt_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
							ca_rolling_msg.roll_status = FALSE;
							return TRUE;
							
						}

					GUI_GetProperty("win_full_screen_text_roll_bottom","times",&sOsdLoopCount);					
					if (sOsdLoopCount < 1)
					{					
						;
					}
					else
					{
						if (sOsdLoopCount >=1)
							{
							
//								printf("%s hide 1\n",__LINE__);
								dvt_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
								ca_rolling_msg.roll_status = FALSE;
								ca_rolling_msg.wStatus = FALSE;
								sOsdLoopCount = 0;
								// 计算出本次OSD滚动所用的时间
								
								GxCore_GetTickTime(&stoptime);
								printf("stoptime.seconds - starttime.seconds=%d\n",stoptime.seconds - starttime.seconds);
								DVTCASTB_ShowOSDMsgOver(stoptime.seconds - starttime.seconds);
								memset(&stoptime,0,sizeof(GxTime));
								memset(&starttime,0,sizeof(GxTime));							
							}

					}					
				}
		}
	else
		{
			if (TRUE == ca_rolling_msg.roll_status)
				{
				
//					printf("%s hide 2\n",__LINE__);
					dvt_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
					ca_rolling_msg.roll_status = FALSE;	
					ca_rolling_msg.wStatus = FALSE;
					sOsdLoopCount = 0;
					memset(&stoptime,0,sizeof(GxTime));
					memset(&starttime,0,sizeof(GxTime));

				}
		}

	return TRUE;

}





