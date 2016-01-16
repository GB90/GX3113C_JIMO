/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_ca_api.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "app_common_epg.h"
#include "gui_timer.h"
#include "app_divitone_cas_api_pop.h"

static divitone_cas_active_child_st ca_active_child = {0};

extern void app_divitone_cas_activechild_show(uint8_t showFlag,char* message);

int32_t app_divitone_cas_gxmsg_ca_on_event_activechild(GxMsgProperty0_OnEvent* event)
{
	divitone_cas_active_child_st* active_child = NULL;
	if (NULL == event)
		return 0;

	active_child = (divitone_cas_active_child_st*)(event->buf);

	if (NULL == active_child)
		return 0;
	ca_active_child.param1 = active_child->param1;

	switch(ca_active_child.param1)
	{
		case 0:
			break;
		case 1:
			app_divitone_cas_activechild_show(1,"Please insert mother card");
			break;
		case 2:
			app_divitone_cas_activechild_show(1,"Invailid mother card");
			break;
		case 3:
			app_divitone_cas_activechild_show(1,"Please insert child card");
			break;
		case 4:
			app_divitone_cas_activechild_show(1,"Invailid child card");
			break;
		case 5:
			app_divitone_cas_activechild_show(1,"Child&Mother card is unpaired");
			break;
		case 6:
			app_divitone_cas_activechild_show(1,"Read failed,insert again");
			break;
		case 7:
			app_divitone_cas_activechild_show(1,"Read failed,insert again");
			break;
		case 8:
			app_divitone_cas_activechild_show(1,"Not in actived worktime");
			break;
		case 9:
			app_divitone_cas_activechild_show(1,"Successed,next child card or exit");
			break;
		default:
			break;
			
	}

	return 1;
}



