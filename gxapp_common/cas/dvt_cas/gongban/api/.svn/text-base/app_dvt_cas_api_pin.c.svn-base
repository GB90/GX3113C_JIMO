/*****************************************************************************
* 						   CONFIDENTIAL
*        Hangzhou GuoXin Science and Technology Co., Ltd.
*                      (C)2012, All right reserved
******************************************************************************

******************************************************************************
* File Name :	app_dvt_cas_api_pin.c
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
#include "DVTCAS_STBInf.h"
#include "app_common_porting_stb_api.h"
#include "app_dvt_cas_api_pin.h"

char* app_dvt_cas_api_verify_pin(ca_pin_t* data)
{
	SDVTCAPin pNewPin = {0};
	uint8_t Password1[DVTCA_MAXLEN_PINCODE+1]={0};
	HRESULT hresult = 0;
	
	if (NULL == data)
		return NULL;

	memcpy(Password1,data->oldpin, DVTCA_MAXLEN_PINCODE);

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
	return NULL;


}

char* app_dvt_cas_api_change_pin(ca_pin_t* data)
{
	SDVTCAPin pNewPin = {0};
	uint8_t Password1[DVTCA_MAXLEN_PINCODE+1]={0};
	uint8_t newPassword1[DVTCA_MAXLEN_PINCODE+1]={0};
	uint8_t newPassword2[DVTCA_MAXLEN_PINCODE+1]={0};
	HRESULT hresult = 0;
	
	if (NULL == data)
		return NULL;


	memcpy(Password1,data->oldpin, DVTCA_MAXLEN_PINCODE);
	memcpy(newPassword1, data->newpin, DVTCA_MAXLEN_PINCODE);
	memcpy(newPassword2, data->newconfirmpin, DVTCA_MAXLEN_PINCODE);

	if(strlen((char*)Password1)!=DVTCA_MAXLEN_PINCODE)
	{
		return (char*)"Input 8-bit PIN";
	}	
	
	if(strcasecmp((char*)newPassword1,(char*)newPassword2) != 0)
	{
		return (char*)"The new PIN are not consistent";
	}
	
	if(strlen((char*)newPassword1)!=DVTCA_MAXLEN_PINCODE)
	{
		return (char*)"Input 8-bit new PIN";
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
	
	pNewPin.m_byLen = 8;
	pNewPin.m_byszPin[0] = newPassword1[0]-0x30;
	pNewPin.m_byszPin[1] = newPassword1[1]-0x30;
	pNewPin.m_byszPin[2] = newPassword1[2]-0x30;
	pNewPin.m_byszPin[3] = newPassword1[3]-0x30;
	pNewPin.m_byszPin[4] = newPassword1[4]-0x30;
	pNewPin.m_byszPin[5] = newPassword1[5]-0x30;
	pNewPin.m_byszPin[6] = newPassword1[6]-0x30;
	pNewPin.m_byszPin[7] = newPassword1[7]-0x30;
	hresult = DVTCASTB_ChangePin(&pNewPin);
	switch(hresult)
	{
		case DVTCA_OK:
			{	
				return (char*)"Modify PIN successful";
			}
			break;
		case DVTCAERR_STB_DATA_LEN_ERROR:
			{
				return (char*)"Error new PIN length";
			}
			break;
		case DVTCAERR_STB_PIN_LOCKED:
			{
				return (char*)"The PIN has been locked";
			}
			break;
		default:
			break;
	}	


	return NULL;
}







