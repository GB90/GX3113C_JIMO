#include "app.h"
#include "app_qz_cas_api_pop.h"


static int32_t widget_init_mode = 0;

SIGNAL_HANDLER int win_qz_cas_masterslave_create(const char* widgetname, void *usrdata)
{
	char* osd_language=NULL;
    int32_t mode_value = 0;
        
    osd_language = app_flash_get_config_osd_language();
    if (0 == strcmp(osd_language,LANGUAGE_CHINESE))
    {
 //   	GUI_SetProperty("win_qz_cas_masterslave_tip_image_exit", "img", "tips_exit.bmp");
    }
    else if (0 == strcmp(osd_language,LANGUAGE_ENGLISH))
     {
    //	GUI_SetProperty("win_qz_cas_masterslave_tip_image_exit", "img", "tips_exit_e.bmp");
    }

    mode_value = qzcas_get_config_hs_mode();
    GUI_SetProperty("win_qz_cas_masterslave_boxitem1_combobox", "select", (void*)&mode_value);
    widget_init_mode = mode_value;

    //设置时间
	//GUI_SetProperty(OSDSET_TEXT_TIME, "string", app_win_get_local_date_string());
    //初始化菜单语言
	//init_menu_language();

	return 0;
}

SIGNAL_HANDLER  int win_qz_cas_masterslave_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

static status_t key_exit(void)
{
    int32_t mode_value = 0;     
    
    /*check modified?*/
    GUI_GetProperty("win_qz_cas_masterslave_boxitem1_combobox", "select", (void*)&mode_value);

    /*not modified, exit directly*/
    if(mode_value == widget_init_mode)
    {
        return GXCORE_SUCCESS;
    }
    
    /*popmsg to save?*/
    popmsg_ret ret_pop = POPMSG_RET_YES;
    ret_pop = app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START, STR_CHK_SAVE, POPMSG_TYPE_YESNO);
    
	if(ret_pop == POPMSG_RET_YES)
	{
        qzcas_set_config_hs_mode(mode_value);
	}	
	return GXCORE_SUCCESS;
}


SIGNAL_HANDLER int win_qz_cas_masterslave_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	
	if(NULL == usrdata) return EVENT_TRANSFER_STOP;

	event = (GUI_Event *)usrdata;

	switch(event->key.sym)//按键值
	{
    	case KEY_EXIT:
    	case KEY_RECALL:
        case KEY_MENU:
            key_exit();
	        GUI_EndDialog("win_qz_cas_masterslave");
    		return EVENT_TRANSFER_STOP;
                        
		case APPK_LEFT:
		case APPK_RIGHT:
			//key_lr(event->key.sym);
			break;				

		case APPK_OK:
			//key_ok();
			break;
		
		default:
			break;
	}

	
	return EVENT_TRANSFER_STOP;
}

