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
#include "win_desai_cas_5.0_exigent.h"
#include "app_common_porting_ca_flash.h"
#include "app_common_porting_ca_smart.h"
#include "app_common_porting_ca_demux.h"
#include "app_common_porting_ca_descramble.h"
#include "app_desai_cas_5.0_api_pop.h"
#include "desai_ca_api.h"
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
extern void app_rolling_message_hide(void);
#endif


//static desai_cas50_exigent_data_st ca_exigent_flag = {0};
static uint8_t lock_service_status = FALSE;
static INT32 program_ID=0;

uint8_t app_desai_cas50_api_set_lock_status(uint8_t lock_status)
{
	if (lock_service_status != lock_status)
		lock_service_status = lock_status;
	return lock_service_status;
	
}

uint8_t app_desai_cas50_api_get_lock_status(void)
{
	return lock_service_status;
}

int32_t app_desai_cas50_gxmsg_ca_on_event_exigent(GxMsgProperty0_OnEvent* event)
{
	desai_cas50_exigent_data_st* exigent_flag = NULL;
	unsigned int pos = 0;
    char* focus_menu = NULL;

	if (NULL == event)
		return 0;

	exigent_flag = (desai_cas50_exigent_data_st*)(event->buf);

	if (NULL == exigent_flag)
		return 0;

	if (TRUE == exigent_flag->wStatus)
	{
		CAS_Dbg("******************start exigent program***************");
		lock_service_status = TRUE;

#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)

        //2015-03-10
		//app_ds_ad_hide_osd();
		//app_ds_ad_hide_rolling_osd();
		app_rolling_message_hide();
#endif

        //2014-12-03,qm
        #if 0
		if((app_win_get_focus_video_window() != FULL_SCREEN_WIN)&&(app_win_get_focus_video_window() != PROGRAM_BAR_WIN))
			return 1;
        #else
        focus_menu =(char*)GUI_GetFocusWindow();
        if (strcmp(focus_menu,"win_full_screen") != 0)
        {
            GUI_EndDialog(focus_menu);
        }
        #endif
		
		INT32 ret=0;
		GxBusPmDataProg prog_data;
		GxBusPmViewInfo sysinfo;
		INT32  *porg_num=(INT32*)exigent_flag->buffer;
		INT32 num=*porg_num;
		//INT32 num1=num+1;
		CAS_Dbg("prog_num =%d\n\n",num);
		GUI_EndDialog("win_desai_cas50_ippv_buy");
		app_play_clear_ca_msg();
		GUI_EndDialog("win_prog_bar");
		GUI_EndDialog("win_popmsg_ok");
		ret=GxBus_PmProgGetByPos(num,1,(void*)&prog_data);
		if(ret !=-1)
		{
			CAS_Dbg("dscas play prog_num =%d\n\n",num);
			GxBus_PmViewInfoGet(&sysinfo);
			if(sysinfo.stream_type == GXBUS_PM_PROG_TV)
			{
				program_ID=sysinfo.tv_prog_cur;
				sysinfo.tv_prog_cur = prog_data.id;
			}
			else
			{
				program_ID=sysinfo.radio_prog_cur;
				sysinfo.radio_prog_cur = prog_data.id;
			}
			app_play_video_audio(num);
			
			GxBus_PmViewInfoModify(&sysinfo);
		}
//		gApp_prompt_flag = 0;
		GUI_EndDialog("win_volume");

	//	GUI_CreateDialog("win_prog_bar");

		app_play_set_msg_pop_type(MSG_POP_COMMON_END+35+1);//紧急广播开启

	}
	else
	{
		CAS_Dbg("******************stop exigent program***************");

		if((app_win_get_focus_video_window() != FULL_SCREEN_WIN)&&(app_win_get_focus_video_window() != PROGRAM_BAR_WIN))
			return 1;
		lock_service_status = FALSE;
		GxBusPmViewInfo sysinfo;
		GUI_EndDialog("win_desai_cas50_ippv_buy");
		
		app_play_clear_ca_msg();
		GUI_EndDialog("win_prog_bar");
		GUI_EndDialog("win_popmsg_ok");

		GxBus_PmViewInfoGet(&sysinfo);
		if(sysinfo.stream_type == GXBUS_PM_PROG_TV)
		{
			sysinfo.tv_prog_cur = program_ID;
		}
		else
		{
			sysinfo.radio_prog_cur = program_ID;
		}
		GxBus_PmViewInfoModify(&sysinfo);
		app_prog_get_playing_pos_in_group(&pos);
		app_play_video_audio(pos);
		GUI_EndDialog("win_volume"); 
	//	GUI_CreateDialog("win_prog_bar");
		app_play_set_msg_pop_type(MSG_POP_COMMON_END+36+1);//紧急广播关闭
	}
	
	return 1;
}


