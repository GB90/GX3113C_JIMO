#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas312_api_pop.h"
#include "app_mg_cas312_api_event.h"


static bool s_winPopmsgCreateFlag = false;
static int s_msgType = 0;
static mg_cas312_rolling_message_st s_312RollMsg = {0};


void win_mg_cas312_finger_show(uint8_t number, uint8_t showFlag, char* message)
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

void win_mg_cas312_ppvlogo_show(uint8_t showFlag, char *msg)
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

void win_mg_cas312_showDialog(void *data, int size)
{
#ifdef APP_SD
        uint32_t x = 210;
        uint32_t y = 160;
#endif
#ifdef APP_HD
        uint32_t x = 340;
        uint32_t y = 200;
#endif

	//printf("[win_mg_cas312_showDialog]------------.\n");

    if (data == NULL)
    {
    	return;
	}

    if (size == sizeof(mg_cas312_rolling_message_st))
    {
        mg_cas312_rolling_message_st *pdata = data;
        memset(&s_312RollMsg, 0, sizeof(mg_cas312_rolling_message_st));
        memcpy(&s_312RollMsg, pdata, sizeof(mg_cas312_rolling_message_st));
        s_msgType = DVB_CA_CURTAIN_INFO;
    }
    else
    {
        s_msgType = -1;
        return;
    }
    
    if (s_winPopmsgCreateFlag)
    {
        GUI_EndDialog("win_popmsg");
    }
    
    GUI_CreateDialog("win_popmsg");
	GUI_SetProperty("win_popmsg", "move_window_x", &x);
	GUI_SetProperty("win_popmsg", "move_window_y", &y);
    s_winPopmsgCreateFlag = true;
}

SIGNAL_HANDLER int win_popmsg_create(const char* widgetname, void *usrdata)
{
    //ca_get_date_t get_data = {0};
	char* str = NULL;
	
    if (s_msgType == DVB_CA_CURTAIN_INFO)
    {
        str = (char *)s_312RollMsg.content;
    }
    
	if (NULL != str)
	{
        GUI_SetProperty("win_popmsg_text", "string", str);			
	}

    return 0;
}

SIGNAL_HANDLER int win_popmsg_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	if (NULL == usrdata)
	{
		return EVENT_TRANSFER_STOP;
	}

	event = (GUI_Event *)usrdata;
    if (event->type == GUI_KEYDOWN)
    {
        switch (event->key.sym)
		{
            case KEY_EXIT:
			case KEY_RECALL:
            case KEY_OK:
				GUI_EndDialog("win_popmsg");
                s_winPopmsgCreateFlag = false;
				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
    }

	return EVENT_TRANSFER_KEEPON;
}

