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
#include "DiviGuardCa.h"
#include "app_common_porting_stb_api.h"
#include "app_divitone_cas_api_watchlevel.h"
extern UINT8 Repeattime;

uint8_t app_divitone_cas_api_init_watchlevel_data(void)
{
	return 0;
}

char * app_divitone_cas_api_get_watchlevel_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    	static char s_text_buffer[30];

    	INT32 watchlevel;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
	{
		case DIVI_RATING_LEVEL_ID:
			watchlevel = DIVI_GetSMCUserLevel();
			if (watchlevel == -1)
				watchlevel = 0;
			sprintf(s_text_buffer, "%ld", watchlevel);
			return s_text_buffer;
			break;
		default:
			break;
	}
	return NULL;

}

char* app_divitone_cas_api_change_watchlevel(ca_rating_t* data)
{
	INT32 wRet = 0;
	uint8_t pin[6+1]={0};
	INT8 watchlevel;

	if (NULL == data)
		return NULL;
	
	memcpy(pin,data->pin,6);
	if(strlen((char*)pin)!=6)
	{
		 return (char*)"Input new PIN";	 
	}

	watchlevel = data->rate;

	Repeattime = 0;
 	wRet = DIVI_SetCardLevel(pin,watchlevel,&Repeattime);
	if(wRet == 0)
		return (char*)"Set Successfully!";
	else
		return (char*)"PIN Error!";
	
	return NULL;

}







