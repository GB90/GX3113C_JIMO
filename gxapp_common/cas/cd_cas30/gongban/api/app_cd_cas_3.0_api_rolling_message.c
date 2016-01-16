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
#include "app_cd_cas_3.0_api_rolling_message.h"
#include "app_common_porting_ca_os.h"

extern char * GUI_GetFocusWindow(void);

extern 	void cd_cas30_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
static cd_cas30_rolling_message_st ca_rolling_msg[CDCA_OSD_HALFSCREEN];
static handle_t osd_sem = 0;
int8_t app_cd_cas30_rolling_message_init(void)
{
	memset(&ca_rolling_msg[0],0,CDCA_OSD_HALFSCREEN*sizeof(cd_cas30_rolling_message_st));
	app_porting_ca_os_sem_create(&osd_sem,1);
	return 0;
}

int32_t app_cd_cas30_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	cd_cas30_rolling_message_st* rolling_msg = NULL;
	if (NULL == event)
		return 0;

	rolling_msg = (cd_cas30_rolling_message_st*)(event->buf);

	if (NULL == rolling_msg)
		return 0;
#ifdef DVB_AD_TYPE_CMIS_FLAG
{
extern uint8_t  app_cmis_mail_show(char * mail_str);
extern uint32_t cur_prog_pos ;
extern uint8_t g_cmis_mail_repeat_flag;

	uint32_t pos = 0xffff;

	app_prog_get_playing_pos_in_group(&pos);
	g_cmis_mail_repeat_flag = 0;
	if(APP_CMIS_AD_CheckMailIsCmis(rolling_msg->content))
	{
		if(rolling_msg->wStatus == FALSE)
		{
			printf("CMIS hide ,don't care\n");
			return 1;
		}
	if(app_cmis_mail_show(rolling_msg->content))
	{
		printf("CMIS MESSAGE received\n");
			cur_prog_pos = pos;
		return 1;
	}
	}
	else
	{
		printf("Mail is not cmis\n");
	}
}
#endif
	app_porting_ca_os_sem_wait(osd_sem);

	memcpy(&ca_rolling_msg[rolling_msg->byStyle-1],rolling_msg,sizeof(cd_cas30_rolling_message_st));


	if (CDCA_OSD_TOP == rolling_msg->byStyle)	 
		{
			cd_cas30_rolling_message_show(CDCA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[rolling_msg->byStyle-1].content);
			ca_rolling_msg[rolling_msg->byStyle-1].roll_status=FALSE;
		}

	if (CDCA_OSD_BOTTOM == rolling_msg->byStyle)	 
		{
			cd_cas30_rolling_message_show(CDCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[rolling_msg->byStyle-1].content);
			ca_rolling_msg[rolling_msg->byStyle-1].roll_status=FALSE;
		}
	app_porting_ca_os_sem_signal(osd_sem);
	return 1;
}

int32_t app_cd_cas30_rolling_message_exec(void)
{
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if(
	(focus_Window == NULL) || 
	(strcasecmp(focus_Window,"win_full_screen") != 0 
	&& strcasecmp(focus_Window,"win_cd_cas30_ippv_buy") != 0)
	)
	{
		return TRUE;
	}
	app_porting_ca_os_sem_wait(osd_sem);

	if (TRUE == ca_rolling_msg[CDCA_OSD_TOP-1].wStatus)
		{
			/* CDCA_OSD_TOP: OSD风格：显示在屏幕上方 */
			if (FALSE == ca_rolling_msg[CDCA_OSD_TOP-1].roll_status)
				{
					cd_cas30_rolling_message_show(CDCA_OSD_TOP,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg[CDCA_OSD_TOP-1].content);
					ca_rolling_msg[CDCA_OSD_TOP-1].roll_status = TRUE;				
				}				
		}
	else
		{
			if (TRUE == ca_rolling_msg[CDCA_OSD_TOP-1].roll_status)
				{
					cd_cas30_rolling_message_show(CDCA_OSD_TOP,DVB_CA_ROLLING_FLAG_HIDE,NULL);
					ca_rolling_msg[CDCA_OSD_TOP-1].roll_status = FALSE;								
				}
		}

	if (TRUE == ca_rolling_msg[CDCA_OSD_BOTTOM-1].wStatus)
		{
			/* CDCA_OSD_BOTTOM: OSD风格：显示在屏幕下方 */

			if (FALSE == ca_rolling_msg[CDCA_OSD_BOTTOM-1].roll_status)
				{
					cd_cas30_rolling_message_show(CDCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_SHOW,(char*)ca_rolling_msg[CDCA_OSD_BOTTOM-1].content);
					ca_rolling_msg[CDCA_OSD_BOTTOM-1].roll_status = TRUE;				
				}
		}
	else
		{
			if (TRUE == ca_rolling_msg[CDCA_OSD_BOTTOM-1].roll_status)
				{
					cd_cas30_rolling_message_show(CDCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,NULL);
					ca_rolling_msg[CDCA_OSD_BOTTOM-1].roll_status= FALSE;					
				}			
		}
	app_porting_ca_os_sem_signal(osd_sem);

	return TRUE;

}

void app_cd_cas30_hide_rolling_message(void)
{
	app_porting_ca_os_sem_wait(osd_sem);
	cd_cas30_rolling_message_show(CDCA_OSD_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[0].content);
	ca_rolling_msg[0].roll_status=FALSE;

	cd_cas30_rolling_message_show(CDCA_OSD_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)ca_rolling_msg[1].content);
	ca_rolling_msg[1].roll_status=FALSE;
	app_porting_ca_os_sem_signal(osd_sem);
}



