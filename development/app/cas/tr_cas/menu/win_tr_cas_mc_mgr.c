#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"


SIGNAL_HANDLER  int win_tr_cas_mc_mgr_create(const char* widgetname, void *usrdata)
{
	GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string","Please insert the parent-card");			
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_mc_mgr_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_mc_mgr_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	ca_mother_child_card_t card = {0};
	char* str = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
				
			case KEY_RECALL:
				GUI_EndDialog("win_tr_cas_mc_mgr");
				return EVENT_TRANSFER_STOP;
				
			case KEY_OK:
			card.date_type = DVB_CA_MOTHER_CARD_INFO;
			card.pos = 0;
			card.state = 0; // init
			str = app_cas_api_feed_mother_child_card(&card);
			if (0 == strcmp(str,"Please insert the parent-card"))
			{
				GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		

				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

				
				card.state = 1; // read feed data
				str = app_cas_api_feed_mother_child_card(&card);
				if (0 == strcmp(str,"Please insert the child-card"))
				{
					GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		
	
					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

					card.state = 2; // write feed data
					str = app_cas_api_feed_mother_child_card(&card);
					GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		

					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

					return EVENT_TRANSFER_STOP;
				}
				else
				{
					GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		

					app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

					return EVENT_TRANSFER_STOP;
				}
			}
			else if (0 == strcmp(str,"Please insert the child-card"))
			{
				GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);	

				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
	
				card.state = 2; // write feed data
				str = app_cas_api_feed_mother_child_card(&card);
				GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		

				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);
		
									
			}
			else
			{
				GUI_SetProperty("win_tr_cas_mc_mgr_operator3", "string",str);		

				app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,str,POPMSG_TYPE_OK);

				return EVENT_TRANSFER_STOP;
			}
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_STOP;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}

