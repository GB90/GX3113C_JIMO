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
#include "app_common_porting_stb_api.h"
#include "app_divitone_cas_api_pop.h"
#include "app_divitone_cas_api_rolling_message.h"

extern void divitone_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
extern int32_t divitone_cas_rolling_message_get_times(void);

static divitone_cas_rolling_message_st ca_rolling_msg = {0};
static char ca_osd[1024] = {0};

extern BOOL divitone_ads_get_osd_for_service_id(uint32_t service_id);
int32_t app_divitone_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	GxBusPmDataProg Prog;
	uint32_t pos = 0;
	divitone_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (divitone_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;
	if (rolling_msg->from == 1)
	{
		if (GxBus_PmProgNumGet() > 0)
		{
			app_prog_get_playing_pos_in_group(&pos);
			GxBus_PmProgGetByPos(pos,1,&Prog);
			if (FALSE ==  divitone_ads_get_osd_for_service_id(prog.service_id))
			{
				return 0;
			}
		}	
		memcpy(&ca_rolling_msg,rolling_msg,sizeof(divitone_cas_rolling_message_st));

		memset(ca_osd,0,sizeof(ca_osd));
		if(ca_rolling_msg.param2 > sizeof(ca_osd))
			ca_rolling_msg.param2 = sizeof(ca_osd);
		memcpy(ca_osd,(char *)ca_rolling_msg.param1,ca_rolling_msg.param2);
		//ca_osd[ca_rolling_msg.param2] = '\0';

		divitone_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,ca_osd);
		ca_rolling_msg.roll_status=FALSE;
		ca_rolling_msg.wStatus = TRUE;

		return 1;
	}
	int32_t app_divitone_cas_gxmsg_ca_on_event_rolling_test(void)
	{
		//char* strtemp = "DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!";
		char* strtemp = "DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!\
				 DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!DIVI CAS短消息测试，卡号 900001。你是谁:.,yzxp!";
		/*divitone_cas_rolling_message_st* rolling_msg = NULL;
		  if (NULL == event)
		  return 0;

		  rolling_msg = (divitone_cas_rolling_message_st*)(event->buf);

		  if (NULL == rolling_msg)
		  return 0;*/


		memset(ca_osd,0,sizeof(ca_osd));
		memcpy(ca_osd,(char *)ca_rolling_msg.param1,ca_rolling_msg.param2);
		ca_osd[ca_rolling_msg.param2] = 'x';
		ca_osd[ca_rolling_msg.param2+1] = 'y';
		ca_osd[ca_rolling_msg.param2+2] = 'p';
		ca_osd[ca_rolling_msg.param2+3] = '\0';

		memcpy(ca_osd,strtemp,strlen(strtemp));
		divitone_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,ca_osd);
		ca_rolling_msg.roll_status=FALSE;
		ca_rolling_msg.wStatus = TRUE;

		return 1;
	}

	int32_t app_divitone_cas_rolling_message_exec(void)
	{
		if (TRUE == ca_rolling_msg.wStatus )
		{
			if (FALSE == ca_rolling_msg.roll_status)
			{
				divitone_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,ca_osd);
				ca_rolling_msg.roll_status= TRUE;	
			}

			ca_rolling_msg.wTimes = divitone_cas_rolling_message_get_times();
			if (ca_rolling_msg.wTimes >=  1)
			{
				// 查询滚动结束，清除OSD滚动标志
				ca_rolling_msg.wStatus = FALSE;
				ca_rolling_msg.roll_status= FALSE;	
				divitone_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);

			}
		}
		else
		{
			if (TRUE == ca_rolling_msg.roll_status)
			{
				divitone_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
				ca_rolling_msg.roll_status= FALSE;
			}
		}
		return TRUE;
	}





