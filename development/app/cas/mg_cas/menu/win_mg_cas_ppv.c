/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	win_mg_cas_ppv.c
* Author    : 	chenth
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	Ñ¸³ÛCA
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.04.14		  chenth 	 			creation
*****************************************************************************/
#include <gxtype.h>
#include "app_common_porting_stb_api.h"
#include "app_win_interface.h"
#include "app_mg_cas_api_ppv.h"

static int ppv_can_order = FALSE;
int mg_curprogram_is_ppv()
{
    ca_get_date_t get_data = {0};
	char* str = NULL;

	get_data.date_type = DVB_CA_IPPV_POP_INFO;	
	get_data.ID = MG_PPV_ISNO;
	str = app_cas_api_get_data(&get_data);
	if (NULL != str )
	{	
        return (atoi(str)>0)?TRUE:FALSE;
	}

    return FALSE;
}

//µ±Ç°½ÚÄ¿¶©¹º×´Ì¬
static int mg_ppv_orderstatus()
{
    ca_get_date_t get_data = {0};
	char* str = NULL;

	get_data.date_type = DVB_CA_IPPV_POP_INFO;	
	get_data.ID = MG_PPV_ORDER_STATUS_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{	
        return (atoi(str)>0)?TRUE:FALSE;
	}

    return FALSE;
}

static int mg_ppv_order(int order)
{
    ca_buy_ipp_t data;
    memset(&data,0,sizeof(ca_buy_ipp_t));

    data.date_type = DVB_CA_IPPV_POP_INFO;
    data.buystate = (order == TRUE)?1:0;
    return app_cas_api_buy_ipp(&data);
}

SIGNAL_HANDLER  int win_mg_ppv_create(const char* widgetname, void *usrdata)
{
    ca_get_date_t get_data = {0};
	char* str = NULL;
    int program_pric = 0;
    int card_Balance = 0;
    
    ppv_can_order = FALSE;
    
	get_data.date_type = DVB_CA_IPPV_POP_INFO;	
	get_data.ID = MG_PPV_ORDER_STATUS_STRING_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        GUI_SetProperty("win_mg_ppv_p1", "string",str);			
	}

    get_data.ID = MG_PPV_PRO_COST_ID;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        program_pric = atoi(str);
        GUI_SetProperty("win_mg_ppv_p2", "string",str);			
	}

    get_data.ID = MG_PPV_CARD_BLE;
	str = app_cas_api_get_data(&get_data);
	if (NULL !=str )
	{
        card_Balance = atoi(str);
        GUI_SetProperty("win_mg_ppv_p3", "string",str);			
	}

    ppv_can_order = (card_Balance >= program_pric)?TRUE:FALSE;
    
    if(FALSE == mg_ppv_orderstatus())
    {
        GUI_SetFocusWidget("win_mg_ppv_button_yes");
    }
    else
    {
        GUI_SetFocusWidget("win_mg_ppv_button_no");
    }
	return 0;
}

SIGNAL_HANDLER  int win_mg_ppv_destroy(const char* widgetname, void *usrdata)
{
	return 0;
}

SIGNAL_HANDLER  int win_mg_ppv_keypress(const char* widgetname, void *usrdata)
{
	GUI_Event *event = NULL;
	event = (GUI_Event *)usrdata;
	if(event->type == GUI_KEYDOWN)
	{
		switch(event->key.sym)
		{
			case KEY_EXIT:
                GUI_EndDialog("win_mg_cas_ppv"); 
				break;
				return EVENT_TRANSFER_STOP;
			case KEY_RECALL:
				GUI_EndDialog("win_mg_cas_ppv");
				return EVENT_TRANSFER_STOP;
			case KEY_OK:
               {
                    char* name = NULL;
    	            name = (char*)GUI_GetFocusWidget();
                    if(0 == strcmp(name, "win_mg_ppv_button_yes"))
        			{
        				if( FALSE == mg_ppv_orderstatus())
                        {
                            if(TRUE == ppv_can_order)
                            {
                                mg_ppv_order(TRUE);
                                printf("mg ppv todo order!\n");
                            }
                            else //Óà¶î²»×ã
                            {
                                app_popmsg(POP_DIALOG_X_START, POP_DIALOG_Y_START,"Err No Money",POPMSG_TYPE_OK);
                                return EVENT_TRANSFER_STOP;
                            }
                        }
        			}
        			else if(0 == strcasecmp(name, "win_mg_ppv_button_no"))
        			{
        				if( TRUE == mg_ppv_orderstatus())
                        {
                            mg_ppv_order(FALSE);
                            printf("mg ppv todo unorder!\n");
                        }
        			}
			    }
                GUI_EndDialog("win_mg_cas_ppv");
				return EVENT_TRANSFER_STOP;
			case KEY_POWER:
				return EVENT_TRANSFER_KEEPON;
			default:
				return EVENT_TRANSFER_KEEPON;
		}
	}

	return EVENT_TRANSFER_KEEPON;
}





