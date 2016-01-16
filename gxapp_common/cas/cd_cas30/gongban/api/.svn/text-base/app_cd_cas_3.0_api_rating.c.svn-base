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
* Purpose   :	Ä£¿éÍ·ÎÄ¼ş
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2012.12.02		  zhouhm 	 			creation
*****************************************************************************/
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_rating.h"

unsigned char  StartHour;
unsigned char  StartMin;
unsigned char  StartSec;
unsigned char  EndHour;
unsigned char  EndMin;
unsigned char  EndSec;


uint8_t app_cd_cas30_api_init_rating_data(void)
{
	return 0;
}

char * app_cd_cas30_api_get_rating_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    static char            s_text_buffer[30];

    unsigned char ratevalue;


	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
		{
			case CD_RATING_LEVEL_ID:
				CDCASTB_GetRating(&ratevalue);
				ratevalue -=  4;
				sprintf(s_text_buffer, "%d", ratevalue);
				return s_text_buffer;
				break;
			default:
				break;
		}
	return NULL;

}

char* app_cd_cas30_api_change_rating(ca_rating_t* data)
{
	CDCA_U16 wRet = 0;
	uint8_t pin[CDCA_MAXLEN_PINCODE+1]={0};


	if (NULL == data)
		return NULL;
	memcpy(pin,data->pin,CDCA_MAXLEN_PINCODE);

	 pin[0] = pin[0]-0x30;
	 pin[1] = pin[1]-0x30;
	 pin[2] = pin[2]-0x30;
	 pin[3] = pin[3]-0x30;
	 pin[4] = pin[4]-0x30;
	 pin[5] = pin[5]-0x30;
	 data->rate += 4;

     wRet =  CDCASTB_SetRating((CDCA_U8*)pin,data->rate);

	switch( wRet )
	{
		case CDCA_RC_OK:
			/*PINÂëĞŞ\u017eÄÕıÈ·*/
			return (char*)"Teleview rating changed successfully";
		case CDCA_RC_CARD_INVALID:
			return (char*)"Invalid Smartcard";
		case CDCA_RC_PIN_INVALID:
			return (char*)"Invalid PIN";
		default:
			return (char*)"Set Maturity Rating Fail";
	}
	
	return NULL;

}







