/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_rating.c
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
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_rating.h"

unsigned char  StartHour;
unsigned char  StartMin;
unsigned char  StartSec;
unsigned char  EndHour;
unsigned char  EndMin;
unsigned char  EndSec;

BYTE  pbyRating = 0;

uint8_t app_dvt_cas_api_init_rating_data(void)
{
	if (DVTCA_OK ==  DVTCASTB_GetRating(&pbyRating))
	{
		return 1;		
	}
	
	return 0;	
}

char * app_dvt_cas_api_get_rating_data(ca_get_date_t *data)
{
	uint32_t pos = 0 ;                             
	uint32_t ID = 0;
    static char            s_text_buffer[30];

	if (NULL == data)
		return NULL;

	pos = data->pos;
	ID = data->ID;

	memset(s_text_buffer,0,sizeof(s_text_buffer));
	
	switch(ID)
		{
			case DVT_RATING_LEVEL_ID:
				if ( pbyRating >= 3)
					pbyRating -= 3;
				sprintf(s_text_buffer, "%d", pbyRating);
				return s_text_buffer;
				break;
			default:
				break;
		}
	return NULL;

}

char* app_dvt_cas_api_change_rating(ca_rating_t* data)
{
	uint8_t Password1[DVTCA_MAXLEN_PINCODE+1]={0};
	SDVTCAPin pNewPin = {0};
	HRESULT hresult = 0;
	BYTE byRating = 0;

	if (NULL == data)
		return NULL;
	memcpy(Password1,data->pin,DVTCA_MAXLEN_PINCODE);

	if(strlen((char*)Password1)!=DVTCA_MAXLEN_PINCODE)
	{
		return (char*)"Input 8-bit PIN";
	}
	
	pNewPin.m_byLen = 8;
	pNewPin.m_byszPin[0] = Password1[0]-0x30;
	pNewPin.m_byszPin[1] = Password1[1]-0x30;
	pNewPin.m_byszPin[2] = Password1[2]-0x30;
	pNewPin.m_byszPin[3] = Password1[3]-0x30;
	pNewPin.m_byszPin[4] = Password1[4]-0x30;
	pNewPin.m_byszPin[5] = Password1[5]-0x30;
	pNewPin.m_byszPin[6] = Password1[6]-0x30;
	pNewPin.m_byszPin[7] = Password1[7]-0x30;
	hresult = DVTCASTB_VerifyPin(&pNewPin);
	switch(hresult)
	{
		case DVTCAERR_STB_DATA_LEN_ERROR:
			{
				return (char*)"Error PIN length";
			}
			break;
		case DVTCAERR_STB_PIN_LOCKED:
			{
				return (char*)"The PIN has been locked";
			}
			break;
		case DVTCAERR_STB_PIN_INVALID:
			{
				return (char*)"Error PIN";
			}
			break;
		default:
			break;						
	}
		
	byRating = data->rate +3;
	if (DVTCA_OK ==  DVTCASTB_SetRating(byRating))
	{
		return (char*)"Modify level successful";
	}
	else
	{
		return (char*)"Modify level failed";
	}
	
	return NULL;

}







