#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_event.h"
#include "gui_core.h"
#include "mg312def.h"

extern 	void win_mg_cas312_finger_show(uint8_t number, uint8_t showFlag, char* message);
extern void win_mg_cas312_ppvlogo_show(uint8_t showFlag, char *msg);
extern 	void win_mg_cas312_showDialog(void *data, int size);

static mg_ca312_finger_data_st s_caFingerData = {0};

static  int mg_cas312_3minClose_finger(void *userdata)
{
    printf("[Timer]3min Timeout.close fingershow.");
	win_mg_cas312_finger_show(0, DVB_CA_FINGER_FLAG_HIDE, NULL); 
    s_caFingerData.wStatus = FALSE;
	return 0;
}

//消息处理
int32_t app_mg_cas312_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
    mg_ca312_finger_data_st* finger_data = NULL;
	
	if (NULL == event)
	{
		return 0;
	}

	finger_data = (mg_ca312_finger_data_st *)(event->buf);
	if (NULL == finger_data)
	{
		return 0;
	}

    if ((0 != s_caFingerData.wStatus) && (TRUE == s_caFingerData.wShowStatus))
	{
		win_mg_cas312_finger_show(0, DVB_CA_FINGER_FLAG_HIDE, NULL); 				
	}

    create_timer(mg_cas312_3minClose_finger, 3*60*1000, NULL, TIMER_ONCE);

    memcpy(&s_caFingerData, finger_data, sizeof(mg_ca312_finger_data_st));
	s_caFingerData.wShowStatus = FALSE;//初始化未显示

	return 1;
}

int32_t app_mg_cas312_finger_exec(void)
{
	if (FALSE == s_caFingerData.wStatus)
	{
		if (TRUE == s_caFingerData.wShowStatus)
		{
			s_caFingerData.wShowStatus = FALSE;
			win_mg_cas312_finger_show(0, DVB_CA_FINGER_FLAG_HIDE, NULL); 				
		}
	}
	else if (TRUE == s_caFingerData.wStatus)
	{
		if (FALSE == s_caFingerData.wShowStatus)
		{
			win_mg_cas312_finger_show(0, DVB_CA_FINGER_FLAG_SHOW, s_caFingerData.msg); 				
			s_caFingerData.wShowStatus = TRUE;
		}
	}
	
    return 1;
}

static int mg_cas312_Close_ppvlogo(void *userdata)
{
    printf("[mg_cas312_Close_ppvlogo]Timeout.close ppvlogo.\n");
    win_mg_cas312_ppvlogo_show(DVB_CA_FINGER_FLAG_HIDE, NULL);
	return 0;
}

//ppv logo 图标显示
int32_t app_mg_cas312_gxmsg_ca_on_event_ippv_logo(GxMsgProperty0_OnEvent* event)
{
    mg_ca312_ppv_data_st *pdata = NULL;
	
	if (NULL == event)
	{
		return 0;
	}

	pdata = (mg_ca312_ppv_data_st *)(event->buf);
	if (NULL == pdata)
	{
		return 0;
	}

    win_mg_cas312_ppvlogo_show(DVB_CA_FINGER_FLAG_SHOW, pdata->msg);
    create_timer(mg_cas312_Close_ppvlogo, pdata->time*1000, NULL, TIMER_ONCE);

	return 1;
}

int32_t app_mg_cas312_msg_show(GxMsgProperty0_OnEvent* event)
{
    mg_cas312_rolling_message_st *data = NULL;

	if (NULL == event)
	{
		return 0;
	}

	data = (mg_cas312_rolling_message_st *)(event->buf);
	if (NULL == data)
	{
		return 0;
	}

    win_mg_cas312_showDialog((void *)data, sizeof(mg_cas312_rolling_message_st));
    return 1;
}


