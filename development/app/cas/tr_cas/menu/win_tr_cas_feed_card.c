#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"



extern uint8_t win_trcas_ipp_buy_menu_show_flag(void); 

static uint8_t s_bCreateFeedMenu = FALSE;


static void feedcard_pop_tips(char *strTips)
{

	app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, strTips, POPMSG_TYPE_OK);


	return;
}

/*wangjian add on 20150306*/
uint8_t win_trcas_feed_card_menu_show_flag(void)
{
	return s_bCreateFeedMenu;
}

void app_init_feed_card(void)
{	
	if (s_bCreateFeedMenu)
	{
		return;
	}

	if (0 == app_win_is_full_window())
	{
		return;	
	}

	if (TRUE == win_trcas_ipp_buy_menu_show_flag())
	{
		return;
	}
	
	GUI_EndDialog("win_prog_bar");
	GUI_CreateDialog("win_tr_cas_feed_card");
	
	return;
}

SIGNAL_HANDLER  int win_tr_cas_feed_card_create(const char* widgetname, void *usrdata)
{
	s_bCreateFeedMenu = TRUE;
	GUI_SetProperty("win_tr_cas_feed_card_tips", "string", "Please insert the parent-card");
	//app_play_stop();
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_feed_card_destroy(const char* widgetname, void *usrdata)
{
	s_bCreateFeedMenu = FALSE;
	//app_play_reset_play_timer(0);
	
	return 0;
}

SIGNAL_HANDLER  int win_tr_cas_feed_card_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	ca_mother_child_card_t tmpCardInfo = {0};
	char* strTmp = NULL;
	
	event = (GUI_Event *)usrdata;
	if (event->type == GUI_KEYDOWN)
	{
		switch (event->key.sym)
		{
			case KEY_EXIT:
				break;
				return EVENT_TRANSFER_STOP;
				
			case KEY_RECALL:
				GUI_EndDialog("win_tr_cas_feed_card");
				return EVENT_TRANSFER_STOP;
				
			case KEY_OK:
				tmpCardInfo.date_type = DVB_CA_MOTHER_CARD_INFO;
				tmpCardInfo.pos = 0;
				tmpCardInfo.state = 0; // init
				strTmp = app_cas_api_feed_mother_child_card(&tmpCardInfo);
				if (0 == strcmp(strTmp, "Please insert the parent-card"))
				{
					GUI_SetProperty("win_tr_cas_feed_card_tips", "string", strTmp);		
					feedcard_pop_tips(strTmp);
					
					tmpCardInfo.state = 1; // read feed data
					strTmp = app_cas_api_feed_mother_child_card(&tmpCardInfo);
					if (0 == strcmp(strTmp, "Please insert the child-card"))
					{	
						feedcard_pop_tips(strTmp);
						GUI_SetProperty("win_tr_cas_feed_card_tips", "string", "Feed Process");	
						GUI_SetInterface("flush", NULL);
						
						tmpCardInfo.state = 2; // write feed data
						strTmp = app_cas_api_feed_mother_child_card(&tmpCardInfo);
						feedcard_pop_tips(strTmp);
						
						if (0 == strcmp(strTmp, "Feeding successfully"))
						{
							GUI_EndDialog("win_tr_cas_feed_card");
						}
						else
						{
							GUI_SetProperty("win_tr_cas_feed_card_tips", "string", "Please insert the parent-card");
						}
					}
					else
					{
						feedcard_pop_tips(strTmp);
					}
				}
				else if (0 == strcmp(strTmp, "Please insert the child-card"))
				{
					feedcard_pop_tips(strTmp);
					GUI_SetProperty("win_tr_cas_feed_card_tips", "string", "Feed Process");	
					GUI_SetInterface("flush", NULL);
					
					tmpCardInfo.state = 2; // write feed data
					strTmp = app_cas_api_feed_mother_child_card(&tmpCardInfo);
					feedcard_pop_tips(strTmp);	
					
					if (0 == strcmp(strTmp, "Feeding successfully"))
					{
						GUI_EndDialog("win_tr_cas_feed_card");
					}
					else
					{
						GUI_SetProperty("win_tr_cas_feed_card_tips", "string", "Please insert the parent-card");
					}
				}
				else
				{	
					feedcard_pop_tips(strTmp);
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


