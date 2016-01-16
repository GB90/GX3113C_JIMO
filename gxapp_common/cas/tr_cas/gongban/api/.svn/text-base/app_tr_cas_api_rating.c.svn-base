/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_rating.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.05		  zhouhm 	 			creation
*****************************************************************************/
#include "Tr_Cas.h"
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_rating.h"

static U8  s_rating = 0;


uint8_t app_tr_cas_api_init_rating_data(void)
{
	MC_SCARD_GetRating(&s_rating);
	return 1;	
}

char* app_tr_cas_api_get_rating_data(ca_get_date_t *data)
{
	uint32_t pos = 0;                             
	uint32_t ID = 0;
    static char s_text_buffer[30] = {0};

	if (NULL == data)
	{
		return NULL;
	}

	pos = data->pos;
	ID = data->ID;
	memset(s_text_buffer, 0, sizeof(s_text_buffer));
	
	switch (ID)
	{
		case TR_RATING_LEVEL_ID:
			sprintf(s_text_buffer, "%d", s_rating);
			return s_text_buffer;
			break;
			
		default:
			break;
	}
	
	return NULL;
}

char* app_tr_cas_api_change_rating(ca_rating_t* data)
{
	uint8_t Password1[6+1] = {0};
	int hresult = 0;
	char byRating = 0;

	if (NULL == data)
	{
		return NULL;
	}
	
	memcpy(Password1, data->pin, 6);
	if (strlen((char *)Password1) != 6)
	{
		return (char *)"Input 6-bit PIN";
	}

	Password1[0] = Password1[0]-0x30;
	Password1[1] = Password1[1]-0x30;
	Password1[2] = Password1[2]-0x30;
	Password1[3] = Password1[3]-0x30;
	Password1[4] = Password1[4]-0x30;
	Password1[5] = Password1[5]-0x30;

	//printf("\ndata->rate=%d\n", data->rate);
	byRating = data->rate;// +3;
	
	hresult = MC_SCARD_SetRating(Password1, 6 , byRating);
	if (0 == hresult)
	{
		return (char*)"Modify level successful";
	}
	else
	{
		return (char*)"Modify level failed";
	}
	
	return NULL;
}


