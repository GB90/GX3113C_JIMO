#include "app.h"
#include "win_main_menu.h"

SIGNAL_HANDLER int app_search_menu_create(const char* widgetname, void *usrdata)
{
#ifdef CUST_JIMO
			GUI_SetProperty("win_search_menu_boxitem6", "state","disable");
			GUI_SetProperty("win_search_menu_boxitem6", "state","hide");
			GUI_SetProperty("win_search_menu_button12", "unfocus_img","DS_ADVANCED_GRAY.bmp");
			GUI_SetProperty("win_search_menu_button11", "unfocus_img","DS_ADVANCED_GRAY.bmp");
#else
			GUI_SetProperty("win_search_menu_boxitem6", "state","enable");
			GUI_SetProperty("win_search_menu_boxitem6", "state","show");
#endif
	
	return 0;
}
SIGNAL_HANDLER int app_search_menu_keypress(const char* widgetname, void *usrdata)
{ 
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;

	if(event->type == GUI_KEYDOWN)
 	{
		switch(event->key.sym)
 		{
			case KEY_RECALL:
			case KEY_MENU:	
			case KEY_EXIT:
				GUI_EndDialog("win_search_menu");
				return EVENT_TRANSFER_STOP;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}
SIGNAL_HANDLER  int app_search_menu_destroy(const char* widgetname, void *usrdata)
{ 
	return 0;	
}
SIGNAL_HANDLER int app_search_menu_box_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	int i =0;
	event = (GUI_Event *)usrdata;
	switch(event->type)
	{
		case GUI_MOUSEBUTTONDOWN:
			break;

		case GUI_KEYDOWN:
			switch(event->key.sym)
			{
				case KEY_UP:
					GUI_GetProperty("win_search_menu_box", "select", &i);
					i--;
					if (i <0)
					{ 
#ifdef CUST_JIMO
						i = 2;
#else
						i = 3;
#endif
					}
					GUI_SetProperty("win_search_menu_box", "select", &i);
					return EVENT_TRANSFER_STOP;
				case KEY_DOWN:
					GUI_GetProperty("win_search_menu_box", "select", &i);
					i++;
					
#ifdef CUST_JIMO
					if (i > 2)
#else
					if (i > 3)
#endif
					{
						i = 0;
					} 
					GUI_SetProperty("win_search_menu_box", "select", &i);
					return EVENT_TRANSFER_STOP;
				case KEY_OK:
					GUI_GetProperty("win_search_menu_box", "select", &i);
					switch(i)
					{ 
						case 0:
#ifdef CUST_JIMO
							GUI_CreateDialog("win_search_result");
							GUI_SetInterface("flush", NULL);
							app_win_jimo_search();
#else
							app_win_auto_search();
#endif
							break;
						case 1:
#ifdef CUST_JIMO						
							{
							extern int PasswdCheckOK();
							extern void win_SetName(char *win);
							win_SetName("win_manual_search");
							GUI_CreateDialog("win_password_check");
							if(!PasswdCheckOK())
							{
							}
							}
#else
							GUI_CreateDialog("win_manual_search");
#endif
							break;
						case 2:

#ifdef CUST_JIMO
							{
							extern int PasswdCheckOK();
							extern void win_SetName(char *win);
							win_SetName("win_all_search");
							GUI_CreateDialog("win_password_check");
							if(!PasswdCheckOK())
							{
							}
							}
#else
							GUI_CreateDialog("win_all_search");
#endif
							break;
						case 3:
							GUI_CreateDialog("win_main_frequecy_set");
							break;

					}
					return EVENT_TRANSFER_KEEPON;
				default:
					return EVENT_TRANSFER_KEEPON;
			}

		default:
			return EVENT_TRANSFER_KEEPON;
	}
	return EVENT_TRANSFER_KEEPON;
} 


