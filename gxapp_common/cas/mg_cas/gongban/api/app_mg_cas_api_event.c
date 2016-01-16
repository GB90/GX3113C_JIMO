#include "app_common_porting_stb_api.h"
#include "app_mg_cas_api_event.h"
#include "gui_core.h"
#include "mgdef.h"

#define LANGUAGE_CHINESE ("Chinese")
#define LANGUAGE_ENGLISH ("English")

extern 	void win_mg_cas_finger_show(uint8_t number,uint8_t showFlag,char* message);
extern 	void win_mg_cas_ppvlogo_show(uint8_t showFlag,char *msg);
extern 	void win_mg_cas_entitle_showDialog(void *data,int size);

//static  uint8_t fullscreenFlag = FALSE;
static mg_ca_finger_data_st ca_finger_data = {0};

static  int mg_cas_3minClose_finger(void *userdata)
{
    printf("[Timer]3min Timeout.close fingershow.");
	win_mg_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 
    ca_finger_data.wStatus = FALSE;
	return 0;
}


//消息处理
int32_t app_mg_cas_gxmsg_ca_on_event_finger(GxMsgProperty0_OnEvent* event)
{
    mg_ca_finger_data_st* finger_data = NULL;
	if (NULL == event)
		return 0;

	finger_data = (mg_ca_finger_data_st*)(event->buf);

	if (NULL == finger_data)
		return 0;

    if ((0 != ca_finger_data.wStatus)&&(TRUE == ca_finger_data.wShowStatus))
	{
		win_mg_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 				
	}

    create_timer(mg_cas_3minClose_finger, 3*60*1000, NULL,  TIMER_ONCE);

    memcpy(&ca_finger_data,finger_data,sizeof(mg_ca_finger_data_st));
	ca_finger_data.wShowStatus = FALSE;//初始化未显示

	return 1;
}

//200ms定时器
int32_t app_mg_cas_finger_exec(void)
{
	if (FALSE == ca_finger_data.wStatus)
	{
		/*清除指纹信息*/
		if (TRUE == ca_finger_data.wShowStatus)
		{
			ca_finger_data.wShowStatus = FALSE;
			win_mg_cas_finger_show(0,DVB_CA_FINGER_FLAG_HIDE,NULL); 				
		}
	}
	else if (TRUE == ca_finger_data.wStatus)
	{
		/*显示指纹信息*/
		if (FALSE == ca_finger_data.wShowStatus)
		{
			win_mg_cas_finger_show(0,DVB_CA_FINGER_FLAG_SHOW,ca_finger_data.msg); 				
			ca_finger_data.wShowStatus = TRUE;
		}
	}
    return 1;
}


static  int mg_cas_Close_ppvlogo(void *userdata)
{
    printf("[Timer] Timeout.close ppvlogo.\n");
    win_mg_cas_ppvlogo_show(DVB_CA_FINGER_FLAG_HIDE,NULL);
	return 0;
}

//ppv logo 图标显示
int32_t app_mg_cas_gxmsg_ca_on_event_ippv_logo(GxMsgProperty0_OnEvent* event)
{
    mg_ca_ppv_data_st *pdata = NULL;
	if (NULL == event)
		return 0;

	pdata = (mg_ca_ppv_data_st *)(event->buf);

	if (NULL == pdata)
		return 0;

    win_mg_cas_ppvlogo_show(DVB_CA_FINGER_FLAG_SHOW,pdata->msg);
    create_timer(mg_cas_Close_ppvlogo, pdata->time*1000, NULL,  TIMER_ONCE);

	return 1;

}

char * app_mg_cas_api_get_entitle_data(ca_get_date_t *data)
{
    uint32_t pos = 0 ; 
	uint32_t ID = 0; 
    static char  s_text_buffer[70]={0};

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));

	switch(ID)
	{
	    case MGCA_ENTITLE_INFO_ID1:
        {
            char* osd_language=NULL;
            osd_language = app_flash_get_config_osd_language();
            if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
            {
            	sprintf(s_text_buffer,"有节目授权剩余%d天到期, 请及时续费",MG_Get_RecentExpireDay());
            }
            else if(0 == strcmp(osd_language,LANGUAGE_ENGLISH))
            {
            	sprintf(s_text_buffer,"Programs authorized surplus in %d days, please renew it",MG_Get_RecentExpireDay());
            }

		    return s_text_buffer;
	    }
        case MGCA_ENTITLE_INFO_ID2:
            sprintf(s_text_buffer,"Program authorization has expired, please renew it");
            return s_text_buffer;
	default:
		break;
	}
	return NULL;
}

bool app_mg_cas_gxmsg_ca_check_entitle(mg_cas_auth_st *pauth)
{
    if(NULL == pauth)
        return FALSE;

    if(pauth->status == MG_ERR_AUTHNEARBOOT_REMIND
        || pauth->status == MG_ERR_AUTHNEARTIME_REMIND
        || pauth->status == MG_ERR_AUTHEXPIRE_REMIND)
    {
        return TRUE;
    }

    return FALSE;
}

int app_mg_cas_gxmsg_ca_entitle_ID(mg_cas_auth_st *pauth)
{
    if(NULL == pauth)
        return -1;

    if(pauth->status == MG_ERR_AUTHNEARBOOT_REMIND
        || pauth->status == MG_ERR_AUTHNEARTIME_REMIND)
    {
        return MGCA_ENTITLE_INFO_ID1;
    }
    else if(pauth->status == MG_ERR_AUTHEXPIRE_REMIND)
    {
        return MGCA_ENTITLE_INFO_ID2;
    }

    return -1;
}
int32_t app_mg_cas_gxmsg_ca_on_event_entitle(GxMsgProperty0_OnEvent* event)
{
    mg_cas_auth_st *data = NULL;
	if (NULL == event)
		return 0;

	data = (mg_cas_auth_st *)(event->buf);

	if (NULL == data)
		return 0;

    win_mg_cas_entitle_showDialog((void *)data,sizeof(mg_cas_auth_st));

    return 1;
}

int32_t app_mg_cas_msg_show(GxMsgProperty0_OnEvent* event)
{
    mg_cas_rolling_message_st *data = NULL;
	if (NULL == event)
		return 0;

	data = (mg_cas_rolling_message_st *)(event->buf);

	if (NULL == data)
		return 0;

    win_mg_cas_entitle_showDialog((void *)data,sizeof(mg_cas_rolling_message_st));

    return 1;
}

