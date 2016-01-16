#include "app_common_play.h"
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"

/*
* app_cd_cas30_show_ca_prompt与app_show_prompt实现功能有可能存在差别
* 
*/
extern void app_show_prompt(char* context);
extern void app_hide_prompt(void); 

void app_kp_cas_show_ca_prompt(char* context)
{
	app_show_prompt(context);
	return;		
}


void app_kp_cas_hide_ca_prompt(void)
{
	app_hide_prompt();
	return;			
}




