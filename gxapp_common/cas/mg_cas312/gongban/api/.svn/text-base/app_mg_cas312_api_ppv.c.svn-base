/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_mg_cas_api_ppv.c
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2014.4.14		  chenth 	 			creation
*****************************************************************************/
#include "mg312def.h"
#include "app_common_porting_stb_api.h"
#include "app_mg_cas312_api_ppv.h"


int32_t app_mg_cas312_api_init_ppv_data(void)
{
	return 0;
}

char* app_mg_cas312_api_get_ppv_data(ca_get_date_t *data)
{
    uint32_t pos = 0; 
	uint32_t ID = 0; 
	MG_U8 curPPVProgNum;
	MG_U32 uPrice, fPrice;
	MG_S32 cardBalance;
    static char s_text_buffer[70] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer, 0, sizeof(s_text_buffer));

	switch (ID)
	{
    case MG312_PPV_ISNO:
        curPPVProgNum = MG_Get_Current_Program();
        sprintf(s_text_buffer,"%d",
           (MG_Get_Program_Type(curPPVProgNum) == MG312_TYPE_PPV)?1:0);
        return s_text_buffer;
		
    case MG312_PPV_ORDER_STATUS_ID:
		curPPVProgNum = MG_Get_Current_Program();
        sprintf(s_text_buffer,"%d",
           (MG_Get_TeleviwPPVTag(curPPVProgNum) == MG_TRUE)?1:0);
        return s_text_buffer;
		
	case MG312_PPV_ORDER_STATUS_STRING_ID:
		curPPVProgNum = MG_Get_Current_Program();
        sprintf(s_text_buffer,"%s",
           (MG_Get_TeleviwPPVTag(curPPVProgNum) == MG_TRUE)?"Ordered":"No order");
        return s_text_buffer;
		
    case MG312_PPV_PRO_COST_ID:
        curPPVProgNum = MG_Get_Current_Program();
		uPrice = MG_Get_Program_Price(curPPVProgNum) / 100;
		fPrice = MG_Get_Program_Price(curPPVProgNum) % 100;
		sprintf(s_text_buffer, "%d.%02d", uPrice, fPrice);
		return s_text_buffer;
		
    case MG312_PPV_CARD_BLE:
        cardBalance = MG_Get_Card_Balance();
		if (cardBalance >= 0)
		{
			uPrice = cardBalance / 10000;
			fPrice = (cardBalance - uPrice * 10000) / 100;
			sprintf(s_text_buffer, "%d.%02d", uPrice, fPrice);
		}
		else
		{
			cardBalance *= (-1);
			uPrice = cardBalance / 10000;
			fPrice = (cardBalance - uPrice * 10000) / 100;
			sprintf(s_text_buffer, "-%d.%02d", uPrice, fPrice);
		}
		
		return s_text_buffer;
		
	default:
		break;
	}
	
	return NULL;
}

uint8_t app_mg_cas312_api_buy_ppv_data(ca_buy_ipp_t *data)
{
    if (NULL == data)
    {
    	return 0;
	}
    
    if (data->buystate)
    {
    	MG_Set_CurTeleviewPPVTag(MG_TRUE);
	}
    else
    {
    	MG_Set_CurTeleviewPPVTag(MG_FALSE);
	}
	
    return 1;
}

