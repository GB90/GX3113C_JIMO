/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_tr_cas_api_pin.c
* Author    : 	zhouhm
* Project   :	goxceed dvbc
* Type      :
******************************************************************************
* Purpose   :	模块头文件
******************************************************************************
* Release History:
VERSION	Date			  AUTHOR         Description
1.0  	2013.12.04		  zhouhm 	 			creation
*****************************************************************************/
#include "Tr_Cas.h"
#include "app_common_porting_stb_api.h"
#include "app_tr_cas_api_pin.h"


char* app_tr_cas_api_verify_pin(ca_pin_t* data)
{
	uint8_t Password1[TRCAS_PIN_MAXLEN+1] = {0};
	int hresult = 0;
	
	if (NULL == data)
	{
		return NULL;
	}

	memcpy(Password1, data->oldpin, TRCAS_PIN_MAXLEN);

	if (strlen((char *)Password1) != TRCAS_PIN_MAXLEN)
	{
		return (char *)"Input 6-bit PIN";
	}		

	Password1[0] = Password1[0]-0x30;
	Password1[1] = Password1[1]-0x30;
	Password1[2] = Password1[2]-0x30;
	Password1[3] = Password1[3]-0x30;
	Password1[4] = Password1[4]-0x30;
	Password1[5] = Password1[5]-0x30;
	hresult = MC_SCARD_ChangePin(Password1, NULL, TRCAS_PIN_MAXLEN);
	if (hresult)
	{
		return (char *)"Error PIN";
	}

	return NULL;
}

char* app_tr_cas_api_change_pin(ca_pin_t* data)
{
	uint8_t Password1[TRCAS_PIN_MAXLEN+1] = {0};
	uint8_t newPassword1[TRCAS_PIN_MAXLEN+1] = {0};
	uint8_t newPassword2[TRCAS_PIN_MAXLEN+1] = {0};
	int hresult = 0;
	
	if (NULL == data)
	{
		return NULL;
	}

	memcpy(Password1, data->oldpin, TRCAS_PIN_MAXLEN);
	memcpy(newPassword1, data->newpin, TRCAS_PIN_MAXLEN);
	memcpy(newPassword2, data->newconfirmpin, TRCAS_PIN_MAXLEN);

	if (strlen((char *)Password1) != TRCAS_PIN_MAXLEN)
	{
		return (char *)"Input 6-bit PIN";
	}	
	
	if (strcasecmp((char*)newPassword1, (char*)newPassword2) != 0)
	{
		return (char*)"The new PIN are not consistent";
	}
	
	if (strlen((char *)newPassword1) != TRCAS_PIN_MAXLEN)
	{
		return (char*)"Input 6-bit new PIN";
	}
	
	Password1[0] = Password1[0]-0x30;
	Password1[1] = Password1[1]-0x30;
	Password1[2] = Password1[2]-0x30;
	Password1[3] = Password1[3]-0x30;
	Password1[4] = Password1[4]-0x30;
	Password1[5] = Password1[5]-0x30;

	newPassword1[0] = newPassword1[0]-0x30;
	newPassword1[1] = newPassword1[1]-0x30;
	newPassword1[2] = newPassword1[2]-0x30;
	newPassword1[3] = newPassword1[3]-0x30;
	newPassword1[4] = newPassword1[4]-0x30;
	newPassword1[5] = newPassword1[5]-0x30;
	hresult = MC_SCARD_ChangePin(Password1, newPassword1, TRCAS_PIN_MAXLEN);
	switch (hresult)
	{
		case 0:
		{	
			return (char *)"Modify PIN successful";
		}
			break;
			
		default:
			return (char *)"Error PIN";
			break;
	}	

	return NULL;
}

