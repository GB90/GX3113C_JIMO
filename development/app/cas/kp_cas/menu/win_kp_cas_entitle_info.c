#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_entitle.h"


static ca_get_count_t get_count = {0};	

SIGNAL_HANDLER  int app_kp_cas_entitle_create(const char* widgetname, void *usrdata)
{
    uint32_t pos = 0;
    char sTextBuffer[32];

	get_count.date_type = DVB_CA_ENTITLE_INFO;
	app_cas_api_get_count(&get_count);
	sprintf(sTextBuffer,"%d",get_count.totalnum);
	GUI_SetProperty("win_kp_cas_entitle_text_entitle_count", "string", sTextBuffer);
	GUI_SetProperty("win_kp_cas_entitle_listview","select",(void*)&pos);
	return 0;
}

SIGNAL_HANDLER  int  app_kp_cas_entitle_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int app_kp_cas_entitle_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	switch(event->type)
	{
		case GUI_KEYDOWN:
			switch(event->key.sym)
			{
				case KEY_EXIT:
					GUI_EndDialog("win_kp_cas_entitle_info");
					return EVENT_TRANSFER_STOP;
				case KEY_MENU:
				case KEY_RECALL:
					GUI_EndDialog("win_kp_cas_entitle_info");
					return EVENT_TRANSFER_STOP;
				case KEY_F1:
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
					return EVENT_TRANSFER_STOP;
				default:
					break;
			}
			break;
		default:
			break;
	}
	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int app_kp_cas_entitle_get_count(const char* widgetname, void *usrdata)
{ 
    char sTextBuffer[32];
    
    get_count.date_type = DVB_CA_ENTITLE_INFO;
    app_cas_api_get_count(&get_count);
    sprintf(sTextBuffer,"%d",get_count.totalnum);
	GUI_SetProperty("win_kp_cas_entitle_text_entitle_count", "string", sTextBuffer);

    return get_count.totalnum;
}

SIGNAL_HANDLER  int app_kp_cas_entitle_get_data(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
    unsigned int      ID    = 0;
    unsigned int      Row    = 0;
    static char      id[32] = {0};
    ListItemPara *item_para =NULL;
    if(NULL == widgetname || NULL == usrdata )
    {
        return 1;
    }
    
    item_para = (ListItemPara *)(usrdata);
    Row = item_para->sel;
    
	memset(id,0,sizeof(id));
	sprintf(id,"%d",Row+1);
	item_para->string = id;
	item_para = item_para->next;
    
	get_data.date_type = DVB_CA_ENTITLE_INFO;
	get_data.pos = Row; // 行

	for (ID = KP_ENTITLE_ID;ID <= KP_ENTITLE_END;ID++)
	{
		get_data.ID = ID;
		item_para->string = app_cas_api_get_data(&get_data);

		if (ID < KP_ENTITLE_END)
		{
			/*
			* 不是最后一列
			*/
			item_para= item_para->next;
		}
	}
	return 0;
}


