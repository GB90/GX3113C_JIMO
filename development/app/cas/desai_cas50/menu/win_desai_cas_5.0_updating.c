/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_desai_cas_5.0_updating.c
* Author    : 	qm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2015.1.28		  qm 	 			creation
*****************************************************************************/
#include "app_common_porting_stb_api.h"
#include "desai_ca_api.h"
#include "app_win_interface.h"
#include "app_desai_cas_5.0_api_pop.h"

static int32_t card_update_flag = FALSE;

int32_t app_desai_cas50_gxmsg_ca_on_event_updating(GxMsgProperty0_OnEvent* event)
{
    DESAI_CAS50_CARD_UPDATE_S *updateData = (DESAI_CAS50_CARD_UPDATE_S*)event->buf;
	char* focus_Window = (char*)GUI_GetFocusWindow();
	int8_t strPercent[10] = {0};
    int32_t barValue = 0;

	if (NULL == updateData)
		return 0;

    if (updateData->bStatus == DESAI_CAS50_CARD_UPDATE_START)
    {
        printf("show updating bar...\n");
		if ((NULL != focus_Window)&&(strcasecmp("win_prog_bar", focus_Window) == 0))
		{
			GUI_EndDialog("win_prog_bar");

			GxCore_ThreadDelay(300);
			GUI_SetInterface("flush", NULL);
			GxCore_ThreadDelay(50);
		}
        GUI_CreateDialog("win_desai_cas50_updating");

        GUI_SetProperty("win_desai_cas50_updating_note", "string", "(DSCA)Card Updating, Pls Don`t Pown Down!");
        barValue = updateData->bProgress;
        GUI_SetProperty("win_desai_cas50_updating_progbar", "value", &barValue);

        sprintf((char *)strPercent,"%d%%",updateData->bProgress);
        GUI_SetProperty("win_desai_cas50_updating_text_percent", "string", strPercent);

        card_update_flag = TRUE;
    }
    else if (updateData->bStatus == DESAI_CAS50_CARD_UPDATE_PROCESS)
    {
        printf("updating bar(%d)\n", updateData->bProgress);

        barValue = updateData->bProgress;
        GUI_SetProperty("win_desai_cas50_updating_progbar", "value", &barValue);

        sprintf((char *)strPercent,"%d%%",updateData->bProgress);
        GUI_SetProperty("win_desai_cas50_updating_text_percent", "string", strPercent);
    }
    else if (updateData->bStatus == DESAI_CAS50_CARD_UPDATE_SUCCESS)
    {
        printf("updating success(%d)\n", updateData->bProgress);

        GUI_SetProperty("win_desai_cas50_updating_note", "string", "(DSCA)Card Update Sucess");
	    GxCore_ThreadDelay(3000);

        GUI_EndDialog("win_desai_cas50_updating");

        card_update_flag = FALSE;

    }
    else if (updateData->bStatus == DESAI_CAS50_CARD_UPDATE_ERR)
    {
        printf("updating err(%d)\n", updateData->bProgress);
        GUI_SetProperty("win_desai_cas50_updating_note", "string", "(DSCA)Card Update failed");
	    GxCore_ThreadDelay(5000);

        GUI_EndDialog("win_desai_cas50_updating");

        card_update_flag = FALSE;
    }
	return 0;
}

/*
* 2014-11-27，qm,获取卡升级状态
*/
int32_t app_desai_cas50_get_card_update_status(void)
{
	return card_update_flag;
}
/**/

SIGNAL_HANDLER  int win_desai_cas50_updating_create(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_updating_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_desai_cas50_updating_change(const char* widgetname, void *usrdata)
{
	return 0;
}

