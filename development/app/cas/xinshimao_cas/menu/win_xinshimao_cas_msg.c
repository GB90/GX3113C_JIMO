#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_xinshimao_cas_api_rolling_message.h"

bool g_win_popmsg_createflag = false;
static xinshimao_cas_rolling_message_st msg = {0};
void win_xinshimao_cas_entitle_showDialog(void *data,int size)
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

    if(size == sizeof(xinshimao_cas_rolling_message_st))
    {
        memset(&msg,0,sizeof(xinshimao_cas_rolling_message_st));
        memcpy(&msg,data,sizeof(xinshimao_cas_rolling_message_st));
    }
    else
    {
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

	char* str = NULL;
    str = (char *)msg.content;
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

