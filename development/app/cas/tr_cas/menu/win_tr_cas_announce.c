#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_tr_cas_api_announce.h"

static uint8_t s_bAnnounceShow = 0;
static uint8_t s_bValidData = 0;
static TR_CAS_ANNOUNCE_INFO_t s_curAnnounceInfo = {0};


uint8_t win_tr_cas_announce_show(void *data, int size)
{
	TR_CAS_ANNOUNCE_INFO_t *pTmpData = NULL;
	uint32_t x = 210;
	uint32_t y = 150;

	if (NULL == data)
	{
		return 1;
	}

	if (size == sizeof(TR_CAS_ANNOUNCE_INFO_t))
	{
		pTmpData = (TR_CAS_ANNOUNCE_INFO_t *)data;
		memset(&s_curAnnounceInfo, 0, sizeof(TR_CAS_ANNOUNCE_INFO_t));
		memcpy(&s_curAnnounceInfo, pTmpData, sizeof(TR_CAS_ANNOUNCE_INFO_t));
		s_bValidData = 1;
	}
	else
	{
		s_bValidData = 0;
		printf("[win_tr_cas_announce_show]invalid announce data size!!!\n");
		return 2;
	}

	/*wangjian modify on 20150327 for update announce display string.*/
	if (0 == s_bAnnounceShow)
	{
		GUI_CreateDialog("win_tr_cas_announce");
		GUI_SetProperty("win_tr_cas_announce", "move_window_x", &x);
		GUI_SetProperty("win_tr_cas_announce", "move_window_y", &y);
		s_bAnnounceShow = 1;
	}
	else
	{
		GUI_SetProperty("win_announce_head", "string", s_curAnnounceInfo.title);
		GUI_SetProperty("win_announce_content", "string", s_curAnnounceInfo.content);
	}
	
	return 0;
}

uint8_t win_tr_cas_announce_get_show_status(void)
{
	return s_bAnnounceShow;
}

SIGNAL_HANDLER int win_tr_cas_announce_create(const char* widgetname, void *usrdata)
{	
    if (s_bValidData)
    {
    	GUI_SetProperty("win_announce_head", "string", s_curAnnounceInfo.title);
		GUI_SetProperty("win_announce_content", "string", s_curAnnounceInfo.content);
    }
    
    return 0;
}

SIGNAL_HANDLER  int win_tr_cas_announce_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER int win_tr_cas_announce_keypress(const char* widgetname, void *usrdata)
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
				GUI_EndDialog("win_tr_cas_announce");
                s_bAnnounceShow = 0;
				return EVENT_TRANSFER_STOP;

			case KEY_UP:
				GUI_SetProperty("win_announce_content", "page_up", NULL);
				return EVENT_TRANSFER_STOP;
					
			case KEY_DOWN:
				GUI_SetProperty("win_announce_content", "page_down", NULL);
				return EVENT_TRANSFER_STOP;			
				
			default:
				return EVENT_TRANSFER_KEEPON;
		}
    }
	
	return EVENT_TRANSFER_KEEPON;
}

