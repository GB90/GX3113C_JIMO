#include "KPAPP.h"
#include "app_common_porting_stb_api.h"
#include "app_kp_cas_api_watchlevel.h"

char * app_kp_cas_api_get_watchlevel_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
	static char s_text_buffer[30];

	U8 watchlevel;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
	{
		case KP_RATING_LEVEL_ID:
			if(KP_RET_SUCCESS == KPCAAPP_GetRating(&watchlevel))
			{
    			sprintf(s_text_buffer, "%d", watchlevel-4);
    			return s_text_buffer;
			}
            return NULL;
		default:
			break;
	}
	return NULL;

}

char* app_kp_cas_api_change_watchlevel(ca_rating_t* data)
{
	U8 wRet = 0;
	uint8_t pin[6+1]={0};
	U8 watchlevel;

	if (NULL == data)
		return NULL;
	
	memcpy(pin,data->pin,KPCAS_PIN_LEN);
	if(strlen((char*)pin) != KPCAS_PIN_LEN)
	{
		 return (char*)"Input new PIN";	 
	}

	watchlevel = data->rate+4;
 	wRet = KPCAAPP_SetRating(pin,watchlevel);
	if(wRet == KP_RET_SUCCESS)
		return (char*)"Set Successfully";
	else if(wRet == KP_RET_PIN_ERROR)
		return (char*)"PIN Error";
	
	return NULL;

}


