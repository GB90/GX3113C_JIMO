#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_ippv_slot.h"

SIGNAL_HANDLER  int win_kp_cas_ippv_slot_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_ippv_slot_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_kp_cas_ippv_slot_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_kp_cas_ippv_slot");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

SIGNAL_HANDLER  int win_kp_cas_ippv_slot_get_count(const char* widgetname, void *usrdata)
{
    ca_get_count_t get_count = {0};	
    get_count.date_type = DVB_CA_IPPV_SLOT_INFO;
    app_cas_api_get_count(&get_count); 
    return get_count.totalnum;
}

SIGNAL_HANDLER  int win_kp_cas_ippv_slot_get_data(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
    unsigned int      Row    = 0;
    unsigned int      ID    = 0;
	ListItemPara *item_para =NULL;
	if(NULL == widgetname || NULL == usrdata )
	{
		return (1);
	}
	item_para = (ListItemPara *)(usrdata);

	Row = item_para->sel;

	get_data.date_type = DVB_CA_IPPV_SLOT_INFO;
	get_data.pos = Row; // 行

	for (ID = DVB_CA_IPPV_SLOT_INFO;ID<=KP_IPPV_SLOT_USED_ID;ID++)
	{
		get_data.ID = ID;
		item_para->string = app_cas_api_get_data(&get_data);

		if (ID < KP_IPPV_SLOT_USED_ID)
		{
			/*
			* 不是最后一列
			*/
			item_para= item_para->next;
			if(NULL == item_para) 
				return GXCORE_ERROR;
		}
	}

	return 0;
}




