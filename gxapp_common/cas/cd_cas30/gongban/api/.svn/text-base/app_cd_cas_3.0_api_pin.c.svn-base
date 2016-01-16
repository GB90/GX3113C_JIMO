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
#include "CDCASS.h"
#include "app_common_porting_stb_api.h"
#include "app_cd_cas_3.0_api_pin.h"

char* app_cd_cas30_api_change_pin(ca_pin_t* data)
{
	uint8_t newPassword1[CDCA_MAXLEN_PINCODE+1]={0};
	uint8_t newPassword2[CDCA_MAXLEN_PINCODE+1]={0};
    uint8_t newPassword3[CDCA_MAXLEN_PINCODE+1]={0};
    unsigned short               wRet            = 0;
    unsigned char i = 0;

	if (NULL == data)
		return NULL;


	memcpy(newPassword1,data->oldpin, CDCA_MAXLEN_PINCODE);
	memcpy(newPassword2, data->newpin, CDCA_MAXLEN_PINCODE);
	memcpy(newPassword3, data->newconfirmpin, CDCA_MAXLEN_PINCODE);
	if(strlen((char*)newPassword1)!=CDCA_MAXLEN_PINCODE )
	{
		return (char*)"Invalid PIN";//"Input old PIN";
	}
	else if(strlen((char*)newPassword2)!=CDCA_MAXLEN_PINCODE)
	{
		return (char*)"Invalid PIN";//"Input new PIN";
	}
	else if(strlen((char*)newPassword3)!=CDCA_MAXLEN_PINCODE )
	{
		return (char*)"Invalid PIN";//"Input new PIN";
	}
	if(atoi( (char*)newPassword3 ) != atoi( (char*)newPassword2 ))
	{
		return (char*)"Invalid PIN";//"Verify new PIN failed";
	}
	for( i = 0; i < CDCA_MAXLEN_PINCODE; i++ )
	{
		newPassword1[i] -= 0x30;
		newPassword2[i] -= 0x30;
		newPassword3[i] -= 0x30;
	}

	wRet = CDCASTB_ChangePin(newPassword1,newPassword2);
	switch( wRet )
	{
		case CDCA_RC_OK:
			/*PIN码修改正确*/
			data->errorCode = 0;
			return (char*)"PIN has been changed";
		case CDCA_RC_CARD_INVALID:
			return (char*)"Invalid Smartcard";
		case CDCA_RC_PIN_INVALID:
			return (char*)"Invalid PIN";
		default:
			break;
	}

	return NULL;
}







