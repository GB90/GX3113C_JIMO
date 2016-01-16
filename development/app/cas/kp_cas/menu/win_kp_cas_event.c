#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_kp_cas_api_event.h"

void win_kp_cas_finger_show(uint8_t showFlag,char* message)
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

