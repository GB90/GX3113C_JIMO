/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
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
	
#include "app_gy_cas_api_pin.h"
#include "app_gy_cas_api_smart.h"
#include "app_gy_cas_api_demux.h"
#include "app_gy_cas_api_baseinfo.h"
#include "app_gy_cas_api_pop.h"
#include "app_gy_cas_api_rolling_message.h"
#include "app_common_porting_ca_os.h"
static handle_t osd_sem = 0;

extern char * GUI_GetFocusWindow(void);

extern 	void gy_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);

static gy_cas_rolling_message_st ca_rolling_msg[4];

int8_t app_gy_cas_rolling_message_init(void)
{
	memset(&ca_rolling_msg[0],0,4*sizeof(gy_cas_rolling_message_st));
	app_porting_ca_os_sem_create(&osd_sem,1);
	return 0;
}
void app_gy_cas_rolling_message_hide(void)
{
	GxTime  time = {0};
	app_porting_ca_os_sem_wait(osd_sem);
	GxCore_GetTickTime(&time);
	if(ca_rolling_msg[GYCA_OSD_TOP].roll_status)
	{
		gy_cas_rolling_message_show(GYCA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,"");
		ca_rolling_msg[GYCA_OSD_TOP].roll_status= FALSE;					
		ca_rolling_msg[GYCA_OSD_TOP].startTime = time.seconds;
	}
	if(ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status)
	{
		gy_cas_rolling_message_show(GYCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,"");
		ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status= FALSE;					
		ca_rolling_msg[GYCA_OSD_BOTTOM].startTime = time.seconds;
	}
	app_porting_ca_os_sem_signal(osd_sem);

}

int32_t app_gy_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	gy_cas_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (gy_cas_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;
	app_porting_ca_os_sem_wait(osd_sem);

	ca_rolling_msg[rolling_msg->byStyle].wStatus = rolling_msg->wStatus;
	ca_rolling_msg[rolling_msg->byStyle].byStyle = rolling_msg->byStyle;
	ca_rolling_msg[rolling_msg->byStyle].dwTime = rolling_msg->dwTime;
	memcpy(ca_rolling_msg[rolling_msg->byStyle].content,
		rolling_msg->content,sizeof(rolling_msg->content));
	/*Show New OSD when the osd is showing*/
	if(ca_rolling_msg[rolling_msg->byStyle].wStatus)
	{
		ca_rolling_msg[rolling_msg->byStyle].roll_status = FALSE;
	}
	app_porting_ca_os_sem_signal(osd_sem);
			
	return 1;
}

int32_t app_gy_cas_rolling_message_exec(void)
{
	char* focus_Window = (char*)GUI_GetFocusWindow();
	GxTime  time = {0};
	
	if(0 != strcasecmp("win_full_screen", focus_Window))
		return 0;
	app_porting_ca_os_sem_wait(osd_sem);

	if (TRUE == ca_rolling_msg[GYCA_OSD_TOP].wStatus)
	{
		if (FALSE == ca_rolling_msg[GYCA_OSD_TOP].roll_status)
		{
			gy_cas_rolling_message_show(GYCA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,
				(char*)ca_rolling_msg[GYCA_OSD_TOP].content);
			gy_cas_rolling_message_show(GYCA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,NULL);
			ca_rolling_msg[GYCA_OSD_TOP].roll_status = TRUE;
			GxCore_GetTickTime(&time);
			ca_rolling_msg[GYCA_OSD_TOP].startTime = time.seconds;
		}
		else
		{
			GxCore_GetTickTime(&time);
			if((time.seconds - ca_rolling_msg[GYCA_OSD_TOP].startTime)
				> ca_rolling_msg[GYCA_OSD_TOP].dwTime)
			{
				gy_cas_rolling_message_show(GYCA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE," ");
				ca_rolling_msg[GYCA_OSD_TOP].wStatus = FALSE; 	
				ca_rolling_msg[GYCA_OSD_TOP].roll_status= FALSE; 			
			}
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[GYCA_OSD_TOP].roll_status)
		{
			gy_cas_rolling_message_show(GYCA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE," ");
			ca_rolling_msg[GYCA_OSD_TOP].roll_status = FALSE;								
		}
	}

	if (TRUE == ca_rolling_msg[GYCA_OSD_BOTTOM].wStatus)
	{
		if (FALSE == ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status)
		{
			gy_cas_rolling_message_show(GYCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,
				(char*)ca_rolling_msg[GYCA_OSD_BOTTOM].content);
			gy_cas_rolling_message_show(GYCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_SHOW,NULL);
			ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status = TRUE;	
			GxCore_GetTickTime(&time);
			ca_rolling_msg[GYCA_OSD_BOTTOM].startTime = time.seconds;
		}
		else
		{
			GxCore_GetTickTime(&time);
			if((time.seconds - ca_rolling_msg[GYCA_OSD_BOTTOM].startTime)
				> ca_rolling_msg[GYCA_OSD_BOTTOM].dwTime)
			{
				gy_cas_rolling_message_show(GYCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE," ");
				ca_rolling_msg[GYCA_OSD_BOTTOM].wStatus = FALSE; 	
				ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status= FALSE; 			
			}
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status)
		{
			gy_cas_rolling_message_show(GYCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE," ");
			ca_rolling_msg[GYCA_OSD_BOTTOM].roll_status= FALSE;					
		}			
	}
	app_porting_ca_os_sem_signal(osd_sem);

	return TRUE;

}
/**/
