#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas_api_pop.h"
#include "app_mg_cas_api_event.h"

bool g_win_popmsg_createflag = false;

void win_mg_cas_finger_show(uint8_t number,uint8_t showFlag,char* message)
{
	if (DVB_CA_FINGER_FLAG_HIDE == showFlag)
	{	
		GUI_SetProperty("win_full_screen_text_finger","state","hide");
	}
	else if (DVB_CA_FINGER_FLAG_SHOW == showFlag)
	{
		GUI_SetProperty("win_full_screen_text_finger","string", message);					
		GUI_SetProperty("win_full_screen_text_finger","state","show");
	}
	return;
}

void win_mg_cas_ppvlogo_show(uint8_t showFlag,char *msg)
{

    if (DVB_CA_FINGER_FLAG_HIDE == showFlag)
	{	
		GUI_SetProperty("win_full_screen_text_finger1","state","hide");
	}
	else if (DVB_CA_FINGER_FLAG_SHOW == showFlag)
	{
		GUI_SetProperty("win_full_screen_text_finger1","string", msg);					
		GUI_SetProperty("win_full_screen_text_finger1","state","show");
	}
	return;
}
static int msg_type = 0;
static mg_cas_auth_st auth = {0};
static mg_cas_rolling_message_st msg = {0};
void win_mg_cas_entitle_showDialog(void *data,int size)
{
#ifdef APP_SD
        uint32_t x = 210;
        uint32_t y = 150;
#endif
#ifdef APP_HD

        uint32_t x = 340;
        uint32_t y = 200;
#endif
    if(data == NULL)
        return ;

    if(size == sizeof(mg_cas_auth_st))
    {
        mg_cas_auth_st *pdata = data;
        if(FALSE == app_mg_cas_gxmsg_ca_check_entitle(pdata))
        {
            return ;
        }
        memcpy(&auth,pdata,sizeof(mg_cas_auth_st));
        msg_type = DVB_CA_ENTITLE_INFO;
    }
    else if(size == sizeof(mg_cas_rolling_message_st))
    {
        mg_cas_rolling_message_st *pdata = data;
        memset(&msg,0,sizeof(mg_cas_rolling_message_st));
        memcpy(&msg,pdata,sizeof(mg_cas_rolling_message_st));
        msg_type = DVB_CA_CURTAIN_INFO;
    }
    else
    {
        msg_type = -1;
        return ;
    }
    
    if(g_win_popmsg_createflag)
    {
        GUI_EndDialog("win_popmsg");
    }
    
    GUI_CreateDialog("win_popmsg");
	GUI_SetProperty("win_popmsg", "move_window_x", &x);
	GUI_SetProperty("win_popmsg", "move_window_y", &y);
    g_win_popmsg_createflag = true;
}


SIGNAL_HANDLER int win_popmsg_create(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
	char* str = NULL;
    if(msg_type == DVB_CA_ENTITLE_INFO)
    {
        get_data.date_type = DVB_CA_ENTITLE_INFO;	
    	get_data.ID = app_mg_cas_gxmsg_ca_entitle_ID((void *)&auth);
    	str = app_cas_api_get_data(&get_data);
    }
    else if(msg_type == DVB_CA_CURTAIN_INFO)
    {
        str = (char *)msg.content;
    }
    
	if (NULL != str )
	{
        GUI_SetProperty("win_popmsg_text", "string",str);			
	}
    return 0;
}

SIGNAL_HANDLER int win_popmsg_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	if(NULL == usrdata) return EVENT_TRANSFER_STOP;

	event = (GUI_Event *)usrdata;
    if(event->type == GUI_KEYDOWN)
    {
        switch(event->key.sym)
		{
            case KEY_EXIT:
			case KEY_RECALL:
            case KEY_OK:
				GUI_EndDialog("win_popmsg");
                g_win_popmsg_createflag = false;
				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
    }
	return EVENT_TRANSFER_KEEPON;
}

