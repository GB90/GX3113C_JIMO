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
#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "win_desai_cas_5.0_finger.h"
#include "app_desai_cas_5.0_api_pop.h"
#include "app_common_porting_ca_os.h"

//static  uint8_t fullscreenFlag = FALSE;
static desai_cas50_finger_data_st ca_finger_data = {0};
static handle_t ds_ca_finger_sem = 0;
static ST_FINGER_SHOW_INFO ds_ca_finger_data;

static void app_desai_cas50_finger_show(uint8_t number,uint8_t showFlag,char* message)
{
	char	 sTextBuffer[40]={0};
	int index = 3;
	if(number == SHOW_LEFT_TOP)
	{
		index = 3;
	}
	else if(number == SHOW_LEFT_BOTTOM)
	{
		index = 7;
	}
	else if(number == SHOW_RIGHT_TOP)
	{
		index = 11;
	}
	else if(number == SHOW_RIGHT_BOTTOM)
	{
		index = 10;
	}
	
	printf("app_desai_cas50_finger_show number=%d\n",index);
	if (DVB_CA_FINGER_FLAG_HIDE == showFlag)
	{	
		sprintf( sTextBuffer, "win_full_screen_text_finger%d", index);
		GUI_SetProperty(sTextBuffer,"string"," ");
		GUI_SetProperty(sTextBuffer,"state","hide");
	}
	else if (DVB_CA_FINGER_FLAG_TRANS_HIDE == showFlag)
	{
		sprintf( sTextBuffer, "win_full_screen_text_finger%d", index); 
		GUI_SetProperty(sTextBuffer,"state","osd_trans_hide");				
	}
	else if (DVB_CA_FINGER_FLAG_SHOW == showFlag)
	{
		sprintf( sTextBuffer, "win_full_screen_text_finger%d", index);					
		GUI_SetProperty(sTextBuffer,"string", message); 				
		GUI_SetProperty(sTextBuffer,"state","show");	
	}
	return;
}

void app_desai_cas50_finger_init(void)
{
	app_porting_ca_os_sem_create(&ds_ca_finger_sem,1);
	memset(&ds_ca_finger_data,0,sizeof(ST_FINGER_SHOW_INFO));
	ca_finger_data.buffer = (uint8_t *)&ds_ca_finger_data;
}

int32_t app_desai_cas50_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
	desai_cas50_finger_data_st* finger_data = NULL;
	ST_FINGER_SHOW_INFO * ds_finger_data = NULL;

	if (NULL == event)
		return 0;

	finger_data = (desai_cas50_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;
	app_porting_ca_os_sem_wait(ds_ca_finger_sem);
	ds_finger_data = (ST_FINGER_SHOW_INFO *)finger_data->buffer;
	ca_finger_data.wStatus = finger_data->wStatus;
	if(ca_finger_data.wStatus && ds_finger_data)
	{
		memcpy(ca_finger_data.buffer,ds_finger_data,sizeof(ST_FINGER_SHOW_INFO));
		ca_finger_data.position = finger_data->position;
		ca_finger_data.wShowStatus  = FALSE;			
	}
	app_porting_ca_os_sem_signal(ds_ca_finger_sem);
	return 1;
}


int32_t app_desai_cas50_finger_exec(void)
{
	//uint8_t chFingerBuf[20]={0};	
	char* focus_Window = NULL;
	ST_FINGER_SHOW_INFO * ds_finger_data = NULL;
// 指纹显示
	focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcmp("win_full_screen", focus_Window) != 0))
	{
		return TRUE;
	}
	app_porting_ca_os_sem_wait(ds_ca_finger_sem);
	
	if (TRUE == ca_finger_data.wStatus	)	
	{
		ds_finger_data =(ST_FINGER_SHOW_INFO *)ca_finger_data.buffer;
		if (FALSE == ca_finger_data.wShowStatus && ds_finger_data)
		{
			app_desai_cas50_finger_show(SHOW_LEFT_TOP,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_LEFT_BOTTOM,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_RIGHT_TOP,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_RIGHT_BOTTOM,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);

			app_desai_cas50_finger_show(ca_finger_data.position,
				DVB_CA_FINGER_FLAG_SHOW,(char*)ds_finger_data->pucShowContent);

			ca_finger_data.wShowStatus = TRUE;				
		}
	}	
	else if (FALSE == ca_finger_data.wStatus	)	
	{
		if (TRUE == ca_finger_data.wShowStatus)
		{
			app_desai_cas50_finger_show(SHOW_LEFT_TOP,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_LEFT_BOTTOM,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_RIGHT_TOP,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			app_desai_cas50_finger_show(SHOW_RIGHT_BOTTOM,DVB_CA_FINGER_FLAG_TRANS_HIDE,NULL);
			ca_finger_data.wShowStatus = FALSE;
		}
	}
	app_porting_ca_os_sem_signal(ds_ca_finger_sem);
	return TRUE;
}

