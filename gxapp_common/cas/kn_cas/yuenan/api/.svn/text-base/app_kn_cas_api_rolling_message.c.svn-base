/******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_kn_cas_api_pop.h"
#include "app_kn_cas_api_rolling_message.h"
#include "gxmsg.h"
#include "cas_hcn_Public.h"
#include "gui_core.h"



extern 	void kn_cas_rolling_message_show(uint8_t byStyle,uint8_t showFlag,char* message);
extern 	int32_t kn_cas_rolling_message_get_times(void);

static int rolling_status = 0;
static int rolling_flag = 0;
static hcncas_osd rolling_osd = {0};
void reset_rolling_status(void)
{
	rolling_status = 0;
}
int32_t app_kn_cas_gxmsg_ca_on_event_rolling(GxMsgProperty0_OnEvent* event)
{
	hcncas_osd* rolling_msg = NULL;
	if (NULL == event)
		return 0;
	
	rolling_msg = (hcncas_osd*)(event->buf);

	if (NULL == rolling_msg)
	return 0;
	
	memset(&rolling_osd,0,sizeof(hcncas_osd));
	memcpy(&rolling_osd,rolling_msg,sizeof(hcncas_osd));
	
	//rolling_msg->subdata[rolling_msg->len]=0;
//	printf("\napp_kn_cas_gxmsg_ca_on_event_rolling****%d*****\n",strlen(rolling_osd.subdata));
//	rolling_osd.subdata[rolling_osd.len-1]='\0';
	printf("\napp_kn_cas_gxmsg_ca_on_event_rolling****%s*****\n",rolling_osd.subdata);
	printf("\napp_kn_cas_gxmsg_ca_on_event_rolling****%d*****\n",rolling_osd.len);
	
	kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_RESET,(char *)rolling_osd.subdata);
//	kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,(char *)rolling_osd.subdata);
	rolling_flag = 1;
	rolling_status=0;

	return 1;
}


int32_t app_kn_cas_rolling_message_exec(void)
{

	int wTimes;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	if ((NULL != focus_Window)&&(strcasecmp("win_full_screen", focus_Window) != 0))
	{
		return 1;
	}
	if(rolling_flag)
	{
		if(rolling_status == 0)
		{
			kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_SHOW,(char *)rolling_osd.subdata);
			rolling_status = 1;
		}
		wTimes = kn_cas_rolling_message_get_times();
		if (wTimes >=  1)
		{	
			kn_cas_rolling_message_show(0,DVB_CA_ROLLING_FLAG_HIDE,NULL);
			rolling_flag = 0;
			rolling_status=0;
		}
	}

	return 0;
}







