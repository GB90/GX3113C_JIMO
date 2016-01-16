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
#include "sys_common_config.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "desai_ca_api.h"
#include "win_desai_cas_5.0_rolling_message.h"
#include "app_desai_cas_5.0_api_pop.h"
#include "app_common_porting_ca_os.h"

extern int  app_ds_ad_osd_display_status(void);
extern void dsca_show_osd_retry(void);
extern void app_ds_ad_hide_rolling_osd(void);


static desai_cas50_osd_data_st ca_rolling_msg[2];
#define DSAD_OSD_BUFF_LEN (512)
static char* buf1;//[DSAD_OSD_BUFF_LEN+1]={0,};
static char* buf2;//[DSAD_OSD_BUFF_LEN+1]={0,};
static handle_t osd_sem = 0;
static int32_t lastRollingPos = 0;


#ifdef DVB_AD_TYPE_DS_FLAG41
static int8_t ucOSDTempo = 0;

char * get_osd_showing_buf_top(void)
{
	return buf1;
}
char * get_osd_showing_buf_bottom(void)
{
	return buf2;
}
int get_osd_showing_top_status(void)
{
	return ca_rolling_msg[0].roll_status;
}
int get_osd_showing_bottom_status(void)
{
	return ca_rolling_msg[1].roll_status;
}
static void set_osd_showing_speed(char * text_handle)
{
	//gxgui_set_osd_speed(ucOSDTempo);
	char buf[5];
	if(ucOSDTempo<=0)
	{
		ucOSDTempo =1;
	}
	//sprintf((void*)buf,"%d",ucOSDTempo);

	switch(ucOSDTempo)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			sprintf((void*)buf,"%d",1);
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			sprintf((void*)buf,"%d",2);
			break;
		case 8:
		case 9:
		case 10:
			sprintf((void*)buf,"%d",3);
			break;
		default:
			sprintf((void*)buf,"%d",1);
	}
	GUI_SetProperty(text_handle, "step", buf);
}
#endif


int8_t app_desai_cas50_rolling_message_init(void)
{
	buf1 =GxCore_Malloc(DSAD_OSD_BUFF_LEN+1);
	buf2 =GxCore_Malloc(DSAD_OSD_BUFF_LEN+1);
	memset(&ca_rolling_msg[0],0,2*sizeof(desai_cas50_osd_data_st));
	app_porting_ca_os_sem_create(&osd_sem,1);
	return 0;
}
void app_desai_cas50_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message)
{

	CAS_Dbg("%s>>style=%d,showFlag=%d,memssage=%s\n",__FUNCTION__,byStyle,showFlag,message);
	switch(byStyle)
	{
		case SHOW_TOP:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);	
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
			//	GUI_SetProperty("win_full_screen_text_roll_top_back","state","hide");
				lastRollingPos = 0;
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
			//	GUI_SetProperty("win_full_screen_text_roll_top_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_top","state","show");
				GUI_SetInterface("flush", NULL);
			}
 			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_top","string","");
				GUI_SetInterface("flush", NULL);			
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);
#ifdef DVB_AD_TYPE_DS_FLAG41
				set_osd_showing_speed("win_full_screen_text_roll_top");
#endif
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
			//	GUI_SetProperty("win_full_screen_text_roll_top_back","state","show");
				//GUI_SetInterface("flush", NULL);

				//GxCore_ThreadDelay(100);
				GUI_SetProperty("win_full_screen_text_roll_top","reset_rolling", "");
				GUI_SetProperty("win_full_screen_text_roll_top","state","hide");
				lastRollingPos = 0;
			}
			else if (DVB_CA_ROLLING_FLAG_TRANS_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_top","rolling_stop",NULL);	
				GUI_SetProperty("win_full_screen_text_roll_top","string", message);
				GUI_SetProperty("win_full_screen_text_roll_top","state","osd_trans_hide");
			//	GUI_SetProperty("win_full_screen_text_roll_top_back","state","osd_trans_hide");
				//GUI_SetInterface("flush", NULL);
				lastRollingPos = 0;
			}
			break;
		case SHOW_BOTTOM:
			if (DVB_CA_ROLLING_FLAG_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);			
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
			//	GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","hide");
				lastRollingPos = 0;
			}
			else if (DVB_CA_ROLLING_FLAG_SHOW == showFlag)
			{
			//	GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","show");
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","show");
				GUI_SetInterface("flush", NULL);
			}
			else if (DVB_CA_ROLLING_FLAG_RESET == showFlag)
			{
				GUI_SetProperty("win_full_screen_text_roll_bottom","string","");
				GUI_SetInterface("flush", NULL);
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);
#ifdef DVB_AD_TYPE_DS_FLAG41
				set_osd_showing_speed("win_full_screen_text_roll_bottom");
#endif
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);
			//	GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","show");
				//GUI_SetInterface("flush", NULL);

				//GxCore_ThreadDelay(100);
				GUI_SetProperty("win_full_screen_text_roll_bottom","reset_rolling", "");
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","hide");
				lastRollingPos = 0;
			}	
			else if (DVB_CA_ROLLING_FLAG_TRANS_HIDE == showFlag)
			{	
				GUI_SetProperty("win_full_screen_text_roll_bottom","rolling_stop",NULL);							
				GUI_SetProperty("win_full_screen_text_roll_bottom","string", message);
				GUI_SetProperty("win_full_screen_text_roll_bottom","state","osd_trans_hide");
			//	GUI_SetProperty("win_full_screen_text_roll_bottom_back","state","osd_trans_hide");
				//GUI_SetInterface("flush", NULL);
				lastRollingPos = 0;
			}
			break;
		default:
			break;
	}
	CAS_Dbg("%s %d>>exit\n",__FUNCTION__,__LINE__);
}


char app_desai_cas50_rolling_message_one_time(void)
{
	int32_t times = 0;
	/*OSD is showing*/
	if((ca_rolling_msg[0].wStatus== FALSE) && (ca_rolling_msg[1].wStatus == FALSE))
	{
		lastRollingPos = 0;
		return FALSE;
	}
	if(ca_rolling_msg[0].roll_status == TRUE)
	{
		GUI_GetProperty("win_full_screen_text_roll_top","times",&times);
		//CAS_Dbg("get curPos %d.\n", times);

	}
	if(ca_rolling_msg[1].roll_status == TRUE)
	{
		GUI_GetProperty("win_full_screen_text_roll_bottom","times",&times);
		//CAS_Dbg("get times %d.\n", times);
	}
	if(times > lastRollingPos)
	{
		lastRollingPos = times;
		//app_rolling_message_hide();
		app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_HIDE," ");
		ca_rolling_msg[0].wStatus= FALSE;
		ca_rolling_msg[0].roll_status= FALSE;
		app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE," ");
		ca_rolling_msg[1].wStatus= FALSE;
		ca_rolling_msg[1].roll_status= FALSE;
		return TRUE;
	}
	else
	{
		//lastRollingPos = 0;
		return FALSE;
	}

}

int32_t app_desai_cas50_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	desai_cas50_osd_data_st* osd_data = NULL;
	ST_OSD_SHOW_INFO *rolling_ca = NULL;
	//char* focus_Window = (char*)GUI_GetFocusWindow();
	int pos = -1;
	//int buflen = -1;
	int contentLen = 0;
	char * pstrbuf = NULL;
	if (NULL == event)
	{
		return 0;
	}
	osd_data = (desai_cas50_osd_data_st *)(event->buf);
	if(!osd_data)
	{
		return 1;
	}
	app_porting_ca_os_sem_wait(osd_sem);

	CAS_Dbg("*******app_desai_cas50_gxmsg_ca_on_event_rolling*******\n");
	if(osd_data->from == 1)
	{
		rolling_ca = (ST_OSD_SHOW_INFO *)(osd_data->buffer);
	}

	if(osd_data->from == 1 && rolling_ca != NULL)
	{
		pos = rolling_ca->ucDisplayPos;
		pstrbuf = (char *)rolling_ca->pucDisplayBuffer;
		contentLen = rolling_ca->usContentSize;
#ifdef DVB_AD_TYPE_DS_FLAG41
		ucOSDTempo = rolling_ca->ucOSDTempo;
#endif
	}
	CAS_Dbg("*******from=%d*******\n",osd_data->from);
	CAS_Dbg("*******wStatus=%d*******\n",osd_data->wStatus);
	CAS_Dbg("*******pos=%d*******\n",pos);
#ifdef DVB_AD_TYPE_DS_FLAG41
	CAS_Dbg("*******ucOSDTempo=%d*******\n",ucOSDTempo);
#endif
	if(pstrbuf)
	{
		CAS_Dbg("*******str=%s*******\n",pstrbuf);
		CAS_Dbg("*******contentLen=%d*******\n",contentLen);
	}
	do{
		if(pos == -1)/*Hide OSD*/
		{
			memset(buf1,0,DSAD_OSD_BUFF_LEN+1);
			memset(buf2,0,DSAD_OSD_BUFF_LEN+1);
			ca_rolling_msg[0].wStatus = osd_data->wStatus;
			ca_rolling_msg[1].wStatus = osd_data->wStatus;
		}
		if(FALSE ==osd_data->wStatus)
		{
			memset(buf1,0,DSAD_OSD_BUFF_LEN+1);
			memset(buf2,0,DSAD_OSD_BUFF_LEN+1);
			ca_rolling_msg[0].wStatus = osd_data->wStatus;
			ca_rolling_msg[1].wStatus = osd_data->wStatus;
		}
		if((pos == SHOW_TOP))
		{
			if(pstrbuf && contentLen <= DSAD_OSD_BUFF_LEN)
			{
				memset(buf1,0,DSAD_OSD_BUFF_LEN+1);
				memcpy(buf1,pstrbuf,contentLen);
			}
			ca_rolling_msg[1].wStatus = FALSE;
			memcpy(&ca_rolling_msg[0],osd_data,sizeof(desai_cas50_osd_data_st));
		}
		else if(pos == SHOW_BOTTOM)
		{
			if(pstrbuf && contentLen <= DSAD_OSD_BUFF_LEN)
			{
				memset(buf2,0,DSAD_OSD_BUFF_LEN+1);
				memcpy(buf2,pstrbuf,contentLen);
			}
			ca_rolling_msg[0].wStatus = FALSE;
			memcpy(&ca_rolling_msg[1],osd_data,sizeof(desai_cas50_osd_data_st));
		}
		else if(pos == SHOW_TOP_BOTTOM)
		{
			if(pstrbuf && contentLen <= DSAD_OSD_BUFF_LEN)
			{
				memset(buf1,0,DSAD_OSD_BUFF_LEN+1);
				memcpy(buf1,pstrbuf,contentLen);
			}
			if(pstrbuf && contentLen <= DSAD_OSD_BUFF_LEN)
			{
				memset(buf2,0,DSAD_OSD_BUFF_LEN+1);
				memcpy(buf2,pstrbuf,contentLen);
			}
			memcpy(&ca_rolling_msg[0],osd_data,sizeof(desai_cas50_osd_data_st));
			memcpy(&ca_rolling_msg[1],osd_data,sizeof(desai_cas50_osd_data_st));
		}
	}while(0);
	app_porting_ca_os_sem_signal(osd_sem);
	return 1;
}
int32_t app_desai_cas50_rolling_message_exec(void)
{
	ST_OSD_SHOW_INFO *rolling_ca;
	char * pstrbuf = NULL;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	static char flush_ctr = 3;
	app_porting_ca_os_sem_wait(osd_sem);
	if(flush_ctr)
	{
		flush_ctr--;
		//GUI_SetInterface("flush", NULL);
	}
	
	if (TRUE == ca_rolling_msg[0].wStatus)
	{
		if (FALSE == ca_rolling_msg[0].roll_status)
		{
			if(ca_rolling_msg[0].from == 1)
			{
				rolling_ca = (ST_OSD_SHOW_INFO *)(ca_rolling_msg[0].buffer);
				if (rolling_ca)
				{
					pstrbuf = (char *)rolling_ca->pucDisplayBuffer;
				}
			}
			CAS_Dbg("%s %d>>\n",__FUNCTION__,__LINE__);
			if(0 == strcasecmp("win_full_screen", focus_Window))
			{
				app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_RESET,(char*)buf1);
				app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_SHOW,NULL);
				ca_rolling_msg[0].roll_status = TRUE;				
				flush_ctr = 3;
			}	
		}				
	}
	else
	{
		if (TRUE == ca_rolling_msg[0].roll_status)
		{
			CAS_Dbg("%s %d>>HIDE\n",__FUNCTION__,__LINE__);
			//app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_RESET," ");
			app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_HIDE," ");
			ca_rolling_msg[0].roll_status = FALSE;
		}
	}

	if (TRUE == ca_rolling_msg[1].wStatus)
	{
		if (FALSE == ca_rolling_msg[1].roll_status)
		{
			if(ca_rolling_msg[1].from == 1)
			{
				rolling_ca = (ST_OSD_SHOW_INFO *)(ca_rolling_msg[0].buffer);
				if (rolling_ca)
				{
					pstrbuf = (char *)rolling_ca->pucDisplayBuffer;
				}
			}
			CAS_Dbg("%s %d>>\n",__FUNCTION__,__LINE__);
			if(0 == strcasecmp("win_full_screen", focus_Window))
			{
				app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_RESET,(char*)buf2);
				app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_SHOW,NULL);
				ca_rolling_msg[1].roll_status = TRUE;		
				flush_ctr = 3;
			}
		}
	}
	else
	{
		if (TRUE == ca_rolling_msg[1].roll_status)
		{
			CAS_Dbg("%s %d>>\n",__FUNCTION__,__LINE__);
			//app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_RESET," ");
			app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE," ");
			ca_rolling_msg[1].roll_status= FALSE;
		}
	}
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
	if((app_ds_ad_osd_display_status() == -1))
#endif
	{
		if(app_desai_cas50_rolling_message_one_time())
		{
			DSCA_OsdOneTimeOk();
		}
		else if((FALSE == ca_rolling_msg[0].wStatus && FALSE == ca_rolling_msg[0].roll_status)
			&& (FALSE == ca_rolling_msg[1].wStatus && FALSE == ca_rolling_msg[1].roll_status)
			&& (0 == strcasecmp("win_full_screen", focus_Window)))
		{
			dsca_show_osd_retry();
		}
	}
	app_porting_ca_os_sem_signal(osd_sem);

	return TRUE;

}

void app_rolling_message_hide(void)
{

#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
	if (TRUE == app_cas_api_get_lock_status()){
		app_ds_ad_hide_rolling_osd();
		ca_rolling_msg[1].wStatus= FALSE;
		ca_rolling_msg[0].wStatus= FALSE;
	}
#endif 
	app_porting_ca_os_sem_wait(osd_sem);
	app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_HIDE," ");
	ca_rolling_msg[0].roll_status= FALSE;
	app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE," ");
	ca_rolling_msg[1].roll_status= FALSE;
	app_porting_ca_os_sem_signal(osd_sem);
	
	
}

void app_desai_cas50_rolling_message_hide(void)
{
#if defined (DVB_AD_TYPE_DS_FLAG)||defined (DVB_AD_TYPE_DS_FLAG41)
	if(app_ds_ad_osd_display_status() != -1)
	{
		return;
	}
#endif
	app_porting_ca_os_sem_wait(osd_sem);
	app_desai_cas50_rolling_message_show(SHOW_TOP,DVB_CA_ROLLING_FLAG_HIDE,"");
	ca_rolling_msg[0].roll_status= FALSE;
	app_desai_cas50_rolling_message_show(SHOW_BOTTOM,DVB_CA_ROLLING_FLAG_HIDE,"");
	ca_rolling_msg[1].roll_status= FALSE;
	app_porting_ca_os_sem_signal(osd_sem);
}
/**/
